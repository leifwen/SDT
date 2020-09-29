//
//  HTML.h
//  SDT
//
//  Created by Leif Wen on 25/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

//------------------------------------------------------------------------------------------//
#include "DS_ARRAY.h"
#include "DS_Tree.h"
#if defined DS_Tree_h && defined DS_ARRAY_h
//------------------------------------------------------------------------------------------//
#ifndef HTML_h
#define HTML_h
#ifdef HTML_h
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#define HTML_SingleTagList "link/img/hr/br/!--/!DOCTYPE"
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
class HTML_NODE : public TNFP{
	protected:
		enum	{RFLAG_C = 1, RFLAG_S = TNFP::RFLAG_S + TNFP::RFLAG_C};
		enum	{HTML_blSingleTag = RFLAG_CREATE(0)};
	public:
		enum	{HTML_None = 0,HTML_Tags,HTML_Attribute,HTML_Content,};
	public:
				 HTML_NODE(void);
		virtual ~HTML_NODE(void);
	public:
				void		Init				(void);
		static	ARRAY&		GetLogArray			(void);
		static	TNFP&		GetTrashOwer		(void);
	private:
		virtual	TNFP*		GetTrash			(void);
		virtual	TNF*		CreateNode			(void);
				void		MoveToTrash			(TNF* tFirstNode,TNF* tEndNode = nullptr);
	public:
				STDSTR		cgTagsName;
				STDSTR		cgTagsValue;
				uint32		cgTagsType;
	private:
				bool32		IsSingleTag				(void)const;
				bool32		ResolveContent			(ARRAY* _in);
				bool32		ResolveAttribute		(ARRAY* _in);
				bool32		ResolveTags				(ARRAY* _in);
		const	STDSTR&		ComposeTag				(STDSTR* strRet)const;
		const	STDSTR&		ComposeTagForLog		(STDSTR* strRet,const HTML_NODE* htmlNode);
	public:
				bool32		Resolve					(ARRAY* _in);
		const	STDSTR&		Compose					(STDSTR* strRet,const STDSTR& strDeep,G_SPACE blSpace)const;
	public:
				uint64		CheckTags				(const STDSTR& tagName,const STDSTR& attrName,const STDSTR& attrValue);
				uint64		SetTags					(const STDSTR& tagName,const STDSTR& attrName,const STDSTR& attrValue
													,const STDSTR& newTagName,const STDSTR& newAttrName,const STDSTR& newAttrValue);
	public:
				uint64		DelAttribute			(const STDSTR& tagName,const STDSTR& attrName,const STDSTR& attrValue);
				uint64		DelTagsExcludeContent	(const STDSTR& tagName,const STDSTR& attrName,const STDSTR& attrValue);
				uint64		DelTagsIncludeContent	(const STDSTR& tagName,const STDSTR& attrName,const STDSTR& attrValue);
				uint64		DelTagsBlankContent		(const STDSTR& tagName);
				void		DelTagExcludeContent	(void);
};
//------------------------------------------------------------------------------------------//
#endif /* HTML_h */
#endif /* HTML_h */
#endif /* DS_Tree_h */
