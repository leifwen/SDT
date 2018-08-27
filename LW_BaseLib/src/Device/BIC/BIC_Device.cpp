//
//  BIC_Device.cpp
//  SDT
//
//  Created by Leif Wen on 14/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "stdafx.h"
#include "BIC_Device.h"
#ifdef BIC_Device_h
//------------------------------------------------------------------------------------------//
#ifdef SList_h
//------------------------------------------------------------------------------------------//
CMDID BIC_SC_SEND::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env, cgCommand, "Send command by sID if connected");
	if (B_ChkFLAG32(flag, CMD_blPrintSimple))
		return(cgCommandID);
	PrintHelpSubItem(env,"[sID]"		,"Command sID");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_SC_SEND::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	ExpandDeviceAttr* attr = BIC_ENV_DEV::GetEDA(env);
	SC_NODE		*nextNode;
	STDSTR		StrCommand;
	int32		blEnableSendCR;
	uint32		sDID;
	
	StrCommand = "";
	nextNode = nullptr;
	if (attr->IsOpened()){
		if (msg.length() > 0){
			sDID = (uint32)strtol(msg.c_str(),nullptr,10);
			nextNode = (SC_NODE*)FindInLChildRChainByDRNodeID(BIC_ENV::GetSCList(env), sDID);
			if (nextNode != nullptr){
				nextNode->InUse_set();
				StrCommand = nextNode->StrCommand;
				blEnableSendCR = nextNode->blEnableSendCR;
				nextNode->InUse_clr();
				PrintEnable(env);
				attr->device->SendCommandWithPrint(StrCommand,(CMD_TAIL)blEnableSendCR,G_ESCAPE_ON);
				BIC_CONNECT::PressAnyKey(env,attr,nullptr);
				PrintSuccess(env);
			}
			else{
				PrintFail(env);
			}
		}
	}
	else{
		PrintFail(env,"no connect");
	}
	return(cgCommandID);
}
//------------------------------------------------------------------------------------------//
#endif
#if defined BIC_GCM_h && defined Script_h
//------------------------------------------------------------------------------------------//
CMDID BIC_GC_GROUP_SEND::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env, cgCommand, "Send command by cID if connected");
	if (B_ChkFLAG32(flag, CMD_blPrintSimple))
		return(cgCommandID);
	PrintHelpSubItem(env,"[cID]"		,"Command cID");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_GC_GROUP_SEND::Command(CMD_ENV* env,const STDSTR& msg,void* cg)const{
	COMMAND_GROUP*	group = static_cast<COMMAND_GROUP*>(cg);
	COMMAND_NODE	command,*node;
	uint32			cDID;
		
	command.Init();
	
	if (msg.length() > 0){
		cDID = (uint32)strtol(msg.c_str(),nullptr,10);
		node = (COMMAND_NODE*)FindInLChildRChainByDRNodeID(group, cDID);
		if (node != nullptr){
			COMMAND_NODE::CopyCOMMAND_NODE(&command,node);
			command.blEnableSend = 1;
			PrintEnable(env);
			if (BIC_ENV_DEV::GetScript(env)->Execute(BIC_ENV_DEV::GetEDA(env)->device,&command) == G_FALSE){
				PrintFail(env,"script is running");
				return(cgCommandID);
			}
			if (InPressKeyMode(env) > 0){
				BIC_ENV_DEV::GetScript(env)->Stop();
				PrintSuccess(env,"Pressed ESC key, stopped script");
			}
			return(cgCommandID);
		}
	}
	PrintFail(env);
	return(cgCommandID);
}
//------------------------------------------------------------------------------------------//
bool32 BIC_GC_GROUP_SEND::InPressKeyModeExit(CMD_ENV* env)const{
	return(BIC_ENV_DEV::GetScript(env)->IsStop());
}
//------------------------------------------------------------------------------------------//
#endif
//------------------------------------------------------------------------------------------//
CMDID BIC_SEND::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env,cgCommand,"Send data if connected");
	if (B_ChkFLAG32(flag, CMD_blPrintSimple))
		return(cgCommandID);
	PrintHelpSubItem(env,"<data>","Data");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_SEND::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	ExpandDeviceAttr* attr = BIC_ENV_DEV::GetEDA(env);
	
	if (msg.length() == 0)
		return(cgCommandID);
	if (attr->IsOpened()){
		PrintEnable(env);
		attr->device->SendCommandWithPrint(msg,CMD_NONE,G_ESCAPE_ON);
		SYS_SleepMS(100);
	}
	else{
		PrintFail(env,"no connect");
	}
	return(cgCommandID);
}
//------------------------------------------------------------------------------------------//
#ifdef SendFile_h
//------------------------------------------------------------------------------------------//
CMDID BIC_SENDFILE::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env,cgCommand,"Send file if connected");
	if (B_ChkFLAG32(flag, CMD_blPrintSimple))
		return(cgCommandID);
	PrintHelpSubItem(env,"<filename>","File Name");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_SENDFILE::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	ExpandDeviceAttr* attr = BIC_ENV_DEV::GetEDA(env);
	
	STDSTR	strPar;
	strPar = msg;
	strPar = Str_SplitSubItem(&strPar, ' ');
	
	PrintEnable(env);
	if (BIC_ENV_DEV::GetFileSend(env)->Execute(attr->device,strPar) == G_FALSE){
		PrintFail(env,"file is sending");
		return(cgCommandID);
	}
	InPressKeyMode(env);
	return(cgCommandID);
}
//------------------------------------------------------------------------------------------//
bool32 BIC_SENDFILE::InPressKeyModeExit(CMD_ENV* env)const{
	return(BIC_ENV_DEV::GetFileSend(env)->IsStop());
}
//------------------------------------------------------------------------------------------//
#endif
//------------------------------------------------------------------------------------------//
CMDID BIC_STOP::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env,cgCommand,"Stop script execution");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_STOP::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	if (msg.length() == 0){
		PrintEnable(env);
#ifdef Script_h
		BIC_ENV_DEV::GetScript(env)->Stop();
#endif
#ifdef SendFile_h
		BIC_ENV_DEV::GetFileSend(env)->Stop();
#endif
		PrintSuccess(env,"Script is stopped");
		return(cgCommandID);
	}
	return(Help(env,0));
}
//------------------------------------------------------------------------------------------//
#ifdef Script_h
//------------------------------------------------------------------------------------------//
CMDID BIC_SENDA::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env,cgCommand,"Send data automatically if connected");
	if (B_ChkFLAG32(flag, CMD_blPrintSimple))
		return(cgCommandID);
	PrintHelpSubItem(env,"<s>","Time interval");
	PrintHelpSubItem(env,"<data>","Data");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_SENDA::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	ExpandDeviceAttr* attr = BIC_ENV_DEV::GetEDA(env);
	
	STDSTR			strPar1,strPar2;
	COMMAND_NODE	commandNode;
	
	if (msg.length() == 0)
		return(Help(env,0));
	
	if (attr->IsOpened()){
		commandNode.Init();
		
		SplitPar1(strPar1, strPar2, msg);
		
		commandNode.StrCommand = strPar2;
		commandNode.StrTimeout = strPar1;
		commandNode.StrCycle = '0';
		commandNode.cmdTail = CMD_NONE;
		
		PrintEnable(env);
		if (BIC_ENV_DEV::GetScript(env)->Execute(attr->device,&commandNode) == G_FALSE){
			PrintFail(env,"script is running");
			return(cgCommandID);
		}
		if (InPressKeyMode(env) > 0){
			BIC_ENV_DEV::GetScript(env)->Stop();
			PrintSuccess(env,"Pressed ESC key, stopped script");
		}
	}
	else{
		PrintFail(env,"no connect");
	}
	return(cgCommandID);
}
//------------------------------------------------------------------------------------------//
bool32 BIC_SENDA::InPressKeyModeExit(CMD_ENV* env)const{
	return(BIC_ENV_DEV::GetScript(env)->IsStop());
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIC_RUN::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env,cgCommand,"Execute group command script");
	if (B_ChkFLAG32(flag, CMD_blPrintSimple))
		return(cgCommandID);
	PrintHelpSubItem(env,"[gID [ct]]"	,"Execute group by gID");
	PrintHelpSubItem(env,""				,"ct: Cycle times");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_RUN::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	ExpandDeviceAttr* attr = BIC_ENV_DEV::GetEDA(env);
	
	uint32			num;
	STDSTR			strPar1,strPar2;
	COMMAND_GROUP	*group;
	
	if (msg.length() == 0){
		PrintEnable(env);
		if (BIC_ENV_DEV::GetScript(env)->Execute(attr->device,BIC_ENV::GetGCList(env)) == G_FALSE){
			PrintFail(env,"script is running");
			return(cgCommandID);
		}
	}
	else{
		SplitPar1(strPar1,strPar2,msg);
		
		num = (uint32)strtol(strPar1.c_str(),nullptr,10);
		
		group = (COMMAND_GROUP*)FindInLChildRChainByDRNodeID(BIC_ENV::GetGCList(env), num);
		if (group != nullptr){
			if (strPar2.length() > 0)
				group->autoRunTimes = atoi(strPar2.c_str());
			PrintEnable(env);
			if (BIC_ENV_DEV::GetScript(env)->Execute(attr->device,group) == G_FALSE){
				PrintFail(env,"script is running");
				return(cgCommandID);
			}
		}
		else{
			PrintFail(env);
			return(cgCommandID);
		}
	}
	InPressKeyMode(env);
	return(cgCommandID);
}
//------------------------------------------------------------------------------------------//
bool32 BIC_RUN::InPressKeyModeExit(CMD_ENV* env)const{
	return(BIC_ENV_DEV::GetScript(env)->IsStop());
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIC_SCRIPT::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env, cgCommand, "Set script parameter");
	if (B_ChkFLAG32(flag, CMD_blPrintSimple))
		return(cgCommandID);
	PrintHelpItem(env, "     [-l"				, "List Script Build-in Command");
	PrintHelpItem(env, "     |[-A<on|off>]"		, "Check standard AT response");
	PrintHelpItem(env, "      [-E<on|off>]"		, "Display script BIC execution");
	PrintHelpItem(env, "      [-CE<on|off>]]"	, "Display script BIC explain report");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_SCRIPT::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	bool32	blcheck;
	STDSTR	strCommand, strPar1, strPar2;
	
	strCommand = Str_Trim(msg);
	
	blcheck = G_FALSE;
	if (msg == "-l"){
		BIC_ENV_DEV::GetScript(env)->Help(GetSTDOUT(env));
		return(cgCommandID);
	}
	SplitPar1(strPar1, strPar2, msg);
	do{
		if (strPar1 == "-A"){
			SplitPar1(&strPar1,&strPar2);
			if (strPar1 == "on"){
				BIC_ENV_DEV::GetScript(env)->SetblATResponse();
				blcheck = G_TRUE;
			}
			else if (strPar1 == "off"){
				BIC_ENV_DEV::GetScript(env)->ClrblATResponse();
				blcheck = G_TRUE;
			}
		}
		else if (strPar1 == "-E"){
			SplitPar1(&strPar1,&strPar2);
			if (strPar1 == "on"){
				BIC_ENV_DEV::GetScript(env)->SetblPrintSBICinfo();
				blcheck = G_TRUE;
			}
			else if (strPar1 == "off"){
				BIC_ENV_DEV::GetScript(env)->ClrblPrintSBICinfo();
				blcheck = G_TRUE;
			}
		}
		else if (strPar1 == "-CE"){
			SplitPar1(&strPar1,&strPar2);
			if (strPar1 == "on"){
				BIC_ENV_DEV::GetScript(env)->SetblCommandExplain();
				blcheck = G_TRUE;
			}
			else if (strPar1 == "off"){
				BIC_ENV_DEV::GetScript(env)->ClrblCommandExplain();
				blcheck = G_TRUE;
			}
		}
		SplitPar1(&strPar1,&strPar2);
	}while(strPar1.length() > 0);
	
	if ((msg.length() > 0) && (blcheck == G_FALSE))
		return(Help(env,0));
	
	PrintResult(env
				,((BIC_ENV_DEV::GetScript(env)->CheckATResponse() == 0)?"Disable":"Enable"),"check AT,"
				,((BIC_ENV_DEV::GetScript(env)->CheckPrintSBICinfo() == 0)?"disable":"enable"),"show script BIC excution,"
				,((BIC_ENV_DEV::GetScript(env)->CheckCommandExplain() == 0)?"disable":"enable"),"show script BIC explain");
	return(cgCommandID);
}
//------------------------------------------------------------------------------------------//
#endif










//------------------------------------------------------------------------------------------//
BIC_CONN::BIC_CONN(void) : BIC_BASE_S() {
	SetSFlag(CMD_blTrySubCMD);
	cgCommandID = BIC_ID_CONN;
	cgCommand = "conn";
	cgConsoleName = cgCommand;
	cgHelpName = "Connection";
	
	Add(cgC_MAIN) < cgC_SECOND
#ifdef SWVERSION_AUXDEVICE
	< cgC_AUX
#endif
	< cgC_CONNECT < cgC_DISCONNECT < cgC_ONLINE < cgC_CR < cgC_ECHO < cgC_REC < cgC_DEV
#ifdef SWVERSION_COM
	< cgC_ACOM
#endif
#if (defined SWVERSION_PORT) || (defined SWVERSION_Server)
	< cgC_ASOCKET
#endif
	< cgC_SEND
#ifdef SendFile_h
	< cgC_SENDFILE
#endif
#ifdef SWVERSION_SCRIPT
	< cgC_SENDA
	< cgC_RUN
#endif
#if defined SendFile_h || defined SWVERSION_SCRIPT
	 < cgC_STOP
#endif
#ifdef SWVERSION_SCRIPT
	< cgC_SCRIPT
#endif
	;
};
//------------------------------------------------------------------------------------------//
CMDID BIC_CONN::MessageProcessing(CMD_ENV* env,const uint32& mID,const STDSTR& msg,void* p)const{
	return(BIC_BASE_S::MessageProcessing(env,mID,msg,BIC_ENV_DEV::GetEDA(env)));
};
//------------------------------------------------------------------------------------------//
#endif /* BIC_Device_h */
