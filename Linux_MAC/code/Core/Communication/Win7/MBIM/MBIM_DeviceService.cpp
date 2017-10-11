/*
 * Copyright (c) 2012-2014
 * All rights reserved.
 *
 * File Name: MBIM_DeviceService.cpp
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
#include "MBIM_DeviceService.h"
#include "Comm_Convert.h"
#include "ODEV_Include.h"
#include "SMC.h"
#ifdef CommonDefH_VC
#ifdef MBIM_DeviceServiceH
//------------------------------------------------------------------------------------------//
void MBIM_DevService::Init(void){
	vPortName = "";
	Disable();
	
	osHandle = INVALID_HANDLE_VALUE;
	
	txThread.Enable();
	rxThread.Enable();
	exThread.Enable();
	ex2Thread.Enable();
}
//------------------------------------------------------------------------------------------//
int32 MBIM_DevService::Open(const STDSTR &tCDBufName, int32 tCDBufPar, CSType tCSType, int32 blEnEcho){
	HRESULT	hr;
	SMC_EncryptI(0)

	hr = CoCreateInstance(CLSID_MbnInterfaceManager
							,NULL,CLSCTX_ALL
							,IID_IMbnInterfaceManager
							,(void**)&cgMbnInterfaceMgr);
	if (FAILED(hr))
		return 0;
	hr = CoCreateInstance(CLSID_MbnDeviceServicesManager
							,NULL, CLSCTX_ALL
							,IID_IMbnDeviceServicesManager
							,(void**)&cgMbnDeviceServicesMgr);
	if (FAILED(hr))
		return 0;
	SAFEARRAY	*psa = NULL;

	hr = cgMbnInterfaceMgr->GetInterfaces(&psa);
	if (FAILED(hr))
		return 0;

	LONG lLower, lUpper;
	hr = SafeArrayGetLBound(psa, 1, &lLower);
	if (FAILED(hr))
		return 0;

	SafeArrayGetElement(psa, &lLower, &cgMbnInterface);
	BSTR GuidName;
	cgMbnInterface->get_InterfaceID(&GuidName);

	SysFreeString(GuidName);

	cgMbnDeviceServicesMgr->GetDeviceServicesContext(GuidName, &cgMbnDeviceServicesContext);
	SafeArrayDestroy(psa);
	cgMbnDeviceServicesContext->EnumerateDeviceServices(&psa);

	hr = SafeArrayGetLBound(psa, 1, &lLower);
	if (FAILED(hr))
		return 0;
	hr = SafeArrayGetUBound(psa, 1, &lUpper);
	if (FAILED(hr))
		return 0;

	SafeArrayGetElement(psa, &lLower, &cgMbnDeviceService);
	cgMbnDeviceService->get_DeviceServiceID(&GuidName);


	DisableEcho();
	if (blEnEcho != 0)
		EnableEcho();
	SMC_EncryptS(0)

	Enable();
	ClrblSDC();
	ClrblAClose();
	
	//threadPool.LThreadRun();

	SMC_EncryptE(0)
	SetblConnected();
	return 1;
}
//------------------------------------------------------------------------------------------//
int32 MBIM_DevService::OpenV(const STDSTR &tCDBufName, int32 tCDBufPar, CSType tCSType, int32 blEnEcho){
	if (IsConnected() > 0)
		return 1;
	return 1;
}
//------------------------------------------------------------------------------------------//
int32 MBIM_DevService::CloseV(void){
	return 1;
}

//------------------------------------------------------------------------------------------//
int32 MBIM_DevService::CloseAPICOM(void){
	if (IsConnected() > 0){
		PurgeComm(osHandle,PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
		if (CloseHandle(osHandle) == 0)
			return 0;
	}
	osHandle = INVALID_HANDLE_VALUE;

	return 1;
}
//------------------------------------------------------------------------------------------//
int32 MBIM_DevService::Close(int32 blClr){
	SetblAClose();
	CloseAPICOM();
	ClrblSDC();
	COMMU_DBUF::CloseD(blClr);
	return 1;
}
//------------------------------------------------------------------------------------------//
int32 MBIM_DevService::ReadFromDevice(uint8 *buffer, uint32 length, uint32 *retNum){
	*retNum = 0;

#ifdef CommonDefH_VC
	COMSTAT	ComStat;
	DWORD dwBytesRead,dwErrorFlags;

	ClearCommError(osHandle,&dwErrorFlags,&ComStat);		
	if (ComStat.cbInQue > 0){
		dwBytesRead = ComStat.cbInQue;
		if (length > dwBytesRead)
			length = dwBytesRead;
		if (ReadFile(osHandle,buffer,length,&dwBytesRead,NULL)){
			*retNum = (uint32)dwBytesRead;
			return 1;
		}
	}
	return 0;
#endif
}
//------------------------------------------------------------------------------------------//
int32 MBIM_DevService::SendToDevice(uint8 *buffer, uint32 length, uint32 *retNum){

	return 1;
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 MBIM_DevService::ExThreadFun(void){
	while(exThread.IsTerminated() == 0){
		SYS_SleepMS(50);
		//UpdateMainComModemStatus();
	}
	return 0;
}
//------------------------------------------------------------------------------------------//
#endif
#endif
