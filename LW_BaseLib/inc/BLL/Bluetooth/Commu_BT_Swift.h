//
//  Commu_BT_Swift.h
//  SDT
//
//  Created by Leif Wen on 12/09/2020.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

//------------------------------------------------------------------------------------------//
#ifndef Commu_BT_Swift_h
#define Commu_BT_Swift_h
#ifdef Commu_BT_Swift_h
//------------------------------------------------------------------------------------------//
extern "C" int		Swift_BT_Scan(void);
extern "C" int		Swift_BT_StopScan(void);
extern "C" int		Swift_BT_ScanState(void);
extern "C" bool		Swift_BT_IsUpdate(void);
extern "C" int		Swift_BT_Connect(const char*);
extern "C" void		Swift_BT_Disconnect(const char*);
extern "C" int		Swift_BT_OpenRFCOMMChannelSync(const char*,int8,uint32);
extern "C" void		Swift_BT_CloseRFCOMMChannelSync(const char*);
extern "C" int		Swift_BT_RFCOMMChannelState(void);
extern "C" int		Swift_BT_WriteRFCOMMChannelSync(const char*,const char*,int32);
extern "C" char*	Swift_BT_GetDevicesDescription(void);
//------------------------------------------------------------------------------------------//
#endif /* Commu_BT_Swift_h */
#endif /* Commu_BT_Swift_h */
