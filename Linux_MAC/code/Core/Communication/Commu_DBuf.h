/*
 * Copyright (c) 2012-2014 
 * All rights reserved.
 *
 * File Name: Commu_DBuf.h
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2012.02.09
 * @2013.6.30 :	change to Liunx style
 				add init method
 				use int32 to replace bool
 				use int32 to replace uint32
*/
#ifndef COMMU_DBUFH
#define COMMU_DBUFH
//------------------------------------------------------------------------------------------//
#include "Comm_Buffer.h"
#include "SYS_Thread.h"
#include "ODEV_Node.h"
//------------------------------------------------------------------------------------------//
class ODEV_NODE_FILE;
class ODEV_NODE_SDOUT;
class ODEV_LIST;
class ODEV_LIST_POOL;
class Field_Node;
//------------------------------------------------------------------------------------------//
class COMMU_DBUF_B : public PUB_DBUF{
	public:
		enum		{RFLAG_C = 6, RFLAG_S = PUB_DBUF::RFLAG_S + PUB_DBUF::RFLAG_C};
		enum		{PACKAGE_MAX_SIZE = 1024 * 8};
		enum CSType	{CSType_None = 0,CSType_TCP,CSType_UDP,CSType_TCPS,CSType_UDPS};
	public:
				 COMMU_DBUF_B(uint32 tSize) : PUB_DBUF(tSize){CreateTrash(this);Init();};
		virtual	~COMMU_DBUF_B(void){MoveNodeToTrash((RTREE_NODE*)cgSelfODdev,this);DestroyAll();UnlinkCoupleNode();CloseD();};
	private:
		void	Init	(void);
		Field_Node		*cgFNode_TX;//only in UDP mode need usd this
		ODEV_NODE		*cgSelfODdev;
	public:
		inline	void	Init(const Field_Node *fn){cgFNode_TX = (Field_Node*)fn;};
	protected:
		SYS_ThreadEx_List				cgThreadList;
		SYS_ThreadEx<COMMU_DBUF_B>		rxThread;
		SYS_ThreadEx<COMMU_DBUF_B>		txThread;
	private:
		virtual	int32	RxThreadFun		(void);
		virtual	int32	TxThreadFun		(void);
		virtual	int32	ReadFromDevice	(uint32 *retNum,uint8 *buffer,uint32 length)		{*retNum = 0;return 0;};
		virtual	int32	SendToDevice	(uint32 *retNum,const uint8 *buffer,uint32 length)	{*retNum = 0;return 0;};
		virtual	void	ForwardToCouple	(const uint8 *databuf,int32 num);
	public:
		virtual	void	DoAfterReadFromDevice	(const uint8 *databuf,int32 num);
	protected:
		virtual	int32	OpenDev		(const std::string &tCDBufName,int32 tCDBufPar,CSType tCSType){return 0;};
		virtual	int32	DoAfterOpen	(void);
		virtual	void	CloseDev	(void);
		virtual	void	OnCloseDev	(void);
		virtual void	ThreadsStart(void);
		virtual	ODEV_NODE*	CreateSelfODev	(COLSTRING::COLType tCOLType = COLSTRING::COLType_TXT);
	public:
		inline	void		SetcgSelfODdev	(ODEV_NODE *tDev){cgSelfODdev = tDev;};
		inline	ODEV_NODE*	GetcgSelfODdev	(void){return(cgSelfODdev);};
	private:
		PUB_SBUF_LIST	cgInternalFwSBufList;
		PUB_SBUF_LIST	*rxFwSBufList;	//used for quick forwder in rx thread
	protected:
		inline	PUB_SBUF_LIST*	GetRxFwSBufList(void){return(rxFwSBufList);};
	public:
		void	UseSelfFwSBL	(void);
		void	UseExternalFwSBL(const PUB_SBUF_LIST *tRxFwSBufList);
	protected:
		void	Run		(const std::string &tCDBufName,int32 tCDBufPar,CSType tCSType,int32 blEnEcho);
		int32	Close_Do(int32 blClrBufName = 1);
	public:
		int32	OpenD	(const std::string &tCDBufName,int32 tCDBufPar,CSType tCSType,int32 blEnEcho);
		int32	CloseD	(int32 blClrBufName = 1);
		void	PushSend(void);
	private:
		std::string		cgCDBufName;
		int32			cgCDBufPar;
		CSType			cgCSType;
	public:
		inline const std::string	&GetBufName		(void)const{return(cgCDBufName);};
		inline const int32			&GetBufPar		(void)const{return(cgCDBufPar);};
		inline const CSType			&GetCSType		(void)const{return(cgCSType);};
		inline const void			SetCSType		(CSType tCSType){cgCSType = tCSType;};
	protected:
		inline void		SetblConnected		(void)		{SetSFlag(RFLAG_CREATE(1));};
		inline void		ClrblConnected		(void)		{ClrSFlag(RFLAG_CREATE(1));};
		inline void		SetblAClose			(void)		{SetSFlag(RFLAG_CREATE(2));};
		inline void		ClrblAClose			(void)		{ClrSFlag(RFLAG_CREATE(2));};
		inline void		SetblSDC			(void)		{SetSFlag(RFLAG_CREATE(3));};
		inline void		ClrblSDC			(void)		{ClrSFlag(RFLAG_CREATE(3));};
		inline int32	CheckblHold			(void)const	{return(CheckSFlag(RFLAG_CREATE(4)));};
		inline void		SetblPushSend		(void)		{SetSFlag(RFLAG_CREATE(5));};
		inline void		ClrblPushSend		(void)		{ClrSFlag(RFLAG_CREATE(5));};
		inline int32	CheckblPushSend		(void)const	{return(CheckSFlag(RFLAG_CREATE(5)));};
		inline void		SetblcgRxBufferUsed	(void)		{SetSFlag(RFLAG_CREATE(6));};
		inline void		ClrblcgRxBufferUsed	(void)		{ClrSFlag(RFLAG_CREATE(6));};
		inline int32	CheckblcgRxBufferUsed(void)const{return(CheckSFlag(RFLAG_CREATE(6)));};
	public:
		inline void		EnableEcho			(void)		{SetSFlag(RFLAG_CREATE(0));};
		inline void		DisableEcho			(void)		{ClrSFlag(RFLAG_CREATE(0));};
		inline int32	CheckEnableEcho		(void)const	{return(CheckSFlag(RFLAG_CREATE(0)));};
		inline int32	IsConnected			(void)const	{return(CheckSFlag(RFLAG_CREATE(1)));};
		inline int32	CheckblAClose		(void)const	{return(CheckSFlag(RFLAG_CREATE(2)));};
		inline int32	CheckblSDC			(void)const	{return(CheckSFlag(RFLAG_CREATE(3)));};
		inline void		SetblHold			(void)		{SetSFlag(RFLAG_CREATE(4));};
		inline void		ClrblHold			(void)		{ClrSFlag(RFLAG_CREATE(4));};
};
//------------------------------------------------------------------------------------------//
class COMMU_DBUF : public COMMU_DBUF_B{
	public:
		enum		{RFLAG_C = 0, RFLAG_S = COMMU_DBUF_B::RFLAG_S + COMMU_DBUF_B::RFLAG_C};
	public:
				 COMMU_DBUF(const ODEV_LIST *tODEV_LIST,uint32 tSize) : COMMU_DBUF_B(tSize){Init(tODEV_LIST);};
		virtual	~COMMU_DBUF(void){exThread.RemoveSelf(); ex2Thread.RemoveSelf();};
	private:
		void	Init	(const ODEV_LIST *tODEV_LIST);
	protected:
		SYS_ThreadEx<COMMU_DBUF>		exThread;
		SYS_ThreadEx<COMMU_DBUF>		ex2Thread;
	private:
		virtual	int32	ExThreadFun		(void){return 1;};
		virtual	int32	Ex2ThreadFun	(void);
	private:
		ODEV_LIST		*cgODevList;	//used for external
		ODEV_LIST_POOL	*cgOutput;		//used for external
	protected:
		inline	ODEV_LIST		*GetcgODevList	(void){return(cgODevList);};
		inline	ODEV_LIST_POOL	*GetcgOutput	(void){return(cgOutput);};
	private:
				ODEV_LIST_POOL	*cgODevPool;
				ODEV_LIST_POOL	*CreateOutputList	(void);
	public:
				void 			SetODEV_LIST	(const ODEV_LIST *tODEV_LIST);
		inline	ODEV_LIST_POOL	*GetcgODevPool	(void){return(cgODevPool);};
				ODEV_NODE_SDOUT	*CreateODevSDOUT(const void *tRichEdit,const void *tCFrm);
				ODEV_NODE_FILE	*CreateODevFile	(const std::string &tfileName);
	public:
		void	UpdataRecordUI(void);
#ifdef CommonDefH_VC
		void	UpdataRecordUISDOUT(void);
#endif
	protected:
		void	PrintRecData		(ODEV_LIST_POOL *output, const std::string &strData, uint32 byteNum, int32 blEnRecMsg);
		void	PrintRecData_lock	(ODEV_LIST_POOL *output, const std::string &strData, uint32 byteNum, int32 blEnRecMsg);
	public:
		void	PrintOpenSuccessReport		(const std::string &strData);
		void	PrintOpenFailReport			(const std::string &strDevName);
	public:
		virtual	void	PrintUserDisconnectReport	(const std::string &strDevName);
		virtual void	CreateLogFile				(void){;};
};
//------------------------------------------------------------------------------------------//
class CDBUF_POOL : public PUB_DBUF_LIST{
	public:
		enum{RFLAG_C = 0, RFLAG_S = PUB_DBUF_LIST::RFLAG_S + PUB_DBUF_LIST::RFLAG_C};
	public:
				 CDBUF_POOL(void) : PUB_DBUF_LIST(){CreateTrash(this);};
		virtual ~CDBUF_POOL(void){ DestroyAll(); };
	private:
	public:
		void	LUpdataRecordUI	(void){RTREE_LChildRChain_T(COMMU_DBUF,UpdataRecordUI());};
#ifdef CommonDefH_VC
		void	LUpdataRecordUISDOUT(void){RTREE_LChildRChain_T(COMMU_DBUF,UpdataRecordUISDOUT());};
#endif
		void	LUnlink			(void){RTREE_LChildRChain_T(COMMU_DBUF,UnlinkCoupleNode());};
	
		void	SEnableEcho		(void){RTREE_LChildRChain_T(COMMU_DBUF,EnableEcho());};
		void	SDisableEcho	(void){RTREE_LChildRChain_T(COMMU_DBUF,DisableEcho());};
		void	LEnableEcho		(void){RTREE_LChildRChain_T(COMMU_DBUF,EnableEcho());};
		void	LDisableEcho	(void){RTREE_LChildRChain_T(COMMU_DBUF,DisableEcho());};
	
		COMMU_DBUF	*Find		(const std::string &tDBufName,int32 tDBufPar);
		COMMU_DBUF	*SetSelect	(const std::string &tDBufName,int32 tDBufPar);
};
//------------------------------------------------------------------------------------------//
#endif