//
//  BasicDefine.h
//  SDT
//
//  Created by Leif Wen on 17/01/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#ifndef BasicDefine_h
#define BasicDefine_h
//------------------------------------------------------------------------------------------//
#include "Config.h"
#include <string>
#include <iostream>
#ifdef CommonDefH_Unix
	#include <unistd.h>
#endif
#ifdef CommonDefH_Linux
	#include <string.h>
#endif
//------------------------------------------------------------------------------------------//
typedef const 		char				char8;
typedef unsigned	char				uint8;
typedef	signed		char				int8;
typedef	unsigned	short int			uint16;
typedef	signed		short int			int16;
typedef	unsigned	int					uint32;
typedef	signed		int					int32;
typedef	signed		long long			int64;
typedef	unsigned	long long			uint64;
typedef				int32				bool32;
typedef				std::string 		STDSTR;
typedef				STDSTR::size_type 	STRSIZE;
//typedef	signed		__int64		int64;
//------------------------------------------------------------------------------------------//
enum 			{G_FALSE = 0		,G_TRUE};
enum G_ENDIAN	{G_ENDIAN_LITTLE = 0,G_ENDIAN_BIG};
enum G_ESCAPE	{G_ESCAPE_OFF = 0	,G_ESCAPE_ON};
enum G_SPACE	{G_SPACE_OFF = 0	,G_SPACE_ON};
enum G_HA		{G_HEX = 0			,G_ASCII};
enum CMD_TAIL	{CMD_NONE = 0,CMD_R,CMD_N,CMD_RN,};
//------------------------------------------------------------------------------------------//
#define BD_FLAG64(offset)				(((uint64)0x01) << (offset))
#define BD_FLAG32(offset)				(((uint32)0x01) << (offset))

static inline uint64	B_FLAG64	(uint64		offset)						{return((uint64)0x01 << offset);};
static inline uint64&	B_SetFLAG64	(uint64&	variable,uint64 u64)		{return(variable |= u64);};
static inline uint64&	B_ClrFLAG64	(uint64&	variable,uint64 u64)		{return(variable &= (~u64));};
static inline bool32	B_ChkFLAG64	(uint64 	variable,uint64 u64)		{return((variable & u64) != 0);};
static inline uint64	B_FLAG32	(uint32		offset)						{return((uint64)0x01 << offset);};
static inline uint32&	B_SetFLAG32	(uint32&	variable,uint32 u32)		{return(variable |= u32);};
static inline uint32&	B_ClrFLAG32	(uint32&	variable,uint32 u32)		{return(variable &= (~u32));};
static inline bool32	B_ChkFLAG32	(uint32 	variable,uint32 u32)		{return((variable & u32) != 0);};
//------------------------------------------------------------------------------------------//
#endif /* BasicDefine_h */
