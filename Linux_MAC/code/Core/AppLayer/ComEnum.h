/*
 * Copyright (c) 2012-2014 
 * All rights reserved.
 *
 * File Name: ComEnum.h
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2012.12.22
 * @2013.6.30 :	change to Liunx style
 				add init method
 				use int32 to replace bool
 				use int32 to replace uint32
*/
#ifndef ComEnumH
#define ComEnumH
//------------------------------------------------------------------------------------------//
#include "Comm_Tree.h"
//------------------------------------------------------------------------------------------//
#define PublicDevice_DEVID_NONE				(0x00)
#define PublicDevice_DEVID_APICOM			(0x04)
#define PublicDevice_DEVID_TCPClient		(0x07)
#define PublicDevice_DEVID_UDPClient		(0x08)
#define PublicDevice_DEVID_TCPServer		(0x09)
#define PublicDevice_DEVID_UDPServer		(0x0A)
//------------------------------------------------------------------------------------------//
class PORTINFO{
	public:
		 PORTINFO(void){
			strDevPath = "";
			strPortName = "";
			strFriendlyName = "";
			strPortDesc = "";
			bUsbDevice = 0;
			blFinished = 0;
		 };
		virtual ~PORTINFO(void){;};
	public:
		std::string		strDevPath;			// Device path for use with CreateFile()
		std::string		strPortName;		// Simple name (for example, COM1)
		std::string		strFriendlyName;	// Full name to be displayed to a user
		std::string		strPortDesc;		// Friendly name without the COMx
		int32			bUsbDevice;			// Provided through a USB connection?
		int32			blFinished;
};
//------------------------------------------------------------------------------------------//
class PORTLIST{
	public:
				 PORTLIST(void);
		virtual ~PORTLIST(void){;};
	private:
		void		DeviceEnumCommPortFriendlyName(void);
	public:
		int32		Sortout(void);
		PORTINFO	portListInfo[257];
		int32		DeviceEnumCommPort(void);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
class IPCOMNAME : public RTREE_NODE{
	public:
				 IPCOMNAME(void) : RTREE_NODE(){Init(this);};
		virtual ~IPCOMNAME(void){;};
	public://need record
		int32			typeID;						//
		std::string		strIPComName;				//Simple name (for example, COM1,IP address)
		int32			portBaudrate;				//
		std::string		TCPTypeOrFriendlyName;		//
		std::string		strUserDefineName;			//
	public:
		std::string		strShowName;				//
		int32			blAvailable;				//only use for COM
	public:
		static void			Init			(IPCOMNAME *node,G_LOCK_VAILD blLock = G_LOCK_ON);
		static void			CreateShowName	(IPCOMNAME *node,G_LOCK_VAILD blLock = G_LOCK_ON);
		static std::string	CreateStrV0_1	(IPCOMNAME *node);
		static std::string	CreateStrV0_2	(IPCOMNAME *node){return(CreateStrV0_1(node));};
		static std::string	CreateStrV0_3	(IPCOMNAME *node){return(CreateStrV0_1(node));};
		static std::string	CreateStrV0_4	(IPCOMNAME *node);
		static std::string	CreateStrV0_5	(IPCOMNAME *node){return(CreateStrV0_4(node));};
		static void			SetIPComV0_1	(IPCOMNAME *node,std::string *strInput);
		static void			SetIPComV0_2	(IPCOMNAME *node,std::string *strInput){SetIPComV0_1(node,strInput);};
		static void			SetIPComV0_3	(IPCOMNAME *node,std::string *strInput){SetIPComV0_1(node,strInput);};
		static void			SetIPComV0_4	(IPCOMNAME *node,std::string *strInput);
		static void			SetIPComV0_5	(IPCOMNAME *node,std::string *strInput){SetIPComV0_4(node,strInput);};
		static void			Copy(IPCOMNAME *node2,IPCOMNAME *node1,G_LOCK_VAILD blLock = G_LOCK_ON);//copy 2 to 1
};
//------------------------------------------------------------------------------------------//
class IPCOMLIST : public RTREE_NODE{
	public:
				 IPCOMLIST(void){selectedNode = nullptr;CreateTrash(this);};
		virtual ~IPCOMLIST(void){DestroyAll();};
	public:
		std::string		CreateStrV0_1(void);
		std::string		CreateStrV0_2(void){return(CreateStrV0_1());};
		std::string		CreateStrV0_3(void){return(CreateStrV0_1());};
		std::string		CreateStrV0_4(void);
		std::string		CreateStrV0_5(void){return(CreateStrV0_4());};
		void			SetIPComListV0_1(std::string *strInput);
		void			SetIPComListV0_2(std::string *strInput){SetIPComListV0_1(strInput);};
		void			SetIPComListV0_3(std::string *strInput){SetIPComListV0_1(strInput);};
		void			SetIPComListV0_4(std::string *strInput);
		void			SetIPComListV0_5(std::string *strInput){SetIPComListV0_4(strInput);};
	public:
	public:
		IPCOMNAME	*selectedNode;
	public:
		IPCOMNAME	*AddWithCheck(IPCOMNAME *tempIPComName,G_LOCK_VAILD blLock = G_LOCK_ON);
		void		Refresh		(void);
		void		Export		(IPCOMLIST *cExportList);
		void		ExportCOM	(IPCOMLIST *cExportList);
};
//------------------------------------------------------------------------------------------//
#endif
