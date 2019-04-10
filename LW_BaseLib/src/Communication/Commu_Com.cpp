//
//  Commu_Com.cpp
//  SDT
//
//  Created by Leif Wen on 05/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "stdafx.h"
#include "Commu_Com.h"
#ifdef Commu_Com_h
//------------------------------------------------------------------------------------------//
#ifdef CommonDefH_Unix
#include "errno.h"
#include <fcntl.h>
#include <termios.h>
#include <sys/ioctl.h>
#endif
//------------------------------------------------------------------------------------------//
ACOM::ACOM(uint32 size,const ODEV_SYSTEM* logSys) : COMMU_THREAD(size,logSys){
	Init();
	TNFP::SetSelfName("ACOM");
	SetSelfName(selfName);
	SetFatherName("");
};
//------------------------------------------------------------------------------------------//
ACOM::~ACOM(void){
	Close();
	modemStatusThread.RemoveSelf();
};
//------------------------------------------------------------------------------------------//
void ACOM::SetSelfName(const STDSTR& strName){
	COMMU_THREAD::SetSelfName(strName);
	modemStatusThread.SetFatherName(GetFullName(this));
};
//------------------------------------------------------------------------------------------//
void ACOM::SetFatherName(const STDSTR& strName){
	COMMU_THREAD::SetFatherName(strName);
	modemStatusThread.SetFatherName(GetFullName(this));
};
//------------------------------------------------------------------------------------------//
void ACOM::Init(void){
#ifdef CommonDefH_Unix
	osHandle = -1;
#endif
#ifdef CommonDefH_VC
	osHandle = INVALID_HANDLE_VALUE;
#endif
	vPortName = "";
	modemStatus = "";
	ClrSFlag(ACOM_blDSR | ACOM_blCTS | ACOM_blDCD | ACOM_blRING | ACOM_blDTR | ACOM_blRTS | ACOM_blCTSFlow | ACOM_blDSRFlow);
	if (GetLogSystem() != nullptr){
		modemStatusThread.ThreadInit(this, &ACOM::ModemStatusThreadFun,"modemStatus");
		cgThreadList < modemStatusThread;
		modemStatusThread.Enable();
	}
}
//------------------------------------------------------------------------------------------//
#ifdef CommonDefH_VC
//------------------------------------------------------------------------------------------//
bool32 ACOM::OpenDev(const OPEN_PAR& par){
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
	
	SetupComm(osHandle,(DWORD)cgRxSBUF.MaxSize(),((DWORD)cgTxSBUF.MaxSize() < OSTXBUF_MAX_SIZE) ? (DWORD)cgTxSBUF.MaxSize() : OSTXBUF_MAX_SIZE);
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
	ComDCB.StopBits = ONESTOPBIT;
	SetCommState(osHandle,&ComDCB);
	
	return G_TRUE;
}
//------------------------------------------------------------------------------------------//
#endif
#ifdef CommonDefH_Unix
//------------------------------------------------------------------------------------------//
bool32 SetBR(ACOM::HANDLE hand,int32 baudrate,uint32 type){
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
		if (type == OPEN_COM)
#endif
			return G_FALSE;
	}
	return G_TRUE;
}
//------------------------------------------------------------------------------------------//
bool32 SetHardFlow(ACOM::HANDLE hand,bool32 blEnable,uint32 type){
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
}
//------------------------------------------------------------------------------------------//
bool32 SetAttr(ACOM::HANDLE hand,uint32 type){
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
		if (type == OPEN_COM)
#endif
			return G_FALSE;
	}
	return G_TRUE;
}

//------------------------------------------------------------------------------------------//
bool32 OpenCOM(ACOM::HANDLE* osHandle,const OPEN_PAR& par){
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
	
	if ((*osHandle < 0) || (SetAttr(*osHandle,par.type) == 0) || (SetBR(*osHandle,par.port,par.type) == 0))
		return G_FALSE;
	return G_TRUE;
}
//------------------------------------------------------------------------------------------//
bool32 ACOM::OpenDev(const OPEN_PAR& par){
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
	return G_TRUE;
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
	if (GetOpenPar().type == OPEN_COMV)
		PrintMessageDot(COL_NormalMessage,"Virtual COM",
						COL_ImportantMessage,vPortName,
						COL_NormalMessage,"has been created");
#endif
};
//------------------------------------------------------------------------------------------//
void ACOM::DoPrintOnClose(void){
	if (CheckSFlag(CF_blCloseDueToOS)){
		PrintConnectionReport(cgAttrTitle,"COM","connection disconnected");
	}
	else{
		PrintConnectionReport("User closed",cgAttrTitle,"COM");
	}
#ifdef CommonDefH_Unix
	if (GetOpenPar().type == OPEN_COMV){
		unlink(vPortName.c_str());
		PrintMessageDot(COL_NormalMessage,"Virtual COM",
						COL_ImportantMessage,vPortName,
						COL_NormalMessage,"has been deleted");
		vPortName = "";
	}
#endif
}
//------------------------------------------------------------------------------------------//
bool32 ACOM::SetBaudrate(int32 comBaudRate){
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
}
//------------------------------------------------------------------------------------------//
void ACOM::CloseDev(void){
	COMMU_THREAD::CloseDev();
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
}
//------------------------------------------------------------------------------------------//
bool32 ACOM::ReadFromDevice(uint32* retNum,uint8* buffer,uint32 length){
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
}
//------------------------------------------------------------------------------------------//
bool32 ACOM::SendToDevice(uint32* retNum,const uint8* buffer,uint32 length){
#ifdef CommonDefH_Unix
	int64		retCode = 0;
	uint32		alreadySend;
	
	alreadySend = 0;
	while((alreadySend < length) && (IsConnected())){
		while (IsConnected() && ((GetCTSFlowStatus() && (GetCTSStatus() == "H")) || (GetDSRFlowStatus() && (GetDSRStatus() == "H"))))
			SYS_SleepMS(2);

		retCode = write(osHandle, &buffer[alreadySend], length - alreadySend);
		if (retCode < 0)
			break;
		alreadySend += retCode;
	}
	*retNum = alreadySend;
	return(!((alreadySend < length) || retCode));
#endif
#ifdef CommonDefH_VC
	COMSTAT		ComStat;
	DWORD		dwErrorFlags,dwBytesWr,dwBytesL,dwBytesBuf,err;
	DCB			ComDCB;

	ComDCB.DCBlength = sizeof(DCB);
	err = GetCommState(osHandle,&ComDCB);
	if (err > 0){
		while ((err > 0) && IsConnected() && ((ComDCB.fOutxCtsFlow && (GetCTSStatus() == "H")) || (ComDCB.fOutxDsrFlow && (GetDSRStatus() == "H")))){
			SYS_SleepMS(2);
			err = GetCommState(osHandle,&ComDCB);
		};

		memset(&ComStat, 0, sizeof(COMSTAT));
		err = ClearCommError(osHandle,&dwErrorFlags,&ComStat);
		if (err > 0){
			dwBytesBuf = ((DWORD)cgTxSBUF.MaxSize() < OSTXBUF_MAX_SIZE) ? (DWORD)cgTxSBUF.MaxSize() : OSTXBUF_MAX_SIZE - ComStat.cbOutQue;
			dwBytesL = (dwBytesBuf < length) ? dwBytesBuf : length;
			dwBytesWr = 0;
			if (dwBytesL > 0){
				if (WriteFile(osHandle,buffer,dwBytesL,&dwBytesWr,nullptr) > 0)
					*retNum = (uint32)dwBytesWr;
			}
			return (!((*retNum < length) || (dwBytesL == dwBytesBuf)));
		}
	}
	return G_FALSE;
#endif
}
//------------------------------------------------------------------------------------------//
bool32 ACOM::ModemStatusThreadFun(void* commu){
	STDSTR	strMSstatus;
	int32	blflag;
	while(modemStatusThread.IsTerminated() == G_FALSE){
		SYS_SleepMS(50);
		if (IsConnected() == G_FALSE)
			continue;
		
		blflag = 0;
#ifdef ODEV_System_h
		if (GetLogSystem() != nullptr)
			blflag = B_ChkFLAG64(GetLogSystem()->envcfg,ODEV_FLAG_EnMSReport);
#endif
		if (blflag == 0){
			modemStatus = GetModemStatus();
		}
		else{
			strMSstatus  = GetModemStatus();
			
			if (strMSstatus != modemStatus){
				PrintConnectionReport("Modem",GetOpenPar().name,"status has changed:",strMSstatus);
				modemStatus = strMSstatus;
			}
		}
	}
	return G_TRUE;
}
//------------------------------------------------------------------------------------------//
STDSTR ACOM::GetModemStatus(void){
	return("CTS=" + GetCTSStatus() + " ,DSR="+ GetDSRStatus() + " ,RING=" + GetRINGStatus() + " ,DCD=" + GetDCDStatus());
}
//------------------------------------------------------------------------------------------//
STDSTR ACOM::GetFullModemStatus(void){
	return("CTS=" + GetCTSStatus()
		   + " ,DSR="+ GetDSRStatus()
		   + " ,RING=" + GetRINGStatus()
		   + " ,DCD=" + GetDCDStatus()
		   + (GetDTRStatus() == G_FALSE ? ", DTR=L" : ", DTR=H")
		   + (GetRTSStatus() == G_FALSE ? ", RTS=L" : ", RTS=H"));
}
//------------------------------------------------------------------------------------------//
STDSTR ACOM::GetCTSStatus(void){
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
}
//------------------------------------------------------------------------------------------//
STDSTR ACOM::GetDSRStatus(void){
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
}
//------------------------------------------------------------------------------------------//
STDSTR ACOM::GetRINGStatus(void){
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
}
//------------------------------------------------------------------------------------------//
STDSTR ACOM::GetDCDStatus(void){
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
}
//------------------------------------------------------------------------------------------//
bool32 ACOM::GetDTRStatus		(void){return(CheckSFlag(ACOM_blDTR));};
bool32 ACOM::GetRTSStatus		(void){return(CheckSFlag(ACOM_blRTS));};
bool32 ACOM::GetDSRFlowStatus	(void){return(CheckSFlag(ACOM_blDSRFlow));};
bool32 ACOM::GetCTSFlowStatus	(void){return(CheckSFlag(ACOM_blCTSFlow));};
//------------------------------------------------------------------------------------------//
void ACOM::SetDTRToHigh(void) {
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
}
//------------------------------------------------------------------------------------------//
void ACOM::SetDTRToLow(void){
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
}
//------------------------------------------------------------------------------------------//
void ACOM::SetRTSToHigh(void){
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
}
//------------------------------------------------------------------------------------------//
void ACOM::SetRTSToLow(void){
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
}
//------------------------------------------------------------------------------------------//
void ACOM::SetDTR(bool32 blHigh) {
	if (blHigh != G_FALSE){
		SetDTRToHigh();
		PrintMessageDot("Set",cgAttrTitle,"DTR to high");
	}
	else{
		SetDTRToLow();
		PrintMessageDot("Set",cgAttrTitle,"DTR to low");
	}
}
//------------------------------------------------------------------------------------------//
void ACOM::SetRTS(bool32 blHigh) {
	if (blHigh != G_FALSE){
		SetRTSToHigh();
		PrintMessageDot("Set",cgAttrTitle,"RTS to high");
	}
	else{
		SetRTSToLow();
		PrintMessageDot("Set",cgAttrTitle,"RTS to low");
	}
}
//------------------------------------------------------------------------------------------//
void ACOM::SetDSRFlow(bool32 blEnable){
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
}
//------------------------------------------------------------------------------------------//
void ACOM::SetCTSFlow(bool32 blEnable){
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
}
//------------------------------------------------------------------------------------------//
#endif /* Commu_Com_h */
