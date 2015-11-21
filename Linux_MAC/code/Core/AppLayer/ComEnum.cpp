/*
 * Copyright (c) 2012-2014
 * All rights reserved.
 *
 * File Name: ComEnum.cpp
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
#include "stdafx.h"
#include "ComEnum.h"
#include "Comm_Convert.h"
//------------------------------------------------------------------------------------------//
#ifdef CommonDefH_VC
#include <devguid.h>
#include <setupapi.h>
#pragma comment(lib,"setupapi")
#endif
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
PORTLIST::PORTLIST(void){
	//DeviceEnumCommPort();//未使用,2013/7/3
}
//------------------------------------------------------------------------------------------//
void PORTLIST::DeviceEnumCommPortFriendlyName(void){
#ifdef CommonDefH_VC
	HDEVINFO	hDevInfo;
	DWORD		DeviceIndex;
	std::string	FriendlyPortName,strPortDesc,strTemp;   /*Extract Serial Port Name*/
	SP_DEVINFO_DATA	DeviceInfoData;
	int32		i;
	char 		charTemp[2048];
	
	// Create a HDEVINFO with all present devices.
	hDevInfo = SetupDiGetClassDevs((LPGUID) &GUID_DEVCLASS_PORTS,0,0,DIGCF_PRESENT);
	/*
	 GUID_DEVCLASS_FDC软盘控制器
	 GUID_DEVCLASS_DISPLAY显示卡
	 GUID_DEVCLASS_CDROM光驱
	 GUID_DEVCLASS_KEYBOARD键盘
	 GUID_DEVCLASS_COMPUTER计算机
	 GUID_DEVCLASS_SYSTEM系统
	 GUID_DEVCLASS_DISKDRIVE磁盘驱动器
	 GUID_DEVCLASS_MEDIA声音、视频和游戏控制器
	 GUID_DEVCLASS_MODEM
	 GUID_DEVCLASS_MOUSE鼠标和其他指针设备
	 GUID_DEVCLASS_NET网络设备器
	 GUID_DEVCLASS_USB通用串行总线控制器
	 GUID_DEVCLASS_FLOPPYDISK软盘驱动器
	 GUID_DEVCLASS_UNKNOWN未知设备
	 GUID_DEVCLASS_SCSIADAPTERSCSI 和 RAID 控制器
	 GUID_DEVCLASS_HDCIDE ATA/ATAPI 控制器
	 GUID_DEVCLASS_PORTS端口（COM 和 LPT）
	 GUID_DEVCLASS_MONITOR监视器
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
			FriendlyPortName = (std::string)charTemp;
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
			strPortDesc = (std::string)charTemp;
			Str_TrimSelf(strPortDesc);
		}
		
		i = 0;
		while(i ++ < 256){
			strTemp = portListInfo[i].strPortName;
			strTemp += ")";
			if ((portListInfo[i].strPortName.length() > 0) && (portListInfo[i].blFinished == 0)){
				if (FriendlyPortName.find(strTemp,0) != std::string::npos){
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
	HKEY   			hKey;
	std::string		strTemp;
	int32			i;
	
	i = 0;
	do{
		portListInfo[i].strDevPath = "";
		portListInfo[i].strPortName = "";
		portListInfo[i].strFriendlyName = "";
		portListInfo[i].strPortDesc = "";
		portListInfo[i].bUsbDevice = 0;
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
			strTemp = (std::string)charTemp;
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
			portListInfo[i].strDevPath = portListInfo[j].strDevPath;
			portListInfo[i].strPortName = portListInfo[j].strPortName;
			portListInfo[i].strFriendlyName = portListInfo[j].strFriendlyName;
			portListInfo[i].strPortDesc = portListInfo[j].strPortDesc;
			portListInfo[i].bUsbDevice = portListInfo[j].bUsbDevice;
			portListInfo[i].blFinished = portListInfo[j].blFinished;
			++ i;
		}
	}
	return(i);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
void IPCOMNAME::Init(IPCOMNAME *node,G_LOCK_VAILD blLock){
	node->Spin_InUse_set(blLock);
	node->typeID = PublicDevice_DEVID_NONE;
	node->strIPComName = "";
	node->portBaudrate = 0;
	node->TCPTypeOrFriendlyName = "";
	node->strUserDefineName = "";
	node->strShowName = "";
	node->blAvailable = 1;
	node->Spin_InUse_clr(blLock);
}
//------------------------------------------------------------------------------------------//
void IPCOMNAME::CreateShowName(IPCOMNAME *node,G_LOCK_VAILD blLock){
	std::string		strTemp;
	node->Spin_InUse_set(blLock);
	node->strShowName = "";
	if (node->typeID == PublicDevice_DEVID_APICOM){
		node->strShowName = node->strUserDefineName;
		if (node->strShowName.length() == 0){
			node->strShowName = node->TCPTypeOrFriendlyName;
			if (node->strShowName.length() == 0){
				node->strShowName = node->strIPComName;
				return;
			}
		}
		node->strShowName = node->strIPComName + "  ::  " + node->strShowName;
	}
	if (node->typeID == PublicDevice_DEVID_TCPClient){
		if (node->strUserDefineName.length() == 0){
			node->strShowName = "TCP  ::  ";
			node->strShowName += node->strIPComName;
			node->strShowName += "  :  ";
			node->strShowName += Str_IntToString(node->portBaudrate);
		}
		else{
			node->strShowName = "TCP  ::  " + node->strUserDefineName;
			node->strShowName += "  :  ";
			node->strShowName += Str_IntToString(node->portBaudrate);
		}
	}
	if (node->typeID == PublicDevice_DEVID_UDPClient){
		if (node->strUserDefineName.length() == 0){
			node->strShowName = "UDP  ::  ";
			node->strShowName += node->strIPComName;
			node->strShowName += "  :  ";
			node->strShowName += Str_IntToString(node->portBaudrate);
		}
		else{
			node->strShowName = "UDP  ::  " + node->strUserDefineName;
			node->strShowName += "  :  ";
			node->strShowName += Str_IntToString(node->portBaudrate);
		}
	}
	if (node->typeID == PublicDevice_DEVID_TCPServer){
		if (node->strUserDefineName.length() == 0){
			node->strShowName = "TCP SERVER";
			node->strShowName += "  :  ";
			node->strShowName += Str_IntToString(node->portBaudrate);
		}
		else{
			node->strShowName = "TCP SERVER :: " + node->strUserDefineName;
			node->strShowName += "  :  ";
			node->strShowName += Str_IntToString(node->portBaudrate);
		}
	}
	if (node->typeID == PublicDevice_DEVID_UDPServer){
		if (node->strUserDefineName.length() == 0){
			node->strShowName = "UDP SERVER";
			node->strShowName += "  :  ";
			node->strShowName += Str_IntToString(node->portBaudrate);
		}
		else{
			node->strShowName = "UDP SERVER :: " + node->strUserDefineName;
			node->strShowName += "  :  ";
			node->strShowName += Str_IntToString(node->portBaudrate);
		}
	}
	node->Spin_InUse_clr(blLock);
}
//------------------------------------------------------------------------------------------//
std::string IPCOMNAME::CreateStrV0_4(IPCOMNAME *node){
	//V0.4.11/25/2013
	//将IPCOMNAME
	//	[ipcom]
	//		typeID =
	//		strIPComName =
	//		portBaudrate =
	//		tcpTypeOrFriendlyName =
	//		strUserDefineName =
	//	[ipcom_end]
	
	std::string	strTemp,strResult;
	node->Spin_InUse_set();
	{
		strResult  =  "  [ipcom]\n";
		strResult += ("    typeID = " + Str_IntToString(node->typeID) + "\n");
		strResult += ("    strIPComName = " + node->strIPComName + "\n");
		strResult += ("    portBaudrate = " + Str_IntToString(node->portBaudrate) + "\n");
		strResult += ("    tcpTypeOrFriendlyName = " + node->TCPTypeOrFriendlyName + "\n");
		if (node->strUserDefineName.length() > 0)
			strResult += ("    strUserDefineName = " + node->strUserDefineName + "\n");
		strResult +=  "  [ipcom_end]\n";
	}
	node->Spin_InUse_clr();
	return(strResult);
}
//------------------------------------------------------------------------------------------//
std::string IPCOMNAME::CreateStrV0_1(IPCOMNAME *node){
	//V0.1 12/19/2012
	//IPCOMNAME (typeID,strIPComName,portBaudrate,TCPTypeOrFriendlyName,strUserDefineName)
	
	std::string	strTemp,strResult;
	node->Spin_InUse_set();
	{
		strResult = '(';
		
		strTemp = Str_IntToString(node->typeID);
		strResult += Str_ASCIIToHEX(strTemp,G_ESCAPE_OFF);
		strResult += ",";
		
		strResult += Str_ASCIIToHEX(node->strIPComName,G_ESCAPE_OFF);
		strResult += ",";
		
		strTemp = Str_IntToString(node->portBaudrate);
		strResult += Str_ASCIIToHEX(strTemp,G_ESCAPE_OFF);
		strResult += ",";
		
		strResult += Str_ASCIIToHEX(node->TCPTypeOrFriendlyName,G_ESCAPE_OFF);
		strResult += ",";
		
		strResult += Str_ASCIIToHEX(node->strUserDefineName,G_ESCAPE_OFF);
		
		strResult += ')';
	}
	node->Spin_InUse_clr();
	return(strResult);
}
// ------------------------------------------------------------------------------------------//
void IPCOMNAME::SetIPComV0_4(IPCOMNAME *node,std::string *strInput){
	//V0.4.11/25/2013
	//IPCOMNAME
	//	[ipcom]
	//		typeID =
	//		strIPComName =
	//		portBaudrate =
	//		tcpTypeOrFriendlyName =
	//		strUserDefineName =
	//	[ipcom_end]
	std::string		strLine,strItem;
	node->Spin_InUse_set();
	{
		Init(node,G_LOCK_OFF);
		while(strInput->length() > 0){
			strLine = Str_Trim(Str_ReadSubItem(strInput,"\n"));
			if (Str_LowerCase(strLine) == "[ipcom_end]")
				break;
			strItem = Str_LowerCase(Str_Trim(Str_ReadSubItem(&strLine,"=")));
			Str_TrimSelf(strLine);
			if (strItem == "typeid"){
				node->typeID = atoi(strLine.c_str());
			}
			else if (strItem == "stripcomname"){
				node->strIPComName = strLine;
			}
			if (strItem == "portbaudrate"){
				node->portBaudrate = atoi(strLine.c_str());
			}
			else if (strItem == "tcptypeorfriendlyname"){
				node->TCPTypeOrFriendlyName = strLine;
			}
			else if (strItem == "struserdefinename"){
				node->strUserDefineName = strLine;
			}
		}
		CreateShowName(node,G_LOCK_OFF);
	}
	node->Spin_InUse_clr();
}
// ------------------------------------------------------------------------------------------//
void IPCOMNAME::SetIPComV0_1(IPCOMNAME *node,std::string *strInput){
	//V0.1,12/19/2012
	//:(typeID,strIPComName,portBaudrate,TCPTypeOrFriendlyName,strUserDefineName)
	node->Spin_InUse_set();
	{
		Init(node,G_LOCK_OFF);
		node->typeID = atoi(Str_HEXToASCII(Str_ReadSubItem(strInput,",")).c_str());
		node->strIPComName = Str_HEXToASCII(Str_ReadSubItem(strInput,","));
		node->portBaudrate = atoi(Str_HEXToASCII(Str_ReadSubItem(strInput,",")).c_str());
		node->portBaudrate = 0;
		
		node->TCPTypeOrFriendlyName = Str_HEXToASCII(Str_ReadSubItem(strInput,","));
		node->strUserDefineName = Str_HEXToASCII(Str_ReadSubItem(strInput,","));
		CreateShowName(node,G_LOCK_OFF);
	}
	node->Spin_InUse_clr();
}
//------------------------------------------------------------------------------------------//
void IPCOMNAME::Copy(IPCOMNAME *node2,IPCOMNAME *node1,G_LOCK_VAILD blLock){
	node1->Spin_InUse_set(blLock);
	node2->Spin_InUse_set(blLock);
	node1->typeID = node2->typeID;
	node1->strIPComName = node2->strIPComName;
	node1->portBaudrate = node2->portBaudrate;
	node1->TCPTypeOrFriendlyName = node2->TCPTypeOrFriendlyName;
	node1->strUserDefineName = node2->strUserDefineName;
	node1->strShowName = node2->strShowName;
	node1->blAvailable = node2->blAvailable;
	node2->Spin_InUse_clr(blLock);
	node1->Spin_InUse_clr(blLock);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
std::string IPCOMLIST::CreateStrV0_4(void){
	//V0.4,11/25/2013
	//{(IPCOMNAME1)(IPCOMNAME2)}
	//	[ipcomlist]
	//		[ipcom]
	//		[ipcom_end]
	//		[ipcom]
	//		[ipcom_end]
	//	[ipcomlist_end]
	
	std::string	strResult;
	
	strResult = "[ipcomlist]\n";
	Spin_InUse_set();
	RTREE_LChildRChain_Traversal_LINE(IPCOMNAME,this,strResult += IPCOMNAME::CreateStrV0_4(operateNode_t));
	Spin_InUse_clr();
	strResult += "[ipcomlist_end]\n";
	
	return(strResult);
}
//------------------------------------------------------------------------------------------//
std::string IPCOMLIST::CreateStrV0_1(void){
	//V0.1,12/29/2012
	//{(IPCOMNAME1)(IPCOMNAME2)}
	
	std::string	strResult;
	
	strResult = '{';
	Spin_InUse_set();
	RTREE_LChildRChain_Traversal_LINE(IPCOMNAME,this,strResult += IPCOMNAME::CreateStrV0_1(operateNode_t));
	Spin_InUse_clr();
	strResult += '}';
	return(strResult);
}
//------------------------------------------------------------------------------------------//
void IPCOMLIST::SetIPComListV0_4(std::string *strInput){
	//V0.4,11/25/2013
	//{(IPCOMNAME1)(IPCOMNAME2)}
	//	[ipcomlist]
	//		[ipcom]
	//		[ipcom_end]
	//		[ipcom]
	//		[ipcom_end]
	//	[ipcomlist_end]
	
	IPCOMNAME		node;
	std::string		strLine,strItem;
	while(strInput->length() > 0){
		strLine = Str_LowerCase(Str_Trim(Str_ReadSubItem(strInput,"\n")));
		if (strLine == "[ipcomlist_end]")
			break;
		if (strLine == "[ipcom]"){
			IPCOMNAME::SetIPComV0_4(&node,strInput);
			AddWithCheck(&node);
		}
	}
}
//------------------------------------------------------------------------------------------//
void IPCOMLIST::SetIPComListV0_1(std::string *strInput){
	//V0.1,12/29/2012
	//{(IPCOMNAME1)(IPCOMNAME2)}
	
	IPCOMNAME		node;
	std::string		strResult;
	
	while(strInput->length() > 0){
		strResult = Str_Trim(Str_ReadSubItem(strInput,"|"));
		IPCOMNAME::SetIPComV0_1(&node,&strResult);
		AddWithCheck(&node);
	}
}
//------------------------------------------------------------------------------------------//
int32 NewNodeCheck(IPCOMNAME *nodeInList,IPCOMNAME *checkNode){
	int32 blChecked;
	blChecked = 0;
	if ((nodeInList->typeID == checkNode->typeID) && (Str_UpperCase(nodeInList->strIPComName) == Str_UpperCase(checkNode->strIPComName))){
		if (checkNode->typeID == PublicDevice_DEVID_APICOM){
			if (Str_UpperCase(nodeInList->TCPTypeOrFriendlyName) == Str_UpperCase(checkNode->TCPTypeOrFriendlyName)){
				blChecked = 1;
				nodeInList->blAvailable = 1;
			}
		}
		else if (nodeInList->portBaudrate == checkNode->portBaudrate){
			blChecked = 1;
		}
	}
	return(blChecked);
}
//------------------------------------------------------------------------------------------//
IPCOMNAME *IPCOMLIST::AddWithCheck(IPCOMNAME *newIPComName,G_LOCK_VAILD blLock){
	IPCOMNAME	*retNode;

	Spin_InUse_set(blLock);
	RTREE_LChildRChain_Find(IPCOMNAME,this,retNode,(NewNodeCheck(operateNode_t,newIPComName) != 0));
	
	if (retNode == nullptr){
		retNode = new IPCOMNAME;
		if (retNode != nullptr){
			IPCOMNAME::Copy(newIPComName,retNode,G_LOCK_OFF);
			AddNode(retNode);
		}
	}
	Spin_InUse_clr(blLock);
	return(retNode);
}
//------------------------------------------------------------------------------------------//
void IPCOMLIST::Refresh(void){
	PORTLIST	tPortlist;
	int32		i,num;
	IPCOMNAME 	node;
	
	Spin_InUse_set();
	RTREE_LChildRChain_Traversal_LINE(IPCOMNAME,this,
		operateNode_t->Spin_InUse_set();
		if (operateNode_t->typeID == PublicDevice_DEVID_APICOM)
			operateNode_t->blAvailable = 0;
		operateNode_t->Spin_InUse_clr();
	);
	
	num = tPortlist.DeviceEnumCommPort();
	i = 0;
	while (i++ < num){
		IPCOMNAME::Init(&node,G_LOCK_OFF);
		node.typeID = PublicDevice_DEVID_APICOM;
		node.strIPComName = tPortlist.portListInfo[i - 1].strPortName;
		node.portBaudrate = 115200;
		node.TCPTypeOrFriendlyName = tPortlist.portListInfo[i - 1].strFriendlyName;
		node.strUserDefineName = "";
		IPCOMNAME::CreateShowName(&node,G_LOCK_OFF);
		AddWithCheck(&node,G_LOCK_OFF);
	}
	Spin_InUse_clr();
}
//------------------------------------------------------------------------------------------//
void IPCOMLIST::Export(IPCOMLIST *cExportList){
	cExportList->DestroyAll();
	cExportList->Spin_InUse_set();
	Spin_InUse_set();
	RTREE_LChildRChain_Traversal_LINE(IPCOMNAME,this,
		operateNode_t->Spin_InUse_set();
		if (operateNode_t->blAvailable != 0)
			cExportList->AddWithCheck(operateNode_t,G_LOCK_OFF);
		operateNode_t->Spin_InUse_clr();
	);
	Spin_InUse_clr();
	cExportList->Spin_InUse_clr();
}
//------------------------------------------------------------------------------------------//
void IPCOMLIST::ExportCOM(IPCOMLIST *cExportList){
	cExportList->DestroyAll();
	cExportList->Spin_InUse_set();
	Spin_InUse_set();
	RTREE_LChildRChain_Traversal_LINE(IPCOMNAME,this,
		operateNode_t->Spin_InUse_set();
		if ((operateNode_t->blAvailable != 0) && (operateNode_t->typeID == PublicDevice_DEVID_APICOM))
			cExportList->AddWithCheck(operateNode_t,G_LOCK_OFF);
		operateNode_t->Spin_InUse_clr();
	);
	Spin_InUse_clr();
	cExportList->Spin_InUse_clr();
}
//------------------------------------------------------------------------------------------//
