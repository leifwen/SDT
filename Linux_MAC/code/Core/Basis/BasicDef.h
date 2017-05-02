/*
 * Copyright (c) 2012-2014 
 * All rights reserved.
 *
 * File Name: BasicDef.h
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2013.12.25
 *
*/

#ifndef BasicDefH
#define BasicDefH
//------------------------------------------------------------------------------------------//
//#define CommonDefH_VC2013
//#define CommonDefH_Linux
#define CommonDefH_MAC
//------------------------------------------------------------------------------------------//
#ifdef CommonDefH_VC2013
	#define CommonDefH_VC
#endif
#ifdef CommonDefH_MAC
	#define CommonDefH_Unix
#endif
#ifdef CommonDefH_Linux
	#define CommonDefH_Unix
#endif
//------------------------------------------------------------------------------------------//
#define USE_MUTEXLOCK
#define USE_OPENSSL

#ifdef CommonDefH_VC
	#define CommonDefH_EnableLicense
	#define CommonDefH_EnableSMC
#endif
#ifdef CommonDefH_Unix
	#undef USE_MUTEXLOCK
	//#undef USE_OPENSSL
#endif
#ifdef CommonDefH_Linux
	//#undef USE_OPENSSL
#endif
//------------------------------------------------------------------------------------------//
#ifndef CommonDefH_VC
	//#define LOGPRINT
#endif
//------------------------------------------------------------------------------------------//
#include <string>
#include <iostream>
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
#ifdef CommonDefH_Unix
#include <unistd.h>
#endif
//------------------------------------------------------------------------------------------//
typedef const 		char		char8;
typedef unsigned	char		uint8;
typedef	signed		char		int8;
typedef	unsigned	short int	uint16;
typedef	signed		short int	int16;
typedef	unsigned	int			uint32;
typedef	signed		int			int32;
typedef	signed		long long	int64;
typedef	unsigned	long long	uint64;
typedef				std::string STDSTR;
//typedef	signed		__int64		int64;
//------------------------------------------------------------------------------------------//
enum G_ENDIAN	{G_ENDIAN_LITTLE = 0,G_ENDIAN_BIG};
enum G_ESCAPE	{G_ESCAPE_OFF = 0	,G_ESCAPE_ON};
enum G_SPACE	{G_SPACE_OFF = 0	,G_SPACE_ON};
enum G_APPEND	{G_APPEND_OFF = 0	,G_APPEND_ON};
enum G_HA		{G_HEX = 0			,G_ASCII};
enum CMD_TAIL	{CMD_NONE = 0,CMD_R,CMD_N,CMD_RN,};
//------------------------------------------------------------------------------------------//
#include "GlobalDef.h"
//------------------------------------------------------------------------------------------//
#endif
