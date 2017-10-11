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
#include "Comm_File.h"
#ifdef PL_ModemH
//------------------------------------------------------------------------------------------//
PL_MODEM::PL_MODEM(uint32 tSize,const ODEV_SYSTEM *logSys) : ACOM(tSize,logSys){
	cgHotplug = nullptr;

	cgVendorM = "";
	cgVendor = "";
	usbport = 0;
	comBR = 115200;
	hotplugThread.ThreadInit(this,&PL_MODEM::hotplugThreadFun,"hotplugThread");
}
//------------------------------------------------------------------------------------------//
PL_MODEM::~PL_MODEM(void){
	hotplugThread.RemoveSelf();
	hotplugThread.ThreadStop();
}
//------------------------------------------------------------------------------------------//
int32 PL_MODEM::OpenPort(const STDSTR &strVerdorM,int32 comBaudRate){
	int32 ret;
	ret = 0;
	if (InDoing_try() != 0){
		cgVendorM = strVerdorM;
		comBR = comBaudRate;
		UpdateTTYUSBList_M();
		ret = ACOM::OpenD(cgTTYUSB[usbport],comBR,CSType_COM,0,G_LOCK_OFF);
		InDoing_clr();
	}
	return(ret);
}
//------------------------------------------------------------------------------------------//
int32 PL_MODEM::ReOpenPort(void){
	int32 ret;
	ret = 0;
	if (InDoing_try() != 0){
		ret = 1;
		if (IsConnected() == 0){
			UpdateTTYUSBList_M();
			ret = ACOM::OpenD(cgTTYUSB[usbport],comBR,CSType_COM,0,G_LOCK_OFF);
		}
		InDoing_clr();
	}
	return(ret);
}
//------------------------------------------------------------------------------------------//
void PL_MODEM::CloseDev(void){
	int32 i;
	i = 10;
	while(i -- > 0)
		cgTTYUSB[i] = "";
	ACOM::CloseDev();
}
//------------------------------------------------------------------------------------------//
int32 PL_MODEM::CheckTTYUSB(const STDSTR &ttyUSB)const{
	int32 i;
	i = 0;
	do{
		if (cgTTYUSB[i] == ttyUSB)
			return 1;
	}while(++i < 10);
	return 0;
}
//------------------------------------------------------------------------------------------//
int32 PL_MODEM::hotplugThreadFun(void *p){
	SBUFFER	hotplugBuffer;
	STDSTR	strRecBuf,strSubMsg,sub0,strSubHead,strVendor,printData;
	uint32	byteNum;
	hotplugBuffer.Init(1024 * 4);
	TIME	dT;
	
	UpdateTTYUSBList_M();
	if (cgHotplug != nullptr){
		cgHotplug->RxDataShareTo(&hotplugBuffer);
		byteNum = 0;
		strRecBuf = "";
		while(hotplugThread.IsTerminated() == 0){
			if (hotplugBuffer.Used() > 0){
				hotplugBuffer.Get(&strRecBuf, G_ESCAPE_OFF);
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
								PrintNormalMessage(strVendor,strSubHead,"is added");
								UpdateTTYUSBList_M();
							}
						}
					}
					else if (/*(IsConnected() > 0) && */(strSubHead == "remove")){
						strSubHead = Str_ReadSubItemR(&strSubMsg,"/ttyUSB");
						if (strSubHead.length() > 0){
							if (CheckTTYUSB("/dev/ttyUSB" + strSubHead) == 1){
								PrintNormalMessage(strVendor,"is removed");
								PrintNormalMessage("Close usbport",cgTTYUSB[usbport]);
								SelfClose();
							}
						}
					}
				}
			}
			if ((IsConnected() == 0) && (ReOpenPort() != 0))
				PrintNormalMessage("Reopen usbport",cgTTYUSB[usbport]);
			SYS_SleepMS(10);
		}
	}
	return 1;
}
//------------------------------------------------------------------------------------------//
int32 PL_MODEM::CheckVendor_M(const STDSTR vendor){
	STDSTR	strSubVendor,vendorM,strPort;
	
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
	STDSTR	strSubVendor,vendorM,strPort;
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
int32 PL_MODEM::UpdateTTYUSBList(const STDSTR &vendor){
	STDSTR	strResult,strSubline,strSub,strTTYUSB,strVendor;
	STDSTR::size_type		pos;
	int32	i;
	
	strResult = "";
	i = 10;
	while(i -- > 0)
		cgTTYUSB[i] = "";
	
	i = 0;
	if (CFS_ReadFile(&strResult,"/proc/tty/driver/usbserial") > 0){
		Str_ReadSubItem(&strResult, "\n");
		while(strResult.length() > 0){
			strSubline = Str_ReadSubItem(&strResult, "\n");
			strTTYUSB = Str_ReadSubItem(&strSubline,":");
			
			strVendor = "";
			pos = strSubline.find("vendor");
			if (pos != STDSTR::npos)
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
int32 PL_MODEM::GetUSBSerialVendor(const STDSTR &ttyUSB,STDSTR *retVendor){
	//return vendor&product ID like "vendor:0403 product:6001"
	
	STDSTR	strResult,strSubline,strSub,strTTYUSB;
	STDSTR::size_type		pos;
	
	strResult = "";
	*retVendor = "";
	if (CFS_ReadFile(&strResult,"/proc/tty/driver/usbserial") > 0){
		Str_ReadSubItem(&strResult, "\n");
		while(strResult.length() > 0){
			strSubline = Str_ReadSubItem(&strResult, "\n");
			strTTYUSB = Str_ReadSubItem(&strSubline,":");
			if (ttyUSB == strTTYUSB){
				pos = strSubline.find("vendor");
				if (pos != STDSTR::npos){
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
