/*
 * Copyright (c) 2012-2014 
 * All rights reserved.
 *
 * File Name: PL_Hotplug.h
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2012.02.09
 * @2013.7.03 :	dedicated for Linux
 * @2013.6.30 :	change to Liunx style
 				add init method
 				use int32 to replace bool
 				use int32 to replace uint32
*/
//------------------------------------------------------------------------------------------//
/*
 add@/devices/pci0000:00/0000:00:1d.0/usb2/2-1
 add@/devices/pci0000:00/0000:00:1d.0/usb2/2-1/2-1:1.0
 add@/devices/pci0000:00/0000:00:1d.0/usb2/2-1/2-1:1.0/ttyUSB0
 add@/devices/pci0000:00/0000:00:1d.0/usb2/2-1/2-1:1.0/ttyUSB0/tty/ttyUSB0
 add@/devices/pci0000:00/0000:00:1d.0/usb2/2-1/2-1:1.1
 add@/devices/pci0000:00/0000:00:1d.0/usb2/2-1/2-1:1.1/ttyUSB1
 add@/devices/pci0000:00/0000:00:1d.0/usb2/2-1/2-1:1.1/ttyUSB1/tty/ttyUSB1
 add@/devices/pci0000:00/0000:00:1d.0/usb2/2-2
 add@/devices/pci0000:00/0000:00:1d.0/usb2/2-2/2-2:1.0
 add@/devices/pci0000:00/0000:00:1d.0/usb2/2-2/2-2.1
 add@/devices/pci0000:00/0000:00:1d.0/usb2/2-2/2-2.1/2-2.1:1.0
 add@/devices/pci0000:00/0000:00:1d.0/usb2/2-2/2-2.1/2-2.1:1.0/ttyUSB2
 add@/devices/pci0000:00/0000:00:1d.0/usb2/2-2/2-2.1/2-2.1:1.0/ttyUSB2/tty/ttyUSB2
 add@/devices/pci0000:00/0000:00:1d.0/usb2/2-2/2-2.1/2-2.1:1.1
 add@/devices/pci0000:00/0000:00:1d.0/usb2/2-2/2-2.1/2-2.1:1.1/ttyUSB3
 add@/devices/pci0000:00/0000:00:1d.0/usb2/2-2/2-2.1/2-2.1:1.1/ttyUSB3/tty/ttyUSB3
 
 remove@/devices/pci0000:00/0000:00:1d.0/usb2/2-1/2-1:1.0/ttyUSB0/tty/ttyUSB0\0
 ACTION=remove\0
 DEVPATH=/devices/pci0000:00/0000:00:1d.0/usb2/2-1/2-1:1.0/ttyUSB0/tty/ttyUSB0\0
 SUBSYSTEM=tty\0MAJOR=188\0MINOR=0\0DEVNAME=ttyUSB0\0
 SEQNUM=1611\0
 
 remove@/devices/pci0000:00/0000:00:1d.0/usb2/2-1/2-1:1.0/ttyUSB0\0
 ACTION=remove\0
 DEVPATH=/devices/pci0000:00/0000:00:1d.0/usb2/2-1/2-1:1.0/ttyUSB0\0
 SUBSYSTEM=usb-serial\0
 SEQNUM=1612\0
 
 remove@/devices/pci0000:00/0000:00:1d.0/usb2/2-1/2-1:1.0\0ACTION=remove\0
 DEVPATH=/devices/pci0000:00/0000:00:1d.0/usb2/2-1/2-1:1.0\0
 SUBSYSTEM=usb\0DEVTYPE=usb_interface\0PRODUCT=403/6001/400\0
 TYPE=0/0/0\0
 INTERFACE=255/255/255\0
 MODALIAS=usb:v0403p6001d0400dc00dsc00dp00icFFiscFFipFF\0
 SEQNUM=1613\0
 
 remove@/devices/pci0000:00/0000:00:1d.0/usb2/2-1\0
 ACTION=remove\0
 DEVPATH=/devices/pci0000:00/0000:00:1d.0/usb2/2-1\0
 SUBSYSTEM=usb\0MAJOR=189\0MINOR=154\0DEVNAME=bus/usb/002/027\0
 DEVTYPE=usb_device\0
 PRODUCT=403/6001/400\0
 TYPE=0/0/0\0
 BUSNUM=002\0
 DEVNUM=027\0
 SEQNUM=1614\0
 */
//------------------------------------------------------------------------------------------//
#include "Commu_Socket.h"
//------------------------------------------------------------------------------------------//
#ifdef CommonDefH_Linux
#ifndef PL_HotplugH
#define PL_HotplugH
#ifdef PL_HotplugH
//------------------------------------------------------------------------------------------//
class PL_HOTPLUG : public ASOCKET{
	public:
				 PL_HOTPLUG(uint32 tSize,const ODEV_SYSTEM *logSys = nullptr) : ASOCKET(tSize,logSys){SetSelfName("PL_HOTPLUG");};
		virtual ~PL_HOTPLUG(void){;};
	protected:
		int32	osHandle;
	private:
		void 	Init(void);
	protected:
		virtual	int32	OpenDev				(const STDSTR &tCDBufName,int32 tCDBufPar,CSType tCSType,int32 blEnEcho);
		virtual	void	CloseDev			(void);
	public:
				int32	Open				(void){return(OpenD("",0,CSType_None,0));};
};
//------------------------------------------------------------------------------------------//
#endif
#endif
#endif

