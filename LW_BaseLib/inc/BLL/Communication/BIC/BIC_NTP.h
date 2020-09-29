//
//  BIC_NTP.h
//  SDT
//
//  Created by Leif Wen on 16/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "BIC_Commu.h"
#include "Commu_NTPServer.h"
#if defined BIC_Commu_h && defined Commu_NTPServer_h
//------------------------------------------------------------------------------------------//
#ifndef BIC_NTP_h
#define BIC_NTP_h
#ifdef BIC_NTP_h
//------------------------------------------------------------------------------------------//
BIC_CLASSTYPE	(NTPS,	"ntps");
BIC_CLASSTYPE	(NTP,	"ntp");
//------------------------------------------------------------------------------------------//
#else
	#undef SWVERSION_NTP
#endif /* BIC_NTP_h */
#endif /* BIC_NTP_h */
#else
	#undef SWVERSION_NTP
#endif /* BIC_Define_h */
