//
//  ADS_SMC.hpp
//  SDT
//
//  Created by Leif Wen on 27/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

//------------------------------------------------------------------------------------------//
#include "BasicDefine.h"
//------------------------------------------------------------------------------------------//
#ifndef CommonDefH_EnableLicense
	#ifndef SMC_NOH
		#define SMC_NOH
		#define	SMC_EncryptI(No);
		#define	SMC_EncryptS(No);
		#define	SMC_EncryptE(No);
	#endif
#else
	#ifndef SMC_YESH
		#define SMC_YESH
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
		};
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
		};

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
		SMCEncrypt_Begin;\
		{ __asm _emit 0x90 };

	#define		SMC_EncryptE(No) \
		SMCEncrypt_End; \
		SMC_Conn(SMC_end,No):; \
		{ __asm _emit 0x90 }; \
		SMC_Conn(smcblock,No).Resume();\
		SMC_Conn(SMC_quit,No):;

	#endif
#endif //CommonDefH_EnableLicense
//------------------------------------------------------------------------------------------//
#ifndef ADS_SMC_h
#define ADS_SMC_h
#ifdef ADS_SMC_h
//------------------------------------------------------------------------------------------//
class SMC_BLOCK{
	public:
				 SMC_BLOCK(void);
		virtual ~SMC_BLOCK(void){;};
	private:
				bool32		blInit;
				uint8*		cgMemOffset;
				uint32		cgCodeSize;
				STDSTR		cgOldMemCode;
	public:
				bool32		Init			(uint8* offset,uint32 codeSize);
				bool32		Decrypt			(void);
				bool32		Resume			(void);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif /* ADS_SMC_h */
#endif /* ADS_SMC_h */

