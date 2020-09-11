//
//  Commu_Com.cpp
//  SDT
//
//  Created by Leif Wen on 05/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "stdafx.h"
//------------------------------------------------------------------------------------------//
#include "Commu_Com.h"
#ifdef Commu_Com_h
//------------------------------------------------------------------------------------------//
#ifdef CommonDefH_Unix
#include "errno.h"
#include <fcntl.h>
#include <termios.h>
#include <sys/ioctl.h>
#endif
//#define LOGPRINT_ENABLE
//#define LOGTHREAD_ENABLE
#include "SYS_Log.h"
//------------------------------------------------------------------------------------------//
CORE_ACOM::CORE_ACOM(void) : COMMU_CORE(){
#ifdef CommonDefH_Unix
	osHandle = -1;
#endif
#ifdef CommonDefH_VC
	osHandle = INVALID_HANDLE_VALUE;
#endif
	vPortName = "";
	modemStatus = "";
	ClrSFlag(ACOM_blDSR | ACOM_blCTS | ACOM_blDCD | ACOM_blRING | ACOM_blDTR | ACOM_blRTS | ACOM_blCTSFlow | ACOM_blDSRFlow);
	
	modemStatusThread.ThreadInit(this, &CORE_ACOM::ModemStatusThreadFun,"ms");
	
	SetSelfName("CORE_ACOM");
	SetUpName("");
};
//------------------------------------------------------------------------------------------//
CORE_ACOM::~CORE_ACOM(void){
	CloseDev();
	modemStatusThread.RemoveSelf();
};
//------------------------------------------------------------------------------------------//
void CORE_ACOM::SetSelfName(const STDSTR& strName){
	COMMU_CORE::SetSelfName(strName);
	modemStatusThread.SetUpName(GetFullName(this));
};
//------------------------------------------------------------------------------------------//
void CORE_ACOM::SetUpName(const STDSTR& strName){
	COMMU_CORE::SetUpName(strName);
	modemStatusThread.SetUpName(GetFullName(this));
};
//------------------------------------------------------------------------------------------//
void CORE_ACOM::Init(const COMMU_TEAM* _team){
	COMMU_CORE::Init(_team);
	unitTeam->commu->ThreadAccept(&modemStatusThread);
};
//------------------------------------------------------------------------------------------//
#ifdef CommonDefH_VC
//------------------------------------------------------------------------------------------//
bool32 CORE_ACOM::OpenDev(const OPEN_PAR& par){
	STDSTR			strComPortNo;
	DCB				ComDCB;
	COMMTIMEOUTS	ComTimeOuts;
	
	strComPortNo = "\\\\.\\" + par.name;
	osHandle = CreateFile(Str_ANSIToUnicode(strComPortNo).c_str(),
						  GENERIC_READ | GENERIC_WRITE,
						  0,
						  nullptr,
						  OPEN_EXISTING,
						  0,//FILE_FLAG_OVERLAPPED,
						  0);
	
	if(osHandle == INVALID_HANDLE_VALUE)
		return G_FALSE;
	
	SetupComm(osHandle, (DWORD)COMMU_BRIDGE::RxMaxSize(unitTeam->bridge), ((DWORD)COMMU_BRIDGE::TxMaxSize(unitTeam->bridge) < OSTXBUF_MAX_SIZE) ? (DWORD)COMMU_BRIDGE::TxMaxSize(unitTeam->bridge) : OSTXBUF_MAX_SIZE);
	PurgeComm(osHandle, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
	
	ComTimeOuts.ReadIntervalTimeout = 1000;
	ComTimeOuts.ReadTotalTimeoutMultiplier = 1;
	ComTimeOuts.ReadTotalTimeoutConstant = 10;
	ComTimeOuts.WriteTotalTimeoutMultiplier = 1;
	ComTimeOuts.WriteTotalTimeoutConstant = 10;
	SetCommTimeouts(osHandle,&ComTimeOuts);
	
	ComDCB.DCBlength = sizeof(DCB);
	GetCommState(osHandle,&ComDCB);
	ComDCB.BaudRate = par.port;
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
	ComDCB.StopBits = TWOSTOPBITS;
	SetCommState(osHandle,&ComDCB);
	
	return(COMMU_CORE::OpenDev(par));
};
//------------------------------------------------------------------------------------------//
#endif
#ifdef CommonDefH_Unix
//------------------------------------------------------------------------------------------//
bool32 SetBR(CORE_ACOM::HANDLE hand,int32 baudrate,uint32 type){
	struct	termios 	serCfg;
	
	if (tcgetattr(hand, &serCfg) != 0){
#ifdef CommonDefH_MAC
		if (type == OPEN_COM)
#endif
			return G_FALSE;
	}
	cfmakeraw(&serCfg);
	switch(baudrate){
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
		case 14400:
			cfsetispeed(&serCfg, B14400);
			cfsetospeed(&serCfg, B14400);
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
		case 115200:
			cfsetispeed(&serCfg, B115200);
			cfsetospeed(&serCfg, B115200);
			break;
		case 230400:
			cfsetispeed(&serCfg, B230400);
			cfsetospeed(&serCfg, B230400);
			break;
#ifdef CommonDefH_Linux
		case 460800:
			cfsetispeed(&serCfg, B460800);
			cfsetospeed(&serCfg, B460800);
			break;
		case 576000:
			cfsetispeed(&serCfg, B576000);
			cfsetospeed(&serCfg, B576000);
			break;
		case 921600:
			cfsetispeed(&serCfg, B921600);
			cfsetospeed(&serCfg, B921600);
			break;
		case 1000000:
			cfsetispeed(&serCfg, B1000000);
			cfsetospeed(&serCfg, B1000000);
			break;
		case 921600:
			cfsetispeed(&serCfg, B921600);
			cfsetospeed(&serCfg, B921600);
			break;
		case 1152000:
			cfsetispeed(&serCfg, B1152000);
			cfsetospeed(&serCfg, B1152000);
			break;
		case 1500000:
			cfsetispeed(&serCfg, B1500000);
			cfsetospeed(&serCfg, B1500000);
			break;
		case 2000000:
			cfsetispeed(&serCfg, B2000000);
			cfsetospeed(&serCfg, B2000000);
			break;
		case 2500000:
			cfsetispeed(&serCfg, B2500000);
			cfsetospeed(&serCfg, B2500000);
			break;
		case 3000000:
			cfsetispeed(&serCfg, B3000000);
			cfsetospeed(&serCfg, B3000000);
			break;
		case 3500000:
			cfsetispeed(&serCfg, B3500000);
			cfsetospeed(&serCfg, B3500000);
			break;
		case 4000000:
			cfsetispeed(&serCfg, B4000000);
			cfsetospeed(&serCfg, B4000000);
			break;
#endif
		default:
			return G_FALSE;
	}
	if (tcsetattr(hand, TCSANOW, &serCfg) != 0){
#ifdef CommonDefH_MAC
		if (type == OPEN_COM)
#endif
			return G_FALSE;
	}
	return G_TRUE;
};
//------------------------------------------------------------------------------------------//
bool32 SetHardFlow(CORE_ACOM::HANDLE hand,bool32 blEnable,uint32 type){
	struct	termios 	serCfg;
	
	if (tcgetattr(hand, &serCfg) != 0){
#ifdef CommonDefH_MAC
		if (type == OPEN_COM)
#endif
			return G_FALSE;
	}
	cfmakeraw(&serCfg);
	if (blEnable == G_FALSE){
		serCfg.c_cflag &= ~CRTSCTS;
	}
	else{
		serCfg.c_cflag |= CRTSCTS;
	}

	if(tcsetattr(hand, TCSANOW, &serCfg) != 0){
#ifdef CommonDefH_MAC
		if (type == OPEN_COM)
#endif
			return G_FALSE;
	}
	return G_TRUE;
};
//------------------------------------------------------------------------------------------//
bool32 SetAttr(CORE_ACOM::HANDLE hand,uint32 type){
	struct	termios 	serCfg;
	
	if (tcgetattr(hand, &serCfg) != 0){
#ifdef CommonDefH_MAC
		if (type == OPEN_COM)
#endif
			return G_FALSE;
	}
	cfmakeraw(&serCfg);
	
	serCfg.c_cflag |= (CLOCAL | CREAD);
	serCfg.c_cflag &= ~CRTSCTS;
	serCfg.c_cflag &= ~CSIZE;
	serCfg.c_cflag |= CS8;
	serCfg.c_cflag &= ~PARENB;
	serCfg.c_iflag &= ~INPCK;        //enable parity checking
	serCfg.c_cflag |= CSTOPB;
	
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
		if (type == OPEN_COM)
#endif
			return G_FALSE;
	}
	return G_TRUE;
}

//------------------------------------------------------------------------------------------//
bool32 OpenCOM(CORE_ACOM::HANDLE* osHandle,const OPEN_PAR& par){
	uint32  bitPar;
	
	if (par.name.length() == 0)
		return G_FALSE;
	
	bitPar = O_RDWR | O_NONBLOCK | O_NOCTTY | O_NDELAY;
	
	if (par.type == OPEN_COMV){
#ifdef CommonDefH_MAC
		*osHandle = posix_openpt(bitPar);
#endif
#ifdef CommonDefH_Linux
		*osHandle = open("/dev/ptmx", bitPar);
#endif
	}
	else{
		*osHandle = open(par.name.c_str(), bitPar);
	}
	
	if ((*osHandle < 0) || (SetAttr(*osHandle,par.type) == G_FALSE) || (SetBR(*osHandle,par.port,par.type) == G_FALSE))
		return G_FALSE;
	return G_TRUE;
};
//------------------------------------------------------------------------------------------//
bool32 CORE_ACOM::OpenDev(const OPEN_PAR& par){
	if (OpenCOM(&osHandle,par) == G_FALSE)
		return G_FALSE;
#ifdef CommonDefH_Unix
	if (par.type == OPEN_COMV){
		char *slavename,*sUid,*gUid;
		
		grantpt(osHandle);
		unlockpt(osHandle);
		slavename = ptsname(osHandle);
		
		if (symlink(slavename,par.name.c_str()) == 0){// try create GetBufName in the linux
			sUid = getenv("SUDO_UID");
			gUid = getenv("SUDO_GID");
			
			if ((sUid != nullptr) && (gUid != nullptr)){
				if (chown(par.name.c_str(), atoi(sUid), atoi(gUid)) == 0){
					vPortName = par.name;
				}
				else{
					vPortName = Str_CharToASCII((uint8*)slavename,(uint32)strlen(slavename),G_ESCAPE_OFF);
				}
			}
		}
		else{// if fail , use the name which OS assign
			vPortName = Str_CharToASCII((uint8*)slavename,(uint32)strlen(slavename),G_ESCAPE_OFF);
		}
	}
#endif
	return(COMMU_CORE::OpenDev(par));
};
//------------------------------------------------------------------------------------------//
#endif
//------------------------------------------------------------------------------------------//
void CORE_ACOM::PrintOpenSuccess(const STDSTR& strTitle){
	COMMU_CORE::PrintOpenSuccess();
#ifdef CommonDefH_Unix
	if (GetOpenPar().type == OPEN_COMV)
		COMMU_LOGSYS::PrintMessageDot(unitTeam->logSys,COL_NormalMessage,"Virtual COM",
									  COL_ImportantMessage,vPortName,
									  COL_NormalMessage,"has been created");
#endif
};
//------------------------------------------------------------------------------------------//
void CORE_ACOM::PrintClose(const uint64& rxBytes,const uint64& txBytes,const uint64& fwBytes){
	if (unitTeam->commu->IsCloseDueToOS()){
		COMMU_LOGSYS::PrintConnectionReport(unitTeam->logSys,rxBytes,txBytes,fwBytes,"COM","connection disconnected");
	}
	else{
		COMMU_LOGSYS::PrintConnectionReport(unitTeam->logSys,rxBytes,txBytes,fwBytes,"User closed","COM");
	}
#ifdef CommonDefH_Unix
	if (GetOpenPar().type == OPEN_COMV){
		unlink(vPortName.c_str());
		COMMU_LOGSYS::PrintMessageDot(unitTeam->logSys,COL_NormalMessage,"Virtual COM",
									  COL_ImportantMessage,vPortName,
									  COL_NormalMessage,"has been deleted");
		vPortName = "";
	}
#endif
};
//------------------------------------------------------------------------------------------//
bool32 CORE_ACOM::SetBaudrate(int32 comBaudRate){
#ifdef CommonDefH_VC
	DCB		ComDCB;
	
	if (IsConnected()){
		ComDCB.DCBlength = sizeof(DCB);
		if (GetCommState(osHandle,&ComDCB)){
			ComDCB.BaudRate = comBaudRate;
			return(SetCommState(osHandle,&ComDCB));
		}
	}
	return G_FALSE;
#endif
#ifdef CommonDefH_Unix
	return(SetBR(osHandle,comBaudRate,GetOpenPar().type));
#endif
};
//------------------------------------------------------------------------------------------//
void CORE_ACOM::CloseDev(void){
#ifdef CommonDefH_Unix
	if (IsConnected())
		close(osHandle);
	osHandle = -1;
#endif
#ifdef CommonDefH_VC
	COMSTAT	ComStat;
	DWORD  dwErrorFlags;
	if (IsConnected()){
		SetCTSFlow(G_FALSE);
		SetDSRFlow(G_FALSE);
		ClearCommError(osHandle, &dwErrorFlags, &ComStat);
		PurgeComm(osHandle,PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
		ClearCommError(osHandle, &dwErrorFlags, &ComStat);
		CloseHandle(osHandle);
	}
	osHandle = INVALID_HANDLE_VALUE;
#endif
	ClrSFlag(ACOM_blDSR | ACOM_blCTS | ACOM_blDCD | ACOM_blRING | ACOM_blDTR | ACOM_blRTS | ACOM_blCTSFlow | ACOM_blDSRFlow);
	modemStatus = "";
	
	COMMU_CORE::CloseDev();
};
//------------------------------------------------------------------------------------------//
bool32 CORE_ACOM::ReadFromDevice(uint32* retNum,uint8* buffer,uint32 length){
	*retNum = 0;
#ifdef CommonDefH_Unix
	int64		retCode;
#ifdef CommonDefH_Linux
	retCode = read(osHandle,buffer,length);
	if ((retCode == 0) && (length > 0))
		return -1;
	if (retCode > 0){
		*retNum = (uint32)retCode;
		return G_TRUE;
	}
	return G_FALSE;
#endif
#ifdef CommonDefH_MAC
	retCode = read(osHandle,buffer,length);
	if (((retCode < 0) && (errno != EINTR) && (errno != EWOULDBLOCK) && (errno != EAGAIN)))
		return -1;
	if (retCode > 0){
		*retNum = (uint32)retCode;
		return G_TRUE;
	}
	return G_FALSE;
#endif
#endif
#ifdef CommonDefH_VC
	COMSTAT	ComStat;
	DWORD dwBytesRead,dwErrorFlags,err;
	
	memset(&ComStat, 0, sizeof(COMSTAT));
	err = ClearCommError(osHandle,&dwErrorFlags,&ComStat);
	if (err > 0){
		dwBytesRead = ComStat.cbInQue;
		if ((dwBytesRead > 0) && (length > 0)){
			if (length > dwBytesRead)
				length = dwBytesRead;
			if (ReadFile(osHandle,buffer,length,&dwBytesRead,nullptr) > 0){
				*retNum = (uint32)dwBytesRead;
				if (dwBytesRead == 0)
					return -1;
				return G_TRUE;
			}
		}
	}
	else{
		dwErrorFlags = GetLastError();
		if ((dwErrorFlags != ERROR_SUCCESS) && (dwErrorFlags != ERROR_IO_PENDING))
			return -1;
	}
	return G_FALSE;
#endif
};
//------------------------------------------------------------------------------------------//
bool32 CORE_ACOM::SendToDevice(uint32* retNum,const uint8* buffer,uint32 length){
#ifdef CommonDefH_Unix
	int64		bytesWr = 0;
	uint32		alreadySend = 0;
	
	do{
		if ((GetCTSFlowStatus() && (GetCTSStatus() == "H")) || (GetDSRFlowStatus() && (GetDSRStatus() == "H")))//flow
			break;
		bytesWr = write(osHandle, buffer, length);
		if (bytesWr > 0)
			alreadySend = (uint32)bytesWr;
	}while(0);
	*retNum = alreadySend;
	return(alreadySend == length);
#endif
#ifdef CommonDefH_VC
	COMSTAT		ComStat;
	DWORD		dwErrorFlags,dwBytesWr,dwBytesL,dwBytesBuf,err;
	uint32		alreadySend = 0;

	memset(&ComStat, 0, sizeof(COMSTAT));
	err = ClearCommError(osHandle,&dwErrorFlags,&ComStat);
	
	do{
		if ((GetCTSFlowStatus() && (GetCTSStatus() == "H")) || (GetDSRFlowStatus() && (GetDSRStatus() == "H")))//flow
			break;
		if (err > 0){
			dwBytesBuf = ((DWORD)COMMU_BRIDGE::TxMaxSize(unitTeam->bridge) < OSTXBUF_MAX_SIZE) ? (DWORD)COMMU_BRIDGE::TxMaxSize(unitTeam->bridge) : OSTXBUF_MAX_SIZE - ComStat.cbOutQue;
			dwBytesL = (dwBytesBuf < length) ? dwBytesBuf : length;
			dwBytesWr = 0;
			if (dwBytesL > 0){
				if (WriteFile(osHandle,buffer,dwBytesL,&dwBytesWr,nullptr) > 0)
					alreadySend = (uint32)dwBytesWr;
			}
		}
	}while (0);
	*retNum = alreadySend;
	return(alreadySend == length);
#endif
};
//------------------------------------------------------------------------------------------//
bool32 CORE_ACOM::ModemStatusThreadFun(void* _team){
	COMMU_TEAM*		team = static_cast<COMMU_TEAM*>(_team);
	COMMU_FRAME*	commu = team->commu;
	COMMU_LOGSYS*	logSys = team->logSys;
	STDSTR			strMSstatus;
	int32			blflag;

	ETLogThreadStart(modemStatusThread);
	while(modemStatusThread.IsTerminated() == G_FALSE){
		SYS_SleepMS(50);
		if (IsConnected() == G_FALSE)
			continue;
		
		blflag = 0;
#ifdef ODEV_System_h
		if (COMMU_LOGSYS::GetLogSystem(logSys) != nullptr)
			blflag = B_ChkFLAG64(COMMU_LOGSYS::GetLogSystem(unitTeam->logSys)->envcfg,ODEV_FLAG_EnMSReport);
#endif
		if (blflag == 0){
			modemStatus = GetModemStatus();
		}
		else{
			strMSstatus  = GetModemStatus();
			
			if (strMSstatus != modemStatus){
				COMMU_LOGSYS::PrintConnectionReport(logSys,commu->RxBytes(),commu->TxBytes(),commu->FwBytes()
													,"Modem",GetOpenPar().name,"status has changed:",strMSstatus);
				modemStatus = strMSstatus;
			}
		}
	}
	ETLogThreadStop(modemStatusThread);
	return G_TRUE;
};
//------------------------------------------------------------------------------------------//
STDSTR CORE_ACOM::GetModemStatus(void){
	return("CTS=" + GetCTSStatus() + " ,DSR="+ GetDSRStatus() + " ,RING=" + GetRINGStatus() + " ,DCD=" + GetDCDStatus());
};
//------------------------------------------------------------------------------------------//
STDSTR CORE_ACOM::GetFullModemStatus(void){
	return("CTS=" + GetCTSStatus()
		   + " ,DSR="+ GetDSRStatus()
		   + " ,RING=" + GetRINGStatus()
		   + " ,DCD=" + GetDCDStatus()
		   + (GetDTRStatus() == G_FALSE ? ", DTR=L" : ", DTR=H")
		   + (GetRTSStatus() == G_FALSE ? ", RTS=L" : ", RTS=H"));
};
//------------------------------------------------------------------------------------------//
STDSTR CORE_ACOM::GetCTSStatus(void){
	STDSTR	retStr = "";
	
#ifdef CommonDefH_Unix
	int status;
#endif
#ifdef CommonDefH_VC
	DWORD	dwStatus;
#endif
	if (IsConnected()){
#ifdef CommonDefH_Unix
		if (ioctl(osHandle, TIOCMGET, &status) == 0){
			if (status & TIOCM_CTS){
#endif
#ifdef CommonDefH_VC
		if (GetCommModemStatus(osHandle,&dwStatus) > 0){
			if (dwStatus & MS_CTS_ON){
#endif
				ClrSFlag(ACOM_blCTS);
			}
			else{
				SetSFlag(ACOM_blCTS);
			}
		}
		retStr = "L";
		if (CheckSFlag(ACOM_blCTS))
			retStr = "H";
	}
	return(retStr);
};
//------------------------------------------------------------------------------------------//
STDSTR CORE_ACOM::GetDSRStatus(void){
	STDSTR	retStr = "";
	
#ifdef CommonDefH_Unix
	int status;
#endif
#ifdef CommonDefH_VC
	DWORD	dwStatus;
#endif
	if (IsConnected()){
#ifdef CommonDefH_Unix
		if (ioctl(osHandle, TIOCMGET, &status) == 0){
			if (status & TIOCM_DSR){
#endif
#ifdef CommonDefH_VC
		if (GetCommModemStatus(osHandle,&dwStatus) > 0){
			if (dwStatus & MS_DSR_ON){
#endif
				ClrSFlag(ACOM_blDSR);
			}
			else{
				SetSFlag(ACOM_blDSR);
			}
		}
		retStr = "L";
		if (CheckSFlag(ACOM_blDSR))
			retStr = "H";
	}
	return(retStr);
};
//------------------------------------------------------------------------------------------//
STDSTR CORE_ACOM::GetRINGStatus(void){
	STDSTR	retStr = "";
	
#ifdef CommonDefH_Unix
	int status;
#endif
#ifdef CommonDefH_VC
	DWORD	dwStatus;
#endif
	if (IsConnected()){
#ifdef CommonDefH_Unix
		if (ioctl(osHandle, TIOCMGET, &status) == 0){
			if (status & TIOCM_RNG){
#endif
#ifdef CommonDefH_VC
		if (GetCommModemStatus(osHandle,&dwStatus) > 0){
			if (dwStatus & MS_RING_ON){
#endif
				ClrSFlag(ACOM_blRING);
			}
			else{
				SetSFlag(ACOM_blRING);
			}
		}
		retStr = "L";
		if (CheckSFlag(ACOM_blRING))
			retStr = "H";
	}
	return(retStr);
};
//------------------------------------------------------------------------------------------//
STDSTR CORE_ACOM::GetDCDStatus(void){
	STDSTR	retStr = "";
	
#ifdef CommonDefH_Unix
	int status;
#endif
#ifdef CommonDefH_VC
	DWORD	dwStatus;
#endif
	if (IsConnected()){
#ifdef CommonDefH_Unix
		if (ioctl(osHandle, TIOCMGET, &status) == 0){
			if (status & TIOCM_CAR){
#endif
#ifdef CommonDefH_VC
		if (GetCommModemStatus(osHandle,&dwStatus) > 0){
			if (dwStatus & MS_RLSD_ON){
#endif
				ClrSFlag(ACOM_blDCD);
			}
			else{
				SetSFlag(ACOM_blDCD);
			}
		}
		retStr = "L";
		if (CheckSFlag(ACOM_blDCD))
			retStr = "H";
	}
	return(retStr);
};
//------------------------------------------------------------------------------------------//
bool32 CORE_ACOM::GetDTRStatus		(void){return(CheckSFlag(ACOM_blDTR));};
bool32 CORE_ACOM::GetRTSStatus		(void){return(CheckSFlag(ACOM_blRTS));};
bool32 CORE_ACOM::GetDSRFlowStatus	(void){return(CheckSFlag(ACOM_blDSRFlow));};
bool32 CORE_ACOM::GetCTSFlowStatus	(void){return(CheckSFlag(ACOM_blCTSFlow));};
//------------------------------------------------------------------------------------------//
void CORE_ACOM::SetDTRToHigh(void) {
#ifdef CommonDefH_Unix
	int status;
	if (IsConnected()){
		ioctl(osHandle, TIOCMGET, &status);
		status &= ~TIOCM_DTR;
		ioctl(osHandle, TIOCMSET, &status);
	}
#endif
#ifdef CommonDefH_VC
	if (IsConnected())
		EscapeCommFunction(osHandle,CLRDTR);
#endif
	SetSFlag(ACOM_blDTR);
};
//------------------------------------------------------------------------------------------//
void CORE_ACOM::SetDTRToLow(void){
#ifdef CommonDefH_Unix
	int status;
	if (IsConnected()){
		ioctl(osHandle, TIOCMGET, &status);
		status |= TIOCM_DTR;
		ioctl(osHandle, TIOCMSET, &status);
	}
#endif
#ifdef CommonDefH_VC
	if (IsConnected())
		EscapeCommFunction(osHandle,SETDTR);
#endif
	ClrSFlag(ACOM_blDTR);
};
//------------------------------------------------------------------------------------------//
void CORE_ACOM::SetRTSToHigh(void){
#ifdef CommonDefH_Unix
	int status;
	if (IsConnected()){
		ioctl(osHandle, TIOCMGET, &status);
		status &= ~TIOCM_RTS;
		ioctl(osHandle, TIOCMSET, &status);
	}
#endif
#ifdef CommonDefH_VC
	if (IsConnected())
		EscapeCommFunction(osHandle,CLRRTS);
#endif
	SetSFlag(ACOM_blRTS);
};
//------------------------------------------------------------------------------------------//
void CORE_ACOM::SetRTSToLow(void){
#ifdef CommonDefH_Unix
	int status;
	if (IsConnected()){
		ioctl(osHandle, TIOCMGET, &status);
		status |= TIOCM_RTS;
		ioctl(osHandle, TIOCMSET, &status);
	}
#endif
#ifdef CommonDefH_VC
	if (IsConnected())
		EscapeCommFunction(osHandle,SETRTS);
#endif
	ClrSFlag(ACOM_blRTS);
};
//------------------------------------------------------------------------------------------//
void CORE_ACOM::SetDTR(bool32 blHigh) {
	if (blHigh != G_FALSE){
		SetDTRToHigh();
		COMMU_LOGSYS::PrintMessageDot(unitTeam->logSys,"Set","DTR to high");
	}
	else{
		SetDTRToLow();
		COMMU_LOGSYS::PrintMessageDot(unitTeam->logSys,"Set","DTR to low");
	}
};
//------------------------------------------------------------------------------------------//
void CORE_ACOM::SetRTS(bool32 blHigh) {
	if (blHigh != G_FALSE){
		SetRTSToHigh();
		COMMU_LOGSYS::PrintMessageDot(unitTeam->logSys,"Set","RTS to high");
	}
	else{
		SetRTSToLow();
		COMMU_LOGSYS::PrintMessageDot(unitTeam->logSys,"Set","RTS to low");
	}
};
//------------------------------------------------------------------------------------------//
void CORE_ACOM::SetDSRFlow(bool32 blEnable){
#ifdef CommonDefH_Unix
	if (SetHardFlow(osHandle, blEnable, GetOpenPar().type)){
		if (blEnable == G_FALSE){
			ClrSFlag(ACOM_blDSRFlow);
		}
		else{
			SetSFlag(ACOM_blDSRFlow);
		}
	}
#endif
#ifdef CommonDefH_VC
	DCB	ComDCB;
	
	ComDCB.DCBlength = sizeof(DCB);
	if (GetCommState(osHandle,&ComDCB)){
		ComDCB.fOutxDsrFlow = (blEnable != G_FALSE);
		SetCommState(osHandle,&ComDCB);
		if (blEnable == G_FALSE){
			ClrSFlag(ACOM_blDSRFlow);
		}
		else{
			SetSFlag(ACOM_blDSRFlow);
		}
	}
#endif
};
//------------------------------------------------------------------------------------------//
void CORE_ACOM::SetCTSFlow(bool32 blEnable){
#ifdef CommonDefH_Unix
	if (SetHardFlow(osHandle, blEnable, GetOpenPar().type)){
		if (blEnable == G_FALSE){
			ClrSFlag(ACOM_blDSRFlow);
		}
		else{
			SetSFlag(ACOM_blDSRFlow);
		}
	}
#endif
#ifdef CommonDefH_VC
	DCB	ComDCB;
	ComDCB.DCBlength = sizeof(DCB);
	if (GetCommState(osHandle,&ComDCB)){
		ComDCB.fOutxCtsFlow = (blEnable != G_FALSE);
		SetCommState(osHandle,&ComDCB);
		if (blEnable == G_FALSE){
			ClrSFlag(ACOM_blCTSFlow);
		}
		else{
			SetSFlag(ACOM_blCTSFlow);
		}
	}
#endif
};
//------------------------------------------------------------------------------------------//							
#endif /* Commu_Com_h */
