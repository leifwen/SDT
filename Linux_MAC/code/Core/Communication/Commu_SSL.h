/*
 * Copyright (c) 2012-2014 
 * All rights reserved.
 *
 * File Name: Commu_SSL.h
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2015.10.26
*/

//------------------------------------------------------------------------------------------//
#include "SYS_Thread.h"
#include "Comm_PA.h"
#include "Comm_Buffer.h"
#include "Comm_Crypto.h"
//------------------------------------------------------------------------------------------//
#if defined USE_OPENSSL && defined Comm_PAH && defined Comm_BufferH
#ifndef Commu_SSLH
#define Commu_SSLH
#ifdef Commu_SSLH
//------------------------------------------------------------------------------------------//
class CSSL_FN_T0 :public PROTOCOL_NODE{
	public:
		enum{MAX_PACKAGE_DEALYTM = 1024 * 4};
				 CSSL_FN_T0(void) : PROTOCOL_NODE(){Add(pn_Head) < pn_CH < pn_AESCC;};
		virtual	~CSSL_FN_T0(void){;};
	private:
		PROTOCOL_NODE	pn_Head;
		PROTOCOL_NODE	pn_CH;
		CCY_FN_AES		pn_AESCC;
	
		STDSTR		cgKey;
		double		cgPackageDlyMS;
	private:
		virtual int32	ChecksumResult	(const FIFO8 &fifo)const{
			STDSTR	retStrOriginal;
			return(pn_AESCC.CheckContent(&retStrOriginal, cgKey, &fifo));
		};
	public:
				void	SetPackageDlyMS(double dlyMS);
				double	GetPackageDlyMS(void);
		inline  uint32	GetInvalidLength(void)const{return(6 + pn_AESCC.GetInvalidLength() + 1);};
	public:
		void	Init(const FIFO8 *tDefFifo,CCT_AES_KEYL type = CCT_AES128,CCT_AES_MODE mode = CCT_AES_CBC,CCT_Digest digestType = CCT_MD5,G_ENDIAN tEV = G_ENDIAN_LITTLE){
			PROTOCOL_NODE::Init(FNT_MESSAGE,tDefFifo,tEV);
			pn_Head.Init	(FNT_HEAD	,tDefFifo,6,tEV);	pn_Head.SetFixValue("LF.W.T");
			pn_CH.Init		(FNT_CTRL	,tDefFifo,1,tEV);
			pn_AESCC.Init	(			 tDefFifo,type,mode,digestType,tEV);
			ResetKey();
			cgPackageDlyMS = MAX_PACKAGE_DEALYTM;
		};
		void	SetConfig(CCT_AES_KEYL type = CCT_AES128,CCT_AES_MODE mode = CCT_AES_CBC,CCT_Digest digestType = CCT_MD5,G_ENDIAN tEV = G_ENDIAN_LITTLE){
			SetEndianType(tEV);
			pn_Head.SetEndianType(tEV);
			pn_CH.SetEndianType(tEV);
			pn_AESCC.SetConfig(type,mode,digestType,tEV);
		};
		inline	void	ResetKey	(void)					{Spin_InUse_set();cgKey = CCY_AESKey32Bye("LF.W.TZ");Spin_InUse_clr();};
		inline	void	SetKey		(const STDSTR &skey)	{Spin_InUse_set();cgKey = skey;Spin_InUse_clr();};
	public:
				void	RxDelivery	(TREE_NODE *retFNList);//one in one out mode
		inline	const	CSSL_FN_T0&	SetContent	(const PROTOCOL_NODE &pnIn,uint32 channel){
			Spin_InUse_set();
			PROTOCOL_NODE::_Start();
			pn_Head.SetFIFOFixValue();
			pn_CH.SetFIFOByte(channel);
			pn_AESCC.SetContent(pnIn, cgKey);
			PROTOCOL_NODE::_Endl();
			Spin_InUse_clr();
			return(*this);
		};
		inline	const	CSSL_FN_T0&	SetContent	(const FIFO8 &fifoIn,uint32 num,uint32 offset,uint32 channel){//one in one out mode,
			Spin_InUse_set();
			PROTOCOL_NODE::_Start();
			pn_Head.SetFIFOFixValue();
			pn_CH.SetFIFOByte(channel);
			pn_AESCC.SetContent(fifoIn, num, offset, cgKey);
			PROTOCOL_NODE::_Endl();
			Spin_InUse_clr();
			return(*this);
		};
		inline	const	CSSL_FN_T0&	SetContent	(const uint8 *data,uint32 num,uint32 channel){
			Spin_InUse_set();
			PROTOCOL_NODE::_Start();
			pn_Head.SetFIFOFixValue();
			pn_CH.SetFIFOByte(channel);
			pn_AESCC.SetContent(data, num, cgKey);
			PROTOCOL_NODE::_Endl();
			Spin_InUse_clr();
			return(*this);
		};
		inline	const	CSSL_FN_T0&	SetContent	(const STDSTR &strIn,uint32 channel){
			return(CSSL_FN_T0::SetContent((uint8*)strIn.c_str(),(uint32)strIn.length(),channel));
		};
};
//------------------------------------------------------------------------------------------//
class CSSL_FR_T1 :public PROTOCOL_NODE{
	public:
		enum{MAX_PACKAGE_SIZE = 1024 / 2};
	public:
				 CSSL_FR_T1(void) : PROTOCOL_NODE(){Add(pn_GroupID) < pn_CtrlID < pn_Offset < pn_Info;};
		virtual	~CSSL_FR_T1(void){;};
	private:
		PROTOCOL_NODE	pn_GroupID;
		PROTOCOL_NODE	pn_CtrlID;
		PROTOCOL_NODE	pn_Offset;
		PNODE_LC		pn_Info;
	
		uint32		cgCH;
		uint32		cgGroupID;
		uint32		cgLastOffset;
	
		FIFO8		cgRxFIFO;
		FIFO8		cgTxFIFO;
		double		cgPackageDlyMS;
	public:
		void		Init			(uint32 tCH,uint32 rxfifoSize,G_ENDIAN tEV = G_ENDIAN_LITTLE);
		void		SetConfig		(G_ENDIAN tEV);
	
		inline void	Clean(void)		{cgRxFIFO.Empty();cgTxFIFO.Empty();};
		inline void	Reset(void)		{cgGroupID = 0;};
	
		inline const	uint32		&GetcgCH			(void)const	{return(cgCH);};
		inline		 	uint32		GetInvalidLength	(void)const	{return(2 + 1 + 2 + 2);};
	private:
		const	CSSL_FR_T1&	SetContent		(const uint32 &tGroupID,const uint32 &tCtrlID,const uint32 &tOffset,const uint8 *data,uint32 num);
		const	CSSL_FR_T1&	SetContent		(const uint32 &tGroupID,const uint32 &tCtrlID,const uint32 &tOffset,const STDSTR &strIn);
	
		void	ANS_ACK						(CSSL_FN_T0 *sslT0Tx,const uint32 &tGroupID,const uint32 &offset);
		void	ANS_ENDACK					(CSSL_FN_T0 *sslT0Tx,const uint32 &tGroupID);
		void	ANS_Request_ResendPackage	(CSSL_FN_T0 *sslT0Tx,const uint32 &tGroupID,const uint32 &offset);
		void	ANS_Request_ResendAll		(CSSL_FN_T0 *sslT0Tx,const uint32 &tGroupID);
		uint32	TX_Data (CSSL_FN_T0 *sslT0Tx,const uint8 *&data,const uint32 &num,const uint32 &offset,const uint32 &packageSize,const uint32 &tGroupID);
		int32	RX_Data (CSSL_FN_T0 *sslT0Tx,uint32 *recNum,uint32 *tGroupID,uint32 *lastOffset,FIFO8 *retFifo,uint32 *retGroupID,uint32 *retCtrlID,uint32 *retOffset);
		int32	CHK_Data					(uint32 *retGroupID,uint32 *retCtrlID,uint32 *retOffset);
	public:
		void	ReceiveT0Data	(CCY_FN_AES *fnAESofT0,const STDSTR &strKey);
		uint32	RX_Packaging	(CSSL_FN_T0 *sslT0Tx,FIFO8 *retFifo);
		uint32	TX_Packaging	(CSSL_FN_T0 *sslT0Tx,const PROTOCOL_NODE &pnNode);
		uint32	TX_Packaging	(CSSL_FN_T0 *sslT0Tx,const FIFO8 &fifo,uint32 num,uint32 offset);
		uint32	TX_Packaging	(CSSL_FN_T0 *sslT0Tx,const uint8 *data,uint32 num);
		uint32	TX_Packaging	(CSSL_FN_T0 *sslT0Tx,const STDSTR &strIn)	{return(TX_Packaging(sslT0Tx,(uint8 *)strIn.c_str(),(uint32)strIn.length()));};
};
//------------------------------------------------------------------------------------------//
class CSSL_FR_T1D : public TREE_NODE{
	public:
				 CSSL_FR_T1D(void) : TREE_NODE(){;};
		virtual ~CSSL_FR_T1D(void){;};
	public:
		void	Init(uint32 tCH,const CSSL_FN_T0 *fnT0tx,TREE_NODE *fwPNList,uint32 rxfifoSize,G_ENDIAN tEV = G_ENDIAN_LITTLE){
			tCH <<= 1;
			pn_sslT0_TX = (CSSL_FN_T0*)fnT0tx;
			pn_sslT1_RXCH.Init(tCH,rxfifoSize,tEV);
			pn_sslT1_TXCH.Init(tCH + 1,CSSL_FR_T1::MAX_PACKAGE_SIZE << 2,tEV);
			SetSelfName("");
			*fwPNList < pn_sslT1_RXCH < pn_sslT1_TXCH;
		}
		void	SetConfig(G_ENDIAN tEV){pn_sslT1_RXCH.SetConfig(tEV);pn_sslT1_TXCH.SetConfig(tEV);};
	private:
		CSSL_FN_T0		*pn_sslT0_TX;
		CSSL_FR_T1		pn_sslT1_RXCH;
		CSSL_FR_T1		pn_sslT1_TXCH;
	public:
		inline	virtual	void	SetSelfName(const STDSTR &strName){
			selfName = strName;
			pn_sslT1_RXCH.selfName = selfName + "::RxCH" + Str_ToString(pn_sslT1_RXCH.GetcgCH());
			pn_sslT1_TXCH.selfName = selfName + "::TxCH" + Str_ToString(pn_sslT1_TXCH.GetcgCH());
		}
		inline	void	Clean		(void){pn_sslT1_RXCH.Clean();pn_sslT1_TXCH.Clean();};
		inline	void	Reset		(void){pn_sslT1_RXCH.Reset();pn_sslT1_TXCH.Reset();};
		inline	uint32	RX_Packaging(FIFO8 *retFifo)								{return(pn_sslT1_RXCH.RX_Packaging(pn_sslT0_TX,retFifo));};
		inline	uint32	TX_Packaging(const PROTOCOL_NODE &pnNode)					{return(pn_sslT1_TXCH.TX_Packaging(pn_sslT0_TX,pnNode));};
		inline	uint32	TX_Packaging(const FIFO8 &fifo,uint32 num,uint32 offset)	{return(pn_sslT1_TXCH.TX_Packaging(pn_sslT0_TX,fifo,num,offset));};
		inline	uint32	TX_Packaging(const uint8 *data,uint32 num)					{return(pn_sslT1_TXCH.TX_Packaging(pn_sslT0_TX,data,num));};
		inline	uint32	TX_Packaging(const STDSTR &strData)							{return(pn_sslT1_TXCH.TX_Packaging(pn_sslT0_TX,strData));};
};
//------------------------------------------------------------------------------------------//
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
	MESG_NEXT,
};
class COMMU_DBUF_SSL;
//------------------------------------------------------------------------------------------//
class CSSL_FR_T2 : public TREE_NODE{
	public:
		enum{RFLAG_C = 4, RFLAG_S = TREE_NODE::RFLAG_S + TREE_NODE::RFLAG_C};
		enum{HandshakeTime = 20};	//8kbps = 1024 bytes per s
	private:
		enum{
			blHandshakeY = RFLAG_CREATE(0),
			blHandshakeN = RFLAG_CREATE(1),
			blClose		= RFLAG_CREATE(2),
			blNoBlock	= RFLAG_CREATE(3),
		};
	public:
				 CSSL_FR_T2(void) : TREE_NODE(){SetSelfName("CSSL_FR_T2");};
		virtual ~CSSL_FR_T2(void){ctrlCHThread.RemoveSelf();dataCHRxThread.RemoveSelf();dataCHTxThread.RemoveSelf();FreeRSA_Prk();};
	public:
		void	Init	(COMMU_DBUF_SSL *tCOMMU,const SBUFFER *rxSBUF,const SBUFFER *txSBUF
						 ,CCT_AES_KEYL type = CCT_AES128,CCT_AES_MODE mode = CCT_AES_CBC,CCT_Digest digestType = CCT_MD5,G_ENDIAN tEV = G_ENDIAN_LITTLE);
		void	SetConfig(CCT_AES_KEYL type = CCT_AES128,CCT_AES_MODE mode = CCT_AES_CBC,CCT_Digest digestType = CCT_MD5,G_ENDIAN tEV = G_ENDIAN_LITTLE);
		void	Reset	(int32 blResetPuk = 1);
	
		inline			void	Clean		(void){CSSL_T1D_CtrlCH.Clean();CSSL_T1D_DataCH.Clean();};
		inline	virtual	void	SetSelfName	(const STDSTR &strName){
			selfName = strName;
			pn_sslT0_Tx.selfName	= strName + "::T0_Tx";
			pn_sslT0_Rx.selfName	= strName + "::T0_Rx";
			CSSL_T1D_DataCH.SetSelfName(strName + "::T1D_DCH");
			CSSL_T1D_CtrlCH.SetSelfName(strName + "::T1D_CCH");
		};
	private:
		SYS_Lock		cgSendCHMesgLock;
		STDSTR			cgKey;
		CCT_AES_KEYL	cgAES_type;
		CCT_AES_MODE	cgAES_mode;
		CCT_Digest 		cgDigestType;
		G_ENDIAN		cgEndianType;
	private:
		FIFO8			*cgT0TxFifo;
		FIFO8			cgT0RxFifo;
		CSSL_FN_T0		pn_sslT0_Rx;
		CSSL_FN_T0		pn_sslT0_Tx;
		TREE_NODE		fwPNList;
	private:
		SBUFFER			cgDataInSBUF;//used for no block mode
		SBUFFER			*cgDataOutSBUF;
		FIFO8			cgCtrlOutFifo;
		CSSL_FR_T1D		CSSL_T1D_CtrlCH;
		CSSL_FR_T1D		CSSL_T1D_DataCH;
		FIFO8			cgfnMesgTxFIFO;
		PNODE_MESG8		cgfnMesgTx;
		PNODE_MESG8		cgfnMesgRx;
	private:
		CCY_FR_Signature	cgfnSignature;
	private:
		SYS_Thread<CSSL_FR_T2>		ctrlCHThread;
		SYS_Thread<CSSL_FR_T2>		dataCHRxThread;
		SYS_Thread<CSSL_FR_T2>		dataCHTxThread;
	private:
		int32	CtrlCHThreadFun(void *p);
		int32	DataCHRxThreadFun(void *p);
		int32	DataCHTxThreadFun(void *p);
	private:
		RSA				*cgRSA_Prk;
		RSA				*cgRSA_Puk;
	private:
				void	FreeRSA_Prk	(void){try{RSA_free(cgRSA_Prk);RSA_free(cgRSA_Puk);}catch(...){;};};
		virtual	int32	SetRSA_Prk	(void){return(CCY_RSA_CreateKey(cgRSA_Prk,2048));};
		inline	void	SetKey		(const STDSTR &skey){cgKey = skey;pn_sslT0_Rx.SetKey(skey);pn_sslT0_Tx.SetKey(skey);};
	public:
		inline int32	ChkblClose	(void){return(CheckSFlag(blClose));};
		inline void		SetNoBlock	(void){dataCHTxThread.Enable();SetSFlag(blNoBlock);};
		inline void		SetBlock	(void){dataCHTxThread.Disable();dataCHTxThread.ThreadStop();ClrSFlag(blNoBlock);};
	public:
		void	SSLEnable			(COMMU_DBUF_SSL *tCOMMU);
		void	SSLDisable			(void);
		void	SSLThreadRun		(void);
	public:
		FIFO8	*GetT0RxFifo		(void){return(&cgT0RxFifo);};
		void	AfterReadFromDevice	(void){pn_sslT0_Rx.RxDelivery(&fwPNList);};
	public:
		int32	MessageProcessing	(COMMU_DBUF_SSL	*tCOMMU,const uint32 &mID,const STDSTR &strMesg);
		int32	Handshake			(COMMU_DBUF_SSL	*tCOMMU);
	private:
		inline	uint32	CtrlCHRead(STDSTR *retStrMesg,uint32 *retMID){
			*retStrMesg = "";
			*retMID = MESG_NONE;
			cgfnMesgRx.Spin_InUse_set();
			CSSL_T1D_CtrlCH.RX_Packaging(&cgCtrlOutFifo);
			if (cgfnMesgRx.TryGetFrame() > 0){
				cgfnMesgRx.ReadContent(retStrMesg,retMID);
				cgfnMesgRx.Out();
			}
			cgfnMesgRx.Spin_InUse_clr();
			return(*retMID);
		};
		inline	uint32	DataCHRead (FIFO8 *retFifo){
			return(CSSL_T1D_DataCH.RX_Packaging(retFifo));
		};
	public:
		inline	uint32	CtrlCHWrite(uint32 mesgID,const PROTOCOL_NODE &pnNode){
			int32 ret;
			cgfnMesgTx.Spin_InUse_set();
			cgfnMesgTx.Clean();
			ret = CSSL_T1D_CtrlCH.TX_Packaging(cgfnMesgTx.SetContent(mesgID,pnNode));
			cgfnMesgTx.Spin_InUse_clr();
			return(ret);
		};
		inline	uint32	CtrlCHWrite(uint32 mesgID,const FIFO8 &fifoIn,uint32 num,uint32 offset){
			int32 ret;
			cgfnMesgTx.Spin_InUse_set();
			cgfnMesgTx.Clean();
			ret = CSSL_T1D_CtrlCH.TX_Packaging(cgfnMesgTx.SetContent(mesgID,fifoIn,num,offset));
			cgfnMesgTx.Spin_InUse_clr();
			return(ret);
		};
		inline	uint32	CtrlCHWrite(uint32 mesgID,const uint8 *data,uint32 num){
			int32 ret;
			cgfnMesgTx.Spin_InUse_set();
			cgfnMesgTx.Clean();
			ret = CSSL_T1D_CtrlCH.TX_Packaging(cgfnMesgTx.SetContent(mesgID,data,num));
			cgfnMesgTx.Spin_InUse_clr();
			return(ret);
		};
		inline	uint32	CtrlCHWrite(uint32 mesgID,const STDSTR &strData){
			int32 ret;
			cgfnMesgTx.Spin_InUse_set();
			cgfnMesgTx.Clean();
			ret = CSSL_T1D_CtrlCH.TX_Packaging(cgfnMesgTx.SetContent(mesgID,strData));
			cgfnMesgTx.Spin_InUse_clr();
			return(ret);
		};
	public:
		inline	uint32	DataCHWrite(const PROTOCOL_NODE &pnNode){
			return((CheckSFlag(blNoBlock) == 0)?CSSL_T1D_DataCH.TX_Packaging(pnNode)		:cgDataInSBUF.Put(pnNode,G_LOCK_ON));
		};
		inline	uint32	DataCHWrite(const FIFO8 &fifo,uint32 num,uint32 offset){
			return((CheckSFlag(blNoBlock) == 0)?CSSL_T1D_DataCH.TX_Packaging(fifo,num,offset):cgDataInSBUF.Put(fifo,num,offset,G_LOCK_ON));
		};
		inline	uint32	DataCHWrite(const uint8 *data,uint32 num){
			return((CheckSFlag(blNoBlock) == 0)?CSSL_T1D_DataCH.TX_Packaging(data,num)		:cgDataInSBUF.Put(data,num,G_LOCK_ON));
		};
		inline	uint32	DataCHWrite(const uint8 data){
			return((CheckSFlag(blNoBlock) == 0)?CSSL_T1D_DataCH.TX_Packaging(&data,1)		:cgDataInSBUF.Put(data,G_LOCK_ON));
		};
		inline	uint32	DataCHWrite(const STDSTR &strData){
			return((CheckSFlag(blNoBlock) == 0)?CSSL_T1D_DataCH.TX_Packaging(strData)		:cgDataInSBUF.Put(strData,G_ESCAPE_OFF,G_LOCK_ON));
		};
		inline	uint32	DataCHWriteInHex(const STDSTR &strData){
			return((CheckSFlag(blNoBlock) == 0)?CSSL_T1D_DataCH.TX_Packaging(strData)		:cgDataInSBUF.PutInHEX(strData,G_LOCK_ON));
		};
};
//------------------------------------------------------------------------------------------//
#endif
#endif
#endif
