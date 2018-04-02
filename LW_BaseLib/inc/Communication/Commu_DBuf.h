//
//  Commu_DBuf.h
//  SDT
//
//  Created by Leif Wen on 02/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "ADS_Buffer.h"
#ifdef ADS_Buffer_h
//------------------------------------------------------------------------------------------//
#ifndef Commu_DBuf_h
#define Commu_DBuf_h
#ifdef Commu_DBuf_h
#include "SYS_Thread.h"
#include "SYS_Time.h"
#include "ODEV_System.h"
#include "ADS_SSL.h"
#include "MSG_Center.h"
#include "MSG_Handshake.h"
//------------------------------------------------------------------------------------------//
enum {
	OPEN_None = 0,
	OPEN_COM,
	OPEN_COMV,
	OPEN_TCP,
	OPEN_UDP,
	OPEN_TCPS,
	OPEN_UDPS,
};
struct OPEN_PAR{
	uint32		type;
	STDSTR		name;
	uint32		port;
	uint32		cfg;
};
//------------------------------------------------------------------------------------------//
#define		COL_DB_TxTitle		COL_clPurple
#define 	COL_DB_TxText		COL_clBlack
#define 	COL_DB_RxTitle		COL_clRed
#define 	COL_DB_RxText		COL_clBlue
#define 	COL_DB_RxTitleM		COL_DB_TxTitle
#define 	COL_DB_RxTextM		COL_DB_TxText
#define 	COL_DB_RxTitleS		COL_DB_RxTitle
#define 	COL_DB_RxTextS		COL_DB_RxText
#define 	COL_DB_Statistics	COL_clRed
//------------------------------------------------------------------------------------------//
static inline OPEN_PAR& SetOpenPar(OPEN_PAR* op,uint32 type,const STDSTR& name,uint32 port,uint32 cfg);
static inline OPEN_PAR	SetOpenPar(				uint32 type,const STDSTR& name,uint32 port,uint32 cfg);
static inline OPEN_PAR& SetOpenPar(OPEN_PAR* op1,const OPEN_PAR& op2);
//------------------------------------------------------------------------------------------//
class COMMU_FRAME : public DBUF{
	protected:
		typedef COMMU_FRAME COMMUF;
		enum	{PACKAGE_MAX_SIZE = 1024 * 8};
		enum	{RFLAG_C = 7, RFLAG_S = DBUF::RFLAG_S + DBUF::RFLAG_C};
		enum	{CFG_C = 1, CFG_S = 0};
		enum	{CF_blOpened			= RFLAG_CREATE(0)
				,CF_blConnected			= RFLAG_CREATE(1)
				,CF_blEnableSelfClose	= RFLAG_CREATE(2)
				,CF_blCloseDueToOS		= RFLAG_CREATE(3)
				,CF_blPushSend			= RFLAG_CREATE(4)
				,CF_blNoInitSize		= RFLAG_CREATE(5)
				,CF_blEnCloseCouple		= RFLAG_CREATE(6)
		};
	public:
		enum	{CFG_blEnableEcho = CGF_CREATE(0),CFG_blDisableEcho = 0};
	public:
				 COMMU_FRAME(uint32 size);
		virtual	~COMMU_FRAME(void);
	protected:
		OPEN_PAR			cgOpenPar;
		STDSTR				cgDevName;
		SYS_Thread_List		cgThreadList;
		SYS_Thread<COMMUF>	selfCloseThread;
	public:
		inline	virtual	void		SetSelfName			(const STDSTR& strName);
		inline	virtual	void		SetFatherName		(const STDSTR& strName);
	protected:
				virtual	bool32		SelfCloseThreadFun	(void* commu);
						void		DoSelfClose			(void);
	
				virtual	bool32		DoOpen				(const OPEN_PAR& par);
		inline	virtual	bool32		OpenDev				(const OPEN_PAR& par);
	
				virtual	void		DoClose				(void);
		inline	virtual	void		CloseDev			(void);

				virtual void		DoThreadsStart		(void);
		inline	virtual bool32		ThreadsStart		(void);
		inline	virtual void		ThreadsStop			(void);
	protected:
		inline			bool32		IsConnected			(void)const;
		inline			void		ClrblPushSend		(void);
		inline			bool32		ChkblPushSend		(void)const;
	public:
		inline			bool32		IsOpened			(void)const;
		inline	virtual	void		EnableEcho			(void);
		inline	virtual	void		DisableEcho			(void);
		inline			bool32		CheckEcho			(void)const;
		inline			void		EnableCloseCouple	(void);
	public:
		inline	const	STDSTR&		GetDevName			(void)const;
		inline	const	OPEN_PAR&	GetOpenPar			(void)const;
		inline	const	void		SetTypePar			(uint32 type);
	public:
						bool32		Open				(const OPEN_PAR& par,G_LOCK blLock = G_LOCK_ON);
						void		Close				(void);
						bool32		CloseSelf			(uint32 timeoutMS);
				virtual	void		CloseChild			(COMMU_FRAME* commu);
				virtual	void		CloseAllChild		(void);
					SYS_AThread*	GetThread			(const STDSTR& name);
	
						void		PushSend			(uint32 timeMS);
		inline	virtual	uint32		Send				(const UVIn&  _in);
		inline	virtual	uint32		Read				(const UVOut& _out,uint32 num);
						COMMUF*		Find				(const STDSTR& name,uint32 port);
};
//------------------------------------------------------------------------------------------//
class COMMU_FRAME_FW : public COMMU_FRAME{
	public:
				 COMMU_FRAME_FW(uint32 size);
		virtual	~COMMU_FRAME_FW(void);
	public:
		inline	virtual	void		SetSelfName				(const STDSTR& strName);
		inline	virtual	void		SetFatherName			(const STDSTR& strName);
	public:
		inline	virtual	DBUF*		AddChild				(DBUF* sdb);
		inline	virtual	void		RemoveSelf				(void);
	protected:
		SYS_Thread<COMMU_FRAME_FW>	fwThread;
		TNFP						cgInternalFwSBufList;
	private:
		TNFP*						rxFwSBufList;
	private:
				virtual	bool32		FwThreadFun				(void* commu);
		static			uint32		ForwardToCommu			(COMMU_FRAME_FW* commu,uint32* outNum,const ARRAY& array,uint32 inNum,bool32 blInit);
	public:
		inline	const	TNFP*		GetFwSBufList			(void);
		inline			void		UseSelfFwSBufList		(void);
		inline			void		UseExternalFwSBufList	(const TNFP* tRxFwSBufList);
		inline			void		RxDataShareTo			(SBUF* tSBUF);
		
		inline			void		SetGetDataByRead		(void);
		inline			void		SetGetDataByShare		(void);
};
//------------------------------------------------------------------------------------------//
struct ODEV_SYSTEM;
class ODEV_VG3D;
class OUTPUT_NODE;
class ODEV_STDOUT;
class ODEV_FILE;
//------------------------------------------------------------------------------------------//
class COMMU_FRAME_LOGSYS : public COMMU_FRAME_FW{
	public:
				 COMMU_FRAME_LOGSYS(uint32 size,const ODEV_SYSTEM* logSys);
		virtual	~COMMU_FRAME_LOGSYS(void);
	public:
						void		InitLogSys				(const ODEV_SYSTEM* logSys);
	protected:
				virtual	bool32		DoOpen					(const OPEN_PAR& par);
				virtual	void		DoClose					(void);
	private:
		inline	virtual	void		DoPrintOnOpenSuccess	(void);
		inline	virtual	void		DoPrintOnOpenFail		(void);
		inline	virtual	void		DoPrintOnClose			(void);
	private:
		ODEV_SYSTEM*	logSystem;
		ODEV_VG3D*		cgVG3D;
		OUTPUT_NODE*	cgG2Self;
	public:
						ODEV_VG3D*		CreateODev_VG3D		(void);
						ODEV_FILE*		CreateODev_G3File	(const STDSTR& fName);
				virtual	OUTPUT_NODE*	CreateODev_G2Self	(void);
						OUTPUT_NODE*	AddG3D_Self			(OUTPUT_NODE* oG3D);
						ODEV_STDOUT*	AddG3D_STDOUT		(ODEV_STDOUT* oG3D);
	public:
		inline			ODEV_SYSTEM*	GetLogSystem		(void)const;
		inline			ODEV_VG3D*		GetVG3D				(void)const;
						OUTPUT_NODE*	GetG2DefSelf		(void)const;
						OUTPUT_NODE*	GetG3DefSelf		(void)const;
						ODEV_STDOUT*	GetG3DefSTDOUT		(void)const;
						ODEV_FILE*		GetG3DefFile		(void)const;
	protected:
		STDSTR			cgAttrTitle;
	public:
		inline			void			SetAttrTitle		(const STDSTR& strIn);
	protected:
		template<typename... Args>inline void PrintStr				(					const Args&... args);
		template<typename... Args>inline void PrintStrNL			(					const Args&... args);
		template<typename... Args>inline void PrintALine			(					const Args&... args);
		template<typename... Args>inline void PrintWithTime			(					const Args&... args);
		template<typename... Args>inline void PrintWithDividingLine	(					const Args&... args);
		template<typename... Args>inline void PrintMessageDot		(					const Args&... args);
		template<typename... Args>inline void PrintWarningMessageDot(					const Args&... args);
		template<typename... Args>inline void PrintConnectionReport	(					const Args&... args);
		template<typename... Args>		 void PrintConnectionReport	(OUTPUT_NODE* node, const Args&... args);
	protected:
		inline			void			PrintOpenSuccessReport	(const STDSTR& strTitle = "");
		inline			void			PrintOpenFailReport		(const STDSTR& strTitle = "");
};
//------------------------------------------------------------------------------------------//
class COMMU_THREAD : public COMMU_FRAME_LOGSYS{
	public:
				 COMMU_THREAD(uint32 size,const ODEV_SYSTEM* logSys);
		virtual	~COMMU_THREAD(void);
	protected:
		SYS_Thread<COMMU_THREAD>	rxThread;
		SYS_Thread<COMMU_THREAD>	txThread;
		SYS_Thread<COMMU_THREAD>	logThread;
		ARRAY*						cgArrayOut;
		ARRAY*						cgArrayIn;
		SBUF						cgLogSBUF;
	public:
		inline	virtual	void	SetSelfName			(const STDSTR& strName);
		inline	virtual	void	SetFatherName		(const STDSTR& strName);
	private:
				virtual	bool32	RxThreadFun			(void* commu);
				virtual	bool32	TxThreadFun			(void* commu);
				virtual	bool32	LogThreadFun		(void* commu);
		inline	virtual	bool32	SendToDevice		(uint32* retNum,const uint8 *buffer,uint32 length);
		inline	virtual	bool32	ReadFromDevice		(uint32* retNum,	  uint8 *buffer,uint32 length);
	protected:
		inline	virtual	void	AfterReadFromDevice	(void){;};
				virtual	void	PrintRecData		(const STDSTR& strData, uint32 byteNum, bool32 blEnRecMsg);
	public:
		void	EnableLog		(void);
		void	DisableLog		(void);
};
//------------------------------------------------------------------------------------------//
#ifdef ADS_SSL_h
static inline OPEN_PAR	SetOpenSSL(uint32 type,const STDSTR& name,uint32 port);
//------------------------------------------------------------------------------------------//
class COMMU_SSL : public COMMU_THREAD{
	protected:
		enum	{CFG_C = 1, CFG_S = COMMU_THREAD::CFG_C + COMMU_THREAD::CFG_S};
	public:
		enum	{CFG_blEnableSSL = CGF_CREATE(1),CFG_blDisableSSL = 0};
	public:
				 COMMU_SSL(uint32 size,const ODEV_SYSTEM* logSys);
		virtual	~COMMU_SSL(void){;};
	private:
		ARRAY			cgTxArray;
		ARRAY			cgRxArray;
		CMD_ENV			cgMSGENV;
	public:
		MSG_CENTER		cgMsgCenter;
		MSG_HANDSHAKE	cgMsgHandshake;
	public:
		inline	virtual	void	SetSelfName			(const STDSTR& strName);
		inline	virtual	void	SetFatherName		(const STDSTR& strName);
				virtual	void	Empty				(void);
	protected:
				virtual	bool32	DoOpen				(const OPEN_PAR& par);
				virtual	void	DoClose				(void);

				virtual bool32	ThreadsStart		(void);
				virtual	bool32	SelfCloseThreadFun	(void* commu);
				virtual	void	AfterReadFromDevice	(void);
};
//------------------------------------------------------------------------------------------//
template <typename T_COMMU,typename T_MSG> class COMMU_MSG : public T_COMMU{
	public:
				 COMMU_MSG(uint32 size,const ODEV_SYSTEM* logSys);
		virtual	~COMMU_MSG(void){;};
	public:
		T_MSG	cgMsg;
	public:
		inline	virtual	void		SetSelfName			(const STDSTR& strName);
		inline	virtual	void		SetFatherName		(const STDSTR& strName);
						MSG_NODE*	GetMSG				(CMDID mID);
};
//------------------------------------------------------------------------------------------//
#endif
//------------------------------------------------------------------------------------------//
template <typename T_COMMU> class COMMU_NOSMC : public T_COMMU{
	public:
				 COMMU_NOSMC(uint32 size,const ODEV_SYSTEM* logSys) : T_COMMU(size,logSys){;};
		virtual ~COMMU_NOSMC(void){;};
	public:
		virtual void	DoThreadsStart	(void){this->cgThreadList.ThreadRun(this);};
};
//------------------------------------------------------------------------------------------//
template <typename T_COMMU> class COMMU_POOL : public COMMU_FRAME{
	public:
				 COMMU_POOL(uint32 size,const ODEV_SYSTEM* logSys);
		virtual ~COMMU_POOL(void){;};
	protected:
		ODEV_SYSTEM*	logSystem;
	private:
		virtual	TNFP*		CreateNode			(void);
	public:
				T_COMMU*	GetNewChild			(void);
};
//------------------------------------------------------------------------------------------//
template <typename T_COMMU_A,typename T_COMMU_B> class COMMU_CPPOOL : public TNFP{
	public:
				 COMMU_CPPOOL(uint32 size,const ODEV_SYSTEM* logSys);
		virtual ~COMMU_CPPOOL(void);
	public:
		virtual void	SetSelfName		(const STDSTR& strName);
		virtual	void	SetFatherName	(const STDSTR& strName);
	public:
		COMMU_POOL<T_COMMU_A>	cgPoolA;
		COMMU_POOL<T_COMMU_B>	cgPoolB;
	public:
		bool32	GetCP			(T_COMMU_A** cmmuA,T_COMMU_B** cmmuB);
		void	CloseChild		(T_COMMU_A*  cmmuA,T_COMMU_B*  cmmuB);
		void	Close			(void);
};
//------------------------------------------------------------------------------------------//
STDSTR	MakeTxTitle			(										uint32 ByteNum, uint64 total, bool32 blCR = G_FALSE);
STDSTR	MakeRxTitle			(										uint32 ByteNum, uint64 total, bool32 blCR = G_TRUE);
STDSTR	MakeEchoTitle		(										uint32 ByteNum, uint64 total, bool32 blCR = G_TRUE);
STDSTR	MakeTxTitle			(const STDSTR& strM, const STDSTR& strS,uint32 ByteNum, uint64 total, bool32 blCR = G_TRUE);
STDSTR	MakeRxTitle			(const STDSTR& strM, const STDSTR& strS,uint32 ByteNum, uint64 total, bool32 blCR = G_TRUE);
STDSTR	MakeEchoTitle		(const STDSTR& strM, 					uint32 ByteNum, uint64 total, bool32 blCR = G_TRUE);
//------------------------------------------------------------------------------------------//
#include "Commu_DBuf.hpp"
#endif /* Commu_DBuf_h */
#endif /* Commu_DBuf_h */
#endif /* ADS_Buffer_h */
