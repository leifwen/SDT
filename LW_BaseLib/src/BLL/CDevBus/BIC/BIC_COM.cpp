//
//  BIC_COM.cpp
//  SDT
//
//  Created by Leif Wen on 16/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "stdafx.h"
//------------------------------------------------------------------------------------------//
#include "BIC_COM.h"
#ifdef BIC_COM_h
#include "Commu_ComEnum.h"
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIC_COM::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env,cgCommand,"Set as COM mode");
	if (B_ChkFLAG32(flag, CMD_blPrintSimple))
		return(cgCommandID);
	PrintHelpSubItem(env,"[name][br]","COM name and baudrate");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_COM::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	BIC_CONNECT::SetConnectPar(env,msg,OPEN_COM);
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIC_COM_BR::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env,cgCommand,"Set COM baudrate");
	if (B_ChkFLAG32(flag, CMD_blPrintSimple))
		return(cgCommandID);
	PrintHelpSubItem(env,"<n>","Baudrate");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_COM_BR::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	ExpandDevBusAttr* attr = BIC_ENV_DEV::GetEDA(env);
		if (msg.length() == 0)
			return(Help(env,0));
	
	if (attr->IsCom()){
		BIC_CONNECT::SetConnectPar2(env,msg);
	}
	else{
		PrintFail(env,"no COM device");
	}
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIC_COM_DTR::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env,cgCommand,"Set COM DTR");
	if (B_ChkFLAG32(flag, CMD_blPrintSimple))
		return(cgCommandID);
	PrintHelpSubItem(env,"[-H [t]]","Set to High, t is time, unit is S");
	PrintHelpSubItem(env,"[-L [t]]","Set to Low , t is time, unit is S");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_COM_DTR::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	ExpandDevBusAttr* attr = BIC_ENV_DEV::GetEDA(env);
	STDSTR		strPar1,strPar2,strPar3;
	SYS_TIME_S	timeS;
	bool32		bltime;
	
	if (attr->IsComOpened()){
		if (msg.length() == 0){
			PrintResult(env,"COM DTR is",((attr->AComCore()->GetDTRStatus()) ? "low" : "high"));
		}
		else{
			SplitPar1(strPar1, strPar3, msg);
			do{
				bltime = G_FALSE;
				SplitPar1(&strPar2,&strPar3);
				if ((strPar2.length() > 0) && (strPar2 != "-H") && (strPar2 != "-L")){
					SYS_Delay_SetTS(&timeS,atof(strPar2.c_str()) * 1000);
					bltime = G_TRUE;
				}
				if (strPar1 == "-H"){
					attr->AComCore()->SetDTR(1);
				}
				else if (strPar1 == "-L"){
					attr->AComCore()->SetDTR(0);
				}
				else{
					break;
				}
				if (bltime > 0){
					if (BIC_CONNECT::PressAnyKey(env,attr,&timeS) == 27){
						PrintSuccess(env,"Pressed ESC key, stopped execute command");
						return(cgCommandID);
					}
				}
				else{
					strPar1 = strPar2;
					continue;
				}
				SplitPar1(&strPar1,&strPar3);
			}while(strPar1.length() > 0);
		}
	}
	else{
		PrintFail(env,"no COM connected");
	}
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIC_COM_RTS::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env,cgCommand,"Set COM RTS");
	if (B_ChkFLAG32(flag, CMD_blPrintSimple))
		return(cgCommandID);
	PrintHelpSubItem(env,"[-H [t]]","Set to High, t is time, unit is S");
	PrintHelpSubItem(env,"[-L [t]]","Set to Low , t is time, unit is S");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_COM_RTS::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	ExpandDevBusAttr* attr = BIC_ENV_DEV::GetEDA(env);
	STDSTR		strPar1,strPar2,strPar3;
	SYS_TIME_S	timeS;
	int32		bltime;
	
	if (attr->IsComOpened()){
		if (msg.length() == 0){
			PrintResult(env,"COM RTS is",((attr->AComCore()->GetRTSStatus()) ? "low" : "high"));
		}
		else{
			SplitPar1(strPar1, strPar3, msg);
			do{
				bltime = G_FALSE;
				SplitPar1(&strPar2,&strPar3);
				if ((strPar2.length() > 0) && (strPar2 != "-H") && (strPar2 != "-L")){
					SYS_Delay_SetTS(&timeS,atof(strPar2.c_str()) * 1000);
					bltime = G_TRUE;
				}
				if (strPar1 == "-H"){
					attr->AComCore()->SetRTS(1);
				}
				else if (strPar1 == "-L"){
					attr->AComCore()->SetRTS(0);
				}
				else{
					break;
				}
				if (bltime > 0){
					if (BIC_CONNECT::PressAnyKey(env,attr,&timeS) == 27){
						PrintSuccess(env,"Pressed ESC key, stopped execute command");
						return(cgCommandID);
					}
				}
				else{
					strPar1 = strPar2;
					continue;
				}
				SplitPar1(&strPar1,&strPar3);
			}while(strPar1.length() > 0);
		}
	}
	else{
		PrintFail(env,"no COM connected");
	}
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIC_COM_CTSFLOW::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env,cgCommand,"Set COM CTS flow");
	if (B_ChkFLAG32(flag, CMD_blPrintSimple))
		return(cgCommandID);
	PrintHelpSubItem(env,"[on|off]"	,"Enable/disable hard CTS flow");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_COM_CTSFLOW::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	ExpandDevBusAttr* attr = BIC_ENV_DEV::GetEDA(env);
	if (attr->IsComOpened()){
		if (msg == "on"){
			attr->AComCore()->SetCTSFlow(G_TRUE);
			PrintResult(env,"Enable hard CTS flow");
		}
		else if (msg == "off"){
			attr->AComCore()->SetCTSFlow(G_FALSE);
			PrintResult(env,"Disable hard CTS flow");
		}
		else{
			Help(env,0);
		}
	}
	else{
		PrintFail(env,"no COM connected");
	}
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIC_COM_DSRFLOW::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env,cgCommand,"Set COM DSR flow");
	if (B_ChkFLAG32(flag, CMD_blPrintSimple))
		return(cgCommandID);
	PrintHelpSubItem(env,"[on|off]"	,"Enable/disable hard DSR flow");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_COM_DSRFLOW::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	ExpandDevBusAttr* attr = BIC_ENV_DEV::GetEDA(env);
	if (attr->IsComOpened()){
		if (msg == "on"){
			attr->AComCore()->SetDSRFlow(G_TRUE);
			PrintResult(env,"Enable hard DSR flow");
		}
		else if (msg == "off"){
			attr->AComCore()->SetDSRFlow(G_FALSE);
			PrintResult(env,"Disable hard DSR flow");
		}
		else{
			Help(env,0);
		}
	}
	else{
		PrintFail(env,"no COM connected");
	}
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIC_COM_MS::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env,cgCommand,"Modem status report");
	if (B_ChkFLAG32(flag, CMD_blPrintSimple))
		return(cgCommandID);
	PrintHelpSubItem(env,"[on|off]"	,"Enable/disable modem status report");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_COM_MS::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	ExpandDevBusAttr* attr = BIC_ENV_DEV::GetEDA(env);
	if (attr->IsComOpened()){
		if (msg == "on"){
			B_SetFLAG64(*attr->envcfg, ODEV_FLAG_EnMSReport);
			PrintResult(env,"Enable modem status report");
		}
		else if (msg == "off"){
			B_ClrFLAG64(*attr->envcfg, ODEV_FLAG_EnMSReport);
			PrintResult(env,"Disable modem status report");
		}
		else if (msg.length() == 0){
			PrintResult(env,attr->AComCore()->GetFullModemStatus());
		}
		else{
			Help(env,0);
		}
	}
	else{
		PrintFail(env,"no COM connected");
	}
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
#if defined Commu_ComEnum_h && defined CommonDefH_VC
//------------------------------------------------------------------------------------------//
CMDID BIC_COM_LSCOM::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env,cgCommand,"List valid COM");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_COM_LSCOM::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	if ((BIC_ENV_DEV::GetValidComList(env) != nullptr) && (CMD_ENV::GetSTDOUT(env) != nullptr)){
		BIC_ENV_DEV::GetValidComList(env)->Refresh();
		
		*GetSTDOUT(env) << Begin() << NL() << COL_DivStar_Default << COL_Result;
		TREE_DownChain_Traversal_LINE(IPCOMNAME,BIC_ENV_DEV::GetValidComList(env),
			if ((_opNode->typeID == COMENUM_DEVID_APICOM) && (_opNode->blAvailable))
				*GetSTDOUT(env) << _opNode->strShowName << "\n";
		);
		*GetSTDOUT(env) << COL_DivStar_Default << Endl();
	}
 return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
#endif
//------------------------------------------------------------------------------------------//
CMDID BIC_COM_OPENV::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env,cgCommand,"Open as virtual COM");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_COM_OPENV::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	ExpandDevBusAttr* attr = BIC_ENV_DEV::GetEDA(env);
#ifdef CommonDefH_Unix
	if (BIC_CONNECT::SetConnectPar(env,msg,OPEN_COMV)){
		if (attr->IsOpened() == G_FALSE){
			attr->Open();
		}
	}
#endif
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
BIC_ACOM::BIC_ACOM(void) : BIC_BASE_S() {
	SetSFlag(CMD_blTrySubCMD);
	cgCommandID = BIC_ID_ASOCKET;
	cgCommand = "acom";
	cgConsoleName = cgCommand;
	cgHelpName = "COM setting";
	
	*this
#if defined Commu_ComEnum_h && defined CommonDefH_VC
	< cgSub_com_lscom
#endif
	< cgSub_com < cgSub_com_br < cgSub_com_openv< cgSub_com_ms
	< cgSub_com_dtr < cgSub_com_rts
	< cgSub_com_ctsflow < cgSub_com_dsrflow;
};
//------------------------------------------------------------------------------------------//
#endif /* BIC_COM_h */
