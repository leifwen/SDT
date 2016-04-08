/*
 * Copyright (c) 2012-2014 
 * All rights reserved.
 *
 * File Name: Device.h
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2012.12.12
 * @2013.7.05 :	use PUB_RECORD to replace Memo,RichMemo
 * @2013.6.30 :	change to Liunx style
 				use int32 to replace bool
 				use int32 to replace uint32
*/
#ifndef DeviceH
#define DeviceH
//------------------------------------------------------------------------------------------//
#include "Comm_Tree.h"
#include "ODEV_Include.h"
#include "Commu_Include.h"
//------------------------------------------------------------------------------------------//
class ExpandDevFlag{
	public:
		ExpandDevFlag(void){
			blCommandExplain = 0;
			blScriptAT = 1;
			blEnablePrintSBICinfo = 1;
			blEnableEcho = 0;

			com = "/dev/ttyUSB0";
			baudrate = 115200;
            comType = 0;
			tcpIP = "leifwen.com";
			tcpPort = 9527;
			udpIP = "127.0.0.1";
			udpPort = 8192;
			tcpsPort = 9527;
			udpsPort = 8192;
		};
		~ExpandDevFlag(void){;};
	public:
		int32		blCommandExplain;
		int32		blScriptAT;
		int32		blEnablePrintSBICinfo;
		int32		blEnableEcho;

		std::string	com;
		int32		baudrate;
	    int32	    comType;
		std::string	tcpIP;
		int32		tcpPort;
		std::string	udpIP;
		int32		udpPort;
		int32		tcpsPort;
		int32		udpsPort;
};
//------------------------------------------------------------------------------------------//
#define	DEVICE_T_RC(DB_TYPE,SER_FUN)\
{\
	uint32 ret;\
	if (CheckblConnect() == 0)\
		return 0;\
	ret = 0;\
	Spin_InUse_set(blLock);\
	switch (cgDevType){\
		case DEVID_APICOM:\
		case DEVID_TCPClient:\
		case DEVID_UDPClient:\
			if (cgCurrentDB != nullptr)\
				ret = cgCurrentDB->DB_TYPE;\
				break;\
		case DEVID_TCPServer:\
		case DEVID_UDPServer:\
			if (cgAPISocketServer != nullptr)\
				ret = cgAPISocketServer->SER_FUN;\
				break;\
		case DEVID_NONE:\
			break;\
	}\
	Spin_InUse_clr(blLock);\
	return(ret);\
};
//------------------------------------------------------------------------------------------//
#define	DEVICE_T_R(DB_TYPE,SER_FUN,V_TYPE)\
{\
	V_TYPE ret;\
	ret = 0;\
	Spin_InUse_set(blLock);\
	switch (cgDevType){\
		case DEVID_APICOM:\
		case DEVID_TCPClient:\
		case DEVID_UDPClient:\
			if (cgCurrentDB != nullptr)\
				ret = cgCurrentDB->DB_TYPE;\
				break;\
		case DEVID_TCPServer:\
		case DEVID_UDPServer:\
			if (cgAPISocketServer != nullptr)\
				ret = cgAPISocketServer->SER_FUN;\
				break;\
		case DEVID_NONE:\
			break;\
	}\
	Spin_InUse_clr(blLock);\
	return(ret);\
};
//------------------------------------------------------------------------------------------//
#define	DEVICE_T(DB_TYPE,SER_FUN)\
{\
	Spin_InUse_set(blLock);\
		switch (cgDevType){\
			case DEVID_APICOM:\
			case DEVID_TCPClient:\
			case DEVID_UDPClient:\
				if (cgCurrentDB != nullptr)\
					cgCurrentDB->DB_TYPE;\
					break;\
			case DEVID_TCPServer:\
			case DEVID_UDPServer:\
				if (cgAPISocketServer != nullptr)\
					cgAPISocketServer->SER_FUN;\
					break;\
			case DEVID_NONE:\
				break;\
		}\
	Spin_InUse_clr(blLock);\
}
//------------------------------------------------------------------------------------------//
class DEVICE : public RTREE_NODE{
	public:
		enum			{RFLAG_C = 1, RFLAG_S = RTREE_NODE::RFLAG_S + RTREE_NODE::RFLAG_C};
		enum DEVID_TYPE	{DEVID_NONE = 0,DEVID_APICOM,DEVID_TCPClient,DEVID_UDPClient,DEVID_TCPServer,DEVID_UDPServer};
		enum OPEN_TYPE	{OP_NONE = 0,OP_COM,OP_TCPClient,OP_UDPClient,OP_TCPServer,OP_UDPServer};
	public:
				 DEVICE(void) : RTREE_NODE(){CreateTrash(this);Init();};
		virtual ~DEVICE(void){Close(0);DestroyAll();};
	public:
		void	Init			(const ODEV_LIST_POOL *outList,uint32 tSize);
		int32	Open			(OPEN_TYPE tOPEN_TYPE,const std::string &devName,int32 baudrate,int32 blEnEcho);
		void	Close			(int32 blPrint);
    
		int32	CheckblConnect	(void){return(CheckSFlag(RFLAG_CREATE(0)));}
		void	UpdataUIRecord	(void);
#ifdef CommonDefH_VC
		void	UpdataUIRecordSDOUT(void);
#endif
		std::string	MakeFrmTitle(void);
	protected:
		uint32	cgBufMaxSize;
    
    	void	Init(void);
		void	SetblConnect	(void){SetSFlag(RFLAG_CREATE(0));};
		void	ClrblConnect	(void){ClrSFlag(RFLAG_CREATE(0));};
	protected:
		int32	APIEComOpen			(const std::string &devName,int32 baudrate,int32 blEnEcho);
		void	APIEComClose		(int32 blPrint);
		int32	APISocketOpen		(const std::string &devName,int32 baudrate,COMMU_DBUF::CSType tStype,int32 blEnEcho);
		void	APISocketClose		(int32 blPrint);
		int32	SocketServerOpen	(int32 baudrate,COMMU_DBUF::CSType tStype,int32 blEnEcho);
		void	SocketServerClose	(int32 blPrint);
	protected:
		void	LinkDB_nolock		(void);
	public:
		PUB_SBUF_LIST	rxBufferList;
    
		ExpandDevFlag	cEDevFlag;
    	ODEV_LIST		cgODevList;

    	DEVID_TYPE		cgDevType;
		COMMU_DBUF		*cgCurrentDB;
	protected:
		APICOM 			*CreateNewCOM			(const ODEV_LIST *tODEV_LIST,uint32 tSize);
		APISocket		*CreateNewSocket		(const ODEV_LIST *tODEV_LIST,uint32 tSize);
		APISocketServer	*CreateNewSocketServer	(const ODEV_LIST *tODEV_LIST,uint32 tSize);
	public:
		APICOM			*cgAPIECom;
		APISocket		*cgAPISocket;
		APISocketServer	*cgAPISocketServer;
	public:
		uint32	BufferRxMaxSize	(G_LOCK_VAILD blLock = G_LOCK_ON){DEVICE_T_R(BufferRxMaxSize(),SBufferRxMaxSize(),uint32);};
		uint32	BufferTxMaxSize	(G_LOCK_VAILD blLock = G_LOCK_ON){DEVICE_T_R(BufferTxMaxSize(),SBufferTxMaxSize(),uint32);};
		uint32	UnreadBytes		(G_LOCK_VAILD blLock = G_LOCK_ON){DEVICE_T_R(UnreadBytes(),SUnreadBytes(),uint32);};
		uint32	UnsentBytes		(G_LOCK_VAILD blLock = G_LOCK_ON){DEVICE_T_R(UnsentBytes(),SUnsentBytes(),uint32);};
		uint64	FwBytes			(G_LOCK_VAILD blLock = G_LOCK_ON){DEVICE_T_R(FwBytes(),SFwBytes(),uint64);};
		uint64	RxBytes			(G_LOCK_VAILD blLock = G_LOCK_ON){DEVICE_T_R(RxBytes(),SRxBytes(),uint64);};
		uint64	TxBytes			(G_LOCK_VAILD blLock = G_LOCK_ON){DEVICE_T_R(TxBytes(),STxBytes(),uint64);};
		void	EnableEcho		(G_LOCK_VAILD blLock = G_LOCK_ON){DEVICE_T(EnableEcho(),SEnableEcho());};
		void	DisableEcho		(G_LOCK_VAILD blLock = G_LOCK_ON){DEVICE_T(DisableEcho(),SDisableEcho());};
		void	Clean			(G_LOCK_VAILD blLock = G_LOCK_ON);
		uint32	Read			(uint8 *buffer,uint32 num,G_LOCK_VAILD blLock = G_LOCK_ON)
									{DEVICE_T_RC(Read(buffer,num),SRead(buffer,num));};
		uint32	Write			(const uint8 *newData,uint32 num,G_LOCK_VAILD blLock = G_LOCK_ON)
									{DEVICE_T_RC(Write(newData,num),SWrite(newData,num));};
		uint32	ReadInHEX		(std::string *strRec,G_SPACE_VAILD blSpace,G_LOCK_VAILD blLock = G_LOCK_ON)
									{DEVICE_T_RC(ReadInHEX(strRec,blSpace),SReadInHEX(strRec,blSpace));};
		uint32	WriteInHEX		(const std::string &strContent,G_LOCK_VAILD blLock = G_LOCK_ON)
									{DEVICE_T_RC(WriteInHEX(strContent),SWriteInHEX(strContent));};
		uint32	ReadInASCII		(std::string *strRec,G_ESCAPE_VAILD blEscape,G_LOCK_VAILD blLock = G_LOCK_ON)
									{DEVICE_T_RC(ReadInASCII(strRec,blEscape),SReadInASCII(strRec,blEscape));};
		uint32	WriteInASCII	(const std::string &strContent,G_ESCAPE_VAILD blEscape,G_LOCK_VAILD blLock = G_LOCK_ON)
									{DEVICE_T_RC(WriteInASCII(strContent,blEscape),SWriteInASCII(strContent,blEscape));};
	public:
		void	SendCommand		(const std::string &inputCommand,int32 blHEX,int32 bl0x0D,G_ESCAPE_VAILD blEscape);
		void	SendCommandOnly	(const std::string &inputCommand,int32 blHEX,int32 bl0x0D,G_ESCAPE_VAILD blEscape);
		void	PrintSendCommand(const std::string &strPrintData,uint32 num,COLSTRING::COLEnforcePrint blEP = COLSTRING::COL_EP_NO);
		void	PrintSendStr	(const std::string &strPrintData, const std::string &rColor, COLSTRING::COLEnforcePrint blEP = COLSTRING::COL_EP_NO);
		int32	CheckResult		(PUB_SBUF *cBuffer,const std::string &cCheckResult,int32 cTimeout);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif
