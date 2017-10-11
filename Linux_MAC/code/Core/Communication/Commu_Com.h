/*
 * Copyright (c) 2012-2014 
 * All rights reserved.
 *
 * File Name: Commu_Com.h
 * File ID	:
 * Remark	:
 * Writer   : Leif Wen
 * Date     : 2012.12.22
 * @2013.6.30 :	change to Liunx style
 				add init method
 				use int32 to replace bool
 				use int32 to replace uint32
*/
//------------------------------------------------------------------------------------------//
#include "Commu_DBuf.h"
#ifdef COMMU_DBUFH
#ifndef Commu_ComH
#define Commu_ComH
#ifdef Commu_ComH
//------------------------------------------------------------------------------------------//
class ACOM : public COMMU_DBUF{
	public:
	#ifdef CommonDefH_Unix
		typedef	int32	HANDLE;
	#endif
	public:
		enum	{RFLAG_C = 2, RFLAG_S = COMMU_DBUF::RFLAG_S + COMMU_DBUF::RFLAG_C};
	private:
		enum	{blDTR = RFLAG_CREATE(0),blRTS = RFLAG_CREATE(1),};
	public:
				 ACOM(uint32 tSize,const ODEV_SYSTEM *logSys = nullptr) : COMMU_DBUF(tSize,logSys){Init();SetSelfName("ACOM");};
		virtual ~ACOM(void){CloseD();modemStatusThread.RemoveSelf();};
	private:
		void			Init(void);
	protected:
		virtual	int32	OpenDev				(const STDSTR &tCDBufName,int32 tCDBufPar,CSType tCSType,int32 blEnEcho);
		virtual	void	CloseDev			(void);
	private:
		virtual	int32	ReadFromDevice		(uint32 *retNum,uint8 *buffer,uint32 length);
		virtual	int32	SendToDevice		(uint32 *retNum,const uint8 *buffer,uint32 length);
	private:
		virtual void	DoPrintOnOpenSuccess(void);
		virtual void	DoPrintOnClose		(void);
	protected:
		SYS_Thread<ACOM>	modemStatusThread;
		int32				ModemStatusThreadFun(void *p);
	protected:
		HANDLE		osHandle;
		STDSTR		vPortName;
	private:
		STDSTR		modemStatus;
	public:
		STDSTR		GetFullModemStatus(void);
		STDSTR		GetModemStatus	(void);
		STDSTR		GetCTSStatus	(void);
		STDSTR		GetDSRStatus	(void);
		STDSTR		GetRINGStatus	(void);
		STDSTR		GetDCDStatus	(void);
		int32		GetDTRStatus	(void){return(CheckSFlag(blDTR) != 0);};
		int32		GetRTSStatus	(void){return(CheckSFlag(blRTS) != 0);};
	public:
		void		SetDTRToHigh	(void);
		void		SetDTRToLow		(void);
		void		SetRTSToHigh	(void);
		void		SetRTSToLow		(void);
		void		SetDTR			(int32 blHigh);
		void		SetRTS			(int32 blHigh);
		void		SetDSRFlow		(int32 blEnable);
		void		SetCTSFlow		(int32 blEnable);
	public:
		int32		SetBaudrate		(int32 comBaudRate);
};
//------------------------------------------------------------------------------------------//
#endif
#endif
#endif

