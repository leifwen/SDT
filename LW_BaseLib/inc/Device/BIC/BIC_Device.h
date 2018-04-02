//
//  BIC_Device.h
//  SDT
//
//  Created by Leif Wen on 14/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "BIC_Dev.h"
#include "BIC_COM.h"
#include "BIC_Socket.h"
#include "BIC_GCM.h"
#include "Script.h"
#include "SendFile.h"
#ifdef BIC_Dev_h
//------------------------------------------------------------------------------------------//
#ifndef BIC_Device_h
#define BIC_Device_h
#ifdef BIC_Device_h
//------------------------------------------------------------------------------------------//
#define BIC_CLASSTYPE_PRESSKEY(_name,_base,_CMD) \
class BIC_##_name : public _base{\
	public:\
				 BIC_##_name(void) : _base (){cgCommandID = BIC_ID_##_name;cgCommand = _CMD;};\
		virtual ~BIC_##_name(void){;};\
	public:\
		virtual	bool32	InPressKeyModeExit		(CMD_ENV* env)const;\
		virtual	CMDID	Command					(CMD_ENV* env,const STDSTR& msg,void* p)const;\
		virtual	CMDID	Help					(CMD_ENV* env,uint32 flag)const;\
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#if defined BIC_GCM_h && defined Script_h
BIC_CLASSTYPE_PRESSKEY	(GC_GROUP_SEND,GC_GROUP_BASE,	"send");
#endif
#ifdef SList_h
BIC_CLASSTYPE			(SC_SEND,			"send");
#endif
BIC_CLASSTYPE			(SEND,				"send");
BIC_CLASSTYPE			(STOP,				"stop");
//------------------------------------------------------------------------------------------//
#ifdef SendFile_h
BIC_CLASSTYPE_PRESSKEY	(SENDFILE,BIC_BASE,	"sendfile");
#endif
//------------------------------------------------------------------------------------------//
#ifdef Script_h
BIC_CLASSTYPE_PRESSKEY	(SENDA,	BIC_BASE,	"senda");
BIC_CLASSTYPE_PRESSKEY	(RUN,	BIC_BASE,	"run");
BIC_CLASSTYPE			(SCRIPT,			"script");
#endif
//------------------------------------------------------------------------------------------//
class BIC_CONN : public BIC_BASE_S{
	public:
				 BIC_CONN(void);
		virtual ~BIC_CONN(void){;};
	public:
		virtual	CMDID	MessageProcessing	(CMD_ENV* env,const uint32& mID,const STDSTR& msg,void* p)const;
	private:
		BIC_MAIN		cgC_MAIN;
#ifdef SWVERSION_AUXDEVICE
		BIC_AUX			cgC_AUX;
#endif
		BIC_CONNECT		cgC_CONNECT;
		BIC_DISCONNECT	cgC_DISCONNECT;
		BIC_ONLINE		cgC_ONLINE;
		BIC_CR			cgC_CR;
		
		BIC_ECHO		cgC_ECHO;
		BIC_REC			cgC_REC;
		BIC_DEV			cgC_DEV;
#ifdef SWVERSION_COM
		BIC_ACOM		cgC_ACOM;
#endif
#if (defined SWVERSION_PORT) || (defined SWVERSION_Server)
		BIC_ASOCKET		cgC_ASOCKET;
#endif
		BIC_SEND		cgC_SEND;
#if defined SendFile_h || defined SWVERSION_SCRIPT
		BIC_STOP		cgC_STOP;
#endif
#ifdef SendFile_h
		BIC_SENDFILE	cgC_SENDFILE;
#endif
#ifdef SWVERSION_SCRIPT
		BIC_SENDA		cgC_SENDA;
		BIC_RUN			cgC_RUN;
		BIC_SCRIPT		cgC_SCRIPT;
#endif
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif /* BIC_Device_h */
#endif /* BIC_Device_h */
#endif /* BIC_Dev_h */
