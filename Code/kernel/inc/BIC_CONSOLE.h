//
//  BIC_CONSOLE.h
//  SDT
//
//  Created by Leif Wen on 20/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "BIC_Define.h"
#include "BIC_Calc.h"
#include "BIC_SCM.h"
#include "BIC_GCM.h"

#include "BIC_License.h"

#include "BIC_Dict.h"

#include "BIC_EXE.h"
#include "BIC_NTP.h"
#include "BIC_RST.h"

#include "BIC_Device.h"
#include "BIC_CMUX.h"

#include "Console.h"
//------------------------------------------------------------------------------------------//
#ifdef Console_h
class AppConsole : public CONSOLE{
	public:
				 AppConsole(uint32 size);
		virtual ~AppConsole(void){;};
	private:
		virtual		bool32		BICThreadFun(void* p);
};
#endif
//------------------------------------------------------------------------------------------//
#ifdef BIC_Define_h
//------------------------------------------------------------------------------------------//
#ifndef BIC_CONSOLE_h
#define BIC_CONSOLE_h
#ifdef BIC_CONSOLE_h
//------------------------------------------------------------------------------------------//
class AppBIC : public BIC_BASE_S{
	public:
				 AppBIC(void);
		virtual ~AppBIC(void){;};
	public:
		virtual	void		PrintConsoleName	(CMD_ENV* env)const;
	public:
#ifdef	BIC_EXE_h
		BIC_EXEC 			cgSub_exec;
		BIC_EXE_BASH 		cgSub_exebash;
		BIC_EXE_SH 			cgSub_exesh;
#endif
#ifdef ODEV_System_h
		BIC_NEWRECORD		cgSub_newrecord;
#endif
#ifdef BIC_Calc_h
		BIC_CALC			cgSub_calc;
#endif
#ifdef	BIC_RST_h
		BIC_RST				cgSub_rst;
#endif
#ifdef	BIC_Device_h
		BIC_CONN 			cgSub_conn;
#endif
#ifdef BIC_SCM_h
		BIC_SCM				cgSub_scm;
	#if defined BIC_Device_h && defined SWVERSION_SCRIPT
		BIC_SC_SEND			scm_send;
	#endif
#endif
#ifdef BIC_GCM_h
		BIC_GCM				cgSub_gcm;
	#if defined BIC_Device_h && defined SWVERSION_SCRIPT
		BIC_GC_GROUP_SEND	gcm_group_send;
		BIC_SCRIPT			gcm_script;
		BIC_STOP			gcm_stop;
		BIC_RUN				gcm_run;
		BIC_SCRIPT			gcm_group_script;
		BIC_STOP			gcm_group_stop;
	#endif
#endif
#if defined	BIC_CMUX_h && defined SWVERSION_CMUX
		BIC_CMUX			cgSub_cmux;
#endif
#if defined	BIC_NTP_h && defined SWVERSION_NTP
		BIC_NTPS			cgSub_ntps;
		BIC_NTP				cgSub_ntp;
#endif
#if defined	BIC_Dict_h && defined SWVERSION_DICT
		BIC_DICT			cgSub_dict;
#endif
#ifdef BIC_License_h
		BIC_BIN				cgSub_bin;
#endif
		BIC_SELFNAME		cgSub_selfname;
#ifdef CommonDefH_Unix
		BIC_DAEMON			cgSub_daemon;
#endif
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif /* BIC_CONSOLE_h */
#endif /* BIC_CONSOLE_h */
#endif
