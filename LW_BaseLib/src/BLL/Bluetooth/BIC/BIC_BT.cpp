//
//  BIC_BT.cpp
//  SDT
//
//  Created by Leif Wen on 12/09/2020.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "stdafx.h"
//------------------------------------------------------------------------------------------//
#include "BIC_BT.h"
#ifdef BIC_BT_h
#include "Commu_BT_SPP.h"
#include "BIC_Dev.h"
//------------------------------------------------------------------------------------------//
#include "Commu_BT_Swift.h"
//------------------------------------------------------------------------------------------//
CMDID BIC_BT_SCAN::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env,cgCommand,"Scan BT peripherals");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_BT_SCAN::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	STDSTR	strPar1, strPar2;

	Swift_BT_Scan();

	PrintALine(env,"Scanning...");
	if (IntoPressKeyMode(env) > 0){
		Swift_BT_StopScan();
		PrintSuccess(env,"Pressed ESC key,Stopped scan");
	}

	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
bool32 BIC_BT_SCAN::IsExitPressKeyMode(CMD_ENV* env)const{
	STDSTR	ret = "";
	if (Swift_BT_IsUpdate()){
		ret.append(Swift_BT_GetDevicesDescription());
		PrintWithDividingLine(env);
		PrintStrNL(env,ret);
	}
	if (Swift_BT_ScanState() == 3){	//complete = 3
		ret.append(Swift_BT_GetDevicesDescription());
		PrintWithDividingLine(env);
		PrintStrNL(env,ret);
		PrintStrNL(env,"Scan commpleted.");
		return G_TRUE;
	}
	return G_FALSE;
};
//------------------------------------------------------------------------------------------//
CMDID BIC_BT_LIST::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env,cgCommand,"List BT peripherals");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_BT_LIST::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	
	PrintStrNL(env,Swift_BT_GetDevicesDescription());
	SYS_SleepMS(10);
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIC_BT_SPP::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env,cgCommand,"Connect BT peripheral with SPP");
	if (B_ChkFLAG32(flag, CMD_blPrintSimple))
		return(cgCommandID);
	PrintHelpSubItem(env,"<mac addr>","MAC Address");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_BT_SPP::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	STDSTR	strPar1, strPar2;
	if (msg.length() == 0)
		return(cgCommandID);

	PrintALine(env,"Connecting...");

	if (BIC_ENV_DEV::GetEDA(env)->Open(DRV_BTSPP, SetOpenPar(OPEN_None,msg,1,0)) == G_FALSE){
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
BIC_BT::BIC_BT(void) : BIC_BASE_S() {
	/*SetSFlag(CMD_blTrySubCMD);*/
	cgCommandID = BIC_ID_BT;
	cgCommand = "bt";
	cgConsoleName = cgCommand;
	cgHelpName = "BT control";

	*this
	< cgSub_scan
	< cgSub_list
	< cgSub_spp
	;
};
//------------------------------------------------------------------------------------------//
#endif /* BIC_BT_h */
