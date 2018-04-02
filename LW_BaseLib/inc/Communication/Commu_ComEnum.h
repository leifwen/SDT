//
//  Commu_ComEnum.h
//  SDT
//
//  Created by Leif Wen on 17/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "DS_Tree.h"
//------------------------------------------------------------------------------------------//
#ifndef Commu_ComEnum_h
#define Commu_ComEnum_h
#ifdef Commu_ComEnum_h
//------------------------------------------------------------------------------------------//
enum{
	PublicDevice_DEVID_NONE			= (0x00),
	PublicDevice_DEVID_APICOM		= (0x04),
	PublicDevice_DEVID_TCPClient	= (0x07),
	PublicDevice_DEVID_UDPClient	= (0x08),
	PublicDevice_DEVID_TCPServer	= (0x09),
	PublicDevice_DEVID_UDPServer	= (0x0A),
};
//------------------------------------------------------------------------------------------//
class PORTINFO{
	public:
			 PORTINFO(void);
	virtual ~PORTINFO(void){;};
	public:
		STDSTR		strPortName;		// Simple name (for example, COM1)
		STDSTR		strFriendlyName;	// Full name to be displayed to a user
		STDSTR		strPortDesc;		// Friendly name without the COMx
		bool32		blFinished;
};
//------------------------------------------------------------------------------------------//
class PORTLIST{
	public:
				 PORTLIST(void){;};
		virtual ~PORTLIST(void){;};
	private:
		void		DeviceEnumCommPortFriendlyName(void);
	public:
		PORTINFO	portListInfo[257];
		bool32		Sortout				(void);
		bool32		DeviceEnumCommPort	(void);
};
//------------------------------------------------------------------------------------------//
class IPCOMNAME : public TREE_NODE{
	public:
				 IPCOMNAME(void);
		virtual ~IPCOMNAME(void){;};
	public://need record
		int32		typeID;						//
		STDSTR		strIPComName;				//Simple name (for example, COM1,IP address)
		int32		portBaudrate;				//
		STDSTR		TCPTypeOrFriendlyName;		//
		STDSTR		strUserDefineName;			//
	public:
		STDSTR		strShowName;				//
		bool32		blAvailable;				//only use for COM
	private:
				STDSTR&		ExportV0_4		(uint32 ver,STDSTR* strOut);
				void		ImportV0_4		(uint32 ver,STDSTR* strIn);
	public:
				void 		Init(void);
				STDSTR&		Export			(uint32 ver,STDSTR* strOut);
				void		Import			(uint32 ver,STDSTR* strIn);
	
				void		CreateShowName	(G_LOCK blLock = G_LOCK_ON);
		static	void		Copy			(IPCOMNAME* node1,const IPCOMNAME* node2,G_LOCK blLock = G_LOCK_ON);//copy 2 to 1
};
//------------------------------------------------------------------------------------------//
class IPCOMLIST : public TREE_NODE{
	public:
				 IPCOMLIST(void);
		virtual ~IPCOMLIST(void);
	protected:
		virtual	TNF*		CreateNode		(void);
	private:
				STDSTR&		ExportV0_4		(uint32 ver,STDSTR* strOut);
				void		ImportV0_4		(uint32 ver,STDSTR* strIn);
	public:
		IPCOMNAME*			selectedNode;
	public:
				STDSTR&		Export			(uint32 ver,STDSTR* strOut);
				void		Import			(uint32 ver,STDSTR* strIn);
	
				void		Save			(const STDSTR& fileName);
				void		Load			(const STDSTR& fileName);
	public:
				void		Empty			(void);
				IPCOMNAME*	AddWithCheck	(IPCOMNAME* tempIPComName,G_LOCK blLock = G_LOCK_ON);
				void		Refresh			(void);
				void		Export			(IPCOMLIST* cExportList);
				void		ExportCOM		(IPCOMLIST* cExportList);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif /* Commu_ComEnum_h */
#endif /* Commu_ComEnum_h */
