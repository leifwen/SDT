//
//  MSG_Center.h
//  SDT
//
//  Created by Leif Wen on 14/03/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "SYS_Thread.h"
#include "ADS_SSL.h"
#include "CMD.h"
#include "SYS_Time.h"
#if defined ADS_SSL_h && defined CMD_h && defined SYS_Thread_h
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
		CMDID				cgMSGID;
	protected:
		virtual	CMDID		MessageProcessing	(CMD_ENV* env,const uint32& mID,const STDSTR& msg,void* _team);
	public:
		virtual	void		Init				(uint32 rxSize,uint32 txSize);
		virtual	void		Reset				(void* _team);
		virtual	void		Close				(void* _team);
				CMDID		GetMSGID			(void);
};
//------------------------------------------------------------------------------------------//
class MSG_CENTER : public MSG_BASE{
	public:
		enum	{HandshakeTime = 5};	//8kbps = 1024 bytes per second
	protected:
		enum	{RFLAG_C = 2, RFLAG_S = MSG_BASE::RFLAG_S + MSG_BASE::RFLAG_C};
		enum	{HS_blHandshakeY = RFLAG_CREATE(0),HS_blREQClose = RFLAG_CREATE(1)};
	public:
				 MSG_CENTER(void);
		virtual	~MSG_CENTER(void);
	protected:
		CSSL_T1MSG					cgT1CtrlCH;
		CSSL_DEV*					cgSSLDev;
		CMD_ENV*					cgENV;
		void*						cgTeam;
	private:
		SYS_Thread<MSG_CENTER>		t1CtrlRxThread;
				bool32				T1CtrlRxThreadFun	(void* _team);
	public:
				void		Init				(void* _team,CMD_ENV* env);
				void		Init				(const CSSL_DEV* sslDev,uint32 maxPackage,uint32 maxPages);
		virtual	void		Init				(uint32 rxSize,uint32 txSize);
				void		SetDelayPar			(uint32 maxDlyMS,const uint32 maxTimes);
		virtual	void		SetSelfName			(const STDSTR& strName);
		virtual	void		SetUpName			(const STDSTR& strName);
	public:
				void		Reset				(void* _team);
				void		Close				(void* _team);
				void		Empty				(void);
				void		SetConfig			(uint32 cfg,const STDSTR& sKey);
				void		SetKey				(			const STDSTR& sKey);
				CSSL_DEV*	GetSSLDev			(void);
	public:
				void		SetblREQClose		(void);
				void		SetblREQHS			(void);
				bool32		ChkblREQClose		(void);
				bool32		ChkblREQHS			(void);
	public:
				void		Register			(MSG_BASE* msg);
				bool32		Send				(IOS* _ios,uint32  mID,const UVIn&  _in);
};
//------------------------------------------------------------------------------------------//
class MSG_NODE : public MSG_BASE{
	public:
				 MSG_NODE(void);
		virtual	~MSG_NODE(void){;};
	protected:
		MSG_CENTER* 		cgMsgCenter;
	public:
		virtual	void		SetSelfName			(const STDSTR& strName);
		virtual	void		SetUpName			(const STDSTR& strName);

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
