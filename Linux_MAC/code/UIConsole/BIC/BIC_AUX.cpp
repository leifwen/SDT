/*
 * Copyright (c) 2012-2014
 * All rights reserved.
 *
 * File Name: BIC_AUX.cpp
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2014.01.25
 
 */
//------------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "BIC_AUX.h"
#include "Comm_Convert.h"
#include "AppLayer.h"
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_AUX_COM::Help(BICPAR *tBICPAR,int32 blDetail)const{
	PrintHelpItem(tBICPAR,cgCommand,"Set auxiliary COM name and baudrate.");
	if (blDetail == 0)
		return(cgReturnCode);
	PrintHelpItem(tBICPAR,"     [name][br]","Auxiliary COM name and baudrate.");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_AUX_COM::Command(BICPAR *tBICPAR, const std::string &par,std::string *ret)const{
	int32 baud;
	std::string		strPar1,strPar2;
	*ret = "";
	strPar2 = Str_Trim(par);
	strPar1 = Str_ReadSubItem(&strPar2," ");
	Str_LTrimSelf(strPar2);
	
	if (tBICPAR->sdtApp->m_Device2.CheckblConnect() == 0){
		if(par.length() > 0){
			tBICPAR->sdtApp->m_Device2.cEDevFlag.com = strPar1;
			if (strPar2.length() > 0){
				baud = atoi(strPar1.c_str());
				if (baud < 0)
					baud = 0;
				tBICPAR->sdtApp->m_Device2.cEDevFlag.baudrate = baud;
			}
		}
	}
	PrintStrN(tBICPAR," " + tBICPAR->sdtApp->m_Device2.cEDevFlag.com + "@" + Str_IntToString(tBICPAR->sdtApp->m_Device2.cEDevFlag.baudrate),RICH_LIN_clDefault);
	
	if (tBICPAR->sdtApp->m_Device2.CheckblConnect() == 0){
		PrintStr(tBICPAR,", closed.\n",RICH_LIN_clDefault);
	}
	else{
		PrintStr(tBICPAR,", open.\n",RICH_LIN_clDefault);
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_AUX_BR::Help(BICPAR *tBICPAR,int32 blDetail)const{
	PrintHelpItem(tBICPAR,cgCommand,"Set auxiliary COM baudrate.");
	if (blDetail == 0)
		return(cgReturnCode);
	PrintHelpItem(tBICPAR,"     <n>","Auxiliary COM baudrate.");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_AUX_BR::Command(BICPAR *tBICPAR, const std::string &par,std::string *ret)const{
	int32 baud;
	*ret = "";
	
	if(par.length() > 0){
		baud = atoi(par.c_str());
		if (baud < 0)
			baud = 0;
		tBICPAR->sdtApp->m_Device2.cEDevFlag.baudrate = baud;
		if (tBICPAR->sdtApp->m_Device2.cgAPIECom != nullptr)
			tBICPAR->sdtApp->m_Device2.cgAPIECom->SetBaudrate(baud);
	}
	
	BIC_AUX_COM::Command(tBICPAR,"",ret);
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_AUX_REC::Help(BICPAR *tBICPAR,int32 blDetail)const{
	PrintHelpItem(tBICPAR,cgCommand,"Set receive data report parameter.");
	if (blDetail == 0)
		return(cgReturnCode);
	PrintHelpItem(tBICPAR,"     [-m<H|A>]"		,"Receive mode: Hex/ASCII.");
	PrintHelpItem(tBICPAR,"     [-E<on|off>]"	,"Enable/disable escape.");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_AUX_REC::Command(BICPAR *tBICPAR, const std::string &par,std::string *ret)const{
	int32 blcheck;
	std::string		strCommand,strPar1,strPar2;
	*ret = "";
	strCommand = Str_Trim(par);
	blcheck = 0;
	do{
		strPar1 = Str_ReadSubItem(&strCommand," ");
		Str_LTrimSelf(strCommand);
		if (strPar1 == "-mH"){
			B_SetFLAG64(tBICPAR->sdtApp->m_Device2.cgODevList.cgODevFlagU64, ODEV_FLAG_EnHEXViewMode);
			blcheck = 1;
		}
		else if (strPar1 == "-mA"){
			B_ClrFLAG64(tBICPAR->sdtApp->m_Device2.cgODevList.cgODevFlagU64, ODEV_FLAG_EnHEXViewMode);
			blcheck = 1;
		}
		
		else if (strPar1 == "-Eon"){
			B_SetFLAG64(tBICPAR->sdtApp->m_Device2.cgODevList.cgODevFlagU64, ODEV_FLAG_EnEscape);
			blcheck = 1;
		}
		else if (strPar1 == "-Eoff"){
			B_ClrFLAG64(tBICPAR->sdtApp->m_Device2.cgODevList.cgODevFlagU64, ODEV_FLAG_EnEscape);
			blcheck = 1;
		}
	}while(strCommand.length() > 0);
	
	if ((par.length() > 0) && (blcheck == 0))
		return(Help(tBICPAR));
	
	if (B_ChkFLAG64(tBICPAR->sdtApp->m_Device2.cgODevList.cgODevFlagU64, ODEV_FLAG_EnHEXViewMode) == 0){
		PrintStrN(tBICPAR," ASCII mode,",RICH_LIN_clDefault);
	}
	else{
		PrintStrN(tBICPAR," HEX mode,",RICH_LIN_clDefault);
	}
	if (B_ChkFLAG64(tBICPAR->sdtApp->m_Device2.cgODevList.cgODevFlagU64, ODEV_FLAG_EnEscape) == 0){
		PrintStr(tBICPAR," disable escape.\n",RICH_LIN_clDefault);
	}
	else{
		PrintStr(tBICPAR," enable escape.\n",RICH_LIN_clDefault);
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_AUX_OPEN::Help(BICPAR *tBICPAR,int32 blDetail)const{
	PrintHelpItem(tBICPAR,cgCommand,"Open auxiliary COM.");
	if (blDetail == 0)
		return(cgReturnCode);
	PrintHelpItem(tBICPAR,"     [-v]","Open as virtual COM.");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_AUX_OPEN::Command(BICPAR *tBICPAR, const std::string &par,std::string *ret)const{
	*ret = "";
	if (par.length() == 0){
		tBICPAR->sdtApp->m_Device2.cEDevFlag.comType = 0;
		if (tBICPAR->sdtApp->m_Device2.CheckblConnect() == 0){
			tBICPAR->sdtApp->m_Device2.Open(DEVICE::OP_COM,tBICPAR->sdtApp->m_Device2.cEDevFlag.com,tBICPAR->sdtApp->m_Device2.cEDevFlag.baudrate,0);
		}
		else{
			BIC_AUX_COM::Command(tBICPAR,"",ret);
		}
	}
#ifdef CommonDefH_Unix
	else if (par == "-v"){
		tBICPAR->sdtApp->m_Device2.cEDevFlag.comType = 1;
		if (tBICPAR->sdtApp->m_Device2.CheckblConnect() == 0){
			if (tBICPAR->sdtApp->m_Device2.Open(DEVICE::OP_COM,tBICPAR->sdtApp->m_Device2.cEDevFlag.com,tBICPAR->sdtApp->m_Device2.cEDevFlag.baudrate,0) == 0){
				tBICPAR->sdtApp->m_Device2.Close(0);
				tBICPAR->sdtApp->m_Device2.cEDevFlag.comType = 0;
				unlink(tBICPAR->sdtApp->m_Device2.cEDevFlag.com.c_str());
				PrintStrN(tBICPAR,DEV_LINE_START,RICH_LIN_clDefault);
				PrintStrN(tBICPAR," Create virtual COM ",RICH_LIN_clCyan);
				PrintStr(tBICPAR,tBICPAR->sdtApp->m_Device2.cEDevFlag.com,RICH_LIN_clRed);
				PrintStr(tBICPAR," fail.\n",RICH_LIN_clCyan);
				PrintStrN(tBICPAR,DEV_LINE_START,RICH_LIN_clDefault);
			}
		}
		else{
			BIC_AUX_COM::Command(tBICPAR,"",ret);
		}
	}
#endif
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_AUX_CLOSE::Help(BICPAR *tBICPAR,int32 blDetail)const{
	PrintHelpItem(tBICPAR,cgCommand,"Close auxiliary COM.");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_AUX_CLOSE::Command(BICPAR *tBICPAR, const std::string &par,std::string *ret)const{
	*ret = "";
	
	tBICPAR->sdtApp->m_Device2.Close(0);
	BIC_AUX_COM::Command(tBICPAR,"",ret);
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_AUX_DTR::Help(BICPAR *tBICPAR,int32 blDetail)const{
	PrintHelpItem(tBICPAR,cgCommand,"Set auxiliary COM DTR.");
	if (blDetail == 0)
		return(cgReturnCode);
	PrintHelpItem(tBICPAR,"     [-H<t>]","Set to High, t is time, unit is S.");
	PrintHelpItem(tBICPAR,"     [-L<t>]","Set to Low, t is time, unit is S.");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_AUX_DTR::Command(BICPAR *tBICPAR,const std::string &par,std::string *ret)const{
	std::string		strPrintData,strPar1,strPar2,strSub;
	double			timeMs;
	SYS_TIME_S		timeS;
	
	*ret = "";
	if ((tBICPAR->sdtApp->m_Device2.cgDevType == DEVICE::DEVID_APICOM) && (tBICPAR->sdtApp->m_Device2.CheckblConnect() != 0)){
		if (par.length() == 0){
			if (tBICPAR->sdtApp->m_Device2.cgAPIECom->DTR == 0){
				PrintStrN(tBICPAR," DTR is high.\n",RICH_LIN_clDefault);
			}
			else{
				PrintStrN(tBICPAR," DTR is low.\n",RICH_LIN_clDefault);
			}
		}
		else{
			strPar2 = par;
			while(strPar2.length() > 0){
				strPar1 = Str_Trim(Str_ReadSubItem(&strPar2, " "));
				if (strPar1 == "-H"){
					tBICPAR->sdtApp->m_Device2.cgAPIECom->SetDTRToHigh();
					strPrintData = SYS_MakeTimeNow();
					strPrintData += " Set Aux DTR to high\r\n";
					if (tBICPAR->sdtApp->m_Device2.cgODevList.cgOutput != nullptr){
						tBICPAR->sdtApp->m_Device2.cgODevList.cgOutput->Spin_InUse_set();
						tBICPAR->sdtApp->m_Device2.cgODevList.cgOutput->WriteDividingLine(RICH_CF_clMaroon,COLSTRING::COL_EP_YES);
						tBICPAR->sdtApp->m_Device2.cgODevList.cgOutput->WriteStrN(strPrintData,RICH_CF_clPurple,COLSTRING::COL_EP_YES);
						tBICPAR->sdtApp->m_Device2.cgODevList.cgOutput->WriteDividingLine(RICH_CF_clMaroon,COLSTRING::COL_EP_YES);
						tBICPAR->sdtApp->m_Device2.cgODevList.cgOutput->Spin_InUse_clr();
					}
					return(cgReturnCode);
				}
				else if (strPar1 == "-L"){
					tBICPAR->sdtApp->m_Device2.cgAPIECom->SetDTRToLow();
					strPrintData = SYS_MakeTimeNow();
					strPrintData += " Set Aux DTR to Low\r\n";
					if (tBICPAR->sdtApp->m_Device2.cgODevList.cgOutput != nullptr){
						tBICPAR->sdtApp->m_Device2.cgODevList.cgOutput->Spin_InUse_set();
						tBICPAR->sdtApp->m_Device2.cgODevList.cgOutput->WriteDividingLine(RICH_CF_clMaroon,COLSTRING::COL_EP_YES);
						tBICPAR->sdtApp->m_Device2.cgODevList.cgOutput->WriteStrN(strPrintData,RICH_CF_clPurple,COLSTRING::COL_EP_YES);
						tBICPAR->sdtApp->m_Device2.cgODevList.cgOutput->WriteDividingLine(RICH_CF_clMaroon,COLSTRING::COL_EP_YES);
						tBICPAR->sdtApp->m_Device2.cgODevList.cgOutput->Spin_InUse_clr();
					}
					return(cgReturnCode);
				}
				else if(strPar1.length() > 2){
					strSub = strPar1.substr(0,2);
					if (strSub == "-H"){
						strSub = strPar1.substr(2);
						timeMs = atof(strSub.c_str());
						tBICPAR->sdtApp->m_Device2.cgAPIECom->SetDTRToHigh();
						strPrintData = SYS_MakeTimeNow();
						SYS_Delay_SetTS(&timeS,timeMs * 1000);
						strPrintData += " Set Aux DTR to high\r\n";
						if (tBICPAR->sdtApp->m_Device2.cgODevList.cgOutput != nullptr){
							tBICPAR->sdtApp->m_Device2.cgODevList.cgOutput->Spin_InUse_set();
							tBICPAR->sdtApp->m_Device2.cgODevList.cgOutput->WriteDividingLine(RICH_CF_clMaroon,COLSTRING::COL_EP_YES);
							tBICPAR->sdtApp->m_Device2.cgODevList.cgOutput->WriteStrN(strPrintData,RICH_CF_clPurple,COLSTRING::COL_EP_YES);
							tBICPAR->sdtApp->m_Device2.cgODevList.cgOutput->WriteDividingLine(RICH_CF_clMaroon,COLSTRING::COL_EP_YES);
							tBICPAR->sdtApp->m_Device2.cgODevList.cgOutput->Spin_InUse_clr();
						}
						
						while ((tBICPAR->blExit == 0) && (SYS_Delay_CheckTS(&timeS) == 0)){
							tBICPAR->blInPressKeyMode = 1;
							if (BI_ReadChar(tBICPAR,0) == 27){
								PrintStrN(tBICPAR,"Pressed ESC, stop execute command.\r\n",RICH_LIN_clRed);
								tBICPAR->blInPressKeyMode = 0;
								return(cgReturnCode);
							}
						}
						tBICPAR->blInPressKeyMode = 0;
						continue;
					}
					else if (strSub == "-L"){
						strSub = strPar1.substr(2);
						timeMs = atof(strSub.c_str());
						tBICPAR->sdtApp->m_Device2.cgAPIECom->SetDTRToLow();
						strPrintData = SYS_MakeTimeNow();
						SYS_Delay_SetTS(&timeS,timeMs * 1000);
						strPrintData += " Set Aux DTR to Low\r\n";
						if (tBICPAR->sdtApp->m_Device2.cgODevList.cgOutput != nullptr){
							tBICPAR->sdtApp->m_Device2.cgODevList.cgOutput->Spin_InUse_set();
							tBICPAR->sdtApp->m_Device2.cgODevList.cgOutput->WriteDividingLine(RICH_CF_clMaroon,COLSTRING::COL_EP_YES);
							tBICPAR->sdtApp->m_Device2.cgODevList.cgOutput->WriteStrN(strPrintData,RICH_CF_clPurple,COLSTRING::COL_EP_YES);
							tBICPAR->sdtApp->m_Device2.cgODevList.cgOutput->WriteDividingLine(RICH_CF_clMaroon,COLSTRING::COL_EP_YES);
							tBICPAR->sdtApp->m_Device2.cgODevList.cgOutput->Spin_InUse_clr();
						}
						
						while ((tBICPAR->blExit == 0) && (SYS_Delay_CheckTS(&timeS) == 0)){
							tBICPAR->blInPressKeyMode = 1;
							if (BI_ReadChar(tBICPAR,0) == 27){
								PrintStrN(tBICPAR,"Pressed ESC, stop execute command.\r\n",RICH_LIN_clRed);
								tBICPAR->blInPressKeyMode = 0;
								return(cgReturnCode);
							}
						}
						tBICPAR->blInPressKeyMode = 0;
						continue;
					}
				}
				break;
			}
		}
	}
	else{
		PrintDoRet(tBICPAR,"Set fail due to no auxiliary COM connected");
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_AUX_RTS::Help(BICPAR *tBICPAR,int32 blDetail)const{
	PrintHelpItem(tBICPAR,cgCommand,"Set auxiliary COM RTS.");
	if (blDetail == 0)
		return(cgReturnCode);
	PrintHelpItem(tBICPAR,"     [-H<t>]","Set to High, t is time, unit is S.");
	PrintHelpItem(tBICPAR,"     [-L<t>]","Set to Low, t is time, unit is S.");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_AUX_RTS::Command(BICPAR *tBICPAR,const std::string &par,std::string *ret)const{
	std::string		strPrintData,strPar1,strPar2,strSub;
	double			timeMs;
	SYS_TIME_S		timeS;
	
	*ret = "";
	if ((tBICPAR->sdtApp->m_Device2.cgDevType == DEVICE::DEVID_APICOM) && (tBICPAR->sdtApp->m_Device2.CheckblConnect() != 0)){
		if (par.length() == 0){
			if (tBICPAR->sdtApp->m_Device2.cgAPIECom->RTS == 0){
				PrintStrN(tBICPAR," RTS is high.\n",RICH_LIN_clDefault);
			}
			else{
				PrintStrN(tBICPAR," RTS is low.\n",RICH_LIN_clDefault);
			}
		}
		else{
			strPar2 = par;
			while(strPar2.length() > 0){
				strPar1 = Str_Trim(Str_ReadSubItem(&strPar2, " "));
				if (strPar1 == "-H"){
					tBICPAR->sdtApp->m_Device2.cgAPIECom->SetRTSToHigh();
					strPrintData = SYS_MakeTimeNow();
					strPrintData += " Set Aux RTS to high\r\n";
					if (tBICPAR->sdtApp->m_Device2.cgODevList.cgOutput != nullptr){
						tBICPAR->sdtApp->m_Device2.cgODevList.cgOutput->Spin_InUse_set();
						tBICPAR->sdtApp->m_Device2.cgODevList.cgOutput->WriteDividingLine(RICH_CF_clMaroon,COLSTRING::COL_EP_YES);
						tBICPAR->sdtApp->m_Device2.cgODevList.cgOutput->WriteStrN(strPrintData,RICH_CF_clPurple,COLSTRING::COL_EP_YES);
						tBICPAR->sdtApp->m_Device2.cgODevList.cgOutput->WriteDividingLine(RICH_CF_clMaroon,COLSTRING::COL_EP_YES);
						tBICPAR->sdtApp->m_Device2.cgODevList.cgOutput->Spin_InUse_clr();
					}
					return(cgReturnCode);
				}
				else if (strPar1 == "-L"){
					tBICPAR->sdtApp->m_Device2.cgAPIECom->SetRTSToLow();
					strPrintData = SYS_MakeTimeNow();
					strPrintData += " Set Aux RTS to Low\r\n";
					if (tBICPAR->sdtApp->m_Device2.cgODevList.cgOutput != nullptr){
						tBICPAR->sdtApp->m_Device2.cgODevList.cgOutput->Spin_InUse_set();
						tBICPAR->sdtApp->m_Device2.cgODevList.cgOutput->WriteDividingLine(RICH_CF_clMaroon,COLSTRING::COL_EP_YES);
						tBICPAR->sdtApp->m_Device2.cgODevList.cgOutput->WriteStrN(strPrintData,RICH_CF_clPurple,COLSTRING::COL_EP_YES);
						tBICPAR->sdtApp->m_Device2.cgODevList.cgOutput->WriteDividingLine(RICH_CF_clMaroon,COLSTRING::COL_EP_YES);
						tBICPAR->sdtApp->m_Device2.cgODevList.cgOutput->Spin_InUse_clr();
					}
					return(cgReturnCode);
				}
				else if(strPar1.length() > 2){
					strSub = strPar1.substr(0,2);
					if (strSub == "-H"){
						strSub = strPar1.substr(2);
						timeMs = atof(strSub.c_str());
						tBICPAR->sdtApp->m_Device2.cgAPIECom->SetRTSToHigh();
						strPrintData = SYS_MakeTimeNow();
						SYS_Delay_SetTS(&timeS,timeMs * 1000);
						strPrintData += " Set Aux RTS to high\r\n";
						if (tBICPAR->sdtApp->m_Device2.cgODevList.cgOutput != nullptr){
							tBICPAR->sdtApp->m_Device2.cgODevList.cgOutput->Spin_InUse_set();
							tBICPAR->sdtApp->m_Device2.cgODevList.cgOutput->WriteDividingLine(RICH_CF_clMaroon,COLSTRING::COL_EP_YES);
							tBICPAR->sdtApp->m_Device2.cgODevList.cgOutput->WriteStrN(strPrintData,RICH_CF_clPurple,COLSTRING::COL_EP_YES);
							tBICPAR->sdtApp->m_Device2.cgODevList.cgOutput->WriteDividingLine(RICH_CF_clMaroon,COLSTRING::COL_EP_YES);
							tBICPAR->sdtApp->m_Device2.cgODevList.cgOutput->Spin_InUse_clr();
						}
						while ((tBICPAR->blExit == 0) && (SYS_Delay_CheckTS(&timeS) == 0)){
							tBICPAR->blInPressKeyMode = 1;
							if (BI_ReadChar(tBICPAR,0) == 27){
								PrintStrN(tBICPAR,"Pressed ESC, stop execute command.\r\n",RICH_LIN_clRed);
								tBICPAR->blInPressKeyMode = 0;
								return(cgReturnCode);
							}
						}
						tBICPAR->blInPressKeyMode = 0;
						continue;
					}
					else if (strSub == "-L"){
						strSub = strPar1.substr(2);
						timeMs = atof(strSub.c_str());
						tBICPAR->sdtApp->m_Device2.cgAPIECom->SetRTSToLow();
						strPrintData = SYS_MakeTimeNow();
						SYS_Delay_SetTS(&timeS,timeMs * 1000);
						strPrintData += " Set Aux RTS to Low\r\n";
						if (tBICPAR->sdtApp->m_Device2.cgODevList.cgOutput != nullptr){
							tBICPAR->sdtApp->m_Device2.cgODevList.cgOutput->Spin_InUse_set();
							tBICPAR->sdtApp->m_Device2.cgODevList.cgOutput->WriteDividingLine(RICH_CF_clMaroon,COLSTRING::COL_EP_YES);
							tBICPAR->sdtApp->m_Device2.cgODevList.cgOutput->WriteStrN(strPrintData,RICH_CF_clPurple,COLSTRING::COL_EP_YES);
							tBICPAR->sdtApp->m_Device2.cgODevList.cgOutput->WriteDividingLine(RICH_CF_clMaroon,COLSTRING::COL_EP_YES);
							tBICPAR->sdtApp->m_Device2.cgODevList.cgOutput->Spin_InUse_clr();
						}
						
						while ((tBICPAR->blExit == 0) && (SYS_Delay_CheckTS(&timeS) == 0)){
							tBICPAR->blInPressKeyMode = 1;
							if (BI_ReadChar(tBICPAR,0) == 27){
								PrintStrN(tBICPAR,"Pressed ESC, stop execute command.\r\n",RICH_LIN_clRed);
								tBICPAR->blInPressKeyMode = 0;
								return(cgReturnCode);
							}
						}
						tBICPAR->blInPressKeyMode = 0;
						continue;
					}
				}
				break;
			}
		}
	}
	else{
		PrintDoRet(tBICPAR,"Set fail due to no auxiliary COM connected");
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_AUX_MS::Help(BICPAR *tBICPAR,int32 blDetail)const{
	PrintHelpItem(tBICPAR,cgCommand,"Auxiliary modem status report");
	
	if (blDetail == 0)
		return(cgReturnCode);
	PrintHelpItem(tBICPAR,"     [on|off]"	,"Enable/disable auxiliary modem status report.");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_AUX_MS::Command(BICPAR *tBICPAR,const std::string &par,std::string *ret)const{
	std::string		strPrintData;
	*ret = "";
	
	if ((tBICPAR->sdtApp->m_Device2.cgDevType == DEVICE::DEVID_APICOM) && (tBICPAR->sdtApp->m_Device2.CheckblConnect() != 0)){
		if (par == "on"){
			B_SetFLAG64(tBICPAR->sdtApp->m_Device2.cgODevList.cgODevFlagU64, ODEV_FLAG_EnMSReport);
			PrintStrN(tBICPAR," enable auxiliary modem status report.\n",RICH_LIN_clDefault);
		}
		else if (par == "off"){
			B_ClrFLAG64(tBICPAR->sdtApp->m_Device2.cgODevList.cgODevFlagU64, ODEV_FLAG_EnMSReport);
			PrintStrN(tBICPAR," disable auxiliary modem status report.\n",RICH_LIN_clDefault);
		}
		else if (par.length() == 0){
			if (tBICPAR->sdtApp->m_Device2.cgAPIECom->DTR == 0){
				strPrintData = " DTR=L,";
			}
			else{
				strPrintData += " DTR=H,";
			}
			if (tBICPAR->sdtApp->m_Device2.cgAPIECom->RTS == 0){
				strPrintData += " RTS=L,";
			}
			else{
				strPrintData += " RTS=H,";
			}
			strPrintData += (" CTS=" + tBICPAR->sdtApp->m_Device2.cgAPIECom->GetCTSStatus() + ",");
			strPrintData += (" DSR=" + tBICPAR->sdtApp->m_Device2.cgAPIECom->GetDSRStatus() + ",");
			strPrintData += (" RING=" + tBICPAR->sdtApp->m_Device2.cgAPIECom->GetRINGStatus() + ",");
			strPrintData += (" DCD=" + tBICPAR->sdtApp->m_Device2.cgAPIECom->GetDCDStatus() + ",");
			PrintStrN(tBICPAR,strPrintData + "\n",RICH_LIN_clDefault);
		}
		else{
			Help(tBICPAR);
		}
	}
	else{
		PrintDoRet(tBICPAR,"Set fail due to no auxiliary COM connected");
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//

