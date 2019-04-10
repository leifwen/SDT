//
//  GlobalDef.h
//  SDT
//
//  Created by Leif Wen on 23/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#ifndef GlobalDef_h
#define GlobalDef_h
//------------------------------------------------------------------------------------------//
#undef SWVERSION_SHORTNAME
#undef SWVERSION_VER

#define		SWVERSION_COM
#define		SWVERSION_TCPClient
#define		SWVERSION_UDPCLient
#define		SWVERSION_TCPServer
#define		SWVERSION_UDPServer
#define		SWVERSION_AUXDEVICE
#define		SWVERSION_CMUX
#define		SWVERSION_SCRIPT
#define		SWVERSION_NTP
#define		SWVERSION_DICT

#define		SWVERSION_SHORTNAME		"SDT"
#define		SWVERSION_VER			"V0.6.4"
#define		SWVERSION_WR			"Written by Leif Wen"

#ifdef CommonDefH_MAC
#define		SWVERSION_APPID			"LeifWen.SDT.MAC.V0.6.0"
#define		SWVERSION_NAME			"Script Debugging Tools(MAC Version)"
#define		SWVERSION				"Script Debugging Tools(MAC Version) V0.6.4, Written by Leif Wen"
#define		CMUX_COM_NAME			"/dev/ttySDTMUX"
#endif
#ifdef CommonDefH_Linux
#define		SWVERSION_APPID			"LeifWen.SDT.Linux.V0.6.0"
#define		SWVERSION_NAME			"Script Debugging Tools(Linux Version)"
#define		SWVERSION				"Script Debugging Tools(Linux Version) V0.6.4, Written by Leif Wen"
#define		CMUX_COM_NAME			"/dev/ttySDTMUX"
#undef		SWVERSION_DICT
#endif
#ifdef CommonDefH_VC2013
#define		SWVERSION_APPID			"LeifWen.SDT.MFC.V0.6.0"
#define		SWVERSION_NAME			"Script Debugging Tools(VC2013 MFC Version)"
#define		SWVERSION				"SDT(VC2013 MFC Version) V0.6.4, Written by Leif Wen"
#define		SWVERSION_TOOLS
#undef		SWVERSION_DICT
#endif
#define	DEV_LINE_STAR			"****************************************\n"
#define	DIVIDING_LINE			"----------------------------------------------------------------------\n"
#define	ODEV_LOG_FDIR			"_Record"
#define	DEFAULT_TIMEFORMATE		"[hh:mm:ss.zzz]"
#define	DEFAULT_INI_FILE		"Default.ini"
//------------------------------------------------------------------------------------------//
#endif /* GlobalDef_h */
