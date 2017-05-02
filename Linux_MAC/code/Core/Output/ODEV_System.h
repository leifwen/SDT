/*
 * Copyright (c) 2012-2014 
 * All rights reserved.
 *
 * File Name: ODEV_LIST.h
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2013.7.5
 *			: 2014.1.13 remove support BCB2012
 *			: 2014.1.14 remove support Txt output
 *			: 2015.1.06 change flag
*/

//------------------------------------------------------------------------------------------//
#include "OUTPUT.h"
#include "ODEV_FILE.h"
#include "ODEV_STDOUT.h"
//------------------------------------------------------------------------------------------//
#if defined ODEV_SDOUTH && defined ODEV_FILEH
#ifndef ODEV_SystemH
#define ODEV_SystemH
#ifdef ODEV_SystemH
//------------------------------------------------------------------------------------------//
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
class ODEV_VG3D;
//------------------------------------------------------------------------------------------//
class ODEV_CACHE : public OUTPUT_CACHE{
	public:
		enum{RFLAG_C = 1, RFLAG_S = OUTPUT_CACHE::RFLAG_S + OUTPUT_CACHE::RFLAG_C};
	private:
		enum{blAdd_defSTDOUT = RFLAG_CREATE(0)};
	public:
				 ODEV_CACHE(uint32 tSize) : OUTPUT_CACHE(tSize){Init();CreateTrash(&cgSubNodeList);SetSelfName("ODEV_VG3D");};
		virtual ~ODEV_CACHE(void){UnregisterAll();cgSubNodeList.DestroyAll();};
	private:
				void	Init(void);
		virtual	void	Unregister(OUTPUT_NODE *oNode);
		virtual	void	UnregisterAll(void);
	private:
		ODEV_STDOUT		*defSTDOUT;
		ODEV_FILE		*defFile;
	public:
		inline	ODEV_STDOUT		*GetG1_STDOUT	(void)const {return(defSTDOUT);};
		inline	ODEV_FILE		*GetG2_File		(void)const {return(defFile);};
	public:
		ODEV_STDOUT		*CreateG1_STDOUT		(OUTPUT_NODE::COLType tColType = OUTPUT_NODE::COLType_COL);
		ODEV_STDOUT		*AddG1_STDOUT			(ODEV_STDOUT *oG1D);
		ODEV_FILE		*CreateG2_FILE			(const STDSTR &fName,uint64 tColType = ODEV_FILE::of_blRTF | ODEV_FILE::of_blTXT);
};
//------------------------------------------------------------------------------------------//
class ODEV_VG3D : public VG3D_POOL{
	public:
		enum{RFLAG_C = 3, RFLAG_S = VG3D_POOL::RFLAG_S + VG3D_POOL::RFLAG_C};
	private:
		enum{blAdd_defODEV = RFLAG_CREATE(0),blAdd_defSTDOUT = RFLAG_CREATE(1),};
	public:
				 ODEV_VG3D(OUTPUT_CACHE *tOutputCache = nullptr){Init();SetSelfName("ODEV_VG3D");};
		virtual ~ODEV_VG3D(void){;};
	private:
		void	Init(void);
	private:
		OUTPUT_NODE		*defODEV;
		ODEV_STDOUT		*defSTDOUT;
		ODEV_FILE		*defFile;
	public:
		inline	OUTPUT_NODE		*GetDefODEV		(void)const {return(defODEV);};
		inline	ODEV_STDOUT		*GetDefSTDOUT	(void)const {return(defSTDOUT);};
		inline	ODEV_FILE		*GetDefFile		(void)const {return(defFile);};
	public:
		virtual	void	UnregisterAll		(void);
		virtual	void	Unregister			(OUTPUT_NODE *oG3D);
	public:
		OUTPUT_NODE		*AddG3D_ODEV		(OUTPUT_NODE *oG3D,int32 blManage = 1);
		ODEV_STDOUT		*AddG3D_STDOUT		(ODEV_STDOUT *oG3D,int32 blManage = 1);
		ODEV_FILE		*CreateG3D_FILE		(const STDSTR &fName,uint64 tColType = ODEV_FILE::of_blRAW | ODEV_FILE::of_blTXT);
};
//------------------------------------------------------------------------------------------//
struct ODEV_SYSTEM{
	ODEV_CACHE	*cache;
	uint64		envcfg;
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif
#endif
#endif
