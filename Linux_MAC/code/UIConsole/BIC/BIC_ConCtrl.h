//
//  BIC_ConCtrl.h
//  SDT
//
//  Created by Leif Wen on 04/04/2017.
//  Copyright © 2017 leif wen. All rights reserved.
//
//------------------------------------------------------------------------------------------//
#include "BIC_B.h"
//------------------------------------------------------------------------------------------//
#ifdef BIC_BH
#ifndef BIC_ConCtrl_hpp
#define BIC_ConCtrl_hpp
#ifdef BIC_ConCtrl_hpp
//------------------------------------------------------------------------------------------//
class BIC_DISPLAY : public BIC_Node{
	public:
				 BIC_DISPLAY(void) : BIC_Node() {cgCommand = "display";cgReturnCode = BI_RETCODE_DISPLAY;};
		virtual ~BIC_DISPLAY(void){;};
	public:
		virtual	int32	Command (BIC_ENV *env,const STDSTR &par,void *p)const;
		virtual	int32	Help	(BIC_ENV *env,int32 blDetail = 1)const{
			PrintHelpItem(env,cgCommand,"Enable/disable view.");
			if (blDetail == 0)
				return(BI_RETCODE_DISPLAY);
			PrintHelpSubItem(env,"[on"		,"Always enable.");
			PrintHelpSubItem(env,"|off"		,"Always disable.");
			PrintHelpSubItem(env,"|auto]"	,"Set in auto mode.");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class BIC_NEWRECORD : public BIC_Node{
	public:
				 BIC_NEWRECORD(void) : BIC_Node() {cgCommand = "nr/newrecord";cgReturnCode = BI_RETCODE_NEWRECORD;};
		virtual ~BIC_NEWRECORD(void){;};
	public:
		virtual	int32	Command (BIC_ENV *env,const STDSTR &par,void *p)const;
		virtual	int32	Help	(BIC_ENV *env,int32 blDetail = 1)const{
			PrintHelpItem(env,cgCommand,"Create new record file.");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
#endif
#endif
#endif /* BIC_ConCtrl_hpp */
