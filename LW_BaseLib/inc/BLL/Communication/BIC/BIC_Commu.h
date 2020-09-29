//
//  BIC_Commu.h
//  SDT
//
//  Created by Leif Wen on 24/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "BIC_Define.h"
#include "Commu_Terminal.h"
#include "Commu_RST.h"
#include "MSG_RMS.h"
#if defined BIC_Define_h
//------------------------------------------------------------------------------------------//
#ifndef BIC_Commu_h
#define BIC_Commu_h
#ifdef BIC_Commu_h
class AEXEPOOL;
class NTPServer;
class COMMU_DRV;
#ifndef Commu_RST_h
typedef void	RSTBServer;
typedef void	RSTBSocket;
#endif
#ifndef Commu_Terminal_h
typedef void	TerminalServer;
#endif
//------------------------------------------------------------------------------------------//
namespace BIC_ENV_COMMU {
	enum	{CMD_VID_AEXEPOOL = BIC_ENV::CMD_VID_NEXT
			,CMD_VID_NTPS
			,CMD_VID_RSTSERVER
			,CMD_VID_RSTCLIENT
			,CMD_VID_TS
			,CMD_VID_NEXT
	};
	void				InitQuantity	(CMD_ENV* env);
	void				Init			(CMD_ENV* env);
	void				DeInit			(CMD_ENV* env);
	
	AEXEPOOL*			GetAExePool		(CMD_ENV* env);
	NTPServer*			GetNTPS			(CMD_ENV* env);
	RSTBServer*			GetRSTServer	(CMD_ENV* env);
	RSTBSocket*			GetRSTClient	(CMD_ENV* env);
	TerminalServer*		GetTS			(CMD_ENV* env);
	
	CMD_SETVAR(SetAExePool,		CMD_VID_AEXEPOOL,		AEXEPOOL*);
	CMD_SETVAR(SetNTPS,			CMD_VID_NTPS,			NTPServer*);
	CMD_SETVAR(SetRSTServer,	CMD_VID_RSTSERVER,		RSTBServer*);
	CMD_SETVAR(SetRSTClient,	CMD_VID_RSTCLIENT,		RSTBSocket*);
	CMD_SETVAR(SetTS,			CMD_VID_TS,				TerminalServer*);
};
//------------------------------------------------------------------------------------------//
class BIC_COMMU : public BIC_BASE{
	public:
				 BIC_COMMU(void) : BIC_BASE() {;};
		virtual ~BIC_COMMU(void){;};
	public:
				bool32	OnlineMode			(CMD_ENV* env,COMMU_DRV* cmmu,CMD_TAIL tail)const;
		virtual	bool32	OnlineModeExit		(CMD_ENV* env)const;
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif /* BIC_Commu_h */
#endif /* BIC_Commu_h */
#endif
