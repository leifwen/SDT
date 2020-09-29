//
//  BIC_NTP.cpp
//  SDT
//
//  Created by Leif Wen on 16/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "stdafx.h"
//------------------------------------------------------------------------------------------//
#include "BIC_NTP.h"
#ifdef BIC_NTP_h
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIC_NTPS::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env, cgCommand, "Start/Stop NTP server");
	if (B_ChkFLAG32(flag, CMD_blPrintSimple))
		return(cgCommandID);
	PrintHelpSubItem(env, "[off"	,"Stop NTP server");
	PrintHelpSubItem(env, " |PORT]"	,"NTP server port, default is 123");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_NTPS::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	int32	port;
	if (BIC_ENV_COMMU::GetNTPS(env) == nullptr){
		PrintFail(env,"no set NTPS in BIC_ENV_DEV");
	}
	else{
		if (msg == "off"){
			BIC_ENV_COMMU::GetNTPS(env)->Close();
			PrintSuccess(env, "NTP server is stop");
			return(cgCommandID);
		}
		
		port = atoi(msg.c_str());
		
		if (port == 0)
			port = 123;
		
		if (BIC_ENV_COMMU::GetNTPS(env)->Run(port)){
			PrintSuccess(env, "NTP server is started at Port", Str_ToStr(port));
			return(cgCommandID);
		}
		
		PrintFail(env);
	}
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
namespace BICNTP {
	static bool32 NTPSync(CMD_ENV* env, const STDSTR& IP, const int32 port, STDSTR* retPackage, double* dTCompensation, double* dT1Ret, double* dT4Ret);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_NTP::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env,cgCommand,"Network Time Protocol");
	if (B_ChkFLAG32(flag, CMD_blPrintSimple))
		return(cgCommandID);
	PrintHelpSubItem(env,"[-c"		,"Clean NTP compensation");
	PrintHelpSubItem(env," |[IP]"	,"NTP server address");
	PrintHelpSubItem(env,"  [PORT]]","NTP server port, default is 123");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_NTP::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	STDSTR		strPar1,strPar2;
	STDSTR		strRec, strH, strR1, strR2, strR3, strTR, strT1, strT2, strT3;
	DTIME		dT1, dTR, dT2, dT3, dT4;
	double		dTCompensation, dT1Ret, dT4Ret;
	int32		port, retCode;
	
	if (msg == "-c"){
		dT1.gCompensation = 0.0;
		PrintSuccess(env);
		return(cgCommandID);
	}
	
	SplitPar1(strPar1,strPar2,msg);

	if (strPar1.length() == 0)
		strPar1 = "time.nist.gov";
	
	if (strPar2.length() == 0)
		strPar2 = "123";
	
	port = atoi(strPar2.c_str());
	
	PrintStrNL(env,COLOR(COL_clPurple,IUD("NTP server:")),strPar1 + "@" + Str_ToStr(port));
	retCode = BICNTP::NTPSync(env, strPar1, port, &strRec, &dTCompensation, &dT1Ret, &dT4Ret);
	if (retCode > 0){
		dT1 = dT1Ret;
		dTR = SYS_NTPToTime(SYS_CharToNTP((uint8*)strRec.c_str() + 16));
		dT2 = SYS_NTPToTime(SYS_CharToNTP((uint8*)strRec.c_str() + 32));
		dT3 = SYS_NTPToTime(SYS_CharToNTP((uint8*)strRec.c_str() + 40));
		dT4 = dT4Ret;
		dT4Ret = ((dT4 - dT1) + (dT3 - dT2)).GetSec();
		
		strH = strRec.substr(0, 4);
		strR1 = strRec.substr(4, 4);
		strR2 = strRec.substr(8, 4);
		strR3 = strRec.substr(12, 4);
		strTR = strRec.substr(16, 8);
		strT1 = strRec.substr(24, 8);
		strT2 = strRec.substr(32, 8);
		strT3 = strRec.substr(40, 8);
#ifdef Output_h
		if (GetSTDOUT(env) != nullptr){
			*GetSTDOUT(env) << Begin() << NL() << COL_clMaroon << "Send 48 bytes:\n"
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
			<< NL() << Str_ASCIIToHEXs(strH, G_ESCAPE_OFF)
			<< NL() << Str_ASCIIToHEXs(strR1, G_ESCAPE_OFF)
			<< NL() << Str_ASCIIToHEXs(strR2, G_ESCAPE_OFF)
			<< NL() << Str_ASCIIToHEXs(strR3, G_ESCAPE_OFF)
			<< NL() << Str_ASCIIToHEXs(strTR, G_ESCAPE_OFF)
			<< NL() << Str_ASCIIToHEXs(strT1, G_ESCAPE_OFF)
			<< NL() << Str_ASCIIToHEXs(strT2, G_ESCAPE_OFF)
			<< NL() << Str_ASCIIToHEXs(strT3, G_ESCAPE_OFF)
			<< NL() << COL_DivLine_Maroon
			<< COL_clPurple << "T1: " << COL_clBlack << SYS_NTPToHEXStr(SYS_TimeToNTP(dT1Ret)) << COL_clPurple << dT1.Format(" YYYY/MM/DD,hh:mm:ss.zzz\n")
			<< COL_clPurple << "TR: " << COL_clBlue << Str_ASCIIToHEXs(strTR, G_ESCAPE_OFF) << COL_clPurple << dTR.Format(" YYYY/MM/DD,hh:mm:ss.zzz\n")
			<< COL_clPurple << "T2: " << COL_clBlue << Str_ASCIIToHEXs(strT2, G_ESCAPE_OFF) << COL_clPurple << dT2.Format(" YYYY/MM/DD,hh:mm:ss.zzz\n")
			<< COL_clPurple << "T3: " << COL_clBlue << Str_ASCIIToHEXs(strT3, G_ESCAPE_OFF) << COL_clPurple << dT3.Format(" YYYY/MM/DD,hh:mm:ss.zzz\n")
			<< COL_clPurple << dT4.Format("T4:                          YYYY/MM/DD,hh:mm:ss.zzz\n\n")
			<< "Network delay        [(T4 - T1) - (T2 - T3)]:  " << Str_FloatToStr(dT4Ret,1,5,4) << "s.\n"
			<< "Compensation   [((T2 - T1) + (T3 - T4)) / 2]:  " << Str_FloatToStr(dTCompensation,1,5,4) << "s.\n\n"
			<< Endl();
		};
#endif
		dT1.gCompensation += dTCompensation;
	}
	else if (retCode == -1){
		PrintFail(env,"Connection timeout");
	}
	else if (retCode == 0){
		PrintFail(env,"Receive error");
	}
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
typedef COMMU_DRV_T<TMEM|TBIRDGE,COMMU_DRV,CORE_SOCKET>	NTP_SOCKET;
static bool32 BICNTP::NTPSync(CMD_ENV* env, const STDSTR& IP, const int32 port, STDSTR* retPackage, double* dTCompensation, double* dT1Ret, double* dT4Ret){
	STDSTR		strRec;
	DTIME		dT1, dT2, dT3, dT4;
	bool32		blTimeout, retCode;
	uint8		buffer[48];
	NTP_SOCKET	cASOCKET(1024,1024,nullptr);
	SYS_TIME_S	timeS;
	
	cASOCKET.Open(SetOpenPar(OPEN_UDP,IP, port, 0));
	
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
	
	cASOCKET.Send(IUD(buffer,48),0);

	SYS_Delay_SetTS(&timeS, 1 * 1000);//10s
	
	strRec = "";
	blTimeout = G_FALSE;
	BIC_BASE::SetInPressAnyKeyMode(env);
	while (CMD_BASE::IsExit(env) == G_FALSE){
		uint8	chkey;
		cASOCKET.Read(&strRec, -1);
		dT4.Now();
		if (strRec.length() >= 48)
			break;
		blTimeout = SYS_Delay_IsTimeout(&timeS);
		if (blTimeout)
			break;
		chkey = BIC_BASE::ReadChar(env,G_FALSE);
		if (chkey == 27){
			break;
		}
		else if (chkey == '\r'){
			BIC_BASE::PrintEnable(env);
		}
	}
	BIC_BASE::ClrInPressAnyKeyMode(env);
	
	*dTCompensation = 0.0;
	*dT4Ret = 0.0;
	*dT1Ret = 0.0;
	*retPackage = "";
	retCode = 0;
	if (strRec.length() >= 48){
		*retPackage = strRec;
		dT2 = SYS_NTPToTime(SYS_CharToNTP((uint8*)strRec.c_str() + 32));
		dT3 = SYS_NTPToTime(SYS_CharToNTP((uint8*)strRec.c_str() + 40));
		//dTRet = (dT4->GetSec() - dT1.GetSec()) + (dT3.GetSec() - dT2.GetSec());			//Network delay [(T4 - T1) - (T2 - T3)]
		*dTCompensation = (dT2.GetSec() - dT1.GetSec() + dT3.GetSec() - dT4.GetSec()) / 2;	//Compensation [((T2 - T1) + (T3 - T4)) / 2]
		*dT4Ret = dT4.GetSec();
		*dT1Ret = dT1.GetSec();
		retCode = G_TRUE;
	}
	else if (blTimeout){
		retCode = -1;
	}
	cASOCKET.Close();
	return(retCode);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif /* BIC_NTP_h */
