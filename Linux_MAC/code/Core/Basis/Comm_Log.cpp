//
//  Comm_Log.cpp
//  SDT
//
//  Created by Leif Wen on 26/03/2017.
//  Copyright © 2017 leif wen. All rights reserved.
//
//------------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "Comm_Log.h"
#include "SYS_Time.h"
#include "Comm_Tree.h"
//------------------------------------------------------------------------------------------//
#ifdef LOGPRINT
//------------------------------------------------------------------------------------------//
SYS_Lock		ErrLog;
//------------------------------------------------------------------------------------------//
void LockErrLog(void){ErrLog.Lock();}
void UnlockErrLog(void){ErrLog.Unlock();}
//------------------------------------------------------------------------------------------//
STDSTR LogTime(const STDSTR &style){
	TIME	dtime;
	dtime.Now();
	return(SYS_FormatDateTime1970(style,dtime));
}
//------------------------------------------------------------------------------------------//
std::ostream& operator << (std::ostream& _pn,const TREE_NODE *treeNode){
	#ifdef Comm_TreeH
	return(_pn << "[" << treeNode->selfName << "]");
	#else
	return(_pn);
	#endif
};
//------------------------------------------------------------------------------------------//
#endif /* LOGPRINT */
