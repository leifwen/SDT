//
//  Commu_Com.h
//  SDT
//
//  Created by Leif Wen on 05/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "Commu_DBuf.h"
#ifdef Commu_DBuf_h
//------------------------------------------------------------------------------------------//
#ifndef Commu_Com_h
#define Commu_Com_h
#ifdef Commu_Com_h
//------------------------------------------------------------------------------------------//
class ACOM : public COMMU_THREAD{
	public:
#ifdef CommonDefH_Unix
		typedef	int32	HANDLE;
#endif
	protected:
		enum	{RFLAG_C = 2, RFLAG_S = COMMU_THREAD::RFLAG_S + COMMU_THREAD::RFLAG_C};
	private:
		enum	{blDTR = RFLAG_CREATE(0),blRTS = RFLAG_CREATE(1),};
	public:
				 ACOM(uint32 size,const ODEV_SYSTEM* logSys);
		virtual ~ACOM(void);
	private:
				void		Init					(void);
	public:
		virtual	void		SetSelfName				(const STDSTR& strName);
		virtual	void		SetFatherName			(const STDSTR& strName);
	protected:
		virtual	bool32		OpenDev					(const OPEN_PAR& par);
		virtual	void		CloseDev				(void);
	private:
		virtual	bool32		SendToDevice			(uint32* retNum,const uint8* buffer,uint32 length);
		virtual	bool32		ReadFromDevice			(uint32* retNum,	  uint8* buffer,uint32 length);
	private:
		virtual void		DoPrintOnOpenSuccess	(void);
		virtual void		DoPrintOnClose			(void);
	protected:
		SYS_Thread<ACOM>	modemStatusThread;
		bool32				ModemStatusThreadFun	(void* commu);
	protected:
		HANDLE				osHandle;
		STDSTR				vPortName;
	private:
		STDSTR				modemStatus;
	public:
				STDSTR		GetFullModemStatus		(void);
				STDSTR		GetModemStatus			(void);
				STDSTR		GetCTSStatus			(void);
				STDSTR		GetDSRStatus			(void);
				STDSTR		GetRINGStatus			(void);
				STDSTR		GetDCDStatus			(void);
				bool32		GetDTRStatus			(void);
				bool32		GetRTSStatus			(void);
	public:
				void		SetDTRToHigh			(void);
				void		SetDTRToLow				(void);
				void		SetRTSToHigh			(void);
				void		SetRTSToLow				(void);
				void		SetDTR					(bool32 blHigh);
				void		SetRTS					(bool32 blHigh);
				void		SetDSRFlow				(bool32 blEnable);
				void		SetCTSFlow				(bool32 blEnable);
	public:
				bool32		SetBaudrate				(int32 comBaudRate);
};
//------------------------------------------------------------------------------------------//
#endif /* Commu_Com_h */
#endif /* Commu_Com_h */
#endif /* Commu_DBuf_h */
