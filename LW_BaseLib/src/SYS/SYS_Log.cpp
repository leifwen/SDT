//
//  SYS_Log.cpp
//  SDT
//
//  Created by Leif Wen on 26/03/2017.
//  Copyright © 2017 leif wen. All rights reserved.
//

#include "stdafx.h"
//------------------------------------------------------------------------------------------//
#include "SYS_Log.h"
#include "SYS_Time.h"
#include "DS_Transform.h"
#include <sstream>
//------------------------------------------------------------------------------------------//
#if defined SYS_Log_h && defined SYS_Time_h && defined DS_Transform_h
#ifdef LOGPRINT
//------------------------------------------------------------------------------------------//
#ifdef DS_Lock_h
DS_SpinLock		ErrLog;
//------------------------------------------------------------------------------------------//
void LockErrLog		(void){ErrLog.Set();}
void UnlockErrLog	(void){ErrLog.Clr();}
#else 
void LockErrLog		(void){;}
void UnlockErrLog	(void){;}
#endif
#define TREENAME_SIZE	54
#define CLASSNAME_SIZE	16
#define FUNNAME_SIZE	24
//------------------------------------------------------------------------------------------//
STDSTR LogTime(const STDSTR &style){
	DTIME	dtime;
	dtime.Now();
	return(SYS_FormatDateTime1970(style,dtime));
};
//------------------------------------------------------------------------------------------//
STDSTR LogThread(const TNFP* treeNode){
	return(LogFillSpace(TREENAME_SIZE+CLASSNAME_SIZE+FUNNAME_SIZE, "[", treeNode->fatherName, ".Thread:", treeNode->selfName, "]") + ":: ");
};
//------------------------------------------------------------------------------------------//
STDSTR LogThread(const TNFP& treeNode){
	return(LogThread(&treeNode));
};
//------------------------------------------------------------------------------------------//
STDSTR LogTitle(const TNFP* treeNode,const STDSTR& className,const STDSTR& funName,const STDSTR& dir){
	STDSTR	retStr;
	uint32	length;
	
	retStr = "[";
	retStr += TNFP::GetFullName(treeNode);
	retStr += "]";
	
	length = TREENAME_SIZE;
	if (retStr.length() < length)
		retStr.insert(retStr.length(),length - retStr.length(),' ');

	length = (uint32)retStr.length() + CLASSNAME_SIZE;
	retStr += className;
	if (retStr.length() < length)
		retStr.insert(retStr.length(),length - retStr.length(),' ');
	
	length = (uint32)retStr.length() + FUNNAME_SIZE;
	retStr += ".";
	retStr += funName;
	retStr += "()";
	if (retStr.length() < length)
		retStr.insert(retStr.length(),length - retStr.length(),' ');
	
	retStr += dir;
	retStr += " ";
	return(retStr);
};
//------------------------------------------------------------------------------------------//
STDSTR LogTitle(const TNFP& treeNode,const STDSTR& className,const STDSTR& funName,const STDSTR& dir){
	return(LogTitle(&treeNode,className,funName,dir));
};
//------------------------------------------------------------------------------------------//
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
	DS_IO_NODE::GetDSIOList().Save(nullptr,OUD_FILEApp("LOG.txt"),g_LOGString);
}
#endif
//------------------------------------------------------------------------------------------//
#endif /* LOGPRINT */
#endif /* defined SYS_Log_h && defined SYS_Time_h && defined DS_Tree_h */
