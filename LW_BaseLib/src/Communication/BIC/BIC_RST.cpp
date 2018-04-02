//
//  BIC_RST.cpp
//  SDT
//
//  Created by Leif Wen on 24/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "stdafx.h"
#include "BIC_RST.h"
#include "MSG_ID.h"
#ifdef BIC_RST_h
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIC_RST_ON::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env, cgCommand, "Start remote SSL terminal server");
	if (B_ChkFLAG32(flag, CMD_blPrintSimple))
		return(cgCommandID);
	PrintHelpSubItem(env, "[PORT]"	,"RST server control port, default is 16384");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_RST_ON::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	uint32	port;
	
	if (BIC_ENV_COMMU::GetRSTServer(env)->IsOpened()){
		PrintSuccess(env, "RST server is start");
		return(cgCommandID);
	}
	
	if (msg.length() == 0){
		port = 16384;
	}
	else{
		port = atoi(msg.c_str());
	}
	
	if (port == 0){
		PrintFail(env, "Input ERROR");
		return(cgCommandID);
	}
	
	if (BIC_ENV_COMMU::GetRSTServer(env)->Open(SetOpenSSL(OPEN_TCPS,"",port))){
		PrintSuccess(env, "RST server is started at Port",Str_ToStr(port));
	}
	else{
		PrintFail(env);
	}
	return(cgCommandID);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIC_RST_OFF::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env, cgCommand, "Stop remote SSL terminal server");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_RST_OFF::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	BIC_ENV_COMMU::GetRSTServer(env)->Close();
	PrintSuccess(env, "RST server is stopped");
	return(cgCommandID);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIC_RST_LINK::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env, cgCommand, "Link to RST server control port");
	if (B_ChkFLAG32(flag, CMD_blPrintSimple))
		return(cgCommandID);
	PrintHelpSubItem(env, "[off"				, "Disconnect RST server");
	PrintHelpSubItem(env, " |<RMS IP>"			, "RST server IP, default is leifwen.com");
	PrintHelpSubItem(env, "  [control PORT]]"	, "RST server control port, default is 16384");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_RST_LINK::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	uint32	port;
	STDSTR	strIP,strPar;
	
	if (msg == "off"){
		BIC_ENV_COMMU::GetRSTClient(env)->Close();
		PrintSuccess(env, "Control connection close");
		return(cgCommandID);
	}
	
	if (msg.length() == 0){
		strIP = "leifwen.com";
		port = 16384;
	}
	else{
		strPar = msg;
		strIP = Str_ReadSubItem(&strPar, " ");
		port = atoi(Str_Trim(strPar).c_str());
		
		if (port == 0)
			port = 16384;
	}
	
	if (BIC_ENV_COMMU::GetRSTClient(env)->IsOpened()){
		PrintSuccess(env, "Control connection is active");
	}
	else if (BIC_ENV_COMMU::GetRSTClient(env)->Open(SetOpenSSL(OPEN_TCP,strIP, port))){
		PrintSuccess(env, "Control connection is active");
	}
	else{
		PrintFail(env);
	}
	return(cgCommandID);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIC_RST_MAPPING::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env, cgCommand, "Configure mapping server");
	if (B_ChkFLAG32(flag, CMD_blPrintSimple))
		return(cgCommandID);
	PrintHelpSubItem(env, "[off]"			, "Close mapping server");
	PrintHelpSubItem(env, "[RST PORT]"		, "RST server listen PORT");
	PrintHelpSubItem(env, "[-tcp|-udp]"		, "Mapping mode TCP/UDP, default is -tcp");
	PrintHelpSubItem(env, "[mapping PORT]"	, "Mapping server PORT, default is RST PORT");
	PrintHelpSubItem(env, "[mapping IP]"	, "Mapping server IP, default is 127.0.0.1");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_RST_MAPPING::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	uint32		remotePort,mappingPort;
	STDSTR		strMappingIP,strType;
	MSG_RMS*	msgRMS = (MSG_RMS*)BIC_ENV_COMMU::GetRSTClient(env)->GetMSG(MESG_ID_RMS);
	
	if (BIC_ENV_COMMU::GetRSTClient(env)->IsOpened() == G_FALSE){
		PrintFail(env, "RST server control link was not setup");
		return(cgCommandID);
	}
	
	if (msg.length() == 0){
		if (msgRMS->Send_REG_ListMServer(env,&strMappingIP)){
			PrintALine(env,
					   COLOR(COL_clDefault,IUD(DEV_LINE_STAR)),
					   COL_clCyan, strMappingIP,
					   COLOR(COL_clDefault,IUD(DEV_LINE_STAR)));
		}
		else{
			PrintFail(env);
		}
		return(cgCommandID);
	}
	
	SplitPar1(strType,strMappingIP,msg," ");
	if (strType == "off"){
		if (strMappingIP.length() == 0){
			if (msgRMS->Send_REQ_CloseMServer(env,0)){
				PrintSuccess(env, "All mapping server are off");
			}
			else{
				PrintFail(env);
			}
		}
		else{
			remotePort = atoi(strMappingIP.c_str());
			if (msgRMS->Send_REQ_CloseMServer(env,remotePort)){
				strMappingIP = "Mapping server(";
				strMappingIP += Str_ToStr(remotePort);
				strMappingIP += ") is off";
				PrintSuccess(env, strMappingIP);
			}
			else{
				PrintFail(env);
			}
		}
		return(cgCommandID);
	}
	
	remotePort = atoi(strType.c_str());
	mappingPort = remotePort;
	strType = "-tcp";
	if (strMappingIP.length() > 0){
		SplitPar1(&strType,&strMappingIP," ");
		if ((strType != "-tcp") && (strType != "-udp")){
			mappingPort = atoi(strType.c_str());
			strType = "-tcp";
		}
		else{
			mappingPort = atoi(Str_Trim(Str_ReadSubItem(&strMappingIP, " ")).c_str());
			strMappingIP = Str_Trim(strMappingIP);
		}
	}
	if (strMappingIP.length() == 0)
		strMappingIP = "127.0.0.1";
	
	if (BIC_ENV_COMMU::GetRSTClient(env)->IsOpened()){
		if (msgRMS->Send_REQ_SetupMServer(env,remotePort,
										  SetOpenPar((strType == "-tcp") ? OPEN_TCPS : OPEN_UDPS,strMappingIP, mappingPort,0))){
			PrintSuccess(env, "Mapping server is started at Port",Str_ToStr(remotePort));
		}
		else{
			PrintFail(env);
		}
	}
	else{
		PrintFail(env, "RST control link was not setup");
	}
	return(cgCommandID);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIC_RST_LOGIN::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env, cgCommand, "Login control terminal");
	if (B_ChkFLAG32(flag, CMD_blPrintSimple))
		return(cgCommandID);
	PrintHelpSubItem(env,"[none|r|n|rn]","Send with tail none,\\r,\\n,\\r\\n");
	PrintHelpItem(env,"","Goto online mode after connected");
	PrintHelpItem(env,"","Double press ESC + ESC to exit online mode");
	PrintHelpItem(env,"","Double press ESC + A~Z to send 01~1A");
	PrintHelpItem(env,"","Double press ESC + [ to send 1B");
	PrintHelpItem(env,"","Double press ESC + \\ to send 1C");
	PrintHelpItem(env,"","Double press ESC + ] to send 1D");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_RST_LOGIN::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
#ifdef MSG_Terminal_h
	CMD_TAIL		tail;
	MSG_Terminal*	msgTerminal = (MSG_Terminal*)BIC_ENV_COMMU::GetRSTClient(env)->GetMSG(MESG_ID_TERMINAL);
	
	tail = CMD_R;
	if (msg == "none"){
		tail = CMD_NONE;
	}
	else if (msg == "r"){
		tail = CMD_R;
	}
	else if (msg == "n"){
		tail = CMD_N;
	}
	else if (msg == "rn"){
		tail = CMD_RN;
	}
	else if(msg.length() > 0){
		return(Help(env,0));
	}
	do{
		if (BIC_ENV_COMMU::GetRSTClient(env)->IsOpened() == G_FALSE){
			PrintFail(env, "RST server control link was not setup");
			break;
		}
		
		if (msgTerminal->Send_REQ_SetupTerminal(env) == G_FALSE){
			PrintFail(env);
			break;
		}
		
		if (OnlineMode(env,BIC_ENV_COMMU::GetRSTClient(env),tail)){
			PrintSuccess(env,"Pressed double ESC key, exit login");
			msgTerminal->Send_REQ_CloseTerminal(env);
		}
		break;
	}while(0);
#endif
	return(cgCommandID);
}
//------------------------------------------------------------------------------------------//
bool32 BIC_RST_LOGIN::OnlineModeExit(CMD_ENV* env)const{
#ifdef MSG_Terminal_h
	MSG_Terminal*	msgTerminal = (MSG_Terminal*)BIC_ENV_COMMU::GetRSTClient(env)->GetMSG(MESG_ID_TERMINAL);
	return(msgTerminal->CheckTerminalClosed());
#else
	return G_FALSE;
#endif
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIC_RST_TS::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env, cgCommand, "Start/Stop terminal server");
	if (B_ChkFLAG32(flag, CMD_blPrintSimple))
		return(cgCommandID);
	PrintHelpSubItem(env, "[off"	, "Stop terminal server");
	PrintHelpSubItem(env, "|PORT]"	, "Terminal server port, default is 16384");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_RST_TS::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
#ifdef Commu_Terminal_h
	int32	port;
	
	if (msg == "off"){
		BIC_ENV_COMMU::GetTS(env)->Close();
		PrintSuccess(env, "Terminal server is stop");
		return(cgCommandID);
	}
	
	port = atoi(msg.c_str());
	
	if (port == 0)
		port = 16384;
	
	if (BIC_ENV_COMMU::GetTS(env)->Open(SetOpenPar(OPEN_TCPS, "", port, 0))){
		PrintSuccess(env, "Terminal server is started at Port",Str_ToStr(port));
		return(cgCommandID);
	}
	
	PrintFail(env);
#endif
	return(cgCommandID);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIC_RST_APPROVE::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env, cgCommand, "Approve BIN");
	if (B_ChkFLAG32(flag, CMD_blPrintSimple))
		return(cgCommandID);
	PrintHelpSubItem(env,"<hours>"	,"720H = 1M,8760H = 1Y,867240H = 99Y");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_RST_APPROVE::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
#ifdef MSG_Register_h
	MSG_Rerister*	magRerister = (MSG_Rerister*)BIC_ENV_COMMU::GetRSTClient(env)->GetMSG(MESG_ID_REG);
	
	do{
		if (BIC_ENV_COMMU::GetRSTClient(env)->IsOpened() == G_FALSE){
			PrintFail(env, "RST server control link was not setup");
			break;
		}
		if (msg.length() == 0){
			PrintFail(env);
			break;
		}
		
		if (magRerister->Approve(env, atoi(msg.c_str()))){
			PrintSuccess(env);
		}
		else{
			PrintFail(env);
		}
	}while(0);
#endif
	return(cgCommandID);
}
//------------------------------------------------------------------------------------------//











//------------------------------------------------------------------------------------------//
BIC_RST::BIC_RST(void) : BIC_BASE_S() {
	cgCommandID = BIC_ID_RST;
	cgCommand = "rst";
	cgConsoleName = cgCommand;
	cgHelpName = "Remote SSL terminal server";
	
	Add(cgSub_on) < cgSub_off < cgSub_link < cgSub_mapping
#ifdef MSG_Terminal_h
	< cgSub_login
#endif
#ifdef Commu_Terminal_h
	< cgSub_ts
#endif
#if defined MSG_Register_h && defined CommonDefH_MAC
	< cgSub_approve
#endif
	;
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif /* BIC_RST_h */
