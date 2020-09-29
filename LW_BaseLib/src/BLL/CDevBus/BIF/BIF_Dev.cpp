//
//  BIF_Device.cpp
//  SDT
//
//  Created by Leif Wen on 19/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "stdafx.h"
//------------------------------------------------------------------------------------------//
#include "BIF_Dev.h"
#ifdef BIF_Dev_h
#include "Bus.h"
#include "Script.h"
#include "BIF_Expression.h"
//------------------------------------------------------------------------------------------//
void BIF_ENV_DEV::InitQuantity(CMD_ENV* env){
	if (env != nullptr)
		env->InitQuantity(CMD_VID_NEXT);
};
//------------------------------------------------------------------------------------------//
void BIF_ENV_DEV::Init(CMD_ENV* env){
	BIF_ENV::Init(env);
#ifdef BIF_Expression_h
	static	BIF_CTS		sgBIF_CTS;
	static	BIF_DSR		sgBIF_DSR;
	static	BIF_RING	sgBIF_RING;
	static	BIF_DCD		sgBIF_DCD;

	BIF_CONDITION::GetCondition() < sgBIF_CTS < sgBIF_DSR < sgBIF_RING < sgBIF_DCD;
#endif
};
//------------------------------------------------------------------------------------------//
void BIF_ENV_DEV::DeInit(CMD_ENV* env){
	BIF_ENV::DeInit(env);
};
//------------------------------------------------------------------------------------------//
AEXEPOOL* BIF_ENV_DEV::GetAExePool(CMD_ENV* env){
#ifdef Commu_AEXE_h
	return(CMD_ENV::GetVar(env, CMD_VID_AEXEPOOL, (AEXEPOOL*)nullptr));
#else
	return(nullptr);
#endif
};
//------------------------------------------------------------------------------------------//
ExpandDevBusAttr* BIF_ENV_DEV::GetEDA(CMD_ENV* env){
#ifdef Bus_h
	return(CMD_ENV::GetVar(env,CMD_VID_EDA,(ExpandDevBusAttr*)nullptr));
#else
	return(nullptr);
#endif
};
//------------------------------------------------------------------------------------------//
SCRIPT* BIF_ENV_DEV::GetScript(CMD_ENV* env){
#ifdef Script_h
	return(CMD_ENV::GetVar(env,CMD_VID_SCRIPT,(SCRIPT*)nullptr));
#else
	return(nullptr);
#endif
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
CMDID BIF_OPEN::Help(CMD_ENV* env,uint32 flag)const{
	PrintB(env,".CMD = Open=<type>,<name>,<port>,<cfg> -->Open device.");
	PrintB(env,"  Command = <'Open=<type>,<name>,<port>,<cfg>>[//COMMENT]");
	PrintP(env,"  Notes:");
	PrintP(env,"     1.<type> should be COM, TCP or UDP.");
	PrintP(env,"     2.<cfg>  should be ECHO.Default is none");
	PrintP(env,"   eg:");
	PrintP(env,"     Command = 'Open=COM,COM3,115200,ECHO");
	PrintP(env,"               //Open COM3, baud rate is 115200,with echo enable");
	PrintP(env,"     Command = 'Open=TCP,127.0.0.1,9527");
	PrintP(env,"               //Open TCP socket, IP is 127.0.0.1, port is 9527");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIF_OPEN::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	STDSTR		strPar1,strPar2,drvName;
	OPEN_PAR	openpar;
	
	if ((env != nullptr) && (BIF_ENV_DEV::GetEDA(env) != nullptr)){
		BIF_ENV::RetFun(env) = 'F';
		BIF_ENV_DEV::GetEDA(env)->cdevbus->Close();
		
		SplitPar1(strPar1, strPar2, msg, ",");
		Str_LowerCaseSelf(strPar1);
		openpar.type = (strPar1 == "com") ? OPEN_COM : ((strPar1 == "tcp") ? OPEN_TCP : ((strPar1 == "udp") ? OPEN_UDP : OPEN_None));
		drvName = (openpar.type == OPEN_COM) ? DRV_COM : ((openpar.type == OPEN_TCP || openpar.type == OPEN_UDP) ? DRV_SOCKET : "");
		SplitPar1(&strPar1, &strPar2, ",");
		openpar.name = strPar1;
		
		SplitPar1(&strPar1, &strPar2, ",");
		openpar.port = atoi(strPar1.c_str());
		openpar.cfg = 0;
		if (strPar2.length() > 0){
			Str_LowerCaseSelf(strPar2);
			if (strPar2 == "echo")
				openpar.cfg |= CDEVBUS::CFG_blEnableEcho;
		}
		else if (BIF_ENV_DEV::GetEDA(env)->cdevbus->IsEnableEcho()){
			openpar.cfg |= CDEVBUS::CFG_blEnableEcho;
		}
		
		PrintExecute(env,"Open",msg);
		if (BIF_ENV_DEV::GetEDA(env)->Open(drvName,openpar)){
			BIF_ENV_DEV::GetScript(env)->ClrblStopSelf();
			BIF_ENV::RetFun(env) = 'T';
		}
#ifdef GList_h
		else if (BIF_ENV::GetCommandNode(env) != nullptr){
			BIF_ENV::GetCommandNode(env)->StrCatch = "Fail";
			++ BIF_ENV::GetCommandNode(env)->catchTimes;
		}
#endif
	}
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIF_CLOSE::Help(CMD_ENV* env,uint32 flag)const{
	PrintB(env,".CMD = Close -->Close COM, TCP socket or UDP socket.");
	PrintB(env,"  Command = <'Close>[//COMMENT]");
	PrintP(env,"   eg:");
	PrintP(env,"     Command = 'Close  //Close");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIF_CLOSE::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	if ((env != nullptr) && (BIF_ENV_DEV::GetEDA(env) != nullptr)){
		PrintExecute(env,"Close");
		BIF_ENV_DEV::GetEDA(env)->cdevbus->Close();
		BIF_ENV::RetFun(env) = 'T';
		BIF_ENV_DEV::GetScript(env)->SetblStopSelf();
	}
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIF_BR::Help(CMD_ENV* env,uint32 flag)const{
	PrintB(env,".CMD = BR=<PAR> -->Set baud rate.");
	PrintB(env,"   <PAR> = 1200,2400,9600,14400,19200,38400,56000,57600");
	PrintB(env,"          ,115200,128000,230400,256000,460800,912600.");
	PrintB(env,"  Command = <'BR=<PAR>>[//COMMENT]");
	PrintP(env,"   eg:");
	PrintP(env,"     Command = 'BR=115200  //set baud rate 115200");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIF_BR::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	int32 	baudrate;
	if ((env != nullptr) && (BIF_ENV_DEV::GetEDA(env) != nullptr) && (BIF_ENV_DEV::GetEDA(env)->IsCom())){
		baudrate = atoi(Str_Trim(msg).c_str());
		PrintExecute(env,"Set Baudrate to",Str_ToStr(baudrate));
#ifdef Commu_Com_h
		BIF_ENV_DEV::GetEDA(env)->AComCore()->SetBaudrate(baudrate);
#endif
	}
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIF_DTR::Help(CMD_ENV* env,uint32 flag)const{
	PrintB(env,".CMD = DTR=<H|L>[,<PAR>] -->Set DTR output high/low and delay PAR time.");
	PrintB(env,"  Command = <'DTR=<H|L>[,<PAR>]>[//COMMENT]");
	PrintP(env,"   eg:");
	PrintP(env,"     Command = 'DTR=H,1.2    //Set DTR output high, delay 1.2s");
	PrintP(env,"     Command = 'DTR=L,1.2s   //Set DTR output low, delay 1.2s");
	PrintP(env,"     Command = 'DTR=L,500ms  //Set DTR output low, delay 500ms");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIF_DTR::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	int32 		timeout;
	STDSTR		strPar1,strPar2,strTempData;
	
	if ((env != nullptr) && (BIF_ENV_DEV::GetEDA(env) != nullptr) && (BIF_ENV_DEV::GetEDA(env)->IsCom())){
		SplitPar1(strPar1, strPar2, msg, ",");
		Str_LowerCaseSelf(strPar1);
		timeout = GetMS(strPar2);
		
		strTempData = "Set DTR to ";
		if ((strPar1 == "h") || (strPar1 == "1") || (strPar1 == "high")){
			strTempData += "high";
			if (timeout > 0){
				strTempData += ", and delay ";
				strTempData += GetMSSTR(timeout);
			}
			PrintExecute(env,strTempData);
#ifdef Commu_Com_h
			BIF_ENV_DEV::GetEDA(env)->AComCore()->SetDTRToHigh();
#endif
			DelayMS(env,timeout);
		}else if ((strPar1 == "l") || (strPar1 == "0") || (strPar1 == "low")){
			strTempData += "low";
			if (timeout > 0){
				strTempData += ", and delay ";
				strTempData += GetMSSTR(timeout);
			}
			PrintExecute(env,strTempData);
#ifdef Commu_Com_h
			BIF_ENV_DEV::GetEDA(env)->AComCore()->SetDTRToLow();
#endif
			DelayMS(env,timeout);
		}
	}
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIF_RTS::Help(CMD_ENV* env,uint32 flag)const{
	PrintB(env,".CMD = RTS=<H|L>[,<PAR>] -->Set RTS output high/low and delay PAR time.");
	PrintB(env,"  Command = <'RTS=<H|L>[,<PAR>]>[//COMMENT]");
	PrintP(env,"   eg:");
	PrintP(env,"     Command = 'RTS=H,1.2    //Set RTS output high, delay 1.2s");
	PrintP(env,"     Command = 'RTS=L,1.2s   //Set RTS output low, delay 1.2s");
	PrintP(env,"     Command = 'RTS=L,500ms  //Set RTS output low, delay 500ms");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIF_RTS::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	int32 		timeout;
	STDSTR		strPar1,strPar2,strTempData;
	
	if ((env != nullptr) && (BIF_ENV_DEV::GetEDA(env) != nullptr) && (BIF_ENV_DEV::GetEDA(env)->IsCom())){
		SplitPar1(strPar1, strPar2, msg, ",");
		Str_LowerCaseSelf(strPar1);
		timeout = GetMS(strPar2);
		
		strTempData = "Set RTS to ";
		if ((strPar1 == "h") || (strPar1 == "1") || (strPar1 == "high")){
			strTempData += "high";
			if (timeout > 0){
				strTempData += ", and delay ";
				strTempData += GetMSSTR(timeout);
			}
			PrintExecute(env,strTempData);
#ifdef Commu_Com_h
			BIF_ENV_DEV::GetEDA(env)->AComCore()->SetRTSToHigh();
#endif
			DelayMS(env,timeout);
		}else if ((strPar1 == "l") || (strPar1 == "0") || (strPar1 == "low")){
			strTempData += "low";
			if (timeout > 0){
				strTempData += ", and delay ";
				strTempData += GetMSSTR(timeout);
			}
			PrintExecute(env,strTempData);
#ifdef Commu_Com_h
			BIF_ENV_DEV::GetEDA(env)->AComCore()->SetRTSToLow();
#endif
			DelayMS(env,timeout);
		}
	}
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIF_CTS::Help(CMD_ENV* env,uint32 flag)const{
	PrintB(env,".CMD = CTS==<H|L> -->check CTS status.Used as sub-command in Condition Expression.");
	PrintB(env,"  Command = <'CTS==<H|L>>");
	PrintP(env,"   eg:");
	PrintP(env,"     Condition Expression = 'CTS==H   //Expression is true if CTS is high");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIF_CTS::Command(CMD_ENV* env,const STDSTR& msg,void* retTrue)const{
	STDSTR *ret = static_cast<STDSTR*>(retTrue);
	STDSTR strPar;
	
	*ret = 'F';
#ifdef Commu_Com_h
	strPar = Str_LowerCase(Str_Trim(msg));
	if ((env != nullptr) && (BIF_ENV_DEV::GetEDA(env) != nullptr) && (BIF_ENV_DEV::GetEDA(env)->IsCom())){
		if (BIF_ENV_DEV::GetEDA(env)->AComCore()->GetCTSStatus() == "H"){
			if ((strPar == "h") || (strPar == "1") || (strPar == "high"))
				*ret = 'T';
		}
		else if ((strPar == "l") || (strPar == "0") || (strPar == "low")){
			*ret = 'T';
		}
	}
#endif
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIF_DSR::Help(CMD_ENV* env,uint32 flag)const{
	PrintB(env,".CMD = DSR==<H|L> -->check DSR status.Used as sub-command in Condition Expression.");
	PrintB(env,"  Command = <'DSR==<H|L>>");
	PrintP(env,"   eg:");
	PrintP(env,"     Condition Expression = 'DST==H   //Expression is true if DSR is high");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIF_DSR::Command(CMD_ENV* env,const STDSTR& msg,void* retTrue)const{
	STDSTR *ret = static_cast<STDSTR*>(retTrue);
	STDSTR strPar;
	
	*ret = 'F';
#ifdef Commu_Com_h
	strPar = Str_LowerCase(Str_Trim(msg));
	if ((env != nullptr) && (BIF_ENV_DEV::GetEDA(env) != nullptr) && (BIF_ENV_DEV::GetEDA(env)->IsCom())){
		if (BIF_ENV_DEV::GetEDA(env)->AComCore()->GetDSRStatus() == "H"){
			if ((strPar == "h") || (strPar == "1") || (strPar == "high"))
				*ret = 'T';
		}
		else if ((strPar == "l") || (strPar == "0") || (strPar == "low")){
			*ret = 'T';
		}
	}
#endif
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIF_RING::Help(CMD_ENV* env,uint32 flag)const{
	PrintB(env,".CMD = RING==<H|L> -->check RING status.Used as sub-command in Condition Expression.");
	PrintB(env,"  Command = <'RING==<H|L>>");
	PrintP(env,"   eg:");
	PrintP(env,"     Condition Expression = 'RING==H  //Expression is true if RING is high");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIF_RING::Command(CMD_ENV* env,const STDSTR& msg,void* retTrue)const{
	STDSTR *ret = static_cast<STDSTR*>(retTrue);
	STDSTR strPar;
	
	*ret = 'F';
#ifdef Commu_Com_h
	strPar = Str_LowerCase(Str_Trim(msg));
	if ((env != nullptr) && (BIF_ENV_DEV::GetEDA(env) != nullptr) && (BIF_ENV_DEV::GetEDA(env)->IsCom())){
		if (BIF_ENV_DEV::GetEDA(env)->AComCore()->GetRINGStatus() == "H"){
			if ((strPar == "h") || (strPar == "1") || (strPar == "high"))
				*ret = 'T';
		}
		else if ((strPar == "l") || (strPar == "0") || (strPar == "low")){
			*ret = 'T';
		}
	}
#endif
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIF_DCD::Help(CMD_ENV* env,uint32 flag)const{
	PrintB(env,".CMD = DCD==<H|L> -->check DCD status.Used as sub-command in Condition Expression.");
	PrintB(env,"  Command = <'DCD==<H|L>>");
	PrintP(env,"   eg:");
	PrintP(env,"     Condition Expression = 'DCD==H   //Expression is true if DCD is high");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIF_DCD::Command(CMD_ENV* env,const STDSTR& msg,void* retTrue)const{
	STDSTR *ret = static_cast<STDSTR*>(retTrue);
	STDSTR strPar;
	
	*ret = 'F';
#ifdef Commu_Com_h
	strPar = Str_LowerCase(Str_Trim(msg));
	if ((env != nullptr) && (BIF_ENV_DEV::GetEDA(env) != nullptr) && (BIF_ENV_DEV::GetEDA(env)->IsCom())){
		if (BIF_ENV_DEV::GetEDA(env)->AComCore()->GetDCDStatus() == "H"){
			if ((strPar == "h") || (strPar == "1") || (strPar == "high"))
				*ret = 'T';
		}
		else if ((strPar == "l") || (strPar == "0") || (strPar == "low")){
			*ret = 'T';
		}
	}
#endif
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIF_SETRECVDATAMODE::Help(CMD_ENV* env,uint32 flag)const{
	PrintB(env,".CMD = SetRecvDataMode=<HEX|ASCII> -->Change receive data print mode, HEE or ASCII.");
	PrintB(env,"  Command = <'SetRecvDataMode=<HEX|ASCII>>[//COMMENT]");
	PrintP(env,"   eg:");
	PrintP(env,"     Command = 'SetRecvDataMode=HEX    //change to HEX mode");
	PrintP(env,"     Command = 'SetRecvDataMode=ASCII  //change to ASCII mode");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIF_SETRECVDATAMODE::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	if ((env != nullptr) && (BIF_ENV_DEV::GetEDA(env) != nullptr)){
		if (Str_UpperCase(Str_Trim(msg)) == "HEX"){
			B_SetFLAG64(*BIF_ENV_DEV::GetEDA(env)->envcfg, ODEV_FLAG_EnHEXViewMode);
		}else if (Str_UpperCase(Str_Trim(msg)) == "ASCII"){
			B_ClrFLAG64(*BIF_ENV_DEV::GetEDA(env)->envcfg, ODEV_FLAG_EnHEXViewMode);
		}
	}
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIF_SETRECMSGREPORT::Help(CMD_ENV* env,uint32 flag)const{
	PrintB(env,".CMD = SetRecMsgReport=<Enable|Disable> -->Enable/Disable receive report.");
	PrintB(env,"  Command = <'SetRecMsgReport=<Enable|Disable>>[//COMMENT]");
	PrintP(env,"   eg:");
	PrintP(env,"     Command = 'SetRecMsgReport=Enable    //enable report");
	PrintP(env,"     Command = 'SetRecMsgReport=Disable   //disable report");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIF_SETRECMSGREPORT::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	if ((env != nullptr) && (BIF_ENV_DEV::GetEDA(env) != nullptr)){
		if (Str_UpperCase(Str_Trim(msg)) == "ENABLE"){
			B_SetFLAG64(*BIF_ENV_DEV::GetEDA(env)->envcfg, ODEV_FLAG_EnRecMsg);
		}else if (Str_UpperCase(Str_Trim(msg)) == "DISABLE"){
			B_ClrFLAG64(*BIF_ENV_DEV::GetEDA(env)->envcfg, ODEV_FLAG_EnRecMsg);
		}
	}
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif /* BIF_Dev_h */
