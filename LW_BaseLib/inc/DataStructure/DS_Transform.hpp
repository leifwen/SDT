//
//  DS_Transform.hpp
//  SDT
//
//  Created by Leif Wen on 19/01/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#ifndef DS_Transform_hpp
#define DS_Transform_hpp
//------------------------------------------------------------------------------------------//
#include "DS_Transform.h"
#ifdef DS_Transform_h
//------------------------------------------------------------------------------------------//
#define	uInt32_MaxSize	((uint32)-1)
//------------------------------------------------------------------------------------------//
static inline IOSTATUS* IOSTATUS_Add(IOSTATUS* _out,const IOSTATUS& _in){
	if (_out != nullptr){
		_out->avail_in  =  _in.avail_in;
		_out->total_in  += _in.total_in;
		_out->avail_out =  _in.avail_out;
		_out->total_out += _in.total_out;
		_out->status    =  _in.status;
	}
	return(_out);
};
//------------------------------------------------------------------------------------------//
static inline IOSTATUS* IOSTATUS_Clr(IOSTATUS* _ios){
	if (_ios != nullptr){
		_ios->avail_in  = 0;
		_ios->total_in  = 0;
		_ios->avail_out = 0;
		_ios->total_out = 0;
		_ios->status    = IOS_OK;
	}
	return(_ios);
};
//------------------------------------------------------------------------------------------//
inline DS_IO_NODE& DS_IO_NODE::GetDSIOList(void){
	static DS_IO_NODE	sgIOList;
	return(sgIOList);
};
//------------------------------------------------------------------------------------------//
inline DS_IO_NODE& DS_IO_NODE::Transform(IOSTATUS* _ios,const UVOut& _out,const UVIn& _in){
	SetSFlag(DSTF_blStart);
	GetDSIOList().DoConvert(this,_ios,_out,_in);
	return(*this);
};
//------------------------------------------------------------------------------------------//
inline DS_IO_NODE& DS_IO_NODE::Transform(IOSTATUS* _ios,const UVOut& _out,const uint8* data,const uint64& length){
	SetSFlag(DSTF_blStart);
	return(DoTransform(_ios,_out,data,length));
};
//------------------------------------------------------------------------------------------//
inline DS_IO_NODE& DS_IO_NODE::Final(IOSTATUS* _ios,const UVOut& _out){
	if (CheckSFlag(DSTF_blStart) == G_FALSE)
		return(*this);
	ClrSFlag(DSTF_blStart);
	return(DoFinal(_ios,_out));
};
//------------------------------------------------------------------------------------------//
inline uint64 DS_IO_NODE::GetInLength(const UVIn& _in){
	uint64	num;
	GetDSIOList().DoGetInLength(&num,_in);
	return(num);
};
//------------------------------------------------------------------------------------------//
inline void DS_IO_NODE::Save(IOSTATUS* _ios,const UVOut& _out,const uint8* data,const uint64& length){
	GetDSIOList().DoSave(_ios,_out,data,length);
}
//------------------------------------------------------------------------------------------//
inline void DS_IO_NODE::Save(IOSTATUS* _ios,const UVOut& _out,const UVIn& _in){
	GetDSIOList().Transform(_ios,_out,_in);
}
//------------------------------------------------------------------------------------------//
inline DS_IO_NODE& DS_IO_NODE::DoTransform(IOSTATUS* _ios,const UVOut& _out,const uint8* data,const uint64& length){
	Save(_ios,_out,data,length);
	return(*this);
};
//------------------------------------------------------------------------------------------//
inline DS_IO_NODE& DS_IO_NODE::DoFinal(IOSTATUS* _ios,const UVOut& _out){
	Save(_ios,_out,nullptr,0);
	return(*this);
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
static inline STARTUP Startup(IOSTATUS* _ios,const UVOut& _out){
	
	return{_ios,(UVOut*)&_out};
};
//------------------------------------------------------------------------------------------//










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
inline DSTF& DSTF::_Startup(IOSTATUS* _ios, const UVOut& _out){
	cgStartup.ios	= _ios;
	cgStartup.uvOut	= (UVOut*)&_out;
		
	return(*this);
};
//------------------------------------------------------------------------------------------//
inline DSTF& DSTF::AllIn(IOSTATUS* _ios,const UVOut& _out,const UVIn& _in){
	Transform(_ios,_out,_in);
	Final(_ios,_out);
	return(*this);
};
//------------------------------------------------------------------------------------------//









//------------------------------------------------------------------------------------------//
static inline DSTF_DIR_CTX* DSTF_DIR_CTX_Clr(DSTF_DIR_CTX* ctx){
	IOSTATUS_Clr(ctx);
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
bool32 DSTF_DIR<DS_CTX>::Save(DS_CTX* _ctx,IOSTATUS* _ios,const UVOut& _out,const uint8* data,const uint64& length){
	uint32		flength,offset,slength;
	IOSTATUS	ios_in,ios_out;
	
	_ctx->next_in = (uint8*)data;
	
	IOSTATUS_Clr(&ios_in);
	IOSTATUS_Clr(&ios_out);
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
			slength = cgArray.CalcInCapacity(flength,offset);
			
			ios_in.status = _ctx->Execute(_ctx
										  ,cgArray.GetPointer(offset)
										  ,flength
										  ,_ctx->next_in
										  ,ios_in.avail_in);
			if (ios_in.status < 0)
				break;
			if ((ios_in.status == 0) && (_ctx->total_out == 0))
				break;
			
			if ((slength > 0) && (_ctx->avail_out == 0) && (_ctx->avail_in > 0))
				ios_in.status = _ctx->Execute(_ctx
											  ,cgArray.GetPointer(offset)
											  ,slength
											  ,_ctx->next_in
											  ,ios_in.avail_in);
			ios_in.total_in += _ctx->total_in;
			ios_in.avail_in -= _ctx->total_in;
			
			cgArray.In((uint32)_ctx->total_out);
			
			IOSTATUS_Clr(&ios_out);
			DSTF::Save(&ios_out,_out,IUD(&cgArray,cgArray.Used(),0));
			cgArray.Out((uint32)ios_out.total_in);
			
			ios_in.total_out += ios_out.total_out;
		}while((ios_in.avail_in > 0) && (cgArray.Unused() > 0));
	}while((_ctx->Execute == _ctx->Final) && (ios_in.status == IOS_OK));
	ios_in.avail_out = ios_out.avail_out;
	ios_in.status = (ios_in.avail_in == 0);
	IOSTATUS_Add(_ios,ios_in);
	return(ios_in.status);
}
//------------------------------------------------------------------------------------------//
template <typename DS_CTX>
inline DSTF_DIR<DS_CTX>& DSTF_DIR<DS_CTX>::DoTransform(IOSTATUS* _ios,const UVOut& _out,const uint8* data,const uint64& length){
	cgCTX.Execute = cgCTX.Update;
	Save(&cgCTX,_ios,_out,data,length);
	return(*this);
}
//------------------------------------------------------------------------------------------//
template <typename DS_CTX> inline DSTF_DIR<DS_CTX>& DSTF_DIR<DS_CTX>::DoFinal(IOSTATUS* _ios,const UVOut& _out){
	cgCTX.Execute = cgCTX.Final;
	Save(&cgCTX,_ios,_out,nullptr,0);
	DSTF::Save(_ios,_out,nullptr,0);
	cgCTX.ReInit(&cgCTX);
	return(*this);
}
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
template <typename T_A,typename T_B> DSTF_AB_FRAME<T_A,T_B>::DSTF_AB_FRAME(void) : DSTF_DIR(){
	TNFP::SetSelfName("DSTF_AB_FRAME");
	SetFatherName("");
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
	SetFatherName("");
};
//------------------------------------------------------------------------------------------//
template <typename T_A,typename T_B> inline void DSTF_AB_FRAME<T_A,T_B>::SetFatherName(const STDSTR& strName){
	fatherName = strName;
	cgA.SetFatherName(GetFullName(this));
	cgB.SetFatherName(GetFullName(this));
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
template <typename T_A,typename T_B> inline DSTF_AB_FRAME<T_A,T_B>& DSTF_AB_FRAME<T_A,T_B>::_Startup(IOSTATUS* _ios, const UVOut& _out){
	DSTF_DIR::_Startup(_ios, _out);
	cgA._Startup(_ios, _out);
	cgB._Startup(_ios, _out);
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
DSTF_AB_FRAME<T_A,T_B>& DSTF_AB_FRAME<T_A,T_B>::DoTransform(IOSTATUS* _ios,const UVOut& _out,const uint8* data,const uint64& length){

	if (CheckSFlag(DSTF_blEnA)){
		if (CheckSFlag(DSTF_blEnB)){
			if (B_ChkFLAG32(cgCTX.cfg,CFG_ATOB)){
				cgA.Transform(_ios,OUD(&cgB,_out),data,length);
			}
			else{
				cgB.Transform(_ios,OUD(&cgA,_out),data,length);
			}
		}
		else{
			cgA.Transform(_ios,_out,data,length);
		}
	}
	else{
		if (CheckSFlag(DSTF_blEnB)){
			cgB.Transform(_ios,_out,data,length);
		}
		else{
			DSTF::Transform(_ios,_out,data,length);
		}
	}
	return(*this);
};
//------------------------------------------------------------------------------------------//
template <typename T_A,typename T_B>
DSTF_AB_FRAME<T_A,T_B>& DSTF_AB_FRAME<T_A,T_B>::DoFinal(IOSTATUS* _ios,const UVOut& _out){

	if (CheckSFlag(DSTF_blEnA)){
		if (CheckSFlag(DSTF_blEnB)){
			if (B_ChkFLAG32(cgCTX.cfg,CFG_ATOB)){
				cgA.Final(_ios,OUD(&cgB,_out));
			}
			else{
				cgB.Final(_ios,OUD(&cgA,_out));
			}
		}
		else{
			cgA.Final(_ios,_out);
		}
	}
	else{
		if (CheckSFlag(DSTF_blEnB)){
			cgB.Final(_ios,_out);
		}
		else{
			DSTF::Final(_ios,_out);
		}
	}
	return(*this);
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
	
	static inline				bool32 DIR_Test		(IOSTATUS* _ios,STDSTR* _out,const STDSTR& _in
													 ,const STDSTR& name,uint32 layer,uint32 round
													 ,DSTF_DIRBASE** dirList,_UVV_TRAN* outList);
}
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
static inline bool32 DSTF_TEST::DIR_Test(IOSTATUS* _ios,STDSTR* _out,const STDSTR& _in
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
		
		dirList[0]->AllIn(IOSTATUS_Clr(_ios),outList[0],_in);
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
template <typename T_DIR> bool32 DSTF_TEST::DIR_Test(IOSTATUS* _ios,STDSTR* _out,const STDSTR& _in
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
template<typename... Args> bool32 DSTF_TEST::DIR_Test(IOSTATUS* _ios,STDSTR* _out,const STDSTR& _in
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
