//
//  ADS_Buffer.h
//  SDT
//
//  Created by Leif Wen on 01/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

//------------------------------------------------------------------------------------------//
#include "DS_Lock.h"
#include "DS_ARRAY.h"
#include "DS_Tree.h"
#if defined DS_Lock_h && defined DS_ARRAY_h && defined DS_Tree_h
//------------------------------------------------------------------------------------------//
#ifndef ADS_Buffer_h
#define ADS_Buffer_h
#ifdef ADS_Buffer_h
//------------------------------------------------------------------------------------------//
class SBUF : public TNF{
	public:
				 SBUF(void);
				 SBUF(uint32 size);
		virtual ~SBUF(void){;};
	public:
		ARRAY			array;
};
//------------------------------------------------------------------------------------------//
class SBUFPOOL : public TNFP{
	public:
				 SBUFPOOL(void) : TNFP(){;};
		virtual ~SBUFPOOL(void);
	public:
		virtual	TNF*		CreateNode			(void);
		static	SBUF* 		GetSBUF				(void);
		static	uint32 		Put					(uint32 uid,const char* data,uint32 len);
};
//------------------------------------------------------------------------------------------//
#endif /* ADS_Buffer_h */
#endif /* ADS_Buffer_h */
#endif
