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
#ifndef Commu_ComH
#define Commu_ComH
//------------------------------------------------------------------------------------------//
#include "Commu_DBuf.h"
//------------------------------------------------------------------------------------------//
class APICOM : public COMMU_DBUF{
	#ifdef CommonDefH_Unix
		typedef	int32	HANDLE;
	#endif
	public:
		enum{RFLAG_C = 0, RFLAG_S = COMMU_DBUF::RFLAG_S + COMMU_DBUF::RFLAG_C};
	public:
				 APICOM(const ODEV_LIST *tODEV_LIST,uint32 tSize) : COMMU_DBUF(tODEV_LIST,tSize){Init();selfName = "APICOM";};
		virtual ~APICOM(void){CloseD();};
	private:
		void			Init(void);
		virtual	int32	ReadFromDevice	(uint32 *retNum,uint8 *buffer,uint32 length);
		virtual	int32	SendToDevice	(uint32 *retNum,const uint8 *buffer,uint32 length);
		virtual	int32	ExThreadFun		(void);
				void	UpdateMainComModemStatus	(void);
	protected:
		virtual	int32	OpenDev			(const std::string &tCDBufName,int32 tCDBufPar,CSType tCSType);
		virtual	void	CloseDev		(void);
				int32	SetBaudrate_Do	(int32 comBaudRate);
	public:
				int32	OpenV			(const std::string &tCDBufName,int32 tCDBufPar,CSType tCSType,int32 blEnEcho);
	protected:
		virtual	int32	OpenVDev		(const std::string &tCDBufName,int32 tCDBufPar,CSType tCSType);
		virtual	void	CloseVDev		(void);
	protected:
		HANDLE			Handle;
		std::string		vPortName;
	private:
		std::string		CTS;
		std::string		DSR;
		std::string		RING;
		std::string		DCD;
		int32			DTR;
		int32			RTS;
	public:
		void			SetDSRFlow			(int32 blEnable);
		void			SetCTSFlow			(int32 blEnable);
		std::string		GetCTSStatus		(void);
		std::string		GetDSRStatus		(void);
		std::string		GetRINGStatus		(void);
		std::string		GetDCDStatus		(void);
		int32			GetDTRStatus		(void){return(DTR);};
		int32			GetRTSStatus		(void){return(RTS);};
		void			SetDTRToHigh		(void);
		void			SetDTRToLow			(void);
		void			SetRTSToHigh		(void);
		void			SetRTSToLow			(void);
		int32			SetBaudrate			(int32 comBaudRate);
};
//------------------------------------------------------------------------------------------//
#endif
