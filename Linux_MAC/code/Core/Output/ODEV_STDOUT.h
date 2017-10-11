/*
 * Copyright (c) 2012-2014 
 * All rights reserved.
 *
 * File Name: ODEV_STDOUT.h
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2013.7.5
 *			: 2014.1.13 remove support BCB2012
 *			: 2014.1.14 remove support Txt output
*/

//------------------------------------------------------------------------------------------//
#include "OUTPUT.h"
//------------------------------------------------------------------------------------------//
#ifndef ODEV_COLORH
#define ODEV_COLORH
#ifdef CommonDefH_Unix
	#define		COL_STR_clDefault		"\033[0m"

	#define		COL_STR_clBlack			"\033[22;30m"
	#define		COL_STR_clRed			"\033[22;31m"
	#define		COL_STR_clGreen			"\033[22;32m"
	#define		COL_STR_clYellow		"\033[22;33m"
	#define		COL_STR_clBlue			"\033[22;34m"
	#define		COL_STR_clMagenta		"\033[22;35m"
	#define		COL_STR_clCyan			"\033[22;36m"
	#define		COL_STR_clWhite			"\033[22;37m"

	#define		COL_STR_clBrightBlack	"\033[01;30m"
	#define		COL_STR_clBrightRed		"\033[01;31m"
	#define		COL_STR_clBrightGreen	"\033[01;32m"
	#define		COL_STR_clBrightYellow	"\033[01;33m"
	#define		COL_STR_clBrightBlue	"\033[01;34m"
	#define		COL_STR_clBrightMagenta	"\033[01;35m"
	#define		COL_STR_clBrightCyan	"\033[01;36m"
	#define		COL_STR_clBrightWhite	"\033[01;37m"

	#define		COL_STR_clPurple		COL_STR_clMagenta
#endif
#ifdef CommonDefH_VC
	#define		COL_RGB_clDefault		COL_RGB_clBlack

	#define		COL_RGB_clBlack			(RGB(0,0,0))
	#define		COL_RGB_clMaroon		(RGB(127,0,0))
	#define		COL_RGB_clDGreen		(RGB(0,160,0))
	#define		COL_RGB_clDYellow		(RGB(160,160,0))
	#define		COL_RGB_clDBlue			(RGB(0,0,160))
	#define		COL_RGB_clDMagenta		(RGB(160,0,160))
	#define		COL_RGB_clDCyan			(RGB(0,160,160))
	#define		COL_RGB_clDGray			(RGB(192,192,192))

	#define		COL_RGB_clGray			(RGB(128,128,128))
	#define		COL_RGB_clRed			(RGB(255,0,0))
	#define		COL_RGB_clGreen			(RGB(0,255,0))
	#define		COL_RGB_clYellow		(RGB(255,255,0))
	#define		COL_RGB_clBlue			(RGB(0,0,255))
	#define		COL_RGB_clMagenta		(RGB(255,0,255))
	#define		COL_RGB_clCyan			(RGB(0,255,255))
	#define		COL_RGB_clWhite			(RGB(255,255,255))

	#define		COL_RGB_clPurple		(RGB(127,0,127))
#endif
#endif
//------------------------------------------------------------------------------------------//
#ifdef OUTPUTH
#ifndef ODEV_SDOUTH
#define ODEV_SDOUTH
#ifdef ODEV_SDOUTH
//------------------------------------------------------------------------------------------//
CreateOperatorSet(Left)
CreateOperatorSet(Right)
CreateOperatorSet(ICH)
CreateOperatorSet(DCH)
CreateOperatorSet(EL)
class CMyRichView;
//------------------------------------------------------------------------------------------//
class ODEV_STDOUT : public OUTPUT_NODE{
	public:
		enum {BUF_MAX_SIZE = 1024 * 1024 * 64};
	public:
				 ODEV_STDOUT(COLType tColType = COLType_RAW,OUTPUT_CACHE *tOutputCache = nullptr,uint32 group = 0) : OUTPUT_NODE(tColType,tOutputCache,group){SetSelfName("ODEV_STDOUT");};
		virtual ~ODEV_STDOUT(void);
	public:
		virtual void	Print	(uint32 col, uint32 ctrl,const uint8 *data,uint32 num);
	#ifdef CommonDefH_Unix
	public:
		void	SetCurLeft		(int32 num);
		void	SetCurRight		(int32 num);
		void	InsterChar		(uint32 col,const uint8* data,uint32 length);
		void	BKChar			(void);
		void	DelChar			(void);
		void	RewriteChar		(int32 moveLeft,uint32 col,const STDSTR &strIn);
	
		inline	ODEV_STDOUT& operator << (ODEV_STDOUT&(*fun)(ODEV_STDOUT&)){return((*fun)(*this));};
	private:
				ODEV_STDOUT& operator << (const _Left &data);
				ODEV_STDOUT& operator << (const _Right &data);
				ODEV_STDOUT& operator << (const _ICH &data);
				ODEV_STDOUT& operator << (const _DCH &data);
				ODEV_STDOUT& operator << (const _EL &data);
	#endif
	#ifdef CommonDefH_VC
	public:
		CMyRichView		*cgRichEdit;
		void			**cgCFrm;
	public:
		void	Init				(const void *tRichEdit,const void **tCFrm,COLType tColType = COLType_COL);
		void	Clean				(G_LOCK blLock = G_LOCK_ON);
	public:
		void	SetCurFromEnd		(int32 offset);
		void	DelCharFromEnd		(int32 offset,int32 length);
		void	RewriteCharFromEnd	(int32 offset,uint32 col,const STDSTR &strIn);
		void	InsterCharFromEnd	(int32 offset,uint32 col,const STDSTR &strIn);
		void	AppendChar			(uint32 col,const STDSTR &strIn);
		void	ToHome(void)		{Write(COL_FF,0);};
	#endif
};
//------------------------------------------------------------------------------------------//
#endif
#endif
#endif
