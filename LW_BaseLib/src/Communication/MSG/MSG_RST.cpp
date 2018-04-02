//
//  MSG_RST.cpp
//  SDT
//
//  Created by Leif Wen on 17/03/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "stdafx.h"
#include "MSG_RST.h"
#ifdef MSG_RST_h
//------------------------------------------------------------------------------------------//
MSG_RST::MSG_RST(uint32 size,void* p) : MSG_NODE(size,p)
#ifdef MSG_RMS_h
,cgMsgRMS(size,p)
#endif
#ifdef MSG_Terminal_h
,cgMsgTerminal(size,p)
#endif
#if defined MSG_Register_h
,cgMsgRegister(size,p)
#endif
{
#ifdef MSG_RMS_h
	Add(cgMsgRMS);
#endif
#ifdef MSG_Terminal_h
	Add(cgMsgTerminal);
#endif
#if defined MSG_Register_h
	Add(cgMsgRegister);
#endif
	selfName = "MSG_RST";
	SetFatherName("");
};
//------------------------------------------------------------------------------------------//
#endif /* MSG_RST_h */
