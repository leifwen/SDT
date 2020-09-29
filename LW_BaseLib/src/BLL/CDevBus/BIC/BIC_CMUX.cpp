//
//  BIC_CMUX.cpp
//  SDT
//
//  Created by Leif Wen on 26/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "stdafx.h"
//------------------------------------------------------------------------------------------//
#include "BIC_CMUX.h"
//------------------------------------------------------------------------------------------//
#ifdef BIC_CMUX_h
#include "Script.h"
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIC_CMUX_ON::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env,cgCommand,"Start CMUX");
	if (B_ChkFLAG32(flag, CMD_blPrintSimple))
		return(cgCommandID);
	PrintHelpSubItem(env,"[n]"		,"Start CMUX with external group initialization");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_CMUX_ON::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	COMMAND_GROUP	*group;
	uint32			gDID;
	CMUXDriver*		drv = BIC_ENV_DEV::GetCMUX(env);
	
	if (drv->GetCDevBus()->EDA()->IsComOpened() == G_FALSE){
		PrintFail(env,"no COM device");
		return(cgCommandID);
	}
	
	PrintEnable(env);
	if (msg.length() > 0){
#ifdef Script_h
		gDID = (uint32)strtoll(msg.c_str(),nullptr,10);
		
		group = (COMMAND_GROUP*)FindInDownChainByDRNodeID(BIC_ENV::GetGCList(env), gDID);
		if (group != nullptr){
			group->autoRunTimes = 1;
			if (BIC_ENV_DEV::GetScript(env)->Execute(drv->GetCDevBus(),group) == G_FALSE){
				PrintFail(env,"script is running");
				return(cgCommandID);
			}
		}
		else{
			PrintFail(env);
			return(cgCommandID);
		}
		
		if (IntoPressKeyMode(env) > 0){
			BIC_ENV_DEV::GetScript(env)->Stop();
			PrintFail(env,"Pressed ESC key, stopped CMUX");
			return(cgCommandID);
		}
		drv->Open("");
#endif
	}
	else{
		drv->Open(CMUX_DEFATCMDS);
	}
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
bool32 BIC_CMUX_ON::IsExitPressKeyMode(CMD_ENV* env)const{
#ifdef Script_h
	return(BIC_ENV_DEV::GetScript(env)->IsStop());
#endif
	return G_FALSE;
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIC_CMUX_OFF::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env,cgCommand,"Stop CMUX");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_CMUX_OFF::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	PrintEnable(env);
	BIC_ENV_DEV::GetCMUX(env)->Close();
	SYS_SleepMS(50);
	PrintSuccess(env,"CMUX stopped");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIC_CMUX_CLD::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env,cgCommand,"Send CLD");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_CMUX_CLD::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	if (BIC_ENV_DEV::GetCMUX(env)->IsOpened() == G_FALSE){
		PrintFail(env,"CMUX no start");
		return(cgCommandID);
	}
	PrintEnable(env);
	BIC_ENV_DEV::GetCMUX(env)->SendCLD();
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIC_CMUX_PSC::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env,cgCommand,"Send PSC");
	if (B_ChkFLAG32(flag, CMD_blPrintSimple))
		return(cgCommandID);
	PrintHelpSubItem(env,"[n]"		,"num");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_CMUX_PSC::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	int32	num;
	if (BIC_ENV_DEV::GetCMUX(env)->IsOpened() == G_FALSE){
		PrintFail(env,"CMUX no start");
		return(cgCommandID);
	}
	PrintEnable(env);
	if (msg.length() == 0){
		BIC_ENV_DEV::GetCMUX(env)->SendStdPSC(0xff);
	}
	else{
		num = atoi(msg.c_str());
		BIC_ENV_DEV::GetCMUX(env)->SendStdPSC(num);
	}
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIC_CMUX_FC::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env,cgCommand,"Send FC");
	if (B_ChkFLAG32(flag, CMD_blPrintSimple))
		return(cgCommandID);
	PrintHelpSubItem(env,"<on|off>","On/Off");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_CMUX_FC::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	if (BIC_ENV_DEV::GetCMUX(env)->IsOpened() == G_FALSE){
		PrintFail(env,"CMUX no start");
		return(cgCommandID);
	}
	PrintEnable(env);
	if (msg.length() > 0){
		if (msg == "on"){
			BIC_ENV_DEV::GetCMUX(env)->SendFCC(1);
			return(cgCommandID);
		}
		else if (msg == "off"){
			BIC_ENV_DEV::GetCMUX(env)->SendFCC(0);
		}
		return(cgCommandID);
	}
	PrintFail(env);
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIC_CMUX_DTR::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env,cgCommand,"Set DTR");
	if (B_ChkFLAG32(flag, CMD_blPrintSimple))
		return(cgCommandID);
	PrintHelpSubItem(env,"<num>"		,"CUX virtual COM num(1-4)");
	PrintHelpSubItem(env,"<[-H [t]]"	,"Set to High, t is time, unit is S");
	PrintHelpSubItem(env," [-L [t]]>"	,"Set to Low, t is time, unit is S");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_CMUX_DTR::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	STDSTR		strPar1,strPar2,strPar3;
	SYS_TIME_S	timeS;
	int32		bltime;
	int32		num;
	VCOM		*vcom;
	CMUXDriver*	drv = BIC_ENV_DEV::GetCMUX(env);
	
	if (drv->IsOpened() == G_FALSE){
		PrintFail(env,"CMUX no start");
		return(cgCommandID);
	}
	
	SplitPar1(strPar1,strPar3,msg);
	
	num = atoi(strPar1.c_str());
	if ((num == 0) || (num > CMUX::DLCI_MAX)){
		PrintFail(env);
		return(cgCommandID);
	}
	vcom = drv->GetVCOM(num);
	if (vcom == nullptr){
		PrintFail(env);
		return(cgCommandID);
	}
	
	SplitPar1(&strPar1, &strPar3);
	while(strPar1.length() > 0){
		bltime = 0;
		SplitPar1(&strPar2,&strPar3);
		if ((strPar2.length() > 0) && (strPar2 != "-H") && (strPar2 != "-L")){
			SYS_Delay_SetTS(&timeS,atof(strPar2.c_str()) * 1000);
			bltime = 1;
		}
		if (strPar1 == "-H"){
			drv->SendMSC(num,1,CORE_VCOM::Check_RTS(vcom));
		}
		else if (strPar1 == "-L"){
			drv->SendMSC(num,0,CORE_VCOM::Check_RTS(vcom));
		}
		else{
			break;
		}
		if (bltime > 0){
			if (BIC_CONNECT::PressAnyKey(env,drv->GetCDevBus()->EDA(),&timeS) == 27){
				PrintSuccess(env,"Pressed ESC key, stopped execute command");
				return(cgCommandID);
			}
		}
		else{
			strPar1 = strPar2;
			continue;
		}
		SplitPar1(&strPar1,&strPar3);
	};
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIC_CMUX_RTS::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env,cgCommand,"Set RTS");
	if (B_ChkFLAG32(flag, CMD_blPrintSimple))
		return(cgCommandID);
	PrintHelpSubItem(env,"<num>"		,"CUX virtual COM num(1-4)");
	PrintHelpSubItem(env,"<[-H [t]]"	,"Set to High, t is time, unit is S");
	PrintHelpSubItem(env," [-L [t]]>"	,"Set to Low, t is time, unit is S");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_CMUX_RTS::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	STDSTR		strPar1,strPar2,strPar3;
	SYS_TIME_S	timeS;
	int32		bltime;
	int32		num;
	VCOM		*vcom;
	CMUXDriver*	drv = BIC_ENV_DEV::GetCMUX(env);
	
	if (drv->IsOpened() == G_FALSE){
		PrintFail(env,"CMUX no start");
		return(cgCommandID);
	}
	
	SplitPar1(strPar1,strPar3,msg);
	
	num = atoi(strPar1.c_str());
	if ((num == 0) || (num > CMUX::DLCI_MAX)){
		PrintFail(env);
		return(cgCommandID);
	}
	vcom = drv->GetVCOM(num);
	if (vcom == nullptr){
		PrintFail(env);
		return(cgCommandID);
	}
	
	SplitPar1(&strPar1, &strPar3);
	while(strPar1.length() > 0){
		bltime = 0;
		SplitPar1(&strPar2,&strPar3);
		if ((strPar2.length() > 0) && (strPar2 != "-H") && (strPar2 != "-L")){
			SYS_Delay_SetTS(&timeS,atof(strPar2.c_str()) * 1000);
			bltime = 1;
		}
		if (strPar1 == "-H"){
			drv->SendMSC(num,CORE_VCOM::Check_DTR(vcom),1);
		}
		else if (strPar1 == "-L"){
			drv->SendMSC(num,CORE_VCOM::Check_DTR(vcom),0);
		}
		else{
			break;
		}
		if (bltime > 0){
			if (BIC_CONNECT::PressAnyKey(env,drv->GetCDevBus()->EDA(),&timeS) == 27){
				PrintSuccess(env,"Pressed ESC key, stopped execute command");
				return(cgCommandID);
			}
		}
		else{
			strPar1 = strPar2;
			continue;
		}
		SplitPar1(&strPar1,&strPar3);
	};
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIC_CMUX_DLCI::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env,cgCommand,"Set/list CMUX virtual COM status");
	if (B_ChkFLAG32(flag, CMD_blPrintSimple))
		return(cgCommandID);
	PrintHelpSubItem(env,"[num"				,"CUX virtual COM num(1-4)");
	PrintHelpSubItem(env," <-H|-A"			,"Receive mode: Hex/ASCII");
	PrintHelpSubItem(env,"  |-E <on|off>>]"	,"Enable/disable escape");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_CMUX_DLCI::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	int32		blcheck,num;
	STDSTR		strPar1,strPar2,strPrintData;
	VCOM		*vcom;
	
	if (BIC_ENV_DEV::GetCMUX(env)->IsOpened() == G_FALSE){
		PrintFail(env,"CMUX no start");
		return(cgCommandID);
	}
	
	if (msg.length() == 0){
		num = 0;
		PrintStrNL(env,COLOR(COL_clDefault,IUD(DEV_LINE_STAR)));
		while(num++ < CMUX::DLCI_MAX){
			vcom = BIC_ENV_DEV::GetCMUX(env)->GetVCOM(num);
			if (vcom == nullptr)
				continue;
			PrintStrNL(env,COLOR(COL_clCyan,CORE_VCOM::DlciStatus(vcom,&strPrintData)));
		}
		PrintStrNL(env,COLOR(COL_clDefault,IUD(DEV_LINE_STAR)));
		return(cgCommandID);
	}
	
	SplitPar1(strPar1,strPar2,msg);
	
	num = atoi(strPar1.c_str());
	if ((num == 0) || (num > CMUX::DLCI_MAX)){
		PrintFail(env);
		return(cgCommandID);
	}
	vcom = BIC_ENV_DEV::GetCMUX(env)->GetVCOM(num);
	if (vcom == nullptr){
		PrintFail(env);
		return(cgCommandID);
	}
	SplitPar1(&strPar1,&strPar2);
	blcheck = 0;
	while(strPar1.length() > 0){
		if (strPar1 == "-H"){
			CORE_VCOM::Update_HEX(vcom,1);
			CORE_VCOM::Update_Escape(vcom,0);
			blcheck = 1;
		}
		else if (strPar1 == "-A"){
			CORE_VCOM::Update_HEX(vcom,0);
			blcheck = 1;
		}
		else if (strPar1 == "-E"){
			SplitPar1(&strPar1,&strPar2);
			if (strPar1 == "on"){
				CORE_VCOM::Update_Escape(vcom,1);
				CORE_VCOM::Update_HEX(vcom,0);
				blcheck = 1;
			}
			else if (strPar1 == "off"){
				CORE_VCOM::Update_Escape(vcom,0);
				blcheck = 1;
			}
		}
		SplitPar1(&strPar1,&strPar2);
	};
	
	if ((msg.length() > 0) && (blcheck == 0))
		return(Help(env,0));
	
	PrintResult(env,"DLCI",Str_ToStr(num),":"
				,(CORE_VCOM::Check_HEX(vcom) == G_FALSE)?"ASCII mode,":"HEX mode,"
				,(CORE_VCOM::Check_Escape(vcom) == G_FALSE)?"disable escape":"enable escape");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIC_CMUX_LEN::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env,cgCommand,"Set maximum information frame size");
	if (B_ChkFLAG32(flag, CMD_blPrintSimple))
		return(cgCommandID);
	PrintHelpSubItem(env,"[size]"		,"Frame size,default is 121");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_CMUX_LEN::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	int32	num;

	PrintEnable(env);
	if (msg.length() == 0){
		PrintResult(env,"Max information frame size is",Str_ToStr(CMUX::UIH_FRAME::GetInfoSizeMax()));
	}
	else{
		num = atoi(msg.c_str());
		CMUX::UIH_FRAME::GetInfoSizeMax() = num;
		PrintSuccess(env,"Max information frame size was set to",Str_ToStr(CMUX::UIH_FRAME::GetInfoSizeMax()));
	}
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//

















//------------------------------------------------------------------------------------------//
BIC_CMUX::BIC_CMUX(void) : BIC_BASE_S(){
	cgCommandID = BIC_ID_CMUX;
	cgCommand = "cmux";
	cgConsoleName = cgCommand;
	cgHelpName = "CMUX";
	
	AppendDown(cgSub_on) < cgSub_off < cgSub_cld < cgSub_psc < cgSub_fc < cgSub_dtr < cgSub_rts < cgSub_dlci < cgSub_len;
};
//------------------------------------------------------------------------------------------//
#endif /* BIC_CMUX_h */
