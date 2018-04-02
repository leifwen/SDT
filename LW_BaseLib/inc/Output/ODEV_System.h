//
//  ODEV_System.h
//  SDT
//
//  Created by Leif Wen on 31/01/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

//------------------------------------------------------------------------------------------//
#include "Output.h"
#include "ODEV_FILE.h"
#include "ODEV_STDOUT.h"
//------------------------------------------------------------------------------------------//
#if defined ODEV_STDOUT_h && defined ODEV_FILE_h
#ifndef ODEV_System_h
#define ODEV_System_h
#ifdef ODEV_System_h
//------------------------------------------------------------------------------------------//
class ODEV_VG3D;
class ODEV_CACHE;

struct ODEV_SYSTEM{
	ODEV_CACHE	*cache;
	uint64		envcfg;
};
//------------------------------------------------------------------------------------------//
enum{
	ODEV_FLAG_EnView		= BD_FLAG64(0),
	ODEV_FLAG_EnHEXViewMode	= BD_FLAG64(1),
	ODEV_FLAG_EnRecMsg		= BD_FLAG64(2),
	ODEV_FLAG_EnOSPMsgLine	= BD_FLAG64(3),
	ODEV_FLAG_EnEscape		= BD_FLAG64(4),
	ODEV_FLAG_EnMSReport	= BD_FLAG64(5),
};
//------------------------------------------------------------------------------------------//
class ODEV_CACHE : public OUTPUT_CACHE{
	public:
				 ODEV_CACHE(uint32 size);
		virtual ~ODEV_CACHE(void){;};
	private:
		ODEV_STDOUT*	defSTDOUT;
		ODEV_FILE*		defFile;
	public:
		inline			ODEV_STDOUT*	GetG1_STDOUT		(void)const;
		inline			ODEV_FILE*		GetG2_File			(void)const;
	public:
				virtual	void			Unregister			(OUTPUT_NODE* oNode);	
						ODEV_STDOUT*	CreateG1_STDOUT		(OUTPUT_NODE::COLType colType = OUTPUT_NODE::COLType_COL);
						ODEV_STDOUT*	AddG1_STDOUT		(ODEV_STDOUT* oG1D);
						ODEV_FILE*		CreateG2_FILE		(const STDSTR& fName,uint64 colType = ODEV_FILE::ODEV_blRTF | ODEV_FILE::ODEV_blTXT);
};
//------------------------------------------------------------------------------------------//
class ODEV_VG3D : public VG3D_POOL{
	public:
				 ODEV_VG3D(OUTPUT_CACHE *cache = nullptr);
		virtual ~ODEV_VG3D(void){;};
	private:
		OUTPUT_NODE*	defODEV;
		ODEV_STDOUT*	defSTDOUT;
		ODEV_FILE*		defFile;
	public:
		inline			OUTPUT_NODE*	GetDefODEV			(void)const;
		inline			ODEV_STDOUT*	GetDefSTDOUT		(void)const;
		inline			ODEV_FILE*		GetDefFile			(void)const;
	public:
				virtual	void			UnregisterChild		(OUTPUT_NODE* oG3D);
						OUTPUT_NODE*	AddG3D_ODEV			(OUTPUT_NODE* oG3D);
						ODEV_STDOUT*	AddG3D_STDOUT		(ODEV_STDOUT* oG3D);
						ODEV_FILE*		CreateG3D_FILE		(const STDSTR& fName,uint64 colType = ODEV_FILE::ODEV_blRAW | ODEV_FILE::ODEV_blTXT);
};
//------------------------------------------------------------------------------------------//
#include "ODEV_System.hpp"
#endif /* ODEV_System_h */
#endif /* ODEV_System_h */
#endif
