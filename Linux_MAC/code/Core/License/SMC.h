/*
 * Copyright (c) 2012-2014 
 * All rights reserved.
 *
 * File Name: SMC.h
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2014.03.26
*/
#ifndef SMCH
#define SMCH
//------------------------------------------------------------------------------------------//
#include "Comm_PA.h"
//------------------------------------------------------------------------------------------//
#ifndef CommonDefH_EnableSMC
	#ifndef SMC_NOH
		#define SMC_NOH
		#define	SMC_EncryptI(No);
		#define	SMC_EncryptS(No);
		#define	SMC_EncryptE(No);
	#endif
#else
#ifdef USE_OPENSSL
#ifndef SMC_YESH
#define SMC_YESH
#ifdef CommonDefH_VC
#define	SMC_Conn(x,y)	x##y
#define SMCEncrypt_Begin \
{\
	__asm _emit 'S' \
	__asm _emit 'M' \
	__asm _emit 'C' \
	__asm _emit 'E' \
	__asm _emit 'n' \
	__asm _emit 'c' \
	__asm _emit 'r' \
	__asm _emit 'y' \
	__asm _emit 'p' \
	__asm _emit 't' \
	__asm _emit '_' \
	__asm _emit 'B' \
	__asm _emit 'e' \
	__asm _emit 'g' \
	__asm _emit 'i' \
	__asm _emit 'n' \
}
#define SMCEncrypt_End \
{\
	__asm _emit 'S' \
	__asm _emit 'M' \
	__asm _emit 'C' \
	__asm _emit 'E' \
	__asm _emit 'n' \
	__asm _emit 'c' \
	__asm _emit 'r' \
	__asm _emit 'y' \
	__asm _emit 'p' \
	__asm _emit 't' \
	__asm _emit '_' \
	__asm _emit 'E' \
	__asm _emit 'n' \
	__asm _emit 'd' \
}
#define		SMC_EncryptI(No) \
	DWORD	SMC_Conn(arS,No),SMC_Conn(arE,No); \
	SMC_BLOCK	SMC_Conn(smcblock,No); \
	{\
		__asm mov SMC_Conn(arS,No),offset SMC_Conn(SMC_beg,No) \
		__asm mov SMC_Conn(arE,No),offset SMC_Conn(SMC_end,No) \
	}
#define		SMC_EncryptS(No) \
	SMC_Conn(smcblock,No).Init((uint8*)SMC_Conn(arS,No),SMC_Conn(arE,No) - SMC_Conn(arS,No));\
	if (SMC_Conn(smcblock,No).Decrypt() != 1) \
		goto SMC_Conn(SMC_quit,No); \
	SMC_Conn(SMC_beg,No):; \
	SMCEncrypt_Begin\
	{ __asm _emit 0x90 }
#define		SMC_EncryptE(No) \
	SMCEncrypt_End \
	SMC_Conn(SMC_end,No):; \
	{ __asm _emit 0x90 } \
	SMC_Conn(smcblock,No).Resume();\
	SMC_Conn(SMC_quit,No):;
//------------------------------------------------------------------------------------------//
class SMC_BLOCK{
	public:
				 SMC_BLOCK(void){blInit = 0;cgMemOffset = nullptr;cgCodeSize = 0;cgOldMemCode = "";};
		virtual ~SMC_BLOCK(void){;};
	private:
		int32		blInit;
		uint8		*cgMemOffset;
		uint32		cgCodeSize;
		STDSTR		cgOldMemCode;
	public:
		int32		Init(uint8 *cOffset,uint32 cCodeSize);
		int32		Decrypt(void);
		int32		Resume(void);
};
//------------------------------------------------------------------------------------------//
#endif//CommonDefH_VC
//------------------------------------------------------------------------------------------//
int32	SMC_DecryptCYCodeBlock	(STDSTR *retStr,const FIFO8 &cyCodeStream,uint32 codeNo);
//------------------------------------------------------------------------------------------//
extern FIFO8	g_SMCCyCodeStream;
//------------------------------------------------------------------------------------------//
#endif//SMCYESH
#endif//CommonDefH_EnableSMC
#endif//USE_OPENSSL
//------------------------------------------------------------------------------------------//
#ifdef USE_OPENSSL
int32	SMC_SourceCodeAnalysis	(STDSTR *retStr,const STDSTR &sFN);
void	SMC_EncryptCYCodeBlock	(uint8 *data,uint32 num);
int32	SMC_CreateCYCodeStream	(STDSTR *retStr,const STDSTR &sFN_SDT,STDSTR strAnalysisFIFO);
int32	SMC_SourceCodeReplace	(const STDSTR &sFN_SDT,STDSTR strAnalysisFIFO);
#endif
//------------------------------------------------------------------------------------------//
#endif//SMCH

