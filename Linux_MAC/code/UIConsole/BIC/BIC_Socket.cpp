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
//------------------------------------------------------------------------------------------//
int32 BIC_TCP::Command(BICPAR *tBICPAR, const std::string &par,std::string *ret)const{
	*ret = "";
	if (BI_SET_ConnectPar(tBICPAR,par,DEVICE::DEVID_TCPClient) == 0)
		PrintDoRet(tBICPAR,"Set fail due to already connected");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_TCP::Help(BICPAR *tBICPAR,int32 blDetail)const{
	PrintHelpItem(tBICPAR,cgCommand,"Set as TCP mode.");
	if (blDetail == 0)
		return(cgReturnCode);
	PrintHelpItem(tBICPAR,"     [IP][PORT]","Destination IP and port.");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_UDP::Command(BICPAR *tBICPAR, const std::string &par,std::string *ret)const{
	*ret = "";
	if (BI_SET_ConnectPar(tBICPAR,par,DEVICE::DEVID_UDPClient) == 0)
		PrintDoRet(tBICPAR,"Set fail set due to already connected");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_UDP::Help(BICPAR *tBICPAR,int32 blDetail)const{
	PrintHelpItem(tBICPAR,cgCommand,"Set as UDP mode.");
	if (blDetail == 0)
		return(cgReturnCode);
	PrintHelpItem(tBICPAR,"     [IP][PORT]","Destination IP and port.");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_TCPS::Command(BICPAR *tBICPAR, const std::string &par,std::string *ret)const{
	*ret = "";
	if (BI_SET_ConnectPar(tBICPAR,par,DEVICE::DEVID_TCPServer) == 0)
		PrintDoRet(tBICPAR,"Set fail due to already connected");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_TCPS::Help(BICPAR *tBICPAR,int32 blDetail)const{
	PrintHelpItem(tBICPAR,cgCommand,"Set as TCP server mode.");
	if (blDetail == 0)
		return(cgReturnCode);
	PrintHelpItem(tBICPAR,"     [PORT]","Local listen port.");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_UDPS::Command(BICPAR *tBICPAR, const std::string &par,std::string *ret)const{
	*ret = "";
	if (BI_SET_ConnectPar(tBICPAR,par,DEVICE::DEVID_UDPServer) == 0)
		PrintDoRet(tBICPAR,"Set fail due to already connected");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_UDPS::Help(BICPAR *tBICPAR,int32 blDetail)const{
	PrintHelpItem(tBICPAR,cgCommand,"Set as UDP server mode.");
	if (blDetail == 0)
		return(cgReturnCode);
	PrintHelpItem(tBICPAR,"     [PORT]","Local listen port.");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_PORT::Command(BICPAR *tBICPAR, const std::string &par,std::string *ret)const{
	*ret = "";
	if (par.length() == 0)
		return(Help(tBICPAR));
	if (BI_SET_ConnectPar2(tBICPAR,par) == 0)
		PrintDoRet(tBICPAR,"Set fail due to already connected");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_PORT::Help(BICPAR *tBICPAR,int32 blDetail)const{
	PrintHelpItem(tBICPAR,cgCommand,"Set port.");
	if (blDetail == 0)
		return(cgReturnCode);
	PrintHelpItem(tBICPAR,"     <n>","Port.");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_SI::Help(BICPAR *tBICPAR,int32 blDetail)const{
	PrintHelpItem(tBICPAR,cgCommand,"Socket information.");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_SI::Command(BICPAR *tBICPAR, const std::string &par,std::string *ret)const{
	*ret = "";
	if (par.length() > 0)
		return(Help(tBICPAR));

	if ((tBICPAR->sdtApp->m_Device.cgDevType == DEVICE::DEVID_TCPServer) || (tBICPAR->sdtApp->m_Device.cgDevType == DEVICE::DEVID_UDPServer)){
		PrintStrN(tBICPAR,DEV_LINE_START,RICH_LIN_clDefault);
		tBICPAR->sdtApp->m_Device.cgAPISocketServer->Spin_InUse_set();
		RTREE_LChildRChain_Traversal_LINE_nolock(APISocket,tBICPAR->sdtApp->m_Device.cgAPISocketServer,BIC_SI_LS_PrintSocket(tBICPAR,operateNode_t));
		tBICPAR->sdtApp->m_Device.cgAPISocketServer->Spin_InUse_clr();
		PrintStrN(tBICPAR,DEV_LINE_START,RICH_LIN_clDefault);
	}
	else{
		PrintDoRet(tBICPAR,"Fail execute due to no connected");
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_SI::BIC_SI_LS_PrintSocket(BICPAR *tBICPAR,APISocket *tSocket){
	std::string		strPrintData;
	
	if (tSocket != nullptr){
		strPrintData = Str_IntToString(GetdRNodeID(tSocket));
		if (strPrintData.length() < 3)
			strPrintData.insert(0,3 - strPrintData.length(),' ');
		strPrintData += ".";
		
		if (tSocket->CheckblSelected() == 0){
			strPrintData += "   ";
		}
		else{
			strPrintData += " * ";
		}
		PrintStr(tBICPAR,strPrintData,RICH_LIN_clDefault);
		PrintStr(tBICPAR,tSocket->GetBufName(),RICH_LIN_clBrown);
		PrintStr(tBICPAR,"@",RICH_LIN_clDefault);
		PrintStr(tBICPAR,Str_IntToString(tSocket->GetBufPar()) + "\n",RICH_LIN_clBrown);
		PrintStr(tBICPAR,"     Received : " + Str_UInt64ToString(tSocket->RxBytes()) + " bytes\n",RICH_LIN_clCyan);
		PrintStr(tBICPAR,"     Echo     : " + Str_UInt64ToString(tSocket->FwBytes()) + " bytes\n",RICH_LIN_clCyan);
		PrintStr(tBICPAR,"     Sent     : " + Str_UInt64ToString(tSocket->TxBytes()) + " bytes\n",RICH_LIN_clCyan);
	}
	return 1;
}
//------------------------------------------------------------------------------------------//
int32 BIC_SS::Help(BICPAR *tBICPAR,int32 blDetail)const{
	PrintHelpItem(tBICPAR,cgCommand,"Socket select.");
	if (blDetail == 0)
		return(cgReturnCode);
	PrintHelpItem(tBICPAR,"     <num>","Socket number.");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_SS::Command(BICPAR *tBICPAR, const std::string &par,std::string *ret)const{
	APISocket		*socketSelected;
	APISocketServer	*socketPool;
	int32 			num;
	
	*ret = "";
	if (par.length() == 0)
		return(Help(tBICPAR));
	
	if ((tBICPAR->sdtApp->m_Device.cgDevType == DEVICE::DEVID_TCPServer) || (tBICPAR->sdtApp->m_Device.cgDevType == DEVICE::DEVID_UDPServer)){
		num = atoi(par.c_str());
		socketPool = tBICPAR->sdtApp->m_Device.cgAPISocketServer;
		socketPool->Spin_InUse_set();
		do{
			socketSelected = (APISocket*)GetSelectedInLChildRChain(socketPool);
			if (socketSelected != nullptr){
				socketSelected->ClrblSelected();
				socketSelected->UseExternalFwSBL(nullptr);
				socketSelected->UnlinkCoupleNode();
				tBICPAR->sdtApp->m_Device.cgAPISocketServer->SetblUpdate();
				if (GetdRNodeID(socketSelected) == (uint32)num){
					PrintStrN(tBICPAR,DEV_LINE_START,RICH_LIN_clDefault);
					BIC_SI::BIC_SI_LS_PrintSocket(tBICPAR,socketSelected);
					PrintStrN(tBICPAR,DEV_LINE_START,RICH_LIN_clDefault);
					break;
				}
			}
			socketSelected = (APISocket*)FindInLChildRChainByDRNodeID_nolock(socketPool,num);
			if (socketSelected != nullptr){
				socketSelected->SetblSelected();
				socketSelected->UseExternalFwSBL(&tBICPAR->sdtApp->m_Device.rxBufferList);
				if (tBICPAR->sdtApp->m_Device2.CheckblConnect() != 0)
					socketSelected->LinkCoupleNode(tBICPAR->sdtApp->m_Device2.cgCurrentDB);
				
				PrintStrN(tBICPAR,DEV_LINE_START,RICH_LIN_clDefault);
				BIC_SI::BIC_SI_LS_PrintSocket(tBICPAR,socketSelected);
				PrintStrN(tBICPAR,DEV_LINE_START,RICH_LIN_clDefault);
				tBICPAR->sdtApp->m_Device.cgAPISocketServer->SetblUpdate();
			}
		}while(0);
		socketPool->Spin_InUse_clr();
	}
	else{
		PrintDoRet(tBICPAR,"Fail execute due to no connected");
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_SSD::Help(BICPAR *tBICPAR,int32 blDetail)const{
	PrintHelpItem(tBICPAR,cgCommand,"Socket shutdown.");
	if (blDetail == 0)
		return(cgReturnCode);
	PrintHelpItem(tBICPAR,"     <num>","Socket number.");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_SSD::Command(BICPAR *tBICPAR, const std::string &par,std::string *ret)const{
	APISocket			*tSocket;
	APISocketServer		*socketPool;
	int32 			num;
	*ret = "";
	if (par.length() == 0)
		return(Help(tBICPAR));
	
	if ((tBICPAR->sdtApp->m_Device.cgDevType == DEVICE::DEVID_TCPServer) || (tBICPAR->sdtApp->m_Device.cgDevType == DEVICE::DEVID_UDPServer)){
		socketPool = tBICPAR->sdtApp->m_Device.cgAPISocketServer;
		num = atoi(par.c_str());
		socketPool->Spin_InUse_set();
		tSocket = (APISocket*)FindInLChildRChainByDRNodeID_nolock(socketPool, num);
		if (tSocket != nullptr)
			tSocket->CloseD(0);
		socketPool->Spin_InUse_clr();
	}
	else{
		PrintDoRet(tBICPAR,"Fail execute due to no connected");
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//