//
//  BIC_COM.h
//  SDT
//
//  Created by Leif Wen on 16/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "BIC_Dev.h"
#include "Commu_ComEnum.h"
#if defined BIC_Dev_h && defined Commu_Com_h
//------------------------------------------------------------------------------------------//
#ifndef BIC_COM_h
#define BIC_COM_h
#ifdef BIC_COM_h
//------------------------------------------------------------------------------------------//
BIC_CLASSTYPE	(COM,		"com");
BIC_CLASSTYPE	(COM_BR,	"br");
BIC_CLASSTYPE	(COM_DTR,	"dtr");
BIC_CLASSTYPE	(COM_RTS,	"rts");
BIC_CLASSTYPE	(COM_MS,	"ms");
#if defined Commu_ComEnum_h && defined CommonDefH_VC
BIC_CLASSTYPE	(COM_LSCOM,	"lscom");
#endif
BIC_CLASSTYPE	(COM_OPENV,	"openv");
//------------------------------------------------------------------------------------------//
class BIC_ACOM : public BIC_BASE_S{
	public:
				 BIC_ACOM(void);
		virtual ~BIC_ACOM(void){;};
	private:
		BIC_COM			cgSub_com;
		BIC_COM_BR		cgSub_com_br;
		BIC_COM_MS		cgSub_com_ms;
		BIC_COM_DTR		cgSub_com_dtr;
		BIC_COM_RTS		cgSub_com_rts;
#if defined Commu_ComEnum_h && defined CommonDefH_VC
		BIC_COM_LSCOM	cgSub_com_lscom;
#endif
		BIC_COM_OPENV	cgSub_com_openv;
};
//------------------------------------------------------------------------------------------//
#else
	#undef SWVERSION_COM
#endif /* BIC_COM_h */
#endif /* BIC_COM_h */
#else
	#undef SWVERSION_COM
#endif /* BIC_Dev_h */
