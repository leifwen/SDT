//
//  BIC_Device.cpp
//  SDT
//
//  Created by Leif Wen on 13/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "stdafx.h"
#include "BIC_Dev.h"
#ifdef BIC_Dev_h
//------------------------------------------------------------------------------------------//
#include "Commu_ComEnum.h"
#include "Script.h"
#include "SendFile.h"
#include "CMUX.h"
#ifdef CommonDefH_VC
#include "SDT.h"
#endif
//------------------------------------------------------------------------------------------//
void BIC_ENV_DEV::InitQuantity(CMD_ENV* env){
	if (env != nullptr){
		env->InitQuantity(CMD_VID_NEXT);
	}
};
//------------------------------------------------------------------------------------------//
void BIC_ENV_DEV::Init(CMD_ENV* env){
	BIC_ENV_COMMU::Init(env);
};
//------------------------------------------------------------------------------------------//
void BIC_ENV_DEV::DeInit(CMD_ENV* env){
	BIC_ENV_COMMU::DeInit(env);
};
//------------------------------------------------------------------------------------------//
ExpandDeviceAttr* BIC_ENV_DEV::GetEDA(CMD_ENV* env){
#ifdef Device_h
	return(CMD_ENV::GetVar(env,CMD_VID_EDA,(ExpandDeviceAttr*)nullptr));
#else
	return(nullptr);
#endif
};
//------------------------------------------------------------------------------------------//
ExpandDeviceAttr* BIC_ENV_DEV::GetEDA_M(CMD_ENV* env){
#ifdef Device_h
	return(CMD_ENV::GetVar(env,CMD_VID_EDA_M,(ExpandDeviceAttr*)nullptr));
#else
	return(nullptr);
#endif
};
//------------------------------------------------------------------------------------------//
ExpandDeviceAttr* BIC_ENV_DEV::GetEDA_A(CMD_ENV* env){
#ifdef Device_h
	return(CMD_ENV::GetVar(env,CMD_VID_EDA_A,(ExpandDeviceAttr*)nullptr));
#else
	return(nullptr);
#endif
};
//------------------------------------------------------------------------------------------//
ExpandDeviceAttr* BIC_ENV_DEV::GetEDA_S(CMD_ENV* env){
#ifdef Device_h
	return(CMD_ENV::GetVar(env,CMD_VID_EDA_S,(ExpandDeviceAttr*)nullptr));
#else
	return(nullptr);
#endif
};
//------------------------------------------------------------------------------------------//
IPCOMLIST* BIC_ENV_DEV::GetValidComList(CMD_ENV* env){
#ifdef Commu_ComEnum_h
	return(CMD_ENV::GetVar(env,CMD_VID_VALIDCOMLIST,(IPCOMLIST*)nullptr));
#else
	return(nullptr);
#endif
};
//------------------------------------------------------------------------------------------//
SCRIPT* BIC_ENV_DEV::GetScript(CMD_ENV* env){
#ifdef Script_h
	return(CMD_ENV::GetVar(env,CMD_VID_SCRIPT,(SCRIPT*)nullptr));
#else
	return(nullptr);
#endif
};
//------------------------------------------------------------------------------------------//
TFileSend* BIC_ENV_DEV::GetFileSend(CMD_ENV* env){
#ifdef SendFile_h
	return(CMD_ENV::GetVar(env,CMD_VID_FILESEND,(TFileSend*)nullptr));
#else
	return(nullptr);
#endif
};
//------------------------------------------------------------------------------------------//
CMUXDriver* BIC_ENV_DEV::GetCMUX(CMD_ENV* env){
#ifdef CMUX_h
	return(CMD_ENV::GetVar(env,CMD_VID_CMUX,(CMUXDriver*)nullptr));
#else
	return(nullptr);
#endif
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
CMDID BIC_MAIN::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env,cgCommand,"Switch to main connection");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_MAIN::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	BIC_ENV_DEV::SetEDA(env, BIC_ENV_DEV::GetEDA_M(env));
	BIC_ENV_DEV::GetEDA(env)->name = "M";
	return(cgCommandID);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIC_AUX::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env,cgCommand,"Switch to aux connection");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_AUX::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	BIC_ENV_DEV::SetEDA(env, BIC_ENV_DEV::GetEDA_A(env));
	BIC_ENV_DEV::GetEDA(env)->name = "A";
	return(cgCommandID);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIC_SECOND::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env,cgCommand,"Switch to second connection");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_SECOND::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	BIC_ENV_DEV::SetEDA(env, BIC_ENV_DEV::GetEDA_S(env));
	BIC_ENV_DEV::GetEDA(env)->name = "S";
	return(cgCommandID);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
namespace BIC {
	static	STDSTR CreateHelpDescribe(void);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_CONNECT::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env,cgCommand,"Open" + BIC::CreateHelpDescribe());
	if (B_ChkFLAG32(flag, CMD_blPrintSimple))
		return(cgCommandID);
	PrintHelpSubItem(env,"[none|r|n|rn]","Send with tail none,\\r,\\n,\\r\\n");
	PrintHelpSubItem(env," ","Goto online mode after connected");
	PrintHelpSubItem(env," ","Double press ESC + ESC to exit online mode");
	PrintHelpSubItem(env," ","Double press ESC + A~Z to send 01~1A");
	PrintHelpSubItem(env," ","Double press ESC + [ to send 1B");
	PrintHelpSubItem(env," ","Double press ESC + \\ to send 1C");
	PrintHelpSubItem(env," ","Double press ESC + ] to send 1D");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_CONNECT::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	ExpandDeviceAttr* attr = BIC_ENV_DEV::GetEDA(env);
	CMD_TAIL	tail;
	
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

	if (attr->IsOpened() == G_FALSE){
		attr->aCOM.type = OPEN_COM;
		attr->Open();
	}
	else{
		OnlineMode(env,attr,tail);
	}
	return(cgCommandID);
}
//------------------------------------------------------------------------------------------//
bool32 BIC_CONNECT::SetConnectPar(CMD_ENV* env,ExpandDeviceAttr* eda,const STDSTR& par,uint32 devType){
	STDSTR	strPar1,strPar2;
	
	if (eda->IsOpened() == G_FALSE){
		eda->openType = devType;
		if(par.length() > 0){
			SplitPar1(strPar1,strPar2,par);
			switch(devType){
				case OPEN_TCP:
					eda->aTCP.name = strPar1;
					return(SetConnectPar2(env,eda,strPar2));
				case OPEN_UDP:
					eda->aUDP.name = strPar1;
					return(SetConnectPar2(env,eda,strPar2));
				case OPEN_TCPS:
				case OPEN_UDPS:
					return(SetConnectPar2(env,eda,par));
				case OPEN_COM:
				case OPEN_COMV:
				default:
					eda->aCOM.name = strPar1;
					eda->aCOM.type = OPEN_COM;
					return(SetConnectPar2(env,eda,strPar2));
			}
		}
		return G_TRUE;
	}
	PrintFail(env,"already connected");
	return G_FALSE;
}
//------------------------------------------------------------------------------------------//
bool32 BIC_CONNECT::SetConnectPar2(CMD_ENV* env,ExpandDeviceAttr* eda,const STDSTR& par){
	int32 baud;
	
	switch(eda->openType){
		case OPEN_TCP: baud = eda->aTCP.port;break;
		case OPEN_UDP: baud = eda->aUDP.port;break;
		case OPEN_TCPS:baud = eda->aTCPS.port;break;
		case OPEN_UDPS:baud = eda->aUDPS.port;break;
		case OPEN_COM:;
		case OPEN_COMV:;
		default:baud = eda->aCOM.port;break;
	}
	
	if (par.length() > 0){
		baud = atoi(par.c_str());
		if (baud < 0)
			baud = 0;
	}
	if (eda->IsOpened() == G_FALSE){
		switch(eda->openType){
			case OPEN_TCP: eda->aTCP.port = baud;break;
			case OPEN_UDP: eda->aUDP.port = baud;break;
			case OPEN_TCPS:eda->aTCPS.port = baud;break;
			case OPEN_UDPS:eda->aUDPS.port = baud;break;
			case OPEN_COM:;
			case OPEN_COMV:;
			default:eda->aCOM.port = baud;break;
		}
		return G_TRUE;
	}
#ifdef Commu_Com_h
	else if (eda->IsCom()){
		eda->ACom()->SetBaudrate(baud);
		return G_TRUE;
	}
#endif
	PrintFail(env,"already connected");
	return G_FALSE;
}
//------------------------------------------------------------------------------------------//
uint8 BIC_CONNECT::PressAnyKey(CMD_ENV* env,ExpandDeviceAttr* eda,SYS_TIME_S* dly){
	uint8	retChar;
	retChar = 0;
	SetInPressAnyKeyMode(env);
	while(ChkblExit(env) == G_FALSE){
		retChar = ReadChar(env,G_FALSE);
		if (retChar > 0)
			break;
		if (dly == nullptr)
			SYS_SleepMS(10);
		if (eda->IsOpened() == G_FALSE)
			break;
		if (SYS_Delay_CheckTS(dly))
			break;
	}
	ClrInPressAnyKeyMode(env);
	return(retChar);
}
//------------------------------------------------------------------------------------------//
bool32 BIC_CONNECT::OnlineMode(CMD_ENV* env,ExpandDeviceAttr* attr,CMD_TAIL tail){
	uint8	chKey;
	STDSTR	sendData,strTail;
	bool32	ret;
	
	ret = G_FALSE;
	strTail = "";
	switch (tail) {
		case CMD_R:	strTail = "\\r";break;
		case CMD_N:	strTail = "\\n";break;
		case CMD_RN:strTail = "\\r\\n";break;
		default:
			break;
	}
	//define ctrl^A~Z is 1~26,HEX is 01~1A. Escape ctrl^~ to 128
	
	PrintALine(env,COLOR(COL_clCyan,IUD("In online mode:")));
	SetInOnlineMode(env);
	SwitchToAux(env);
	PrintEnable(env);
	sendData = "";
	while((ChkblExit(env) == G_FALSE) && attr->IsOpened()){
		SYS_SleepMS(10);
		chKey = ReadChar(env,G_FALSE);
		if ((chKey >= 32) && (chKey <= 126)){
			sendData += chKey;
		}
		else if ((chKey == '\r') || (chKey == '\n')){
			PrintEnable(env);
			PrintStr(env,"\n");
			attr->device->PrintSendStrWOG1(sendData + strTail + "\n");//for log
			attr->device->SendCommand(sendData,tail,G_ESCAPE_ON);
			sendData = "";
		}
		else if (chKey == 27){
			ret = G_TRUE;
			break;
		}
		else{
			if (chKey == 0x80)
				chKey = 27;
			if ((chKey > 0) && (chKey <= 31)){
				PrintEnable(env);
				STDSTR strT;
				strT = '^';
				strT += (chKey + 0x40);
				attr->device->PrintSendStrWOG1(strT);
				attr->device->SSend(IUD(chKey));
			}
		}
	}
	PrintEnable(env);
	PrintStr(env,"\n");
	ClrInOnlineMode(env);
	SwitchToMain(env);
	return(ret);
}
//------------------------------------------------------------------------------------------//
void BIC_CONNECT::PrintTitle(CMD_ENV* env,ExpandDeviceAttr* eda,bool32 blPrintTail){
	if (GetSTDOUT(env) != nullptr){
		*GetSTDOUT(env) << Begin() << NL()
		<< BIC_ENV::SelfName(env) << eda->name << ":";
		switch(eda->openType){
			case OPEN_COM:
			case OPEN_COMV:
				*GetSTDOUT(env)
				<< ColData(COL_clBlue	,"COM")
				<< ColData(COL_clDefault,":")
				<< ColData(COL_clDYellow,eda->aCOM.name)
				<< ColData(COL_clDefault,"@")
				<< ColData(COL_clDYellow,Str_ToStr(eda->aCOM.port));
				break;
			case OPEN_TCP:
				*GetSTDOUT(env)
				<< ColData(COL_clBlue	,"TCP")
				<< ColData(COL_clDefault,":")
				<< ColData(COL_clDYellow,eda->aTCP.name)
				<< ColData(COL_clDefault,"@")
				<< ColData(COL_clDYellow,Str_ToStr(eda->aTCP.port));
				break;
			case OPEN_UDP:
				*GetSTDOUT(env)
				<< ColData(COL_clBlue	,"UDP")
				<< ColData(COL_clDefault,":")
				<< ColData(COL_clDYellow,eda->aUDP.name)
				<< ColData(COL_clDefault,"@")
				<< ColData(COL_clDYellow,Str_ToStr(eda->aUDP.port));
				break;
			case OPEN_TCPS:
				*GetSTDOUT(env)
				<< ColData(COL_clBlue	,"TCP Server")
				<< ColData(COL_clDefault,"@")
				<< ColData(COL_clDYellow,Str_ToStr(eda->aTCPS.port));
				break;
			case OPEN_UDPS:
				*GetSTDOUT(env)
				<< ColData(COL_clBlue	,"UDP Server")
				<< ColData(COL_clDefault,"@")
				<< ColData(COL_clDYellow,Str_ToStr(eda->aUDPS.port));
				break;
			case OPEN_None:
			default:
				*GetSTDOUT(env) << ColData(COL_clDefault,SWVERSION_SHORTNAME);
				break;
		}
		if (blPrintTail)
			*GetSTDOUT(env) << ColData(COL_clDefault,">");
		*GetSTDOUT(env) << Endl();
	}
	return;
}
//------------------------------------------------------------------------------------------//









//------------------------------------------------------------------------------------------//
CMDID BIC_ONLINE::Help(CMD_ENV* env,uint32 flag)const{
	BIC_CONNECT::Help(env,flag);
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_ONLINE::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	ExpandDeviceAttr* attr = BIC_ENV_DEV::GetEDA(env);
	if (attr->IsOpened() == G_FALSE){
		BIC_CONNECT::Command(env,msg,p);
	}
	if (attr->IsOpened())
		BIC_CONNECT::Command(env,msg,p);
	return(cgCommandID);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIC_DISCONNECT::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env,cgCommand,"Close" + BIC::CreateHelpDescribe());
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_DISCONNECT::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	if (msg.length() > 0)
		return(Help(env,0));
	BIC_ENV_DEV::GetEDA(env)->device->CloseSelf(0);
	return(cgCommandID);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIC_CR::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env,cgCommand,"Connection status report");
	if (B_ChkFLAG32(flag, CMD_blPrintSimple))
		return(cgCommandID);
	PrintHelpSubItem(env,"[-c]","Clear report");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_CR::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	ExpandDeviceAttr* attr = BIC_ENV_DEV::GetEDA(env);
	
	if (attr->IsOpened() == G_FALSE){
		PrintFail(env,"no connect");
		return(cgCommandID);
	}
	if (msg == "-c"){
#ifdef CommonDefH_VC
		CMDIFrameWndEx *mainFrame;
		mainFrame = (CMDIFrameWndEx*)theApp.GetMainWnd();
		if (mainFrame != nullptr){
			mainFrame->PostMessageW(WM_ON_BICCLEAN,0,0);
			SYS_SleepMS(100);
		}
		return(cgCommandID);
#endif
#ifdef CommonDefH_Unix
		attr->device->SEmpty();
#endif
	}
	
	PrintStrNL(env,COLOR(COL_clDefault,IUD(DEV_LINE_STAR)));
	BIC_CONNECT::PrintTitle(env,attr,G_FALSE);
	PrintStrNL(env,COL_clCyan
			   , "Received :", Str_ToStr(attr->device->SRxBytes()), "bytes\n"
			   , "Echo     :", Str_ToStr(attr->device->SFwBytes()), "bytes\n"
			   , "Sent     :", Str_ToStr(attr->device->STxBytes()), "bytes\n"
			   , COLOR(COL_clDefault,IUD(DEV_LINE_STAR)));
	return(cgCommandID);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIC_ECHO::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env,cgCommand,"Enable/disable echo");
	if (B_ChkFLAG32(flag, CMD_blPrintSimple))
		return(cgCommandID);
	PrintHelpSubItem(env,"[on|off]","ON/OFF");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_ECHO::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	ExpandDeviceAttr* attr = BIC_ENV_DEV::GetEDA(env);
	
	if (msg == "on"){
		attr->device->EnableEcho();
	}
	else if (msg == "off"){
		attr->device->DisableEcho();
	}
	else if(msg.length() > 0){
		return(Help(env,0));
	}
	
	if (attr->device->CheckEcho() == 0){
		PrintResult(env,"ECHO is disable");
	}
	else{
		PrintResult(env,"ECHO is enable");
	}
	return(cgCommandID);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIC_REC::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env,cgCommand,"Set receive data report parameter");
	if (B_ChkFLAG32(flag, CMD_blPrintSimple))
		return(cgCommandID);
	PrintHelpSubItem(env,"[-t<on|off>]"		,"Enable/disable receive tip");
	PrintHelpSubItem(env,"[-H|-A]"			,"Receive mode: Hex/ASCII");
	PrintHelpSubItem(env,"[-OSP<on|off>]"	,"Enable/disable OSP message delimiter");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_REC::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	ExpandDeviceAttr* attr = BIC_ENV_DEV::GetEDA(env);
	
	bool32	blcheck;
	STDSTR	strCommand,strPar1,strPar2;
	
	strCommand = Str_Trim(msg);
	
	blcheck = G_FALSE;
	SplitPar1(strPar1, strPar2, msg);
	do{
		if (strPar1 == "-t"){
			SplitPar1(&strPar1,&strPar2);
			if (strPar1 == "on"){
				B_SetFLAG64(*attr->envcfg,ODEV_FLAG_EnRecMsg);
				blcheck = 1;
			}
			else if (strPar1 == "off"){
				B_ClrFLAG64(*attr->envcfg,ODEV_FLAG_EnRecMsg);
				blcheck = 1;
			}
		}
		else if (strPar1 == "-H"){
			B_SetFLAG64(*attr->envcfg,ODEV_FLAG_EnHEXViewMode);
			blcheck = 1;
		}
		else if (strPar1 == "-A"){
			B_ClrFLAG64(*attr->envcfg,ODEV_FLAG_EnHEXViewMode);
			blcheck = 1;
		}
		else if (strPar1 == "-OSP"){
			SplitPar1(&strPar1,&strPar2);
			if (strPar1 == "on"){
				B_SetFLAG64(*attr->envcfg,ODEV_FLAG_EnOSPMsgLine);
				blcheck = 1;
			}
			else if (strPar1 == "off"){
				B_ClrFLAG64(*attr->envcfg,ODEV_FLAG_EnOSPMsgLine);
				blcheck = 1;
			}
		}
		SplitPar1(&strPar1,&strPar2);
	}while(strPar1.length() > 0);
	
	if ((msg.length() > 0) && (blcheck == G_FALSE))
		return(Help(env,0));
	
	PrintResult(env
				,((B_ChkFLAG64(*attr->envcfg,ODEV_FLAG_EnRecMsg) == G_FALSE)		?"Disable"	:"Enable")	,"receive tip,"
				,((B_ChkFLAG64(*attr->envcfg,ODEV_FLAG_EnHEXViewMode) == G_FALSE)	?"ASCII"	:"HEX")		,"mode,"
				,((B_ChkFLAG64(*attr->envcfg,ODEV_FLAG_EnOSPMsgLine) == G_FALSE)	?"disable"	:"enable")	,"OSP message delimiter");
	return(cgCommandID);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIC_DEV::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env,cgCommand,"List" + BIC::CreateHelpDescribe());
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_DEV::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	ExpandDeviceAttr* attr = BIC_ENV_DEV::GetEDA(env);
	
	if (msg.length() > 0 )
		return(Help(env,0));
	if (GetSTDOUT(env) != nullptr){
		*GetSTDOUT(env) << Begin() << NL() << COL_DivStar_Default
#ifdef SWVERSION_COM
		<< ColData(COL_clBlue	,"COM")
		<< ColData(COL_clDefault,":")
		<< ColData(COL_clDYellow,attr->aCOM.name)
		<< ColData(COL_clDefault,"@")
		<< ColData(COL_clDYellow,Str_ToStr(attr->aCOM.port))
		<< NL()
#endif
#ifdef SWVERSION_TCPClient
		<< ColData(COL_clBlue	,"TCP")
		<< ColData(COL_clDefault,":")
		<< ColData(COL_clDYellow,attr->aTCP.name)
		<< ColData(COL_clDefault,"@")
		<< ColData(COL_clDYellow,Str_ToStr(attr->aTCP.port))
		<< NL()
#endif
#ifdef SWVERSION_UDPCLient
		<< ColData(COL_clBlue	,"UDP")
		<< ColData(COL_clDefault,":")
		<< ColData(COL_clDYellow,attr->aUDP.name)
		<< ColData(COL_clDefault,"@")
		<< ColData(COL_clDYellow,Str_ToStr(attr->aUDP.port))
		<< NL()
#endif
#ifdef SWVERSION_TCPServer
		<< ColData(COL_clBlue	,"TCP Server")
		<< ColData(COL_clDefault,"@")
		<< ColData(COL_clDYellow,Str_ToStr(attr->aTCPS.port))
		<< NL()
#endif
#ifdef SWVERSION_UDPServer
		<< ColData(COL_clBlue	,"UDP Server")
		<< ColData(COL_clDefault,"@")
		<< ColData(COL_clDYellow,Str_ToStr(attr->aUDPS.port))
		<< NL()
#endif
		<< COL_DivStar_Default << Endl();
	}
	return(cgCommandID);
}
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
static STDSTR BIC::CreateHelpDescribe(void){
	STDSTR		strC;
	bool32		blAdd = G_FALSE;
	strC = "";
#ifdef SWVERSION_COM
	strC += " COM";
	blAdd = G_TRUE;
#endif
#ifdef SWVERSION_TCPClient
	if (blAdd)
		strC += ",";
	strC += " TCP";
	blAdd = G_TRUE;
#endif
#ifdef SWVERSION_UDPCLient
	if (blAdd)
		strC += ",";
	strC += " UDP";
	blAdd = G_TRUE;
#endif
#ifdef SWVERSION_TCPServer
	if (blAdd)
		strC += ",";
	strC += " TCP server";
	blAdd = G_TRUE;
#endif
#ifdef SWVERSION_UDPServer
	if (blAdd)
		strC += ",";
	strC += " UDP server";
#endif
	return(strC);
}
//------------------------------------------------------------------------------------------//
#endif /* BIC_Dev_h */
