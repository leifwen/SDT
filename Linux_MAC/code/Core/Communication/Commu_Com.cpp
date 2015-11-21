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
//------------------------------------------------------------------------------------------//
void APICOM::Init(void){
	vPortName = "";
#ifdef CommonDefH_Unix
	Handle = -1;//
#endif
#ifdef CommonDefH_VC
	Handle = INVALID_HANDLE_VALUE;
#endif
	DTR = 0;
	RTS = 0;
	CTS = "";
	DSR = "";
	RING = "";
	DCD = "";
}
//------------------------------------------------------------------------------------------//
int32 APICOM::OpenDev(const std::string &tCDBufName,int32 tCDBufPar,CSType tCSType){
#ifdef CommonDefH_Unix
	{
		struct	termios 	serCfg;
		
		if (tCDBufName.length() == 0)
			return 0;
#ifdef CommonDefH_MAC
		if (tCDBufName == "/dev/ptmx"){
			Handle = posix_openpt(O_RDWR | O_NONBLOCK | O_NOCTTY | O_NDELAY);
		}
		else{
			Handle = open(tCDBufName.c_str(), O_RDWR | O_NONBLOCK | O_NOCTTY | O_NDELAY);
		}
#endif
#ifdef CommonDefH_Linux
		Handle = open(tCDBufName.c_str(), O_RDWR | O_NONBLOCK | O_NOCTTY | O_NDELAY);
#endif
		if(Handle < 0){
			Close_Do();
			return 0;
		}
		else{
			if (tcgetattr(Handle, &serCfg) != 0){
#ifdef CommonDefH_MAC
				if (tCDBufName != "/dev/ptmx"){
					Close_Do();
					return 0;
				}
#endif
#ifdef CommonDefH_Linux
				Close_Do();
				return 0;
#endif
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
			
			if(tcsetattr(Handle, TCSANOW, &serCfg) != 0){
#ifdef CommonDefH_MAC
				if (tCDBufName != "/dev/ptmx"){
					Close_Do();
					return 0;
				}
#endif
#ifdef CommonDefH_Linux
				Close_Do();
				return 0;
#endif
			}
		}
	}
#endif
#ifdef CommonDefH_VC
	{
		std::string			strComPortNo;
		DCB					ComDCB;
		COMMTIMEOUTS		ComTimeOuts;
		
		if (IsConnected() > 0)
			Close_Do();
		Clean();
		
		strComPortNo = "\\\\.\\" + tCDBufName;
		Handle = CreateFile(Str_ANSIToUnicode(strComPortNo).c_str(),
						 GENERIC_READ | GENERIC_WRITE,
						 0,
						 nullptr,
						 OPEN_EXISTING,
						 0,//FILE_FLAG_OVERLAPPED,
						 0);
		
		if(Handle == INVALID_HANDLE_VALUE){
			Close_Do();
			return 0;
		}
		else{
			SetupComm(Handle,(DWORD)cgRxBuffer.BufferMaxSize(),(DWORD)cgTxBuffer.BufferMaxSize());
			PurgeComm(Handle, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
			
			ComTimeOuts.ReadIntervalTimeout = 1000;
			ComTimeOuts.ReadTotalTimeoutMultiplier = 1;
			ComTimeOuts.ReadTotalTimeoutConstant = 10;
			ComTimeOuts.WriteTotalTimeoutMultiplier = 1;
			ComTimeOuts.WriteTotalTimeoutConstant = 10;
			SetCommTimeouts(Handle,&ComTimeOuts);
			
			ComDCB.DCBlength = sizeof(DCB);
			GetCommState(Handle,&ComDCB);
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
			SetCommState(Handle,&ComDCB);
		}
	}
#endif
	return 1;
}
//------------------------------------------------------------------------------------------//
int32 APICOM::OpenV(const std::string &tCDBufName,int32 tCDBufPar,CSType tCSType,int32 blEnEcho){
	int32 ret;
	ret = 0;
	if (InDoing_try() > 0){
		if (IsConnected() > 0)
			Close_Do();
		Clean();
		ret = OpenVDev(tCDBufName,tCDBufPar,tCSType);
		if (ret > 0)
			Run(tCDBufName,tCDBufPar,tCSType,blEnEcho);
		InDoing_clr();
	}
	return(ret);
}
//------------------------------------------------------------------------------------------//
int32 APICOM::OpenVDev(const std::string &tCDBufName,int32 tCDBufPar,CSType tCSType){
#ifdef CommonDefH_Unix
	char *slavename,*sUid,*gUid;
	int32	ret;
	
	if (APICOM::OpenDev("/dev/ptmx",tCDBufPar,COMMU_DBUF::CSType_None) < 1)
		return 0;
	
	vPortName = tCDBufName;
	grantpt(Handle);
	unlockpt(Handle);
	slavename = ptsname(Handle);
	ret = symlink(slavename,vPortName.c_str());
	if (ret == 0){
		sUid = getenv("SUDO_UID");
		gUid = getenv("SUDO_GID");
		
		if ((sUid != nullptr) && (gUid != nullptr))
			chown(vPortName.c_str(), atoi(sUid), atoi(gUid));
	}
	else{
		vPortName = Str_CharToASCIIStr((uint8*)slavename,(uint32)strlen(slavename),G_ESCAPE_OFF);
	}
	if (GetcgOutput() != nullptr){
		GetcgOutput()->Spin_InUse_set();
		GetcgOutput()->WriteStrN("Virtual COM ",RICH_LIN_clCyan,COLSTRING::COL_EP_YES);
		GetcgOutput()->WriteStr(vPortName,RICH_LIN_clBrown,COLSTRING::COL_EP_YES);
		GetcgOutput()->WriteStr(" has been created.\r\n",RICH_LIN_clCyan,COLSTRING::COL_EP_YES);
		GetcgOutput()->Spin_InUse_clr();
	}
#endif
	return 1;
}
//------------------------------------------------------------------------------------------//
void APICOM::CloseVDev(void){
#ifdef CommonDefH_Unix
	if (vPortName.length() > 0){
		unlink(vPortName.c_str());
		if (GetcgOutput() != nullptr){
			GetcgOutput()->Spin_InUse_set();
			GetcgOutput()->WriteStrN("Virtual COM ",RICH_LIN_clCyan,COLSTRING::COL_EP_YES);
			GetcgOutput()->WriteStr(vPortName,RICH_LIN_clBrown,COLSTRING::COL_EP_YES);
			GetcgOutput()->WriteStr(" has been deleted.\r\n",RICH_LIN_clCyan,COLSTRING::COL_EP_YES);
			GetcgOutput()->Spin_InUse_clr();
		}
		vPortName = "";
	}
#endif
}
//------------------------------------------------------------------------------------------//
int32 APICOM::SetBaudrate(int32 tCDBufPar){
	int32 ret;
	ret = 0;
	if (InDoing_try() > 0){
		Spin_InUse_set();
		ret = SetBaudrate_Do(tCDBufPar);
		Spin_InUse_clr();
		InDoing_clr();
	}
	return(ret);
}
//------------------------------------------------------------------------------------------//
int32 APICOM::SetBaudrate_Do(int32 tCDBufPar){
#ifdef CommonDefH_Unix
	{
		struct	termios 	serCfg;
		
		if (IsConnected() > 0){
			if (tcgetattr(Handle, &serCfg) != 0){
#ifdef CommonDefH_MAC
				if (GetBufName() != "/dev/ptmx"){
					Close_Do();
					return 0;
				}
#endif
#ifdef CommonDefH_Linux
				Close();
				return 0;
#endif
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
			if(tcsetattr(Handle, TCSANOW, &serCfg) != 0){
#ifdef CommonDefH_MAC
				if (GetBufName() != "/dev/ptmx"){
					Close_Do();
					return 0;
				}
#endif
#ifdef CommonDefH_Linux
				Close_Do();
				return 0;
#endif
			}
			return 1;
		}
		return 0;
	}
#endif
#ifdef CommonDefH_VC
	{
		DCB		ComDCB;
		
		if (IsConnected() > 0){
			ComDCB.DCBlength = sizeof(DCB);
			if (GetCommState(Handle,&ComDCB)){
				ComDCB.BaudRate = tCDBufPar;
				return(SetCommState(Handle,&ComDCB));
			}
		}
		return 0;
	}
#endif
}
//------------------------------------------------------------------------------------------//
void APICOM::CloseDev(void){
#ifdef CommonDefH_Unix
	if (IsConnected() > 0)
		close(Handle);
	CloseVDev();
	Handle = -1;
#endif
#ifdef CommonDefH_VC
	if (IsConnected() > 0){
		PurgeComm(Handle,PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
		CloseHandle(Handle);
	}
	Handle = INVALID_HANDLE_VALUE;
#endif
	DTR = 0;
	RTS = 0;
	CTS = "";
	DSR = "";
	RING = "";
	DCD = "";
}
//------------------------------------------------------------------------------------------//
int32 APICOM::ReadFromDevice(uint32 *retNum,uint8 *buffer,uint32 length){
	*retNum = 0;
#ifdef CommonDefH_Unix
	int64		retCode;
#ifdef CommonDefH_Linux
	retCode = read(Handle,buffer,length);
	if (retCode == 0){
		SYS_SleepMS(50);
		ClrblConnected();
		if (CheckblAClose() == 0){
			SetblSDC();
			PrintUserDisconnectReport(" COM " + GetBufName() + ":" + Str_IntToString(GetBufPar()) + " connection disconnected.\r\n");
		}
		CloseDev();
		return -1;
	}
	if (retCode > 0){
		*retNum = (uint32)retCode;
		return 1;
	}
	return 0;
#endif
#ifdef CommonDefH_MAC
	retCode = read(Handle,buffer,length);
	if (((retCode == -1) && (errno != EINTR) && (errno != EWOULDBLOCK) && (errno != EAGAIN))){
		SYS_SleepMS(50);
		ClrblConnected();
		if (CheckblAClose() == 0){
			SetblSDC();
			PrintUserDisconnectReport(" COM " + GetBufName() + ":" + Str_IntToString(GetBufPar()) + " connection disconnected.\r\n");
		}
		CloseDev();
		return -1;
	}
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
	
	ClearCommError(Handle,&dwErrorFlags,&ComStat);
	if (ComStat.cbInQue > 0){
		dwBytesRead = ComStat.cbInQue;
		if (length > dwBytesRead)
			length = dwBytesRead;
		if (ReadFile(Handle,buffer,length,&dwBytesRead,nullptr)){
			*retNum = (uint32)dwBytesRead;
			return 1;
		}
	}
	return 0;
#endif
}
//------------------------------------------------------------------------------------------//
int32 APICOM::SendToDevice(uint32 *retNum,const uint8 *buffer,uint32 length){
#ifdef CommonDefH_Unix
	int64		retCode;
	uint32		i;
	
	i = 0;
	while((i < length) && (CheckblAClose() == 0)){
		retCode = write(Handle, &buffer[i], length - i);
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
	GetCommState(Handle,&ComDCB);
	while((i < length) && (CheckblAClose() == 0)){
		while((CheckblAClose() == 0) && ((ComDCB.fOutxCtsFlow && (GetCTSStatus() == "H")) || (ComDCB.fOutxDsrFlow && (GetDSRStatus() == "H")))){
			SYS_SleepMS(2);
			GetCommState(Handle,&ComDCB);
		}
		
		PurgeComm(Handle, PURGE_TXABORT | PURGE_TXCLEAR);
		ClearCommError(Handle,&dwErrorFlags,&ComStat);
		WriteFile(Handle,&buffer[i],length - i,&dwBytesWr,nullptr);
		i += dwBytesWr;
	}
#endif
	*retNum = (uint32)i;
	return 1;
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 APICOM::ExThreadFun(void){
	while(exThread.IsTerminated() == 0){
		SYS_SleepMS(50);
		UpdateMainComModemStatus();
	}
	return 0;
}
//------------------------------------------------------------------------------------------//
std::string APICOM::GetCTSStatus(void){
	std::string	retStr;
	retStr = "";

	Spin_InUse_set();
#ifdef CommonDefH_Unix
	int status;
	if (IsConnected() > 0){
		ioctl(Handle, TIOCMGET, &status);
		if (status & TIOCM_CTS){
			retStr = 'L';
		}
		else{
			retStr = 'H';
		}
	}
#endif
#ifdef CommonDefH_VC
	DWORD	dwStatus;
	
	if (IsConnected() > 0){
		GetCommModemStatus(Handle,&dwStatus);
		if (dwStatus & MS_CTS_ON){
			retStr = 'L';
		}
		else{
			retStr = 'H';
		}
	}
#endif
	Spin_InUse_clr();
	return(retStr);
}
//------------------------------------------------------------------------------------------//
std::string APICOM::GetDSRStatus(void){
	std::string	retStr;
	retStr = "";

	Spin_InUse_set();
#ifdef CommonDefH_Unix
	int status;
	if (IsConnected() > 0){
		ioctl(Handle, TIOCMGET, &status);
		if (status & TIOCM_DSR){
			retStr = 'L';
		}
		else{
			retStr = 'H';
		}
	}
#endif
#ifdef CommonDefH_VC
	DWORD	dwStatus;
	
	if (IsConnected() > 0){
		GetCommModemStatus(Handle,&dwStatus);
		if (dwStatus & MS_DSR_ON){
			retStr = 'L';
		}
		else{
			retStr = 'H';
		}
	}
#endif
	Spin_InUse_clr();
	return(retStr);
}
//------------------------------------------------------------------------------------------//
std::string APICOM::GetRINGStatus(void){
	std::string	retStr;
	retStr = "";

	Spin_InUse_set();
#ifdef CommonDefH_Unix
	int status;
	if (IsConnected() > 0){
		ioctl(Handle, TIOCMGET, &status);
		if (status & TIOCM_RNG){
			retStr = 'L';
		}
		else{
			retStr = 'H';
		}
	}
#endif
#ifdef CommonDefH_VC
	DWORD	dwStatus;
	
	if (IsConnected() > 0){
		GetCommModemStatus(Handle,&dwStatus);
		if (dwStatus & MS_RING_ON){
			retStr = 'L';
		}
		else{
			retStr = 'H';
		}
	}
#endif
	Spin_InUse_clr();
	return(retStr);
}
//------------------------------------------------------------------------------------------//
std::string APICOM::GetDCDStatus(void){
	std::string	retStr;
	retStr = "";

	Spin_InUse_set();
#ifdef CommonDefH_Unix
	int status;
	if (IsConnected() > 0){
		ioctl(Handle, TIOCMGET, &status);
		if (status & TIOCM_CAR){
			retStr = 'L';
		}
		else{
			retStr = 'H';
		}
	}
#endif
#ifdef CommonDefH_VC
	DWORD	dwStatus;
	
	if (IsConnected() > 0){
		GetCommModemStatus(Handle,&dwStatus);
		if (dwStatus & MS_RLSD_ON){
			retStr = 'L';
		}
		else{
			retStr = 'H';
		}
	}
#endif
	Spin_InUse_clr();
	return(retStr);
}
//------------------------------------------------------------------------------------------//
void APICOM::SetDTRToHigh(void) {

	Spin_InUse_set();
#ifdef CommonDefH_Unix
	int status;
	if (IsConnected() > 0){
		ioctl(Handle, TIOCMGET, &status);
		status &= ~TIOCM_DTR;
		ioctl(Handle, TIOCMSET, &status);
	}
#endif
#ifdef CommonDefH_VC
	if (IsConnected() > 0)
		EscapeCommFunction(Handle,CLRDTR);
#endif
	DTR = 1;
	Spin_InUse_clr();
}
//------------------------------------------------------------------------------------------//
void APICOM::SetDTRToLow(void){

	Spin_InUse_set();
#ifdef CommonDefH_Unix
	int status;
	if (IsConnected() > 0){
		ioctl(Handle, TIOCMGET, &status);
		status |= TIOCM_DTR;
		ioctl(Handle, TIOCMSET, &status);
	}
#endif
#ifdef CommonDefH_VC
	if (IsConnected() > 0)
		EscapeCommFunction(Handle,SETDTR);
#endif
	DTR = 0;
	Spin_InUse_clr();
}
//------------------------------------------------------------------------------------------//
void APICOM::SetRTSToHigh(void){

	Spin_InUse_set();
#ifdef CommonDefH_Unix
	int status;
	if (IsConnected() > 0){
		ioctl(Handle, TIOCMGET, &status);
		status &= ~TIOCM_RTS;
		ioctl(Handle, TIOCMSET, &status);
	}
#endif
#ifdef CommonDefH_VC
	if (IsConnected() > 0)
		EscapeCommFunction(Handle,CLRRTS);
#endif
	RTS = 1;
	Spin_InUse_clr();
}
//------------------------------------------------------------------------------------------//
void APICOM::SetRTSToLow(void){

	Spin_InUse_set();
#ifdef CommonDefH_Unix
	int status;
	if (IsConnected() > 0){
		ioctl(Handle, TIOCMGET, &status);
		status |= TIOCM_RTS;
		ioctl(Handle, TIOCMSET, &status);
	}
#endif
#ifdef CommonDefH_VC
	if (IsConnected() > 0)
		EscapeCommFunction(Handle,SETRTS);
#endif
	RTS = 0;
	Spin_InUse_clr();
}
//------------------------------------------------------------------------------------------//
void APICOM::SetDSRFlow(int32 blEnable){
#ifdef CommonDefH_VC
	DCB	ComDCB;
	Spin_InUse_set();
	ComDCB.DCBlength = sizeof(DCB);
	if (GetCommState(Handle,&ComDCB)){
		ComDCB.fOutxDsrFlow = (blEnable != 0);
		SetCommState(Handle,&ComDCB);
	}
	Spin_InUse_clr();
#endif
}
//------------------------------------------------------------------------------------------//
void APICOM::SetCTSFlow(int32 blEnable){
#ifdef CommonDefH_VC
	DCB	ComDCB;
	Spin_InUse_set();
	ComDCB.DCBlength = sizeof(DCB);
	if (GetCommState(Handle,&ComDCB)){
		ComDCB.fOutxCtsFlow = (blEnable != 0);
		SetCommState(Handle,&ComDCB);
	}
	Spin_InUse_clr();
#endif
}
//------------------------------------------------------------------------------------------//
void APICOM::UpdateMainComModemStatus(void){
	std::string	strPrintdata,strMSstatus1,strMSstatus2;
	int32		blflag;
	
	blflag = 0;
	Spin_InUse_set();
	if (GetcgODevList() != nullptr)
		blflag = B_ChkFLAG64(GetcgODevList()->cgODevFlagU64,ODEV_FLAG_EnMSReport);
	Spin_InUse_clr();
	
	if (blflag == 0){
		CTS = GetCTSStatus();
		DSR = GetDSRStatus();
		DCD = GetDCDStatus();
		RING = GetRINGStatus();
	}
	else{
		strMSstatus1  = CTS + DSR + DCD + RING;
		CTS = GetCTSStatus();
		DSR = GetDSRStatus();
		DCD = GetDCDStatus();
		RING = GetRINGStatus();
		strMSstatus2  = CTS + DSR + DCD + RING;
		
		if (strMSstatus1 != strMSstatus2){
			strPrintdata = SYS_MakeTimeNow();
			strPrintdata += " Modem ";
			strPrintdata += GetBufName();
			strPrintdata += " status has changed: ";
			strPrintdata += ("CTS=" + CTS);
			strPrintdata += (" ,DSR=" + DSR);
			strPrintdata += (" ,RING=" + RING);
			strPrintdata += (" ,DCD=" + DCD);
			strPrintdata += "\r\nCurrently,total already sent ";
			strPrintdata += Str_UInt64ToString(TxBytes());
			strPrintdata += " bytes ,received ";
			strPrintdata += Str_UInt64ToString(RxBytes());
			strPrintdata += " bytes.\r\n";
			Spin_InUse_set();
			if (GetcgOutput() != nullptr){
				GetcgOutput()->Spin_InUse_set();
				GetcgOutput()->WriteDividingLine(RICH_CF_clMaroon,COLSTRING::COL_EP_YES);
				GetcgOutput()->WriteStrN(strPrintdata,RICH_CF_clPurple,COLSTRING::COL_EP_YES);
				GetcgOutput()->WriteDividingLine(RICH_CF_clMaroon,COLSTRING::COL_EP_YES);
				GetcgOutput()->Spin_InUse_clr();
			}
			Spin_InUse_clr();
		}
	}
}
//------------------------------------------------------------------------------------------//
