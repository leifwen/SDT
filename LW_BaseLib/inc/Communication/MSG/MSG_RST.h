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
#ifdef MSG_Center_h
//------------------------------------------------------------------------------------------//
#ifndef MSG_RST_h
#define MSG_RST_h
#ifdef MSG_RST_h
//------------------------------------------------------------------------------------------//
class MSG_RST : public MSG_NODE{
	public:
				 MSG_RST(uint32 size,void* p);
		virtual ~MSG_RST(void){;};
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
};
//------------------------------------------------------------------------------------------//
#endif /* MSG_RST_h */
#endif /* MSG_RST_h */
#endif /* MSG_Center_h */
