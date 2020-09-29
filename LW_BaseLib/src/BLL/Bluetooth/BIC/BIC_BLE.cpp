//
//  BIC_BLE.cpp
//  SDT
//
//  Created by Leif Wen on 12/09/2020.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "stdafx.h"
//------------------------------------------------------------------------------------------//
#include "BIC_BLE.h"
#ifdef BIC_BLE_h
#include "Commu_BLE.h"
#include "BIC_Dev.h"
//------------------------------------------------------------------------------------------//
#include "Commu_BLE_Swift.h"
//------------------------------------------------------------------------------------------//
CMDID BIC_BLE_SCAN::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env,cgCommand,"Scan BLE peripherals");
	if (B_ChkFLAG32(flag, CMD_blPrintSimple))
		return(cgCommandID);
	PrintHelpSubItem(env,"[-r]"		,"Refresh scan");
	PrintHelpSubItem(env,"[UUIDs]"	,"Peripherals UUIDs");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_BLE_SCAN::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	STDSTR	strPar1, strPar2;
	int ret = 0;
	
	strPar1 = "";
	if (msg.length() > 0){
		SplitPar1(strPar1,strPar2,msg);
		if (strPar1 == "-r"){
			Swift_BLE_Refresh();
			strPar1 = strPar2;
		}
	}

	ret = Swift_BLE_Scan(strPar1.c_str());
	if (ret == 0){
		SYS_SleepMS(100);
		ret = Swift_BLE_Scan(strPar1.c_str());
	}
	if (ret == 5){//.poweredOn
		PrintALine(env,"Scanning...");
		if (IntoPressKeyMode(env) > 0){
			Swift_BLE_StopScan();
			PrintSuccess(env,"Pressed ESC key, stopped scan");
		}
	}
	else{
		PrintFail(env,"unknown reason");
	}
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
bool32 BIC_BLE_SCAN::IsExitPressKeyMode(CMD_ENV* env)const{
	STDSTR	ret = "";
	if (Swift_BLE_IsUpdate()){
		ret.append(Swift_BLE_GetDevicesDescription());
		PrintWithDividingLine(env);
		PrintStrNL(env,ret);
	}
	return G_FALSE;
};
//------------------------------------------------------------------------------------------//
static inline OPEN_PAR SetOpenPar(const STDSTR& msg){
	STDSTR		strPar1, strPar2;
	OPEN_PAR	par;
	par.cfg = 0;
	par.port = 0;
	BIC_BLE_SCAN::SplitPar1(strPar1,strPar2,msg);
	if (strPar1 == "-n"){
		par.name = strPar2;
		par.type = BLE_OPENTYPE_NAME;
	}
	else if (strPar1 == "-id"){
		par.name = strPar2;
		par.type = BLE_OPENTYPE_ID;
	}
	else if (strPar1 == "-i"){
		par.name = strPar2;
		par.type = BLE_OPENTYPE_INDEX;
	}
	else{
		par.name = strPar1;
		par.type = BLE_OPENTYPE_ANY;
	}
	return(par);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_BLE_LIST::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env,cgCommand,"List BLE peripherals");
	if (B_ChkFLAG32(flag, CMD_blPrintSimple))
		return(cgCommandID);
	PrintHelpSubItem(env,"[[-n]"	,"Peripheral name");
	PrintHelpSubItem(env," |[-id]"	,"Peripheral identifier");
	PrintHelpSubItem(env," |[-i]]"	,"Peripheral index");
	PrintHelpSubItem(env,"<content>","name/identifier/index");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_BLE_LIST::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	
	PrintStrNL(env,BLEPeripheral::ListDescription(SetOpenPar(msg)));
	SYS_SleepMS(10);
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIC_BLE_CONNECT::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env,cgCommand,"Connect BLE peripheral");
	if (B_ChkFLAG32(flag, CMD_blPrintSimple))
		return(cgCommandID);
	PrintHelpSubItem(env,"[[-n]"	,"Peripheral name");
	PrintHelpSubItem(env," |[-id]"	,"Peripheral identifier");
	PrintHelpSubItem(env," |[-i]]"	,"Peripheral index");
	PrintHelpSubItem(env,"<content>","name/identifier/index");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_BLE_CONNECT::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	STDSTR	strPar1, strPar2;
	if (msg.length() == 0)
		return(cgCommandID);

	PrintALine(env,"Connecting...");

	if (BIC_ENV_DEV::GetEDA(env)->Open(DRV_BLEPeripheral, SetOpenPar(msg)) == G_FALSE){
		SETBICRESTULT(G_FALSE);
	}
	else{
		SETBICRESTULT(G_TRUE);
	}
	SYS_SleepMS(10);
		
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIC_BLE_DISCONNECT::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env,cgCommand,"Disconnect BLE peripheral");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_BLE_DISCONNECT::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	if (msg.length() > 0)
		return(Help(env,0));
	BIC_ENV_DEV::GetEDA(env)->cdevbus->CloseSelf(0);
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
static bool32 BIC_PrintBind(CMD_ENV* env,BLEBIND* bind){
	STDSTR		strPrintData;
	if (BIC_BLE_BIND::GetSTDOUT(env) != nullptr){
		if (bind != nullptr){
			strPrintData = Str_ToStr(TNF::GetDRNodeID(bind));
			Str_AddSpaceInFront(&strPrintData,3);
			strPrintData += ".";
			
			if (DRV_NODE::IsSelected(bind) == G_FALSE){
				strPrintData += "   ";
			}
			else{
				strPrintData += " * ";
			}
			*BIC_BLE_BIND::GetSTDOUT(env)  << Begin() << NL() << COL_clDefault << strPrintData
			<< COL_clYellow << bind->Core()->GetOpenPar().name << NL()
			<< COL_clCyan
			<< "     Received : " << Str_ToStr(bind->RxBytes()) << " bytes\n"
			<< "     Echo     : " << Str_ToStr(bind->DPBytes()) << " bytes\n"
			<< "     Sent     : " << Str_ToStr(bind->TxBytes()) << " bytes\n"
			<< Endl();
		}
	}
	return G_TRUE;
};
//------------------------------------------------------------------------------------------//
CMDID BIC_BLE_BIND::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env,cgCommand,"Bind BLE characteristics");
	if (B_ChkFLAG32(flag, CMD_blPrintSimple))
		return(cgCommandID);
	PrintHelpSubItem(env,""				,"List binds");
	PrintHelpSubItem(env,"[-n ids]"		,"Bind Notify with S.index-C.index");
	PrintHelpSubItem(env,"[-w ids]"		,"Bind Write with S.index-C.index");
	PrintHelpSubItem(env,"[-as name]"	,"Bind aliasName");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_BLE_BIND::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	ExpandDevBusAttr* attr = BIC_ENV_DEV::GetEDA(env);
	BLEPeripheral *peripheral = static_cast<BLEPeripheral*>(attr->cdevbus->GetDrv(DRV_BLEPeripheral));
	STDSTR	strPar1, strPar2;
	STDSTR	aliasName = "",_write = "",_notify = "";

	do{
		if ((attr->IsOpened() == G_FALSE) || (peripheral == nullptr)){
			PrintFail(env,"no connect");
			break;
		}
		
		if (msg.length() == 0){
			PrintStrNL(env,COLOR(COL_clDefault,IUD(DEV_LINE_STAR)));
			TREE_DownChain_Traversal_LINE_nolock(BLEBIND,peripheral,BIC_PrintBind(env,_opNode));
			PrintStrNL(env,COLOR(COL_clDefault,IUD(DEV_LINE_STAR)));
			break;
		}

		SplitPar1(strPar1,strPar2,msg);
		
		do{
			if (strPar1 == "-w"){
				SplitPar1(&strPar1,&strPar2);
				_write = strPar1;
			}
			else if (strPar1 == "-n"){
				SplitPar1(&strPar1,&strPar2);
				_notify = strPar1;
			}
			else if (strPar1 == "-as"){
				SplitPar1(&strPar1,&strPar2);
				aliasName = strPar1;
			}
			SplitPar1(&strPar1,&strPar2);
		}while(strPar1.length() > 0);
		peripheral->Bind(aliasName, _write, _notify);
	}while(0);
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIC_BLE_UNBIND::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env,cgCommand,"Bind BLE characteristics");
	if (B_ChkFLAG32(flag, CMD_blPrintSimple))
		return(cgCommandID);
	PrintHelpSubItem(env,"<num>","Bind number");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_BLE_UNBIND::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	ExpandDevBusAttr* attr = BIC_ENV_DEV::GetEDA(env);
	BLEPeripheral *peripheral = static_cast<BLEPeripheral*>(attr->cdevbus->GetDrv(DRV_BLEPeripheral));
	STDSTR	strPar1, strPar2;
	uint32	index;
	
	if (msg.length() == 0)
		return(Help(env,0));
	
	if ((attr->IsOpened()) && (peripheral != nullptr)){
		SplitPar1(strPar1, strPar2, msg);
		if (strPar1 == "all"){
			peripheral->CloseAllChild();
			SetblUpdate(peripheral);
		}
		else{
			while(strPar1.length() > 0){
				index = (uint32)strtol(strPar1.c_str(),nullptr,10);
				peripheral->CloseChild(static_cast<BLEBIND*>(FindInDownChainByDRNodeID(peripheral, index)));
				SplitPar1(&strPar1, &strPar2);
			};
			SetblUpdate(attr->AServer());
		}
	}
	else{
		PrintFail(env,"no connect");
	}
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIC_BLE_ATTACH::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env,cgCommand,"Bind attach");
	if (B_ChkFLAG32(flag, CMD_blPrintSimple))
		return(cgCommandID);
	PrintHelpSubItem(env,"<num>","Bind number");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_BLE_ATTACH::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	ExpandDevBusAttr* attr = BIC_ENV_DEV::GetEDA(env);
	BLEPeripheral *peripheral = static_cast<BLEPeripheral*>(attr->cdevbus->GetDrv(DRV_BLEPeripheral));
	
	BLEBIND*	bind;
	int32		num;
	
	if (msg.length() == 0)
		return(Help(env,0));
	
	if ((peripheral != nullptr) && (peripheral->IsOpened())){
		num = atoi(msg.c_str());
		bind = peripheral->Attach(num);
		
		if (bind != nullptr){
			PrintStrNL(env,COLOR(COL_clDefault,IUD(DEV_LINE_STAR)));
			BIC_PrintBind(env,bind);
			PrintStrNL(env,COLOR(COL_clDefault,IUD(DEV_LINE_STAR)));
		}
	}
	else{
		PrintFail(env,"no connect");
	}
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIC_BLE_WRITE::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env,cgCommand,"Write BLE characteristic");
	if (B_ChkFLAG32(flag, CMD_blPrintSimple))
		return(cgCommandID);
	PrintHelpSubItem(env,"<ids>"		,"S.index-C.index");
	PrintHelpSubItem(env,"<data>"		,"data");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_BLE_WRITE::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	ExpandDevBusAttr* attr = BIC_ENV_DEV::GetEDA(env);
	BLEPeripheral *peripheral = static_cast<BLEPeripheral*>(attr->cdevbus->GetDrv(DRV_BLEPeripheral));
	STDSTR	strPar1, strPar2;

	if (msg.length() == 0)
		return(cgCommandID);
	
	if ((peripheral != nullptr) && (peripheral->IsOpened())){
		SplitPar1(strPar1,strPar2,msg);
		peripheral->CWrite(strPar1, strPar2);
	}
	else{
		PrintFail(env,"no connect");
	}
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIC_BLE_READ::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env,cgCommand,"Read BLE characteristic/Not implemented");
	if (B_ChkFLAG32(flag, CMD_blPrintSimple))
		return(cgCommandID);
	PrintHelpSubItem(env,"<ids>"		,"S.index-C.index");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_BLE_READ::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	/*ExpandDevBusAttr* attr = BIC_ENV_DEV::GetEDA(env);
	BLEPeripheral *peripheral = static_cast<BLEPeripheral*>(attr->cdevbus->GetDrv(DRV_BLEPeripheral));
	STDSTR	strPar1, strPar2;

	if (msg.length() == 0)
		return(cgCommandID);
	
	if ((peripheral != nullptr) && (peripheral->IsOpened())){
		SplitPar1(strPar1,strPar2,msg);
		peripheral->CRead(strPar1, &strPar2,10);
	}
	else{
		PrintFail(env,"no connect");
	}*/
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//









//------------------------------------------------------------------------------------------//
BIC_BLE::BIC_BLE(void) : BIC_BASE_S() {
	/*SetSFlag(CMD_blTrySubCMD);*/
	cgCommandID = BIC_ID_BLE;
	cgCommand = "ble";
	cgConsoleName = cgCommand;
	cgHelpName = "BLE control";

	*this
	< cgSub_scan
	< cgSub_list
	< cgSub_connect
	< cgSub_disconnect
	< cgSub_bind
	< cgSub_unbind
	< cgSub_attach
	< cgSub_write
	< cgSub_read
	;
};
//------------------------------------------------------------------------------------------//
#endif /* BIC_BLE_h */
