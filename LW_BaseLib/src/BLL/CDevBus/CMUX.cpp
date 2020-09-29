//
//  CMUX.cpp
//  SDT
//
//  Created by Leif Wen on 25/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "stdafx.h"
//------------------------------------------------------------------------------------------//
#include "CMUX.h"
#ifdef CMUX_h
//------------------------------------------------------------------------------------------//
#include "DS_STRING.h"
#include "ADS_SMC.h"
//#define LOGPRINT_ENABLE
//#define LOGTHREAD_ENABLE
#include "SYS_Log.h"
//------------------------------------------------------------------------------------------//
CORE_VCOM::CORE_VCOM(void) : CORE_ACOM(){
	cgPortID = 0;
	ClrSFlag(VCOM_blDSR | VCOM_blCTS | VCOM_blDCD | VCOM_blRING | VCOM_blDTR | VCOM_blRTS | VCOM_blHEX | VCOM_blEscape);
	SetSFlag(VCOM_blEscape);
	
	SetSelfName("CORE_VCOM");
	SetUpName("");
};
//------------------------------------------------------------------------------------------//
CORE_VCOM::~CORE_VCOM(void){
	CloseDev();
};
//------------------------------------------------------------------------------------------//
void CORE_VCOM::Init(const COMMU_TEAM* _team){
	CORE_ACOM::Init(_team);
	modemStatusThread.RemoveSelf();
};
//------------------------------------------------------------------------------------------//
bool32 CORE_VCOM::OpenDev(const OPEN_PAR& par){
	ClrSFlag(VCOM_blDSR | VCOM_blCTS | VCOM_blDCD | VCOM_blRING | VCOM_blDTR | VCOM_blRTS | VCOM_blHEX | VCOM_blEscape);
	SetSFlag(VCOM_blEscape);
	
#ifdef CommonDefH_VC
	return(CORE_ACOM::OpenDev(par));
#endif
#ifdef CommonDefH_Unix
	OPEN_PAR	oPar;
	
	SetOpenPar(&oPar, par);
	oPar.type = OPEN_COMV;
	oPar.name = CMUX_COM_NAME;
	oPar.name += Str_ToStr(cgPortID);
	return(CORE_ACOM::OpenDev(oPar));
#endif
};
//------------------------------------------------------------------------------------------//
void CORE_VCOM::PrintOpenSuccess(const STDSTR& strTitle){
#ifdef CommonDefH_Unix
	COMMU_ODEVBUS::PrintMessageDot(unitTeam->odevBus,"Open CMUX virtual port",Str_ToStr(cgPortID),":",vPortName);
#else
	COMMU_ODEVBUS::PrintMessageDot(unitTeam->odevBus,"Open CMUX virtual port",Str_ToStr(cgPortID),":",GetOpenPar().name);
#endif
};
//------------------------------------------------------------------------------------------//
void CORE_VCOM::PrintClose(const uint64& rxBytes,const uint64& txBytes,const uint64& dpBytes){
#ifdef CommonDefH_Unix
	COMMU_ODEVBUS::PrintMessageDot(unitTeam->odevBus,"Close CMUX virtual port",Str_ToStr(cgPortID),":",vPortName);
#else
	COMMU_ODEVBUS::PrintMessageDot(unitTeam->odevBus,"Open CMUX virtual port",Str_ToStr(cgPortID),":",GetOpenPar().name);
#endif
};
//------------------------------------------------------------------------------------------//
void CORE_VCOM::Update(COMMU_DRV* vcom,bool32 bl,uint32 flag){
	if (vcom == nullptr)
		return;
	if (bl > 0){
		static_cast<CORE_VCOM*>(vcom->Core())->SetSFlag(flag);
	}
	else{
		static_cast<CORE_VCOM*>(vcom->Core())->ClrSFlag(flag);
	}
};
//------------------------------------------------------------------------------------------//
void CORE_VCOM::Update_DSR		(COMMU_DRV* vcom,bool32 bl){Update(vcom,bl,VCOM_blDSR);};
void CORE_VCOM::Update_CTS		(COMMU_DRV* vcom,bool32 bl){Update(vcom,bl,VCOM_blCTS);};
void CORE_VCOM::Update_DCD		(COMMU_DRV* vcom,bool32 bl){Update(vcom,bl,VCOM_blDCD);};
void CORE_VCOM::Update_RING		(COMMU_DRV* vcom,bool32 bl){Update(vcom,bl,VCOM_blRING);};
void CORE_VCOM::Update_DTR		(COMMU_DRV* vcom,bool32 bl){Update(vcom,bl,VCOM_blDTR);};
void CORE_VCOM::Update_RTS		(COMMU_DRV* vcom,bool32 bl){Update(vcom,bl,VCOM_blRTS);};
void CORE_VCOM::Update_HEX		(COMMU_DRV* vcom,bool32 bl){Update(vcom,bl,VCOM_blHEX);};
void CORE_VCOM::Update_Escape	(COMMU_DRV* vcom,bool32 bl){Update(vcom,bl,VCOM_blEscape);};
//------------------------------------------------------------------------------------------//
bool32	CORE_VCOM::Check_DSR	(COMMU_DRV* vcom){return((vcom == nullptr)?G_FALSE:static_cast<CORE_VCOM*>(vcom->Core())->CheckSFlag(VCOM_blDSR));};
bool32	CORE_VCOM::Check_CTS	(COMMU_DRV* vcom){return((vcom == nullptr)?G_FALSE:static_cast<CORE_VCOM*>(vcom->Core())->CheckSFlag(VCOM_blCTS));};
bool32	CORE_VCOM::Check_DCD	(COMMU_DRV* vcom){return((vcom == nullptr)?G_FALSE:static_cast<CORE_VCOM*>(vcom->Core())->CheckSFlag(VCOM_blDCD));};
bool32	CORE_VCOM::Check_RING	(COMMU_DRV* vcom){return((vcom == nullptr)?G_FALSE:static_cast<CORE_VCOM*>(vcom->Core())->CheckSFlag(VCOM_blRING));};
bool32	CORE_VCOM::Check_DTR	(COMMU_DRV* vcom){return((vcom == nullptr)?G_FALSE:static_cast<CORE_VCOM*>(vcom->Core())->CheckSFlag(VCOM_blDTR));};
bool32	CORE_VCOM::Check_RTS	(COMMU_DRV* vcom){return((vcom == nullptr)?G_FALSE:static_cast<CORE_VCOM*>(vcom->Core())->CheckSFlag(VCOM_blRTS));};
bool32	CORE_VCOM::Check_HEX	(COMMU_DRV* vcom){return((vcom == nullptr)?G_FALSE:static_cast<CORE_VCOM*>(vcom->Core())->CheckSFlag(VCOM_blHEX));};
bool32	CORE_VCOM::Check_Escape	(COMMU_DRV* vcom){return((vcom == nullptr)?G_FALSE:static_cast<CORE_VCOM*>(vcom->Core())->CheckSFlag(VCOM_blEscape));};
//------------------------------------------------------------------------------------------//
STDSTR& CORE_VCOM::DlciStatus(COMMU_DRV* vcom,STDSTR* retStr){
	if (vcom != nullptr){
		*retStr = " DLCI " + Str_ToStr(static_cast<CORE_VCOM*>(vcom->Core())->cgPortID) + " :";
		*retStr += " DTR=";	*retStr += (Check_DTR(vcom)  == G_FALSE)?"L,":"H,";
		*retStr += " RTS=";	*retStr += (Check_RTS(vcom)  == G_FALSE)?"L,":"H,";
		*retStr += " CTS=";	*retStr += (Check_CTS(vcom)  == G_FALSE)?"L,":"H,";
		*retStr += " DSR=";	*retStr += (Check_DSR(vcom)  == G_FALSE)?"L,":"H,";
		*retStr += " RING=";*retStr += (Check_RING(vcom) == G_FALSE)?"L,":"H,";
		*retStr += " DCD=";	*retStr += (Check_DCD(vcom)  == G_FALSE)?"L,":"H,";
		*retStr += (Check_HEX(vcom) == G_FALSE)?" ASCII mode,":" HEX mode,";
		*retStr += (Check_Escape(vcom) == G_FALSE)?" disable escape":" enable escape";
	}
	return(*retStr);
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
bool32 DISPOSE_VCOM::DPThreadFunc(void* _team){
	COMMU_TEAM*		team = static_cast<COMMU_TEAM*>(_team);
	VCOM*			vcom = static_cast<VCOM*>(team->cdrv);
	COMMU_MEM*		mem = team->mem;
	_CMUXDriver*	drv = static_cast<_CMUXDriver*>(GetUp(vcom));

	uint32		byteNum;
	STDSTR		strRecData;
	
	ETLogThreadStart(dpThread);
	
	while(dpThread.IsTerminated() == G_FALSE){
		byteNum = COMMU_MEM::GetArrayRx(mem)->Used();
		if (byteNum == 3){
			COMMU_MEM::GetArrayRx(mem)->Read(_EMPTY(&strRecData),3,0);
				if (strRecData == "+++")
					drv->Send3Pluse(vcom);
		}
		if (COMMU_MEM::GetArrayRx(mem)->Used() > 0){
			byteNum = drv->Send(vcom,COMMU_MEM::GetArrayRx(mem));
			COMMU_MEM::GetArrayRx(mem)->Out(byteNum);
		}
		SYS_SleepMS(2);
	}
	ETLogThreadStop(dpThread);
	return G_TRUE;
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
//waittime/dlytime/check/response/AT
const STDSTR CMUX_DEFATCMDS ="\
1000\n 100\n T\n OK\n AT\r\
1000\n 100\n T\n OK\n AT\r\
1000\n 100\n T\n OK\n AT\r\
1000\n 100\n T\n OK\n AT+IPR=115200\r\
1000\n 100\n T\n OK\n ATE0V1&K3&D2\r\
1000\n 100\n T\n OK\n AT+CMEE=2\r\
1000\n 100\n T\n OK\n AT+CGMI\r\
1000\n 100\n T\n OK\n AT+CGMM\r\
1000\n 100\n T\n OK\n AT+CGMR\r\
1000\n 100\n F\n OK\n AT#SELINT=2\r\
1000\n 100\n F\n OK\n AT#CMUXMODE=1\r\
1000\n 300\n T\n OK\n AT+CMUX=0\r\
";
using namespace CMUX;
static inline CDEVBUS* _GetDevice(COMMU_TEAM* team){
	return(static_cast<_CMUXDriver*>(team->cdrv)->GetCDevBus());
}
static inline CORE_VCOM* _GetCoreVCOM(VCOM* vcom){
	return(static_cast<CORE_VCOM*>(vcom->Core()));
}
static inline CMUX::UIH_FRAME* _GetTxUIH(COMMU_TEAM* team){
	return(&static_cast<_CMUXDriver*>(team->cdrv)->cgTxUIH);
}
//------------------------------------------------------------------------------------------//
CORE_CMUXDriver::CORE_CMUXDriver(void) : COMMU_CORE(){
	cgCMDsInit = "";
	
	SetSelfName("CORE_CMUXDriver");
	SetUpName("");
};
//------------------------------------------------------------------------------------------//
CORE_CMUXDriver::~CORE_CMUXDriver(void){
	CloseDev();
};
//------------------------------------------------------------------------------------------//
void CORE_CMUXDriver::SetSelfName(const STDSTR& strName){
	COMMU_CORE::SetSelfName(strName);
	commandThread.SetUpName(GetFullName(this));
};
//------------------------------------------------------------------------------------------//
void CORE_CMUXDriver::SetUpName(const STDSTR& strName){
	COMMU_CORE::SetUpName(strName);
	commandThread.SetUpName(GetFullName(this));
};
//------------------------------------------------------------------------------------------//
void CORE_CMUXDriver::PrintOpenSuccess(const STDSTR& strTitle){
	COMMU_ODEVBUS::PrintMessageDot(unitTeam->odevBus,"CMUX driver start success");
};
//------------------------------------------------------------------------------------------//
void CORE_CMUXDriver::PrintOpenFail(const STDSTR& strTitle){
	COMMU_ODEVBUS::PrintMessageDot(unitTeam->odevBus,"CMUX driver start fail");
};
//------------------------------------------------------------------------------------------//
void CORE_CMUXDriver::PrintClose(const uint64& rxBytes,const uint64& txBytes,const uint64& dpBytes){
	COMMU_ODEVBUS::PrintMessageDot(unitTeam->odevBus,"CMUX driver stop");
};
//------------------------------------------------------------------------------------------//
bool32 CORE_CMUXDriver::OpenDev(const OPEN_PAR& par){
#ifdef CommonDefH_Unix
	static_cast<_CMUXDriver*>(unitTeam->cdrv)->OpenVCOM(1,"",115200);
	static_cast<_CMUXDriver*>(unitTeam->cdrv)->OpenVCOM(2,"",115200);
	static_cast<_CMUXDriver*>(unitTeam->cdrv)->OpenVCOM(3,"",115200);
	static_cast<_CMUXDriver*>(unitTeam->cdrv)->OpenVCOM(4,"",115200);
#endif
	return(COMMU_CORE::OpenDev(par));
};
//------------------------------------------------------------------------------------------//
void CORE_CMUXDriver::CloseDev(void){
	if ((_GetDevice(unitTeam) != nullptr) && _GetDevice(unitTeam)->IsOpened() && IsConnected())
		CMUXStop();
	commandThread.ThreadStop();
	if ((_GetDevice(unitTeam) != nullptr) && (_GetDevice(unitTeam)->IsFor(DRV_COM) != nullptr))
		COMMU_RECORD::Enable(_GetDevice(unitTeam)->IsFor(DRV_COM)->unitTeam.record,G_TRUE);
	COMMU_CORE::CloseDev();
};
//------------------------------------------------------------------------------------------//
void CORE_CMUXDriver::CMUXStartInThread(const STDSTR& cmdsInit){
	commandThread.ThreadStop();
	cgCMDsInit = cmdsInit;
	commandThread.ThreadInit(this,&CORE_CMUXDriver::CmuxStartThreadFunc,"CMUXStart");
	commandThread.ThreadRun(unitTeam);
};
//------------------------------------------------------------------------------------------//
void CORE_CMUXDriver::CMUXStopInThread(void){
	commandThread.ThreadStop();
	commandThread.ThreadInit(this,&CORE_CMUXDriver::CmuxStopThreadFunc,"CMUXStop");
	commandThread.ThreadRun(unitTeam);
};
//------------------------------------------------------------------------------------------//
bool32 CORE_CMUXDriver::CmuxStartThreadFunc(void* _team){
	COMMU_TEAM*		team = static_cast<COMMU_TEAM*>(_team);
	COMMU_DRV*		cdrv = team->cdrv;
	if (CMUXStart(cgCMDsInit) == G_FALSE)
		cdrv->CloseSelf(0);
	return G_TRUE;
};
//------------------------------------------------------------------------------------------//
bool32 CORE_CMUXDriver::CmuxStopThreadFunc(void* _team){
	CMUXStop();
	return G_TRUE;
};
//------------------------------------------------------------------------------------------//
void CMDAnalysis(STDSTR& cmds,uint32& waitTime,uint32& dlyTime,STDSTR& check,STDSTR& atResponse,STDSTR& atCMD){
	atCMD = Str_ReadSubItem(&cmds,"\r");
	
	check = Str_ReadSubItem(&atCMD,"\n");
	Str_TrimSelf(check);
	waitTime = atoi(check.c_str());
	
	check = Str_ReadSubItem(&atCMD,"\n");
	Str_TrimSelf(check);
	dlyTime = atoi(check.c_str());
	
	check = Str_ReadSubItem(&atCMD,"\n");
	Str_TrimSelf(check);
	Str_UpperCaseSelf(check);
	
	atResponse = Str_ReadSubItem(&atCMD,"\n");
	Str_TrimSelf(atResponse);
	
	Str_TrimSelf(atCMD);
};
//------------------------------------------------------------------------------------------//
bool32 CORE_CMUXDriver::CMUXStart(STDSTR cmdsInit){
	CMUX::UIH_FRAME*	uih = _GetTxUIH(unitTeam);
	_CMUXDriver*		drv = static_cast<_CMUXDriver*>(unitTeam->cdrv);
	bool32	blOK,blInit;
	SBUF	cBuffer;
	STDSTR	atCMD,atResponse,check,hexCMD;;
	uint32	waitTime,dlyTime;
	
	cBuffer.array.InitSize(128);
	cBuffer.array.Empty();
	_GetDevice(unitTeam)->RxDataShareTo(&cBuffer);
	blOK = G_FALSE;
	blInit = cmdsInit.length() > 0;
	B_ClrFLAG64(_GetDevice(unitTeam)->GetODevBus()->envcfg, ODEV_FLAG_EnHEXViewMode);
	while(cmdsInit.length() > 0){
		CMDAnalysis(cmdsInit,waitTime,dlyTime,check,atResponse,atCMD);
		blOK = drv->SendATCMD(" ",atCMD,atResponse,&cBuffer.array,waitTime,dlyTime);
		if ((blOK == G_FALSE) && (check == "T"))
			break;
	};
	
	do{
		if (blInit != G_FALSE){
			COMMU_ODEVBUS::PrintWithDividingLine(unitTeam->odevBus,"CMUXDriver::Finish the AT commands initialization");
			if (blOK == G_FALSE)
				break;
		}
		
		blOK = G_FALSE;
		COMMU_RECORD::Enable(_GetDevice(unitTeam)->IsFor(DRV_COM)->unitTeam.record,G_FALSE);
		B_SetFLAG64(_GetDevice(unitTeam)->GetODevBus()->envcfg, ODEV_FLAG_EnHEXViewMode);
		do{
			if (drv->SendCMUXCMD("SABM Frame. Enable DLCI 0.\n",			uih->CreateSABMFrameD0(&hexCMD), "!'nullptr", &cBuffer.array, 2000, 100) == 0) break;
			if (drv->SendCMUXCMD("SABM Frame. Enable DLCI 1.\n",			uih->CreateSABMFrameV1(&hexCMD), "!'nullptr", &cBuffer.array, 2000, 100) == 0) break;
			if (drv->SendCMUXCMD("MSCCmd. Set DTR=0 & RTS=0 on DLCI=1.\n", 	uih->CreateDefaultMSV1(&hexCMD), "!'nullptr", &cBuffer.array, 2000, 100) == 0) break;
			if (drv->SendCMUXCMD("SABM Frame. Enable DLCI 2.\n",			uih->CreateSABMFrameV2(&hexCMD), "!'nullptr", &cBuffer.array, 2000, 100) == 0) break;
			if (drv->SendCMUXCMD("MSCCmd. Set DTR=0 & RTS=0 on DLCI=2.\n", 	uih->CreateDefaultMSV2(&hexCMD), "!'nullptr", &cBuffer.array, 2000, 100) == 0) break;
			if (drv->SendCMUXCMD("SABM Frame. Enable DLCI 3.\n",			uih->CreateSABMFrameV3(&hexCMD), "!'nullptr", &cBuffer.array, 2000, 100) == 0) break;
			if (drv->SendCMUXCMD("MSCCmd. Set DTR=0 & RTS=0 on DLCI=3.\n",	uih->CreateDefaultMSV3(&hexCMD), "!'nullptr", &cBuffer.array, 2000, 100) == 0) break;
			if (drv->SendCMUXCMD("SABM Frame. Enable DLCI 4.\n",			uih->CreateSABMFrameV4(&hexCMD), "!'nullptr", &cBuffer.array, 2000, 100) == 0) break;
			if (drv->SendCMUXCMD("MSCCmd. Set DTR=0 & RTS=0 on DLCI=4.\n",	uih->CreateDefaultMSV4(&hexCMD), "!'nullptr", &cBuffer.array, 2000, 100) == 0) break;
			blOK = G_TRUE;
		} while (0);
		
		COMMU_ODEVBUS::PrintWithDividingLine(unitTeam->odevBus,"CMUXDriver::Finish the CMUX initialization");
	}while(0);
	cBuffer.RemoveSelf();
	return(blOK);
};
//------------------------------------------------------------------------------------------//
void CORE_CMUXDriver::CMUXStop(void){
	CMUX::UIH_FRAME*	uih = _GetTxUIH(unitTeam);
	_CMUXDriver*		drv = static_cast<_CMUXDriver*>(unitTeam->cdrv);
	STDSTR	strCMD;
	SBUF	cBuffer;
	cBuffer.array.InitSize(128);
	cBuffer.array.Empty();
	_GetDevice(unitTeam)->RxDataShareTo(&cBuffer);
	B_SetFLAG64(_GetDevice(unitTeam)->GetODevBus()->envcfg, ODEV_FLAG_EnHEXViewMode);
	do{
		if (drv->SendCMUXCMD("DISC Frame. Close DLCI 4.\n", uih->CreateDISCFrameV4(&strCMD), "!'nullptr", &cBuffer.array, 2000, 100) == 0) break;
		if (drv->SendCMUXCMD("DISC Frame. Close DLCI 3.\n", uih->CreateDISCFrameV3(&strCMD), "!'nullptr", &cBuffer.array, 2000, 100) == 0) break;
		if (drv->SendCMUXCMD("DISC Frame. Close DLCI 2.\n", uih->CreateDISCFrameV2(&strCMD), "!'nullptr", &cBuffer.array, 2000, 100) == 0) break;
		if (drv->SendCMUXCMD("DISC Frame. Close DLCI 1.\n", uih->CreateDISCFrameV1(&strCMD), "!'nullptr", &cBuffer.array, 2000, 100) == 0) break;
		drv->SendCMUXCMD("DISC Frame. Close DLCI 0.\n", uih->CreateDISCFrameD0(&strCMD), "!'nullptr", &cBuffer.array, 2000, 100);
	} while (0);
	B_ClrFLAG64(_GetDevice(unitTeam)->GetODevBus()->envcfg, ODEV_FLAG_EnHEXViewMode);
	cBuffer.RemoveSelf();
	COMMU_RECORD::Enable(_GetDevice(unitTeam)->IsFor(DRV_COM)->unitTeam.record,G_TRUE);
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
template<typename... Args> inline void PrintFrame(COMMU_TEAM* team,const STDSTR& strFrame,const Args&... args){
#ifdef ODEV_BUS_h
	OUTPUT_NODE	*node = COMMU_ODEVBUS::GetVG3D(team->odevBus);
	if (node != nullptr){
		*node << Begin() << DefGroup() << NL()
		<< COL_DivLineTime
		<< COL_NormalMessage;
		
		DoPrintStr(node,args...)
		<< ":\n"
		<< COL_DB_RxText << strFrame
		<< "\n"
		<< Endl();
	}
#endif
};
//------------------------------------------------------------------------------------------//
template<typename... Args> inline void PrintFrame(COMMU_TEAM* team,const _ColData& strFrame,const Args&... args){
#ifdef ODEV_BUS_h
	OUTPUT_NODE	*node = COMMU_ODEVBUS::GetVG3D(team->odevBus);
	if (node != nullptr){
		*node << Begin() << DefGroup() << NL()
		<< COL_DivLineTime
		<< COL_NormalMessage;
		
		DoPrintStr(node,args...)
		<< ":\n"
		<< strFrame
		<< "\n"
		<< Endl();
	}
#endif
};
//------------------------------------------------------------------------------------------//
bool32 BRIDGE_CMUXDriver::TxThreadFunc(void* _team){
	COMMU_TEAM*			team = static_cast<COMMU_TEAM*>(_team);
	_CMUXDriver*		drv = static_cast<_CMUXDriver*>(unitTeam->cdrv);
	COMMU_ODEVBUS*		odevBus = team->odevBus;
	CMUX::UIH_FRAME* 	uih = _GetTxUIH(team);
	
	STDSTR		strData,strFrame,strDataT;
	VCOM		*vcom;
	uint32		dlci;
	uint8		ctrlType;
	
	ETLogThreadStart(txThread);
	while(txThread.IsTerminated() == G_FALSE){
		if (uih->GetDefArrayWR()->Used() > 0){
			if (uih->Analysis(0) > 0){
				uih->Read(nullptr, _EMPTY(&strFrame));
				
				ctrlType = uih->pn_Ctrl_Type.GetValueAMaskRE();
				dlci	 = uih->pn_Addr_DLCI.GetValueCalcRE();
				
				if ((ctrlType == CTRL_UIH) && (dlci > 0)){
					vcom = drv->GetVCOM(dlci);
					if (vcom != nullptr){
						if (CORE_VCOM::Check_HEX(vcom)){
							uih->pn_Info.Read(nullptr,OUD_HEXs(_EMPTY(&strData)));
							Str_RTrimSelf(strData);
						}
						else{
							uih->pn_Info.Read(nullptr,_EMPTY(&strData));
							if (CORE_VCOM::Check_Escape(vcom))
								strData = Str_EscapeToASCII(_EMPTY(&strDataT),strData);
						}
					}
					PrintFrame(team,COLOR(COL_clDCyan,strData),"CMUXThread::Received data from DLCI",Str_ToStr(dlci));
					COMMU_ODEVBUS::PrintWithTime(odevBus,COL_clDCyan,"CMUXThread::Send CMUX frame:\n",COLOR(COL_clDCyan,Str_ASCIIToHEXs(strFrame, G_ESCAPE_OFF)),"\n");
				}
				_GetDevice(team)->SendCommand(strFrame, CMD_NONE, G_ESCAPE_OFF);
				uih->OutRE();
			};
		}
		SYS_SleepMS(2);
	}
	ETLogThreadStop(txThread);
	return G_TRUE;
};
//------------------------------------------------------------------------------------------//
bool32 BRIDGE_CMUXDriver::RxThreadFunc(void* _team){
	COMMU_TEAM*		team = static_cast<COMMU_TEAM*>(_team);
	STDSTR			strFrame,strInfo;
	SBUF			rxSBUF;
	CMUX::UIH_FRAME	rxUIH;
	rxSBUF.array.InitSize(1024*4);
	rxUIH.InitPN(&rxSBUF.array, &rxSBUF.array);
#ifdef LOGTHREAD_ENABLE
	STDSTR	strRec;
#endif
	SMC_EncryptI(0)
	SMC_EncryptS(0)
	_GetDevice(team)->RxDataShareTo(&rxSBUF);
	SMC_EncryptE(0)
	ETLogThreadStart(rxThread);
	while(rxThread.IsTerminated() == G_FALSE){
		SYS_SleepMS(2);
		if (rxUIH.GetDefArrayRE()->Used() > 0){
			if (rxUIH.TryGetFrame() > 0){
				rxUIH.Read(nullptr,OUD_HEXs(_EMPTY(&strFrame)));
				DeliveryRxFrame(team,&rxUIH,strFrame,strInfo);
				rxUIH.OutRE();
			}
		}
	}
	rxSBUF.RemoveSelf();
	ETLogThreadStop(rxThread);
	return G_TRUE;
};
//------------------------------------------------------------------------------------------//
void BRIDGE_CMUXDriver::DeliveryRxFrame(COMMU_TEAM* team,UIH_FRAME* rxUIH,const STDSTR& strFrame,STDSTR& strInfo){
	_CMUXDriver*	drv = static_cast<_CMUXDriver*>(unitTeam->cdrv);
	VCOM		*vcom;
	uint8		ctrlType;
	uint32		dlci;
	
	dlci = rxUIH->pn_Addr_DLCI.GetValueCalcRE();	// Check DLCI: Addr field:  |dlci|C/R|E/A|
	
	if (dlci > _CMUXDriver::Max_CMUXCOM){
		PrintFrame(team,strFrame,"CMUXThread::DLCI vaule is invaild");
		return;
	}
	
	ctrlType = rxUIH->pn_Ctrl_Type.GetValueAMaskRE();//Check CTRL: CTRL field:  |X|X|X|X|P/F|X|X|X|,Suppose P/F bit in control field always = 0
	
	switch(ctrlType){
		case CTRL_SABM:
			PrintFrame(team,strFrame,"CMUXThread::Received SABM frame, MS wants connect to me, no implemented Ctrl Type");
			break;
		case CTRL_UA:
			PrintFrame(team,strFrame,"CMUXThread::Received UA response frame on DLCI",Str_ToStr(dlci));
			break;
		case CTRL_DM:
			PrintFrame(team,strFrame,"CMUXThread::Received DM response frame on DLCI",Str_ToStr(dlci));
			break;
		case CTRL_DISC:
			PrintFrame(team,strFrame,"CMUXThread::Received DISC frame, MS wants disconnect to me, no implemented Ctrl Type");
			break;
		case CTRL_UIH:
			if (dlci == DLCI0){
				CMuxCtrlCmdExec(team,rxUIH,strFrame,strInfo);
			}
			else{
				PrintFrame(team,strFrame,"CMUXThread::Received UIH frame on DLCI",Str_ToStr(dlci));
				vcom = drv->GetVCOM(dlci);
				if (vcom != nullptr){
					rxUIH->pn_Info.Read(nullptr, _EMPTY(&strInfo));
					vcom->Send(strInfo,0);
				}
			}
			break;
		case CTRL_UI:
			PrintFrame(team,strFrame,"CMUXThread::Received UI frame received, no implemented Ctrl Type");
			break;
		default:
			PrintFrame(team,strFrame,"CMUXThread::CTRL Field |X|X|X|X|P/F|X|X|X| is invaild");
			break;
	}
};
//------------------------------------------------------------------------------------------//
void BRIDGE_CMUXDriver::CMuxCtrlCmdExec(COMMU_TEAM* team,UIH_FRAME* rxUIH,const STDSTR& strFrame,STDSTR& strInfo){
	//There is no specified response to the UIH cpmmand/response
	//MSC Command Application->Module => C/R bit (RED) 1 and C/R bit (BLUE) 1
	//MSC Command Module->Application => C/R bit (RED) 0 and C/R bit (BLUE) 1
	//MSC Response Application->Module => C/R bit (RED) 1 and C/R bit (BLUE) 0
	//MSC Response Module->Application => C/R bit (RED) 0 and C/R bit (BLUE) 0
	
	uint8 	charTemp;
	STDSTR	strMSC;
	uint32	cMSCCRbit,cMSCDlci,cMS,frameCRbit,cmdType;
	
	rxUIH->pn_Info.Read(nullptr, _EMPTY(&strInfo));
	charTemp = strInfo[0];
	cmdType = charTemp & ~(CR_BIT | EA_BIT);
	switch(cmdType){
		case CMD_MSC:{		// Modem Status Command
			cMSCCRbit = charTemp & CR_BIT;
			cMSCDlci  = strInfo[2] >> 2;											// Get DLCI from frame
			cMS 	  = strInfo[3];
			frameCRbit = rxUIH->pn_Addr_CR.GetValueAMaskRE();						//addr & CMUX_CR_BIT;
			if ((frameCRbit == 0) && (cMSCCRbit != 0)){								//MSC Command Module->Application => C/R bit (RED) 0 and C/R bit (BLUE) 1
				
				PrintFrame(team,strFrame,"CMUXThread::Received MSC update frame on DLCI",Str_ToStr(cMSCDlci));
				COMMU_ODEVBUS::PrintWithTime(unitTeam->odevBus,COL_clDCyan,"CMUXThread::",UpdateModemStatus(&strMSC,cMSCDlci,cMS));
				
				_GetTxUIH(team)->CreateMSCCmd(&strMSC,cMSCDlci,cMS,CR_BIT,0);//MSC Response Application->Module => C/R bit (RED) 1 and C/R bit (BLUE) 0
				COMMU_ODEVBUS::PrintWithTime(unitTeam->odevBus,COL_clDCyan,"CMUXThread::Send MSC confirm frame:\n",COLOR(COL_clDCyan,strMSC),"\n");
			}
			else{
				PrintFrame(team,strFrame,"CMUXThread::Received MSC response on DLCI",Str_ToStr(cMSCDlci));
			}
			break;
		}
		case CMD_PSC:{		// Power Saving Control: Not Impl.
			cMSCCRbit = charTemp & CR_BIT;
			if (cMSCCRbit == 0){
				PrintFrame(team,strFrame,"CMUXThread::Received PSC response frame");
			}
			else{
				PrintFrame(team,strFrame,"CMUXThread::Received PSC frame, no supported in current version");
			}
			break;
		}
		case CMD_TEST:		// Test command
			PrintFrame(team,strFrame,"CMUXThread::Received Test Command, no supported in current version");
			break;
		case NSC_RES:		// Not Supported Command: Not Impl.
			PrintFrame(team,strFrame,"CMUXThread::Received NSC CMD, no supported CMD");
			break;
		case CMD_PN:		// Parameter Negotiation: Not Impl.
			PrintFrame(team,strFrame,"CMUXThread::Received Parameter Negotiation CMD, no supported CMD");
			break;
		case CMD_MUX_CLD:	// CMux Close Down: Not Impl.
			PrintFrame(team,strFrame,"CMUXThread::Received CMux Close Down CMD, no supported CMD");
			break;
		case CMD_FCON:		// Flow Control ON: Not Impl.
			PrintFrame(team,strFrame,"CMUXThread::Received Flow Control ON CMD, no supported CMD");
			break;
		case CMD_FCOFF:		// Flow Control OFF: Not Impl.
			PrintFrame(team,strFrame,"CMUXThread::Received Flow Control OFF CMD, no supported CMD");
			break;
		case CMD_RPN:		// Remote Port Negotiation: Not Impl.
			PrintFrame(team,strFrame,"CMUXThread::Received Remote Port Negotiation CMD, no supported CMD");
			break;
		case CMD_RLS:		// Remote Line Status: Not Impl.
			PrintFrame(team,strFrame,"CMUXThread::Received Remote Line Status CMD, no supported CMD");
			break;
		case CMD_SNC:		// Service Negotiation Command: Not Impl.
			PrintFrame(team,strFrame,"CMUXThread::Received Service Negotiation CMD, no supported CMD");
			break;
		default:;
			PrintFrame(team,strFrame,"CMUXThread::Received Other no supported CMD");
	}
};
//------------------------------------------------------------------------------------------//
const STDSTR& BRIDGE_CMUXDriver::UpdateModemStatus(STDSTR* strForPrint,uint32 cMSCDlci,uint32 cMS){
	_CMUXDriver*	drv = static_cast<_CMUXDriver*>(unitTeam->cdrv);
	VCOM* vcom;
	
	vcom = drv->GetVCOM(cMSCDlci);
	
	if (cMS & MS_CTS){
		CORE_VCOM::Update_CTS(vcom,0);
		*strForPrint = "CTS=0";
	}
	else{
		CORE_VCOM::Update_CTS(vcom,1);
		*strForPrint = "CTS=1";
	}
	if (cMS & MS_DSR){
		CORE_VCOM::Update_DSR(vcom,0);
		*strForPrint += ",DSR=0";
	}
	else{
		CORE_VCOM::Update_DSR(vcom,1);
		*strForPrint += ",DSR=1";
	}
	if (cMS & MS_RING){
		CORE_VCOM::Update_RING(vcom,0);
		*strForPrint += ",RING=0";
	}
	else{
		CORE_VCOM::Update_RING(vcom,1);
		*strForPrint += ",RING=1";
	}
	if (cMS & MS_DCD){
		CORE_VCOM::Update_DCD(vcom,0);
		*strForPrint += ",DCD=0";
	}
	else{
		CORE_VCOM::Update_DCD(vcom,1);
		*strForPrint += ",DCD=1";
	}

	return(*strForPrint);
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
void _CMUXDriver::Init(uint32 rxSize,uint32 txSize,const CDEVBUS* cDevBus){
	cgCDevBus = (CDEVBUS*)cDevBus;
	COMMU_DRV::Init(rxSize,txSize,cDevBus->GetODevBus());
	cgArray.InitSize(txSize);
	cgTxUIH.InitPN(&cgArray, &cgArray);
};
//------------------------------------------------------------------------------------------//
bool32 _CMUXDriver::DoOpen(const OPEN_PAR& par){
	if (COMMU_DRV::DoOpen(par)){
		if (CheckSFlag(CMUX_blInitInThread)){
			static_cast<CORE_CMUXDriver*>(unitTeam.core)->CMUXStartInThread(cgCMDsInit);
			return G_TRUE;
		}
		return(static_cast<CORE_CMUXDriver*>(unitTeam.core)->CMUXStart(cgCMDsInit));
	}
	return G_FALSE;
};
//------------------------------------------------------------------------------------------//
bool32 _CMUXDriver::Open(const STDSTR& cmdsInit,uint64 blInitInThread){
	bool32	ret;
	ret = 0;
	if (cgOpenLock.Try()){
		if (cgCDevBus->EDA()->IsComOpened()){
			cgCMDsInit = cmdsInit;
			ClrSFlag(CMUX_blInitInThread);
			SetSFlag(blInitInThread & CMUX_blInitInThread);
			ret = COMMU_DRV::Open(SetOpenPar(OPEN_None,"CMUX",999,0));
		}
		cgOpenLock.Clr();
	}
	return(ret);
};
//------------------------------------------------------------------------------------------//
CDEVBUS* _CMUXDriver::GetCDevBus(void){
	return(cgCDevBus);
};
//------------------------------------------------------------------------------------------//
bool32 _CMUXDriver::SendATCMD(const STDSTR& strTitle,const STDSTR& strCMD,const STDSTR& strCondition,ARRAY* array,uint32 waitMS,uint32 delyMS){
	cgCDevBus->PrintSendCommand("CMUXDriver::Send",strTitle,strCMD);
	cgCDevBus->SendCommand(strCMD,CMD_R,G_ESCAPE_ON);
	return(cgCDevBus->CheckReceive(strCondition, array, waitMS, delyMS));
};
//------------------------------------------------------------------------------------------//
bool32 _CMUXDriver::SendCMUXCMD(const STDSTR& strTitle,const STDSTR& strCMD,const STDSTR& strCondition,ARRAY* array,uint32 waitMS,uint32 delyMS){
	cgCDevBus->PrintSendCommand("CMUXDriver::Send",strTitle,strCMD);
	return(cgCDevBus->CheckReceive(strCondition, array, waitMS, delyMS));
};
//------------------------------------------------------------------------------------------//
VCOM* _CMUXDriver::GetVCOM(int32 dlci){
	VCOM	*vcom;
	TREE_DownChain_Find(VCOM,this,vcom,(_GetCoreVCOM(_opNode)->cgPortID == dlci));
	return (vcom);
};
//------------------------------------------------------------------------------------------//
VCOM* _CMUXDriver::OpenVCOM(int32 dlci,const STDSTR& name,uint32 baudrate){
	VCOM	*vcom;
	
	if ((dlci > Max_CMUXCOM) || (dlci == 0))
		return(nullptr);
	
	vcom = GetVCOM(dlci);
	if (vcom == nullptr){
		vcom = static_cast<VCOM*>(GetNewNode());
		if (vcom != nullptr){
			_GetCoreVCOM(vcom)->cgPortID = dlci;
			AppendDownNode(vcom);
		}
	}
	if (vcom != nullptr){
		if (vcom->Open(SetOpenPar(OPEN_COM,name,baudrate,0)) == G_FALSE)
			vcom = nullptr;
	}
	return(vcom);
};
//------------------------------------------------------------------------------------------//
void _CMUXDriver::CloseVCOM(int32 dlci){
	VCOM	*vcom;
	if (dlci > Max_CMUXCOM)
		return;
	vcom = GetVCOM(dlci);
	if (vcom != nullptr)
		vcom->Close();
};
//------------------------------------------------------------------------------------------//
void _CMUXDriver::SendCLD(void){
	STDSTR	strCMD;
	SBUF	cBuffer;
	cBuffer.array.InitSize(128);
	cBuffer.array.Empty();
	cgCDevBus->RxDataShareTo(&cBuffer);
	B_SetFLAG64(cgCDevBus->GetODevBus()->envcfg, ODEV_FLAG_EnHEXViewMode);
	SendCMUXCMD("CLD frame. Exit CMUX mode:\n",cgTxUIH.CreateCLDFrame(&strCMD),"!'nullptr",&cBuffer.array,2000,100);
	B_ClrFLAG64(cgCDevBus->GetODevBus()->envcfg, ODEV_FLAG_EnHEXViewMode);
	cBuffer.RemoveSelf();
	COMMU_RECORD::Enable(cgCDevBus->IsFor(DRV_COM)->unitTeam.record,G_TRUE);
};
//------------------------------------------------------------------------------------------//
void _CMUXDriver::SendStdPSC(uint8 cmode){
	STDSTR	strCMD;
	SBUF	cBuffer;
	cBuffer.array.InitSize(128);
	cBuffer.array.Empty();
	cgCDevBus->RxDataShareTo(&cBuffer);
	B_SetFLAG64(cgCDevBus->GetODevBus()->envcfg, ODEV_FLAG_EnHEXViewMode);
	if (cmode == 0xff){
		SendCMUXCMD("standard PCS frame:\n",cgTxUIH.CreateStdPSCFrame(&strCMD),"!'nullptr",&cBuffer.array,2000,100);
	}
	else{
		SendCMUXCMD("Telit PCS frame, mode " + Str_ToStr(cmode) + ":\n",cgTxUIH.CreateTPSCxFrame(&strCMD,cmode),"!'nullptr",&cBuffer.array,2000,100);
	}
	cBuffer.RemoveSelf();
};
//------------------------------------------------------------------------------------------//
void _CMUXDriver::SendFCC(bool32 blIsOn){
	STDSTR	strCMD;
	SBUF	cBuffer;
	cBuffer.array.InitSize(128);
	cBuffer.array.Empty();
	cgCDevBus->RxDataShareTo(&cBuffer);
	B_SetFLAG64(cgCDevBus->GetODevBus()->envcfg, ODEV_FLAG_EnHEXViewMode);
	if (blIsOn){
		SendCMUXCMD("FCon frame:\n",cgTxUIH.CreateFCONFrame(&strCMD,CR_BIT),"!'nullptr",&cBuffer.array,2000,100);
	}
	else{
		SendCMUXCMD("FCoff frame:\n",cgTxUIH.CreateFCOFFFrame(&strCMD,CR_BIT),"!'nullptr",&cBuffer.array,2000,100);
	}
	cBuffer.RemoveSelf();
};
//------------------------------------------------------------------------------------------//
void _CMUXDriver::SendMSC(int32 dlci,bool32 blDTR,bool32 blRTS){
	VCOM	*vcom;
	STDSTR	strCMD,strTitle;
	uint8	modemStatus;
	SBUF	cBuffer;
	cBuffer.array.InitSize(128);
	cBuffer.array.Empty();
	cgCDevBus->RxDataShareTo(&cBuffer);
	B_SetFLAG64(cgCDevBus->GetODevBus()->envcfg, ODEV_FLAG_EnHEXViewMode);
	modemStatus = 0;
	
	vcom = GetVCOM(dlci);
	strTitle = "MSC CMD, Set DTR=";
	if (blDTR == 0){
		modemStatus |= MS_DTR;
		strTitle += '0';
		CORE_VCOM::Update_DTR(vcom,0);
	}
	else{
		strTitle += '1';
		CORE_VCOM::Update_DTR(vcom,1);
	}
	strTitle += " & RTS=";
	if (blRTS == 0)	{
		modemStatus |= MS_RTS;
		strTitle += '0';
		CORE_VCOM::Update_RTS(vcom,0);
	}
	else{
		strTitle += '1';
		CORE_VCOM::Update_RTS(vcom,1);
	}
	strTitle += " on DLCI=";
	strTitle += Str_ToStr(dlci);
	strTitle += "::\n";
	
	SendCMUXCMD(strTitle,cgTxUIH.CreateMSCCmd(&strCMD,dlci,modemStatus,CR_BIT,CR_BIT),"!'nullptr",&cBuffer.array,2000,300);
	cBuffer.RemoveSelf();
};

//------------------------------------------------------------------------------------------//
uint32 _CMUXDriver::Send(VCOM* vcom,const UVIn& _in){
	uint32	num;
	num = 0;
	cgTxUIH.cgLock.Set();
	cgTxUIH.SetUIHFrame(&num,nullptr,_GetCoreVCOM(vcom)->cgPortID,_in,CR_BIT);
	cgTxUIH.cgLock.Clr();
	return(num);
};
//------------------------------------------------------------------------------------------//
void _CMUXDriver::Send3Pluse(VCOM* vcom){
	STDSTR	strHexFrame;
	uint8	modemStatus = 0;
	
	COMMU_ODEVBUS::PrintWithDividingLine(unitTeam.odevBus,"CMUXDriver::Received +++ from DLCI",Str_ToStr(_GetCoreVCOM(vcom)->cgPortID),", switch to DTR control");
	
	if (CORE_VCOM::Check_RTS(vcom) == G_FALSE)
		modemStatus |= MS_RTS;
	
	cgTxUIH.CreateMSCCmd(&strHexFrame,_GetCoreVCOM(vcom)->cgPortID,modemStatus,CR_BIT,CR_BIT);
	COMMU_ODEVBUS::PrintWithTime(unitTeam.odevBus,COL_clDCyan,"CMUXDriver::Send MSC confirm frame:\n",strHexFrame,"\n");
	
	SYS_SleepMS(200);
	
	modemStatus |= MS_DTR;
	
	cgTxUIH.CreateMSCCmd(&strHexFrame,_GetCoreVCOM(vcom)->cgPortID,modemStatus,CR_BIT,CR_BIT);
	COMMU_ODEVBUS::PrintWithTime(unitTeam.odevBus,COL_clDCyan,"CMUXDriver::Send MSC confirm frame:\n",strHexFrame,"\n");
	SYS_SleepMS(200);
};
//------------------------------------------------------------------------------------------//
#endif /* CMUX_h */
