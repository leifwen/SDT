/*
 * Copyright (c) 2012-2014
 * All rights reserved.
 *
 * File Name: SBIC_Condition.cpp
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2014.08.12
 */
//------------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "SBIC_Condition.h"
#include "Comm_Convert.h"
#include "Device.h"
//------------------------------------------------------------------------------------------//
int32 SBIC_CTS::Help(SBICPAR *tBICPAR,int32 blDetail)const{
	PrintB(tBICPAR,".CMD = CTS==<H/L> -->check CTS status.Used as sub-command in Condition Expression.");
	PrintB(tBICPAR,"  Command = <'CTS==<H/L>>");
	PrintP(tBICPAR,"   eg:");
	PrintP(tBICPAR,"     Condition Expression = 'CTS==H      //if CTS is high, the Condition Expression is true");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 SBIC_CTS::Command(SBICPAR *tBICPAR,const std::string &par,std::string *ret)const{
	std::string		strPar;
	
	*ret = 'F';
	strPar = Str_LowerCase(Str_Trim(par));
	if ((tBICPAR != nullptr) && (tBICPAR->cgDevice != nullptr) && (tBICPAR->cgDevice->cgDevType == DEVICE::DEVID_APICOM)){
		if (tBICPAR->cgDevice->cgAPIECom->GetCTSStatus() == "H"){
			if ((strPar == "h") || (strPar == "1") || (strPar == "high"))
				*ret = 'T';
		}
		else if ((strPar == "l") || (strPar == "0") || (strPar == "low")){
			*ret = 'T';
		}
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 SBIC_DSR::Help(SBICPAR *tBICPAR,int32 blDetail)const{
	PrintB(tBICPAR,".CMD = DSR==<H/L> -->check DSR status.Used as sub-command in Condition Expression.");
	PrintB(tBICPAR,"  Command = <'DSR==<H/L>>");
	PrintP(tBICPAR,"   eg:");
	PrintP(tBICPAR,"     Condition Expression = 'DST==H      //if DSR is high, the Condition Expression is true");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 SBIC_DSR::Command(SBICPAR *tBICPAR,const std::string &par,std::string *ret)const{
	std::string		strPar;
	
	*ret = 'F';
	strPar = Str_LowerCase(Str_Trim(par));
	if ((tBICPAR != nullptr) && (tBICPAR->cgDevice != nullptr) && (tBICPAR->cgDevice->cgDevType == DEVICE::DEVID_APICOM)){
		if (tBICPAR->cgDevice->cgAPIECom->GetDSRStatus() == "H"){
			if ((strPar == "h") || (strPar == "1") || (strPar == "high"))
				*ret = 'T';
		}
		else if ((strPar == "l") || (strPar == "0") || (strPar == "low")){
			*ret = 'T';
		}
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 SBIC_RING::Help(SBICPAR *tBICPAR,int32 blDetail)const{
	PrintB(tBICPAR,".CMD = RING==<H/L> -->check RING status.Used as sub-command in Condition Expression.");
	PrintB(tBICPAR,"  Command = <'RING==<H/L>>");
	PrintP(tBICPAR,"   eg:");
	PrintP(tBICPAR,"     Condition Expression = 'RING==H      //if RING is high, the Condition Expression is true");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 SBIC_RING::Command(SBICPAR *tBICPAR,const std::string &par,std::string *ret)const{
	std::string		strPar;
	
	*ret = 'F';
	strPar = Str_LowerCase(Str_Trim(par));
	if ((tBICPAR != nullptr) && (tBICPAR->cgDevice != nullptr) && (tBICPAR->cgDevice->cgDevType == DEVICE::DEVID_APICOM)){
		if (tBICPAR->cgDevice->cgAPIECom->GetRINGStatus() == "H"){
			if ((strPar == "h") || (strPar == "1") || (strPar == "high"))
				*ret = 'T';
		}
		else if ((strPar == "l") || (strPar == "0") || (strPar == "low")){
			*ret = 'T';
		}
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 SBIC_DCD::Help(SBICPAR *tBICPAR,int32 blDetail)const{
	PrintB(tBICPAR,".CMD = DCD==<H/L> -->check DCD status.Used as sub-command in Condition Expression.");
	PrintB(tBICPAR,"  Command = <'DCD==<H/L>>");
	PrintP(tBICPAR,"   eg:");
	PrintP(tBICPAR,"     Condition Expression = 'DCD==H      //if DCD is high, the Condition Expression is true");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 SBIC_DCD::Command(SBICPAR *tBICPAR,const std::string &par,std::string *ret)const{
	std::string		strPar;
	
	*ret = 'F';
	strPar = Str_LowerCase(Str_Trim(par));
	if ((tBICPAR != nullptr) && (tBICPAR->cgDevice != nullptr) && (tBICPAR->cgDevice->cgDevType == DEVICE::DEVID_APICOM)){
		if (tBICPAR->cgDevice->cgAPIECom->GetDCDStatus() == "H"){
			if ((strPar == "h") || (strPar == "1") || (strPar == "high"))
				*ret = 'T';
		}
		else if ((strPar == "l") || (strPar == "0") || (strPar == "low")){
			*ret = 'T';
		}
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 SBIC_RECEIVE::Help(SBICPAR *tBICPAR,int32 blDetail)const{
	PrintB(tBICPAR,".CMD = receive==<par> -->check recevie buffer status.Used as sub-command in Condition Expression.");
	PrintB(tBICPAR,"  Command = <'receive==<par>>");
	PrintP(tBICPAR,"  Notes:1.support * and ?.");
	PrintP(tBICPAR,"        2.Expression Operators is +.");
	PrintP(tBICPAR,"        3.Can use \"\" set string. Use \\\", \\+ to escape \", +.");
	PrintP(tBICPAR,"        4.Support \\0xhh, \\0Xhh, \\a, \\b, \\f, \\n, \\r, \\t, \\v, \\\\, \\', \\\", \\0, \\/, \\*, \\?.");
	PrintP(tBICPAR,"        5.support sub command :'','hex,'ret,'time,'string.");
	PrintP(tBICPAR,"   eg:");
	PrintP(tBICPAR,"     Condition Expression = 'receive== OK   //if receive buffer contain OK, the Condition Expression is true");
	PrintP(tBICPAR,"     Condition Expression = 'receive== O+K  //if receive buffer contain OK, the Condition Expression is true");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 SBIC_RECEIVE::Command(SBICPAR *tBICPAR,const std::string &par,std::string *ret)const{
	std::string		strSR,strSub;
	std::string::size_type	retD;
	SBIC_Build		*subCommand_Build;
	SBIC_Search		*subCommand_Search;
	
	subCommand_Build = const_cast<SBIC_Build *>(&cgSubC_Build);
	subCommand_Search = const_cast<SBIC_Search *>(&cgSubC_Search);
	
	*ret = 'F';
	if (tBICPAR != nullptr){
		subCommand_Build->Build(tBICPAR,par,&strSub);
		if (subCommand_Search->Search(tBICPAR->cgRecvbuf,strSub,&strSR) != 0){
			*ret = 'T';
		}
		else{
			retD = Str_DCompare(tBICPAR->cgRecvbuf,Str_ASCIIToHEX(strSub,G_ESCAPE_ON),0);
			if (retD != std::string::npos)
				*ret = 'T';
		}
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 SBIC_TIMEOUT::Help(SBICPAR *tBICPAR,int32 blDetail)const{
	PrintB(tBICPAR,".CMD = Timeout -->check timeout status.Used as sub-command in Condition Expression.");
	PrintB(tBICPAR,"  Command = <'Timeout>");
	PrintP(tBICPAR,"   eg:");
	PrintP(tBICPAR,"     Condition Expression = 'Timeout      //if Timeout is true, the Condition Expression is true");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 SBIC_TIMEOUT::Command(SBICPAR *tBICPAR,const std::string &par,std::string *ret)const{
	*ret = 'F';
	if ((tBICPAR != nullptr) && (tBICPAR->cgTimeS != nullptr)){
		if (SYS_Delay_CheckTS(tBICPAR->cgTimeS) != 0)
			*ret = 'T';
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 SBIC_nullptr::Help(SBICPAR *tBICPAR,int32 blDetail)const{
	PrintB(tBICPAR,".CMD = nullptr -->check receive buffer status.Used as sub-command in Condition Expression.");
	PrintB(tBICPAR,"  Command = <'nullptr>");
	PrintP(tBICPAR,"   eg:");
	PrintP(tBICPAR,"     Condition Expression = 'nullptr      //if nothing received, the Condition Expression is true");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 SBIC_nullptr::Command(SBICPAR *tBICPAR,const std::string &par,std::string *ret)const{
	*ret = 'F';
	if ((tBICPAR != nullptr) && (tBICPAR->cgRecvbuf.length() == 0))
		*ret = 'T';
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 SBIC_RX_B::Help(SBICPAR *tBICPAR,int32 blDetail)const{
	PrintB(tBICPAR,".CMD = RX> <num> -->check receive buffer status.Used as sub-command in Condition Expression.");
	PrintB(tBICPAR,"  Command = <'RX> <num>>");
	PrintP(tBICPAR,"   eg:");
	PrintP(tBICPAR,"     Condition Expression = 'RX>10      //if the bytes received is more then 10, the Condition Expression is true");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 SBIC_RX_B::Command(SBICPAR *tBICPAR,const std::string &par,std::string *ret)const{
	*ret = 'F';
	if ((tBICPAR != nullptr) && (tBICPAR->cgRecvbuf.length() / 2 > (std::string::size_type)strtoll(par.c_str(),nullptr,10)))
		*ret = 'T';
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 SBIC_RX_S::Help(SBICPAR *tBICPAR,int32 blDetail)const{
	PrintB(tBICPAR,".CMD = RX< <num> -->check receive buffer status.Used as sub-command in Condition Expression.");
	PrintB(tBICPAR,"  Command = <'RX< <num>>");
	PrintP(tBICPAR,"   eg:");
	PrintP(tBICPAR,"     Condition Expression = 'RX<10      //if the bytes received is less then 10, the Condition Expression is true");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 SBIC_RX_S::Command(SBICPAR *tBICPAR,const std::string &par,std::string *ret)const{
	*ret = 'F';
	if ((tBICPAR != nullptr) && (tBICPAR->cgRecvbuf.length() / 2 < (std::string::size_type)strtoll(par.c_str(),nullptr,10)))
		*ret = 'T';
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 SBIC_RX_E::Help(SBICPAR *tBICPAR,int32 blDetail)const{
	PrintB(tBICPAR,".CMD = RX==<num> -->check receive buffer status.Used as sub-command in Condition Expression.");
	PrintB(tBICPAR,"  Command = <'RX==<num>>");
	PrintP(tBICPAR,"   eg:");
	PrintP(tBICPAR,"     Condition Expression = 'RX==10      //if the bytes received equal to 10, the Condition Expression is true");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 SBIC_RX_E::Command(SBICPAR *tBICPAR,const std::string &par,std::string *ret)const{
	*ret = 'F';
	if ((tBICPAR != nullptr) && (tBICPAR->cgRecvbuf.length() / 2 == (std::string::size_type)strtoll(par.c_str(),nullptr,10)))
		*ret = 'T';
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 SBIC_LCS::Help(SBICPAR *tBICPAR,int32 blDetail)const{
	PrintB(tBICPAR,".CMD = lcs==<T/F> -->check last command execution status.Used as sub-command in Condition Expression.");
	PrintB(tBICPAR,"  Command = <'lcs==<T/F>>");
	PrintP(tBICPAR,"  Notes:1.T is true,F is false.");
	PrintP(tBICPAR,"        2.support command,'Search, 'ChangeCom, 'OpenTCPSocket, 'CloseTCPSocket 'OpenUDPSocket, 'CloseUDPSocket.");
	PrintP(tBICPAR,"   eg:");
	PrintP(tBICPAR,"     Last command is 'ChangeCom and execute successful.");
	PrintP(tBICPAR,"     Condition Expression = 'lcs==T      //if 'ChangeCom execute successful the Condition Expression is true");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 SBIC_LCS::Command(SBICPAR *tBICPAR,const std::string &par,std::string *ret)const{
	std::string		strPar;
	
	*ret = 'F';
	strPar = Str_LowerCase(Str_Trim(par));
	if (tBICPAR != nullptr){
		if (tBICPAR->cgFunRet == "T"){
			if ((strPar == "t") || (strPar == "1") || (strPar == "true"))
				*ret = 'T';
		}
		else if ((strPar == "f") || (strPar == "0") || (strPar == "false")){
			*ret = 'T';
		}
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 SBIC_Conditon::Execute(SBICPAR *tBICPAR,const std::string &fullCMD,std::string *ret)const{
	int32		retCode;
	std::string	retPar;
	
	if (Check(tBICPAR,fullCMD,&retPar) == cgReturnCode){
		retCode = Command(tBICPAR,retPar,ret);
		if (retCode == SBI_RETCODE_NO)
			*ret = 'F';
		return(cgReturnCode);
	}
	return(SBI_RETCODE_NO);
}
//------------------------------------------------------------------------------------------//
int32 SBIC_Conditon::Help(SBICPAR *tBICPAR,int32 blDetail)const{
	const_cast<SBIC_Conditon *>(this)->HelpLC(tBICPAR,blDetail);
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
