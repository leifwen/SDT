/*
 * Copyright (c) 2012-2014 
 * All rights reserved.
 *
 * File Name: ColorRecord.h
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2013.7.5
*/
#ifndef ColorRecordH
#define ColorRecordH
//------------------------------------------------------------------------------------------//
#include "Comm_Tree.h"
//------------------------------------------------------------------------------------------//
#define		RICH_RGB_clBlack		(RGB(0,0,0))
#define		RICH_RGB_clMaroon		(RGB(127,0,0))
#define		RICH_RGB_clDGreen		(RGB(0,160,0))
#define		RICH_RGB_clDBrown		(RGB(160,160,0))
#define		RICH_RGB_clDBlue		(RGB(0,0,160))
#define		RICH_RGB_clDMagenta		(RGB(160,0,160))
#define		RICH_RGB_clDCyan		(RGB(0,160,160))
#define		RICH_RGB_clDGray		(RGB(192,192,192))

#define		RICH_RGB_clGray			(RGB(128,128,128))
#define		RICH_RGB_clRed			(RGB(255,0,0))
#define		RICH_RGB_clGreen		(RGB(0,255,0))
#define		RICH_RGB_clYellow		(RGB(255,255,0))
#define		RICH_RGB_clBlue			(RGB(0,0,255))
#define		RICH_RGB_clMagenta		(RGB(255,0,255))
#define		RICH_RGB_clCyan			(RGB(0,255,255))
#define		RICH_RGB_clPurple		(RGB(127,0,127))

#define		RICH_RGB_clWhite		(RGB(255,255,255))
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
\\red127\\green0\\blue127;\
\\red255\\green255\\blue255;\
}\r\n"
//------------------------------------------------------------------------------------------//
#define		RICH_CF_clBlack		"\\cf1 "
#define		RICH_CF_clMaroon	"\\cf2 "
#define		RICH_CF_clDGreen	"\\cf3 "
#define		RICH_CF_clDBrown	"\\cf4 "
#define		RICH_CF_clDBlue		"\\cf5 "
#define		RICH_CF_clDMagenta	"\\cf6 "
#define		RICH_CF_clDCyan		"\\cf7 "
#define		RICH_CF_clDGray		"\\cf8 "

#define		RICH_CF_clGray		"\\cf9 "
#define		RICH_CF_clRed		"\\cf10 "
#define		RICH_CF_clGreen		"\\cf11 "
#define		RICH_CF_clYellow	"\\cf12 "
#define		RICH_CF_clBlue		"\\cf13 "
#define		RICH_CF_clMagenta	"\\cf14 "
#define		RICH_CF_clCyan		"\\cf15 "
#define		RICH_CF_clPurple	"\\cf16 "

#define		RICH_CF_clWhite		"\\cf17 "
//------------------------------------------------------------------------------------------//
#ifdef CommonDefH_Unix
#define		RICH_LIN_clDefault		"\033[0m"

#define		RICH_LIN_clBlack		"\033[22;30m"
#define		RICH_LIN_clRed			"\033[22;31m"
#define		RICH_LIN_clGreen		"\033[22;32m"
#define		RICH_LIN_clBrown		"\033[22;33m"
#define		RICH_LIN_clBlue			"\033[22;34m"
#define		RICH_LIN_clMagenta		"\033[22;35m"
#define		RICH_LIN_clCyan			"\033[22;36m"
#define		RICH_LIN_clGray			"\033[22;37m"

#define		RICH_LIN_clDarkGray		"\033[01;30m"
#define		RICH_LIN_clLightRed		"\033[01;31m"
#define		RICH_LIN_clLightGreen	"\033[01;32m"
#define		RICH_LIN_clYellow		"\033[01;33m"
#define		RICH_LIN_clLightBlue	"\033[01;34m"
#define		RICH_LIN_clLightMagenta	"\033[01;35m"
#define		RICH_LIN_clLightCyan	"\033[01;36m"
#define		RICH_LIN_clWhite		"\033[01;37m"

#define		RICH_CFL_clBlack	RICH_LIN_clDefault
#define		RICH_CFL_clMaroon	RICH_LIN_clGreen
#define		RICH_CFL_clDGreen	RICH_LIN_clGreen
#define		RICH_CFL_clDBrown	RICH_LIN_clBrown
#define		RICH_CFL_clDBlue	RICH_LIN_clBlue
#define		RICH_CFL_clDMagenta	RICH_LIN_clMagenta
#define		RICH_CFL_clDCyan	RICH_LIN_clCyan
#define		RICH_CFL_clDGray	RICH_LIN_clGray

#define		RICH_CFL_clGray		RICH_LIN_clDarkGray
#define		RICH_CFL_clRed		RICH_LIN_clRed
#define		RICH_CFL_clGreen	RICH_LIN_clLightGreen
#define		RICH_CFL_clYellow	RICH_LIN_clYellow
#define		RICH_CFL_clBlue		RICH_LIN_clYellow
#define		RICH_CFL_clMagenta	RICH_LIN_clLightMagenta
#define		RICH_CFL_clCyan		RICH_LIN_clLightCyan
#define		RICH_CFL_clPurple	RICH_LIN_clCyan

#define		RICH_CFL_clWhite	RICH_LIN_clWhite

#endif
#ifdef CommonDefH_VC
#define		RICH_LIN_clDefault		RICH_CF_clBlack

#define		RICH_LIN_clBlack		RICH_CF_clBlack
#define		RICH_LIN_clRed			RICH_CF_clMaroon
#define		RICH_LIN_clGreen		RICH_CF_clDGreen
#define		RICH_LIN_clBrown		RICH_CF_clDBrown
#define		RICH_LIN_clBlue			RICH_CF_clDBlue
#define		RICH_LIN_clMagenta		RICH_CF_clDMagenta
#define		RICH_LIN_clCyan			RICH_CF_clDCyan
#define		RICH_LIN_clGray			RICH_CF_clDGray

#define		RICH_LIN_clDarkGray		RICH_CF_clGray
#define		RICH_LIN_clLightRed		RICH_CF_clRed
#define		RICH_LIN_clLightGreen	RICH_CF_clGreen
#define		RICH_LIN_clYellow		RICH_CF_clYellow
#define		RICH_LIN_clLightBlue	RICH_CF_clBlue
#define		RICH_LIN_clLightMagenta	RICH_CF_clMagenta
#define		RICH_LIN_clLightCyan	RICH_CF_clCyan
#define		RICH_LIN_clWhite		RICH_CF_clBlack
#endif
//------------------------------------------------------------------------------------------//
class COLSTRING : public RTREE_NODE{
	public:
		enum		{RFLAG_C = 0, RFLAG_S = RTREE_NODE::RFLAG_S + RTREE_NODE::RFLAG_C};
		enum		{BUF_MAX_SIZE = 1024 * 1024 * 16 * 3};
		enum COLType			{COLType_NONE = 0,COLType_TXT,COLType_COL};
		enum COLEnforcePrint	{COL_EP_NO = 0,COL_EP_YES};
	public:
				 COLSTRING(COLType tCOLType = COLType_NONE) : RTREE_NODE(){ChageType(tCOLType);};
		virtual ~COLSTRING(void){;};
	private:
		std::string				LastChar;
		std::string				unreadContent;
		std::string::size_type	UnreadLength;
		void					UpdateLastChar(const std::string &inputStr);\
	protected:
				COLType			cgCOLType;
		inline	std::string		GetLastChar	(void)const{return(LastChar);};
				void			ChageType	(COLType tCOLType,G_LOCK_VAILD blLock = G_LOCK_ON);
	public:
		uint64			GetUnreadLength	(G_LOCK_VAILD blLock = G_LOCK_ON);
		virtual	void	Clean			(G_LOCK_VAILD blLock = G_LOCK_ON);
				void	WriteToStr	(const std::string &strContent,const std::string &rColor,COLEnforcePrint blEP = COL_EP_NO,G_LOCK_VAILD blLock = G_LOCK_ON);
				void	WriteToStrN	(const std::string &strContent,const std::string &rColor,COLEnforcePrint blEP = COL_EP_NO,G_LOCK_VAILD blLock = G_LOCK_ON);
		const	std::string &ReadStr(std::string *retStr,G_LOCK_VAILD blLock = G_LOCK_ON);
};
//------------------------------------------------------------------------------------------//
#endif