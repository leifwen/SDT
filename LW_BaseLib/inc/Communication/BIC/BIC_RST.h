//
//  BIC_RST.h
//  SDT
//
//  Created by Leif Wen on 24/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "BIC_Commu.h"
#include "Commu_RST.h"
#if defined BIC_Commu_h && defined Commu_RST_h
//------------------------------------------------------------------------------------------//
#ifndef BIC_RST_h
#define BIC_RST_h
#ifdef BIC_RST_h
//------------------------------------------------------------------------------------------//
BIC_CLASSTYPE	(RST_ON,		"on");
BIC_CLASSTYPE	(RST_OFF,		"off");
BIC_CLASSTYPE	(RST_LINK,		"link");
BIC_CLASSTYPE	(RST_MAPPING,	"map");
//------------------------------------------------------------------------------------------//
class BIC_RST_LOGIN : public BIC_COMMU{
	public:
				 BIC_RST_LOGIN(void) : BIC_COMMU() {cgCommandID = BIC_ID_RST_LOGIN;cgCommand = "login";};
		virtual ~BIC_RST_LOGIN(void){;};
	public:
		virtual	CMDID	Command				(CMD_ENV* env,const STDSTR& msg,void* p)const;
		virtual	CMDID	Help				(CMD_ENV* env,uint32 flag)const;
	public:
		virtual	bool32	OnlineModeExit		(CMD_ENV* env)const;
};
//------------------------------------------------------------------------------------------//
BIC_CLASSTYPE	(RST_TS,		"ts");
BIC_CLASSTYPE	(RST_APPROVE,	"approve");
//------------------------------------------------------------------------------------------//
class BIC_RST : public BIC_BASE_S{
	public:
				 BIC_RST(void);
		virtual ~BIC_RST(void){;};
	public:
		BIC_RST_ON				cgSub_on;
		BIC_RST_OFF				cgSub_off;
		BIC_RST_LINK			cgSub_link;
		BIC_RST_MAPPING			cgSub_mapping;
		BIC_RST_LOGIN			cgSub_login;
		BIC_RST_TS				cgSub_ts;
#if defined MSG_Register_h && defined CommonDefH_MAC
		BIC_RST_APPROVE			cgSub_approve;
#endif
};
//------------------------------------------------------------------------------------------//
#endif /* BIC_RST_h */
#endif /* BIC_RST_h */
#endif
