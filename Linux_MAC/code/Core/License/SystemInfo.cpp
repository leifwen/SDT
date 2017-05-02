/*
 * Copyright (c) 2012-2014
 * All rights reserved.
 *
 * File Name: SystemInfo.cpp
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2013.10.22
 */
//------------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "SystemInfo.h"
#include "Comm_Convert.h"
//------------------------------------------------------------------------------------------//
#ifdef CommonDefH_VC
#include <WINSOCK2.H>
#include <Iphlpapi.h>
#include <stddef.h>
#include <winioctl.h>
#include <versionhelpers.h>
#pragma comment(lib,"Iphlpapi.lib")
//------------------------------------------------------------------------------------------//
#define  FILE_DEVICE_SCSI				(0x0000001b)
#define  IOCTL_SCSI_MINIPORT_IDENTIFY	((FILE_DEVICE_SCSI << 16) + 0x0501)
#define  IOCTL_SCSI_MINIPORT			(0x0004D008)  //  see NTDDSCSI.H for definition
//#define  IDENTIFY_BUFFER_SIZE			(512)
#define  SENDIDLENGTH  					(sizeof(SENDCMDOUTPARAMS) + IDENTIFY_BUFFER_SIZE)
#define  IDE_ATAPI_IDENTIFY				(0xA1)  //  Returns ID sector for ATAPI.
#define  IDE_ATA_IDENTIFY				(0xEC)  //  Returns ID sector for ATA.
#define  DFP_RECEIVE_DRIVE_DATA			(0x0007c088)

typedef struct _IDSECTOR{
	USHORT  wGenConfig;
	USHORT  wNumCyls;
	USHORT  wReserved;
	USHORT  wNumHeads;
	USHORT  wBytesPerTrack;
	USHORT  wBytesPerSector;
	USHORT  wSectorsPerTrack;
	USHORT  wVendorUnique[3];
	CHAR    sSerialNumber[20];
	USHORT  wBufferType;
	USHORT  wBufferSize;
	USHORT  wECCSize;
	CHAR    sFirmwareRev[8];
	CHAR    sModelNumber[40];
	USHORT  wMoreVendorUnique;
	USHORT  wDoubleWordIO;
	USHORT  wCapabilities;
	USHORT  wReserved1;
	USHORT  wPIOTiming;
	USHORT  wDMATiming;
	USHORT  wBS;
	USHORT  wNumCurrentCyls;
	USHORT  wNumCurrentHeads;
	USHORT  wNumCurrentSectorsPerTrack;
	ULONG   ulCurrentSectorCapacity;
	USHORT  wMultSectorStuff;
	ULONG   ulTotalAddressableSectors;
	USHORT  wSingleWordDMA;
	USHORT  wMultiWordDMA;
	BYTE    bReserved[128];
}IDSECTOR, *PIDSECTOR;

typedef struct _SRB_IO_CONTROL{
	ULONG HeaderLength;
	UCHAR Signature[8];
	ULONG Timeout;
	ULONG ControlCode;
	ULONG ReturnCode;
	ULONG Length;
}SRB_IO_CONTROL, *PSRB_IO_CONTROL;

typedef struct _GETVERSIONOUTPARAMS{
	BYTE bVersion;      // Binary driver version.
	BYTE bRevision;     // Binary driver revision.
	BYTE bReserved;     // Not used.
	BYTE bIDEDeviceMap; // Bit map of IDE devices.
	DWORD fCapabilities; // Bit mask of driver capabilities.
	DWORD dwReserved[4]; // For future use.
}GETVERSIONOUTPARAMS, *PGETVERSIONOUTPARAMS, *LPGETVERSIONOUTPARAMS;

#define IOCTL_STORAGE_QUERY_PROPERTY   CTL_CODE(IOCTL_STORAGE_BASE, 0x0500, METHOD_BUFFERED, FILE_ANY_ACCESS)

//////////////////////////////////////////////////////////////////////

//结构定义
typedef struct _UNICODE_STRING{
	USHORT  Length;//长度
	USHORT  MaximumLength;//最大长度
	PWSTR  Buffer;//缓存指针
}UNICODE_STRING,*PUNICODE_STRING;

typedef struct _OBJECT_ATTRIBUTES{
	ULONG Length;//长度 18h
	HANDLE RootDirectory;//  00000000
	PUNICODE_STRING ObjectName;//指向对象名的指针
	ULONG Attributes;//对象属性00000040h
	PVOID SecurityDescriptor;        // Points to type SECURITY_DESCRIPTOR，0
	PVOID SecurityQualityOfService;  // Points to type SECURITY_QUALITY_OF_SERVICE，0
}OBJECT_ATTRIBUTES;
typedef OBJECT_ATTRIBUTES *POBJECT_ATTRIBUTES;

//函数指针变量类型
typedef DWORD  (__stdcall *ZWOS )( PHANDLE,ACCESS_MASK,POBJECT_ATTRIBUTES);
typedef DWORD  (__stdcall *ZWMV )( HANDLE,HANDLE,PVOID,ULONG,ULONG,PLARGE_INTEGER,PSIZE_T,DWORD,ULONG,ULONG);
typedef DWORD  (__stdcall *ZWUMV )( HANDLE,PVOID);
//------------------------------------------------------------------------------------------//
int32 WinNTHDSerialNumAsPhysicalReadWithAdminRights		(BYTE* dwSerial,UINT* puSerialLen,UINT uMaxSerialLen);
int32 WinNTHDSerialNumAsScsiReadWithAdminRights			(BYTE* dwSerial,UINT* puSerialLen,UINT uMaxSerialLen);
int32 WinNTHDSerialNumAsPhysicalReadWithZeroRights		(BYTE* dwSerial,UINT* puSerialLen,UINT uMaxSerialLen);
BOOL DoIdentify(HANDLE hPhysicalDriveIOCTL,PSENDCMDINPARAMS pSCIP,PSENDCMDOUTPARAMS pSCOP,BYTE bIDCmd,BYTE bDriveNum,PDWORD lpcbBytesReturned);
UINT FindAwardBios	(BYTE** ppBiosAddr);
UINT FindAmiBios	(BYTE** ppBiosAddr);
UINT FindPhoenixBios(BYTE** ppBiosAddr);
//------------------------------------------------------------------------------------------//
int32 WinNTHDSerialNumAsPhysicalReadWithAdminRights(BYTE* dwSerial,UINT* puSerialLen,UINT uMaxSerialLen){
#define  DFP_GET_VERSION		(0x00074080)
	bool bInfoLoaded = false;
	
	for(UINT uDrive = 0;uDrive < 4;++ uDrive){
		HANDLE hPhysicalDriveIOCTL;
		//  Try to get a handle to PhysicalDrive IOCTL, report failure and exit if can't.
		
		CString		szDriveName;
		szDriveName = _T("\\\\.\\PhysicalDrive");
		szDriveName += Str_ANSIToUnicode(Str_ToString(uDrive)).c_str();
		//  Windows NT, Windows 2000, must have admin rights
		hPhysicalDriveIOCTL = CreateFile(szDriveName,
										 GENERIC_READ | GENERIC_WRITE,
										 FILE_SHARE_READ | FILE_SHARE_WRITE,nullptr,
										 OPEN_EXISTING,0,nullptr);
		
		if( hPhysicalDriveIOCTL != INVALID_HANDLE_VALUE ){
			GETVERSIONOUTPARAMS VersionParams = { 0 };
			DWORD		cbBytesReturned = 0;
			
			// Get the version, etc of PhysicalDrive IOCTL
			if(DeviceIoControl(hPhysicalDriveIOCTL,DFP_GET_VERSION,nullptr,0,&VersionParams,sizeof(GETVERSIONOUTPARAMS),&cbBytesReturned,nullptr)){
				// If there is a IDE device at number "i" issue commands to the device
				if(VersionParams.bIDEDeviceMap != 0){
					BYTE	bIDCmd;   // IDE or ATAPI IDENTIFY cmd
					SENDCMDINPARAMS  scip = { 0 };
					
					// Now, get the ID sector for all IDE devices in the system.
					// If the device is ATAPI use the IDE_ATAPI_IDENTIFY command,
					// otherwise use the IDE_ATA_IDENTIFY command
					bIDCmd = ( VersionParams.bIDEDeviceMap >> uDrive & 0x10 ) ? IDE_ATAPI_IDENTIFY : IDE_ATA_IDENTIFY;
					BYTE IdOutCmd[sizeof( SENDCMDOUTPARAMS ) + IDENTIFY_BUFFER_SIZE - 1] = { 0 };
					
					if(DoIdentify(hPhysicalDriveIOCTL,&scip,(PSENDCMDOUTPARAMS)&IdOutCmd,(BYTE)bIDCmd,(BYTE)uDrive,&cbBytesReturned)){
						if(*puSerialLen + 20U <= uMaxSerialLen ){
							CopyMemory(dwSerial + * puSerialLen,((USHORT*)(((PSENDCMDOUTPARAMS)IdOutCmd)->bBuffer)) + 10,20);  // 序列号
							// Cut off the trailing blanks
							UINT i;
							for(i = 20; i != 0U && ' ' == dwSerial[* puSerialLen + i - 1]; -- i );
							*puSerialLen += i;
							CopyMemory(dwSerial + *puSerialLen,((USHORT*)(((PSENDCMDOUTPARAMS)IdOutCmd)->bBuffer)) + 27, 40); // 型号
							// Cut off the trailing blanks
							for(i = 40;i != 0U && ' ' == dwSerial[* puSerialLen + i - 1]; -- i );
							* puSerialLen += i;
							bInfoLoaded = TRUE;
						}
						else{
							CloseHandle(hPhysicalDriveIOCTL);
							return bInfoLoaded;
						}
					}
				}
			}
			CloseHandle(hPhysicalDriveIOCTL);
		}
	}
	return bInfoLoaded;
}
//------------------------------------------------------------------------------------------//
int32 WinNTHDSerialNumAsScsiReadWithAdminRights(BYTE* dwSerial,UINT* puSerialLen,UINT uMaxSerialLen){
	bool bInfoLoaded = false;
	
	for(int iController = 0;iController < 2;++ iController){
		HANDLE hScsiDriveIOCTL;
		//  Try to get a handle to PhysicalDrive IOCTL, report failure
		//  and exit if can't.
		CString		szDriveName;
		szDriveName = _T("\\\\.\\Scsi");
		szDriveName += Str_ANSIToUnicode(Str_ToString(iController)).c_str();
		szDriveName += _T(":");
		//  Windows NT, Windows 2000, any rights should do
		hScsiDriveIOCTL = CreateFile(szDriveName,
									 GENERIC_READ | GENERIC_WRITE,
									 FILE_SHARE_READ | FILE_SHARE_WRITE,nullptr,
									 OPEN_EXISTING,0,nullptr);
		
		// if (hScsiDriveIOCTL == INVALID_HANDLE_VALUE)
		//    printf ("Unable to open SCSI controller %d, error code: 0x%lX/n",
		//            controller, GetLastError ());
		
		if( hScsiDriveIOCTL != INVALID_HANDLE_VALUE ){
			int iDrive;
			for(iDrive = 0;iDrive < 2;++ iDrive){
				char szBuffer[sizeof( SRB_IO_CONTROL ) + SENDIDLENGTH] = { 0 };
				
				SRB_IO_CONTROL* p = (SRB_IO_CONTROL*)szBuffer;
				SENDCMDINPARAMS* pin = (SENDCMDINPARAMS*)(szBuffer + sizeof(SRB_IO_CONTROL));
				DWORD dwResult;
				
				p->HeaderLength = sizeof(SRB_IO_CONTROL);
				p->Timeout = 10000;
				p->Length = SENDIDLENGTH;
				p->ControlCode = IOCTL_SCSI_MINIPORT_IDENTIFY;
				strncpy_s((char*)p->Signature,sizeof(p->Signature),"SCSIDISK",8);
				
				pin->irDriveRegs.bCommandReg = IDE_ATA_IDENTIFY;
				pin->bDriveNumber = iDrive;
				
				if( DeviceIoControl(hScsiDriveIOCTL,IOCTL_SCSI_MINIPORT,szBuffer,sizeof(SRB_IO_CONTROL) + sizeof(SENDCMDINPARAMS) - 1,szBuffer,sizeof(SRB_IO_CONTROL) + SENDIDLENGTH,&dwResult,nullptr)){
					SENDCMDOUTPARAMS* pOut = ( SENDCMDOUTPARAMS* )( szBuffer + sizeof( SRB_IO_CONTROL ) );
					IDSECTOR* pId = (IDSECTOR*)(pOut->bBuffer);
					if( pId->sModelNumber[0] ){
						if( * puSerialLen + 20U <= uMaxSerialLen){
							// 序列号
							CopyMemory(dwSerial + *puSerialLen,((USHORT*)pId) + 10, 20);
							// Cut off the trailing blanks
							UINT i;
							for(i = 20;i != 0U && ' ' == dwSerial[*puSerialLen + i - 1];-- i);
							*puSerialLen += i;
							// 型号
							CopyMemory(dwSerial + *puSerialLen,((USHORT*)pId) + 27, 40);
							// Cut off the trailing blanks
							for(i = 40; i != 0U && ' ' == dwSerial[*puSerialLen + i - 1]; -- i);
							*puSerialLen += i;
							bInfoLoaded = TRUE;
						}
						else{
							CloseHandle(hScsiDriveIOCTL);
							return bInfoLoaded;
						}
					}
				}
			}
			CloseHandle(hScsiDriveIOCTL);
		}
	}
	return bInfoLoaded;
}
//------------------------------------------------------------------------------------------//
//  function to decode the serial numbers of IDE hard drives
//  using the IOCTL_STORAGE_QUERY_PROPERTY command
char * flipAndCodeBytes (const char * str,int pos,int flip,char * buf){
	int i;
	int j = 0;
	int k = 0;
	
	buf [0] = '\0';
	if (pos <= 0)
		return buf;
	
	if ( ! j){
		char p = 0;
		
		// First try to gather all characters representing hex digits only.
		j = 1;
		k = 0;
		buf[k] = 0;
		for (i = pos; j && str[i] != '\0'; ++i){
			char c = tolower((unsigned char)str[i]);
			
			if (isspace((unsigned char)c))
				c = '0';
			
			++p;
			buf[k] <<= 4;
			
			if (c >= '0' && c <= '9'){
				buf[k] |= (unsigned char) (c - '0');
			}
			else if (c >= 'a' && c <= 'f'){
				buf[k] |= (unsigned char) (c - 'a' + 10);
			}
			else{
				j = 0;
				break;
			}
			
			if (p == 2){
				if (buf[k] != '\0' && ! isprint((unsigned char)buf[k])){
					j = 0;
					break;
				}
				++k;
				p = 0;
				buf[k] = 0;
			}
		}
	}
	
	if ( ! j){
		// There are non-digit characters, gather them as is.
		j = 1;
		k = 0;
		for (i = pos; j && str[i] != '\0'; ++i){
			char c = str[i];
			if ( ! isprint((unsigned char)c)){
				j = 0;
				break;
			}
			buf[k++] = c;
		}
	}
	
	if ( ! j){
		// The characters are not there or are not printable.
		k = 0;
	}
	
	buf[k] = '\0';
	
	if (flip){
		// Flip adjacent characters
		for (j = 0; j < k; j += 2){
			char t = buf[j];
			buf[j] = buf[j + 1];
			buf[j + 1] = t;
		}
	}
	// Trim any beginning and end space
	i = j = -1;
	for (k = 0; buf[k] != '\0'; ++k){
		if (! isspace(buf[k])){
			if (i < 0)
				i = k;
			j = k;
		}
	}
	
	if ((i >= 0) && (j >= 0)){
		for (k = i; (k <= j) && (buf[k] != '\0'); ++k)
			buf[k - i] = buf[k];
		buf[k - i] = '\0';
	}
	return buf;
}
//------------------------------------------------------------------------------------------//
int32 WinNTHDSerialNumAsPhysicalReadWithZeroRights(BYTE* dwSerial,UINT* puSerialLen,UINT uMaxSerialLen){
	BOOL done = FALSE;
	int drive = 0;
	
	for (drive = 0; drive < 4; drive++){
		HANDLE hPhysicalDriveIOCTL = 0;
		
		//  Try to get a handle to PhysicalDrive IOCTL, report failure
		//  and exit if can't.
		CString		szDriveName;
		szDriveName = _T("\\\\.\\PhysicalDrive");
		szDriveName += Str_ANSIToUnicode(Str_ToString(drive)).c_str();
		
		//  Windows NT, Windows 2000, Windows XP - admin rights not required
		hPhysicalDriveIOCTL = CreateFile (szDriveName, 0,
										  FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr,
										  OPEN_EXISTING, 0, nullptr);
		if (hPhysicalDriveIOCTL != INVALID_HANDLE_VALUE){
			STORAGE_PROPERTY_QUERY query;
			DWORD cbBytesReturned = 0;
			char local_buffer [10000];
			
			memset ((void *) & query, 0, sizeof (query));
			query.PropertyId = StorageDeviceProperty;
			query.QueryType = PropertyStandardQuery;
			
			memset (local_buffer, 0, sizeof (local_buffer));
			
			if (DeviceIoControl (hPhysicalDriveIOCTL,IOCTL_STORAGE_QUERY_PROPERTY,&query,sizeof(query),&local_buffer[0],sizeof (local_buffer),&cbBytesReturned,nullptr)){
				STORAGE_DEVICE_DESCRIPTOR * descrip = (STORAGE_DEVICE_DESCRIPTOR *) & local_buffer;
				char serialNumber [1000];
				char modelNumber [1000];
				char vendorId [1000];
				char productRevision [1000];
				
				flipAndCodeBytes (local_buffer,descrip->VendorIdOffset,0,vendorId);
				flipAndCodeBytes (local_buffer,descrip->ProductIdOffset,0,modelNumber);
				flipAndCodeBytes (local_buffer,descrip->ProductRevisionOffset,0,productRevision);
				flipAndCodeBytes (local_buffer,descrip->SerialNumberOffset,1,serialNumber);
				
				if (isalnum ((unsigned char)serialNumber [0]) || isalnum ((unsigned char)serialNumber [19])){
					//  serial number must be alphanumeric (but there can be leading spaces on IBM drives)
					CopyMemory(dwSerial + *puSerialLen,serialNumber, strlen(serialNumber));
					*puSerialLen += strlen(serialNumber);
					CopyMemory(dwSerial + *puSerialLen,modelNumber, strlen(modelNumber));
					*puSerialLen += strlen(modelNumber);
					done = TRUE;
				}
				// Get the disk drive geometry.
				memset (local_buffer, 0, sizeof(local_buffer));
				if (DeviceIoControl (hPhysicalDriveIOCTL,IOCTL_DISK_GET_DRIVE_GEOMETRY_EX,nullptr,0,&local_buffer[0],sizeof(local_buffer),&cbBytesReturned,nullptr)){
					DISK_GEOMETRY_EX * geom = (DISK_GEOMETRY_EX * ) & local_buffer [0];
					int fixed = (geom->Geometry.MediaType == FixedMedia);
					__int64 size = geom->DiskSize.QuadPart;
				}
			}
			else{
				DWORD err = GetLastError ();
			}
			CloseHandle (hPhysicalDriveIOCTL);
		}
	}
	return done;
}
//------------------------------------------------------------------------------------------//
BOOL DoIdentify(HANDLE hPhysicalDriveIOCTL, PSENDCMDINPARAMS pSCIP,
				PSENDCMDOUTPARAMS pSCOP, BYTE bIDCmd, BYTE bDriveNum,
				PDWORD lpcbBytesReturned ){
	// Set up data structures for IDENTIFY command.
	pSCIP->cBufferSize                  = IDENTIFY_BUFFER_SIZE;
	pSCIP->irDriveRegs.bFeaturesReg     = 0;
	pSCIP->irDriveRegs.bSectorCountReg  = 1;
	pSCIP->irDriveRegs.bSectorNumberReg = 1;
	pSCIP->irDriveRegs.bCylLowReg       = 0;
	pSCIP->irDriveRegs.bCylHighReg      = 0;
	
	// calc the drive number.
	pSCIP->irDriveRegs.bDriveHeadReg = 0xA0 | ( ( bDriveNum & 1 ) << 4 );
	
	// The command can either be IDE identify or ATAPI identify.
	pSCIP->irDriveRegs.bCommandReg = bIDCmd;
	pSCIP->bDriveNumber = bDriveNum;
	pSCIP->cBufferSize = IDENTIFY_BUFFER_SIZE;
	
	return DeviceIoControl( hPhysicalDriveIOCTL, DFP_RECEIVE_DRIVE_DATA,
						   ( LPVOID ) pSCIP,
						   sizeof( SENDCMDINPARAMS ) - 1,
						   ( LPVOID ) pSCOP,
						   sizeof( SENDCMDOUTPARAMS ) + IDENTIFY_BUFFER_SIZE - 1,
						   lpcbBytesReturned, nullptr );
}
//------------------------------------------------------------------------------------------//
UINT FindAwardBios(BYTE** ppBiosAddr){
	BYTE* pBiosAddr = * ppBiosAddr + 0xEC71;
	
	BYTE szBiosData[128];
	CopyMemory(szBiosData,pBiosAddr,127);
	szBiosData[127] = 0;
	
	int iLen = strlen((char*)szBiosData);
	if(iLen > 0 && iLen < 128){
		//AWard:         07/08/2002-i845G-ITE8712-JF69VD0CC-00
		//Phoenix-Award: 03/12/2002-sis645-p4s333
		if(szBiosData[2] == '/' && szBiosData[5] == '/'){
			BYTE* p = szBiosData;
			while(*p)	{
				if(*p < ' ' || *p >= 127)
					break;
				++ p;
			}
			if(*p == 0){
				*ppBiosAddr = pBiosAddr;
				return (UINT)iLen;
			}
		}
	}
	return 0;
}
//------------------------------------------------------------------------------------------//
UINT FindAmiBios( BYTE** ppBiosAddr){
	BYTE* pBiosAddr = * ppBiosAddr + 0xF478;
	
	BYTE szBiosData[128];
	CopyMemory(szBiosData,pBiosAddr,127);
	szBiosData[127] = 0;
	
	int iLen = strlen((char*)szBiosData);
	if(iLen > 0 && iLen < 128){
		// Example: "AMI: 51-2300-000000-00101111-030199-"
		if(szBiosData[2] == '-' && szBiosData[7] == '-'){
			BYTE* p = szBiosData;
			while(*p){
				if(*p < ' ' || *p >= 127 )
					break;
				++ p;
			}
			if(*p == 0){
				*ppBiosAddr = pBiosAddr;
				return ( UINT )iLen;
			}
		}
	}
	return 0;
}
//------------------------------------------------------------------------------------------//
UINT FindPhoenixBios(BYTE** ppBiosAddr){
	UINT uOffset[3] = {0x6577,0x7196,0x7550};
	for(UINT i = 0;i < 3;++ i){
		BYTE *pBiosAddr = *ppBiosAddr + uOffset[i];
		
		BYTE szBiosData[128];
		CopyMemory(szBiosData,pBiosAddr,127);
		szBiosData[127] = 0;
		int iLen = strlen((char*)szBiosData);
		if(iLen > 0 && iLen < 128){
			// Example: Phoenix "NITELT0.86B.0044.P11.9910111055"
			if(szBiosData[7] == '.' && szBiosData[11] == '.'){
				BYTE* p = szBiosData;
				while(*p){
					if(*p < ' ' || *p >= 127)
						break;
					++ p;
				}
				if(*p == 0){
					*ppBiosAddr = pBiosAddr;
					return ( UINT )iLen;
				}
			}
		}
	}
	return 0;
}
//------------------------------------------------------------------------------------------//
void APIComSignature_GetNetCardInfo(STDSTR *stringData){
	unsigned long stSize;
	int32 nRel,netCardNum,IPnumPerNetCard;
	
	PIP_ADAPTER_INFO pIpAdapterInfo = new IP_ADAPTER_INFO();//PIP_ADAPTER_INFO结构体指针存储本机网卡信息
	stSize = sizeof(IP_ADAPTER_INFO);												//得到结构体大小,用于GetAdaptersInfo参数
	nRel = GetAdaptersInfo(pIpAdapterInfo,&stSize);					//调用GetAdaptersInfo函数,填充pIpAdapterInfo指针变量;其中stSize参数既是一个输入量也是一个输出量
	netCardNum = 0;//记录网卡数量
	IPnumPerNetCard = 0;//记录每张网卡上的IP地址数量
	if (nRel == ERROR_BUFFER_OVERFLOW){
		//如果函数返回的是ERROR_BUFFER_OVERFLOW
		//则说明GetAdaptersInfo参数传递的内存空间不够,同时其传出stSize,表示需要的空间大小
		//这也是说明为什么stSize既是一个输入量也是一个输出量
		//释放原来的内存空间
		delete pIpAdapterInfo;//重新申请内存空间用来存储所有网卡信息
		pIpAdapterInfo = (PIP_ADAPTER_INFO) new BYTE[stSize];
		nRel = GetAdaptersInfo(pIpAdapterInfo,&stSize);//再次调用GetAdaptersInfo函数,填充pIpAdapterInfo指针变量
	}
	if (nRel == ERROR_SUCCESS){//输出网卡信息,可能有多网卡,因此通过循环去判断
		*stringData = "----------------------------------------------------------------------\n";
		*stringData += "NetCard Detail Information:\n";
		*stringData += "----------------------------------------------------------------------\n";
		while (pIpAdapterInfo){
			*stringData += "Number               : ";
			*stringData += Str_ToString(++ netCardNum);
			*stringData += "\n";
			*stringData += "Name                 : ";
			*stringData += pIpAdapterInfo->AdapterName;
			*stringData += "\n";
			*stringData += "Description          : ";
			*stringData += pIpAdapterInfo->Description;
			*stringData += "\n";
			*stringData += "Type                 : ";
			switch(pIpAdapterInfo->Type){
				case MIB_IF_TYPE_OTHER:
					*stringData += "OTHER\n";
					break;
				case MIB_IF_TYPE_ETHERNET:
					*stringData += "ETHERNET\n";
					break;
				case MIB_IF_TYPE_TOKENRING:
					*stringData += "TOKENRING\n";
					break;
				case MIB_IF_TYPE_FDDI:
					*stringData += "FDDI\n";
					break;
				case MIB_IF_TYPE_PPP:
					*stringData += "PPP\n";
					break;
				case MIB_IF_TYPE_LOOPBACK:
					*stringData += "LOOPBACK\n";
					break;
				case MIB_IF_TYPE_SLIP:
					*stringData += "SLIP\n";
					break;
				default:
					*stringData += "\n";
					break;
			}
			*stringData += "MAC Address          : ";
			for (DWORD i = 0; i < pIpAdapterInfo->AddressLength; i++){
				*stringData += Str_RTrim(Str_CharToHEXStr(&pIpAdapterInfo->Address[i],1,G_SPACE_OFF));
				if (i < pIpAdapterInfo->AddressLength - 1)
					*stringData += '-';
			}
			//*stringData += "\r\nIP Address on NetCard:\n";
			*stringData += "\n\n";
			IP_ADDR_STRING *pIpAddrString =&(pIpAdapterInfo->IpAddressList);
			IPnumPerNetCard = 0;
			do{//可能网卡有多IP,因此通过循环去判断
				*stringData += "IPv4 Address Number  : ";
				*stringData += Str_ToString(++IPnumPerNetCard);
				*stringData += "\n";
				*stringData += "IPv4 Address         : ";
				*stringData += pIpAddrString->IpAddress.String;
				*stringData += "\n";
				*stringData += "IPv4 Subnet Mask     : ";
				*stringData += pIpAddrString->IpMask.String;
				*stringData += "\n";
				*stringData += "IPv4 Default Gateway : ";
				*stringData += pIpAdapterInfo->GatewayList.IpAddress.String;
				*stringData += "\n";
				pIpAddrString = pIpAddrString->Next;
			}while(pIpAddrString);
			pIpAdapterInfo = pIpAdapterInfo->Next;
			*stringData += "----------------------------------------------------------------------\n";
		}
	}
	if (pIpAdapterInfo != nullptr)
		delete []pIpAdapterInfo;
}
//------------------------------------------------------------------------------------------//
void APIComSignature_GetHardiskInfo(STDSTR *stringData){
	// 硬盘序列号，注意：有的硬盘没有序列号,需要管理员权限
	BYTE szSystemInfo[4096]; // 在程序执行完毕后，此处存储取得的系统特征码
	UINT uSystemInfoLen = 0; // 在程序执行完毕后，此处存储取得的系统特征码的长度
 
	*stringData = "";
	if (IsWindowsXPOrGreater()){
		if (!WinNTHDSerialNumAsPhysicalReadWithAdminRights(szSystemInfo, &uSystemInfoLen, 1024)){
			if (!WinNTHDSerialNumAsScsiReadWithAdminRights(szSystemInfo, &uSystemInfoLen, 1024))
				WinNTHDSerialNumAsPhysicalReadWithZeroRights(szSystemInfo, &uSystemInfoLen, 1024);
		}
		*stringData = Str_CharToASCIIStr(szSystemInfo, uSystemInfoLen, G_ESCAPE_OFF);
	}
}
//------------------------------------------------------------------------------------------//
void APIComSignature_GetBOISInfo(STDSTR *stringData){//读不到
	// BIOS 编号，支持 AMI, AWARD, PHOENIX
	SIZE_T ssize;
	BYTE szSystemInfo[4096]; // 在程序执行完毕后，此处存储取得的系统特征码
	UINT uSystemInfoLen = 0; // 在程序执行完毕后，此处存储取得的系统特征码的长度
	LARGE_INTEGER so;
	so.LowPart = 0x000f0000;
	so.HighPart = 0x00000000;
	ssize = 0xffff;
	wchar_t strPH[30] = L"\\device\\physicalmemory";
	
	DWORD ba=0;
	
	UNICODE_STRING struniph;
	struniph.Buffer = strPH;
	struniph.Length = 0x2c;
	struniph.MaximumLength = 0x2e;
	
	OBJECT_ATTRIBUTES obj_ar;
	obj_ar.Attributes = 64;
	obj_ar.Length = 24;
	obj_ar.ObjectName = &struniph;
	obj_ar.RootDirectory = 0;
	obj_ar.SecurityDescriptor = 0;
	obj_ar.SecurityQualityOfService = 0;
	
	HMODULE hinstLib = LoadLibrary(_T("ntdll.dll"));
	if (hinstLib == nullptr)
		return;
	ZWOS ZWopenS=(ZWOS)GetProcAddress(hinstLib,"ZwOpenSection");
	ZWMV ZWmapV=(ZWMV)GetProcAddress(hinstLib,"ZwMapViewOfSection");
	ZWUMV ZWunmapV=(ZWUMV)GetProcAddress(hinstLib,"ZwUnmapViewOfSection");
	
	
	//调用函数，对物理内存进行映射
	HANDLE hSection;
	
	ZWopenS(&hSection,4,&obj_ar);
	if (hSection == nullptr){
		*stringData = "ZWopenS fail";
		return;
	}
	*stringData = "ZWopenS ok";
	ZWmapV(	( HANDLE )hSection,   //打开Section时得到的句柄
					( HANDLE )0xFFFFFFFF, //将要映射进程的句柄，
					&ba,                  //映射的基址
					0,
					0xFFFF,               //分配的大小
					&so,                  //物理内存的地址
					&ssize,               //指向读取内存块大小的指针
					1,                    //子进程的可继承性设定
					0,                    //分配类型
					2);                   //保护类型
	{
		//执行后会在当前进程的空间开辟一段64k的空间，并把f000:0000到f000:ffff处的内容映射到这里
		//映射的基址由ba返回,如果映射不再有用,应该用ZwUnmapViewOfSection断开映射
		BYTE* pBiosSerial = ( BYTE* )ba;
		UINT uBiosSerialLen = FindAwardBios( &pBiosSerial );
		if( uBiosSerialLen == 0U ){
			*stringData = "no AwardBios\n";
			uBiosSerialLen = FindAmiBios( &pBiosSerial );
			if( uBiosSerialLen == 0U )
				*stringData += "no AmiBios\n";
			uBiosSerialLen = FindPhoenixBios( &pBiosSerial );
			if( uBiosSerialLen == 0U )
				*stringData += "no PhoenixBios\n";
		}
		if( uBiosSerialLen != 0U ){
			CopyMemory(szSystemInfo + uSystemInfoLen,pBiosSerial,uBiosSerialLen);
			uSystemInfoLen += uBiosSerialLen;
		}
		ZWunmapV((HANDLE)0xFFFFFFFF,(void*)ba);
	}
	*stringData += Str_CharToHEXStr(szSystemInfo,uSystemInfoLen,G_SPACE_OFF);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
__int64 WinNTHDSerialNumAsPhysicalReadWithZeroRights(int driveNo,BYTE* dwSerial,UINT* puSerialLen,UINT uMaxSerialLen){
	BOOL		done = FALSE;
	HANDLE		hPhysicalDriveIOCTL = 0;
	CString		szDriveName;
	__int64		size = 0;
	
	szDriveName = _T("\\\\.\\PhysicalDrive");
	szDriveName += Str_ANSIToUnicode(Str_ToString(driveNo)).c_str();
	
	//  Windows NT, Windows 2000, Windows XP - admin rights not required
	hPhysicalDriveIOCTL = CreateFile (szDriveName, 0,
									  FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr,
									  OPEN_EXISTING, 0, nullptr);
	if (hPhysicalDriveIOCTL != INVALID_HANDLE_VALUE){
		STORAGE_PROPERTY_QUERY query;
		DWORD cbBytesReturned = 0;
		char local_buffer [10000];
		
		memset ((void *) & query, 0, sizeof (query));
		query.PropertyId = StorageDeviceProperty;
		query.QueryType = PropertyStandardQuery;
		
		memset (local_buffer, 0, sizeof (local_buffer));
		
		if (DeviceIoControl (hPhysicalDriveIOCTL,IOCTL_STORAGE_QUERY_PROPERTY,&query,sizeof(query),&local_buffer[0],sizeof (local_buffer),&cbBytesReturned,nullptr)){
			STORAGE_DEVICE_DESCRIPTOR * descrip = (STORAGE_DEVICE_DESCRIPTOR *) & local_buffer;
			char serialNumber[1000];
			char modelNumber[1000];
			char vendorId[1000];
			char productRevision[1000];
			
			flipAndCodeBytes (local_buffer,descrip->VendorIdOffset,0,vendorId);
			flipAndCodeBytes (local_buffer,descrip->ProductIdOffset,0,modelNumber);
			flipAndCodeBytes (local_buffer,descrip->ProductRevisionOffset,0,productRevision);
			flipAndCodeBytes (local_buffer,descrip->SerialNumberOffset,1,serialNumber);
			if (*puSerialLen + strlen(vendorId) + strlen(modelNumber) + strlen(productRevision) + strlen(serialNumber) < uMaxSerialLen){
				CopyMemory(dwSerial + *puSerialLen,vendorId, strlen(vendorId));
				*puSerialLen += strlen(vendorId);
				CopyMemory(dwSerial + *puSerialLen,modelNumber, strlen(modelNumber));
				*puSerialLen += strlen(modelNumber);
				CopyMemory(dwSerial + *puSerialLen,serialNumber, strlen(serialNumber));
				*puSerialLen += strlen(serialNumber);
				CopyMemory(dwSerial + *puSerialLen,productRevision, strlen(productRevision));
				*puSerialLen += strlen(productRevision);
			}
			// Get the disk drive geometry.
			memset (local_buffer, 0, sizeof(local_buffer));
			if (DeviceIoControl (hPhysicalDriveIOCTL,IOCTL_DISK_GET_DRIVE_GEOMETRY_EX,nullptr,0,&local_buffer[0],sizeof(local_buffer),&cbBytesReturned,nullptr)){
				DISK_GEOMETRY_EX * geom = (DISK_GEOMETRY_EX * ) & local_buffer [0];
				int fixed = (geom->Geometry.MediaType == FixedMedia);
				size = geom->DiskSize.QuadPart;
			}
			char tmpstr[256];
			memset(tmpstr,'\0',sizeof(tmpstr));
			_i64toa_s(size,tmpstr,256,10);
			if (*puSerialLen + strlen(tmpstr) < uMaxSerialLen){
				CopyMemory(dwSerial + *puSerialLen,tmpstr, strlen(tmpstr));
				*puSerialLen += strlen(tmpstr);
			}
		}
		else{
			DWORD err = GetLastError ();
		}
		CloseHandle (hPhysicalDriveIOCTL);
	}
	return(size);
}
//------------------------------------------------------------------------------------------//
void SystemInfo_GetHardiskInfo(STDSTR *stringData){
	BYTE szSystemInfo[4096]; // 在程序执行完毕后，此处存储取得的系统特征码
	UINT uSystemInfoLen = 0; // 在程序执行完毕后，此处存储取得的系统特征码的长度
	
	if (IsWindowsXPOrGreater()){
		return;// Only Windows 2000, Windows XP, Windows Server 2003...
		uSystemInfoLen = 0;
		WinNTHDSerialNumAsPhysicalReadWithZeroRights(0, szSystemInfo, &uSystemInfoLen, sizeof(szSystemInfo));
		*stringData += Str_CharToASCIIStr(szSystemInfo, uSystemInfoLen, G_ESCAPE_OFF);
		*stringData += '\r';
		uSystemInfoLen = 0;
		WinNTHDSerialNumAsPhysicalReadWithZeroRights(1, szSystemInfo, &uSystemInfoLen, sizeof(szSystemInfo));
		*stringData += Str_CharToASCIIStr(szSystemInfo, uSystemInfoLen, G_ESCAPE_OFF);
		*stringData += '\r';
		uSystemInfoLen = 0;
		WinNTHDSerialNumAsPhysicalReadWithZeroRights(2, szSystemInfo, &uSystemInfoLen, sizeof(szSystemInfo));
		*stringData += Str_CharToASCIIStr(szSystemInfo, uSystemInfoLen, G_ESCAPE_OFF);
		*stringData += '\r';
		uSystemInfoLen = 0;
		WinNTHDSerialNumAsPhysicalReadWithZeroRights(3, szSystemInfo, &uSystemInfoLen, sizeof(szSystemInfo));
		*stringData += Str_CharToASCIIStr(szSystemInfo, uSystemInfoLen, G_ESCAPE_OFF);
		*stringData += '\r';
	}
}
//------------------------------------------------------------------------------------------//
int32 SystemInfo_GetNetCardInfo(STDSTR *stringData){
	unsigned long stSize;
	int32 nRel,netCardNum,IPnumPerNetCard;
	
	PIP_ADAPTER_INFO pIpAdapterInfo = new IP_ADAPTER_INFO();//PIP_ADAPTER_INFO结构体指针存储本机网卡信息
	stSize = sizeof(IP_ADAPTER_INFO);						//得到结构体大小,用于GetAdaptersInfo参数
	nRel = GetAdaptersInfo(pIpAdapterInfo,&stSize);			//调用GetAdaptersInfo函数,填充pIpAdapterInfo指针变量;其中stSize参数既是一个输入量也是一个输出量
	netCardNum = 0;//记录网卡数量
	IPnumPerNetCard = 0;//记录每张网卡上的IP地址数量
	if (nRel == ERROR_BUFFER_OVERFLOW){
		//如果函数返回的是ERROR_BUFFER_OVERFLOW
		//则说明GetAdaptersInfo参数传递的内存空间不够,同时其传出stSize,表示需要的空间大小
		//这也是说明为什么stSize既是一个输入量也是一个输出量
		//释放原来的内存空间
		delete pIpAdapterInfo;//重新申请内存空间用来存储所有网卡信息
		pIpAdapterInfo = (PIP_ADAPTER_INFO) new BYTE[stSize];
		nRel = GetAdaptersInfo(pIpAdapterInfo,&stSize);//再次调用GetAdaptersInfo函数,填充pIpAdapterInfo指针变量
	}
	if (nRel == ERROR_SUCCESS){//输出网卡信息,可能有多网卡,因此通过循环去判断
		while (pIpAdapterInfo){
			*stringData += pIpAdapterInfo->AdapterName;
			*stringData += "\n";
			*stringData += pIpAdapterInfo->Description;
			*stringData += "\n";
			for (DWORD i = 0; i < pIpAdapterInfo->AddressLength; i++){
				*stringData += Str_RTrim(Str_CharToHEXStr(&pIpAdapterInfo->Address[i],1,G_SPACE_OFF));
				if (i < pIpAdapterInfo->AddressLength - 1)
					*stringData += '-';
			}
			*stringData += "\r";
			netCardNum ++;
			pIpAdapterInfo = pIpAdapterInfo->Next;
		}
	}
	if (pIpAdapterInfo != nullptr)
		delete []pIpAdapterInfo;
	return(netCardNum);
}
//------------------------------------------------------------------------------------------//
void SystemInfo_GetCPUID(STDSTR *stringData){
	BYTE szSystemInfo[80] = { 0 }; // 在程序执行完毕后，此处存储取得的系统特征码
	bool bException = false;
	try{
		_asm{
			mov eax, 0
			cpuid
			mov dword ptr szSystemInfo[0], ebx
			mov dword ptr szSystemInfo[4], edx
			mov dword ptr szSystemInfo[8], ecx
			
			mov eax,1
			cpuid
			mov dword ptr szSystemInfo[12],edx
			mov dword ptr szSystemInfo[16],ebx
			mov dword ptr szSystemInfo[20],ecx
			mov dword ptr szSystemInfo[24],eax
			
			mov eax,0x80000003
			cpuid
			mov dword ptr szSystemInfo[28],  eax
			mov dword ptr szSystemInfo[32],  ecx
			mov dword ptr szSystemInfo[36],  edx
			mov dword ptr szSystemInfo[40],  ebx
			mov eax,0x80000004
			cpuid
			mov dword ptr szSystemInfo[44],  eax
			mov dword ptr szSystemInfo[48],  ebx
			mov dword ptr szSystemInfo[52],  ecx
			mov dword ptr szSystemInfo[56],  edx
			mov eax,0x80000002
			cpuid
			mov dword ptr szSystemInfo[60],  ebx
			mov dword ptr szSystemInfo[64],  eax
			mov dword ptr szSystemInfo[68],  edx
			mov dword ptr szSystemInfo[72],  ecx
		}
	}
	catch(...){//( EXCEPTION_EXECUTE_HANDLER ){
		bException = TRUE;
	}
	szSystemInfo[19] = 157;
	*stringData += Str_CharToASCIIStr(szSystemInfo,76,G_ESCAPE_OFF);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif
#ifdef CommonDefH_MAC
#include <net/if.h>
#include <sys/ioctl.h>
#include <net/if_dl.h>
#include <net/ethernet.h>
#include <arpa/inet.h>
#define max(a,b)    ((a) > (b) ? (a) : (b))
const STDSTR &SystemInfo_GetNetCardInfo(STDSTR *retStr){
	char				buffer[1024 * 4], *ptr;
	struct ifconf		ifc;
	struct ifreq		*ifr;
	struct sockaddr_in 	*sin;
	struct sockaddr_dl	*sdl;
	
	uint32	num;
	uint32	a,b,c,d,e,f;
	char	macaddr[18];
	int 	sockfd;
	STDSTR strT,lastName;
	
	*retStr = "";
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd > 0){
		ifc.ifc_len = sizeof(buffer);
		ifc.ifc_buf = buffer;
		if (ioctl(sockfd, SIOCGIFCONF, &ifc) == 0){
			ptr = buffer;
			num = ifc.ifc_len / sizeof(struct ifreq);
			lastName = "";
			while(num -- > 0){
				ifr = (struct ifreq *)ptr;
				ptr += sizeof(ifr->ifr_name) + max(sizeof(struct sockaddr),ifr->ifr_addr.sa_len);
				if (ifr->ifr_addr.sa_family == AF_LINK){
					strT = ifr->ifr_name;
					if (retStr->length() > 0)
						*retStr += '\r';
					*retStr += strT;
					*retStr += ',';
					sdl = (struct sockaddr_dl *)&ifr->ifr_addr;
					strcpy(macaddr, (char *)ether_ntoa((const struct ether_addr *)LLADDR(sdl)));
					sscanf(macaddr, "%x:%x:%x:%x:%x:%x", &a, &b, &c, &d, &e, &f);
					snprintf(macaddr,sizeof(macaddr), "%02X:%02X:%02X:%02X:%02X:%02X",a,b,c,d,e,f);
					strT = macaddr;
					*retStr += strT;
				}
				else if (ifr->ifr_addr.sa_family == AF_INET){
					*retStr += ',';
					sin = (struct sockaddr_in *)&(ifr->ifr_addr);
					strT = inet_ntoa(sin->sin_addr);
					*retStr += strT;
				}
			};
		}
		close(sockfd);
	}
	*retStr += '\r';
	return(*retStr);
}
#endif
#ifdef CommonDefH_Linux
const STDSTR &SystemInfo_GetNetCardInfo(STDSTR *retStr){
	*retStr += '\r';
	return(*retStr);
};
#endif
//------------------------------------------------------------------------------------------//
