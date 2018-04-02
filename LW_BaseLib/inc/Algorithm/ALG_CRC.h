//
//  ALG_CRC.h
//  SDT
//
//  Created by Leif Wen on 23/01/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#ifndef ALG_CRC_h
#define ALG_CRC_h
#ifdef ALG_CRC_h
//------------------------------------------------------------------------------------------//
#include "BasicDefine.h"
#include "DS_Transform.h"
//------------------------------------------------------------------------------------------//
namespace CRC8ID{
	enum {none = 0,eCRC8,eAUTOSAR,eBLUETOOTH,eCDMA2000,eDARC,eDVB_S2
		,eEBU,eAES,eGSM_A,eGSM_B,eI_CODE,eITU,eLTE,eMAXIM
		,eOPENSAFETY,eROHC,eSAE_J1850,eWCDMA,eDefault = eCRC8};
}
namespace CRC16ID{
	enum {none = 0,eARC,eCRC_16,eIBM,eLHA,eAUG_CCITT,eSPI_FUJITSU,eBUYPASS
		,eVERIFONE,eUMTS,eCCITT_FALSE,eCDMA2000,eCMS,eDDS_110,eDECT_R
		,eDECT_X,eDNP,eEN_13757,eGENIBUS,eEPC,eI_CODE,eDARC,eGSM,eLJ1200
		,eMAXIM,eMCRF4XX,eOPENSAFETY_A,eOPENSAFETY_B,ePROFIBUS,eIEC_61158_2
		,eRIELLO,eT10_DIF,eTELEDISK,eTMS37157,eUSB,eCRC_A,eCCITT,eKERMIT
		,eMODBUS,eX_25,eISO_HDLC,eIBM_SDLC,eCRC_B,eXMODEM,eZMODEM,eACORN,eLTE,eDefault = eXMODEM};
}
namespace CRC32ID{
	enum {none = 0,eADCCP,ePKZIP,eCRC32,eAUTOSAR,eBZIP2,eAAL5,eDECT_B
		,eB_CRC32,eCRC32C,eISCSI,eCASTAGNOLI,eINTERLAKEN
		,eCRC32D,eMPEG2,ePOSIX,eCKSUM,eCRC32Q,eJAMCRC,eXFER,eDefault = eMPEG2};
}
//------------------------------------------------------------------------------------------//
template <typename CRC_BIT> struct ALG_CRC_CTX{
	uint32	cfg;
	uint32	result;
	
	uint32	ref;
	uint32	width;
	uint32	highbit;

	CRC_BIT	crcTable[256];
	CRC_BIT	polynomial;
	CRC_BIT	initValue;
	CRC_BIT	xorValue;
	CRC_BIT	remainder;
};
//------------------------------------------------------------------------------------------//
typedef ALG_CRC_CTX<uint8>	ALG_CRC8_CTX;
typedef ALG_CRC_CTX<uint16> ALG_CRC16_CTX;
typedef ALG_CRC_CTX<uint32> ALG_CRC32_CTX;
//------------------------------------------------------------------------------------------//
void ALG_CRC8_Init	(ALG_CRC8_CTX*  ctx,uint32 cfg);
void ALG_CRC16_Init	(ALG_CRC16_CTX* ctx,uint32 cfg);
void ALG_CRC32_Init	(ALG_CRC32_CTX* ctx,uint32 cfg);

static inline void ALG_CRC_Init (ALG_CRC8_CTX*  ctx,uint32 cfg);
static inline void ALG_CRC_Init (ALG_CRC16_CTX* ctx,uint32 cfg);
static inline void ALG_CRC_Init (ALG_CRC32_CTX* ctx,uint32 cfg);

template <typename CRC_BIT> static inline void		ALG_CRC_Update	(ALG_CRC_CTX<CRC_BIT>* ctx,const uint8* data,const uint64& length);
template <typename CRC_BIT> static inline CRC_BIT	ALG_CRC_Final	(ALG_CRC_CTX<CRC_BIT>* ctx);
//------------------------------------------------------------------------------------------//
#ifdef DS_Transform_h
template <typename ALGCTX>
class ALG_CRC_T : public DSTF{
	public:
				 ALG_CRC_T(void);
		virtual ~ALG_CRC_T(void){;};
	protected:
		ALGCTX	cgCTX;
	public:
		inline	virtual	ALG_CRC_T&	InitCFG		(uint32 cfg = -1,const void* p = nullptr);
	protected:
		inline	virtual	ALG_CRC_T&	InitSize	(uint32 size);
		inline	virtual	ALG_CRC_T&	DoTransform	(IOSTATUS* _ios,const UVOut& _out,const uint8* data,const uint64& length);
		inline	virtual	ALG_CRC_T&	DoFinal		(IOSTATUS* _ios,const UVOut& _out);
	public:
		inline	uint32	Calc					(IOSTATUS* _ios,const UVOut& _out,const UVIn& _in);
		inline	uint32	GetCRCReasult			(void);
		inline	uint32	GetCRCBit				(void);
};
//------------------------------------------------------------------------------------------//
typedef ALG_CRC_T<ALG_CRC8_CTX>	 ALG_CRC8;
typedef ALG_CRC_T<ALG_CRC16_CTX> ALG_CRC16;
typedef ALG_CRC_T<ALG_CRC32_CTX> ALG_CRC32;
#endif
//------------------------------------------------------------------------------------------//
#include "ALG_CRC.hpp"
#endif /* ALG_CRC_h */
#endif /* ALG_CRC_h */
