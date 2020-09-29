//
//  BIC_Socket.cpp
//  SDT
//
//  Created by Leif Wen on 13/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "stdafx.h"
//------------------------------------------------------------------------------------------//
#include "BIC_Socket.h"
#ifdef BIC_Socket_h
#include "Commu_Socket.h"
#include "Bus.h"
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIC_TCP::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env,cgCommand,"Set as TCP mode");
	if (B_ChkFLAG32(flag, CMD_blPrintSimple))
		return(cgCommandID);
	PrintHelpSubItem(env,"[IP][PORT]","Destination IP and port");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_TCP::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	BIC_CONNECT::SetConnectPar(env,msg,OPEN_TCP);
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIC_UDP::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env,cgCommand,"Set as UDP mode");
	if (B_ChkFLAG32(flag, CMD_blPrintSimple))
		return(cgCommandID);
	PrintHelpSubItem(env,"[IP][PORT]","Destination IP and port");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_UDP::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	BIC_CONNECT::SetConnectPar(env,msg,OPEN_UDP);
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIC_TCPS::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env,cgCommand,"Set as TCP server mode");
	if (B_ChkFLAG32(flag, CMD_blPrintSimple))
		return(cgCommandID);
	PrintHelpSubItem(env,"[PORT]","Local listen port");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_TCPS::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	BIC_CONNECT::SetConnectPar(env,"TCPS " + msg,OPEN_TCPS);
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIC_UDPS::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env,cgCommand,"Set as UDP server mode");
	if (B_ChkFLAG32(flag, CMD_blPrintSimple))
		return(cgCommandID);
	PrintHelpSubItem(env,"[PORT]","Local listen port");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_UDPS::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	BIC_CONNECT::SetConnectPar(env,"UDPS "+ msg,OPEN_UDPS);
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIC_PORT::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env,cgCommand,"Set port");
	if (B_ChkFLAG32(flag, CMD_blPrintSimple))
		return(cgCommandID);
	PrintHelpSubItem(env,"<n>","Port");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_PORT::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	if (msg.length() == 0)
		return(Help(env,0));
	BIC_CONNECT::SetConnectPar2(env,msg);
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
static bool32 BIC_PrintSocket(CMD_ENV* env,ASOCKET* socket){
	STDSTR		strPrintData;
	if (BIC_SI::GetSTDOUT(env) != nullptr){
		if (socket != nullptr){
			strPrintData = Str_ToStr(TNF::GetDRNodeID(socket));
			Str_AddSpaceInFront(&strPrintData,3);
			strPrintData += ".";
			
			if (DRV_NODE::IsSelected(socket) == G_FALSE){
				strPrintData += "   ";
			}
			else{
				strPrintData += " * ";
			}
			*BIC_SI::GetSTDOUT(env)  << Begin() << NL() << COL_clDefault << strPrintData
			<< COL_clYellow << socket->Core()->GetOpenPar().name << COL_clDefault << "@" << COL_clYellow << Str_ToStr(socket->Core()->GetOpenPar().port) << NL()
			<< COL_clCyan
			<< "     Received : " << Str_ToStr(socket->RxBytes()) << " bytes\n"
			<< "     Echo     : " << Str_ToStr(socket->DPBytes()) << " bytes\n"
			<< "     Sent     : " << Str_ToStr(socket->TxBytes()) << " bytes\n"
			<< Endl();
		}
	}
	return G_TRUE;
};
//------------------------------------------------------------------------------------------//
CMDID BIC_SI::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env,cgCommand,"Socket information");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_SI::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	ExpandDevBusAttr* attr = BIC_ENV_DEV::GetEDA(env);
	
	if (msg.length() > 0)
		return(Help(env,0));
	
	if (attr->IsServerOpened()){
		PrintStrNL(env,COLOR(COL_clDefault,IUD(DEV_LINE_STAR)));
		TREE_DownChain_Traversal_LINE_nolock(ASOCKET,attr->AServer(),BIC_PrintSocket(env,_opNode));
		PrintStrNL(env,COLOR(COL_clDefault,IUD(DEV_LINE_STAR)));
	}
	else{
		PrintFail(env,"no connect");
	}
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIC_SS::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env,cgCommand,"Socket select");
	if (B_ChkFLAG32(flag, CMD_blPrintSimple))
		return(cgCommandID);
	PrintHelpSubItem(env,"<num>","Socket number");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_SS::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	ExpandDevBusAttr* attr = BIC_ENV_DEV::GetEDA(env);
	
	ASOCKET	*selSocket;
	int32	num;
	
	if (msg.length() == 0)
		return(Help(env,0));
	
	if (attr->IsServerOpened()){
		num = atoi(msg.c_str());
		
		selSocket = attr->AServer()->Attach(num);

		PrintStrNL(env,COLOR(COL_clDefault,IUD(DEV_LINE_STAR)));
		BIC_PrintSocket(env,selSocket);
		PrintStrNL(env,COLOR(COL_clDefault,IUD(DEV_LINE_STAR)));
	}
	else{
		PrintFail(env,"no connect");
	}
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIC_SD::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env,cgCommand,"Socket shutdown");
	if (B_ChkFLAG32(flag, CMD_blPrintSimple))
		return(cgCommandID);
	PrintHelpSubItem(env,"[num]","Socket number");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_SD::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	ExpandDevBusAttr* attr = BIC_ENV_DEV::GetEDA(env);
	
	uint32	sDID;
	STDSTR	strPar1,strPar2;
	
	if (msg.length() == 0)
		return(Help(env,0));
	
	if (attr->IsServerOpened()){
		SplitPar1(strPar1, strPar2, msg);
		if (strPar1 == "all"){
			attr->AServer()->CloseAllChild();
			SetblUpdate(attr->AServer());
		}
		else{
			while(strPar1.length() > 0){
				sDID = (uint32)strtol(strPar1.c_str(),nullptr,10);
				attr->AServer()->CloseChild(static_cast<ASOCKET*>(FindInDownChainByDRNodeID(attr->AServer(), sDID)));
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
BIC_ASOCKET::BIC_ASOCKET(void) : BIC_BASE_S() {
	SetSFlag(CMD_blTrySubCMD);
	cgCommandID = BIC_ID_ASOCKET;
	cgCommand = "socket";
	cgConsoleName = cgCommand;
	cgHelpName = "Socket setting";

	*this
#ifdef	SWVERSION_TCPClient
	< cgSub_tcp
#endif
#ifdef	SWVERSION_UDPCLient
	< cgSub_udp
#endif
#ifdef	SWVERSION_TCPServer
	< cgSub_tcps
#endif
#ifdef	SWVERSION_UDPServer
	< cgSub_udps
#endif
#ifdef SWVERSION_PORT
	< cgSub_port
#endif
#ifdef SWVERSION_Server
	< cgSub_si < cgSub_ss < cgSub_sd
#endif
	;
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif /* BIC_Socket_h */
