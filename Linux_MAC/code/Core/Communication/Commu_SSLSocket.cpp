
/*
 * Copyright (c) 2012-2014
 * All rights reserved.
 *
 * File Name: Commu_SSLSocket.cpp
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2015.10.26
 */
//------------------------------------------------------------------------------------------//
#include "stdafx.h"
//#define LOGPRINT_ENABLE
//#define LOGPRINT_ENABLE2
#include "Commu_SSLSocket.h"
#include "Comm_Convert.h"
#include "SYS_Time.h"
#include "Comm_File.h"
//------------------------------------------------------------------------------------------//
#if (defined USE_OPENSSL) || (defined CommonDefH_MAC)
//------------------------------------------------------------------------------------------//
void CSSL_FN_T0::Delivery(RTREE_NODE *retFNList,const uint8 *databuf,int32 num){
	uint32	tCH;
	cgPSBUF->Put(databuf,num);
	ELogPrint(this,"Delivery()::cgPSBUF:%d/%d",cgPSBUF->Used(),cgPSBUF->Unused());
	Lock();
	retFNList->Spin_InUse_set();
	while (AnalysisFrameTryAgain(*GetcgDefFifo()) > 0){
		tCH = fn_CH.GetValueCalc();
		RTREE_LChildRChain_Traversal_LINE_nolock(CSSL_FR_T1,retFNList,
			if (operateNode_t->GetcgCH() == tCH){
				ELogPrint(operateNode_t,"Delivery()::T0:%d",GetLength());
				operateNode_t->ReceiveT0Data(&fn_AESCC,cgKey);
				break;
			}
		);
		Out();
	}
	retFNList->Spin_InUse_clr();
	Unlock();
};
//------------------------------------------------------------------------------------------//
void CSSL_FN_T0::SetPackageDlyMS(double dlyMS){
	static	int32 count = 0;
	Lock();
	cgPackageDlyMS = 50 + (cgPackageDlyMS + dlyMS) / 2;
	if (cgPackageDlyMS > MAX_PACKAGE_DEALYTM){
		++ count;
		cgPackageDlyMS = MAX_PACKAGE_DEALYTM;
	}
	else{
		count = 0;
	}
	if (count > 1){
		cgPackageDlyMS = 200;
		count = 0;
	}
	Unlock();
};
//------------------------------------------------------------------------------------------//
double CSSL_FN_T0::GetPackageDlyMS(void){
	double	dt;
	Lock();
	dt = cgPackageDlyMS;
	Unlock();
	return(dt);
};
//------------------------------------------------------------------------------------------//
void CSSL_FR_T1::ReceiveT0Data(CCY_FN_AES *fnAESofT0,const std::string &strKey){
	fnAESofT0->ReadContent(&cgRxSBUF,strKey);
	ELogPrint(this,"ReceiveT0Data()::cgRxSBUF:%d/%d",cgRxSBUF.Used(),cgRxSBUF.Unused());
};
//------------------------------------------------------------------------------------------//
void CSSL_FR_T1::Init(uint32 rxfifoSize,uint32 tCH,G_Endian_VAILD tEV){
	
	cgTxFIFO.Init(MAX_PACKAGE_SIZE << 2);
	cgRxSBUF.Init(rxfifoSize);
	
	Field_Node::Init(FNT_MESSAGE,&cgTxFIFO,tEV);
	fn_GroupID.Init	(FNT_CTRL,&cgTxFIFO,2,tEV);
	fn_CtrlID.Init	(FNT_CTRL,&cgTxFIFO,1,tEV);
	fn_Offset.Init	(FNT_CTRL,&cgTxFIFO,2,tEV);
	fn_Info.Init	(&cgTxFIFO,2,tEV);
	
	cgCH = tCH;
	cgGroupID = 0;
	cgLastOffset = 0;
	cgPackageDlyMS = CSSL_FN_T0::MAX_PACKAGE_DEALYTM;
};
//------------------------------------------------------------------------------------------//
void CSSL_FR_T1::SetConfig(G_Endian_VAILD tEV){
	SetEndianType(tEV);
	fn_GroupID.SetEndianType(tEV);
	fn_CtrlID.SetEndianType(tEV);
	fn_Offset.SetEndianType(tEV);
	fn_Info.SetEndianType(tEV);
};
//------------------------------------------------------------------------------------------//
enum{
	REC_Nothing = 0,
	REC_RecPackage,
	REC_GetPackage,
};
//------------------------------------------------------------------------------------------//
enum{
	CSSL_Forward = 0,
	CSSL_Package,
	CSSL_ENDPackage,
	CSSL_ACK,
	CSSL_ENDACK,
	CSSL_ResendPackage,
	CSSL_ResendAll,
};
//------------------------------------------------------------------------------------------//
int32 CSSL_FR_T1::RX_Data(CSSL_FN_T0 *sslT0Tx,uint32 *tGroupID,uint32 *lastOffset,PUB_SBUF *retSBUFInfo
						  ,uint32 *retGroupID,uint32 *retCtrlID,uint32 *retOffset){
	uint32		offset,nextOffset,requestTimes;
	uint32		recResult,rec_CtrlID,rec_GroupID,rec_Offset,rec_InfoL,rec_PGoffset,rec_PGlength;
	SYS_TIME_S	dlyTS;
	double		delayMS;
	
	requestTimes = 5;
	nextOffset = *lastOffset;
	if (sslT0Tx != nullptr)
		cgPackageDlyMS = sslT0Tx->GetPackageDlyMS();
	delayMS = cgPackageDlyMS;//4s as first

	SYS_Delay_SetTS(&dlyTS,delayMS);
	do{
		offset = 0;
		recResult = REC_Nothing;
		if (cgRxSBUF.Used() > 0){
			ELogPrint(this, "RX_Data()::Rec dly:%dms,D1:%s",(uint32)delayMS,dlyTS.DTime1.FormatDateTime("[hh:mm:ss.zzz]").c_str());
			cgRxSBUF.Spin_InUse_set();
			while(AnalysisFrame(cgRxSBUF.cgBufFIFO,offset) > 0){
				recResult = REC_RecPackage;
				rec_PGoffset = GetOffset();
				rec_PGlength = GetLength();
				offset = rec_PGoffset + rec_PGlength;
				rec_GroupID = fn_GroupID.GetValueCalc(&cgRxSBUF.cgBufFIFO);
				rec_CtrlID = fn_CtrlID.GetValueCalc(&cgRxSBUF.cgBufFIFO);
				rec_Offset = fn_Offset.GetValueCalc(&cgRxSBUF.cgBufFIFO);
				rec_InfoL = fn_Info.GetContentLength();
				ELogPrint(this, "RX_Data()::     GroupID:%d, reqTimes:%d,nextOffset:%d",*tGroupID,requestTimes,nextOffset);
				ELogPrint(this, "RX_Data()::ANAL GroupID:%d,RecCtrlID:%d, RecOffset:%d,RecInfoL:%d,PGoffset:%d,PGlength:%d"
						  ,rec_GroupID,rec_CtrlID,rec_Offset,rec_InfoL,rec_PGoffset,rec_PGlength);
				if (*tGroupID > rec_GroupID){
					if (*tGroupID - rec_GroupID < 0x7fff){
						if (rec_PGoffset == 0)
							cgRxSBUF.Out(rec_PGlength,G_LOCK_OFF);
						break;
					}
					*tGroupID = rec_GroupID;
					*lastOffset = 0;
				}
				switch (rec_CtrlID){
					case CSSL_ENDPackage:
						if ((*tGroupID == rec_GroupID) && (nextOffset == rec_Offset)){
							fn_Info.ReadContent(retSBUFInfo,&cgRxSBUF.cgBufFIFO);
							if (rec_PGoffset == 0){
								cgRxSBUF.Out(rec_PGlength,G_LOCK_OFF);
								offset = 0;
							}
							ANS_ENDACK(sslT0Tx, *tGroupID);
							++ *tGroupID;
							recResult = REC_GetPackage;
						}
						break;
					case CSSL_Package:{
						if (*tGroupID == rec_GroupID){
							if (nextOffset == rec_Offset){
								if (retSBUFInfo != nullptr)
									fn_Info.ReadContent(retSBUFInfo,&cgRxSBUF.cgBufFIFO);
								if (rec_PGoffset == 0){
									cgRxSBUF.Out(rec_PGlength,G_LOCK_OFF);
									offset = 0;
								}
								
								ANS_ACK(sslT0Tx, *tGroupID, rec_Offset);
								nextOffset += rec_InfoL;
								requestTimes = 5;
								SYS_Delay_SetTS(&dlyTS,delayMS);
								break;
							}
							else if (nextOffset > rec_Offset){
								if (rec_PGoffset == 0){
									cgRxSBUF.Out(rec_PGlength,G_LOCK_OFF);
									offset = 0;
								}
							}
							else if (SYS_Delay_CheckTS(&dlyTS) > 0){
								if (requestTimes-- == 0){
									++ *tGroupID;
									requestTimes = 5;
									nextOffset = *lastOffset;
								}
								else{
									ANS_Request_ResendPackage(sslT0Tx, *tGroupID, nextOffset);
								}
								SYS_Delay_SetTS(&dlyTS,delayMS);
							}
						}
						else if (SYS_Delay_CheckTS(&dlyTS) > 0){
							if (requestTimes-- == 0){
								*tGroupID = rec_GroupID;
								requestTimes = 5;
								nextOffset = 0;
								*lastOffset = 0;
							}
							else{
								ANS_Request_ResendAll(sslT0Tx, *tGroupID);
							}
							SYS_Delay_SetTS(&dlyTS,delayMS);
						}
						break;
					}
					case CSSL_ACK:
					case CSSL_ENDACK:
					case CSSL_ResendPackage:
					case CSSL_ResendAll:
					default:{
						*retGroupID = rec_GroupID;
						*retCtrlID = rec_CtrlID;
						*retOffset = rec_Offset;
						if (rec_PGoffset == 0){
							cgRxSBUF.Out(rec_PGlength,G_LOCK_OFF);
							offset = 0;
						}
						recResult = REC_GetPackage;
						break;
					}
				}
				if (recResult == REC_GetPackage)
					break;
			}
			cgRxSBUF.Spin_InUse_clr();
			if (recResult == REC_GetPackage){
				*lastOffset = 0;
				ELogPrint(this, "RX_Data()::Get full Package,cgRxSBUF:%d/%d",cgRxSBUF.Used(),cgRxSBUF.Unused());
				return 1;
			}
			SYS_SleepMS(2);
		}
	}while(recResult > 0);
	*lastOffset = nextOffset;
	return 0;
}
//------------------------------------------------------------------------------------------//
int32 CSSL_FR_T1::RX_Packaging(CSSL_FN_T0 *sslT0Tx,PUB_SBUF *retSBUFInfo){
	return(RX_Data(sslT0Tx,&cgGroupID,&cgLastOffset,retSBUFInfo,nullptr,nullptr,nullptr));
}
//------------------------------------------------------------------------------------------//
void CSSL_FR_T1::ANS_ACK(CSSL_FN_T0 *sslT0Tx,const uint32 &tGroupID,const uint32 &offset){
	if (sslT0Tx == nullptr)
		return;
	Spin_InUse_set();
	GetcgDefFifo()->Empty();
	HoldOffset();
	fn_GroupID.SetFIFOByte(tGroupID);
	fn_CtrlID.SetFIFOByte(CSSL_ACK);
	fn_Offset.SetFIFOByte(offset);
	fn_Info.SetContent("");
	UpdateLength();
	sslT0Tx->SetContent(*GetcgDefFifo(),GetLength(),GetOffset(),cgCH + 1);
	ELogPrint(this, "RX_Data()::Send GroupID:%d,CtrlID:CSSL_ACK,Offset:%d,T1:%d,T0:%d",tGroupID,offset,GetLength(),sslT0Tx->GetLength());
	Spin_InUse_clr();
}
//------------------------------------------------------------------------------------------//
void CSSL_FR_T1::ANS_ENDACK(CSSL_FN_T0 *sslT0Tx,const uint32 &tGroupID){
	if (sslT0Tx == nullptr)
		return;
	Spin_InUse_set();
	GetcgDefFifo()->Empty();
	HoldOffset();
	fn_GroupID.SetFIFOByte(tGroupID);
	fn_CtrlID.SetFIFOByte(CSSL_ENDACK);
	fn_Offset.SetFIFOByte(0);
	fn_Info.SetContent("");
	UpdateLength();
	sslT0Tx->SetContent(*GetcgDefFifo(),GetLength(),GetOffset(),cgCH + 1);
	ELogPrint(this, "RX_Data()::Send GroupID:%d,CtrlID:CSSL_ENDACK,Offset:0,T1:%d,T0:%d",tGroupID,GetLength(),sslT0Tx->GetLength());
	Spin_InUse_clr();
}
//------------------------------------------------------------------------------------------//
void CSSL_FR_T1::ANS_Request_ResendPackage(CSSL_FN_T0 *sslT0Tx,const uint32 &tGroupID,const uint32 &offset){
	if (sslT0Tx == nullptr)
		return;
	Spin_InUse_set();
	GetcgDefFifo()->Empty();
	HoldOffset();
	fn_GroupID.SetFIFOByte(tGroupID);
	fn_CtrlID.SetFIFOByte(CSSL_ResendPackage);
	fn_Offset.SetFIFOByte(offset);
	fn_Info.SetContent("");
	UpdateLength();
	sslT0Tx->SetContent(*GetcgDefFifo(),GetLength(),GetOffset(),cgCH + 1);
	ELogPrint(this, "RX_Data()::Send GroupID:%d,CtrlID:CSSL_ResendPackage,Offset:%d,T1:%d,T0:%d",tGroupID,offset,offset,GetLength(),sslT0Tx->GetLength());
	Spin_InUse_clr();
}
//------------------------------------------------------------------------------------------//
void CSSL_FR_T1::ANS_Request_ResendAll(CSSL_FN_T0 *sslT0Tx,const uint32 &tGroupID){
	if (sslT0Tx == nullptr)
		return;
	Spin_InUse_set();
	GetcgDefFifo()->Empty();
	HoldOffset();
	fn_GroupID.SetFIFOByte(tGroupID);
	fn_CtrlID.SetFIFOByte(CSSL_ResendAll);
	fn_Offset.SetFIFOByte(0);
	fn_Info.SetContent("");
	UpdateLength();
	sslT0Tx->SetContent(*GetcgDefFifo(),GetLength(),GetOffset(),cgCH + 1);
	ELogPrint(this, "RX_Data()::Send GroupID:%d,CtrlID:CSSL_ResendAll,Offset:0,T1:%d,T0:%d",tGroupID,GetLength(),sslT0Tx->GetLength());
	Spin_InUse_clr();
}
//------------------------------------------------------------------------------------------//
int32 CSSL_FR_T1::CHK_Data(uint32 *retGroupID,uint32 *retCtrlID,uint32 *retOffset){
	uint32 tGroupID,lastOffset;
	tGroupID = cgGroupID;
	lastOffset = 0;
	if (RX_Data(nullptr,&tGroupID,&lastOffset,nullptr,retGroupID,retCtrlID,retOffset) > 0)
		return(cgGroupID == tGroupID);
	return 0;
}
//------------------------------------------------------------------------------------------//
uint32 CSSL_FR_T1::TX_Data(CSSL_FN_T0 *sslT0Tx,const uint8 *&data,const uint32 &num,const uint32 &offset,const uint32 &packageSize,const uint32 &tGroup){
	uint32 length;
	length = (packageSize < (num - offset)) ? packageSize : (num - offset);
	GetcgDefFifo()->Empty();
	HoldOffset();
	fn_GroupID.SetFIFOByte(tGroup);
	fn_CtrlID.SetFIFOByte(((length + offset) < num) ? CSSL_Package : CSSL_ENDPackage);
	fn_Offset.SetFIFOByte(offset);
	fn_Info.SetContent(data + offset,length);
	UpdateLength();
	
	sslT0Tx->SetContent(*GetcgDefFifo(),GetLength(),GetOffset(),cgCH - 1);
	ELogPrint(this, "TX_Packaging()::TX_Data()::Send GroupID:%d,CtrlID:%s,Offset:%d,DataNum:%d,T1:%d,T0:%d"
				,tGroup,((length + offset) < num) ? "CSSL_Package" : "CSSL_ENDPackage",offset,length,GetLength(),sslT0Tx->GetLength());
	return(length);
}
//------------------------------------------------------------------------------------------//
int32 CSSL_FR_T1::TX_Packaging(CSSL_FN_T0 *sslT0Tx,const uint8 *data,uint32 num){
	uint32		packageSize,maxCount,count;
	uint32		offset,ackOffset,resendTimes,retGroupID,retCtrlID,retOffset;
	SYS_TIME_S	dlyTS,reDlyTS,totalTS;
	//SYS_DateTime	dT1,dT2;
	double		delayMS,avgMS;
	
	if (num == 0)
		return 1;
	
	Spin_InUse_set();
	packageSize = (((MAX_PACKAGE_SIZE - sslT0Tx->GetInvalidLength()) >> 4) << 4) - 1 - GetInvalidLength();
	maxCount = (COMMU_DBUF_B::PACKAGE_MAX_SIZE) / (MAX_PACKAGE_SIZE);
	resendTimes = 0;
	ELogPrint(this, "TX_Packaging()::data num:%d,InvalidLength:%d,PGsize:%d,maxCount:%d"
			  ,num,sslT0Tx->GetInvalidLength() + GetInvalidLength(),packageSize,maxCount);

	cgPackageDlyMS = sslT0Tx->GetPackageDlyMS();
	ackOffset = 0;
	count = 0;
	do{
		delayMS = sslT0Tx->GetPackageDlyMS();
		offset = ackOffset;
		SYS_Delay_SetTS(&dlyTS,0);
		SYS_Delay_SetTS(&totalTS,delayMS);
		do{
			if ((offset < num) && (count < maxCount)){
				++ count;
				SYS_Delay_AddTS(&dlyTS,delayMS);
				ELogPrint(this, "TX_Packaging()::T%d,dly:%dms,D1:%s"
						  ,resendTimes + 1,(uint32)delayMS,dlyTS.DTime1.FormatDateTime("[hh:mm:ss.zzz]").c_str());
				offset += TX_Data(sslT0Tx,data,num,offset,packageSize,cgGroupID);
			}
			while(CHK_Data(&retGroupID,&retCtrlID,&retOffset) > 0){
				ELogPrint(this, "TX_Packaging()::Rec RecGroupID:%d,RecCtrlID:%d,RecOffset:%d,offset:%d",retGroupID,retCtrlID,retOffset,offset);
				if (retCtrlID == CSSL_ResendPackage){
					SYS_Delay_SetTS(&reDlyTS,delayMS);
					while((TX_Data(sslT0Tx,data,num,retOffset,packageSize,cgGroupID) == 0) && (SYS_Delay_CheckTS(&reDlyTS) == 0));
					SYS_Delay_AddTS(&dlyTS,delayMS);
				}
				else if (retCtrlID == CSSL_ResendAll){
					SYS_Delay_SetTS(&dlyTS,delayMS);
					ackOffset = 0;
					count = 0;
					break;
				}
				else if (retCtrlID == CSSL_ACK){
					if (ackOffset <= retOffset){
						if (count != 0)
							-- count;
						ackOffset = retOffset + packageSize;
					}
				}
				else if (retCtrlID == CSSL_ENDACK){
					ELogPrint(this, "TX_Packaging()::T%d,GroupID:%d,End:OK",resendTimes + 1,cgGroupID);
					SYS_Delay_CheckTS(&totalTS);
					totalTS.DTime2 += (delayMS / 1000);
					totalTS.DTime2 -= totalTS.DTime1;
					avgMS = totalTS.DTime2.GetSec() * 1000 / (1 + num / MAX_PACKAGE_SIZE);
					sslT0Tx->SetPackageDlyMS(avgMS);
					ELogPrint(this, "TX_Packaging()::avgMS:%dms,new dlyMS:%dms",(uint32)avgMS,(uint32)sslT0Tx->GetPackageDlyMS());
					goto TX_Packaging_END;
				}
			}
			if (count == maxCount)
				SYS_SleepMS(10);
		}while(SYS_Delay_CheckTS(&dlyTS) == 0);
		ELogPrint(this, "TX_Packaging()::T%d,GroupID:%d,End:%s",resendTimes + 1,cgGroupID,(retCtrlID == CSSL_ENDACK)?"OK":"Fail");
		delayMS = sslT0Tx->GetPackageDlyMS() * 3;
		sslT0Tx->SetPackageDlyMS(delayMS);
	}while(++resendTimes < 5);
TX_Packaging_END:;
	++ cgGroupID;
	Spin_InUse_clr();
	cgPackageDlyMS = sslT0Tx->GetPackageDlyMS();
	return(retCtrlID == CSSL_ENDACK);
}
//------------------------------------------------------------------------------------------//
int32 CSSL_FR_T1::TX_Packaging(CSSL_FN_T0 *sslT0Tx,const FIFO_UINT8 &fifo,uint32 num,uint32 offset){
	uint32	slength;
	int32	ret;

	ret = 0;
	slength = fifo.CalcOutLength(num, offset);
	if (num > 0)
		ret = TX_Packaging(sslT0Tx,fifo.GetOutPointer(offset),num);
	if (slength > 0)
		ret &= TX_Packaging(sslT0Tx,fifo.GetOutPointer(0),slength);
	return(ret);
}
//------------------------------------------------------------------------------------------//
int32 CSSL_FR_T1::TX_Packaging(CSSL_FN_T0 *sslT0Tx,const Field_Node &fnNode){
	uint32	slength,num,offset;
	int32	ret;
	
	ret = 0;
	num = fnNode.GetLength();
	offset = fnNode.GetOffset();
	slength = fnNode.GetcgDefFifo()->CalcOutLength(num, offset);
	if (num > 0)
		ret = TX_Packaging(sslT0Tx,fnNode.GetcgDefFifo()->GetOutPointer(offset),num);
	if (slength > 0)
		ret &= TX_Packaging(sslT0Tx,fnNode.GetcgDefFifo()->GetOutPointer(0),slength);
	return(ret);
}
//------------------------------------------------------------------------------------------//
#endif
#ifdef USE_OPENSSL
//------------------------------------------------------------------------------------------//
class SSLSocket_ODEV : public ODEV_NODE{
	public:
		enum{RFLAG_C = 0, RFLAG_S = ODEV_NODE::RFLAG_S + ODEV_NODE::RFLAG_C};
	public:
				 SSLSocket_ODEV(void) : ODEV_NODE(){cgSSLSocket = nullptr;};
				 SSLSocket_ODEV(const SSLSocket *tODEV,COLType tCOLType = COLType_TXT) : ODEV_NODE(){Init(tODEV,tCOLType);};
		virtual ~SSLSocket_ODEV(void){;};
	public:
		virtual int32	Print		(G_LOCK_VAILD blLock = G_LOCK_ON){
			std::string		strT;
			if (cgSSLSocket != nullptr)
				cgSSLSocket->SSLWrite(ReadStr(&strT,blLock));
			return 1;
		};
		virtual void	UnInit		(G_LOCK_VAILD blLock = G_LOCK_ON){Spin_InUse_set(blLock);cgSSLSocket = nullptr;Spin_InUse_clr(blLock);};
	private:
		SSLSocket	*cgSSLSocket;
	public:
		void			Init(const SSLSocket *tODEV,COLType tCOLType = COLType_TXT){
			Spin_InUse_set();
			ChageType(tCOLType,G_LOCK_OFF);
			cgSSLSocket = (SSLSocket*)tODEV;
			Spin_InUse_clr();
		};
};
//------------------------------------------------------------------------------------------//




//------------------------------------------------------------------------------------------//
#define SetSSLFLAG(u64)		B_SetFLAG32(cgSSLFlag,(u64))
#define ClrSSLFLAG(u64)		B_ClrFLAG32(cgSSLFlag,(u64))
#define ChkSSLFLAG(u64)		B_ChkFLAG32(cgSSLFlag,(u64))
enum{
	blHandshakeS = BD_FLAG64(0),
	blHandshakeY = BD_FLAG64(1),
	blHandshakeN = BD_FLAG64(2),
	blUpdateKeyS = BD_FLAG64(3),
	blUpdateKeyY = BD_FLAG64(4),
	blUpdateKeyN = BD_FLAG64(5),
	blClose		= BD_FLAG64(6),
};
//------------------------------------------------------------------------------------------//
void SSLSocket::Init(CCT_AES_KEYL type,CCT_AES_MODE mode,CCT_Digest digestType,G_Endian_VAILD tEV){
	dataCHThread.ThreadInit(this, &SSLSocket::DataCHThreadFun);
	cgThreadList.AddNode(&dataCHThread);
	
	cgBufferT0_Rx.Init(1024 * 8);
	fn_sslT0_Tx.Init(&cgTxBuffer,type,mode,digestType,tEV);
	fn_sslT0_Rx.Init(&cgBufferT0_Rx,type,mode,digestType,tEV);
	CSSL_T1D_CH0.Init(&fwFNList,&fn_sslT0_Tx,cgRxBuffer.BufferMaxSize() >> 2,0,tEV);
	CSSL_T1D_CH1.Init(&fwFNList,&fn_sslT0_Tx,cgRxBuffer.BufferMaxSize() >> 2,1,tEV);
	Reset(0);
	SetConfig(type,mode,digestType,tEV);
	cgFIFOfnMesgTx.Init(1024 * 16);
	cgfnMesgTx.Init(&cgFIFOfnMesgTx);
	cgfnSignature.Init(1024 * 8,type,mode,digestType,tEV);
	
	SetSelfName(selfName);
	cgRSA_Prk = RSA_new();
	cgRSA_Puk = RSA_new();
	SetRSA_Prk();
	cgSSLFlag = 0;
};
//------------------------------------------------------------------------------------------//
void SSLSocket::Reset(int32 blResetPuk){
	SetConfig(CCT_AES128,CCT_AES_CBC,CCT_MD5,G_LITTLE_ENDIAN);
	ClrSSLFLAG(blHandshakeS | blHandshakeY | blHandshakeN | blUpdateKeyS | blUpdateKeyY | blUpdateKeyN | blClose);
	ClrblHold();
	SetKey(CCY_AESKey32Bye("LF.W.TZ"));
	fn_sslT0_Rx.SetPackageDlyMS(CSSL_FN_T0::MAX_PACKAGE_DEALYTM * 3);
	fn_sslT0_Tx.SetPackageDlyMS(CSSL_FN_T0::MAX_PACKAGE_DEALYTM * 3);
	fn_sslT0_Rx.ResetKey();
	fn_sslT0_Tx.ResetKey();
	CSSL_T1D_CH0.Reset();
	CSSL_T1D_CH1.Reset();
	CSSL_T1D_CH0.Clean();
	CSSL_T1D_CH1.Clean();
	
	if (blResetPuk > 0){
		RSA_free(cgRSA_Puk);
		cgRSA_Puk = RSA_new();
	}
};
//------------------------------------------------------------------------------------------//
void SSLSocket::OnCloseDev(void){
	E2LogPrint(this, "SSLSocket::OnCloseDev()");
	if (IsConnected() > 0){
		E2LogPrint(this, "SSLSocket::OnCloseDev()::Call SendREQ_CloseSocket()");
		SendREQ_CloseSocket();
	}
	APISocket::OnCloseDev();
};
//------------------------------------------------------------------------------------------//
void SSLSocket::CloseDev(void){
	E2LogPrint(this, "SSLSocket::CloseDev()");
	APISocket::CloseDev();
	Reset();
};
//------------------------------------------------------------------------------------------//
int32 SSLSocket::SendREQ_CloseSocket(void){
	SYS_TIME_S		Timedly;
	int32			blRet;
	blRet = 1;
	if (IsConnected() > 0){
		ClrSSLFLAG(blClose);
		E2LogPrint(this, "Send MESG_REQ_CloseSocket");
		blRet = CtrlCHWrite("",MESG_REQ_CloseSocket);
		E2LogPrint(this, "Send MESG_REQ_CloseSocket %s",(blRet > 0) ? "successful" : "fail");
		if (blRet > 0){
			SYS_Delay_SetTS(&Timedly, 1000);
			while((ChkSSLFLAG(blClose) == 0) && (IsConnected() > 0) && (SYS_Delay_CheckTS(&Timedly) == 0))
				SYS_SleepMS(100);
		}
		return(ChkSSLFLAG(blClose));
	}
	return(blRet);
};
//------------------------------------------------------------------------------------------//
void SSLSocket::SetConfig(CCT_AES_KEYL type,CCT_AES_MODE mode,CCT_Digest digestType,G_Endian_VAILD tEV){
	fn_sslT0_Tx.SetConfig(type,mode,digestType,tEV);
	fn_sslT0_Rx.SetConfig(type,mode,digestType,tEV);
	CSSL_T1D_CH0.SetConfig(tEV);
	cgAES_type = type;
	cgAES_mode = mode;
	cgDigestType = digestType;
	cgEndianType = tEV;
};
//------------------------------------------------------------------------------------------//
void SSLSocket::ThreadsStart(void){
	txThread.ThreadRun();
	if (GetCSType() != CSType_UDPS)
		rxThread.ThreadRun();
	exThread.ThreadRun();
	dataCHThread.ThreadRun();
}
//------------------------------------------------------------------------------------------//
void SSLSocket::DoAfterReadFromDevice(const uint8 *databuf,int32 num){
	E2LogPrint(this, "SSLSocket::DoAfterReadFromDevice()::Rec data num:%d",num);
	//E2LogPrint(this, "SSLSocket::DoAfterReadFromDevice()::rec data:%s",Str_CharToHEXStr(databuf, num, G_SPACE_ON).c_str());
	fn_sslT0_Rx.Delivery(&fwFNList,databuf,num);
};
//------------------------------------------------------------------------------------------//
int32 SSLSocket::DoAfterOpen(void){
	Handshake();
	if (GetCSType() == CSType_UDPS)
		return 1;
	return(CheckHandshake());
}
//------------------------------------------------------------------------------------------//
int32 SSLSocket::ExThreadFun(void){
	FNode_MESG		fnMesg;
	int32			blReady;
	E2LogPrint(this, "ExThreadFun()::Running");
	
	fnMesg.Init(&cgRxBuffer.cgBufFIFO);
	SYS_SleepMS(2);//must need, cannot delete , 2016 Jan.1
	
	cgRxBuffer.Clean();
	while((exThread.IsTerminated() == 0) && (IsConnected() > 0)){
		blReady = 0;
		if (CtrlCHRead(&cgRxBuffer) > 0){
			E2LogPrint(CSSL_T1D_CH0,"ExThreadFun()::cgRxBuffer:%d/%d",cgRxBuffer.Used(),cgRxBuffer.Unused());
			blReady = fnMesg.AnalysisFrame(cgRxBuffer.cgBufFIFO);
		}
		if (MessageProcessing_Handshake(&fnMesg,blReady) == 0)
			MessageProcessing(&fnMesg,blReady);
		if (blReady > 0){
			fnMesg.Out();
			continue;
		}
		SYS_SleepMS(2);
	}
	return 1;
}
//------------------------------------------------------------------------------------------//
int32 SSLSocket::DataCHThreadFun(void){
	uint32			num,offset,slength;
	
	cgDataRXSBUF.Init(cgRxBuffer.BufferMaxSize());
	cgDataCHSBUF.Init(cgRxBuffer.BufferMaxSize());

	E2LogPrint(this, "DataCHThreadFun()::Running");
	SYS_SleepMS(2);

	while((dataCHThread.IsTerminated() == 0) && (IsConnected() > 0)){
		cgDataRXSBUF.Clean();
		CSSL_T1D_CH1.RX_Packaging(&cgDataRXSBUF);
		if (cgDataRXSBUF.Used() > 0){
			num = cgDataRXSBUF.Used();
			if (CheckEnableEcho() != 0){
				SSLWrite(cgDataRXSBUF.cgBufFIFO, -1, 0);
				cgFwBytes += num;//Echo
			}
			Spin_InUse_set();
			if (GetcgCoupleNode(this) != nullptr){
				offset = 0;
				slength = cgDataRXSBUF.cgBufFIFO.CalcOutLength(num, offset);
				if (num > 0)
					ForwardToCouple(cgDataRXSBUF.cgBufFIFO.GetOutPointer(offset),num);
				if (slength > 0)
					ForwardToCouple(cgDataRXSBUF.cgBufFIFO.GetOutPointer(0),slength);
			}
			if (GetRxFwSBufList() != nullptr)
				GetRxFwSBufList()->LPut(cgDataRXSBUF.cgBufFIFO, -1, 0);//used for internal AP
			Spin_InUse_clr();
			cgDataCHSBUF.Spin_InUse_set();
			cgDataRXSBUF.Get(&cgDataCHSBUF.cgBufFIFO, -1);
			cgDataCHSBUF.Spin_InUse_clr();
			continue;
		}
		SYS_SleepMS(2);
	}
	return 1;
}
//------------------------------------------------------------------------------------------//
int32 SSLSocket::MessageProcessing_Handshake(FNode_MESG *RecMesg,int32 blReady){
	std::string			strMesg,strContent;
	CCT_AES_KEYL		newAES_type;
	CCT_AES_MODE		newAES_mode;
	CCT_Digest 			newDigestType;
	G_Endian_VAILD		newEndianType;
	int32				blDo,blRet;
	uint32				mID;

	static	CCT_AES_KEYL	oldAES_type = cgAES_type;
	static	CCT_AES_MODE	oldAES_mode = cgAES_mode;
	static	CCT_Digest 		oldDigestType = cgDigestType;
	static	G_Endian_VAILD	oldEndianType = cgEndianType;
	static	std::string		oldStrKey = cgKey;
	static SYS_TIME_S		Time_UpdateAESKey,Time_Handshake;
	
	if (((ChkSSLFLAG(blUpdateKeyN) > 0) && (ChkSSLFLAG(blUpdateKeyS) == 0))
		|| ((ChkSSLFLAG(blUpdateKeyS) > 0) && (SYS_Delay_CheckTS(&Time_UpdateAESKey) > 0))){
		E2LogPrint(this, "MessageProcessing_Handshake()::UpdateAESKey fail");
		SetSSLFLAG(blUpdateKeyN);
		ClrSSLFLAG(blUpdateKeyS | blUpdateKeyY);
		SetConfig(oldAES_type,oldAES_mode,oldDigestType,oldEndianType);
		SetKey(oldStrKey);
		E2LogPrint(this, "MessageProcessing_Handshake()::Using the old Key");
	}
	if (((ChkSSLFLAG(blHandshakeN) > 0) && (ChkSSLFLAG(blUpdateKeyS) == 0))
		 || ((ChkSSLFLAG(blHandshakeS) > 0) && (SYS_Delay_CheckTS(&Time_Handshake) > 0))){
		E2LogPrint(this, "MessageProcessing_Handshake()::Handshake fail, disconnect socket");
		ClrSSLFLAG(blUpdateKeyS | blUpdateKeyY | blUpdateKeyN);
		SetSSLFLAG(blHandshakeN);
		ClrSSLFLAG(blHandshakeS | blHandshakeY);
		ClrblConnected();
	}
	if (blReady == 0)
		return 0;
	blDo = 1;
	blRet = 1;
	RecMesg->ReadContent(&strMesg,&mID);
	switch (mID){
		case MESG_INI_HandshakeInit:
			ClrSSLFLAG(blHandshakeS | blHandshakeY | blHandshakeN | blUpdateKeyS | blUpdateKeyY | blUpdateKeyN);
			SetSSLFLAG(blHandshakeS);
			SYS_Delay_SetTS(&Time_Handshake,HandshakeTime);
			break;
		case MESG_REQ_StartHandshake:
			E2LogPrint(this, "MessageProcessing_Handshake()::Rec  MESG_REQ_StartHandshake");
			ClrSSLFLAG(blHandshakeS | blHandshakeY | blHandshakeN | blUpdateKeyS | blUpdateKeyY | blUpdateKeyN);
			SetSSLFLAG(blHandshakeS);
			E2LogPrint(this, "MessageProcessing_Handshake()::Send MESG_REQ_RSAPuk");
			SYS_Delay_SetTS(&Time_Handshake,HandshakeTime);
			blRet = CtrlCHWrite(CCY_Encode_RSAPublicKey(&strMesg,cgRSA_Prk),MESG_REQ_RSAPuk);
			E2LogPrint(this, "MessageProcessing_Handshake()::Send MESG_REQ_RSAPuk %s",(blRet > 0) ? "successful" : "fail");
			break;
		case MESG_REQ_RSAPuk:
			E2LogPrint(this, "MessageProcessing_Handshake()::Rec  MESG_REQ_RSAPuk");
			CCY_Decode_RSAPublicKey(&cgRSA_Puk,strMesg);
			CCY_Encode_RSAPublicKey(&strMesg,cgRSA_Prk);
			E2LogPrint(this, "MessageProcessing_Handshake()::Send MESG_ANS_RSAPuk");
			SYS_Delay_SetTS(&Time_Handshake,HandshakeTime);
			blRet = CtrlCHWrite(cgfnSignature.Encode(strMesg,cgRSA_Puk),MESG_ANS_RSAPuk);
			E2LogPrint(this, "MessageProcessing_Handshake()::Send MESG_ANS_RSAPuk %s",(blRet > 0) ? "successful" : "fail");
			break;
		case MESG_ANS_RSAPuk:
			blRet = 0;
			E2LogPrint(this, "MessageProcessing_Handshake()::Rec  MESG_ANS_RSAPuk");
			if (cgfnSignature.Decode(&strContent,strMesg,cgRSA_Prk) > 0){
				CCY_Decode_RSAPublicKey(&cgRSA_Puk,strContent);
				strMesg = Str_DecToHex(CCT_AES256);
				strMesg += ',';
				strMesg += Str_DecToHex(CCT_AES_CBC);
				strMesg += ',';
				strMesg += Str_DecToHex(CCT_SHA1);
				strMesg += ',';
				strMesg += Str_DecToHex(G_LITTLE_ENDIAN);
				strMesg += ',';
				strMesg += CCY_CreateRandKey(32);
				E2LogPrint(this, "MessageProcessing_Handshake()::Send MESG_REQ_UpdateAESKey");
				SYS_Delay_SetTS(&Time_Handshake,HandshakeTime);
				blRet = CtrlCHWrite(cgfnSignature.Encode(strMesg,cgRSA_Puk),MESG_REQ_UpdateAESKey);
				E2LogPrint(this, "MessageProcessing_Handshake()::Send MESG_REQ_UpdateAESKey %s",(blRet > 0) ? "successful" : "fail");
			}
			else{
				E2LogPrint(this, "MessageProcessing_Handshake()::Fail MESG_ANS_RSAPuk,Signature");
			}
			break;
		case MESG_REQ_UpdateAESKey:
			blRet = 0;
			E2LogPrint(this, "MessageProcessing_Handshake()::Rec  MESG_REQ_UpdateAESKey");
			ClrSSLFLAG(blUpdateKeyS | blUpdateKeyY | blUpdateKeyN);
			SetSSLFLAG(blUpdateKeyS);
			if (cgfnSignature.Decode(&strContent,strMesg,cgRSA_Prk) > 0){
				E2LogPrint(this, "MessageProcessing_Handshake()::Send MESG_ANS_UpdateAESKey");
				SYS_Delay_SetTS(&Time_Handshake,HandshakeTime);
				blRet = CtrlCHWrite(cgfnSignature.Encode(strContent,cgRSA_Puk),MESG_ANS_UpdateAESKey);
				E2LogPrint(this, "MessageProcessing_Handshake()::Send MESG_ANS_UpdateAESKey %s",(blRet > 0) ? "successful" : "fail");
				if (blRet > 0){
					oldAES_type = cgAES_type;
					oldAES_mode = cgAES_mode;
					oldDigestType = cgDigestType;
					oldEndianType = cgEndianType;
					oldStrKey = cgKey;
					
					strMesg = CCY_AESKey32Bye(strContent);
					newAES_type = (CCT_AES_KEYL)Str_HexToDec(Str_ReadSubItem(&strContent, ","));
					newAES_mode = (CCT_AES_MODE)Str_HexToDec(Str_ReadSubItem(&strContent, ","));
					newDigestType = (CCT_Digest)Str_HexToDec(Str_ReadSubItem(&strContent, ","));
					newEndianType = (G_Endian_VAILD)Str_HexToDec(Str_ReadSubItem(&strContent, ","));
					
					SetConfig(newAES_type,newAES_mode,newDigestType,newEndianType);
					SetKey(strMesg);
					E2LogPrint(this, "MessageProcessing_Handshake()::Using the new Key");
					SYS_Delay_SetTS(&Time_UpdateAESKey,HandshakeTime);
				}
			}
			else{
				E2LogPrint(this, "MessageProcessing_Handshake()::Fail MESG_REQ_UpdateAESKey,Signature");
			}
			break;
		case MESG_ANS_UpdateAESKey:
			blRet = 0;
			E2LogPrint(this, "MessageProcessing_Handshake()::Rec  MESG_ANS_UpdateAESKey");
			ClrSSLFLAG(blUpdateKeyS | blUpdateKeyY | blUpdateKeyN);
			SetSSLFLAG(blUpdateKeyS);
			if (cgfnSignature.Decode(&strContent, strMesg, cgRSA_Prk) > 0){
				oldAES_type = cgAES_type;
				oldAES_mode = cgAES_mode;
				oldDigestType = cgDigestType;
				oldEndianType = cgEndianType;
				oldStrKey = cgKey;
				
				strMesg = CCY_AESKey32Bye(strContent);
				newAES_type = (CCT_AES_KEYL)Str_HexToDec(Str_ReadSubItem(&strContent, ","));
				newAES_mode = (CCT_AES_MODE)Str_HexToDec(Str_ReadSubItem(&strContent, ","));
				newDigestType = (CCT_Digest)Str_HexToDec(Str_ReadSubItem(&strContent, ","));
				newEndianType = (G_Endian_VAILD)Str_HexToDec(Str_ReadSubItem(&strContent, ","));
				
				SetConfig(newAES_type,newAES_mode,newDigestType,newEndianType);
				SetKey(strMesg);
				E2LogPrint(this, "MessageProcessing_Handshake()::Using the new Key");
				SYS_Delay_SetTS(&Time_UpdateAESKey,HandshakeTime);
				SYS_DelayMS(10);
				E2LogPrint(this, "MessageProcessing_Handshake()::Send MESG_REQ_Test");
				SYS_Delay_SetTS(&Time_Handshake,HandshakeTime);
				blRet = CtrlCHWrite(CCY_CreateRandKey(32),MESG_REQ_Test);
				E2LogPrint(this, "MessageProcessing_Handshake()::Send MESG_REQ_Test %s",(blRet > 0) ? "successful" : "fail");
			}
			else{
				E2LogPrint(this, "MessageProcessing_Handshake()::Fail MESG_ANS_UpdateAESKey,Signature");
			}
			break;
		case MESG_REQ_Test:
			E2LogPrint(this, "MessageProcessing_Handshake()::Rec  MESG_REQ_Test");
			E2LogPrint(this, "MessageProcessing_Handshake()::Send MESG_ANS_Test");
			blRet = CtrlCHWrite(CCY_CreateRandKey(32),MESG_ANS_Test);
			E2LogPrint(this, "MessageProcessing_Handshake()::Send MESG_ANS_Test %s",(blRet > 0) ? "successful" : "fail");
			goto T1;
		case MESG_ANS_Test:
			E2LogPrint(this, "MessageProcessing_Handshake()::Rec  MESG_ANS_Test");
		T1:;
			if (ChkSSLFLAG(blUpdateKeyS) > 0){
				SetSSLFLAG(blUpdateKeyY);
				ClrSSLFLAG(blUpdateKeyS);
				E2LogPrint(this, "MessageProcessing_Handshake()::UpdateAESKey success");
			}
			if (ChkSSLFLAG(blHandshakeS) > 0){
				SetSSLFLAG(blHandshakeY);
				ClrSSLFLAG(blHandshakeS);
				E2LogPrint(this, "MessageProcessing_Handshake()::Handshake success");
			}
			break;
		case MESG_REQ_CloseSocket:
			E2LogPrint(this, "MessageProcessing_Handshake()::Rec  MESG_REQ_CloseSocket");
			E2LogPrint(this, "MessageProcessing_Handshake()::Send MESG_ANS_CloseSocket");
			blRet = CtrlCHWrite("",MESG_ANS_CloseSocket);
			E2LogPrint(this, "MessageProcessing_Handshake()::Send MESG_ANS_CloseSocket %s",(blRet > 0) ? "successful" : "fail");
			PushSend();
			E2LogPrint(this, "MessageProcessing_Handshake()::Call ClrblConnected()");
			SetSSLFLAG(blClose);
			ClrblConnected();
			break;
		case MESG_ANS_CloseSocket:
			E2LogPrint(this, "MessageProcessing_Handshake()::Rec  MESG_ANS_CloseSocket");
			PushSend();
			E2LogPrint(this, "MessageProcessing_Handshake()::Call ClrblConnected()");
			SetSSLFLAG(blClose);
			ClrblConnected();
			break;
		default:
			blDo = 0;
			break;
	}
	if (blRet == 0){
		if (ChkSSLFLAG(blUpdateKeyS) > 0){
			SetSSLFLAG(blUpdateKeyN);
			ClrSSLFLAG(blUpdateKeyS);
		}
		if (ChkSSLFLAG(blHandshakeS) > 0){
			SetSSLFLAG(blHandshakeN);
			ClrSSLFLAG(blHandshakeS);
		}
	}
	return(blDo);
}
//------------------------------------------------------------------------------------------//
void SSLSocket::Handshake(void){
	int32 blRet;
	cgFIFOfnMesgTx.Empty();
	cgfnMesgTx.SetContent("", MESG_INI_HandshakeInit);
	MessageProcessing_Handshake(&cgfnMesgTx,1);
	cgFIFOfnMesgTx.Empty();

	if ((GetCSType() == CSType_TCPS) || (GetCSType() == CSType_UDPS))
		return;
	E2LogPrint(this, "Send MESG_REQ_StartHandshake");
	blRet = CtrlCHWrite(CCY_CreateRandKey(32), MESG_REQ_StartHandshake);
	E2LogPrint(this, "Send MESG_REQ_StartHandshake %s",(blRet > 0) ? "successful" : "fail");
	if (blRet > 0){
		ClrSSLFLAG(blHandshakeY);
		SetSSLFLAG(blHandshakeS);
	}
	else{
		ClrSSLFLAG(blHandshakeN);
		ClrSSLFLAG(blHandshakeS);
	}
}
//------------------------------------------------------------------------------------------//
int32 SSLSocket::CheckHandshake(void){
	while(ChkSSLFLAG(blHandshakeS) > 0)
		SYS_SleepMS(100);
	return(ChkSSLFLAG(blHandshakeY));
}
//------------------------------------------------------------------------------------------//
ODEV_NODE *SSLSocket::CreateSelfODev(COLSTRING::COLType tCOLType){
	Spin_InUse_set();
	if (GetcgSelfODdev() == nullptr)
		SetcgSelfODdev(new SSLSocket_ODEV(this,tCOLType));
	Spin_InUse_clr();
	return(GetcgSelfODdev());
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif