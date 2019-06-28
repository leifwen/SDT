//
//  Dict.h
//  SDT
//
//  Created by Leif Wen on 25/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

//------------------------------------------------------------------------------------------//
#include "HTML.h"
#include "SYS_Thread.h"
#if defined HTML_h && defined SYS_Thread_h
//------------------------------------------------------------------------------------------//
#ifndef Dict_h
#define Dict_h
#ifdef Dict_h
//------------------------------------------------------------------------------------------//
enum	G_DICT_RM	{G_DICT_RM_COMPRESS_NO = 0	,G_DICT_RM_COMPRESS_YES};
//------------------------------------------------------------------------------------------//
class DICTWORD_NODE : public TNFP{
	public:
				 DICTWORD_NODE(void);
		virtual ~DICTWORD_NODE(void);
	public:
		static	ARRAY&		GetLogArray			(void);
	public:
				STDSTR		cgOWord;
				STDSTR		cgOWordLowerCase;
				STDSTR		cgOExplain;
				uint32		cgOrderNumber;
				uint64		cgELength;
				HTML_NODE	cgHtmlContent;
	public:
		void	Init					(void);
		void	Empty					(void);
		bool32	ReadCompressDict		(const STDSTR& fileName);
		bool32	ReadDict				(const STDSTR& fileName);
		bool32	SaveCompressDict		(const STDSTR& fileName);
		bool32	SaveDict				(const STDSTR& fileName);
		uint64	CheckTags				(const STDSTR& tagName,const STDSTR& attrName,const STDSTR& attrValue);
		uint64	SetTags					(const STDSTR& tagName,const STDSTR& attrName,const STDSTR& attrValue
								 		,const STDSTR& newTagName,const STDSTR& newAttrName,const STDSTR& newAttrValue);
		uint64	DelAttribute			(const STDSTR& tagName,const STDSTR& attrName,const STDSTR& attrValue);
		uint64	DelTagsExcludeContent	(const STDSTR& tagName,const STDSTR& attrName,const STDSTR& attrValue);
		uint64	DelTagsIncludeContent	(const STDSTR& tagName,const STDSTR& attrName,const STDSTR& attrValue);
		uint64	DelTagsBlankContent		(const STDSTR& tagName);
};
//------------------------------------------------------------------------------------------//
class DICT_THREAD : public SYS_AThread{
	public:
				 DICT_THREAD(void);
		virtual ~DICT_THREAD(void){;};
	private:
		DICTWORD_NODE*	cgDict;
		G_DICT_RM		cgblCompress;
		STDSTR			cgFilename;
		uint32			cgReadNum;
	private:
				void	Execute	(void* exep);
	public:
		const	uint32&	ReadNum	(void);
				void	Read	(DICTWORD_NODE* dict,const STDSTR& filename,G_DICT_RM blCompose);
};
//------------------------------------------------------------------------------------------//
class COCAWORD_NODE : public TNFP{
	public:
				 COCAWORD_NODE(void);
		virtual ~COCAWORD_NODE(void);
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
		void	Empty			(void);
		bool32	ReadFromFile	(const STDSTR& fileName);
		bool32	WriteToFile		(const STDSTR& fileName);
		bool32	UpdateDict		(DICTWORD_NODE* dict9,DICTWORD_NODE* dict8);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif /* Dict_h */
#endif /* Dict_h */
#endif /* HTML.h */
