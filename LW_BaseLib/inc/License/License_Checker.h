//
//  License_Checker.h
//  SDT
//
//  Created by Leif Wen on 05/03/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "ADS_Crypto.h"
#ifdef ADS_Crypto_h
//------------------------------------------------------------------------------------------//
#ifndef License_Checker_h
#define License_Checker_h
#ifdef License_Checker_h
//------------------------------------------------------------------------------------------//
void	CHK_CheckerInit		(void);
void	CHK_CheckerInitClear(void);
bool32	CHK_CheckTime		(void);
bool32	CHK_GetSMC			(STDSTR* smc,uint32 codeNo);
//------------------------------------------------------------------------------------------//
#endif /* License_Checker_h */
#endif /* License_Checker_h */
//------------------------------------------------------------------------------------------//
#else
#ifndef License_CheckerT_h
#define License_CheckerT_h
inline void	CHK_CheckerInit			(void){;};
inline void	CHK_CheckerInitClear	(void){ ; };
inline bool32	CHK_CheckTime		(void){ return 0; };
inline bool32	CHK_GetSMC			(STDSTR* smc, uint32 codeNo){ return 0; };
#endif
//------------------------------------------------------------------------------------------//
#endif /* ADS_Crypto_h */
