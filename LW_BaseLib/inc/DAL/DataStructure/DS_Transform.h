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
enum IOSE{
	IOS_TIMEOUT = -4,
	IOS_NO_MEM = -3,
	IOS_NO_OBJECT = -1,
	IOS_ERR = G_FALSE,
	IOS_OK = G_TRUE,
	IOS_FINISH = 2,
};
struct IOS{
	uint64	avail_in;	// number of bytes available at next_in
	uint64	total_in;	// total number of input bytes read so far
	uint64	avail_out;	// remaining free space at next_out
	uint64	total_out;	// total number of bytes output so far
	IOSE	rcode;
};
						STDSTR 	_GetIOSEMesg	(IOSE 	code);
static inline			IOS* 	IOS_clr			(IOS* 	_ios);
static inline			IOS* 	IOS_reset		(IOS* 	_ios);
static inline	const	IOSE&	IOS_update		(IOS* 	_out,const IOS& _in);
static inline	const	IOSE&	IOS_update		(IOS* 	_ios,const IOSE& _in);
static inline	const	IOSE&	IOS_update		(IOSE* 	_out,const IOSE& _in);
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
						IOSE	BaseConvert		(DSIO* ioNode,	IOS* _ios,const UVOut& _out,const UVIn& _in);
						IOSE	BaseSave		(				IOS* _ios,const UVOut& _out,const uint8* data,const uint64& length);
						IOSE	BaseGetUVInLen	(uint64* num,								const UVIn& _in);
						IOSE	BaseAddUVInOffset(											const UVIn& _in,const int64& length);
						IOSE	ArrayConvert	(DSIO* ioNode,	IOS* _ios,const UVOut& _out,const ARRAY* _array,uint32 length,uint32 offset);
						IOSE	FileConvert		(DSIO* ioNode,	IOS* _ios,const UVOut& _out,const STDSTR& name,uint64 length,const uint64& offset);
						IOSE	FileAdd			(				IOS* _ios,const STDSTR& name,const uint8* data,const uint64& length);
						IOSE	FileWrite		(				IOS* _ios,const STDSTR& name,const uint8* data,const uint64& length);
						IOSE	FileUpdate		(				IOS* _ios,const STDSTR& name,const uint8* data,const uint64& length,const uint64& offset);
	protected:
		inline	virtual	IOSE	DoTransform		(				IOS* _ios,const UVOut& _out,const uint8* data,const uint64& length);
		inline	virtual	IOSE	DoFinal			(				IOS* _ios,const UVOut& _out);
	private:
				virtual	IOSE	DoConvert		(DSIO* ioNode,	IOS* _ios,const UVOut& _out,const UVIn& _in);
				virtual	IOSE	DoSave			(				IOS* _ios,const UVOut& _out,const uint8* data,const uint64& length);
				virtual	IOSE	DoGetUVInLength	(uint64* num,								const UVIn& _in);
				virtual	IOSE	DoAddUVInOffset	(											const UVIn& _in,const int64& length);
	public:
		inline	virtual	uint64	GetUVInLength	(											const UVIn& _in);
		inline			IOSE	AddUVInOffset	(											const UVIn& _in,const int64& length);
		inline			IOSE	Save			(				IOS* _ios,const UVOut& _out,const UVIn& _in);
		inline			IOSE	Save			(				IOS* _ios,const UVOut& _out,const uint8* data,const uint64& length);
	public:
		inline			IOSE	Transform		(				IOS* _ios,const UVOut& _out,const UVIn& _in);
		inline			IOSE	Transform		(				IOS* _ios,const UVOut& _out,const uint8* data,const uint64& length);
		inline			IOSE	Final			(				IOS* _ios,const UVOut& _out);
	public:
		static	inline	DSIO& 	GetDSIOList		(void);
};
//------------------------------------------------------------------------------------------//
class	DS_TRANSFORM_FRAME;
typedef DS_TRANSFORM_FRAME DSTF;
//------------------------------------------------------------------------------------------//
struct	STARTUP	{
	IOS*	ios;
	UVOut*		uvOut;
};
#define CreateOperatorSetUint32(x) \
	struct 					_##x							{uint32 value;};\
	static inline _##x		Set##x	(const uint32 &value)	{return{value};};

#define CreateOperatorClrUint32(x) \
	struct					_Clr##x							{uint32 value;};\
	static inline _Clr##x	Clr##x	(const uint32 &value)	{return{value};};

#define CreateOperatorFunc(x) \
	struct 			_##x##F				{void* p;};\
	static inline	_##x##F	x	(void)	{return{(void*)nullptr};};

#define CreateOperatorFunc2(x) \
	template <typename T>static inline T& x(T& _tn){_tn._##x();return(_tn);};

struct 			_SetUVOut									{UVOut value;};
static inline	_SetUVOut	SetOut	(const UVOut& _uvout)	{return{_uvout};};

struct 			_BeginIOS 							{IOS* value;};
static inline	_BeginIOS	Begin	(IOS* _ios)		{return{_ios};};

struct 			_EndIOS 							{IOS* value;};
static inline	_EndIOS		Endl	(IOS* _ios)		{return{_ios};};

CreateOperatorFunc(Begin);
CreateOperatorFunc(Endl);
//------------------------------------------------------------------------------------------//
#define CreateOperatorFunc3(x) \
template <typename T>static inline T& x(T& _tn){_tn._##x(nullptr);return(_tn);};

CreateOperatorFunc3(Begin);
//------------------------------------------------------------------------------------------//
#define CreateOperatorFunc4(x) \
template <typename T>static inline T& x(T& _tn){_tn._##x(_tn.cgStartup.ios);return(_tn);};

CreateOperatorFunc4(Endl);
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
		inline	virtual	DSTF&	_SetOut		(const UVOut& _out);
		inline	virtual	DSTF&	_SetIOS		(IOS* _ios);
	public:
		inline	virtual	IOSE	_Begin		(IOS* _ios);
		inline	virtual	IOSE	_Endl		(IOS* _ios);
	public:
		inline			IOSE	AllIn		(IOS* _ios,const UVOut& _out,const UVIn& _in);
	public:
		template <typename T_DSTF> inline friend T_DSTF&	operator <<	(T_DSTF& _dstf,const IOS& _ios){
			_dstf._SetIOS(&_ios);
			return(_dstf);
		};
		template <typename T_DSTF> inline friend T_DSTF&	operator <<	(T_DSTF& _dstf,IOS* _ios){
			_dstf._SetIOS(_ios);
			return(_dstf);
		};
		template <typename T_DSTF> inline friend T_DSTF&	operator <<	(T_DSTF& _dstf,const _SetUVOut& _out){
			_dstf._SetOut(_out.value);
			return(_dstf);
		};
		template <typename T_DSTF>	inline friend T_DSTF&	operator << (T_DSTF& _dstf,T_DSTF& (*func)(T_DSTF&)){
			return((*func)(_dstf));
		};
		template <typename T_DSTF>	inline friend T_DSTF&	operator << (T_DSTF& _dstf,const _BeginIOS& _sios){
			_dstf._Begin(_sios.value);
			return(_dstf);
		};
		template <typename T_DSTF>	inline friend T_DSTF&	operator << (T_DSTF& _dstf,const _BeginF& _func){
			_dstf._Begin(nullptr);
			return(_dstf);
		};
		template <typename T_DSTF>	inline	friend	IOSE	operator << (T_DSTF& _dstf,const _EndIOS& _sios){
			return(_dstf._Endl(_sios.value));
		};
		template <typename T_DSTF>	inline	friend	IOSE	operator << (T_DSTF& _dstf,const _EndlF& _func){
			return(_dstf._Endl(_dstf.cgStartup.ios));
		};
		template <typename T_DSTF>	inline	friend	T_DSTF&	operator <<	(T_DSTF& _dstf,const UVIn& _in){
			_dstf.DSTF::Transform(_dstf.cgStartup.ios,_dstf.cgStartup.uvOut,_in);
			return(_dstf);
		};
		template <typename T_DSTF>	inline	friend	T_DSTF&	operator <<	(T_DSTF& _dstf,const char* _in){
			_dstf.DSTF::Transform(_dstf.cgStartup.ios,_dstf.cgStartup.uvOut,IUD(_in));
			return(_dstf);
		};
};
//------------------------------------------------------------------------------------------//





//------------------------------------------------------------------------------------------//
namespace DSTF_DIR_FUN{
	typedef void(*Init)		(void* ctx,uint32 cfg,const void* p);
	typedef IOSE (*Release)	(void* ctx);
	typedef IOSE (*Execute)	(void* ctx,uint8* _out,const uint64& outSize,const uint8* data,const uint64& length);
};
//------------------------------------------------------------------------------------------//
struct DSTF_DIR_CTX : public IOS{
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
		inline	virtual	IOSE		DoTransform		(IOS* _ios,const UVOut& _out,const uint8* data,const uint64& length);
		inline	virtual	IOSE		DoFinal			(IOS* _ios,const UVOut& _out);
						IOSE		Save			(DS_CTX* _ctx,IOS* _ios,const UVOut& _out,const uint8* data,const uint64& length);
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
		inline	virtual	void			SetUpName		(const STDSTR& strName);
	public:
		inline	virtual	DSTF_AB_FRAME&	InitSize		(uint32 size);
		inline	virtual	DSTF_AB_FRAME&	InitCFG 		(uint32 cfg,const void* par);
		inline	virtual	DSTF_AB_FRAME&	_SetOut			(const UVOut& _out);
		inline	virtual	DSTF_AB_FRAME&	_SetIOS			(IOS* _ios);
		inline			void			DisableA		(void);
		inline			void			DisableB		(void);
		inline			void			EnableAB		(void);
	protected:
		inline	virtual	IOSE			DoTransform		(IOS* _ios,const UVOut& _out,const uint8* data,const uint64& length);
		inline	virtual	IOSE			DoFinal			(IOS* _ios,const UVOut& _out);
};
//------------------------------------------------------------------------------------------//
namespace DSTF_TEST {
	template <typename T_DIR> 	T_DIR* DIR_Create	(const STDSTR& name,T_DIR* _nullptr,uint32 size,uint32 cfg,void* p);
	
	template <typename T_DIR>	bool32 DIR_Test		(IOS* _ios,STDSTR* _out,const STDSTR& _in
													 ,const STDSTR& name,uint32 layer,uint32 round
													 ,T_DIR* _nullptr,uint32 size,uint32 cfg,void* p);
	
	template<typename... Args>	bool32 DIR_Test		(IOS* _ios,STDSTR* _out,const STDSTR& _in
													 ,const STDSTR& name,uint32 round
													 ,Args* ...args);
};
//------------------------------------------------------------------------------------------//
#include "DS_Transform.hpp"
#endif /* DS_Transform_h */
#endif /* DS_Transform_h */
#endif

