/*
 * Copyright (c) 2012-2014
 * All rights reserved.
 *
 * File Name: Commu_Com.cpp
 * File ID	:
 * Remark	:
 * Writer   : Leif Wen
 * Date     : 2012.12.22
 * @2013.6.30 :	change to Liunx style
 				add init method
				use int32 to replace bool
				use int32 to replace uint32
 */
//------------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "Commu_Com.h"
#include "Comm_Convert.h"
#include "ODEV_Include.h"
//------------------------------------------------------------------------------------------//
#ifdef CommonDefH_Unix
#include "errno.h"
#include <fcntl.h>
#include <termios.h>
#include <sys/ioctl.h>
#endif
#ifdef Commu_ComH
//------------------------------------------------------------------------------------------//
void ACOM::Init(void){
#ifdef CommonDefH_Unix
	osHandle = -1;//
#endif
#ifdef CommonDefH_VC
	osHandle = INVALID_HANDLE_VALUE;
#endif
	vPortName = "";
	modemStatus = "";
	ClrSFlag(blDTR | blRTS);
	if (GetLogSystem() != nullptr){
		modemStatusThread.ThreadInit(this, &ACOM::ModemStatusThreadFun,"modemStatus");
		cgThreadList < modemStatusThread;
		modemStatusThread.Enable();
	}
}
//------------------------------------------------------------------------------------------//
#ifdef CommonDefH_VC
//------------------------------------------------------------------------------------------//
int32 ACOM::OpenDev(const STDSTR &tCDBufName,int32 tCDBufPar,CSType tCSType,int32 blEnEcho){
	STDSTR			strComPortNo;
	DCB				ComDCB;
	COMMTIMEOUTS	ComTimeOuts;
	
	strComPortNo = "\\\\.\\" + tCDBufName;
	osHandle = CreateFile(Str_ANSIToUnicode(strComPortNo).c_str(),
						GENERIC_READ | GENERIC_WRITE,
						0,
						nullptr,
						OPEN_EXISTING,
						0,//FILE_FLAG_OVERLAPPED,
						0);
	
	if(osHandle == INVALID_HANDLE_VALUE)
		return 0;
	
	SetupComm(osHandle,(DWORD)cgRxSBUF.BufferMaxSize(),(DWORD)cgTxSBUF.BufferMaxSize());
	PurgeComm(osHandle, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
	
	ComTimeOuts.ReadIntervalTimeout = 1000;
	ComTimeOuts.ReadTotalTimeoutMultiplier = 1;
	ComTimeOuts.ReadTotalTimeoutConstant = 10;
	ComTimeOuts.WriteTotalTimeoutMultiplier = 1;
	ComTimeOuts.WriteTotalTimeoutConstant = 10;
	SetCommTimeouts(osHandle,&ComTimeOuts);
	
	ComDCB.DCBlength = sizeof(DCB);
	GetCommState(osHandle,&ComDCB);
	ComDCB.BaudRate = tCDBufPar;
	ComDCB.fBinary = true;
	ComDCB.fParity = false;
	ComDCB.fOutxCtsFlow = false;
	ComDCB.fOutxDsrFlow = false;
	ComDCB.fDtrControl = DTR_CONTROL_ENABLE;
	ComDCB.fDsrSensitivity = false;
	ComDCB.fTXContinueOnXoff = true;
	ComDCB.fOutX = false;
	ComDCB.fInX = false;
	ComDCB.fErrorChar = false;
	ComDCB.fNull = false;
	ComDCB.fRtsControl = RTS_CONTROL_ENABLE;
	ComDCB.fAbortOnError = true;
	ComDCB.ByteSize = 8;
	ComDCB.Parity = NOPARITY;
	ComDCB.StopBits = ONESTOPBIT;
	SetCommState(osHandle,&ComDCB);
	
	return 1;
}
//------------------------------------------------------------------------------------------//
#endif
#ifdef CommonDefH_Unix
//------------------------------------------------------------------------------------------//
int32 SetBR(ACOM::HANDLE hand,int32 tCDBufPar,CSType tCSType){
	struct	termios 	serCfg;
	
	if (tcgetattr(hand, &serCfg) != 0){
		#ifdef CommonDefH_MAC
		if (tCSType == CSType_COM)
		#endif
			return 0;
	}
	cfmakeraw(&serCfg);
	switch(tCDBufPar){
		case 1200:
			cfsetispeed(&serCfg, B1200);
			cfsetospeed(&serCfg, B1200);
			break;
		case 2400:
			cfsetispeed(&serCfg, B2400);
			cfsetospeed(&serCfg, B2400);
			break;
		case 4800:
			cfsetispeed(&serCfg, B4800);
			cfsetospeed(&serCfg, B4800);
			break;
		case 9600:
			cfsetispeed(&serCfg, B9600);
			cfsetospeed(&serCfg, B9600);
			break;
		case 19200:
			cfsetispeed(&serCfg, B19200);
			cfsetospeed(&serCfg, B19200);
			break;
		case 38400:
			cfsetispeed(&serCfg, B38400);
			cfsetospeed(&serCfg, B38400);
			break;
		case 57600:
			cfsetispeed(&serCfg, B57600);
			cfsetospeed(&serCfg, B57600);
			break;
		case 460800:
		#ifdef CommonDefH_Linux
			cfsetispeed(&serCfg, B460800);
			cfsetospeed(&serCfg, B460800);
		#endif
			break;
		case 115200:
		default:
			cfsetispeed(&serCfg, B115200);
			cfsetospeed(&serCfg, B115200);
	}
	if (tcsetattr(hand, TCSANOW, &serCfg) != 0){
	#ifdef CommonDefH_MAC
		if (tCSType == CSType_COM)
	#endif
			return 0;
	}
	return 1;
}
//------------------------------------------------------------------------------------------//
int32 SetAttr(ACOM::HANDLE hand,CSType tCSType){
	struct	termios 	serCfg;
	
	if (tcgetattr(hand, &serCfg) != 0){
		#ifdef CommonDefH_MAC
		if (tCSType == CSType_COM)
		#endif
		return 0;
	}
	cfmakeraw(&serCfg);
	
	serCfg.c_cflag |= (CLOCAL | CREAD);
	serCfg.c_cflag &= ~CSIZE;
	serCfg.c_cflag |= CS8;
	serCfg.c_cflag &= ~PARENB;
	serCfg.c_iflag &= ~INPCK;        //enable parity checking
	serCfg.c_cflag &= ~CSTOPB;
	
	serCfg.c_cflag |= (CLOCAL | CREAD);
	serCfg.c_iflag &= ~(IXON | IXOFF | IXANY);
	serCfg.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
	
	serCfg.c_iflag &= ~(INLCR | ICRNL | IGNCR);
	serCfg.c_oflag &= ~(ONLCR | OCRNL);
	
	serCfg.c_oflag &= ~OPOST;
	
	serCfg.c_cc[VTIME] = 1;
	serCfg.c_cc[VMIN] = 1;        //Update the Opt and do it now
	
	if(tcsetattr(hand, TCSANOW, &serCfg) != 0){
		#ifdef CommonDefH_MAC
		if (tCSType == CSType_COM)
		#endif
			return 0;
	}
	return 1;
}
//------------------------------------------------------------------------------------------//
int32 OpenCOM(ACOM::HANDLE *osHandle,const STDSTR &tCDBufName,int32 tCDBufPar,CSType tCSType,int32 blEnEcho){
	uint32  par;
	
	if (tCDBufName.length() == 0)
		return 0;
	
	par = O_RDWR | O_NONBLOCK | O_NOCTTY | O_NDELAY;
	
	if (tCSType == CSType_COMV){
#ifdef CommonDefH_MAC
		*osHandle = posix_openpt(par);
#endif
#ifdef CommonDefH_Linux
		*osHandle = open("/dev/ptmx", par);
#endif
	}
	else{
		*osHandle = open(tCDBufName.c_str(), par);
	}
	
	if ((*osHandle < 0) || (SetAttr(*osHandle,tCSType) == 0) || (SetBR(*osHandle,tCDBufPar,tCSType) == 0))
		return 0;
	return 1;
}
//------------------------------------------------------------------------------------------//
int32 ACOM::OpenDev(const STDSTR &tCDBufName,int32 tCDBufPar,CSType tCSType,int32 blEnEcho){
	if (OpenCOM(&osHandle,tCDBufName,tCDBufPar,tCSType,blEnEcho) == 0)
		return 0;
#ifdef CommonDefH_Unix
	if (tCSType == CSType_COMV){
		char	*slavename,*sUid,*gUid;
		
		grantpt(osHandle);
		unlockpt(osHandle);
		slavename = ptsname(osHandle);
		
		if (symlink(slavename,tCDBufName.c_str()) == 0){// try create GetBufName in the linux
			sUid = getenv("SUDO_UID");
			gUid = getenv("SUDO_GID");
			
			if ((sUid != nullptr) && (gUid != nullptr))
				chown(tCDBufName.c_str(), atoi(sUid), atoi(gUid));
			vPortName = tCDBufName;
		}
		else{// if fail , use the name which OS assign
			vPortName = Str_CharToASCIIStr((uint8*)slavename,(uint32)strlen(slavename),G_ESCAPE_OFF);
		}
	}
#endif
	return 1;
}
//------------------------------------------------------------------------------------------//
#endif
//------------------------------------------------------------------------------------------//
void ACOM::DoPrintOnOpenSuccess(void){
	if (GetCNType(this) == CN_S){
		PrintOpenSuccessReport("COM");
	}
	else{
		PrintOpenSuccessReport();
	}
	#ifdef CommonDefH_Unix
	if (GetCSType() == CSType_COMV)
		PrintMessage(Data(COL_NormalMessage,"Virtual COM"),Data(COL_ImportantMessage,&vPortName),Data(COL_NormalMessage,"has been created"));
	#endif
};
//------------------------------------------------------------------------------------------//
void ACOM::DoPrintOnClose(void){
	if (ChkblCloseDueToOS() != 0){
		PrintConnectionReport(cgAttrTitle,"COM","connection disconnected");
	}
	else{
		PrintConnectionReport("User closed",cgAttrTitle,"COM");
	}
#ifdef CommonDefH_Unix
	if (GetCSType() == CSType_COMV){
		unlink(vPortName.c_str());
		PrintMessage(Data(COL_NormalMessage,"Virtual COM"),Data(COL_ImportantMessage,&vPortName),Data(COL_NormalMessage,"has been deleted"));
		vPortName = "";
	}
#endif
}
//------------------------------------------------------------------------------------------//
int32 ACOM::SetBaudrate(int32 tCDBufPar){
#ifdef CommonDefH_VC
	DCB		ComDCB;
	
	if (IsConnected() != 0){
		ComDCB.DCBlength = sizeof(DCB);
		if (GetCommState(osHandle,&ComDCB)){
			ComDCB.BaudRate = tCDBufPar;
			return(SetCommState(osHandle,&ComDCB));
		}
	}
	return 0;
#endif
#ifdef CommonDefH_Unix
	return(SetBR(osHandle,GetBufPar(),GetCSType()));
#endif
}
//------------------------------------------------------------------------------------------//
void ACOM::CloseDev(void){
	COMMU_DBUF::CloseDev();
#ifdef CommonDefH_Unix
	if (IsConnected() != 0)
		close(osHandle);
	osHandle = -1;
#endif
#ifdef CommonDefH_VC
	if (IsConnected() != 0){
		PurgeComm(osHandle,PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
		CloseHandle(osHandle);
	}
	osHandle = INVALID_HANDLE_VALUE;
#endif
	ClrSFlag(blDTR | blRTS);
	modemStatus = "";
}
//------------------------------------------------------------------------------------------//
int32 ACOM::ReadFromDevice(uint32 *retNum,uint8 *buffer,uint32 length){
	*retNum = 0;
#ifdef CommonDefH_Unix
	int64		retCode;
#ifdef CommonDefH_Linux
	retCode = read(osHandle,buffer,length);
	if (retCode == 0)
		return -1;
	if (retCode > 0){
		*retNum = (uint32)retCode;
		return 1;
	}
	return 0;
#endif
#ifdef CommonDefH_MAC
	retCode = read(osHandle,buffer,length);
	if (((retCode == -1) && (errno != EINTR) && (errno != EWOULDBLOCK) && (errno != EAGAIN)))
		return -1;
	if (retCode > 0){
		*retNum = (uint32)retCode;
		return 1;
	}
	return 0;
#endif
#endif
#ifdef CommonDefH_VC
	COMSTAT	ComStat;
	DWORD dwBytesRead,dwErrorFlags;
	
	ClearCommError(osHandle,&dwErrorFlags,&ComStat);
	if (ComStat.cbInQue > 0){
		dwBytesRead = ComStat.cbInQue;
		if (length > dwBytesRead)
			length = dwBytesRead;
		if (ReadFile(osHandle,buffer,length,&dwBytesRead,nullptr)){
			*retNum = (uint32)dwBytesRead;
			return 1;
		}
	}
	return 0;
#endif
}
//------------------------------------------------------------------------------------------//
int32 ACOM::SendToDevice(uint32 *retNum,const uint8 *buffer,uint32 length){
#ifdef CommonDefH_Unix
	int64		retCode;
	uint32		i;
	
	i = 0;
	while((i < length) && (IsConnected() != 0)){
		retCode = write(osHandle, &buffer[i], length - i);
		if (retCode == -1){
			*retNum = i;
			return -1;
		}
		i += retCode;
	}
#endif
#ifdef CommonDefH_VC
	COMSTAT		ComStat;
	DWORD		dwErrorFlags,i,dwBytesWr;
	DCB			ComDCB;
	i = 0;
	ComDCB.DCBlength = sizeof(DCB);
	GetCommState(osHandle,&ComDCB);
	while ((i < length) && (IsConnected() != 0)){
		while ((IsConnected() == 0) && ((ComDCB.fOutxCtsFlow && (GetCTSStatus() == "H")) || (ComDCB.fOutxDsrFlow && (GetDSRStatus() == "H")))){
			SYS_SleepMS(2);
			GetCommState(osHandle,&ComDCB);
		}
		
		PurgeComm(osHandle, PURGE_TXABORT | PURGE_TXCLEAR);
		ClearCommError(osHandle,&dwErrorFlags,&ComStat);
		WriteFile(osHandle,&buffer[i],length - i,&dwBytesWr,nullptr);
		i += dwBytesWr;
	}
#endif
	*retNum = (uint32)i;
	return 1;
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 ACOM::ModemStatusThreadFun(void *p){
	STDSTR	strMSstatus;
	int32	blflag;
	while(modemStatusThread.IsTerminated() == 0){
		SYS_SleepMS(50);
		if (IsConnected() == 0)
			continue;
		
		blflag = 0;
		if (GetLogSystem() != nullptr)
			blflag = B_ChkFLAG64(GetLogSystem()->envcfg,ODEV_FLAG_EnMSReport);
		
		if (blflag == 0){
			modemStatus = GetModemStatus();
		}
		else{
			strMSstatus  = GetModemStatus();
			
			if (strMSstatus != modemStatus){
				PrintConnectionReport("Modem",GetBufName(),"status has changed:",strMSstatus);
				modemStatus = strMSstatus;
			}
		}
	}
	return 0;
}
//------------------------------------------------------------------------------------------//
STDSTR ACOM::GetModemStatus(void){
	return("CTS=" + GetCTSStatus() + " ,DSR="+ GetDSRStatus() + " ,RING=" + GetDCDStatus() + " ,DCD=" + GetRINGStatus());
}
//------------------------------------------------------------------------------------------//
STDSTR ACOM::GetFullModemStatus(void){
	return("CTS=" + GetCTSStatus()
		   + " ,DSR="+ GetDSRStatus()
		   + " ,RING=" + GetDCDStatus()
		   + " ,DCD=" + GetRINGStatus()
		   + (GetDTRStatus() == 0 ? ", DTR=L" : ", DTR=H")
		   + (GetRTSStatus() == 0 ? ", RTS=L" : ", RTS=H"));
}
//------------------------------------------------------------------------------------------//
STDSTR ACOM::GetCTSStatus(void){
	STDSTR	retStr;
	retStr = "";

#ifdef CommonDefH_Unix
	int status;
	if (IsConnected() != 0){
		retStr = 'H';
		if (ioctl(osHandle, TIOCMGET, &status) == 0){
			if (status & TIOCM_CTS)
				retStr = 'L';
		}
	}
#endif
#ifdef CommonDefH_VC
	DWORD	dwStatus;
	
	if (IsConnected() != 0){
		GetCommModemStatus(osHandle,&dwStatus);
		if (dwStatus & MS_CTS_ON){
			retStr = 'L';
		}
		else{
			retStr = 'H';
		}
	}
#endif
	return(retStr);
}
//------------------------------------------------------------------------------------------//
STDSTR ACOM::GetDSRStatus(void){
	STDSTR	retStr;
	retStr = "";

#ifdef CommonDefH_Unix
	int status;
	if (IsConnected() != 0){
		retStr = 'H';
		if (ioctl(osHandle, TIOCMGET, &status) == 0){
			if (status & TIOCM_DSR)
				retStr = 'L';
		}
	}
#endif
#ifdef CommonDefH_VC
	DWORD	dwStatus;
	
	if (IsConnected() != 0){
		GetCommModemStatus(osHandle,&dwStatus);
		if (dwStatus & MS_DSR_ON){
			retStr = 'L';
		}
		else{
			retStr = 'H';
		}
	}
#endif
	return(retStr);
}
//------------------------------------------------------------------------------------------//
STDSTR ACOM::GetRINGStatus(void){
	STDSTR	retStr;
	retStr = "";

#ifdef CommonDefH_Unix
	int status;
	if (IsConnected() != 0){
		retStr = 'H';
		if (ioctl(osHandle, TIOCMGET, &status) == 0){
			if (status & TIOCM_RNG)
				retStr = 'L';
		}
	}
#endif
#ifdef CommonDefH_VC
	DWORD	dwStatus;
	
	if (IsConnected() != 0){
		GetCommModemStatus(osHandle,&dwStatus);
		if (dwStatus & MS_RING_ON){
			retStr = 'L';
		}
		else{
			retStr = 'H';
		}
	}
#endif
	return(retStr);
}
//------------------------------------------------------------------------------------------//
STDSTR ACOM::GetDCDStatus(void){
	STDSTR	retStr;
	retStr = "";

#ifdef CommonDefH_Unix
	int status;
	if (IsConnected() != 0){
		retStr = 'H';
		if (ioctl(osHandle, TIOCMGET, &status) == 0){
			if (status & TIOCM_CAR)
				retStr = 'L';
		}
	}
#endif
#ifdef CommonDefH_VC
	DWORD	dwStatus;
	
	if (IsConnected() != 0){
		GetCommModemStatus(osHandle,&dwStatus);
		if (dwStatus & MS_RLSD_ON){
			retStr = 'L';
		}
		else{
			retStr = 'H';
		}
	}
#endif
	return(retStr);
}
//------------------------------------------------------------------------------------------//
void ACOM::SetDTRToHigh(void) {

#ifdef CommonDefH_Unix
	int status;
	if (IsConnected() != 0){
		ioctl(osHandle, TIOCMGET, &status);
		status &= ~TIOCM_DTR;
		ioctl(osHandle, TIOCMSET, &status);
	}
#endif
#ifdef CommonDefH_VC
	if (IsConnected() != 0)
		EscapeCommFunction(osHandle,CLRDTR);
#endif
	SetSFlag(blDTR);
}
//------------------------------------------------------------------------------------------//
void ACOM::SetDTRToLow(void){

#ifdef CommonDefH_Unix
	int status;
	if (IsConnected() != 0){
		ioctl(osHandle, TIOCMGET, &status);
		status |= TIOCM_DTR;
		ioctl(osHandle, TIOCMSET, &status);
	}
#endif
#ifdef CommonDefH_VC
	if (IsConnected() != 0)
		EscapeCommFunction(osHandle,SETDTR);
#endif
	ClrSFlag(blDTR);
}
//------------------------------------------------------------------------------------------//
void ACOM::SetRTSToHigh(void){

#ifdef CommonDefH_Unix
	int status;
	if (IsConnected() != 0){
		ioctl(osHandle, TIOCMGET, &status);
		status &= ~TIOCM_RTS;
		ioctl(osHandle, TIOCMSET, &status);
	}
#endif
#ifdef CommonDefH_VC
	if (IsConnected() != 0)
		EscapeCommFunction(osHandle,CLRRTS);
#endif
	SetSFlag(blRTS);
}
//------------------------------------------------------------------------------------------//
void ACOM::SetRTSToLow(void){

#ifdef CommonDefH_Unix
	int status;
	if (IsConnected() != 0){
		ioctl(osHandle, TIOCMGET, &status);
		status |= TIOCM_RTS;
		ioctl(osHandle, TIOCMSET, &status);
	}
#endif
#ifdef CommonDefH_VC
	if (IsConnected() != 0)
		EscapeCommFunction(osHandle,SETRTS);
#endif
	ClrSFlag(blRTS);
}
//------------------------------------------------------------------------------------------//
void ACOM::SetDTR(int32 blHigh) {
	if (blHigh != 0){
		SetDTRToHigh();
		PrintNormalMessage("Set",cgAttrTitle,"DTR to high");
	}
	else{
		SetDTRToLow();
		PrintNormalMessage("Set",cgAttrTitle,"DTR to low");
	}
}
//------------------------------------------------------------------------------------------//
void ACOM::SetRTS(int32 blHigh) {
	if (blHigh != 0){
		SetRTSToHigh();
		PrintNormalMessage("Set",cgAttrTitle,"RTS to high");
	}
	else{
		SetRTSToLow();
		PrintNormalMessage("Set",cgAttrTitle,"RTS to low");
	}
}
//------------------------------------------------------------------------------------------//
void ACOM::SetDSRFlow(int32 blEnable){
#ifdef CommonDefH_VC
	DCB	ComDCB;
	
	ComDCB.DCBlength = sizeof(DCB);
	if (GetCommState(osHandle,&ComDCB)){
		ComDCB.fOutxDsrFlow = (blEnable != 0);
		SetCommState(osHandle,&ComDCB);
	}
#endif
}
//------------------------------------------------------------------------------------------//
void ACOM::SetCTSFlow(int32 blEnable){
#ifdef CommonDefH_VC
	DCB	ComDCB;
	ComDCB.DCBlength = sizeof(DCB);
	if (GetCommState(osHandle,&ComDCB)){
		ComDCB.fOutxCtsFlow = (blEnable != 0);
		SetCommState(osHandle,&ComDCB);
	}
#endif
}
//------------------------------------------------------------------------------------------//
#endif
