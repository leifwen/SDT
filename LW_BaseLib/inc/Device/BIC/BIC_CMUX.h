//
//  BIC_CMUX.hpp
//  SDT
//
//  Created by Leif Wen on 26/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "BIC_Dev.h"
#include "CMUX.h"
#if defined BIC_Dev_h && defined CMUX_h
//------------------------------------------------------------------------------------------//
#ifndef BIC_CMUX_h
#define BIC_CMUX_h
#ifdef BIC_CMUX_h
//------------------------------------------------------------------------------------------//
class BIC_CMUX_ON : public BIC_BASE{
	public:
				 BIC_CMUX_ON(void) : BIC_BASE() {cgCommandID = BIC_ID_CMUX_ON;cgCommand = "on";};
		virtual ~BIC_CMUX_ON(void){;};
	public:
		virtual	CMDID	Command					(CMD_ENV* env,const STDSTR& msg,void* p)const;
		virtual	CMDID	Help					(CMD_ENV* env,uint32 flag)const;
	private:
		virtual	bool32	IsExitPressKeyMode		(CMD_ENV* env)const;
};
//------------------------------------------------------------------------------------------//
BIC_CLASSTYPE(CMUX_OFF,		"off");
BIC_CLASSTYPE(CMUX_CLD,		"cld");
BIC_CLASSTYPE(CMUX_PSC,		"psc");
BIC_CLASSTYPE(CMUX_FC,		"fc");
BIC_CLASSTYPE(CMUX_DTR,		"dtr");
BIC_CLASSTYPE(CMUX_RTS,		"rts");
BIC_CLASSTYPE(CMUX_DLCI,	"dlci");
BIC_CLASSTYPE(CMUX_LEN,		"len");
//------------------------------------------------------------------------------------------//
class BIC_CMUX : public BIC_BASE_S{
	public:
				 BIC_CMUX(void);
		virtual ~BIC_CMUX(void){;};
	public:
		BIC_CMUX_ON		cgSub_on;
		BIC_CMUX_OFF	cgSub_off;
		BIC_CMUX_CLD	cgSub_cld;
		BIC_CMUX_PSC	cgSub_psc;
		BIC_CMUX_FC		cgSub_fc;
		BIC_CMUX_DTR	cgSub_dtr;
		BIC_CMUX_RTS	cgSub_rts;
		BIC_CMUX_DLCI	cgSub_dlci;
		BIC_CMUX_LEN	cgSub_len;
};
//------------------------------------------------------------------------------------------//
#endif /* BIC_CMUX_h */
#endif /* BIC_CMUX_h */
#endif /* BIC_Dev_h */
