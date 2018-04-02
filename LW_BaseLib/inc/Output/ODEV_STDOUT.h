//
//  ODEV_STDOUT.h
//  SDT
//
//  Created by Leif Wen on 31/01/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "Output.h"
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
#ifdef Output_h
//------------------------------------------------------------------------------------------//
#ifndef ODEV_STDOUT_h
#define ODEV_STDOUT_h
#ifdef ODEV_STDOUT_h
//------------------------------------------------------------------------------------------//
CreateOperatorSetUint32(Left)
CreateOperatorSetUint32(Right)
CreateOperatorSetUint32(ICH)
CreateOperatorSetUint32(DCH)
CreateOperatorSetUint32(EL)
class CMyRichView;
//------------------------------------------------------------------------------------------//
class ODEV_STDOUT : public OUTPUT_NODE{
	public:
		enum	{BUF_MAX_SIZE = 1024 * 1024 * 64};
	public:
				 ODEV_STDOUT(COLType colType = COLType_RAW,OUTPUT_CACHE *cache = nullptr,uint32 group = 0);;
		virtual ~ODEV_STDOUT(void);
	public:
				virtual void		Print			(uint32 ctrl,COLORENUM col,const uint8* data,uint32 num);
#ifdef CommonDefH_Unix
	public:
		inline			void		SetCurLeft		(int32 num);
		inline			void		SetCurRight		(int32 num);
		inline			void		InsterChar		(COLORENUM col,const uint8* data,uint32 length);
		inline			void		BKChar			(void);
		inline			void		DelChar			(void);
		inline			void		RewriteChar		(int32 moveLeft,COLORENUM col,const STDSTR& strIn);
	private:
		inline			ODEV_STDOUT& operator <<	(const _Left&	data);
		inline			ODEV_STDOUT& operator <<	(const _Right&	data);
		inline			ODEV_STDOUT& operator <<	(const _ICH&	data);
		inline			ODEV_STDOUT& operator <<	(const _DCH&	data);
		inline			ODEV_STDOUT& operator <<	(const _EL&		data);
#endif
#ifdef CommonDefH_VC
	public:
		CMyRichView*	cgRichEdit;
	public:
						void	Init				(const void* tRichEdit,COLType colType = COLType_COL);
						void	Clean				(G_LOCK blLock = G_LOCK_ON);
	public:
						void	SetCurFromEnd		(int32 offset);
						void	DelCharFromEnd		(int32 offset,int32 length);
						void	RewriteCharFromEnd	(int32 offset,COLORENUM col,const STDSTR& strIn);
						void	InsterCharFromEnd	(int32 offset,COLORENUM col,const STDSTR& strIn);
						void	AppendChar			(			  COLORENUM col,const STDSTR& strIn);
		inline			void	ToHome				(void);
#endif
};
//------------------------------------------------------------------------------------------//
#include "ODEV_STDOUT.hpp"
#endif /* ODEV_STDOUT_h */
#endif /* ODEV_STDOUT_h */
#endif /* Output_h */

