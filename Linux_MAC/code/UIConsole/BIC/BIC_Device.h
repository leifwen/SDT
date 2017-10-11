/*
 * Copyright (c) 2012-2014 
 * All rights reserved.
 *
 * File Name: BIC_Device.h
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2014.01.25
*/

//------------------------------------------------------------------------------------------//
#include "BIC_B.h"
//------------------------------------------------------------------------------------------//
#ifdef BIC_BH
#ifndef BIC_DeviceH
#define BIC_DeviceH
#ifdef BIC_DeviceH
//------------------------------------------------------------------------------------------//
STDSTR GetFullName(void);
//------------------------------------------------------------------------------------------//
class BIC_CONNECT : public BIC_Node{
	public:
				 BIC_CONNECT(void) : BIC_Node() {cgCommand = "c/connect";cgReturnCode = BI_RETCODE_CONNECT;};
		virtual ~BIC_CONNECT(void){;};
	public:
		virtual	int32	Command (BIC_ENV *env,const STDSTR &par,void *eda)const;
		virtual	int32	Help	(BIC_ENV *env,int32 blDetail = 1)const{
			PrintHelpItem(env,cgCommand,"Open" + GetFullName());
			if (blDetail == 0)
				return(cgReturnCode);
			PrintHelpSubItem(env,"[none|r|n|rn]","Send with tail none,\\r,\\n,\\r\\n");
			PrintHelpItem(env,"","Goto online mode after connected.");
			PrintHelpItem(env,"","Double press ESC + ESC to exit online mode.");
			PrintHelpItem(env,"","Double press ESC + A~Z to send 01~1A.");
			PrintHelpItem(env,"","Double press ESC + [ to send 1B.");
			PrintHelpItem(env,"","Double press ESC + \\ to send 1C.");
			PrintHelpItem(env,"","Double press ESC + ] to send 1D.");
			return(cgReturnCode);
		};
	public:
		static	int32	OnlineMode	(BIC_ENV *env,ExpandDeviceAttr *attr,CMD_TAIL tail);
};
//------------------------------------------------------------------------------------------//
class BIC_ONLINE : public BIC_CONNECT{
	public:
				 BIC_ONLINE(void) : BIC_CONNECT() {cgCommand = "o/online";cgReturnCode = BI_RETCODE_ONLINE;};
		virtual ~BIC_ONLINE(void){;};
	public:
		virtual	int32	Command (BIC_ENV *env,const STDSTR &par,void *eda)const;
};
//------------------------------------------------------------------------------------------//
class BIC_DISCONNECT : public BIC_Node{
	public:
				 BIC_DISCONNECT(void) : BIC_Node() {cgCommand = "d/disconnect";cgReturnCode = BI_RETCODE_DISCONNECT;};
		virtual ~BIC_DISCONNECT(void){;};
	public:
		virtual	int32	Command (BIC_ENV *env,const STDSTR &par,void *eda)const;
		virtual	int32	Help	(BIC_ENV *env,int32 blDetail = 1)const{
			PrintHelpItem(env,cgCommand,"Close" + GetFullName());
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class BIC_CR : public BIC_Node{
	public:
				 BIC_CR(void) : BIC_Node() {cgCommand = "cr";cgReturnCode = BI_RETCODE_CR;};
		virtual ~BIC_CR(void){;};
	public:
		virtual	int32	Command (BIC_ENV *env,const STDSTR &par,void *eda)const;
		virtual	int32	Help	(BIC_ENV *env,int32 blDetail = 1)const{
			PrintHelpItem(env,cgCommand,"Connection status report.");
			if (blDetail == 0)
				return(cgReturnCode);
			PrintHelpSubItem(env,"[-c]","Clear report.");
			return(cgReturnCode);
		};
};
//-------------------------------------- ----------------------------------------------------//
class BIC_ECHO : public BIC_Node{
	public:
				 BIC_ECHO(void) : BIC_Node() {cgCommand = "echo";cgReturnCode = BI_RETCODE_ECHO;};
		virtual ~BIC_ECHO(void){;};
	public:
		virtual	int32	Command (BIC_ENV *env,const STDSTR &par,void *eda)const;
		virtual	int32	Help	(BIC_ENV *env,int32 blDetail = 1)const{
			PrintHelpItem(env,cgCommand,"Enable/disable echo.");
			if (blDetail == 0)
				return(cgReturnCode);
			PrintHelpSubItem(env,"[on|off]","");
			return(cgReturnCode);
	};
};
//------------------------------------------------------------------------------------------//
class BIC_REC : public BIC_Node{
	public:
				 BIC_REC(void) : BIC_Node() {cgCommand = "rec";cgReturnCode = BI_RETCODE_REC;};
		virtual ~BIC_REC(void){;};
	public:
		virtual	int32	Command (BIC_ENV *env,const STDSTR &par,void *eda)const;
		virtual	int32	Help	(BIC_ENV *env,int32 blDetail = 1)const{
			PrintHelpItem(env,cgCommand,"Set receive data report parameter.");
			if (blDetail == 0)
				return(cgReturnCode);
			PrintHelpSubItem(env,"[-t<on|off>]"		,"Enable/disable receive tip.");
			PrintHelpSubItem(env,"[-H|-A]"			,"Receive mode: Hex/ASCII.");
			PrintHelpSubItem(env,"[-OSP<on|off>]"	,"Enable/disable OSP message delimiter.");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class BIC_DEV : public BIC_Node{
	public:
				 BIC_DEV(void) : BIC_Node() {cgCommand = "dev";cgReturnCode = BI_RETCODE_DEV;};
		virtual ~BIC_DEV(void){;};
	public:
		virtual	int32	Command (BIC_ENV *env,const STDSTR &par,void *eda)const;
		virtual	int32	Help	(BIC_ENV *env,int32 blDetail = 1)const{
			PrintHelpItem(env,cgCommand,"List" + GetFullName());
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class BIC_SEND : public BIC_Node{
	public:
				 BIC_SEND(void) : BIC_Node() {cgCommand = "send";cgReturnCode = BI_RETCODE_SEND;};
		virtual ~BIC_SEND(void){;};
	public:
		virtual	int32	Command (BIC_ENV *env,const STDSTR &par,void *eda)const;
		virtual	int32	Help	(BIC_ENV *env,int32 blDetail = 1)const{
			PrintHelpItem(env,cgCommand,"Send data if connected.");
			if (blDetail == 0)
				return(cgReturnCode);
			PrintHelpSubItem(env,"<data>","Data.");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class BIC_SENDA : public BIC_Node{
	public:
				 BIC_SENDA(void) : BIC_Node() {cgCommand = "senda";cgReturnCode = BI_RETCODE_SENDA;};
		virtual ~BIC_SENDA(void){;};
	public:
		virtual	int32	Command (BIC_ENV *env,const STDSTR &par,void *eda)const;
		virtual	int32	Help	(BIC_ENV *env,int32 blDetail = 1)const{
			PrintHelpItem(env,cgCommand,"Send data automatically if connected.");
			if (blDetail == 0)
				return(cgReturnCode);
			PrintHelpSubItem(env,"<s>","Time interval.");
			PrintHelpSubItem(env,"<data>","Data.");
			return(cgReturnCode);
		};
	public:
		virtual	int32	InPressKeyModeExit	(BIC_ENV *env)const;
};
//------------------------------------------------------------------------------------------//
class BIC_SENDFILE : public BIC_Node{
	public:
				 BIC_SENDFILE(void) : BIC_Node() {cgCommand = "sendfile";cgReturnCode = BI_RETCODE_SENDFILE;};
		virtual ~BIC_SENDFILE(void){;};
	public:
		virtual	int32	Command (BIC_ENV *env,const STDSTR &par,void *eda)const;
		virtual	int32	Help	(BIC_ENV *env,int32 blDetail = 1)const{
			PrintHelpItem(env,cgCommand,"Send file if connected.");
			if (blDetail == 0)
				return(cgReturnCode);
			PrintHelpSubItem(env,"<filename>","File Name.");
			return(cgReturnCode);
		};
	public:
		virtual	int32	InPressKeyModeExit	(BIC_ENV *env)const;
};
//------------------------------------------------------------------------------------------//
class BIC_RUN : public BIC_Node{
	public:
				 BIC_RUN(void) : BIC_Node() {cgCommand = "run";cgReturnCode = BI_RETCODE_RUN;};
		virtual ~BIC_RUN(void){;};
	public:
		virtual	int32	Command (BIC_ENV *env,const STDSTR &par,void *eda)const;
		virtual	int32	Help	(BIC_ENV *env,int32 blDetail = 1)const{
			PrintHelpItem(env,cgCommand,"Execute group command script.");
			if (blDetail == 0)
				return(cgReturnCode);
			PrintHelpSubItem(env,"[gID [ct]]"	,"Execute group by gID.");
			PrintHelpSubItem(env,""				,"ct: Cycle times.");
			return(cgReturnCode);
		};
	public:
		virtual	int32	InPressKeyModeExit	(BIC_ENV *env)const;
};
//------------------------------------------------------------------------------------------//
class BIC_STOP : public BIC_Node{
	public:
				 BIC_STOP(void) : BIC_Node() {cgCommand = "stop";cgReturnCode = BI_RETCODE_STOP;};
		virtual ~BIC_STOP(void){;};
	public:
		virtual	int32	Command (BIC_ENV *env,const STDSTR &par,void *eda)const;
		virtual	int32	Help	(BIC_ENV *env,int32 blDetail = 1)const{
			PrintHelpItem(env,cgCommand,"Stop script execution.");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class BIC_SCRIPT : public BIC_Node{
	public:
				 BIC_SCRIPT(void) : BIC_Node() { cgCommand = "script"; cgReturnCode = BI_RETCODE_SCRIPT; };
		virtual ~BIC_SCRIPT(void){ ; };
	public:
		virtual	int32	Command(BIC_ENV *env, const STDSTR &par, void *eda)const;
		virtual	int32	Help(BIC_ENV *env, int32 blDetail = 1)const{
			PrintHelpItem(env, cgCommand, "Set script parameter.");
			if (blDetail == 0)
				return(cgReturnCode);
			PrintHelpItem(env, "     [-l"				, "List Script Build-in Command.");
			PrintHelpItem(env, "     |[-A<on|off>]"		, "Check standard AT response.");
			PrintHelpItem(env, "      [-E<on|off>]"		, "Display script BIC execution.");
			PrintHelpItem(env, "      [-CE<on|off>]]"	, "Display script BIC explain report.");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
#endif
#endif
#endif
