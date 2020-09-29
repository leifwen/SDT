//
//  Commu_Bus.h
//  SDT
//
//  Created by Leif Wen on 13/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "Commu_Base.h"
#if defined ODEV_BUS_h && defined Commu_Base_h
//------------------------------------------------------------------------------------------//
#ifndef Commu_Bus_h
#define Commu_Bus_h
#ifdef Commu_Bus_h
//------------------------------------------------------------------------------------------//
class ODEV_CACHE;
//------------------------------------------------------------------------------------------//
typedef void*(*TypeConstructor)(uint32 rxSize,uint32 txSize,const ODEV_BUS* odevBus);
class COMMU_DEV : public TNF{
	public:
				 COMMU_DEV(void){name = "";Constructor = nullptr;};
		virtual	~COMMU_DEV(void){;};
	public:
		STDSTR				name;
		TypeConstructor		Constructor;
};
//------------------------------------------------------------------------------------------//
#define	CONSTRUCTOR_FUNC(_func)		Constructor_##_func
#define	DEF_CONSTRUCTOR_FUNC(_type,_func) \
	static void* Constructor_##_func (uint32 rxSize,uint32 txSize,const ODEV_BUS* odevBus){\
		return(new _type(rxSize,txSize,odevBus));\
	};
//------------------------------------------------------------------------------------------//
class CORE_DEVBUS : public COMMU_CORE{
	public:
				 CORE_DEVBUS(void);
		virtual ~CORE_DEVBUS(void);
	private:
		virtual	TNF*			CreateNode				(void);
	protected:
		virtual	bool32			OpenDev					(const OPEN_PAR& par);
		virtual	void			CloseDev				(void);
	private:
		virtual void			PrintOpenSuccess		(const STDSTR& strTitle = ""){;};
		virtual void			PrintOpenFail			(const STDSTR& strTitle = ""){;};
		virtual void			PrintClose				(const uint64& rxBytes,const uint64& txBytes,const uint64& dpBytes){;};
	public:
		COMMU_DEV*				IsRegistered			(const STDSTR &name);
		bool32					Register				(const STDSTR& name,TypeConstructor func);
};
//------------------------------------------------------------------------------------------//
typedef  CDRV_POOL_T<COMMU_DRV_T<TODEVBUS|TDISPOSE,COMMU_DRV,CORE_DEVBUS>,COMMU_DRV>	_COMMU_DEVBUS;
//------------------------------------------------------------------------------------------//
class COMMU_DEVBUS : public _COMMU_DEVBUS{
	public:
				 COMMU_DEVBUS(uint32 rxSize,uint32 txSize,const ODEV_CACHE* cache);
		virtual ~COMMU_DEVBUS(void);
	private:
				virtual	void	CloseChild			(COMMU_DRV *cdrv);
		inline	virtual	void	CloseAllChild		(void){;};
	private:
		ODEV_BUS			cgODevBus;
	public:
		bool32				SetTo					(const STDSTR& drvName);
		COMMU_DRV*			IsFor					(const STDSTR& drvName);
		COMMU_DRV*			GetDrv					(const STDSTR& drvName);
		bool32				Register				(const STDSTR& name,TypeConstructor func);
	public:
		OUTPUT_NODE* 		GetSelSTDOUT			(void);
		ODEV_BUS*			GetODevBus				(void)const;
		void				SetEnvCFG				(const uint64& flag);
		void				ClrEnvCFG				(const uint64& flag);
	public:
		uint32				TransformCommand		(STDSTR* retForSend,STDSTR* retForPrint,const STDSTR& cmd,CMD_TAIL tail,G_ESCAPE blEscape);
		
		uint32				SendCommand				(const STDSTR& cmd);
		uint32				SendCommand				(const STDSTR& cmd,CMD_TAIL tail,G_ESCAPE blEscape);
		bool32				SendCommand				(const STDSTR& cmd,const STDSTR& strCondition,ARRAY* array,uint32 waitMS,uint32 delyMS);
		bool32				CheckReceive			(const STDSTR& strCondition,ARRAY* array,uint32 waitMS,uint32 delyMS);
		bool32				Open					(const STDSTR& drvName,const OPEN_PAR& par);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif /* Commu_Bus_h */
#endif /* Commu_Bus_h */
#endif /* Commu_Base_h */
