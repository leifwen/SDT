//
//  SYS_Log.h
//  SDT
//
//  Created by Leif Wen on 26/03/2017.
//  Copyright © 2017 leif wen. All rights reserved.
//

//------------------------------------------------------------------------------------------//
#ifndef SYS_Log_h
#define SYS_Log_h
//------------------------------------------------------------------------------------------//
#include "BasicDefine.h"
#include <sstream>
//------------------------------------------------------------------------------------------//
#ifdef LOGPRINT

#ifdef CommonDefH_VC
	#define ELogBase(x) 	LockErrLog();g_LOGStream << LogTime() << x; DoLOG();UnlockErrLog();
#else
	#define ELogBase(x) 	LockErrLog();std::cerr << LogTime() << x << std::endl;UnlockErrLog();
#endif
	#define ELogThread(x,y)	LogThread(x) << y

	#ifdef LOGPRINT_ENABLE
		#define ELogLock(x)			{if (x != G_FALSE) LockErrLog(); else UnlockErrLog();}
		#define ELogLine(x)			x
		#define ELog(x)				ELogBase(x);
		#define ELogIF(x,y)			if (x != G_FALSE) {y;}
		#define ELogON(this,x,y)	if ((x == "ALL") || (TNFP::GetFullName(this) == x)) {y;}
		#define ELogTNFP(x,y)		ELogON(this,x,y)
	#else
		#define ELogLock(x);
		#define ELogLine(x);
		#define ELog(x);
		#define ELogIF(x,y);
		#define ELogON(this,x,y);
		#define ELogTNFP(x,y);
	#endif

	#ifdef LOGTHREAD_ENABLE
		#define ETLog(x)  			ELogBase(x);

		#define ETLogThreadStart(x)	ETLog(ELogThread(x,"Running"));
		#define ETLogThreadStop(x)	ETLog(ELogThread(x,"Stop"));
	#else
		#define ETLog(x);
		#define ETLogThreadStart(x);
		#define ETLogThreadStop(x);
	#endif
#else
	#define ELogBase(x);
	#define ELogLine(x);
	#define ELog(x);
	#define ELogIF(x,y);
	#define ELogTNP(x,y);

	#define ETLog(x);
	#define ETLogThreadStart(x);
	#define ETLogThreadStop(x);
#endif
//------------------------------------------------------------------------------------------//
#ifdef LOGPRINT
class TREE_NODE_FRAME_POOL;
void LockErrLog		(void);
void UnlockErrLog	(void);
//------------------------------------------------------------------------------------------//
STDSTR	LogTime	(const STDSTR &style = "[hh:mm:ss.zzz]  ");
STDSTR	LogTitle(const TREE_NODE_FRAME_POOL* treeNode,const STDSTR& className,const STDSTR& funName,const STDSTR& dir);
STDSTR	LogTitle(const TREE_NODE_FRAME_POOL& treeNode,const STDSTR& className,const STDSTR& funName,const STDSTR& dir);
STDSTR	LogThread(const TREE_NODE_FRAME_POOL* treeNode);
STDSTR	LogThread(const TREE_NODE_FRAME_POOL& treeNode);
//------------------------------------------------------------------------------------------//
template<typename T>void DoLogFillSpace(std::stringstream* text,const T& first){
	*text << first;
};
//------------------------------------------------------------------------------------------//
template<typename T,typename... Args>void DoLogFillSpace(std::stringstream* text,const T& first, const Args&... args){
	DoLogFillSpace(text,first);
	DoLogFillSpace(text,args...);
};
//------------------------------------------------------------------------------------------//
template<typename T,typename... Args>STDSTR	LogFillSpace(uint32 length,const T& first,const Args&... args){
	std::stringstream	text;
	STDSTR	retStr;

	DoLogFillSpace(&text,first,args...);
	
	retStr = text.str();
	
	if (retStr.length() < length)
		retStr.insert(retStr.length(),length - retStr.length(),' ');
	return(retStr);
};
//------------------------------------------------------------------------------------------//
std::ostream& operator << (std::ostream& _tn,const TREE_NODE_FRAME_POOL* treeNode);
std::ostream& operator << (std::ostream& _tn,const TREE_NODE_FRAME_POOL& treeNode);
//------------------------------------------------------------------------------------------//
#ifdef CommonDefH_VC
extern std::stringstream	g_LOGStream;
void DoLOG(void);
#endif
//------------------------------------------------------------------------------------------//
#endif /* LOGPRINT */
//------------------------------------------------------------------------------------------//
#endif /* SYS_Log_h */
