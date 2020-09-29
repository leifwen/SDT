//
//  MSG_RST.cpp
//  SDT
//
//  Created by Leif Wen on 17/03/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "stdafx.h"
//------------------------------------------------------------------------------------------//
#include "MSG_RST.h"
#ifdef MSG_RST_h
//------------------------------------------------------------------------------------------//
MSG_RSTerminal::MSG_RSTerminal(void) : MSG_NODE(){
#ifdef MSG_RMS_h
	AppendDown(cgMsgRMS);
#endif
#ifdef MSG_Terminal_h
	AppendDown(cgMsgTerminal);
#endif
#if defined MSG_Register_h
	AppendDown(cgMsgRegister);
#endif
#if defined MSG_Files_h
	AppendDown(cgMsgFiles);
#endif
	selfName = "MSG_RST";
	SetUpName("");
};
//------------------------------------------------------------------------------------------//
#endif /* MSG_RST_h */
