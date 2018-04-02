//
//  SYS_Log.h
//  SDT
//
//  Created by Leif Wen on 26/03/2017.
//  Copyright © 2017 leif wen. All rights reserved.
//

#ifndef SYS_Log_h
#define SYS_Log_h
//------------------------------------------------------------------------------------------//
#include "BasicDefine.h"
//------------------------------------------------------------------------------------------//
#ifdef LOGPRINT

#ifdef CommonDefH_VC
	#define ELogBase(x) 	LockErrLog();g_LOGStream << LogTime() << x; DoLOG();UnlockErrLog()
#else
	#define ELogBase(x) 	LockErrLog();std::cerr << LogTime() << x << std::endl;UnlockErrLog();
#endif
	#define ELogThread(x,y)	"[" << x.fatherName << ".Thread:" << x.selfName << "]" << y

	#ifdef LOGPRINT_ENABLE
		#define ELog(x)				ELogBase(x);
	#else
		#define ELog(x);
	#endif
	#ifdef LOGPRINT_ENABLE2
		#define E2Log(x)			ELogBase(x);
	#else
		#define E2Log(x);
	#endif
	#ifdef LOGTHREAD_ENABLE
		#define ETLog(x)  			ELogBase(x);

		#define ETLogThreadStart(x)	ETLog(ELogThread(x,"Running"));
		#define ETLogThreadStop(x)	ETLog(ELogThread(x,"Stop"));
	#else
		#define ETLog(x)
		#define ETLogThreadStart(x)
		#define ETLogThreadStop(x)
	#endif
#else
	#define ELogBase(x)
	#define ELog(x)
	#define E2Log(x)

	#define ETLog(x)
	#define ETLogThreadStart(x)
	#define ETLogThreadStop(x)
#endif
//------------------------------------------------------------------------------------------//
#ifdef LOGPRINT
class TREE_NODE_FRAME_POOL;
void LockErrLog		(void);
void UnlockErrLog	(void);
//------------------------------------------------------------------------------------------//
STDSTR LogTime(const STDSTR &style = "[hh:mm:ss.zzz]  ");

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
