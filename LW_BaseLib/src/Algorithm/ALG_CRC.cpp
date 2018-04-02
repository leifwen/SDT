//
//  ALG_CRC.cpp
//  SDT
//
//  Created by Leif Wen on 23/01/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "stdafx.h"
#include "ALG_CRC.h"
//------------------------------------------------------------------------------------------//
#ifdef ALG_CRC_h
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
void ALG_CRC8_Init(ALG_CRC_CTX<uint8>* ctx,uint32 cfg){
	//http://reveng.sourceforge.net/crc-catalogue/all.htm
	ctx->cfg = cfg;
	switch (cfg){
		case CRC8ID::eCRC8:
			Init(ctx,0x07,0x00,0x00,0);
			break;
		case CRC8ID::eAUTOSAR:
			Init(ctx,0x2f,0xff,0xff,0);
			break;
		case CRC8ID::eBLUETOOTH:
			Init(ctx,0xa7,0x00,0x00,CRC_REFIN | CRC_REFOUT);
			break;
		case CRC8ID::eCDMA2000:
			Init(ctx,0x9b,0xff,0x00,0);
			break;
		case CRC8ID::eDARC:
			Init(ctx,0x39,0x00,0x00,CRC_REFIN | CRC_REFOUT);
			break;
		case CRC8ID::eDVB_S2:
			Init(ctx,0xd5,0x00,0x00,0);
			break;
		case CRC8ID::eEBU:
		case CRC8ID::eAES:
			Init(ctx,0x1d,0xff,0x00,CRC_REFIN | CRC_REFOUT);
			break;
		case CRC8ID::eGSM_A:
			Init(ctx,0x1d,0x00,0x00,0);
			break;
		case CRC8ID::eGSM_B:
			Init(ctx,0x49,0x00,0xff,0);
			break;
		case CRC8ID::eI_CODE:
			Init(ctx,0x1d,0xfd,0x00,0);
			break;
		case CRC8ID::eITU:
			Init(ctx,0x07,0x00,0x55,0);
			break;
		case CRC8ID::eLTE:
			Init(ctx,0x9b,0x00,0x00,0);
			break;
		case CRC8ID::eMAXIM:
			Init(ctx,0x31,0x00,0x00,CRC_REFIN | CRC_REFOUT);
			break;
		case CRC8ID::eOPENSAFETY:
			Init(ctx,0x2f,0x00,0x00,0);
			break;
		case CRC8ID::eROHC:
			Init(ctx,0x07,0xff,0x00,CRC_REFIN | CRC_REFOUT);
			break;
		case CRC8ID::eSAE_J1850:
			Init(ctx,0x1d,0xff,0xff,0);
			break;
		case CRC8ID::eWCDMA:
			Init(ctx,0x9b,0x00,0x00,CRC_REFIN | CRC_REFOUT);
			break;
		default:
			Init(ctx,0x07,0x00,0x00,0);
			break;
	}
}
//------------------------------------------------------------------------------------------//
void ALG_CRC16_Init(ALG_CRC_CTX<uint16>* ctx,uint32 cfg){
	ctx->cfg = cfg;
	switch (cfg){
		case CRC16ID::eARC:
		case CRC16ID::eCRC_16:
		case CRC16ID::eIBM:
		case CRC16ID::eLHA:
			Init(ctx,0x8005,0x0000,0x0000,CRC_REFIN | CRC_REFOUT);
			break;
		case CRC16ID::eAUG_CCITT:
		case CRC16ID::eSPI_FUJITSU:
			Init(ctx,0x1021,0x1d0f,0x0000,0);
			break;
		case CRC16ID::eBUYPASS:
		case CRC16ID::eVERIFONE:
		case CRC16ID::eUMTS:
			Init(ctx,0x8005,0x0000,0x0000,0);
			break;
		case CRC16ID::eCCITT_FALSE:
			Init(ctx,0x1021,0xFFFF,0x0000,0);
			break;
		case CRC16ID::eCDMA2000:
			Init(ctx,0xc867,0xffff,0x0000,0);
			break;
		case CRC16ID::eCMS:
			Init(ctx,0x8005,0xffff,0x0000,0);
			break;
		case CRC16ID::eDDS_110:
			Init(ctx,0x8005,0x800d,0x0000,0);
			break;
		case CRC16ID::eDECT_R:
			Init(ctx,0x0589,0x0000,0x0001,0);
			break;
		case CRC16ID::eDECT_X:
			Init(ctx,0x0589,0x0000,0x0000,0);
			break;
		case CRC16ID::eDNP:
			Init(ctx,0x3d65,0x0000,0xffff,CRC_REFIN | CRC_REFOUT);
			break;
		case CRC16ID::eEN_13757:
			Init(ctx,0x3d65,0x0000,0xffff,0);
			break;
		case CRC16ID::eGENIBUS:
		case CRC16ID::eEPC:
		case CRC16ID::eI_CODE:
		case CRC16ID::eDARC:
			Init(ctx,0x1021,0xffff,0xffff,0);
			break;
		case CRC16ID::eGSM:
			Init(ctx,0x1021,0x0000,0xffff,0);
			break;
		case CRC16ID::eLJ1200:
			Init(ctx,0x6f63,0x0000,0x0000,0);
			break;
		case CRC16ID::eMAXIM:
			Init(ctx,0x8005,0x0000,0xffff,CRC_REFIN | CRC_REFOUT);
			break;
		case CRC16ID::eMCRF4XX:
			Init(ctx,0x1021,0xFFFF,0x0000,CRC_REFIN | CRC_REFOUT);
			break;
		case CRC16ID::eOPENSAFETY_A:
			Init(ctx,0x5935,0x0000,0x0000,0);
			break;
		case CRC16ID::eOPENSAFETY_B:
			Init(ctx,0x755b,0x0000,0x0000,0);
			break;
		case CRC16ID::ePROFIBUS:
		case CRC16ID::eIEC_61158_2:
			Init(ctx,0x1dcf,0xffff,0xffff,0);
			break;
		case CRC16ID::eRIELLO:
			Init(ctx,0x1021,0xb2aa,0x0000,CRC_REFIN | CRC_REFOUT);
			break;
		case CRC16ID::eT10_DIF:
			Init(ctx,0x8bb7,0x0000,0x0000,0);
			break;
		case CRC16ID::eTELEDISK:
			Init(ctx,0xa097,0x0000,0x0000,0);
			break;
		case CRC16ID::eTMS37157:
			Init(ctx,0x1021,0x89ec,0x0000,CRC_REFIN | CRC_REFOUT);
			break;
		case CRC16ID::eUSB:
			Init(ctx,0x8005,0xffff,0xffff,CRC_REFIN | CRC_REFOUT);
			break;
		case CRC16ID::eCRC_A:
			Init(ctx,0x1021,0xc6c6,0x0000,CRC_REFIN | CRC_REFOUT);
			break;
		case CRC16ID::eCCITT:
		case CRC16ID::eKERMIT:
			Init(ctx,0x1021,0x0000,0x0000,CRC_REFIN | CRC_REFOUT);
			break;
		case CRC16ID::eMODBUS:
			Init(ctx,0x8005,0xffff,0x0000,CRC_REFIN | CRC_REFOUT);
			break;
		case CRC16ID::eX_25:
		case CRC16ID::eISO_HDLC:
		case CRC16ID::eIBM_SDLC:
		case CRC16ID::eCRC_B:
			Init(ctx,0x1021,0xffff,0xffff,CRC_REFIN | CRC_REFOUT);
			break;
		case CRC16ID::eXMODEM:
		case CRC16ID::eZMODEM:
		case CRC16ID::eACORN:
		case CRC16ID::eLTE:
		default:
			Init(ctx,0x1021,0x0000,0x0000,0);
			break;
	}
}
//------------------------------------------------------------------------------------------//
void ALG_CRC32_Init(ALG_CRC_CTX<uint32>* ctx,uint32 cfg){
	ctx->cfg = cfg;
	switch (ctx->cfg){
		case CRC32ID::eADCCP:
		case CRC32ID::ePKZIP:
		case CRC32ID::eCRC32:
			Init(ctx,0x04c11db7,0xffffffff,0xffffffff,CRC_REFIN | CRC_REFOUT);
			break;
		case CRC32ID::eAUTOSAR:
			Init(ctx,0xf4acfb13,0xffffffff,0xffffffff,CRC_REFIN | CRC_REFOUT);
			break;
		case CRC32ID::eBZIP2:
		case CRC32ID::eAAL5:
		case CRC32ID::eDECT_B:
		case CRC32ID::eB_CRC32:
			Init(ctx,0x04c11db7,0xffffffff,0xffffffff,0);
			break;
		case CRC32ID::eCRC32C:
		case CRC32ID::eISCSI:
		case CRC32ID::eCASTAGNOLI:
		case CRC32ID::eINTERLAKEN:
			Init(ctx,0x1edc6f41,0xffffffff,0xffffffff,CRC_REFIN | CRC_REFOUT);
			break;
		case CRC32ID::eCRC32D:
			Init(ctx,0xa833982b,0xffffffff,0xffffffff,CRC_REFIN | CRC_REFOUT);
			break;
		case CRC32ID::ePOSIX:
		case CRC32ID::eCKSUM:
			Init(ctx,0x04c11db7,0x00000000,0xffffffff,0);
			break;
		case CRC32ID::eCRC32Q:
			Init(ctx,0x814141ab,0x00000000,0x00000000,0);
			break;
		case CRC32ID::eJAMCRC:
			Init(ctx,0x04c11db7,0xffffffff,0x00000000,CRC_REFIN | CRC_REFOUT);
			break;
		case CRC32ID::eXFER:
			Init(ctx,0x000000af,0x00000000,0x00000000,0);
			break;
		case CRC32ID::eMPEG2:
		default:
			Init(ctx,0x04c11db7,0xffffffff,0x00000000,0);
			break;
	}
}
//------------------------------------------------------------------------------------------//
#endif
