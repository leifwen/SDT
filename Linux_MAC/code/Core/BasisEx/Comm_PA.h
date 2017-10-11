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

//------------------------------------------------------------------------------------------//
#include "Comm_FIFO.h"
#include "Comm_Tree.h"
//------------------------------------------------------------------------------------------//
#if defined Comm_FIFOH && defined Comm_TreeH
#ifndef Comm_PAH
#define Comm_PAH
#ifdef Comm_PAH
//------------------------------------------------------------------------------------------//
class PROTOCOL_NODE : public TREE_NODE{
	public:
		enum	FNT_Type	{FNT_UNINIT = 0,FNT_MESSAGE,FNT_HEAD,FNT_ADDR,FNT_CTRL,FNT_LENGTH,FNT_CONTENT,FNT_CHECKSUM,FNT_TAIL,FNT_MASK};
	public:
				 PROTOCOL_NODE(void);
		virtual ~PROTOCOL_NODE(void){;};
	private:
		FIFO8		*cgDefFifo;	//used as fifoin or fifoout
	
		FNT_Type		cgType;
		G_ENDIAN		cgEndianType;	//Big or Little
		uint32			cgObyteNum;		//default byteNum
		//In FN_MESSAGE, FN_CONTENT, 0 means no limit.decide by FN_LENGTH or FN_TAIL
		//In FN_ADDR, FN_CTRL, FN_LENGTH, FN_CHECKSUM,the value is 1~4(max 32bit)
		STDSTR			fixValue;	//only available in FN_HEAD, FN_TAIL.
		uint32			maskBit;	//only available in FN_ADDR,FN_CTRL,FN_LENGTH
		uint32			moveBit;
	public:
		PROTOCOL_NODE	*vaildAreaH;	//FN_LENGTH available area, Head
		PROTOCOL_NODE	*vaildAreaT;	//FN_LENGTH available area, Tail
		PROTOCOL_NODE	*linkNode;
		//in FN_TAIL, point to FN_CONTENT, FN_MESSAGE
		//in FN_CONTENT, FN_MESSAGE, point to length field
	protected:
		uint32			fnOffset;		//FN offset in exFrameFifo
		uint32			fnlength;		//FN data length in exFrameFifo
	private:
		int32			AnalysisFrameR1	(const FIFO8 &fifo);
		int32			AnalysisFrameR0	(const FIFO8 &fifo,uint32 fifoOffset = 0);
		int32			CheckFixValue	(const FIFO8 &fifo)const;
		uint32			CalcCLength		(const FIFO8 &fifo)const;
	public:
		void			Init			(FNT_Type fType,const FIFO8 *defFifo,uint32 fObyteNum = 0,G_ENDIAN fEndianType = G_ENDIAN_LITTLE);
		inline	void	SetDefFifo		(const FIFO8 *defFifo){cgDefFifo = (FIFO8*)defFifo;};
		inline	void	SetEndianType	(G_ENDIAN fEndianType = G_ENDIAN_LITTLE){cgEndianType = fEndianType;};
		void			SetMaskBit		(uint32	mask);			//only available in FN_ADDR,FN_CTRL,FN_LENGTH
		void			SetFixValue		(const STDSTR &str);	//only available in FN_HEAD, FN_TAIL
		void			FillMaskField	(void);
	public:
		int32			AnalysisFrame	(const FIFO8 *fifo = nullptr,uint32 fifoOffset = 0);
		int32			TryGetFrame		(FIFO8 *fifo = nullptr);
		uint32			Out				(FIFO8 *fifo = nullptr);
		int32			TryGetFrame		(const STDSTR &strIn);
	public:
		inline			FIFO8*		GetDefFifo(void)const	{return(cgDefFifo);};
		inline	const	uint32&		GetOffset(void)const	{return(fnOffset);};
		inline	const	uint32&		GetLength(void)const	{return(fnlength);};
	
		uint32			GetOriginalValue(const FIFO8 *fifo = nullptr)const;
		uint32			GetValueAMask	(const FIFO8 *fifo = nullptr)const;
		uint32			GetValueCalc	(const FIFO8 *fifo = nullptr)const;
	
		const STDSTR		&ReadAllContent	(STDSTR *retStr,const FIFO8 *fifo = nullptr)		const;
		const FIFO8			&ReadAllContent	(FIFO8 *retFifo,const FIFO8 *fifo = nullptr)		const;
		const PROTOCOL_NODE	&ReadAllContent	(PROTOCOL_NODE *retPN,const FIFO8 *fifo = nullptr)	const;
	
		const STDSTR		&ReadAllContentInHEXs(STDSTR *retStr,const FIFO8 *fifo = nullptr)	const;
	public:
		void			FillZero		(void){cgDefFifo->FillZero();};
		void			Clean			(void){cgDefFifo->Empty();};
		void			SetFIFO			(void);
		void			SetFIFO_1Byte	(uint32 tInput);
		void			SetFIFO_2Byte	(uint32 tInput);
		void			SetFIFO_3Byte	(uint32 tInput);
		void			SetFIFO_4Byte	(uint32 tInput);
		void			SetFIFOByte		(uint64 tInput);
		void			SetFIFOFixValue	(void);
		void			SetFIFO			(std::stringstream &streamIn);
		void			SetFIFO			(const PROTOCOL_NODE &fnIn);
		void			SetFIFO			(const FIFO8 &fifoIn,uint32 num,uint32 offset = 0);
		void			SetFIFO			(const uint8 *data,uint32 num);
		inline	void	SetFIFO			(const STDSTR &strIn){SetFIFO((uint8*)strIn.c_str(),(uint32)strIn.length());};
	public:
		void			UpdateFIFO_1Byte(uint32 tInput);
		void			UpdateFIFO_2Byte(uint32 tInput);
		void			UpdateFIFO_3Byte(uint32 tInput);
		void			UpdateFIFO_4Byte(uint32 tInput);
		void			UpdateFIFOByte	(uint64 tInput);
	public:
		virtual void	ResetfnLength	(const FIFO8 &fifo){fnlength = cgObyteNum;};
				//will be called after read cgObyteNum byte data, return the new fnlength
				//if result is changed, will re-read from fifoIn for this FN
				//FN is variable, and decide by cgObyteNum byte data
		virtual int32	ChecksumResult	(const FIFO8 &fifo)const{return 1;};
	public:
		inline	virtual	TREE_NODE*	AddNode	(TNF *tTreeNode)	{((PROTOCOL_NODE*)tTreeNode)->SetDefFifo(GetDefFifo());return(TREE_NODE::AddNode(tTreeNode));};
	public:
		inline	virtual	void	_Start	(void){cgDefFifo->Prepare_Set();fnOffset = cgDefFifo->GetPreInNum();};
		inline	virtual	void	_Endl	(void){fnlength = cgDefFifo->GetPreInNum() - fnOffset;cgDefFifo->Prepare_Clr();};
		inline PROTOCOL_NODE& operator << (PROTOCOL_NODE&(*fun)(PROTOCOL_NODE&)){return((*fun)(*this));};
	
		inline PROTOCOL_NODE& operator << (std::stringstream &streamIn)		{*GetDefFifo() << streamIn;				return(*this);};
		inline PROTOCOL_NODE& operator << (const PROTOCOL_NODE &pnIn)		{pnIn.ReadAllContent(GetDefFifo());		return(*this);};
		inline PROTOCOL_NODE& operator << (const _Data<const FIFO8*> &data)	{*GetDefFifo() << data;					return(*this);};
		inline PROTOCOL_NODE& operator << (const _Data<const uint8*> &data)	{*GetDefFifo() << data;					return(*this);};
		inline PROTOCOL_NODE& operator << (const _Data<const char*> &data)	{*GetDefFifo() << data;					return(*this);};
		inline PROTOCOL_NODE& operator << (const STDSTR &strIn)				{*GetDefFifo() << strIn;				return(*this);};
		inline PROTOCOL_NODE& operator << (const char *data)				{*GetDefFifo() << data;					return(*this);};
};
//------------------------------------------------------------------------------------------//
class PNODE_LC :public PROTOCOL_NODE{
	public:
				 PNODE_LC(void) :PROTOCOL_NODE(){Add(pn_len) < pn_content;};
		virtual ~PNODE_LC(void){;};
	public:
		PROTOCOL_NODE	pn_len;
		PROTOCOL_NODE	pn_content;
	public:
		void Init(const FIFO8 *tfifo,uint32 fObyteNum = 2,G_ENDIAN tEV = G_ENDIAN_LITTLE){
			PROTOCOL_NODE::Init	(FNT_MESSAGE,tfifo,0,tEV);
			pn_len.Init		(FNT_LENGTH	,tfifo,(fObyteNum < 1 ? 1 : (fObyteNum > 4 ? 4 : fObyteNum)),tEV);
			pn_content.Init	(FNT_MESSAGE,tfifo,0,tEV);		pn_content.linkNode = &pn_len;
		}
		inline			void			SetEndianType	(G_ENDIAN tEV = G_ENDIAN_LITTLE){
			PROTOCOL_NODE::SetEndianType(tEV);
			pn_len.SetEndianType(tEV);
			pn_content.SetEndianType(tEV);
		};
	public:
		const PNODE_LC &operator =(const STDSTR &strIn){
			PNODE_LC::_Start();
			pn_content.SetFIFO(strIn);
			PNODE_LC::_Endl();
			return(*this);
		};
		const PNODE_LC &operator =(const PROTOCOL_NODE &pnNode){
			PNODE_LC::_Start();
			pn_content.SetFIFO(pnNode);
			PNODE_LC::_Endl();
			return(*this);
		};
	public:
		inline	const	uint32&			GetContentOffset(void)const{return(pn_content.GetOffset());};
		inline	const	uint32&			GetContentLength(void)const{return(pn_content.GetLength());};
	public:
		inline	const	PNODE_LC&		SetContent		(std::stringstream &streamIn)	{
			PNODE_LC::_Start();
			pn_content.SetFIFO(streamIn);
			PNODE_LC::_Endl();
			return(*this);
		};
		inline	const	PNODE_LC&		SetContent		(const PROTOCOL_NODE &pnIn)		{
			PNODE_LC::_Start();
			pn_content.SetFIFO(pnIn);
			PNODE_LC::_Endl();
			return(*this);
		};
		inline	const	PNODE_LC&		SetContent		(const FIFO8 &fifoIn,uint32 num,uint32 offset = 0)	{
			PNODE_LC::_Start();
			pn_content.SetFIFO(fifoIn,num,offset);
			PNODE_LC::_Endl();
			return(*this);
		};
		inline	const	PNODE_LC&		SetContent		(const uint8* data,uint32 num)	{
			PNODE_LC::_Start();
			pn_content.SetFIFO(data,num);
			PNODE_LC::_Endl();
			return(*this);
		};
		inline	const	PNODE_LC&		SetContent		(const uint8 data)	{
			PNODE_LC::_Start();
			pn_content.SetFIFO(&data,1);
			PNODE_LC::_Endl();
			return(*this);
		};
		inline	const	PNODE_LC&		SetContent		(const STDSTR &strIn)	{
			return(PNODE_LC::SetContent((uint8*)strIn.c_str(),(uint32)strIn.length()));
		};
		inline	const	STDSTR&	ReadContent		(STDSTR *retStr,const FIFO8 *fifo = nullptr)const{
			return(pn_content.ReadAllContent(retStr,fifo));
		};
		inline	const	FIFO8&		ReadContent		(FIFO8 *retFifo,const FIFO8 *fifo = nullptr)const{
			return(pn_content.ReadAllContent(retFifo,fifo));
		};
	public:
		inline	virtual void	_Start	(void){
			PROTOCOL_NODE::_Start();
			pn_len.SetFIFOByte(-1);
			pn_content._Start();
		};
		inline	virtual	void	_Endl	(void){
			pn_content._Endl();
			pn_len.UpdateFIFOByte(pn_content.GetLength());
			PROTOCOL_NODE::_Endl();
		};
	public:
		inline PNODE_LC& operator << (PNODE_LC&(*fun)(PNODE_LC&))		{return((*fun)(*this));};
		inline PNODE_LC& operator << (std::stringstream &streamIn)		{pn_content << streamIn	;return(*this);};
		inline PNODE_LC& operator << (const PROTOCOL_NODE &pnIn)		{pn_content << pnIn		;return(*this);};
		inline PNODE_LC& operator << (const _Data<const FIFO8*> &data)	{pn_content << data		;return(*this);};
		inline PNODE_LC& operator << (const _Data<const uint8*> &data)	{pn_content << data		;return(*this);};
		inline PNODE_LC& operator << (const _Data<const char*> &data)	{pn_content << data		;return(*this);};
		inline PNODE_LC& operator << (const STDSTR &strIn)				{pn_content << strIn	;return(*this);};
		inline PNODE_LC& operator << (const char *data)					{pn_content << data		;return(*this);};
};
//------------------------------------------------------------------------------------------//
CreateOperatorSet(mID)
class PNODE_MESGx :public PROTOCOL_NODE{
	public:
				 PNODE_MESGx(void) : PROTOCOL_NODE(){Add(pn_Head) < pn_MesgID < pnlc_Mesg < pn_Trail;};
		virtual ~PNODE_MESGx(void){;};
	private:
		PROTOCOL_NODE	pn_Head;
		PROTOCOL_NODE	pn_MesgID;
		PNODE_LC		pnlc_Mesg;
		PROTOCOL_NODE	pn_Trail;
	public:
		void Init(const FIFO8 *tfifo,uint32 fObyeNumOfID = 1,uint32 fObyeNumOfMesg = 2,G_ENDIAN tEV = G_ENDIAN_LITTLE){
			PROTOCOL_NODE::Init(FNT_MESSAGE,tfifo,0,tEV);
			pn_Head.Init	(FNT_HEAD, tfifo);pn_Head.SetFixValue("\xF9");
			pn_Trail.Init	(FNT_TAIL, tfifo);pn_Trail.SetFixValue("\xF9");
			
			pn_MesgID.Init	(FNT_CTRL,tfifo,(fObyeNumOfID < 1 ? 1 : (fObyeNumOfID > 4 ? 4 : fObyeNumOfID)),tEV);
			pnlc_Mesg.Init	(tfifo,(fObyeNumOfMesg < 1 ? 1 : (fObyeNumOfMesg > 4 ? 4 : fObyeNumOfMesg)),tEV);
		}
	public:
		inline	const	uint32	&GetMesgOffset(void)const{return(pnlc_Mesg.GetContentOffset());};
		inline	const	uint32	&GetMesgLength(void)const{return(pnlc_Mesg.GetContentLength());};
	public:
		inline			uint32	ReadMesgID(const FIFO8 *fifo = nullptr)const{return(pn_MesgID.GetValueCalc(fifo));};
		inline	const	STDSTR	&ReadContent(STDSTR *retStrMesg,uint32 *retMesgID = nullptr,const FIFO8 *fifo = nullptr)const{
			if (retMesgID != nullptr)
				*retMesgID = pn_MesgID.GetValueCalc(fifo);
			return(pnlc_Mesg.ReadContent(retStrMesg,fifo));
		};
		inline	const	FIFO8	&ReadContent(FIFO8 *retfifoMesg,uint32 *retMesgID = nullptr,const FIFO8 *fifo = nullptr)const{
			if (retMesgID != nullptr)
				*retMesgID = pn_MesgID.GetValueCalc(fifo);
			return(pnlc_Mesg.ReadContent(retfifoMesg,fifo));
		};
		inline	const	PNODE_MESGx	&SetContent	(uint32 tMesgID,std::stringstream &streamIn){
			PROTOCOL_NODE::_Start();
			pn_Head.SetFIFOFixValue();
			pn_MesgID.SetFIFOByte(tMesgID);
			pnlc_Mesg.SetContent(streamIn);
			pn_Trail.SetFIFOFixValue();
			PROTOCOL_NODE::_Endl();
			return(*this);
		};
		inline	const	PNODE_MESGx	&SetContent	(uint32 tMesgID,const PROTOCOL_NODE &pnIn){
			PROTOCOL_NODE::_Start();
			pn_Head.SetFIFOFixValue();
			pn_MesgID.SetFIFOByte(tMesgID);
			pnlc_Mesg.SetContent(pnIn);
			pn_Trail.SetFIFOFixValue();
			PROTOCOL_NODE::_Endl();
			return(*this);
		};
		inline	const	PNODE_MESGx	&SetContent	(uint32 tMesgID,const FIFO8 &fifoIn,uint32 num,uint32 offset){
			PROTOCOL_NODE::_Start();
			pn_Head.SetFIFOFixValue();
			pn_MesgID.SetFIFOByte(tMesgID);
			pnlc_Mesg.SetContent(fifoIn, num, offset);
			pn_Trail.SetFIFOFixValue();
			PROTOCOL_NODE::_Endl();
			return(*this);
		};
		inline	const	PNODE_MESGx	&SetContent	(uint32 tMesgID,const uint8 *data,uint32 num){
			PROTOCOL_NODE::_Start();
			pn_Head.SetFIFOFixValue();
			pn_MesgID.SetFIFOByte(tMesgID);
			pnlc_Mesg.SetContent(data, num);
			pn_Trail.SetFIFOFixValue();
			PROTOCOL_NODE::_Endl();
			return(*this);
		};
		inline	const	PNODE_MESGx	&SetContent	(uint32 tMesgID,const uint8 data){
			PROTOCOL_NODE::_Start();
			pn_Head.SetFIFOFixValue();
			pn_MesgID.SetFIFOByte(tMesgID);
			pnlc_Mesg.SetContent(&data, 1);
			pn_Trail.SetFIFOFixValue();
			PROTOCOL_NODE::_Endl();
			return(*this);
		};
		inline	const	PNODE_MESGx	&SetContent	(uint32 tMesgID,const STDSTR &strIn){
			return(PNODE_MESGx::SetContent(tMesgID,(uint8*)strIn.c_str(),(uint32)strIn.length()));
		};
	public:
		inline	virtual	void	_Start	(void){
			PROTOCOL_NODE::_Start();
			pn_Head.SetFIFOFixValue();
			pn_MesgID.SetFIFOByte(0);
			pnlc_Mesg._Start();
		};
		inline	virtual	void	_Endl	(void){
			pnlc_Mesg._Endl();
			pn_Trail.SetFIFOFixValue();
			PROTOCOL_NODE::_Endl();
		};
	public:
				inline PNODE_MESGx& operator << (PNODE_MESGx&(*fun)(PNODE_MESGx&))					{return((*fun)(*this));};
		friend	inline PNODE_MESGx& operator << (PNODE_MESGx& _pn,std::stringstream &streamIn)		{_pn.pnlc_Mesg << streamIn;	return(_pn);};
		friend	inline PNODE_MESGx& operator << (PNODE_MESGx& _pn,const PROTOCOL_NODE &pnIn)		{_pn.pnlc_Mesg << pnIn;		return(_pn);};
		friend	inline PNODE_MESGx& operator << (PNODE_MESGx& _pn,const _Data<const FIFO8*> &data)	{_pn.pnlc_Mesg << data;		return(_pn);};
		friend	inline PNODE_MESGx& operator << (PNODE_MESGx& _pn,const _Data<const uint8*> &data)	{_pn.pnlc_Mesg << data;		return(_pn);};
		friend	inline PNODE_MESGx& operator << (PNODE_MESGx& _pn,const _Data<const char*> &data)	{_pn.pnlc_Mesg << data;		return(_pn);};
		friend	inline PNODE_MESGx& operator << (PNODE_MESGx& _pn,const STDSTR &strIn)				{_pn.pnlc_Mesg << strIn;	return(_pn);};
		friend	inline PNODE_MESGx& operator << (PNODE_MESGx& _pn,const char *data)					{_pn.pnlc_Mesg << data;		return(_pn);};
	
		friend	inline PNODE_MESGx& operator << (PNODE_MESGx& _pn,const _mID &data)					{_pn.pn_MesgID.UpdateFIFOByte(data.value);	return(_pn);};
		friend	inline PNODE_MESGx& operator << (PNODE_MESGx& _pn,uint32 data)						{_pn.pn_MesgID.UpdateFIFOByte(data);		return(_pn);};
};
//------------------------------------------------------------------------------------------//
class PNODE_MESG8 :public PNODE_MESGx{
	public:
				 PNODE_MESG8(void) : PNODE_MESGx(){;};
		virtual ~PNODE_MESG8(void){;};
	public:
		void Init(const FIFO8 *tfifo,G_ENDIAN tEV = G_ENDIAN_LITTLE){PNODE_MESGx::Init(tfifo,1,2,tEV);};
	public:
		inline PNODE_MESG8& operator << (PNODE_MESG8&(*fun)(PNODE_MESG8&)){return((*fun)(*this));};
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif
#endif
#endif
