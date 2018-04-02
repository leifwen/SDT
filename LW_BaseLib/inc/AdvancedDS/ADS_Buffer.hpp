//
//  ADS_Buffer.hpp
//  SDT
//
//  Created by Leif Wen on 01/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#ifndef ADS_Buffer_hpp
#define ADS_Buffer_hpp
//------------------------------------------------------------------------------------------//
#include "ADS_Buffer.h"
#ifdef ADS_Buffer_h
//------------------------------------------------------------------------------------------//
inline void		SBUF::InLock_set(G_LOCK blLock){InUse_set(blLock);};
inline void		SBUF::InLock_clr(G_LOCK blLock){InUse_clr(blLock);};
inline bool32	SBUF::InLock_try(G_LOCK blLock){return(InUse_try(blLock));};
//------------------------------------------------------------------------------------------//
inline void		SBUF::OutLock_set(G_LOCK blLock){InDoing_set(blLock);};
inline void		SBUF::OutLock_clr(G_LOCK blLock){InDoing_clr(blLock);};
inline bool32	SBUF::OutLock_try(G_LOCK blLock){return(InDoing_try(blLock));};
//------------------------------------------------------------------------------------------//
inline			void	SBUF::Empty		(void)		{cgAlreadyReadNum = 0;return(cgArray.Empty());};
inline	const	uint32&	SBUF::MaxSize	(void)const	{return(cgArray.MaxSize());};
inline			uint32	SBUF::Unused	(void)const	{return(cgArray.Unused());};
inline			uint32	SBUF::Used		(void)const	{return(cgArray.Used());};
inline			bool32	SBUF::IsEmpty	(void)const	{return(cgArray.IsEmpty());};
inline			bool32	SBUF::IsFull	(void)const	{return(cgArray.IsFull());};
inline			ARRAY&	SBUF::GetArray	(void)		{return(cgArray);};
//------------------------------------------------------------------------------------------//
inline SBUF& SBUF::InitSize(uint32 size){
	cgAlreadyReadNum = 0;
	cgArray.InitSize(size);
	return(*this);
};
//------------------------------------------------------------------------------------------//









//------------------------------------------------------------------------------------------//
inline	const	uint32&	DBUF_FRAME::RxSBUFMaxSize	(void)const	{return(cgRxSBUF.MaxSize());};
inline	const	uint32&	DBUF_FRAME::TxSBUFMaxSize	(void)const	{return(cgTxSBUF.MaxSize());};
inline			uint32	DBUF_FRAME::UnreadBytes		(void)const	{return(cgRxSBUF.Used());};
inline			uint32	DBUF_FRAME::UnsentBytes		(void)const	{return(cgTxSBUF.Used());};
inline	const	uint64&	DBUF_FRAME::FwBytes			(void)const	{return(cgFwBytes);};
inline	const	uint64&	DBUF_FRAME::RxBytes			(void)const	{return(cgRxBytes);};
inline	const	uint64&	DBUF_FRAME::TxBytes			(void)const	{return(cgTxBytes);};
//------------------------------------------------------------------------------------------//
inline	uint32	DBUF_FRAME::Put(IOSTATUS* _ios,const UVIn&  _in)				{return(cgTxSBUF.Put(_ios,_in,G_LOCK_ON));};
inline	uint32	DBUF_FRAME::Get(IOSTATUS* _ios,const UVOut& _out,uint32 num)	{return(cgRxSBUF.Get(_ios,_out,num,G_LOCK_OFF));};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
inline DBUF* DBUF::GetNewChild	(void){return(AddChild(static_cast<DBUF*>(GetNewNode())));};
inline DBUF* DBUF::GetFDB		(void){return(static_cast<DBUF*>(GetUp(this)));};
inline DBUF* DBUF::GetSelDB		(void){return(cgSelDB);};
//------------------------------------------------------------------------------------------//
inline DBUF* DBUF::AddChild(DBUF* sdb){
	AddNode(sdb);
	if (GetdRNodeID(sdb) == 0)
		ChildSetSel(sdb);
	return(sdb);
};
//------------------------------------------------------------------------------------------//
inline void DBUF::JoinTo(DBUF* fdb){
	if (fdb != nullptr)
		fdb->AddChild(this);
};
//------------------------------------------------------------------------------------------//
inline void DBUF::SetblSelected(void){
	if (GetFDB() != nullptr){
		GetFDB()->ChildSetSel(this);
	}
	else{
		TREE_NODE::SetblSelected();
	}
};
//------------------------------------------------------------------------------------------//
inline void DBUF::ClrblSelected(void){
	if (GetFDB() != nullptr){
		GetFDB()->ChildClrSel(this);
	}
	else{
		TREE_NODE::ClrblSelected();
	}
};
//------------------------------------------------------------------------------------------//
inline void DBUF::RemoveSelf(void){
	ClrblSelected();
	DBUF_FRAME::RemoveSelf();
};
//------------------------------------------------------------------------------------------//
template <typename T_FUN,typename T_RET> static inline	T_RET& RunFunInLock(DBUF* db,T_FUN fun,T_RET* ret){
	db->InUse_set();
	*ret = (db->GetSelDB()->*fun)();
	db->InUse_clr();
	return(*ret);
};
//------------------------------------------------------------------------------------------//
template <typename T_FUN> static inline void RunFunInLock(DBUF* db,T_FUN fun){
	db->InUse_set();
	(db->GetSelDB()->*fun)();
	db->InUse_clr();
};
//------------------------------------------------------------------------------------------//
inline uint32 DBUF::SSend(const UVIn& _in){
	uint32 ret;
	InUse_set();
	ret = cgSelDB->Put(nullptr,_in);
	InUse_clr();
	return(ret);
};
//------------------------------------------------------------------------------------------//
inline uint32 DBUF::SRead(const UVOut& _out,uint32 num){
	uint32 ret;
	InUse_set();
	ret = cgSelDB->Get(nullptr,_out,num);
	InUse_clr();
	return(ret);
};
//------------------------------------------------------------------------------------------//
inline void		DBUF::SEmpty		(void){return(RunFunInLock(cgSelDB,&DBUF_FRAME::Empty));};
inline uint32	DBUF::SRxSBUFMaxSize(void){uint32 ret;return(RunFunInLock(this,&DBUF_FRAME::RxSBUFMaxSize,&ret));};
inline uint32	DBUF::STxSBUFMaxSize(void){uint32 ret;return(RunFunInLock(this,&DBUF_FRAME::TxSBUFMaxSize,&ret));};
inline uint32	DBUF::SUnreadBytes	(void){uint32 ret;return(RunFunInLock(this,&DBUF_FRAME::UnreadBytes,&ret));};
inline uint32	DBUF::SUnsentBytes	(void){uint32 ret;return(RunFunInLock(this,&DBUF_FRAME::UnsentBytes,&ret));};
inline uint64	DBUF::SFwBytes		(void){uint64 ret;return(RunFunInLock(this,&DBUF_FRAME::FwBytes,&ret));};
inline uint64	DBUF::SRxBytes		(void){uint64 ret;return(RunFunInLock(this,&DBUF_FRAME::RxBytes,&ret));};
inline uint64	DBUF::STxBytes		(void){uint64 ret;return(RunFunInLock(this,&DBUF_FRAME::TxBytes,&ret));};
//------------------------------------------------------------------------------------------//
#endif /* ADS_Buffer_h */
#endif /* ADS_Buffer_hpp */
