//
//  Console.h
//  SDT
//
//  Created by Leif Wen on 05/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "SYS_Time.h"
#include "SYS_Thread.h"
#include "ADS_Buffer.h"
//------------------------------------------------------------------------------------------//
#ifdef ADS_Buffer_h
//------------------------------------------------------------------------------------------//
#ifndef Console_h
#define Console_h
#ifdef Console_h
//------------------------------------------------------------------------------------------//
class Line_Edit{
	public:
				 Line_Edit(void);
		virtual ~Line_Edit(void){;};
	private:
		STDSTR				cgEditContent;
	public:
		uint32				cgEditCur;
	public:
		void				Init		(void);
		uint32				KeyLeft		(void);
		uint32				KeyRight	(void);
		uint32				KeyHome		(void);
		uint32				KeyEnd		(void);
		uint32				KeyBK		(void);
		uint32				KeyDel		(void);
		uint32				Insert		(uint8 key);
		uint32				Insert		(const uint8* key, uint32 length);
		const STDSTR&		SetData		(const STDSTR& newInput);
		const STDSTR&		GetData		(void)const;
		uint32				GetLength	(void)const;
};
//------------------------------------------------------------------------------------------//
class Line_History{
		enum	{MaxRecord = 512};
	public:
				 Line_History(void);
		virtual ~Line_History(void){;};
	private:
		STDSTR			cgEditHistory[MaxRecord];
		int32			cgCount;
		int32			cgHead;
		int32			cgTop;
		int32			cgHistoryCur;
	public:
		void			Init			(void);
		bool32			SetPrior		(void);
		bool32			SetNext			(void);
		void			Add				(const STDSTR& newInput);
		const STDSTR&	GetData			(void)const;
		STDSTR			GetHistory		(void);
		STDSTR			GetSimilar		(const STDSTR& inputStr);
		STDSTR			GetSimilarGroup	(const STDSTR& inputStr);
		void			ClearHistory	(void);
};
//------------------------------------------------------------------------------------------//
enum{
	VK_DBCLK = 0x01,
	VK_COCLK = 0x01 << 1,
};
#ifdef CommonDefH_Unix
enum{
	VK_ERR = 0,
	VK_NOKEY,
	VK_UP,
	VK_DOWN,
	VK_LEFT,
	VK_RIGHT,
	VK_HOME,
	VK_END,
	VK_PRIOR,
	VK_NEXT,
	VK_DELETE,
	VK_BACK,
	VK_RETURN,
	VK_ESCAPE,
	VK_SPACE,
	VK_INSERT,
	VK_TAB,
	VK_CONTROL,
	VK_MENU,
};
enum{
	WM_NOKEY = 0,
	WM_CHAR,
	WM_KEYDOWN,
};
struct MSG{
	uint32	message;
	uint8	wParam;
	uint32	lParam;
};
#endif

struct KEYMSG{
	uint32	message;
	uint8	wParam;
	uint32	lParam;
	DTIME	time;
};
class CMD_ENV;
class BIC_BASE_S;
class ODEV_STDOUT;
//------------------------------------------------------------------------------------------//
class CONSOLE : public BASE_FLAG{
	protected:
		enum	{RFLAG_C = 5, RFLAG_S = BASE_FLAG::RFLAG_S + BASE_FLAG::RFLAG_C};
		enum	{CON_blExit					= RFLAG_CREATE(0)
				,CON_blInDBEscMode			= RFLAG_CREATE(1)
				,CON_blInPressAnyKeyMode	= RFLAG_CREATE(2)
				,CON_blInOnlineMode			= RFLAG_CREATE(3)
				,CON_blDisplayAuto			= RFLAG_CREATE(4)
		};
	public:
				 CONSOLE(uint32 size);
		virtual ~CONSOLE(void);
	private:
		ODEV_STDOUT*		cgOdevSTDOUT;
		Line_History		cgHistoryMain;
		Line_History		cgHistoryAux;
		Line_History*		cgHistory;
		Line_Edit			cgCInput;
		KEYMSG				cgLastKey;
		SBUF				cgCMDSBUF;
		CMD_ENV*			cgBICENV;
		SYS_Thread<CONSOLE>	bicThread;
	protected:
		virtual		bool32		BICThreadFun			(void* p);
	private:
					void		KeyLeft					(void);
					void		KeyRight				(void);
					void		KeyHome					(void);
					void		KeyEnd					(void);
					void		KeyBK					(void);
					void		KeyDel					(void);
					void		KeyUp					(void);
					void		KeyDown					(void);
					void		KeyReturn				(void);
					void		KeyEscape				(void);
					void		KeyTab					(void);
					void		KeyDBTab				(void);
	private:
					bool32		InsertChar				(uint8 data);
					bool32		InsertChar				(const uint8*	data,uint32 length);
					KEYMSG*		AnalyzeKey				(const MSG*		keyMsg);
					bool32		DoControl				(const KEYMSG*	keyMsg);
	#ifdef CommonDefH_Unix
		static	const MSG*		GetKey					(MSG* keyMsg);
	#endif
	public:
					bool32		LoadDefault				(const STDSTR& filename = "console.ini");
					void		AddToMain				(const STDSTR& newInput);
					void		SwitchToMain			(void);
					void		SwitchToAux				(void);
					bool32		Paste					(uint8* key, uint32 length);
	public:
					bool32		ReceiveKey				(const MSG* keyMsg);
	#ifdef CommonDefH_Unix
					bool32		ReceiveKey				(void);
	#endif
	public:
					void		SetInPressAnyKeyMode	(void);
					void		ClrInPressAnyKeyMode	(void);
					void		SetInOnlineMode			(void);
					void		ClrInOnlineMode			(void);
					void		SetblDisplayAuto		(void);
					void		ClrblDisplayAuto		(void);
					bool32		IsDisplayAuto			(void)const;
					void		SetExit					(void);
	public:
					CONSOLE&	Init					(const ODEV_STDOUT* oDevSTDOUT);
					void		StartWithBIC			(CMD_ENV* env,BIC_BASE_S* bic);
					void		Stop					(void);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif /* Console_h */
#endif /* Console_h */
#endif
