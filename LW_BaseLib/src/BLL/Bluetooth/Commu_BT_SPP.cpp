//
//  Commu_BT_SPP.cpp
//  SDT
//
//  Created by Leif Wen on 12/09/2020.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "stdafx.h"
//------------------------------------------------------------------------------------------//
#include "Commu_BT_SPP.h"
#ifdef Commu_BT_SPP_h
#include "Commu_BT_Swift.h"
//------------------------------------------------------------------------------------------//
//#define LOGPRINT_ENABLE
//#define LOGTHREAD_ENABLE
#include "SYS_Log.h"
//------------------------------------------------------------------------------------------//
enum{
	disconnect = 0,
	discovering = 1,
	discovered = 2,
	connecting = 3,
	connected = 4,
	fail = 5,
};
//------------------------------------------------------------------------------------------//
extern "C" uint32 BT_ReadCallBack(uint32 uid,const unsigned char* buffer,uint32 len){
	//this func was called by in Swift BluetoothRFCOMMChannelDelegate
	return(SBUFPOOL::Put(uid,(char*)buffer, len));
};
//------------------------------------------------------------------------------------------//
CORE_BTSPP::CORE_BTSPP(void) : COMMU_CORE(){
	SetSelfName("CORE_BTSPP");
	SetUpName("");
	
	cgBTrxSBUF = nullptr;
};
//------------------------------------------------------------------------------------------//
CORE_BTSPP::~CORE_BTSPP(void){
	CloseDev();
};
//------------------------------------------------------------------------------------------//
bool32 CORE_BTSPP::OpenDev(const OPEN_PAR& par){
	
	if (cgBTrxSBUF == nullptr)
		cgBTrxSBUF = SBUFPOOL::GetSBUF();
	if (Swift_BT_Connect(par.name.c_str()) != 0){
		if (Swift_BT_OpenRFCOMMChannelSync(par.name.c_str(),par.port,GetUniqueID(cgBTrxSBUF)) != 0)
			return(COMMU_CORE::OpenDev(par));
	}
	return G_FALSE;
}
//------------------------------------------------------------------------------------------//
void CORE_BTSPP::CloseDev(void){

	Swift_BT_CloseRFCOMMChannelSync(cgOpenPar.name.c_str());
	Swift_BT_Disconnect(cgOpenPar.name.c_str());
	COMMU_CORE::CloseDev();
};
//------------------------------------------------------------------------------------------//
bool32 CORE_BTSPP::ReadFromDevice(uint32* retNum,uint8* buffer,uint32 length){
	int64	retCode = 0;
	*retNum = 0;
	
	if (cgBTrxSBUF != nullptr)
		retCode = cgBTrxSBUF->array.Get(buffer, length);
	if (retCode == 0)
		return G_FALSE;
	if (retCode > 0){
		*retNum = (uint32)retCode;
		return G_TRUE;
	}
	return -1;
};
//------------------------------------------------------------------------------------------//
bool32 CORE_BTSPP::SendToDevice(uint32* retNum,const uint8* buffer,uint32 length){
	int64		bytesWr = 0;
	uint32		alreadySend = 0;

	bytesWr = Swift_BT_WriteRFCOMMChannelSync(cgOpenPar.name.c_str(),(char*)buffer,(uint16)length);
	if (bytesWr > 0)
		alreadySend = (uint32)bytesWr;
	*retNum = alreadySend;
	return(alreadySend == length);
};
//------------------------------------------------------------------------------------------//
void CORE_BTSPP::PrintOpenSuccess(const STDSTR& strTitle){
	COMMU_ODEVBUS::PrintMessageDot(unitTeam->odevBus,"Successfully RFCOMM",GetDevName());
};
//------------------------------------------------------------------------------------------//
void CORE_BTSPP::PrintOpenFail(const STDSTR& strTitle){
	COMMU_ODEVBUS::PrintMessageDot(unitTeam->odevBus,"Failed RFCOMM",GetDevName());
};
//------------------------------------------------------------------------------------------//
void CORE_BTSPP::PrintClose(const uint64& rxBytes,const uint64& txBytes,const uint64& dpBytes){
	COMMU_ODEVBUS::PrintConnectionReport(unitTeam->odevBus,rxBytes,txBytes,dpBytes,"RFCOMM",GetDevName());
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
#ifndef CommonDefH_MAC
extern "C" int		Swift_BT_Scan(void){return 0;};
extern "C" int		Swift_BT_StopScan(void){return 0;};
extern "C" int		Swift_BT_ScanState(void){return 0;};
extern "C" bool		Swift_BT_IsUpdate(void){return 0;};
extern "C" int		Swift_BT_Connect(const char*){return 0;};
extern "C" void		Swift_BT_Disconnect(const char*){;};
extern "C" int		Swift_BT_OpenRFCOMMChannelSync(const char*,int8,uint32){return 0;};
extern "C" void		Swift_BT_CloseRFCOMMChannelSync(const char*){;};
extern "C" int		Swift_BT_RFCOMMChannelState(void){return 0;};
extern "C" int		Swift_BT_WriteRFCOMMChannelSync(const char*,const char*,int32){return 0;};
extern "C" char*	Swift_BT_GetDevicesDescription(void){return NULL;};
#endif
//------------------------------------------------------------------------------------------//
#endif /* Commu_BT_SPP_h */
