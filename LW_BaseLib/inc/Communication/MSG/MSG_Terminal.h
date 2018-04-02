//
//  MSG_Terminal.h
//  SDT
//
//  Created by Leif Wen on 16/03/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "MSG_Center.h"
#ifdef MSG_Center_h
//------------------------------------------------------------------------------------------//
#ifndef MSG_Terminal_h
#define MSG_Terminal_h
#ifdef MSG_Terminal_h
//------------------------------------------------------------------------------------------//
class MSG_Terminal : public MSG_NODE{
	protected:
		enum	{RFLAG_C = 3, RFLAG_S = MSG_NODE::RFLAG_S + MSG_NODE::RFLAG_C};
		enum	{TER_blSetupTerminalY = RFLAG_CREATE(0)
				,TER_blSetupTerminalN = RFLAG_CREATE(1)
				,TER_blCloseTerminalY = RFLAG_CREATE(2)
		};
	public:
				 MSG_Terminal(uint32 size,void* p);
		virtual ~MSG_Terminal(void){;};
	private:
		SYS_Thread<MSG_Terminal>	BICThread;
	private:
				bool32	BICThreadOnStop				(void* commu);
				bool32	BICThreadFun				(void* commu);
		virtual	CMDID	MessageProcessing			(CMD_ENV* env,const uint32& mID,const STDSTR& msg,void* commu);
	public:
		virtual	void	Reset						(void* commu);
		virtual	void	Close						(void* commu);
				bool32	Send_REQ_SetupTerminal		(CMD_ENV* env);
				bool32	Send_REQ_CloseTerminal		(CMD_ENV* env);
				bool32	CheckTerminalClosed			(void);
};
//------------------------------------------------------------------------------------------//
#endif /* MSG_Terminal_h */
#endif /* MSG_Terminal_h */
#endif /* MSG_Center_h */
