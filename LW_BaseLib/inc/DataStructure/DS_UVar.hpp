//
//  DS_UVar.hpp
//  SDT
//
//  Created by Leif Wen on 18/01/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

//------------------------------------------------------------------------------------------//
#ifndef DS_UVar_hpp
#define DS_UVar_hpp
//------------------------------------------------------------------------------------------//
#include "DS_UVar.h"
#ifdef DS_UVar_h
//------------------------------------------------------------------------------------------//
static inline	_UVBASE		_NONE		(void)									{return{nullptr	,UVID_NONE};};
static inline	_UVBASE		OUDB		(void*		p,	const uint32& uvid)		{return{p		,uvid};};
static inline	_UVBASE&	OUD			(_UVBASE&	uvb)						{return(uvb);};
static inline	_UVBASE		OUD			(_UVV_TRAN&	uvt)						{return{&uvt	,UVID_UVVTRAN};};
#ifdef DS_ARRAY_h
static inline	_UVBASE		OUD			(ARRAY* 	p)							{return{p		,UVID_ARRAY1};};
#endif
static inline	_UVBASE		OUD			(STDSTR* 	p)							{return{p		,UVID_STR};};
static inline	_UVBASE		OUD_HEX		(STDSTR* 	p)							{return{p		,UVID_STRHEX};};
static inline	_UVBASE		OUD_HEXs	(STDSTR* 	p)							{return{p		,UVID_STRHEXs};};

static inline	_UVBASE		OUD			(const UVOut&	uv)						{return{uv.uvp	,uv.uvid};};

static inline	_UVV_CHARS	OUD_CHARS	(uint8*	p,uint64 num,uint64 offset)		{return{{(void*)p,num,offset}		,UVID_CHARS};};
static inline	_UVV_FILE	OUD_FILEApp	(const STDSTR&	name)					{return{{(STDSTR*)&name,0,0,0}		,UVID_FILE_ADD};};
static inline	_UVV_FILE	OUD_FILEWR	(const STDSTR&	name)					{return{{(STDSTR*)&name,0,0,1}		,UVID_FILE_WR};};
static inline	_UVV_FILE	OUD_FILE	(const STDSTR&	name,uint64 offset)		{return{{(STDSTR*)&name,0,offset,1}	,UVID_FILE};};

template <typename DS_UV>
static inline	_UVV_TRAN	OUD			(void* iop,	const DS_UV&	uvp)		{return{{iop,OUD(uvp)},UVID_UVVTRAN};};
static inline	_UVV_TRAN	OUD			(void* iop)								{return{{iop,{iop,UVID_SELF}},UVID_UVVTRAN};};
//------------------------------------------------------------------------------------------//
static inline	_UVBASE		IUDB		(const void* 	p,const uint32& uvid)			{return{(void*)p						,uvid};};
static inline	_UVV_CHARS	IUD			(const char*	p)								{return{{(void*)p	,strlen(p)	,0}		,UVID_CHARS};};
static inline	_UVV_CHARS	IUD			(const uint8&	p)								{return{{(void*)&p	,1			,0}		,UVID_CHARS};};
static inline	_UVV_CHARS	IUD			(const uint8*	p,uint64 num,uint64 offset)		{return{{(void*)p	,num+offset	,offset},UVID_CHARS};};
static inline	_UVV_CHARS	IUD			(const STDSTR&	p,uint64 num,uint64 offset)		{
	num += offset;
	if (num > p.length())
		num = p.length();
	return{{(void*)p.c_str(),num,offset},UVID_CHARS};
};
static inline	_UVV_CHARS	IUD			(const STDSTR*	p,uint64 num,uint64 offset)		{
	num += offset;
	if (num > p->length())
		num = p->length();
	return{{(void*)p->c_str(),num,offset},UVID_CHARS};
};
#ifdef DS_ARRAY_h
static inline	_UVV_ARRAY	IUD			(const ARRAY&	p,uint32 num,uint32 offset)		{
	num += offset;
	if (num > p.Used())
		num = p.Used();
	return{{(void*)&p	,num	,offset},UVID_ARRAY1};
};
static inline	_UVV_ARRAY	IUD			(const ARRAY*	p,uint32 num,uint32 offset)		{
	num += offset;
	if (num > p->Used())
		num = p->Used();
	return{{(void*)p	,num	,offset},UVID_ARRAY1};
};
#endif
static inline	_UVV_FILE	IUD_FILE	(const STDSTR&	fn,uint64 num,uint64 offset)	{return{{(void*)&fn	,num+offset,offset},UVID_FILE};};
//------------------------------------------------------------------------------------------//
inline UVOut::UVOut(void)										{uvid = UVID_NONE;	uvp = nullptr;};
inline UVOut::UVOut(const UVOut&		uv)						{uvid = uv.uvid;	uvp = uv.uvp;};
inline UVOut::UVOut(const _UVBASE&		uv)						{uvid = uv.uvid;	uvp = (void*)uv.p;};
inline UVOut::UVOut(const STDSTR*		uv)						{uvid = UVID_STR;	uvp = (void*)uv;};
#ifdef DS_ARRAY_h
inline UVOut::UVOut(const ARRAY* uv)							{uvid = UVID_ARRAY1;uvp = (void*)uv;};
#endif
inline const UVOut& UVOut::operator = (const UVOut&			uv)	{uvid = uv.uvid;	uvp = (void*)uv.uvp;	return(*this);};
inline const UVOut& UVOut::operator = (const _UVBASE&		uv)	{uvid = uv.uvid;	uvp = (void*)uv.p;		return(*this);};
inline const UVOut& UVOut::operator = (const STDSTR*		uv)	{uvid = UVID_STR;	uvp = (void*)uv;		return(*this);};
#ifdef DS_ARRAY_h
inline const UVOut& UVOut::operator = (const ARRAY* uv)			{uvid = UVID_ARRAY1;uvp = (void*)uv;		return(*this);};
#endif
//------------------------------------------------------------------------------------------//
template <typename UVT> inline const UVOut& UVOut::operator = (const _UVVT<UVT>& uv){
	uvid = uv.uvid;
	uvp = (void*)&uv.var;
	return(*this);
};
//------------------------------------------------------------------------------------------//
inline UVOut::UVOut(const UVOut* uv){
	uvid = UVID_NONE;
	uvp = nullptr;
	if (uv != nullptr){
		uvid = uv->uvid;
		uvp = uv->uvp;
	}
};
//-----------------------*-------------------------------------------------------------------//
inline UVOut::UVOut(const _UVBASE* uv){
	uvid = UVID_NONE;
	uvp = nullptr;
	if (uv != nullptr){
		uvid = uv->uvid;
		uvp = (void*)uv->p;
	}
};
//------------------------------------------------------------------------------------------//
template <typename UVT> inline UVOut::UVOut(const _UVVT<UVT>& uv){
	uvid = uv.uvid;
	uvp = (void*)&uv.var;
};
//------------------------------------------------------------------------------------------//
inline UVIn::UVIn(void)					{uvid = UVID_NONE;	uvp = nullptr;		uvOffset = 0;};
inline UVIn::UVIn(const UVIn&		uv)	{uvid = uv.uvid;	uvp = uv.uvp;		uvOffset = uv.uvOffset;};
inline UVIn::UVIn(const _UVBASE&	uv)	{uvid = uv.uvid;	uvp = (void*)uv.p;	uvOffset = 0;};
inline UVIn::UVIn(const STDSTR&		uv)	{uvid = UVID_STR;	uvp = (void*)&uv;	uvOffset = 0;};
inline UVIn::UVIn(const STDSTR*		uv)	{uvid = UVID_STR;	uvp = (void*)uv;	uvOffset = 0;};
#ifdef DS_ARRAY_h
inline UVIn::UVIn(const ARRAY&		uv)	{uvid = UVID_ARRAY0;uvp = (void*)&uv;	uvOffset = 0;};
inline UVIn::UVIn(const ARRAY*		uv)	{uvid = UVID_ARRAY0;uvp = (void*)uv;	uvOffset = 0;};
#endif
//------------------------------------------------------------------------------------------//
inline UVIn::UVIn(const _UVBASE* uv){
	uvid = UVID_NONE;
	uvp = nullptr;
	uvOffset = 0;
	if (uv != nullptr){
		uvid = uv->uvid;
		uvp = (void*)uv->p;
	}
};
//------------------------------------------------------------------------------------------//
template <typename UVT> inline UVIn::UVIn(const _UVVT<UVT>& uv){
	uvid = uv.uvid;
	uvp = (void*)&uv.var;
	uvOffset = 0;
};
//------------------------------------------------------------------------------------------//
inline const UVIn& UVIn::operator = (const UVIn&		uv)		{uvid = uv.uvid;	uvp = (void*)uv.uvp;	uvOffset = uv.uvOffset;return(*this);};
inline const UVIn& UVIn::operator = (const _UVBASE&		uv)		{uvid = uv.uvid;	uvp = (void*)uv.p;		uvOffset = 0;return(*this);};
//------------------------------------------------------------------------------------------//
template <typename UVT> inline const UVIn& UVIn::operator = (const _UVVT<UVT>& uv){
	uvid = uv.uvid;
	uvp = (void*)&uv.var;
	uvOffset = 0;
	return(*this);
};
//------------------------------------------------------------------------------------------//
#endif /* DS_UVar_h */
#endif /* DS_UVar_hpp */
