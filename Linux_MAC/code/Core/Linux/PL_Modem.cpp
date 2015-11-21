/*
 * Copyright (c) 2012-2014
 * All rights reserved.
 *
 * File Name: PL_Modem.cpp
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2013.6.27
 * @2013.6.26 :	use int32 to replace bool
 use int32 to replace uint32
 * @2013.6.18 :	update from BCB SSPT
 */
//------------------------------------------------------------------------------------------//
#include "PL_Modem.h"
#include "ODEV_Include.h"
#include "Commu_DBuf.h"
#include "Commu_Com.h"
#include "PL_Hotplug.h"
#ifdef CommonDefH_Linux
//------------------------------------------------------------------------------------------//
PL_MODEM::PL_MODEM(ODEV_LIST *tODEV_LIST,uint32 tSize) : APICOM(tODEV_LIST,tSize){
	cgHotplug = nullptr;

	cgVendorM = "";
	cgVendor = "";
	usbport = 0;
	comBR = 115200;
	hotplugThread.ThreadInit(this,&PL_MODEM::hotplugThreadFun);
}
//------------------------------------------------------------------------------------------//
PL_MODEM::~PL_MODEM(void){
	Close(0);
	hotplugThread.ThreadStop();
}
//------------------------------------------------------------------------------------------//
int32 PL_MODEM::OpenPort(const std::string &strVerdorM,int32 comBaudRate){
	
	cgVendorM = strVerdorM;
	comBR = comBaudRate;
	
	UpdateTTYUSBList_M();
	return(Open(cgTTYUSB[usbport],comBR,CSType_None,0));
}
//------------------------------------------------------------------------------------------//
int32 PL_MODEM::ReOpenPort(void){
	if (IsConnected() == 0){
		UpdateTTYUSBList_M();
		return(Open(cgTTYUSB[usbport],comBR,CSType_None,0));
	}
	return 1;
}
//------------------------------------------------------------------------------------------//
int32 PL_MODEM::CheckTTYUSB(const std::string &ttyUSB)const{
	int32 i;
	i = 0;
	do{
		if (cgTTYUSB[i] == ttyUSB)
			return 1;
	}while(++i < 10);
	return 0;
}
//------------------------------------------------------------------------------------------//
int32 PL_MODEM::hotplugThreadFun(void){
	PUB_SBUF	hotplugBuffer;
	std::string	strRecBuf,strSubMsg,sub0,strSubHead,strVendor,printData;
	uint32	    byteNum;
	int32		i;
	hotplugBuffer.Init(1024 * 4);
	SYS_DateTime	dT;
	
	UpdateTTYUSBList_M();
	if (cgHotplug != nullptr){
		cgHotplug->rxFwSBufList->Add(&hotplugBuffer);
		byteNum = 0;
		strRecBuf = "";
		while(hotplugThread.IsTerminated() == 0){
			if (hotplugBuffer.Used() > 0){
				hotplugBuffer.ReadInASCII(&strRecBuf, G_NO_ESCAPE);
				sub0 = '\0';
				while(strRecBuf.length() > 0){
					strSubMsg = Str_ReadSubItem(&strRecBuf,sub0);
					strSubHead = Str_ReadSubItem(&strSubMsg,"@");
					if (/*(IsConnected() == 0) && */(strSubHead == "add")){
						strSubHead = Str_ReadSubItemR(&strSubMsg,"/ttyUSB");
						if (strSubHead.length() > 0){
							GetUSBSerialVendor(strSubHead, &strVendor);
							if (CheckVendor_M(strVendor) != 0){
								dT.Now();
								cgOutput->WriteStrN(strVendor + " " + strSubHead + " is added.\r\n",RICH_CF_clPurple,1);
								//cgOutput->WriteStrN(dT.FormatDateTime("[hh:mm:ss.zzz] : ") + strVendor + " " + strSubHead + " is added.\r\n",RICH_CF_clPurple,1);
								UpdateTTYUSBList_M();
							}
						}
					}
					else if (/*(IsConnected() > 0) && */(strSubHead == "remove")){
						strSubHead = Str_ReadSubItemR(&strSubMsg,"/ttyUSB");
						if (strSubHead.length() > 0){
							if (CheckTTYUSB("/dev/ttyUSB" + strSubHead) == 1){
								Close(0);
								if (cgOutput != nullptr){
									dT.Now();
									cgOutput->WriteStrN(cgVendor + " is removed.\r\nClose usbport " + cgTTYUSB[usbport] + ".\r\n",RICH_CF_clPurple,1);
									//cgOutput->WriteStrN(dT.FormatDateTime("[hh:mm:ss.zzz] : ") + cgVendor + " is removed. Close usbport " + cgTTYUSB[usbport] + ".\r\n",RICH_CF_clPurple,1);
								}
								i = 10;
								while(i -- > 0)
									cgTTYUSB[i] = "";
							}
						}
					}
				}
			}
			if ((IsConnected() == 0) && (ReOpenPort() != 0) && (cgOutput != nullptr)){
				dT.Now();
				cgOutput->WriteStrN("Reopen usbport " + cgTTYUSB[usbport] + ".\r\n",RICH_CF_clPurple,1);
				//cgOutput->WriteStrN(dT.FormatDateTime("[hh:mm:ss.zzz] : ") + "Reopen usbport " + cgTTYUSB[usbport] + ".\r\n",RICH_CF_clPurple,1);
			}
			SYS_SleepMS(10);
		}
	}
	return 1;
}
//------------------------------------------------------------------------------------------//
int32 PL_MODEM::CheckVendor_M(const std::string vendor){
	std::string	strSubVendor,vendorM,strPort;
	
	vendorM = cgVendorM;
	do{
		strPort = Str_ReadSubItem(&vendorM, "/");
		strSubVendor = Str_ReadSubItem(&strPort,",");
		if (strSubVendor == vendor)
			return 1;
	}while(vendorM.length() > 0);
	
	return 0;
}
//------------------------------------------------------------------------------------------//
int32 PL_MODEM::UpdateTTYUSBList_M(void){
	std::string	strSubVendor,vendorM,strPort;
	int32		port;
	
	vendorM = cgVendorM;
	do{
		strPort = Str_ReadSubItem(&vendorM, "/");
		strSubVendor = Str_ReadSubItem(&strPort,",");
		port = atoi(strPort.c_str());
		
		if (UpdateTTYUSBList(strSubVendor) > 0){
			cgVendor = strSubVendor;
			usbport = port;
			return 1;
		}
	}while(vendorM.length() > 0);
	
	return 0;
}
//------------------------------------------------------------------------------------------//
int32 PL_MODEM::UpdateTTYUSBList(const std::string &vendor){
	std::string	strResult,strSubline,strSub,strTTYUSB,strVendor;
	std::string::size_type		pos;
	int32	i;
	
	strResult = "";
	i = 10;
	while(i -- > 0)
		cgTTYUSB[i] = "";
	
	i = 0;
	if (FILES_ReadFile("/proc/tty/driver/usbserial",&strResult) > 0){
		Str_ReadSubItem(&strResult, "\n");
		while(strResult.length() > 0){
			strSubline = Str_ReadSubItem(&strResult, "\n");
			strTTYUSB = Str_ReadSubItem(&strSubline,":");
			
			strVendor = "";
			pos = strSubline.find("vendor");
			if (pos != std::string::npos)
				strVendor = strSubline.substr(pos,24);
			if (strVendor == vendor){
				cgTTYUSB[i] = "/dev/ttyUSB" + strTTYUSB;
				if (++ i > 9)
					return(i);
			}
		}
	}
	return(i);
}
//------------------------------------------------------------------------------------------//
int32 PL_MODEM::GetUSBSerialVendor(const std::string &ttyUSB,std::string *retVendor){
	//return vendor&product ID like "vendor:0403 product:6001"
	
	std::string	strResult,strSubline,strSub,strTTYUSB;
	std::string::size_type		pos;
	
	strResult = "";
	*retVendor = "";
	if (FILES_ReadFile("/proc/tty/driver/usbserial",&strResult) > 0){
		Str_ReadSubItem(&strResult, "\n");
		while(strResult.length() > 0){
			strSubline = Str_ReadSubItem(&strResult, "\n");
			strTTYUSB = Str_ReadSubItem(&strSubline,":");
			if (ttyUSB == strTTYUSB){
				pos = strSubline.find("vendor");
				if (pos != std::string::npos){
					*retVendor = strSubline.substr(pos,24);
					return 1;
				}
			}
		}
	}
	return 0;
}
//------------------------------------------------------------------------------------------//
#endif