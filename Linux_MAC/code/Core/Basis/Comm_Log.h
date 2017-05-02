//
//  Comm_Log.h
//  SDT
//
//  Created by Leif Wen on 26/03/2017.
//  Copyright © 2017 leif wen. All rights reserved.
//

#ifndef Comm_Log_hpp
#define Comm_Log_hpp
//------------------------------------------------------------------------------------------//
#include "BasicDef.h"
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#ifdef LOGPRINT
	#ifdef LOGPRINT_ENABLE
		#define ELog(x)  LockErrLog();std::cerr << LogTime() << x << std::endl;UnlockErrLog();
	#else
		#define ELog(x);
	#endif
	#ifdef LOGPRINT_ENABLE2
		#define E2Log(x)  LockErrLog();std::cerr << LogTime() << x << std::endl;UnlockErrLog();
	#else
		#define E2Log(x);
	#endif
#else
	#define ELog(x);
	#define E2Log(x);
#endif
//------------------------------------------------------------------------------------------//
#ifdef LOGPRINT
class TREE_NODE;
void LockErrLog		(void);
void UnlockErrLog	(void);
//------------------------------------------------------------------------------------------//
STDSTR LogTime(const STDSTR &style = "[hh:mm:ss.zzz]  ");
std::ostream& operator << (std::ostream& _pn,const TREE_NODE *treeNode);
//------------------------------------------------------------------------------------------//
#endif /* LOGPRINT */
//------------------------------------------------------------------------------------------//
#endif /* Comm_Log_hpp */
