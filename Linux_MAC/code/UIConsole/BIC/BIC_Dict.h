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
//------------------------------------------------------------------------------------------//
#include "BIC_B.h"
//------------------------------------------------------------------------------------------//
#if defined BIC_BH && defined SWVERSION_DICT
#ifndef BIC_DICTH
#define BIC_DICTH
#ifdef BIC_DICTH
//------------------------------------------------------------------------------------------//
#define HTML_SingleTagList "link/img/hr/br/!--/!DOCTYPE"
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
class HTML_NODE : public TREE_NODE{
	public:
		enum{
			HTML_None = 0,
			HTML_Tags,
			HTML_Attribute,
			HTML_Content,
		};
	public:
				 HTML_NODE(void) : TREE_NODE()	{Init();};
		virtual ~HTML_NODE(void){;};
	private:
		inline	virtual	TNFP*	CreateNode(void){return(SetSubNodeSelfName(new HTML_NODE));};
				HTML_NODE*		GetNewNode(void);
		inline	virtual	void	MoveToTrash	(TNF *tFirstNode,TNF *tEndNode = nullptr){MoveNodesToTrash(tFirstNode,tEndNode,&sgSpareOwner);};
	public:
		void	Init(void){cgTagsName = "";cgTagsValue = "";cgTagsType = HTML_None;cgIsTagSingle = 0;};
	public:
		static	TREE_NODE	sgSpareOwner;
		static	FIFO8		sgLogFifo;
	public:
		STDSTR		cgTagsName;
		STDSTR		cgTagsValue;
		uint32		cgTagsType;
		uint32		cgIsTagSingle;
	private:
				int32	ResolveContent		(FIFO8 *fifoIn);
				int32	ResolveAttribute	(FIFO8 *fifoIn);
				int32	ResolveTags			(FIFO8 *fifoIn);
				int32	IsTagSingle			(void)const;
		const	STDSTR&	ComposeTag			(STDSTR *strRet)const;
		const	STDSTR&	ComposeTagForLog	(const HTML_NODE *tHtmlNode,STDSTR *strRet);
	public:
		inline	int32	Resolve				(FIFO8 *fifoIn){return(ResolveTags(fifoIn));};
		const	STDSTR&	Compose				(STDSTR *strRet,STDSTR strDeep,G_SPACE blSpace)const;
	public:
				uint64	CheckTags			(const STDSTR &tagName,const STDSTR &attrName,const STDSTR &attrValue);
				uint64	SetTags				(const STDSTR &tagName,const STDSTR &attrName,const STDSTR &attrValue
											 ,const STDSTR &newTagName,const STDSTR &newAttrName,const STDSTR &newAttrValue);
	public:
				uint64	DelAttribute			(const STDSTR &tagName,const STDSTR &attrName,const STDSTR &attrValue);
				uint64	DelTagsExcludeContent	(const STDSTR &tagName,const STDSTR &attrName,const STDSTR &attrValue);
				uint64	DelTagsIncludeContent	(const STDSTR &tagName,const STDSTR &attrName,const STDSTR &attrValue);
				uint64	DelTagsBlankContent		(const STDSTR &tagName);
				void	DelTagExcludeContent	(void);
};
//------------------------------------------------------------------------------------------//
enum	G_DICT_RM	{G_DICT_RM_COMPRESS_NO = 0	,G_DICT_RM_COMPRESS_YES};
//------------------------------------------------------------------------------------------//
class DICTWORD_NODE : public TREE_NODE{
	public:
				 DICTWORD_NODE(void) : TREE_NODE(){Init();};
		virtual ~DICTWORD_NODE(void){;};
	public:
		void	Init(void);
	public:
		static	FIFO8	sgLogFifo;
	public:
		STDSTR		cgOWord;
		STDSTR		cgOWordLowerCase;
		STDSTR		cgOExplain;
		uint32		cgOrderNumber;
		uint64		cgELength;
		HTML_NODE	cgHtmlContent;
	public:
		int32	ReadCompressDict	(const STDSTR &fileName);
		int32	ReadDict			(const STDSTR &fileName);
		int32	SaveCompressDict	(const STDSTR &fileName);
		int32	SaveDict			(const STDSTR &fileName);
		uint64	CheckTags			(const STDSTR &tagName,const STDSTR &attrName,const STDSTR &attrValue);
		uint64	SetTags				(const STDSTR &tagName,const STDSTR &attrName,const STDSTR &attrValue
									 ,const STDSTR &newTagName,const STDSTR &newAttrName,const STDSTR &newAttrValue);
		uint64	DelAttribute			(const STDSTR &tagName,const STDSTR &attrName,const STDSTR &attrValue);
		uint64	DelTagsExcludeContent	(const STDSTR &tagName,const STDSTR &attrName,const STDSTR &attrValue);
		uint64	DelTagsIncludeContent	(const STDSTR &tagName,const STDSTR &attrName,const STDSTR &attrValue);
		uint64	DelTagsBlankContent		(const STDSTR &tagName);
};
//------------------------------------------------------------------------------------------//
class DICT_THREAD : public SYS_AThread{
	public:
				 DICT_THREAD(void) : SYS_AThread(){cgDict = nullptr;};
		virtual ~DICT_THREAD(void){;};
	private:
		DICTWORD_NODE	*cgDict;
		G_DICT_RM		cgblCompress;
		STDSTR			cgFilename;
		uint32			cgReadNum;
	private:
		void	Execute(void){
			cgReadNum = 0;
			if (cgDict != nullptr){
				if (cgblCompress == G_DICT_RM_COMPRESS_YES){
					cgReadNum = cgDict->ReadCompressDict(cgFilename);
				}
				else{
					cgReadNum = cgDict->ReadDict(cgFilename);
				}
			}
			cgDict = nullptr;
		};
	public:
		inline	const uint32&	ReadNum	(void){return(cgReadNum);};
		void	Read(DICTWORD_NODE *dict,const STDSTR &filename,G_DICT_RM blCompose){
			cgDict = dict;
			cgFilename = filename;
			cgblCompress = blCompose;
			ThreadRun();
		};
};
//------------------------------------------------------------------------------------------//
class COCAWORD_NODE : public TREE_NODE{
	public:
				 COCAWORD_NODE(void) : TREE_NODE(){
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
		STDSTR	cgOWord;
		STDSTR	cgO9W;
		STDSTR	cgO8W;
		STDSTR	cgOrder;
		STDSTR	cgAttribute;
		STDSTR	cgCStar;
		STDSTR	cgDeck;
		STDSTR	cgO9E;
		STDSTR	cgO8E;
		uint32	cgLineNumber;
	public:
		int32	ReadFromFile(const STDSTR &fileName);
		int32	UpdateDict	(DICTWORD_NODE *dict9,DICTWORD_NODE *dict8);
		int32	WriteToFile	(const STDSTR &fileName);
};
//------------------------------------------------------------------------------------------//






//------------------------------------------------------------------------------------------//
#ifdef SWVERSION_DICT
//------------------------------------------------------------------------------------------//
class BIC_DICT_DO : public BIC_Node{
	public:
				 BIC_DICT_DO(void) : BIC_Node() {cgCommand = "do";cgReturnCode = BI_RETCODE_DICT_DO;};
		virtual ~BIC_DICT_DO(void){;};
	public:
		virtual	int32	Command (BIC_ENV *env,const STDSTR &par,void *p)const;
		virtual	int32	Help	(BIC_ENV *env,int32 blDetail = 1)const{
			PrintHelpItem(env,cgCommand,"Handle Html");
			if (blDetail == 0)
				return(cgReturnCode);
			PrintHelpSubItem(env,"<-d>  <tag>"			,"Delete tags attribute.");
			PrintHelpSubItem(env,"      <attr>"			,"Attribute.");
			PrintHelpSubItem(env,"      <value>"		,"Attribute Value.");
			PrintHelpSubItem(env,"<-db> <tag>"			,"Delete tags which has no content.");
			PrintHelpSubItem(env,"<-dti><tag>"			,"Delete tags include content.");
			PrintHelpSubItem(env,"      <attr>"			,"Attribute.");
			PrintHelpSubItem(env,"      <value>"		,"Attribute Value.");
			PrintHelpSubItem(env,"<-dte><tag>"			,"Delete tags exclude content.");
			PrintHelpSubItem(env,"      <attr>"			,"Attribute.");
			PrintHelpSubItem(env,"      <value>"		,"Attribute Value.");
		
			PrintHelpSubItem(env,"<-s>  <tag>"			,"Set tags' attribute.");
			PrintHelpSubItem(env,"      <attr>"			,"Attribute name.");
			PrintHelpSubItem(env,"      <value>"		,"Attribute Value.");
			PrintHelpSubItem(env,"      <nTag>"			,"New tag name.");
			PrintHelpSubItem(env,"      <nAttr>"		,"New attribute name.");
			PrintHelpSubItem(env,"      <nValue>"		,"New attribute Value.");
			//PrintHelpSubItem(env,"<-st> <tage>"			,"Set tags' name.");
			//PrintHelpSubItem(env,"      <newTag>"			,"New tag name.");
		
			PrintHelpSubItem(env,"<-c>  <tag>"			,"Check tags' attribute.");
			PrintHelpSubItem(env,"      <attr>"			,"Attribute name.");
			PrintHelpSubItem(env,"      <value>"		,"Attribute Value.");
		
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class BIC_DICT_READ : public BIC_Node{
	public:
				 BIC_DICT_READ(void) : BIC_Node() {cgCommand = "read";cgReturnCode = BI_RETCODE_DICT_READ;};
		virtual ~BIC_DICT_READ(void){;};
	public:
		virtual	int32	Command (BIC_ENV *env,const STDSTR &par,void *p)const;
		virtual	int32	Help	(BIC_ENV *env,int32 blDetail = 1)const{
			PrintHelpItem(env,cgCommand,"Read dict/coca data");
			if (blDetail == 0)
				return(cgReturnCode);
			PrintHelpSubItem(env,"<d|dict|coca>"	,"Compress dict/dict/coca.");
			PrintHelpSubItem(env,"<filename>"		,"File name.");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class BIC_DICT_SAVE : public BIC_Node{
	public:
				 BIC_DICT_SAVE(void) : BIC_Node() {cgCommand = "save";cgReturnCode = BI_RETCODE_DICT_SAVE;};
		virtual ~BIC_DICT_SAVE(void){;};
	public:
		virtual	int32	Command (BIC_ENV *env,const STDSTR &par,void *p)const;
		virtual	int32	Help	(BIC_ENV *env,int32 blDetail = 1)const{
			PrintHelpItem(env,cgCommand,"Save dict");
			if (blDetail == 0)
				return(cgReturnCode);
			PrintHelpSubItem(env,"<d|dict>"		,"Compress dict/dict.");
			PrintHelpSubItem(env,"<filename>"	,"File name.");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class BIC_DICT_FIND : public BIC_Node{
	public:
				 BIC_DICT_FIND(void) : BIC_Node() {cgCommand = "find";cgReturnCode = BI_RETCODE_DICT_FIND;};
		virtual ~BIC_DICT_FIND(void){;};
	public:
		virtual	int32	Command (BIC_ENV *env,const STDSTR &par,void *p)const;
		virtual	int32	Help	(BIC_ENV *env,int32 blDetail = 1)const{
			PrintHelpItem(env,cgCommand,"Find word in dict/coca");
			if (blDetail == 0)
				return(cgReturnCode);
			PrintHelpSubItem(env,"<d|coca><word>"	,"Search in Dict.");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class BIC_DICT_UPDATE : public BIC_Node{
	public:
				 BIC_DICT_UPDATE(void) : BIC_Node() {cgCommand = "update";cgReturnCode = BI_RETCODE_DICT_UPDATE;};
		virtual ~BIC_DICT_UPDATE(void){;};
	public:
		virtual	int32	Command (BIC_ENV *env,const STDSTR &par,void *p)const;
		virtual	int32	Help	(BIC_ENV *env,int32 blDetail = 1)const{
				PrintHelpItem(env,cgCommand,"Update explanation");
				if (blDetail == 0)
					return(cgReturnCode);
				PrintHelpSubItem(env,"<d9|d8>"		,"Output file name.");
				PrintHelpSubItem(env,"<filename>"	,"Output file name.");
				return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class BIC_DICT : public BIC_Node_S{
	public:
				 BIC_DICT(void) : BIC_Node_S() {cgCommand = "dict";cgTitle = cgCommand;cgReturnCode = BI_RETCODE_DICT;Init();};
		virtual ~BIC_DICT(void){;};
	public:
		virtual	int32	Help	(BIC_ENV *env,int32 blDetail = 1)const{
			if (blDetail == 1){
				HelpTraversalChild(env,0);
				return(cgReturnCode);
			}
			PrintHelpItem(env, cgCommand, "-> Dict.");
			if (blDetail == 0)
				return(cgReturnCode);
			return(cgReturnCode);
	};;
	private:
		void	Init(void){Add(cgSub_read) < cgSub_save < cgSub_find < cgSub_update < cgSub_do;}
		BIC_DICT_READ		cgSub_read;
		BIC_DICT_SAVE		cgSub_save;
		BIC_DICT_FIND		cgSub_find;
		BIC_DICT_UPDATE		cgSub_update;
		BIC_DICT_DO			cgSub_do;
};
//------------------------------------------------------------------------------------------//
#endif
//------------------------------------------------------------------------------------------//
#endif
#endif
#endif

