//
//  BIC_Define.cpp
//  SDT
//
//  Created by Leif Wen on 11/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "stdafx.h"
//------------------------------------------------------------------------------------------//
#include "BIC_Define.h"
#ifdef BIC_Define_h
#include "Console.h"
//------------------------------------------------------------------------------------------//
uint8 BIC_BASE::ReadChar(CMD_ENV* env,bool32 blWait){
	uint8	charGet = 0;
	
	while(IsExit(env) == G_FALSE){
		if (BIC_ENV::GetSTDIN(env)->Get(&charGet,1) > 0)
			break;
		charGet = 0;
		if (blWait == G_FALSE)
			break;
		SYS_SleepMS(10);
	}
	return(charGet);
};
//------------------------------------------------------------------------------------------//
bool32 BIC_BASE::ReadCommand(CMD_ENV* env,STDSTR* retCMD){
	uint8	charGet = 0;
	
	while(IsExit(env) == G_FALSE){
		charGet = ReadChar(env,G_TRUE);
		if ((charGet >= 0x20) && (charGet <= 0x7e)){
			*retCMD += charGet;
		}
		else if ((charGet == '\r') || (charGet == '\n')){
			return G_TRUE;
		}
	}
	return G_FALSE;
};
//------------------------------------------------------------------------------------------//
CMDID BIC_BASE::Execute(CMD_ENV* env,const uint32& mID,const STDSTR& msg,void* p)const{
	if (mID == (uint32)cgCommandID){
		if (CheckSFlag(CMD_blHidenHelp) == G_FALSE){
			if ((msg == "?") || (msg == "??") || (msg == "-help"))
				return(Help(env,0));
		}
	}
	return(CMD_NODE::Execute(env,mID,msg,p));
};
//------------------------------------------------------------------------------------------//
uint8 BIC_BASE::PrintPressAnyKey(CMD_ENV* env){
	uint8	retChar = 0;
	SetInPressAnyKeyMode(env);
	if (IsExit(env) == G_FALSE){
		PrintALine(env,COLOR(COL_clDYellow,IUD("Press any key to continue.")));
		retChar = ReadChar(env,G_TRUE);
	}
	ClrInPressAnyKeyMode(env);
	return(retChar);
};
//------------------------------------------------------------------------------------------//
bool32 BIC_BASE::IntoPressKeyMode(CMD_ENV* env)const{
	uint8	chKey;
	bool32	ret = G_FALSE;
	
	SetInPressAnyKeyMode(env);
	PrintEnable(env);
	
	while((IsExit(env) == G_FALSE) && (IsExitPressKeyMode(env) == G_FALSE)){
		SYS_SleepMS(10);
		chKey = ReadChar(env,G_FALSE);
		if (chKey == '\n'){
			PrintEnable(env);
		}
		else if (chKey == 27){
			ret = G_TRUE;
			break;
		}
	}
	ClrInPressAnyKeyMode(env);
	return(ret);
};
//------------------------------------------------------------------------------------------//
bool32 BIC_BASE::IsExitPressKeyMode(CMD_ENV* env)const{
	return G_FALSE;
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
CMDID BIC_VERSION::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env,cgCommand,"SW Version");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_VERSION::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	PrintResult(env, SWVERSION_SHORTNAME, SWVERSION_VER);
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIC_EXIT::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env,cgCommand,"Exit SW");
	return(BIC_ID_EXIT_HELP);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_EXIT::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
#ifdef CommonDefH_VC
	return(BIC_ID_NO);
#endif
#ifdef CommonDefH_Unix
	SetblExitForce(env);
	PrintStr(env,"\n");
	return(cgCommandID);
#endif
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIC_RETURN::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env,cgCommand,"Return to the upper menu");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_RETURN::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	PrintStr(env,"\n");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIC_DISPLAY::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env,cgCommand,"Enable/disable view");
	if (B_ChkFLAG32(flag, CMD_blPrintSimple))
		return(cgCommandID);
	PrintHelpSubItem(env,"[on"		,"Always enable");
	PrintHelpSubItem(env," |off"	,"Always disable");
	PrintHelpSubItem(env," |auto]"	,"Set in auto mode");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_DISPLAY::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	if (msg == "on"){
		ClrblDisplayAuto(env);
		PrintEnable(env);
	}
	else if (msg == "off"){
		ClrblDisplayAuto(env);
		PrintDisable(env);
	}
	else if (msg == "auto"){
		SetblDisplayAuto(env);
	}
	else if (msg.length() > 0){
		return(Help(env,0));
	}
	if (IsDisplayAuto(env)){
		PrintResult(env,"Display is in auto mode");
	}
	else if (IsPrintEnable(env)){
		PrintResult(env,"Display is enable");
	}
	else{
		PrintResult(env,"Display is disable");
	}
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIC_NEWRECORD::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env,cgCommand,"Create new record file");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_NEWRECORD::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	if (msg.length() > 0)
		return(Help(env,0));
#ifdef ODEV_BUS_h
	PrintEnable(env);
	BIC_ENV::GetCache(env)->CreateG2_FILE(ODEV_FILE::CreateNewLOGFileName());
	PrintResult(env,"Set new record file:\n",BIC_ENV::GetCache(env)->GetG2_File()->GetFileName());
#endif
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIC_SELFNAME::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env,cgCommand,"Set self name");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_SELFNAME::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	BIC_ENV::SelfName(env) = msg;
	if (msg.length() > 0)
		BIC_ENV::SelfName(env) += ".";
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#ifdef CommonDefH_Unix
#include <fcntl.h>
#include <signal.h>
#include <termios.h>
#ifdef CommonDefH_Linux
#include <sys/types.h>
#include <sys/wait.h>
#endif
//------------------------------------------------------------------------------------------//
CMDID BIC_DAEMON::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env,cgCommand,"Change to daemon");
	if (B_ChkFLAG32(flag, CMD_blPrintSimple))
		return(cgCommandID);
	PrintHelpSubItem(env,"[BIC]","Build-in command, default is rst ts");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
bool32 DeamonExecuteCommand(const STDSTR& cmd){
	char	**argv;
	STDSTR	strCMD,strArg;
	int32	num,ret;
	
	num = 0;
	strCMD = cmd;
	do{
		Str_TrimSelf(strCMD);
		strArg = Str_ReadSubItem(&strCMD, " ");
		if (strArg.length() > 0)
			++ num;
	}while(strCMD.length() > 0);
	
	if (num > 0){
		argv = new char*[num + 1];
		argv[num] = (char*)nullptr;
		
		num = 0;
		strCMD = cmd;
		do{
			Str_TrimSelf(strCMD);
			strArg = Str_ReadSubItem(&strCMD, " ");
			if (strArg.length() > 0){
				argv[num] = new char[strArg.length() + 1];
				memcpy(argv[num],strArg.c_str(),strArg.length());
				argv[num][strArg.length()] = 0;
				++ num;
			}
		}while(strCMD.length() > 0);
		ret = execvp(argv[0],argv);
		delete []argv;
		return(ret);
	}
	return G_FALSE;
};
//------------------------------------------------------------------------------------------//
CMDID BIC_DAEMON::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	int		status,fd_Child;
	pid_t	childpid;
	STDSTR	strCMD = "";
#ifdef CommonDefH_Linux
	char strProcessPath[1024] = {0};
	readlink("/proc/self/exe", strProcessPath,1024);
	strCMD = strProcessPath;
#endif
#ifdef CommonDefH_MAC
	strCMD = "./SDT";
#endif
	childpid = fork();
	if (childpid == 0){				//enter to child process.
		status = 0;
		fd_Child = open("/dev/null",O_RDWR);
		dup2(fd_Child,0);	//redirect standard input to read pipe.
		dup2(fd_Child,1);	//redirect write pipe to standard output.
		dup2(fd_Child,2);	//redirect write pipe to standard output.
		
		strCMD += " -bic selfname_daemon";
		if (msg.length() == 0)
			strCMD += " rst_ts";
		
		status = DeamonExecuteCommand(strCMD);
		
		exit(status);
	}
	else if (childpid > 0){			//enter to fater process.
		PrintSuccess(env);
		env->SetblExit();
	}
	else{
		PrintFail(env,"fork() failed");
	}
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
#endif
//------------------------------------------------------------------------------------------//









//------------------------------------------------------------------------------------------//
BIC_BASE_S::BIC_BASE_S(void) : BIC_BASE(){
	AppendDown(cgSub_EXIT) < cgSub_VERSION < cgSub_RETURN < cgSub_DISPLAY;
	cgConsoleName = "";
	cgHelpName = "";
	fatherName = "";
	cgSub_Default = nullptr;
	
	cgSub_EXIT.SetHidenHelp();
	cgSub_VERSION.SetHidenHelp();
	cgSub_DISPLAY.SetHidenHelp();
};
//------------------------------------------------------------------------------------------//
BIC_BASE_S* BIC_BASE_S::AppendDownNode(TNF* tnfNode){
	TNFP *node = static_cast<TNFP*>(tnfNode);
	BIC_BASE::AppendDownNode(node);
	if (fatherName.length() > 0){
		node->SetUpName(fatherName + "/" + Str_UpperCase((cgConsoleName.length() > 0)?cgConsoleName:cgCommand));
	}
	else{
		node->SetUpName(Str_UpperCase((cgConsoleName.length() > 0)?cgConsoleName:cgCommand));
	}
	return(this);
};
//------------------------------------------------------------------------------------------//
void BIC_BASE_S::PrintConsoleName(CMD_ENV* env)const{
	STDSTR	consoleName;
	
	consoleName = BIC_ENV::SelfName(env)
				+ fatherName + ((fatherName.length() > 0) ? "/" : "") + Str_UpperCase((cgConsoleName.length() > 0) ? cgConsoleName : cgCommand)
				+ ">";
	PrintStr(env, COLOR(COL_clDefault,consoleName));
};
//------------------------------------------------------------------------------------------//
CMDID BIC_BASE_S::Help(CMD_ENV* env,uint32 flag)const{
	if (B_ChkFLAG32(flag, CMD_blPrintSubCMD)){
		PrintALine(env,COL_clDYellow,"Build-in command explain:");
		TraversalChildHelp(env,flag & CMD_blPrintSimple);
	}
	else{
		PrintHelpItem(env, cgCommand, "->" ,cgHelpName);
	}
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_BASE_S::Execute(CMD_ENV* env,const uint32& mID,const STDSTR& msg,void* p)const{
	if (mID == (uint32)cgCommandID){
		if (CheckSFlag(CMD_blHidenHelp) == G_FALSE){
			if ((msg == "?") || (msg == "-help"))
				return(Help(env,CMD_blPrintSubCMD | CMD_blPrintSimple));
			if (msg == "??")
				return(Help(env,CMD_blPrintSubCMD));
		}
	}
	return(CMD_NODE::Execute(env,mID,msg,p));
};
//------------------------------------------------------------------------------------------//
CMDID BIC_BASE_S::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	CMDID	retCode = cgCommandID;
	STDSTR	cmd;
	if (msg.length() > 0){
		retCode = TraversalChildExecute(env,CMD_ID_NO,msg,p);
	}
	else{
		while(IsExit(env) == G_FALSE){
			PrintConsoleName(env);
			if (ReadCommand(env,_EMPTY(&cmd)) == G_FALSE)
				return(BIC_ID_NO);
			
			Str_TrimSelf(cmd);
			if (cmd.length() > 0){
				retCode = Execute(env,cgCommandID,cmd,p);
				if (retCode == BIC_ID_RETURN){
					retCode = cgCommandID;
					if (GetUp(this) != nullptr)
						break;
				}
				else if ((retCode == BIC_ID_NO) && (cgSub_Default != nullptr)){
					cgSub_Default->Command(env, cmd, p);
				}
			}
			if (IsDisplayAuto(env))
				PrintEnable(env);
		}
	}
	return (retCode);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif
