/*
 * Copyright (c) 2012-2014 
 * All rights reserved.
 *
 * File Name: ODEV_LIST.h
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2013.7.5
 *			: 2014.1.13 remove support BCB2012
 *			: 2014.1.14 remove support Txt output
 *			: 2015.1.06 change flag
*/
#ifndef ODEV_LISTH
#define ODEV_LISTH
//------------------------------------------------------------------------------------------//
#include "ODEV_Node.h"
//------------------------------------------------------------------------------------------//
#define ODEV_FLAG_EnView		B_FLAG64(0)
#define ODEV_FLAG_EnHEXViewMode	B_FLAG64(1)
#define ODEV_FLAG_EnRecMsg		B_FLAG64(2)
#define ODEV_FLAG_EnOSPMsgLine	B_FLAG64(3)
#define ODEV_FLAG_EnEscape		B_FLAG64(4)
#define ODEV_FLAG_EnMSReport	B_FLAG64(5)
//------------------------------------------------------------------------------------------//
class ODEV_NODE_FILE;
class ODEV_NODE_DBUF;
class ODEV_NODE_SDOUT;
class COMMU_DBUF;
//------------------------------------------------------------------------------------------//
class ODEV_LIST_POOL : public RTREE_NODE{
	public:
		enum{RFLAG_C = 0, RFLAG_S = RTREE_NODE::RFLAG_S + RTREE_NODE::RFLAG_C};
				 ODEV_LIST_POOL(void);
		virtual ~ODEV_LIST_POOL(void){DestroyAll();};
	public:
		int32	Print		(void);
		void	PrintSDOUT	(void);
		void	Clean		(void){RTREE_LChildRChain_T(ODEV_NODE,Clean());};
	
		void	WriteChar			(uint8 charData,const std::string &rColor,COLSTRING::COLEnforcePrint blEP = COLSTRING::COL_EP_NO)
										{RTREE_LChildRChain_T(ODEV_NODE,WriteChar(charData,rColor,blEP));};
		void	WriteStr			(const std::string &strContent,const std::string &rColor,COLSTRING::COLEnforcePrint blEP = COLSTRING::COL_EP_NO)
										{RTREE_LChildRChain_T(ODEV_NODE,WriteToStr(strContent,rColor,blEP));};
		void	WriteStrN			(const std::string &strContent,const std::string &rColor,COLSTRING::COLEnforcePrint blEP = COLSTRING::COL_EP_NO)
										{RTREE_LChildRChain_T(ODEV_NODE,WriteToStrN(strContent,rColor,blEP));};
		void	WriteDividingLine	(const std::string &rColor,COLSTRING::COLEnforcePrint blEP = COLSTRING::COL_EP_NO)
										{RTREE_LChildRChain_T(ODEV_NODE,WriteDividingLine(rColor,blEP));};
		void	WriteTxTitle		(uint32 ByteNum, uint64 tTotal, const std::string &rColor, COLSTRING::COLEnforcePrint blEP = COLSTRING::COL_EP_NO)
										{RTREE_LChildRChain_T(ODEV_NODE, WriteTxTitle(ByteNum, tTotal, rColor, blEP));};
		void	WriteRxTitle		(uint32 ByteNum, uint64 tTotal, const std::string &rColor, COLSTRING::COLEnforcePrint blEP = COLSTRING::COL_EP_NO)
										{RTREE_LChildRChain_T(ODEV_NODE, WriteRxTitle(ByteNum, tTotal, rColor, blEP));};
		void	WriteEchoTitle		(uint32 ByteNum, uint64 tTotal, const std::string &rColor, COLSTRING::COLEnforcePrint blEP = COLSTRING::COL_EP_NO)
										{RTREE_LChildRChain_T(ODEV_NODE, WriteEchoTitle(ByteNum, tTotal, rColor, blEP));};
		void	WriteTxTitle		(const std::string &strM,const std::string &strS,uint32 ByteNum,uint64 tTotal,const std::string &rColor
									 ,COLSTRING::COLEnforcePrint blEP = COLSTRING::COL_EP_NO)
										{RTREE_LChildRChain_T(ODEV_NODE, WriteTxTitle(strM, strS, ByteNum, tTotal, rColor, blEP));};
		void	WriteRxTitle		(const std::string &strM,const std::string &strS,uint32 ByteNum,uint64 tTotal,const std::string &rColor
									 ,COLSTRING::COLEnforcePrint blEP = COLSTRING::COL_EP_NO)
										{RTREE_LChildRChain_T(ODEV_NODE, WriteRxTitle(strM, strS, ByteNum, tTotal, rColor, blEP));};
		void	WriteEchoTitle		(const std::string &strM, uint32 ByteNum, uint64 tTotal, const std::string &rColor
									 , COLSTRING::COLEnforcePrint blEP = COLSTRING::COL_EP_NO)
										{RTREE_LChildRChain_T(ODEV_NODE, WriteEchoTitle(strM, ByteNum, tTotal, rColor, blEP));};
	private:
		ODEV_NODE_FILE		*CreateNewODevFile	(const std::string &tODEV,COLSTRING::COLType tCOLType = COLSTRING::COLType_COL);
		ODEV_NODE_SDOUT		*CreateNewODevSDOUT	(const void *tRichEdit,const void *tCFrm,COLSTRING::COLType tCOLType = COLSTRING::COLType_COL);
		//ODEV_NODE_DBUF		*CreateNewODevDBUF	(COMMU_DBUF *tODEV,COLSTRING::COLType tCOLType = COLSTRING::COLType_TXT);
	public:
		ODEV_NODE_SDOUT		*cODevSDOUT;
		ODEV_NODE_FILE		*cODevFileTXT;
		ODEV_NODE_FILE		*cODevFileRTF;
		ODEV_NODE_DBUF		*cODevDBUF;
	

		ODEV_NODE_FILE		*CreateODevFileTXT	(const std::string &tfileName);
		ODEV_NODE_FILE		*CreateODevFileRTF	(const std::string &tfileName);
		void				CreateODevFile		(const std::string &tfileName);
	
		ODEV_NODE_SDOUT		*CreateODevSDOUT	(const void *tRichEdit,const void *tCFrm,COLSTRING::COLType tCOLType = COLSTRING::COLType_COL);
		//ODEV_NODE_DBUF		*CreateODevDBUF		(COMMU_DBUF *tODEV);
};
//------------------------------------------------------------------------------------------//
class ODEV_LIST{
	public:
				 ODEV_LIST(void){
                    cgOutput = nullptr;
					B_ClrFLAG64(cgODevFlagU64, 0);// | ODEV_FLAG_EnView | ODEV_FLAG_EnOSPMsgLine | ODEV_FLAG_EnMSReport);
                 };
		virtual ~ODEV_LIST(void){;};
	private:
	public:
		uint64				cgODevFlagU64;
    	ODEV_LIST_POOL		*cgOutput;
};
//------------------------------------------------------------------------------------------//
#endif