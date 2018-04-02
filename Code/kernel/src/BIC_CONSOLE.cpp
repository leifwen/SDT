//
//  BIC_CONSOLE.hpp
//  SDT
//
//  Created by Leif Wen on 20/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "stdafx.h"
#include "BIC_CONSOLE.h"
#ifdef BIC_CONSOLE_h
#include "License_Checker.h"
//------------------------------------------------------------------------------------------//
#ifdef Console_h
//------------------------------------------------------------------------------------------//
AppConsole::AppConsole(uint32 size) : CONSOLE(size){
}
//------------------------------------------------------------------------------------------//
bool32 AppConsole::BICThreadFun(void* p){
#ifdef CommonDefH_VC
	bool32 err = G_TRUE;
	CHK_CheckerInit();
	err = CHK_CheckTime();
	CONSOLE::BICThreadFun(p);
	return(err);
#else
	return(CONSOLE::BICThreadFun(p));
#endif
};
//------------------------------------------------------------------------------------------//
#endif









//------------------------------------------------------------------------------------------//
AppBIC::AppBIC(void) : BIC_BASE_S(){
	SetSFlag(CMD_blTrySubCMD);
	cgCommandID = BIC_ID_CONSOLE;
	cgCommand = SWVERSION_SHORTNAME;
	cgConsoleName = cgCommand;
	
	cgSub_RETURN.RemoveSelf();
	cgSub_EXIT.ClrHidenHelp();
	cgSub_VERSION.ClrHidenHelp();
	cgSub_DISPLAY.ClrHidenHelp();
#ifdef	BIC_EXE_h
	cgSub_Default = &cgSub_exebash;
#endif
	*this
#ifdef	BIC_EXE_h
	< cgSub_exec
	< cgSub_exebash
	< cgSub_exesh
#endif
#ifdef ODEV_System_h
	< cgSub_newrecord
#endif
	;
	cgSub_DISPLAY.RemoveSelf();
	*this < cgSub_DISPLAY
#ifdef BIC_License_h
	< cgSub_bin
#endif
#ifdef BIC_Calc_h
	< cgSub_calc
#endif
#ifdef	BIC_RST_h
	< cgSub_rst
#endif
#ifdef	BIC_Device_h
	< cgSub_conn
#endif
#ifdef BIC_SCM_h
	< cgSub_scm
#endif
#ifdef BIC_GCM_h
	< cgSub_gcm
#endif
#if defined	BIC_CMUX_h && defined SWVERSION_CMUX
	< cgSub_cmux
#endif
#if defined	BIC_NTP_h && defined SWVERSION_NTP
	< cgSub_ntps
	< cgSub_ntp
#endif
#if defined	BIC_Dict_h && defined SWVERSION_DICT
	< cgSub_dict
#endif
	;
#if defined BIC_SCM_h && defined BIC_Device_h && defined SWVERSION_SCRIPT
	InsertAfter(&cgSub_scm.cgSub_save, &scm_send);
#endif
#if defined BIC_GCM_h && defined BIC_Device_h && defined SWVERSION_SCRIPT
	InsertBefore(&cgSub_gcm.cgSub_ls, &gcm_script);
	InsertBefore(&cgSub_gcm.cgSub_ls, &gcm_stop);
	InsertBefore(&cgSub_gcm.cgSub_ls, &gcm_run);
	InsertBefore(&cgSub_gcm.cgSub_group.cgSub_ls, &gcm_group_script);
	InsertBefore(&cgSub_gcm.cgSub_group.cgSub_ls, &gcm_group_stop);
	InsertBefore(&cgSub_gcm.cgSub_group.cgSub_ls, &gcm_group_send);
#endif
};
//------------------------------------------------------------------------------------------//
void AppBIC::PrintConsoleName(CMD_ENV* env)const{
#ifdef BIC_Dev_h
	BIC_CONNECT::PrintTitle(env,BIC_ENV_DEV::GetEDA(env),G_TRUE);
#else
	BIC_BASE_S::PrintConsoleName(env);
#endif
};
//------------------------------------------------------------------------------------------//
#endif /* BIC_CONSOLE_h */
