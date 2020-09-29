//
//  BIC_BT.h
//  SDT
//
//  Created by Leif Wen on 12/09/2020.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "BIC_Commu.h"
#if defined BIC_Commu_h
//------------------------------------------------------------------------------------------//
#ifndef BIC_BT_h
#define BIC_BT_h
#ifdef BIC_BT_h
//------------------------------------------------------------------------------------------//
class BIC_BT_SCAN : public BIC_BASE{
	public:
				 BIC_BT_SCAN(void) : BIC_BASE() {cgCommandID = BIC_ID_BT_SCAN;cgCommand = "scan";};
		virtual ~BIC_BT_SCAN(void){;};
	public:
		virtual	CMDID	Command				(CMD_ENV* env,const STDSTR& msg,void* p)const;
		virtual	CMDID	Help				(CMD_ENV* env,uint32 flag)const;
	private:
		virtual	bool32	IsExitPressKeyMode	(CMD_ENV* env)const;
};
//------------------------------------------------------------------------------------------//
BIC_CLASSTYPE	(BT_LIST	,"list");
BIC_CLASSTYPE	(BT_SPP		,"spp");
//------------------------------------------------------------------------------------------//
class BIC_BT : public BIC_BASE_S{
	public:
				 BIC_BT(void);
		virtual ~BIC_BT(void){;};
	private:
		BIC_BT_SCAN			cgSub_scan;
		BIC_BT_LIST			cgSub_list;
		BIC_BT_SPP			cgSub_spp;
};
//------------------------------------------------------------------------------------------//
#endif /* BIC_BT_h */
#endif /* BIC_BT_h */
#endif
