//
//  ADS_Buffer.cpp
//  SDT
//
//  Created by Leif Wen on 01/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "stdafx.h"
//------------------------------------------------------------------------------------------//
#include "ADS_Buffer.h"
//------------------------------------------------------------------------------------------//
#ifdef ADS_Buffer_h
//------------------------------------------------------------------------------------------//
SBUF::SBUF(void) : TNF(){;};
//------------------------------------------------------------------------------------------//
SBUF::SBUF(uint32 size) : TNF(){array.InitSize(size);};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
static SBUFPOOL* GetSBUFPOOL(void){
	static SBUFPOOL pool;
	return &pool;
}
//------------------------------------------------------------------------------------------//
SBUFPOOL::~SBUFPOOL(void){
	CleanDownTree(this, this);
	CleanTrash(this);
};
//------------------------------------------------------------------------------------------//
TNF* SBUFPOOL::CreateNode(void){
	return (new SBUF());
};
//------------------------------------------------------------------------------------------//
SBUF* SBUFPOOL::GetSBUF(void){
	SBUFPOOL *pool = GetSBUFPOOL();

	if (pool != nullptr){
		SBUF* node;
		node = static_cast<SBUF*>(pool->GetNewNode());
		pool->AppendDownNode(node);
		return(node);
	}
	return nullptr;
}
//------------------------------------------------------------------------------------------//
uint32 SBUFPOOL::Put(uint32 uid,const char* data,uint32 len){
	SBUFPOOL *pool = GetSBUFPOOL();
	SBUF *buf;
	buf = static_cast<SBUF*>(FindInDownChainByUniqueID(pool, uid));
	if (buf != nullptr){
		buf->array.InitSize(buf->array.Used() + len);
		return(buf->array.Put((uint8*)data, len));
	}
	return 0;
};
//------------------------------------------------------------------------------------------//
#endif /* ADS_Buffer_h */
