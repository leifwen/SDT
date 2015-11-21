/*
 * Copyright (c) 2012-2014 
 * All rights reserved.
 *
 * File Name: Commu_SSLSocket.h
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2015.10.26
*/
#ifndef Commu_SSLSocketH
#define Commu_SSLSocketH
//------------------------------------------------------------------------------------------//
#include "Commu_DBuf.h"
#include "Comm_Crypto.h"
#include "Commu_Socket.h"
//------------------------------------------------------------------------------------------//
class CSSL_FN_T0 :public Field_Node{
	public:
		enum{MAX_PACKAGE_SZIE = 1024 / 2};
				 CSSL_FN_T0(void) : Field_Node(){AddNode(&fn_Head);AddNode(&fn_AESCC);};
		virtual	~CSSL_FN_T0(void){;};
	private:
		Field_Node		fn_Head;
		CCY_FN_AES		fn_AESCC;
	
		PUB_SBUF		*cgPSBUF;
		std::string		cgKey;
		uint32			cgMaxPDSize;
		uint32			cgInvalidLength;
		inline	void	Lock(void)	{Spin_InUse_set();cgPSBUF->Spin_InUse_set();};
		inline	void	Unlock(void){cgPSBUF->Spin_InUse_clr();Spin_InUse_clr();};
	public:
		inline const uint32&	GetPackageDataSize(void)const{return(cgMaxPDSize);};
		inline const uint32&	GetcgInvalidLength(void)const{return(cgInvalidLength);};
		void	Init(const PUB_SBUF *tSBUF,CCT_AES_KEYL type = CCT_AES128,CCT_AES_MODE mode = CCT_AES_CBC
					 ,CCT_Digest digestType = CCT_MD5,G_Endian_VAILD tEV = G_LITTLE_ENDIAN){
			cgPSBUF = (PUB_SBUF*)tSBUF;
			Field_Node::Init(FNT_MESSAGE,&cgPSBUF->cgBufFIFO,tEV);
			fn_Head.Init	(FNT_HEAD,&cgPSBUF->cgBufFIFO,7,tEV);	fn_Head.SetFixValue("LF.W.TZ");
			fn_AESCC.Init	(&cgPSBUF->cgBufFIFO,type,mode,digestType,tEV);
			
			cgKey = CCY_AESKey32Bye("LF.W.TZ");
			cgInvalidLength = 7 + fn_AESCC.GetcgInvalidLength();
			cgMaxPDSize = MAX_PACKAGE_SZIE - cgInvalidLength;
		};
		void	SetConfig(CCT_AES_KEYL type = CCT_AES128,CCT_AES_MODE mode = CCT_AES_CBC
						  ,CCT_Digest digestType = CCT_MD5,G_Endian_VAILD tEV = G_LITTLE_ENDIAN){
			SetEndianType(this,tEV);
			SetEndianType(&fn_Head,tEV);
			fn_AESCC.SetConfig(type,mode,digestType,tEV);
		};
	public:
		inline	void	SetKey		(const std::string &skey)	{Lock();cgKey = skey;Unlock();};
		inline	int32	Decode		(FIFO_UINT8 *retfifoOriginal,const uint8 *data,uint32 num){
			int32	ret;
			Lock();
			GetcgDefFifo(this)->Put(data,num);
			ret = 0;
			while (TryGetFrameAgain_if_NoEnoughDataInFIFO() > 0)
				ret |= fn_AESCC.ReadContent(retfifoOriginal,cgKey);
			Unlock();
			return(ret);
		};
		inline	int32	SetContent	(const FIFO_UINT8 &fifoIn,uint32 num,uint32 offset){
			int32 ret;
			Lock();
			ret = 0;
			if (GetcgDefFifo(this)->Unused() > (num + cgInvalidLength)){
				HoldOffset();
				fn_Head.SetFIFOFixValue();
				fn_AESCC.SetContent(fifoIn, num, offset, cgKey);
				UpdateLength();
				ret = 1;
			}
			Unlock();
			return(ret);
		};
		inline	int32	SetContent	(const uint8 *data,uint32 num){
			int32 ret;
			Lock();
			ret = 0;
			if (GetcgDefFifo(this)->Unused() > (num + cgInvalidLength)){
				HoldOffset();
				fn_Head.SetFIFOFixValue();
				fn_AESCC.SetContent(data, num, cgKey);
				UpdateLength();
				ret = 1;
			}
			Unlock();
		return(ret);
		};
		inline	int32	SetContent	(const std::string &strInput){return(CSSL_FN_T0::SetContent((uint8*)strInput.c_str(),(uint32)strInput.length()));};
};
//------------------------------------------------------------------------------------------//
class CSSL_FR_T1 :public Field_Node{
	public:
				 CSSL_FR_T1(void) : Field_Node(){AddNode(&fn_CH);AddNode(&fn_ID);AddNode(&fn_Control);AddNode(&fn_Offset);AddNode(&fn_Info);};
		virtual	~CSSL_FR_T1(void){;};
	private:
		Field_Node		fn_CH;
		Field_Node		fn_ID;
		Field_Node		fn_Control;
		Field_Node		fn_Offset;
		FNode_LC		fn_Info;
	
		uint32			cgCH;
		uint32			cgID;
		uint32			cgInvalidLength;
	
		PUB_SBUF		cgRxSBUF;
		FIFO_UINT8		cgTxFIFO;
	public:
		inline const uint32	&GetcgInvalidLength	(void)const{return(cgInvalidLength);};
	
		void	Init		(uint32 txfifoSize,uint32 rxfifoSize,uint32 tCH,G_Endian_VAILD tEV = G_LITTLE_ENDIAN);
		void	SetConfig	(G_Endian_VAILD tEV);
	private:
		void	ANS_ACK						(CSSL_FN_T0 *sslT0Tx,const uint32 &tID,const uint32 &offset);
		void	ANS_Request_ResendPackage	(CSSL_FN_T0 *sslT0Tx,const uint32 &tID,const uint32 &offset);
		void	ANS_Request_ResendAll		(CSSL_FN_T0 *sslT0Tx,const uint32 &tID);
		int32	CHK_Data(uint32 *tID,uint32 *ctrl,uint32 *offset);
		uint32	TX_Data	(CSSL_FN_T0 *sslT0Tx,const uint8 *&data,const uint32 &num,const uint32 &offset,const uint32 &packageSize,const uint32 &tID);
	public:
		void	Delivery	(CSSL_FN_T0 *sslT0Rx,RTREE_NODE *retFNList,const uint8 *databuf,int32 num);
		int32	RX_Packaging(CSSL_FN_T0 *sslT0Tx,FIFO_UINT8 *retFifoInfo,uint32 kbps = 8);
		int32	TX_Packaging(CSSL_FN_T0 *sslT0Tx,const uint8 *data,uint32 num,uint32 kbps = 8);
		int32	TX_Packaging(CSSL_FN_T0 *sslT0Tx,const FIFO_UINT8 &fifo,uint32 num,uint32 offset,uint32 kbps = 8);
		int32	TX_Packaging(CSSL_FN_T0 *sslT0Tx,const Field_Node &fnNode,uint32 kbps = 8);
		int32	TX_LoopBack	(const Field_Node &fnNode,uint32 kbps = 8);
};
//------------------------------------------------------------------------------------------//
class CSSL_FR_T1D : public RTREE_NODE{
	public:
		enum{RFLAG_C = 0, RFLAG_S = RTREE_NODE::RFLAG_S + RTREE_NODE::RFLAG_C};
	public:
				 CSSL_FR_T1D(void) : RTREE_NODE(){;};
		virtual ~CSSL_FR_T1D(void){;};
	public:
		void	Init(RTREE_NODE *fwFNList,const CSSL_FN_T0 *t0_tx,uint32 rxfifoSize,uint32 tCH,G_Endian_VAILD tEV = G_LITTLE_ENDIAN){
			tCH <<= 1;
			fn_sslT0_TX = (CSSL_FN_T0*)t0_tx;
			fn_sslT1_CHRX.Init(t0_tx->GetPackageDataSize(),rxfifoSize,tCH,tEV);
			fn_sslT1_CHTX.Init(t0_tx->GetPackageDataSize(),rxfifoSize,tCH + 1,tEV);
			fn_sslT1_CHRX.selfName = "fn_sslT1_CH";
			fn_sslT1_CHRX.selfName += Str_IntToString(tCH);
			fn_sslT1_CHTX.selfName = "fn_sslT1_CH";
			fn_sslT1_CHTX.selfName += Str_IntToString(tCH + 1);
			fwFNList->AddNode(&fn_sslT1_CHRX);
			fwFNList->AddNode(&fn_sslT1_CHTX);
		}
		void	SetConfig(G_Endian_VAILD tEV){fn_sslT1_CHRX.SetConfig(tEV);fn_sslT1_CHTX.SetConfig(tEV);};
	private:
		CSSL_FN_T0		*fn_sslT0_TX;
		CSSL_FR_T1		fn_sslT1_CHRX;
		CSSL_FR_T1		fn_sslT1_CHTX;
	public:
		inline	int32	RX_Packaging(FIFO_UINT8 *retFifoInfo,uint32 kbps = 8){
			return(fn_sslT1_CHRX.RX_Packaging(fn_sslT0_TX,retFifoInfo,kbps));
		};
		inline	int32	TX_Packaging(const uint8 *data,uint32 num,uint32 kbps = 8){
			return(fn_sslT1_CHTX.TX_Packaging(fn_sslT0_TX,data,num,kbps));
		};
		inline	int32	TX_Packaging(const std::string &strData,uint32 kbps = 8){
			return(fn_sslT1_CHTX.TX_Packaging(fn_sslT0_TX,(uint8*)strData.c_str(),(uint32)strData.length(),kbps));
		};
		inline	int32	TX_Packaging(const FIFO_UINT8 &fifo,uint32 num,uint32 offset,uint32 kbps = 8){
			return(fn_sslT1_CHTX.TX_Packaging(fn_sslT0_TX,fifo,num,offset,kbps));
		};
		inline	int32	TX_Packaging(const Field_Node &fnNode,uint32 kbps = 8){
			return(fn_sslT1_CHTX.TX_Packaging(fn_sslT0_TX,fnNode,kbps));
		};
		inline int32	TX_LoopBack	(const Field_Node &fnNode,uint32 kbps = 8){
			return(fn_sslT1_CHRX.TX_LoopBack(fnNode,kbps));
		};
};
//------------------------------------------------------------------------------------------//
class CSSL_FR_T1D_POOL : public RTREE_NODE{
	public:
		enum{RFLAG_C = 0, RFLAG_S = RTREE_NODE::RFLAG_S + RTREE_NODE::RFLAG_C};
	public:
				 CSSL_FR_T1D_POOL(void) : RTREE_NODE(){CreateTrash(this);cgCHID = 0;};
		virtual ~CSSL_FR_T1D_POOL(void){DestroyAll();};
	public:
		uint32	cgCHID;
		CSSL_FR_T1D	*CreateNewT1D(RTREE_NODE *fwFNList,const CSSL_FN_T0 *t0_tx,uint32 rxfifoSize,G_Endian_VAILD tEV = G_LITTLE_ENDIAN){
			CSSL_FR_T1D *ret;
			ret = new CSSL_FR_T1D;
			if (ret != nullptr){
				ret->Init(fwFNList,t0_tx,rxfifoSize,++cgCHID,tEV);
				AddNode(ret);
			}
			return(ret);
	};
};
//------------------------------------------------------------------------------------------//
#ifdef USE_OPENSSL
//------------------------------------------------------------------------------------------//
class SSLSocket : public APISocket{
	public:
		enum{RFLAG_C = 2, RFLAG_S = APISocket::RFLAG_S + APISocket::RFLAG_C};
		enum{HandshakeTime = 5000};
	public:
				 SSLSocket(const ODEV_LIST *tODEV_LIST,uint32 tSize) : APISocket(tODEV_LIST,tSize){
					 Init(CCT_AES128,CCT_AES_CBC,CCT_MD5,G_LITTLE_ENDIAN);
					 selfName = "SSLSocket ";
				 };
		virtual ~SSLSocket(void){FreeRSA_Prk();};
	protected:
		void	Init	(CCT_AES_KEYL type = CCT_AES128,CCT_AES_MODE mode = CCT_AES_CBC,CCT_Digest digestType = CCT_MD5,G_Endian_VAILD tEV = G_LITTLE_ENDIAN);
	public:
		void	SetConfig(CCT_AES_KEYL type = CCT_AES128,CCT_AES_MODE mode = CCT_AES_CBC,CCT_Digest digestType = CCT_MD5,G_Endian_VAILD tEV = G_LITTLE_ENDIAN);
		virtual	ODEV_NODE	*CreateSelfODev	(COLSTRING::COLType tCOLType = COLSTRING::COLType_TXT);
	private:
		CSSL_FN_T0		fn_sslT0_Rx;
		CSSL_FN_T0		fn_sslT0_Tx;
		CSSL_FR_T1		fn_sslT1_RXFW;
		RTREE_NODE		fwFNList;
	
		std::string		cgKey;
		CCT_AES_KEYL	cgAES_type;
		CCT_AES_MODE	cgAES_mode;
		CCT_Digest 		cgDigestType;
		G_Endian_VAILD	cgEndianType;
	protected:
		RSA				*cgRSA_Prk;
		CSSL_FR_T1D		CSSL_T1D_CH0;
		CSSL_FR_T1D		CSSL_T1D_CH1;
	private:
		inline	void	SetKey			(const std::string &skey)	{cgKey = skey;fn_sslT0_Rx.SetKey(skey);fn_sslT0_Tx.SetKey(skey);};
				void	FreeRSA_Prk		(void)						{try{RSA_free(cgRSA_Prk);}catch(...){;};};
		virtual	int32	SetRSA_Prk		(void)						{return(CCY_RSA_CreateKey(cgRSA_Prk,2048));};
		virtual	void	Handshake		(CCY_FNLC_MESG *fnMesg);
		virtual void	RxForward		(const uint8 *databuf,int32 num){fn_sslT1_RXFW.Delivery(&fn_sslT0_Rx,&fwFNList,databuf,num);};
		virtual	int32	ExThreadFun		(void);
		virtual	int32	PrintThreadFun	(void){return 0;};
		virtual void	ThreadsStart	(void);
		virtual	int32	DoAfterOpen		(void){return(CheckHandshake());};
	private:
		inline	void	SetblUpdatedAESKey	(void)		{SetSFlag(RFLAG_CREATE(0));};
		inline	void	ClrblUpdatedAESKey	(void)		{ClrSFlag(RFLAG_CREATE(0));};
		inline	int32	CheckblUpdatedAESKey(void)const	{return(CheckSFlag(RFLAG_CREATE(0)));};
		inline	void	SetblHandshaked		(void)		{SetSFlag(RFLAG_CREATE(1));};
		inline	void	ClrblHandshaked		(void)		{ClrSFlag(RFLAG_CREATE(1));};
		inline	int32	CheckblHandshaked	(void)const	{return(CheckSFlag(RFLAG_CREATE(1)));};
				int32	CheckHandshake		(void);
	public:
		inline	int32	SSLRead	(FIFO_UINT8 *retFifoInfo,uint32 kbps = 8){return(CSSL_T1D_CH1.RX_Packaging(retFifoInfo,kbps));};
		inline	int32	SSLWrite(const uint8 *data,uint32 num,uint32 kbps = 8){return(CSSL_T1D_CH1.TX_Packaging(data,num,kbps));};
		inline	int32	SSLWrite(const std::string &strData,uint32 kbps = 8){return(CSSL_T1D_CH1.TX_Packaging(strData,kbps));};
		inline	int32	SSLWrite(const Field_Node &fnNode,uint32 kbps = 8){return(CSSL_T1D_CH1.TX_Packaging(fnNode,kbps));};
};
//------------------------------------------------------------------------------------------//
#endif
//------------------------------------------------------------------------------------------//
#endif