/*
 * Copyright (c) 2012-2014 
 * All rights reserved.
 *
 * File Name: ODEV_SDOUT.h
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2013.7.5
 *			: 2014.1.13 remove support BCB2012
 *			: 2014.1.14 remove support Txt output
*/
#ifndef ODEV_SDOUTH
#define ODEV_SDOUTH
//------------------------------------------------------------------------------------------//
#include "ODEV_Node.h"
//------------------------------------------------------------------------------------------//
#ifdef CommonDefH_VC
	class	CChildFrame;
#endif
//------------------------------------------------------------------------------------------//
class ODEV_NODE_SDOUT : public ODEV_NODE{
	public:
		enum{RFLAG_C = 0, RFLAG_S = ODEV_NODE::RFLAG_S + ODEV_NODE::RFLAG_C};
	public:
				 ODEV_NODE_SDOUT(void) : ODEV_NODE(){cgRichEdit = nullptr;cgCFrm = nullptr; };
				 ODEV_NODE_SDOUT(const void *tRichEdit,const void *tCFrm,COLType tCOLType = COLType_COL) : ODEV_NODE(){Init(tRichEdit,tCFrm,tCOLType);};
		virtual ~ODEV_NODE_SDOUT(void);
	private:
		SYS_Lock		cgPrintLock;
	public:
		inline	void	Spin_Print_Lock		(G_LOCK_VAILD blVaild = G_LOCK_ON){cgPrintLock.Lock(blVaild);};
		inline	void	Spin_Print_Unlock	(G_LOCK_VAILD blVaild = G_LOCK_ON){cgPrintLock.Unlock(blVaild);};
		inline	int32	Spin_Print_Try		(G_LOCK_VAILD blVaild = G_LOCK_ON){return(cgPrintLock.TryLock(blVaild));};
	public:
		#ifdef CommonDefH_VC
		virtual	void	Clean	(G_LOCK_VAILD blLock = G_LOCK_ON);
		#endif
		virtual int32	Print	(G_LOCK_VAILD blLock = G_LOCK_ON);
		virtual void	UnInit	(G_LOCK_VAILD blLock = G_LOCK_ON){Spin_InUse_set(blLock);cgRichEdit = nullptr;cgCFrm = nullptr;Spin_InUse_clr(blLock);};
	private:
		int32	PrintTXT(G_LOCK_VAILD blLock);
		int32	PrintCOL(G_LOCK_VAILD blLock);
		#ifdef CommonDefH_Unix
	    void	DoPrintf(std::string * strOutput);
		#endif
	public:
		#ifdef CommonDefH_VC
			CRichEditView	*cgRichEdit;
			CChildFrame		*cgCFrm;
			CHARFORMAT2		cgCF;
		#endif
	private:
		#ifdef CommonDefH_Unix
			void	*cgRichEdit;
			void	*cgCFrm;
		#endif
	public:
		void	Init(const void *tRichEdit,const void *tCFrm,COLType tCOLType = COLType_COL){
			Spin_InUse_set();
			ChageType(tCOLType,G_LOCK_OFF);
			#ifdef CommonDefH_VC
				cgRichEdit = (CRichEditView*)tRichEdit;
				cgCFrm = (CChildFrame*)tCFrm;
				SetExPar(&cgCF);
			#else
				cgRichEdit = (void*)tRichEdit;
				cgCFrm = (void*)tCFrm;
			#endif
			Spin_InUse_clr();
		};
	public:
		#ifdef CommonDefH_VC
			void	SetExPar			(CHARFORMAT2 *tcf);
			void	SetCurFromEnd		(int32 offset);
			void	DelCharFromEnd		(int32 offset,int32 length);
			void	InsterCharFromEnd	(int32 offset,const std::string &strIn);
		#endif
		void	SetCurLeft		(int32 num);
		void	SetCurRight		(int32 num);
		void	OutputStr		(const std::string &outStr,const std::string rColor,G_LOCK_VAILD blLock = G_LOCK_ON)
									{WriteToStr (outStr,rColor,COL_EP_YES,blLock);Print(blLock);};
	    void	OutputStrN		(const std::string &outStr,const std::string rColor,G_LOCK_VAILD blLock = G_LOCK_ON)
									{WriteToStrN(outStr,rColor,COL_EP_YES,blLock);Print(blLock);};
};
//------------------------------------------------------------------------------------------//
#endif