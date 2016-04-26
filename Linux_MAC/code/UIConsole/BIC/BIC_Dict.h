/*
 * Copyright (c) 2012-2014 
 * All rights reserved.
 *
 * File Name: BIC_DICT.h
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2014.01.25
*/
#ifndef BIC_DICTH
#define BIC_DICTH
//------------------------------------------------------------------------------------------//
#include "BIC_B.h"
//------------------------------------------------------------------------------------------//
#define HTML_SingleTagList "link/img/hr/br/!--/!DOCTYPE"
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
class HTML_NODE : public RTREE_NODE{
	public:
		enum{RFLAG_C = 0, RFLAG_S = RTREE_NODE::RFLAG_S + RTREE_NODE::RFLAG_C};
		enum{
			HTML_None = 0,
			HTML_Tags,
			HTML_Attribute,
			HTML_Content,
		};
	public:
				 HTML_NODE(void) : RTREE_NODE()	{Init();};
				 HTML_NODE(const HTML_NODE& x)	{Init();CreateTrash(&sgSpareOwner);};
		virtual ~HTML_NODE(void){;};
	private:
		virtual	RTREE_NODE*	CreateNode		(void){return(new HTML_NODE);};
	public:
		void	Init(void){cgTagsName = "";cgTagsValue = "";cgTagsType = HTML_None;cgIsTagSingle = 0;};
	public:
		static	HTML_NODE	sgSpareOwner;
	public:
		static	FIFO_UINT8	sgLogFifo;
		std::string	cgTagsName;
		std::string	cgTagsValue;
		uint32		cgTagsType;
		uint32		cgIsTagSingle;
	public:
				int32	ResolveContent		(FIFO_UINT8 *fifoIn);
				int32	ResolveAttribute	(FIFO_UINT8 *fifoIn);
				int32	ResolveTags			(FIFO_UINT8 *fifoIn);
				int32	IsTagSingle			(void)const;
		const	std::string&	ComposeTag	(std::string *strRet)const;
		const	std::string&	Compose		(std::string *strRet,std::string strDeep,G_SPACE_VAILD blSpace)const;
				uint64	CheckTags			(const std::string &tagName,const std::string &attrName,const std::string &attrValue);
				uint64	SetTags				(const std::string &tagName,const std::string &attrName,const std::string &attrValue
											 ,const std::string &newTagName,const std::string &newAttrName,const std::string &newAttrValue);
				uint64	DelAttribute			(const std::string &tagName,const std::string &attrName,const std::string &attrValue);
				uint64	DelTagsExcludeConetent	(const std::string &tagName,const std::string &attrName,const std::string &attrValue);
				uint64	DelTagsIncludeConetent	(const std::string &tagName,const std::string &attrName,const std::string &attrValue);
				uint64	DelTagsBlankContent		(const std::string &tagName);
				void	DelTagExcludeConetent	(void);
};
//------------------------------------------------------------------------------------------//
enum	G_DICT_RM	{G_DICT_RM_COMPRESS_NO = 0	,G_DICT_RM_COMPRESS_YES};
//------------------------------------------------------------------------------------------//
class DICTWORD_NODE : public RTREE_NODE{
	public:
		enum{RFLAG_C = 0, RFLAG_S = RTREE_NODE::RFLAG_S + RTREE_NODE::RFLAG_C};
	public:
				 DICTWORD_NODE(void) : RTREE_NODE(){Init();};
		virtual ~DICTWORD_NODE(void){;};
	public:
		void	Init(void);
	public:
		static	FIFO_UINT8	sgLogFifo;
		std::string	cgOWord;
		std::string	cgOWordLowerCase;
		std::string	cgOExplain;
		uint32		cgOrderNumber;
		uint64		cgELength;
		HTML_NODE	cgHtmlContent;
	public:
		int32	ReadCompressDict	(const std::string &fileName);
		int32	ReadDict			(const std::string &fileName);
		int32	SaveCompressDict	(const std::string &fileName);
		int32	SaveDict			(const std::string &fileName);
		uint64	CheckTags			(const std::string &tagName,const std::string &attrName,const std::string &attrValue);
		uint64	SetTags				(const std::string &tagName,const std::string &attrName,const std::string &attrValue
									 ,const std::string &newTagName,const std::string &newAttrName,const std::string &newAttrValue);
		uint64	DelAttribute			(const std::string &tagName,const std::string &attrName,const std::string &attrValue);
		uint64	DelTagsExcludeConetent	(const std::string &tagName,const std::string &attrName,const std::string &attrValue);
		uint64	DelTagsIncludeConetent	(const std::string &tagName,const std::string &attrName,const std::string &attrValue);
		uint64	DelTagsBlankContent		(const std::string &tagName);
};
//------------------------------------------------------------------------------------------//
class DICT_THREAD : public BASIC_CFLAG{
		enum{RFLAG_C = 1, RFLAG_S = BASIC_CFLAG::RFLAG_S + BASIC_CFLAG::RFLAG_C};
	public:
				 DICT_THREAD(void) : BASIC_CFLAG(){ClrReading();cgDict = nullptr;rThread.ThreadInit(this, &DICT_THREAD::ReadDict);};
		virtual ~DICT_THREAD(void){;};
	public:
		void	Init(DICTWORD_NODE *dict,const std::string &filename,G_DICT_RM blCompose){
						cgDict = dict;cgFilename = filename;cgblCompress = blCompose;};
	private:
		SYS_ThreadEx<DICT_THREAD>	rThread;
		DICTWORD_NODE	*cgDict;
		G_DICT_RM		cgblCompress;
		std::string		cgFilename;
		int32			cgReadNum;
		int32	ReadDict(void){
			cgReadNum = 0;
			if (cgDict != nullptr){
				SetReading();
				if (cgblCompress == G_DICT_RM_COMPRESS_YES){
					cgReadNum = cgDict->ReadCompressDict(cgFilename);
				}
				else{
					cgReadNum = cgDict->ReadDict(cgFilename);
				}
				
				ClrReading();
			}
			cgDict = nullptr;
			return(cgReadNum);
		};
		inline	void	SetReading		(void)		{SetSFlag(RFLAG_CREATE(0));};
		inline	void	ClrReading		(void)		{ClrSFlag(RFLAG_CREATE(0));};
	public:
		inline	int32	CheckReading	(void)const	{return(CheckSFlag(RFLAG_CREATE(0)));};
		inline	const int32&	ReadNum	(void){return(cgReadNum);};
		inline	void	Read			(void){rThread.ThreadRun();};
};
//------------------------------------------------------------------------------------------//
class COCAWORD_NODE : public RTREE_NODE{
	public:
		enum{RFLAG_C = 0, RFLAG_S = RTREE_NODE::RFLAG_S + RTREE_NODE::RFLAG_C};
	public:
				 COCAWORD_NODE(void) : RTREE_NODE(){
					 cgOWord = "";
					 cgO9W = "";
					 cgO8W = "";
					 cgOrder = "";
					 cgAttribute = "";
					 cgCStar ="";
					 cgDeck = "";
					 cgO9E = "";
					 cgO8E = "";
				 };
		virtual ~COCAWORD_NODE(void){;};
	public:
		std::string	cgOWord;
		std::string	cgO9W;
		std::string	cgO8W;
		std::string	cgOrder;
		std::string	cgAttribute;
		std::string	cgCStar;
		std::string	cgDeck;
		std::string	cgO9E;
		std::string	cgO8E;
		uint32		cgLineNumber;
	public:
		int32	ReadFromFile(const std::string &fileName);
		int32	UpdateDict	(DICTWORD_NODE *dict9,DICTWORD_NODE *dict8);
		int32	WriteToFile	(const std::string &fileName);
};
//------------------------------------------------------------------------------------------//


//------------------------------------------------------------------------------------------//





//------------------------------------------------------------------------------------------//
class BIC_DICT_DO : public BIC_Node{
	public:
		enum{RFLAG_C = 0, RFLAG_S = BIC_Node::RFLAG_S + BIC_Node::RFLAG_C};
	public:
				 BIC_DICT_DO(void) : BIC_Node() {cgCommand = "do";cgReturnCode = BI_RETCODE_DICT_DO;};
		virtual ~BIC_DICT_DO(void){;};
	public:
		virtual	int32	Command	(BICPAR *tBICPAR,const std::string &par,std::string *ret)const;
		virtual	int32	Help	(BICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class BIC_DICT_READ : public BIC_Node{
	public:
		enum{RFLAG_C = 0, RFLAG_S = BIC_Node::RFLAG_S + BIC_Node::RFLAG_C};
	public:
				 BIC_DICT_READ(void) : BIC_Node() {cgCommand = "read";cgReturnCode = BI_RETCODE_DICT_READ;};
		virtual ~BIC_DICT_READ(void){;};
	public:
		virtual	int32	Command	(BICPAR *tBICPAR,const std::string &par,std::string *ret)const;
		virtual	int32	Help	(BICPAR *tBICPAR,int32 blDetail = 1)const;
				int32	ReadDcict(DICTWORD_NODE *dict);
};
//------------------------------------------------------------------------------------------//
class BIC_DICT_SAVE : public BIC_Node{
	public:
		enum{RFLAG_C = 0, RFLAG_S = BIC_Node::RFLAG_S + BIC_Node::RFLAG_C};
	public:
				 BIC_DICT_SAVE(void) : BIC_Node() {cgCommand = "save";cgReturnCode = BI_RETCODE_DICT_SAVE;};
		virtual ~BIC_DICT_SAVE(void){;};
	public:
		virtual	int32	Command	(BICPAR *tBICPAR,const std::string &par,std::string *ret)const;
		virtual	int32	Help	(BICPAR *tBICPAR,int32 blDetail = 1)const;
		int32	ReadDcict(DICTWORD_NODE *dict);
};
//------------------------------------------------------------------------------------------//
class BIC_DICT_FIND : public BIC_Node{
	public:
		enum{RFLAG_C = 0, RFLAG_S = BIC_Node::RFLAG_S + BIC_Node::RFLAG_C};
	public:
				 BIC_DICT_FIND(void) : BIC_Node() {cgCommand = "find";cgReturnCode = BI_RETCODE_DICT_FIND;};
		virtual ~BIC_DICT_FIND(void){;};
	public:
		virtual	int32	Command	(BICPAR *tBICPAR,const std::string &par,std::string *ret)const;
		virtual	int32	Help	(BICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class BIC_DICT_UPDATE : public BIC_Node{
	public:
		enum{RFLAG_C = 0, RFLAG_S = BIC_Node::RFLAG_S + BIC_Node::RFLAG_C};
	public:
				 BIC_DICT_UPDATE(void) : BIC_Node() {cgCommand = "update";cgReturnCode = BI_RETCODE_DICT_UPDATE;};
		virtual ~BIC_DICT_UPDATE(void){;};
	public:
		virtual	int32	Command	(BICPAR *tBICPAR,const std::string &par,std::string *ret)const;
		virtual	int32	Help	(BICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class BIC_DICT : public BIC_Node_S{
	public:
		enum{RFLAG_C = 0, RFLAG_S = BIC_Node_S::RFLAG_S + BIC_Node_S::RFLAG_C};
	public:
				 BIC_DICT(void) : BIC_Node_S() {cgCommand = "dict";cgTitle = cgCommand;cgReturnCode = BI_RETCODE_DICT;Init();};
		virtual ~BIC_DICT(void){;};
	public:
		virtual	int32	Help	(BICPAR *tBICPAR,int32 blDetail = 1)const;
	private:
		void	Init(void){
			AddNode(&cgSub_read);
			AddNode(&cgSub_save);
			AddNode(&cgSub_find);
			AddNode(&cgSub_update);
			AddNode(&cgSub_do);
		}
		BIC_DICT_READ		cgSub_read;
		BIC_DICT_SAVE		cgSub_save;
		BIC_DICT_FIND		cgSub_find;
		BIC_DICT_UPDATE		cgSub_update;
		BIC_DICT_DO			cgSub_do;
};
//------------------------------------------------------------------------------------------//





























//------------------------------------------------------------------------------------------//
#endif