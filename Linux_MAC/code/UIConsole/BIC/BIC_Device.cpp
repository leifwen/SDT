/*
 * Copyright (c) 2012-2014
 * All rights reserved.
 *
 * File Name: BIC_COM.cpp
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2014.01.25
 
 */
//------------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "BIC_Device.h"
#include "Comm_Convert.h"
#include "AppLayer.h"
//------------------------------------------------------------------------------------------//
#ifdef BIC_DeviceH
//------------------------------------------------------------------------------------------//
int32 BIC_CONNECT::Command(BIC_ENV *env,const STDSTR &par,void *eda)const{
	ExpandDeviceAttr* attr = static_cast<ExpandDeviceAttr*>(eda);
	CMD_TAIL	tail;
	
	tail = CMD_R;
	if (par == "none"){
		tail = CMD_NONE;
	}
	else if (par == "r"){
		tail = CMD_R;
	}
	else if (par == "n"){
		tail = CMD_N;
	}
	else if (par == "rn"){
		tail = CMD_RN;
	}
	else if(par.length() > 0){
		return(Help(env));
	}
	
	if (attr->IsConnected() == 0){
		switch(attr->csType){
		#ifdef SWVERSION_TCPClient
			case CSType_TCP:attr->device->Open(attr->aTCP);break;
		#endif
		#ifdef SWVERSION_UDPCLient
			case CSType_UDP:attr->device->Open(attr->aUDP);break;
		#endif
		#ifdef SWVERSION_TCPServer
			case CSType_TCPS:attr->device->Open(attr->aTCPS);break;
		#endif
		#ifdef SWVERSION_UDPServer
			case CSType_UDPS:attr->device->Open(attr->aUDPS);break;
		#endif
		#ifdef SWVERSION_COM
			case CSType_COM:;
			case CSType_COMV:
				attr->aCOM.type = CSType_COM;
				attr->device->Open(attr->aCOM);break;
		#endif
			default:break;
		}
	}
	else{
		OnlineMode(env,attr,tail);
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_CONNECT::OnlineMode(BIC_ENV *env,ExpandDeviceAttr *attr,CMD_TAIL tail){
	uint8	chKey;
	STDSTR	sendData,strTail;
	uint32	ret;
	
	ret = 0;
	strTail = "";
	switch (tail) {
		case CMD_R:	strTail = "\\r";break;
		case CMD_N:	strTail = "\\n";break;
		case CMD_RN:strTail = "\\r\\n";break;
		default:
			break;
	}
	//define ctrl^A~Z is 1~26,HEX is 01~1A. Escape ctrl^~ to 128
	
	PrintALine(env,Data(COL_clCyan,"In online mode:"));
	env->blInOnlineMode = 1;
	env->blUseSecondLH = 1;
	env->cstdout->PrintEnable();
	sendData = "";
	while((env->blExit == 0) && (attr->IsConnected() != 0)){
		SYS_SleepMS(10);
		chKey = BI_ReadChar(env,0);
		if ((chKey >= 32) && (chKey <= 126)){
			sendData += chKey;
		}
		else if (chKey == '\n'){
			env->cstdout->PrintEnable();
			PrintStr(env,"\n");
			attr->device->PrintSendStrWOG1(sendData + strTail + "\n");//for log
			attr->device->SendCommand(sendData, tail,G_ESCAPE_ON);
			sendData = "";
		}
		else if (chKey == 27){
			ret = 1;
			break;
		}
		else{
			if (chKey == 0x80)
				chKey = 27;
			if ((chKey > 0) && (chKey <= 31)){
				STDSTR strT;
				strT = '^';
				strT += (chKey + 0x40);
				attr->device->PrintSendStrWOG1(strT);
				attr->device->SSend(chKey);
			}
		}
	}
	env->cstdout->PrintEnable();
	PrintStr(env,"\n");
	env->blInOnlineMode = 0;
	env->blUseSecondLH = 0;
	return(ret);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_ONLINE::Command(BIC_ENV *env,const STDSTR &par,void *eda)const{
	ExpandDeviceAttr* attr = static_cast<ExpandDeviceAttr*>(eda);
	if (attr->IsConnected() == 0){
		BIC_CONNECT::Command(env,par,eda);
	}
	if (attr->IsConnected() != 0)
		BIC_CONNECT::Command(env,par,eda);
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_DISCONNECT::Command(BIC_ENV *env,const STDSTR &par,void *eda)const{
	if (par.length() > 0)
		return(Help(env));
	env->cSDTApp->ChildClose(static_cast<ExpandDeviceAttr*>(eda)->device);
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_CR::Command(BIC_ENV *env,const STDSTR &par,void *eda)const{
	ExpandDeviceAttr* attr = static_cast<ExpandDeviceAttr*>(eda);

	if (attr->IsConnected() == 0){
		PrintFail(env,"no connect");
		return(cgReturnCode);
	}
	if (par == "-c"){
#ifdef CommonDefH_VC
		CMDIFrameWndEx *mainFrame;
		mainFrame = (CMDIFrameWndEx*)theApp.GetMainWnd();
		if (mainFrame != nullptr){
			mainFrame->PostMessageW(WM_ON_BICCLEAN,0,0);
			SYS_SleepMS(100);
		}
		return(cgReturnCode);
#endif
#ifdef CommonDefH_Unix
		attr->device->SClean();
#endif
	}
	
	PrintStrNL(env,Data(COL_clDefault,DEV_LINE_START));
	PrintTitle(env,attr->DevType());
	PrintNL(env) << COL_clCyan
	<< "Received : " << Str_ToString(attr->device->SRxBytes()) << " bytes\n"
	<< "Echo     : " << Str_ToString(attr->device->SFwBytes()) << " bytes\n"
	<< "Sent     : " << Str_ToString(attr->device->STxBytes()) << " bytes\n"
	<< COL_DivStar_Default << Endl;
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_ECHO::Command(BIC_ENV *env,const STDSTR &par,void *eda)const{
	ExpandDeviceAttr* attr = static_cast<ExpandDeviceAttr*>(eda);

	if (par == "on"){
		attr->device->EnableEcho();
	}
	else if (par == "off"){
		attr->device->DisableEcho();
	}
	else if(par.length() > 0){
		return(Help(env));
	}
	
	if (attr->device->CheckEcho() == 0){
		PrintResult(env,"ECHO is disable");
	}
	else{
		PrintResult(env,"ECHO is enable");
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_REC::Command(BIC_ENV *env,const STDSTR &par,void *eda)const{
	ExpandDeviceAttr* attr = static_cast<ExpandDeviceAttr*>(eda);
	
	int32	blcheck;
	STDSTR	strCommand,strPar1,strPar2;

	strCommand = Str_Trim(par);
	
	blcheck = 0;
	SplitPar1(strPar1, strPar2, par);
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
	
	if ((par.length() > 0) && (blcheck == 0))
		return(Help(env));
	
	PrintResult(env,((B_ChkFLAG64(*attr->envcfg,ODEV_FLAG_EnRecMsg) == 0)?"Disable":"Enable"),"receive tip,"
				,((B_ChkFLAG64(*attr->envcfg,ODEV_FLAG_EnHEXViewMode) == 0)?"ASCII":"HEX"),"mode,"
				,((B_ChkFLAG64(*attr->envcfg,ODEV_FLAG_EnOSPMsgLine) == 0)?"disable":"enable"),"OSP message delimiter");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_DEV::Command(BIC_ENV *env,const STDSTR &par,void *eda)const{
	ExpandDeviceAttr* attr = static_cast<ExpandDeviceAttr*>(eda);
	
	if (par.length() > 0 )
		return(Help(env));
	
	PrintNL(env)<< COL_DivStar_Default
#ifdef SWVERSION_COM
	<< ColData(COL_clBlue	,"COM")
	<< ColData(COL_clDefault,":")
	<< ColData(COL_clDYellow,attr->aCOM.name)
	<< ColData(COL_clDefault,"@")
	<< ColData(COL_clDYellow,Str_ToString(attr->aCOM.port))
	<< NL
#endif
#ifdef SWVERSION_TCPClient
	<< ColData(COL_clBlue	,"TCP")
	<< ColData(COL_clDefault,":")
	<< ColData(COL_clDYellow,attr->aTCP.name)
	<< ColData(COL_clDefault,"@")
	<< ColData(COL_clDYellow,Str_ToString(attr->aTCP.port))
	<< NL
#endif
#ifdef SWVERSION_UDPCLient
	<< ColData(COL_clBlue	,"UDP")
	<< ColData(COL_clDefault,":")
	<< ColData(COL_clDYellow,attr->aUDP.name)
	<< ColData(COL_clDefault,"@")
	<< ColData(COL_clDYellow,Str_ToString(attr->aUDP.port))
	<< NL
#endif
#ifdef SWVERSION_TCPServer
	<< ColData(COL_clBlue	,"TCP Server")
	<< ColData(COL_clDefault,"@")
	<< ColData(COL_clDYellow,Str_ToString(attr->aTCPS.port))
	<< NL
#endif
#ifdef SWVERSION_UDPServer
	<< ColData(COL_clBlue	,"UDP Server")
	<< ColData(COL_clDefault,"@")
	<< ColData(COL_clDYellow,Str_ToString(attr->aUDPS.port))
	<< NL
#endif
	<< COL_DivStar_Default << Endl;
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_SEND::Command(BIC_ENV *env, const STDSTR &par,void *eda)const{
	ExpandDeviceAttr* attr = static_cast<ExpandDeviceAttr*>(eda);
	if (par.length() == 0)
		return(cgReturnCode);
	if (attr->IsConnected() != 0){
		env->cstdout->PrintEnable();
		attr->device->SendCommandWithPrint(par,CMD_NONE,G_ESCAPE_ON);
		SYS_SleepMS(100);
	}
	else{
		PrintFail(env,"no connect");
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_SENDA::InPressKeyModeExit(BIC_ENV *env)const{
	return(env->cSDTApp->m_Script.IsStop());
}
//------------------------------------------------------------------------------------------//
int32 BIC_SENDA::Command(BIC_ENV *env, const STDSTR &par,void *eda)const{
	ExpandDeviceAttr* attr = static_cast<ExpandDeviceAttr*>(eda);
	STDSTR			strPar1,strPar2;
	COMMAND_NODE	commandNode;
	int32			blScriptAT;
	
	if (par.length() == 0)
		return(Help(env));
	if (attr->IsConnected() != 0){
		
		commandNode.Init();
		
		SplitPar1(strPar1, strPar2, par);
		
		commandNode.StrCommand = strPar2;
		commandNode.StrTimeout = strPar1;
		commandNode.StrCycle = '0';
		commandNode.cmdTail = CMD_NONE;
		
		blScriptAT = env->cSDTApp->m_Script.CheckATResponse() != 0;
		env->cSDTApp->m_Script.ClrblATResponse();
		
		env->cstdout->PrintEnable();
		if (env->cSDTApp->m_Script.Execute(attr->device,&commandNode) == 0){
			PrintFail(env,"script is running");
			return(cgReturnCode);
		}
		if (InPressKeyMode(env) > 0){
			env->cSDTApp->m_Script.Stop();
			PrintSuccess(env,"Pressed double ESC key, stopped script");
		}
		
		if (blScriptAT != 0)
			env->cSDTApp->m_Script.SetblATResponse();
	}
	else{
		PrintFail(env,"no connect");
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_RUN::InPressKeyModeExit(BIC_ENV *env)const{
	return(env->cSDTApp->m_Script.IsStop());
}
//------------------------------------------------------------------------------------------//
int32 BIC_RUN::Command(BIC_ENV *env, const STDSTR &par,void *eda)const{
	ExpandDeviceAttr* attr = static_cast<ExpandDeviceAttr*>(eda);
	uint32			num;
	STDSTR			strPar1,strPar2;
	COMMAND_GROUP	*group;
	
	if (par.length() == 0){
		env->cstdout->PrintEnable();
		if (env->cSDTApp->m_Script.Execute(attr->device,&env->cSDTApp->m_GCList) == 0){
			PrintFail(env,"script is running");
			return(cgReturnCode);
		}
	}
	else{
		SplitPar1(strPar1,strPar2,par);
		
		num = (uint32)strtol(strPar1.c_str(),nullptr,10);
		
		group = (COMMAND_GROUP*)FindInLChildRChainByDRNodeID(&env->cSDTApp->m_GCList, num);
		if (group != nullptr){
			if (strPar2.length() > 0)
				group->autoRunTimes = atoi(strPar2.c_str());
			env->cstdout->PrintEnable();
			if (env->cSDTApp->m_Script.Execute(attr->device,group) == 0){
				PrintFail(env,"script is running");
				return(cgReturnCode);
			}
		}
		else{
			PrintFail(env);
			return(cgReturnCode);
		}
	}
	InPressKeyMode(env);
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_STOP::Command(BIC_ENV *env, const STDSTR &par,void *p)const{
	if (par.length() == 0){
		env->cstdout->PrintEnable();
		env->cSDTApp->m_Script.Stop();
#ifdef SendFilesH
		env->cSDTApp->m_FileSend.Stop();
#endif
		PrintSuccess(env,"Script is stopped");
		return(cgReturnCode);
	}
	return(Help(env));
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_SENDFILE::InPressKeyModeExit(BIC_ENV *env)const{
	return(env->cSDTApp->m_FileSend.IsStop());
}
//------------------------------------------------------------------------------------------//
int32 BIC_SENDFILE::Command(BIC_ENV *env, const STDSTR &par,void *eda)const{
	#ifdef SendFilesH
	ExpandDeviceAttr* attr = static_cast<ExpandDeviceAttr*>(eda);
	STDSTR	strPar;
	strPar = par;
	strPar = Str_SplitSubItem(&strPar, ' ');
	
	env->cstdout->PrintEnable();
	if (env->cSDTApp->m_FileSend.Execute(attr->device,strPar) == 0){
		PrintFail(env,"file is sending");
		return(cgReturnCode);
	}
	
	InPressKeyMode(env);
	#endif
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_SCRIPT::Command(BIC_ENV *env, const STDSTR &par, void *eda)const{
	int32	blcheck;
	STDSTR	strCommand, strPar1, strPar2;
	
	strCommand = Str_Trim(par);
	
	blcheck = 0;
	if (par == "-l"){
		env->cSDTApp->m_Script.Help(env->cstdout);
		return(cgReturnCode);
	}
	SplitPar1(strPar1, strPar2, par);
	do{
		if (strPar1 == "-A"){
			SplitPar1(&strPar1,&strPar2);
			if (strPar1 == "on"){
				env->cSDTApp->m_Script.SetblATResponse();
				blcheck = 1;
			}
			else if (strPar1 == "off"){
				env->cSDTApp->m_Script.ClrblATResponse();
				blcheck = 1;
			}
		}
		else if (strPar1 == "-E"){
			SplitPar1(&strPar1,&strPar2);
			if (strPar1 == "on"){
				env->cSDTApp->m_Script.SetblPrintSBICinfo();
				blcheck = 1;
			}
			else if (strPar1 == "off"){
				env->cSDTApp->m_Script.ClrblPrintSBICinfo();
				blcheck = 1;
			}
		}
		else if (strPar1 == "-CE"){
			SplitPar1(&strPar1,&strPar2);
			if (strPar1 == "on"){
				env->cSDTApp->m_Script.SetblCommandExplain();
				blcheck = 1;
			}
			else if (strPar1 == "off"){
				env->cSDTApp->m_Script.ClrblCommandExplain();
				blcheck = 1;
			}
		}
		SplitPar1(&strPar1,&strPar2);
	}while(strPar1.length() > 0);
	
	if ((par.length() > 0) && (blcheck == 0))
		return(Help(env));
	
	PrintResult(env,((env->cSDTApp->m_Script.CheckATResponse() == 0)?"Disable":"Enable"),"check AT,"
				,((env->cSDTApp->m_Script.CheckPrintSBICinfo() == 0)?"disable":"enable"),"show script BIC excution,"
				,((env->cSDTApp->m_Script.CheckCommandExplain() == 0)?"disable":"enable"),"show script BIC explain");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
STDSTR GetFullName(void){
	STDSTR		strC;
	int32		blAdd;
	blAdd = 0;
	strC = "";
#ifdef SWVERSION_COM
	strC += " COM";
	blAdd = 1;
#endif
#ifdef SWVERSION_TCPClient
	if (blAdd == 1)
		strC += ",";
	strC += " TCP";
	blAdd = 1;
#endif
#ifdef SWVERSION_UDPCLient
	if (blAdd == 1)
		strC += ",";
	strC += " UDP";
	blAdd = 1;
#endif
#ifdef SWVERSION_TCPServer
	if (blAdd == 1)
		strC += ",";
	strC += " TCP server";
	blAdd = 1;
#endif
#ifdef SWVERSION_UDPServer
	if (blAdd == 1)
		strC += ",";
	strC += " UDP server";
#endif
	strC += ".";
	return(strC);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif

