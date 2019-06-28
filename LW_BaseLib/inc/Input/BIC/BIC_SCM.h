//
//  BIC_SCM.h
//  SDT
//
//  Created by Leif Wen on 19/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

//------------------------------------------------------------------------------------------//
#include "BIC_Define.h"
#if defined BIC_Define_h && defined SList_h
//------------------------------------------------------------------------------------------//
#ifndef BIC_SCM_h
#define BIC_SCM_h
#ifdef BIC_SCM_h
//------------------------------------------------------------------------------------------//
BIC_CLASSTYPE(SC_EG,		"eg");
BIC_CLASSTYPE(SC_LOAD,		"load");
BIC_CLASSTYPE(SC_SAVE,		"save");
BIC_CLASSTYPE(SC_LS,		"ls/list");
BIC_CLASSTYPE(SC_SET,		"set");
BIC_CLASSTYPE(SC_CLONE,		"clone");
BIC_CLASSTYPE(SC_DEL,		"del");
BIC_CLASSTYPE(SC_MVUP,		"mvup");
BIC_CLASSTYPE(SC_MVDN,		"mvdn");
BIC_CLASSTYPE(SC_COMMAND,	"cmd/command");
//------------------------------------------------------------------------------------------//
class BIC_SCM : public BIC_BASE_S{
	public:
				 BIC_SCM(void);
		virtual ~BIC_SCM(void){;};
	public:
		BIC_SC_EG		cgSub_eg;
		BIC_SC_LOAD		cgSub_load;
		BIC_SC_SAVE		cgSub_save;
		BIC_SC_LS		cgSub_ls;
		BIC_SC_SET		cgSub_set;
		BIC_SC_CLONE	cgSub_clone;
		BIC_SC_DEL		cgSub_del;
		BIC_SC_MVUP		cgSub_mvup;
		BIC_SC_MVDN		cgSub_mvdn;
		BIC_SC_COMMAND	cgSub_command;
};
//------------------------------------------------------------------------------------------//
#endif /* BIC_SCM_h */
#endif /* BIC_SCM_h */
#endif /* BIC_Define_h */
