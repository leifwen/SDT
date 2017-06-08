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
//------------------------------------------------------------------------------------------//
#include "SYS_Time.h"
#include "Commu_SSL.h"
#include "ODEV_Include.h"
//------------------------------------------------------------------------------------------//
#ifndef CSTypeH
#define CSTypeH
enum CSType	{CSType_None = 0,CSType_COM,CSType_COMV,CSType_TCP,CSType_UDP,CSType_TCPS,CSType_UDPS};
#endif
//------------------------------------------------------------------------------------------//
#if defined Comm_BufferH && defined ODEV_SystemH
#ifndef COMMU_DBUFH
#define COMMU_DBUFH
#ifdef COMMU_DBUFH
//------------------------------------------------------------------------------------------//
struct OPEN_PAR{
	STDSTR		name;
	int32		port;
	CSType		type;
	int32		blEcho;
};
inline void SetOpenPar(OPEN_PAR &op,const STDSTR &tCDBufName,int32 tCDBufPar,CSType tCSType,int32 blEnEcho){
	op.name = tCDBufName;
	op.port = tCDBufPar;
	op.type = tCSType;
	op.blEcho = blEnEcho;
};
enum{
	COL_DB_TxTitle		= COL_clPurple,
	COL_DB_TxText		= COL_clBlack,
	COL_DB_RxTitle		= COL_clRed,
	COL_DB_RxText		= COL_clBlue,
	COL_DB_RxTitleM		= COL_DB_TxTitle,
	COL_DB_RxTextM		= COL_DB_TxText,
	COL_DB_RxTitleS		= COL_DB_RxTitle,
	COL_DB_RxTextS		= COL_DB_RxText,
	COL_DB_Statistics	= COL_clRed,
};
#define	COL_RxData(_str)	ColData(COL_DB_RxText,_str)
#define	COL_TxData(_str)	ColData(COL_DB_TxText,_str)
//------------------------------------------------------------------------------------------//
class COMMU_DBUF_FRAME : public DBUFFER{
	public:
		enum	{RFLAG_C = 6, RFLAG_S = DBUFFER::RFLAG_S + DBUFFER::RFLAG_C};
		enum	{PACKAGE_MAX_SIZE = 1024 * 8};
	private:
		enum{
			cdf_blConnected		= RFLAG_CREATE(0),
			cdf_blOpened		= RFLAG_CREATE(1),
			cdf_blCloseByUser	= RFLAG_CREATE(2),
			cdf_blCloseDueToOS	= RFLAG_CREATE(3),
			cdf_blPushSend		= RFLAG_CREATE(4),
			cdf_blEcho			= RFLAG_CREATE(5),
		};
	public:
				 COMMU_DBUF_FRAME(void):DBUFFER(){;};
				 COMMU_DBUF_FRAME(uint32 tSize) : DBUFFER()	{Init(tSize);SetSelfName("COMMU_DBUF_FRAME");};
		virtual	~COMMU_DBUF_FRAME(void)						{UnlinkCoupleNode();CloseD();selfCloseThread.ThreadStop();};
	protected:
		void	Init	(uint32 tSize);
	protected:
		SYS_Thread_List					cgThreadList;
		SYS_Thread<COMMU_DBUF_FRAME>	selfCloseThread;
	private:
		int32	SelfCloseThreadFun	(void *p);
	protected:
				virtual	int32	DoOpen			(const STDSTR &tCDBufName,int32 tCDBufPar,CSType tCSType,int32 blEnEcho);
		inline	virtual	int32	OpenDev			(const STDSTR &tCDBufName,int32 tCDBufPar,CSType tCSType,int32 blEnEcho){return 0;};
				virtual void	DoThreadsStart	(void);
		inline	virtual int32	ThreadsStart	(void){DoThreadsStart();return 1;};
				virtual	void	DoClose			(void);
		inline	virtual	void	CloseDev		(void){;};
		inline	virtual void	ThreadsStop		(void){cgThreadList.LThreadStop();};
	public:
				virtual	void	ChildCloseAll	(void);
				virtual	void	ChildClose		(COMMU_DBUF_FRAME *commu);
	public:
				int32	Open		(const OPEN_PAR& par){return(OpenD(par.name,par.port,par.type,par.blEcho));};
				int32	OpenD		(const STDSTR &tCDBufName,int32 tCDBufPar,CSType tCSType,int32 blEnEcho,G_LOCK blLock = G_LOCK_ON);
				void	CloseD		(void);
				void	SelfClose	(void);
	public:
				void	PushSend	(void);
		inline	void	AddThread	(TREE_NODE *thread){cgThreadList.AddNode(thread);};
	private:
		STDSTR		cgDevName;
		STDSTR		cgCDBufName;
		int32		cgCDBufPar;
		CSType		cgCSType;
	public:
		inline const STDSTR	&GetDevName		(void)const		{return(cgDevName);};
		inline const STDSTR	&GetBufName		(void)const		{return(cgCDBufName);};
		inline const int32	&GetBufPar		(void)const		{return(cgCDBufPar);};
		inline const CSType	&GetCSType		(void)const		{return(cgCSType);};
		inline const void	SetCSType		(CSType tCSType){cgCSType = tCSType;};
	protected:
		inline int32	ChkblOpened			(void)const	{return(CheckSFlag(cdf_blOpened));};
		inline int32	ChkblCloseByUser	(void)const	{return(CheckSFlag(cdf_blCloseByUser));};
		inline int32	ChkblCloseDueToOS	(void)const	{return(CheckSFlag(cdf_blCloseDueToOS));};
	
		inline void		ClrblPushSend		(void)		{ClrSFlag(cdf_blPushSend);};
		inline int32	ChkblPushSend		(void)const	{return(CheckSFlag(cdf_blPushSend));};
	public:
		inline int32	IsConnected			(void)const	{return(CheckSFlag(cdf_blConnected));};
	
		inline virtual	void	EnableEcho	(void)		{SetSFlag(cdf_blEcho);};
		inline virtual	void	DisableEcho	(void)		{ClrSFlag(cdf_blEcho);};
		inline			int32	CheckEcho	(void)const	{return(CheckSFlag(cdf_blEcho));};
	public:
		COMMU_DBUF_FRAME *Find(const STDSTR &tDBufName,int32 tDBufPar){
			COMMU_DBUF_FRAME *retCmmu;
			retCmmu = nullptr;
			TREE_LChildRChain_Find(COMMU_DBUF_FRAME,this,retCmmu,((operateNode_t->GetBufName() == tDBufName) && (operateNode_t->GetBufPar() == tDBufPar)));
			return(retCmmu);
		}
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
class COMMU_DBUF_FRAME_LOG : public COMMU_DBUF_FRAME{
	public:
		enum	{RFLAG_C = 1, RFLAG_S = COMMU_DBUF_FRAME::RFLAG_S + COMMU_DBUF_FRAME::RFLAG_C};
	private:
		enum	{cdfl_blLog = RFLAG_CREATE(0),};
	public:
				 COMMU_DBUF_FRAME_LOG(void):COMMU_DBUF_FRAME(){;};
				 COMMU_DBUF_FRAME_LOG(uint32 tSize,const ODEV_SYSTEM *logSys = nullptr) : COMMU_DBUF_FRAME(){Init(tSize,logSys);SetSelfName("COMMU_DBUF_FRAME_LOG");};
		virtual	~COMMU_DBUF_FRAME_LOG(void);
	protected:
		void	Init	(uint32 tSize,const ODEV_SYSTEM *logSys = nullptr);
	protected:
		virtual	int32	DoOpen					(const STDSTR &tCDBufName,int32 tCDBufPar,CSType tCSType,int32 blEnEcho);
		virtual	void	DoClose					(void);
	private:
		virtual	void	DoPrintOnOpenSuccess	(void){PrintOpenSuccessReport();};
		virtual	void	DoPrintOnOpenFail		(void){PrintOpenFailReport();};
		virtual	void	DoPrintOnClose			(void){PrintConnectionReport("User closed the connection");};
	private:
		ODEV_SYSTEM		*logSystem;
		ODEV_VG3D		*cgVG3D;
		OUTPUT_NODE		*cgG2Self;
	public:
		inline	ODEV_SYSTEM		*GetLogSystem		(void)const {return(logSystem);};
		inline	ODEV_VG3D		*GetVG3D			(void)const	{return(cgVG3D);};
				ODEV_VG3D		*CreateODev_VG3D	(void);
				ODEV_FILE		*CreateODev_G3File	(const STDSTR &fName);
		virtual	OUTPUT_NODE		*CreateODev_G2Self	(void);
	public:
		OUTPUT_NODE		*GetG2DefSelf		(void)const;
		OUTPUT_NODE		*GetG3DefSelf		(void)const;
		ODEV_STDOUT		*GetG3DefSTDOUT		(void)const;
		ODEV_FILE		*GetG3DefFile		(void)const;
	public:
		OUTPUT_NODE		*AddG3D_Self	(OUTPUT_NODE *oG3D);
		ODEV_STDOUT		*AddG3D_STDOUT	(ODEV_STDOUT *oG3D);
	protected:
		STDSTR	cgAttrTitle;
	public:
		inline	void SetAttrTitle	(const STDSTR &strIn){cgAttrTitle = strIn;};
	protected:
		void PrintStr				(const COLSTR &colStr1,const COLSTR &colStr2 = "",const COLSTR &colStr3 = "",const COLSTR &colStr4 = ""
									,const COLSTR &colStr5 = "",const COLSTR &colStr6 = "",const COLSTR &colStr7 = "",const COLSTR &colStr8 = "");
		void PrintStrNL				(const COLSTR &colStr1,const COLSTR &colStr2 = "",const COLSTR &colStr3 = "",const COLSTR &colStr4 = ""
									,const COLSTR &colStr5 = "",const COLSTR &colStr6 = "",const COLSTR &colStr7 = "",const COLSTR &colStr8 = "");
		void PrintWithTime			(const COLSTR &colStr1,const COLSTR &colStr2 = "",const COLSTR &colStr3 = "",const COLSTR &colStr4 = ""
									,const COLSTR &colStr5 = "",const COLSTR &colStr6 = "",const COLSTR &colStr7 = "",const COLSTR &colStr8 = "");
		void PrintWithDividingLine	(const COLSTR &colStr1,const COLSTR &colStr2 = "",const COLSTR &colStr3 = "",const COLSTR &colStr4 = ""
									,const COLSTR &colStr5 = "",const COLSTR &colStr6 = "",const COLSTR &colStr7 = "",const COLSTR &colStr8 = "");
		void PrintMessage			(const COLSTR &colStr1,const COLSTR &colStr2 = "",const COLSTR &colStr3 = "",const COLSTR &colStr4 = ""
									,const COLSTR &colStr5 = "",const COLSTR &colStr6 = "",const COLSTR &colStr7 = "",const COLSTR &colStr8 = "");
		void PrintNormalMessage		(const STDSTR &strData1,const STDSTR &strData2 = "",const STDSTR &strData3 = "",const STDSTR &strData4 = ""
									,const STDSTR &strData5 = "",const STDSTR &strData6 = "",const STDSTR &strData7 = "",const STDSTR &strData8 = "");
		void PrintWarningMessage	(const STDSTR &strData1C1,const STDSTR &strData2C2 = "",const STDSTR &strData3C2 = "",const STDSTR &strData4C2 = ""
									,const STDSTR &strData5C2 = "",const STDSTR &strData6C2 = "",const STDSTR &strData7C2 = "",const STDSTR &strData8C2 = "");
		void PrintConnectionReport	(OUTPUT_NODE *node,const STDSTR &strData1,const STDSTR &strData2 = "",const STDSTR &strData3 = "",const STDSTR &strData4 = "");
	protected:
				void PrintConnectionReport	(const STDSTR &strData1,const STDSTR &strData2 = "",const STDSTR &strData3 = "",const STDSTR &strData4 = "");
		inline	void PrintOpenSuccessReport	(const STDSTR &strTitle = ""){PrintNormalMessage ("Successfully connect to",cgAttrTitle,strTitle,GetDevName());};
		inline	void PrintOpenFailReport	(const STDSTR &strTitle = ""){PrintWarningMessage("Failed connect to",strTitle,GetDevName());};
	protected:
		SYS_Thread<COMMU_DBUF_FRAME_LOG>	logThread;
	protected:
					SBUFFER		cgLogSBUF;
		virtual		int32		LogThreadFun			(void *p){cgLogSBUF.Init(PACKAGE_MAX_SIZE);return 1;};
	public:
		inline void		EnableLog			(void)		{logThread.Enable();SetSFlag(cdfl_blLog);};
		inline void		DisableLog			(void)		{logThread.Disable();ClrSFlag(cdfl_blLog);};
		inline int32	CheckEnableLog		(void)const	{return(CheckSFlag(cdfl_blLog));};
};
//------------------------------------------------------------------------------------------//
class COMMU_DBUF_FRAME_FW : public COMMU_DBUF_FRAME_LOG{
	public:
				 COMMU_DBUF_FRAME_FW(void):COMMU_DBUF_FRAME_LOG(){;};
				 COMMU_DBUF_FRAME_FW(uint32 tSize,const ODEV_SYSTEM *logSys = nullptr) : COMMU_DBUF_FRAME_LOG(){Init(tSize,logSys);SetSelfName("COMMU_DBUF_FRAME_FW");};
		virtual	~COMMU_DBUF_FRAME_FW(void);
	protected:
		void	Init	(uint32 tSize,const ODEV_SYSTEM *logSys = nullptr);
	public:
		inline	virtual	void	AddSon		(DBUFFER *sdb)	{COMMU_DBUF_FRAME_LOG::AddSon(sdb);static_cast<COMMU_DBUF_FRAME_FW*>(sdb)->UseExternalFwSBufList(GetFwSBufList());};
		inline	virtual	void	RemoveSelf	(void)			{UseSelfFwSBufList();COMMU_DBUF_FRAME_LOG::RemoveSelf();};
	protected:
		SYS_Thread<COMMU_DBUF_FRAME_FW>	fwThread;
		SBUF_LIST	cgInternalFwSBufList;
	private:
		SBUF_LIST	*rxFwSBufList;
	private:
		virtual	int32	FwThreadFun		(void *p);
	private:
		static	uint32	ForwardToSelf	(COMMU_DBUF_FRAME_FW *commu,const FIFO8 &fifoIn,uint32 num,uint32 *outNum,int32 blInit);
	public:
		const SBUF_LIST *GetFwSBufList	(void){return(rxFwSBufList);};
		void	UseSelfFwSBufList		(void);
		void	UseExternalFwSBufList	(const SBUF_LIST *tRxFwSBufList);
		void	RxDataShareTo			(SBUFFER *tSBUF);
		
		inline	void	SetGetDataByRead	(void){fwThread.Disable();fwThread.ThreadStop();};
		inline	void	SetGetDataByShare	(void){fwThread.Enable();};
};
//------------------------------------------------------------------------------------------//
class COMMU_DBUF_THREAD : public COMMU_DBUF_FRAME_FW{
	public:
				 COMMU_DBUF_THREAD():COMMU_DBUF_FRAME_FW(){;};
				 COMMU_DBUF_THREAD(uint32 tSize,const ODEV_SYSTEM *logSys) : COMMU_DBUF_FRAME_FW(){Init(tSize,logSys);SetSelfName("COMMU_DBUF_THREAD");};
		virtual	~COMMU_DBUF_THREAD(void);
	protected:
		void	Init	(uint32 tSize,const ODEV_SYSTEM *logSys = nullptr);
	protected:
		SYS_Thread<COMMU_DBUF_THREAD>	rxThread;
		SYS_Thread<COMMU_DBUF_THREAD>	txThread;
	private:
		virtual	int32	RxThreadFun			(void *p);
		virtual	int32	TxThreadFun			(void *p);
		virtual	int32	SendToDevice		(uint32 *retNum,const uint8 *buffer,uint32 length)	{*retNum = 0;return 0;};
		virtual	int32	ReadFromDevice		(uint32 *retNum,uint8 *buffer,uint32 length)		{*retNum = 0;return 0;};
	protected:
		virtual	FIFO8	*ReceiveFifo		(void)												{return(&cgRxSBUF.cgFIFO);};
		virtual	void	AfterReadFromDevice	(void){;};
};
//------------------------------------------------------------------------------------------//
#ifdef Commu_SSLH
enum{
	MESG_REQ_Close = MESG_NEXT,
	MESG_ANS_Close,
	MESG_NEXT_SSL,
};
//------------------------------------------------------------------------------------------//
class COMMU_DBUF_SSL : public COMMU_DBUF_THREAD{
	public:
		enum	{RFLAG_C = 2, RFLAG_S = COMMU_DBUF_THREAD::RFLAG_S + COMMU_DBUF_THREAD::RFLAG_C};
	private:
		enum	{cds_blUseSSL = RFLAG_CREATE(0),cds_blClose = RFLAG_CREATE(1),};
	public:
				 COMMU_DBUF_SSL():COMMU_DBUF_THREAD(){SetSelfName("COMMU_DBUF_SSL");};
				 COMMU_DBUF_SSL(uint32 tSize,const ODEV_SYSTEM *logSys = nullptr) : COMMU_DBUF_THREAD(){
					 Init(tSize,logSys);
					 Init(CCT_AES128,CCT_AES_CBC,CCT_MD5,G_ENDIAN_LITTLE);
					 SetSelfName("COMMU_DBUF_SSL");
				 };
		virtual	~COMMU_DBUF_SSL(void){;};
	private:
				void	Init	(CCT_AES_KEYL type,CCT_AES_MODE mode,CCT_Digest digestType,G_ENDIAN tEV);
	protected:
		inline	void	Init	(uint32 tSize,const ODEV_SYSTEM *logSys = nullptr)	{COMMU_DBUF_THREAD::Init(tSize, logSys);Init(CCT_AES128,CCT_AES_CBC,CCT_MD5,G_ENDIAN_LITTLE);};
	public:
		inline	virtual void	SetSelfName	(const STDSTR &strName)					{selfName = strName;cgSSLT2.SetSelfName(strName);};
						void	SetConfig	(CCT_AES_KEYL type = CCT_AES128,CCT_AES_MODE mode = CCT_AES_CBC,CCT_Digest digestType = CCT_MD5,G_ENDIAN tEV = G_ENDIAN_LITTLE);
	private:
		CSSL_FR_T2		cgSSLT2;
		SYS_Lock		cgSendCHMesgLock;
	protected:
		virtual	STDSTR	GetMesgText			(uint32 mID);
	protected:
		virtual	void	Clean				(void){COMMU_DBUF_THREAD::Clean();cgSSLT2.Clean();};
		virtual int32	ThreadsStart		(void);
		virtual	void	DoClose				(void);
		virtual	void	CloseDev			(void);
		virtual	FIFO8	*ReceiveFifo		(void);
		virtual	void	AfterReadFromDevice	(void);
	public:
		virtual	int32	MessageProcessing	(const uint32 &mID,const STDSTR &strMesg);
		void			SSLEnable		(void);
		void			SSLDisable		(void);
		inline void		SetNoBlock		(void){cgSSLT2.SetNoBlock();};
		inline void		SetBlock		(void){cgSSLT2.SetBlock();};
	public:
				int32	SendCHMesg		(uint32 mID,const STDSTR &strMesg,const uint64 &flagY,const uint64 &flagN,const uint32 &waitTimeS);
				int32	CheckFlagStatus	(const uint64 &flagY,const uint64 &flagN,SYS_TIME_S &timedly);
				int32	SendREQ_Close	(void);
	
		inline	uint32	CtrlCHWrite		(uint32 mesgID,const PROTOCOL_NODE &pnNode)					{return(cgSSLT2.CtrlCHWrite(mesgID,pnNode));};
		inline	uint32	CtrlCHWrite		(uint32 mesgID,const FIFO8 &fifoIn,uint32 num,uint32 offset){return(cgSSLT2.CtrlCHWrite(mesgID,fifoIn,num,offset));};
		inline	uint32	CtrlCHWrite		(uint32 mesgID,const uint8 *data,uint32 num)				{return(cgSSLT2.CtrlCHWrite(mesgID,data,num));};
		inline	uint32	CtrlCHWrite		(uint32 mesgID,const STDSTR &strData)						{return(cgSSLT2.CtrlCHWrite(mesgID,strData));};
	
		inline	uint32	DataCHWrite		(const PROTOCOL_NODE &pnNode)				{return(cgSSLT2.DataCHWrite(pnNode));};
		inline	uint32	DataCHWrite		(const FIFO8 &fifo,uint32 num,uint32 offset){return(cgSSLT2.DataCHWrite(fifo,num,offset));};
		inline	uint32	DataCHWrite		(const uint8 *data,uint32 num)				{return(cgSSLT2.DataCHWrite(data,num));};
		inline	uint32	DataCHWrite		(const uint8 data)							{return(cgSSLT2.DataCHWrite(data));};
		inline	uint32	DataCHWrite		(const STDSTR &strData)						{return(cgSSLT2.DataCHWrite(strData));};
		inline	uint32	DataCHWriteInHex(const STDSTR &strData)						{return(cgSSLT2.DataCHWriteInHex(strData));};
	public:
		virtual	uint32	Send			(const PROTOCOL_NODE &pnIn);
		virtual	uint32	Send			(const FIFO8 &fifoIn,uint32 num,uint32 offset);
		virtual	uint32	Send			(const uint8 *data,uint32 num);
		virtual	uint32	Send			(const uint8 data);
		virtual	uint32	Send			(const STDSTR &strIn,G_ESCAPE blEscape);
		virtual	uint32	SendInHEX		(const STDSTR &strIn);
	protected:
		inline void		SetblUseSSL			(void){SetSFlag(cds_blUseSSL);};
		inline void		ClrblUseSSL			(void){ClrSFlag(cds_blUseSSL);};
		inline int32	SetblNoSendCloseMesg(void)const	{return(CheckSFlag(cds_blClose));};
};
//------------------------------------------------------------------------------------------//
#define CHSend(_mID,_strMesg,_blRet,_Title) \
	E2Log(this << _Title << "Send " << GetMesgText(_mID));\
	_blRet = CtrlCHWrite(_mID,_strMesg);\
	E2Log(this << _Title << "Send " << GetMesgText(_mID) << ((_blRet > 0) ? "successful" : "fail"));
#define CHRec(_mID,_Title) E2Log(this << _Title << "Rec  " << GetMesgText(_mID));
//------------------------------------------------------------------------------------------//
class COMMU_DBUF : public COMMU_DBUF_SSL{
	public:
				 COMMU_DBUF():COMMU_DBUF_SSL(){;};
	COMMU_DBUF(uint32 tSize,const ODEV_SYSTEM *logSys = nullptr) : COMMU_DBUF_SSL(){Init(tSize,logSys);SetSelfName("COMMU_DBUF");};
		virtual	~COMMU_DBUF(void){;};
	protected:
		void	Init	(uint32 tSize,const ODEV_SYSTEM *logSys = nullptr){COMMU_DBUF_SSL::Init(tSize, logSys);SetGetDataByShare();};
	private:
		virtual	int32	LogThreadFun	(void *p);
	protected:
		virtual	void	PrintRecData	(const STDSTR &strData, uint32 byteNum, int32 blEnRecMsg);
};
//------------------------------------------------------------------------------------------//
#else
//------------------------------------------------------------------------------------------//
class COMMU_DBUF : public COMMU_DBUF_THREAD{
	public:
				 COMMU_DBUF():COMMU_DBUF_THREAD(){;};
				 COMMU_DBUF(uint32 tSize,const ODEV_SYSTEM *logSys = nullptr) : COMMU_DBUF_THREAD(){Init(tSize,logSys);SetSelfName("COMMU_DBUF");};
		virtual	~COMMU_DBUF(void){;};
	protected:
		void	Init	(uint32 tSize,const ODEV_SYSTEM *logSys = nullptr){COMMU_DBUF_THREAD::Init(tSize, logSys);SetGetDataByShare();};
	private:
		virtual	int32	LogThreadFun	(void *p);
	protected:
		virtual	void	PrintRecData	(const STDSTR &strData, uint32 byteNum, int32 blEnRecMsg);
};
#endif
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
		STDSTR		MakeByteNum			(uint32 ByteNum, uint64 tTotal, uint32 blCR);
		STDSTR		MakeTitle			(const STDSTR &strTitle, uint32 ByteNum, uint64 tTotal, uint32 blCR);

inline	STDSTR		MakeTxTitle			(uint32 ByteNum, uint64 tTotal, uint32 blCR = 0) {return(MakeTitle(" Send     ", ByteNum, tTotal,blCR));};
inline	STDSTR		MakeRxTitle			(uint32 ByteNum, uint64 tTotal, uint32 blCR = 1) {return(MakeTitle(" Received ", ByteNum, tTotal,blCR));};
inline	STDSTR		MakeEchoTitle		(uint32 ByteNum, uint64 tTotal, uint32 blCR = 1) {return(MakeTitle(" Echo     ", ByteNum, tTotal,blCR));};

		STDSTR		MakeTxTitle			(const STDSTR &strM, const STDSTR &strS, uint32 ByteNum, uint64 tTotal, uint32 blCR = 1);
		STDSTR		MakeRxTitle			(const STDSTR &strM, const STDSTR &strS, uint32 ByteNum, uint64 tTotal, uint32 blCR = 1);
		STDSTR		MakeEchoTitle		(const STDSTR &strM, uint32 ByteNum, uint64 tTotal, uint32 blCR = 1);
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif
#endif
#endif

