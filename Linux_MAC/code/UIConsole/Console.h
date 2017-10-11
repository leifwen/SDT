/*
 * Copyright (c) 2012-2014 
 * All rights reserved.
 *
 * File Name: Console.h
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2014.01.24
*/

//------------------------------------------------------------------------------------------//
#include "BasicDef.h"
#include "BasicClass.h"
#include "SYS_Thread.h"
#include "SYS_Time.h"
#include "Comm_Buffer.h"
#include "ODEV_STDOUT.h"
#include "BIC.h"
//------------------------------------------------------------------------------------------//
#if defined SYS_ThreadH && defined Comm_BufferH && defined ODEV_SDOUTH
#ifndef ConsoleH
#define ConsoleH
#ifdef ConsoleH
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
class CON_LineEdit{
	public:
				 CON_LineEdit(void){Init();};
		virtual ~CON_LineEdit(void){;};
	private:
		STDSTR	cgEditContent;
	public:
		uint32				cgEditCur;
		void				Init(void);
		uint32				KeyLeft		(void);
		uint32				KeyRight	(void);
		uint32				KeyHome		(void);
		uint32				KeyEnd		(void);
		uint32				KeyBK		(void);
		uint32				KeyDel		(void);
		uint32				Insert		(uint8 key);
		uint32				Insert		(const uint8 *key, uint32 length);
		const STDSTR&		SetData		(const STDSTR &newData);
		const STDSTR&		GetData		(void)const	{return(cgEditContent);};
		uint32				GetLength	(void)const	{return((uint32)cgEditContent.length());};
};
//------------------------------------------------------------------------------------------//
class CON_LineHistory{
		enum{MaxRecord = 512};
	public:
				 CON_LineHistory(void){Init();};
		virtual ~CON_LineHistory(void){;};
	private:
		STDSTR		cgEditHistory[MaxRecord];
		int32			cgCount;
		int32			cgHead;
		int32			cgTop;
		int32			cgHistoryCur;
	public:
		void			Init			(void);
		int32			SetPrior		(void);
		int32			SetNext			(void);
		void			Add				(const STDSTR &newData);
		const STDSTR&	GetData			(void)const{return(cgEditHistory[cgHistoryCur]);};
		STDSTR			GetHistory		(void);
		STDSTR			GetSimilar		(const STDSTR &inputStr);
		STDSTR			GetSimilarGroup	(const STDSTR &inputStr);
		void			ClearHistory	(void){Init();};
};
//------------------------------------------------------------------------------------------//
enum{
	VK_DBCLK = 0x01,
	VK_COCLK = 0x01 << 1,
};
#ifdef CommonDefH_Unix
enum{
	VK_UP = 0,
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
	TIME	keyTime;
};
//------------------------------------------------------------------------------------------//
class SDTAPP;
//------------------------------------------------------------------------------------------//
class CON_ReadInLine{
	public:
				 CON_ReadInLine(void);
		virtual ~CON_ReadInLine(void){Exit();};
	private:
		ODEV_STDOUT			*cgOdevSTDOUT;
	private:
		CON_LineHistory		cgMainLH;
		CON_LineHistory		cgSecondLH;
		CON_LineHistory		*cgCLH;
		CON_LineEdit		cgCInput;
	private:
		int32				cgblDBEscMode;
		KEYMSG				cgLastKey;
		KEYMSG				cgNewKey;
	private:
		BIC_ENV				cgBICenv;
		SBUFFER				cmdSBUF;
		SYS_Thread<CON_ReadInLine>	BICThread;
	protected:
		virtual		int32	BICThreadFun(void *p);
	private:
		void		KeyLeft			(void);
		void		KeyRight		(void);
		void		KeyHome			(void);
		void		KeyEnd			(void);
		void		KeyBK			(void);
		void		KeyDel			(void);
		void		KeyUp			(void);
		void		KeyDown			(void);
		void		KeyReturn		(void);
		void		KeyEscape		(void);
		void		KeyTab			(void);
		void		KeyDBTab		(void);
	private:
		int32		InsertChar		(uint8 key);
		int32		InsertChar		(const uint8 *key,uint32 length);
		int32		DoContrl		(void);
		void		CheckPressStatus(void);
		int32		AnalyzeKey		(const MSG *pMsg);//no use pMsg under linux, and receive key from pMsg in win
		void		SwitchMainLH	(void){cgCLH = &cgMainLH;};
		void		SwitchSecondLH	(void){cgCLH = &cgSecondLH;};
	public:
		int32		Paste			(uint8 *key, uint32 length) {return(InsertChar(key, length));};
		int32		LoadDefault		(const STDSTR &filename = "console.ini");
		void		AddToMLH        (const STDSTR &newData){cgMainLH.Add(newData);};
	public:
		void		Init			(const ODEV_STDOUT *oDevSTDOUT)	{SetDefConsoleAttr(&cgBICenv);cgOdevSTDOUT = (ODEV_STDOUT*)oDevSTDOUT;};
		void		Init			(const SDTAPP *sdt)				{BICThread.ThreadInit(this, &CON_ReadInLine::BICThreadFun,"BIC",(void*)sdt);};
		void		Exit			(void)							{cgBICenv.blExit = -1; BICThread.ThreadStop();};
		void		RunWithBIC		(void)							{BICThread.ThreadRun();};
		void		ExecuteBICCMD	(const STDSTR &cmd)				{cmdSBUF.Put(cmd + "\n",G_ESCAPE_OFF);};
		int32		ReceiveKey		(const MSG *pMsg);//no use pMsg under linux, and receive key from pMsg in win
};
//------------------------------------------------------------------------------------------//
#endif
#endif
#endif
