//
//  BIC_BLE.h
//  SDT
//
//  Created by Leif Wen on 12/09/2020.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "BIC_Commu.h"
#if defined BIC_Commu_h
//------------------------------------------------------------------------------------------//
#ifndef BIC_BLE_h
#define BIC_BLE_h
#ifdef BIC_BLE_h
//------------------------------------------------------------------------------------------//
class BIC_BLE_SCAN : public BIC_BASE{
	public:
				 BIC_BLE_SCAN(void) : BIC_BASE() {cgCommandID = BIC_ID_BLE_SCAN;cgCommand = "scan";};
		virtual ~BIC_BLE_SCAN(void){;};
	public:
		virtual	CMDID	Command				(CMD_ENV* env,const STDSTR& msg,void* p)const;
		virtual	CMDID	Help				(CMD_ENV* env,uint32 flag)const;
	private:
		virtual	bool32	IsExitPressKeyMode	(CMD_ENV* env)const;
};
//------------------------------------------------------------------------------------------//
BIC_CLASSTYPE	(BLE_LIST			,"list");
BIC_CLASSTYPE	(BLE_CONNECT		,"connect");
BIC_CLASSTYPE	(BLE_DISCONNECT		,"disconnect");
BIC_CLASSTYPE	(BLE_BIND			,"bind");
BIC_CLASSTYPE	(BLE_UNBIND			,"unbind");
BIC_CLASSTYPE	(BLE_ATTACH			,"attach");
BIC_CLASSTYPE	(BLE_WRITE			,"write");
BIC_CLASSTYPE	(BLE_READ			,"read");
//BIC_CLASSTYPE	(BLE_INDICATE		,"indicate");
//------------------------------------------------------------------------------------------//
class BIC_BLE : public BIC_BASE_S{
	public:
				 BIC_BLE(void);
		virtual ~BIC_BLE(void){;};
	private:
		BIC_BLE_SCAN			cgSub_scan;
		BIC_BLE_LIST			cgSub_list;
		BIC_BLE_CONNECT			cgSub_connect;
		BIC_BLE_DISCONNECT		cgSub_disconnect;
		BIC_BLE_BIND			cgSub_bind;
		BIC_BLE_UNBIND			cgSub_unbind;
		BIC_BLE_ATTACH			cgSub_attach;
		BIC_BLE_WRITE			cgSub_write;
		BIC_BLE_READ			cgSub_read;
};
//------------------------------------------------------------------------------------------//
#endif /* BIC_BLE_h */
#endif /* BIC_BLE_h */
#endif
