
/*
 * Copyright (c) 2012-2014
 * All rights reserved.
 *
 * File Name: Commu_SSL.cpp
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2015.10.26
 */
//------------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "Commu_SSL.h"
#include "Comm_Convert.h"
#include "Commu_DBuf.h"
//#define LOGPRINT_ENABLE
#define LOGPRINT_ENABLE2
#include "Comm_Log.h"
//------------------------------------------------------------------------------------------//
#if defined Commu_SSLH && defined COMMU_DBUFH
//------------------------------------------------------------------------------------------//
void CSSL_FN_T0::RxDelivery(TREE_NODE *retFNList){
	uint32	tCH;
	STDSTR retStr;
	retStr = "";
	ELog(this << "RxDelivery()::T0.cgDefFifo:" << GetDefFifo()->Used() << "/" << GetDefFifo()->Unused());
	while(TryGetFrame() > 0){
		tCH = pn_CH.GetValueCalc();
		ELog(this << "RxDelivery()::L:" << GetLength() << ",CH:" << tCH);
		TREE_LChildRChain_Traversal_LINE_nolock(CSSL_FR_T1,retFNList,
			if (operateNode_t->GetcgCH() == tCH){
				ELog(operateNode_t << "RxDelivery()::L:" << GetLength() << ",CH:" << tCH);
				operateNode_t->ReceiveT0Data(&pn_AESCC,cgKey);
				break;
			}
		);
		Out();
	}
};
//------------------------------------------------------------------------------------------//
void CSSL_FN_T0::SetPackageDlyMS(double dlyMS){
	static	int32 count = 0;
	Spin_InUse_set();
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
	Spin_InUse_clr();
};
//------------------------------------------------------------------------------------------//
double CSSL_FN_T0::GetPackageDlyMS(void){
	double	dt;
	Spin_InUse_set();
	dt = cgPackageDlyMS;
	Spin_InUse_clr();
	return(dt);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
void CSSL_FR_T1::ReceiveT0Data(CCY_FN_AES *fnAESofT0,const STDSTR &strKey){
	fnAESofT0->ReadContent(&cgRxFIFO,strKey);
	ELog(this << "ReceiveT0Data()::T1.cgRxFIFO:<<" << cgRxFIFO.Used() << "/" <<cgRxFIFO.Unused());
};
//------------------------------------------------------------------------------------------//
void CSSL_FR_T1::Init(uint32 tCH,uint32 rxfifoSize,G_ENDIAN tEV){
	
	cgTxFIFO.Init(MAX_PACKAGE_SIZE << 2);
	cgRxFIFO.Init(rxfifoSize);
	
	PROTOCOL_NODE::Init(FNT_MESSAGE,&cgTxFIFO,tEV);
	pn_GroupID.Init	(FNT_CTRL,&cgTxFIFO,2,tEV);
	pn_CtrlID.Init	(FNT_CTRL,&cgTxFIFO,1,tEV);
	pn_Offset.Init	(FNT_CTRL,&cgTxFIFO,2,tEV);
	pn_Info.Init	(&cgTxFIFO,2,tEV);
	
	cgCH = tCH;
	cgGroupID = 0;
	cgLastOffset = 0;
	cgPackageDlyMS = CSSL_FN_T0::MAX_PACKAGE_DEALYTM;
};
//------------------------------------------------------------------------------------------//
void CSSL_FR_T1::SetConfig(G_ENDIAN tEV){
	SetEndianType(tEV);
	pn_GroupID.SetEndianType(tEV);
	pn_CtrlID.SetEndianType(tEV);
	pn_Offset.SetEndianType(tEV);
	pn_Info.SetEndianType(tEV);
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
int32 CSSL_FR_T1::RX_Data(CSSL_FN_T0 *sslT0Tx,uint32 *recNum,uint32 *tGroupID,uint32 *lastOffset,FIFO8 *retFifo,uint32 *retGroupID,uint32 *retCtrlID,uint32 *retOffset){
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
	if (recNum != nullptr)
		*recNum = 0;
	do{
		offset = 0;
		recResult = REC_Nothing;
		if (cgRxFIFO.Used() > 0){
			ELog(this << "RX_Data()::Rec dly:" << (uint32)delayMS << "ms,D1:" << dlyTS.DTime1.FormatDateTime("<hh:mm:ss.zzz>"));
			while(AnalysisFrame(&cgRxFIFO,offset) > 0){
				recResult = REC_RecPackage;
				rec_PGoffset = GetOffset();
				rec_PGlength = GetLength();
				offset = rec_PGoffset + rec_PGlength;
				rec_GroupID = pn_GroupID.GetValueCalc(&cgRxFIFO);
				rec_CtrlID = pn_CtrlID.GetValueCalc(&cgRxFIFO);
				rec_Offset = pn_Offset.GetValueCalc(&cgRxFIFO);
				rec_InfoL = pn_Info.GetContentLength();
				ELog(this << "RX_Data()::     GroupID:" << *tGroupID << ", reqTimes:" << requestTimes << ",nextOffset:" << nextOffset);
				ELog(this << "RX_Data()::ANAL GroupID:" << rec_GroupID <<",RecCtrlID:" << rec_CtrlID << ", RecOffset:" << rec_Offset
					 << ",RecInfoL:" << rec_InfoL << ",PGoffset:" << rec_PGoffset << ",PGlength:" << rec_PGlength);
				if (*tGroupID > rec_GroupID){
					if (*tGroupID - rec_GroupID < 0x7fff){
						if (rec_PGoffset == 0)
							cgRxFIFO.Out(rec_PGlength);
						break;
					}
					*tGroupID = rec_GroupID;
					*lastOffset = 0;
				}
				switch (rec_CtrlID){
					case CSSL_ENDPackage:
						if ((*tGroupID == rec_GroupID) && (nextOffset == rec_Offset)){
							if (retFifo != nullptr){
								pn_Info.ReadContent(retFifo,&cgRxFIFO);
								if (recNum != nullptr)
									*recNum += rec_InfoL;
							}
							if (rec_PGoffset == 0){
								cgRxFIFO.Out(rec_PGlength);
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
								if (retFifo != nullptr){
									pn_Info.ReadContent(retFifo,&cgRxFIFO);
									if (recNum != nullptr)
										*recNum += rec_InfoL;
								}
								if (rec_PGoffset == 0){
									cgRxFIFO.Out(rec_PGlength);
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
									cgRxFIFO.Out(rec_PGlength);
									offset = 0;
								}
							}
							else if (SYS_Delay_CheckTS(&dlyTS) != 0){
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
						else if (SYS_Delay_CheckTS(&dlyTS) != 0){
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
							cgRxFIFO.Out(rec_PGlength);
							offset = 0;
						}
						recResult = REC_GetPackage;
						break;
					}
				}
				if (recResult == REC_GetPackage)
					break;
			}
			if (recResult == REC_GetPackage){
				*lastOffset = 0;
				ELog(this << "RX_Data()::Get full Package,T1.cgRxFIFO:" << cgRxFIFO.Used() << "/" << cgRxFIFO.Unused());
				return 1;
			}
			SYS_SleepMS(2);
		}
	}while(recResult > 0);
	*lastOffset = nextOffset;
	return 0;
}
//------------------------------------------------------------------------------------------//
uint32 CSSL_FR_T1::RX_Packaging(CSSL_FN_T0 *sslT0Tx,FIFO8 *retFifo){
	uint32 recNum;
	return(RX_Data(sslT0Tx,&recNum,&cgGroupID,&cgLastOffset,retFifo,nullptr,nullptr,nullptr) > 0?recNum:0);
}
//------------------------------------------------------------------------------------------//
const CSSL_FR_T1&CSSL_FR_T1::SetContent(const uint32 &tGroupID,const uint32 &tCtrlID,const uint32 &offset,const uint8 *data,uint32 num){
	PROTOCOL_NODE::_Start();
	pn_GroupID.SetFIFOByte(tGroupID);
	pn_CtrlID.SetFIFOByte(tCtrlID);
	pn_Offset.SetFIFOByte(offset);
	pn_Info.SetContent(data,num);
	PROTOCOL_NODE::_Endl();
	return(*this);
}
//------------------------------------------------------------------------------------------//
const CSSL_FR_T1&CSSL_FR_T1::SetContent(const uint32 &tGroupID,const uint32 &tCtrlID,const uint32 &offset,const STDSTR &strIn){
	PROTOCOL_NODE::_Start();
	pn_GroupID.SetFIFOByte(tGroupID);
	pn_CtrlID.SetFIFOByte(tCtrlID);
	pn_Offset.SetFIFOByte(offset);
	pn_Info.SetContent(strIn);
	PROTOCOL_NODE::_Endl();
	return(*this);
}
//------------------------------------------------------------------------------------------//
void CSSL_FR_T1::ANS_ACK(CSSL_FN_T0 *sslT0Tx,const uint32 &tGroupID,const uint32 &offset){
	if (sslT0Tx == nullptr)
		return;
	GetDefFifo()->Empty();
	sslT0Tx->SetContent(SetContent(tGroupID,CSSL_ACK,offset,""),cgCH + 1);
	ELog(this << "RX_Data()::Send GroupID:" << tGroupID << ",CtrlID:CSSL_ACK,Offset:" << offset << ",T1:" << GetLength() << ",T0:" << sslT0Tx->GetLength());
}
//------------------------------------------------------------------------------------------//
void CSSL_FR_T1::ANS_ENDACK(CSSL_FN_T0 *sslT0Tx,const uint32 &tGroupID){
	if (sslT0Tx == nullptr)
		return;
	GetDefFifo()->Empty();
	sslT0Tx->SetContent(SetContent(tGroupID,CSSL_ENDACK,0,""),cgCH + 1);
	ELog(this << "RX_Data()::Send GroupID:" << tGroupID << ",CtrlID:CSSL_ENDACK,Offset:0,T1:" << GetLength()<< ",T0:" << sslT0Tx->GetLength());
}
//------------------------------------------------------------------------------------------//
void CSSL_FR_T1::ANS_Request_ResendPackage(CSSL_FN_T0 *sslT0Tx,const uint32 &tGroupID,const uint32 &offset){
	if (sslT0Tx == nullptr)
		return;
	GetDefFifo()->Empty();
	sslT0Tx->SetContent(SetContent(tGroupID,CSSL_ResendPackage,offset,""),cgCH + 1);
	ELog(this << "RX_Data()::Send GroupID:" << tGroupID << ",CtrlID:CSSL_ResendPackage,Offset:" << offset << ",T1:" << GetLength() << ",T0:" << sslT0Tx->GetLength());
}
//------------------------------------------------------------------------------------------//
void CSSL_FR_T1::ANS_Request_ResendAll(CSSL_FN_T0 *sslT0Tx,const uint32 &tGroupID){
	if (sslT0Tx == nullptr)
		return;
	GetDefFifo()->Empty();
	sslT0Tx->SetContent(SetContent(tGroupID,CSSL_ResendAll,0,""),cgCH + 1);
	ELog(this << "RX_Data()::Send GroupID:" << tGroupID << ",CtrlID:CSSL_ResendAll,Offset:0,T1:" << GetLength() << ",T0:" << sslT0Tx->GetLength());
}
//------------------------------------------------------------------------------------------//
int32 CSSL_FR_T1::CHK_Data(uint32 *retGroupID,uint32 *retCtrlID,uint32 *retOffset){
	uint32 tGroupID,lastOffset;
	tGroupID = cgGroupID;
	lastOffset = 0;
	if (RX_Data(nullptr,nullptr,&tGroupID,&lastOffset,nullptr,retGroupID,retCtrlID,retOffset) > 0)
		return(cgGroupID == tGroupID);
	return 0;
}
//------------------------------------------------------------------------------------------//
uint32 CSSL_FR_T1::TX_Data(CSSL_FN_T0 *sslT0Tx,const uint8 *&data,const uint32 &num,const uint32 &offset,const uint32 &packageSize,const uint32 &tGroupID){
	uint32 length;
	length = (packageSize < (num - offset)) ? packageSize : (num - offset);
	GetDefFifo()->Empty();
	sslT0Tx->SetContent(SetContent(tGroupID,((length + offset) < num) ? CSSL_Package : CSSL_ENDPackage,offset,data + offset,length),cgCH - 1);
	ELog(this << "TX_Packaging()::TX_Data()::Send GroupID:" << tGroupID << ",CtrlID:" << (((length + offset) < num) ? "CSSL_Package" : "CSSL_ENDPackage")
		 << ",Offset:" << offset << ",DataNum:" << length << ",T1:" << GetLength() << ",T0:" << sslT0Tx->GetLength());
	return(length);
}
//------------------------------------------------------------------------------------------//
uint32 CSSL_FR_T1::TX_Packaging(CSSL_FN_T0 *sslT0Tx,const uint8 *data,uint32 num){
	uint32		packageSize,maxCount,count;
	uint32		offset,ackOffset,resendTimes,retGroupID,retCtrlID,retOffset;
	SYS_TIME_S	dlyTS,reDlyTS,totalTS;
	double		delayMS,avgMS;
	
	if (num == 0)
		return 1;
	
	Spin_InUse_set();
	packageSize = (((MAX_PACKAGE_SIZE - sslT0Tx->GetInvalidLength()) >> 4) << 4) - 1 - GetInvalidLength();
	maxCount = (COMMU_DBUF_SSL::PACKAGE_MAX_SIZE) / (MAX_PACKAGE_SIZE);
	resendTimes = 0;
	ELog(this << "TX_Packaging()::data num:" << num <<",InvalidLength:" << sslT0Tx->GetInvalidLength() + GetInvalidLength() << ",PGsize:" << packageSize << ",maxCount:" << maxCount);

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
				ELog(this << "TX_Packaging()::T" << resendTimes + 1 << ",dly:" << (uint32)delayMS << "ms,D1:" << dlyTS.DTime1.FormatDateTime("[hh:mm:ss.zzz]"));
				offset += TX_Data(sslT0Tx,data,num,offset,packageSize,cgGroupID);
			}
			while(CHK_Data(&retGroupID,&retCtrlID,&retOffset) > 0){
				ELog(this << "TX_Packaging()::Rec RecGroupID:" << retGroupID << ",RecCtrlID:" << retCtrlID << ",RecOffset:" << retOffset << ",offset:" << offset);
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
					ELog(this << "TX_Packaging()::T" << resendTimes + 1 << ",GroupID:" << cgGroupID <<",End:OK");
					SYS_Delay_CheckTS(&totalTS);
					totalTS.DTime2 += (delayMS / 1000);
					totalTS.DTime2 -= totalTS.DTime1;
					avgMS = totalTS.DTime2.GetSec() * 1000 / (1 + num / MAX_PACKAGE_SIZE);
					sslT0Tx->SetPackageDlyMS(avgMS);
					ELog(this << "TX_Packaging()::avgMS" << (uint32)avgMS << "dms,new dlyMS:" << (uint32)sslT0Tx->GetPackageDlyMS() << "ms");
					goto TX_Packaging_END;
				}
			}
			if (count == maxCount)
				SYS_SleepMS(10);
		}while(SYS_Delay_CheckTS(&dlyTS) == 0);
		ELog(this << "TX_Packaging()::T" << resendTimes + 1 << ",GroupID:" << cgGroupID << ",End:" << ((retCtrlID == CSSL_ENDACK)?"OK":"Fail"));
		delayMS = sslT0Tx->GetPackageDlyMS() * 3;
		sslT0Tx->SetPackageDlyMS(delayMS);
	}while(++resendTimes < 5);
TX_Packaging_END:;
	++ cgGroupID;
	Spin_InUse_clr();
	cgPackageDlyMS = sslT0Tx->GetPackageDlyMS();
	return((retCtrlID == CSSL_ENDACK)?num:0);
}
//------------------------------------------------------------------------------------------//
uint32 CSSL_FR_T1::TX_Packaging(CSSL_FN_T0 *sslT0Tx,const FIFO8 &fifo,uint32 num,uint32 offset){
	uint32	slength;
	int32	ret;

	ret = 0;
	slength = fifo.CalcOutLength(num, offset);
	if (num > 0)
		ret = TX_Packaging(sslT0Tx,fifo.GetPointer(offset),num);
	if (slength > 0)
		ret += TX_Packaging(sslT0Tx,fifo.GetPointer(0),slength);
	return(ret);
}
//------------------------------------------------------------------------------------------//
uint32 CSSL_FR_T1::TX_Packaging(CSSL_FN_T0 *sslT0Tx,const PROTOCOL_NODE &pnNode){
	uint32	slength,num,offset;
	int32	ret;
	
	ret = 0;
	num = pnNode.GetLength();
	offset = pnNode.GetOffset();
	slength = pnNode.GetDefFifo()->CalcOutLength(num, offset);
	if (num > 0)
		ret = TX_Packaging(sslT0Tx,pnNode.GetDefFifo()->GetPointer(offset),num);
	if (slength > 0)
		ret += TX_Packaging(sslT0Tx,pnNode.GetDefFifo()->GetPointer(0),slength);
	return(ret);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
STDSTR GetMesgText(uint32 mID){
	STDSTR retStr = "";
#ifdef LOGPRINT
	retStr = "(" + Str_ToString(mID)+ ")";
	switch(mID){
		case MESG_NONE					:retStr += "MESG_NONE ";break;
		case MESG_INI_HandshakeInit		:retStr += "MESG_INI_HandshakeInit ";break;
		case MESG_REQ_StartHandshake	:retStr += "MESG_REQ_StartHandshake ";break;
		case MESG_REQ_RSAPuk			:retStr += "MESG_REQ_RSAPuk ";break;
		case MESG_ANS_RSAPuk			:retStr += "MESG_ANS_RSAPuk ";break;
		case MESG_REQ_UpdateAESKey		:retStr += "MESG_REQ_UpdateAESKey ";break;
		case MESG_ANS_UpdateAESKey		:retStr += "MESG_ANS_UpdateAESKey ";break;
		case MESG_REQ_Test				:retStr += "MESG_REQ_Test ";break;
		case MESG_ANS_Test				:retStr += "MESG_ANS_Test ";break;
		default							:retStr = "";break;
	}
#endif
	return(retStr);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
void CSSL_FR_T2::Init(COMMU_DBUF_SSL *tCOMMU,const SBUFFER *rxSBUF,const SBUFFER *txSBUF,CCT_AES_KEYL type,CCT_AES_MODE mode,CCT_Digest digestType,G_ENDIAN tEV){
	cgDataOutSBUF = (SBUFFER*)rxSBUF;
	cgT0RxFifo.Init(cgDataOutSBUF->BufferMaxSize());
	cgT0TxFifo = &((SBUFFER*)txSBUF)->cgFIFO;
	pn_sslT0_Rx.Init(&cgT0RxFifo,type,mode,digestType,tEV);
	pn_sslT0_Tx.Init( cgT0TxFifo,type,mode,digestType,tEV);
	CSSL_T1D_DataCH.Init(0,&pn_sslT0_Tx,&fwPNList,cgDataOutSBUF->BufferMaxSize(),tEV);
	CSSL_T1D_CtrlCH.Init(1,&pn_sslT0_Tx,&fwPNList,COMMU_DBUF_SSL::PACKAGE_MAX_SIZE << 2,tEV);
	cgCtrlOutFifo.Init(COMMU_DBUF_SSL::PACKAGE_MAX_SIZE);
	Reset(0);
	SetConfig(type,mode,digestType,tEV);
	cgfnMesgTxFIFO.Init(COMMU_DBUF_SSL::PACKAGE_MAX_SIZE << 2);
	cgfnMesgTx.Init(&cgfnMesgTxFIFO);
	cgfnMesgRx.Init(&cgCtrlOutFifo);
	cgfnSignature.Init(1024 * 8,type,mode,digestType,tEV);
	
	cgRSA_Prk = RSA_new();
	cgRSA_Puk = RSA_new();
	SetRSA_Prk();
	
	ctrlCHThread.ThreadInit(this,	&CSSL_FR_T2::CtrlCHThreadFun,"ctrlCH",(void*)tCOMMU);
	dataCHRxThread.ThreadInit(this, &CSSL_FR_T2::DataCHRxThreadFun,"dataCHRx",(void*)tCOMMU);
	dataCHTxThread.ThreadInit(this, &CSSL_FR_T2::DataCHTxThreadFun,"dataCHTx",(void*)tCOMMU);
	
	tCOMMU->AddThread(&ctrlCHThread);
	tCOMMU->AddThread(&dataCHRxThread);
	tCOMMU->AddThread(&dataCHTxThread);
	
	SSLDisable();
};
//------------------------------------------------------------------------------------------//
void CSSL_FR_T2::Reset(int32 blResetPuk){
	SetConfig(CCT_AES128,CCT_AES_CBC,CCT_MD5,G_ENDIAN_LITTLE);
	ClrSFlag(blHandshakeY | blHandshakeN | blClose);

	SetKey(CCY_AESKey32Bye("LF.W.TZ"));
	pn_sslT0_Rx.SetPackageDlyMS(CSSL_FN_T0::MAX_PACKAGE_DEALYTM * 3);
	pn_sslT0_Tx.SetPackageDlyMS(CSSL_FN_T0::MAX_PACKAGE_DEALYTM * 3);
	pn_sslT0_Rx.ResetKey();
	pn_sslT0_Tx.ResetKey();
	CSSL_T1D_DataCH.Reset();
	CSSL_T1D_CtrlCH.Reset();
	CSSL_T1D_DataCH.Clean();
	CSSL_T1D_CtrlCH.Clean();
	
	if (blResetPuk > 0){
		RSA_free(cgRSA_Puk);
		cgRSA_Puk = RSA_new();
	}
};
//------------------------------------------------------------------------------------------//
void CSSL_FR_T2::SetConfig(CCT_AES_KEYL type,CCT_AES_MODE mode,CCT_Digest digestType,G_ENDIAN tEV){
	pn_sslT0_Tx.SetConfig(type,mode,digestType,tEV);
	pn_sslT0_Rx.SetConfig(type,mode,digestType,tEV);
	CSSL_T1D_DataCH.SetConfig(tEV);
	CSSL_T1D_CtrlCH.SetConfig(tEV);
	cgAES_type = type;
	cgAES_mode = mode;
	cgDigestType = digestType;
	cgEndianType = tEV;
};
//------------------------------------------------------------------------------------------//
void CSSL_FR_T2::SSLEnable(COMMU_DBUF_SSL *tCOMMU){
	ctrlCHThread.Enable();
	dataCHRxThread.Enable();
	if (CheckSFlag(blNoBlock) != 0)
		dataCHTxThread.Enable();
};
//------------------------------------------------------------------------------------------//
void CSSL_FR_T2::SSLThreadRun(void){
	ctrlCHThread.ThreadRun();
	dataCHRxThread.ThreadRun();
	if (CheckSFlag(blNoBlock) != 0)
		dataCHTxThread.ThreadRun();
};
//------------------------------------------------------------------------------------------//
void CSSL_FR_T2::SSLDisable(void){
	ctrlCHThread.ThreadStop();
	dataCHRxThread.ThreadStop();
	dataCHTxThread.ThreadStop();
	ctrlCHThread.Disable();
	dataCHRxThread.Disable();
	dataCHTxThread.Disable();
};
//------------------------------------------------------------------------------------------//
int32 CSSL_FR_T2::CtrlCHThreadFun(void *p){
	COMMU_DBUF_SSL	*commu= static_cast<COMMU_DBUF_SSL*>(p);
	STDSTR		strMesg;
	uint32		mID;
	
	E2Log(this << "CtrlThreadFun()::Running");
	MessageProcessing(commu,MESG_INI_HandshakeInit,"");
	while(ctrlCHThread.IsTerminated() == 0){
		if (commu->IsConnected() != 0){
			if (CtrlCHRead(&strMesg,&mID) > 0){
				ELog(&CSSL_T1D_CtrlCH << "CtrlThreadFun()::T2.cgCtrlOutFifo:" << cgCtrlOutFifo.Used() << "/" << cgCtrlOutFifo.Unused());
			}
			if (MessageProcessing(commu,mID,strMesg) == 0)
				commu->MessageProcessing(mID,strMesg);
		}
		SYS_SleepMS(2);
	}
	E2Log(this << "CtrlThreadFun()::Stop");
	return 1;
}
//------------------------------------------------------------------------------------------//
int32 CSSL_FR_T2::DataCHRxThreadFun(void *p){
	COMMU_DBUF_SSL	*commu= static_cast<COMMU_DBUF_SSL*>(p);
	
	E2Log(this << "DataCHRxThreadFun()::Running");
	
	while(dataCHRxThread.IsTerminated() == 0){
		if (commu->IsConnected() != 0){
			if (DataCHRead(&cgDataOutSBUF->cgFIFO) > 0)
				continue;
		}
		SYS_SleepMS(2);
	}
	E2Log(this << "DataCHRxThreadFun()::Stop");
	return 1;
}
//------------------------------------------------------------------------------------------//
int32 CSSL_FR_T2::DataCHTxThreadFun(void *p){
	COMMU_DBUF_SSL	*commu= static_cast<COMMU_DBUF_SSL*>(p);
	uint32 num;
	
	E2Log(this << "DataCHTxThreadFun()::Running");
	cgDataInSBUF.Init(cgT0TxFifo->MaxSize());
	while(dataCHTxThread.IsTerminated() == 0){
		if (commu->IsConnected() != 0){
			num = cgDataInSBUF.Used();
			if (CSSL_T1D_DataCH.TX_Packaging(cgDataInSBUF.cgFIFO,num,0) > 0){
				ELog(this << "DataCHTxThreadFun()::Send " << Str_ToString(num) << "bytes.");
				cgDataInSBUF.Out(num);
				continue;
			}
		}
		SYS_SleepMS(2);
	}
	E2Log(this << "DataCHTxThreadFun()::Stop");
	return 1;
}
//------------------------------------------------------------------------------------------//
#define CHSendInMP(_mID,_strMesg,_blRet)	CHSend(_mID,_strMesg,_blRet,"MessageProcessing_Handshake()::")
#define CHRecInMP(_mID)						CHRec(_mID,"MessageProcessing_Handshake()::");
#define E2LogInMP(_strMesg)					E2Log(this << "MessageProcessing_Handshake()::" << _strMesg);
//------------------------------------------------------------------------------------------//
int32 CSSL_FR_T2::MessageProcessing(COMMU_DBUF_SSL *tCOMMU,const uint32 &mID,const STDSTR &strMesg){
	STDSTR			strMesgT,strContent;
	CCT_AES_KEYL	newAES_type;
	CCT_AES_MODE	newAES_mode;
	CCT_Digest 		newDigestType;
	G_ENDIAN		newEndianType;
	int32			blDo,blRet;

	static	CCT_AES_KEYL	oldAES_type = cgAES_type;
	static	CCT_AES_MODE	oldAES_mode = cgAES_mode;
	static	CCT_Digest 		oldDigestType = cgDigestType;
	static	G_ENDIAN		oldEndianType = cgEndianType;
	static	STDSTR			oldStrKey = cgKey;
	static	SYS_TIME_S		Time_Handshake;
	
	blDo = 1;
	blRet = 1;
	if ((mID > MESG_NONE) && (mID < MESG_NEXT_SSL)){
		SYS_Delay_SetTS(&Time_Handshake,HandshakeTime << 10);
		CHRecInMP(mID);
	}
	switch (mID){
		case MESG_INI_HandshakeInit:
			ClrSFlag(blHandshakeY | blHandshakeN);
			break;
		case MESG_REQ_StartHandshake:
			ClrSFlag(blHandshakeY | blHandshakeN);
			CHSendInMP(MESG_REQ_RSAPuk,CCY_Encode_RSAPublicKey(&strMesgT,cgRSA_Prk),blRet);
			break;
		case MESG_REQ_RSAPuk:
			blRet = 0;
			if (CCY_Decode_RSAPublicKey(&cgRSA_Puk,strMesg) != 0){
				CCY_Encode_RSAPublicKey(&strMesgT,cgRSA_Prk);
				CHSendInMP(MESG_ANS_RSAPuk,cgfnSignature.Encode(strMesgT,cgRSA_Puk),blRet);
			}
			else{
				E2LogInMP("Fail MESG_REQ_RSAPuk,Decode");
			}
			break;
		case MESG_ANS_RSAPuk:
			blRet = 0;
			if (cgfnSignature.Decode(&strContent,strMesg,cgRSA_Prk) > 0){
				CCY_Decode_RSAPublicKey(&cgRSA_Puk,strContent);
				strMesgT = Str_DecToHex(CCT_AES256);
				strMesgT += ',';
				strMesgT += Str_DecToHex(CCT_AES_CBC);
				strMesgT += ',';
				strMesgT += Str_DecToHex(CCT_SHA1);
				strMesgT += ',';
				strMesgT += Str_DecToHex(G_ENDIAN_LITTLE);
				strMesgT += ',';
				strMesgT += CCY_CreateRandKey(32);
				CHSendInMP(MESG_REQ_UpdateAESKey,cgfnSignature.Encode(strMesgT,cgRSA_Puk),blRet);
			}
			else{
				E2LogInMP("Fail MESG_ANS_RSAPuk,Signature");
			}
			break;
		case MESG_REQ_UpdateAESKey:
			blRet = 0;
			if (cgfnSignature.Decode(&strContent,strMesg,cgRSA_Prk) > 0){
				CHSendInMP(MESG_ANS_UpdateAESKey,cgfnSignature.Encode(strContent,cgRSA_Puk),blRet);
				if (blRet > 0){
					oldAES_type = cgAES_type;
					oldAES_mode = cgAES_mode;
					oldDigestType = cgDigestType;
					oldEndianType = cgEndianType;
					oldStrKey = cgKey;
					
					strMesgT = CCY_AESKey32Bye(strContent);
					newAES_type = (CCT_AES_KEYL)Str_HexToDec(Str_ReadSubItem(&strContent, ","));
					newAES_mode = (CCT_AES_MODE)Str_HexToDec(Str_ReadSubItem(&strContent, ","));
					newDigestType = (CCT_Digest)Str_HexToDec(Str_ReadSubItem(&strContent, ","));
					newEndianType = (G_ENDIAN)Str_HexToDec(Str_ReadSubItem(&strContent, ","));
					
					E2LogInMP("Using the new Key");
					SetConfig(newAES_type,newAES_mode,newDigestType,newEndianType);
					SetKey(strMesgT);
				}
			}
			else{
				E2LogInMP("Fail MESG_REQ_UpdateAESKey,Signature");
			}
			break;
		case MESG_ANS_UpdateAESKey:
			blRet = 0;
			if (cgfnSignature.Decode(&strContent, strMesg, cgRSA_Prk) > 0){
				oldAES_type = cgAES_type;
				oldAES_mode = cgAES_mode;
				oldDigestType = cgDigestType;
				oldEndianType = cgEndianType;
				oldStrKey = cgKey;
				
				strMesgT = CCY_AESKey32Bye(strContent);
				newAES_type = (CCT_AES_KEYL)Str_HexToDec(Str_ReadSubItem(&strContent, ","));
				newAES_mode = (CCT_AES_MODE)Str_HexToDec(Str_ReadSubItem(&strContent, ","));
				newDigestType = (CCT_Digest)Str_HexToDec(Str_ReadSubItem(&strContent, ","));
				newEndianType = (G_ENDIAN)Str_HexToDec(Str_ReadSubItem(&strContent, ","));
				
				E2LogInMP("Using the new Key");
				SetConfig(newAES_type,newAES_mode,newDigestType,newEndianType);
				SetKey(strMesgT);
				SYS_DelayMS(10);
				CHSendInMP(MESG_REQ_Test,CCY_CreateRandKey(32),blRet);
			}
			else{
				E2LogInMP("Fail MESG_ANS_UpdateAESKey,Signature");
			}
			break;
		case MESG_REQ_Test:
			CHSendInMP(MESG_ANS_Test,CCY_CreateRandKey(32),blRet);
			if (blRet == 0){
				E2LogInMP("MESG_REQ_Test fail");
				break;
			}
		case MESG_ANS_Test:
			E2LogInMP("MESG_REQ_Test success");
			if (CheckSFlag(blHandshakeY | blHandshakeN) == 0){
				SetSFlag(blHandshakeY);
				E2LogInMP("Handshake success");
			}
			break;
		case MESG_NONE:;
		default:
			blDo = 0;
			break;
	}
	if ((blDo != 0) && (blRet == 0))
		SetSFlag(blHandshakeN);
	
	if (CheckSFlag(blHandshakeN) || ((CheckSFlag(blHandshakeY) == 0) && (SYS_Delay_CheckTS(&Time_Handshake) != 0))){
		E2LogInMP("Handshake fail, disconnect socket");
		SetSFlag(blHandshakeN);
		SetSFlag(blClose);
		tCOMMU->SelfClose();
	}
	return(blDo);
}
//------------------------------------------------------------------------------------------//
int32 CSSL_FR_T2::Handshake(COMMU_DBUF_SSL *tCOMMU){
	int32	blRet;
	cgSendCHMesgLock.Lock();
	blRet = 0;
	do{
		if (tCOMMU->IsConnected() == 0)
			break;
		blRet = 1;
		if (tCOMMU->GetCSType() == CSType_UDPS)
			break;
		
		if (tCOMMU->GetCSType() != CSType_TCPS){
			CHSend(MESG_REQ_StartHandshake,CCY_CreateRandKey(32),blRet,"Handshake()::");
		}
		if (blRet > 0){
			while(CheckSFlag(blHandshakeY | blHandshakeN) == 0)
				SYS_SleepMS(100);
			blRet = CheckSFlag(blHandshakeY);
		}
	}while(0);
	cgSendCHMesgLock.Unlock();
	return((blRet > 0)?1:0);
}
//------------------------------------------------------------------------------------------//
#endif

