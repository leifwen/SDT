//
//  Config.h
//  SDT
//
//  Created by Leif Wen on 17/01/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#ifndef Config_h
#define Config_h
//------------------------------------------------------------------------------------------//
//#define CommonDefH_VC2013
//#define CommonDefH_Linux
#define CommonDefH_MAC
//------------------------------------------------------------------------------------------//
#ifdef CommonDefH_VC2013
	#define CommonDefH_VC
#endif
#ifdef CommonDefH_MAC
	#define CommonDefH_Unix
#endif
#ifdef CommonDefH_Linux
	#define CommonDefH_Unix
#endif
//------------------------------------------------------------------------------------------//
#define USE_MUTEXLOCK
#define USE_OPENSSL

#ifdef CommonDefH_VC
	#define CommonDefH_EnableLicense
#endif
#ifdef CommonDefH_Unix
	#undef USE_MUTEXLOCK
	//#undef USE_OPENSSL
#endif
#ifdef CommonDefH_Linux
	//#undef USE_OPENSSL
#endif
#ifdef CommonDefH_MAC
	#ifdef USE_OPENSSL
		#define USE_MAC_OPENSSL
		//#define USE_MAC_Crypto
	#else
		#define USE_MAC_Crypto
	#endif
#endif
//------------------------------------------------------------------------------------------//
#ifndef SWVERSION_SHORTNAME
	#define	SWVERSION_SHORTNAME		"LW_BASELib"
#endif
#ifndef SWVERSION_VER
	#define	SWVERSION_VER			"V0.6"
#endif
//------------------------------------------------------------------------------------------//
#define LOGPRINT
//------------------------------------------------------------------------------------------//
#include "GlobalDef.h"
//------------------------------------------------------------------------------------------//
#endif /* Config_h */
