//
//  Commu_BLE_Swift.h
//  SDT
//
//  Created by Leif Wen on 12/09/2020.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

//------------------------------------------------------------------------------------------//
#ifndef Commu_BLE_Swift_h
#define Commu_BLE_Swift_h
#ifdef Commu_BLE_Swift_h
//------------------------------------------------------------------------------------------//
extern "C" int		Swift_BLE_Scan(const char*);
extern "C" void		Swift_BLE_Refresh(void);
extern "C" void		Swift_BLE_StopScan(void);
extern "C" bool		Swift_BLE_IsUpdate(void);
extern "C" char*	Swift_BLE_GetDevicesDescription(void);
extern "C" char*	Swift_BLE_GetDevicesDetailDescription(void);
extern "C" char*	Swift_BLE_GetDeviceDetailDescriptionByIndex(int);
extern "C" char*	Swift_BLE_GetDeviceDetailDescriptionByName(const char*);
extern "C" char*	Swift_BLE_GetDeviceDetailDescriptionByIdentifier(const char*);
extern "C" char*	Swift_BLE_GetDeviceDetailDescriptionByAny(const char*);
extern "C" int		Swift_BLE_ConnectByIndex(int);
extern "C" int		Swift_BLE_ConnectByName(const char*);
extern "C" int		Swift_BLE_ConnectByIdentifier(const char*);
extern "C" int		Swift_BLE_ConnectByAny(const char*);
extern "C" char*	Swift_BLE_GetPeripheralByIndex(int);
extern "C" char*	Swift_BLE_GetPeripheralByName(const char*);
extern "C" char*	Swift_BLE_GetPeripheralByAny(const char*);
extern "C" char*	Swift_BLE_GetServiceAndCharacteristic(const char*,int sindex,int cindex);
extern "C" int		Swift_BLE_DisconnectByIndex(int);
extern "C" int		Swift_BLE_DisconnectByName(const char*);
extern "C" int		Swift_BLE_DisconnectByIdentifier(const char*);
extern "C" int		Swift_BLE_DisconnectByAny(const char*);
extern "C" int		Swift_BLE_GetMTU(const char* identifier,const char* service,const char* characteristic);
extern "C" int		Swift_BLE_Write(const char* identifier,const char* service,const char* characteristic,const char* buffer,int len);
extern "C" int		Swift_BLE_Read(const char* identifier,const char* service,const char* characteristic,char* buffer,int len);
extern "C" int		Swift_BLE_SetNotify(const char* identifier,const char* service,const char* characteristic,uint32);
extern "C" bool		Swift_BLE_CheckID(const char* identifier,const char* service,const char* characteristic);
//------------------------------------------------------------------------------------------//
#endif /* Commu_BLE_Swift_h */
#endif /* Commu_BLE_Swift_h */
