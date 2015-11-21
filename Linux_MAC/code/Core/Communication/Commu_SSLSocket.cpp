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
#include "Commu_SSLSocket.h"
#include "Comm_Convert.h"
#include "SYS_Time.h"
#include "Comm_File.h"
//------------------------------------------------------------------------------------------//
void CSSL_FR_T1::Init(uint32 txfifoSize,uint32 rxfifoSize,uint32 tCH,G_Endian_VAILD tEV){
	
	cgTxFIFO.Init(txfifoSize << 1);
	cgRxSBUF.Init(rxfifoSize);
	
	Field_Node::Init(FNT_MESSAGE,&cgTxFIFO,tEV);
	fn_CH.Init		(FNT_CTRL,&cgTxFIFO,1,tEV);
	fn_ID.Init		(FNT_CTRL,&cgTxFIFO,2,tEV);
	fn_Control.Init	(FNT_CTRL,&cgTxFIFO,1,tEV);
	fn_Offset.Init	(FNT_CTRL,&cgTxFIFO,2,tEV);
	fn_Info.Init	(&cgTxFIFO,2,tEV);
	
	cgCH = tCH;
	cgID = 0;
	cgInvalidLength = 1 + 2 + 1 + 2 + 2;
};
//------------------------------------------------------------------------------------------//
void CSSL_FR_T1::SetConfig(G_Endian_VAILD tEV){
	SetEndianType(this,tEV);
	SetEndianType(&fn_CH,tEV);
	SetEndianType(&fn_ID,tEV);
	SetEndianType(&fn_Control,tEV);
	SetEndianType(&fn_Offset,tEV);
	SetEndianType(&fn_Info,tEV);
};
//------------------------------------------------------------------------------------------//
void CSSL_FR_T1::Delivery(CSSL_FN_T0 *sslT0Rx,RTREE_NODE *retFNList,const uint8 *databuf,int32 num){
	cgRxSBUF.Spin_InUse_set();
	sslT0Rx->Decode(&cgRxSBUF.cgBufFIFO,databuf,num);
	while (AnalysisFrameTryAgain(cgRxSBUF.cgBufFIFO) > 0){
		RTREE_LChildRChain_Traversal_LINE(CSSL_FR_T1,retFNList,
			if (operateNode_t->cgCH == fn_CH.GetOriginalValue(&cgRxSBUF.cgBufFIFO)){
				ReadAllContent(&operateNode_t->cgRxSBUF,&cgRxSBUF.cgBufFIFO);
				break;
			}
		);
		Out(&cgRxSBUF.cgBufFIFO);
	}
	cgRxSBUF.Spin_InUse_clr();
};
//------------------------------------------------------------------------------------------//
enum{
	REC_Nothing = 0,
	REC_Package,
	REC_NextPackage,
	REC_EndPackage,
	REC_TimeOut,
	REC_UpdataID,
};
//------------------------------------------------------------------------------------------//
enum{
	CSSL_LoopBack = 0,
	CSSL_Package,
	CSSL_ENDPackage,
	CSSL_ACK,
	CSSL_ResendPackage,
	CSSL_ResendAll,
};
//------------------------------------------------------------------------------------------//
int32 CSSL_FR_T1::RX_Packaging(CSSL_FN_T0 *sslT0Tx,FIFO_UINT8 *retFifoInfo,uint32 kbps){
	uint32	offset,next_fn_Offset,requestTimes,recRet;
	uint32	rec_fn_ID,rec_fn_Ctrl,rec_fn_Offset,rec_fn_ContentL,rec_fnOffset,rec_fnLength;
	SYS_TIME_S	dlyTime;
	double	ts;

	requestTimes = 5;
	next_fn_Offset = 0;
	ts = (sslT0Tx->GetPackageDataSize() << 1) / kbps + 50;
	SYS_Delay_SetTS(&dlyTime,ts);
	do{
		offset = 0;
		recRet = REC_Nothing;
		if (cgRxSBUF.Used() > 0){
			cgRxSBUF.Spin_InUse_set();
			while(AnalysisFrame(cgRxSBUF.cgBufFIFO,offset) > 0){
				rec_fnOffset = GetOffset();
				rec_fnLength = GetLength();
				rec_fn_ID = fn_ID.GetValueCalc(&cgRxSBUF.cgBufFIFO);
				rec_fn_Ctrl = fn_Control.GetValueCalc(&cgRxSBUF.cgBufFIFO);
				rec_fn_Offset = fn_Offset.GetValueCalc(&cgRxSBUF.cgBufFIFO);
				rec_fn_ContentL = fn_Info.GetContentLength();
				if ((rec_fn_Ctrl == CSSL_LoopBack) && (next_fn_Offset == 0) && (offset == 0)){
					fn_Info.ReadContent(retFifoInfo,&cgRxSBUF.cgBufFIFO);
					cgRxSBUF.Out(rec_fnOffset+ rec_fnLength,G_LOCK_OFF);
					next_fn_Offset = 0;
					recRet = REC_EndPackage;
					break;
				}
				else{
					if (cgID > rec_fn_ID){
						if (cgID - rec_fn_ID < 0x7fffffff){
							if (offset == 0)
								cgRxSBUF.Out(rec_fnOffset + rec_fnLength,G_LOCK_OFF);
							break;
						}
						cgID = rec_fn_ID;
					}
					if (cgID == rec_fn_ID){
						if (next_fn_Offset == rec_fn_Offset){
							requestTimes = 5;
							SYS_Delay_SetTS(&dlyTime,ts);
							fn_Info.ReadContent(retFifoInfo,&cgRxSBUF.cgBufFIFO);
							recRet = REC_Package;
							if (offset == 0)
								cgRxSBUF.Out(rec_fnOffset+ rec_fnLength,G_LOCK_OFF);
							ANS_ACK(sslT0Tx, cgID, next_fn_Offset);
							next_fn_Offset += rec_fn_ContentL;
							if (rec_fn_Ctrl == CSSL_ENDPackage){
								ANS_ACK(sslT0Tx, cgID, next_fn_Offset);
								++ cgID;
								next_fn_Offset = 0;
								recRet = REC_EndPackage;
							}
							break;
						}
						else if (next_fn_Offset > rec_fn_Offset){
							if (offset == 0){
								cgRxSBUF.Out(rec_fnOffset + rec_fnLength,G_LOCK_OFF);
								break;
							}
						}
						else{
							recRet = REC_NextPackage;
							if (SYS_Delay_CheckTS(&dlyTime) > 0){
								if (requestTimes-- == 0){
									++ cgID;
									next_fn_Offset = 0;
									recRet = REC_UpdataID;
								}
								else{
									ANS_Request_ResendPackage(sslT0Tx, cgID, next_fn_Offset);
								}
								SYS_Delay_SetTS(&dlyTime,ts);
							}
						}
					}
					else{
						if (SYS_Delay_CheckTS(&dlyTime) > 0){
							if (requestTimes-- == 0){
								cgID = rec_fn_ID;
								requestTimes = 5;
								next_fn_Offset = 0;
								recRet = REC_TimeOut;
							}
							else{
								ANS_Request_ResendAll(sslT0Tx, rec_fn_ID - 1);
							}
							SYS_Delay_SetTS(&dlyTime,ts);
						}
					}
				}
				offset = rec_fnOffset + rec_fnLength;
			}
			cgRxSBUF.Spin_InUse_clr();
		}
		if (recRet == REC_EndPackage)
			break;
		SYS_DelayMS(2);
	}while(recRet > 0);
	return(recRet == REC_EndPackage);
}
//------------------------------------------------------------------------------------------//
int32 CSSL_FR_T1::TX_LoopBack(const Field_Node &fnNode,uint32 kbps){
	Spin_InUse_set();
	GetcgDefFifo(this)->Empty();
	HoldOffset();
	fn_CH.SetFIFOByte(cgCH);
	fn_ID.SetFIFOByte(cgID);
	fn_Control.SetFIFOByte(CSSL_LoopBack);
	fn_Offset.SetFIFOByte(0);
	fn_Info.SetContent(fnNode);
	UpdateLength();
	cgRxSBUF.Put(*GetcgDefFifo(this),GetLength(),GetOffset());
	Spin_InUse_clr();
	return 1;
}
//------------------------------------------------------------------------------------------//
void CSSL_FR_T1::ANS_ACK(CSSL_FN_T0 *sslT0Tx,const uint32 &tID,const uint32 &offset){
	Spin_InUse_set();
	GetcgDefFifo(this)->Empty();
	HoldOffset();
	fn_CH.SetFIFOByte(cgCH + 1);
	fn_ID.SetFIFOByte(tID);
	fn_Control.SetFIFOByte(CSSL_ACK);
	fn_Offset.SetFIFOByte(offset);
	fn_Info.SetContent("");
	UpdateLength();
	sslT0Tx->SetContent(*GetcgDefFifo(this),GetLength(),GetOffset());
	Spin_InUse_clr();
}
//------------------------------------------------------------------------------------------//
void CSSL_FR_T1::ANS_Request_ResendPackage(CSSL_FN_T0 *sslT0Tx,const uint32 &tID,const uint32 &offset){
	Spin_InUse_set();
	GetcgDefFifo(this)->Empty();
	HoldOffset();
	fn_CH.SetFIFOByte(cgCH + 1);
	fn_ID.SetFIFOByte(tID);
	fn_Control.SetFIFOByte(CSSL_ResendPackage);
	fn_Offset.SetFIFOByte(offset);
	fn_Info.SetContent("");
	UpdateLength();
	sslT0Tx->SetContent(*GetcgDefFifo(this),GetLength(),GetOffset());
	Spin_InUse_clr();
}
//------------------------------------------------------------------------------------------//
void CSSL_FR_T1::ANS_Request_ResendAll(CSSL_FN_T0 *sslT0Tx,const uint32 &tID){
	Spin_InUse_set();
	GetcgDefFifo(this)->Empty();
	HoldOffset();
	fn_CH.SetFIFOByte(cgCH + 1);
	fn_ID.SetFIFOByte(tID);
	fn_Control.SetFIFOByte(CSSL_ResendAll);
	fn_Offset.SetFIFOByte(0);
	fn_Info.SetContent("");
	UpdateLength();
	sslT0Tx->SetContent(*GetcgDefFifo(this),GetLength(),GetOffset());
	Spin_InUse_clr();
}
//------------------------------------------------------------------------------------------//
int32 CSSL_FR_T1::CHK_Data(uint32 *tID,uint32 *ctrl,uint32 *offset){
	int32 ret;
	cgRxSBUF.Spin_InUse_set();
	ret = 0;
	while((AnalysisFrame(cgRxSBUF.cgBufFIFO) > 0) && (ret == 0)){
		if (fn_CH.GetValueCalc(&cgRxSBUF.cgBufFIFO) == cgCH){
			*tID = fn_ID.GetValueCalc(&cgRxSBUF.cgBufFIFO);
			*ctrl = fn_Control.GetValueCalc(&cgRxSBUF.cgBufFIFO);
			*offset = fn_Offset.GetValueCalc(&cgRxSBUF.cgBufFIFO);
			ret = 1;
		}
		Out(&cgRxSBUF.cgBufFIFO);
	}
	cgRxSBUF.Spin_InUse_clr();
	return(ret);
}
//------------------------------------------------------------------------------------------//
uint32 CSSL_FR_T1::TX_Data(CSSL_FN_T0 *sslT0Tx,const uint8 *&data,const uint32 &num,const uint32 &offset,const uint32 &packageSize,const uint32 &tID){
	uint32 length;
	length = (packageSize < (num - offset)) ? packageSize : (num - offset);
	Spin_InUse_set();
	GetcgDefFifo(this)->Empty();
	HoldOffset();
	fn_CH.SetFIFOByte(cgCH - 1);
	fn_ID.SetFIFOByte(tID);
	fn_Control.SetFIFOByte(((length + offset) < num) ? CSSL_Package : CSSL_ENDPackage);
	fn_Offset.SetFIFOByte(offset);
	fn_Info.SetContent(data + offset,length);
	UpdateLength();
	if (sslT0Tx->SetContent(*GetcgDefFifo(this),GetLength(),GetOffset()) == 0)
		length = 0;
	Spin_InUse_clr();
	return(length);
}
//------------------------------------------------------------------------------------------//
int32 CSSL_FR_T1::TX_Packaging(CSSL_FN_T0 *sslT0Tx,const uint8 *data,uint32 num,uint32 kbps){
	//8kbps = 1024 bytes per s
	uint32	packageSize,length,offset;
	uint32	rxID,rxCtrl,rxOffset,ackOffset;
	SYS_TIME_S	dlyTime;
	double	ts;
	
	packageSize = (((sslT0Tx->GetPackageDataSize() - GetcgInvalidLength()) >> 4) << 4);
	offset = 0;
	ackOffset = 0;
	length = 0;
	ts = (((num / packageSize + 1) * packageSize) << 1) / kbps + 50;
	SYS_Delay_SetTS(&dlyTime,ts);
	cgRxSBUF.Clean();
	do{
		if (offset < num){
			length = TX_Data(sslT0Tx,data,num,offset,packageSize,cgID);
			offset += length;
		}
		while(CHK_Data(&rxID,&rxCtrl,&rxOffset) > 0){
			if (rxID == cgID){
				if (rxCtrl == CSSL_ResendPackage){
					SYS_Delay_SetTS(&dlyTime,ts);
					do{
					   if (TX_Data(sslT0Tx,data,num,rxOffset,packageSize,cgID) > 0)
						   break;
					}while(SYS_Delay_CheckTS(&dlyTime) == 0);
					break;
				}
				else if (rxCtrl == CSSL_ResendAll){
					SYS_Delay_SetTS(&dlyTime,ts);
					offset = 0;
					ackOffset = 0;
					continue;
				}
				else if ((rxCtrl == CSSL_ACK) && (ackOffset < rxOffset)){
					ackOffset = rxOffset;
					if (ackOffset == num)
						goto HAPPYEND;
				}
			}
		}
		if (length == 0)
			SYS_DelayMS(2);
	}while(SYS_Delay_CheckTS(&dlyTime) == 0);
HAPPYEND:;
	++ cgID;
	return(ackOffset == num);
}
//------------------------------------------------------------------------------------------//
int32 CSSL_FR_T1::TX_Packaging(CSSL_FN_T0 *sslT0Tx,const FIFO_UINT8 &fifo,uint32 num,uint32 offset,uint32 kbps){
	uint32	slength;
	int32	ret;

	ret = 0;
	slength = fifo.CalcOutLength(num, offset);
	if (num > 0)
		ret = TX_Packaging(sslT0Tx,fifo.GetOutPointer(offset),num,kbps);
	if (slength > 0)
		ret &= TX_Packaging(sslT0Tx,fifo.GetOutPointer(0),slength,kbps);
	return(ret);
}
//------------------------------------------------------------------------------------------//
int32 CSSL_FR_T1::TX_Packaging(CSSL_FN_T0 *sslT0Tx,const Field_Node &fnNode,uint32 kbps){
	uint32	slength,num,offset;
	int32	ret;
	
	ret = 0;
	num = fnNode.GetLength();
	offset = fnNode.GetOffset();
	slength = GetcgDefFifo(&fnNode)->CalcOutLength(num, offset);
	if (num > 0)
		ret = TX_Packaging(sslT0Tx,GetcgDefFifo(&fnNode)->GetOutPointer(offset),num,kbps);
	if (slength > 0)
		ret &= TX_Packaging(sslT0Tx,GetcgDefFifo(&fnNode)->GetOutPointer(0),slength,kbps);
	return(ret);
}
//------------------------------------------------------------------------------------------//










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
			Spin_InUse_set(blLock);
			if (cgSSLSocket != nullptr)
				cgSSLSocket->SSLWrite(ReadStr(G_LOCK_OFF),8);
			Spin_InUse_clr(blLock);
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
enum{
	MESG_NONE = 0,
	MESG_Request_RSA_Puk,
	MESG_ANS_RSA_Puk,
	MESG_Start_Handshake,
	MESG_SH_Request_RSA_Puk,
	MESG_SH_ANS_RSA_Puk,
	MESG_Request_UpdateAESKey,
	MESG_ANS_UpdatedAESKey,
	MESG_Request_TestNewConfiguration,
	MESG_ANS_TestNewConfiguration,
};
//------------------------------------------------------------------------------------------//
void SSLSocket::Init(CCT_AES_KEYL type,CCT_AES_MODE mode,CCT_Digest digestType,G_Endian_VAILD tEV){
	fn_sslT0_Tx.Init(&cgTxBuffer,type,mode,digestType,tEV);
	fn_sslT0_Rx.Init(&cgRxBuffer,type,mode,digestType,tEV);
	fn_sslT1_RXFW.Init(fn_sslT0_Tx.GetPackageDataSize(),cgRxBuffer.BufferMaxSize(),tEV);
	CSSL_T1D_CH0.Init(&fwFNList,&fn_sslT0_Tx,cgRxBuffer.BufferMaxSize(),0,tEV);
	CSSL_T1D_CH1.Init(&fwFNList,&fn_sslT0_Tx,cgRxBuffer.BufferMaxSize(),1,tEV);
	
	SetKey(CCY_AESKey32Bye("LF.W.TZ"));
	
	cgAES_type = type;
	cgAES_mode = mode;
	cgDigestType = digestType;
	
	fn_sslT0_Tx.selfName	= "fn_sslT0_Tx";
	fn_sslT0_Rx.selfName	= "fn_sslT0_Rx";
	fn_sslT1_RXFW.selfName	= "fn_sslT1_RXFW";
	
	cgRSA_Prk = RSA_new();
	SetRSA_Prk();
};
//------------------------------------------------------------------------------------------//
void SSLSocket::SetConfig(CCT_AES_KEYL type,CCT_AES_MODE mode,CCT_Digest digestType,G_Endian_VAILD tEV){
	fn_sslT0_Tx.SetConfig(type,mode,digestType,tEV);
	fn_sslT0_Rx.SetConfig(type,mode,digestType,tEV);
	fn_sslT1_RXFW.SetConfig(tEV);
	CSSL_T1D_CH0.SetConfig(tEV);
	cgAES_type = type;
	cgAES_mode = mode;
	cgDigestType = digestType;
	cgEndianType = tEV;
};
//------------------------------------------------------------------------------------------//
void SSLSocket::ThreadsStart(void){
	txThread.ThreadRun();
	rxThread.ThreadRun();
	exThread.ThreadRun();
	printThread.ThreadRun();
}
//------------------------------------------------------------------------------------------//
int32 SSLSocket::ExThreadFun(void){
	std::string			strMesg,strContent,strOldKey;
	uint32 				mID,kbps;
	CCY_FNLC_MESG		fnMesg;
	CCY_FR_Signature	fnSignature;
	FIFO_UINT8			rxfifo;
	RSA					*rsaPuk;
	SYS_TIME_S			Time_UpdateAESKey,Time_Handshake;
	int32				blUpdate_AESKey,blUpdate_Handshake;
	CCT_AES_KEYL		oldAES_type;
	CCT_AES_MODE		oldAES_mode;
	CCT_Digest 			oldDigestType;
	G_Endian_VAILD		oldEndianType;
	
	kbps = 8;
	rxfifo.Init(1024 * 8);
	fnMesg.Init(&rxfifo);
	fnSignature.Init(1024 * 8,cgAES_type,cgAES_mode,cgDigestType);
	
	rsaPuk = RSA_new();
	strOldKey = cgKey;
	blUpdate_AESKey = 0;
	Handshake(&fnMesg);
	blUpdate_Handshake = 1;
	SYS_Delay_SetTS(&Time_Handshake,HandshakeTime);
	while((exThread.IsTerminated() == 0) && (IsConnected() > 0)){
		rxfifo.Empty();
		if (CSSL_T1D_CH0.RX_Packaging(&rxfifo,kbps) > 0){
			if (fnMesg.AnalysisFrame(rxfifo) > 0){
				fnMesg.ReadContent(&strMesg, &mID,&rxfifo);
				rxfifo.Empty();
				switch (mID) {
					case MESG_Request_RSA_Puk:
						//MyLogPrint(this, "Rec MESG_Request_RSA_Puk");
						CCY_Decode_RSAPublicKey(&rsaPuk, strMesg);
						CCY_Encode_RSAPublicKey(&strContent,cgRSA_Prk);
						CSSL_T1D_CH0.TX_Packaging(fnMesg.SetContent(fnSignature.Encode(strContent,rsaPuk),MESG_ANS_RSA_Puk),kbps);
						break;
					case MESG_ANS_RSA_Puk:
						//MyLogPrint(this, "Rec MESG_Request_RSA_Puk");
						if (fnSignature.Decode(&strContent, strMesg, cgRSA_Prk) > 0)
							CCY_Decode_RSAPublicKey(&rsaPuk, strContent);
						break;
					case MESG_Start_Handshake:
						//MyLogPrint(this, "Rec MESG_Start_Handshake");
						ClrblUpdatedAESKey();
						CSSL_T1D_CH0.TX_Packaging(fnMesg.SetContent(CCY_Encode_RSAPublicKey(&strContent,cgRSA_Prk), MESG_SH_Request_RSA_Puk),kbps);
						break;
					case MESG_SH_Request_RSA_Puk:
						//MyLogPrint(this, "Rec MESG_SH_Request_RSA_Puk");
						CCY_Decode_RSAPublicKey(&rsaPuk, strMesg);
						CCY_Encode_RSAPublicKey(&strContent,cgRSA_Prk);
						CSSL_T1D_CH0.TX_Packaging(fnMesg.SetContent(fnSignature.Encode(strContent,rsaPuk),MESG_SH_ANS_RSA_Puk),kbps);
						break;
					case MESG_SH_ANS_RSA_Puk:
						//MyLogPrint(this, "Rec MESG_SH_ANS_RSA_Puk");
						if (fnSignature.Decode(&strContent, strMesg, cgRSA_Prk) > 0){
							CCY_Decode_RSAPublicKey(&rsaPuk, strContent);
							strContent = Str_DecToHex(CCT_AES256);
							strContent += ',';
							strContent += Str_DecToHex(CCT_AES_CBC);
							strContent += ',';
							strContent += Str_DecToHex(CCT_SHA1);
							strContent += ',';
							strContent += Str_DecToHex(G_LITTLE_ENDIAN);
							strContent += ',';
							strContent += CCY_CreateRandKey(32);
							CSSL_T1D_CH0.TX_Packaging(fnMesg.SetContent(fnSignature.Encode(strContent,rsaPuk),MESG_Request_UpdateAESKey),kbps);
						}
						break;
					case MESG_Request_UpdateAESKey:
						//MyLogPrint(this, "Rec MESG_Request_UpdateAESKey");
						ClrblUpdatedAESKey();
						if (fnSignature.Decode(&strContent, strMesg, cgRSA_Prk) > 0){
							CSSL_T1D_CH0.TX_Packaging(fnMesg.SetContent(fnSignature.Encode(strContent,rsaPuk),MESG_ANS_UpdatedAESKey),kbps);
							strOldKey = cgKey;
							strMesg = CCY_AESKey32Bye(strContent);
							oldAES_type = (CCT_AES_KEYL)Str_HexToDec(Str_ReadSubItem(&strContent, ","));
							oldAES_mode = (CCT_AES_MODE)Str_HexToDec(Str_ReadSubItem(&strContent, ","));
							oldDigestType = (CCT_Digest)Str_HexToDec(Str_ReadSubItem(&strContent, ","));
							oldEndianType = (G_Endian_VAILD)Str_HexToDec(Str_ReadSubItem(&strContent, ","));
							SetConfig(oldAES_type,oldAES_mode,oldDigestType,oldEndianType);
							SetKey(strMesg);
							SYS_DelayMS(10);
							blUpdate_AESKey = 1;
							SYS_Delay_SetTS(&Time_UpdateAESKey, HandshakeTime);
						}
						break;
					case MESG_ANS_UpdatedAESKey:
						//MyLogPrint(this, "Rec MESG_ANS_UpdatedAESKey");
						if (fnSignature.Decode(&strContent, strMesg, cgRSA_Prk) > 0){
							strOldKey = cgKey;
							strMesg = CCY_AESKey32Bye(strContent);
							oldAES_type = (CCT_AES_KEYL)Str_HexToDec(Str_ReadSubItem(&strContent, ","));
							oldAES_mode = (CCT_AES_MODE)Str_HexToDec(Str_ReadSubItem(&strContent, ","));
							oldDigestType = (CCT_Digest)Str_HexToDec(Str_ReadSubItem(&strContent, ","));
							oldEndianType = (G_Endian_VAILD)Str_HexToDec(Str_ReadSubItem(&strContent, ","));
							SetConfig(oldAES_type,oldAES_mode,oldDigestType,oldEndianType);
							SetKey(strMesg);
							SYS_DelayMS(10);
							blUpdate_AESKey = 1;
							SYS_Delay_SetTS(&Time_UpdateAESKey, HandshakeTime);
							CSSL_T1D_CH0.TX_Packaging(fnMesg.SetContent(CCY_CreateRandKey(32),MESG_Request_TestNewConfiguration),kbps);
						}
						break;
					case MESG_Request_TestNewConfiguration:
						//MyLogPrint(this, "Rec MESG_Request_TestNewConfiguration");
						CSSL_T1D_CH0.TX_Packaging(fnMesg.SetContent(CCY_CreateRandKey(32),MESG_ANS_TestNewConfiguration),kbps);
					case MESG_ANS_TestNewConfiguration:
						//MyLogPrint(this, "Rec MESG_ANS_TestNewConfiguration");
						blUpdate_AESKey = 0;
						SetblUpdatedAESKey();
						if (blUpdate_Handshake > 0){
							blUpdate_Handshake = 0;
							SetblHandshaked();
						}
						break;
					default:
						break;
				}
			}
		}
		if ((blUpdate_AESKey > 0) && (SYS_Delay_CheckTS(&Time_UpdateAESKey) > 0)){
			blUpdate_AESKey = 0;
			SetKey(strOldKey);
		}
		if ((blUpdate_Handshake > 0) && (SYS_Delay_CheckTS(&Time_Handshake) > 0)){
			//MyLogPrint(this, "Handshake timeover, disconnect socket");
			CloseDev();
			ClrblConnected();
			break;
		}
		SYS_SleepMS(10);
	}
	RSA_free(rsaPuk);
	return 1;
}
//------------------------------------------------------------------------------------------//
void SSLSocket::Handshake(CCY_FNLC_MESG *fnMesg){
	//MyLogPrint(this, "Send MESG_Start_Handshake");
	CSSL_T1D_CH0.TX_Packaging(fnMesg->SetContent(CCY_CreateRandKey(32), MESG_Start_Handshake),8);
}
//------------------------------------------------------------------------------------------//
int32 SSLSocket::CheckHandshake(void){
	SYS_TIME_S	Timedly;
	SYS_Delay_SetTS(&Timedly, HandshakeTime + 1000);
	while((CheckblHandshaked() == 0) && (IsConnected() > 0) && (SYS_Delay_CheckTS(&Timedly) == 0))
		SYS_DelayMS(100);
	return(1);//CheckblHandshaked());
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