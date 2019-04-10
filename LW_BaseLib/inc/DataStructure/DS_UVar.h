//
//  DS_UVar.h
//  SDT
//
//  Created by Leif Wen on 18/01/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#ifndef DS_UVar_h
#define DS_UVar_h
//------------------------------------------------------------------------------------------//
#include "BasicDefine.h"
#include "DS_ARRAY.h"
//------------------------------------------------------------------------------------------//
enum {
	UVID_NONE = 0,
	
	UVID_SELF,
	UVID_UVVTRAN,
	UVID_CHARS,
	UVID_ARRAY,
	
	UVID_STR,
	UVID_STRHEX,
	UVID_STRHEXs,
	
	UVID_FILE,
	UVID_FILE_ADD,
	UVID_FILE_WR,

	UVID_Next,
};
//------------------------------------------------------------------------------------------//
template <typename DS_V>
struct	_UVVT		{DS_V var;	uint32  uvid;};

struct	_UVBASE		{void* p;	uint32	uvid;};
struct	_UVTRAN		{void* iop;	_UVBASE	uvb;};
struct	_UVCHARS	{void* p;	uint64	num;	uint64 offset;};
struct	_UVARRAY	{void* p;	uint32	num;	uint32 offset;};
struct	_UVFILE		{void* p;	uint64	num;	uint64 offset;	uint32 cfg;};

typedef _UVVT<_UVTRAN>		_UVV_TRAN;	//{_UVTRAN var,	uint32  uvid}
typedef _UVVT<_UVCHARS>		_UVV_CHARS;	//{_UVCHARS var,uint32  uvid}
typedef _UVVT<_UVARRAY>		_UVV_ARRAY;	//{_UVARRAY var,uint32  uvid}
typedef _UVVT<_UVFILE>		_UVV_FILE;	//{_UVFILE var, uint32  uvid}
//------------------------------------------------------------------------------------------//
class UVOut{
	public:
		void*	uvp;
		uint32	uvid;
	public:
		inline ~UVOut(void){;};
	public:
								inline UVOut(void);
								inline UVOut(const UVOut&		uv);
								inline UVOut(const UVOut*		uv);
								inline UVOut(const _UVBASE&		uv);
								inline UVOut(const _UVBASE*		uv);
								inline UVOut(const STDSTR*		uv);
	#ifdef DS_ARRAY_h
								inline UVOut(const ARRAY*		uv);
	#endif
		template <typename UVT> inline UVOut(const _UVVT<UVT>&	uv);
	public:
								inline	const UVOut& operator = (const UVOut&		uv);
								inline	const UVOut& operator = (const _UVBASE&		uv);
								inline	const UVOut& operator = (const STDSTR*		uv);
	#ifdef DS_ARRAY_h
								inline	const UVOut& operator = (const ARRAY*		uv);
	#endif
		template <typename UVT> inline	const UVOut& operator = (const _UVVT<UVT>&	uv);
};
//------------------------------------------------------------------------------------------//
class UVIn{
	public:
		void*	uvp;
		uint32	uvid;
	public:
								inline ~UVIn(void){;};
								inline UVIn(void);
								inline UVIn(const UVIn&			uv);
								inline UVIn(const _UVBASE&		uv);
								inline UVIn(const _UVBASE*		uv);
								inline UVIn(const STDSTR&		uv);
								inline UVIn(const STDSTR*		uv);
		template <typename UVT> inline UVIn(const _UVVT<UVT>&	uv);
	public:
								inline	const UVIn& operator = (const UVIn&		uv);
								inline	const UVIn& operator = (const _UVBASE&	uv);
		template <typename UVT> inline	const UVIn& operator = (const _UVVT<UVT>& uv);
};
//------------------------------------------------------------------------------------------//
static inline	STDSTR*		_EMPTY		(STDSTR* p);
#ifdef DS_ARRAY_h
static inline	ARRAY*		_EMPTY		(ARRAY*  p);
#endif

static inline	_UVBASE		_NONE		(void);
static inline	_UVBASE		OUDB		(void*		p,const uint32& uvid);
static inline	_UVBASE&	OUD			(_UVBASE&	uvb);
static inline	_UVBASE		OUD			(_UVV_TRAN&	uvt);
static inline	_UVBASE		OUD			(STDSTR* 	p);
static inline	_UVBASE		OUD_HEX		(STDSTR* 	p);
static inline	_UVBASE		OUD_HEXs	(STDSTR* 	p);

static inline	_UVBASE		OUD			(const UVOut&	uv);

static inline	_UVV_CHARS	OUD_CHARS	(uint8*	p,uint64 num = 1,uint64 offset = 0);
static inline	_UVV_FILE	OUD_FILEADD	(const STDSTR&	name);
static inline	_UVV_FILE	OUD_FILEWR	(const STDSTR&	name);

template <typename DS_UV>
static inline	_UVV_TRAN	OUD			(void* iop,	const DS_UV& uvp);
static inline	_UVV_TRAN	OUD			(void* iop);
//------------------------------------------------------------------------------------------//
static inline	_UVBASE		IUDB		(const void* 	p,const uint32& uvid);
static inline	_UVV_CHARS	IUD			(const char*	p);
static inline	_UVV_CHARS	IUD			(const uint8&	p);
static inline	_UVV_CHARS	IUD			(const uint8*	p,uint64 num);
#ifdef DS_ARRAY_h
static inline	_UVV_ARRAY	IUD			(const ARRAY&	p,uint32 num = -1,uint32 offset = 0);
static inline	_UVV_ARRAY	IUD			(const ARRAY*	p,uint32 num = -1,uint32 offset = 0);
#endif
static inline	_UVV_FILE	IUD_FILE	(const STDSTR&	name,uint64 num = -1,uint64 offset = 0);
//------------------------------------------------------------------------------------------//
#include "DS_UVar.hpp"
#endif /* DS_UVar_h */
