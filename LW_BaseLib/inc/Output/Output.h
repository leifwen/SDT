//
//  Output.h
//  SDT
//
//  Created by Leif Wen on 30/01/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

//------------------------------------------------------------------------------------------//
#include "ColorRecord.h"
#include "SYS_Thread.h"
#include "DS_Bitmap.h"
//------------------------------------------------------------------------------------------//
#if defined ColorRecord_h && defined DS_Bitmap_h && defined SYS_Thread_h
//------------------------------------------------------------------------------------------//
#ifndef Output_h
#define Output_h
#ifdef Output_h
//------------------------------------------------------------------------------------------//
class OUTPUT_NODE;
//------------------------------------------------------------------------------------------//
class OUTPUT_CACHE : public CRDC{
	public:
		enum{OC_G1AMASK = 0x0FFF
			,OC_G2AMASK = 0x0FFF
			,OC_G3AMASK = 0x0FF8
			,OC_DEVIDMASK = 0x0007
			,OC_G3D0 = 0x0000,OC_G3D1,OC_G3D2,OC_G3D3,OC_G3D4,OC_G3D5,OC_G3D6,OC_G3Broadcast};
	public:
				 OUTPUT_CACHE(uint32 size);
		virtual ~OUTPUT_CACHE(void);
	private:
		DS_BITMAP		g2AddrM;
		DS_BITMAP		g3AddrM;
		DS_SpinLock		cgMapLock;
	protected:
		TNFP			cgONList;
		COLORENUM		cgLastCol;
		uint32			cgLastCtrl;
	public:
						void			Register			(OUTPUT_NODE* oNode,uint32 group);
				virtual	void			Unregister			(OUTPUT_NODE* oNode);
				virtual	void			UnregisterAll		(void);
	private:
		SYS_Thread<OUTPUT_CACHE>		outCacheThread;
				virtual	bool32			OutCacheThreadFunc	(void* p);
	
						void			RecycleAddress		(uint32 addr);
						uint32			AssignAddress		(uint32 group);
						uint32			AssignMask			(uint32 group)const;
	protected:
				virtual	IOSE			DoTransform			(IOS* _ios,const UVOut& _out,const uint8* data,const uint64& length);
	public:
						void			Delivery			(void);
		inline			void			Start				(void);
		inline			void			Stop				(void);
};
//------------------------------------------------------------------------------------------//
class OUTPUT_NODE : public CRDN{
	protected:
		enum	{RFLAG_C = 1, RFLAG_S = CRDN::RFLAG_S + CRDN::RFLAG_C};
		enum	{ON_blDestoryByCache = RFLAG_CREATE(0)};
	public:
		enum COLType	{COLType_RAW = 0,COLType_TXT,COLType_COL};
	public:
				 OUTPUT_NODE(COLType colType = COLType_RAW,OUTPUT_CACHE* cache = nullptr,uint32 group = COLRECORD::CRD_G1);
		virtual ~OUTPUT_NODE(void){;};
	protected:
		COLType			cgColType;
		COLORENUM		cgLastCOL;
		uint32			cgLastCR;
	public:
						void			Init					(COLType colType = COLType_RAW,uint32 group = 0);
		inline			void			SetDestoryByCache		(void);
		inline			bool32			IsDestoryByCache		(void)const;
		inline			void			SetCOlType				(COLType colType);
	
				virtual void			RegisterToCache			(OUTPUT_CACHE* cache,uint32 group);
				virtual void			UnregisterToCache		(void);
				virtual	void			UnregisterChild			(OUTPUT_NODE* son);
				virtual	void			RemoveSelf				(void);
	protected:
				static	void			ShareAddressKeepDevID	(OUTPUT_NODE* node1,const OUTPUT_NODE& node2);
	public:
						void			UpdataLastStatus		(COLORENUM col,const uint8* data);
				virtual	bool32			CheckPrint				(uint32 ctrl)const;
		inline	virtual	void			Print					(uint32 ctrl,COLORENUM col,const uint8* data,uint32 num);
	public:
		inline	virtual void 			DoCleanLastLine			(void);
		static	inline	void 			CleanLastLine			(OUTPUT_NODE* node);
		template<typename T,typename... Args>static void PrintStr					(OUTPUT_NODE* node,const T&			first,	const Args&... args);
		template<typename T,typename... Args>static void PrintStrNL					(OUTPUT_NODE* node,const T&			first,	const Args&... args);
		template<typename T,typename... Args>static void PrintALine					(OUTPUT_NODE* node,const T&			first,	const Args&... args);
		template<typename T,typename... Args>static void PrintALineDot				(OUTPUT_NODE* node,const T&			first,	const Args&... args);
		template<			typename... Args>static void PrintALineDot				(OUTPUT_NODE* node,const _ColData&	first,	const Args&... args);
		template<			typename... Args>static void PrintALineDot				(OUTPUT_NODE* node,const COLORENUM&	col,	const Args&... args);
		template<			typename... Args>static void PrintWithTime				(OUTPUT_NODE* node,							const Args&... args);
		template<			typename... Args>static void PrintWithTime_noNL			(OUTPUT_NODE* node,							const Args&... args);
		template<			typename... Args>static void PrintWithDividingLine		(OUTPUT_NODE* node,							const Args&... args);
		template<			typename... Args>static void PrintWithDividingLineDot	(OUTPUT_NODE* node,							const Args&... args);
		template<			typename... Args>static void PrintWithDividingLineDot	(OUTPUT_NODE* node,const _ColData&	first,	const Args&... args);
		template<			typename... Args>static void PrintWithDividingLineDot	(OUTPUT_NODE* node,const COLORENUM&	col,	const Args&... args);
		template<			typename... Args>static void PrintMessageDot			(OUTPUT_NODE* node,							const Args&... args);
		template<			typename... Args>static void PrintMessageDot			(OUTPUT_NODE* node,const _ColData&	first,	const Args&... args);
		template<			typename... Args>static void PrintMessageDot			(OUTPUT_NODE* node,const COLORENUM&	col,	const Args&... args);
		template<typename T,typename... Args>static void PrintWarningMessageDot		(OUTPUT_NODE* node,const T&			first,	const Args&... args);
		template<			typename... Args>static void PrintWarningMessageDot		(OUTPUT_NODE* node,const _ColData&	first,	const Args&... args);
		template<			typename... Args>static void PrintWarningMessageDot		(OUTPUT_NODE* node,const COLORENUM&	col,	const Args&... args);
};
//------------------------------------------------------------------------------------------//
class VG3D_POOL : public OUTPUT_NODE{
	public:
			 	 VG3D_POOL(OUTPUT_CACHE* cache = nullptr);
		virtual ~VG3D_POOL(void);
	private:
		DS_BITMAP		g3AddrM;
	public:
				virtual void			Print				(uint32 ctrl,COLORENUM col,const uint8* data,uint32 num);
				virtual void			RegisterToCache		(OUTPUT_CACHE* cache,uint32 group = COLRECORD::CRD_G3);
				virtual void			UnregisterToCache	(void);
						void			UnregisterAll		(void);
						OUTPUT_NODE*	AddG3D				(OUTPUT_NODE* oG3D,G_LOCK blLock = G_LOCK_ON);
	protected:
						uint32			AssignDevID			(void);
};
//------------------------------------------------------------------------------------------//
#include "Output.hpp"
#endif /* Output_h */
#endif /* Output_h */
#endif /* ColorRecord_h */

