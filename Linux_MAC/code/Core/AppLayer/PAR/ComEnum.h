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
//------------------------------------------------------------------------------------------//
#include "Comm_Tree.h"
//------------------------------------------------------------------------------------------//
#ifdef Comm_TreeH
#ifndef ComEnumH
#define ComEnumH
#ifdef ComEnumH
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
		 PORTINFO(void){
			strPortName = "";
			strFriendlyName = "";
			strPortDesc = "";
			blFinished = 0;
		 };
		virtual ~PORTINFO(void){;};
	public:
		STDSTR		strPortName;		// Simple name (for example, COM1)
		STDSTR		strFriendlyName;	// Full name to be displayed to a user
		STDSTR		strPortDesc;		// Friendly name without the COMx
		int32		blFinished;
};
//------------------------------------------------------------------------------------------//
class PORTLIST{
	public:
				 PORTLIST(void){;};
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
class IPCOMNAME : public TREE_NODE{
	public:
				 IPCOMNAME(void) : TREE_NODE(){Init();SetSelfName("IPCOMNAME");};
		virtual ~IPCOMNAME(void){;};
	public://need record
		int32		typeID;						//
		STDSTR		strIPComName;				//Simple name (for example, COM1,IP address)
		int32		portBaudrate;				//
		STDSTR		TCPTypeOrFriendlyName;		//
		STDSTR		strUserDefineName;			//
	public:
		STDSTR		strShowName;				//
		int32		blAvailable;				//only use for COM
	public:
		void	Init			(void);
		void	CreateShowName	(G_LOCK blLock = G_LOCK_ON);
		STDSTR	&CreateStrV0_1	(STDSTR *retStr);
		STDSTR	&CreateStrV0_2	(STDSTR *retStr){return(CreateStrV0_1(retStr));};
		STDSTR	&CreateStrV0_3	(STDSTR *retStr){return(CreateStrV0_1(retStr));};
		STDSTR	&CreateStrV0_4	(STDSTR *retStr);
		STDSTR	&CreateStrV0_5	(STDSTR *retStr){return(CreateStrV0_4(retStr));};
		STDSTR	&CreateStrV0_6	(STDSTR *retStr){return(CreateStrV0_4(retStr));};
		void	SetIPComV0_1	(STDSTR *strIn);
		void	SetIPComV0_2	(STDSTR *strIn){SetIPComV0_1(strIn);};
		void	SetIPComV0_3	(STDSTR *strIn){SetIPComV0_1(strIn);};
		void	SetIPComV0_4	(STDSTR *strIn);
		void	SetIPComV0_5	(STDSTR *strIn){SetIPComV0_4(strIn);};
		void	SetIPComV0_6	(STDSTR *strIn){SetIPComV0_4(strIn);};
		static void	Copy		(IPCOMNAME *node1,const IPCOMNAME *node2,G_LOCK blLock = G_LOCK_ON);//copy 2 to 1
};
//------------------------------------------------------------------------------------------//
class IPCOMLIST : public TREE_NODE{
	public:
				 IPCOMLIST(void){selectedNode = nullptr;CreateTrash(this);SetSelfName("IPCOMLIST");};
		virtual ~IPCOMLIST(void){DestroyAll();};
	protected:
		inline	virtual	TNF*	CreateNode	(void){return(SetSubNodeSelfName(new IPCOMNAME));};
	public:
		inline	void	Empty	(void){CleanChild(this);};
		STDSTR		&CreateStrV0_1(STDSTR *retStr);
		STDSTR		&CreateStrV0_2(STDSTR *retStr){return(CreateStrV0_1(retStr));};
		STDSTR		&CreateStrV0_3(STDSTR *retStr){return(CreateStrV0_1(retStr));};
		STDSTR		&CreateStrV0_4(STDSTR *retStr);
		STDSTR		&CreateStrV0_5(STDSTR *retStr){return(CreateStrV0_4(retStr));};
		STDSTR		&CreateStrV0_6(STDSTR *retStr){return(CreateStrV0_4(retStr));};
		void		SetIPComListV0_1(STDSTR *strIn);
		void		SetIPComListV0_2(STDSTR *strIn){SetIPComListV0_1(strIn);};
		void		SetIPComListV0_3(STDSTR *strIn){SetIPComListV0_1(strIn);};
		void		SetIPComListV0_4(STDSTR *strIn);
		void		SetIPComListV0_5(STDSTR *strIn){SetIPComListV0_4(strIn);};
		void		SetIPComListV0_6(STDSTR *strIn){SetIPComListV0_4(strIn);};
	public:
	public:
		IPCOMNAME	*selectedNode;
	public:
		IPCOMNAME	*AddWithCheck(IPCOMNAME *tempIPComName,G_LOCK blLock = G_LOCK_ON);
		void		Refresh		(void);
		void		Export		(IPCOMLIST *cExportList);
		void		ExportCOM	(IPCOMLIST *cExportList);
};
//------------------------------------------------------------------------------------------//
#endif
#endif
#endif

