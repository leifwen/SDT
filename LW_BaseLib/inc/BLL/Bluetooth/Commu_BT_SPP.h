//
//  Commu_BT_SPP.h
//  SDT
//
//  Created by Leif Wen on 12/09/2020.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "Commu_Base.h"
#ifdef Commu_Base_h
//------------------------------------------------------------------------------------------//
#ifndef Commu_BT_SPP_h
#define Commu_BT_SPP_h
#ifdef Commu_BT_SPP_h
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
class CORE_BTSPP : public COMMU_CORE{
	public:
		enum	{PACKAGE_MAX_SIZE = 1024 * 8};
	public:
				 CORE_BTSPP(void);
		virtual ~CORE_BTSPP(void);
	protected:
		virtual	bool32	OpenDev				(const OPEN_PAR& par);
		virtual	void	CloseDev			(void);
	private:
		virtual	bool32	SendToDevice		(uint32* retNum,const uint8 *buffer,uint32 length);
		virtual	bool32	ReadFromDevice		(uint32* retNum,	  uint8 *buffer,uint32 length);
	private:
		virtual void	PrintOpenSuccess	(const STDSTR& strTitle = "");
		virtual void	PrintOpenFail		(const STDSTR& strTitle = "");
		virtual void	PrintClose			(const uint64& rxBytes,const uint64& txBytes,const uint64& dpBytes);
	public:
		SBUF			*cgBTrxSBUF;
};
//------------------------------------------------------------------------------------------//
typedef COMMU_DRV_T<TDEFALL,COMMU_DRV,CORE_BTSPP>	BTSPP;
//------------------------------------------------------------------------------------------//

#endif /* Commu_BT_SPP_h */
#endif /* Commu_BT_SPP_h */
#endif /* Commu_Base_h */
