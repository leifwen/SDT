//
//  MSG_Center.h
//  SDT
//
//  Created by Leif Wen on 14/03/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "SYS_Thread.h"
#include "ADS_SSL.h"
#include "ADS_Buffer.h"
#include "CMD.h"
#if defined ADS_SSL_h && defined ADS_Buffer_h && defined CMD_h
//------------------------------------------------------------------------------------------//
#ifndef MSG_Center_h
#define MSG_Center_h
#ifdef MSG_Center_h
//------------------------------------------------------------------------------------------//
class MSG_BASE : public TNFP{
	public:
				 MSG_BASE(void);
		virtual	~MSG_BASE(void){;};
	protected:
		CMDID			cgMSGID;
	protected:
		virtual	CMDID		MessageProcessing	(CMD_ENV* env,const uint32& mID,const STDSTR& msg,void* commu);
	public:
		virtual	void		Reset				(void* commu);
		virtual	void		Close				(void* commu);
				CMDID		GetMSGID			(void);
};
//------------------------------------------------------------------------------------------//
class MSG_CENTER : public MSG_BASE{
	public:
		enum	{HandshakeTime = 5};	//8kbps = 1024 bytes per second
	protected:
		enum	{RFLAG_C = 4, RFLAG_S = MSG_BASE::RFLAG_S + MSG_BASE::RFLAG_C};
		enum	{MSGC_blEnableTxBlock = RFLAG_CREATE(0),MSGC_blEnableRxBlock = RFLAG_CREATE(1)
				,HS_blHandshakeY = RFLAG_CREATE(2),HS_blREQClose = RFLAG_CREATE(3)};
	public:
				 MSG_CENTER(void);
		virtual	~MSG_CENTER(void);
	protected:
		CSSL_T0						cgT0;
		CSSL_T1MSG					cgT1CtrlCH;
		CSSL_T1						cgT1DataCH;
		SBUF*						cgTxIn;
		SBUF*						cgRxOut;
		CMD_ENV*					cgENV;
	private:
		SYS_Thread<MSG_CENTER>		t0Thread;
		SYS_Thread<MSG_CENTER>		t1CtrlRxThread;
		SYS_Thread<MSG_CENTER>		t1DataRxThread;
		SYS_Thread<MSG_CENTER>		t1DataTxThread;
	private:
				bool32		T0ThreadFun			(void* commu);
				bool32		T1CtrlRxThreadFun	(void* commu);
				bool32		T1DataRxThreadFun	(void* commu);
				bool32		T1DataTxThreadFun	(void* commu);
	public:
				void		Init				(const ARRAY* _txout,const ARRAY* _rxin,const SBUF* _rxout,const SBUF* _txin);
				void		Init				(CMD_ENV* env,SYS_Thread_List* list);
		virtual	void		SetSelfName			(const STDSTR& strName);
		virtual	void		SetFatherName		(const STDSTR& strName);
	public:
				void		Reset				(void* commu);
				void		Empty				(void);
				void		SetConfig			(uint32 cfg,const STDSTR& sKey);
				void		SetKey				(			const STDSTR& sKey);
	public:
				void		TxBlock				(bool32 blBlock);
				void		RxBlock				(bool32 blBlock);
	public:
				void		SSLEnable			(void);
				void		SSLDisable			(void);
				void		SSLThreadRun		(void);
				void		ThreadRun			(void);
	public:
				void		SetblREQClose		(void);
				void		SetblREQHS			(void);
				bool32		ChkblREQClose		(void);
				bool32		ChkblREQHS			(void);
	public:
				void		Register			(MSG_BASE* msg);
				void		Delivery			(void);
				bool32		Send				(IOSTATUS* _ios,uint32  mID,const UVIn&  _in);
				uint32		Read				(IOSTATUS* _ios,uint32* mID,const UVOut& msg);
				bool32		Send				(IOSTATUS* _ios,const UVIn&  _in);
				uint32		Read				(IOSTATUS* _ios,const UVOut& _out);
};
//------------------------------------------------------------------------------------------//
class MSG_NODE : public MSG_BASE{
	public:
				 MSG_NODE(uint32 size,void* p);
		virtual	~MSG_NODE(void){;};
	protected:
		MSG_CENTER* 		cgMsgCenter;
	public:
		virtual	void		SetSelfName			(const STDSTR& strName);
		virtual	void		SetFatherName		(const STDSTR& strName);

				void		Join				(MSG_CENTER* msgCenter);
				void		SetConfig			(uint32 cfg,const STDSTR& sKey);
				void		SetKey				(			const STDSTR& sKey);
	public:
				bool32		Send				(uint32 mID,const UVIn& _in);
				bool32		Send				(CMD_ENV* env,uint32 mID,const UVIn& _in,const uint64& flagY,const uint64& flagN,const uint32& waitTimeS);
				bool32		Check				(CMD_ENV* env,							 const uint64& flagY,const uint64& flagN,SYS_TIME_S& timedly);
};
//------------------------------------------------------------------------------------------//
#endif /* MSG_Center_h */
#endif /* MSG_Center_h */
#endif /* ADS_SSL_h */
