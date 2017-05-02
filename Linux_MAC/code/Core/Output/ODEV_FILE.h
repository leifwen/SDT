/*
 * Copyright (c) 2012-2014 
 * All rights reserved.
 *
 * File Name: ODEV_FILE.h
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2013.7.5
 *			: 2014.1.13 remove support BCB2012
 *			: 2014.1.14 remove support Txt output
*/

//------------------------------------------------------------------------------------------//
#include "OUTPUT.h"
#ifdef OUTPUTH
#ifndef ODEV_FILEH
#define ODEV_FILEH
#ifdef ODEV_FILEH
//------------------------------------------------------------------------------------------//
#include "SYS_Time.h"
//------------------------------------------------------------------------------------------//
#ifndef ODEV_LOG_FDIR
#define	ODEV_LOG_FDIR	"_Record"
#endif
//------------------------------------------------------------------------------------------//
#define		RICH_HEAD				"{\\rtf1\\ansi\\ansicpg1252\\deff0\\deflang1033\\deflangfe2052{\\fonttbl{\\f0\\fnil\\fcharset134 Courier New;}}\r\n"
#define		RICH_TEXT_HEAD			"\\viewkind4\\uc1\\pard\\lang2052\\f0\\fs20 "
#define		RICH_END				"}\r\n"
#define		RICH_PARL				"\\par\r\n"
#define		RICH_COLOR				"{\\colortbl;\
\\red0\\green0\\blue0;\
\\red127\\green0\\blue0;\
\\red0\\green160\\blue0;\
\\red160\\green160\\blue0;\
\\red0\\green0\\blue160;\
\\red160\\green0\\blue160;\
\\red0\\green160\\blue160;\
\\red192\\green192\\blue192;\
\\red128\\green128\\blue128;\
\\red255\\green0\\blue0;\
\\red0\\green255\\blue0;\
\\red255\\green255\\blue0;\
\\red0\\green0\\blue255;\
\\red255\\green0\\blue255;\
\\red0\\green255\\blue255;\
\\red255\\green255\\blue255;\
\\red127\\green0\\blue127;\
}\r\n"
//------------------------------------------------------------------------------------------//
#define		COL_CF_clDefault	COL_CF_clBlack

#define		COL_CF_clBlack		"\\cf1 "
#define		COL_CF_clMaroon		"\\cf2 "
#define		COL_CF_clDGreen		"\\cf3 "
#define		COL_CF_clDYellow	"\\cf4 "
#define		COL_CF_clDBlue		"\\cf5 "
#define		COL_CF_clDMagenta	"\\cf6 "
#define		COL_CF_clDCyan		"\\cf7 "
#define		COL_CF_clDGray		"\\cf8 "

#define		COL_CF_clGray		"\\cf9 "
#define		COL_CF_clRed		"\\cf10 "
#define		COL_CF_clGreen		"\\cf11 "
#define		COL_CF_clYellow		"\\cf12 "
#define		COL_CF_clBlue		"\\cf13 "
#define		COL_CF_clMagenta	"\\cf14 "
#define		COL_CF_clCyan		"\\cf15 "
#define		COL_CF_clWhite		"\\cf16 "

#define		COL_CF_clPurple		"\\cf17 "
//------------------------------------------------------------------------------------------//
class ODEV_FILE : public OUTPUT_NODE{
	public:
		enum	{RFLAG_C = 3, RFLAG_S = OUTPUT_NODE::RFLAG_S + OUTPUT_NODE::RFLAG_C};
		enum	{FREQUENCY = 1000,UNWRITESIZE = 1024 *8,};
		enum	{of_blRAW = RFLAG_CREATE(0),of_blTXT = RFLAG_CREATE(1),of_blRTF = RFLAG_CREATE(2),};
	public:
				 ODEV_FILE(OUTPUT_CACHE *tOutputCache = nullptr,uint32 group = 3) : OUTPUT_NODE(COLType_RAW,tOutputCache,group){Init("",of_blRTF);SetSelfName("ODEV_FILE");};
		virtual ~ODEV_FILE(void){;};
	public:
		virtual	uint32	CheckPrint	(uint32 tCtrl)const;
		virtual void	Print		(uint32 col,uint32 ctrl,const uint8 *data,uint32 num);
				void	PrintRTF	(uint32 col,uint32 ctrl,const uint8 *data,uint32 num);
				void	PrintTXT	(uint32 col,uint32 ctrl,const uint8 *data,uint32 num);
				void	PrintRAW	(uint32 col,uint32 ctrl,const uint8 *data,uint32 num);
	private:
		STDSTR			cgfileName;
		STDSTR			cgContentRAW;
		STDSTR			cgContentTXT;
		STDSTR			cgContentRTF;
		SYS_TIME_S		cgTimeS;
	public:
		inline void		SetFileName(const STDSTR &filename)		{Spin_InUse_set();cgfileName = filename;Spin_InUse_clr();};
		inline STDSTR	GetFileName(void)const					{return(cgfileName);};
	public:
		void	Init	(const STDSTR &filename,uint64 tColType = of_blRTF);
	public:
		static	STDSTR	CreateFileTime			(void);
		static	STDSTR	CreateLOGDIR			(void);
		static	STDSTR	GetLOGDIR				(void);
		static	STDSTR	CreateNewLOGFileName	(void);
		static	STDSTR	CreateNewLOGFileName	(const STDSTR &tIP,int32 tPort);
	
		static	void	CreateEmptyRTFFile	(const STDSTR &fName);
		static	void	AddToTXTFile		(const STDSTR &fName,const STDSTR &content);
		static	void	AddToRTFFile		(const STDSTR &fName,const STDSTR &content);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif
#endif
#endif
