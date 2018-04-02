//
//  DS_Transform.h
//  SDT
//
//  Created by Leif Wen on 19/01/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

//------------------------------------------------------------------------------------------//
#include "BasicDefine.h"
#include "DS_ARRAY.h"
#include "DS_Tree.h"
#include "DS_UVar.h"
//------------------------------------------------------------------------------------------//
#if defined DS_ARRAY_h && defined DS_Tree_h && defined DS_UVar_h
#ifndef DS_Transform_h
#define DS_Transform_h
#ifdef DS_Transform_h
//------------------------------------------------------------------------------------------//
struct IOSTATUS{
	uint64	avail_in;	// number of bytes available at next_in
	uint64	total_in;	// total number of input bytes read so far
	uint64	avail_out;	// remaining free space at next_out
	uint64	total_out;	// total number of bytes output so far
	bool32	status;
};
enum {
	IOS_ERR = -1,
	IOS_NOMEM = G_FALSE,
	IOS_OK = G_TRUE,
	IOS_FINISH = 2,
};
static inline IOSTATUS* IOSTATUS_Add(IOSTATUS* _out,const IOSTATUS& _in);
static inline IOSTATUS* IOSTATUS_Clr(IOSTATUS* _ios);
//------------------------------------------------------------------------------------------//
class DS_IO_NODE : public TNFP{
	public:
		typedef DS_IO_NODE DSIO;
	protected:
		enum	{RFLAG_C = 1, RFLAG_S = TNFP::RFLAG_S + TNFP::RFLAG_C};
		enum	{DSTF_blStart = RFLAG_CREATE(0)};
	public:
				 DS_IO_NODE(void);
		virtual ~DS_IO_NODE(void){;};
	protected:
						bool32	BaseSave		(				IOSTATUS* _ios,const UVOut& _out,const uint8* data,const uint64& length);
						bool32	BaseConvert		(DSIO* ioNode,	IOSTATUS* _ios,const UVOut& _out,const UVIn& _in);
						bool32	BaseGetInLength	(uint64* num,									 const UVIn& _in);
						void	FileAdd			(				IOSTATUS* _ios,const STDSTR& name,const uint8* data,const uint64& length);
						void	FileWrite		(				IOSTATUS* _ios,const STDSTR& name,const uint8* data,const uint64& length);
						void	ArrayConvert	(DSIO* ioNode,	IOSTATUS* _ios,const UVOut& _out,const ARRAY* _array,uint32 length,uint32 offset);
						void	FileConvert		(DSIO* ioNode,	IOSTATUS* _ios,const UVOut& _out,const STDSTR& name,uint64 length,uint64 offset);
	protected:
		inline	virtual	DSIO&	DoTransform		(				IOSTATUS* _ios,const UVOut& _out,const uint8* data,const uint64& length);
		inline	virtual	DSIO&	DoFinal			(				IOSTATUS* _ios,const UVOut& _out);
	private:
				virtual	bool32	DoConvert		(DSIO* ioNode,	IOSTATUS* _ios,const UVOut& _out,const UVIn& _in);
				virtual	bool32	DoSave			(				IOSTATUS* _ios,const UVOut& _out,const uint8* data,const uint64& length);
				virtual	bool32	DoGetInLength	(uint64* num,									 const UVIn& _in);
	public:
		inline	virtual	uint64	GetInLength		(												 const UVIn& _in);
		inline			void	Save			(				IOSTATUS* _ios,const UVOut& _out,const UVIn& _in);
		inline			void	Save			(				IOSTATUS* _ios,const UVOut& _out,const uint8* data,const uint64& length);
	public:
		inline			DSIO&	Transform		(				IOSTATUS* _ios,const UVOut& _out,const UVIn& _in);
		inline			DSIO&	Transform		(				IOSTATUS* _ios,const UVOut& _out,const uint8* data,const uint64& length);
		inline			DSIO&	Final			(				IOSTATUS* _ios,const UVOut& _out);
	public:
		static	inline	DSIO& 	GetDSIOList		(void);
};
//------------------------------------------------------------------------------------------//
class	DS_TRANSFORM_FRAME;
typedef DS_TRANSFORM_FRAME DSTF;
//------------------------------------------------------------------------------------------//
struct	STARTUP	{
	IOSTATUS*	ios;
	UVOut*		uvOut;
};
static inline STARTUP	Startup		(IOSTATUS* _ios, const UVOut& _out);
struct _FinishF{void *p;};
static inline _FinishF	Finish		(void)	{return{(void*)nullptr};};
//------------------------------------------------------------------------------------------//
#define CGF_CREATE(offset)	(((uint64)0x01) << (CFG_S + (offset)))
//------------------------------------------------------------------------------------------//
class DS_TRANSFORM_FRAME : public DS_IO_NODE{
	protected:
		enum	{DEFAULTSIZE = 1024};
		enum	{CFG_C = 8, CFG_S = 0};
	public:
		enum	{CFG_INIT			= CGF_CREATE(0)
				,CFG_INIT_RE		= CGF_CREATE(1)	,CFG_INIT_WR	= CGF_CREATE(2)
				,CFG_INIT_A			= CGF_CREATE(3)	,CFG_INIT_B		= CGF_CREATE(4)
				,CFG_INIT_CFG		= CGF_CREATE(5)	,CFG_INIT_PAR	= CGF_CREATE(6)
				,CFG_INIT_CFGPAR	= CFG_INIT_CFG | CFG_INIT_PAR
				,CFG_INIT_RE_CFGPAR = CFG_INIT_RE | CFG_INIT_CFGPAR
				,CFG_INIT_RE_CFG	= CFG_INIT_RE | CFG_INIT_CFG
				,CFG_INIT_RE_PAR	= CFG_INIT_RE | CFG_INIT_PAR
				,CFG_INIT_WR_CFGPAR = CFG_INIT_WR | CFG_INIT_CFGPAR
				,CFG_INIT_WR_CFG	= CFG_INIT_WR | CFG_INIT_CFG
				,CFG_INIT_WR_PAR	= CFG_INIT_WR | CFG_INIT_PAR
				,CFG_INIT_A_CFGPAR	= CFG_INIT_A | CFG_INIT_CFGPAR
				,CFG_INIT_A_CFG		= CFG_INIT_A | CFG_INIT_CFG
				,CFG_INIT_A_PAR		= CFG_INIT_A | CFG_INIT_PAR
				,CFG_INIT_B_CFGPAR	= CFG_INIT_B | CFG_INIT_CFGPAR
				,CFG_INIT_B_CFG		= CFG_INIT_B | CFG_INIT_CFG
				,CFG_INIT_B_PAR		= CFG_INIT_B | CFG_INIT_PAR
				,CFG_DSTF_CTRL		= CFG_INIT_RE | CFG_INIT_WR | CFG_INIT_A | CFG_INIT_B | CFG_INIT_CFG | CFG_INIT_PAR
		};
	public:
				 DS_TRANSFORM_FRAME(void);
		virtual ~DS_TRANSFORM_FRAME(void){;};
	public:
				STARTUP		cgStartup;
				ARRAY		cgArray;
	public:
		inline	virtual	DSTF&	InitSize	(uint32 size);
		inline	virtual	DSTF&	InitCFG		(uint32 cfg,const void* par = nullptr);
		inline	virtual	DSTF&	_Startup	(IOSTATUS* _ios,const UVOut& _out);
	public:
		inline			DSTF&	AllIn		(IOSTATUS* _ios,const UVOut& _out,const UVIn& _in);
	public:
		template <typename T_DSTF> inline	friend	T_DSTF& operator <<	(T_DSTF& _dstf,const STARTUP& sp){
			_dstf._Startup(sp.ios,sp.uvOut);
			return(_dstf);
		};
		template <typename T_DSTF> inline friend T_DSTF& operator << (T_DSTF& _dstf,const _FinishF& _fun){
			_dstf.Final(_dstf.cgStartup.ios,_dstf.cgStartup.uvOut);
			return(_dstf);
		};
		template <typename T_DSTF> inline	friend	T_DSTF& operator <<	(T_DSTF& _dstf,const UVIn& _in){
			_dstf.DSTF::Transform(_dstf.cgStartup.ios,_dstf.cgStartup.uvOut,_in);
			return(_dstf);
		};
		template <typename T_DSTF> inline	friend	T_DSTF& operator <<	(T_DSTF& _dstf,const char* _in){
			_dstf.DSTF::Transform(_dstf.cgStartup.ios,_dstf.cgStartup.uvOut,IUD(_in));
			return(_dstf);
		};
};
//------------------------------------------------------------------------------------------//





//------------------------------------------------------------------------------------------//
namespace DSTF_DIR_FUN{
	typedef void(*Init)		(void* ctx,uint32 cfg,const void* p);
	typedef int (*Release)	(void* ctx);
	typedef int (*Execute)	(void* ctx,uint8* _out,const uint64& outSize,const uint8* data,const uint64& length);
}
//------------------------------------------------------------------------------------------//
struct DSTF_DIR_CTX : public IOSTATUS{
	uint8*	next_in;	// next input byte
	uint8*	next_out;	// next output byte should be put there
	
	uint32	cfg;
	void*	par;
	DSTF*	dstf;
	
	DSTF_DIR_FUN::Init		Init;
	DSTF_DIR_FUN::Execute	Update;
	DSTF_DIR_FUN::Execute	Final;
	DSTF_DIR_FUN::Release	Release;
	DSTF_DIR_FUN::Release	ReInit;
	DSTF_DIR_FUN::Execute	Execute;
};
static inline DSTF_DIR_CTX* DSTF_DIR_CTX_Clr(DSTF_DIR_CTX* ctx);
//------------------------------------------------------------------------------------------//
template <typename DS_CTX> class DSTF_DIR : public DSTF{
	protected:
		enum	{CFG_C = 1, CFG_S = DSTF::CFG_S + DSTF::CFG_C};
	public:
		enum	{CFG_DIR0 = CGF_CREATE(0),CFG_DIR1 = 0};
	public:
				 DSTF_DIR(void);
		virtual ~DSTF_DIR(void){;};
	protected:
		DS_CTX			cgCTX;
	public:
		inline	virtual void		SetSelfName		(const STDSTR& strName);
		inline	virtual	DSTF_DIR&	InitCFG			(uint32 cfg,const void* par = nullptr);
	protected:
		inline	virtual	DSTF_DIR&	DoTransform		(IOSTATUS* _ios,const UVOut& _out,const uint8* data,const uint64& length);
		inline	virtual	DSTF_DIR&	DoFinal			(IOSTATUS* _ios,const UVOut& _out);
						bool32		Save			(DS_CTX* _ctx,IOSTATUS* _ios,const UVOut& _out,const uint8* data,const uint64& length);
};
//------------------------------------------------------------------------------------------//
typedef DSTF_DIR<DSTF_DIR_CTX> DSTF_DIRBASE;
//------------------------------------------------------------------------------------------//
template <typename T_A,typename T_B>
class DSTF_AB_FRAME : public DSTF_DIR<DSTF_DIR_CTX>{
	protected:
		enum	{RFLAG_C = 2, RFLAG_S = DSTF::RFLAG_S + DSTF::RFLAG_C};
		enum	{DSTF_blEnA = RFLAG_CREATE(0),DSTF_blEnB = RFLAG_CREATE(1),};
	public:
		enum	{CFG_ATOB = CFG_DIR0, CFG_BTOA = CFG_DIR1};
	public:
				 DSTF_AB_FRAME(void);
		virtual ~DSTF_AB_FRAME(void){;};
	protected:
		T_A		cgA;
		T_B		cgB;
	public:
		inline	virtual void			SetSelfName		(const STDSTR& strName);
		inline	virtual	void			SetFatherName	(const STDSTR& strName);
	public:
		inline	virtual	DSTF_AB_FRAME&	InitSize		(uint32 size);
		inline	virtual	DSTF_AB_FRAME&	InitCFG 		(uint32 cfg,const void* par);
		inline	virtual	DSTF_AB_FRAME&	_Startup		(IOSTATUS* _ios, const UVOut& _out);
		inline			void			DisableA		(void);
		inline			void			DisableB		(void);
		inline			void			EnableAB		(void);
	protected:
		inline	virtual	DSTF_AB_FRAME&	DoTransform		(IOSTATUS* _ios,const UVOut& _out,const uint8* data,const uint64& length);
		inline	virtual	DSTF_AB_FRAME&	DoFinal			(IOSTATUS* _ios,const UVOut& _out);
};
//------------------------------------------------------------------------------------------//
namespace DSTF_TEST {
	template <typename T_DIR> 	T_DIR* DIR_Create	(const STDSTR& name,T_DIR* _nullptr,uint32 size,uint32 cfg,void* p);
	
	template <typename T_DIR>	bool32 DIR_Test		(IOSTATUS* _ios,STDSTR* _out,const STDSTR& _in
													 ,const STDSTR& name,uint32 layer,uint32 round
													 ,T_DIR* _nullptr,uint32 size,uint32 cfg,void* p);
	
	template<typename... Args>	bool32 DIR_Test		(IOSTATUS* _ios,STDSTR* _out,const STDSTR& _in
													 ,const STDSTR& name,uint32 round
													 ,Args* ...args);
};
//------------------------------------------------------------------------------------------//
#include "DS_Transform.hpp"
#endif /* DS_Transform_h */
#endif /* DS_Transform_h */
#endif

