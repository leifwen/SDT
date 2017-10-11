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

//------------------------------------------------------------------------------------------//
#include "ComEnum.h"
#include "CommandGList.h"
#include "CommandSList.h"
//------------------------------------------------------------------------------------------//
#if defined CommandGListH && defined CommandSlistH && defined ComEnumH
#ifndef ParRecordH
#define ParRecordH
#ifdef ParRecordH
//------------------------------------------------------------------------------------------//
enum{
	READINITYPE_NONE				= 0,
	READINITYPE_AddSingleCommand	= BD_FLAG32(0),
	READINITYPE_CoverSingleCommand	= BD_FLAG32(1),
	READINITYPE_AddGroupCommand		= BD_FLAG32(2),
	READINITYPE_CoverGroupCommand	= BD_FLAG32(3),
	READINITYPE_AddDeviceList		= BD_FLAG32(4),
	READINITYPE_CoverDeviceList		= BD_FLAG32(5),
};
//------------------------------------------------------------------------------------------//
class PR_Record{
	public:
				 PR_Record(void){cgIPComList = nullptr;cgSCList = nullptr;cgGCList = nullptr;}
		virtual ~PR_Record(void){;};
	public:
		IPCOMLIST	*cgIPComList;
		SC_LIST		*cgSCList;
		GC_LIST		*cgGCList;
	public:
		void	Init(IPCOMLIST *iplist,SC_LIST *sclist,GC_LIST *gclist);

		void	WriteScriptToIniFile		(const STDSTR &strFileName,uint32 ReadPart);
		void	WriteScriptToIniFileV0_6	(const STDSTR &strFileName,uint32 ReadPart);
		void	WriteScriptToIniFileV0_5	(const STDSTR &strFileName,uint32 ReadPart);
		void	WriteScriptToIniFileV0_4	(const STDSTR &strFileName,uint32 ReadPart);
		void	WriteScriptToIniFileV0_3	(const STDSTR &strFileName,uint32 ReadPart);
		void	WriteScriptToIniFileV0_2	(const STDSTR &strFileName);
		void	ReadScriptFromIniFile		(const STDSTR &strFileName,uint32 ReadPart);
		void	ReadScriptFromIniFileV0_6	(STDSTR *strIn,uint32 ReadPart);
		void	ReadScriptFromIniFileV0_5	(STDSTR *strIn,uint32 ReadPart);
		void	ReadScriptFromIniFileV0_4	(STDSTR *strIn,uint32 ReadPart);
		void	ReadScriptFromIniFileV0_3	(STDSTR strIn,uint32 ReadPart);
		void	ReadScriptFromIniFileV0_2	(STDSTR strIn,uint32 ReadPart);
};
//------------------------------------------------------------------------------------------//
#endif
#endif
#endif

