/*
 * Copyright (c) 2012-2014 
 * All rights reserved.
 *
 * File Name: Comm_PA.h
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2015.09.06
*/
#ifndef Comm_PAH
#define Comm_PAH
//------------------------------------------------------------------------------------------//
#include "Comm_Tree.h"
#include "Comm_Buffer.h"
//------------------------------------------------------------------------------------------//
class Field_Node : public RTREE_NODE{
	public:
		enum	{RFLAG_C = 0, RFLAG_S = RTREE_NODE::RFLAG_S + RTREE_NODE::RFLAG_C};
		enum	FNT_Type	{FNT_UNINIT = 0,FNT_MESSAGE,FNT_HEAD,FNT_ADDR,FNT_CTRL,FNT_LENGTH,FNT_CONTENT,FNT_CHECKSUM,FNT_TAIL,FNT_MASK};
	public:
				 Field_Node(void);
		virtual ~Field_Node(void){;};
	private:
		FNT_Type		cgType;
		G_Endian_VAILD	cgEndianType;	//Big or Little
		uint32			cgObyteNum;		//default byteNum
		//In FN_MESSAGE, FN_CONTENT, 0 means no limit.decide by FN_LENGTH or FN_TAIL
		//In FN_ADDR, FN_CTRL, FN_LENGTH, FN_CHECKSUM,the value is 1~4(max 32bit)
	protected:
		uint32			fnOffset;		//FN offset in exFrameFifo
		uint32			fnlength;		//FN data length in exFrameFifo
	public:
		Field_Node		*vaildAreaH;	//FN_LENGTH available area, Head
		Field_Node		*vaildAreaT;	//FN_LENGTH available area, Tail
		Field_Node		*linkNode;
			//in FN_TAIL, point to FN_CONTENT, FN_MESSAGE
			//in FN_CONTENT, FN_MESSAGE, point to length field
	private:
		FIFO_UINT8		*cgDefFifo;	//used as fifoin or fifoout
		std::string		fixValue;	//only available in FN_HEAD, FN_TAIL.
		uint32			maskBit;	//only available in FN_ADDR,FN_CTRL,FN_LENGTH
		uint32			moveBit;
	public:
		inline	static 	FIFO_UINT8*	GetcgDefFifo	(const Field_Node *mySelf){return(mySelf->cgDefFifo);};
		inline	static 	void		SetEndianType	(Field_Node *mySelf,G_Endian_VAILD tEV = G_LITTLE_ENDIAN){mySelf->cgEndianType = tEV;};
	private:
		int32			CheckFixValue	(const FIFO_UINT8 &fifobuf)const;
		uint32			CalcCLength		(const FIFO_UINT8 &fifobuf)const;
	public:
		void			Init			(FNT_Type fType,const FIFO_UINT8 *tfifo,uint32 fbyteNum = 0,G_Endian_VAILD tEV = G_LITTLE_ENDIAN);
		void			SetMaskBit		(uint32	mask);				//only available in FN_ADDR,FN_CTRL,FN_LENGTH
		void			SetFixValue		(const std::string &str);	//only available in FN_HEAD, FN_TAIL
		void			FillMaskField	(void);
	private:
		int32			AnalysisFrameR1	(const FIFO_UINT8 &fifoIn);
		int32			AnalysisFrameR0	(const FIFO_UINT8 &fifoIn,uint32 fifoInOffset = 0);
	public:
		uint32			AnalysisFrame	(const FIFO_UINT8 &fifoIn,uint32 fifoInOffset = 0);
		uint32			AnalysisFrameTryAgain(const FIFO_UINT8 &fifoIn);
		uint32			Out				(FIFO_UINT8 *fifobuf = nullptr){
			if (fifobuf == nullptr)
				fifobuf = cgDefFifo;
				return(fifobuf->Out(fnlength + fnOffset));
		};
		uint32			TryGetFrame		(void);
		uint32			TryGetFrame		(const FIFO_UINT8 &fifoIn);
		int32			TryGetFrame		(const uint8 *tInput,uint32 num);
		int32			TryGetFrame		(const std::string &strInput);
		uint32			TryGetFrameAgain_if_NoEnoughDataInFIFO(FIFO_UINT8 *fifoIn);
		uint32			TryGetFrameAgain_if_NoEnoughDataInFIFO(void);
		uint32			TryGetFrameAgain_if_NoEnoughDataInFIFO(const FIFO_UINT8 &fifoIn);
		uint32			TryGetFrameAgain_if_NoEnoughDataInFIFO(const uint8 *data,uint32 num);
	public:
		inline	const	uint32&			GetOffset(void)const	{return(fnOffset);};
		inline	const	uint32&			GetLength(void)const	{return(fnlength);};
		uint32			GetOriginalValue(const FIFO_UINT8 *fifobuf = nullptr)const;
		uint32			GetValueAMask	(const FIFO_UINT8 *fifobuf = nullptr)const;
		uint32			GetValueCalc	(const FIFO_UINT8 *fifobuf = nullptr)const;
	
		const std::string	&ReadAllContent	(std::string *retStr,const FIFO_UINT8 *fifobuf = nullptr)const;
		const FIFO_UINT8	&ReadAllContent	(FIFO_UINT8 *retfifo,const FIFO_UINT8 *fifobuf = nullptr)const;
		const PUB_SBUF		&ReadAllContent	(PUB_SBUF *retPSBUF,const FIFO_UINT8 *fifobuf = nullptr)const;
		const Field_Node	&ReadAllContent	(Field_Node *retFN,const FIFO_UINT8 *fifobuf = nullptr)const;
	public:
		void			FillZero		(void){cgDefFifo->FillZero();};
		void			SetFIFO			(void);
		void			SetFIFO_1Byte	(uint32 tInput);
		void			SetFIFO_2Byte	(uint32 tInput);
		void			SetFIFO_3Byte	(uint32 tInput);
		void			SetFIFO_4Byte	(uint32 tInput);
		void			SetFIFOByte		(uint64 tInput);
		void			SetFIFOFixValue	(void);
		void			SetFIFO			(const uint8 *tInput,uint32 num);
		inline	void	SetFIFO			(const std::string &strData){SetFIFO((uint8*)strData.c_str(),(uint32)strData.length());};
		void			SetFIFO			(const FIFO_UINT8 &fifoIn,uint32 num,uint32 offset = 0);
		void			SetFIFO			(const Field_Node &fnIn);
		inline	void	HoldOffset		(void){fnOffset = cgDefFifo->Used();};
		inline	void	UpdateLength	(void){fnlength = cgDefFifo->Used() - fnOffset;};
	public:
		void			UpdateFIFO_1Byte(uint32 tInput);
		void			UpdateFIFO_2Byte(uint32 tInput);
		void			UpdateFIFO_3Byte(uint32 tInput);
		void			UpdateFIFO_4Byte(uint32 tInput);
		void			UpdateFIFOByte	(uint64 tInput);
	public:
		virtual void	ResetfnLength	(const FIFO_UINT8 &fifobuf){fnlength = cgObyteNum;};
				//will be called after read cgObyteNum byte data, return the new fnlength
				//if result is changed, will re-read from fifoIn for this FN
				//FN is variable, and decide by cgObyteNum byte data
		virtual int32	ChecksumResult	(const FIFO_UINT8 &fifobuf)const{return 1;};
};
//------------------------------------------------------------------------------------------//
class FNode_LC :public Field_Node{
	public:
				 FNode_LC(void) :Field_Node(){AddNode(&fnlc_len);AddNode(&fnlc_content);};
		virtual ~FNode_LC(void){;};
	public:
		Field_Node	fnlc_len;
		Field_Node	fnlc_content;
	public:
		void Init(const FIFO_UINT8 *tfifo,uint32 fbyteNum,G_Endian_VAILD tEV = G_LITTLE_ENDIAN){
			Field_Node::Init	(FNT_MESSAGE,tfifo,0,tEV);
			fnlc_len.Init		(FNT_LENGTH	,tfifo,fbyteNum,tEV);
			fnlc_content.Init	(FNT_MESSAGE,tfifo,0,tEV);		fnlc_content.linkNode = &fnlc_len;
		}
		const FNode_LC &operator =(const std::string &strInput){
			FNode_LC::HoldOffset();
			fnlc_content.SetFIFO(strInput);
			FNode_LC::UpdateLength();
			return(*this);
		};
	public:
		inline	static 	void			SetEndianType	(FNode_LC *mySelf,G_Endian_VAILD tEV = G_LITTLE_ENDIAN){
			Field_Node::SetEndianType(mySelf,tEV);
			Field_Node::SetEndianType(&mySelf->fnlc_len,tEV);
			Field_Node::SetEndianType(&mySelf->fnlc_content,tEV);
		};
		inline			int32			AddSubNode		(Field_Node *tTreeNode)			{return(fnlc_content.AddNode(tTreeNode));};
		inline	const	FNode_LC&		SetContent		(const FIFO_UINT8 &fifoIn,uint32 num,uint32 offset = 0)	{
			FNode_LC::HoldOffset();
			fnlc_content.SetFIFO(fifoIn,num,offset);
			FNode_LC::UpdateLength();
			return(*this);
		};
		inline	const	FNode_LC&		SetContent		(const uint8* data,uint32 num)	{
			FNode_LC::HoldOffset();
			fnlc_content.SetFIFO(data,num);
			FNode_LC::UpdateLength();
			return(*this);
		};
		inline	const	FNode_LC&		SetContent		(const std::string &strInput)	{
			return(FNode_LC::SetContent((uint8*)strInput.c_str(),(uint32)strInput.length()));
		};
		inline	const	FNode_LC&		SetContent		(const Field_Node &fnInput)		{
			FNode_LC::HoldOffset();
			fnlc_content.SetFIFO(fnInput);
			FNode_LC::UpdateLength();
			return(*this);
		};
		inline	const	std::string&	ReadContent		(std::string *retStr,const FIFO_UINT8 *fifobuf = nullptr){
			return(fnlc_content.ReadAllContent(retStr,fifobuf));
		};
		inline	const	FIFO_UINT8&		ReadContent		(FIFO_UINT8 *retfifo,const FIFO_UINT8 *fifobuf = nullptr){
			return(fnlc_content.ReadAllContent(retfifo,fifobuf));
		};
		inline	const	PUB_SBUF&		ReadContent		(PUB_SBUF *retPSBUF,const FIFO_UINT8 *fifobuf = nullptr){
			return(fnlc_content.ReadAllContent(retPSBUF,fifobuf));
		};
		inline	const	uint32&			GetContentOffset(void){return(fnlc_content.GetOffset());};
		inline	const	uint32&			GetContentLength(void){return(fnlc_content.GetLength());};
		inline			void			HoldOffset		(void){
			Field_Node::HoldOffset();
			fnlc_len.SetFIFOByte(0);
			fnlc_content.HoldOffset();
		};
		inline			void			UpdateLength	(void){
			fnlc_content.UpdateLength();
			fnlc_len.UpdateFIFOByte(fnlc_content.GetLength());
			Field_Node::UpdateLength();
		};
};
//------------------------------------------------------------------------------------------//
class CCY_FNLC_MESG :public FNode_LC{
	public:
				 CCY_FNLC_MESG(void) :FNode_LC(){AddSubNode(&fn_MesgID);AddSubNode(&fnlc_Mesg);};
		virtual ~CCY_FNLC_MESG(void){;};
	public:
		Field_Node	fn_MesgID;
		FNode_LC	fnlc_Mesg;
	public:
		void Init(const FIFO_UINT8 *tfifo,G_Endian_VAILD tEV = G_LITTLE_ENDIAN){
			FNode_LC::Init	(tfifo,4,tEV);
			fn_MesgID.Init	(FNT_CTRL,tfifo,4,tEV);
			fnlc_Mesg.Init	(tfifo,4,tEV);
		}
	public:
		inline	const	std::string		&ReadContent(std::string *retStrMesg,uint32 *RetMesgID,const FIFO_UINT8 *fifobuf = nullptr){
			*RetMesgID = fn_MesgID.GetValueCalc();
			return(fnlc_Mesg.ReadContent(retStrMesg));
		};
		inline	const	FIFO_UINT8		&ReadContent(FIFO_UINT8 *retfifoMesg,uint32 *RetMesgID,const FIFO_UINT8 *fifobuf = nullptr){
			*RetMesgID = fn_MesgID.GetValueCalc();
			return(fnlc_Mesg.ReadContent(retfifoMesg));
		};
		inline	const	PUB_SBUF		&ReadContent(PUB_SBUF *retPSBUFMesg,uint32 *RetMesgID,const FIFO_UINT8 *fifobuf = nullptr){
			*RetMesgID = fn_MesgID.GetValueCalc();
			return(fnlc_Mesg.ReadContent(retPSBUFMesg));
		};
		inline	const	CCY_FNLC_MESG	&SetContent	(const FIFO_UINT8 &fifoIn,uint32 num,uint32 offset,uint8 tMesgID){
			FNode_LC::HoldOffset();
			fn_MesgID.SetFIFOByte(tMesgID);
			fnlc_Mesg.SetContent(fifoIn, num, offset);
			FNode_LC::UpdateLength();
			return(*this);
		};
		inline	const	CCY_FNLC_MESG	&SetContent	(const uint8 *data,uint32 num,uint32 tMesgID){
			FNode_LC::HoldOffset();
			fn_MesgID.SetFIFOByte(tMesgID);
			fnlc_Mesg.SetContent(data, num);
			FNode_LC::UpdateLength();
			return(*this);
		};
		inline	const	CCY_FNLC_MESG	&SetContent	(const std::string &strInput,uint32 tMesgID){
			return(CCY_FNLC_MESG::SetContent((uint8*)strInput.c_str(),(uint32)strInput.length(),tMesgID));
		};
		inline	const	CCY_FNLC_MESG	&SetContent	(const Field_Node &fnInput,uint32 tMesgID){
			FNode_LC::HoldOffset();
			fn_MesgID.SetFIFOByte(tMesgID);
			fnlc_Mesg.SetContent(fnInput);
			FNode_LC::UpdateLength();
			return(*this);
		};
};
//------------------------------------------------------------------------------------------//
template <typename TYPE_CLASS>
	class Field_NodeEx : public Field_Node{
		typedef int32(TYPE_CLASS::*TYPE_CVoidFunP)(void);
	public:
		enum{RFLAG_C = 0, RFLAG_S = Field_Node::RFLAG_S + Field_Node::RFLAG_C};
	public:
				 Field_NodeEx(void) : Field_Node() {node_TYPE_CLASS = nullptr;node_CVoidFunP = nullptr;};
		virtual ~Field_NodeEx(void){;};
	private:
		TYPE_CLASS		*node_TYPE_CLASS;
		TYPE_CVoidFunP	node_CVoidFunP;
		void		InitChecksumFun	(TYPE_CLASS *tTYPE_CLASS,TYPE_CVoidFunP tTPYE_CVoidFunP){node_TYPE_CLASS = tTYPE_CLASS;node_CVoidFunP = tTPYE_CVoidFunP;};
		virtual int32	ChecksumResult	(void){
			if (node_CVoidFunP != nullptr)
				return ((node_TYPE_CLASS->*node_CVoidFunP)());
			return 1;
		};
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif