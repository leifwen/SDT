//
//  Commu_BLE.cpp
//  SDT
//
//  Created by Leif Wen on 12/09/2020.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "stdafx.h"
//------------------------------------------------------------------------------------------//
#include "Commu_BLE.h"
#ifdef Commu_BLE_h
#include "Commu_BLE_Swift.h"
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
static bool32 Resolve(BINDPAR* bindpar,const STDSTR& _ids){
	bindpar->characteristic = _ids;
	bindpar->identifier = Str_ReadSubItem(&bindpar->characteristic, "|");
	bindpar->service = Str_ReadSubItem(&bindpar->characteristic, "|");
	if ((bindpar->characteristic.length() > 0) && (bindpar->identifier.length() > 0) && (bindpar->service.length() > 0))
		return G_TRUE;
	
	bindpar->characteristic = "";
	bindpar->identifier = "";
	bindpar->service = "";
	return G_FALSE;
};
//------------------------------------------------------------------------------------------//
extern "C" uint32 BLE_ReadCallBack(uint32 uid,const void* buffer,uint32 len){
	//this func was called by in Swift BLECoreDelegate.peripheral
	return(SBUFPOOL::Put(uid,(char*)buffer, len));
};
//------------------------------------------------------------------------------------------//
CORE_BLEBIND::CORE_BLEBIND(void) : COMMU_CORE(){
	SetSelfName("CORE_BLEBIND");
	SetUpName("");
	
	cgBTrxSBUF = nullptr;
	
	cgMTU = 0;
	
	cgBindParWrite.identifier = "";
	cgBindParWrite.service = "";
	cgBindParWrite.characteristic = "";
	
	cgBindParNotify.identifier = "";
	cgBindParNotify.service = "";
	cgBindParNotify.characteristic = "";
};
//------------------------------------------------------------------------------------------//
CORE_BLEBIND::~CORE_BLEBIND(void){
	CloseDev();
};
//------------------------------------------------------------------------------------------//
bool32 CORE_BLEBIND::OpenDev(const OPEN_PAR& par){
	bool32	ret = G_FALSE,blNotify = G_FALSE;
	STDSTR	aliasName,_write,_notify;
	
	_notify = par.name;
	aliasName = Str_ReadSubItem(&_notify, ",");
	_write = Str_ReadSubItem(&_notify, ",");
	
	if (Resolve(&cgBindParWrite,_write) != G_FALSE){
		if (Swift_BLE_CheckID(cgBindParWrite.identifier.c_str(),cgBindParWrite.service.c_str(),cgBindParWrite.characteristic.c_str()) != false)
			ret = G_TRUE;
	}
	if (Resolve(&cgBindParNotify,_notify) != G_FALSE){
		if (Swift_BLE_CheckID(cgBindParNotify.identifier.c_str(),cgBindParNotify.service.c_str(),cgBindParNotify.characteristic.c_str()) != false){
			blNotify = G_TRUE;
			ret = G_TRUE;
		}
	}

	if (ret != G_FALSE)
		ret = COMMU_CORE::OpenDev(SetOpenPar(OPEN_None,aliasName,0,par.cfg));
	if ((ret != G_FALSE) && (blNotify != G_FALSE))
		Swift_BLE_SetNotify( cgBindParNotify.identifier.c_str()
							,cgBindParNotify.service.c_str()
							,cgBindParNotify.characteristic.c_str()
							,GetUniqueID(cgBTrxSBUF));
	if (ret != G_FALSE)
		cgMTU = Swift_BLE_GetMTU(cgBindParWrite.identifier.c_str(),cgBindParWrite.service.c_str(),cgBindParWrite.characteristic.c_str());
	cgDevName = cgOpenPar.name;
	return ret;
}
//------------------------------------------------------------------------------------------//
bool32 CORE_BLEBIND::Open(const STDSTR& aliasName,const STDSTR& _write,const STDSTR& _notify,uint32 cfg){
	STDSTR name;
	
	name = aliasName + ",";
	name += _write;
	name += ",";
	name += _notify;

	if (cgBTrxSBUF == nullptr)
		cgBTrxSBUF = SBUFPOOL::GetSBUF();
	
	return(unitTeam->cdrv->Open(SetOpenPar(OPEN_None,name,0,cfg)));
};
//------------------------------------------------------------------------------------------//
void CORE_BLEBIND::CloseDev(void){
	cgBindParWrite.identifier = "";
	cgBindParWrite.service = "";
	cgBindParWrite.characteristic = "";
	
	cgBindParNotify.identifier = "";
	cgBindParNotify.service = "";
	cgBindParNotify.characteristic = "";
	
	COMMU_CORE::CloseDev();
};
//------------------------------------------------------------------------------------------//
bool32 CORE_BLEBIND::ReadFromDevice(uint32* retNum,uint8* buffer,uint32 length){
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
bool32 CORE_BLEBIND::SendToDevice(uint32* retNum,const uint8* buffer,uint32 length){
	uint32		sendNum = 0;
	
	sendNum = length < cgMTU ? length : cgMTU;
	Swift_BLE_Write(cgBindParWrite.identifier.c_str(),cgBindParWrite.service.c_str(),cgBindParWrite.characteristic.c_str(),(const char *)buffer,sendNum);

	*retNum = sendNum;
	return(sendNum == length);
};
//------------------------------------------------------------------------------------------//
void CORE_BLEBIND::PrintOpenSuccess(const STDSTR& strTitle){
	COMMU_ODEVBUS::PrintMessageDot(unitTeam->odevBus,"Successfully bind",GetDevName());
};
//------------------------------------------------------------------------------------------//
void CORE_BLEBIND::PrintOpenFail(const STDSTR& strTitle){
	COMMU_ODEVBUS::PrintMessageDot(unitTeam->odevBus,"Failed bind",GetDevName());
};
//------------------------------------------------------------------------------------------//
void CORE_BLEBIND::PrintClose(const uint64& rxBytes,const uint64& txBytes,const uint64& dpBytes){
	COMMU_ODEVBUS::PrintConnectionReport(unitTeam->odevBus,rxBytes,txBytes,dpBytes,"Unbind",GetDevName());
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
CORE_BLEPeripheral::CORE_BLEPeripheral(void) : COMMU_CORE(){
	SetSelfName("CORE_BLEPeripheral");
	SetUpName("");
	
	cgIdentifier = "";
	cgMTU = PACKAGE_MAX_SIZE;
};
//------------------------------------------------------------------------------------------//
CORE_BLEPeripheral::~CORE_BLEPeripheral(void){
	CloseDev();
};
//------------------------------------------------------------------------------------------//
bool32 CORE_BLEPeripheral::OpenDev(const OPEN_PAR& par){
	int32	state = 0;
	int		index = 0;

	do{
		switch (par.type) {
			case BLE_OPENTYPE_NAME:
				state = Swift_BLE_ConnectByName(par.name.c_str());
				break;
			case BLE_OPENTYPE_ID:
				state = Swift_BLE_ConnectByIdentifier(par.name.c_str());
				break;
			case BLE_OPENTYPE_INDEX:
				index = atoi(par.name.c_str());
				state = Swift_BLE_ConnectByIndex(index);
				break;
			default:
				state = Swift_BLE_ConnectByAny(par.name.c_str());
				break;
		}
		if ((state == connected) || (state == fail))
			break;
		SYS_SleepMS(10);
	}while (1);
	cgIdentifier = "";
	if (state == connected){// need update Peripheral Identifier
		switch (par.type) {
			case BLE_OPENTYPE_NAME:
				cgIdentifier.append(Swift_BLE_GetPeripheralByName(par.name.c_str()));
				break;
			case BLE_OPENTYPE_ID:
				cgIdentifier = par.name;
				break;
			case BLE_OPENTYPE_INDEX:
				cgIdentifier.append(Swift_BLE_GetPeripheralByIndex(index));
				break;
			default:
				cgIdentifier.append(Swift_BLE_GetPeripheralByAny(par.name.c_str()));
				break;
		}
		return(COMMU_CORE::OpenDev(par));
	}
	return G_FALSE;
};
//------------------------------------------------------------------------------------------//
void CORE_BLEPeripheral::CloseDev(void){
	Swift_BLE_DisconnectByIdentifier(cgIdentifier.c_str());
	cgIdentifier = "";
	COMMU_CORE::CloseDev();
};
//------------------------------------------------------------------------------------------//
void CORE_BLEPeripheral::PrintOpenSuccess(const STDSTR& strTitle){
	STDSTR strPar = "";
	strPar.append(Swift_BLE_GetDeviceDetailDescriptionByIdentifier(cgIdentifier.c_str()));
	
	COMMU_ODEVBUS::PrintWithDividingLine(unitTeam->odevBus);
	COMMU_ODEVBUS::PrintStrNL(unitTeam->odevBus,strPar);
};
//------------------------------------------------------------------------------------------//
void CORE_BLEPeripheral::PrintOpenFail(const STDSTR& strTitle){
	COMMU_ODEVBUS::PrintMessageDot(unitTeam->odevBus,"Failed connect to peripheral");
};
//------------------------------------------------------------------------------------------//











//------------------------------------------------------------------------------------------//
STDSTR BLEPeripheral::ListDescription(const OPEN_PAR& par){
	STDSTR	result = "";
	int		index = 0;
	
	if (par.name.length() == 0){
		result.append(Swift_BLE_GetDevicesDescription());
	}
	else{
		switch (par.type) {
			case BLE_OPENTYPE_NAME:
				result.append(Swift_BLE_GetDeviceDetailDescriptionByName(par.name.c_str()));
				break;
			case BLE_OPENTYPE_ID:
				result.append(Swift_BLE_GetDeviceDetailDescriptionByIdentifier(par.name.c_str()));
				break;
			case BLE_OPENTYPE_INDEX:
				index = atoi(par.name.c_str());
				result.append(Swift_BLE_GetDeviceDetailDescriptionByIndex(index));
				break;
			default:
				result.append(Swift_BLE_GetDeviceDetailDescriptionByAny(par.name.c_str()));
				break;
		}
	}
	return(result);
};
//------------------------------------------------------------------------------------------//
bool32 BLEPeripheral::Bind(const STDSTR& aliasName,const STDSTR& _write,const STDSTR& _notify){
	bool32	ret = G_FALSE;
	STDSTR	w_s,w_c,n_s,n_c;
	STDSTR	w_ids = "";
	STDSTR	n_ids = "";
	STDSTR	identifier;
	STDSTR	name;
	int	wsid = -1,wcid = -1,nsid = -1,ncid = -1;
	BLEBIND				*bind;
	CORE_BLEBIND 		*bindCore;
	CORE_BLEPeripheral	*peripheralCore;
	
	if (IsOpened()){
		peripheralCore = static_cast<CORE_BLEPeripheral*>(Core());
		identifier = peripheralCore->cgIdentifier;
		bind = static_cast<BLEBIND*>(GetNewNode());
		if (bind == nullptr){
			COMMU_ODEVBUS:: PrintWarningMessageDot(unitTeam.odevBus,"No enough resource to bind");
		}
		else{
			bindCore = static_cast<CORE_BLEBIND*>(bind->Core());
			AppendChild(bind);
			
			name = aliasName;
			if (name.length() == 0){
				if (_notify.length() > 0)
					name = "n:" + _notify;
				name += "w:";
				name += _write;
			}
			
			w_c = _write;
			w_s = Str_ReadSubItem(&w_c, "-");

			n_c = _notify;
			n_s = Str_ReadSubItem(&n_c, "-");

			if (w_s.length() > 0)
				wsid = atoi(w_s.c_str());
			if (w_c.length() > 0)
				wcid = atoi(w_c.c_str());
			if (n_s.length() > 0)
				nsid = atoi(n_s.c_str());
			if (n_c.length() > 0)
				ncid = atoi(n_c.c_str());
			
			w_ids.append(Swift_BLE_GetServiceAndCharacteristic(identifier.c_str(),wsid,wcid));
			n_ids.append(Swift_BLE_GetServiceAndCharacteristic(identifier.c_str(),nsid,ncid));
			
			if (w_ids.length() > 0){
				w_ids = identifier + "|" + w_ids;
			}
			if (n_ids.length() > 0){
				n_ids = identifier + "|" + n_ids;
			}
			
			if (bindCore->Open(name,w_ids,n_ids,peripheralCore->GetOpenPar().cfg) > 0){
				SetblUpdate(this);
				bind = nullptr;
				ret = G_TRUE;
			}
		}
		CloseChild(bind);
	}
	return(ret);
};
//------------------------------------------------------------------------------------------//
uint32 BLEPeripheral::CWrite(const STDSTR& ids,const STDSTR&  _in){
	STDSTR	w_s,w_c;
	int	wsid = -1,wcid = -1;
	STDSTR	identifier,service,characteristic = "";
	
	CORE_BLEPeripheral	*peripheralCore;

	if (IsOpened()){
		peripheralCore = static_cast<CORE_BLEPeripheral*>(Core());
		identifier = peripheralCore->cgIdentifier;
		
		w_c = ids;
		w_s = Str_ReadSubItem(&w_c, "-");
		
		if (w_s.length() > 0)
			wsid = atoi(w_s.c_str());
		if (w_c.length() > 0)
			wcid = atoi(w_c.c_str());

		characteristic.append(Swift_BLE_GetServiceAndCharacteristic(identifier.c_str(),wsid,wcid));
		service = Str_ReadSubItem(&characteristic,"|");
		
		Swift_BLE_Write(identifier.c_str(),service.c_str(),characteristic.c_str(),_in.c_str(),(int)_in.length());
		return G_TRUE;
	}
	return G_FALSE;
};
//------------------------------------------------------------------------------------------//
/*uint32 BLEPeripheral::CRead(const STDSTR &ids, STDSTR *_out, uint32 num){
	STDSTR	w_s,w_c;
	int	wsid = -1,wcid = -1;
	STDSTR	identifier,service,characteristic = "";
	
	CORE_BLEPeripheral	*peripheralCore;

	if (IsOpened()){
		peripheralCore = static_cast<CORE_BLEPeripheral*>(Core());
		identifier = peripheralCore->cgIdentifier;
		
		w_c = ids;
		w_s = Str_ReadSubItem(&w_c, "-");
		
		if (w_s.length() > 0)
			wsid = atoi(w_s.c_str());
		if (w_c.length() > 0)
			wcid = atoi(w_c.c_str());

		characteristic.append(Swift_BLE_GetServiceAndCharacteristic(identifier.c_str(),wsid,wcid));
		service = Str_ReadSubItem(&characteristic,"|");
		char * buffer;
		
		Swift_BLE_Read(identifier.c_str(),service.c_str(),characteristic.c_str(),buffer,0);
		return G_TRUE;
	}
	return G_FALSE;
};*/
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
#ifndef CommonDefH_MAC
extern "C" int		Swift_BLE_Scan(const char*){return 0;};
extern "C" void		Swift_BLE_Refresh(void){;};
extern "C" void		Swift_BLE_StopScan(void){;};
extern "C" bool		Swift_BLE_IsUpdate(void){return 0;};
extern "C" char*	Swift_BLE_GetDevicesDescription(void){return NULL;};
extern "C" char*	Swift_BLE_GetDevicesDetailDescription(void){return NULL;};
extern "C" char*	Swift_BLE_GetDeviceDetailDescriptionByIndex(int){return NULL;};
extern "C" char*	Swift_BLE_GetDeviceDetailDescriptionByName(const char*){return NULL;};
extern "C" char*	Swift_BLE_GetDeviceDetailDescriptionByIdentifier(const char*){return NULL;};
extern "C" char*	Swift_BLE_GetDeviceDetailDescriptionByAny(const char*){return NULL;};
extern "C" int		Swift_BLE_ConnectByIndex(int){return 0;};
extern "C" int		Swift_BLE_ConnectByName(const char*){return 0;};
extern "C" int		Swift_BLE_ConnectByIdentifier(const char*){return 0;};
extern "C" int		Swift_BLE_ConnectByAny(const char*){return 0;};
extern "C" char*	Swift_BLE_GetPeripheralByIndex(int){return NULL;};
extern "C" char*	Swift_BLE_GetPeripheralByName(const char*){return NULL;};
extern "C" char*	Swift_BLE_GetPeripheralByAny(const char*){return NULL;};
extern "C" char*	Swift_BLE_GetServiceAndCharacteristic(const char*,int sindex,int cindex){return NULL;};
extern "C" int		Swift_BLE_DisconnectByIndex(int){return 0;};
extern "C" int		Swift_BLE_DisconnectByName(const char*){return 0;};
extern "C" int		Swift_BLE_DisconnectByIdentifier(const char*){return 0;};
extern "C" int		Swift_BLE_DisconnectByAny(const char*){return 0;};
extern "C" int		Swift_BLE_GetMTU(const char* identifier,const char* service,const char* characteristic){return 0;};
extern "C" int		Swift_BLE_Write(const char* identifier,const char* service,const char* characteristic,const char* buffer,int len){return 0;};
extern "C" int		Swift_BLE_Read(const char* identifier,const char* service,const char* characteristic,char* buffer,int len){return 0;};
extern "C" int		Swift_BLE_SetNotify(const char* identifier,const char* service,const char* characteristic,uint32){return 0;};
extern "C" bool		Swift_BLE_CheckID(const char* identifier,const char* service,const char* characteristic){return 0;};
#endif
//------------------------------------------------------------------------------------------//
#endif /* Commu_BLE_h */
