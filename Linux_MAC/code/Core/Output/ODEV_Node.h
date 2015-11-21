/*
 * Copyright (c) 2012-2014 
 * All rights reserved.
 *
 * File Name: ODEV_Node.h
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2013.7.5
 *			: 2014.1.13 remove support BCB2012
 *			: 2014.1.14 remove support Txt output
*/
#ifndef ODEV_NodeH
#define ODEV_NodeH
//------------------------------------------------------------------------------------------//
#include "ColorRecord.h"
//------------------------------------------------------------------------------------------//
class ODEV_NODE : public COLSTRING{
	public:
		enum{RFLAG_C = 0, RFLAG_S = COLSTRING::RFLAG_S + COLSTRING::RFLAG_C};
	public:
				 ODEV_NODE(void) : COLSTRING()	{;};
		virtual ~ODEV_NODE(void)				{UnInit();};
	public:
		virtual int32	Print		(G_LOCK_VAILD blLock = G_LOCK_ON){return 0;};
		virtual void	UnInit		(G_LOCK_VAILD blLock = G_LOCK_ON){;};
	private:
		static	std::string		MakeByteNum			(uint32 ByteNum, uint64 tTotal);
		static	std::string		MakeTimeNow			(void);
		static	std::string		MakeDividingLine	(void);
		static	std::string		MakeTitle			(const std::string &strTitle, uint32 ByteNum, uint64 tTotal);
		
		static	std::string		MakeTxTitle			(uint32 ByteNum, uint64 tTotal) {return(MakeTitle(" Send     ", ByteNum, tTotal));};
		static	std::string		MakeRxTitle			(uint32 ByteNum, uint64 tTotal) {return(MakeTitle(" Received ", ByteNum, tTotal));};
		static	std::string		MakeEchoTitle		(uint32 ByteNum, uint64 tTotal) {return(MakeTitle(" Echo     ", ByteNum, tTotal));};

		static	std::string		MakeTxTitle			(const std::string &strM, const std::string &strS, uint32 ByteNum, uint64 tTotal);
		static	std::string		MakeRxTitle			(const std::string &strM, const std::string &strS, uint32 ByteNum, uint64 tTotal);
		static	std::string		MakeEchoTitle		(const std::string &strM, uint32 ByteNum, uint64 tTotal);
	public:
		void WriteChar			(uint8 charData,const std::string &rColor,COLEnforcePrint blEP = COL_EP_NO,G_LOCK_VAILD blLock = G_LOCK_ON);

		void WriteDividingLine(const std::string &rColor,COLEnforcePrint blEP = COL_EP_NO,G_LOCK_VAILD blLock = G_LOCK_ON)
									{WriteToStrN(MakeDividingLine(),rColor,blEP,blLock);};
		void WriteTxTitle	(uint32 ByteNum, uint64 tTotal, const std::string &rColor, COLEnforcePrint blEP = COL_EP_NO,G_LOCK_VAILD blLock = G_LOCK_ON)
									{WriteToStrN(MakeTxTitle(ByteNum, tTotal), rColor, blEP,blLock);};
		void WriteRxTitle	(uint32 ByteNum, uint64 tTotal, const std::string &rColor, COLEnforcePrint blEP = COL_EP_NO,G_LOCK_VAILD blLock = G_LOCK_ON)
									{WriteToStrN(MakeRxTitle(ByteNum, tTotal) + "\r\n", rColor, blEP,blLock);};
		void WriteEchoTitle	(uint32 ByteNum, uint64 tTotal, const std::string &rColor, COLEnforcePrint blEP = COL_EP_NO,G_LOCK_VAILD blLock = G_LOCK_ON)
									{WriteToStrN(MakeEchoTitle(ByteNum, tTotal) + "\r\n", rColor, blEP,blLock);};
		void WriteTxTitle	(const std::string &strM,const std::string &strS,uint32 ByteNum,uint64 tTotal
							 ,const std::string &rColor,COLEnforcePrint blEP = COL_EP_NO,G_LOCK_VAILD blLock = G_LOCK_ON)
									{WriteToStrN(MakeTxTitle(strM, strS, ByteNum, tTotal) + "\r\n", rColor, blEP,blLock);};
		void WriteRxTitle	(const std::string &strM,const std::string &strS,uint32 ByteNum,uint64 tTotal
							 ,const std::string &rColor,COLEnforcePrint blEP = COL_EP_NO,G_LOCK_VAILD blLock = G_LOCK_ON)
									{WriteToStrN(MakeRxTitle(strM, strS, ByteNum, tTotal) + "\r\n", rColor, blEP,blLock);};
		void WriteEchoTitle	(const std::string &strM,uint32 ByteNum,uint64 tTotal
							 ,const std::string &rColor,COLEnforcePrint blEP = COL_EP_NO,G_LOCK_VAILD blLock = G_LOCK_ON)
									{WriteToStrN(MakeEchoTitle(strM, ByteNum, tTotal) + "\r\n", rColor, blEP,blLock);};
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif