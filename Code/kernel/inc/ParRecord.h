//
//  ParRecord.h
//  SDT
//
//  Created by Leif Wen on 23/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "SList.h"
#include "GList.h"
#include "Commu_ComEnum.h"
//------------------------------------------------------------------------------------------//
#if defined SList_h && defined GList_h && defined Commu_ComEnum_h
//------------------------------------------------------------------------------------------//
#ifndef ParRecord_h
#define ParRecord_h
#ifdef ParRecord_h
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
				 PR_Record(void);
		virtual ~PR_Record(void){;};
	public:
		IPCOMLIST*	cgIPComList;
		SC_LIST*	cgSCList;
		GC_LIST*	cgGCList;
	private:
		void	ImportV0_4	(uint32 ver,STDSTR* strIn,uint32 cfg);
		STDSTR&	ExportV0_4	(uint32 ver,STDSTR* strOut,uint32 cfg);
	public:
		void	Init		(IPCOMLIST* iplist,SC_LIST* sclist,GC_LIST* gclist);
		void	Load		(const STDSTR& fileName,uint32 cfg);
		void	Save		(const STDSTR& fileName,uint32 cfg);
};
//------------------------------------------------------------------------------------------//
#endif /* ParRecord_h */
#endif /* ParRecord_h */
#endif
