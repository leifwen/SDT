//
//  DS_Transform.hpp
//  SDT
//
//  Created by Leif Wen on 19/01/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

//------------------------------------------------------------------------------------------//
#ifndef DS_Transform_hpp
#define DS_Transform_hpp
//------------------------------------------------------------------------------------------//
#include "DS_Transform.h"
#ifdef DS_Transform_h
//------------------------------------------------------------------------------------------//
#define	uInt32_MaxSize	((uint32)-1)
//------------------------------------------------------------------------------------------//
static inline IOS* IOS_clr(IOS* _ios){
	if (_ios != nullptr){
		_ios->avail_in  = 0;
		_ios->total_in  = 0;
		_ios->avail_out = 0;
		_ios->total_out = 0;
		_ios->rcode		= IOS_OK;
	}
	return(_ios);
};
//------------------------------------------------------------------------------------------//
static inline IOS* IOS_reset(IOS* _ios){
	if (_ios != nullptr)
		_ios->rcode	= IOS_OK;
	return(_ios);
};
//------------------------------------------------------------------------------------------//
static inline const IOSE& IOS_update(IOS* _out,const IOS& _in){
	if (_out != nullptr){
		if (_out->rcode > 0){
			_out->avail_in  =  _in.avail_in;
			_out->total_in  += _in.total_in;
			_out->avail_out =  _in.avail_out;
			_out->total_out += _in.total_out;
		}
		IOS_update(&_out->rcode,_in.rcode);
		return(_out->rcode);
	}
	return(_in.rcode);
};
//------------------------------------------------------------------------------------------//
static inline const IOSE& IOS_update(IOSE* _out,const IOSE& _in){
	if (*_out > 0)
		*_out = _in;
	return(*_out);
};
//------------------------------------------------------------------------------------------//
static inline const IOSE& IOS_update(IOS* _ios,const IOSE& _in){
	if (_ios != nullptr)
		return(IOS_update(&_ios->rcode,_in));
	return(_in);
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
inline DS_IO_NODE& DS_IO_NODE::GetDSIOList(void){
	static DS_IO_NODE	sgIOList;
	return(sgIOList);
};
//------------------------------------------------------------------------------------------//
inline IOSE DS_IO_NODE::Save(IOS* _ios,const UVOut& _out,const UVIn& _in){
	return(GetDSIOList().DoConvert(&GetDSIOList(),_ios,_out,_in));
};
//------------------------------------------------------------------------------------------//
inline IOSE DS_IO_NODE::Save(IOS* _ios,const UVOut& _out,const uint8* data,const uint64& length){
	return(GetDSIOList().DoSave(_ios,_out,data,length));
};
//------------------------------------------------------------------------------------------//
inline IOSE DS_IO_NODE::Transform(IOS* _ios,const UVOut& _out,const UVIn& _in){
	SetSFlag(DSTF_blStart);
	return(GetDSIOList().DoConvert(this,_ios,_out,_in));
};
//------------------------------------------------------------------------------------------//
inline IOSE DS_IO_NODE::Transform(IOS* _ios,const UVOut& _out,const uint8* data,const uint64& length){
	SetSFlag(DSTF_blStart);
	return(DoTransform(_ios,_out,data,length));
};
//------------------------------------------------------------------------------------------//
inline IOSE DS_IO_NODE::Final(IOS* _ios,const UVOut& _out){
	IOSE	rcode;
	if (CheckSFlag(DSTF_blStart) == G_FALSE)
		return(IOS_update(_ios, IOS_FINISH));
	ClrSFlag(DSTF_blStart);
	rcode = DoFinal(_ios,_out);
	if (rcode == IOS_OK)
		SetSFlag(DSTF_blStart);
	return(rcode);
};
//------------------------------------------------------------------------------------------//
inline uint64 DS_IO_NODE::GetUVInLength(const UVIn& _in){
	uint64	num;
	GetDSIOList().DoGetUVInLength(&num,_in);
	return(num);
};
//------------------------------------------------------------------------------------------//
inline IOSE DS_IO_NODE::AddUVInOffset(const UVIn& _in,const int64& length){
	return(GetDSIOList().DoAddUVInOffset(_in,length));
};
//------------------------------------------------------------------------------------------//
inline IOSE DS_IO_NODE::DoTransform(IOS* _ios,const UVOut& _out,const uint8* data,const uint64& length){
	return(Save(_ios,_out,data,length));
};
//------------------------------------------------------------------------------------------//
inline IOSE DS_IO_NODE::DoFinal(IOS* _ios,const UVOut& _out){
	IOSE	rcode;
	rcode = Save(_ios,_out,nullptr,0);
	if (rcode == IOS_OK)
		rcode = IOS_FINISH;
	return(IOS_update(_ios, rcode));
};
//------------------------------------------------------------------------------------------/










//------------------------------------------------------------------------------------------//
inline DSTF& DSTF::InitSize(uint32 size){
	cgArray.InitSize(size);
	return(*this);
};
//------------------------------------------------------------------------------------------//
inline DSTF& DSTF::InitCFG(uint32 cfg,const void* par){
	return(*this);
};
//------------------------------------------------------------------------------------------//
inline IOSE DSTF::AllIn(IOS* _ios,const UVOut& _out,const UVIn& _in){
	IOSE rcode = IOS_OK;
	IOS_update(&rcode,Transform(_ios,_out,_in));
	IOS_update(&rcode,Final(_ios,_out));
	return(rcode);
};
//------------------------------------------------------------------------------------------//
inline IOSE DSTF::_Begin(IOS* _ios){
	SetSFlag(DSTF_blStart);
	_SetIOS(_ios);
	return(IOS_update(_ios, IOS_OK));
};
//------------------------------------------------------------------------------------------//
inline IOSE DSTF::_Endl(IOS* _ios){
	return(Final(_ios,cgStartup.uvOut));
};
//------------------------------------------------------------------------------------------//
inline DSTF& DSTF::_SetOut(const UVOut& _out){
	cgStartup.uvOut	= (UVOut*)&_out;
	return(*this);
};
//------------------------------------------------------------------------------------------//
inline DSTF& DSTF::_SetIOS(IOS* _ios){
	cgStartup.ios = _ios;
	return(*this);
};
//------------------------------------------------------------------------------------------//









//------------------------------------------------------------------------------------------//
static inline DSTF_DIR_CTX* DSTF_DIR_CTX_Clr(DSTF_DIR_CTX* ctx){
	IOS_clr(ctx);
	ctx->next_in = nullptr;
	ctx->next_out = nullptr;
	
	ctx->cfg = 0;
	ctx->par = nullptr;
	ctx->dstf = nullptr;
	
	ctx->Init = nullptr;
	ctx->Update = nullptr;
	ctx->Final = nullptr;
	ctx->Release = nullptr;
	ctx->ReInit = nullptr;
	ctx->Execute = nullptr;
	return(ctx);
};
//------------------------------------------------------------------------------------------//
static inline void DSTF_DIR_CTX_Init(void* ctx,uint32 cfg,const void* par){
	DSTF_DIR_CTX *_ctx = static_cast<DSTF_DIR_CTX*>(ctx);
	_ctx->cfg = cfg;
	_ctx->par = (void*)par;
};
//------------------------------------------------------------------------------------------//
template <typename DS_CTX> DSTF_DIR<DS_CTX>::DSTF_DIR(void) : DSTF(){
	DSTF_DIR_CTX_Clr(&cgCTX);
	cgCTX.dstf = this;
	cgCTX.Init = DSTF_DIR_CTX_Init;
};
//------------------------------------------------------------------------------------------//
template <typename DS_CTX> inline void DSTF_DIR<DS_CTX>::SetSelfName(const STDSTR& strName){
	selfName = strName;
};
//------------------------------------------------------------------------------------------//
template <typename DS_CTX> inline DSTF_DIR<DS_CTX>& DSTF_DIR<DS_CTX>::InitCFG(uint32 cfg,const void* par){
	
	if (B_ChkFLAG32(cgCTX.cfg, CFG_INIT) == G_FALSE)
		cfg |= CFG_INIT_CFGPAR;
	
	DSTF::InitCFG(cfg,par);
	if ((B_ChkFLAG32(cfg, CFG_INIT_CFG | CFG_INIT_PAR) == G_FALSE)
		|| (B_ChkFLAG32(cfg, CFG_INIT_CFG) && B_ChkFLAG32(cfg,CFG_INIT_PAR))){
		B_ClrFLAG32(cfg, DSTF::CFG_DSTF_CTRL | DSTF::CFG_INIT);
		cgCTX.Init(&cgCTX,cfg,par);
	}
	else if (B_ChkFLAG32(cfg, CFG_INIT_CFG)){
		B_ClrFLAG32(cfg, DSTF::CFG_DSTF_CTRL | DSTF::CFG_INIT);
		cgCTX.Init(&cgCTX,cfg,cgCTX.par);
	}
	else if (B_ChkFLAG32(cfg, CFG_INIT_PAR)){
		B_ClrFLAG32(cfg, DSTF::CFG_DSTF_CTRL | DSTF::CFG_INIT);
		cgCTX.Init(&cgCTX,cgCTX.cfg,par);
	}
	return(*this);
};
//------------------------------------------------------------------------------------------//
template <typename DS_CTX>
IOSE DSTF_DIR<DS_CTX>::Save(DS_CTX* _ctx,IOS* _ios,const UVOut& _out,const uint8* data,const uint64& length){
	uint32	flength,offset;
	IOS		ios_in,ios_out;
	
	_ctx->next_in = (uint8*)data;
	
	IOS_clr(&ios_in);
	IOS_clr(&ios_out);
	ios_in.avail_in = length;
	if (cgArray.MaxSize() == 0)
		cgArray.InitSize(DEFAULTSIZE);
	do{
		do{
			if (cgArray.IsEmpty())
				cgArray.Reset();
			
			_ctx->total_in = 0;
			_ctx->total_out = 0;
			
			flength = 0;
			offset = 0;
			cgArray.CalcInCapacity(flength,offset);
			
			ios_in.rcode = _ctx->Execute(_ctx
										 ,cgArray.GetPointer(offset)
										 ,flength
										 ,_ctx->next_in
										 ,ios_in.avail_in);
			if (ios_in.rcode == IOS_ERR)
				break;
			if ((ios_in.rcode == IOS_NO_MEM) && (_ctx->total_out == 0))
				break;
			
			ios_in.total_in += _ctx->total_in;
			ios_in.avail_in -= _ctx->total_in;
			
			cgArray.In((uint32)_ctx->total_out);
			
			DSTF::Save(IOS_clr(&ios_out),_out,cgArray);
			cgArray.Out((uint32)ios_out.total_in);
			
			ios_in.total_out += ios_out.total_out;
		}while((ios_in.avail_in > 0) && (cgArray.Unused() > 0));
	}while((_ctx->Execute == _ctx->Final) && (ios_in.rcode == IOS_OK));
	ios_in.avail_out = ios_out.avail_out;
	IOS_update(_ios,ios_in);
	return(IOS_update(_ios,IOS_update(&ios_out.rcode, ios_in.rcode)));
};
//------------------------------------------------------------------------------------------//
template <typename DS_CTX>
inline IOSE DSTF_DIR<DS_CTX>::DoTransform(IOS* _ios,const UVOut& _out,const uint8* data,const uint64& length){
	cgCTX.Execute = cgCTX.Update;
	return(Save(&cgCTX,_ios,_out,data,length));
};
//------------------------------------------------------------------------------------------//
template <typename DS_CTX> inline IOSE DSTF_DIR<DS_CTX>::DoFinal(IOS* _ios,const UVOut& _out){
	IOSE	rcode = IOS_OK;
	cgCTX.Execute = cgCTX.Final;
	IOS_update(&rcode,		Save(&cgCTX,_ios,_out,nullptr,0));
	IOS_update(&rcode,DSTF::Save(		_ios,_out,nullptr,0));
	cgCTX.ReInit(&cgCTX);
	cgArray.Reset();
	return(rcode);
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
template <typename T_A,typename T_B> DSTF_AB_FRAME<T_A,T_B>::DSTF_AB_FRAME(void) : DSTF_DIR(){
	TNFP::SetSelfName("DSTF_AB_FRAME");
	SetUpName("");
	EnableAB();
};
//------------------------------------------------------------------------------------------//
template <typename T_A,typename T_B> inline	DSTF_AB_FRAME<T_A,T_B>& DSTF_AB_FRAME<T_A,T_B>::InitSize(uint32 size){
	cgA.InitSize(size);
	cgB.InitSize(size);
	return(*this);
};
//------------------------------------------------------------------------------------------//
template <typename T_A,typename T_B> inline void DSTF_AB_FRAME<T_A,T_B>::SetSelfName(const STDSTR& strName){
	selfName = strName;
	SetUpName("");
};
//------------------------------------------------------------------------------------------//
template <typename T_A,typename T_B> inline void DSTF_AB_FRAME<T_A,T_B>::SetUpName(const STDSTR& strName){
	fatherName = strName;
	cgA.SetUpName(GetFullName(this));
	cgB.SetUpName(GetFullName(this));
};
//------------------------------------------------------------------------------------------//
template <typename T_A,typename T_B> inline DSTF_AB_FRAME<T_A,T_B>& DSTF_AB_FRAME<T_A,T_B>::InitCFG(uint32 cfg,const void* par){
	DSTF_DIRBASE::InitCFG(cfg,par);
	
	if (B_ChkFLAG32(cfg, CFG_INIT_A))
		cgA.InitCFG(cfg,par);
	if (B_ChkFLAG32(cfg, CFG_INIT_B))
		cgB.InitCFG(cfg,par);
	return(*this);
};
//------------------------------------------------------------------------------------------//
template <typename T_A,typename T_B> inline DSTF_AB_FRAME<T_A,T_B>& DSTF_AB_FRAME<T_A,T_B>::_SetOut(const UVOut &_out){
	DSTF_DIR::_SetOut(_out);
	cgA._SetOut(_out);
	cgB._SetOut(_out);
	return(*this);
};
//------------------------------------------------------------------------------------------//
template <typename T_A,typename T_B> inline DSTF_AB_FRAME<T_A,T_B>& DSTF_AB_FRAME<T_A,T_B>::_SetIOS(IOS *_ios){
	DSTF_DIR::_SetIOS(_ios);
	cgA._SetIOS(_ios);
	cgB._SetIOS(_ios);
	return(*this);
};
//------------------------------------------------------------------------------------------//
template <typename T_A,typename T_B> inline	void DSTF_AB_FRAME<T_A,T_B>::DisableA(void){
	
	ClrSFlag(DSTF_blEnA);
};
//------------------------------------------------------------------------------------------//
template <typename T_A,typename T_B> inline	void DSTF_AB_FRAME<T_A,T_B>::DisableB(void){
	
	ClrSFlag(DSTF_blEnB);
};
//------------------------------------------------------------------------------------------//
template <typename T_A,typename T_B> inline	void DSTF_AB_FRAME<T_A,T_B>::EnableAB(void){
	
	SetSFlag(DSTF_blEnA | DSTF_blEnB);
};
//------------------------------------------------------------------------------------------//
template <typename T_A,typename T_B>
IOSE DSTF_AB_FRAME<T_A,T_B>::DoTransform(IOS* _ios,const UVOut& _out,const uint8* data,const uint64& length){
	IOSE rcode;
	if (CheckSFlag(DSTF_blEnA)){
		if (CheckSFlag(DSTF_blEnB)){
			if (B_ChkFLAG32(cgCTX.cfg,CFG_ATOB)){
				rcode = cgA.Transform(_ios,OUD(&cgB,_out),data,length);
			}
			else{
				rcode = cgB.Transform(_ios,OUD(&cgA,_out),data,length);
			}
		}
		else{
			rcode = cgA.Transform(_ios,_out,data,length);
		}
	}
	else{
		if (CheckSFlag(DSTF_blEnB)){
			rcode = cgB.Transform(_ios,_out,data,length);
		}
		else{
			rcode = DSTF::Transform(_ios,_out,data,length);
		}
	}
	return(rcode);
};
//------------------------------------------------------------------------------------------//
template <typename T_A,typename T_B>
IOSE DSTF_AB_FRAME<T_A,T_B>::DoFinal(IOS* _ios,const UVOut& _out){
	IOSE rcode;
	
	if (CheckSFlag(DSTF_blEnA)){
		if (CheckSFlag(DSTF_blEnB)){
			if (B_ChkFLAG32(cgCTX.cfg,CFG_ATOB)){
				rcode = cgA.Final(_ios,OUD(&cgB,_out));
			}
			else{
				rcode = cgB.Final(_ios,OUD(&cgA,_out));
			}
		}
		else{
			rcode = cgA.Final(_ios,_out);
		}
	}
	else{
		if (CheckSFlag(DSTF_blEnB)){
			rcode = cgB.Final(_ios,_out);
		}
		else{
			rcode = DSTF::Final(_ios,_out);
		}
	}
	return(rcode);
};
//------------------------------------------------------------------------------------------//









#include "DS_STRING.h"
//------------------------------------------------------------------------------------------//
namespace DSTF_TEST {
	static inline								void DIR_GetLayer	(uint32& layer);
	template <typename T>						void DIR_GetLayer	(uint32& layer,T* first);
	template <typename T,typename... Args>		void DIR_GetLayer	(uint32& layer,T* first,Args* ...args);
	
	static inline								void DIR_SetDir		(DSTF_DIRBASE** _dir,const uint32& layer,uint32& i);
	template <typename T_DIR>					void DIR_SetDir		(DSTF_DIRBASE** _dir,const uint32& layer,uint32& i,T_DIR* first);
	template <typename T_DIR,typename... Args>	void DIR_SetDir		(DSTF_DIRBASE** _dir,const uint32& layer,uint32& i,T_DIR* first,Args* ...args);
	
	static inline				bool32 DIR_Test		(IOS* _ios,STDSTR* _out,const STDSTR& _in
													 ,const STDSTR& name,uint32 layer,uint32 round
													 ,DSTF_DIRBASE** dirList,_UVV_TRAN* outList);
};
//------------------------------------------------------------------------------------------//
static inline void DSTF_TEST::DIR_GetLayer(uint32& layer){
	layer = 0;
};
//------------------------------------------------------------------------------------------//
template <typename T>void DSTF_TEST::DIR_GetLayer(uint32& layer,T* first){
	DIR_GetLayer(layer);
	++ layer;
};
//------------------------------------------------------------------------------------------//
template <typename T,typename... Args> void DSTF_TEST::DIR_GetLayer(uint32& layer,T* first,Args* ...args){
	DIR_GetLayer(layer,args...);
	++ layer;
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
static inline void DSTF_TEST::DIR_SetDir(DSTF_DIRBASE** _dir,const uint32& layer,uint32& i){
};
//------------------------------------------------------------------------------------------//
template <typename T_DIR> void DSTF_TEST::DIR_SetDir(DSTF_DIRBASE** _dir,const uint32& layer,uint32& i,T_DIR* first){
	_dir[i] = (DSTF_DIRBASE*)&first[0];
	_dir[layer * 2 - 1 - i] = (DSTF_DIRBASE*)&first[1];
	++i;
};
//------------------------------------------------------------------------------------------//
template <typename T_DIR,typename... Args> void DSTF_TEST::DIR_SetDir(DSTF_DIRBASE** _dir,const uint32& layer,uint32& i,T_DIR* first,Args* ...args){
	DIR_SetDir(_dir,layer,i,first);
	DIR_SetDir(_dir,layer,i,args...);
};
//------------------------------------------------------------------------------------------//







//------------------------------------------------------------------------------------------//
template <typename T_DIR> T_DIR* DSTF_TEST::DIR_Create(const STDSTR& name,T_DIR* _nullptr,uint32 size,uint32 cfg,void* p){
	T_DIR *_dir = new T_DIR[2];
	
	_dir[0].SetSelfName(name + "_0");
	_dir[0].InitSize(size);
	_dir[0].InitCFG(DSTF_DIRBASE::CFG_DIR0 | cfg, p);
	
	_dir[1].SetSelfName(name + "_1");
	_dir[1].InitSize(size);
	_dir[1].InitCFG(DSTF_DIRBASE::CFG_DIR1 | cfg, p);
	
	return(_dir);
};
//------------------------------------------------------------------------------------------//
static inline bool32 DSTF_TEST::DIR_Test(IOS* _ios,STDSTR* _out,const STDSTR& _in
						   ,const STDSTR& name,uint32 layer,uint32 round
						   ,DSTF_DIRBASE** dirList,_UVV_TRAN* outList){
	uint32  i = layer * 2 - 1;
	bool32	err = G_TRUE;
	
	outList[i] = OUD(&DS_IO_NODE::GetDSIOList(),_out);
	while(i-- > 0)
		outList[i] = OUD(dirList[i + 1],outList[i + 1]);
	printf("----------------------------------------------------------\n");
	i = 0;
	do{
		*_out = "";
		
		dirList[0]->AllIn(IOS_clr(_ios),outList[0],_in);
		if (*_out == _in){
			printf("DIR_Test<%s> round %d is ok. In/Out=%llu/%llu\n",name.c_str(),i,_ios->total_in,_ios->total_out);
		}
		else{
			err = G_FALSE;
			printf("DIR_Test<%s> round %d is fail.\n",name.c_str(),i);
		}
	}while(++i < round);
	
	if (err){
		printf("DIR_Test<%s> is ok.\n",name.c_str());
	}
	else{
		printf("DIR_Test<%s> is fail.\n",name.c_str());
	}
	
	return(err);
};
//------------------------------------------------------------------------------------------//
template <typename T_DIR> bool32 DSTF_TEST::DIR_Test(IOS* _ios,STDSTR* _out,const STDSTR& _in
													 ,const STDSTR& name,uint32 layer,uint32 round
													 ,T_DIR* _nullptr,uint32 size,uint32 cfg,void* p){
	T_DIR			*_dir = new T_DIR[layer * 2];
	DSTF_DIRBASE	**dirList = new DSTF_DIRBASE*[layer * 2];
	_UVV_TRAN		*outList = new _UVV_TRAN[layer * 2];
	uint32			i = 0;
	bool32			err = G_TRUE;
	
	do{
		_dir[i].SetSelfName(name + Str_ToStr(i));
		_dir[i].InitSize(size);
		_dir[i].InitCFG(DSTF_DIRBASE::CFG_DIR0 | cfg, p);
		dirList[i] = (DSTF_DIRBASE*)&_dir[i];
	}while(++i < layer);
	
	layer *= 2;
	do{
		_dir[i].SetSelfName(name + Str_ToStr(i));
		_dir[i].InitSize(size);
		_dir[i].InitCFG(DSTF_DIRBASE::CFG_DIR1 | cfg, p);
		dirList[i] = (DSTF_DIRBASE*)&_dir[i];
	}while(++i < layer);
	
	layer /= 2;
	
	err = DIR_Test(_ios,_out,_in,name,layer,round,dirList,outList);
	
	delete []_dir;
	delete []dirList;
	delete []outList;
	return(err);
};
//------------------------------------------------------------------------------------------//
template<typename... Args> bool32 DSTF_TEST::DIR_Test(IOS* _ios,STDSTR* _out,const STDSTR& _in
													  ,const STDSTR& name,uint32 round
													  ,Args* ...args){
	uint32 	layer,i;
	bool32	err = G_TRUE;
	
	DIR_GetLayer(layer = 0,args...);
	
	DSTF_DIRBASE	**dirList = new DSTF_DIRBASE*[layer * 2];
	_UVV_TRAN		*outList = new _UVV_TRAN[layer * 2];
	
	DIR_SetDir(dirList,layer,i = 0,args...);
	
	err = DIR_Test(_ios,_out,_in,name,layer,round,dirList,outList);
	
	delete []dirList;
	delete []outList;
	return(err);
};
//------------------------------------------------------------------------------------------//
#endif /* DS_Transform_h */
#endif /* DS_Transform_hpp */
