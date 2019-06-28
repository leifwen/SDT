//
//  HTML.cpp
//  SDT
//
//  Created by Leif Wen on 25/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "stdafx.h"
//------------------------------------------------------------------------------------------//
#include "HTML.h"
#ifdef HTML_h
//------------------------------------------------------------------------------------------//
#include "DS_STRING.h"
//------------------------------------------------------------------------------------------//
enum{
	HTML_Wait_fls = 0,	// first <
	HTML_Wait_fchar,	// first no space char for cgTagsName
	HTML_Wait_fspace,	// first space
	HTML_Wait_nspace,	// next space
	HTML_Wait_equal,	// =
	HTML_Wait_fdq,		// first "
	HTML_Wait_sdq,		// second "
	HTML_Wait_nls,		// next <
	HTML_Wait_nmr,		// next >
	HTML_Wait_end,		// </>
	HTML_Wait_slash,	// /
};
enum{
	HTML_R_NoResource = -3,
	HTML_R_NoComplete = -2,
	HTML_R_NoRecMore = -1,
	HTML_R_Error = 0,
	HTML_R_OK,
	HTML_R_NoRecValue,
	HTML_R_Exchange,
};
//------------------------------------------------------------------------------------------//
HTML_NODE::HTML_NODE(void) : TNFP(){
	Init();
};
//------------------------------------------------------------------------------------------//
HTML_NODE::~HTML_NODE(void){
	
	CleanDownTree(this,this);
};
//------------------------------------------------------------------------------------------//
void HTML_NODE::Init(void){
	cgTagsName = "";
	cgTagsValue = "";
	cgTagsType = HTML_None;
	ClrSFlag(HTML_blSingleTag);
};
//------------------------------------------------------------------------------------------//
ARRAY& HTML_NODE::GetLogArray(void){
	static ARRAY sgLogArray;
	return(sgLogArray);
};
//------------------------------------------------------------------------------------------//
TNFP& HTML_NODE::GetTrashOwer(void){
	static TNFP sgSpareOwner;
	return(sgSpareOwner);
};
//------------------------------------------------------------------------------------------//
TNFP* HTML_NODE::GetTrash(void){
	
	return(GetTrashOwer().GetTrash());
};
//------------------------------------------------------------------------------------------//
TNFP* HTML_NODE::CreateNode(void){
	
	return(SetNodeUpName(new HTML_NODE));
};
//------------------------------------------------------------------------------------------//
void HTML_NODE::MoveToTrash(TNF* tFirstNode,TNF* tEndNode){
	
	MoveNodesToTrash(&GetTrashOwer(),tFirstNode,tEndNode);
};
//------------------------------------------------------------------------------------------//
bool32 HTML_NODE::IsSingleTag(void)const{
	STDSTR	tagsList = HTML_SingleTagList;

	if (CheckSFlag(HTML_blSingleTag))
		return G_TRUE;
	
	while(tagsList.length() > 0){
		if (cgTagsName == Str_ReadSubItem(&tagsList, "/"))
			return G_TRUE;
	};
	return G_FALSE;
};
//------------------------------------------------------------------------------------------//
bool32 HTML_NODE::ResolveContent(ARRAY* _in){
	uint8	charData;
	STDSTR	strTag;
	bool32	blSpace;
	
	cgTagsName = "";
	cgTagsValue = " ";
	cgTagsType = HTML_Content;
	blSpace = G_FALSE;
	charData = 0;
	while(_in->IsEmpty() == G_FALSE){
		_in->Get(&charData, 1);
		if (charData < 0x20)
			continue;
		if (charData == '<'){
			_in->Out_RollBack(1);
			_in->UpdateByOffsetOut(&charData, 1, 0);
			Str_TrimSelf(cgTagsValue);
			return HTML_R_OK;
		}
		if (blSpace == G_FALSE){
			if (charData == ' ')
				blSpace = G_TRUE;
		}
		else{
			if (charData == ' ')
				continue;
			blSpace = G_FALSE;
		}
		cgTagsValue += charData;
	}
	GetLogArray().Put("[E][cont] no complete.",G_ESCAPE_OFF);
	GetLogArray().Put(ComposeTagForLog(&strTag,(HTML_NODE*)GetUp(this)),G_ESCAPE_OFF);
	return HTML_R_NoComplete;
};
//------------------------------------------------------------------------------------------//
bool32 HTML_NODE::ResolveAttribute(ARRAY* _in){
	uint8	charData;
	bool32	status,blRecSpace;
	STDSTR	strTag;
	
	cgTagsName = "";
	cgTagsValue = "";
	cgTagsType = HTML_Attribute;
	charData = 0;
	status = HTML_Wait_equal;
	blRecSpace = G_FALSE;
	while(_in->IsEmpty() == G_FALSE){
		_in->Get(&charData, 1);
		if (charData < 0x20)
			continue;
		if (charData == '<'){
			GetLogArray().Put("[E][attr] receive '<'.",G_ESCAPE_OFF);
			GetLogArray().Put(ComposeTagForLog(&strTag,(HTML_NODE*)GetUp(this)),G_ESCAPE_OFF);
			return HTML_R_Error;
		}
		if (charData == '>'){
			if (status == HTML_Wait_equal){
				_in->Out_RollBack(1);
				_in->UpdateByOffsetOut(&charData, 1, 0);
				if (cgTagsName != "/"){
					GetLogArray().Put("[W][attr] no receive cgTagsValue.",G_ESCAPE_OFF);
					GetLogArray().Put(ComposeTagForLog(&strTag,(HTML_NODE*)GetUp(this)),G_ESCAPE_OFF);
					return HTML_R_NoRecValue;
				}
				return HTML_R_OK;
			}
			GetLogArray().Put("[E][attr] receive '>' when decode cgTagsValue.",G_ESCAPE_OFF);
			GetLogArray().Put(ComposeTagForLog(&strTag,(HTML_NODE*)GetUp(this)),G_ESCAPE_OFF);
			return HTML_R_Error;
		}
		switch (status){
			case HTML_Wait_equal:
				if (charData == '"'){
					GetLogArray().Put("[E][attr] receive '\"' when decode cgTagsName.",G_ESCAPE_OFF);
					GetLogArray().Put(ComposeTagForLog(&strTag,(HTML_NODE*)GetUp(this)),G_ESCAPE_OFF);
					return HTML_R_Error;
				}
				if (charData == '='){
					if (cgTagsName.length() == 0){
						GetLogArray().Put("[E][attr] receive '=' but cgTagsName is blank.",G_ESCAPE_OFF);
						GetLogArray().Put(ComposeTagForLog(&strTag,(HTML_NODE*)GetUp(this)),G_ESCAPE_OFF);
						return HTML_R_Error;
					}
					status = HTML_Wait_fdq;
				}
				else if (charData == ' '){
					if (cgTagsName.length() > 0)
						blRecSpace = G_TRUE;
				}
				else {
					if (blRecSpace == G_TRUE){
						_in->Out_RollBack(1);
						_in->UpdateByOffsetOut(&charData, 1, 0);
						GetLogArray().Put("[W][attr] no receive cgTagsValue.",G_ESCAPE_OFF);
						GetLogArray().Put(ComposeTagForLog(&strTag,(HTML_NODE*)GetUp(this)),G_ESCAPE_OFF);
						return HTML_R_NoRecValue;
					}
					cgTagsName += charData;
				}
				break;
			case HTML_Wait_fdq:
				if (charData == '"'){
					status = HTML_Wait_sdq;
				}
				else if (charData != ' '){
					GetLogArray().Put("[E][attr] receive char between '=' and '\"'.",G_ESCAPE_OFF);
					GetLogArray().Put(ComposeTagForLog(&strTag,(HTML_NODE*)GetUp(this)),G_ESCAPE_OFF);
					return HTML_R_Error;
				}
				break;
			case HTML_Wait_sdq:
				if (charData == '"'){
					cgTagsValue = Str_Trim(cgTagsValue);
					return HTML_R_OK;
				}
				cgTagsValue += charData;
			default:;
		}
	}
	GetLogArray().Put("[E][attr] no complete.",G_ESCAPE_OFF);
	GetLogArray().Put(ComposeTagForLog(&strTag,(HTML_NODE*)GetUp(this)),G_ESCAPE_OFF);
	return HTML_R_NoComplete;
};
//------------------------------------------------------------------------------------------//
bool32 HTML_NODE::ResolveTags(ARRAY* _in){
	uint8		charData;
	bool32		status,ret;
	HTML_NODE	*newHtmlNode;
	STDSTR		strEnd,strTag;
	
	cgTagsName = "";
	cgTagsValue = "";
	cgTagsType = HTML_Tags;
	strEnd = "";
	charData = 0;
	status = HTML_Wait_fls;
	while(_in->IsEmpty() == G_FALSE){
		_in->Get(&charData, 1);
		if (charData < 0x20)
			continue;
		switch (status){
			case HTML_Wait_fls:
				if (charData == '<')
					status = HTML_Wait_fchar;
				break;
			case HTML_Wait_fchar:
				if (charData == ' ')
					break;
				status = HTML_Wait_fspace;
			case HTML_Wait_fspace:
				if ((charData == '<') || (charData == '"') || (charData == '=')){
					GetLogArray().Put("[E][tags] receive '<' or '\"' or '='.",G_ESCAPE_OFF);
					GetLogArray().Put(ComposeTagForLog(&strTag,this),G_ESCAPE_OFF);
					return HTML_R_Error;
				}
				if (charData == '>'){
					if (cgTagsName.length() == 0){
						cgTagsName = "";
						cgTagsValue = "&lt; &gt;";
						cgTagsType = HTML_Content;
						GetLogArray().Put("[W][tags] receive '>' but cgTagsName is blank.",G_ESCAPE_OFF);
						GetLogArray().Put(ComposeTagForLog(&strTag,this),G_ESCAPE_OFF);
						GetLogArray().Put("[W][tags] exchage '<>' to '&lt; &gt;'.",G_ESCAPE_OFF);
						GetLogArray().Put(ComposeTagForLog(&strTag,this),G_ESCAPE_OFF);
						return HTML_R_Exchange;
					}
					if (IsSingleTag())
						return HTML_R_OK;
					status = HTML_Wait_nls;
				}
				else if (charData == ' '){
					status = HTML_Wait_nspace;
				}
				else{
					cgTagsName += charData;
				}
				break;
			case HTML_Wait_nspace:
				if (charData == '<'){
					GetLogArray().Put("[E][tags] receive '<'.",G_ESCAPE_OFF);
					GetLogArray().Put(ComposeTagForLog(&strTag,this),G_ESCAPE_OFF);
					return HTML_R_Error;
				}
				if (charData == '>'){
					newHtmlNode = (HTML_NODE*)GetTail(GetDown(this));
					if ((newHtmlNode != nullptr) && (newHtmlNode->cgTagsName == "/")){
						SetSFlag(HTML_blSingleTag);
						return HTML_R_OK;
					}
					if (IsSingleTag())
						return HTML_R_OK;
					status = HTML_Wait_nls;
				}
				else{
					_in->Out_RollBack(1);
					_in->UpdateByOffsetOut(&charData, 1, 0);
					newHtmlNode = (HTML_NODE*)GetNewNode();
					if (newHtmlNode == nullptr){
						GetLogArray().Put("[E][tags] no enough memory.",G_ESCAPE_OFF);
						GetLogArray().Put(ComposeTagForLog(&strTag,this),G_ESCAPE_OFF);
						return HTML_R_NoResource;
					}
					newHtmlNode->Init();
					InsertDownTail_nolock(this,newHtmlNode);
					ret = newHtmlNode->ResolveAttribute(_in);
					if (ret <= 0)
						return(ret);
				}
				break;
			case HTML_Wait_nls:
				if (charData == '<'){
					status = HTML_Wait_nmr;
				}
				else{
					_in->Out_RollBack(1);
					_in->UpdateByOffsetOut(&charData, 1, 0);
					newHtmlNode = (HTML_NODE*)GetNewNode();
					if (newHtmlNode == nullptr){
						GetLogArray().Put("[E][tags] no enough memory.",G_ESCAPE_OFF);
						GetLogArray().Put(ComposeTagForLog(&strTag,this),G_ESCAPE_OFF);
						return HTML_R_NoResource;
					}
					newHtmlNode->Init();
					InsertDownTail_nolock(this,newHtmlNode);
					ret = newHtmlNode->ResolveContent(_in);
					if (ret <= 0)
						return(ret);
					if (newHtmlNode->cgTagsValue.length() == 0)
						MoveToTrash(newHtmlNode,newHtmlNode);
				}
				break;
			case HTML_Wait_nmr:
				if (charData == '>'){
					status = HTML_Wait_nls;
				}
				else if (charData == '/'){
					strEnd = "";
					status = HTML_Wait_end;
				}
				else{
					_in->Out_RollBack(1);
					_in->UpdateByOffsetOut(&charData, 1, 0);
					_in->Out_RollBack(1);
					charData = '<';
					_in->UpdateByOffsetOut(&charData, 1, 0);
					newHtmlNode = (HTML_NODE*)GetNewNode();
					if (newHtmlNode == nullptr){
						GetLogArray().Put("[E][tags] no enough memory.",G_ESCAPE_OFF);
						GetLogArray().Put(ComposeTagForLog(&strTag,this),G_ESCAPE_OFF);
						return HTML_R_NoResource;
					}
					newHtmlNode->Init();
					InsertDownTail_nolock(this,newHtmlNode);
					ret = newHtmlNode->ResolveTags(_in);
					if (ret <= 0)
						return(ret);
					status = HTML_Wait_nls;
				}
				break;
			case HTML_Wait_end:
				if ((charData == '<') || (charData == '"') || (charData == '=')){
					GetLogArray().Put("[E][tags] receive '<' or '\"' or '=' when decode </> part.",G_ESCAPE_OFF);
					GetLogArray().Put(ComposeTagForLog(&strTag,this),G_ESCAPE_OFF);
					return HTML_R_Error;
				}
				if (charData == '>'){
					if (strEnd == cgTagsName)
						return HTML_R_OK;
					GetLogArray().Put("[E][tags] miss match.",G_ESCAPE_OFF);
					GetLogArray().Put(ComposeTagForLog(&strTag,this),G_ESCAPE_OFF);
					return HTML_R_Error;
				}
				else{
					strEnd += charData;
				}
			default:;
		}
	}
	GetLogArray().Put("[E][tags] no complete.",G_ESCAPE_OFF);
	GetLogArray().Put(ComposeTagForLog(&strTag,this),G_ESCAPE_OFF);
	return HTML_R_NoComplete;
};
//------------------------------------------------------------------------------------------//
const STDSTR& HTML_NODE::ComposeTagForLog(STDSTR* strRet,const HTML_NODE* tHtmlNode){
	*strRet = "[cgTagsName : ";
	tHtmlNode->ComposeTag(strRet);
	*strRet += " ]\n";
	return(*strRet);
};
//------------------------------------------------------------------------------------------//
const STDSTR& HTML_NODE::ComposeTag(STDSTR* strRet)const{
	HTML_NODE	*oNode,*nextNode;
	
	if (cgTagsType == HTML_Tags){
		*strRet += '<';
		*strRet += cgTagsName;
		oNode = (HTML_NODE*)GetDown(this);
		while(oNode != nullptr){
			nextNode = (HTML_NODE*)GetNext(oNode);
			if (oNode->cgTagsType == HTML_Attribute){
				*strRet += ' ';
				*strRet += oNode->cgTagsName;
				if (oNode->cgTagsValue.length() > 0){
					*strRet += "=\"";
					*strRet += oNode->cgTagsValue;
					*strRet += '"';
				}
			}
			oNode = nextNode;
		};
		*strRet += '>';
	}
	return(*strRet);
};
//------------------------------------------------------------------------------------------//
bool32 HTML_NODE::Resolve(ARRAY* _in){
	return(ResolveTags(_in));
};
//------------------------------------------------------------------------------------------//
const STDSTR& HTML_NODE::Compose(STDSTR* strRet,const STDSTR& strDeep,G_SPACE blSpace)const{
	HTML_NODE	*oNode,*nextNode;
	STDSTR 		strDeepNew;
	
	if (cgTagsType == HTML_Tags){
		if (blSpace == G_SPACE_ON){
			*strRet += strDeep;
			strDeepNew = strDeep + "  ";
		}
		*strRet += '<';
		*strRet += cgTagsName;
		oNode = (HTML_NODE*)GetDown(this);
		while(oNode != nullptr){
			nextNode = (HTML_NODE*)GetNext(oNode);
			if (oNode->cgTagsType == HTML_Attribute){
				*strRet += ' ';
				*strRet += oNode->cgTagsName;
				if (oNode->cgTagsValue.length() > 0){
					*strRet += "=\"";
					*strRet += oNode->cgTagsValue;
					*strRet += '"';
				}
			}
			oNode = nextNode;
		};
		if (blSpace == G_SPACE_ON){
			*strRet += ">\n";
		}
		else{
			*strRet += '>';
		}
	}
	if (IsSingleTag() == 0){
		oNode = (HTML_NODE*)GetDown(this);
		while(oNode != nullptr){
			nextNode = (HTML_NODE*)GetNext(oNode);
			if (oNode->cgTagsType == HTML_Tags){
				oNode->Compose(strRet,strDeepNew,blSpace);
			}
			else if (oNode->cgTagsType == HTML_Content){
				if (blSpace == G_SPACE_ON)
					*strRet += strDeepNew;
				*strRet += ' ';
				*strRet += oNode->cgTagsValue;
				if (blSpace == G_SPACE_ON)
					*strRet += '\n';
			}
			oNode = nextNode;
		};
		if (cgTagsType == HTML_Tags){
			if (blSpace == G_SPACE_ON)
				*strRet += strDeep;
			*strRet += "</";
			*strRet += cgTagsName;
			if (blSpace == G_SPACE_ON){
				*strRet += ">\n";
			}
			else{
				*strRet += '>';
			}
		}
	}
	return(*strRet);
};
//------------------------------------------------------------------------------------------//
uint64 HTML_NODE::CheckTags(const STDSTR& tagName,const STDSTR& attrName,const STDSTR& attrValue){
	HTML_NODE	*oNode,*nextNode;
	uint64		count;
	bool32		blHasAttr;
	
	count = 0;
	if ((cgTagsType == HTML_Tags) || (cgTagsType == HTML_None)){
		oNode = (HTML_NODE*)GetDown(this);
		blHasAttr = G_FALSE;
		while(oNode != nullptr){
			nextNode = (HTML_NODE*)GetNext(oNode);
			if (oNode->cgTagsType == HTML_Attribute){
				blHasAttr = G_TRUE;
				if (((tagName == "*") || (cgTagsName == tagName))
					&& ((attrName == "*") || (oNode->cgTagsName == attrName))
					&& ((attrValue == "*") || (oNode->cgTagsValue == attrValue))){
					++ count;
				}
			}
			else if (oNode->cgTagsType == HTML_Tags){
				count += oNode->CheckTags(tagName,attrName,attrValue);
			}
			oNode = nextNode;
		};
		if ((blHasAttr == G_FALSE) && ((tagName == "*") || (cgTagsName == tagName)) && (attrName == "*") && (attrValue == "*"))
			++ count;
	}
	return(count);
};
//------------------------------------------------------------------------------------------//
uint64 HTML_NODE::SetTags(const STDSTR& tagName,const STDSTR& attrName,const STDSTR& attrValue
						  ,const STDSTR& newTagName,const STDSTR& newAttrName,const STDSTR& newAttrValue){
	HTML_NODE	*oNode,*nextNode;
	uint64		count;
	bool32		blHasAttr;
	
	count = 0;
	if ((cgTagsType == HTML_Tags) || (cgTagsType == HTML_None)){
		oNode = (HTML_NODE*)GetDown(this);
		blHasAttr = G_FALSE;
		while(oNode != nullptr){
			nextNode = (HTML_NODE*)GetNext(oNode);
			if (oNode->cgTagsType == HTML_Attribute){
				blHasAttr = G_TRUE;
				if (((tagName == "*") || (cgTagsName == tagName))
					&& ((attrName == "*") || (oNode->cgTagsName == attrName))
					&& ((attrValue == "*") || (oNode->cgTagsValue == attrValue))){
					if (newTagName != "*")
						cgTagsName = newTagName;
					if (newAttrName != "*")
						oNode->cgTagsName = newAttrName;
					if (newAttrValue != "*")
						oNode->cgTagsValue = newAttrValue;
					++ count;
				}
			}
			else if (oNode->cgTagsType == HTML_Tags){
				count += oNode->SetTags(tagName,attrName,attrValue,newTagName,newAttrName,newAttrValue);
			}
			oNode = nextNode;
		};
		if ((blHasAttr == G_FALSE) && ((tagName == "*") || (cgTagsName == tagName)) && (attrName == "*") && (attrValue == "*")){
			if (newTagName != "*")
				cgTagsName = newTagName;
			++ count;
		}
	}
	return(count);
};
//------------------------------------------------------------------------------------------//
uint64 HTML_NODE::DelAttribute(const STDSTR& tagName,const STDSTR& attrName,const STDSTR& attrValue){
	HTML_NODE	*oNode,*nextNode;
	uint64		count;
	bool32		blHasAttr;
	
	count = 0;
	if ((cgTagsType == HTML_Tags) || (cgTagsType == HTML_None)){
		oNode = (HTML_NODE*)GetDown(this);
		blHasAttr = G_FALSE;
		while(oNode != nullptr){
			nextNode = (HTML_NODE*)GetNext(oNode);
			if (oNode->cgTagsType == HTML_Attribute){
				blHasAttr = G_TRUE;
				if (((tagName == "*") || (cgTagsName == tagName))
					&& ((attrName == "*") || (oNode->cgTagsName == attrName))
					&& ((attrValue == "*") || (oNode->cgTagsValue == attrValue))){
					MoveToTrash(oNode,oNode);
					++ count;
				}
			}
			else if (oNode->cgTagsType == HTML_Tags){
				count += oNode->DelAttribute(tagName,attrName,attrValue);
			}
			oNode = nextNode;
		};
		if ((blHasAttr == G_FALSE) && ((tagName == "*") || (cgTagsName == tagName)) && (attrName == "*") && (attrValue == "*")){
			MoveToTrash(oNode,oNode);
			++ count;
		}
	}
	return(count);
};
//------------------------------------------------------------------------------------------//
void HTML_NODE::DelTagExcludeContent(void){
	HTML_NODE	*oNode,*nextNode;
	
	if ((cgTagsType == HTML_Tags) || (cgTagsType == HTML_None)){
		oNode = (HTML_NODE*)GetDown(this);
		while(oNode != nullptr){
			nextNode = (HTML_NODE*)GetNext(oNode);
			if (oNode->cgTagsType == HTML_Attribute)
				MoveToTrash(oNode,oNode);
			oNode = nextNode;
		};
		oNode = (HTML_NODE*)DetachDown_nolock(this);
		InsertAfter_nolock(this,oNode);
		MoveToTrash(this,this);
	}
};
//------------------------------------------------------------------------------------------//
uint64 HTML_NODE::DelTagsBlankContent(const STDSTR& tagName){
	HTML_NODE	*oNode,*nextNode;
	uint64		count,delCount;
	bool32		blContent,blTag;
	
	count = 0;
	if ((cgTagsType == HTML_Tags) || (cgTagsType == HTML_None)){
		if (IsSingleTag())
			return 0;
		do{
			oNode = (HTML_NODE*)GetDown(this);
			blContent = G_FALSE;
			blTag = G_FALSE;
			delCount = 0;
			while(oNode != nullptr){
				nextNode = (HTML_NODE*)GetNext(oNode);
				if (oNode->cgTagsType == HTML_Content){
					blContent = G_TRUE;
				}
				else if (oNode->cgTagsType == HTML_Tags){
					blTag = G_TRUE;
					delCount += oNode->DelTagsBlankContent(tagName);
				}
				oNode = nextNode;
			};
			count += delCount;
		}while(delCount > 0);
		if ((tagName == "*") || (cgTagsName == tagName)){
			if ((blContent == G_FALSE) && (blTag == G_FALSE)){
				++ count;
				DelTagExcludeContent();
			}
		}
	}
	return(count);
};
//------------------------------------------------------------------------------------------//
uint64 HTML_NODE::DelTagsExcludeContent(const STDSTR& tagName,const STDSTR& attrName,const STDSTR& attrValue){
	HTML_NODE	*oNode,*nextNode;
	uint64		count;
	bool32		blDel,blHasAttr;
	
	count = 0;
	if ((cgTagsType == HTML_Tags) || (cgTagsType == HTML_None)){
		oNode = (HTML_NODE*)GetDown(this);
		blDel = G_FALSE;
		blHasAttr = G_FALSE;
		while(oNode != nullptr){
			nextNode = (HTML_NODE*)GetNext(oNode);
			if (oNode->cgTagsType == HTML_Attribute){
				blHasAttr = G_TRUE;
				if (((tagName == "*") || (cgTagsName == tagName))
					&& ((attrName == "*") || (oNode->cgTagsName == attrName))
					&& ((attrValue == "*") || (oNode->cgTagsValue == attrValue))){
					blDel = G_TRUE;
				}
			}
			else if (oNode->cgTagsType == HTML_Tags){
				count += oNode->DelTagsExcludeContent(tagName,attrName,attrValue);
			}
			oNode = nextNode;
		};
		if (cgTagsType == HTML_Tags){
			if (blDel
				|| ((blHasAttr == G_FALSE) && ((tagName == "*") || (cgTagsName == tagName)) && (attrName == "*") && (attrValue == "*"))){
				++ count;
				DelTagExcludeContent();
			}
		}
	}
	return(count);
};
//------------------------------------------------------------------------------------------//
uint64 HTML_NODE::DelTagsIncludeContent(const STDSTR& tagName,const STDSTR& attrName,const STDSTR& attrValue){
	HTML_NODE	*oNode,*nextNode;
	uint64		count;
	bool32		blDel,blHasAttr;
	
	count = 0;
	if ((cgTagsType == HTML_Tags) || (cgTagsType == HTML_None)){
		oNode = (HTML_NODE*)GetDown(this);
		blDel = G_FALSE;
		blHasAttr = G_FALSE;
		while(oNode != nullptr){
			nextNode = (HTML_NODE*)GetNext(oNode);
			if (oNode->cgTagsType == HTML_Attribute){
				blHasAttr = G_TRUE;
				if (((tagName == "*") || (cgTagsName == tagName))
					&& ((attrName == "*") || (oNode->cgTagsName == attrName))
					&& ((attrValue == "*") || (oNode->cgTagsValue == attrValue))){
					blDel = G_TRUE;
				}
			}
			else if (oNode->cgTagsType == HTML_Tags){
				count += oNode->DelTagsIncludeContent(tagName,attrName,attrValue);
			}
			oNode = nextNode;
		};
		if (cgTagsType == HTML_Tags){
			if (blDel
				|| ((blHasAttr == G_FALSE) && ((tagName == "*") || (cgTagsName == tagName)) && (attrName == "*") && (attrValue == "*"))){
				++ count;
				MoveToTrash(this,this);
			}
		}
	}
	return(count);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif /* HTML_h */
