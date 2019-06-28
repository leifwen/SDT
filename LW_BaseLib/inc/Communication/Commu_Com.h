//
//  Commu_Com.h
//  SDT
//
//  Created by Leif Wen on 05/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "Commu_Base.h"
#ifdef Commu_Base_h
//------------------------------------------------------------------------------------------//
#ifndef Commu_Com_h
#define Commu_Com_h
#ifdef Commu_Com_h
//------------------------------------------------------------------------------------------//
class CORE_ACOM : public COMMU_CORE{
	typedef CORE_ACOM CACOM;
	public:
#ifdef CommonDefH_Unix
		typedef	int32	HANDLE;
#endif
	protected:
		enum	{RFLAG_C = 8, RFLAG_S = COMMU_CORE::RFLAG_S + COMMU_CORE::RFLAG_C};
		enum	{ACOM_blDSR		= RFLAG_CREATE(0)
				,ACOM_blCTS		= RFLAG_CREATE(1)
				,ACOM_blDCD		= RFLAG_CREATE(2)
				,ACOM_blRING	= RFLAG_CREATE(3)
				,ACOM_blDTR		= RFLAG_CREATE(4)
				,ACOM_blRTS		= RFLAG_CREATE(5)
				,ACOM_blCTSFlow	= RFLAG_CREATE(6)
				,ACOM_blDSRFlow	= RFLAG_CREATE(7)
		};
	private:
		enum	{OSTXBUF_MAX_SIZE = 1024 * 8};
	public:
				 CORE_ACOM(void);
		virtual ~CORE_ACOM(void);
	public:
		virtual	void		Init					(const COMMU_TEAM* _team);
		virtual	void		SetSelfName				(const STDSTR& strName);
		virtual	void		SetUpName				(const STDSTR& strName);
	protected:
		virtual	bool32		OpenDev					(const OPEN_PAR& par);
		virtual	void		CloseDev				(void);
	private:
		virtual	bool32		SendToDevice			(uint32* retNum,const uint8* buffer,uint32 length);
		virtual	bool32		ReadFromDevice			(uint32* retNum,	  uint8* buffer,uint32 length);
	private:
		virtual void		PrintOpenSuccess		(const STDSTR& strTitle = "");
		virtual void		PrintClose				(const uint64& rxBytes,const uint64& txBytes,const uint64& fwBytes);
	protected:
		SYS_Thread<CACOM>	modemStatusThread;
		bool32				ModemStatusThreadFun	(void* _team);
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
				bool32		GetDSRFlowStatus		(void);
				bool32		GetCTSFlowStatus		(void);
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
typedef  COMMU<TDEFALL,COMMU_FRAME,CORE_ACOM>	ACOM;
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif /* Commu_Com_h */
#endif /* Commu_Com_h */
#endif /* Commu_Base_h */
