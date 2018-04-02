//
//  Commu_ComEnum.cpp
//  SDT
//
//  Created by Leif Wen on 17/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "stdafx.h"
#include "Commu_ComEnum.h"
#ifdef Commu_ComEnum_h
//------------------------------------------------------------------------------------------//
#include "DS_STRING.h"
#include "SYS_File.h"
//------------------------------------------------------------------------------------------//
#ifdef CommonDefH_VC
#include <devguid.h>
#include <setupapi.h>
#pragma comment(lib,"setupapi")
#endif
//------------------------------------------------------------------------------------------//
PORTINFO::PORTINFO(void){
	strPortName = "";
	strFriendlyName = "";
	strPortDesc = "";
	blFinished = G_FALSE;
};
//------------------------------------------------------------------------------------------//
void PORTLIST::DeviceEnumCommPortFriendlyName(void){
#ifdef CommonDefH_VC
	HDEVINFO	hDevInfo;
	DWORD		DeviceIndex;
	STDSTR		FriendlyPortName,strPortDesc,strTemp;   /*Extract Serial Port Name*/
	SP_DEVINFO_DATA	DeviceInfoData;
	int32		i;
	char 		charTemp[2048];
	
	// Create a HDEVINFO with all present devices.
	hDevInfo = SetupDiGetClassDevs((LPGUID) &GUID_DEVCLASS_PORTS,0,0,DIGCF_PRESENT);
	/*
	 GUID_DEVCLASS_FDCÈíÅÌ¿ØÖÆÆ÷
	 GUID_DEVCLASS_DISPLAYÏÔÊ¾¿¨
	 GUID_DEVCLASS_CDROM¹âÇý
	 GUID_DEVCLASS_KEYBOARD¼üÅÌ
	 GUID_DEVCLASS_COMPUTER¼ÆËã»ú
	 GUID_DEVCLASS_SYSTEMÏµÍ³
	 GUID_DEVCLASS_DISKDRIVE´ÅÅÌÇý¶¯Æ÷
	 GUID_DEVCLASS_MEDIAÉùÒô¡¢ÊÓÆµºÍÓÎÏ·¿ØÖÆÆ÷
	 GUID_DEVCLASS_MODEM
	 GUID_DEVCLASS_MOUSEÊó±êºÍÆäËûÖ¸ÕëÉè±¸
	 GUID_DEVCLASS_NETÍøÂçÉè±¸Æ÷
	 GUID_DEVCLASS_USBÍ¨ÓÃ´®ÐÐ×ÜÏß¿ØÖÆÆ÷
	 GUID_DEVCLASS_FLOPPYDISKÈíÅÌÇý¶¯Æ÷
	 GUID_DEVCLASS_UNKNOWNÎ´ÖªÉè±¸
	 GUID_DEVCLASS_SCSIADAPTERSCSI ºÍ RAID ¿ØÖÆÆ÷
	 GUID_DEVCLASS_HDCIDE ATA/ATAPI ¿ØÖÆÆ÷
	 GUID_DEVCLASS_PORTS¶Ë¿Ú£¨COM ºÍ LPT£©
	 GUID_DEVCLASS_MONITOR¼àÊÓÆ÷
	 */
	if (hDevInfo == INVALID_HANDLE_VALUE){
		//DWORD ErrorCode = GetLastError();
		return;
		// Insert error handling here.
	}
	// Enumerate through all devices in Set.
	
	DeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
	
	for (DeviceIndex = 0;SetupDiEnumDeviceInfo(hDevInfo,DeviceIndex,&DeviceInfoData);DeviceIndex++){
		DWORD DataPropertyType;
		LPTSTR buffer = nullptr;
		DWORD buffersize = 0;
		//
		// Call function with nullptr to begin with,
		// then use the returned buffer size
		// to Alloc the buffer. Keep calling until
		// success or an unknown failure.
		//
		while (!SetupDiGetDeviceRegistryProperty(hDevInfo,&DeviceInfoData,SPDRP_FRIENDLYNAME,&DataPropertyType,(PBYTE)buffer,buffersize,&buffersize)){
			DWORD ErrorCode = GetLastError();
			if (ErrorCode == ERROR_INSUFFICIENT_BUFFER){
				// Change the buffer size.
				if (buffer)
					LocalFree(buffer);
				buffer = (LPTSTR)LocalAlloc(LPTR,buffersize);
			}
			else{
				// Insert error handling here.
				break;
			}
		}
		FriendlyPortName = "";
		if (buffersize > 2048)
			buffersize = 2048;
		if (buffer != nullptr){
			WideCharToMultiByte(CP_ACP,WC_COMPOSITECHECK,buffer,buffersize,charTemp,buffersize,nullptr,nullptr);
			FriendlyPortName = (STDSTR)charTemp;
			Str_TrimSelf(FriendlyPortName);
		}
		
		while (!SetupDiGetDeviceRegistryProperty(hDevInfo,&DeviceInfoData,SPDRP_DEVICEDESC,&DataPropertyType,(PBYTE)buffer,buffersize,&buffersize)){
			DWORD ErrorCode = GetLastError();
			if (ErrorCode == ERROR_INSUFFICIENT_BUFFER){
				// Change the buffer size.
				if (buffer)
					LocalFree(buffer);
				buffer = (LPTSTR)LocalAlloc(LPTR,buffersize);
			}
			else{
				// Insert error handling here.
				break;
			}
		}
		strPortDesc = "";
		if (buffersize > 2048)
			buffersize = 2048;
		if (buffer != nullptr){
			WideCharToMultiByte(CP_ACP,WC_COMPOSITECHECK,buffer,buffersize,charTemp,buffersize,nullptr,nullptr);
			strPortDesc = (STDSTR)charTemp;
			Str_TrimSelf(strPortDesc);
		}
		
		i = 0;
		while(i ++ < 256){
			strTemp = portListInfo[i].strPortName;
			strTemp += ")";
			if ((portListInfo[i].strPortName.length() > 0) && (portListInfo[i].blFinished == 0)){
				if (FriendlyPortName.find(strTemp,0) != STDSTR::npos){
					portListInfo[i].strFriendlyName = FriendlyPortName;
					portListInfo[i].strPortDesc = strPortDesc;
					portListInfo[i].blFinished = 1;
					break;
				}
			}
		}
		
		if(buffer)
			LocalFree(buffer);
	}
	SetupDiDestroyDeviceInfoList(hDevInfo);
#endif
}
//------------------------------------------------------------------------------------------//
int32 PORTLIST::DeviceEnumCommPort(void){
#ifdef CommonDefH_VC
	HKEY   		hKey;
	STDSTR		strTemp;
	int32		i;
	
	i = 0;
	do{
		portListInfo[i].strPortName = "";
		portListInfo[i].strFriendlyName = "";
		portListInfo[i].strPortDesc = "";
		portListInfo[i].blFinished = 0;
	}while(i ++ < 256);
	
	if(RegOpenKeyEx(HKEY_LOCAL_MACHINE,_T("Hardware\\DeviceMap\\SerialComm"),NULL,KEY_READ,&hKey) == ERROR_SUCCESS){
		TCHAR		szPortName[256],szComName[256];
		DWORD		dwLong,dwSize;
		int32		nCount;
		char 		charTemp[2048];
		
		nCount  = 0;
		while(1){
			dwLong = 256;
			dwSize = 256;
			if(RegEnumValue(hKey,nCount,szPortName,&dwLong,nullptr,nullptr,(PUCHAR)szComName,&dwSize) == ERROR_NO_MORE_ITEMS)
				break;
			if (dwSize > 2048)
				dwSize = 2048;
			WideCharToMultiByte(CP_ACP,WC_COMPOSITECHECK,szComName,dwSize,charTemp,dwSize,nullptr,nullptr);
			strTemp = (STDSTR)charTemp;
			Str_TrimSelf(strTemp);
			
			i = atoi(strTemp.substr(3).c_str());
			
			portListInfo[i].strPortName = strTemp;
			portListInfo[i].strFriendlyName = strTemp;
			portListInfo[i].strPortDesc = strTemp;
			++ nCount;
		};
		RegCloseKey(hKey);
		DeviceEnumCommPortFriendlyName();
		return(Sortout());
	}
#endif
	return(0);
}
//------------------------------------------------------------------------------------------//
int32 PORTLIST::Sortout(void){
	int32			i,j;
	
	i = 0;
	j = 0;
	while(j ++ < 256){
		if (portListInfo[j].strPortName.length() > 0){
			portListInfo[i].strPortName = portListInfo[j].strPortName;
			portListInfo[i].strFriendlyName = portListInfo[j].strFriendlyName;
			portListInfo[i].strPortDesc = portListInfo[j].strPortDesc;
			portListInfo[i].blFinished = portListInfo[j].blFinished;
			++ i;
		}
	}
	return(i);
}
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
IPCOMNAME::IPCOMNAME(void) : TREE_NODE(){
	Init();
	SetSelfName("IPCOMNAME");
};
//------------------------------------------------------------------------------------------//
void IPCOMNAME::Init(void){
	typeID = PublicDevice_DEVID_NONE;
	strIPComName = "";
	portBaudrate = 0;
	TCPTypeOrFriendlyName = "";
	strUserDefineName = "";
	strShowName = "";
	blAvailable = 1;
}
//------------------------------------------------------------------------------------------//
void IPCOMNAME::CreateShowName(G_LOCK blLock){
	STDSTR		strTemp;
	InUse_set(blLock);
	strShowName = "";
	if (typeID == PublicDevice_DEVID_APICOM){
		strShowName = strUserDefineName;
		if (strShowName.length() == 0){
			strShowName = TCPTypeOrFriendlyName;
			if (strShowName.length() == 0){
				strShowName = strIPComName;
				return;
			}
		}
		strShowName = strIPComName + "  ::  " + strShowName;
	}
	if (typeID == PublicDevice_DEVID_TCPClient){
		if (strUserDefineName.length() == 0){
			strShowName = "TCP  ::  ";
			strShowName += strIPComName;
			strShowName += "  :  ";
			strShowName += Str_ToStr(portBaudrate);
		}
		else{
			strShowName = "TCP  ::  " + strUserDefineName;
			strShowName += "  :  ";
			strShowName += Str_ToStr(portBaudrate);
		}
	}
	if (typeID == PublicDevice_DEVID_UDPClient){
		if (strUserDefineName.length() == 0){
			strShowName = "UDP  ::  ";
			strShowName += strIPComName;
			strShowName += "  :  ";
			strShowName += Str_ToStr(portBaudrate);
		}
		else{
			strShowName = "UDP  ::  " + strUserDefineName;
			strShowName += "  :  ";
			strShowName += Str_ToStr(portBaudrate);
		}
	}
	if (typeID == PublicDevice_DEVID_TCPServer){
		if (strUserDefineName.length() == 0){
			strShowName = "TCP SERVER";
			strShowName += "  :  ";
			strShowName += Str_ToStr(portBaudrate);
		}
		else{
			strShowName = "TCP SERVER :: " + strUserDefineName;
			strShowName += "  :  ";
			strShowName += Str_ToStr(portBaudrate);
		}
	}
	if (typeID == PublicDevice_DEVID_UDPServer){
		if (strUserDefineName.length() == 0){
			strShowName = "UDP SERVER";
			strShowName += "  :  ";
			strShowName += Str_ToStr(portBaudrate);
		}
		else{
			strShowName = "UDP SERVER :: " + strUserDefineName;
			strShowName += "  :  ";
			strShowName += Str_ToStr(portBaudrate);
		}
	}
	InUse_clr(blLock);
}
//------------------------------------------------------------------------------------------//
STDSTR&	IPCOMNAME::Export(uint32 ver,STDSTR* strOut){
	switch (ver) {
		case 0:;
		case 1:;
		case 2:;
		case 3:;
			break;
		case 4:;
		default:
			ExportV0_4(ver,strOut);
			break;
	}
	return(*strOut);
}
//------------------------------------------------------------------------------------------//
void IPCOMNAME::Import(uint32 ver,STDSTR* strIn){
	switch (ver) {
		case 0:;
		case 1:;
		case 2:;
		case 3:;
			break;
		case 4:;
		default:
			ImportV0_4(ver,strIn);
			break;
	}
	return;
}
//------------------------------------------------------------------------------------------//
STDSTR& IPCOMNAME::ExportV0_4(uint32 ver,STDSTR* strOut){
	//V0.4.11/25/2013
	//½«IPCOMNAME
	//	[ipcom]
	//		typeID =
	//		strIPComName =
	//		portBaudrate =
	//		tcpTypeOrFriendlyName =
	//		strUserDefineName =
	//	[ipcom_end]
	
	InUse_set();
	{
		*strOut +=  "  [ipcom]\n";
		*strOut += ("    typeID = " + Str_ToStr(typeID) + "\n");
		*strOut += ("    strIPComName = " + strIPComName + "\n");
		*strOut += ("    portBaudrate = " + Str_ToStr(portBaudrate) + "\n");
		*strOut += ("    tcpTypeOrFriendlyName = " + TCPTypeOrFriendlyName + "\n");
		if (strUserDefineName.length() > 0)
			*strOut += ("    strUserDefineName = " + strUserDefineName + "\n");
		*strOut +=  "  [ipcom_end]\n";
	}
	InUse_clr();
	return(*strOut);
}
//------------------------------------------------------------------------------------------//
void IPCOMNAME::ImportV0_4(uint32 ver,STDSTR* strIn){
	//V0.4.11/25/2013
	//IPCOMNAME
	//	[ipcom]
	//		typeID =
	//		strIPComName =
	//		portBaudrate =
	//		tcpTypeOrFriendlyName =
	//		strUserDefineName =
	//	[ipcom_end]
	STDSTR		strLine,strItem;
	InUse_set();
	{
		Init();
		while(strIn->length() > 0){
			strLine = Str_Trim(Str_ReadSubItem(strIn,"\n"));
			if (Str_LowerCase(strLine) == "[ipcom_end]")
				break;
			strItem = Str_LowerCase(Str_Trim(Str_ReadSubItem(&strLine,"=")));
			Str_TrimSelf(strLine);
			if (strItem == "typeid"){
				typeID = atoi(strLine.c_str());
			}
			else if (strItem == "stripcomname"){
				strIPComName = strLine;
			}
			if (strItem == "portbaudrate"){
				portBaudrate = atoi(strLine.c_str());
			}
			else if (strItem == "tcptypeorfriendlyname"){
				TCPTypeOrFriendlyName = strLine;
			}
			else if (strItem == "struserdefinename"){
				strUserDefineName = strLine;
			}
		}
		CreateShowName(G_LOCK_OFF);
	}
	InUse_clr();
}
//------------------------------------------------------------------------------------------//
void IPCOMNAME::Copy(IPCOMNAME* node1,const IPCOMNAME* node2,G_LOCK blLock){
	node1->InUse_set(blLock);
	((IPCOMNAME*)node2)->InUse_set(blLock);
	node1->typeID = node2->typeID;
	node1->strIPComName = node2->strIPComName;
	node1->portBaudrate = node2->portBaudrate;
	node1->TCPTypeOrFriendlyName = node2->TCPTypeOrFriendlyName;
	node1->strUserDefineName = node2->strUserDefineName;
	node1->strShowName = node2->strShowName;
	node1->blAvailable = node2->blAvailable;
	((IPCOMNAME*)node2)->InUse_clr(blLock);
	node1->InUse_clr(blLock);
}
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
IPCOMLIST::IPCOMLIST(void) : TREE_NODE(){
	selectedNode = nullptr;
	SetSelfName("IPCOMLIST");
};
//------------------------------------------------------------------------------------------//
IPCOMLIST::~IPCOMLIST(void){
	Empty();
};
//------------------------------------------------------------------------------------------//
TNF* IPCOMLIST::CreateNode(void){
	return(SetSubNodeFatherName(new IPCOMNAME));
};
//------------------------------------------------------------------------------------------//
void IPCOMLIST::Empty(void){
	CleanChild(this,this);
};
//------------------------------------------------------------------------------------------//
void IPCOMLIST::Save(const STDSTR& fileName){
	STDSTR	strResult;
	strResult = "[V0.4]\n";
	
	CFS_WriteFile(fileName,Export(4,&strResult));
};
//------------------------------------------------------------------------------------------//
void IPCOMLIST::Load(const STDSTR& fileName){
	STDSTR	strResult,strLine;
	uint32	ver;
	
	strResult = "";
	CFS_ReadFile(&strResult,fileName);
	
	strLine = Str_LowerCase(Str_Trim(Str_ReadSubItem(&strResult,"\n")));
	
	ver = 4;
	if (strLine == "[v0.1]"){
		ver = 1;
	}
	else if (strLine == "[v0.2]"){
		ver = 2;
	}
	else if (strLine == "[v0.3]"){
		ver = 3;
	}
	
	while(strResult.length() > 0){
		strLine = Str_LowerCase(Str_Trim(Str_ReadSubItem(&strResult,"\n")));
		if (strLine == "[ipcomlist]")
			Import(ver,&strResult);
	};
};
//------------------------------------------------------------------------------------------//
STDSTR&	IPCOMLIST::Export(uint32 ver,STDSTR* strOut){
	switch (ver) {
		case 0:;
		case 1:;
		case 2:;
		case 3:;
			break;
		case 4:;
		default:
			ExportV0_4(ver,strOut);
			break;
	}
	return(*strOut);
}
//------------------------------------------------------------------------------------------//
void IPCOMLIST::Import(uint32 ver,STDSTR* strIn){
	switch (ver) {
		case 0:;
		case 1:;
		case 2:;
		case 3:;
			break;
		case 4:;
		default:
			ImportV0_4(ver,strIn);
			break;
	}
	return;
}
//------------------------------------------------------------------------------------------//
STDSTR& IPCOMLIST::ExportV0_4(uint32 ver,STDSTR* strOut){
	//V0.4,11/25/2013
	//{(IPCOMNAME1)(IPCOMNAME2)}
	//	[ipcomlist]
	//		[ipcom]
	//		[ipcom_end]
	//		[ipcom]
	//		[ipcom_end]
	//	[ipcomlist_end]
	
	*strOut += "[ipcomlist]\n";
	TREE_LChildRChain_Traversal_LINE(IPCOMNAME,this, _opNode->Export(ver,strOut));
	*strOut += "[ipcomlist_end]\n";
	
	return(*strOut);
}
//------------------------------------------------------------------------------------------//
void IPCOMLIST::ImportV0_4(uint32 ver,STDSTR* strIn){
	//V0.4,11/25/2013
	//{(IPCOMNAME1)(IPCOMNAME2)}
	//	[ipcomlist]
	//		[ipcom]
	//		[ipcom_end]
	//		[ipcom]
	//		[ipcom_end]
	//	[ipcomlist_end]
	
	IPCOMNAME	node;
	STDSTR		strLine,strItem;
	while(strIn->length() > 0){
		strLine = Str_LowerCase(Str_Trim(Str_ReadSubItem(strIn,"\n")));
		if (strLine == "[ipcomlist_end]")
			break;
		if (strLine == "[ipcom]"){
			node.Import(ver,strIn);
			AddWithCheck(&node);
		}
	}
}
//------------------------------------------------------------------------------------------//
bool32 NewNodeCheck(IPCOMNAME* nodeInList,IPCOMNAME* checkNode){
	bool32 blChecked;
	blChecked = G_FALSE;
	if ((nodeInList->typeID == checkNode->typeID) && (Str_UpperCase(nodeInList->strIPComName) == Str_UpperCase(checkNode->strIPComName))){
		if (checkNode->typeID == PublicDevice_DEVID_APICOM){
			if (Str_UpperCase(nodeInList->TCPTypeOrFriendlyName) == Str_UpperCase(checkNode->TCPTypeOrFriendlyName)){
				blChecked = G_TRUE;
				nodeInList->blAvailable = G_TRUE;
			}
		}
		else if (nodeInList->portBaudrate == checkNode->portBaudrate){
			blChecked = G_TRUE;
		}
	}
	return(blChecked);
}
//------------------------------------------------------------------------------------------//
IPCOMNAME* IPCOMLIST::AddWithCheck(IPCOMNAME* newIPComName,G_LOCK blLock){
	IPCOMNAME	*retNode;
	
	TREE_LChildRChain_Find(IPCOMNAME,this,retNode,(NewNodeCheck(_opNode,newIPComName)));
	
	if (retNode == nullptr){
		retNode = (IPCOMNAME*)GetNewNode();
		if (retNode != nullptr){
			IPCOMNAME::Copy(retNode,newIPComName,blLock);
			AddNode(retNode);
		}
	}
	return(retNode);
}
//------------------------------------------------------------------------------------------//
void IPCOMLIST::Refresh(void){
	PORTLIST	tPortlist;
	int32		i,num;
	IPCOMNAME 	node;
	
	TREE_LChildRChain_Traversal_LINE(IPCOMNAME,this,
		_opNode->InUse_set();
		if (_opNode->typeID == PublicDevice_DEVID_APICOM)
			_opNode->blAvailable = 0;
			_opNode->InUse_clr();
	);
	
	num = tPortlist.DeviceEnumCommPort();
	i = 0;
	while (i++ < num){
		node.Init();
		node.typeID = PublicDevice_DEVID_APICOM;
		node.strIPComName = tPortlist.portListInfo[i - 1].strPortName;
		node.portBaudrate = 115200;
		node.TCPTypeOrFriendlyName = tPortlist.portListInfo[i - 1].strFriendlyName;
		node.strUserDefineName = "";
		node.CreateShowName();
		AddWithCheck(&node);
	}
}
//------------------------------------------------------------------------------------------//
void IPCOMLIST::Export(IPCOMLIST* cExportList){
	cExportList->Empty();
	
	TREE_LChildRChain_Traversal_LINE(IPCOMNAME,this,
		_opNode->InUse_set();
		if (_opNode->blAvailable != 0)
			cExportList->AddWithCheck(_opNode,G_LOCK_OFF);
			_opNode->InUse_clr();
	);
}
//------------------------------------------------------------------------------------------//
void IPCOMLIST::ExportCOM(IPCOMLIST* cExportList){
	cExportList->Empty();
	
	TREE_LChildRChain_Traversal_LINE(IPCOMNAME,this,
		_opNode->InUse_set();
		if ((_opNode->blAvailable != 0) && (_opNode->typeID == PublicDevice_DEVID_APICOM))
			cExportList->AddWithCheck(_opNode,G_LOCK_OFF);
			_opNode->InUse_clr();
	);
}
//------------------------------------------------------------------------------------------//
#endif /* Commu_ComEnum_h */
