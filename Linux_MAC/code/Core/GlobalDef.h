/*
 * Copyright (c) 2012-2014 
 * All rights reserved.
 *
 * File Name: GlobalDef.h
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2013.11.28
*/
#ifndef GlobalDefH
#define GlobalDefH
/***************************************************************************************/
#define USE_MUTEXLOCK
#define USE_OPENSSL

#ifdef CommonDefH_VC
	#define CommonDefH_EnableLicense
	#define CommonDefH_EnableSMC
#endif
#ifdef CommonDefH_Unix
	#undef USE_MUTEXLOCK
	//#undef USE_OPENSSL
#endif
#ifdef CommonDefH_Linux
	//#undef USE_OPENSSL
#endif
/***************************************************************************************/
#define		SWVERSION_COM
#define		SWVERSION_TCPClient
#define		SWVERSION_UDPCLient
#define		SWVERSION_TCPServer
#define		SWVERSION_UDPServer
#define		SWVERSION_AUXCOM
#define		SWVERSION_CMUX
#define		SWVERSION_SCRIPT
#define		SWVERSION_NTP
#define		SWVERSION_DICT

#define		SWVERSION_SHORTNAME			"SDT"
#define		SWVERSION_VER				"V0.4.1"
#define		SWVERSION_WR				"Written by Leif Wen"

#ifdef CommonDefH_MAC
	#define		SWVERSION_APPID			"LeifWen.SDT.MAC.V0.4.0"
	#define		SWVERSION_NAME			"Script Debugging Tools(MAC Version)"
	#define		SWVERSION				"Script Debugging Tools(MAC Version) V0.4.1, Written by Leif Wen"
	#define		CMUX_COM_NAME			"/dev/ttySDTMUX"
#endif
#ifdef CommonDefH_Linux
	#define		SWVERSION_APPID			"LeifWen.SDT.Linux.V0.4.0"
	#define		SWVERSION_NAME			"Script Debugging Tools(Linux Version)"
	#define		SWVERSION				"Script Debugging Tools(Linux Version) V0.4.1, Written by Leif Wen"
	#define		CMUX_COM_NAME			"/dev/ttySDTMUX"
#endif
#ifdef CommonDefH_VC2013
	#define		SWVERSION_APPID			"LeifWen.SDT.MFC.V0.4.0"
	#define		SWVERSION_NAME			"Script Debugging Tools(VC2013 MFC Version)"
	#define		SWVERSION				"SDT(VC2013 MFC Version) V0.4.1, Written by Leif Wen"
#endif
#define	DEV_LINE_START			"****************************************\n"
#define	DIVIDING_LINE			"----------------------------------------------------------------------"
#define	ODEV_LOG_FDIR			"_Record"
#define	DEFAULT_TIMEFORMATE		"hh:mm:ss.zzz"
/***************************************************************************************/
#define	G_MAXBUFFER_SIZE		(1024 * 128)
/***************************************************************************************/
#endif
