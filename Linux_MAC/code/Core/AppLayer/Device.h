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
//------------------------------------------------------------------------------------------//
#include "Comm_Tree.h"
#include "ODEV_Include.h"
#include "Commu_Include.h"
#include "SBIC.h"
//------------------------------------------------------------------------------------------//
#if defined COMMU_DBUFH && defined OUTPUTH
#ifndef DeviceH
#define DeviceH
#ifdef DeviceH
//------------------------------------------------------------------------------------------//
class DEVICE;
class ExpandDeviceAttr{
	public:
		~ExpandDeviceAttr(void){;};
		ExpandDeviceAttr(void){
			SetOpenPar(aCOM	,"/dev/ttyUSB0"	,115200	,CSType_COM ,0);
			SetOpenPar(aTCP	,"leifwen.com"	,9527	,CSType_TCP ,0);
			SetOpenPar(aUDP	,"127.0.0.1"	,8192	,CSType_UDP ,0);
			SetOpenPar(aTCPS,""				,9527	,CSType_TCPS,0);
			SetOpenPar(aUDPS,""				,8192	,CSType_UDPS,0);
			csType = CSType_None;
			
			device = nullptr;
			envcfg = nullptr;
		};
	public:
		OPEN_PAR	aCOM;
		OPEN_PAR	aTCP;
		OPEN_PAR	aUDP;
		OPEN_PAR	aTCPS;
		OPEN_PAR	aUDPS;
		CSType		csType;
	public:
		DEVICE					*device;
		uint64					*envcfg;
		inline	int32			IsConnected		(void)const;
		inline	const CSType	&DevType		(void)const;
		inline	ACOM 			*ACom			(void)const;
		inline	ASOCKET			*ASocket		(void)const;
		inline	ASOCKETSERVER	*AServer		(void)const;
		inline	int32			IsCom			(void)const;
		inline	int32			IsSocket		(void)const;
		inline	int32			IsServer		(void)const;
		inline	int32			IsComOpened		(void)const;
		inline	int32			IsSocketOpened	(void)const;
		inline	int32			IsServerOpened	(void)const;
		inline	int32			IsTCPOpened		(void)const;
		inline	int32			IsUCPOpened		(void)const;
		inline	int32			IsTCPSOpened	(void)const;
		inline	int32			IsUDPSOpened	(void)const;
	
		inline	ODEV_VG3D		&GetVG3D		(void);
};
//------------------------------------------------------------------------------------------//
class DEVICE_FRAME : public COMMU_DBUF_FRAME_FW{
	public:
				 DEVICE_FRAME(void) : COMMU_DBUF_FRAME_FW(){;};
				 DEVICE_FRAME(uint32 tSize,const ODEV_SYSTEM *logSys = nullptr) : COMMU_DBUF_FRAME_FW(){COMMU_DBUF_FRAME_FW::Init(tSize,logSys);SetSelfName("DEVICE_FRAME");};
		virtual ~DEVICE_FRAME(void){;};
	public:
		inline			uint32	SRead		(FIFO8 *fifoOut,uint32 num){
			uint32 ret;
			Spin_InUse_set();
			ret = GetSelDB()->GetSelDB()->Read(fifoOut,num);
			Spin_InUse_clr();
			return(ret);
		};
		inline			uint32	SRead		(uint8 *dataOut,uint32 num){
			uint32 ret;
			Spin_InUse_set();
			ret = GetSelDB()->GetSelDB()->Read(dataOut,num);
			Spin_InUse_clr();
			return(ret);
		};
		inline	const	STDSTR&	SRead		(STDSTR *retStr,G_ESCAPE blEscape){
			Spin_InUse_set();
			GetSelDB()->GetSelDB()->Read(retStr,blEscape);
			Spin_InUse_clr();
			return(*retStr);
		};
		inline	const	STDSTR&	SReadInHEX	(STDSTR *retStr,G_SPACE blSpace){
			Spin_InUse_set();
			GetSelDB()->GetSelDB()->ReadInHEX(retStr,blSpace);
			Spin_InUse_clr();
			return(*retStr);
		};
	public:
		inline	uint32	SSend		(const PROTOCOL_NODE &pnIn){
			uint32 ret;
			Spin_InUse_set();
			ret = GetSelDB()->GetSelDB()->Send(pnIn);
			Spin_InUse_clr();
			return(ret);
		};
		inline	uint32	SSend		(const FIFO8 &fifoIn,uint32 num,uint32 offset){
			uint32 ret;
			Spin_InUse_set();
			ret = GetSelDB()->GetSelDB()->Send(fifoIn,num,offset);
			Spin_InUse_clr();
			return(ret);
		};
		inline	uint32	SSend		(const uint8 *data,uint32 num){
			uint32 ret;
			Spin_InUse_set();
			ret = GetSelDB()->GetSelDB()->Send(data,num);
			Spin_InUse_clr();
			return(ret);
		};
		inline	uint32	SSend		(const uint8 data){
			uint32 ret;
			Spin_InUse_set();
			ret = GetSelDB()->GetSelDB()->Send(data);
			Spin_InUse_clr();
			return(ret);
		};
		inline	uint32	SSend		(const STDSTR &strIn,G_ESCAPE blEscape){
			uint32 ret;
			Spin_InUse_set();
			ret = GetSelDB()->GetSelDB()->Send(strIn,blEscape);
			Spin_InUse_clr();
			return(ret);
		};
		inline	uint32	SSendInHEX	(const STDSTR &strIn){
			uint32 ret;
			Spin_InUse_set();
			ret = GetSelDB()->GetSelDB()->SendInHEX(strIn);
			Spin_InUse_clr();
			return(ret);
		};
	public:
		inline void		SClean		(void){
			Spin_InUse_set();
			GetSelDB()->GetSelDB()->Clean();
			Spin_InUse_clr();
		};
		inline uint32	SRxSUFMaxSize(void){
			uint32 num;
			Spin_InUse_set();
			num = GetSelDB()->GetSelDB()->RxSUFMaxSize();
			Spin_InUse_clr();
			return(num);
		};
		inline uint32	STxSUFMaxSize(void){
			uint32 num;
			Spin_InUse_set();
			num = GetSelDB()->GetSelDB()->TxSUFMaxSize();
			Spin_InUse_clr();
			return(num);
		};
		inline	uint32	SUnreadBytes(void){
			uint32 num;
			Spin_InUse_set();
			num = GetSelDB()->GetSelDB()->UnreadBytes();
			Spin_InUse_clr();
			return(num);
		};
		inline	uint32	SUnsentBytes(void){
			uint32 num;
			Spin_InUse_set();
			num = GetSelDB()->GetSelDB()->UnsentBytes();
			Spin_InUse_clr();
			return(num);
		};
		inline uint64	SFwBytes(void){
			uint64 num;
			Spin_InUse_set();
			num = GetSelDB()->GetSelDB()->FwBytes();
			Spin_InUse_clr();
			return(num);
		};
		inline uint64	SRxBytes(void){
			uint64 num;
			Spin_InUse_set();
			num = GetSelDB()->GetSelDB()->RxBytes();
			Spin_InUse_clr();
			return(num);
		};
		inline uint64	STxBytes(void){
			uint64 num;
			Spin_InUse_set();
			num = GetSelDB()->GetSelDB()->TxBytes();
			Spin_InUse_clr();
			return(num);
		};
};
//------------------------------------------------------------------------------------------//
class DEVICE : public DEVICE_FRAME{
	public:
		enum	{RFLAG_C = 1, RFLAG_S = DEVICE_FRAME::RFLAG_S + DEVICE_FRAME::RFLAG_C};
	public:
				 DEVICE(void) : DEVICE_FRAME(){Init();};
				 DEVICE(uint32 tSize,const ODEV_CACHE *cache = nullptr) : DEVICE_FRAME(){Init();Init(tSize,cache);DisableLog();};
		virtual ~DEVICE(void){CloseD();DestroyAll();};
	public:
		void	Init	(uint32 tSize,const ODEV_CACHE *cache = nullptr){
			cgLogSystem.cache = (ODEV_CACHE*)cache;
			COMMU_DBUF_FRAME_FW::Init(0,&cgLogSystem);
			cgBufMaxSize = tSize;
			CreateTrash(this);
		};
	private:
		void	Init(void){
			cgBufMaxSize = 0;
			cgCom = nullptr;
			cgSocket = nullptr;
			cgServer = nullptr;
			cgEDA.device = this;
			cgEDA.envcfg = &cgLogSystem.envcfg;
			SetSelfName("Device");
		};
	private:
				virtual	int32	OpenDev					(const STDSTR &tCDBufName,int32 tCDBufPar,CSType tCSType,int32 blEnEcho);
				virtual	void	CloseDev				(void);
		inline	virtual	void	ChildClose				(COMMU_DBUF_FRAME *commu)	{SelfClose();};
		inline	virtual	void	ChildCloseAll			(void){;};
		inline	virtual	void	DoPrintOnOpenSuccess	(void){;};
		inline	virtual	void	DoPrintOnOpenFail		(void){;};
		inline	virtual	void	DoPrintOnClose			(void){;};
	public:
				virtual	void	EnableEcho				(void);
				virtual	void	DisableEcho				(void);
	public:
		int32	Open_Com		(const STDSTR &tCDBufName,int32 tCDBufPar,CSType tCSType,int32 blEnEcho);
		int32	Open_Socket		(const STDSTR &tCDBufName,int32 tCDBufPar,CSType tCSType,int32 blEnEcho);
		int32	Open_Server		(const STDSTR &tCDBufName,int32 tCDBufPar,CSType tCSType,int32 blEnEcho);
		void	Close_Com		(void);
		void	Close_Socket	(void);
		void	Close_Server	(void);
	protected:
		ACOM					*cgCom;
		ASOCKET					*cgSocket;
		ASOCKETSERVER			*cgServer;
	private:
		const	ACOM			*CreateCOM		(void);
		const	ASOCKET			*CreateSocket	(void);
		const	ASOCKETSERVER	*CreateServer	(void);
	public:
		inline	ACOM 			*ACom			(void)const{return(cgCom);};
		inline	ASOCKET			*ASocket		(void)const{return(cgSocket);};
		inline	ASOCKETSERVER	*AServer		(void)const{return(cgServer);};
	protected:
		ODEV_SYSTEM		cgLogSystem;
	public:
		ExpandDeviceAttr	cgEDA;
		STDSTR	MakeFrmTitle(void);
	public:
		OUTPUT_NODE	*GetSelStdout	(void)	{return(static_cast<COMMU_DBUF_FRAME_LOG*>(GetSelDB()->GetSelDB())->GetVG3D());};
	public:
	#ifdef SBIC_BuildH
		SBIC_Print	cgSBIC_PRINT;
	#endif
	#ifdef SBIC_ExpressionH
		SBIC_Expression cgSBIC_Exp;
	#endif
		uint32	SendCommand			(const STDSTR &cmd,CMD_TAIL tail,G_ESCAPE blEscape);
		uint32	SendCommandWithPrint(const STDSTR &cmd,CMD_TAIL tail,G_ESCAPE blEscape);
		int32	SendCommand			(const STDSTR &cmd,CMD_TAIL tail,G_ESCAPE blEscape,const STDSTR &strCheck,SBUFFER *cSBUF,uint32 waitMS,uint32 delyMS);
		int32	SendCommandWithPrint(const STDSTR &cmd,CMD_TAIL tail,G_ESCAPE blEscape,const STDSTR &strCheck,SBUFFER *cSBUF,uint32 waitMS,uint32 delyMS);
		uint32	SendHexCommand		(const STDSTR &hexcmd);
		uint32	SendHexCommandWithPrint(const STDSTR &hexcmd);
		int32	SendHexCommand		(const STDSTR &hexcmd,const STDSTR &strCheck,SBUFFER *cSBUF,uint32 waitMS,uint32 delyMS);
		void	PrintSendCommand	(const STDSTR &strPrintData,uint32 num);
		void	PrintSendCommand	(const STDSTR &strTitle1,const STDSTR &strTitle2,const STDSTR &strPrintData);
		void	PrintSendStrWOG1	(const STDSTR &strPrintData);
};
//------------------------------------------------------------------------------------------//
class DEVICE_EXE_FRAME : public TREE_NODE{
		typedef  SYS_Thread<DEVICE_EXE_FRAME>	ThreadType;
	public:
		enum{RFLAG_C = 1, RFLAG_S = TREE_NODE::RFLAG_S + TREE_NODE::RFLAG_C};
		enum{blIsTerminated = RFLAG_CREATE(0),};
		enum{BUF_MAX_SIZE = 1024 * 8};
	public:
				 DEVICE_EXE_FRAME(void);
		virtual ~DEVICE_EXE_FRAME(void){Stop();};
	protected:
		OUTPUT_NODE	*GetDevOut		(void){return(cgDevice->GetSelStdout());};
		void		DeviceLock		(void){cgDevice->Spin_Child_set();};
		void		DeviceUnlock	(void){cgDevice->Spin_Child_clr();};
	protected:
		ThreadType	executeThread;
		SBUFFER		cgSBUF;
		DEVICE		*cgDevice;
	protected:
				int32	CheckExecute		(const DEVICE *tDevice);
				int32	ExecuteThreadFun	(void *p);
		virtual	int32	Execute				(const DEVICE *tDevice,SYS_AThread *threadP);
		virtual	void	Init				(const DEVICE *tDevice);
		virtual	void	SetblIsTerminated	(void)		{SetSFlag(blIsTerminated);};
		virtual	void	ClrblIsTerminated	(void)		{ClrSFlag(blIsTerminated);};
				int32	IsTerminated		(void)const	{return(CheckSFlag(blIsTerminated));};
	public:
		virtual	void	Stop				(void);
				int32 	IsStop				(void)const	{return((CheckEnable()) == 0);};
				int32	CheckServiceTo		(const DEVICE *tDevice){return(tDevice == cgDevice);};
};
//------------------------------------------------------------------------------------------//
inline	int32			ExpandDeviceAttr::IsConnected	(void)const{return(device->IsConnected());};
inline	const CSType	&ExpandDeviceAttr::DevType		(void)const{return(device->GetCSType());};
inline	ACOM 			*ExpandDeviceAttr::ACom			(void)const{return(device->ACom());};
inline	ASOCKET			*ExpandDeviceAttr::ASocket		(void)const{return(device->ASocket());};
inline	ASOCKETSERVER	*ExpandDeviceAttr::AServer		(void)const{return(device->AServer());};
inline	int32			ExpandDeviceAttr::IsCom			(void)const{return((device->GetCSType() == CSType_COM) || (device->GetCSType() == CSType_COMV));};
inline	int32			ExpandDeviceAttr::IsSocket		(void)const{return((device->GetCSType() == CSType_TCP) || (device->GetCSType() == CSType_UDP));};
inline	int32			ExpandDeviceAttr::IsServer		(void)const{return((device->GetCSType() == CSType_TCPS) || (device->GetCSType() == CSType_UDPS));};
inline	int32			ExpandDeviceAttr::IsComOpened	(void)const{return((device->IsConnected() != 0) && ((device->GetCSType() == CSType_COM) || (device->GetCSType() == CSType_COMV)));};
inline	int32			ExpandDeviceAttr::IsSocketOpened(void)const{return((device->IsConnected() != 0) && ((device->GetCSType() == CSType_TCP) || (device->GetCSType() == CSType_UDP)));};
inline	int32			ExpandDeviceAttr::IsServerOpened(void)const{return((device->IsConnected() != 0) && ((device->GetCSType() == CSType_TCPS) || (device->GetCSType() == CSType_UDPS)));};
inline	int32			ExpandDeviceAttr::IsTCPOpened	(void)const{return((device->IsConnected() != 0) && (device->GetCSType() == CSType_TCP));};
inline	int32			ExpandDeviceAttr::IsUCPOpened	(void)const{return((device->IsConnected() != 0) && (device->GetCSType() == CSType_UDP));};
inline	int32			ExpandDeviceAttr::IsTCPSOpened	(void)const{return((device->IsConnected() != 0) && (device->GetCSType() == CSType_TCPS));};
inline	int32			ExpandDeviceAttr::IsUDPSOpened	(void)const{return((device->IsConnected() != 0) && (device->GetCSType() == CSType_UDPS));};
inline	ODEV_VG3D&		ExpandDeviceAttr::GetVG3D		(void){return(*static_cast<COMMU_DBUF_FRAME_LOG*>(device->GetSelDB())->GetVG3D());};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif
#endif
#endif
