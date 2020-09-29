//
//  Commu_Base.h
//  SDT
//
//  Created by Leif Wen on 02/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

//------------------------------------------------------------------------------------------//
#include "ADS_Buffer.h"
#include "DS_Tree.h"
#include "SYS_Thread.h"
#if defined DS_Tree_h && defined SYS_Thread_h && defined ADS_Buffer_h
//------------------------------------------------------------------------------------------//
#ifndef Commu_Base_h
#define Commu_Base_h
#ifdef Commu_Base_h
#include "SYS_Time.h"
#include "ODEV_BUS.h"
#include "ADS_SSL.h"
#ifndef ODEV_BUS_h
#define ODEV_VG3D		void
#define	ODEV_FILE		void
#define	OUTPUT_NODE 	void
#define	ODEV_STDOUT 	void
#define	OUTPUT_CACHE 	void
#define	ODEV_BUS 		void
#endif
//------------------------------------------------------------------------------------------//
class DRV_NODE : public TNFP{
	protected:
		enum	{RFLAG_C = 1, RFLAG_S = TNFP::RFLAG_S + TNFP::RFLAG_C};
	private:
		enum	{CN_blSelected = RFLAG_CREATE(0)};
	public:
		enum CoupleType	{CT_None = 0,CT_M,CT_S};
	public:
				 DRV_NODE(void);
		virtual	~DRV_NODE(void){;};
	private:
		CoupleType		cgCType;
		DRV_NODE*		cgCouple;
		DRV_NODE*		cgSelDrv;
		DS_RWLock		cgCLock;
	public:
		static	inline 	CoupleType	GetCType			(const DRV_NODE* cdrv);
		static	inline 	DRV_NODE*	GetCouple			(const DRV_NODE* cdrv);
		static	inline 	DRV_NODE*	AcquireCouple		(DRV_NODE* cdrv);
		static	inline 	void		ReleaseCouple		(DRV_NODE* cdrv);
		static			void		LinkCouple			(DRV_NODE* master,DRV_NODE* slave);
		static			void		LinkCouple_nolock	(DRV_NODE* master,DRV_NODE* slave);
		static			DRV_NODE*	UnlinkCouple		(DRV_NODE* cdrv);
	public:
		static	inline	DRV_NODE*	GetFather			(const DRV_NODE* cdrv);
		static	inline 	DRV_NODE*	AcquireSelected		(DRV_NODE* cdrv);
		static	inline 	void		ReleaseSelected		(DRV_NODE* cdrv);

		static	inline	void		SetblSelected		(DRV_NODE* cdrv);
		static	inline	void		ClrblSelected		(DRV_NODE* cdrv);
		static	inline	bool32		IsSelected			(const DRV_NODE* cdrv);
		
				inline	DRV_NODE*	GetNewChild			(void);
				inline	DRV_NODE*	AppendChild			(DRV_NODE* cdrv);
				inline	void		JoinTo				(DRV_NODE* cdrv);
	public:
				virtual	void		SetChildSelected	(DRV_NODE* cdrv);
				virtual	void		ClrChildSelected	(DRV_NODE* cdrv);
		inline			void		ClrSelChild			(void);
		inline	virtual	void		RemoveSelf			(void);
};
typedef DRV_NODE::CoupleType CoupleType;
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
class COMMU_DRV;
class COMMU_TEAM;
//------------------------------------------------------------------------------------------//
class COMMU_UNIT : public TNFP{
	public:
				 COMMU_UNIT(void);
		virtual ~COMMU_UNIT(void){;};
	public:
		COMMU_TEAM*		unitTeam;
	public:
				virtual	void	Init			(const COMMU_TEAM* _team);
				virtual	bool32	Start			(void);
		static			bool32	StartUnit		(COMMU_UNIT* _unit);
				virtual	void	Stop			(void);
		static			void	StopUnit		(COMMU_UNIT* _unit);
};
//------------------------------------------------------------------------------------------//
class COMMU_ODEVBUS : public COMMU_UNIT{
	public:
				 COMMU_ODEVBUS(void);
		virtual	~COMMU_ODEVBUS(void);
	private:
		ODEV_BUS*		cgODevBUS;
		ODEV_VG3D*		cgVG3D;
		OUTPUT_NODE*	cgG2Self;
	public:
		static			void			InitBus				(COMMU_ODEVBUS* _bus,const ODEV_BUS* odevBus);
		static			ODEV_VG3D*		CreateODev_VG3D		(COMMU_ODEVBUS* _bus);
		static			ODEV_FILE*		CreateODev_G3File	(COMMU_ODEVBUS* _bus,const STDSTR& fName);
		static			OUTPUT_NODE*	CreateODev_G2Self	(COMMU_ODEVBUS* _bus,COMMU_DRV* cdrv,OUTPUT_CACHE* cache);
		static			OUTPUT_NODE*	AddG3D_Self			(COMMU_ODEVBUS* _bus,OUTPUT_NODE* oG3D);
		static			ODEV_STDOUT*	AddG3D_STDOUT		(COMMU_ODEVBUS* _bus,ODEV_STDOUT* oG3D);
	public:
		static	inline	ODEV_BUS*		GetODevBus			(COMMU_ODEVBUS* _bus);
		static	inline	ODEV_VG3D*		GetVG3D				(COMMU_ODEVBUS* _bus);
		static			OUTPUT_NODE*	GetG2DefSelf		(COMMU_ODEVBUS* _bus);
		static			OUTPUT_NODE*	GetG3DefSelf		(COMMU_ODEVBUS* _bus);
		static			ODEV_STDOUT*	GetG3DefSTDOUT		(COMMU_ODEVBUS* _bus);
		static			ODEV_FILE*		GetG3DefFile		(COMMU_ODEVBUS* _bus);
	public:
		template<typename... Args>static inline void PrintStr				(COMMU_ODEVBUS* _bus,const Args&... args);
		template<typename... Args>static inline void PrintStrNL				(COMMU_ODEVBUS* _bus,const Args&... args);
		template<typename... Args>static inline void PrintALine				(COMMU_ODEVBUS* _bus,const Args&... args);
		template<typename... Args>static inline void PrintWithTime			(COMMU_ODEVBUS* _bus,const Args&... args);
		template<typename... Args>static inline void PrintWithDividingLine	(COMMU_ODEVBUS* _bus,const Args&... args);
		template<typename... Args>static inline void PrintMessageDot		(COMMU_ODEVBUS* _bus,const Args&... args);
		template<typename... Args>static inline void PrintWarningMessageDot	(COMMU_ODEVBUS* _bus,const Args&... args);
		template<typename... Args>static inline void PrintConnectionReport	(COMMU_ODEVBUS* _bus
																			 ,const uint64& rxBytes
																			 ,const uint64& txBytes
																			 ,const uint64& dpBytes
																			 ,const Args&... args);
};
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
static inline OPEN_PAR& SetOpenPar(OPEN_PAR* op,uint32 type,const STDSTR& name,uint32 port,uint32 cfg);
static inline OPEN_PAR	SetOpenPar(				uint32 type,const STDSTR& name,uint32 port,uint32 cfg);
static inline OPEN_PAR& SetOpenPar(OPEN_PAR* op1,const OPEN_PAR& op2);
//------------------------------------------------------------------------------------------//
class COMMU_CORE : public COMMU_UNIT{
	protected:
		enum	{RFLAG_C = 1, RFLAG_S = COMMU_UNIT::RFLAG_S + COMMU_UNIT::RFLAG_C};
		enum	{CF_blConnected = RFLAG_CREATE(0)};
	public:
				 COMMU_CORE(void);
		virtual ~COMMU_CORE(void);
	protected:
		STDSTR			cgDevName;
		OPEN_PAR		cgOpenPar;
	public:
		inline	virtual	void		SetSelfName				(const STDSTR& strName);
		inline	virtual	void		SetUpName				(const STDSTR& strName);
	public:
		inline	virtual	bool32		OpenDev					(const OPEN_PAR& par);
		inline	virtual	void		CloseDev				(void);
	public:
		inline	virtual	bool32		SendToDevice			(uint32* retNum,const uint8* buffer,uint32 length);
		inline	virtual	bool32		ReadFromDevice			(uint32* retNum,	  uint8* buffer,uint32 length);
	public:
		inline	virtual void		PrintOpenSuccess		(const STDSTR& strTitle = "");
		inline	virtual void		PrintOpenFail			(const STDSTR& strTitle = "");
		inline	virtual void		PrintClose				(const uint64& rxBytes,const uint64& txBytes,const uint64& dpBytes);
	public:
		inline	const	STDSTR&		GetDevName				(void)const;
		inline	const	OPEN_PAR&	GetOpenPar				(void)const;
		inline			void		SetDevName				(const OPEN_PAR& par);
		inline			void		SetTypePar				(uint32 type);
		inline			void		SetCfgPar				(uint32 cfg);
		inline			bool32		IsConnected				(void)const;
						STDSTR		GetCoreName				(void)const;
};
//------------------------------------------------------------------------------------------//
class COMMU_BRIDGE : public COMMU_UNIT{
	protected:
		enum	{RFLAG_C = 1, RFLAG_S = COMMU_UNIT::RFLAG_S + COMMU_UNIT::RFLAG_C};
		enum	{CF_blPushSend = RFLAG_CREATE(0)};
	public:
				 COMMU_BRIDGE(void);
		virtual	~COMMU_BRIDGE(void);
	public:
		static	inline	void	InitArray					(COMMU_BRIDGE* bridge,const ARRAY* _rx, const ARRAY* _tx);
		inline	virtual	void	Init						(const COMMU_TEAM* _team);
		inline	virtual	void	SetSelfName					(const STDSTR& strName);
		inline	virtual	void	SetUpName					(const STDSTR& strName);
	protected:
		ARRAY*			cgArrayRx;
		ARRAY*			cgArrayTx;
		uint64			cgRxBytes;
		uint64			cgTxBytes;
	protected:
		SYS_Thread<COMMU_BRIDGE>	rxThread;
		SYS_Thread<COMMU_BRIDGE>	txThread;
	private:
				virtual	bool32		RxThreadFunc			(void* _team);
				virtual	bool32		TxThreadFunc			(void* _team);
	public:
		static	inline	void		Empty					(COMMU_BRIDGE* bridge);
		static	inline	uint32		RxMaxSize				(COMMU_BRIDGE* bridge);
		static	inline	uint32		TxMaxSize				(COMMU_BRIDGE* bridge);
		static	inline	uint32		UnreadBytes				(COMMU_BRIDGE* bridge);
		static	inline	uint32		UnsentBytes				(COMMU_BRIDGE* bridge);
		static	inline	uint64		RxBytes					(COMMU_BRIDGE* bridge);
		static	inline	uint64		TxBytes					(COMMU_BRIDGE* bridge);
		static	inline	ARRAY*		GetArrayRx				(COMMU_BRIDGE* bridge);
		static	inline	ARRAY*		GetArrayTx				(COMMU_BRIDGE* bridge);
		static	inline	void		EnableRxThread			(COMMU_BRIDGE* bridge);
		static	inline	void		DisableRxThread			(COMMU_BRIDGE* bridge);

		static			void		PushSend				(COMMU_BRIDGE* bridge,uint32 timeMS);
};
//------------------------------------------------------------------------------------------//
class COMMU_MEM : public COMMU_UNIT{
	public:
				 COMMU_MEM(void);
		virtual	~COMMU_MEM(void){;};
	public:
		static	inline	void		InitArray				(COMMU_MEM* mem,uint32 rxSize,uint32 txSize);
		inline	virtual	void		InitArraySize			(uint32 rxSize,uint32 txSize);
		inline	virtual	void		Init					(const COMMU_TEAM* _team);
		inline	virtual	void		SetSelfName				(const STDSTR& strName);
		inline	virtual	void		SetUpName				(const STDSTR& strName);
		inline	virtual	void		AfterReadFromDevice		(void){;};
	protected:
		ARRAY	arrayTx;
		ARRAY	arrayRx;
	public:
		static	inline	void		Empty					(COMMU_MEM* mem);
		static	inline	ARRAY*		GetArrayRx				(COMMU_MEM* mem);
		static	inline	ARRAY*		GetArrayTx				(COMMU_MEM* mem);
		inline	virtual	void		EmptyArray				(void);
		inline	virtual	ARRAY*		GetArrayRx_original		(void);
		inline	virtual	ARRAY*		GetArrayTx_original		(void);
				virtual	uint32		Send					(const ABORTCHECK& _ac,const UVIn&  _in,uint32 timeoutMS);
				virtual	uint32		Read					(const ABORTCHECK& _ac,const UVOut& _out,uint32 num);
};
//------------------------------------------------------------------------------------------//
class COMMU_DISPOSE : public COMMU_UNIT{
	public:
				 COMMU_DISPOSE(void);
		virtual	~COMMU_DISPOSE(void);
	public:
		inline	virtual	void			Init				(const COMMU_TEAM* _team);
		inline	virtual	void			SetSelfName			(const STDSTR& strName);
		inline	virtual	void			SetUpName			(const STDSTR& strName);
	protected:
		DS_RWLock						rwlock;
		COMMU_DISPOSE*					cgDPList;
		SYS_Thread<COMMU_DISPOSE>		dpThread;
		uint64							cgDPBytes;
				virtual	bool32			DPThreadFunc		(void* _team);
	public:
		static	inline	void			Empty				(COMMU_DISPOSE* dispose);
		static	inline	uint64			DPBytes				(COMMU_DISPOSE* dispose);
		static	inline	COMMU_DISPOSE*	GetDPList			(COMMU_DISPOSE* dispose);
		static	inline	void			UseSelfDPList		(COMMU_DISPOSE* dispose);
		static	inline	void			UseExternalDPList	(COMMU_DISPOSE* dispose,const COMMU_DISPOSE* dpList);
		static	inline	void			ShareTo				(COMMU_DISPOSE* dispose,SBUF* tSBUF);
		static	inline	void			Enable				(COMMU_DISPOSE* dispose,bool32	blEnable);
};
//------------------------------------------------------------------------------------------//
class COMMU_RECORD : public COMMU_UNIT{
		enum	{RECBUFFER_SIZE = 1024 * 2};
	public:
				 COMMU_RECORD(void);
		virtual	~COMMU_RECORD(void);
	public:
		inline	virtual	void			Init				(const COMMU_TEAM* _team);
		inline	virtual	void			SetSelfName			(const STDSTR& strName);
		inline	virtual	void			SetUpName			(const STDSTR& strName);
	private:
		SBUF							cgRecSBUF;
		SYS_Thread<COMMU_RECORD>		recThread;
				virtual	bool32			RecThreadFunc		(void* _team);
	public:
		static	inline	void			Enable				(COMMU_RECORD* record,bool32 blEnable);
};
//------------------------------------------------------------------------------------------//
class COMMU_TEAM : public TNFP{
	public:
				 COMMU_TEAM(void);
		virtual ~COMMU_TEAM(void);
	public:
				void		Init			(void);
				void		SetSelfName		(const STDSTR& strName);
				void		SetUpName		(const STDSTR& strName);
	public:
		template<typename T_UNIT>static	T_UNIT*		Create_unit		(T_UNIT* _unit);
		template<typename T_UNIT>static T_UNIT*		Create_core		(COMMU_TEAM* _team,T_UNIT* _unit);
		template<typename T_UNIT>static T_UNIT*		Create_birdge	(COMMU_TEAM* _team,T_UNIT* _unit);
		template<typename T_UNIT>static T_UNIT*		Create_mem		(COMMU_TEAM* _team,T_UNIT* _unit);
		template<typename T_UNIT>static T_UNIT*		Create_odecBus	(COMMU_TEAM* _team,T_UNIT* _unit);
		template<typename T_UNIT>static T_UNIT*		Create_dispose	(COMMU_TEAM* _team,T_UNIT* _unit);
		template<typename T_UNIT>static T_UNIT*		Create_record	(COMMU_TEAM* _team,T_UNIT* _unit);
	public:
		COMMU_DRV*			cdrv;
		COMMU_CORE*			core;
		COMMU_MEM*			mem;
		COMMU_BRIDGE*		bridge;
		COMMU_DISPOSE*		dispose;
		COMMU_ODEVBUS*		odevBus;
		COMMU_RECORD*		record;
};
//------------------------------------------------------------------------------------------//
class _COMMU_DRV : public DRV_NODE{
	protected:
		enum	{PACKAGE_MAX_SIZE = 1024 * 8};
		enum	{RFLAG_C = 4, RFLAG_S = DRV_NODE::RFLAG_S + DRV_NODE::RFLAG_C};
		enum	{CFG_C = 1, CFG_S = 0};
		enum	{CF_blOpened			= RFLAG_CREATE(0)
				,CF_blEnableSelfClose	= RFLAG_CREATE(1)
				,CF_blCloseDueToOS		= RFLAG_CREATE(2)
				,CF_blEnCloseCouple		= RFLAG_CREATE(3)};
	public:
		enum	{CFG_blEnableEcho = CGF_CREATE(0),CFG_blDisableEcho = 0};
	public:
				 _COMMU_DRV(void) : DRV_NODE(){;};
		virtual	~_COMMU_DRV(void){;};
	public:
		COMMU_TEAM						unitTeam;
		DS_SpinLock						cgOpenLock;
	protected:
		uint32							cgMaxRxArraySize;
		uint32							cgMaxTxArraySize;
		SYS_Thread_List					cgThreadList;
	public:
		inline			bool32			IsOpened			(void)const;
		inline			bool32			IsEnableEcho		(void)const;
		inline			bool32			IsCloseDueToOS		(void)const;
		inline			void			SetblCloseCouple	(void);
		inline	const	uint32&			RxArrayMaxSize		(void)const;
		inline	const	uint32&			TxArrayMaxSize		(void)const;
	public:
						COMMU_DRV*		Find				(const STDSTR& name,uint32 port);
		inline			SYS_AThread*	ThreadGet			(const STDSTR& name);
		inline			void			ThreadAccept		(SYS_AThread * _thread);
		inline			void			RxDataShareTo		(SBUF* tSBUF);
		inline			COMMU_CORE*		Core				(void)const;
};
//------------------------------------------------------------------------------------------//
class COMMU_DRV : public _COMMU_DRV{
	public:
				 COMMU_DRV(void);
				 COMMU_DRV(uint32 rxSize,uint32 txSize,const ODEV_BUS* odevBus);
		virtual	~COMMU_DRV(void);
	protected:
		SYS_Thread<COMMU_DRV>		selfCloseThread;
	public:
						void		Init				(uint32 rxSize,uint32 txSize,const ODEV_BUS* odevBus);
				virtual	void		SetSelfName			(const STDSTR& strName);
				virtual	void		SetUpName			(const STDSTR& strName);
	public:
				virtual	void		SetChildSelected	(DRV_NODE* cdrv);
				virtual	void		ClrChildSelected	(DRV_NODE* cdrv);
				virtual	void		EnableEcho			(void);
				virtual	void		DisableEcho			(void);
				virtual	void		Empty				(void);
				virtual	uint64		RxBytes				(void);
				virtual	uint64		TxBytes				(void);
				virtual	uint64		DPBytes				(void);
				virtual	uint32		RxMaxSize			(void);
				virtual	uint32		TxMaxSize			(void);
				virtual	uint32		UnreadBytes			(void);
				virtual	uint32		UnsentBytes			(void);
	protected:
				virtual	bool32		SelfCloseThreadFunc	(void* cdrv);
				virtual	bool32		DoOpen				(const OPEN_PAR& par);
				virtual	void		DoClose				(void);
				virtual	void		DoThreadsStart		(void);
				virtual	void		ThreadsStart		(void);
	public:
				virtual	uint32		Send				(const UVIn&  _in,uint32 timeoutMS);
				virtual	uint32		Read				(const UVOut& _out,uint32 num);
	public:
				virtual	void		CloseChild			(COMMU_DRV* cdrv);
				virtual	void		CloseAllChild		(void);
	
						bool32		CloseSelf			(uint32 timeoutMS);
						void		Close				(void);
						bool32		Open				(const OPEN_PAR& par);
};
//------------------------------------------------------------------------------------------//
class COMMU_DRV_NOSMC : public COMMU_DRV{
	public:
				 COMMU_DRV_NOSMC(void) : COMMU_DRV(){;};
				 COMMU_DRV_NOSMC(uint32 rxSize,uint32 txSize,const ODEV_BUS* odevBus);
		virtual ~COMMU_DRV_NOSMC(void){;};
	public:
		virtual void	DoThreadsStart	(void);
};
//------------------------------------------------------------------------------------------//
template <typename T_CDRV>
class COMMU_NOSMC_T : public T_CDRV{
	public:
				 COMMU_NOSMC_T(void) : T_CDRV(){;};
				 COMMU_NOSMC_T(uint32 rxSize,uint32 txSize,const ODEV_BUS* odevBus);
		virtual ~COMMU_NOSMC_T(void){;};
	public:
		virtual void	DoThreadsStart	(void);
};
//------------------------------------------------------------------------------------------//
enum{TDEFALL=0xff,TMEM = 0x1,TBIRDGE = 0x1 << 1,TDISPOSE = 0x1 << 2, TODEVBUS = 0x1 << 3,TRECORD = 0x1 << 4};
template <
	uint32	 T_FLAG,
	typename T_CDRV,
	typename T_CORE,
	typename T_MEM		= COMMU_MEM,
	typename T_BIRDGE	= COMMU_BRIDGE,
	typename T_DISPOSE	= COMMU_DISPOSE,
	typename T_ODEVBUS	= COMMU_ODEVBUS,
	typename T_RECORD	= COMMU_RECORD
>
class COMMU_DRV_T : public T_CDRV{
	public:
			 COMMU_DRV_T(void) : T_CDRV(){
				 this->unitTeam.cdrv = this;
				 COMMU_TEAM::Create_core	(&this->unitTeam,(T_CORE*)G_TRUE);
				 COMMU_TEAM::Create_mem		(&this->unitTeam,(T_MEM*)((T_FLAG & TMEM) > 0));
				 COMMU_TEAM::Create_birdge	(&this->unitTeam,(T_BIRDGE*)((T_FLAG & TBIRDGE) > 0));
				 COMMU_TEAM::Create_dispose	(&this->unitTeam,(T_DISPOSE*)((T_FLAG & TDISPOSE) > 0));
				 COMMU_TEAM::Create_odecBus	(&this->unitTeam,(T_ODEVBUS*)((T_FLAG & TODEVBUS) > 0));
				 COMMU_TEAM::Create_record	(&this->unitTeam,(T_RECORD*)((T_FLAG & TRECORD) > 0));
				 this->unitTeam.cdrv->TNFP::SetSelfName(this->unitTeam.core->GetCoreName());
				 this->unitTeam.core->SetSelfName("CORE");
			 };
			 COMMU_DRV_T(uint32 rxSize,uint32 txSize,const ODEV_BUS* odevBus) : COMMU_DRV_T(){
				 this->Init(rxSize,txSize,odevBus);
			 };
		virtual	~COMMU_DRV_T(void){;};
	public:
};
//------------------------------------------------------------------------------------------//
template <typename T_CDRV,typename T_CDEV> class CDRV_POOL_T : public T_CDRV{
	public:
				 CDRV_POOL_T(void);
				 CDRV_POOL_T(uint32 rxSize,uint32 txSize,const ODEV_BUS* odevBus);
		virtual ~CDRV_POOL_T(void){;};
	private:
		virtual	TNF*		CreateNode			(void);
	public:
		virtual	uint32		Send				(const UVIn&  _in,uint32 timeoutMS);
		virtual	uint32		Read				(const UVOut& _out,uint32 num);
		virtual	void		Empty				(void);
		virtual	uint64		RxBytes				(void);
		virtual	uint64		TxBytes				(void);
		virtual	uint64		DPBytes				(void);
		virtual	uint32		RxMaxSize			(void);
		virtual	uint32		TxMaxSize			(void);
		virtual	uint32		UnreadBytes			(void);
		virtual	uint32		UnsentBytes			(void);
				T_CDEV*		Attach				(uint32 index);
};
//------------------------------------------------------------------------------------------//
template <typename T_COMMU_A,typename T_COMMU_B> class COMMU_CPPOOL : public TNFP{
	public:
				 COMMU_CPPOOL(void);
		virtual ~COMMU_CPPOOL(void);
	public:
				void	Init			(uint32 rxSize,uint32 txSize);
		virtual void	SetSelfName		(const STDSTR& strName);
		virtual	void	SetUpName		(const STDSTR& strName);
	public:
		CDRV_POOL_T<COMMU_DRV_T<0,COMMU_DRV,COMMU_CORE>,T_COMMU_A>	cgPoolA;
		CDRV_POOL_T<COMMU_DRV_T<0,COMMU_DRV,COMMU_CORE>,T_COMMU_B>	cgPoolB;
	public:
		bool32	GetCP			(T_COMMU_A** cmmuA,T_COMMU_B** cmmuB);
		void	CloseChild		(T_COMMU_A*  cmmuA,T_COMMU_B*  cmmuB);
		void	Close			(void);
};
//------------------------------------------------------------------------------------------//
bool32	CommuIsClosed		(void* _team);
STDSTR	MakeTxTitle			(										uint32 ByteNum, uint64 total, bool32 blCR = G_FALSE);
STDSTR	MakeRxTitle			(										uint32 ByteNum, uint64 total, bool32 blCR = G_TRUE);
STDSTR	MakeEchoTitle		(										uint32 ByteNum, uint64 total, bool32 blCR = G_TRUE);
STDSTR	MakeTxTitle			(const STDSTR& strM, const STDSTR& strS,uint32 ByteNum, uint64 total, bool32 blCR = G_TRUE);
STDSTR	MakeRxTitle			(const STDSTR& strM, const STDSTR& strS,uint32 ByteNum, uint64 total, bool32 blCR = G_TRUE);
STDSTR	MakeEchoTitle		(const STDSTR& strM, 					uint32 ByteNum, uint64 total, bool32 blCR = G_TRUE);
//------------------------------------------------------------------------------------------//
#include "Commu_Base.hpp"
#endif /* Commu_Base_h */
#endif /* Commu_Base_h */
#endif /* ADS_Buffer_h */
