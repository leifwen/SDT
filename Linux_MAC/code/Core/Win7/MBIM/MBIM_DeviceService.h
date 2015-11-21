/*
 * Copyright (c) 2012-2014 
 * All rights reserved.
 *
 * File Name: MBIM_DeviceService.h
 * File ID	:
 * Remark	:
 * Writer   : Leif Wen
 * Date     : 2012.12.22
 * @2013.6.30 :	change to Liunx style
 				add init method
 				use int32 to replace bool
 				use int32 to replace uint32
*/
#ifndef MBIM_DeviceServiceH
#define MBIM_DeviceServiceH
//------------------------------------------------------------------------------------------//
#include "Commu_DBuf.h"
#ifdef CommonDefH_VC
#include <atlbase.h>
#include "mbnapi.h"
//------------------------------------------------------------------------------------------//
class MBIM_DevService : public COMMU_DBUF{
	public:
		enum{RFLAG_C = 0, RFLAG_S = COMMU_DBUF::RFLAG_S + COMMU_DBUF::RFLAG_C};
	public:
				 MBIM_DevService(ODEV_LIST *tODEV_LIST, uint32 tSize) : COMMU_DBUF(tODEV_LIST, tSize){ Init(); };
		virtual ~MBIM_DevService(void){ Close(); };
	private:
		void			Init(void);
		virtual	int32	ExThreadFun		(void);
		virtual	int32	ReadFromDevice	(uint8 *buffer,uint32 readLength,uint32 *retNum);
		virtual	int32	SendToDevice	(uint8 *buffer,uint32 readLength,uint32 *retNum);
	protected:
				int32	CloseAPICOM	(void);
				int32	CloseV		(void);
				CComPtr<IMbnDeviceServicesManager>  cgMbnDeviceServicesMgr;
				CComPtr<IMbnDeviceServicesContext>  cgMbnDeviceServicesContext;
				CComPtr<IMbnInterfaceManager>		cgMbnInterfaceMgr;
				CComPtr<IMbnInterface>				cgMbnInterface;
				CComPtr<IMbnDeviceService>			cgMbnDeviceService;
	public:
		virtual	int32		Open			(const std::string &tCDBufName,int32 tCDBufPar,CSType tCSType,int32 blEnEcho);
		virtual	int32		OpenV			(const std::string &tCDBufName,int32 tCDBufPar,CSType tCSType,int32 blEnEcho);
		virtual	int32		Close			(int32 blClr = 1);
		void	UpdateMainComModemStatus	(void);
	public:
		HANDLE			Handle;
		std::string		vPortName;
};
//------------------------------------------------------------------------------------------//
#endif
#endif
