/*
 * Copyright (c) 2012-2014
 * All rights reserved.
 *
 * File Name: BIC_Socket.cpp
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2014.01.25
 
 */
//------------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "BIC_Socket.h"
#include "Comm_Convert.h"
#include "AppLayer.h"
//------------------------------------------------------------------------------------------//
#ifdef BIC_SocketH
//------------------------------------------------------------------------------------------//
int32 BIC_TCP::Command(BIC_ENV *env, const STDSTR &par,void *eda)const{
	if (BI_SetConnectPar(static_cast<ExpandDeviceAttr*>(eda),par,CSType_TCP) == 0)
		PrintFail(env,"already connected");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_UDP::Command(BIC_ENV *env, const STDSTR &par,void *eda)const{
	if (BI_SetConnectPar(static_cast<ExpandDeviceAttr*>(eda),par,CSType_UDP) == 0)
		PrintFail(env,"already connected");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_TCPS::Command(BIC_ENV *env, const STDSTR &par,void *eda)const{
	if (BI_SetConnectPar(static_cast<ExpandDeviceAttr*>(eda),par,CSType_TCPS) == 0)
		PrintFail(env,"already connected");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_UDPS::Command(BIC_ENV *env, const STDSTR &par,void *eda)const{
	if (BI_SetConnectPar(static_cast<ExpandDeviceAttr*>(eda),par,CSType_UDPS) == 0)
		PrintFail(env,"already connected");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_PORT::Command(BIC_ENV *env, const STDSTR &par,void *eda)const{
	if (par.length() == 0)
		return(Help(env));
	if (BI_SetConnectPar2(static_cast<ExpandDeviceAttr*>(eda),par) == 0)
		PrintFail(env,"already connected");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_SI::Command(BIC_ENV *env, const STDSTR &par,void *eda)const{
	ExpandDeviceAttr* attr = static_cast<ExpandDeviceAttr*>(eda);
	
	if (par.length() > 0)
		return(Help(env));

	if (attr->IsServerOpened()){
		PrintStrNL(env,Data(COL_clDefault,DEV_LINE_START));
		TREE_LChildRChain_Traversal_LINE_nolock(ASOCKET,attr->AServer(),BIC_SI_LS_PrintSocket(env,operateNode_t));
		PrintStrNL(env,Data(COL_clDefault,DEV_LINE_START));
	}
	else{
		PrintFail(env,"no connect");
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_SI::BIC_SI_LS_PrintSocket(BIC_ENV *env,ASOCKET *tSocket){
	STDSTR		strPrintData;
	
	if (tSocket != nullptr){
		strPrintData = Str_ToString(GetdRNodeID(tSocket));
		AddSpaceInFront(&strPrintData,3);
		strPrintData += ".";
		
		if (tSocket->CheckSelected() == 0){
			strPrintData += "   ";
		}
		else{
			strPrintData += " * ";
		}
		PrintNL(env) << COL_clDefault << strPrintData
		<< ColData(COL_clYellow,tSocket->GetBufName()) << ColData(COL_clDefault,"@") << ColData(COL_clYellow,Str_ToString(tSocket->GetBufPar())) << NL
		<< COL_clCyan
		<< "     Received : " << Str_ToString(tSocket->RxBytes()) << " bytes\n"
		<< "     Echo     : " << Str_ToString(tSocket->FwBytes()) << " bytes\n"
		<< "     Sent     : " << Str_ToString(tSocket->TxBytes()) << " bytes\n"
		<< Endl;
	}
	return 1;
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_SS::Command(BIC_ENV *env, const STDSTR &par,void *eda)const{
	ExpandDeviceAttr* attr = static_cast<ExpandDeviceAttr*>(eda);
	ASOCKET			*selSocket;
	int32 			num;
	
	if (par.length() == 0)
		return(Help(env));
	
	if (attr->IsServerOpened()){
		num = atoi(par.c_str());
		
		selSocket = static_cast<ASOCKET*>(FindInLChildRChainByDRNodeID(attr->AServer(), num));
		if (attr->AServer()->GetSelDB() == selSocket){
			attr->AServer()->ChildClrSel(selSocket);
		}
		else{
			attr->AServer()->ChildSetSel(selSocket);
		}
		
		PrintStrNL(env,Data(COL_clDefault,DEV_LINE_START));
		BIC_SI::BIC_SI_LS_PrintSocket(env,selSocket);
		PrintStrNL(env,Data(COL_clDefault,DEV_LINE_START));
	}
	else{
		PrintFail(env,"no connect");
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_SSD::Command(BIC_ENV *env, const STDSTR &par,void *eda)const{
	ExpandDeviceAttr* attr = static_cast<ExpandDeviceAttr*>(eda);
	
	int32 num;

	if (par.length() == 0)
		return(Help(env));
	
	if (attr->IsServerOpened()){
		num = atoi(par.c_str());
		attr->AServer()->ChildClose(static_cast<ASOCKET*>(FindInLChildRChainByDRNodeID(attr->AServer(), num)));
	}
	else{
		PrintFail(env,"no connect");
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif
