/*
 * Copyright (c) 2012-2014 
 * All rights reserved.
 *
 * File Name: ParRecord.h
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2013.11.13
 *
*/

#ifndef ParRecordH
#define ParRecordH
//------------------------------------------------------------------------------------------//
#include "BasicDef.h"
#include "BasicClass.h"
//------------------------------------------------------------------------------------------//
#define READINITYPE_NONE				((uint32)0x00000000)
#define READINITYPE_AddSingleCommand	((uint32)0x00000001)
#define READINITYPE_CoverSingleCommand	(((uint32)0x00000001)<<1)
#define READINITYPE_AddGroupCommand		(((uint32)0x00000001)<<2)
#define READINITYPE_CoverGroupCommand	(((uint32)0x00000001)<<3)
#define READINITYPE_AddDeviceList		(((uint32)0x00000001)<<4)
#define READINITYPE_CoverDeviceList		(((uint32)0x00000001)<<5)
//------------------------------------------------------------------------------------------//
class GC_LIST;
class SC_LIST;
class IPCOMLIST;
//------------------------------------------------------------------------------------------//
class TExpandPar{
	public:
		 TExpandPar(void){
			g_GCList = nullptr;
			g_SCList = nullptr;
			g_IPComList = nullptr;
		 };
		virtual ~TExpandPar(void){;};
	public:
		IPCOMLIST	*g_IPComList;
		SC_LIST		*g_SCList;
		GC_LIST		*g_GCList;
};
//------------------------------------------------------------------------------------------//
class PR_Record{
	public:
				 PR_Record(void){;}
		virtual ~PR_Record(void){;};
	public:
		TExpandPar	cg_ExpandPar;
		void	Init(TExpandPar *cExpandPar);

		void	WriteScriptToIniFile		(const std::string &strFileName,uint32 ReadPart);
		void	WriteScriptToIniFileV0_5	(const std::string &strFileName,uint32 ReadPart);
		void	WriteScriptToIniFileV0_4	(const std::string &strFileName,uint32 ReadPart);
		void	WriteScriptToIniFileV0_3	(const std::string &strFileName,uint32 ReadPart);
		void	WriteScriptToIniFileV0_2	(const std::string &strFileName);
		void	ReadScriptFromIniFile		(const std::string &strFileName,uint32 ReadPart);
		void	ReadScriptFromIniFileV0_5	(std::string *strInput,uint32 ReadPart);
		void	ReadScriptFromIniFileV0_4	(std::string *strInput,uint32 ReadPart);
		void	ReadScriptFromIniFileV0_3	(std::string strInput,uint32 ReadPart);
		void	ReadScriptFromIniFileV0_2	(std::string strInput,uint32 ReadPart);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif
