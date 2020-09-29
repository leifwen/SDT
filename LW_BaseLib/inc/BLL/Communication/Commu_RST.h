//
//  Commu_RST.h
//  SDT
//
//  Created by Leif Wen on 24/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "MSG_RST.h"
#include "Commu_Terminal.h"
#include "Commu_SSL.h"
#if defined MSG_RST_h && defined Commu_Terminal_h && defined Commu_SSL_h
//------------------------------------------------------------------------------------------//
#ifndef Commu_RST_h
#define Commu_RST_h
#ifdef Commu_RST_h
//------------------------------------------------------------------------------------------//
typedef COMMU_DRV_T<TMEM|TBIRDGE|TDISPOSE|TODEVBUS,COMMU_DRV_NOSMC,CORE_TSOCKET,MEM_MSG<MSG_RSTerminal>>	RSTBSocket;
typedef COMMU_NOSMC_T<TServer_T<RSTBSocket>>															RSTBServer;
//------------------------------------------------------------------------------------------//
#endif /* Commu_RST_h */
#endif /* Commu_RST_h */
#endif /* MSG_RMS_h */
