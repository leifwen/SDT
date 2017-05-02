/*
 * Copyright (c) 2012-2014 
 * All rights reserved.
 *
 * File Name: OUTPUT.h
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2013.7.5
 *			: 2014.1.13 remove support BCB2012
 *			: 2014.1.14 remove support Txt output
*/

//------------------------------------------------------------------------------------------//
#include "ColorRecord.h"
//------------------------------------------------------------------------------------------//
#ifdef ColorRecordH
#ifndef OUTPUTH
#define OUTPUTH
#ifdef OUTPUTH
//------------------------------------------------------------------------------------------//
#include "SYS_Thread.h"
//------------------------------------------------------------------------------------------//
class OUTPUT_NODE;
//------------------------------------------------------------------------------------------//
class OUTPUT_CACHE : public COLRECORD_CACHE{
	public:
		enum{OC_G1AMASK = 0x0FFF,OC_G2AMASK = 0x0FFF,OC_G3AMASK = 0x0FF8
			,OC_G3D0 = 0x0000,OC_G3D1,OC_G3D2,OC_G3D3,OC_G3D4,OC_G3D5,OC_G3D6,OC_G3Broadcast};
	public:
				 OUTPUT_CACHE(void)			: COLRECORD_CACHE()	{;};
				 OUTPUT_CACHE(uint32 tSize) : COLRECORD_CACHE()	{Init(tSize);SetSelfName("OUTPUT_CACHE");};
		virtual ~OUTPUT_CACHE(void){;};
	public:
		void	Init	(uint32 tSize);
	private:
		uint64	g2AddrM[(1 << 12)/64];
		uint64	g3AddrM[(1 << 9)/64];
	private:
		void	RecycleAddress	(uint32 address);
		uint32	AssignAddress	(uint32 group);
		uint32	AssignMask		(uint32 group)const;
	protected:
		TREE_NODE		cgSubNodeList;
	public:
		TREE_NODE		&AddToCache	(OUTPUT_NODE *oNode){cgSubNodeList.AddNode((TREE_NODE*)oNode);return(cgSubNodeList);};
	private:
		SYS_Thread<OUTPUT_CACHE>		outThread;
	private:
		virtual	int32	OutThreadFun	(void *p);
	public:
		inline	void	Start	(void){outThread.ThreadRun();};
		inline	void	Stop	(void){outThread.ThreadStop();};
				void	Delivery(void);
	public:
				void	Register		(OUTPUT_NODE *oNode,uint32 group);
		virtual	void	Unregister		(OUTPUT_NODE *oNode);
};
//------------------------------------------------------------------------------------------//
class OUTPUT_NODE : public COLRECORD_NODE{
	public:
		enum COLType	{COLType_RAW = 0,COLType_TXT,COLType_COL};
	public:
				 OUTPUT_NODE(COLType tCOLType = COLType_RAW,OUTPUT_CACHE *cache = nullptr,uint32 group = COLRECORD::CRD_G1) : COLRECORD_NODE(){
					 Init(tCOLType,cache,group);
					 RegisterToCache(cache,group);
					 SetSelfName("OUTPUT_NODE");
				 };
		virtual ~OUTPUT_NODE(void){;};
	protected:
		COLType			cgColType;
		uint32			cgLastCOL;
		uint32			cgLastCR;
	public:
				void	Init					(COLType tCOLType = COLType_RAW,OUTPUT_CACHE *cache = nullptr,uint32 group = 0);
		inline	void	SetCOlType				(COLType tCOLType){cgColType = tCOLType;};
	protected:
		static	void	ShareAddressKeepDevID	(OUTPUT_NODE *node1,const OUTPUT_NODE &node2);
	public:
		virtual	void	Unregister			(OUTPUT_NODE *son){;};
		virtual	void	RemoveSelf			(void);
		virtual void	RegisterToCache		(OUTPUT_CACHE *tOutputCache,uint32 group);
		virtual void	UnregisterFromCache	(void);
	protected:
		inline	OUTPUT_NODE	*GetFatherNode	(void){return(static_cast<OUTPUT_NODE*>(GetcgUp(this)));};
	public:
				void	UpdataLastStatus(const uint8 *data,uint32 tCol);
		virtual	uint32	CheckPrint		(uint32 tCtrl)const;
		virtual void	Print			(uint32 col,uint32 ctrl,const uint8 *data,uint32 num){;};
	public:
		inline OUTPUT_NODE& operator << (OUTPUT_NODE&(*fun)(OUTPUT_NODE&)){return((*fun)(*this));};
	public:
		inline	static	OUTPUT_NODE	&Print	(OUTPUT_NODE *node){return(*node << Start);};
		inline	static	OUTPUT_NODE	&PrintNL(OUTPUT_NODE *node){return(*node << Start << NL);};
		static	void PrintStr				(OUTPUT_NODE *node ,const COLSTR &colStr1,const COLSTR &colStr2 = "",const COLSTR &colStr3 = "",const COLSTR &colStr4 = ""
											 ,const COLSTR &colStr5 = "",const COLSTR &colStr6 = "",const COLSTR &colStr7 = "",const COLSTR &colStr8 = "");
		static	void PrintStrNL				(OUTPUT_NODE *node ,const COLSTR &colStr1,const COLSTR &colStr2 = "",const COLSTR &colStr3 = "",const COLSTR &colStr4 = ""
											,const COLSTR &colStr5 = "",const COLSTR &colStr6 = "",const COLSTR &colStr7 = "",const COLSTR &colStr8 = "");
		static	void PrintWithTime			(OUTPUT_NODE *node ,const COLSTR &colStr1,const COLSTR &colStr2 = "",const COLSTR &colStr3 = "",const COLSTR &colStr4 = ""
											 ,const COLSTR &colStr5 = "",const COLSTR &colStr6 = "",const COLSTR &colStr7 = "",const COLSTR &colStr8 = "");
		static	void PrintWithDividingLine	(OUTPUT_NODE *node ,const COLSTR &colStr1,const COLSTR &colStr2 = "",const COLSTR &colStr3 = "",const COLSTR &colStr4 = ""
											 ,const COLSTR &colStr5 = "",const COLSTR &colStr6 = "",const COLSTR &colStr7 = "",const COLSTR &colStr8 = "");
		static	void PrintMessage			(OUTPUT_NODE *node ,const COLSTR &colStr1,const COLSTR &colStr2 = "",const COLSTR &colStr3 = "",const COLSTR &colStr4 = ""
											 ,const COLSTR &colStr5 = "",const COLSTR &colStr6 = "",const COLSTR &colStr7 = "",const COLSTR &colStr8 = "");
		static	void PrintNormalMessage		(OUTPUT_NODE *node,const STDSTR &strData1,const STDSTR &strData2 = "",const STDSTR &strData3 = "",const STDSTR &strData4 = ""
											 ,const STDSTR &strData5 = "",const STDSTR &strData6 = "",const STDSTR &strData7 = "",const STDSTR &strData8 = "");
		static	void PrintWarningMessage	(OUTPUT_NODE *node,const STDSTR &strData1C1,const STDSTR &strData2C2 = "",const STDSTR &strData3C2 = "",const STDSTR &strData4C2 = ""
											 ,const STDSTR &strData5C2 = "",const STDSTR &strData6C2 = "",const STDSTR &strData7C2 = "",const STDSTR &strData8C2 = "");
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
class VG3D_POOL : public OUTPUT_NODE{
	public:
				 VG3D_POOL(OUTPUT_CACHE *tOutputCache = nullptr) : OUTPUT_NODE(COLType_RAW){CreateTrash(this);SetSelfName("VG3D_POOL");RegisterToCache(tOutputCache);};
		virtual ~VG3D_POOL(void){BeforeDestory();DestroyAll();};
	private:
		uint32	NextDevID;
	public:
		virtual void	RegisterToCache		(OUTPUT_CACHE *tOutputCache,uint32 group = COLRECORD::CRD_G3);
		virtual void	UnregisterFromCache	(void);
				void	AddG3D				(OUTPUT_NODE *oG3D,uint32 devID = 0,G_LOCK blLock = G_LOCK_ON);
		virtual	void	UnregisterAll		(void);
		inline	void	BeforeDestory		(void){UnregisterFromCache(); UnregisterAll();};
	public:
		virtual	uint32	CheckPrint			(uint32 tCtrl)const;
		virtual void	Print				(uint32 col,uint32 ctrl,const uint8 *data,uint32 num);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif
#endif
#endif
