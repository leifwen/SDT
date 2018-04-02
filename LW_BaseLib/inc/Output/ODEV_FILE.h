//
//  ODEV_FILE.h
//  SDT
//
//  Created by Leif Wen on 31/01/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "Output.h"
#ifdef Output_h
//------------------------------------------------------------------------------------------//
#ifndef ODEV_FILE_h
#define ODEV_FILE_h
#ifdef ODEV_FILE_h
#include "SYS_Time.h"
//------------------------------------------------------------------------------------------//
class ODEV_FILE : public OUTPUT_NODE{
	protected:
		enum	{RFLAG_C = 3, RFLAG_S = OUTPUT_NODE::RFLAG_S + OUTPUT_NODE::RFLAG_C};
		enum	{FREQUENCY = 1000,UNWRITESIZE = 1024 * 8,};
	public:
		enum	{ODEV_blRAW = RFLAG_CREATE(0),ODEV_blTXT = RFLAG_CREATE(1),ODEV_blRTF = RFLAG_CREATE(2),};
	public:
				 ODEV_FILE(OUTPUT_CACHE* cache = nullptr,uint32 group = 3);
		virtual ~ODEV_FILE(void){;};
	public:
						void	Init		(const STDSTR& filename,uint64 colType = ODEV_blRTF);
	public:
				virtual	bool32	CheckPrint	(uint32 ctrl)const;
				virtual	void	Print		(uint32 ctrl,COLORENUM col,const uint8* data,uint32 num);
						void	PrintRTF	(uint32 ctrl,COLORENUM col,const uint8* data,uint32 num);
						void	PrintTXT	(uint32 ctrl,COLORENUM col,const uint8* data,uint32 num);
						void	PrintRAW	(uint32 ctrl,COLORENUM col,const uint8* data,uint32 num);
	private:
		STDSTR			cgfileName;
		STDSTR			cgContentRAW;
		STDSTR			cgContentTXT;
		STDSTR			cgContentRTF;
		SYS_TIME_S		cgTimeS;
	public:
		inline 			void	SetFileName				(const STDSTR& filename);
		inline 	const	STDSTR&	GetFileName				(void)const;
	public:
		static			STDSTR	CreateFileTime			(void);
		static			STDSTR	CreateLOGDIR			(void);
		static			STDSTR	GetLOGDIR				(void);
		static			STDSTR	CreateNewLOGFileName	(void);
		static			STDSTR	CreateNewLOGFileName	(const STDSTR& tIP,int32 tPort);
		
		static			void	CreateEmptyRTFFile		(const STDSTR& fName);
		static			void	AddToTXTFile			(const STDSTR& fName,const STDSTR& content);
		static			void	AddToRTFFile			(const STDSTR& fName,const STDSTR& content);
};
//------------------------------------------------------------------------------------------//
#include "ODEV_FILE.hpp"
#endif /* ODEV_FILE_h */
#endif /* ODEV_FILE_h */
#endif /* ColorRecord_h */
