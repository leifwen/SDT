/*
 * Copyright (c) 2012-2014
 * All rights reserved.
 *
 * File Name: BIC_Other.cpp
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2014.01.25
 
 */
//------------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "BIC_Other.h"
#include "Comm_File.h"
#include "Comm_Convert.h"
#include "License_SSLSocket.h"
#include "License_Checker.h"
#include "Commu_RMS.h"
#include "AppLayer.h"
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_NEWRECORD::Help(BICPAR *tBICPAR,int32 blDetail)const{
	PrintHelpItem(tBICPAR,cgCommand,"Create new record file.");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_NEWRECORD::Command(BICPAR *tBICPAR,const std::string &par,std::string *ret)const{
	*ret = "";
	if (par.length() > 0)
		return(Help(tBICPAR));
	
	std::string	strTempData;
	
	if (tBICPAR->sdtApp->m_Device.cgODevList.cgOutput != nullptr){
		tBICPAR->oDevNode->Enable();
		
		tBICPAR->sdtApp->m_Device.cgODevList.cgOutput->Enable();
		tBICPAR->sdtApp->m_Device.cgODevList.cgOutput->CreateODevFile(ODEV_CreateNewLOGFileName());
		tBICPAR->sdtApp->m_Device.cgODevList.cgOutput->Spin_InUse_set();
		tBICPAR->sdtApp->m_Device.cgODevList.cgOutput->WriteDividingLine(RICH_CF_clMaroon);
		strTempData = SYS_MakeTimeNow();
		strTempData += " Set new record file:\r\n";
		tBICPAR->sdtApp->m_Device.cgODevList.cgOutput->WriteStrN(strTempData,RICH_CF_clPurple);
		
		strTempData = tBICPAR->sdtApp->m_Device.cgODevList.cgOutput->cODevFileTXT->GetFileName();
		tBICPAR->sdtApp->m_Device.cgODevList.cgOutput->WriteStr(strTempData,RICH_CF_clBlue);
		tBICPAR->sdtApp->m_Device.cgODevList.cgOutput->WriteDividingLine(RICH_CF_clMaroon);
		tBICPAR->sdtApp->m_Device.cgODevList.cgOutput->Spin_InUse_clr();
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_NTP::NTPSync(BICPAR *tBICPAR, const std::string &IP, const int32 port, std::string *retPackage, double *dTCompensation, double *dT1Ret, double *dT4Ret)const{
	std::string		strRec;
	SYS_DateTime	dT1, dT2, dT3, dT4;
	int32			blTimeout, retCode;
	uint8			buffer[48];
	APISocket		cAPISocket(nullptr, 1024);
	SYS_TIME_S		timeS;
	
	cAPISocket.OpenD(IP, port, COMMU_DBUF::CSType_UDP, 0);
	/*
	 strH = "7B 01 00 00";
	 strR1 = "00 00 00 00";
	 strR2 = "00 00 00 00";
	 strR3 = "00 00 00 00";
	 strTR = "00 00 00 00 00 00 00 00";
	 strT1 = "00 00 00 00 00 00 00 00";
	 strT2 = "00 00 00 00 00 00 00 00";
	 strT3 = "00 00 00 00 00 00 00 00";
	 */
	
	buffer[0] = 0;
	while (++buffer[0] < 48){
		buffer[buffer[0]] = 0;
	};
	buffer[0] = 0x7B;
	buffer[1] = 0x01;
	
	dT1.Now();
	SYS_NTPToChar(SYS_TimeToNTP(dT1.GetSec()),buffer + 24);
	
	cAPISocket.Write(buffer,48);
	
	SYS_Delay_SetTS(&timeS, 10 * 1000);//10s
	
	strRec = "";
	blTimeout = 0;
	while (tBICPAR->blExit == 0){
		uint8	chkey;
		cAPISocket.ReadInASCII(&strRec, G_ESCAPE_OFF);
		dT4.Now();
		if (strRec.length() >= 48)
			break;
		blTimeout = SYS_Delay_CheckTS(&timeS);
		if (blTimeout != 0)
			break;
		tBICPAR->blInPressKeyMode = 1;
		chkey = BI_ReadChar(tBICPAR,0);
		if (chkey == 27){
			break;
		}
		else if (chkey == '\r'){
			tBICPAR->oDevNode->Enable();
		}
	}
	tBICPAR->blInPressKeyMode = 0;
	
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
	cAPISocket.CloseD();
	return(retCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_NTP::Help(BICPAR *tBICPAR,int32 blDetail)const{
	PrintHelpItem(tBICPAR,cgCommand,"Network Time Protocol.");
	if (blDetail == 0)
		return(cgReturnCode);
	PrintHelpItem(tBICPAR,"     [-c"		,"Clean NTP compensation.");
	PrintHelpItem(tBICPAR,"     |[IP]"		,"NTP server address.");
	PrintHelpItem(tBICPAR,"      [PORT]]"	,"NTP server port, default is 123.");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_NTP::Command(BICPAR *tBICPAR,const std::string &par,std::string *ret)const{
	*ret = "";
	
	std::string		strPar1,strPar2;
	std::string		strRec, strH, strR1, strR2, strR3, strTR, strT1, strT2, strT3;
	SYS_DateTime	dT1, dTR, dT2, dT3, dT4;
	double			dTCompensation, dT1Ret, dT4Ret;
	int32			port, retCode;
	
	if (par == "-c"){
		dT1.gCompensation = 0.0;
		PrintDoRet(tBICPAR, "OK");
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
	
	PrintStrN(tBICPAR,"NTP server: " + strPar1 + "@" + Str_IntToString(port) + "\r\n",RICH_CF_clPurple);
	retCode = NTPSync(tBICPAR, strPar1, port, &strRec, &dTCompensation, &dT1Ret, &dT4Ret);
	if (retCode > 0){
		dT1 = dT1Ret;
		dTR = SYS_NTPToTime(SYS_CharToNTP((uint8*)strRec.c_str() + 16));
		dT2 = SYS_NTPToTime(SYS_CharToNTP((uint8*)strRec.c_str() + 32));
		dT3 = SYS_NTPToTime(SYS_CharToNTP((uint8*)strRec.c_str() + 40));
		dT4 = dT4Ret;
		
		strH = strRec.substr(0, 4);
		strR1 = strRec.substr(4, 4);
		strR2 = strRec.substr(8, 4);
		strR3 = strRec.substr(12, 4);
		strTR = strRec.substr(16, 8);
		strT1 = strRec.substr(24, 8);
		strT2 = strRec.substr(32, 8);
		strT3 = strRec.substr(40, 8);
		
		PrintStrN(tBICPAR, "Send 48 bytes:\r\n", RICH_CF_clMaroon);
		PrintStrN(tBICPAR, "7B 01 00 00\r\n", RICH_CF_clBlack);
		PrintStrN(tBICPAR, "00 00 00 00\r\n", RICH_CF_clBlack);
		PrintStrN(tBICPAR, "00 00 00 00\r\n", RICH_CF_clBlack);
		PrintStrN(tBICPAR, "00 00 00 00\r\n", RICH_CF_clBlack);
		PrintStrN(tBICPAR, "00 00 00 00 00 00 00 00\r\n", RICH_CF_clBlack);
		PrintStrN(tBICPAR, SYS_NTPToHEXStr(SYS_TimeToNTP(dT1Ret)) + "\r\n", RICH_CF_clBlack);
		PrintStrN(tBICPAR, "00 00 00 00 00 00 00 00\r\n", RICH_CF_clBlack);
		PrintStrN(tBICPAR, "00 00 00 00 00 00 00 00\r\n", RICH_CF_clBlack);
		
		PrintStrN(tBICPAR, "Receive 48 bytes:\r\n", RICH_CF_clRed);
		PrintStrN(tBICPAR, Str_ASCIIToHEXs(strH, G_ESCAPE_OFF) + "\r\n", RICH_CF_clBlue);
		PrintStrN(tBICPAR, Str_ASCIIToHEXs(strR1, G_ESCAPE_OFF) + "\r\n", RICH_CF_clBlue);
		PrintStrN(tBICPAR, Str_ASCIIToHEXs(strR2, G_ESCAPE_OFF) + "\r\n", RICH_CF_clBlue);
		PrintStrN(tBICPAR, Str_ASCIIToHEXs(strR3, G_ESCAPE_OFF) + "\r\n", RICH_CF_clBlue);
		PrintStrN(tBICPAR, Str_ASCIIToHEXs(strTR, G_ESCAPE_OFF) + "\r\n", RICH_CF_clBlue);
		PrintStrN(tBICPAR, Str_ASCIIToHEXs(strT1, G_ESCAPE_OFF) + "\r\n", RICH_CF_clBlue);
		PrintStrN(tBICPAR, Str_ASCIIToHEXs(strT2, G_ESCAPE_OFF) + "\r\n", RICH_CF_clBlue);
		PrintStrN(tBICPAR, Str_ASCIIToHEXs(strT3, G_ESCAPE_OFF) + "\r\n", RICH_CF_clBlue);
		
		PrintStrN(tBICPAR, DEV_LINE_START, RICH_CF_clPurple);
		
		PrintStrN(tBICPAR, "T1: ", RICH_CF_clPurple);
		PrintStr(tBICPAR, SYS_NTPToHEXStr(SYS_TimeToNTP(dT1Ret)), RICH_CF_clBlack);
		PrintStr(tBICPAR, dT1.FormatDateTime(" YYYY/MM/DD,hh:mm:ss.zzz\r\n"), RICH_CF_clPurple);
		
		PrintStrN(tBICPAR, "TR: ", RICH_CF_clPurple);
		PrintStr(tBICPAR, Str_ASCIIToHEXs(strTR, G_ESCAPE_OFF), RICH_CF_clBlue);
		PrintStr(tBICPAR, dTR.FormatDateTime(" YYYY/MM/DD,hh:mm:ss.zzz\r\n"), RICH_CF_clPurple);
		
		PrintStrN(tBICPAR, "T2: ", RICH_CF_clPurple);
		PrintStr(tBICPAR, Str_ASCIIToHEXs(strT2, G_ESCAPE_OFF), RICH_CF_clBlue);
		PrintStr(tBICPAR, dT2.FormatDateTime(" YYYY/MM/DD,hh:mm:ss.zzz\r\n"), RICH_CF_clPurple);
		
		PrintStrN(tBICPAR, "T3: ", RICH_CF_clPurple);
		PrintStr(tBICPAR, Str_ASCIIToHEXs(strT3, G_ESCAPE_OFF), RICH_CF_clBlue);
		PrintStr(tBICPAR, dT3.FormatDateTime(" YYYY/MM/DD,hh:mm:ss.zzz\r\n"), RICH_CF_clPurple);
		
		PrintStrN(tBICPAR, dT4.FormatDateTime("T4:                          YYYY/MM/DD,hh:mm:ss.zzz\r\n\r\n"), RICH_CF_clPurple);
		
		dT4Ret = (dT4.GetSec() - dT1.GetSec()) + (dT3.GetSec() - dT2.GetSec());
		PrintStrN(tBICPAR, "Network delay        [(T4 - T1) - (T2 - T3)]:  " + Str_FloatToString(dT4Ret) + "s.\r\n", RICH_CF_clPurple);
		
		PrintStrN(tBICPAR, "Compensation   [((T2 - T1) + (T3 - T4)) / 2]:  " + Str_FloatToString(dTCompensation) + "s.\r\n\r\n", RICH_CF_clPurple);
		dT1.gCompensation += dTCompensation;
	}
	else if (retCode == -1){
		PrintStrN(tBICPAR, "Connection timeout.\r\n", RICH_CF_clMaroon);
	}
	else if (retCode == 0){
		PrintStrN(tBICPAR, "Receive error.\r\n", RICH_CF_clMaroon);
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_NTPSERVER::Help(BICPAR *tBICPAR, int32 blDetail)const{
	PrintHelpItem(tBICPAR, cgCommand, "Start/Stop NTP server.");
	if (blDetail == 0)
		return(cgReturnCode);
	
	PrintHelpItem(tBICPAR, "     [off"		, "Stop NTP server.");
	PrintHelpItem(tBICPAR, "     |PORT]"	, "NTP server port, default is 123.");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_NTPSERVER::Command(BICPAR *tBICPAR, const std::string &par, std::string *ret)const{
	*ret = "";
	
	int32	port;
	
	if (par == "off"){
		tBICPAR->sdtApp->m_NTPServer.CloseD();
		PrintDoRet(tBICPAR, "NTP server is stop");
		return(cgReturnCode);
	}
	
	port = atoi(par.c_str());
	
	if (port == 0)
		port = 123;
	
	if (tBICPAR->sdtApp->m_NTPServer.Run(port) != 0){
		PrintDoRet(tBICPAR, "NTP server is started at Port " + Str_IntToString(port));
		return(cgReturnCode);
	}
	
	PrintDoRet(tBICPAR, "NTP server started fail at Port " + Str_IntToString(port));
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_TERMINALSERVER::Help(BICPAR *tBICPAR, int32 blDetail)const{
	PrintHelpItem(tBICPAR, cgCommand, "Start/Stop terminal server.");
	if (blDetail == 0)
		return(cgReturnCode);
	
	PrintHelpItem(tBICPAR, "     [off"		, "Stop terminal server.");
	PrintHelpItem(tBICPAR, "     |PORT]"	, "Terminal server port, default is 16384.");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_TERMINALSERVER::Command(BICPAR *tBICPAR, const std::string &par, std::string *ret)const{
	*ret = "";
	
	int32	port;
	
	if (par == "off"){
		tBICPAR->sdtApp->m_TS.CloseD();
		PrintDoRet(tBICPAR, "Terminal server is stop");
		return(cgReturnCode);
	}
	
	port = atoi(par.c_str());
	
	if (port == 0)
		port = 16384;
	
	if (tBICPAR->sdtApp->m_TS.Run(port) != 0){
		PrintDoRet(tBICPAR, "Terminal server is started at Port " + Str_IntToString(port));
		return(cgReturnCode);
	}
	
	PrintDoRet(tBICPAR, "Terminal server started fail at Port " + Str_IntToString(port));
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#ifdef USE_OPENSSL
//------------------------------------------------------------------------------------------//
int32 BIC_PATCHSDT::Help(BICPAR *tBICPAR,int32 blDetail)const{
	PrintHelpItem(tBICPAR,cgCommand,"Add Patch Code to SDT");
	if (blDetail == 0)
		return(cgReturnCode);
	PrintHelpItem(tBICPAR,"     <file>"		,"SDT file name.");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_PATCHSDT::Command(BICPAR *tBICPAR,const std::string &par,std::string *ret)const{
	std::string		strPrint;
	std::string		strPar;
	Reg_SDT_PatchCode	SDTPC;
	
	*ret = "";
	strPar = Str_Trim(par);
	if (par.length() > 0)
		SDTPC.PatchToSDT(strPar,&strPrint);
	
	PrintStrN(tBICPAR,strPrint,RICH_CF_clPurple);
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_APPROVE::Help(BICPAR *tBICPAR,int32 blDetail)const{
	PrintHelpItem(tBICPAR,cgCommand,"Approve registration file");
	if (blDetail == 0)
		return(cgReturnCode);
	PrintHelpItem(tBICPAR,"     <hours>"	,"720H = 1M,8760H = 1Y,867240H = 99Y");
	PrintHelpItem(tBICPAR,"     <file>"		,"PreRegistration.key");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_APPROVE::Command(BICPAR *tBICPAR,const std::string &par,std::string *ret)const{
	std::string		strPar1,strPar2;
	std::string		strReg,strContent;
	Linense_Signature	tLS;

	strPar2 = par;
	strPar1 = Str_Trim(Str_ReadSubItem(&strPar2, " "));
	strPar2 = Str_Trim(strPar2);
	strReg = "";
	
	CFS_ReadFile(&strReg, strPar2);

	if (tLS.Encode(&strContent, strReg, atoi(strPar1.c_str()) * 60 * 60) > 0){
		Str_ReadSubItemR(&strPar2, "/");
		strPar2 += "/License.key";
		CFS_WriteFile(strPar2, strContent);
		PrintDoRet(tBICPAR,"Create " + strPar2 + "successful");
	}
	else{
		PrintDoRet(tBICPAR,"Fail");
	}
	
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_LSERVER::Help(BICPAR *tBICPAR,int32 blDetail)const{
	PrintHelpItem(tBICPAR,cgCommand,"Setup License Server");
	if (blDetail == 0)
		return(cgReturnCode);
	PrintHelpItem(tBICPAR,"     [port]"		,"Server port, default is 16385.");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_LSERVER::Command(BICPAR *tBICPAR,const std::string &par,std::string *ret)const{
	std::string		strPar,strPar1,strPar2,strPrint,strCommand;
	uint8			chkey;
	SYS_DateTime	tNow;
	LicenseSSLServer	m_LServer(G_MAXBUFFER_SIZE);

	*ret = "";
	strPar = Str_Trim(par);
	if (par.length() == 0)
		strPar = "16385";
	if (m_LServer.Run(atoi(strPar.c_str())) > 0){
		strPrint  = "License server is started at Port ";
		strPrint += strPar;
		strPrint += ".\r\n";
		strPrint += "Press Esc to quit and stop the server.\r\n";
		PrintStrN(tBICPAR,strPrint,RICH_LIN_clLightBlue);

		tBICPAR->blUseSecondLH = 1;
		do{
			if (m_LServer.CheckblRequest() > 0){
				tBICPAR->blInPressKeyMode = 0;
				PrintStrN(tBICPAR,m_LServer.RequestSocketInfo(&strPrint),RICH_CF_clPurple);
				PrintStrN(tBICPAR,"Input reject or approve [hours] to response registration.\r\n",RICH_LIN_clLightBlue);
				PrintStrN(tBICPAR,"[720H = 1M,8760H = 1Y,867240H = 99Y].\r\n",RICH_LIN_clLightBlue);
				while(BI_ReadCommand(tBICPAR) > 0){
					strCommand = Str_LowerCase(Str_Trim(Str_ReadSubItem(&tBICPAR->retCommand,"\r")));
					tBICPAR->retCommand = "";
					if (strCommand.length() > 0){
						if ((strCommand == "reject") || (strCommand == "r")){
							tNow.Now();
							m_LServer.RejectRegistration();
							PrintStrN(tBICPAR,tNow.FormatDateTime("[hh:mm:ss.zzz]  \"Reject registration\".\r\n"),RICH_CF_clPurple);
							break;
						}
						else {
							strPar2 = Str_Trim(strCommand);
							strPar1 = Str_Trim(Str_ReadSubItem(&strPar2," "));
							if ((strPar1 == "approve") || (strPar1 == "a")){
								tNow.Now();
								m_LServer.ApproveRegistration(atoi(strPar2.c_str()) * 60 * 60);
								PrintStrN(tBICPAR,tNow.FormatDateTime("[hh:mm:ss.zzz]  \"Send approve Licence\".\r\n"),RICH_CF_clPurple);
								break;
							}
						}
					}
					SYS_SleepMS(10);
				}
			}
			tBICPAR->blInPressKeyMode = 1;
			chkey = BI_ReadChar(tBICPAR,0);
			if (chkey == 27){
				break;
			}
			else if (chkey == '\r'){
				tBICPAR->oDevNode->Enable();
			}
			SYS_SleepMS(10);
		}while(1);
		tBICPAR->blUseSecondLH = 0;
		m_LServer.CloseD();
	}
	tBICPAR->blInPressKeyMode = 0;
	PrintDoRet(tBICPAR,"License server is stopped");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_CREGREPORT::Help(BICPAR *tBICPAR,int32 blDetail)const{
	PrintHelpItem(tBICPAR,cgCommand,"Create \"PreRegistration.key\" file.");
	if (blDetail == 0)
		return(cgReturnCode);
	PrintHelpItem(tBICPAR,"     [IP]"		,"Server address.");
	PrintHelpItem(tBICPAR,"     [PORT]"		,"Server port, default is 16385.");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_CREGREPORT::Command(BICPAR *tBICPAR,const std::string &par,std::string *ret)const{
	std::string			strPrint,strReg;
	std::string			strPar1,strPar2;
	LicenseSSLSocket	lSocket(nullptr, G_MAXBUFFER_SIZE);
	int32				retUpdate;
	*ret = "";
	
	if (par.length() == 0){
		REG_CreateREGFILE(&strPrint);
	}
	else{
		strPar2 = Str_Trim(par);
		strPar1 = Str_ReadSubItem(&strPar2," ");
		Str_LTrimSelf(strPar2);
		
		if (strPar2.length() == 0)
			strPar2 = "16385";
		
		PrintStrN(tBICPAR,"In doing pls wait...\r\n",RICH_CF_clPurple);
		if (lSocket.OpenD(strPar1, atoi(strPar2.c_str()), COMMU_DBUF::CSType_TCP, 0) > 0){
			PrintStrN(tBICPAR,"Connect to server successful.\r\n",RICH_CF_clPurple);
			if (lSocket.CreateRegSignature(&strReg,&strPrint) > 0){
				PrintStrN(tBICPAR,"Create registration data successful.\r\n",RICH_CF_clPurple);
				PrintStrN(tBICPAR,"Send registration data to server.\r\n",RICH_CF_clPurple);
				retUpdate = lSocket.SendRequestLicense(strReg,20);
				if (retUpdate > 0){
					strPrint = "Update License.key successful.\r\n";
				}
				else if (retUpdate == 0){
					strPrint = "Registration is rejected.\r\n";
				}
				else{
					strPrint = "Timeout for getting License.key.\r\n";
				}
			}
		}
		else{
			strPrint = "Connect server fail.\r\n";
		}
	}
	lSocket.CloseD();
	PrintStrN(tBICPAR,strPrint,RICH_CF_clPurple);
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_RST_ON::Help(BICPAR *tBICPAR, int32 blDetail)const{
	PrintHelpItem(tBICPAR, cgCommand, "Start remote SSL terminal server.");
	if (blDetail == 0)
		return(cgReturnCode);
	
	PrintHelpItem(tBICPAR, "     [PORT]"	, "RST server control port, default is 16384.");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_RST_ON::Command(BICPAR *tBICPAR, const std::string &par, std::string *ret)const{
	*ret = "";
	
	int32	port;
	
	if (tBICPAR->sdtApp->m_RST.CheckblConnected() > 0){
		PrintDoRet(tBICPAR, "RST server is start");
		return(cgReturnCode);
	}
	
	if (par.length() == 0){
		port = 16384;
	}
	else{
		port = atoi(par.c_str());
	}
	
	if (port == 0){
		PrintDoRet(tBICPAR, "Input ERROR");
		return(cgReturnCode);
	}
	
	if (tBICPAR->sdtApp->m_RST.Run(port) > 0){
		PrintDoRet(tBICPAR, "RST server is started at Port " + Str_IntToString(port));
	}
	else{
		PrintDoRet(tBICPAR, "RST server started fail at Port " + Str_IntToString(port));
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_RST_OFF::Help(BICPAR *tBICPAR, int32 blDetail)const{
	PrintHelpItem(tBICPAR, cgCommand, "Stop remote SSL terminal server.");
	if (blDetail == 0)
		return(cgReturnCode);
	
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_RST_OFF::Command(BICPAR *tBICPAR, const std::string &par, std::string *ret)const{
	*ret = "";
	
	tBICPAR->sdtApp->m_RST.CloseD();
	PrintDoRet(tBICPAR, "RST server is stopped");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_RST_LINK::Help(BICPAR *tBICPAR, int32 blDetail)const{
	PrintHelpItem(tBICPAR, cgCommand, "Link to RST server control port.");
	if (blDetail == 0)
		return(cgReturnCode);
	
	PrintHelpItem(tBICPAR, "     [off"				, "Disconnect RST server.");
	PrintHelpItem(tBICPAR, "     |<RMS IP>"			, "RST server IP, default is leifwen.com.");
	PrintHelpItem(tBICPAR, "      [control PORT]]"	, "RST server control port, default is 16384.");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_RST_LINK::Command(BICPAR *tBICPAR, const std::string &par, std::string *ret)const{
	*ret = "";
	
	int32	port;
	std::string	strIP,strPar;
	
	if (par == "off"){
		tBICPAR->sdtApp->m_RSTCilent.CloseD();
		PrintDoRet(tBICPAR, "Control connection close");
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
	
	if ((tBICPAR->sdtApp->m_RSTCilent.IsConnected() == 0) || (tBICPAR->sdtApp->m_RSTCilent.CheckHandshake() == 0))
		tBICPAR->sdtApp->m_RSTCilent.CloseD();
	if ((tBICPAR->sdtApp->m_RSTCilent.IsConnected() > 0) || (tBICPAR->sdtApp->m_RSTCilent.OpenD(strIP, port, COMMU_DBUF::CSType_TCP, 0) > 0)){
		PrintDoRet(tBICPAR, "Control connection is active");
	}
	else{
		PrintDoRet(tBICPAR, "Connect to RST server fail");
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_RST_LOGIN::Help(BICPAR *tBICPAR, int32 blDetail)const{
	PrintHelpItem(tBICPAR, cgCommand, "Login control terminal.");
	if (blDetail == 0)
		return(cgReturnCode);
	
	PrintHelpItem(tBICPAR,"","Double press ESC + ESC to exit.");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_RST_LOGIN::Command(BICPAR *tBICPAR, const std::string &par, std::string *ret)const{
	*ret = "";
	
	uint8	chKey;
	std::string		sendData;
	
	if ((tBICPAR->sdtApp->m_RSTCilent.IsConnected() == 0) || (tBICPAR->sdtApp->m_RSTCilent.CheckHandshake() == 0)){
		PrintDoRet(tBICPAR, "RST server control link was not setup");
		return(cgReturnCode);
	}
	
	if (tBICPAR->sdtApp->m_RSTCilent.SendRequestSetupTerminal() == 0){
		PrintDoRet(tBICPAR, "Request fail");
		return(cgReturnCode);
	}
	
	tBICPAR->blInOnlineMode = 1;
	tBICPAR->blUseSecondLH = 1;
	if (tBICPAR->blDisplayAuto != 0)
		tBICPAR->oDevNode->Enable();
	sendData = "";
	chKey = 0;
	while((tBICPAR->blExit == 0) && (tBICPAR->sdtApp->m_RSTCilent.IsConnected() > 0)
		  && (tBICPAR->sdtApp->m_RSTCilent.CheckRemoteTerminalStatus() == 0)){
		if (tBICPAR->sdtApp->m_RSTCilent.SSLRead(&sendData) > 0){
			PrintStr(tBICPAR, sendData, RICH_LIN_clCyan);
			sendData = "";
		}
		
		SYS_SleepMS(10);
		chKey = BI_ReadChar(tBICPAR,0);
		if ((chKey >= 32) && (chKey <= 126)){
			sendData += chKey;
		}
		else if (chKey == '\r'){
			sendData += chKey;
			tBICPAR->sdtApp->m_RSTCilent.SSLWrite(sendData);
			sendData = "";
		}
		else if (chKey == 27){
			break;
		}
		else{
			if (chKey == 0x80)
				chKey = 27;
			if ((chKey > 0) && (chKey <= 31)){
				std::string strT;
				strT = '^';
				strT += (chKey + 0x40);
				tBICPAR->sdtApp->m_RSTCilent.SSLWrite((uint8*)&chKey,1);
			}
		}
	}
	tBICPAR->sdtApp->m_RSTCilent.SendRequestCloseTerminal();
	PrintStrN(tBICPAR, "\r\n", RICH_CF_clBlack);
	
	tBICPAR->blInOnlineMode = 0;
	tBICPAR->blUseSecondLH = 0;
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_RST_MAPPING::Help(BICPAR *tBICPAR, int32 blDetail)const{
	PrintHelpItem(tBICPAR, cgCommand, "Configure mapping server.");
	if (blDetail == 0)
		return(cgReturnCode);
	
	PrintHelpItem(tBICPAR, "     [off]"				, "Close mapping server.");
	PrintHelpItem(tBICPAR, "     [RST PORT]"		, "RST server listen PORT.");
	PrintHelpItem(tBICPAR, "     [-tcp|-udp]"		, "Mapping mode TCP/UDP, default is -tcp.");
	PrintHelpItem(tBICPAR, "     [mapping PORT]"	, "Mapping server PORT, default is RST PORT.");
	PrintHelpItem(tBICPAR, "     [mapping IP]"		, "Mapping server IP, default is 127.0.0.1.");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_RST_MAPPING::Command(BICPAR *tBICPAR, const std::string &par, std::string *ret)const{
	*ret = "";
	
	int32	remotePort,mappingPort;
	std::string	strlMappingIP,strType;
	
	if ((tBICPAR->sdtApp->m_RSTCilent.IsConnected() == 0) || (tBICPAR->sdtApp->m_RSTCilent.CheckHandshake() == 0)){
		PrintDoRet(tBICPAR, "RST server control link was not setup");
		return(cgReturnCode);
	}
	
	if (par.length() == 0){
		if (tBICPAR->sdtApp->m_RSTCilent.SendRequestListMServer(&strlMappingIP) > 0){
			PrintStrN(tBICPAR,DEV_LINE_START,RICH_LIN_clDefault);
			PrintStrN(tBICPAR,strlMappingIP,RICH_LIN_clCyan);
			PrintStrN(tBICPAR,DEV_LINE_START,RICH_LIN_clDefault);
		}
		else{
			PrintDoRet(tBICPAR, "Request fail");
		}
		return(cgReturnCode);
	}
	
	strlMappingIP = Str_Trim(par);
	strType = Str_Trim(Str_ReadSubItem(&strlMappingIP, " "));
	strlMappingIP = Str_Trim(strlMappingIP);
	if (strType == "off"){
		if (strlMappingIP.length() == 0){
			if (tBICPAR->sdtApp->m_RSTCilent.SendRequestOffMServer(0) > 0){
				PrintDoRet(tBICPAR, "All mapping server are off");
			}
			else{
				PrintDoRet(tBICPAR, "Request fail");
			}
		}
		else{
			remotePort = atoi(strlMappingIP.c_str());
			if (tBICPAR->sdtApp->m_RSTCilent.SendRequestOffMServer(remotePort) > 0){
				strlMappingIP = "Mapping server(";
				strlMappingIP += Str_IntToString(remotePort);
				strlMappingIP += ") is off";
				PrintDoRet(tBICPAR, strlMappingIP);
			}
			else{
				PrintDoRet(tBICPAR, "Request fail");
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
	
	if ((tBICPAR->sdtApp->m_RSTCilent.IsConnected() > 0)
		&& (tBICPAR->sdtApp->m_RSTCilent.CheckHandshake() > 0)){
		if (tBICPAR->sdtApp->m_RSTCilent.SendRequestSetupMServer(remotePort, strlMappingIP, mappingPort
																 ,(strType == "-tcp") ? COMMU_DBUF::CSType_TCP : COMMU_DBUF::CSType_UDP) > 0){
			PrintDoRet(tBICPAR, "Mapping server is started at Port " + Str_IntToString(remotePort));
		}
		else{
			PrintDoRet(tBICPAR, "Mapping server started fail at Port " + Str_IntToString(remotePort));
		}
	}
	else{
		PrintDoRet(tBICPAR, "RST control link was not setup");
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
#endif
//------------------------------------------------------------------------------------------//
