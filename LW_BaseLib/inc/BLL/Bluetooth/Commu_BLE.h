//
//  Commu_BLE.h
//  SDT
//
//  Created by Leif Wen on 12/09/2020.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "Commu_Base.h"
#ifdef Commu_Base_h
//------------------------------------------------------------------------------------------//
#ifndef Commu_BLE_h
#define Commu_BLE_h
#ifdef Commu_BLE_h
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
enum {
	BLE_OPENTYPE_ANY = 0,
	BLE_OPENTYPE_NAME,
	BLE_OPENTYPE_ID,
	BLE_OPENTYPE_INDEX,
};
struct BINDPAR{
	STDSTR 			identifier;
	STDSTR 			service;
	STDSTR			characteristic;
};
//------------------------------------------------------------------------------------------//
class CORE_BLEBIND : public COMMU_CORE{
	public:
		enum	{PACKAGE_MAX_SIZE = 1024 * 8};
	public:
				 CORE_BLEBIND(void);
		virtual ~CORE_BLEBIND(void);
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
	private:
		SBUF			*cgBTrxSBUF;
	public:
		BINDPAR			cgBindParWrite;
		BINDPAR			cgBindParNotify;
		uint32			cgMTU;
	public:
				bool32	Open				(const STDSTR& aliasName,const STDSTR& _write,const STDSTR& _notify,uint32 cfg);
};
//------------------------------------------------------------------------------------------//
class CORE_BLEPeripheral : public COMMU_CORE{
	public:
		enum	{PACKAGE_MAX_SIZE = 512};
	public:
				 CORE_BLEPeripheral(void);
		virtual ~CORE_BLEPeripheral(void);
	protected:
		virtual	bool32	OpenDev				(const OPEN_PAR& par);
		virtual	void	CloseDev			(void);
	private:
		virtual void	PrintOpenSuccess	(const STDSTR& strTitle = "");
		virtual void	PrintOpenFail		(const STDSTR& strTitle = "");
	public:
		STDSTR 			cgIdentifier;
		uint32			cgMTU;
	public:
};
//------------------------------------------------------------------------------------------//
typedef COMMU_DRV_T<TDEFALL,COMMU_DRV,CORE_BLEBIND>											BLEBIND;
typedef CDRV_POOL_T<COMMU_DRV_T<TODEVBUS|TDISPOSE,COMMU_DRV,CORE_BLEPeripheral>,BLEBIND>	_BLEPeripheral;
//------------------------------------------------------------------------------------------//
class BLEPeripheral : public _BLEPeripheral{
	public:
				 BLEPeripheral(void){;};
				 BLEPeripheral(uint32 rxSize,uint32 txSize,const ODEV_BUS* odevBus) : _BLEPeripheral(rxSize,txSize,odevBus){;};
		virtual ~BLEPeripheral(void){;};
	public:
		static	STDSTR		ListDescription	(const OPEN_PAR& par);
				bool32		Bind			(const STDSTR& aliasName,const STDSTR& _write,const STDSTR& _notify);
				bool32		Unbind			(uint32	index);
				uint32		CWrite			(const STDSTR& ids,const STDSTR&  _in);
				//uint32		CRead			(const STDSTR& ids,STDSTR* _out,uint32 num);
				bool32		RecvCallback	(const BINDPAR& bindpar,const uint8* buffer,uint32 num);
};
//------------------------------------------------------------------------------------------//
#endif /* Commu_BLE_h */
#endif /* Commu_BLE_h */
#endif /* Commu_Base_h */
