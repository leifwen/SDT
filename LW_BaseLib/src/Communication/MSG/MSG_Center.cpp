//
//  MSG_Center.cpp
//  SDT
//
//  Created by Leif Wen on 14/03/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "stdafx.h"
#include "MSG_ID.h"
#include "MSG_Center.h"
#ifdef MSG_Center_h
//------------------------------------------------------------------------------------------//
//#define LOGPRINT_ENABLE
//#define LOGTHREAD_ENABLE
#include "SYS_Log.h"
//------------------------------------------------------------------------------------------//
MSG_BASE::MSG_BASE(void) : TNFP(){
	cgMSGID = MESG_NONE;
};
//------------------------------------------------------------------------------------------//
CMDID MSG_BASE::MessageProcessing(CMD_ENV* env,const uint32& mID,const STDSTR& msg,void* commu){
	CMDID	retCode;
	retCode = MESG_NONE;
	TREE_LChildRChain_Traversal_LINE_nolock(MSG_BASE,this,
		retCode = _opNode->MessageProcessing(env,mID,msg,commu);
		if (retCode != MESG_NONE)
			break;
	);
	return(retCode);
};
//------------------------------------------------------------------------------------------//
void MSG_BASE::Reset(void* commu){
	TREE_LChildRChain_Traversal_LINE_nolock(MSG_BASE,this,_opNode->Reset(commu););
};
//------------------------------------------------------------------------------------------//
void MSG_BASE::Close(void* commu){
	TREE_LChildRChain_Traversal_LINE_nolock(MSG_BASE,this,_opNode->Close(commu););
};
//------------------------------------------------------------------------------------------//
CMDID MSG_BASE::GetMSGID(void){
	return(cgMSGID);
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
MSG_CENTER::MSG_CENTER(void) : MSG_BASE(){
	cgMSGID = MESG_NONE;
	cgT0.SetPackageDlyMS(CSSL_T0::MAX_PACKAGE_DEALYTM * 3);
	cgT0.Reset();
	cgT1CtrlCH.Init(&cgT0, 0);
	cgT1DataCH.Init(&cgT0, 1);
	
	t0Thread.ThreadInit			(this,&MSG_CENTER::T0ThreadFun		,"T0");
	t1CtrlRxThread.ThreadInit	(this,&MSG_CENTER::T1CtrlRxThreadFun	,"T1CRx");
	t1DataRxThread.ThreadInit	(this,&MSG_CENTER::T1DataRxThreadFun	,"T1DRx");
	t1DataTxThread.ThreadInit	(this,&MSG_CENTER::T1DataTxThreadFun	,"T1DTx");
	
	t0Thread.Disable();
	t1CtrlRxThread.Disable();
	t1DataRxThread.Disable();
	t1DataTxThread.Disable();
	
	cgTxIn = nullptr;
	cgRxOut = nullptr;
	cgENV = nullptr;
	
	TNFP::SetSelfName		("SSL");
	cgT0.SetSelfName		("T0");
	cgT1CtrlCH.SetSelfName	("T1C");
	cgT1DataCH.SetSelfName	("T1D");
	SetFatherName("");
};
//------------------------------------------------------------------------------------------//
MSG_CENTER::~MSG_CENTER(void){
	SSLDisable();
	t0Thread.RemoveSelf();
	t1CtrlRxThread.RemoveSelf();
	t1DataRxThread.RemoveSelf();
	t1DataTxThread.RemoveSelf();
};
//------------------------------------------------------------------------------------------//
void MSG_CENTER::Init(const ARRAY* _txout,const ARRAY* _rxin,const SBUF* _rxout,const SBUF* _txin){
	cgTxIn = (SBUF*)_txin;
	cgRxOut = (SBUF*)_rxout;
	cgT0.InitPN(_txout,_rxin, G_ENDIAN_LITTLE);
};
//------------------------------------------------------------------------------------------//
void MSG_CENTER::Init(CMD_ENV* env,SYS_Thread_List* list){
	cgENV = env;
	if (list != nullptr)
		*list < t0Thread < t1CtrlRxThread < t1DataRxThread < t1DataTxThread;
}
//------------------------------------------------------------------------------------------//
void MSG_CENTER::SetSelfName(const STDSTR& strName){
	selfName = strName;
	cgT0.SetFatherName(GetFullName(this));
	cgT1CtrlCH.SetFatherName(GetFullName(this));
	cgT1DataCH.SetFatherName(GetFullName(this));
	t0Thread.SetFatherName(GetFullName(this));
	t1CtrlRxThread.SetFatherName(GetFullName(this));
	t1DataRxThread.SetFatherName(GetFullName(this));
	t1DataTxThread.SetFatherName(GetFullName(this));
}
//------------------------------------------------------------------------------------------//
void MSG_CENTER::SetFatherName(const STDSTR& strName){
	fatherName = strName;
	cgT0.SetFatherName(GetFullName(this));
	cgT1CtrlCH.SetFatherName(GetFullName(this));
	cgT1DataCH.SetFatherName(GetFullName(this));
	t0Thread.SetFatherName(GetFullName(this));
	t1CtrlRxThread.SetFatherName(GetFullName(this));
	t1DataRxThread.SetFatherName(GetFullName(this));
	t1DataTxThread.SetFatherName(GetFullName(this));
};
//------------------------------------------------------------------------------------------//
void MSG_CENTER::Reset(void* commu){
	cgT0.SetPackageDlyMS(CSSL_T0::MAX_PACKAGE_DEALYTM * 3);
	cgT0.Reset();
	
	cgT1CtrlCH.Reset();
	cgT1DataCH.Reset();
	
	ClrSFlag(HS_blHandshakeY | HS_blREQClose);
	MSG_BASE::Reset(commu);
};
//------------------------------------------------------------------------------------------//
void MSG_CENTER::Empty(void){
	cgT1CtrlCH.Empty();
	cgT1DataCH.Empty();
};
//------------------------------------------------------------------------------------------//
void MSG_CENTER::SetConfig(uint32 cfg,const STDSTR& sKey){
	cgT0.SetConfig(cfg,sKey);
};
//------------------------------------------------------------------------------------------//
void MSG_CENTER::SetKey(const STDSTR& sKey){
	cgT0.SetKey(sKey);
};
//------------------------------------------------------------------------------------------//
void MSG_CENTER::Register(MSG_BASE* msg){
	AddNode(msg);
	((MSG_NODE*)msg)->Join(this);
};
//------------------------------------------------------------------------------------------//
void MSG_CENTER::TxBlock(bool32 blBlock){
	if (blBlock == G_FALSE){
		ClrSFlag(MSGC_blEnableTxBlock);
		t1DataTxThread.Enable();
	}
	else{
		SetSFlag(MSGC_blEnableTxBlock);
		t1DataTxThread.Disable();
	}
};
//------------------------------------------------------------------------------------------//
void MSG_CENTER::RxBlock(bool32 blBlock){
	if (blBlock == G_FALSE){
		ClrSFlag(MSGC_blEnableRxBlock);
		t1DataRxThread.Enable();
	}
	else{
		SetSFlag(MSGC_blEnableRxBlock);
		t1DataRxThread.Disable();
	}
};
//------------------------------------------------------------------------------------------//
void MSG_CENTER::SSLEnable(void){
	t1CtrlRxThread.Enable();
	if (CheckSFlag(MSGC_blEnableTxBlock) == G_FALSE)
		t1DataTxThread.Enable();
	if (CheckSFlag(MSGC_blEnableRxBlock) == G_FALSE)
		t1DataRxThread.Enable();
};
//------------------------------------------------------------------------------------------//
void MSG_CENTER::SSLDisable(void){
	t0Thread.ThreadStop();
	t1CtrlRxThread.ThreadStop();
	t1DataRxThread.ThreadStop();
	t1DataTxThread.ThreadStop();
	
	t0Thread.Disable();
	t1CtrlRxThread.Disable();
	t1DataRxThread.Disable();
	t1DataTxThread.Disable();
};
//------------------------------------------------------------------------------------------//
void MSG_CENTER::SSLThreadRun(void){
	t0Thread.ThreadRun();
	t1CtrlRxThread.ThreadRun();
	t1DataRxThread.ThreadRun();
	t1DataTxThread.ThreadRun();
};
//------------------------------------------------------------------------------------------//
void MSG_CENTER::ThreadRun(void){
	t0Thread.Enable();
	SSLEnable();
	t0Thread.ThreadRun();
	t1CtrlRxThread.ThreadRun();
	t1DataRxThread.ThreadRun();
	t1DataTxThread.ThreadRun();
}
//------------------------------------------------------------------------------------------//
void MSG_CENTER::SetblREQClose(void){
	SetSFlag(HS_blREQClose);
}
//------------------------------------------------------------------------------------------//
void MSG_CENTER::SetblREQHS(void){
	SetSFlag(HS_blHandshakeY);
}
//------------------------------------------------------------------------------------------//
bool32 MSG_CENTER::ChkblREQClose(void){
	return(CheckSFlag(HS_blREQClose));
}
//------------------------------------------------------------------------------------------//
bool32 MSG_CENTER::ChkblREQHS(void){
	return(CheckSFlag(HS_blHandshakeY));
}
//------------------------------------------------------------------------------------------//
bool32 MSG_CENTER::T0ThreadFun(void* p){
	
	ETLogThreadStart(t0Thread);
	
	while(t0Thread.IsTerminated() == 0){
		
		cgT0.Delivery();
		
		SYS_SleepMS(2);
	}
	ETLogThreadStop(t0Thread);
	return G_TRUE;
}
//------------------------------------------------------------------------------------------//
bool32 MSG_CENTER::T1CtrlRxThreadFun(void* commu){
	IOSTATUS	ios;
	STDSTR		strMesg;
	uint32		mID;
	
	ETLogThreadStart(t1CtrlRxThread);
	while(t1CtrlRxThread.IsTerminated() == 0){
		IOSTATUS_Clr(&ios);
		if (Read(&ios,&mID,_EMPTY(&strMesg)) > 0){
			ELog(ELogThread(t1CtrlRxThread,"ios:in/out=" << ios.total_in << "/" << ios.total_out <<"\r\n"));
			ELog(ELogThread(t1CtrlRxThread,"Received: " << _GetMesgText(mID) << ", MLen=" << strMesg.length()));
		}
		MessageProcessing(cgENV,mID,strMesg,commu);
		SYS_SleepMS(2);
	}
	
	ETLogThreadStop(t1CtrlRxThread);
	return G_TRUE;
}
//------------------------------------------------------------------------------------------//
bool32 MSG_CENTER::T1DataRxThreadFun(void* commu){
	IOSTATUS	ios;
	
	ETLogThreadStart(t1DataRxThread);
	
	while(t1DataRxThread.IsTerminated() == 0){
		IOSTATUS_Clr(&ios);
		if (Read(&ios,&cgRxOut->GetArray()) > 0){
			ELog(ELogThread(t1DataRxThread,"ios:in/out=" << ios.total_in << "/" << ios.total_out <<"\r\n"));
		}
		SYS_SleepMS(2);
	}
	
	ETLogThreadStop(t1DataRxThread);
	return G_TRUE;
}
//------------------------------------------------------------------------------------------//
bool32 MSG_CENTER::T1DataTxThreadFun(void* commu){
	IOSTATUS	ios;
	
	ETLogThreadStart(t1DataTxThread);
	
	while(t1DataTxThread.IsTerminated() == 0){
		IOSTATUS_Clr(&ios);
		if (cgTxIn->Used() > 0){
			if (Send(&ios,IUD(cgTxIn->GetArray())) > 0){
				ELog(ELogThread(t1DataTxThread,"ios:in/out=" << ios.total_in << "/" << ios.total_out <<"\r\n"));
				cgTxIn->Out((uint32)ios.total_in);
			}
		}
		SYS_SleepMS(2);
	}
	
	ETLogThreadStop(t1DataTxThread);
	return G_TRUE;
}
//------------------------------------------------------------------------------------------//
void MSG_CENTER::Delivery(void){
	cgT0.Delivery();
}
//------------------------------------------------------------------------------------------//
bool32 MSG_CENTER::Send(IOSTATUS* _ios,uint32 mID,const UVIn& _in){
	return(cgT1CtrlCH.Send(_ios,mID,_in));
};
//------------------------------------------------------------------------------------------//
uint32 MSG_CENTER::Read(IOSTATUS* _ios,uint32* mID,const UVOut& msg){
	return(cgT1CtrlCH.Read(_ios,mID,msg));
};
//------------------------------------------------------------------------------------------//
bool32 MSG_CENTER::Send(IOSTATUS* _ios,const UVIn& _in){
	return(cgT1DataCH.Send(_ios,_in));
};
//------------------------------------------------------------------------------------------//
uint32 MSG_CENTER::Read(IOSTATUS* _ios,const UVOut& _out){
	return(cgT1DataCH.Read(_ios,_out));
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
MSG_NODE::MSG_NODE(uint32 size,void* p) : MSG_BASE(){
	cgMSGID = MESG_NONE;
	cgMsgCenter = nullptr;
	
	TNFP::SetSelfName("MSG_NODE");
	SetFatherName("");
};
//------------------------------------------------------------------------------------------//
void MSG_NODE::SetSelfName(const STDSTR& strName){
	selfName = strName;
	TREE_LChildRChain_Traversal_LINE_nolock(MSG_NODE,this,_opNode->SetFatherName(GetFullName(this)););
}
//------------------------------------------------------------------------------------------//
void MSG_NODE::SetFatherName(const STDSTR& strName){
	fatherName = strName;
	TREE_LChildRChain_Traversal_LINE_nolock(MSG_NODE,this,_opNode->SetFatherName(GetFullName(this)););
};
//------------------------------------------------------------------------------------------//
void MSG_NODE::Join(MSG_CENTER* msgCenter){
	if (cgMsgCenter == nullptr){
		cgMsgCenter = msgCenter;
		cgMsgCenter->Register(this);
		TREE_LChildRChain_Traversal_LINE_nolock(MSG_NODE,this,_opNode->cgMsgCenter = msgCenter;);
	}
}
//------------------------------------------------------------------------------------------//
void MSG_NODE::SetConfig(uint32 cfg,const STDSTR& sKey){
	cgMsgCenter->SetConfig(cfg, sKey);
};
//------------------------------------------------------------------------------------------//
void MSG_NODE::SetKey(const STDSTR& sKey){
	cgMsgCenter->SetKey(sKey);
};
//------------------------------------------------------------------------------------------//
bool32 MSG_NODE::Send(uint32 mID,const UVIn& _in){
	
	return(cgMsgCenter->Send(nullptr,mID,_in));
};
//------------------------------------------------------------------------------------------//
bool32 MSG_NODE::Send(CMD_ENV* env,uint32 mID,const UVIn& _in,const uint64& flagY,const uint64& flagN,const uint32& waitTimeS){
	SYS_TIME_S		timedly;
	bool32			err;
	
	if (CMD_NODE::ChkblExit(env)){
		err = -1;
	}
	else{
		SYS_Delay_SetTS(&timedly, waitTimeS << 10);
		ClrSFlag(flagY | flagN);
		
		err = Send(mID,_in);
		if (err == G_FALSE){
			err = -3;
		}
		else{
			err = Check(env,flagY,flagN,timedly);
		}
	}
	return(err);
}
//------------------------------------------------------------------------------------------//
bool32 MSG_NODE::Check(CMD_ENV* env,const uint64& flagY,const uint64& flagN,SYS_TIME_S& timedly){
	
	while((CheckSFlag(flagY | flagN) == 0) && (CMD_NODE::ChkblExit(env) == G_FALSE) && (SYS_Delay_CheckTS(&timedly) == 0))
		SYS_SleepMS(2);
	if (CheckSFlag(flagY) > 0){
		return G_TRUE;
	}
	else if (CheckSFlag(flagN) > 0){
		return G_FALSE;
	}
	else if (CMD_NODE::ChkblExit(env)){
		return -1;
	}
	return -2;
}
//------------------------------------------------------------------------------------------//
#endif /* MSG_Center_h */
