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
//#define LOGPRINT
//------------------------------------------------------------------------------------------//
class RTREE_NODE;
void MyLogPrint(const RTREE_NODE *treeNode,const char * format, ...);
void MyLogPrint(const RTREE_NODE &treeNode,const char * format, ...);
void MyLogPrint(const RTREE_NODE *treeNode1,const RTREE_NODE *treeNode2,const char *format, ...);
void MyLogPrint(const RTREE_NODE &treeNode1,const RTREE_NODE &treeNode2,const char *format, ...);
void MyLogPrint(const char * format, ...);
//------------------------------------------------------------------------------------------//
#ifdef LOGPRINT
	#ifdef LOGPRINT_ENABLE
		#define ELogPrint(x,...) MyLogPrint(x,##__VA_ARGS__)
	#else
		#define ELogPrint(x,...) ;
	#endif
	#ifdef LOGPRINT_ENABLE2
		#define E2LogPrint(x,...) MyLogPrint(x,##__VA_ARGS__)
	#else
		#define E2LogPrint(x,...) ;
	#endif
#else
	#define ELogPrint(x,...) ;
	#define E2LogPrint(x,...) ;
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
//typedef	signed		__int64		int64;
enum	G_LOCK_VAILD	{G_LOCK_OFF = 0		,G_LOCK_ON};
enum	G_ESCAPE_VAILD	{G_ESCAPE_OFF = 0	,G_ESCAPE_ON};
enum	G_SPACE_VAILD	{G_SPACE_OFF = 0	,G_SPACE_ON};
enum	G_Endian_VAILD	{G_LITTLE_ENDIAN = 0,G_BIG_ENDIAN};
//------------------------------------------------------------------------------------------//
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef CommonDefH_Unix
	#include <unistd.h>
#endif
//------------------------------------------------------------------------------------------//
#include "GlobalDef.h"
//------------------------------------------------------------------------------------------//
#endif
