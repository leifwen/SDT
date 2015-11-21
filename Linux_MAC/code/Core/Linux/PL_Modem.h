/*
 * Copyright (c) 2012-2014
 * All rights reserved.
 *
 * File Name: PL_Modem.h
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2013.6.27
 */
#ifndef PL_ModemH
#define PL_ModemH
//------------------------------------------------------------------------------------------//
#ifdef CommonDefH_Linux
//------------------------------------------------------------------------------------------//
#define FTDI_232RL_ID				"vendor:0403 product:6001"
#define FTDI_2322RL_ID				"vendor:0403 product:6010"

#define TELIT_QLOAD_PORT			"vendor:05c6 product:9008,0"

#define TELIT_DE910_ID				"vendor:1bc7 product:1010"
#define TELIT_DE910_PORT_Diagnostic	"vendor:1bc7 product:1010,0"
#define TELIT_DE910_PORT_NMEA		"vendor:1bc7 product:1010,1"
#define TELIT_DE910_PORT_Aux		"vendor:1bc7 product:1010,2"
#define TELIT_DE910_PORT_Modem		"vendor:1bc7 product:1010,3"


#define TELIT_CE910_ID				"vendor:1bc7 product:1011"
#define TELIT_CE910_PORT_Diagnostic	"vendor:1bc7 product:1011,0"
#define TELIT_CE910_PORT_Modem		"vendor:1bc7 product:1011,1"


#define TELIT_HE910_ID				"vendor:1bc7 product:0021"
/*
 usbserinfo:1.0 driver:2.0
 0: module:ftdi_sio name:"FTDI USB Serial Device" vendor:0403 product:6001 num_ports:1 port:1 path:usb-0000:00:1d.0-1
 */
//------------------------------------------------------------------------------------------//
class PL_MODEM : public APICOM{
	public:
		enum{RFLAG_C = 0, RFLAG_S = APICOM::RFLAG_S + APICOM::RFLAG_C};
				 PL_MODEM(ODEV_LIST *tODEV_LIST,uint32 tSize);
		virtual	~PL_MODEM(void);
	private:
		int32			usbport;
		int32			comBR;
		SYS_ThreadEx<PL_MODEM>		hotplugThread;
		int32			hotplugThreadFun(void);//used for hotplug
	public:
		PL_HOTPLUG		*cgHotplug;
		std::string		cgVendor;
		std::string		cgVendorM;
		std::string		cgTTYUSB[10];
	
		int32			OpenPort		(const std::string &strVerdor,int32 comBaudRate);
		int32			ReOpenPort		(void);
		void			EnableHotplug	(void){hotplugThread.ThreadRun();};
		void			DisnableHotplug	(void){hotplugThread.ThreadStop();};
	
		int32			CheckVendor_M		(const std::string vendor);
		int32			UpdateTTYUSBList_M	(void);
		int32			CheckTTYUSB			(const std::string &ttyUSB) const;
		int32			UpdateTTYUSBList	(const std::string &strVerdor);
		static	int32	GetUSBSerialVendor	(const std::string &ttyUSB,std::string *retVendor);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif
#endif