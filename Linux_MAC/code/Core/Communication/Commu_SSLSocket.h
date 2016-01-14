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
#include "Commu_SocketServer.h"
//------------------------------------------------------------------------------------------//
#if (defined USE_OPENSSL) || (defined CommonDefH_MAC)
//------------------------------------------------------------------------------------------//
class CSSL_FN_T0 :public Field_Node{
	public:
		enum{MAX_PACKAGE_DEALYTM = 1024 * 4};
				 CSSL_FN_T0(void) : Field_Node(){AddNode(&fn_Head);AddNode(&fn_AESCC);AddNode(&fn_CH);};
		virtual	~CSSL_FN_T0(void){;};
	private:
		Field_Node		fn_Head;
		CCY_FN_AES		fn_AESCC;
		Field_Node		fn_CH;
	
		PUB_SBUF		*cgPSBUF;
		std::string		cgKey;
		double			cgPackageDlyMS;
		inline	void	Lock(void)	{Spin_InUse_set();cgPSBUF->Spin_InUse_set();};
		inline	void	Unlock(void){cgPSBUF->Spin_InUse_clr();Spin_InUse_clr();};
	public:
				void	SetPackageDlyMS(double dlyMS);
				double	GetPackageDlyMS(void);
		inline  uint32	GetInvalidLength(void)const{return(6 + fn_AESCC.GetInvalidLength() + 1);};
		void	Init(const PUB_SBUF *tSBUF,CCT_AES_KEYL type = CCT_AES128,CCT_AES_MODE mode = CCT_AES_CBC
					 ,CCT_Digest digestType = CCT_MD5,G_Endian_VAILD tEV = G_LITTLE_ENDIAN){
			cgPSBUF = (PUB_SBUF*)tSBUF;
			Field_Node::Init(FNT_MESSAGE,&cgPSBUF->cgBufFIFO,tEV);
			fn_Head.Init	(FNT_HEAD	,&cgPSBUF->cgBufFIFO,6,tEV);	fn_Head.SetFixValue("LF.W.T");
			fn_AESCC.Init	(&cgPSBUF->cgBufFIFO,type,mode,digestType,tEV);
			fn_CH.Init		(FNT_CTRL	,&cgPSBUF->cgBufFIFO,1,tEV);
			ResetKey();
			cgPackageDlyMS = MAX_PACKAGE_DEALYTM;
		};
		void	SetConfig(CCT_AES_KEYL type = CCT_AES128,CCT_AES_MODE mode = CCT_AES_CBC
						  ,CCT_Digest digestType = CCT_MD5,G_Endian_VAILD tEV = G_LITTLE_ENDIAN){
			SetEndianType(this,tEV);
			SetEndianType(&fn_Head,tEV);
			SetEndianType(&fn_CH,tEV);
			fn_AESCC.SetConfig(type,mode,digestType,tEV);
		};
	public:
		inline	void	ResetKey	(void)						{cgKey = CCY_AESKey32Bye("LF.W.TZ");};
		inline	void	SetKey		(const std::string &skey)	{Lock();cgKey = skey;Unlock();};
				void	Delivery	(RTREE_NODE *retFNList,const uint8 *databuf,int32 num);
		inline	int32	SetContent	(const FIFO_UINT8 &fifoIn,uint32 num,uint32 offset,uint32 channel){
			Lock();
			HoldOffset();
			fn_Head.SetFIFOFixValue();
			fn_AESCC.SetContent(fifoIn, num, offset, cgKey);
			fn_CH.SetFIFOByte(channel);
			UpdateLength();
			Unlock();
			return 1;
		};
		inline	int32	SetContent	(const uint8 *data,uint32 num,uint32 channel){
			Lock();
			HoldOffset();
			fn_Head.SetFIFOFixValue();
			fn_AESCC.SetContent(data, num, cgKey);
			fn_CH.SetFIFOByte(channel);
			UpdateLength();
			Unlock();
		return 1;
		};
		inline	int32	SetContent	(const std::string &strInput,uint32 channel){
			return(CSSL_FN_T0::SetContent((uint8*)strInput.c_str(),(uint32)strInput.length(),channel));
		};
};
//------------------------------------------------------------------------------------------//
class CSSL_FR_T1 :public Field_Node{
	public:
		enum{MAX_PACKAGE_SIZE = 1024 / 2};
				 CSSL_FR_T1(void) : Field_Node(){AddNode(&fn_GroupID);AddNode(&fn_CtrlID);AddNode(&fn_Offset);AddNode(&fn_Info);};
		virtual	~CSSL_FR_T1(void){;};
	private:
		Field_Node		fn_GroupID;
		Field_Node		fn_CtrlID;
		Field_Node		fn_Offset;
		FNode_LC		fn_Info;
	
		uint32			cgCH;
		uint32			cgGroupID;
		uint32			cgLastOffset;
	
		PUB_SBUF		cgRxSBUF;
		FIFO_UINT8		cgTxFIFO;
		double			cgPackageDlyMS;
	public:
		inline const	uint32		&GetcgCH			(void)const	{return(cgCH);};
		inline		 	uint32		GetInvalidLength	(void)const	{return(2 + 1 + 2 + 2);};
		inline			void		Clean				(void)		{cgRxSBUF.Clean();cgTxFIFO.Empty();};
		inline			void		Reset				(void)		{cgGroupID = 0;};
		void	Init		(uint32 rxfifoSize,uint32 tCH,G_Endian_VAILD tEV = G_LITTLE_ENDIAN);
		void	SetConfig	(G_Endian_VAILD tEV);
	private:
		int32	RX_Data(CSSL_FN_T0 *sslT0Tx,uint32 *tGroupID,uint32 *lastOffset,PUB_SBUF *retSBUFInfo
						,uint32 *retGroupID,uint32 *retCtrlID,uint32 *retOffset);
		void	ANS_ACK						(CSSL_FN_T0 *sslT0Tx,const uint32 &tGroupID,const uint32 &offset);
		void	ANS_ENDACK					(CSSL_FN_T0 *sslT0Tx,const uint32 &tGroupID);
		void	ANS_Request_ResendPackage	(CSSL_FN_T0 *sslT0Tx,const uint32 &tGroupID,const uint32 &offset);
		void	ANS_Request_ResendAll		(CSSL_FN_T0 *sslT0Tx,const uint32 &tGroupID);
		int32	CHK_Data					(uint32 *retGroupID,uint32 *retCtrlID,uint32 *retOffset);
		uint32	TX_Data	(CSSL_FN_T0 *sslT0Tx,const uint8 *&data,const uint32 &num,const uint32 &offset,const uint32 &packageSize,const uint32 &tGroupID);
	public:
		void	ReceiveT0Data	(CCY_FN_AES *fnAESofT0,const std::string &strKey);
		int32	RX_Packaging	(CSSL_FN_T0 *sslT0Tx,PUB_SBUF *retSBUFInfo);
		int32	TX_Packaging	(CSSL_FN_T0 *sslT0Tx,const uint8 *data,uint32 num);
		int32	TX_Packaging	(CSSL_FN_T0 *sslT0Tx,const FIFO_UINT8 &fifo,uint32 num,uint32 offset);
		int32	TX_Packaging	(CSSL_FN_T0 *sslT0Tx,const Field_Node &fnNode);
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
			fn_sslT1_CHRX.Init(rxfifoSize,tCH,tEV);
			fn_sslT1_CHTX.Init(rxfifoSize,tCH + 1,tEV);
			SetSelfName("");
			fwFNList->AddNode(&fn_sslT1_CHRX);
			fwFNList->AddNode(&fn_sslT1_CHTX);
		}
		void	SetConfig(G_Endian_VAILD tEV){fn_sslT1_CHRX.SetConfig(tEV);fn_sslT1_CHTX.SetConfig(tEV);};
	private:
		CSSL_FN_T0		*fn_sslT0_TX;
		CSSL_FR_T1		fn_sslT1_CHRX;
		CSSL_FR_T1		fn_sslT1_CHTX;
	public:
		inline	void	SetSelfName(const std::string &strName){
			selfName = strName;
			if (strName.length() > 0)
				selfName += "::";
			selfName += "CSSL_T1D_CH";
			selfName += Str_IntToString(fn_sslT1_CHRX.GetcgCH() >> 1);
			fn_sslT1_CHRX.selfName = selfName;
			fn_sslT1_CHRX.selfName += "::fn_sslT1_RXCH";
			fn_sslT1_CHRX.selfName += Str_IntToString(fn_sslT1_CHRX.GetcgCH());
			fn_sslT1_CHTX.selfName = selfName;
			fn_sslT1_CHTX.selfName += "::fn_sslT1_TXCH";
			fn_sslT1_CHTX.selfName += Str_IntToString(fn_sslT1_CHTX.GetcgCH());
		}
		inline	void	Clean		(void){fn_sslT1_CHRX.Clean();fn_sslT1_CHTX.Clean();};
		inline	void	Reset		(void){fn_sslT1_CHRX.Reset();fn_sslT1_CHTX.Reset();};
		inline	int32	RX_Packaging(PUB_SBUF *retSBUFInfo){
			return(fn_sslT1_CHRX.RX_Packaging(fn_sslT0_TX,retSBUFInfo));
		};
		inline	int32	TX_Packaging(const uint8 *data,uint32 num){
			return(fn_sslT1_CHTX.TX_Packaging(fn_sslT0_TX,data,num));
		};
		inline	int32	TX_Packaging(const std::string &strData){
			return(fn_sslT1_CHTX.TX_Packaging(fn_sslT0_TX,(uint8*)strData.c_str(),(uint32)strData.length()));
		};
		inline	int32	TX_Packaging(const FIFO_UINT8 &fifo,uint32 num,uint32 offset){
			return(fn_sslT1_CHTX.TX_Packaging(fn_sslT0_TX,fifo,num,offset));
		};
		inline	int32	TX_Packaging(const Field_Node &fnNode){
			return(fn_sslT1_CHTX.TX_Packaging(fn_sslT0_TX,fnNode));
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
#endif
#ifdef USE_OPENSSL
//------------------------------------------------------------------------------------------//
enum{
	MESG_NONE = 0,
	MESG_INI_HandshakeInit,
	MESG_REQ_StartHandshake,
	MESG_REQ_RSAPuk,
	MESG_ANS_RSAPuk,
	MESG_REQ_UpdateAESKey,
	MESG_ANS_UpdateAESKey,
	MESG_REQ_Test,
	MESG_ANS_Test,
	MESG_REQ_CloseSocket,
	MESG_ANS_CloseSocket,
	MESG_NEXT_SSL,
};
//------------------------------------------------------------------------------------------//
class SSLSocket : public APISocket{
	public:
		enum{RFLAG_C = 0, RFLAG_S = APISocket::RFLAG_S + APISocket::RFLAG_C};
		enum{HandshakeTime = 20000};	//8kbps = 1024 bytes per s
	public:
				 SSLSocket(const ODEV_LIST *tODEV_LIST,uint32 tSize) : APISocket(tODEV_LIST,tSize){
					 selfName = "SSLSocket";
					 Init(CCT_AES128,CCT_AES_CBC,CCT_MD5,G_LITTLE_ENDIAN);
				 };
		virtual ~SSLSocket(void){dataCHThread.RemoveSelf();FreeRSA_Prk();};
	protected:
		void	Init	(CCT_AES_KEYL type = CCT_AES128,CCT_AES_MODE mode = CCT_AES_CBC,CCT_Digest digestType = CCT_MD5,G_Endian_VAILD tEV = G_LITTLE_ENDIAN);
	public:
		void	SetConfig(CCT_AES_KEYL type = CCT_AES128,CCT_AES_MODE mode = CCT_AES_CBC,CCT_Digest digestType = CCT_MD5,G_Endian_VAILD tEV = G_LITTLE_ENDIAN);
		virtual	ODEV_NODE	*CreateSelfODev	(COLSTRING::COLType tCOLType = COLSTRING::COLType_TXT);
		inline	void	SetSelfName(const std::string &strName){
			selfName = strName;
			fn_sslT0_Tx.selfName	= strName + "::fn_sslT0_Tx";
			fn_sslT0_Rx.selfName	= strName + "::fn_sslT0_Rx";
			CSSL_T1D_CH0.SetSelfName(strName);
			CSSL_T1D_CH1.SetSelfName(strName);
		};
	private:
		PUB_SBUF		cgBufferT0_Rx;
		CSSL_FN_T0		fn_sslT0_Rx;
		CSSL_FN_T0		fn_sslT0_Tx;
		RTREE_NODE		fwFNList;
	
		std::string		cgKey;
		CCT_AES_KEYL	cgAES_type;
		CCT_AES_MODE	cgAES_mode;
		CCT_Digest 		cgDigestType;
		G_Endian_VAILD	cgEndianType;
	protected:
		PUB_SBUF		cgDataRXSBUF;
		PUB_SBUF		cgDataCHSBUF;
		CSSL_FR_T1D		CSSL_T1D_CH0;
		CSSL_FR_T1D		CSSL_T1D_CH1;
		RSA				*cgRSA_Prk;
		RSA				*cgRSA_Puk;
	private:
		uint32				cgSSLFlag;
		FIFO_UINT8			cgFIFOfnMesgTx;
		FNode_MESG			cgfnMesgTx;
		CCY_FR_Signature	cgfnSignature;
		SYS_ThreadEx<SSLSocket>		dataCHThread;
	private:
		virtual	int32	DataCHThreadFun	(void);
				void	FreeRSA_Prk		(void){try{RSA_free(cgRSA_Prk);RSA_free(cgRSA_Puk);}catch(...){;};};
		virtual	int32	SetRSA_Prk		(void){return(CCY_RSA_CreateKey(cgRSA_Prk,2048));};
		inline	void	SetKey			(const std::string &skey){cgKey = skey;fn_sslT0_Rx.SetKey(skey);fn_sslT0_Tx.SetKey(skey);};
		virtual	int32	ExThreadFun		(void);
		virtual	int32	Ex2ThreadFun	(void){return 1;};
		virtual	int32	DoAfterOpen		(void);
	protected:
				int32	MessageProcessing_Handshake	(FNode_MESG *RecMesg,int32 blReady);
		virtual	int32	MessageProcessing			(FNode_MESG *RecMesg,int32 blReady){return 0;};
		virtual void	ThreadsStart	(void);
		virtual	void	OnCloseDev		(void);
		virtual	void	CloseDev		(void);
		virtual	void	Handshake		(void);
		virtual void	DoAfterReadFromDevice(const uint8 *databuf,int32 num);
		virtual	void	ForwardToCouple	(const uint8 *databuf,int32 num){;};
	public:
				int32	SendREQ_CloseSocket(void);
		virtual	void	Clean	(void){APISocket::Clean();CSSL_T1D_CH0.Clean();CSSL_T1D_CH1.Clean();};
				void	Reset	(int32 blResetPuk = 1);
	public:
		inline	int32	CtrlCHRead(PUB_SBUF *retSBUFInfo){
			return(CSSL_T1D_CH0.RX_Packaging(retSBUFInfo));
		};
		inline	int32	CtrlCHWrite(const uint8 *data,uint32 num,uint32 mesgID){
			cgfnMesgTx.Clean();
			return(CSSL_T1D_CH0.TX_Packaging(cgfnMesgTx.SetContent(data,num,mesgID)));
		};
		inline	int32	CtrlCHWrite(const std::string &strData,uint32 mesgID){
			cgfnMesgTx.Clean();
			return(CSSL_T1D_CH0.TX_Packaging(cgfnMesgTx.SetContent(strData,mesgID)));
		};
		inline	int32	CtrlCHWrite(const Field_Node &fnNode,uint32 mesgID){
			cgfnMesgTx.Clean();
			return(CSSL_T1D_CH0.TX_Packaging(cgfnMesgTx.SetContent(fnNode,mesgID)));
		};
	public:
				int32	CheckHandshake		(void);
		inline	int32	SSLRead	(std::string *retStr)			{return(cgDataCHSBUF.GetInASCII(retStr,G_ESCAPE_OFF));};
		inline	int32	SSLWrite(const uint8 *data,uint32 num)	{return(CSSL_T1D_CH1.TX_Packaging(data,num));};
		inline	int32	SSLWrite(const std::string &strData)	{return(CSSL_T1D_CH1.TX_Packaging(strData));};
		inline	int32	SSLWrite(const FIFO_UINT8 &fifo,uint32 num,uint32 offset)
																{return(CSSL_T1D_CH1.TX_Packaging(fifo,num,offset));};
		inline	int32	SSLWrite(const Field_Node &fnNode)		{return(CSSL_T1D_CH1.TX_Packaging(fnNode));};
};
//------------------------------------------------------------------------------------------//
class SSLSocketServer : public APISocketServer{
	public:
		enum{RFLAG_C = 0, RFLAG_S = APISocketServer::RFLAG_S + APISocketServer::RFLAG_C};
				 SSLSocketServer(void) : APISocketServer(nullptr,G_MAXBUFFER_SIZE){selfName = "SSLSocketServer";};
		virtual ~SSLSocketServer(void){;};
	private:
		virtual APISocket	*CreateNewSocket_TCP(const ODEV_LIST *tODEV_LIST,uint32 tSize){
			SSLSocket *tPDB = new SSLSocket(tODEV_LIST,tSize);
			if (tPDB != nullptr)
				tPDB->SetSelfName("SSLS->SSLSocket" + Str_IntToString(GetnodeID(tPDB).load()));
			return(tPDB);
		};
	public:
		int32	RunAsTCP	(int32 port){return(APISocketServer::OpenD(port,COMMU_DBUF::CSType_TCP,0));};
		int32	RunAsUDP	(int32 port){return(APISocketServer::OpenD(port,COMMU_DBUF::CSType_UDP,0));};
};
//------------------------------------------------------------------------------------------//
#endif
//------------------------------------------------------------------------------------------//
#endif