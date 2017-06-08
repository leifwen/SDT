/*
 * Copyright (c) 2012-2014
 * All rights reserved.
 *
 * File Name: BIC_Terminal.cpp
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2014.01.25
 
 */
//------------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "BIC_Terminal.h"
#include "Comm_File.h"
#include "Comm_Convert.h"
#include "Commu_RMS.h"
#include "AppLayer.h"
//------------------------------------------------------------------------------------------//
#ifdef BIC_NTPH
//------------------------------------------------------------------------------------------//
int32 BIC_NTPSERVER::Command(BIC_ENV *env, const STDSTR &par, void *p)const{
	int32	port;
	
	if (par == "off"){
		env->cSDTApp->m_NTPServer.CloseD();
		PrintSuccess(env, "NTP server is stop");
		return(cgReturnCode);
	}
	
	port = atoi(par.c_str());
	
	if (port == 0)
		port = 123;
	
	if (env->cSDTApp->m_NTPServer.Run(port) != 0){
		PrintSuccess(env, "NTP server is started at Port", Str_ToString(port));
		return(cgReturnCode);
	}
	
	PrintFail(env);
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_NTP::NTPSync(BIC_ENV *env, const STDSTR &IP, const int32 port, STDSTR *retPackage, double *dTCompensation, double *dT1Ret, double *dT4Ret)const{
	STDSTR		strRec;
	TIME		dT1, dT2, dT3, dT4;
	int32		blTimeout, retCode;
	uint8		buffer[48];
	ASOCKET		cASOCKET(1024);
	SYS_TIME_S	timeS;
	
	cASOCKET.SetGetDataByRead();
	cASOCKET.OpenD(IP, port, CSType_UDP, 0);
	
	// strH = "7B 01 00 00";
	// strR1 = "00 00 00 00";
	// strR2 = "00 00 00 00";
	// strR3 = "00 00 00 00";
	// strTR = "00 00 00 00 00 00 00 00";
	// strT1 = "00 00 00 00 00 00 00 00";
	// strT2 = "00 00 00 00 00 00 00 00";
	// strT3 = "00 00 00 00 00 00 00 00";
	
	buffer[0] = 0;
	while (++buffer[0] < 48){
		buffer[buffer[0]] = 0;
	};
	buffer[0] = 0x7B;
	buffer[1] = 0x01;
	
	dT1.Now();
	SYS_NTPToChar(SYS_TimeToNTP(dT1.GetSec()),buffer + 24);
	
	cASOCKET.Send(buffer,48);
	
	SYS_Delay_SetTS(&timeS, 10 * 1000);//10s
	
	strRec = "";
	blTimeout = 0;
	while (env->blExit == 0){
		uint8	chkey;
		cASOCKET.Read(&strRec, G_ESCAPE_OFF);
		dT4.Now();
		if (strRec.length() >= 48)
			break;
		blTimeout = SYS_Delay_CheckTS(&timeS);
		if (blTimeout != 0)
			break;
		env->blInPressKeyMode = 1;
		chkey = BI_ReadChar(env,0);
		if (chkey == 27){
			break;
		}
		else if (chkey == '\r'){
			env->cstdout->PrintEnable();
		}
	}
	env->blInPressKeyMode = 0;
	
	*dTCompensation = 0.0;
	*dT4Ret = 0.0;
	*dT1Ret = 0.0;
	*retPackage = "";
	retCode = 0;
	if (strRec.length() >= 48){
		*retPackage = strRec;
		dT2 = SYS_NTPToTime(SYS_CharToNTP((uint8*)strRec.c_str() + 32));
		dT3 = SYS_NTPToTime(SYS_CharToNTP((uint8*)strRec.c_str() + 40));
		//dTRet = (dT4->GetSec() - dT1.GetSec()) + (dT3.GetSec() - dT2.GetSec());		//Network delay [(T4 - T1) - (T2 - T3)]
		*dTCompensation = (dT2.GetSec() - dT1.GetSec() + dT3.GetSec() - dT4.GetSec()) / 2;	//Compensation [((T2 - T1) + (T3 - T4)) / 2]
		*dT4Ret = dT4.GetSec();
		*dT1Ret = dT1.GetSec();
		retCode = 1;
	}
	else if (blTimeout != 0){
		retCode = -1;
	}
	cASOCKET.CloseD();
	return(retCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_NTP::Command(BIC_ENV *env,const STDSTR &par,void *p)const{
	STDSTR		strPar1,strPar2;
	STDSTR		strRec, strH, strR1, strR2, strR3, strTR, strT1, strT2, strT3;
	TIME		dT1, dTR, dT2, dT3, dT4;
	double		dTCompensation, dT1Ret, dT4Ret;
	int32		port, retCode;
	
	if (par == "-c"){
		dT1.gCompensation = 0.0;
		PrintSuccess(env);
		return(cgReturnCode);
	}
	
	strPar2 = Str_Trim(par);
	strPar1 = Str_ReadSubItem(&strPar2," ");
	Str_LTrimSelf(strPar2);
	
	if (strPar1.length() == 0)
		strPar1 = "time.nist.gov";
	
	if (strPar2.length() == 0)
		strPar2 = "123";
	
	port = atoi(strPar2.c_str());
	
	PrintStrNL(env,Data(COL_clPurple,"NTP server:"),strPar1 + "@" + Str_ToString(port));
	retCode = NTPSync(env, strPar1, port, &strRec, &dTCompensation, &dT1Ret, &dT4Ret);
	if (retCode > 0){
		dT1 = dT1Ret;
		dTR = SYS_NTPToTime(SYS_CharToNTP((uint8*)strRec.c_str() + 16));
		dT2 = SYS_NTPToTime(SYS_CharToNTP((uint8*)strRec.c_str() + 32));
		dT3 = SYS_NTPToTime(SYS_CharToNTP((uint8*)strRec.c_str() + 40));
		dT4 = dT4Ret;
		dT4Ret = (dT4.GetSec() - dT1.GetSec()) + (dT3.GetSec() - dT2.GetSec());
		
		strH = strRec.substr(0, 4);
		strR1 = strRec.substr(4, 4);
		strR2 = strRec.substr(8, 4);
		strR3 = strRec.substr(12, 4);
		strTR = strRec.substr(16, 8);
		strT1 = strRec.substr(24, 8);
		strT2 = strRec.substr(32, 8);
		strT3 = strRec.substr(40, 8);
		
		PrintNL(env) << COL_clMaroon << "Send 48 bytes:\n"
		<< COL_clBlack
		<< "7B 01 00 00\n"
		<< "00 00 00 00\n"
		<< "00 00 00 00\n"
		<< "00 00 00 00\n"
		<< "00 00 00 00 00 00 00 00\n"
		<< SYS_NTPToHEXStr(SYS_TimeToNTP(dT1Ret)) << "\n"
		<< "00 00 00 00 00 00 00 00\n"
		<< "00 00 00 00 00 00 00 00\n"
		<< COL_clMaroon
		<< "Receive 48 bytes:\n"
		<< COL_clBlue
		<< NL << Str_ASCIIToHEXs(strH, G_ESCAPE_OFF)
		<< NL << Str_ASCIIToHEXs(strR1, G_ESCAPE_OFF)
		<< NL << Str_ASCIIToHEXs(strR2, G_ESCAPE_OFF)
		<< NL << Str_ASCIIToHEXs(strR3, G_ESCAPE_OFF)
		<< NL << Str_ASCIIToHEXs(strTR, G_ESCAPE_OFF)
		<< NL << Str_ASCIIToHEXs(strT1, G_ESCAPE_OFF)
		<< NL << Str_ASCIIToHEXs(strT2, G_ESCAPE_OFF)
		<< NL << Str_ASCIIToHEXs(strT3, G_ESCAPE_OFF)
		<< NL << COL_DivLine_Maroon
		<< COL_clPurple << "T1: " << COL_clBlack << SYS_NTPToHEXStr(SYS_TimeToNTP(dT1Ret)) << COL_clPurple << dT1.FormatDateTime(" YYYY/MM/DD,hh:mm:ss.zzz\n")
		<< COL_clPurple << "TR: " << COL_clBlue << Str_ASCIIToHEXs(strTR, G_ESCAPE_OFF) << COL_clPurple << dTR.FormatDateTime(" YYYY/MM/DD,hh:mm:ss.zzz\n")
		<< COL_clPurple << "T2: " << COL_clBlue << Str_ASCIIToHEXs(strT2, G_ESCAPE_OFF) << COL_clPurple << dT2.FormatDateTime(" YYYY/MM/DD,hh:mm:ss.zzz\n")
		<< COL_clPurple << "T3: " << COL_clBlue << Str_ASCIIToHEXs(strT3, G_ESCAPE_OFF) << COL_clPurple << dT3.FormatDateTime(" YYYY/MM/DD,hh:mm:ss.zzz\n")
		<< COL_clPurple << dT4.FormatDateTime("T4:                          YYYY/MM/DD,hh:mm:ss.zzz\n\n")
		<< "Network delay        [(T4 - T1) - (T2 - T3)]:  " << Str_FloatToString(dT4Ret) << "s.\n"
		<< "Compensation   [((T2 - T1) + (T3 - T4)) / 2]:  " << Str_FloatToString(dTCompensation) << "s.\n\n"
		<< Endl;
		dT1.gCompensation += dTCompensation;
	}
	else if (retCode == -1){
		PrintFail(env,"Connection timeout");
	}
	else if (retCode == 0){
		PrintFail(env,"Receive error");
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
#endif
#ifdef BIC_RSTH
//------------------------------------------------------------------------------------------//
int32 BIC_RST_ON::Command(BIC_ENV *env, const STDSTR &par, void *p)const{
	int32	port;
	
	if (env->cSDTApp->m_RST.IsConnected() != 0){
		PrintSuccess(env, "RST server is start");
		return(cgReturnCode);
	}
	
	if (par.length() == 0){
		port = 16384;
	}
	else{
		port = atoi(par.c_str());
	}
	
	if (port == 0){
		PrintFail(env, "Input ERROR");
		return(cgReturnCode);
	}
	
	if (env->cSDTApp->m_RST.Open(port) > 0){
		PrintSuccess(env, "RST server is started at Port",Str_ToString(port));
	}
	else{
		PrintFail(env);
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_RST_OFF::Command(BIC_ENV *env, const STDSTR &par, void *p)const{
	env->cSDTApp->m_RST.CloseD();
	PrintSuccess(env, "RST server is stopped");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_RST_LINK::Command(BIC_ENV *env, const STDSTR &par, void *p)const{
	int32	port;
	STDSTR	strIP,strPar;
	
	if (par == "off"){
		env->cSDTApp->m_RSTCilent.CloseD();
		PrintSuccess(env, "Control connection close");
		return(cgReturnCode);
	}
	
	if (par.length() == 0){
		strIP = "leifwen.com";
		port = 16384;
	}
	else{
		strPar = par;
		strIP = Str_ReadSubItem(&strPar, " ");
		port = atoi(Str_Trim(strPar).c_str());
		
		if (port == 0)
			port = 16384;
	}
	
	if (env->cSDTApp->m_RSTCilent.IsConnected() != 0){
		PrintSuccess(env, "Control connection is active");
	}
	else if (env->cSDTApp->m_RSTCilent.OpenD(strIP, port, CSType_TCP, 0) > 0){
		PrintSuccess(env, "Control connection is active");
	}
	else{
		PrintFail(env);
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_RST_MAPPING::Command(BIC_ENV *env, const STDSTR &par, void *p)const{
	int32	remotePort,mappingPort;
	STDSTR	strlMappingIP,strType;
	
	if (env->cSDTApp->m_RSTCilent.IsConnected() == 0){
		PrintFail(env, "RST server control link was not setup");
		return(cgReturnCode);
	}
	
	if (par.length() == 0){
		if (env->cSDTApp->m_RSTCilent.SendRequestListMServer(&strlMappingIP) > 0){
			PrintNL(env) <<COL_DivStar_Default
			<< COL_clCyan << strlMappingIP
			<< COL_DivStar_Default << Endl;
		}
		else{
			PrintFail(env);
		}
		return(cgReturnCode);
	}
	
	strlMappingIP = Str_Trim(par);
	strType = Str_Trim(Str_ReadSubItem(&strlMappingIP, " "));
	strlMappingIP = Str_Trim(strlMappingIP);
	if (strType == "off"){
		if (strlMappingIP.length() == 0){
			if (env->cSDTApp->m_RSTCilent.SendRequestOffMServer(0) > 0){
				PrintSuccess(env, "All mapping server are off");
			}
			else{
				PrintFail(env);
			}
		}
		else{
			remotePort = atoi(strlMappingIP.c_str());
			if (env->cSDTApp->m_RSTCilent.SendRequestOffMServer(remotePort) > 0){
				strlMappingIP = "Mapping server(";
				strlMappingIP += Str_ToString(remotePort);
				strlMappingIP += ") is off";
				PrintSuccess(env, strlMappingIP);
			}
			else{
				PrintFail(env);
			}
		}
		return(cgReturnCode);
	}
	
	remotePort = atoi(strType.c_str());
	mappingPort = remotePort;
	strType = "-tcp";
	if (strlMappingIP.length() > 0){
		strType = Str_ReadSubItem(&strlMappingIP, " ");
		strlMappingIP = Str_Trim(strlMappingIP);
		if ((strType != "-tcp") && (strType != "-udp")){
			mappingPort = atoi(strType.c_str());
			strType = "-tcp";
		}
		else{
			mappingPort = atoi(Str_Trim(Str_ReadSubItem(&strlMappingIP, " ")).c_str());
			strlMappingIP = Str_Trim(strlMappingIP);
		}
	}
	if (strlMappingIP.length() == 0)
		strlMappingIP = "127.0.0.1";
	
	if (env->cSDTApp->m_RSTCilent.IsConnected() != 0){
		if (env->cSDTApp->m_RSTCilent.SendRequestSetupMServer(remotePort, strlMappingIP, mappingPort,(strType == "-tcp") ? CSType_TCPS : CSType_UDPS) > 0){
			PrintSuccess(env, "Mapping server is started at Port",Str_ToString(remotePort));
		}
		else{
			PrintFail(env);
		}
	}
	else{
		PrintFail(env, "RST control link was not setup");
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_RST_LOGIN::Command(BIC_ENV *env, const STDSTR &par, void *p)const{
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
	
	if (env->cSDTApp->m_RSTCilent.IsConnected() == 0){
		PrintFail(env, "RST server control link was not setup");
		return(cgReturnCode);
	}
	
	if (env->cSDTApp->m_RSTCilent.SendRequestSetupTerminal() == 0){
		PrintFail(env);
		return(cgReturnCode);
	}
	
	if (OnlineMode(env,&env->cSDTApp->m_RSTCilent,tail) > 0){
		PrintSuccess(env,"Pressed double ESC key, exit login");
		env->cSDTApp->m_RSTCilent.SendRequestCloseTerminal();
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_RST_LOGIN::OnlineModeExit(BIC_ENV *env)const{
	return(env->cSDTApp->m_RSTCilent.CheckTerminalClosed());
};
//------------------------------------------------------------------------------------------//
#ifdef Terminal_LicenseH
int32 BIC_APPROVESDT::Command(BIC_ENV *env,const STDSTR &par,void *p)const{
	STDSTR		strPar,strPar1,strPar2;
	STDSTR		strReg,strContent;
	
	if (env->cSDTApp->m_RSTCilent.IsConnected() == 0){
		PrintFail(env, "RST server control link was not setup");
		return(cgReturnCode);
	}
	if (par.length() == 0){
		PrintFail(env);
		return(cgReturnCode);
	}
	
	if (env->cSDTApp->m_RSTCilent.SendApproveSDT(atoi(strPar1.c_str())) > 0){
		PrintSuccess(env);
	}
	else{
		PrintFail(env);
	}
	return(cgReturnCode);
}
#endif
//------------------------------------------------------------------------------------------//
#endif
#ifdef BIC_TSH
//------------------------------------------------------------------------------------------//
int32 BIC_TERMINALSERVER::Command(BIC_ENV *env, const STDSTR &par, void *p)const{
	int32	port;
	
	if (par == "off"){
		env->cSDTApp->m_TS.CloseD();
		PrintSuccess(env, "Terminal server is stop");
		return(cgReturnCode);
	}
	
	port = atoi(par.c_str());
	
	if (port == 0)
		port = 16384;
	
	if (env->cSDTApp->m_TS.Open(port) != 0){
		PrintSuccess(env, "Terminal server is started at Port",Str_ToString(port));
		return(cgReturnCode);
	}
	
	PrintFail(env);
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif
