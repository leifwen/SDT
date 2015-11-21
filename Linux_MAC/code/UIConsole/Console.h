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
#ifndef ConsoleH
#define ConsoleH
//------------------------------------------------------------------------------------------//
#include "BasicDef.h"
#include "BasicClass.h"
#include "SYS_Time.h"
//------------------------------------------------------------------------------------------//
#ifdef CommonDefH_Unix
#define VK_UP		(1)
#define VK_DOWN		(2)
#define VK_LEFT		(3)
#define VK_RIGHT	(4)
#define VK_HOME		(5)
#define VK_END		(6)
#define VK_PRIOR	(7)
#define VK_NEXT		(8)
#define VK_DELETE	(9)
#define VK_BACK		(10)
#define VK_RETURN	(11)
#define VK_ESCAPE	(12)
#define VK_SPACE	(13)
#define VK_INSERT	(14)
#define VK_TAB		(15)
#define VK_CONTROL	(16)
#define VK_MENU		(17)
#endif
#define VK_DBCLK   (0x01)
#define VK_COCLK   (0x01 << 1)
//------------------------------------------------------------------------------------------//
class CON_LineEdit{
	public:
				 CON_LineEdit(void){Init();};
		virtual ~CON_LineEdit(void){;};
	private:
		std::string	cgEditContent;
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
		void				SetData		(std::string const &newData,uint32 cur);
		const std::string&	GetData		(void)const	{return(cgEditContent);};
		uint32				GetLength	(void)const	{return((uint32)cgEditContent.length());};
};
//------------------------------------------------------------------------------------------//
class CON_LineHistory{
		enum{MaxRecord = 512};
	public:
				 CON_LineHistory(void){Init();};
		virtual ~CON_LineHistory(void){;};
	private:
		std::string		cgEditHistory[MaxRecord];
		int32			cgCount;
		int32			cgHead;
		int32			cgTop;
		int32			cgHistoryCur;
	public:
		void				Init			(void);
		int32				SetPrior		(void);
		int32				SetNext			(void);
		void				Add				(const std::string &newData);
		const std::string&	GetData			(void)const{return(cgEditHistory[cgHistoryCur]);};
		std::string			GetHistory		(void);
		std::string			GetSimilar		(const std::string &inputStr);
		std::string			GetSimilarGroup	(const std::string &inputStr);
		void				ClearHistory	(void){Init();};
};
//------------------------------------------------------------------------------------------//
struct KEYMSG{
	uint32			message;
	uint8			wParam;
	uint32			lParam;
	SYS_DateTime	keyTime;
};
#ifdef CommonDefH_Unix
struct MSG{
	uint32			message;
	uint8			wParam;
	uint32			lParam;
};
#define WM_NOKEY    (0)
#define WM_CHAR     (1)
#define WM_KEYDOWN  (2)
#endif
struct BICPAR;
class ODEV_NODE_SDOUT;
//------------------------------------------------------------------------------------------//
class CON_ReadInLine{
	public:
				 CON_ReadInLine(void);
		virtual ~CON_ReadInLine(void){;};
	private:
		BICPAR				*cgBICPAR;
		ODEV_NODE_SDOUT		*cODevOut;
		CON_LineHistory		cgMainLH;
		CON_LineHistory		cgSecondLH;
		CON_LineHistory		*cgCLH;
		CON_LineEdit		cgCInput;
		int32				cgblDBEscMode;
		KEYMSG				cgLastKey;
		KEYMSG				cgNewKey;

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
	
		int32		InsertChar		(uint8 key);
		int32		InsertChar		(const uint8 *key,uint32 length);
		int32		DoContrl		(void);
		void		CheckPressStatus(void);
		int32		AnalyzeKeyStream(const MSG *pMsg);
		void		SwitchMainLH	(void){cgCLH = &cgMainLH;};
		void		SwitchSecondLH	(void){cgCLH = &cgSecondLH;};
	public:
		void		Init			(const BICPAR *tBICPAR,const ODEV_NODE_SDOUT *tNode){
			cgBICPAR = (BICPAR*)tBICPAR;
			cODevOut = (ODEV_NODE_SDOUT*)tNode;
			SwitchMainLH();
		};
		void		AddToMLH        (const std::string &newData){cgMainLH.Add(newData);};
		int32		AKey			(const MSG *pMsg);
		int32		Paste			(uint8 *key, uint32 length){return(InsertChar(key, length));};
};
//------------------------------------------------------------------------------------------//
#endif
