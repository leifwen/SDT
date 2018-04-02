//
//  BIF_CMD.h
//  SDT
//
//  Created by Leif Wen on 22/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "BIF_Define.h"
#ifdef BIF_Define_h
//------------------------------------------------------------------------------------------//
#ifndef BIF_CMD_h
#define BIF_CMD_h
#ifdef BIF_CMD_h
#include "BIF_Transform.h"
#include "BIF_Control.h"
#include "BIF_Dev.h"
#include "BIF_EXE.h"
//------------------------------------------------------------------------------------------//
class BIF_CMD : public BIF_BASE{
	public:
				 BIF_CMD(void);
		virtual ~BIF_CMD(void){;};
	public:
		virtual	CMDID	Help	(CMD_ENV* env,uint32 flag)const;
	public:
#ifdef BIF_Transform_h
		BIF_PRINT				cgSubC_Print;
#endif
#ifdef BIF_Transform_h
		BIF_SEARCH				cgSubC_Search;
#endif
#ifdef BIF_Control_h
		BIF_DELAY				cgSubC_Delay;
		BIF_WAIT				cgSubC_Wait;
		BIF_SYNCHRONOUS			cgSubC_Synchronous;
		BIF_LABLE				cgSubC_Lable;
		BIF_GOTO				cgSubC_Goto;
		BIF_STOP				cgSubC_Stop;
		BIF_BREAK				cgSubC_Break;
#endif
#ifdef BIF_Dev_h
		BIF_OPEN				cgSubC_Open;
		BIF_CLOSE				cgSubC_Close;
		BIF_BR					cgSubC_BR;
		BIF_DTR					cgSubC_DTR;
		BIF_RTS					cgSubC_RTS;
		
		BIF_SETRECVDATAMODE		cgSubC_SetRecvDataMode;
		BIF_SETRECMSGREPORT		cgSubC_SetRecMsgReport;
#endif
#ifdef BIF_EXE_h
		BIF_RUNEXE				cgSubC_RunEXE;
		BIF_STOPEXE				cgSubC_StopEXE;
		BIF_WAITEXE				cgSubC_WaitEXE;
		BIF_GETEXE				cgSubC_GetEXE;
#endif
};
//------------------------------------------------------------------------------------------//
#endif /* BIF_CMD_h */
#endif /* BIF_CMD_h */
#endif /* BIF_Define_h */
