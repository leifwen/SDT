//
//  SYS_Log.cpp
//  SDT
//
//  Created by Leif Wen on 26/03/2017.
//  Copyright © 2017 leif wen. All rights reserved.
//
//------------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "SYS_Log.h"
#include "SYS_Time.h"
#include "DS_Tree.h"
#include "DS_Transform.h"
#include <sstream>
//------------------------------------------------------------------------------------------//
#ifdef LOGPRINT
//------------------------------------------------------------------------------------------//
#ifdef DS_Lock_h
DS_Lock		ErrLog;
//------------------------------------------------------------------------------------------//
void LockErrLog		(void){ErrLog.Lock();}
void UnlockErrLog	(void){ErrLog.Unlock();}
#else 
void LockErrLog		(void){;}
void UnlockErrLog	(void){;}
#endif
//------------------------------------------------------------------------------------------//
STDSTR LogTime(const STDSTR &style){
	DTIME	dtime;
	dtime.Now();
	return(SYS_FormatDateTime1970(style,dtime));
}
#ifdef DS_Tree_h
//------------------------------------------------------------------------------------------//
std::ostream& operator << (std::ostream& _tn,const TNFP* treeNode){
	return(_tn << "[" << TNFP::GetFullName(treeNode) << "]");
};
//------------------------------------------------------------------------------------------//
std::ostream& operator << (std::ostream& _tn,const TNFP& treeNode){
	return(_tn << "[" << TNFP::GetFullName(&treeNode) << "]");
};
#endif
#ifdef CommonDefH_VC
std::stringstream	g_LOGStream;
std::string			g_LOGString;
//------------------------------------------------------------------------------------------//
void DoLOG(void){
	g_LOGString = g_LOGStream.str();
	g_LOGString += "\r\n";
	g_LOGStream.clear();
	g_LOGStream.str("");
	DS_IO_NODE::GetDSIOList().Save(nullptr,OUD_FILEADD("LOG.txt"),g_LOGString);
}
#endif
//------------------------------------------------------------------------------------------//
#endif /* LOGPRINT */
