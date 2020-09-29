//
//  Commu_SSL.cpp
//  SDT
//
//  Created by Leif Wen on 26/01/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "stdafx.h"
//------------------------------------------------------------------------------------------//
#include "ADS_SSL.h"
//------------------------------------------------------------------------------------------//
#ifdef ADS_SSL_h
//------------------------------------------------------------------------------------------//
#include "ALG_zlib.h"
#include "DS_STRING.h"
#include "SYS_Time.h"
//------------------------------------------------------------------------------------------//
//#define T0_TESTMODE0
//#define LOGPRINT_ENABLE
//#define LOGTHREAD_ENABLE
#include "SYS_Log.h"
//------------------------------------------------------------------------------------------//
#define LOGT0_ENABLE
#define LOGMEM_RE_ENABLE
#define LOGMEM_WR_ENABLE
#define LOGT1_RX_ENABLE
#define LOGT1_TX_ENABLE
//------------------------------------------------------------------------------------------//

#ifdef LOGPRINT_ENABLE
	#define ELogBLK1_SIZE		26
	#define ELogBLK2_SIZE		16
	#define ELogBLK3_SIZE		(22+8+8)
	#define	LogIDS(x,y,z)		LogFillSpace(22,"CID=",_GetPNFBT1CtrlIDMesg(x)) << LogFillSpace(8,"BID=",y) << LogFillSpace(8,",OID=",z)
	template<typename T,typename... Args>STDSTR	LogBLK1(const T& first,const Args&... args){return(LogFillSpace(ELogBLK1_SIZE,first,args...));};
	template<typename T,typename... Args>STDSTR	LogBLK2(const T& first,const Args&... args){return(LogFillSpace(ELogBLK2_SIZE,first,args...));};
	template<typename T,typename... Args>STDSTR	LogBLK3(const T& first,const Args&... args){return(LogFillSpace(ELogBLK3_SIZE,first,args...));};
	#define	LogBuffer(x,y)		LogBLK1("Buffer      :",x,"/",y)
	#define	LogPage(_page)		LogBLK1("Buffer[",TNF::GetHEXUniqueID(_page),"]:",_page->array.Used(),"/",_page->array.Unused())
	#define	LogMLEN1(x)			LogBLK2("Mlen=",x)
	#define	LogMLEN2(x,y)		LogBLK2("Mlen=",x,"/",y)
#else
	#define	LogIDS(x,y,z);
	#define	LogBuffer(x,y);
	#define	LogPage(_page);
#endif

#define	ELogTNFP_TORX	"ALL"
#define	ELogTNFP_TOTX	"ALL"

#define	ELogTNFP_MEM_RE	"ALL"
#define	ELogTNFP_MEM_WR	"ALL"

#define	ELogTNFP_T1_RX	"ALL"
#define	ELogTNFP_T1_TX	"ALL"

#ifdef LOGT0_ENABLE
	#define ELogT0(x) 			ELog(x);
#else
	#define ELogT0(x);
#endif

#ifdef LOGMEM_RE_ENABLE
	#define ELogMEM_RE(x)		ELog(x);
#else
	#define ELogMEM_RE(x);
#endif

#ifdef LOGMEM_WR_ENABLE
	#define ELogMEM_WR(x) 		ELog(x);
#else
	#define ELogMEM_WR(x);
#endif

#ifdef LOGT1_RX_ENABLE
	#define ELogT1_RX(x)		ELog(x);
#else
	#define ELogT1_RX(x);
#endif

#ifdef LOGT1_TX_ENABLE
	#define ELogT1_TX(x) 		ELog(x);
#else
	#define ELogT1_TX(x);
#endif
//------------------------------------------------------------------------------------------//
enum{
	CID_Package = 0,
	CID_PackageEnd,
	CID_Finish,
	CID_ACK,
	CID_Resend,
};
//------------------------------------------------------------------------------------------//
STDSTR _GetPNFBT1CtrlIDMesg(uint32 cid){
	STDSTR retStr;
	retStr = "(" + Str_ToStr(cid)+ ")";
	
	switch(cid){
		case CID_Package		:retStr += "CID_Package";	break;
		case CID_PackageEnd		:retStr += "CID_PackageEnd";break;
		case CID_Finish			:retStr += "CID_Finish";	break;
		case CID_ACK			:retStr += "CID_ACK";		break;
		case CID_Resend			:retStr += "CID_Resend";	break;
		default					:retStr = "";				break;
	}
	return(retStr);
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
CSSL_T0::CSSL_T0(void) : DSTF(){
	DSTF::InitSize(MAX_PACKAGE_SIZE);
	cgDefCFG = ALG_AES::CFG_AES256 | ALG_AES::CFG_AES_CFB8;
	cgDefKey = ALG_AESKey32Bye("LF.W.TZ");
	cgPackageDlyMS = MAX_PACKAGE_DEALYMS;
	
	TNFP::SetSelfName("CSSL_T0");
	SetSelfName(selfName);
	SetUpName("");
	Reset();
};
//------------------------------------------------------------------------------------------//
void CSSL_T0::InitPN(const ARRAY* _out,const ARRAY* _in){
	cgMsg.InitPN(_out,_in,1,CRC16ID::eXMODEM,G_ENDIAN_LITTLE);
};
//------------------------------------------------------------------------------------------//
void CSSL_T0::SetSelfName(const STDSTR& strName){
	selfName = strName;
	cgMsg.SetUpName(GetFullName(this));
};
//------------------------------------------------------------------------------------------//
void CSSL_T0::SetUpName(const STDSTR& strName){
	fatherName = strName;
	cgMsg.SetUpName(GetFullName(this));
};
//------------------------------------------------------------------------------------------//
void CSSL_T0::SetConfig(uint32 cfg,const STDSTR& sKey){
	cgSendLock.Set();
	cgMsg.InitCFG(cfg | CFG_INIT_RE_CFGPAR | CFG_INIT_WR_CFGPAR ,&sKey);
	cgSendLock.Clr();
};
//------------------------------------------------------------------------------------------//
void CSSL_T0::SetKey(const STDSTR& sKey){
	cgSendLock.Set();
	cgMsg.InitCFG(CFG_INIT_RE_PAR | CFG_INIT_WR_PAR,&sKey);
	cgSendLock.Clr();
};
//------------------------------------------------------------------------------------------//
void CSSL_T0::Reset(void){
	
	SetConfig(cgDefCFG,cgDefKey);
};
//------------------------------------------------------------------------------------------//
inline void CSSL_T0::Register(TNF* csslT1){
	cgT1SList.AppendDownNode(csslT1);
};
//------------------------------------------------------------------------------------------//
void CSSL_T0::SetPackageDlyMS(double dlyMS){
	static	int32 count = 0;
	cgSendLock.Set();
	cgPackageDlyMS = 50 + (cgPackageDlyMS + dlyMS) / 2;
	if (cgPackageDlyMS > MAX_PACKAGE_DEALYMS){
		++ count;
		cgPackageDlyMS = MAX_PACKAGE_DEALYMS;
	}
	else{
		count = 0;
	}
	if (count > 1){
		cgPackageDlyMS = 200;
		count = 0;
	}
	cgSendLock.Clr();
};
//------------------------------------------------------------------------------------------//
double CSSL_T0::GetPackageDlyMS(void){
	double	dt;
	cgSendLock.Set();
	dt = cgPackageDlyMS;
	cgSendLock.Clr();
	return(dt);
};
//------------------------------------------------------------------------------------------//
bool32 CSSL_T0::Delivery(void){
	bool32		blIsFirstMsg,blDone;
	ELogLock(1);
	blDone = G_FALSE;
	if (cgMsg.GetDefArrayRE()->Used() > 0){
		while((blDone == G_FALSE) && (cgMsg.TryGetFrame() > 0)){
			blIsFirstMsg = G_TRUE;
			do{
				cgArray.Reset();
				cgPort = cgMsg.ReadID();
				ELogTNFP(ELogTNFP_TORX,ELogT0(LogTitle(this,"CSSL_T0","Delivery","::")
											  << LogBuffer(cgMsg.GetDefArrayRE()->Used(),cgMsg.GetDefArrayRE()->Unused())
											  << LogMLEN1(cgMsg.GetLengthRE())
											  << LogBLK3("Get T0 message")
											  << ",Port=" << cgPort));
				if (cgMsg.IsUnread()){
					cgMsg.Read(nullptr,&cgArray);
					switch (Save(nullptr, OUD(this), cgArray)){
						case IOS_NO_MEM:;
							ELogTNFP(ELogTNFP_TORX,ELogT0(LogTitle(this,"CSSL_T0","Delivery","::")
														  << LogBuffer(cgMsg.GetDefArrayRE()->Used(),cgMsg.GetDefArrayRE()->Unused())
														  << LogBLK2("")
														  << LogBLK3("T1 has no enough memory")
														  << ",Port=" << cgPort));
							blIsFirstMsg = G_FALSE;
							break;
						case IOS_NO_OBJECT:;
							ELogTNFP(ELogTNFP_TORX,ELogT0(LogTitle(this,"CSSL_T0","Delivery","::")
														  << LogBuffer(cgMsg.GetDefArrayRE()->Used(),cgMsg.GetDefArrayRE()->Unused())
														  << LogBLK2("")
														  << LogBLK3("Here is no T1 object,abandon ", cgMsg.GetLengthRE(), " bytes")
														  << ",Port=" << cgPort));
						case IOS_ERR:;
						case IOS_OK:;
						case IOS_FINISH:;
						default:
							cgMsg.MarkedRead();
							break;
					};
				}
				if (blIsFirstMsg != G_FALSE){
					cgMsg.OutRE();
				}
				else if (cgMsg.Analysis(cgMsg.GetOffsetRE() + cgMsg.GetLengthRE()) > 0){
					continue;
				}
				else{
					blDone = G_TRUE;
				}
				break;
			}while(1);
		};
	}
	ELogLock(0);
	return(cgMsg.GetDefArrayRE()->Used());
};
//------------------------------------------------------------------------------------------//
IOSE CSSL_T0::DoTransform(IOS* _ios,const UVOut& _out,const uint8* data,const uint64& length){
	IOSE rcode;
	
	rcode = IOS_OK;
	if (length > 0){
		rcode = IOS_NO_OBJECT;
#ifdef T0_TESTMODE0
		TREE_DownChain_Traversal_LINE_nolock
		(TNF,&cgT1SList,
			if (((CSSL_MEMORY*)_opNode)->GetPort() == cgPort){
		 		rcode = ((CSSL_MEMORY*)_opNode)->RecFromT0(data,length);
		 		break;
			}
		);
#else
		TREE_DownChain_Traversal_LINE_nolock
		(TNF,&cgT1SList,
			if (((CSSL_T1RX*)_opNode)->GetPort() == cgPort){
				rcode = ((CSSL_T1RX*)_opNode)->RecFromT0(data,length);
				break;
			}
		);
#endif
	}
	return(rcode);
};
//------------------------------------------------------------------------------------------//
IOSE CSSL_T0::Send(IOS* _ios,uint32 port,const UVIn& _in){
	//CFG_AES_CBC	495/503,505/519,1007/1015,1017/1031
	//CFG_AES_CFB8	495/502,505/512,1007/1014,1017/1024
	ELogLock(1);
#ifdef LOGT0_ENABLE
#define	def_ios		iosp
	IOS	ios;
	IOS	*iosp = &ios;
	IOS_clr(&ios);
#else
#define	def_ios		_ios
#endif
	SYS_TIME_S	tm;
	IOSE	rcode = IOS_OK;
	
	SYS_Delay_SetTS(&tm, GetPackageDlyMS());
	do{
		SYS_SleepMS(2);
		cgSendLock.Set();
		rcode = cgMsg.Write(IOS_reset(def_ios), port, _in);
		cgSendLock.Clr();
	}while((rcode < IOS_OK) && (SYS_Delay_IsTimeout(&tm) == G_FALSE));
#ifdef LOGT0_ENABLE
	CSSL_PNFB_PKG	pnf;
	pnf.InitPN(CSSL_T0::MAX_PACKAGE_SIZE);
	DS_IO_NODE::GetDSIOList().Save(nullptr, pnf.GetDefArrayWR(), _in);
	pnf.Analysis(0);
	
	if (rcode < IOS_OK){
		std::stringstream text;
		ELogTNFP(ELogTNFP_TOTX,ELogT0(LogTitle(this,"CSSL_T0","Send",">>")
									  << LogBuffer(cgMsg.GetDefArrayWR()->Used(),cgMsg.GetDefArrayWR()->Unused())
									  << LogMLEN2(pnf.GetUVInLength(_in),0)
									  << LogIDS(pnf.ReadCtrlID(),pnf.ReadBlockID(),pnf.ReadOrderID())
									  << ",Port=" << port
									  << ",ErrCode:" << _GetIOSEMesg(ios.rcode) << ".Timeout:" << GetPackageDlyMS() << "ms"));
	}
	else{
		ELogTNFP(ELogTNFP_TOTX,ELogT0(LogTitle(this,"CSSL_T0","Send",">>")
									  << LogBuffer(cgMsg.GetDefArrayWR()->Used(),cgMsg.GetDefArrayWR()->Unused())
									  << LogMLEN2(pnf.GetUVInLength(_in),ios.total_out)
									  << LogIDS(pnf.ReadCtrlID(),pnf.ReadBlockID(),pnf.ReadOrderID())
									  << ",Port=" << port));
	}
	IOS_update(_ios, ios);
#endif
	ELogLock(0);
	return(rcode);
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
T1ID SetT1ID(const uint32& _ctrlID,const uint32& _blockID,const uint32& _orderID){
	return {_ctrlID,_blockID,_orderID};
};
//------------------------------------------------------------------------------------------//
CSSL_PNFB_PKG::CSSL_PNFB_PKG(void) : PNF_BLOCK(){
	AppendDown(pn_CtrlID) < pn_BlockID < pn_OrderID < pn_Info;
	InitPN((ARRAY*)nullptr);
};
//------------------------------------------------------------------------------------------//
CSSL_PNFB_PKG& CSSL_PNFB_PKG::InitPN(const ARRAY* array){
	if (array == nullptr)
		array = &cgArray;
	
	PNF_BLOCK::InitPN	(array,array,&pn_Info);
	pn_CtrlID.InitPN	(array,array,1,G_ENDIAN_LITTLE);
	pn_BlockID.InitPN	(array,array,1,G_ENDIAN_LITTLE);
	pn_OrderID.InitPN	(array,array,1,G_ENDIAN_LITTLE);
	pn_Info.InitPN		(array,array);
	return(*this);
};
//------------------------------------------------------------------------------------------//
CSSL_PNFB_PKG& CSSL_PNFB_PKG::InitPN(uint32 size){
	InitSize(size);
	return(InitPN(&cgArray));
};
//------------------------------------------------------------------------------------------//
IOSE CSSL_PNFB_PKG::WritePkg(IOS* _ios,const T1ID& _id,const UVIn& _in){
	IOSIN	iosin;
	IOS_copy(&iosin, _ios);
	IOS_update(&iosin, PNF_BLOCK::_Begin	(_ios));
	IOS_update(&iosin, pn_CtrlID.Write		(_ios,_id.ctrlID));
	IOS_update(&iosin, pn_BlockID.Write		(_ios,_id.blockID));
	IOS_update(&iosin, pn_OrderID.Write		(_ios,_id.orderID));
	IOS_update(&iosin, pn_Info.Write		(_ios,_in));
	IOS_update(&iosin, PNF_BLOCK::_Endl		(_ios));
	return(IOS_copy(_ios,iosin));
};
//------------------------------------------------------------------------------------------//
bool32 CSSL_PNFB_PKG::InvalidMark(void)const{
	return(pn_CtrlID.SetMaxValueRE());
};
//------------------------------------------------------------------------------------------//
bool32 CSSL_PNFB_PKG::IsInvalid(void)const{
	return(pn_CtrlID.IsMaxValueRE());
};
//------------------------------------------------------------------------------------------//
uint32 CSSL_PNFB_PKG::ReadCtrlID(void)const{
	return(pn_CtrlID.GetValueCalcRE());
};
//------------------------------------------------------------------------------------------//
uint32 CSSL_PNFB_PKG::ReadBlockID(void)const{
	return(pn_BlockID.GetValueCalcRE());
};
//------------------------------------------------------------------------------------------//
uint32 CSSL_PNFB_PKG::ReadOrderID(void)const{
	return(pn_OrderID.GetValueCalcRE());
};
//------------------------------------------------------------------------------------------//
uint32 CSSL_PNFB_PKG::MaxBlockID(void)const{
	return(pn_BlockID.GetMaxValue());
};
//------------------------------------------------------------------------------------------//
uint32 CSSL_PNFB_PKG::MaxOrderID(void)const{
	return(pn_OrderID.GetMaxValue());
};
//------------------------------------------------------------------------------------------//
IOSE CSSL_PNFB_PKG::FindPkg_invalid(uint32 pkgSize){
	IOSE	rcode = IOS_NO_OBJECT;
	uint32	offset;
	
	offset = 0;
	while(Analysis(offset) > 0){
		rcode = IOS_NO_OBJECT;
		if ((IsInvalid() != G_FALSE) && (GetLengthRE() == pkgSize)){
			rcode = IOS_OK;
			break;
		}
		offset = GetLengthRE() + GetOffsetRE();
	};
	return(rcode);
};
//------------------------------------------------------------------------------------------//
IOSE CSSL_PNFB_PKG::FindPkg_first(uint32* retCID,uint32* retBID,uint32* retOID){
	IOSE	rcode = IOS_NO_OBJECT;
	uint32	offset;
	
	offset = 0;
	while(Analysis(offset) > 0){
		rcode = IOS_NO_OBJECT;
		if (IsInvalid() == G_FALSE){
			rcode = IOS_OK;
			*retCID = ReadCtrlID();
			*retBID = ReadBlockID();
			*retOID = ReadOrderID();
			break;
		}
		offset = GetLengthRE() + GetOffsetRE();
	};
	return(rcode);
};
//------------------------------------------------------------------------------------------//
IOSE CSSL_PNFB_PKG::FindPkg_next(uint32* retCID,uint32* retBID,uint32* retOID){
	IOSE	rcode = IOS_NO_OBJECT;
	uint32	offset;
	
	offset = GetLengthRE() + GetOffsetRE();
	while(Analysis(offset) > 0){
		rcode = IOS_NO_OBJECT;
		if (IsInvalid() == G_FALSE){
			rcode = IOS_OK;
			*retCID = ReadCtrlID();
			*retBID = ReadBlockID();
			*retOID = ReadOrderID();
			break;
		}
		offset = GetLengthRE() + GetOffsetRE();
	};
	return(rcode);
};
//------------------------------------------------------------------------------------------//
static inline IOSE DoMatchCID(uint32* cID,uint32 rCID){
	IOSE	rcode = IOS_ERR;
	if (cID == nullptr){
		rcode = IOS_OK;
	}
	else{
		if (   (((*cID == CID_Package) || (*cID == CID_PackageEnd)) && ((rCID == CID_Package) || (rCID == CID_PackageEnd)))
			|| ((*cID != CID_Package) && (*cID != CID_PackageEnd) && (rCID != CID_Package) && (rCID != CID_PackageEnd))){
			rcode = IOS_OK;
		}
	}
	return(rcode);
};
//------------------------------------------------------------------------------------------//
IOSE CSSL_PNFB_PKG::FindPkg(uint32* cID,uint32 bID,uint32 oID,bool32 blInvalidMark){
	//IOS_ERR:oID over
	//IOS_NO_OBJECT:no find
	//IOS_OK:find
	IOSE	rcode = IOS_NO_OBJECT;
	uint32	offset,findOffset;
	uint32	rOID;
	
	if (bID > pn_BlockID.GetMaxValue())
		return IOS_ERR;
	if (oID > pn_OrderID.GetMaxValue())
		return IOS_ERR;
	
	offset = 0;
	findOffset = 0;
	while(Analysis(offset) > 0){
		rOID = ReadOrderID();
		if ((IsInvalid() == G_FALSE) && (bID == ReadBlockID())){
			if (DoMatchCID(cID,ReadCtrlID()) > 0){
				if (oID == rOID){
					findOffset = offset;
					if (cID != nullptr)
						*cID = ReadCtrlID();
					rcode = IOS_OK;
					if (blInvalidMark == G_FALSE)
						break;
				}
				else if ((blInvalidMark != G_FALSE) && (oID > rOID)){
					InvalidMark();
				}
			}
		}
		offset = GetLengthRE() + GetOffsetRE();
	};
	if (blInvalidMark != G_FALSE)
		Analysis(findOffset);
	return(rcode);
};
//------------------------------------------------------------------------------------------//
IOSE CSSL_PNFB_PKG::FindPkg_maxOID(uint32* cID,uint32 bID,uint32* oID,bool32 blInvalidMark){
	//IOS_ERR:oID over
	//IOS_NO_OBJECT:no find
	//IOS_OK:find
	/*
	 0	1	2		3		4	5	6
			^		^		^
			|		|		|
		maxOID	  *oID	 maxOID_up
	*/

	IOSE	rcode = IOS_NO_OBJECT;
	bool32	blup = G_FALSE;
	uint32	offset,maxOffset,maxOID,maxOffset_up,maxOID_up;
	uint32	rOID;
	
	if (bID > pn_BlockID.GetMaxValue())
		return IOS_ERR;

	offset = 0;
	maxOffset = 0;
	maxOID = 0;
	maxOffset_up = 0;
	maxOID_up = 0;

	while(Analysis(offset) > 0){
		rOID = ReadOrderID();
		if ((IsInvalid() == G_FALSE) && (bID == ReadBlockID())){
			if (DoMatchCID(cID,ReadCtrlID()) > 0){
				if (*oID == rOID){
					maxOID_up = rOID;
					maxOffset_up = offset;
					rcode = IOS_OK;
					blup = G_TRUE;
					break;
				}
				else if (*oID < rOID){
					if (maxOID_up >= rOID){
						maxOID_up = rOID;
						maxOffset_up = offset;
						rcode = IOS_OK;
						blup = G_TRUE;
					}
				}
				else{
					if (maxOID <= rOID){
						maxOID = rOID;
						maxOffset = offset;
						rcode = IOS_OK;
					}
				}
			}
		}
		offset = GetLengthRE() + GetOffsetRE();
	};
	if (rcode == IOS_OK){
		if (blup == G_FALSE){
			maxOID_up = maxOID;
			maxOffset_up = maxOffset;
		}
		if (blInvalidMark != G_FALSE)
			FindPkg(cID,bID,maxOID_up,G_TRUE);
		*oID = maxOID_up;
		Analysis(maxOffset_up);
		if (cID != nullptr)
			*cID = ReadCtrlID();
	}
	return(rcode);
};
//------------------------------------------------------------------------------------------//
IOSE CSSL_PNFB_PKG::FindPkg_minOID(uint32* cID,uint32 bID,uint32* oID,bool32 blInvalidMark){
	//IOS_ERR:oID over
	//IOS_NO_OBJECT:no find
	//IOS_OK:find
	/*
	 221	222		223	224	255		0	1	2
	 		 ^		 ^				^
			 |		 |				|
			*oID  minOID_up		 minOID
	*/

	IOSE	rcode = IOS_NO_OBJECT;
	bool32	blup = G_FALSE;
	uint32	offset,minOffset,minOID,minOffset_up,minOID_up;
	uint32	rOID;
	
	if (bID > pn_BlockID.GetMaxValue())
		return IOS_ERR;
	if (*oID > pn_OrderID.GetMaxValue())
		return IOS_ERR;
	
	offset = 0;
	minOffset = 0;
	minOID = pn_OrderID.GetMaxValue();
	minOffset_up = 0;
	minOID_up = pn_OrderID.GetMaxValue();
	while(Analysis(offset) > 0){
		rOID = ReadOrderID();
		if ((IsInvalid() == G_FALSE) && (bID == ReadBlockID())){
			if (DoMatchCID(cID,ReadCtrlID()) > 0){
				if (*oID == rOID){
					minOID_up = rOID;
					minOffset_up = offset;
					rcode = IOS_OK;
					blup = G_TRUE;
					break;
				}
				else if (*oID < rOID){
					if (minOID_up >= rOID){
						minOID_up = rOID;
						minOffset_up = offset;
						rcode = IOS_OK;
						blup = G_TRUE;
					}
				}
				else{
					if (minOID >= rOID){
						minOID = rOID;
						minOffset = offset;
						rcode = IOS_OK;
					}
				}
			}
		}
		offset = GetLengthRE() + GetOffsetRE();
	};
	if (rcode == IOS_OK){
		if (blup == G_FALSE){
			minOID_up = minOID;
			minOffset_up = minOffset;
		}
		if (blInvalidMark != G_FALSE)
			FindPkg(cID,bID,minOID_up,G_TRUE);
		*oID = minOID_up;
		Analysis(minOffset_up);
		if (cID != nullptr)
			*cID = ReadCtrlID();
	}
	return(rcode);
};
//------------------------------------------------------------------------------------------//
IOSE CSSL_PNFB_PKG::FindPkg_minBID(uint32* cID,uint32* bID){
	//IOS_ERR:oID over
	//IOS_NO_OBJECT:no find
	//IOS_OK:find
	/*
	 221	222		223	224	255		0	1	2
			 ^		 ^				^
			 |		 |				|
			*oID  minBID_up		 minBID
	*/

	IOSE	rcode = IOS_NO_OBJECT;
	bool32	blup = G_FALSE;
	uint32	offset,minOffset,minBID,minOffset_up,minBID_up;
	uint32	rBID;
	
	if (*bID > pn_BlockID.GetMaxValue())
		return IOS_ERR;
	
	offset = 0;
	minOffset_up = 0;
	minBID_up = pn_BlockID.GetMaxValue();
	minOffset = 0;
	minBID = *bID;
	while(Analysis(offset) > 0){
		rBID = ReadBlockID();
		if ((IsInvalid() == G_FALSE) && (DoMatchCID(cID,ReadCtrlID()) > 0)){
			if (*bID == rBID){
				minBID_up = rBID;
				minOffset_up = offset;
				rcode = IOS_OK;
				blup = G_TRUE;
				break;
			}
			else if (*bID < rBID){
				if (minBID_up >= rBID){
					minBID_up = rBID;
					minOffset_up = offset;
					rcode = IOS_OK;
					blup = G_TRUE;
				}
			}
			else{
				if (minBID >= rBID){
					minBID = rBID;
					minOffset = offset;
					rcode = IOS_OK;
				}
			}
		}
		offset = GetLengthRE() + GetOffsetRE();
	};

	if (rcode == IOS_OK){
		if (blup == G_FALSE){
			minBID_up = minBID;
			minOffset_up = minOffset;
		}
		*bID = minBID_up;
		Analysis(minOffset_up);
		if (cID != nullptr)
			*cID = ReadCtrlID();
	}
	return(rcode);
};
//------------------------------------------------------------------------------------------//
void CSSL_PNFB_PKG::CleanInvalidPkg(void){
	while(Analysis(0) > 0){
		if (IsInvalid() == G_FALSE)
			break;
		OutRE();
	};
};
//------------------------------------------------------------------------------------------//
void CSSL_PNFB_PKG::CleanPackages(uint32* cID,uint32 bID){
	uint32	offset = 0;
	
	while(Analysis(offset) > 0){
		if ((IsInvalid() == G_FALSE) && (bID == ReadBlockID())){
			if (DoMatchCID(cID,ReadCtrlID()) > 0)
				InvalidMark();
		}
		if ((IsInvalid() != G_FALSE) && (offset == 0)){
			OutRE();
			continue;
		}
		offset = GetLengthRE() + GetOffsetRE();
	};
};
//------------------------------------------------------------------------------------------//
void CSSL_PNFB_PKG::CleanPackages(uint32* cID,uint32 bID,uint32 oID){
	uint32	offset = 0;
	
	while(Analysis(offset) > 0){
		if ((IsInvalid() == G_FALSE) && (bID == ReadBlockID())){
			if ((oID >= ReadOrderID()) && (DoMatchCID(cID,ReadCtrlID()) > 0))
				InvalidMark();
		}
		if ((IsInvalid() != G_FALSE) && (offset == 0)){
			OutRE();
			continue;
		}
		offset = GetLengthRE() + GetOffsetRE();
	};
};
//------------------------------------------------------------------------------------------//
IOSE CSSL_PNFB_PKG::ReadPkg(IOS* _ios,const UVOut& _out,uint32* cID,uint32 bID,uint32 oID,bool32 blInvalidMark){
	//IOS_ERR:oID over
	//IOS_NO_OBJECT:no find
	//IOS_NO_MEMPRY:Out no enough memory
	//IOS_OK:ok
	IOSE	rcode;
	rcode = FindPkg(cID, bID, oID, blInvalidMark);
	if (rcode == IOS_OK){
		rcode = Read(_ios,_out);
		if (blInvalidMark != G_FALSE)
			InvalidMark();
	}
	return(IOS_update(_ios, rcode));
};
//------------------------------------------------------------------------------------------//
IOSE CSSL_PNFB_PKG::ReadPkg_maxOID(IOS* _ios,const UVOut& _out,uint32* cID,uint32 bID,uint32* oID,bool32 blInvalidMark){
	//IOS_ERR:oID over
	//IOS_NO_OBJECT:no find
	//IOS_NO_MEMPRY:Out no enough memory
	//IOS_OK:ok
	IOSE	rcode;
	rcode = FindPkg_maxOID(cID, bID, oID, blInvalidMark);
	if (rcode == IOS_OK){
		rcode = Read(_ios,_out);
		if (blInvalidMark != G_FALSE)
			InvalidMark();
	}
	return(IOS_update(_ios, rcode));
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
inline void CSSL_MEMORY::PAGENODE::RemoveSelf(void){
	CSSL_MEMORY	*f;
	f = static_cast<CSSL_MEMORY*>(GetUp(this));
	if (f != nullptr)
		f->RemovePage(this);
};
//------------------------------------------------------------------------------------------//
CSSL_MEMORY::CSSL_MEMORY(void) : TNFP(){
	TNFP::SetSelfName("CSSL_MEMORY");
	cgPkgRecT0.SetSelfName("Package");
	cgPkgRecT0.InitPN((ARRAY*)nullptr);
	SetUpName("");

	cgPageSize = 0;
	cgMaxPages = 0;
	cgPagesCount = 0;
	cgT0 = nullptr;
	cgPort = 0;
};
//------------------------------------------------------------------------------------------//
CSSL_MEMORY::~CSSL_MEMORY(void){
	cgPageSize = 0;
	cgMaxPages = 0;
	cgPagesCount = 0;
	cgT0 = nullptr;
	cgPort = 0;
	CleanDownTree(this,this);
};
//------------------------------------------------------------------------------------------//
void CSSL_MEMORY::Init(uint32 port,const CSSL_T0* csslT0,uint32 maxPackage,uint32 maxPages){
	cgPageSize = CSSL_T0::MAX_PACKAGE_SIZE * maxPackage;
	cgMaxPages = maxPages;
	cgPagesCount = 0;
	cgT0 = (CSSL_T0*)csslT0;
	cgPort = port;
	Empty();
};
//------------------------------------------------------------------------------------------//
void CSSL_MEMORY::SetPort(uint32 port){
	cgPort = port;
};
//------------------------------------------------------------------------------------------//
void CSSL_MEMORY::SetSelfName(const STDSTR& strName){
	selfName = strName;
	cgPkgRecT0.SetUpName(GetFullName(this));
};
//------------------------------------------------------------------------------------------//
void CSSL_MEMORY::SetUpName(const STDSTR& strName){
	fatherName = strName;
	cgPkgRecT0.SetUpName(GetFullName(this));
};
//------------------------------------------------------------------------------------------//
void CSSL_MEMORY::Empty(void){
	TREE_DownChain_Traversal_LINE
	(PAGENODE,this,
		do{
			_opNode->rLock.W_set();
			if (_opNode->wLock.Try() == G_FALSE){
				_opNode->rLock.W_clr();
				continue;
			}
			_opNode->array.Empty();
			_opNode->wLock.Clr();
			_opNode->rLock.W_clr();
			break;
		}while(1);
	);
};
//------------------------------------------------------------------------------------------//
void CSSL_MEMORY::Close(void){
	Empty();
	Modify_set();
	CleanDownTree(this,this);
	cgPagesCount = 0;
	Modify_clr();
};
//------------------------------------------------------------------------------------------//
static TNFP* _GetTrashOwer(void){
	static TNFP sgSpareOwner;
	return(&sgSpareOwner);
};
//------------------------------------------------------------------------------------------//
TNFP* CSSL_MEMORY::GetTrash(void){
	return(_GetTrashOwer()->GetTrash());
};
//------------------------------------------------------------------------------------------//
TNF* CSSL_MEMORY::CreateNode(void){
	return(new PAGENODE(cgPageSize));
};
//------------------------------------------------------------------------------------------//
bool32 CSSL_MEMORY::AddNewPage(void){
	bool32		ret = G_FALSE;
	PAGENODE	*node;
	Modify_set();
	if (cgPagesCount < cgMaxPages){
		node = static_cast<PAGENODE*>(GetNewNode());
		if (node != nullptr){
			node->array.Reset();
			ret = G_TRUE;
			++ cgPagesCount;
			AppendDownNode(node);
		}
	}
	Modify_clr();
	return(ret);
};
//------------------------------------------------------------------------------------------//
void CSSL_MEMORY::RemovePage(PAGENODE* page){
	Modify_set();
	MoveNodesToTrash(_GetTrashOwer(),page,page);
	if ((page != nullptr) && (cgPagesCount > 0))
		-- cgPagesCount;
	Modify_clr();
};
//------------------------------------------------------------------------------------------//
const uint32& CSSL_MEMORY::GetPort(void) const{
	return(cgPort);
};
//------------------------------------------------------------------------------------------//
#ifdef LOGMEM_RE_ENABLE
void MEMLOG(CSSL_MEMORY* mem,CSSL_MEMORY::PAGENODE* page,const uint8* data,const uint64& length){
#ifdef LOGT0_ENABLE
	CSSL_PNFB_PKG	pkg;
	pkg.InitPN(CSSL_T0::MAX_PACKAGE_SIZE);
	DS_IO_NODE::GetDSIOList().Save(nullptr, pkg.GetDefArrayWR(), IUD(data,length));
	pkg.Analysis(0);
	ELogON(mem,ELogTNFP_MEM_RE,ELogMEM_RE(LogTitle(mem,"CSSL_MEMORY","RecFromT0","<<")
										<< LogPage(page)
										<< LogMLEN1(pkg.GetLengthRE())
										<< LogIDS(pkg.ReadCtrlID(),pkg.ReadBlockID(),pkg.ReadOrderID())));
#endif
};
#endif
#ifdef ELogMEM_WR
void MEMLOG(CSSL_MEMORY* mem,CSSL_MEMORY::PAGENODE* page,const T1ID& _id,const UVIn& _in){
	CSSL_PNFB_PKG	pkg;
	pkg.InitPN(CSSL_T0::MAX_PACKAGE_SIZE);
	pkg.WritePkg(nullptr, _id, _in);
	pkg.Analysis(0);
	
	ELogON(mem,ELogTNFP_MEM_WR,ELogMEM_WR(LogTitle(mem,"CSSL_MEMORY","WritePkg",">>")
										  << LogPage(page)
										  << LogMLEN2(DS_IO_NODE::GetDSIOList().GetUVInLength(_in),pkg.GetLengthRE())
										  << LogIDS(pkg.ReadCtrlID(),pkg.ReadBlockID(),pkg.ReadOrderID())));
};
#endif
#ifdef LOGMEM_RE_ENABLE
	#define		_MEMLOG_RecFromT0(_page_,_data_,_length_)		MEMLOG(this,_page_,_data_,_length_)
#else
	#define		_MEMLOG_RecFromT0(_page_,_data_,_length_);
#endif
#ifdef LOGMEM_WR_ENABLE
	#define		_MEMLOG_WritePkg(_page_,_id_,_in_)				MEMLOG(this,_page_,_id_,_in_)
#else
	#define		_MEMLOG_WritePkg(_page_,_id_,_in_);
#endif
//------------------------------------------------------------------------------------------//
static void List(CSSL_MEMORY* mem){
	TREE_DownChain_Traversal_LINE
	(CSSL_MEMORY::PAGENODE,mem,
		_opNode->rLock.W_try();
		ELog(LogTitle(mem,"CSSL_MEMORY","List","::")
			 << LogPage(_opNode));
		_opNode->rLock.W_clr();
	 );
};
//------------------------------------------------------------------------------------------//
IOSE CSSL_MEMORY::SendToT0(CSSL_PNFB_PKG* pkg,IOS* _ios,const T1ID& _id){
	//IOS_ERR:oID over
	//IOS_NO_OBJECT:no find
	//IOS_NO_MEMPRY:T0 has no enough memory
	//IOS_OK:ok
	IOSE	rcode;
	PAGENODE *page = nullptr;
	uint32	cID = _id.ctrlID;
	
	rcode = FindPkg(pkg, &page, &cID, _id.blockID, _id.orderID, G_FALSE);
	if (rcode == IOS_OK)
		rcode = cgT0->Send(_ios, cgPort, IUD(pkg->GetDefArrayRE(),pkg->GetLengthRE(),pkg->GetOffsetRE()));
	FindFinish(page);
	pkg->InitPN((ARRAY*)nullptr);
	return(IOS_update(_ios, rcode));
};
//------------------------------------------------------------------------------------------//
IOSE CSSL_MEMORY::RecFromT0(const uint8* data,const uint64& length){
	ELogLock(1);
	IOSE	rcode = IOS_NO_MEM;
	CSSL_PNFB_PKG	&pkg = cgPkgRecT0;
	do {
		TREE_DownChain_Traversal_LINE
		(PAGENODE,this,
			if (_opNode->array.Unused() >= length){
				_opNode->array.Put(data, (uint32)length);
				_MEMLOG_RecFromT0(_opNode,data,length);
				rcode = IOS_OK;
				break;
			}
		);
		if (rcode == IOS_NO_MEM){
			CleanInvalidPkg(&pkg);
			uint32 count;
			do{
				count = 0;
				TREE_DownChain_Traversal_LINE
				(PAGENODE,this,
					if (_opNode->array.Unused() >= length){
						_opNode->array.Put(data, (uint32)length);
						_MEMLOG_RecFromT0(_opNode,data,length);
						rcode = IOS_OK;
						break;
					}
					if (_opNode->rLock.W_try()){
						++ count;
						if (_opNode->array.Unused() >= length){
							_opNode->array.Put(data, (uint32)length);
							_MEMLOG_RecFromT0(_opNode,data,length);
							rcode = IOS_OK;
						}
						else if (pkg.InitPN(&_opNode->array).FindPkg_invalid((uint32)length) == IOS_OK){
							_opNode->array.UpdateByOffsetOut(data, (uint32)length, pkg.GetOffsetRE());
							_MEMLOG_RecFromT0(_opNode,data,length);
							rcode = IOS_OK;
						}
						_opNode->rLock.W_clr();
					}
					if (rcode == IOS_OK)
						break;
				 );
			}while((rcode == IOS_NO_MEM) && (count < cgPagesCount));
		}
		if (rcode == IOS_NO_MEM){
			if (AddNewPage())
				continue;
		}
		break;
	}while(1);
	pkg.InitPN((ARRAY*)nullptr);
	ELogLock(0);
	return(rcode);
};
//------------------------------------------------------------------------------------------//
IOSE CSSL_MEMORY::WritePkg(CSSL_PNFB_PKG* pkg,IOS* _ios,const T1ID& _id,const UVIn& _in){
	IOSE		rcode = IOS_NO_MEM;
	uint32		length = (uint32)DS_IO_NODE::GetDSIOList().GetUVInLength(_in);
	uint32		count;

	ELogLock(1);
	do {
		do{
			count = 0;
			TREE_DownChain_Traversal_LINE
			(PAGENODE,this,
				if (_opNode->array.Unused() >= length){
					if (_opNode->wLock.Try()){
						if (_opNode->array.Unused() >= (length + MAX_PACKAGE_SIZE_ADJUST_MEM)){
							rcode = pkg->InitPN(&_opNode->array).WritePkg(_ios, _id, _in);
							_MEMLOG_WritePkg(_opNode,_id,_in);
							_opNode->wLock.Clr();
							break;
						}
						++ count;
						_opNode->wLock.Clr();
					}
				}
				else{
					++ count;
				}
			);
		}while((rcode != IOS_FINISH) && (count < cgPagesCount));
		if (rcode == IOS_NO_MEM){
			if (AddNewPage())
				continue;
		}
		break;
	}while(1);

	if (rcode <= 0){
		ELogTNFP(ELogTNFP_MEM_WR,ELogMEM_WR(LogTitle(this,"CSSL_MEMORY","WritePkg",">>")
											<< LogBLK1("ErrCode:",_GetIOSEMesg(rcode))
											<< LogMLEN2(length,0)
											<< LogIDS(_id.ctrlID,_id.blockID,_id.orderID)));
	}
	pkg->InitPN((ARRAY*)nullptr);
	ELogLock(0);
	return(IOS_update(_ios,rcode));
};
//------------------------------------------------------------------------------------------//
void CSSL_MEMORY::FindFinish(PAGENODE* page){
	if (page != nullptr)
		page->rLock.R_clr();
};
//------------------------------------------------------------------------------------------//
IOSE CSSL_MEMORY::FindPkg(CSSL_PNFB_PKG* pkg,PAGENODE** retPage,uint32* cID,uint32 bID,uint32 oID,bool32 blInvalidMark){
	//IOS_ERR:oID over
	//IOS_NO_OBJECT:no find
	//IOS_OK:find
	IOSE	rcode = IOS_NO_OBJECT;
	uint32	count;
	PAGENODE *page;
	
	page = nullptr;
	if (retPage != nullptr)
		*retPage = nullptr;
	
	if (oID > pkg->InitPN((ARRAY*)nullptr).MaxOrderID())
		return IOS_ERR;

	do{
		count = 0;
		TREE_DownChain_Traversal_LINE
		(PAGENODE,this,
			rcode = IOS_NO_OBJECT;
			if (_opNode->rLock.R_try()){
				++ count;
				rcode = pkg->InitPN(&_opNode->array).FindPkg(cID, bID, oID, blInvalidMark);
				if (rcode == IOS_OK){
					page = _opNode;
					break;
				}
				_opNode->rLock.R_clr();
			}
		);
	}while((rcode != IOS_OK) && (count < cgPagesCount));
	
	if (retPage != nullptr){
		*retPage = page;
	}
	else{
		FindFinish(page);
	}
	if (rcode != IOS_OK)
		pkg->InitPN((ARRAY*)nullptr);
	return(rcode);
};
//------------------------------------------------------------------------------------------//
IOSE CSSL_MEMORY::FindPkg_maxOID(CSSL_PNFB_PKG* pkg,PAGENODE** retPage,uint32* cID,uint32 bID,uint32* oID,bool32 blInvalidMark){
	//IOS_ERR:oID over
	//IOS_NO_OBJECT:no find
	//IOS_OK:find
	IOSE		rcode,ret;
	bool32		blup;
	uint32		maxOID,maxOID_up,rOID;
	
	do{
		if (retPage != nullptr)
			*retPage = nullptr;
		
		if (*oID > pkg->InitPN((ARRAY*)nullptr).MaxOrderID())
			return IOS_ERR;
		
		maxOID = 0;
		maxOID_up = 0;
		rcode = IOS_NO_OBJECT;
		blup = G_FALSE;
		
		TREE_DownChain_Traversal_LINE
		(PAGENODE,this,
			_opNode->rLock.R_set();
			rOID = *oID;
			ret = pkg->InitPN(&_opNode->array).FindPkg_maxOID(cID, bID, &rOID, blInvalidMark);
			_opNode->rLock.R_clr();
			if (ret == IOS_OK){
				if (*oID == rOID){
					maxOID_up = rOID;
					rcode = IOS_OK;
					blup = G_TRUE;
					break;
				}
				else if (*oID < rOID){
					if (maxOID_up >= rOID){
						maxOID_up = rOID;
						rcode = IOS_OK;
						blup = G_TRUE;
					}
				}
				else{
					if (maxOID <= rOID){
						maxOID = rOID;
						rcode = IOS_OK;
					}
				}
			}
		);
		if (rcode == IOS_OK){
			if (blup == G_FALSE)
				maxOID_up = maxOID;
			if (FindPkg(pkg,retPage,cID,bID,maxOID_up,blInvalidMark) != IOS_OK)
				continue;
			*oID = maxOID_up;
		}
		break;
	}while(1);
	
	if (rcode != IOS_OK)
		pkg->InitPN((ARRAY*)nullptr);
	return(rcode);
};
//------------------------------------------------------------------------------------------//
IOSE CSSL_MEMORY::FindPkg_minOID(CSSL_PNFB_PKG* pkg,PAGENODE** retPage,uint32* cID,uint32 bID,uint32* oID,bool32 blInvalidMark){
	//IOS_ERR:oID over
	//IOS_NO_OBJECT:no find
	//IOS_OK:find
	IOSE		rcode,ret;
	bool32		blup;
	uint32		minOID,minOID_up,rOID;
	
	do{
		if (retPage != nullptr)
			*retPage = nullptr;
		
		if (*oID > pkg->InitPN((ARRAY*)nullptr).MaxOrderID())
			return IOS_ERR;
		
		minOID = pkg->MaxOrderID();
		minOID_up = pkg->MaxOrderID();
		rcode = IOS_NO_OBJECT;
		blup = G_FALSE;
		
		TREE_DownChain_Traversal_LINE
		(PAGENODE,this,
			 _opNode->rLock.R_set();
			 rOID = *oID;
			 ret = pkg->InitPN(&_opNode->array).FindPkg_minOID(cID, bID, &rOID, blInvalidMark);
			 _opNode->rLock.R_clr();
			 if (ret == IOS_OK){
				 if (*oID == rOID){
					 minOID_up = rOID;
					 rcode = IOS_OK;
					 blup = G_TRUE;
					 break;
				 }
				 else if (*oID < rOID){
					 if (minOID_up >= rOID){
						 minOID_up = rOID;
						 rcode = IOS_OK;
						 blup = G_TRUE;
					 }
				 }
				 else{
					 if (minOID >= rOID){
						 minOID = rOID;
						 rcode = IOS_OK;
					 }
				 }
			 }
		);
		if (rcode == IOS_OK){
			if (blup == G_FALSE)
				minOID_up = minOID;
			if (FindPkg(pkg,retPage,cID,bID,minOID_up,blInvalidMark) != IOS_OK)
				continue;
			*oID = minOID_up;
		}
		break;
	}while(1);
	
	if (rcode != IOS_OK)
		pkg->InitPN((ARRAY*)nullptr);
	return(rcode);
};
//------------------------------------------------------------------------------------------//
IOSE CSSL_MEMORY::FindPkg_minBID(CSSL_PNFB_PKG* pkg,PAGENODE** retPage,uint32* retOID,uint32* cID,uint32* bID){
	//IOS_ERR:oID over
	//IOS_NO_OBJECT:no find
	//IOS_OK:find
	IOSE		rcode,ret;
	uint32		rBID,minBID,minBID_up;
	bool32		blup;
	
	do{
		if (retPage != nullptr)
			*retPage = nullptr;
		
		if (*bID > pkg->InitPN((ARRAY*)nullptr).MaxBlockID())
			return IOS_ERR;
		
		rcode = IOS_NO_OBJECT;
		blup = G_FALSE;
		minBID = pkg->MaxBlockID();
		minBID_up = pkg->MaxBlockID();
		
		TREE_DownChain_Traversal_LINE
		(PAGENODE,this,
			 _opNode->rLock.R_set();
			 rBID = *bID;
			 ret = pkg->InitPN(&_opNode->array).FindPkg_minBID(cID, &rBID);
			 _opNode->rLock.R_clr();
			 if (ret == IOS_OK){
				 if (*bID == rBID){
					 minBID_up = rBID;
					 rcode = IOS_OK;
					 blup = G_TRUE;
					 break;
				 }
				 else if (*bID < rBID){
					 if (minBID_up >= rBID){
						 minBID_up = rBID;
						 rcode = IOS_OK;
						 blup = G_TRUE;
					 }
				 }
				 else{
					 if (minBID >= rBID){
						 minBID = rBID;
						 rcode = IOS_OK;
					 }
				 }
			 }
		);
		
		if (rcode == IOS_OK){
			if (blup == G_FALSE)
				minBID_up = minBID;
			if (FindPkg_minOID(pkg, retPage, cID, minBID_up, retOID, G_FALSE) != IOS_OK)
				continue;
			*bID = minBID_up;
		}
		break;
	}while (1);
	if (rcode != IOS_OK)
		pkg->InitPN((ARRAY*)nullptr);
	return(rcode);
};
//------------------------------------------------------------------------------------------//
IOSE CSSL_MEMORY::ReadPkg(CSSL_PNFB_PKG* pkg,IOS* _ios,const UVOut& _out,uint32* cID,uint32 bID,uint32 oID,bool32 blInvalidMark){
	//IOS_ERR:oID over
	//IOS_NO_OBJECT:no find
	//IOS_NO_MEMPRY:Out has no enough memory
	//IOS_OK:ok
	ELogLock(1);
#ifdef LOGMEM_RE_ENABLE
#define	def_ios1		iosp
	IOS	ios;
	IOS	*iosp = &ios;
	IOS_clr(&ios);
#else
#define	def_ios1		_ios
#endif
	IOSE	rcode;
	PAGENODE *page = nullptr;
	rcode = FindPkg(pkg, &page, cID, bID, oID, blInvalidMark);
	if (rcode == IOS_OK){
		rcode = pkg->Read(def_ios1,_out);
		if (blInvalidMark != G_FALSE)
			pkg->InvalidMark();
		if (rcode > 0){
			ELogTNFP(ELogTNFP_MEM_RE,ELogMEM_RE(LogTitle(this,"CSSL_MEMORY","ReadPkg","<<")
												<< LogPage(page)
												<< LogMLEN2(ios.total_out,pkg->GetLengthRE())
												<< LogIDS(*cID,bID,oID)));
		}
	}
	if ((rcode <= 0) && (rcode != IOS_NO_OBJECT)){
		ELogTNFP(ELogTNFP_MEM_RE,ELogMEM_RE(LogTitle(this,"CSSL_MEMORY","ReadPkg","<<")
											<< LogBLK1("ErrCode:",_GetIOSEMesg(rcode))
											<< LogBLK2("")
											<< LogIDS(*cID,bID,oID)));
	}
	FindFinish(page);
	pkg->InitPN((ARRAY*)nullptr);
#ifdef LOGMEM_RE_ENABLE
	IOS_update(_ios, ios);
#endif
	ELogLock(0);
	return(IOS_update(_ios, rcode));
};
//------------------------------------------------------------------------------------------//
IOSE CSSL_MEMORY::ReadPkg_maxOID(CSSL_PNFB_PKG* pkg,IOS* _ios,const UVOut& _out,uint32* cID,uint32 bID,uint32* oID,bool32 blInvalidMark){
	//IOS_ERR:oID over
	//IOS_NO_OBJECT:no find
	//IOS_NO_MEMPRY:Out has no enough memory
	//IOS_OK:ok
	ELogLock(1);
#ifdef LOGMEM_RE_ENABLE
#define	def_ios2		iosp
	IOS	ios;
	IOS	*iosp = &ios;
	IOS_clr(&ios);
#else
#define	def_ios2		_ios
#endif
	IOSE	rcode;
	PAGENODE *page = nullptr;
	rcode = FindPkg_maxOID(pkg, &page, cID, bID, oID, blInvalidMark);
	if (rcode == IOS_OK){
		rcode = pkg->Read(def_ios2,_out);
		if (blInvalidMark != G_FALSE)
			pkg->InvalidMark();
		if (rcode > 0){
			ELogTNFP(ELogTNFP_MEM_RE,ELogMEM_RE(LogTitle(this,"CSSL_MEMORY","ReadPkg_maxOID","<<")
												<< LogPage(page)
												<< LogMLEN2(ios.total_out,pkg->GetLengthRE())
												<< LogIDS(*cID,bID,*oID)));
		}
	}
	if ((rcode <= 0) && (rcode != IOS_NO_OBJECT)){
		ELogTNFP(ELogTNFP_MEM_RE,ELogMEM_RE(LogTitle(this,"CSSL_MEMORY","ReadPkg_maxOID","<<")
											<< LogBLK1("ErrCode:",_GetIOSEMesg(rcode))
											<< LogBLK2("")
											<< LogIDS(*cID,bID,*oID)));
	}
	FindFinish(page);
	pkg->InitPN((ARRAY*)nullptr);
#ifdef LOGMEM_RE_ENABLE
	IOS_update(_ios, ios);
#endif
	ELogLock(0);
	return(IOS_update(_ios, rcode));
};
//------------------------------------------------------------------------------------------//
bool32 CSSL_MEMORY::CheckInvalidPkg(CSSL_PNFB_PKG* pkg){
	uint32	count;
	bool32	blFind = G_FALSE;
	do{
		count = 0;
		TREE_DownChain_Traversal_LINE
		(PAGENODE,this,
			if (_opNode->rLock.R_try()){
				++ count;
				if (pkg->InitPN(&_opNode->array).Analysis(0) > 0)
					blFind = (pkg->IsInvalid() != G_FALSE);
				_opNode->rLock.R_clr();
				if (blFind == G_TRUE)
					break;
			}
		);
	}while((blFind == G_FALSE) && (count < cgPagesCount));
	
	pkg->InitPN((ARRAY*)nullptr);
	return(blFind);
};
//------------------------------------------------------------------------------------------//
void CSSL_MEMORY::CleanInvalidPkg(CSSL_PNFB_PKG* pkg){
	uint32	count;

	if (CheckInvalidPkg(pkg) != G_FALSE){
		do{
			count = 0;
			TREE_DownChain_Traversal_LINE
			(PAGENODE,this,
				if (_opNode->rLock.W_try()){
					++ count;
					pkg->InitPN(&_opNode->array).CleanInvalidPkg();
					_opNode->rLock.W_clr();
				}
			);
		}while(count < cgPagesCount);
	}
	pkg->InitPN((ARRAY*)nullptr);
};
//------------------------------------------------------------------------------------------//
void CSSL_MEMORY::CleanPackages(CSSL_PNFB_PKG* pkg,uint32* cID,uint32 bID){
	uint32	count;
	
	do{
		count = 0;
		TREE_DownChain_Traversal_LINE
		(PAGENODE,this,
			if (_opNode->rLock.W_try()){
				++ count;
				pkg->InitPN(&_opNode->array).CleanPackages(cID,bID);
				_opNode->rLock.W_clr();
			}
		);
	}while(count < cgPagesCount);
	pkg->InitPN((ARRAY*)nullptr);
};
//------------------------------------------------------------------------------------------//
void CSSL_MEMORY::CleanPackages(CSSL_PNFB_PKG* pkg,uint32* cID,uint32 bID,uint32 oID){
	uint32	count;
	
	do{
		count = 0;
		TREE_DownChain_Traversal_LINE
		(PAGENODE,this,
			if (_opNode->rLock.W_try()){
				++ count;
				pkg->InitPN(&_opNode->array).CleanPackages(cID,bID,oID);
				_opNode->rLock.W_clr();
			}
		);
	}while(count < cgPagesCount);
	pkg->InitPN((ARRAY*)nullptr);
};
//------------------------------------------------------------------------------------------//
bool32 CSSL_MEMORY::CheckPkg(CSSL_PNFB_PKG* pkg,uint32* cID,uint32 bID,uint32 oID){
	return(FindPkg(pkg, nullptr, cID, bID, oID, G_FALSE) == IOS_OK);
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
CSSL_T1RX::CSSL_T1RX(void) : TNFP(){
	TNFP::SetSelfName("CSSL_T1RX");
	cgZLib.SetSelfName("ZLIB");
	cgMEM.SetSelfName("MEM");
	cgPkg.SetSelfName("Pkg");
	cgPkg.InitPN((ARRAY*)nullptr);
	SetUpName("");
};
//------------------------------------------------------------------------------------------//
void CSSL_T1RX::InitPN(uint32 port,const CSSL_T0* csslT0,uint32 maxPackage,uint32 maxPages){
	cgMEM.Init			(port,csslT0, maxPackage, maxPages);
	
	cgZLib.InitSize		(CSSL_T0::MAX_PACKAGE_SIZE * 2);
	cgZLib.InitCFG		(ALG_ZLIB::CFG_Uncompress);
	cgArray.InitSize	(CSSL_T0::MAX_PACKAGE_SIZE);
	Reset();
};
//------------------------------------------------------------------------------------------//
void CSSL_T1RX::SetPort(uint32 port){
	cgMEM.SetPort(port);
};
//------------------------------------------------------------------------------------------//
void CSSL_T1RX::SetSelfName(const STDSTR& strName){
	selfName = strName;
	cgZLib.SetUpName(GetFullName(this));
	cgMEM.SetUpName(GetFullName(this));
	cgPkg.SetUpName(GetFullName(this));
};
//------------------------------------------------------------------------------------------//
void CSSL_T1RX::SetUpName(const STDSTR& strName){
	fatherName = strName;
	cgZLib.SetUpName(GetFullName(this));
	cgMEM.SetUpName(GetFullName(this));
	cgPkg.SetUpName(GetFullName(this));
};
//------------------------------------------------------------------------------------------//
void CSSL_T1RX::Reset(void){
	cgIDS.ctrlID = CID_Package;
	cgIDS.blockID = 0;
	cgIDS.orderID = 0;
	cgArray.Reset();
	cgMEM.Empty();
};
//------------------------------------------------------------------------------------------//
void CSSL_T1RX::Empty(void){
	cgArray.Empty();
	cgMEM.Empty();
};
//------------------------------------------------------------------------------------------//
void CSSL_T1RX::Close(void){
	Empty();
	cgMEM.Close();
};
//------------------------------------------------------------------------------------------//
void CSSL_T1RX::Lock(G_LOCK on_off){
	if (on_off == G_LOCK_OFF){
		cgLock.Clr();
	}
	else{
		cgLock.Set();
	}
};
//------------------------------------------------------------------------------------------//
const uint32& CSSL_T1RX::GetPort(void) const{
	return(cgMEM.GetPort());
};
//------------------------------------------------------------------------------------------//
CSSL_PNFB_PKG* CSSL_T1RX::GetDefPkg(void){
	return(&cgPkg);
};
//------------------------------------------------------------------------------------------//
IOSE CSSL_T1RX::RecFromT0(const uint8* data,const uint64& length){
	return(cgMEM.RecFromT0(data, length));
};
//#define Test255
#ifdef Test255
bool32	_gblTest255 = G_FALSE;
bool32	_gblTest256 = G_FALSE;
#endif
#ifdef Test255
void DoTest255(IOSE& rcode,uint32& cID,uint32 oID){
	if (_gblTest255 == G_FALSE){
		if (oID == 255){
			rcode = IOS_NO_OBJECT;
			cID = CID_Package;
			_gblTest256 = G_TRUE;
		}
	}
	if (_gblTest256 == G_TRUE){
		if (oID == 0){
			rcode = IOS_NO_OBJECT;
			cID = CID_Package;
			_gblTest256 = G_TRUE;
		}
	}
}
void DoTest255(void){
	if (_gblTest255 == G_TRUE)
		_gblTest256 = G_FALSE;
	_gblTest255 = G_TRUE;
}
#endif
//------------------------------------------------------------------------------------------//
IOSE CSSL_T1RX::ReadOneBlock(IOS* _ios,const UVOut& _out,uint32* retWaitOID,uint32 bID){
	//IOS_ERR:oID over
	//IOS_NO_OBJECT:no bID oID data in T1 buffer
	//IOS_NO_MEMPRY:Out has no enough memory
	//IOS_FINISH:ok
	IOS				ios_in,ios_out;
	IOSE			rcode = IOS_ERR;
	T1ID			&t1ID = cgIDS;
	CSSL_PNFB_PKG	&pkg = cgPkg;
	
	ELogLine(bool32 blPrint = G_FALSE);

	IOS_clr(&ios_in);
	IOS_clr(&ios_out);
	if (_ios != nullptr)
		DS_IO_NODE::GetDSIOList().Save(&ios_out, _out, IUD(""));
	
	if (bID != t1ID.blockID){
		t1ID.ctrlID = CID_Package;
		t1ID.blockID = bID;
		t1ID.orderID = 0;
		cgArray.Reset();
		cgZLib.InitCFG(ALG_ZLIB::CFG_Uncompress);
		ELogLine(blPrint = G_TRUE);
	}
	
	if (t1ID.orderID > pkg.MaxOrderID())
		t1ID.orderID = 0;
	do{
		if (cgArray.IsEmpty()){
			cgArray.Reset();
			if (t1ID.ctrlID == CID_Package){
				ELogTNFP(ELogTNFP_T1_RX,ELogIF(blPrint,ELogT1_RX(LogTitle(this,"CSSL_T1RX","ReadOneBlock","::")
																 << LogBLK1("Waiting receive:")
																 << LogBLK2("")
																 << LogIDS(t1ID.ctrlID,bID,t1ID.orderID))));
				rcode = cgMEM.ReadPkg(&pkg,&ios_in,&cgArray,&t1ID.ctrlID,t1ID.blockID,t1ID.orderID,G_TRUE);
#ifdef Test255
				DoTest255(rcode,t1ID.ctrlID,t1ID.orderID);
#endif
				if (rcode == IOS_NO_OBJECT)//no bID oID data in T1 buffer
					break;
				if (rcode == IOS_ERR)//oID over
					break;
				++ t1ID.orderID;
				ELogLine(blPrint = G_TRUE);
			}
			else if (t1ID.ctrlID == CID_PackageEnd){
				rcode = cgZLib.Final(&ios_out,_out);
				if (rcode > 0){
					DS_IO_NODE::GetDSIOList().Save(&ios_out,_out,_NONE());
					rcode = IOS_FINISH;
					t1ID.ctrlID = CID_Finish;
					ELogTNFP(ELogTNFP_T1_RX,ELogT1_RX(LogTitle(this,"CSSL_T1RX","ReadOneBlock","<<")
													  << LogBLK1("Final:")
													  << LogBLK2("")
													  << LogIDS(t1ID.ctrlID,bID,t1ID.orderID - 1)));
				}
				else{
					ELogTNFP(ELogTNFP_T1_RX,ELogT1_RX(LogTitle(this,"CSSL_T1RX","ReadOneBlock","<<")
													  << LogBLK1("ErrCode:",_GetIOSEMesg(rcode))
													  << LogBLK2("")
													  << LogIDS(t1ID.ctrlID,bID,t1ID.orderID - 1)));
				}
				break;
			}
			else{
				break;
			}
		}
		ios_out.avail_in = 0;
		ios_out.total_in = 0;
		rcode = cgZLib.Transform(&ios_out, _out, cgArray);
		cgArray.Out((uint32)ios_out.total_in);
		if (rcode == IOS_ERR)
			break;
		if (rcode == IOS_NO_MEM){
			ELogTNFP(ELogTNFP_T1_RX,ELogT1_RX(LogTitle(this,"CSSL_T1RX","ReadOneBlock","<<")
											  << LogBLK1("ErrCode:",_GetIOSEMesg(rcode))
											  << LogBLK2("")
											  << LogIDS(t1ID.ctrlID,bID,t1ID.orderID - 1)));
			break;
		}
	}while(1);
	if (retWaitOID != nullptr)
		*retWaitOID = t1ID.orderID;
	ios_out.avail_in = ios_in.avail_in;
	ios_out.total_in = ios_in.total_in;
	IOS_update(_ios, ios_out);
	cgMEM.CleanInvalidPkg(&pkg);
	return(IOS_update(_ios, rcode));
};
//------------------------------------------------------------------------------------------//
void CSSL_T1RX::CleanPackages(CSSL_PNFB_PKG* pkg,uint32 cID,uint32 bID,uint32 oID){
	if (pkg == nullptr)
		pkg = &cgPkg;
	
	cgMEM.CleanPackages(pkg, &cID, bID, oID);
};
//------------------------------------------------------------------------------------------//
void CSSL_T1RX::CleanOneBlock(uint32 bID){
	uint32			cID = CID_Package;
	CSSL_PNFB_PKG	&pkg = cgPkg;
	cgMEM.CleanPackages(&pkg,&cID, bID);
};
//------------------------------------------------------------------------------------------//
IOSE CSSL_T1RX::GetMinBID(uint32* bID,uint32* oID){
	uint32			cID = CID_Package;
	CSSL_PNFB_PKG	&pkg = cgPkg;
	
	return(cgMEM.FindPkg_minBID(&pkg, nullptr, oID, &cID, bID));
};
//------------------------------------------------------------------------------------------//
IOSE CSSL_T1RX::ReadMaxACK(CSSL_PNFB_PKG* pkg,IOS* _ios,const UVOut& _out,uint32* retCID,uint32 bID,uint32* oID){
	//IOS_ERR:oID over
	//IOS_NO_OBJECT:no bID oID data in T1 buffer
	//IOS_NO_MEMPRY:no enough out memory
	//IOS_OK:

	IOSE	rcode;
	if (pkg == nullptr)
		pkg = &cgPkg;
	
	*retCID = CID_ACK;
	rcode = cgMEM.ReadPkg_maxOID(pkg, _ios, _out, retCID, bID, oID, G_TRUE);
	cgMEM.CleanInvalidPkg(pkg);
	return(rcode);
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
CSSL_T1TX::CSSL_T1TX(void) : DSTF(),
cgMaxPackage(CSSL_T0::MAX_PACKAGE_SIZE - CSSL_MEMORY::MAX_PACKAGE_SIZE_ADJUST_MEM){
//cgMaxPackage(1){
	DSTF_DIR_CTX_Clr(&cgCTX);
	
	TNFP::SetSelfName("CSSL_T1TX");
	cgMEM.SetSelfName("MEM");
	cgPkg.SetSelfName("Pkg");
	cgPkg.InitPN((ARRAY*)nullptr);
	SetUpName("");
};
//------------------------------------------------------------------------------------------//
CSSL_T1TX::~CSSL_T1TX(void){
	ALG_Zlib_Release(&cgCTX);
};
//------------------------------------------------------------------------------------------//
void CSSL_T1TX::InitPN(uint32 port,const CSSL_T0* csslT0,uint32 maxPackage,uint32 maxPages){
	DSTF::InitSize		(CSSL_T0::MAX_PACKAGE_SIZE);
	
	cgMEM.Init			(port,csslT0,maxPackage,maxPages);

	DSTF_DIR_CTX_Clr	(&cgCTX);
	ALG_Zlib_Init		(&cgCTX, ALG_ZLIB::CFG_Compress | ALG_ZLIB::CFG_DEFAULT_COMPRESSION);
	
	Reset();
};
//------------------------------------------------------------------------------------------//
void CSSL_T1TX::SetPort(uint32 port){
	cgMEM.SetPort(port);
};
//------------------------------------------------------------------------------------------//
void CSSL_T1TX::SetSelfName(const STDSTR& strName){
	selfName = strName;
	cgMEM.SetUpName(GetFullName(this));
	cgPkg.SetUpName(GetFullName(this));
};
//------------------------------------------------------------------------------------------//
void CSSL_T1TX::SetUpName(const STDSTR& strName){
	fatherName = strName;
	cgMEM.SetUpName(GetFullName(this));
	cgPkg.SetUpName(GetFullName(this));
};
//------------------------------------------------------------------------------------------//
void CSSL_T1TX::Reset(void){
	cgIDS.blockID = 0;
	cgIDS.orderID = 0;
	cgIDS.ctrlID = CID_Package;
	cgSendOID = 0;
	ALG_Zlib_ReInit(&cgCTX);
	cgArray.Reset();
	cgMEM.Empty();
};
//------------------------------------------------------------------------------------------//
void CSSL_T1TX::Empty(void){
	cgArray.Empty();
	cgMEM.Empty();
};
//------------------------------------------------------------------------------------------//
void CSSL_T1TX::Close(void){
	Empty();
	cgMEM.Close();
};
//------------------------------------------------------------------------------------------//
void CSSL_T1TX::Lock(G_LOCK on_off){
	if (on_off == G_LOCK_OFF){
		cgLock.Clr();
	}
	else{
		cgLock.Set();
	}
};
//------------------------------------------------------------------------------------------//
CSSL_PNFB_PKG* CSSL_T1TX::GetDefPkg(void){
	return(&cgPkg);
};
//------------------------------------------------------------------------------------------//
IOSE CSSL_T1TX::DoTransform(IOS* _ios,const UVOut& _out,const uint8* data,const uint64& length){
	uint32			flength,offset;
	IOS				ios_in,ios_out;
	T1ID			&t1ID = cgIDS;
	CSSL_PNFB_PKG	&pkg = cgPkg;
	
	IOS_clr(&ios_in);
	IOS_clr(&ios_out);
	
	cgCTX.avail_in = length;
	cgCTX.next_in = (uint8*)data;
	
	t1ID.ctrlID = CID_Package;
	do{
		cgCTX.total_in = 0;
		cgCTX.total_out = 0;
		flength = 0;
		offset = 0;
		
		if (cgArray.IsEmpty())
			cgArray.Reset();
		
		cgArray.CalcInCapacity(flength,offset);
		
		cgCTX.rcode = ALG_Zlib_Update(&cgCTX
									  ,cgArray.GetPointer(offset),flength
									  ,cgCTX.next_in,cgCTX.avail_in);
		ios_in.rcode = cgCTX.rcode;
		ios_in.avail_in = cgCTX.avail_in;
		ios_in.total_in += cgCTX.total_in;
		
		cgArray.In((uint32)cgCTX.total_out);
		
		while(cgArray.Used() > cgMaxPackage){
			if (t1ID.orderID > pkg.MaxOrderID()){
				ios_out.rcode = IOS_ERR;
				break;
			}
			cgMEM.WritePkg(&pkg,IOS_clr(&ios_out),t1ID,IUD(&cgArray,cgMaxPackage,0));
			if (ios_out.rcode > 0){
				cgArray.Out(cgMaxPackage);
				ios_in.total_out += ios_out.total_out;
				++ t1ID.orderID;
			}
			else{
				break;
			}
		};
	}while((ios_in.avail_in > 0) && (cgCTX.rcode == IOS_OK) && (ios_out.rcode > 0));
	ios_in.avail_out = ios_out.avail_out;
	IOS_update(_ios,ios_in);
	return(IOS_update(_ios,IOS_update(&ios_in.rcode, IOS_update(&ios_out.rcode,cgCTX.rcode))));
};
//------------------------------------------------------------------------------------------//
IOSE CSSL_T1TX::DoFinal(IOS* _ios,const UVOut& _out){
	uint32			flength,offset;
	IOS				ios_in,ios_out;
	T1ID			&t1ID = cgIDS;
	CSSL_PNFB_PKG	&pkg = cgPkg;
	
	IOS_clr(&ios_in);
	IOS_clr(&ios_out);
	
	cgCTX.next_in = nullptr;
	cgCTX.avail_in = 0;
	do{
		cgCTX.total_in = 0;
		cgCTX.total_out = 0;
		flength = 0;
		offset = 0;
		
		if (cgArray.IsEmpty())
			cgArray.Reset();
		
		cgArray.CalcInCapacity(flength,offset);
		
		if (cgCTX.rcode == IOS_OK){
			cgCTX.rcode = ALG_Zlib_Final(&cgCTX
										 ,cgArray.GetPointer(offset),flength);
			ios_in.rcode = cgCTX.rcode;
			ios_in.avail_in = cgCTX.avail_in;
			ios_in.total_in += cgCTX.total_in;
			
			cgArray.In((uint32)cgCTX.total_out);
		}
		while(cgArray.Used() > cgMaxPackage){
			if (t1ID.orderID > pkg.MaxOrderID()){
				ios_out.rcode = IOS_ERR;
				break;
			}
			t1ID.ctrlID = CID_Package;
			cgMEM.WritePkg(&pkg,IOS_clr(&ios_out),t1ID,IUD(&cgArray,cgMaxPackage,0));
			if (ios_out.rcode > 0){
				cgArray.Out(cgMaxPackage);
				ios_in.total_out += ios_out.total_out;
				++ t1ID.orderID;
			}
			else{
				break;
			}
		};
	}while((cgCTX.rcode == IOS_OK) && (ios_out.rcode > 0));
	
	if ((ios_out.rcode > 0) && (cgCTX.rcode == IOS_FINISH) && (t1ID.ctrlID != CID_Finish)){
		do{
			if (t1ID.orderID > pkg.MaxOrderID()){
				ios_out.rcode = IOS_ERR;
				break;
			}
			t1ID.ctrlID = CID_PackageEnd;
			cgMEM.WritePkg(&pkg,IOS_clr(&ios_out),t1ID,cgArray);
			if (ios_out.rcode > 0){
				ios_in.total_out += ios_out.total_out;
				ios_out.rcode = IOS_FINISH;
				t1ID.ctrlID = CID_Finish;
				++ t1ID.orderID;
				cgArray.Reset();
				ALG_Zlib_ReInit(&cgCTX);
			}
			else{
				t1ID.ctrlID = CID_ACK;
			}
		}while(0);
	}
	ios_in.avail_out = ios_out.avail_out;
	IOS_update(_ios,ios_in);
	return(IOS_update(_ios,IOS_update(&ios_in.rcode, IOS_update(&ios_out.rcode,cgCTX.rcode))));
};
//------------------------------------------------------------------------------------------//
IOSE CSSL_T1TX::Compress(IOS* _ios,const UVIn& _in){
	uint32			cID;
	T1ID			&t1ID = cgIDS;
	CSSL_PNFB_PKG	&pkg = cgPkg;
	
	if ((cgSendOID == t1ID.orderID) && (t1ID.orderID > pkg.MaxOrderID())){//oID over
		cID = CID_Package;
		if (cgMEM.CheckPkg(&pkg, &cID, t1ID.blockID, cgSendOID - 1) == G_FALSE){
			cgSendOID = 0;
			t1ID.orderID = 0;
		}
	}

	if (_ios->avail_in > 0){
		Transform(IOS_reset(_ios),_NONE(),_in);
		AddUVInOffset(_in, _ios->total_in);
	}
	else{
		Final(IOS_reset(_ios),_NONE());
	}
	
	return(_ios->rcode);
};
//------------------------------------------------------------------------------------------//
IOSE CSSL_T1TX::SendToT0(IOS* _ios){
	IOSE			rcode = IOS_OK;
	T1ID			&t1ID = cgIDS;
	CSSL_PNFB_PKG	&pkg = cgPkg;
	
	while(cgSendOID < t1ID.orderID){
		rcode = cgMEM.SendToT0(&pkg,_ios,SetT1ID(CID_Package,t1ID.blockID,cgSendOID));
		if (rcode > 0){
			++ cgSendOID;
			continue;
		}
		break;
	}
	return(IOS_update(_ios, rcode));
};
//------------------------------------------------------------------------------------------//
IOSE CSSL_T1TX::SendOneBlock(IOS* _ios,uint32* retFinishOID,uint32 bID,const UVIn& _in){
	//IOS_ERR-->oID over
	//IOS_NO_OBJECT-->T1 has no enough memory
	//IOS_NO_MEM-->T0 has no enough memory
	//IOS_FINISH-->finish

	IOS		ios_in,ios_out;
	T1ID	&t1ID = cgIDS;
	
	if (bID != t1ID.blockID){
		t1ID.ctrlID = CID_Package;
		t1ID.blockID = bID;
		t1ID.orderID = 0;
		cgSendOID = 0;
		cgArray.Reset();
		ALG_Zlib_ReInit(&cgCTX);
	}
	
	IOS_clr(&ios_in);
	IOS_clr(&ios_out);
	
	ios_in.avail_in = GetUVInLength(_in);
	do{
		Compress(&ios_in,_in);
		SendToT0(&ios_out);
		
		if (ios_out.rcode == IOS_NO_MEM)//T0 has no enough memory
			break;
		if (ios_in.rcode == IOS_ERR)//over oID
			break;
		if (ios_in.rcode == IOS_NO_MEM){//T1 has no enough memory
			ios_out.rcode = IOS_NO_OBJECT;
			break;
		}
		if (ios_in.rcode == IOS_FINISH)
			break;
	}while(1);
	*retFinishOID = (cgSendOID > 0) ? (cgSendOID - 1) : 0;
	ios_out.avail_in = ios_in.avail_in;
	ios_out.total_in = ios_in.total_in;
	if ((ios_in.rcode == IOS_FINISH) && (ios_out.rcode > 0))
		ios_out.rcode = IOS_FINISH;
	return(IOS_update(_ios, ios_out));
};
//------------------------------------------------------------------------------------------//
IOSE CSSL_T1TX::SendPkg(CSSL_PNFB_PKG* pkg,IOS* _ios,const T1ID& _id,const UVIn& _in){
	//IOS_ERR-->oID over
	//IOS_NO_OBJECT-->T1 has no enough memory
	//IOS_NO_MEM-->T0 has no enough memory

	IOSE	rcode = IOS_OK;
	if (pkg == nullptr)
		pkg = &cgPkg;
	
	if (_ios == nullptr){
		do{
			rcode = cgMEM.SendToT0(pkg, nullptr, _id);
			if (rcode == IOS_NO_OBJECT){//IOS_NO_OBJECT:no find
				if (cgMEM.WritePkg(pkg, nullptr, _id, _in) > 0)
					continue;
			}
			break;
		}while(1);
		return(rcode);
	}
	else{
		IOS	ios_in,ios_out;
		IOS_clr(&ios_in);
		IOS_clr(&ios_out);
		
		do{
			rcode = cgMEM.SendToT0(pkg, IOS_reset(&ios_out), _id);
			if (rcode == IOS_NO_OBJECT){//IOS_NO_OBJECT:no find
				if (cgMEM.WritePkg(pkg, IOS_reset(&ios_in), _id, _in) > 0)
					continue;
				ios_out.rcode = ios_in.rcode;
			}
			ios_out.avail_in = ios_in.avail_in;
			ios_out.total_in = ios_in.total_in;
			break;
		}while(1);
		return(IOS_update(_ios, ios_out));
	}
};
//------------------------------------------------------------------------------------------//
void CSSL_T1TX::CleanPackages(CSSL_PNFB_PKG* pkg,uint32 cID,uint32 bID,uint32 oID){
	if (pkg == nullptr)
		pkg = &cgPkg;

	cgMEM.CleanPackages(pkg, &cID, bID, oID);
};
//------------------------------------------------------------------------------------------//
void CSSL_T1TX::CleanOneBlock(uint32 bID){
	uint32			cID = CID_Package;
	CSSL_PNFB_PKG	&pkg = cgPkg;
	cgMEM.CleanPackages(&pkg,&cID, bID);
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
CSSL_T1FD::CSSL_T1FD(void) : TNFP(){
	TNFP::SetSelfName("CSSL_T1FD");
	cgRx.SetSelfName("RCH");
	cgTx.SetSelfName("TCH");
	SetUpName("");
};
//------------------------------------------------------------------------------------------//
CSSL_T1FD::~CSSL_T1FD(void){
	cgRx.RemoveSelf();
};
//------------------------------------------------------------------------------------------//
void CSSL_T1FD::InitPN(uint32 port,const CSSL_T0* csslT0,uint32 maxPackage,uint32 maxPages){
	CSSL_T0*	t0;
	t0 = (CSSL_T0*)csslT0;
	t0->Register(&cgRx);
	
	cgTx.InitPN(port,csslT0,maxPackage,maxPages);
	cgRx.InitPN(port,csslT0,maxPackage,maxPages);
	
	Reset();
};
//------------------------------------------------------------------------------------------//
void CSSL_T1FD::DetachT0(void){
	cgRx.RemoveSelf();
};
//------------------------------------------------------------------------------------------//
void CSSL_T1FD::SetPort(uint32 port){
	cgTx.SetPort(port);
	cgRx.SetPort(port);
};
//------------------------------------------------------------------------------------------//
void CSSL_T1FD::SetSelfName(const STDSTR& strName){
	selfName = strName + Str_ToStr(cgRx.GetPort());
	cgRx.SetUpName(GetFullName(this));
	cgTx.SetUpName(GetFullName(this));
};
//------------------------------------------------------------------------------------------//
void CSSL_T1FD::SetUpName(const STDSTR& strName){
	fatherName = strName;
	cgRx.SetUpName(GetFullName(this));
	cgTx.SetUpName(GetFullName(this));
};
//------------------------------------------------------------------------------------------//
void CSSL_T1FD::Reset(void){
	RxLock(G_LOCK_ON);
	cgPkgOID_RE = 0;
	cgACKsOID_RE = 0;
	cgBID_RE = 0;
	cgRx.Reset();
	RxLock(G_LOCK_OFF);
	
	TxLock(G_LOCK_ON);
	cgBID_WR = 0;
	cgTx.Reset();
	TxLock(G_LOCK_OFF);
};
//------------------------------------------------------------------------------------------//
void CSSL_T1FD::Empty(void){
	RxLock(G_LOCK_ON);
	cgRx.Empty();
	RxLock(G_LOCK_OFF);
	
	TxLock(G_LOCK_ON);
	cgTx.Empty();
	TxLock(G_LOCK_OFF);
};
//------------------------------------------------------------------------------------------//
void CSSL_T1FD::Close(void){
	Empty();
	cgRx.Close();
	cgTx.Close();
};
//------------------------------------------------------------------------------------------//
void CSSL_T1FD::RxLock(G_LOCK on_off){
	cgRx.Lock(on_off);
};
//------------------------------------------------------------------------------------------//
void CSSL_T1FD::TxLock(G_LOCK on_off){
	cgTx.Lock(on_off);
};
//------------------------------------------------------------------------------------------//
IOSE CSSL_T1FD::ReadOneBlock(const ABORTCHECK& _ac,IOS* _ios,const UVOut &_out,uint32 bID,uint32 maxDlyMS,const uint32 maxTimes){
	//IOS_NO_OBJECT:no receivce data
	//IOS_NO_MEMPRY:Out has no enough memory
	//IOS_TIMEOUT-->time out
	//IOS_ERR-->abort by external
	//IOS_FINISH-->send successful

	IOSE		rcode = IOS_OK;
	uint32		cycle = maxTimes;
	bool32		blNoRecData = G_TRUE;
	SYS_TIME_S	dlyTime;
	
	if (bID != cgBID_RE){
		cgRx.CleanOneBlock(cgBID_RE);
		cgTx.CleanPackages(cgRx.GetDefPkg(), CID_ACK, cgBID_RE, -1);
		cgPkgOID_RE = 0;
		cgACKsOID_RE = 0;
		cgBID_RE = bID;
		if (cgBID_RE > cgTx.GetDefPkg()->MaxBlockID())
			cgBID_RE = 0;
	}
	
	SYS_Delay_SetTS(&dlyTime, maxDlyMS);
	do{
		if (cgPkgOID_RE <= cgRx.GetDefPkg()->MaxOrderID())
			rcode = cgRx.ReadOneBlock(IOS_reset(_ios), _out, &cgPkgOID_RE, cgBID_RE);
		
		if (cgACKsOID_RE < cgPkgOID_RE){
			blNoRecData = G_FALSE;
			if (cgTx.SendPkg(cgRx.GetDefPkg(),nullptr, SetT1ID(CID_ACK, cgBID_RE, cgPkgOID_RE - 1),IUD("")) > 0){
				cgTx.CleanPackages(cgRx.GetDefPkg(), CID_ACK, cgBID_RE, cgPkgOID_RE - 1);
				SYS_Delay_SetTS(&dlyTime, maxDlyMS);
				if (cgPkgOID_RE > cgRx.GetDefPkg()->MaxOrderID())
					cgPkgOID_RE = 0;
				cgACKsOID_RE = cgPkgOID_RE;
				if (rcode == IOS_NO_MEM)
					break;
			}
		}

		if (cgACKsOID_RE == cgPkgOID_RE) {
			if (rcode == IOS_FINISH)
				break;
			if (SYS_Delay_IsTimeout(&dlyTime) != G_FALSE){
				if (blNoRecData != G_FALSE){
					rcode = IOS_NO_OBJECT;
					break;
				}
				if (cgTx.SendPkg(cgRx.GetDefPkg(),nullptr, SetT1ID(CID_Resend, cgBID_RE, cgPkgOID_RE),IUD("")) > 0){
					cgTx.CleanPackages(cgRx.GetDefPkg(), CID_ACK, cgBID_RE, cgPkgOID_RE);
#ifdef Test255
					DoTest255();
#endif
					if (--cycle == 0){
						rcode = IOS_TIMEOUT;
						break;
					}
					SYS_Delay_SetTS(&dlyTime, maxDlyMS);
				}
			}
		}
		if ((_ac.func != nullptr) && (_ac.func(_ac.p) != G_FALSE)){
			rcode = IOS_ERR;
			break;
		}
	}while(1);
	return(rcode);
};
//------------------------------------------------------------------------------------------//
IOSE CSSL_T1FD::Read(const ABORTCHECK& _ac,IOS* _ios,const UVOut &_out,uint32 maxDlyMS,const uint32 maxTimes){
	//IOS_NO_OBJECT:no receivce data
	//IOS_NO_MEMPRY:Out has no enough memory
	//IOS_TIMEOUT-->time out
	//IOS_ERR-->abort by external
	//IOS_FINISH-->send successful
	IOSE	rcode = IOS_NO_OBJECT;
	uint32	bID,oID;
	
	RxLock(G_LOCK_ON);
#ifdef LOGT1_RX_ENABLE
	ALG_MD5	md5;
	STDSTR	str_md5;
	IOS		ios;
#endif

	bID = cgBID_RE;
	oID = 0;
	do{
		if (cgRx.GetMinBID(&bID, &oID) == IOS_OK){
			if (oID == 0){
				ELogTNFP(ELogTNFP_T1_RX,ELogT1_RX(LogTitle(this,"CSSL_T1FD","Read","::") << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<"));
#ifdef LOGT1_RX_ENABLE
				md5.InitCFG();
				rcode = ReadOneBlock(_ac,IOS_clr(&ios),OUD(&md5,_out),bID,maxDlyMS,maxTimes);
				md5.GetResult(nullptr, OUD_HEXs(_EMPTY(&str_md5)));
				IOS_update(_ios, ios);
#else
				rcode = ReadOneBlock(_ac,_ios,_out,bID,maxDlyMS,maxTimes);
#endif
				ELogLock(1);
				ELogTNFP(ELogTNFP_T1_RX,ELogT1_RX(LogTitle(this,"CSSL_T1FD","Read","::") << LogBLK1("Data length=", ios.total_out) << "MD5=" << str_md5));
				ELogTNFP(ELogTNFP_T1_RX,ELogT1_RX(LogTitle(this,"CSSL_T1FD","Read","::") << "Read result:" << _GetIOSEMesg(rcode)));
				ELogTNFP(ELogTNFP_T1_RX,ELogT1_RX(LogTitle(this,"CSSL_T1FD","Read","::") << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<"));
				ELogLock(0);
			}
			else{
				cgRx.CleanOneBlock(bID);
				continue;
			}
		}
		break;
	}while(1);
	RxLock(G_LOCK_OFF);
	return(rcode);
};
//------------------------------------------------------------------------------------------//
IOSE CSSL_T1FD::SendOneBlock(const ABORTCHECK& _ac,IOS* _ios,uint32 bID,uint32 maxDlyMS,const UVIn& _in){
	//IOS_ERR-->abort by external
	//IOS_FINISH-->send successful
	//IOS_TIMEOUT-->time out
	ELogLine(bool32 blPrint = G_TRUE);
	IOSE		rcode;
	IOSIN		iosin;
	bool32		blRecACK = G_FALSE;
	uint32		cID,oID,pkgOID = 0,ackOID = 0;
	SYS_TIME_S	dlyTime;
	
	if (bID != cgBID_WR){
		cgTx.CleanOneBlock(cgBID_WR);
		cgRx.CleanPackages(cgTx.GetDefPkg(), CID_ACK, cgBID_RE, -1);
		cgBID_WR = bID;
		if (cgBID_WR > cgTx.GetDefPkg()->MaxBlockID())
			cgBID_WR = 0;
	}

	SYS_Delay_SetTS(&dlyTime, maxDlyMS);
	do{
		cID = CID_ACK;
		rcode = cgTx.SendOneBlock(IOS_reset(_ios),&pkgOID,cgBID_WR,_in);
		
		if ((blRecACK == G_FALSE) || (ackOID < pkgOID)){
			ELogTNFP(ELogTNFP_T1_TX,ELogIF(blPrint,ELogT1_TX(LogTitle(this,"CSSL_T1FD","SendOneBlock","::")
															 << LogBLK1("Waiting receive:")
															 << LogBLK2("")
															 <<LogIDS(cID, cgBID_WR, pkgOID))));
			ELogLine(blPrint = G_FALSE);
			oID = pkgOID;
			blRecACK = (cgRx.ReadMaxACK(cgTx.GetDefPkg(),nullptr, _NONE(), &cID, cgBID_WR, &oID) > 0);
			if (blRecACK == G_TRUE){
				ELogLine(blPrint = G_TRUE);
				SYS_Delay_SetTS(&dlyTime, maxDlyMS);
				ackOID = oID;
				if (cID == CID_Resend){
					if (ackOID == 0){
						ackOID = cgTx.GetDefPkg()->MaxOrderID();
					}
					if (oID <= pkgOID){
						IOS_copy(&iosin, _ios);
						cgTx.SendPkg(nullptr,IOS_reset(_ios), SetT1ID(CID_Package, cgBID_WR, oID),IUD(""));
						IOS_hdcopy(_ios,iosin);
					}
					if (oID > 0){
						-- ackOID;
						-- oID;
					}
				}
				if (oID > 0)
					cgTx.CleanPackages(nullptr,CID_Package, cgBID_WR, oID);
				if (ackOID == cgTx.GetDefPkg()->MaxOrderID())
					blRecACK = G_FALSE;
			}
			else{
				SYS_SleepMS(1);
				rcode = IOS_ERR;
			}
		}
		if ((rcode == IOS_FINISH) && (ackOID == pkgOID)){
			cgTx.CleanPackages(nullptr,CID_Package, cgBID_WR, ackOID);
			break;
		}
		if (_ac.func != nullptr){
			if ((_ac.func(_ac.p) != G_FALSE)){
				rcode = IOS_ERR;
				break;
			}
		}
		if (SYS_Delay_IsTimeout(&dlyTime) != G_FALSE){
			rcode = IOS_TIMEOUT;
			break;
		}
	}while(1);
	return(rcode);
};
//------------------------------------------------------------------------------------------//
IOSE CSSL_T1FD::Send(const ABORTCHECK& _ac,IOS* _ios,uint32 maxDlyMS,const UVIn& _in){
	//IOS_ERR-->abort by external
	//IOS_FINISH-->send successful
	//IOS_TIMEOUT-->time out
	IOSE	rcode;
	
	if (DS_IO_NODE::GetDSIOList().GetUVInLength(_in) == 0)
		return IOS_OK;
	TxLock(G_LOCK_ON);
#ifdef LOGT1_TX_ENABLE
	ALG_MD5	md5;
	STDSTR	str_md5;
	
	md5.Calc(nullptr, OUD_HEXs(&str_md5), _NONE(), _in);
#endif
	ELogLock(1);
	ELogTNFP(ELogTNFP_T1_TX,ELogT1_TX(LogTitle(this,"CSSL_T1FD","Send","::")
									  << LogBLK1("Data length=", DS_IO_NODE::GetDSIOList().GetUVInLength(_in))
									  << "MD5=" << str_md5));
	ELogTNFP(ELogTNFP_T1_TX,ELogT1_TX(LogTitle(this,"CSSL_T1FD","Send","::") << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>"));
	ELogLock(0);
	rcode = SendOneBlock(_ac,_ios,cgBID_WR + 1,maxDlyMS,_in);
	ELogLock(1);
	ELogTNFP(ELogTNFP_T1_TX,ELogT1_TX(LogTitle(this,"CSSL_T1FD","Send","::") << "Send result:" << _GetIOSEMesg(rcode)));
	ELogTNFP(ELogTNFP_T1_TX,ELogT1_TX(LogTitle(this,"CSSL_T1FD","Send","::") << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>"));
	ELogLock(0);
	cgTx.CleanOneBlock(cgBID_WR);
	cgRx.CleanPackages(cgTx.GetDefPkg(), CID_ACK, cgBID_RE, -1);
	TxLock(G_LOCK_OFF);
	return(rcode);
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
CSSL_T1::CSSL_T1(void) : TNFP(){
	cgPort = 0;
	cgT1FD.SetSelfName("FD");
	SetUpName("");
};
//------------------------------------------------------------------------------------------//
void CSSL_T1::Init(uint32 port,const CSSL_T0* csslT0,uint32 maxPackage,uint32 maxPages){
	cgT1FD.InitPN(port,csslT0,maxPackage,maxPages);
	cgPort = port;
};
//------------------------------------------------------------------------------------------//
void CSSL_T1::SetSelfName(const STDSTR& strName){
	selfName = strName;
	cgT1FD.SetUpName(GetFullName(this));
};
//------------------------------------------------------------------------------------------//
void CSSL_T1::SetUpName(const STDSTR& strName){
	fatherName = strName;
	cgT1FD.SetUpName(GetFullName(this));
};
//------------------------------------------------------------------------------------------//
void CSSL_T1::Reset(void){
	cgT1FD.Reset();
};
//------------------------------------------------------------------------------------------//
void CSSL_T1::Empty(void){
	cgT1FD.Empty();
};
//------------------------------------------------------------------------------------------//
void CSSL_T1::Close(void){
	cgT1FD.Close();
};
//------------------------------------------------------------------------------------------//
void CSSL_T1::DetachT0(void){
	cgT1FD.DetachT0();
};
//------------------------------------------------------------------------------------------//
uint32 CSSL_T1::GetPort(void){
	return(cgPort);
};
//------------------------------------------------------------------------------------------//
void CSSL_T1::SetPort(uint32 port){
	cgT1FD.SetPort(port);
};
//------------------------------------------------------------------------------------------//
void CSSL_T1::SetDelayPar(uint32 maxDlyMS,uint32 maxTimes){
	cgMaxDlyMS = maxDlyMS;
	cgMaxTimes = maxTimes;
};
//------------------------------------------------------------------------------------------//
IOSE CSSL_T1::Read (const ABORTCHECK& _ac,IOS* _ios,const UVOut &_out){
	IOSE	rcode;
	cgT1FD.RxLock(G_LOCK_ON);
	rcode = cgT1FD.Read(_ac,_ios,_out, cgMaxDlyMS/cgMaxTimes, cgMaxTimes);
	cgT1FD.RxLock(G_LOCK_OFF);
	return(rcode);
};
//------------------------------------------------------------------------------------------//
IOSE CSSL_T1::Send (const ABORTCHECK& _ac,IOS* _ios,const UVIn& _in){
	IOSE	rcode;
	cgT1FD.TxLock(G_LOCK_ON);
	rcode = cgT1FD.Send(_ac, _ios, cgMaxDlyMS, _in);
	cgT1FD.TxLock(G_LOCK_OFF);
	return(rcode);
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
void CSSL_T1MSG::Init(uint32 port,const CSSL_T0* csslT0,uint32 maxPackage,uint32 maxPages){
	CSSL_T1::Init(port,csslT0,maxPackage,maxPages);
	cgArrayTx.InitSize(MESG_SIZE);
	cgArrayRx.InitSize(MESG_SIZE);

	cgMsg.InitPN(&cgArrayTx,&cgArrayRx,1,0,G_ENDIAN_LITTLE);
};
//------------------------------------------------------------------------------------------//
uint32&	CSSL_T1MSG::Read(const ABORTCHECK& _ac,IOS* _ios,uint32* mID,const UVOut& msg){
	*mID = 0;
	cgT1FD.RxLock(G_LOCK_ON);
	cgMsg.CleanRE();
	if (cgT1FD.Read(_ac, _ios, cgMsg.GetDefArrayRE(), cgMaxDlyMS/cgMaxTimes, cgMaxTimes) > 0){
		if (cgMsg.TryGetFrame() > 0){
			*mID = cgMsg.ReadID();
			cgMsg.Read(_ios,msg);
		}
	}
	cgT1FD.RxLock(G_LOCK_OFF);
	return(*mID);
};
//------------------------------------------------------------------------------------------//
IOSE CSSL_T1MSG::Send(const ABORTCHECK& _ac,IOS* _ios,uint32 mID,const UVIn& _in){
	IOSE	rcode;
	IOS ios,ios2;
	IOS_clr(&ios);
	IOS_clr(&ios2);
	
	cgT1FD.TxLock(G_LOCK_ON);
	cgMsg.CleanWR();
	cgMsg.Write(&ios2,mID,_in);
	rcode = cgT1FD.Send(_ac, &ios , cgMaxDlyMS, cgMsg.GetDefArrayWR());
	cgT1FD.TxLock(G_LOCK_OFF);
	ios.avail_in = 0;
	ios.total_in = 0;
	if (rcode == IOS_FINISH)
		ios.total_in = ios2.total_in;
	IOS_update(_ios,ios);
	return(rcode);
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
CSSL_DEV::CSSL_DEV(void) : TNFP(){
	cgT1Map.InitSize(256);
	cgT1Map.SetOneBit(0);
	
	cgT0.SetPackageDlyMS(CSSL_T0::MAX_PACKAGE_DEALYMS * 3);
	cgT0.Reset();
	
	SetSelfName				("CSSL_DEV");
	cgT0.SetSelfName		("T0");
	SetUpName("");
};
//------------------------------------------------------------------------------------------//
CSSL_DEV::~CSSL_DEV(void){
	CleanDownTree(this, this);
};
//------------------------------------------------------------------------------------------//
void CSSL_DEV::SetSelfName(const STDSTR& strName){
	selfName = strName;
	cgT0.SetUpName(GetFullName(this));
	TREE_DownChain_Traversal_LINE(CSSL_T1, this,_opNode->SetUpName(GetFullName(this)););
};
//------------------------------------------------------------------------------------------//
void CSSL_DEV::SetUpName(const STDSTR& strName){
	fatherName = strName;
	cgT0.SetUpName(GetFullName(this));
	TREE_DownChain_Traversal_LINE(CSSL_T1, this,_opNode->SetUpName(GetFullName(this)););
};
//------------------------------------------------------------------------------------------//
void CSSL_DEV::Init(const ARRAY* _out,const ARRAY* _in){
	cgT0.InitPN(_out, _in);
	cgT0.Reset();
};
//------------------------------------------------------------------------------------------//
CSSL_T1* CSSL_DEV::GetT1(uint32 port){
	CSSL_T1	*csslT1 = nullptr;
	
	TREE_DownChain_Traversal_LINE
	(CSSL_T1, this,
		if (_opNode->GetPort() == port){
			csslT1 = _opNode;
			break;
		}
	);
	return(csslT1);
};
//------------------------------------------------------------------------------------------//
CSSL_T1* CSSL_DEV::AcquireT1(uint32 maxPackage,uint32 maxPages,uint32 maxDlyMS,uint32 maxTimes){
	CSSL_T1	*csslT1 = nullptr;
	uint32	port;

	cgT1MapLock.Set();
	port = (uint32)cgT1Map.FindFirstZeroBit();
	if (port < cgT1Map.MaxSize()){
		cgT1Map.SetOneBit(port);
	}
	else{
		port = 0;
	}
	cgT1MapLock.Clr();
	
	if (port > 0){
		csslT1 = new CSSL_T1;
		if (csslT1 != nullptr){
			AppendDownNode(csslT1);
			csslT1->Init(port, &cgT0, maxPackage, maxPages);
			csslT1->SetDelayPar(maxDlyMS, maxTimes);
			csslT1->SetSelfName("T1-" + Str_ToStr(port));
			csslT1->SetUpName(GetFullName(this));
		}
	}

	return(csslT1);
};
//------------------------------------------------------------------------------------------//
CSSL_T1MSG* CSSL_DEV::AcquireT1MSG(uint32 maxPackage,uint32 maxPages,uint32 maxDlyMS,uint32 maxTimes){
	CSSL_T1MSG	*csslT1 = nullptr;
	uint32		port;
	
	cgT1MapLock.Set();
	port = (uint32)cgT1Map.FindFirstZeroBit();
	if (port < cgT1Map.MaxSize()){
		cgT1Map.SetOneBit(port);
	}
	else{
		port = 0;
	}
	cgT1MapLock.Clr();
	
	if (port > 0){
		csslT1 = new CSSL_T1MSG;
		if (csslT1 != nullptr){
			AppendDownNode(csslT1);
			csslT1->Init(port, &cgT0, maxPackage, maxPages);
			csslT1->SetDelayPar(maxDlyMS, maxTimes);
			csslT1->SetSelfName("T1-" + Str_ToStr(port));
			csslT1->SetUpName(GetFullName(this));
		}
	}
	
	return(csslT1);
};
//------------------------------------------------------------------------------------------//
void CSSL_DEV::SetPort(CSSL_T1* csslT1,uint32 port){
	cgT1MapLock.Set();
	cgT1Map.ClrOneBit(csslT1->GetPort());
	cgT1Map.SetOneBit(port);
	cgT1MapLock.Clr();
	csslT1->SetPort(port);
};
//------------------------------------------------------------------------------------------//
void CSSL_DEV::ReleaseT1(CSSL_T1* csslT1){
	if (csslT1 != nullptr){
		cgT1MapLock.Set();
		cgT1Map.ClrOneBit(csslT1->GetPort());
		cgT1MapLock.Clr();
		csslT1->DetachT0();
		csslT1->RemoveSelf();
		delete csslT1;
	}
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
#ifdef ADS_SSL_TEST
//------------------------------------------------------------------------------------------//
void CSSL_TEST_PORT::Init(uint32 port,const ARRAY* _out,const ARRAY* _in,uint32 maxPackage,uint32 maxPages){
	cgT0.InitPN(_out, _in);
	cgT1.Init(port, &cgT0, maxPackage, maxPages);
	
	t0Thread.ThreadInit		(this,&CSSL_TEST_PORT::T0ThreadFunc		,"T0");
	t1RxThread.ThreadInit	(this,&CSSL_TEST_PORT::T1RxThreadFunc	,"T1Rx");
	t1TxThread.ThreadInit	(this,&CSSL_TEST_PORT::T1TxThreadFunc	,"T1Tx");
	
	TNFP::SetSelfName	("CSSL_TEST_PORT");
	cgT0.SetSelfName	("T0");
	cgT1.SetSelfName	("T1");
	
	cgT1.SetDelayPar(5000, 5);
	EchoEnable(G_FALSE);
	
	cgT0.Reset();
};
//------------------------------------------------------------------------------------------//
void CSSL_TEST_PORT::SetSelfName(const STDSTR& strName){
	selfName = strName;
	cgT0.SetUpName(GetFullName(this));
	cgT1.SetUpName(GetFullName(this));
	t0Thread.SetUpName(GetFullName(this));
	t1RxThread.SetUpName(GetFullName(this));
	t1TxThread.SetUpName(GetFullName(this));
};
//------------------------------------------------------------------------------------------//
void CSSL_TEST_PORT::SetUpName(const STDSTR& strName){
	fatherName = strName;
	cgT0.SetUpName(GetFullName(this));
	cgT1.SetUpName(GetFullName(this));
	t0Thread.SetUpName(GetFullName(this));
	t1RxThread.SetUpName(GetFullName(this));
	t1TxThread.SetUpName(GetFullName(this));
};
//------------------------------------------------------------------------------------------//
bool32 CSSL_TEST_PORT::T0ThreadFunc(void* p){
	
	ETLogThreadStart(t0Thread);
	
	while(t0Thread.IsTerminated() == G_FALSE){
		
		cgT0.Delivery();
		
		SYS_SleepMS(2);
	}
	ETLogThreadStop(t0Thread);
	return G_TRUE;
};
//------------------------------------------------------------------------------------------//
bool32 CSSL_TEST_PORT::T1RxThreadFunc(void* strMD5){
	IOS		ios;
	STDSTR	str_rec,str_md5,*str;
	ALG_MD5	md5;
	
	str = (STDSTR*)strMD5;
	ELogLock(1);
	ETLogThreadStart(t1RxThread);
	ELog(ELogThread(t1RxThread,"Expect  MD5=" << *str));
	ELogLock(0);
	
	while(t1RxThread.IsTerminated() == G_FALSE){
		md5.InitCFG();
		if (cgT1.Read(ACF(),IOS_clr(&ios),OUD(&md5,_EMPTY(&str_rec))) > 0){
			md5.GetResult(nullptr, OUD_HEXs(&str_md5));
			str = (STDSTR*)strMD5;
			ELogLock(1);
			ELog(ELogThread(t1RxThread,"Expect  MD5=" << *str));
			ELog(ELogThread(t1RxThread,"Receive MD5=" << str_md5));
			ELog(ELogThread(t1RxThread,"ios:in/out=" << ios.total_in << "/" << ios.total_out));
			ELogLock(0);
			if (cgblEcho){
				t1TxThread.ThreadStop();
				t1TxThread.ThreadRun(&str_rec);
			}
			break;
		}
		SYS_SleepMS(2);
	}
	
	ETLogThreadStop(t1RxThread);
	return G_TRUE;
};
//------------------------------------------------------------------------------------------//
bool32 CSSL_TEST_PORT::T1TxThreadFunc(void* strData){
	IOS		ios;
	
	ETLogThreadStart(t1TxThread);
	
	cgT1.Send(ACF(),IOS_clr(&ios), cgComStrBuf);
	ELogLock(1);
	ELog(ELogThread(t1TxThread,"ios:in/out=" << ios.total_in << "/" << ios.total_out));
	ETLogThreadStop(t1TxThread);
	ELogLock(0);
	SYS_SleepMS(10);
	return G_TRUE;
};
//------------------------------------------------------------------------------------------//
void CSSL_TEST_PORT::StartT0(void){
	t0Thread.ThreadRun();
};
//------------------------------------------------------------------------------------------//
void CSSL_TEST_PORT::RecInThread(const STDSTR& strMD5){
	t0Thread.ThreadRun();
	t1RxThread.ThreadRun((void*)&strMD5);
};
//------------------------------------------------------------------------------------------//
void CSSL_TEST_PORT::Stop(void){
	t0Thread.ThreadStop();
	t1RxThread.ThreadStop();
	t1TxThread.ThreadStop();
};
//------------------------------------------------------------------------------------------//
void CSSL_TEST_PORT::EchoEnable(bool32 blEnable){
	cgblEcho = (blEnable != G_FALSE);
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
void CSSL_TEST::DefultInit(void){
	InitA(4,4);
	InitB(4,4);
	InitXY(128,128);
	
	Start();
	SYS_SleepMS(100);
};
//------------------------------------------------------------------------------------------//
void CSSL_TEST::InitA(uint32 maxPackage,uint32 maxPages){
	cgPortA.Init(0, &cgArrayX, &cgArrayY, maxPackage, maxPages);
	
	TNFP::SetSelfName	("CSSL_TEST");
	cgPortA.SetSelfName	("PortA");
	cgPortB.SetSelfName	("PortB");
};
//------------------------------------------------------------------------------------------//
void CSSL_TEST::InitB(uint32 maxPackage,uint32 maxPages){
	cgPortB.Init(0, &cgArrayY, &cgArrayX, maxPackage, maxPages);
	
	TNFP::SetSelfName	("CSSL_TEST");
	cgPortA.SetSelfName	("PortA");
	cgPortB.SetSelfName	("PortB");
};
//------------------------------------------------------------------------------------------//
void CSSL_TEST::InitXY(uint32 maxPackageX,uint32 maxPackageY){
	cgArrayX.InitSize(CSSL_T0::MAX_PACKAGE_SIZE * maxPackageX);
	cgArrayY.InitSize(CSSL_T0::MAX_PACKAGE_SIZE * maxPackageY);
};
//------------------------------------------------------------------------------------------//
void CSSL_TEST::SetSelfName(const STDSTR& strName){
	selfName = strName;
	cgPortA.SetUpName(GetFullName(this));
	cgPortB.SetUpName(GetFullName(this));
};
//------------------------------------------------------------------------------------------//
void CSSL_TEST::SetUpName(const STDSTR& strName){
	fatherName = strName;
	cgPortA.SetUpName(GetFullName(this));
	cgPortB.SetUpName(GetFullName(this));
};
//------------------------------------------------------------------------------------------//
void CSSL_TEST::Start(void){
	cgPortA.StartT0();
	cgPortB.StartT0();
};
//------------------------------------------------------------------------------------------//
void CSSL_TEST::Stop(void){
	cgPortA.Stop();
	cgPortB.Stop();
};
//------------------------------------------------------------------------------------------//
bool32 CSSL_TEST::TestAtoB(const UVIn& _in){
	IOS			ios;
	ALG_MD5		md5;
	STDSTR		str_md5;
	
	cgPortA.EchoEnable(G_FALSE);
	cgPortB.EchoEnable(G_FALSE);
	
	md5.Calc(IOS_clr(&ios), OUD_HEXs(&str_md5), _NONE(), _in);
	ELog(LogTitle(this,"CSSL_TEST","EchoTestAtoB","::")
		 << LogBLK1("Data length=",ios.total_in)
		 << "MD5=" << str_md5);
	
	cgPortB.Stop();
	cgPortB.RecInThread(str_md5);
	SYS_SleepMS(10);
	
	cgPortA.cgT1.Send(ACF(),IOS_clr(&ios), _in);
	cgPortB.Stop();
	ELog(LogTitle(this,"CSSL_TEST","EchoTestAtoB","::") << "ios:in/out=" << ios.total_in << "/" << ios.total_out);
	
	SYS_SleepMS(500);
	return G_TRUE;
};
//------------------------------------------------------------------------------------------//
#endif
//------------------------------------------------------------------------------------------//
#endif /* ADS_SSL_h */
