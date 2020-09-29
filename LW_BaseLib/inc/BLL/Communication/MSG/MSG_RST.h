//
//  MSG_RST.h
//  SDT
//
//  Created by Leif Wen on 17/03/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "MSG_Center.h"
#include "MSG_RMS.h"
#include "MSG_Terminal.h"
#include "MSG_Register.h"
#include "MSG_Files.h"
#ifdef MSG_Center_h
//------------------------------------------------------------------------------------------//
#ifndef MSG_RST_h
#define MSG_RST_h
#ifdef MSG_RST_h
//------------------------------------------------------------------------------------------//
class MSG_RSTerminal : public MSG_NODE{
	public:
				 MSG_RSTerminal(void);
		virtual ~MSG_RSTerminal(void){;};
	public:
#ifdef MSG_RMS_h
		MSG_RMS			cgMsgRMS;
#endif
#ifdef MSG_Terminal_h
		MSG_Terminal	cgMsgTerminal;
#endif
#ifdef MSG_Register_h
		MSG_Rerister	cgMsgRegister;
#endif
#ifdef MSG_Files_h
		MSG_Files		cgMsgFiles;
#endif
};
//------------------------------------------------------------------------------------------//
#endif /* MSG_RST_h */
#endif /* MSG_RST_h */
#endif /* MSG_Center_h */
