//
//  Commu_SSL.cpp
//  SDT
//
//  Created by Leif Wen on 26/01/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "stdafx.h"
#include "ADS_SSL.h"
//------------------------------------------------------------------------------------------//
#ifdef ADS_SSL_h
//------------------------------------------------------------------------------------------//
#include "ALG_zlib.h"
#include "DS_STRING.h"
#include "SYS_Time.h"
//------------------------------------------------------------------------------------------//
//#define LOGPRINT_ENABLE

//#define LOGT0_ENABLE
//#define LOGT1IN_ENABLE
//#define LOGT1OUT_ENABLE

#ifdef LOGT0_ENABLE
	#define ELogT0(x) ELog(x);
#else
	#define ELogT0(x);
#endif

#ifdef LOGT1IN_ENABLE
	#define ELogT1IN(x) ELog(x);
#else
	#define ELogT1IN(x);
#endif

#ifdef LOGT1OUT_ENABLE
	#define ELogT1OUT(x) ELog(x);
#else
	#define ELogT1OUT(x);
#endif

#include "SYS_Log.h"
//------------------------------------------------------------------------------------------//
CSSL_T0::CSSL_T0(void){
	cgDefCFG = ALG_AES::CFG_AES256 | ALG_AES::CFG_AES_CFB8;
	cgDefKey = ALG_AESKey32Bye("LF.W.TZ");
	cgPackageDlyMS = MAX_PACKAGE_DEALYTM;
	
	TNFP::SetSelfName("CSSL_T0");
	SetSelfName(selfName);
	SetFatherName("");
};
//------------------------------------------------------------------------------------------//
void CSSL_T0::InitPN(const ARRAY* _out,const ARRAY* _in,G_ENDIAN endian){
	cgMsg.InitPN(_out,_in,1,CRC16ID::eXMODEM,endian);
};
//------------------------------------------------------------------------------------------//
void CSSL_T0::SetSelfName(const STDSTR& strName){
	selfName = strName;
	cgMsg.SetFatherName(GetFullName(this));
};
//------------------------------------------------------------------------------------------//
void CSSL_T0::SetFatherName(const STDSTR& strName){
	fatherName = strName;
	cgMsg.SetFatherName(GetFullName(this));
};
//------------------------------------------------------------------------------------------//
void CSSL_T0::SetConfig(uint32 cfg,const STDSTR& sKey){
	InUse_set();
	cgMsg.InitCFG(cfg | CFG_INIT_RE_CFGPAR | CFG_INIT_WR_CFGPAR ,&sKey);
	InUse_clr();
};
//------------------------------------------------------------------------------------------//
void CSSL_T0::SetKey(const STDSTR& sKey){
	InUse_set();
	cgMsg.InitCFG(CFG_INIT_RE_PAR | CFG_INIT_WR_PAR,&sKey);
	InUse_clr();
};
//------------------------------------------------------------------------------------------//
void CSSL_T0::Reset(void){
	
	SetConfig(cgDefCFG,cgDefKey);
};
//------------------------------------------------------------------------------------------//
inline void CSSL_T0::Register(TNF* csslT1){
	cgT1SList.AddNode(csslT1);
};
//------------------------------------------------------------------------------------------//
void CSSL_T0::SetPackageDlyMS(double dlyMS){
	static	int32 count = 0;
	InUse_set();
	cgPackageDlyMS = 50 + (cgPackageDlyMS + dlyMS) / 2;
	if (cgPackageDlyMS > MAX_PACKAGE_DEALYTM){
		++ count;
		cgPackageDlyMS = MAX_PACKAGE_DEALYTM;
	}
	else{
		count = 0;
	}
	if (count > 1){
		cgPackageDlyMS = 200;
		count = 0;
	}
	InUse_clr();
};
//------------------------------------------------------------------------------------------//
double CSSL_T0::GetPackageDlyMS(void){
	double	dt;
	InUse_set();
	dt = cgPackageDlyMS;
	InUse_clr();
	return(dt);
};
//------------------------------------------------------------------------------------------//
void CSSL_T0::Delivery(void){
	if (cgMsg.GetDefArrayRE()->Used() > 0){
		ELogT0(this << "CSSL_T0.Delivery()::Rec:" << cgMsg.GetDefArrayRE()->Used() << "/" << cgMsg.GetDefArrayRE()->Unused());
		while(cgMsg.TryGetFrame() > 0){
			ELogT0(this << "CSSL_T0.Delivery()::Get one T0 mesage:Port=" << cgMsg.ReadID() << ",MLen=" << cgMsg.GetLengthRE());
			cgPort = cgMsg.ReadID();
			cgMsg.Read(nullptr,OUD(this));
			cgMsg.Out();
			ELogT0(this << "CSSL_T0.Delivery()::Out:" << cgMsg.GetDefArrayRE()->Used() << "/" << cgMsg.GetDefArrayRE()->Unused());
		};
	}
};
//------------------------------------------------------------------------------------------//
CSSL_T0& CSSL_T0::DoTransform(IOSTATUS* _ios,const UVOut& _out,const uint8* data,const uint64& length){
	if (_out.uvid == UVID_SELF){
		if (length == 0)
			return(*this);
		TREE_LChildRChain_Traversal_LINE_nolock(TNF,(&cgT1SList),
			if (((CSSL_T1HD*)_opNode)->GetPort() == cgPort){
				((CSSL_T1HD*)_opNode)->ReceiveFromT0(nullptr,data,length);
				break;
			}
		);
		if (_ios != nullptr){
			_ios->avail_in -= length;
			_ios->total_in += length;
			_ios->avail_out = 0;
			_ios->total_out += length;
		}
	}
	else{
		DoTransform(_ios,_out,data,length);
	}
	return(*this);
};
//------------------------------------------------------------------------------------------//
bool32 CSSL_T0::Send(IOSTATUS* _ios,uint32 port,const UVIn& _in){
	IOSTATUS ios;
	
	IOSTATUS_Clr(&ios);
	InUse_set();
	cgMsg.Write(&ios, port, _in);
	InUse_clr();
	IOSTATUS_Add(_ios,ios);
	ELogT0(this << "CSSL_T0.Send()::Port=" << port<< ",ios.total_in/total_out=" << ios.total_in << "/" << ios.total_out);
	ELogT0(this << "CSSL_T0.Send()::Array:" << cgMsg.GetDefArrayWR()->Used() << "/" << cgMsg.GetDefArrayWR()->Unused());
	return(ios.status > IOS_NOMEM);
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
CSSL_PNFBT1::CSSL_PNFBT1(void) : PNF_BLOCK(){
	Add(pn_CtrlID) < pn_BlockID < pn_OrderID < pn_Info;
};
//------------------------------------------------------------------------------------------//
void CSSL_PNFBT1::InitPN(const ARRAY* _out,const ARRAY* _in,G_ENDIAN endian){
	if (_out == nullptr)
		_out = &cgArray;
	if (_in == nullptr)
		_in = &cgArray;

	PNF_BLOCK::InitPN	(_out,_in,&pn_Info);
	pn_CtrlID.InitPN	(_out,_in,1,endian);
	pn_BlockID.InitPN	(_out,_in,2,endian);
	pn_OrderID.InitPN	(_out,_in,2,endian);
	pn_Info.InitPN		(_out,_in);
};
//------------------------------------------------------------------------------------------//
uint32 CSSL_PNFBT1::ReadCtrlID	(void)const	{return(pn_CtrlID.GetValueCalcRE());};
uint32 CSSL_PNFBT1::ReadBlockID	(void)const	{return(pn_BlockID.GetValueCalcRE());};
uint32 CSSL_PNFBT1::ReadOrderID	(void)const	{return(pn_OrderID.GetValueCalcRE());};
//------------------------------------------------------------------------------------------//
const CSSL_PNFBT1& CSSL_PNFBT1::Write(IOSTATUS* _ios,const T1ID& _id,const UVIn& _in){
	PNF_BLOCK::_Begin(_ios);
	pn_CtrlID.Write	(_ios,_id.ctrlID);
	pn_BlockID.Write(_ios,_id.blockID);
	pn_OrderID.Write(_ios,_id.orderID);
	pn_Info.Write	(_ios,_in);
	PNF_BLOCK::_Endl();
	return(*this);
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
enum{
	CSSL_Forward = 0,
	CSSL_Package,
	CSSL_ENDPackage,
	CSSL_ACK,
	CSSL_ENDACK,
	CSSL_ResendPackage,
	CSSL_ResendAll,
};
#define _GetMesgCID(cid) (cid == CSSL_Package 		? "Package"\
						:(cid == CSSL_ENDPackage 	? "ENDPackage"\
						:(cid == CSSL_ACK 			? "ACK"\
						:(cid == CSSL_ENDACK 		? "ENDACK"\
						:(cid == CSSL_ResendPackage ? "ResendPackage"\
						:(cid == CSSL_ResendAll 	? "ResendAll"\
						: Str_ToStr(cid)))))))
#define ELogT1IN_ID(_name,_id,_len) \
ELogT1IN(this << "CSSL_PNFT1." << _name << "()::BID=" << _id.blockID << ",OID=" << _id.orderID << ",CID=" << _GetMesgCID(_id.ctrlID) << ",Len=" << _len);
//------------------------------------------------------------------------------------------//
CSSL_T1_RX::CSSL_T1_RX(void) : DSTF(){
	TNFP::SetSelfName("CSSL_PNFT1_RX");
	cgZLib.SetSelfName("ZLIB");
	SetFatherName("");
};
//------------------------------------------------------------------------------------------//
void CSSL_T1_RX::InitPN(G_ENDIAN endian){
	pn_L1.InitPN		(nullptr,nullptr,endian);
	pn_L1.InitSize		(CSSL_PNFBT1::MAX_PACKAGE_SIZE * 16);
	cgZLib.InitSize		(CSSL_PNFBT1::MAX_PACKAGE_SIZE * 2);
	cgZLib.InitCFG		(ALG_ZLIB::CFG_Uncompress);
	
	Reset();
}
//------------------------------------------------------------------------------------------//
void CSSL_T1_RX::SetSelfName(const STDSTR& strName){
	selfName = strName;
	cgZLib.SetFatherName(GetFullName(this));
};
//------------------------------------------------------------------------------------------//
void CSSL_T1_RX::SetFatherName(const STDSTR& strName){
	fatherName = strName;
	cgZLib.SetFatherName(GetFullName(this));
};
//------------------------------------------------------------------------------------------//
void CSSL_T1_RX::ReceiveFromT0(IOSTATUS* _ios,const uint8* data,const uint64& length){
	Save(_ios, pn_L1.GetDefArrayWR(), data, length);
};
//------------------------------------------------------------------------------------------//
bool32 CSSL_T1_RX::HasData(void){
	return(pn_L1.GetDefArrayRE()->Used() > 0);
};
//------------------------------------------------------------------------------------------//
void CSSL_T1_RX::Empty(void){
	pn_L1.cgArray.Empty();
}
//------------------------------------------------------------------------------------------//
void CSSL_T1_RX::Reset(void){
	cgIDRE.blockID = 0;
	cgIDRE.orderID = 0;
	Empty();
};
//------------------------------------------------------------------------------------------//
bool32 CSSL_T1_RX::ReadNext(IOSTATUS* _ios,const UVOut& _out,T1ID* _id){
	uint32 offset,bID,oID;
	
	if (HasData() == 0)
		return 0;
	
	offset = 0;
	
	if (_id != nullptr){
		_id->blockID = cgIDRE.blockID;
		_id->orderID = cgIDRE.orderID;
	}
	
	while(pn_L1.Analysis(offset) > 0){
		ELogT1OUT(this << "CSSL_PNFT1.ReadNext()::L1.Rx :S=" << pn_L1.GetDefArrayRE()->Used() << "/" << pn_L1.GetDefArrayRE()->Unused());
		ELogT1OUT(this << "CSSL_PNFT1.ReadNext()::Expect:BID=" << cgIDRE.blockID << ",OID=" << cgIDRE.orderID);
		ELogT1OUT(this << "CSSL_PNFT1.ReadNext()::Rec   :BID=" << pn_L1.ReadBlockID()
				  << ",OID=" << pn_L1.ReadOrderID()
				  << ",CID=" << _GetMesgCID(pn_L1.ReadCtrlID())
				  << ",MLen=" << pn_L1.GetLengthRE());
		
		bID = pn_L1.ReadBlockID();
		if (bID == cgIDRE.blockID){
			oID = pn_L1.ReadOrderID();
			if (oID == cgIDRE.orderID){
				_id->ctrlID = pn_L1.ReadCtrlID();
				if ((_id->ctrlID == CSSL_Package) || (_id->ctrlID == CSSL_ENDPackage)){
					pn_L1.Read(_ios,OUD(&cgZLib,_out));
					++ cgIDRE.orderID;
					if (_id->ctrlID == CSSL_ENDPackage){
						++ cgIDRE.blockID;
						cgIDRE.orderID = 0;
						cgZLib.Final(_ios,_out);
					}
				}
				if (offset == 0)
					pn_L1.GetDefArrayRE()->Out(pn_L1.GetLengthRE() + pn_L1.GetOffsetRE());
				ELogT1OUT(this << "CSSL_PNFT1.ReadNext()::L1.Rx :O=" << pn_L1.GetDefArrayRE()->Used() << "/" << pn_L1.GetDefArrayRE()->Unused());
				return G_TRUE;
			}
			else if (oID < cgIDRE.orderID){
				if (offset == 0){
					pn_L1.GetDefArrayRE()->Out(pn_L1.GetLengthRE() + pn_L1.GetOffsetRE());
					continue;
				}
			}
		}
		else if (bID < cgIDRE.blockID){
			if (offset == 0){
				pn_L1.GetDefArrayRE()->Out(pn_L1.GetLengthRE() + pn_L1.GetOffsetRE());
				continue;
			}
		}
		offset = pn_L1.GetLengthRE() + pn_L1.GetOffsetRE();
	};
	return G_FALSE;
}
//------------------------------------------------------------------------------------------//
bool32 CSSL_T1_RX::ReadCMD(uint32* ctrlID,uint32* retOrderID,const uint32& blockID){
	uint32 offset,delOffset,cID,oID;
	bool32 blFind;
	
	if (HasData() == 0)
		return G_FALSE;
	
	ELogT1IN(this << "CSSL_PNFT1.ReadCMD() ::L1.Rx :S=" << pn_L1.GetDefArrayRE()->Used() << "/" << pn_L1.GetDefArrayRE()->Unused());
	ELogT1IN(this << "CSSL_PNFT1.ReadCMD() ::Expect:BID=" << blockID);
	
	offset = 0;
	delOffset = 0;
	*retOrderID = 0;
	blFind = G_FALSE;
	while(pn_L1.Analysis(offset) > 0){
		ELogT1IN(this << "CSSL_PNFT1.ReadCMD() ::Rec   :BID=" << pn_L1.ReadBlockID()
				 << ",OID=" << pn_L1.ReadOrderID() << ",CID="
				 << _GetMesgCID(pn_L1.ReadCtrlID())
				 << ",Len=" << pn_L1.GetLengthRE());
		
		offset = pn_L1.GetLengthRE() + pn_L1.GetOffsetRE();
		cID = pn_L1.ReadCtrlID();
		oID = pn_L1.ReadOrderID();
		if (pn_L1.ReadBlockID() == blockID){
			blFind = G_TRUE;
			delOffset = offset;
			if (*retOrderID <= oID){
				*ctrlID = cID;
				*retOrderID = oID;
				if (cID == CSSL_ResendPackage)
					break;
			}
		}
	};
	pn_L1.GetDefArrayRE()->Out(delOffset);
	ELogT1IN(this << "CSSL_PNFT1.ReadCMD() ::L1.Rx :O=" << pn_L1.GetDefArrayRE()->Used() << "/" << pn_L1.GetDefArrayRE()->Unused());
	return(blFind);
}
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
CSSL_T1_TX::CSSL_T1_TX(void) : DSTF(){
	TNFP::SetSelfName("CSSL_PNFT1_TX");
	SetFatherName("");
};
//------------------------------------------------------------------------------------------//
CSSL_T1_TX::~CSSL_T1_TX(void){
	ALG_Zlib_Release(&cgCTX);
};
//------------------------------------------------------------------------------------------//
void CSSL_T1_TX::InitPN(G_ENDIAN endian){
	
	pn_L1.InitPN	(nullptr,nullptr,endian);
	pn_L1.InitSize	(CSSL_PNFBT1::MAX_PACKAGE_SIZE * 16);
	InitSize		(CSSL_PNFBT1::MAX_PACKAGE_SIZE * 4);
	
	DSTF_DIR_CTX_Clr(&cgCTX);
	ALG_Zlib_Init	(&cgCTX, ALG_ZLIB::CFG_Compress | ALG_ZLIB::CFG_DEFAULT_COMPRESSION);
	
	Reset();
}
//------------------------------------------------------------------------------------------//
void CSSL_T1_TX::SetSelfName(const STDSTR& strName){
	selfName = strName;
};
//------------------------------------------------------------------------------------------//
void CSSL_T1_TX::SetFatherName(const STDSTR& strName){
	fatherName = strName;
};
//------------------------------------------------------------------------------------------//
void CSSL_T1_TX::Empty(void){
	pn_L1.cgArray.Empty();
}
//------------------------------------------------------------------------------------------//
void CSSL_T1_TX::Reset(void){
	cgIDRE.blockID = 0;
	cgIDRE.orderID = 0;
	cgIDWR.blockID = 0;
	cgIDWR.orderID = 0;
	ClrSFlag(CSSL_blInit);
	Empty();
};
//------------------------------------------------------------------------------------------//
const uint32& CSSL_T1_TX::GetBlockID(void)	{return(cgIDRE.blockID);};
const uint32& CSSL_T1_TX::GetOrderID(void)	{return(cgIDRE.orderID);};
//------------------------------------------------------------------------------------------//
uint64 CSSL_T1_TX::CompressUpdate(IOSTATUS* _ios,const uint8* data,const uint64& length){
	uint32		flength,slength,offset;
	IOSTATUS	ios;
	IOSTATUS_Clr(&ios);
	
	cgCTX.next_in = (uint8*)data;
	cgCTX.avail_in = length;
	
	cgIDWR.ctrlID = CSSL_Package;
	do{
		if (cgArray.IsEmpty())
			cgArray.Reset();
		
		cgCTX.total_in = 0;
		cgCTX.total_out = 0;
		
		flength = 0;
		offset = 0;
		slength = cgArray.CalcInCapacity(flength,offset);
		
		cgCTX.status = ALG_Zlib_Update(&cgCTX
									   ,cgArray.GetPointer(offset),flength
									   ,cgCTX.next_in,cgCTX.avail_in);
		if ((slength > 0) && (cgCTX.avail_out == 0) && (cgCTX.avail_in > 0))
			cgCTX.status = ALG_Zlib_Update(&cgCTX
										   ,cgArray.GetPointer(0),slength
										   ,cgCTX.next_in,cgCTX.avail_in);
		
		cgArray.In((uint32)cgCTX.total_out);
		while(cgArray.Used() > MAX_PACKAGE_SIZE_ADJUST){
			pn_L1.Write(&ios, cgIDWR, IUD(&cgArray,MAX_PACKAGE_SIZE_ADJUST,0));
			
			ELogT1IN_ID("CompressUpdate",cgIDWR,MAX_PACKAGE_SIZE_ADJUST);
			
			if (ios.status > IOS_NOMEM){
				cgArray.Out(MAX_PACKAGE_SIZE_ADJUST);
				++ cgIDWR.orderID;
			}
			else{
				ELogT1IN(this << "CSSL_PNFT1.UpdateToL1()::No Enough Memory in L1");
				break;
			}
		};
	}while((cgCTX.avail_in > 0) && (cgCTX.status == IOS_OK));
	ios.avail_in = cgCTX.avail_in;
	ios.total_in = cgCTX.total_in;
	IOSTATUS_Add(_ios,ios);
	return(cgCTX.avail_in);
}
//------------------------------------------------------------------------------------------//
bool32 CSSL_T1_TX::CompressFinal(IOSTATUS* _ios){
	uint32		flength,slength,offset;
	IOSTATUS	ios;
	IOSTATUS_Clr(&ios);
	
	cgCTX.avail_in = 0;
	
	do{
		if (cgCTX.status == IOS_OK){
			if (cgArray.IsEmpty())
				cgArray.Reset();

			cgCTX.total_in = 0;
			cgCTX.total_out = 0;
			
			flength = 0;
			offset = 0;
			slength = cgArray.CalcInCapacity(flength,offset);
			
			cgCTX.status = ALG_Zlib_Final(&cgCTX,cgArray.GetPointer(offset),flength);
			if ((slength > 0) && (cgCTX.status == IOS_OK) && (cgCTX.avail_out == 0))
				cgCTX.status = ALG_Zlib_Final(&cgCTX,cgArray.GetPointer(0),slength);
			
			cgArray.In((uint32)cgCTX.total_out);
		}
		cgIDWR.ctrlID = CSSL_Package;
		while (cgArray.Used() > MAX_PACKAGE_SIZE_ADJUST){
			pn_L1.Write(&ios, cgIDWR, IUD(&cgArray,MAX_PACKAGE_SIZE_ADJUST,0));
			
			ELogT1IN_ID("CompressFinal",cgIDWR,MAX_PACKAGE_SIZE_ADJUST);
			
			if (ios.status > IOS_NOMEM){
				cgArray.Out(MAX_PACKAGE_SIZE_ADJUST);
				++ cgIDWR.orderID;
			}
			else{
				ELogT1IN(this << "CSSL_PNFT1.CompressFinal()::No Enough Memory in L1");
				break;
			}
		};
	}while((cgArray.Unused() > 0) && cgCTX.status == IOS_OK);
	
	if ((cgArray.Used() > 0) && (ios.status > IOS_NOMEM) && (cgCTX.status == IOS_FINISH)){
		cgIDWR.ctrlID = CSSL_ENDPackage;
		pn_L1.Write(&ios, cgIDWR, IUD(&cgArray,cgArray.Used(),0));
		
		ELogT1IN_ID("CompressFinal",cgIDWR,cgArray.Used());
		
		if (ios.status > IOS_NOMEM){
			cgArray.Reset();
			++ cgIDWR.blockID;
			cgIDWR.orderID = 0;
			ALG_Zlib_ReInit(&cgCTX);
		}
		else{
			ELogT1IN(this << "CSSL_PNFT1.CompressFinal()::No Enough Memory in L1");
		}
	}
	
	ios.avail_in = 0;
	ios.total_in = 0;
	IOSTATUS_Add(_ios,ios);
	return(ios.status);
}
//------------------------------------------------------------------------------------------//
bool32 CSSL_T1_TX::SendToT0(IOSTATUS* _ios,CSSL_T0* csslT0,const uint32& port,uint32 offset){
	IOSTATUS	ios;
	IOSTATUS_Clr(&ios);
	
	ios.status = -1;
	if (pn_L1.GetDefArrayRE()->Used() > 0){
		ELogT1IN(this << "CSSL_PNFT1.SendToT0()::Array:" << pn_L1.GetDefArrayRE()->Used() << "/" << pn_L1.GetDefArrayRE()->Unused());
		while(pn_L1.Analysis(offset) > 0){
			SetSFlag(CSSL_blInit);
			offset = pn_L1.GetLengthRE() + pn_L1.GetOffsetRE();
			
			ELogT1IN(this << "CSSL_PNFT1.SendToT0()::Port=" << port
					 << ",BID=" << pn_L1.ReadBlockID() << ",OID=" << pn_L1.ReadOrderID() << ",CID=" << _GetMesgCID(pn_L1.ReadCtrlID())
					 << ",Len=" << pn_L1.GetLengthRE());
			
			if (csslT0->Send(&ios,port,IUD(pn_L1.GetDefArrayRE(),pn_L1.GetLengthRE(),pn_L1.GetOffsetRE())) == IOS_NOMEM){
				ELogT1IN(this << "CSSL_PNFT1.SendToT0()::No Enough Memory in T0");
				break;
			}
			cgIDRE.blockID = pn_L1.ReadBlockID();
			cgIDRE.orderID = pn_L1.ReadOrderID();
		};
		ios.total_in = 0;
	}
	IOSTATUS_Add(_ios,ios);
	return(ios.status);
}
//------------------------------------------------------------------------------------------//
bool32 CSSL_T1_TX::UpdateToT0(IOSTATUS* _ios,CSSL_T0* csslT0,const uint32& port){
	uint32 offset = 0;
	
	if (CheckSFlag(CSSL_blInit)){
		while(pn_L1.Analysis(offset) > 0){
			offset = pn_L1.GetLengthRE() + pn_L1.GetOffsetRE();
			if ((pn_L1.ReadBlockID() == cgIDRE.blockID) && (pn_L1.ReadOrderID() == cgIDRE.orderID))
				return(SendToT0(_ios,csslT0,port,offset));
		};
	}
	return(SendToT0(_ios,csslT0,port,0));
}
//------------------------------------------------------------------------------------------//
bool32 CSSL_T1_TX::ReUpdateToT0(IOSTATUS* _ios,CSSL_T0* csslT0,const uint32& port,const uint32& blockID,const uint32& orderID){
	IOSTATUS	ios;
	IOSTATUS_Clr(&ios);
	uint32 offset = 0;
	bool32 err = IOS_OK;
	
	while(pn_L1.Analysis(offset) > 0){
		offset = pn_L1.GetLengthRE() + pn_L1.GetOffsetRE();
		if ((pn_L1.ReadBlockID() == blockID) && (pn_L1.ReadOrderID() == orderID)){
			err = csslT0->Send(&ios,port,IUD(pn_L1.GetDefArrayRE(),pn_L1.GetLengthRE(),pn_L1.GetOffsetRE()));
			if (err == IOS_NOMEM){
				ELogT1IN(this << "CSSL_PNFT1.ReUpdateToT0()::No Enough Memory in T0");
				break;
			}
		}
	}
	ios.total_in = 0;
	IOSTATUS_Add(_ios,ios);
	return(err);
}
//------------------------------------------------------------------------------------------//
void CSSL_T1_TX::CleanBlock(const uint32& blockID,const uint32& orderID){
	uint32 offset = 0;
	
	while(pn_L1.Analysis(offset) > 0){
		offset = pn_L1.GetLengthRE() + pn_L1.GetOffsetRE();
		if ((pn_L1.ReadBlockID() == blockID) && (pn_L1.ReadOrderID() == orderID)){
			pn_L1.GetDefArrayRE()->Out(offset);
			break;
		}
	};
}
//------------------------------------------------------------------------------------------//
bool32 CSSL_T1_TX::SendCMD(IOSTATUS* _ios,CSSL_T0* csslT0,const uint32& port,const T1ID& _id){
	IOSTATUS	ios;
	STDSTR		str = "";
	
	IOSTATUS_Clr(&ios);
	
	Empty();
	pn_L1.Write(nullptr,_id, str);
	SendToT0(_ios,csslT0,port,0);
	ios.total_in = 0;
	IOSTATUS_Add(_ios,ios);
	return G_TRUE;
}
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
CSSL_T1HD::CSSL_T1HD(void) : DS_TRANSFORM_FRAME(){
	cgPort = 0;
	TNFP::SetSelfName("CSSL_T1S");
	cgRx.SetSelfName("RxSCH");
	cgTx.SetSelfName("TxSCH");
	SetFatherName("");
};
//------------------------------------------------------------------------------------------//
void CSSL_T1HD::InitPN(CSSL_T0* csslT0,uint32 port,G_ENDIAN endian){
	
	cgT0 = csslT0;
	cgT0->Register(this);
	
	cgPort = port;
	cgTx.InitPN(endian);
	cgRx.InitPN(endian);
};
//------------------------------------------------------------------------------------------//
void CSSL_T1HD::SetSelfName(const STDSTR& strName){
	selfName = strName + Str_ToStr(GetPort());
	cgRx.SetFatherName(GetFullName(this));
	cgTx.SetFatherName(GetFullName(this));
}
//------------------------------------------------------------------------------------------//
void CSSL_T1HD::SetFatherName(const STDSTR& strName){
	fatherName = strName;
	cgRx.SetFatherName(GetFullName(this));
	cgTx.SetFatherName(GetFullName(this));
};
//------------------------------------------------------------------------------------------//
void CSSL_T1HD::Reset(void){
	cgTx.Reset();
	cgRx.Reset();
};
//------------------------------------------------------------------------------------------//
void CSSL_T1HD::Empty(void){
	cgTx.Empty();
	cgRx.Empty();
};
//------------------------------------------------------------------------------------------//
const uint32& CSSL_T1HD::GetPort(void){
	
	return(cgPort);
};
//------------------------------------------------------------------------------------------//
void CSSL_T1HD::ReceiveFromT0(IOSTATUS* _ios,const uint8* data,const uint64& length){
	
	cgRx.ReceiveFromT0(_ios,data,length);
};
//------------------------------------------------------------------------------------------//
void CSSL_T1HD::Lock(void){
	InUse_set();
};
//------------------------------------------------------------------------------------------//
void CSSL_T1HD::Unlock(void){
	InUse_clr();
};
//------------------------------------------------------------------------------------------//
CSSL_T1HD& CSSL_T1HD::DoTransform(IOSTATUS* _ios,const UVOut& _out,const uint8* data,const uint64& length){
	bool32		err;
	IOSTATUS	ios,ios2,ios3;
	IOSTATUS_Clr(&ios);
	IOSTATUS_Clr(&ios2);
	IOSTATUS_Clr(&ios3);
	
	ELogT1IN(this << "CSSL_T1S.DoTransform()::Input length=" << length);
	ios.avail_in = length;
	do{
		ELogT1IN(this << "CSSL_T1S.DoTransform()::Call cgTx.CompressUpdate()");
		cgTx.CompressUpdate(&ios,data,ios.avail_in);
		ELogT1IN(this << "CSSL_T1S.DoTransform()::Remain length=" << ios.avail_in);
		data += ios.total_in;
		
		ELogT1IN(this << "CSSL_T1S.DoTransform()::Call cgTx.UpdateToT0()");
		err = cgTx.UpdateToT0(&ios2,cgT0, cgPort - 1);
		if (err > IOS_NOMEM){
			ELogT1IN(this << "CSSL_T1S.DoTransform()::Expect ACK:BID=" << cgTx.GetBlockID() << ",OID=" << cgTx.GetOrderID());
		}
		else if (err == IOS_NOMEM){
			CheckACK(&ios3);
		}
	}while((ios.avail_in > 0) || err == IOS_NOMEM);
	ios.avail_out = ios2.avail_out;
	ios.total_out = ios2.total_out + ios3.total_out;
	IOSTATUS_Add(_ios, ios);
	return(*this);
}
//------------------------------------------------------------------------------------------//
CSSL_T1HD& CSSL_T1HD::DoFinal(IOSTATUS* _ios,const UVOut& _out){
	IOSTATUS	ios,ios2,ios3;
	bool32		err,err2,retACK;
	SYS_TIME_S	dlyTS;
	double		delayMS;
	delayMS = cgT0->GetPackageDlyMS();
	SYS_Delay_SetTS(&dlyTS,delayMS);
	
	IOSTATUS_Clr(&ios);
	IOSTATUS_Clr(&ios2);
	IOSTATUS_Clr(&ios3);
	do{
		ELogT1IN(this << "CSSL_T1S.DoFinal()    ::Call cgTx.CompressFinal()");
		err = cgTx.CompressFinal(&ios);
		
		ELogT1IN(this << "CSSL_T1S.DoFinal()    ::Call cgTx.UpdateToT0()");
		err2 = cgTx.UpdateToT0(&ios2,cgT0, cgPort - 1);
		
		ELogT1IN(this << "CSSL_T1S.DoFinal()    ::Expect ACK:BID=" << cgTx.GetBlockID() << ",OID=" << cgTx.GetOrderID());
		ELogT1IN(this << "CSSL_T1S.DoFinal()    ::delayMS=" << delayMS);
		retACK = CheckACK(&ios3);
	}while((err == IOS_NOMEM || err2 == IOS_NOMEM));
	
	if ((CheckSFlag(CSSL_blSendOK) == G_FALSE) && (retACK == G_FALSE))
		while((CheckACK(&ios3) == G_FALSE) && (SYS_Delay_CheckTS(&dlyTS) == 0));
	
	ios.avail_out = ios2.avail_out;
	ios.total_out = ios2.total_out + ios3.total_out;
	IOSTATUS_Add(_ios, ios);
	return(*this);
}
//------------------------------------------------------------------------------------------//
bool32 CSSL_T1HD::CheckACK(IOSTATUS* _ios){
	uint32 		cID,oID;
	SYS_TIME_S	dlyTS;
	double		delayMS;
	
	delayMS = cgT0->GetPackageDlyMS();
	SYS_Delay_SetTS(&dlyTS,delayMS);
	
	oID = 0;
	do{
		while(cgRx.ReadCMD(&cID,&oID,cgTx.GetBlockID()) > 0){
			switch (cID){
				case CSSL_ACK:
					SYS_Delay_SetTS(&dlyTS,delayMS);
					ELogT1IN(this << "CSSL_T1S.CheckACK()   ::Rec:BID=" << cgTx.GetBlockID() << ",OID=" << oID << ",CID=" << _GetMesgCID(cID));
					cgTx.CleanBlock(cgTx.GetBlockID(), oID);
					break;
				case CSSL_ENDACK:
					SYS_Delay_SetTS(&dlyTS,delayMS);
					ELogT1IN(this << "CSSL_T1S.CheckACK()   ::Rec:BID=" << cgTx.GetBlockID() << ",OID=" << oID << ",CID=" << _GetMesgCID(cID));
					cgTx.CleanBlock(cgTx.GetBlockID(), oID);
					SetSFlag(CSSL_blSendOK);
					return 1;
				case CSSL_ResendPackage:
					SYS_Delay_SetTS(&dlyTS,delayMS);
					ELogT1IN(this << "CSSL_T1S.CheckACK()   ::Rec:BID=" << cgTx.GetBlockID() << ",OID=" << oID << ",CID=" << _GetMesgCID(cID));
					cgTx.ReUpdateToT0(nullptr,cgT0, cgPort - 1, cgTx.GetBlockID(), oID);
					break;
				case CSSL_ResendAll:
				case CSSL_Package:
				case CSSL_ENDPackage:
				default:
					ELogT1IN(this << "CSSL_T1S.CheckACK()   ::Err:BID=" << cgTx.GetBlockID() << ",OID=" << oID << ",CID=" << _GetMesgCID(cID));
					break;
			};
			if (oID >= cgTx.GetOrderID()){
				SYS_Delay_SetTS(&dlyTS,0);
				break;
			}
		};
	}while(SYS_Delay_CheckTS(&dlyTS) == 0);
	return G_FALSE;
}
//------------------------------------------------------------------------------------------//
bool32 CSSL_T1HD::ReadPackage(IOSTATUS* _ios,const UVOut& _out){
	IOSTATUS	ios,ios2;
	T1ID		_id;
	uint32		requestTimesNum;
	int32		blRecFirstPackage;
	SYS_TIME_S	dlyTS,stopWatch;
	double		delayMS;
	
	if (cgRx.HasData() == 0)
		return G_FALSE;
	IOSTATUS_Clr(&ios);
	IOSTATUS_Clr(&ios2);
	SYS_StopWatch_Start(&stopWatch);
	
	blRecFirstPackage = 0;
	delayMS = cgT0->GetPackageDlyMS();
	SYS_Delay_SetTS(&dlyTS,delayMS);
	requestTimesNum = REQUESTTIMES;
	
	do{
		if (cgRx.HasData() == 0)
			SYS_DelayMS(1);
		
		if (cgRx.ReadNext(&ios,_out,&_id) > 0){
			blRecFirstPackage = 1;
			SYS_Delay_SetTS(&dlyTS,delayMS);
			requestTimesNum = REQUESTTIMES;
			switch (_id.ctrlID){
				case CSSL_Package:
					ELogT1OUT(this << "CSSL_T1S.ReadPackage()::Call cgTx.SendCMD()");
					_id.ctrlID = CSSL_ACK;
					cgTx.SendCMD(&ios2,cgT0,cgPort + 1,_id);
					break;
				case CSSL_ENDPackage:
					ELogT1OUT(this << "CSSL_T1S.ReadPackage()::Call cgTx.SendCMD()");
					_id.ctrlID = CSSL_ENDACK;
					cgTx.SendCMD(&ios2,cgT0,cgPort + 1,_id);
					cgT0->SetPackageDlyMS(SYS_StopWatch_Stop(&stopWatch) / (_id.orderID + 1));
					ELogT1OUT(this << "CSSL_T1S.ReadPackage()::Read successful");
					ELogT1OUT(this << "CSSL_T1S.ReadPackage()::delayMS = " << cgT0->GetPackageDlyMS());
					ios.avail_in = 0;
					ios.total_in = ios.total_out;
					ios.avail_out = 0;
					ios.total_out = ios2.total_out;
					IOSTATUS_Add(_ios, ios);
					return G_TRUE;
				case CSSL_ACK:
				case CSSL_ENDACK:
				case CSSL_ResendPackage:
				case CSSL_ResendAll:
				default:
					ELogT1OUT(this << "CSSL_T1S.ReadPackage()::Error:BID=" << _id.blockID << ",OID=" << _id.orderID << ",CID=" << _GetMesgCID(_id.ctrlID));
					break;
			};
		}
		if ((blRecFirstPackage != 0) && (SYS_Delay_CheckTS(&dlyTS) != 0)){
			if (requestTimesNum-- > 0){
				_id.ctrlID = CSSL_ResendPackage;
				++ _id.orderID;
				cgTx.SendCMD(&ios2,cgT0,cgPort + 1,_id);
				-- _id.orderID;
				SYS_Delay_SetTS(&dlyTS,delayMS * (REQUESTTIMES - requestTimesNum));
			}
		}
	}while(SYS_Delay_CheckTS(&dlyTS) == 0);
	ios.avail_in = 0;
	ios.total_in = ios.total_out;
	ios.avail_out = 0;
	ios.total_out = ios2.total_out;
	IOSTATUS_Add(_ios, ios);
	return G_FALSE;
}
//------------------------------------------------------------------------------------------//
bool32 CSSL_T1HD::SendPackage(IOSTATUS* _ios,const UVIn& _in){
	SYS_TIME_S	stopWatch;
	
	ClrSFlag(CSSL_blSendOK);
	cgRx.Empty();
	cgTx.Empty();
	
	SYS_StopWatch_Start(&stopWatch);
	
	Transform(_ios, _NONE(), _in);
	Final(_ios, _NONE());
	
	if (CheckSFlag(CSSL_blSendOK))
		cgT0->SetPackageDlyMS(SYS_StopWatch_Stop(&stopWatch) / (cgTx.GetOrderID() + 1));
	
	ELogT1IN(this << "CSSL_T1S.SendPackage()::Send " << (CheckSFlag(CSSL_blSendOK)?"successful":"fail") << "\r\n");
	ELogT1OUT(this << "CSSL_T1S.ReadPackage()::delayMS = " << cgT0->GetPackageDlyMS());
	return(CheckSFlag(CSSL_blSendOK));
}
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
CSSL_T1::CSSL_T1(void) : TNFP(){
	cgRx.SetSelfName("RxCH");
	cgTx.SetSelfName("TxCH");
	SetFatherName("");
};
//------------------------------------------------------------------------------------------//
void CSSL_T1::Init(CSSL_T0* csslT0,uint32 port){
	port <<= 1;
	
	cgRx.InitPN	(csslT0,port,		G_ENDIAN_LITTLE);
	cgTx.InitPN	(csslT0,port + 1,	G_ENDIAN_LITTLE);
};
//------------------------------------------------------------------------------------------//
void CSSL_T1::SetSelfName(const STDSTR& strName){
	selfName = strName;
	cgRx.SetFatherName(GetFullName(this));
	cgTx.SetFatherName(GetFullName(this));
}
//------------------------------------------------------------------------------------------//
void CSSL_T1::SetFatherName(const STDSTR& strName){
	fatherName = strName;
	cgRx.SetFatherName(GetFullName(this));
	cgTx.SetFatherName(GetFullName(this));
};
//------------------------------------------------------------------------------------------//
void CSSL_T1::Reset(void){
	cgTx.Reset();
	cgRx.Reset();
};
//------------------------------------------------------------------------------------------//
void CSSL_T1::Empty(void){
	cgTx.Empty();
	cgRx.Empty();
};
//------------------------------------------------------------------------------------------//
bool32 CSSL_T1::Send (IOSTATUS* _ios,const UVIn&  _in){
	bool32 err;
	cgTx.Lock();
	err = cgTx.SendPackage(_ios,_in);
	cgTx.Unlock();
	return(err);
};
//------------------------------------------------------------------------------------------//
bool32 CSSL_T1::Read (IOSTATUS* _ios,const UVOut& _out){
	bool32 err;
	cgRx.Lock();
	err = cgRx.ReadPackage(_ios,_out);
	cgRx.Unlock();
	return(err);
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
void CSSL_T1MSG::Init(CSSL_T0* csslT0,uint32 port){
	CSSL_T1::Init(csslT0,port);
	cgArrayTx.InitSize(MESG_SIZE);
	cgArrayRx.InitSize(MESG_SIZE);

	cgMsg.InitPN(&cgArrayTx,&cgArrayRx,1,0,G_ENDIAN_LITTLE);
};
//------------------------------------------------------------------------------------------//
bool32 CSSL_T1MSG::Send(IOSTATUS* _ios,uint32 mID,const UVIn& _in){
	bool32	err;
	IOSTATUS ios,ios2;
	IOSTATUS_Clr(&ios);
	IOSTATUS_Clr(&ios2);
	
	cgTx.Lock();
	cgMsg.CleanWR();
	cgMsg.Write(&ios2,mID,_in);
	err = cgTx.SendPackage(&ios,IUD(cgMsg.GetDefArrayWR()));
	cgTx.Unlock();
	ios.avail_in = 0;
	ios.total_in = ios2.total_in;
	IOSTATUS_Add(_ios,ios);
	return(err);
};
//------------------------------------------------------------------------------------------//
uint32&	CSSL_T1MSG::Read(IOSTATUS* _ios,uint32* mID,const UVOut& msg){
	*mID = 0;
	cgRx.Lock();
	cgMsg.CleanRE();
	if (cgRx.ReadPackage(_ios,cgMsg.GetDefArrayRE()) > 0){
		if (cgMsg.TryGetFrame() > 0){
			*mID = cgMsg.ReadID();
			cgMsg.Read(nullptr,msg);
		}
	}
	cgRx.Unlock();
	return(*mID);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif /* ADS_SSL_h */
