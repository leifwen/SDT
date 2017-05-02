/*
 * Copyright (c) 2012-2014 
 * All rights reserved.
 *
 * File Name: BIC_Terminal.h
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2014.01.25
*/

//------------------------------------------------------------------------------------------//
#include "BIC_B.h"
#include "Commu_NTPServer.h"
#include "Terminal.h"
//------------------------------------------------------------------------------------------//
#ifdef BIC_BH
#ifndef BIC_TerminalH
#define BIC_TerminalH
#ifdef BIC_TerminalH
//------------------------------------------------------------------------------------------//
#if defined Commu_NTPServerH && defined SWVERSION_NTP
#ifndef BIC_NTPH
#define BIC_NTPH
#ifdef BIC_NTPH
//------------------------------------------------------------------------------------------//
class BIC_NTPSERVER : public BIC_Node{
	public:
				 BIC_NTPSERVER(void) : BIC_Node() { cgCommand = "ntps"; cgReturnCode = BI_RETCODE_NTPSERVER; };
		virtual ~BIC_NTPSERVER(void){ ; };
	public:
		virtual	int32	Command(BIC_ENV *env,const STDSTR &par,void *p)const;
		virtual	int32	Help(BIC_ENV *env,int32 blDetail = 1)const{
			PrintHelpItem(env, cgCommand, "Start/Stop NTP server.");
			if (blDetail == 0)
				return(cgReturnCode);
		
			PrintHelpSubItem(env, "[off"	,"Stop NTP server.");
			PrintHelpSubItem(env, " |PORT]"	,"NTP server port, default is 123.");
			return(cgReturnCode);
		}
};
//------------------------------------------------------------------------------------------//
class BIC_NTP : public BIC_Node{
	public:
				 BIC_NTP(void) : BIC_Node() {cgCommand = "ntp";cgReturnCode = BI_RETCODE_NTP;};
		virtual ~BIC_NTP(void){;};
	public:
		virtual	int32	Command (BIC_ENV *env,const STDSTR &par,void *p)const;
		virtual	int32	Help	(BIC_ENV *env,int32 blDetail = 1)const{
			PrintHelpItem(env,cgCommand,"Network Time Protocol.");
			if (blDetail == 0)
				return(cgReturnCode);
			PrintHelpSubItem(env,"[-c"		,"Clean NTP compensation.");
			PrintHelpSubItem(env,"|[IP]"	,"NTP server address.");
			PrintHelpSubItem(env," [PORT]]"	,"NTP server port, default is 123.");
			return(cgReturnCode);
		};
		virtual	int32	NTPSync(BIC_ENV *env, const STDSTR &IP, const int32 port, STDSTR *retPackage, double *dTCompensation, double *dT1Ret,double *dT4Ret)const;
};
//------------------------------------------------------------------------------------------//
#endif
#endif
#endif
//------------------------------------------------------------------------------------------//
#ifdef TerminalH
#ifndef BIC_TSH
#define BIC_TSH
#ifdef BIC_TSH
//------------------------------------------------------------------------------------------//
class BIC_TERMINALSERVER : public BIC_Node{
	public:
				 BIC_TERMINALSERVER(void) : BIC_Node() { cgCommand = "ts"; cgReturnCode = BI_RETCODE_TERMINALSERVER; };
		virtual ~BIC_TERMINALSERVER(void){ ; };
	public:
		virtual	int32	Command(BIC_ENV *env,const STDSTR &par, void *p)const;
		virtual	int32	Help(BIC_ENV *env,int32 blDetail = 1)const{
				PrintHelpItem(env, cgCommand, "Start/Stop terminal server.");
			if (blDetail == 0)
				return(cgReturnCode);
			PrintHelpSubItem(env, "[off"	, "Stop terminal server.");
			PrintHelpSubItem(env, "|PORT]"	, "Terminal server port, default is 16384.");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
#endif
#endif
#endif
//------------------------------------------------------------------------------------------//
#ifdef Terminal_RSTH
#ifndef BIC_RSTH
#define BIC_RSTH
#ifdef BIC_RSTH
//------------------------------------------------------------------------------------------//
class BIC_RST_ON : public BIC_Node{
	public:
				 BIC_RST_ON(void) : BIC_Node() { cgCommand = "on"; cgReturnCode = BI_RETCODE_RST_ON; };
		virtual ~BIC_RST_ON(void){ ; };
	public:
		virtual	int32	Command(BIC_ENV *env,const STDSTR &par,void *p)const;
		virtual	int32	Help(BIC_ENV *env,int32 blDetail = 1)const{
			PrintHelpItem(env, cgCommand, "Start remote SSL terminal server.");
			if (blDetail == 0)
				return(cgReturnCode);
			PrintHelpSubItem(env, "[PORT]"	,"RST server control port, default is 16384.");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class BIC_RST_OFF : public BIC_Node{
	public:
				 BIC_RST_OFF(void) : BIC_Node() { cgCommand = "off"; cgReturnCode = BI_RETCODE_RST_OFF; };
		virtual ~BIC_RST_OFF(void){ ; };
	public:
		virtual	int32	Command(BIC_ENV *env,const STDSTR &par,void *p)const;
		virtual	int32	Help(BIC_ENV *env,int32 blDetail = 1)const{
			PrintHelpItem(env, cgCommand, "Stop remote SSL terminal server.");
			if (blDetail == 0)
				return(cgReturnCode);
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class BIC_RST_LINK : public BIC_Node{
	public:
				 BIC_RST_LINK(void) : BIC_Node() { cgCommand = "link"; cgReturnCode = BI_RETCODE_RST_LINK; };
		virtual ~BIC_RST_LINK(void){ ; };
	public:
		virtual	int32	Command(BIC_ENV *env,const STDSTR &par,void *p)const;
		virtual	int32	Help(BIC_ENV *env,int32 blDetail = 1)const{
			PrintHelpItem(env, cgCommand, "Link to RST server control port.");
			if (blDetail == 0)
				return(cgReturnCode);
			
			PrintHelpSubItem(env, "[off"				, "Disconnect RST server.");
			PrintHelpSubItem(env, " |<RMS IP>"			, "RST server IP, default is leifwen.com.");
			PrintHelpSubItem(env, "  [control PORT]]"	, "RST server control port, default is 16384.");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class BIC_RST_MAPPING : public BIC_Node{
	public:
				 BIC_RST_MAPPING(void) : BIC_Node() { cgCommand = "map"; cgReturnCode = BI_RETCODE_RST_MAPPING; };
		virtual ~BIC_RST_MAPPING(void){ ; };
	public:
		virtual	int32	Command(BIC_ENV *env,const STDSTR &par,void *p)const;
		virtual	int32	Help(BIC_ENV *env,int32 blDetail = 1)const{
			PrintHelpItem(env, cgCommand, "Configure mapping server.");
			if (blDetail == 0)
				return(cgReturnCode);
			
			PrintHelpSubItem(env, "[off]"			, "Close mapping server.");
			PrintHelpSubItem(env, "[RST PORT]"		, "RST server listen PORT.");
			PrintHelpSubItem(env, "[-tcp|-udp]"		, "Mapping mode TCP/UDP, default is -tcp.");
			PrintHelpSubItem(env, "[mapping PORT]"	, "Mapping server PORT, default is RST PORT.");
			PrintHelpSubItem(env, "[mapping IP]"	, "Mapping server IP, default is 127.0.0.1.");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class BIC_RST_LOGIN : public BIC_Node{
	public:
				 BIC_RST_LOGIN(void) : BIC_Node() { cgCommand = "login"; cgReturnCode = BI_RETCODE_RST_LOGIN; };
		virtual ~BIC_RST_LOGIN(void){ ; };
	public:
		virtual	int32	Command(BIC_ENV *env,const STDSTR &par,void *p)const;
		virtual	int32	Help(BIC_ENV *env,int32 blDetail = 1)const{
			PrintHelpItem(env, cgCommand, "Login control terminal.");
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
		virtual	int32	OnlineModeExit		(BIC_ENV *env)const;
};
//------------------------------------------------------------------------------------------//
class BIC_RST : public BIC_Node_S{
	public:
				 BIC_RST(void) : BIC_Node_S() {cgCommand = "rst";cgTitle = cgCommand;cgReturnCode = BI_RETCODE_RST;Init();};
		virtual ~BIC_RST(void){;};
	public:
		virtual	int32	Help	(BIC_ENV *env,int32 blDetail = 1)const{
			if (blDetail == 1){
				HelpTraversalChild(env,0);
				return(cgReturnCode);
			}
			PrintHelpItem(env, cgCommand, "-> Remote SSL terminal server.");
			if (blDetail == 0)
				return(cgReturnCode);
			return(cgReturnCode);
		};
	private:
		void	Init(void){
			Add(cgSub_on) < cgSub_off < cgSub_link < cgSub_mapping < cgSub_login
		#ifdef BIC_TSH
			< cgC_TS;
		#endif
			;};
	private:
		BIC_RST_ON				cgSub_on;
		BIC_RST_OFF				cgSub_off;
		BIC_RST_LINK			cgSub_link;
		BIC_RST_MAPPING			cgSub_mapping;
		BIC_RST_LOGIN			cgSub_login;
	#ifdef BIC_TSH
		BIC_TERMINALSERVER		cgC_TS;
	#endif
};
//------------------------------------------------------------------------------------------//
#endif
#endif
#endif
//------------------------------------------------------------------------------------------//
#endif
#endif
#endif
