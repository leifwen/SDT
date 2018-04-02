//
//  Commu_DBuf.hpp
//  SDT
//
//  Created by Leif Wen on 02/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#ifndef Commu_DBuf_hpp
#define Commu_DBuf_hpp
//------------------------------------------------------------------------------------------//
#include "Commu_DBuf.h"
#ifdef Commu_DBuf_h
//------------------------------------------------------------------------------------------//
static inline OPEN_PAR& SetOpenPar(OPEN_PAR* op,uint32 type,const STDSTR& name,uint32 port,uint32 cfg){
	op->type = type;
	op->name = name;
	op->port = port;
	op->cfg = cfg;
	return(*op);
};
//------------------------------------------------------------------------------------------//
static inline OPEN_PAR SetOpenPar(uint32 type,const STDSTR& name,uint32 port,uint32 cfg){
	return{type,name,port,cfg};
};
//------------------------------------------------------------------------------------------//
static inline OPEN_PAR& SetOpenPar(OPEN_PAR* op,const OPEN_PAR& op2){
	op->type = op2.type;
	op->name = op2.name;
	op->port = op2.port;
	op->cfg = op2.cfg;
	return(*op);
};
//------------------------------------------------------------------------------------------//











//------------------------------------------------------------------------------------------//
inline			bool32		COMMU_FRAME::IsOpened			(void)const						{return(CheckSFlag(CF_blOpened));};
inline			bool32		COMMU_FRAME::IsConnected		(void)const						{return(CheckSFlag(CF_blConnected));};
inline			void		COMMU_FRAME::ClrblPushSend		(void)							{		ClrSFlag(CF_blPushSend);};
inline			bool32		COMMU_FRAME::ChkblPushSend		(void)const						{return(CheckSFlag(CF_blPushSend));};
inline			void		COMMU_FRAME::EnableEcho			(void)							{		B_SetFLAG32(cgOpenPar.cfg,CFG_blEnableEcho);};
inline			void		COMMU_FRAME::DisableEcho		(void)							{		B_ClrFLAG32(cgOpenPar.cfg,CFG_blEnableEcho);};
inline			bool32		COMMU_FRAME::CheckEcho			(void)const						{return(B_ChkFLAG32(cgOpenPar.cfg,CFG_blEnableEcho));};
inline			void		COMMU_FRAME::EnableCloseCouple	(void)							{		SetSFlag(CF_blEnCloseCouple);};
//------------------------------------------------------------------------------------------//
inline			bool32		COMMU_FRAME::OpenDev		(const OPEN_PAR& par)			{return G_FALSE;};
inline			void		COMMU_FRAME::CloseDev		(void)							{;};
inline			uint32		COMMU_FRAME::Send			(const UVIn&  _in)				{return(Put(nullptr,_in));};
inline			uint32		COMMU_FRAME::Read			(const UVOut& _out,uint32 num)	{return(Get(nullptr,_out,num));};
//------------------------------------------------------------------------------------------//
inline			bool32		COMMU_FRAME::ThreadsStart	(void)							{DoThreadsStart();return G_TRUE;};
inline			void		COMMU_FRAME::ThreadsStop	(void)							{cgThreadList.ThreadStop();};
//------------------------------------------------------------------------------------------//
inline	const	STDSTR&		COMMU_FRAME::GetDevName		(void)const	 					{return(cgDevName);};
inline	const	OPEN_PAR&	COMMU_FRAME::GetOpenPar		(void)const	 					{return(cgOpenPar);};
inline	const	void		COMMU_FRAME::SetTypePar		(uint32 type)					{cgOpenPar.type = type;};
//------------------------------------------------------------------------------------------//
inline void COMMU_FRAME::SetSelfName(const STDSTR& strName){
	selfName = strName;
	selfCloseThread.SetFatherName(GetFullName(this));
};
//------------------------------------------------------------------------------------------//
inline void COMMU_FRAME::SetFatherName(const STDSTR& strName){
	fatherName = strName;
	selfCloseThread.SetFatherName(GetFullName(this));
};
//------------------------------------------------------------------------------------------//









//------------------------------------------------------------------------------------------//
inline void COMMU_FRAME_FW::SetSelfName(const STDSTR& strName){
	COMMU_FRAME::SetSelfName(strName);
	fwThread.SetFatherName(GetFullName(this));
};
//------------------------------------------------------------------------------------------//
inline void COMMU_FRAME_FW::SetFatherName(const STDSTR& strName){
	COMMU_FRAME::SetFatherName(strName);
	fwThread.SetFatherName(GetFullName(this));
};
//------------------------------------------------------------------------------------------//
inline DBUF* COMMU_FRAME_FW::AddChild(DBUF* sdb){
	COMMU_FRAME::AddChild(sdb);
	static_cast<COMMU_FRAME_FW*>(sdb)->UseExternalFwSBufList(GetFwSBufList());
	return(sdb);
};
//------------------------------------------------------------------------------------------//
inline void	COMMU_FRAME_FW::RemoveSelf(void){
	UseSelfFwSBufList();
	COMMU_FRAME::RemoveSelf();
};
//------------------------------------------------------------------------------------------//
inline const TNFP* COMMU_FRAME_FW::GetFwSBufList(void){
	
	return(rxFwSBufList);
};
//------------------------------------------------------------------------------------------//
inline void COMMU_FRAME_FW::UseSelfFwSBufList(void){
	InUse_set();
	rxFwSBufList = &cgInternalFwSBufList;
	InUse_clr();
}
//------------------------------------------------------------------------------------------//
inline void COMMU_FRAME_FW::UseExternalFwSBufList(const TNFP* tRxFwSBufList){
	InUse_set();
	rxFwSBufList = (TNFP*)tRxFwSBufList;
	InUse_clr();
}
//------------------------------------------------------------------------------------------//
inline void COMMU_FRAME_FW::RxDataShareTo(SBUF* tSBUF){
	InUse_set();
	if (rxFwSBufList != nullptr)
		rxFwSBufList->AddNode(tSBUF);
	InUse_clr();
}
//------------------------------------------------------------------------------------------//
inline void COMMU_FRAME_FW::SetGetDataByRead(void){
	fwThread.Disable();
	fwThread.ThreadStop();
};
//------------------------------------------------------------------------------------------//
inline void COMMU_FRAME_FW::SetGetDataByShare(void){
	fwThread.Enable();
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
inline	ODEV_SYSTEM*	COMMU_FRAME_LOGSYS::GetLogSystem	(void)const				{return(logSystem);};
inline	ODEV_VG3D*		COMMU_FRAME_LOGSYS::GetVG3D			(void)const				{return(cgVG3D);};
inline	void			COMMU_FRAME_LOGSYS::SetAttrTitle	(const STDSTR& strIn)	{cgAttrTitle = strIn;};
//------------------------------------------------------------------------------------------//
inline	void COMMU_FRAME_LOGSYS::DoPrintOnOpenSuccess		(void){PrintOpenSuccessReport();};
inline	void COMMU_FRAME_LOGSYS::DoPrintOnOpenFail			(void){PrintOpenFailReport();};
inline	void COMMU_FRAME_LOGSYS::DoPrintOnClose				(void){PrintConnectionReport("User closed the connection");};
//------------------------------------------------------------------------------------------//
template<typename... Args>inline void COMMU_FRAME_LOGSYS::PrintStr(const Args&... args){
#ifdef ODEV_System_h
	OUTPUT_NODE::PrintStr((OUTPUT_NODE*)cgVG3D,args...);
#endif
};
//------------------------------------------------------------------------------------------//
template<typename... Args>inline void COMMU_FRAME_LOGSYS::PrintStrNL(const Args&... args){
#ifdef ODEV_System_h
	OUTPUT_NODE::PrintStrNL((OUTPUT_NODE*)cgVG3D,args...);
#endif
};
template<typename... Args>inline void COMMU_FRAME_LOGSYS::PrintALine(const Args&... args){
#ifdef ODEV_System_h
		OUTPUT_NODE::PrintALine((OUTPUT_NODE*)cgVG3D,args...);
#endif
};
//------------------------------------------------------------------------------------------//
template<typename... Args>inline void COMMU_FRAME_LOGSYS::PrintWithTime(const Args&... args){
#ifdef ODEV_System_h
	OUTPUT_NODE::PrintWithTime((OUTPUT_NODE*)cgVG3D,args...);
#endif
};
//------------------------------------------------------------------------------------------//
template<typename... Args>inline void COMMU_FRAME_LOGSYS::PrintWithDividingLine(const Args&... args){
#ifdef ODEV_System_h
	OUTPUT_NODE::PrintWithDividingLine((OUTPUT_NODE*)cgVG3D,args...);
#endif
};
//------------------------------------------------------------------------------------------//
template<typename... Args>inline void COMMU_FRAME_LOGSYS::PrintMessageDot(const Args&... args){
#ifdef ODEV_System_h
	OUTPUT_NODE::PrintMessageDot((OUTPUT_NODE*)cgVG3D,args...);
#endif
};
//------------------------------------------------------------------------------------------//
template<typename... Args>inline void COMMU_FRAME_LOGSYS::PrintWarningMessageDot(const Args&... args){
#ifdef ODEV_System_h
	OUTPUT_NODE::PrintWarningMessageDot((OUTPUT_NODE*)cgVG3D,args...);
#endif
};
//------------------------------------------------------------------------------------------//
template<typename... Args> inline void COMMU_FRAME_LOGSYS::PrintConnectionReport(const Args&... args){
#ifdef ODEV_System_h
	PrintConnectionReport((OUTPUT_NODE*)cgVG3D,args...);
#endif
}
//------------------------------------------------------------------------------------------//
template<typename... Args> void COMMU_FRAME_LOGSYS::PrintConnectionReport(OUTPUT_NODE *node,const Args&... args){
#ifdef ODEV_System_h
	if (node != nullptr){
		*node << Begin() << DefGroup() << NL()
		<< COL_DivLineTime
		<< COL_NormalMessage;
		
		DoPrintStr(node,args...)
		<< ".\n"
		<< COL_DB_Statistics
		<< "Received : " << Str_ToStr(RxBytes()) << " bytes\n"
		<< "Echo     : " << Str_ToStr(FwBytes()) << " bytes\n"
		<< "Sent     : " << Str_ToStr(TxBytes()) << " bytes\n"
		<< COL_DivLine << DIVIDING_LINE
		<< Endl();
	}
#endif
}
//------------------------------------------------------------------------------------------//
inline void COMMU_FRAME_LOGSYS::PrintOpenSuccessReport(const STDSTR& strTitle){
	
	PrintMessageDot("Successfully connect to",cgAttrTitle,strTitle,GetDevName());
};
//------------------------------------------------------------------------------------------//
inline void COMMU_FRAME_LOGSYS::PrintOpenFailReport(const STDSTR& strTitle){
	
	PrintMessageDot("Failed connect to",strTitle,GetDevName());
};
//------------------------------------------------------------------------------------------//




















//------------------------------------------------------------------------------------------//
inline void COMMU_THREAD::SetSelfName(const STDSTR& strName){
	COMMU_FRAME_LOGSYS::SetSelfName(strName);
	rxThread.SetFatherName(GetFullName(this));
	txThread.SetFatherName(GetFullName(this));
	logThread.SetFatherName(GetFullName(this));
};
//------------------------------------------------------------------------------------------//
inline void COMMU_THREAD::SetFatherName(const STDSTR& strName){
	COMMU_FRAME_LOGSYS::SetFatherName(strName);
	rxThread.SetFatherName(GetFullName(this));
	txThread.SetFatherName(GetFullName(this));
	logThread.SetFatherName(GetFullName(this));
};
//------------------------------------------------------------------------------------------//
inline bool32 COMMU_THREAD::SendToDevice(uint32* retNum,const uint8* buffer,uint32 length){
	*retNum = 0;
	return 0;
};
//------------------------------------------------------------------------------------------//
inline bool32 COMMU_THREAD::ReadFromDevice(uint32* retNum,uint8* buffer,uint32 length){
	*retNum = 0;
	return 0;
};
//------------------------------------------------------------------------------------------//










#ifdef ADS_SSL_h
//------------------------------------------------------------------------------------------//
static inline OPEN_PAR SetOpenSSL(uint32 type,const STDSTR& name,uint32 port){
	return{type,name,port,COMMU_SSL::CFG_blEnableSSL};
};
//------------------------------------------------------------------------------------------//
inline void COMMU_SSL::SetSelfName(const STDSTR& strName){
	COMMU_THREAD::SetSelfName(strName);
	cgMsgCenter.SetFatherName(GetFullName(this));
	cgMsgHandshake.SetFatherName(GetFullName(this));
};
//------------------------------------------------------------------------------------------//
inline void COMMU_SSL::SetFatherName(const STDSTR& strName){
	COMMU_THREAD::SetFatherName(strName);
	cgMsgCenter.SetFatherName(GetFullName(this));
	cgMsgHandshake.SetFatherName(GetFullName(this));
};
//------------------------------------------------------------------------------------------//
#endif









#ifdef ADS_SSL_h
//------------------------------------------------------------------------------------------//
template <typename T_COMMU,typename T_MSG>
COMMU_MSG<T_COMMU,T_MSG>::COMMU_MSG(uint32 size,const ODEV_SYSTEM* logSys) :
T_COMMU(size,logSys),cgMsg(size,(void*)logSys)
{
	this->cgMsgCenter.Register(&cgMsg);
	
	SetFatherName("");
};
//------------------------------------------------------------------------------------------//
template <typename T_COMMU,typename T_MSG> inline void COMMU_MSG<T_COMMU,T_MSG>::SetSelfName(const STDSTR& strName){
	T_COMMU::SetSelfName(strName);
	cgMsg.SetFatherName(this->GetFullName(this));};
//------------------------------------------------------------------------------------------//
template <typename T_COMMU,typename T_MSG> inline void COMMU_MSG<T_COMMU,T_MSG>::SetFatherName(const STDSTR& strName){
	T_COMMU::SetFatherName(strName);
	cgMsg.SetFatherName(this->GetFullName(this));
};
//------------------------------------------------------------------------------------------//
template <typename T_COMMU,typename T_MSG> MSG_NODE* COMMU_MSG<T_COMMU,T_MSG>::GetMSG(CMDID mID){
	MSG_NODE* ret = nullptr;
	if (cgMsg.GetMSGID() == mID){
		ret = &cgMsg;
	}
	else{
		TREE_LChildRChain_Traversal_LINE_nolock(MSG_NODE,(&cgMsg),
			if (_opNode->GetMSGID() == mID){
				ret = _opNode;
				break;
			}
		);
	}
	return(ret);
};
//------------------------------------------------------------------------------------------//
#endif









//------------------------------------------------------------------------------------------//
template <typename T_COMMU> COMMU_POOL<T_COMMU>::COMMU_POOL(uint32 size,const ODEV_SYSTEM* logSys) : COMMU_FRAME(size){
	SetSFlag(CF_blNoInitSize);
	logSystem = (ODEV_SYSTEM*)logSys;
	SetSelfName("COMMU_POOL");
	SetFatherName("");
};
//------------------------------------------------------------------------------------------//
template <typename T_COMMU> TNFP* COMMU_POOL<T_COMMU>::CreateNode(void){
	return(SetSubNodeFatherName(new T_COMMU(cgMaxSize,logSystem)));
}
//------------------------------------------------------------------------------------------//
template <typename T_COMMU> T_COMMU* COMMU_POOL<T_COMMU>::GetNewChild(void){
	T_COMMU *node = static_cast<T_COMMU*>(GetNewNode());
	AddNode(node);
	return(node);
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
template <typename T_COMMU_A,typename T_COMMU_B>
COMMU_CPPOOL<T_COMMU_A,T_COMMU_B>::COMMU_CPPOOL(uint32 size,const ODEV_SYSTEM* logSys) : TNFP()
,cgPoolA(size,logSys)
,cgPoolB(size,logSys)
{
	TNFP::SetSelfName("COMMU_CPPOOL");
	SetSelfName(selfName);
	cgPoolA.selfName = "PoolA";
	cgPoolB.selfName = "PoolB";
	SetFatherName("");
};
//------------------------------------------------------------------------------------------//
template <typename T_COMMU_A,typename T_COMMU_B>
COMMU_CPPOOL<T_COMMU_A,T_COMMU_B>::~COMMU_CPPOOL(void){
	Close();
};
//------------------------------------------------------------------------------------------//
template <typename T_COMMU_A,typename T_COMMU_B>
void COMMU_CPPOOL<T_COMMU_A,T_COMMU_B>::SetSelfName(const STDSTR& strName){
	selfName = strName;
	cgPoolA.SetFatherName(GetFullName(this));
	cgPoolB.SetFatherName(GetFullName(this));
};
//------------------------------------------------------------------------------------------//
template <typename T_COMMU_A,typename T_COMMU_B>
void COMMU_CPPOOL<T_COMMU_A,T_COMMU_B>::SetFatherName(const STDSTR& strName){
	fatherName = strName;
	cgPoolA.SetFatherName(GetFullName(this));
	cgPoolB.SetFatherName(GetFullName(this));
};
//------------------------------------------------------------------------------------------//
template <typename T_COMMU_A,typename T_COMMU_B>
void COMMU_CPPOOL<T_COMMU_A,T_COMMU_B>::Close(void){
	cgPoolA.Close();
	cgPoolB.Close();
};
//------------------------------------------------------------------------------------------//
template <typename T_COMMU_A,typename T_COMMU_B>
void COMMU_CPPOOL<T_COMMU_A,T_COMMU_B>::CloseChild(T_COMMU_A* cmmuA,T_COMMU_B* cmmuB){
	cgPoolA.CloseChild(cmmuA);
	cgPoolB.CloseChild(cmmuB);
}
//------------------------------------------------------------------------------------------//
template <typename T_COMMU_A,typename T_COMMU_B>
bool32 COMMU_CPPOOL<T_COMMU_A,T_COMMU_B>::GetCP(T_COMMU_A** cmmuA,T_COMMU_B** cmmuB){
	*cmmuA = (T_COMMU_A*)cgPoolA.GetNewChild();
	*cmmuB = (T_COMMU_B*)cgPoolB.GetNewChild();
	if (*cmmuA == nullptr){
		cgPoolB.CloseChild(*cmmuB);
		cmmuB = nullptr;
	}
	else if (cmmuB == nullptr){
		cgPoolA.CloseChild(*cmmuA);
		cmmuA = nullptr;
	}
	else{
		(*cmmuA)->EnableCloseCouple();
		(*cmmuB)->EnableCloseCouple();
		(*cmmuA)->LinkCoupleNode(*cmmuB);
		return G_TRUE;
	}
	return G_FALSE;
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif /* Commu_DBuf_h */
#endif /* Commu_DBuf_hpp */
