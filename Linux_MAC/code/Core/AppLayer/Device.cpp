/*
 * Copyright (c) 2012-2014
 * All rights reserved.
 *
 * File Name: Device.cpp
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2012.12.12
 * @2013.7.05 :	use PUB_RECORD to replace Memo,RichMemo
 * @2013.6.30 :	change to Liunx style
 use int32 to replace bool
 use int32 to replace uint32
 */
//------------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "Device.h"
#include "Comm_Convert.h"
#include "SBIC.h"
#include "SMC.h"
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
void DEVICE::Init(void){
	cgBufMaxSize = 0;
	ClearAllFlag();
	cgDevType = DEVID_NONE;
	cgCurrentDB = nullptr;
	
	cgAPIECom = nullptr;
	cgAPISocket = nullptr;
	cgAPISocketServer = nullptr;
}
//------------------------------------------------------------------------------------------//
void DEVICE::Init(const ODEV_LIST_POOL *outList,uint32 tSize){
	cgODevList.cgOutput = (ODEV_LIST_POOL*)outList;
	cgBufMaxSize = tSize;
}
//------------------------------------------------------------------------------------------//
void DEVICE::LinkDB_nolock(void){
	APISocket	*tempDB;
	DEVICE		*coupleDev;
	
	coupleDev = (DEVICE*)GetcgCoupleNode(this);
	switch(GetcgCNType(this)){
		case CN_1:{
			if (cgCurrentDB != nullptr){
				coupleDev->Spin_InUse_set();
				if (coupleDev->CheckblConnect() != 0)
					cgCurrentDB->LinkCoupleNode(coupleDev->cgCurrentDB);
				coupleDev->Spin_InUse_clr();
			}
			break;
		}
		case CN_0:
			coupleDev->Spin_InUse_set();
			if (coupleDev->cgCurrentDB != nullptr){
				coupleDev->cgCurrentDB->LinkCoupleNode(cgCurrentDB);
			}
			else if (coupleDev->cgAPISocketServer != nullptr){
				tempDB = (APISocket*)GetSelectedInLChildRChain(coupleDev->cgAPISocketServer);
				if (tempDB != nullptr)
					tempDB->LinkCoupleNode(cgCurrentDB);
			}
			coupleDev->Spin_InUse_clr();
		case CN_None:
		default:;
	}
}
//------------------------------------------------------------------------------------------//
int32 DEVICE::Open(OPEN_TYPE tOPEN_TYPE,const std::string &devName,int32 baudrate,int32 blEnEcho){
	int32	ret;
	SMC_EncryptI(0)
	SMC_EncryptI(1)
	SMC_EncryptI(2)
	SMC_EncryptI(3)
	SMC_EncryptI(4)
	
	ret = 0;
	Spin_InUse_set();
	
	switch(tOPEN_TYPE){
#ifdef SWVERSION_COM
		case OP_COM:
			SMC_EncryptS(0)
			ret = APIEComOpen(devName,baudrate,blEnEcho);
			SMC_EncryptE(0)
			break;
#endif
#ifdef SWVERSION_TCPClient
		case OP_TCPClient:
			SMC_EncryptS(1)
			ret = APISocketOpen(devName,baudrate,COMMU_DBUF::CSType_TCP,blEnEcho);
			SMC_EncryptE(1)
			break;
#endif
#ifdef SWVERSION_UDPCLient
		case OP_UDPClient:
			SMC_EncryptS(2)
			ret = APISocketOpen(devName,baudrate,COMMU_DBUF::CSType_UDP,blEnEcho);
			SMC_EncryptE(2)
			break;
#endif
#ifdef SWVERSION_TCPServer
		case OP_TCPServer:
			SMC_EncryptS(3)
			ret = SocketServerOpen(baudrate,COMMU_DBUF::CSType_TCP,blEnEcho);
			SMC_EncryptE(3)
			break;
#endif
#ifdef SWVERSION_UDPServer
		case OP_UDPServer:
			SMC_EncryptS(4)
			ret = SocketServerOpen(baudrate,COMMU_DBUF::CSType_UDP,blEnEcho);
			SMC_EncryptE(4)
			break;
#endif
		default:
			break;
	}
	Spin_InUse_clr();
	return ret;
}
//------------------------------------------------------------------------------------------//
int32 DEVICE::APIEComOpen(const std::string &devName,int32 baudrate,int32 blEnEcho){
	int32		ret;
	
	ret = 0;
	do{
		if (cgAPIECom == nullptr){
			cgAPIECom = CreateNewCOM(&cgODevList,cgBufMaxSize);
			if (cgAPIECom == nullptr)
				break;
			cgAPIECom->UseExternalFwSBL(&rxBufferList);
		}
		
		cEDevFlag.com = devName;
		cEDevFlag.baudrate = baudrate;
		
		cgDevType = DEVID_APICOM;
		cgCurrentDB = cgAPIECom;
		
		LinkDB_nolock();
		
		if (cEDevFlag.comType == 0){
			ret = cgAPIECom->OpenD(devName,baudrate,COMMU_DBUF::CSType_None,blEnEcho);
		}
		else{
			ret = cgAPIECom->OpenV(devName,baudrate,COMMU_DBUF::CSType_None,blEnEcho);
		}
		if (ret!= 0){
			SetblConnect();
			if (GetcgCNType(this) == CN_0){
				cgAPIECom->PrintOpenSuccessReport("Auxiliary COM");
			}
			else{
				cgAPIECom->PrintOpenSuccessReport("");
			}
			break;
		}
		cgAPIECom->PrintOpenFailReport(devName);
		ClrblConnect();
	}while(0);
	return(ret);
}
//------------------------------------------------------------------------------------------//
void DEVICE::APIEComClose(int32 blPrint){
	if (cgAPIECom != nullptr){
		cgAPIECom->CloseD(1);
		cgAPIECom->UnlinkCoupleNode();
		if (blPrint != 0){
			if (GetcgCNType(this) == CN_0){
				cgAPIECom->PrintUserDisconnectReport(" Close auxiliary COM.\r\n");
			}
			else{
				cgAPIECom->PrintUserDisconnectReport(" User stopped the connection.\r\n");
			}
		}
		ClrblConnect();
	}
}
//------------------------------------------------------------------------------------------//
int32 DEVICE::APISocketOpen(const std::string &devName,int32 baudrate,COMMU_DBUF::CSType tStype,int32 blEnEcho){
	std::string		strData;
	int32			ret;
	
	ret = 0;
	do{
		if (cgAPISocket == nullptr){
			cgAPISocket = CreateNewSocket(&cgODevList,cgBufMaxSize);
			if (cgAPISocket == nullptr)
				break;
			cgAPISocket->UseExternalFwSBL(&rxBufferList);
		}
		if (tStype == COMMU_DBUF::CSType_TCP){
			cgDevType = DEVID_TCPClient;
			cEDevFlag.tcpIP = devName;
			cEDevFlag.tcpPort = baudrate;
		}
		else{
			cgDevType = DEVID_UDPClient;
			cEDevFlag.udpIP = devName;
			cEDevFlag.udpPort = baudrate;
		}
		
		cgCurrentDB = cgAPISocket;
		LinkDB_nolock();
		
		if (cgAPISocket->OpenD(devName,baudrate,tStype,blEnEcho) != 0){
			SetblConnect();
			if (GetcgCNType(this) == CN_0){
				if (tStype == COMMU_DBUF::CSType_TCP){
					cgAPISocket->PrintOpenSuccessReport("Auxiliary TCP");
				}
				else{
					cgAPISocket->PrintOpenSuccessReport("Auxiliary UDP");
				}
			}
			else{
				if (tStype == COMMU_DBUF::CSType_TCP){
					cgAPISocket->PrintOpenSuccessReport("TCP");
				}
				else{
					cgAPISocket->PrintOpenSuccessReport("UDP");
				}
			}
			ret = 1;
			break;
		}
		strData = devName + ":" + Str_IntToString(baudrate);
		cgAPISocket->PrintOpenFailReport(strData);
		ClrblConnect();
	}while(0);
	return(ret);
}
//------------------------------------------------------------------------------------------//
void DEVICE::APISocketClose(int32 blPrint){
	if (cgAPISocket != nullptr){
		cgAPISocket->CloseD(1);
		cgAPISocket->UnlinkCoupleNode();
		if (blPrint != 0)
			cgAPISocket->PrintUserDisconnectReport(" User stopped the connection.\r\n");
		ClrblConnect();
	}
}
//------------------------------------------------------------------------------------------//
int32 DEVICE::SocketServerOpen(int32 baudrate,COMMU_DBUF::CSType tStype,int32 blEnEcho){
	std::string strPrintData;
	int32		ret;
	
	ret = 0;
	do{
		if (cgAPISocketServer == nullptr){
			cgAPISocketServer = CreateNewSocketServer(&cgODevList,cgBufMaxSize);
			if (cgAPISocketServer == nullptr)
				break;
		}
		
		if (cgCurrentDB != nullptr)
			cgCurrentDB->UnlinkCoupleNode();
		cgCurrentDB = nullptr;
		
		if (tStype == APISocket::CSType_TCP){
			cgDevType = DEVID_TCPServer;
			cEDevFlag.tcpsPort = baudrate;
		}
		else{
			cgDevType = DEVID_UDPServer;
			cEDevFlag.udpsPort = baudrate;
		}
		
		if (cgAPISocketServer->OpenD(baudrate,tStype,blEnEcho) != 0){
			SetblConnect();
			cgAPISocketServer->PrintOpenSuccessReport();
			ret = 1;
			break;
		}
		cgAPISocketServer->PrintOpenFailReport();
		ClrblConnect();
	}while(0);
	return(ret);
}
//------------------------------------------------------------------------------------------//
void DEVICE::SocketServerClose(int32 blPrint){

	if (cgAPISocketServer != nullptr){
		cgAPISocketServer->CloseD();
		if (blPrint != 0)
			cgAPISocketServer->PrintUserDisconnectReport(" User stopped the server.\r\n");
		ClrblConnect();
	}
}
//------------------------------------------------------------------------------------------//
APICOM *DEVICE::CreateNewCOM(const ODEV_LIST *tODEV_LIST,uint32 tSize){
	APICOM *tPDB;
	tPDB = new APICOM(tODEV_LIST,tSize);
	AddNode(tPDB);
	return(tPDB);
}
//------------------------------------------------------------------------------------------//
APISocket *DEVICE::CreateNewSocket(const ODEV_LIST *tODEV_LIST,uint32 tSize){
	APISocket *tPDB;
	tPDB = new APISocket(tODEV_LIST,tSize);
	AddNode(tPDB);
	return(tPDB);
}
//------------------------------------------------------------------------------------------//
APISocketServer *DEVICE::CreateNewSocketServer(const ODEV_LIST *tODEV_LIST,uint32 tSize){
	APISocketServer *tPDB;
	tPDB = new APISocketServer(tODEV_LIST,tSize);
	AddNode(tPDB);
	return(tPDB);
}
//------------------------------------------------------------------------------------------//
void DEVICE::Close(int32 blPrint){
	Spin_InUse_set();
	switch (cgDevType){
		case DEVID_NONE:
			APIEComClose(0);
			APISocketClose(0);
			SocketServerClose(0);
			break;
		case DEVID_APICOM:
			APIEComClose(blPrint);
			APISocketClose(0);
			SocketServerClose(0);
			break;
		case DEVID_TCPClient:
		case DEVID_UDPClient:
			APIEComClose(0);
			APISocketClose(blPrint);
			SocketServerClose(0);
			break;
		case DEVID_TCPServer:
		case DEVID_UDPServer:
			APIEComClose(0);
			APISocketClose(0);
			SocketServerClose(blPrint);
	}
	Spin_InUse_clr();
}/*
//------------------------------------------------------------------------------------------//
uint32 DEVICE::WriteInHEX(const std::string &strContent){
	uint32 ret;
	if (CheckblConnect() == 0)
		return 0;
	ret = 0;
	Spin_InUse_set();
	switch (cgDevType){
		case DEVID_APICOM:
		case DEVID_TCPClient:
		case DEVID_UDPClient:
			if (cgCurrentDB != nullptr)
				ret = cgCurrentDB->WriteInHEX(strContent);
			break;
		case DEVID_TCPServer:
		case DEVID_UDPServer:
			if (cgAPISocketServer != nullptr)
				ret = cgAPISocketServer->SWriteInHEX(strContent);
			break;
		case DEVID_NONE:
			break;
	}
	Spin_InUse_clr();
	return(ret);
}
//------------------------------------------------------------------------------------------//
uint32 DEVICE::ReadInHEX(std::string *strRec,int32 blSpace){
	uint32 ret;
	if (CheckblConnect() == 0)
		return 0;
	ret = 0;
	Spin_InUse_set();
	switch (cgDevType){
		case DEVID_APICOM:
		case DEVID_TCPClient:
		case DEVID_UDPClient:
			if (cgCurrentDB != nullptr)
				ret = cgCurrentDB->ReadInHEX(strRec,blSpace);
			break;
		case DEVID_TCPServer:
		case DEVID_UDPServer:
			if (cgAPISocketServer != nullptr)
				ret = cgAPISocketServer->SReadInHEX(strRec,blSpace);
			break;
		case DEVID_NONE:
			break;
	}
	Spin_InUse_clr();
	return(ret);
}
//------------------------------------------------------------------------------------------//
uint32 DEVICE::WriteInASCII(const std::string &strContent,int32 blEscape){
	uint32 ret;
	if (CheckblConnect() == 0)
		return 0;
	ret = 0;
	Spin_InUse_set();
	switch (cgDevType){
		case DEVID_APICOM:
		case DEVID_TCPClient:
		case DEVID_UDPClient:
			if (cgCurrentDB != nullptr)
				ret = cgCurrentDB->WriteInASCII(strContent,blEscape);
			break;
		case DEVID_TCPServer:
		case DEVID_UDPServer:
			if (cgAPISocketServer != nullptr)
				ret = cgAPISocketServer->SWriteInASCII(strContent,blEscape);
			break;
		case DEVID_NONE:
			break;
	}
	Spin_InUse_clr();
	return(ret);
}
//------------------------------------------------------------------------------------------//
uint32 DEVICE::ReadInASCII(std::string *strRec,int32 blEscape){
	uint32 ret;
	if (CheckblConnect() == 0)
		return 0;
	ret = 0;
	Spin_InUse_set();
	switch (cgDevType){
		case DEVID_APICOM:
		case DEVID_TCPClient:
		case DEVID_UDPClient:
			if (cgCurrentDB != nullptr)
				ret = cgCurrentDB->ReadInASCII(strRec,blEscape);
			break;
		case DEVID_TCPServer:
		case DEVID_UDPServer:
			if (cgAPISocketServer != nullptr)
				ret = cgAPISocketServer->SReadInASCII(strRec,blEscape);
			break;
		case DEVID_NONE:
			break;
	}
	Spin_InUse_clr();
	return(ret);
}
//------------------------------------------------------------------------------------------//
uint32 DEVICE::Read(uint8 *buffer,uint32 num){
	uint32 ret;
	if (CheckblConnect() == 0)
		return 0;
	ret = 0;
	Spin_InUse_set();
	switch (cgDevType){
		case DEVID_APICOM:
		case DEVID_TCPClient:
		case DEVID_UDPClient:
			if (cgCurrentDB != nullptr)
				ret = cgCurrentDB->Read(buffer,num);
			break;
		case DEVID_TCPServer:
		case DEVID_UDPServer:
			if (cgAPISocketServer != nullptr)
				ret = cgAPISocketServer->SRead(buffer,num);
			break;
		case DEVID_NONE:
			break;
	}
	Spin_InUse_clr();
	return(ret);
}
//------------------------------------------------------------------------------------------//
uint32 DEVICE::Write(uint8 *newData,uint32 num){
	uint32 ret;
	if (CheckblConnect() == 0)
		return 0;
	ret = 0;
	Spin_InUse_set();
	switch (cgDevType){
		case DEVID_APICOM:
		case DEVID_TCPClient:
		case DEVID_UDPClient:
			if (cgCurrentDB != nullptr)
				ret = cgCurrentDB->Write(newData,num);
			break;
		case DEVID_TCPServer:
		case DEVID_UDPServer:
			if (cgAPISocketServer != nullptr)
				ret = cgAPISocketServer->SWrite(newData,num);
			break;
		case DEVID_NONE:
			break;
	}
	Spin_InUse_clr();
	return(ret);
}
//------------------------------------------------------------------------------------------//
void DEVICE::EnableEcho(void){
	Spin_InUse_set();
	switch (cgDevType){
		case DEVID_APICOM:
		case DEVID_TCPClient:
		case DEVID_UDPClient:
			if (cgCurrentDB != nullptr)
				cgCurrentDB->EnableEcho();
			break;
		case DEVID_TCPServer:
		case DEVID_UDPServer:
			if (cgAPISocketServer != nullptr)
				cgAPISocketServer->SEnableEcho();
			break;
		case DEVID_NONE:
			break;
	}
	Spin_InUse_clr();
}
//------------------------------------------------------------------------------------------//
void DEVICE::DisableEcho(void){
	Spin_InUse_set();
	switch (cgDevType){
		case DEVID_APICOM:
		case DEVID_TCPClient:
		case DEVID_UDPClient:
			if (cgCurrentDB != nullptr)
				cgCurrentDB->DisableEcho();
			break;
		case DEVID_TCPServer:
		case DEVID_UDPServer:
			if (cgAPISocketServer != nullptr)
				cgAPISocketServer->SDisableEcho();
			break;
		case DEVID_NONE:
			break;
	}
	Spin_InUse_clr();
}*/
//------------------------------------------------------------------------------------------//
void DEVICE::Clean(G_LOCK_VAILD blLock){
	Spin_InUse_set(blLock);
	if (cgODevList.cgOutput != nullptr)
		cgODevList.cgOutput->Clean();
	switch (cgDevType){
		case DEVID_APICOM:
		case DEVID_TCPClient:
		case DEVID_UDPClient:
			if (cgCurrentDB != nullptr)
				cgCurrentDB->Clean();
			break;
		case DEVID_TCPServer:
		case DEVID_UDPServer:
			if (cgAPISocketServer != nullptr)
				cgAPISocketServer->SClean();
			break;
		case DEVID_NONE:
			break;
	}
	Spin_InUse_clr(blLock);
}
/*
//------------------------------------------------------------------------------------------//
uint32 DEVICE::BufferRxMaxSize(void){
	uint32 internalResult;
	internalResult = 0;
	Spin_InUse_set();
	switch (cgDevType){
		case DEVID_APICOM:
		case DEVID_TCPClient:
		case DEVID_UDPClient:
			if (cgCurrentDB != nullptr)
				internalResult = cgCurrentDB->BufferRxMaxSize();
			break;
		case DEVID_TCPServer:
		case DEVID_UDPServer:
			if (cgAPISocketServer != nullptr)
				internalResult = cgAPISocketServer->SBufferRxMaxSize();
			break;
		case DEVID_NONE:
			break;
	}
	Spin_InUse_clr();
	return(internalResult);
}
//------------------------------------------------------------------------------------------//
uint32 DEVICE::BufferTxMaxSize(void){
	uint32 internalResult;
	internalResult = 0;
	Spin_InUse_set();
	switch (cgDevType){
		case DEVID_APICOM:
		case DEVID_TCPClient:
		case DEVID_UDPClient:
			if (cgCurrentDB != nullptr)
				internalResult = cgCurrentDB->BufferTxMaxSize();
			break;
		case DEVID_TCPServer:
		case DEVID_UDPServer:
			if (cgAPISocketServer != nullptr)
				internalResult = cgAPISocketServer->SBufferTxMaxSize();
			break;
		case DEVID_NONE:
			break;
	}
	Spin_InUse_clr();
	return(internalResult);
}
//------------------------------------------------------------------------------------------//
uint32 DEVICE::UnreadBytes(void){
	uint32 internalResult;
	internalResult = 0;
	Spin_InUse_set();
	switch (cgDevType){
		case DEVID_APICOM:
		case DEVID_TCPClient:
		case DEVID_UDPClient:
			if (cgCurrentDB != nullptr)
				internalResult = cgCurrentDB->UnreadBytes();
			break;
		case DEVID_TCPServer:
		case DEVID_UDPServer:
			if (cgAPISocketServer != nullptr)
				internalResult = cgAPISocketServer->SUnreadBytes();
			break;
		case DEVID_NONE:
			break;
	}
	Spin_InUse_clr();
	return(internalResult);
}
//------------------------------------------------------------------------------------------//
uint32 DEVICE::UnsentBytes(void){
	uint32 internalResult;
	internalResult = 0;
	Spin_InUse_set();
	switch (cgDevType){
		case DEVID_APICOM:
		case DEVID_TCPClient:
		case DEVID_UDPClient:
			if (cgCurrentDB != nullptr)
				internalResult = cgCurrentDB->UnsentBytes();
			break;
		case DEVID_TCPServer:
		case DEVID_UDPServer:
			if (cgAPISocketServer != nullptr)
				internalResult = cgAPISocketServer->SUnsentBytes();
			break;
		case DEVID_NONE:
			break;
	}
	Spin_InUse_clr();
	return(internalResult);
}
//------------------------------------------------------------------------------------------//
uint64 DEVICE::FwBytes(void){
	uint64 internalResult;
	internalResult = 0;
	Spin_InUse_set();
	switch (cgDevType){
		case DEVID_APICOM:
		case DEVID_TCPClient:
		case DEVID_UDPClient:
			if (cgCurrentDB != nullptr)
				internalResult = cgCurrentDB->FwBytes();
			break;
		case DEVID_TCPServer:
		case DEVID_UDPServer:
			if (cgAPISocketServer != nullptr)
				internalResult = cgAPISocketServer->SFwBytes();
			break;
		case DEVID_NONE:
			break;
	}
	Spin_InUse_clr();
	return(internalResult);
}
//------------------------------------------------------------------------------------------//
uint64 DEVICE::RxBytes(void){
	uint64 internalResult;
	internalResult = 0;
	Spin_InUse_set();
	switch (cgDevType){
		case DEVID_APICOM:
		case DEVID_TCPClient:
		case DEVID_UDPClient:
			if (cgCurrentDB != nullptr)
				internalResult = cgCurrentDB->RxBytes();
			break;
		case DEVID_TCPServer:
		case DEVID_UDPServer:
			if (cgAPISocketServer != nullptr)
				internalResult = cgAPISocketServer->SRxBytes();
			break;
		case DEVID_NONE:
			break;
	}
	Spin_InUse_clr();
	return(internalResult);
}
//------------------------------------------------------------------------------------------//
uint64 DEVICE::TxBytes(void){
	uint64 internalResult;
	internalResult = 0;
	Spin_InUse_set();
	switch (cgDevType){
		case DEVID_APICOM:
		case DEVID_TCPClient:
		case DEVID_UDPClient:
			if (cgCurrentDB != nullptr)
				internalResult = cgCurrentDB->TxBytes();
			break;
		case DEVID_TCPServer:
		case DEVID_UDPServer:
			if (cgAPISocketServer != nullptr)
				internalResult = cgAPISocketServer->STxBytes();
			break;
		case DEVID_NONE:
			break;
	}
	Spin_InUse_clr();
	return(internalResult);
}*/
//------------------------------------------------------------------------------------------//
void DEVICE::SendCommand(const std::string &inputCommand,int32 blHEX,int32 bl0x0D,G_ESCAPE_VAILD blEscape){
	std::string		strCommand,strPrintData;
	int32			ByteNum;
	
	if (CheckblConnect() != 0){
		cgSBIC_PRINT.CreateHexCommand(nullptr,inputCommand,blHEX,bl0x0D,blEscape,&strCommand,&strPrintData);
		ByteNum = WriteInHEX(strCommand);
		PrintSendCommand(strPrintData, ByteNum, COLSTRING::COL_EP_YES);
	}
}
//------------------------------------------------------------------------------------------//
void DEVICE::PrintSendCommand(const std::string &strPrintData,uint32 num,COLSTRING::COLEnforcePrint blEP){
	ODEV_LIST_POOL	*output;
	APISocket		*dbuf;
	std::string		strM;

	output = nullptr;
	dbuf = nullptr;
	strM = "";
	Spin_InUse_set();
	switch (cgDevType){
		case DEVID_APICOM:
		case DEVID_TCPClient:
		case DEVID_UDPClient:
			if (cgCurrentDB != nullptr){
				cgCurrentDB->Spin_InUse_set();
				output = cgCurrentDB->GetcgODevPool();
				strM = "";
				if (output != nullptr){
					output->Spin_InUse_set();
					output->WriteDividingLine(RICH_CF_clMaroon, blEP);
					output->WriteTxTitle(num, TxBytes(G_LOCK_OFF) + num, RICH_CF_clMaroon, blEP);
					output->WriteStr(strPrintData, RICH_CF_clBlack, blEP);
					output->Spin_InUse_clr();
				}
				cgCurrentDB->Spin_InUse_clr();
			}
			break;
		case DEVID_TCPServer:
		case DEVID_UDPServer:
			if (cgAPISocketServer != nullptr){
				cgAPISocketServer->Spin_InUse_set();
				dbuf = (APISocket*)GetSelectedInLChildRChain(cgAPISocketServer);
				if (dbuf != nullptr){
					dbuf->Spin_InUse_set();
					output = dbuf->GetcgODevPool();
					strM = dbuf->GetBufName() + ":" + Str_IntToString(dbuf->GetBufPar());
					if (output != nullptr){
						output->Spin_InUse_set();
						output->WriteDividingLine(RICH_CF_clMaroon, blEP);
						output->WriteTxTitle(num, TxBytes(G_LOCK_OFF) + num, RICH_CF_clMaroon, blEP);
						output->WriteStr(strPrintData, RICH_CF_clBlack, blEP);
						output->Spin_InUse_clr();
					}
					dbuf->Spin_InUse_clr();
				}
				cgAPISocketServer->Spin_InUse_clr();
			}
			break;
		case DEVID_NONE:
			break;
	}
	if (cgODevList.cgOutput != nullptr){
		cgODevList.cgOutput->Spin_InUse_set();
		cgODevList.cgOutput->WriteDividingLine(RICH_CF_clMaroon, blEP);
		if (strM.length() > 0){
			cgODevList.cgOutput->WriteTxTitle(strM, "" , num, TxBytes(G_LOCK_OFF) + num, RICH_CF_clMaroon, blEP);
		}
		else{
			cgODevList.cgOutput->WriteTxTitle(num, TxBytes(G_LOCK_OFF) + num, RICH_CF_clMaroon, blEP);
		}
		cgODevList.cgOutput->WriteStr(strPrintData, RICH_CF_clBlack, blEP);
		cgODevList.cgOutput->Spin_InUse_clr();
	}

	Spin_InUse_clr();
}
//------------------------------------------------------------------------------------------//
void DEVICE::PrintSendStr(const std::string &strPrintData, const std::string &rColor ,COLSTRING::COLEnforcePrint blEP){
	ODEV_LIST_POOL	*output;
	APISocket		*dbuf;

	if (cgODevList.cgOutput != nullptr)
		cgODevList.cgOutput->WriteStr(strPrintData, rColor, blEP);
	output = nullptr;
	dbuf = nullptr;
	Spin_InUse_set();
	switch (cgDevType){
		case DEVID_APICOM:
		case DEVID_TCPClient:
		case DEVID_UDPClient:
			if (cgCurrentDB != nullptr){
				cgCurrentDB->Spin_InUse_set();
				output = cgCurrentDB->GetcgODevPool();
				if (output != nullptr)
					output->WriteStr(strPrintData, rColor, blEP);
				cgCurrentDB->Spin_InUse_clr();
			}
			break;
		case DEVID_TCPServer:
		case DEVID_UDPServer:
			if (cgAPISocketServer != nullptr){
				cgAPISocketServer->Spin_InUse_set();
				dbuf = (APISocket*)GetSelectedInLChildRChain(cgAPISocketServer);
				if (dbuf != nullptr){
					dbuf->Spin_InUse_set();
					output = dbuf->GetcgODevPool();
					if (output != nullptr)
						output->WriteStr(strPrintData, rColor, blEP);
					dbuf->Spin_InUse_clr();
				}
				cgAPISocketServer->Spin_InUse_clr();
			}
			break;
		case DEVID_NONE:
			break;
	}
	Spin_InUse_clr();
}
//------------------------------------------------------------------------------------------//
void DEVICE::SendCommandOnly(const std::string &inputCommand,int32 blHEX,int32 bl0x0D,G_ESCAPE_VAILD blEscape){
	std::string		strCommand,strPrintData;
	
	if (CheckblConnect() != 0){
		cgSBIC_PRINT.CreateHexCommand(nullptr,inputCommand,blHEX,bl0x0D,blEscape,&strCommand,&strPrintData);
		WriteInHEX(strCommand);
	}
}
//------------------------------------------------------------------------------------------//
int32 DEVICE::CheckResult(PUB_SBUF *cBuffer,const std::string &cCheckResult,int32 cTimeout){
	SBIC_Expression	subC_Expression;
	SBICPAR			tSBICPAR;
	SYS_TIME_S		tTimeS;
	tSBICPAR.blExit = 0;
	tSBICPAR.blRSC = 0;
	tSBICPAR.cgBuffer = cBuffer;
	tSBICPAR.cgCommand = nullptr;
	tSBICPAR.cgDevice = this;
	tSBICPAR.cgRecvbuf = "";
	tSBICPAR.cgSearchRet = "";
	tSBICPAR.cgFunRet = "";
	tSBICPAR.cgODevNode = nullptr;
	tSBICPAR.cgTimeS = &tTimeS;
#ifdef CommonDefH_Unix
	tSBICPAR.cgSBIC_RE_LIST = nullptr;
#endif	
	SYS_Delay_SetTS(&tTimeS,cTimeout);
	
	do
	{
		cBuffer->GetInHEX(&tSBICPAR.cgRecvbuf,G_SPACE_OFF);
		if (subC_Expression.Expression(&tSBICPAR,cCheckResult) != 0){
			cBuffer->Clean();
			return 1;
		}
	}while(SYS_Delay_CheckTS(&tTimeS) == 0);
	cBuffer->Clean();
	return 0;
}
//------------------------------------------------------------------------------------------//
void DEVICE::UpdataUIRecord(void){
	if (cgODevList.cgOutput != nullptr)
		cgODevList.cgOutput->Print();
	if (cgAPISocketServer != nullptr)
		cgAPISocketServer->LUpdataRecordUI();
}
//------------------------------------------------------------------------------------------/
#ifdef CommonDefH_VC
//------------------------------------------------------------------------------------------//
void DEVICE::UpdataUIRecordSDOUT(void){
	if (cgODevList.cgOutput != nullptr)
		cgODevList.cgOutput->PrintSDOUT();
	if (cgAPISocketServer != nullptr)
		cgAPISocketServer->LUpdataRecordUISDOUT();
}
//------------------------------------------------------------------------------------------//
#endif
//------------------------------------------------------------------------------------------//
std::string DEVICE::MakeFrmTitle(void){
	std::string	strRet;
	
	switch(cgDevType){
		case DEVICE::DEVID_TCPClient:
			strRet = "TCP:";
			strRet += cEDevFlag.tcpIP;
			strRet += "@";
			strRet += Str_IntToString(cEDevFlag.tcpPort);
			break;
		case DEVICE::DEVID_UDPClient:
			strRet = "UDP:";
			strRet += cEDevFlag.udpIP;
			strRet += "@";
			strRet += Str_IntToString(cEDevFlag.udpPort);
			break;
		case DEVICE::DEVID_TCPServer:
			strRet = "TCP SERVER";
			strRet += "@";
			strRet += Str_IntToString(cEDevFlag.tcpsPort);
			break;
		case DEVICE::DEVID_UDPServer:
			strRet = "UDP SERVER";
			strRet += "@";
			strRet += Str_IntToString(cEDevFlag.udpsPort);
			break;
		case DEVICE::DEVID_APICOM:
			strRet = cEDevFlag.com;
			strRet += "@";
			strRet += Str_IntToString(cEDevFlag.baudrate);
			break;
		default:
			break;
	}
	Str_UpperCaseSelf(strRet);
	return(strRet);
}
//------------------------------------------------------------------------------------------//

