/*
 * Copyright (c) 2012-2014 
 * All rights reserved.
 *
 * File Name: ODEV_FILE.h
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2013.7.5
 *			: 2014.1.13 remove support BCB2012
 *			: 2014.1.14 remove support Txt output
*/
#ifndef ODEV_FILEH
#define ODEV_FILEH
//------------------------------------------------------------------------------------------//
#include "ODEV_Node.h"
#include "SYS_Time.h"
//------------------------------------------------------------------------------------------//
#ifndef ODEV_LOG_FDIR
#define	ODEV_LOG_FDIR	"_Record"
#endif
//------------------------------------------------------------------------------------------//
class ODEV_NODE_FILE : public ODEV_NODE{
	public:
		enum{RFLAG_C = 0, RFLAG_S = ODEV_NODE::RFLAG_S + ODEV_NODE::RFLAG_C};
	public:
				 ODEV_NODE_FILE(void) : ODEV_NODE(){ cgfileName = ""; SYS_Delay_SetTS(&cgTimeS, 1000); };
				 ODEV_NODE_FILE(const std::string &tODEV,COLType tCOLType = COLType_COL) : ODEV_NODE(){ Init(tODEV,tCOLType); SYS_Delay_SetTS(&cgTimeS, 1000); };
		virtual ~ODEV_NODE_FILE(void){;};
	public:
		virtual int32	Print(G_LOCK_VAILD blLock = G_LOCK_ON);
	private:
		std::string		cgfileName;
		SYS_TIME_S		cgTimeS;
	public:
		void	Init(const std::string &tODEV,COLType tCOLType = COLType_COL){
			Spin_InUse_set();
			ChageType(tCOLType,G_LOCK_OFF);
			cgfileName = tODEV;
			Spin_InUse_clr();
		};
		std::string		GetFileName(void){return(cgfileName);};
};
//------------------------------------------------------------------------------------------//
std::string		ODEV_CreateFileTime			(void);
std::string		ODEV_CreateLOGDIR			(void);
std::string		ODEV_GetLOGDIR				(void);
std::string		ODEV_CreateNewLOGFileName	(void);
std::string		ODEV_CreateNewLOGFileName	(const std::string &tIP,int32 tPort);
//------------------------------------------------------------------------------------------//
void	ODEV_CreateEmptyRTFFile	(const std::string &fName);
void	ODEV_AddToTXTFile		(const std::string &fName,const std::string &content);
void	ODEV_AddToRTFFile		(const std::string &fName,const std::string &content);
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif