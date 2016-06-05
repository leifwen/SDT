/*
 * Copyright (c) 2012-2014
 * All rights reserved.
 *
 * File Name: BIC_DICT.cpp
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2014.01.25
 
 */
//------------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "BIC_Dict.h"
#include "Comm_Convert.h"
#include "Comm_File.h"
#include "AppLayer.h"
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
HTML_NODE	HTML_NODE::sgSpareOwner = HTML_NODE();
FIFO_UINT8	HTML_NODE::sgLogFifo = FIFO_UINT8();
FIFO_UINT8	DICTWORD_NODE::sgLogFifo = FIFO_UINT8();
//------------------------------------------------------------------------------------------//
int32 HTML_NODE::IsTagSingle(void)const{
	std::string		tagsList;
	tagsList = HTML_SingleTagList;
	while(tagsList.length() > 0){
		if ((cgIsTagSingle > 0) || (cgTagsName == Str_ReadSubItem(&tagsList, "/")))
			return 1;
	};
	return 0;
}
//------------------------------------------------------------------------------------------//
const std::string &ComposeTagForLog(const HTML_NODE &tHtmlNode,std::string *strRet){
	*strRet = "[cgTagsName : ";
	tHtmlNode.ComposeTag(strRet);
	*strRet += " ]\n";
	return(*strRet);
}
//------------------------------------------------------------------------------------------//
int32 HTML_NODE::ResolveContent(FIFO_UINT8 *fifoIn){
	uint8	charData;
	std::string	strTag;
	int32	blSpace;

	cgTagsName = "";
	cgTagsValue = " ";
	cgTagsType = HTML_Content;
	blSpace = 0;
	charData = 0;
	while(!fifoIn->IsEmpty()){
		fifoIn->Get(&charData, 1);
		if (charData < 0x20)
			continue;
		if (charData == '<'){
			fifoIn->Out_RollBack(1);
			fifoIn->Update(&charData, 1,0);
			cgTagsValue = Str_Trim(cgTagsValue);
			return HTML_R_OK;
		}
		if (blSpace == 0){
			if (charData == ' ')
				blSpace = 1;
		}
		else{
			if (charData == ' ')
				continue;
			blSpace = 0;
		}
		cgTagsValue += charData;
	}
	sgLogFifo.PutInASCII("[E][cont] no complete." + ComposeTagForLog(*(HTML_NODE*)GetFather(this), &strTag),G_ESCAPE_OFF);
	return HTML_R_NoComplete;
}
//------------------------------------------------------------------------------------------//
int32 HTML_NODE::ResolveAttribute(FIFO_UINT8 *fifoIn){
	//cgTagsName="cgTagsValue"
	uint8	charData;
	int32	status,blRecSpace;
	std::string	strTag;
	
	
	cgTagsName = "";
	cgTagsValue = "";
	cgTagsType = HTML_Attribute;
	charData = 0;
	status = HTML_Wait_equal;
	blRecSpace = 0;
	while(!fifoIn->IsEmpty()){
		fifoIn->Get(&charData, 1);
		if (charData < 0x20)
			continue;
		if (charData == '<'){
			sgLogFifo.PutInASCII("[E][attr] receive '<'." + ComposeTagForLog(*(HTML_NODE*)GetFather(this), &strTag),G_ESCAPE_OFF);
			return HTML_R_Error;
		}
		if (charData == '>'){
			if (status == HTML_Wait_equal){
				fifoIn->Out_RollBack(1);
				fifoIn->Update(&charData, 1, 0);
				if (cgTagsName != "/"){
					sgLogFifo.PutInASCII("[W][attr] no receive cgTagsValue." + ComposeTagForLog(*(HTML_NODE*)GetFather(this), &strTag),G_ESCAPE_OFF);
					return HTML_R_NoRecValue;
				}
				return HTML_R_OK;
			}
			sgLogFifo.PutInASCII("[E][attr] receive '>' when decode cgTagsValue." + ComposeTagForLog(*(HTML_NODE*)GetFather(this), &strTag),G_ESCAPE_OFF);
			return HTML_R_Error;
		}
		switch (status){
			case HTML_Wait_equal:
				if (charData == '"'){
					sgLogFifo.PutInASCII("[E][attr] receive '\"' when decode cgTagsName." + ComposeTagForLog(*(HTML_NODE*)GetFather(this), &strTag),G_ESCAPE_OFF);
					return HTML_R_Error;
				}
				if (charData == '='){
					if (cgTagsName.length() == 0){
						sgLogFifo.PutInASCII("[E][attr] receive '=' but cgTagsName is blank." + ComposeTagForLog(*(HTML_NODE*)GetFather(this), &strTag),G_ESCAPE_OFF);
						return HTML_R_Error;
					}
					status = HTML_Wait_fdq;
				}
				else if (charData == ' '){
					if (cgTagsName.length() > 0)
						blRecSpace = 1;
				}
				else {
					if (blRecSpace == 1){
						fifoIn->Out_RollBack(1);
						fifoIn->Update(&charData, 1, 0);
						sgLogFifo.PutInASCII("[W][attr] no receive cgTagsValue." + ComposeTagForLog(*(HTML_NODE*)GetFather(this), &strTag),G_ESCAPE_OFF);
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
					sgLogFifo.PutInASCII("[E][attr] receive char between '=' and '\"'." + ComposeTagForLog(*(HTML_NODE*)GetFather(this), &strTag),G_ESCAPE_OFF);
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
	sgLogFifo.PutInASCII("[E][attr] no complete." + ComposeTagForLog(*(HTML_NODE*)GetFather(this), &strTag),G_ESCAPE_OFF);
	return HTML_R_NoComplete;
}
//------------------------------------------------------------------------------------------//
int32 HTML_NODE::ResolveTags(FIFO_UINT8 *fifoIn){
	uint8	charData;
	int32	status,ret;
	HTML_NODE	*newHtmlNode;
	std::string	strEnd,strTag;
	
	cgTagsName = "";
	cgTagsValue = "";
	cgTagsType = HTML_Tags;
	strEnd = "";
	charData = 0;
	status = HTML_Wait_fls;
	while(!fifoIn->IsEmpty()){
		fifoIn->Get(&charData, 1);
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
					sgLogFifo.PutInASCII("[E][tags] receive '<' or '\"' or '='." + ComposeTagForLog(*this, &strTag),G_ESCAPE_OFF);
					return HTML_R_Error;
				}
				if (charData == '>'){
					if (cgTagsName.length() == 0){
						cgTagsName = "";
						cgTagsValue = "&lt; &gt;";
						cgTagsType = HTML_Content;
						sgLogFifo.PutInASCII("[W][tags] receive '>' but cgTagsName is blank." + ComposeTagForLog(*this, &strTag),G_ESCAPE_OFF);
						sgLogFifo.PutInASCII("[W][tags] exchage '<>' to '&lt; &gt;'." + ComposeTagForLog(*this, &strTag),G_ESCAPE_OFF);
						return HTML_R_Exchange;
					}
					if (IsTagSingle() > 0)
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
					sgLogFifo.PutInASCII("[E][tags] receive '<'." + ComposeTagForLog(*this, &strTag),G_ESCAPE_OFF);
					return HTML_R_Error;
				}
				if (charData == '>'){
					newHtmlNode = (HTML_NODE*)GetLastChild(this);
					if ((newHtmlNode != nullptr) && (newHtmlNode->cgTagsName == "/")){
						cgIsTagSingle = 1;
						return HTML_R_OK;
					}
					if (IsTagSingle() > 0)
						return HTML_R_OK;
					status = HTML_Wait_nls;
				}
				else{
					fifoIn->Out_RollBack(1);
					fifoIn->Update(&charData, 1, 0);
					newHtmlNode = (HTML_NODE*)GetNewNode(&sgSpareOwner);
					newHtmlNode->Init();
					if (newHtmlNode == nullptr){
						sgLogFifo.PutInASCII("[E][tags] no enough memory." + ComposeTagForLog(*this, &strTag),G_ESCAPE_OFF);
						return HTML_R_NoResource;
					}
					this->AddNode(newHtmlNode);
					ret = newHtmlNode->ResolveAttribute(fifoIn);
					if (ret <= 0)
						return(ret);
				}
				break;
			case HTML_Wait_nls:
				if (charData == '<'){
					status = HTML_Wait_nmr;
				}
				else{
					fifoIn->Out_RollBack(1);
					fifoIn->Update(&charData, 1, 0);
					newHtmlNode = (HTML_NODE*)GetNewNode(&sgSpareOwner);
					if (newHtmlNode == nullptr){
						sgLogFifo.PutInASCII("[E][tags] no enough memory." + ComposeTagForLog(*this, &strTag),G_ESCAPE_OFF);
						return HTML_R_NoResource;
					}
					newHtmlNode->Init();
					AddNode(newHtmlNode);
					ret = newHtmlNode->ResolveContent(fifoIn);
					if (ret <= 0)
						return(ret);
					if (newHtmlNode->cgTagsValue.length() == 0)
						MoveNodeToTrash(newHtmlNode, &sgSpareOwner);
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
					fifoIn->Out_RollBack(1);
					fifoIn->Update(&charData, 1, 0);
					fifoIn->Out_RollBack(1);
					charData = '<';
					fifoIn->Update(&charData, 1, 0);
					newHtmlNode = (HTML_NODE*)GetNewNode(&sgSpareOwner);
					if (newHtmlNode == nullptr){
						sgLogFifo.PutInASCII("[E][tags] no enough memory." + ComposeTagForLog(*this, &strTag),G_ESCAPE_OFF);
						return HTML_R_NoResource;
					}
					newHtmlNode->Init();
					AddNode(newHtmlNode);
					ret = newHtmlNode->ResolveTags(fifoIn);
					if (ret <= 0)
						return(ret);
					status = HTML_Wait_nls;
				}
				break;
			case HTML_Wait_end:
				if ((charData == '<') || (charData == '"') || (charData == '=')){
					sgLogFifo.PutInASCII("[E][tags] receive '<' or '\"' or '=' when decode </> part." + ComposeTagForLog(*this, &strTag),G_ESCAPE_OFF);
					return HTML_R_Error;
				}
				if (charData == '>'){
					if (strEnd == cgTagsName)
						return HTML_R_OK;
					sgLogFifo.PutInASCII("[E][tags] miss match." + ComposeTagForLog(*this, &strTag),G_ESCAPE_OFF);
					return HTML_R_Error;
				}
				else{
					strEnd += charData;
				}
			default:;
		}
	}
	sgLogFifo.PutInASCII("[E][tags] no complete." + ComposeTagForLog(*this, &strTag),G_ESCAPE_OFF);
	return HTML_R_NoComplete;
}
//------------------------------------------------------------------------------------------//
const std::string &HTML_NODE::ComposeTag(std::string *strRet)const{
	HTML_NODE	*operateNode,*nextNode;

	if (cgTagsType == HTML_Tags){
		*strRet += '<';
		*strRet += cgTagsName;
		operateNode = (HTML_NODE*)GetFirstChild((HTML_NODE*)this);
		while(operateNode != nullptr){
			nextNode = (HTML_NODE*)RTREE_NODE::GetcgRChild(operateNode);
			if (operateNode->cgTagsType == HTML_Attribute){
				*strRet += ' ';
				*strRet += operateNode->cgTagsName;
				if (operateNode->cgTagsValue.length() > 0){
					*strRet += "=\"";
					*strRet += operateNode->cgTagsValue;
					*strRet += '"';
				}
			}
			operateNode = nextNode;
		};
		*strRet += '>';
	}
	return(*strRet);
}
//------------------------------------------------------------------------------------------//
const std::string &HTML_NODE::Compose(std::string *strRet,std::string strDeep,G_SPACE_VAILD blSpace)const{
	HTML_NODE	*operateNode,*nextNode;
	std::string strDeepNew;

	if (cgTagsType == HTML_Tags){
		if (blSpace == G_SPACE_ON){
			*strRet += strDeep;
			strDeepNew = strDeep + "  ";
		}
		*strRet += '<';
		*strRet += cgTagsName;
		operateNode = (HTML_NODE*)GetFirstChild((HTML_NODE*)this);
		while(operateNode != nullptr){
			nextNode = (HTML_NODE*)RTREE_NODE::GetcgRChild(operateNode);
			if (operateNode->cgTagsType == HTML_Attribute){
				*strRet += ' ';
				*strRet += operateNode->cgTagsName;
				if (operateNode->cgTagsValue.length() > 0){
					*strRet += "=\"";
					*strRet += operateNode->cgTagsValue;
					*strRet += '"';
				}
			}
			operateNode = nextNode;
		};
		if (blSpace == G_SPACE_ON){
			*strRet += ">\n";
		}
		else{
			*strRet += '>';
		}
	}
	if (IsTagSingle() == 0){
		operateNode = (HTML_NODE*)GetFirstChild((HTML_NODE*)this);
		while(operateNode != nullptr){
			nextNode = (HTML_NODE*)RTREE_NODE::GetcgRChild(operateNode);
			if (operateNode->cgTagsType == HTML_Tags){
				operateNode->Compose(strRet,strDeepNew,blSpace);
			}
			else if (operateNode->cgTagsType == HTML_Content){
				if (blSpace == G_SPACE_ON)
					*strRet += strDeepNew;
				*strRet += ' ';
				*strRet += operateNode->cgTagsValue;
				if (blSpace == G_SPACE_ON)
					*strRet += '\n';
			}
			operateNode = nextNode;
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
}
//------------------------------------------------------------------------------------------//
uint64 HTML_NODE::DelAttribute(const std::string &tagName,const std::string &attrName,const std::string &attrValue){
	HTML_NODE	*operateNode,*nextNode;
	uint64		count;
	int32		blHasAttr;

	count = 0;
	if ((cgTagsType == HTML_Tags) || (cgTagsType == HTML_None)){
		operateNode = (HTML_NODE*)GetFirstChild(this);
		blHasAttr = 0;
		while(operateNode != nullptr){
			nextNode = (HTML_NODE*)RTREE_NODE::GetcgRChild(operateNode);
			if (operateNode->cgTagsType == HTML_Attribute){
				blHasAttr = 1;
				if (((tagName == "*") || (cgTagsName == tagName))
					&& ((attrName == "*") || (operateNode->cgTagsName == attrName))
					&& ((attrValue == "*") || (operateNode->cgTagsValue == attrValue))){
					MoveNodeToTrash(operateNode, &sgSpareOwner);
					++ count;
				}
			}
			else if (operateNode->cgTagsType == HTML_Tags){
				count += operateNode->DelAttribute(tagName,attrName,attrValue);
			}
			operateNode = nextNode;
		};
		if ((blHasAttr == 0) && ((tagName == "*") || (cgTagsName == tagName)) && (attrName == "*") && (attrValue == "*")){
			MoveNodeToTrash(operateNode, &sgSpareOwner);
			++ count;
		}
	}
	return(count);
}
//------------------------------------------------------------------------------------------//
uint64 HTML_NODE::CheckTags(const std::string &tagName,const std::string &attrName,const std::string &attrValue){
	HTML_NODE	*operateNode,*nextNode;
	uint64		count;
	int32		blHasAttr;
	
	count = 0;
	if ((cgTagsType == HTML_Tags) || (cgTagsType == HTML_None)){
		operateNode = (HTML_NODE*)GetFirstChild(this);
		blHasAttr = 0;
		while(operateNode != nullptr){
			nextNode = (HTML_NODE*)RTREE_NODE::GetcgRChild(operateNode);
			if (operateNode->cgTagsType == HTML_Attribute){
				blHasAttr = 1;
				if (((tagName == "*") || (cgTagsName == tagName))
					&& ((attrName == "*") || (operateNode->cgTagsName == attrName))
					&& ((attrValue == "*") || (operateNode->cgTagsValue == attrValue))){
					++ count;
				}
			}
			else if (operateNode->cgTagsType == HTML_Tags){
				count += operateNode->CheckTags(tagName,attrName,attrValue);
			}
			operateNode = nextNode;
		};
		if ((blHasAttr == 0) && ((tagName == "*") || (cgTagsName == tagName)) && (attrName == "*") && (attrValue == "*"))
			++ count;
	}
	return(count);
}
//------------------------------------------------------------------------------------------//
uint64 HTML_NODE::SetTags(const std::string &tagName,const std::string &attrName,const std::string &attrValue
						  ,const std::string &newTagName,const std::string &newAttrName,const std::string &newAttrValue){
	HTML_NODE	*operateNode,*nextNode;
	uint64		count;
	int32		blHasAttr;
	
	count = 0;
	if ((cgTagsType == HTML_Tags) || (cgTagsType == HTML_None)){
		operateNode = (HTML_NODE*)GetFirstChild(this);
		blHasAttr = 0;
		while(operateNode != nullptr){
			nextNode = (HTML_NODE*)RTREE_NODE::GetcgRChild(operateNode);
			if (operateNode->cgTagsType == HTML_Attribute){
				blHasAttr = 1;
				if (((tagName == "*") || (cgTagsName == tagName))
					&& ((attrName == "*") || (operateNode->cgTagsName == attrName))
					&& ((attrValue == "*") || (operateNode->cgTagsValue == attrValue))){
					if (newTagName != "*")
						cgTagsName = newTagName;
					if (newAttrName != "*")
						operateNode->cgTagsName = newAttrName;
					if (newAttrValue != "*")
						operateNode->cgTagsValue = newAttrValue;
					++ count;
				}
			}
			else if (operateNode->cgTagsType == HTML_Tags){
				count += operateNode->SetTags(tagName,attrName,attrValue,newTagName,newAttrName,newAttrValue);
			}
			operateNode = nextNode;
		};
		if ((blHasAttr == 0) && ((tagName == "*") || (cgTagsName == tagName)) && (attrName == "*") && (attrValue == "*")){
			if (newTagName != "*")
				cgTagsName = newTagName;
			++ count;
		}
	}
	return(count);
}
//------------------------------------------------------------------------------------------//
uint64 HTML_NODE::DelTagsBlankContent(const std::string &tagName){
	HTML_NODE	*operateNode,*nextNode;
	uint64		count,delCount;
	int32		blContent,blTag;
	
	count = 0;
	if ((cgTagsType == HTML_Tags) || (cgTagsType == HTML_None)){
		if (IsTagSingle() > 0)
			return 0;
		do{
			operateNode = (HTML_NODE*)GetFirstChild(this);
			blContent = 0;
			blTag = 0;
			delCount = 0;
			while(operateNode != nullptr){
				nextNode = (HTML_NODE*)RTREE_NODE::GetcgRChild(operateNode);
				if (operateNode->cgTagsType == HTML_Content){
					blContent = 1;
				}
				else if (operateNode->cgTagsType == HTML_Tags){
					blTag = 1;
					delCount += operateNode->DelTagsBlankContent(tagName);
				}
				operateNode = nextNode;
			};
			count += delCount;
		}while(delCount > 0);
		if ((tagName == "*") || (cgTagsName == tagName)){
			if ((blContent == 0) && (blTag == 0)){
				++ count;
				DelTagExcludeConetent();
			}
		}
	}
	return(count);
}
//------------------------------------------------------------------------------------------//
void HTML_NODE::DelTagExcludeConetent(void){
	HTML_NODE	*operateNode,*nextNode;

	if ((cgTagsType == HTML_Tags) || (cgTagsType == HTML_None)){
		operateNode = (HTML_NODE*)GetFirstChild(this);
		while(operateNode != nullptr){
			nextNode = (HTML_NODE*)RTREE_NODE::GetcgRChild(operateNode);
			if (operateNode->cgTagsType == HTML_Attribute)
				MoveNodeToTrash(operateNode, &sgSpareOwner);
			operateNode = nextNode;
		};
		operateNode = (HTML_NODE*)BreakLChild(this);
		InsertRChild(this, operateNode);
		MoveNodeToTrash(this ,&sgSpareOwner);
	}
}
//------------------------------------------------------------------------------------------//
uint64 HTML_NODE::DelTagsExcludeConetent(const std::string &tagName,const std::string &attrName,const std::string &attrValue){
	HTML_NODE	*operateNode,*nextNode;
	uint64		count;
	int32		blDel,blHasAttr;

	count = 0;
	if ((cgTagsType == HTML_Tags) || (cgTagsType == HTML_None)){
		operateNode = (HTML_NODE*)GetFirstChild(this);
		blDel = 0;
		blHasAttr = 0;
		while(operateNode != nullptr){
			nextNode = (HTML_NODE*)RTREE_NODE::GetcgRChild(operateNode);
			if (operateNode->cgTagsType == HTML_Attribute){
				blHasAttr = 1;
				if (((tagName == "*") || (cgTagsName == tagName))
					&& ((attrName == "*") || (operateNode->cgTagsName == attrName))
					&& ((attrValue == "*") || (operateNode->cgTagsValue == attrValue))){
					blDel = 1;
				}
			}
			else if (operateNode->cgTagsType == HTML_Tags){
				count += operateNode->DelTagsExcludeConetent(tagName,attrName,attrValue);
			}
			operateNode = nextNode;
		};
		if (cgTagsType == HTML_Tags){
			if ((blDel != 0)
				|| ((blHasAttr == 0) && ((tagName == "*") || (cgTagsName == tagName)) && (attrName == "*") && (attrValue == "*"))){
				++ count;
				DelTagExcludeConetent();
			}
		}
	}
	return(count);
}
//------------------------------------------------------------------------------------------//
uint64 HTML_NODE::DelTagsIncludeConetent(const std::string &tagName,const std::string &attrName,const std::string &attrValue){
	HTML_NODE	*operateNode,*nextNode;
	uint64		count;
	int32		blDel,blHasAttr;
	
	count = 0;
	if ((cgTagsType == HTML_Tags) || (cgTagsType == HTML_None)){
		operateNode = (HTML_NODE*)GetFirstChild(this);
		blDel = 0;
		blHasAttr = 0;
		while(operateNode != nullptr){
			nextNode = (HTML_NODE*)RTREE_NODE::GetcgRChild(operateNode);
			if (operateNode->cgTagsType == HTML_Attribute){
				blHasAttr = 1;
				if (((tagName == "*") || (cgTagsName == tagName))
					&& ((attrName == "*") || (operateNode->cgTagsName == attrName))
					&& ((attrValue == "*") || (operateNode->cgTagsValue == attrValue))){
					blDel = 1;
				}
			}
			else if (operateNode->cgTagsType == HTML_Tags){
				count += operateNode->DelTagsIncludeConetent(tagName,attrName,attrValue);
			}
			operateNode = nextNode;
		};
		if (cgTagsType == HTML_Tags){
			if ((blDel != 0)
				|| ((blHasAttr == 0) && ((tagName == "*") || (cgTagsName == tagName)) && (attrName == "*") && (attrValue == "*"))){
				++ count;
				MoveNodeToTrash(this, &sgSpareOwner);
			}
		}
	}
	return(count);
}
//------------------------------------------------------------------------------------------//




//------------------------------------------------------------------------------------------//
void DICTWORD_NODE::Init(void){
	cgOWord = "";
	cgOWordLowerCase = "";
	cgOExplain = "";
	cgOrderNumber = 0;
	cgELength = 0;
	cgHtmlContent.Init();
}
//------------------------------------------------------------------------------------------//
int32 DICTWORD_NODE::ReadCompressDict(const std::string &fileName){
	std::fstream	fileStream;
	int32			readNum;
	uint64			num;
	uint8			buffer[1024 * 8];
	DICTWORD_NODE	*ndNode,*ldNode;
	
	fileStream.open(fileName.c_str(),std::ios::in|std::ios::binary);
	fileStream.clear();
	readNum = 0;
	ndNode = this;
	ldNode = ndNode;
	while(!fileStream.eof()){
		ndNode->cgOExplain = "";
		do{
			fileStream.getline((char*)buffer, sizeof(buffer));
			num = fileStream.gcount();
			Str_CharToStr(&ndNode->cgOExplain,buffer,num,0,G_ESCAPE_OFF,G_SPACE_OFF,G_Append_ON);
			if (fileStream.fail() && !fileStream.bad() && !fileStream.eof()){
				fileStream.clear();
				continue;
			}
			break;
		}while(1);
		if (ndNode->cgOExplain.length() > 0){
			++ readNum;
			Str_Trim(&ndNode->cgOWord,Str_ReadSubItem(&ndNode->cgOExplain, "\t"));
			if ((ndNode->cgOExplain.length() > 0) && (ndNode->cgOExplain[ndNode->cgOExplain.length()] == '\0'))
				ndNode->cgOExplain.erase(ndNode->cgOExplain.length() - 1);
			ndNode->cgOWordLowerCase = Str_LowerCase(ndNode->cgOWord);
			ndNode->cgOrderNumber = readNum;
			ndNode->cgELength = ndNode->cgOWord.length();
			
			if (ldNode->cgOWordLowerCase == ndNode->cgOWordLowerCase){
				ldNode->AddNode(ndNode);
			}
			else{
				InsertRChild(ldNode,ndNode);
				ldNode = ndNode;
			}
			if (!fileStream.eof()){
				ndNode = new(DICTWORD_NODE);
				if (ndNode == nullptr)
					return -1;
			}
		}
	};
	AddNode(ndNode);
	fileStream.close();
	return(readNum);
}
//------------------------------------------------------------------------------------------//
int32 DICTWORD_NODE::ReadDict(const std::string &fileName){
	//split by \b
	std::fstream	fileStream;
	uint64			num;
	int32			count,result;
	uint8			buffer[1024 * 8];
	HTML_NODE		*htmlNode;
	DICTWORD_NODE	*ndNode,*ldNode;
	FIFO_UINT8		fifoBuf;
	
	fifoBuf.Init(1024 * 1024 * 16);
	cgHtmlContent.sgLogFifo.Init(1024 * 1024);
	sgLogFifo.Init(1024 * 1024);

	fileStream.open(fileName.c_str(),std::ios::in|std::ios::binary);
	fileStream.clear();
	count = 0;
	ndNode = this;
	ldNode = ndNode;
	while(!fileStream.eof()){
		fifoBuf.Empty();
		fifoBuf.PutInASCII("<WORD>", G_ESCAPE_OFF);
		do{
			fileStream.getline((char*)buffer, sizeof(buffer),'\b');
			num = fileStream.gcount();
			fifoBuf.Put(buffer, (uint32)num);
			if (fileStream.fail() && !fileStream.bad() && !fileStream.eof()){
				fileStream.clear();
				continue;
			}
			if (!fileStream.eof())
				fifoBuf.In_RollBack(1);
			if (fifoBuf.Used() == sizeof("<WORD>"))
				continue;
			fifoBuf.PutInASCII("</WORD>", G_ESCAPE_OFF);
			break;
		}while(!fileStream.eof());
		if (fifoBuf.Used() > sizeof("<WORD></WORD>")){
			++ count;
			cgHtmlContent.sgLogFifo.Empty();

			result = ndNode->cgHtmlContent.ResolveTags(&fifoBuf);
			ndNode->cgHtmlContent.cgTagsType = HTML_NODE::HTML_None;
			htmlNode = (HTML_NODE*)GetFirstChild(&ndNode->cgHtmlContent);
			
			if (htmlNode != nullptr){
				ndNode->cgOWord = htmlNode->cgTagsValue;
				htmlNode->RemoveSelf();
				DestroyTree(htmlNode);
				htmlNode = nullptr;
				
				ndNode->cgOWordLowerCase = Str_LowerCase(ndNode->cgOWord);
				ndNode->cgOExplain = "";
				ndNode->cgHtmlContent.Compose(&ndNode->cgOExplain, "", G_SPACE_OFF);
				
				ndNode->cgOrderNumber = count;
				ndNode->cgELength = ndNode->cgOExplain.length();
				if (ldNode->cgOWordLowerCase == ndNode->cgOWordLowerCase){
					ldNode->AddNode(ndNode);
				}
				else{
					InsertRChild(ldNode,ndNode);
					ldNode = ndNode;
				}
				if (result <= 0){
					sgLogFifo.PutInASCII("[E][dict] read fail.[word : " + ndNode->cgOWord + " ]\n", G_ESCAPE_OFF);
					cgHtmlContent.sgLogFifo.Out(sgLogFifo.Put(cgHtmlContent.sgLogFifo, -1));
				}
				else{
					//sgLogFifo.PutInASCII("[O][dict] read new.[word : " + dictNode->cgOWord + " ]\n", G_ESCAPE_OFF);
					//cgHtmlContent.sgLogFifo.Out(sgLogFifo.Put(cgHtmlContent.sgLogFifo, -1));
				}
				if (!fileStream.eof()){
					ndNode = new(DICTWORD_NODE);
					if (ndNode == nullptr)
						return -1;
				}
			}
		}
	};
	AddNode(ndNode);
	fileStream.close();
	return(count);
}
//------------------------------------------------------------------------------------------//
int32 DICTWORD_NODE::SaveDict(const std::string &fileName){
	std::fstream	fileStream;
	std::string		strContent;
	DICTWORD_NODE	*operateNode,*nextNode;
	
	fileStream.open(fileName.c_str(),std::ios::out|std::ios::trunc|std::ios::binary);
	
	operateNode = this;
	strContent = "";
	while(operateNode != nullptr){
		nextNode = (DICTWORD_NODE*)RTREE_NODE::GetcgRChild(operateNode);
		strContent = '\b';
		strContent += operateNode->cgOWord;		strContent += "\n";
		operateNode->cgHtmlContent.Compose(&strContent,"",G_SPACE_ON);
		RTREE_LChildRChain_Traversal_LINE_nolock(DICTWORD_NODE,operateNode,
			strContent += '\b';
			strContent += operateNode_t->cgOWord;		strContent += "\n";
			operateNode_t->cgHtmlContent.Compose(&strContent,"",G_SPACE_ON);
		);
		fileStream << strContent;
		operateNode = nextNode;
	};
	fileStream.flush();
	fileStream.close();
	return 1;
}
//------------------------------------------------------------------------------------------//
int32 DICTWORD_NODE::SaveCompressDict(const std::string &fileName){
	std::fstream	fileStream;
	std::string		strContent;
	DICTWORD_NODE	*operateNode,*nextNode;
	
	fileStream.open(fileName.c_str(),std::ios::out|std::ios::trunc|std::ios::binary);
	
	operateNode = this;
	strContent = "";
	while(operateNode != nullptr){
		nextNode = (DICTWORD_NODE*)RTREE_NODE::GetcgRChild(operateNode);
		strContent = operateNode->cgOWord;		strContent += "\t";
		operateNode->cgHtmlContent.Compose(&strContent,"",G_SPACE_OFF);
		strContent += '\n';
		RTREE_LChildRChain_Traversal_LINE_nolock(DICTWORD_NODE,operateNode,
			strContent += operateNode_t->cgOWord;		strContent += "\t";
			operateNode_t->cgHtmlContent.Compose(&strContent,"",G_SPACE_OFF);
			strContent += '\n';
		);
		fileStream << strContent;
		operateNode = nextNode;
	};
	fileStream.flush();
	fileStream.close();
	return 1;
}
//------------------------------------------------------------------------------------------//
uint64 DICTWORD_NODE::CheckTags(const std::string &tagName,const std::string &attrName,const std::string &attrValue){
	DICTWORD_NODE	*operateNode,*nextNode;
	uint64	count;
	
	count = 0;
	operateNode = this;
	while(operateNode != nullptr){
		nextNode = (DICTWORD_NODE*)RTREE_NODE::GetcgRChild(operateNode);
		count += operateNode->cgHtmlContent.CheckTags(tagName, attrName, attrValue);
		RTREE_LChildRChain_Traversal_LINE_nolock(DICTWORD_NODE,operateNode,
			count += operateNode_t->cgHtmlContent.CheckTags(tagName, attrName, attrValue)
		);
		operateNode = nextNode;
	};
	return(count);
}
//------------------------------------------------------------------------------------------//
uint64 DICTWORD_NODE::SetTags(const std::string &tagName,const std::string &attrName,const std::string &attrValue
							  ,const std::string &newTagName,const std::string &newAttrName,const std::string &newAttrValue){
	DICTWORD_NODE	*operateNode,*nextNode;
	uint64	count;
	
	count = 0;
	operateNode = this;
	while(operateNode != nullptr){
		nextNode = (DICTWORD_NODE*)RTREE_NODE::GetcgRChild(operateNode);
		count += operateNode->cgHtmlContent.SetTags(tagName, attrName, attrValue, newTagName, newAttrName, newAttrValue);
		RTREE_LChildRChain_Traversal_LINE_nolock(DICTWORD_NODE,operateNode,
			count += operateNode_t->cgHtmlContent.SetTags(tagName, attrName, attrValue, newTagName, newAttrName, newAttrValue)
		);
		operateNode = nextNode;
	};
	return(count);
}
//------------------------------------------------------------------------------------------//
uint64 DICTWORD_NODE::DelAttribute(const std::string &tagName,const std::string &attrName,const std::string &attrValue){
	DICTWORD_NODE	*operateNode,*nextNode;
	uint64	count;
	
	count = 0;
	operateNode = this;
	while(operateNode != nullptr){
		nextNode = (DICTWORD_NODE*)RTREE_NODE::GetcgRChild(operateNode);
		count += operateNode->cgHtmlContent.DelAttribute(tagName, attrName, attrValue);
		RTREE_LChildRChain_Traversal_LINE_nolock(DICTWORD_NODE,operateNode,
			count += operateNode_t->cgHtmlContent.DelAttribute(tagName, attrName, attrValue)
		);
		operateNode = nextNode;
	};
	return(count);
}
//------------------------------------------------------------------------------------------//
uint64 DICTWORD_NODE::DelTagsBlankContent(const std::string &tagName){
	DICTWORD_NODE	*operateNode,*nextNode;
	uint64	count;
	
	count = 0;
	operateNode = this;
	while(operateNode != nullptr){
		nextNode = (DICTWORD_NODE*)RTREE_NODE::GetcgRChild(operateNode);
		count += operateNode->cgHtmlContent.DelTagsBlankContent(tagName);
		RTREE_LChildRChain_Traversal_LINE_nolock(DICTWORD_NODE,operateNode,
			count += operateNode_t->cgHtmlContent.DelTagsBlankContent(tagName)
		);
		operateNode = nextNode;
	};
	return(count);
}
//------------------------------------------------------------------------------------------//
uint64 DICTWORD_NODE::DelTagsExcludeConetent(const std::string &tagName,const std::string &attrName,const std::string &attrValue){
	DICTWORD_NODE	*operateNode,*nextNode;
	uint64	count;
	
	count = 0;
	operateNode = this;
	while(operateNode != nullptr){
		nextNode = (DICTWORD_NODE*)RTREE_NODE::GetcgRChild(operateNode);
		count += operateNode->cgHtmlContent.DelTagsExcludeConetent(tagName,attrName,attrValue);
		RTREE_LChildRChain_Traversal_LINE_nolock(DICTWORD_NODE,operateNode,
			count += operateNode_t->cgHtmlContent.DelTagsExcludeConetent(tagName,attrName,attrValue)
		);
		operateNode = nextNode;
	};
	return(count);
}
//------------------------------------------------------------------------------------------//
uint64 DICTWORD_NODE::DelTagsIncludeConetent(const std::string &tagName,const std::string &attrName,const std::string &attrValue){
	DICTWORD_NODE	*operateNode,*nextNode;
	uint64	count;
	
	count = 0;
	operateNode = this;
	while(operateNode != nullptr){
		nextNode = (DICTWORD_NODE*)RTREE_NODE::GetcgRChild(operateNode);
		count += operateNode->cgHtmlContent.DelTagsIncludeConetent(tagName,attrName,attrValue);
		RTREE_LChildRChain_Traversal_LINE_nolock(DICTWORD_NODE,operateNode,
			count += operateNode_t->cgHtmlContent.DelTagsIncludeConetent(tagName,attrName,attrValue)
		);
		operateNode = nextNode;
	};
	return(count);
}
//------------------------------------------------------------------------------------------//
int32 COCAWORD_NODE::ReadFromFile(const std::string &fileName){
	std::fstream	fileStream;
	int32			readNum;
	uint64			num;
	uint8			buffer[1024 * 8];
	COCAWORD_NODE	*newNode,*lastNode;
	
	fileStream.open(fileName.c_str(),std::ios::in|std::ios::binary);
	fileStream.clear();
	readNum = 0;
	newNode = this;
	lastNode = newNode;
	while(!fileStream.eof()){
		newNode->cgO8E = "";
		do{
			fileStream.getline((char*)buffer, sizeof(buffer));
			num = fileStream.gcount();
			Str_CharToStr(&newNode->cgO8E,buffer,num,0,G_ESCAPE_OFF,G_SPACE_OFF,G_Append_ON);
			if (fileStream.fail() && !fileStream.bad() && !fileStream.eof()){
				fileStream.clear();
				continue;
			}
			break;
		}while(1);
		if (newNode->cgO8E.length() > 0){
			++ readNum;
			Str_Trim(&newNode->cgOWord		,Str_ReadSubItem(&newNode->cgO8E, "\t"));
			Str_Trim(&newNode->cgO9W		,Str_ReadSubItem(&newNode->cgO8E, "\t"));
			Str_Trim(&newNode->cgO8W		,Str_ReadSubItem(&newNode->cgO8E, "\t"));
			Str_Trim(&newNode->cgOrder		,Str_ReadSubItem(&newNode->cgO8E, "\t"));
			Str_Trim(&newNode->cgAttribute	,Str_ReadSubItem(&newNode->cgO8E, "\t"));
			Str_Trim(&newNode->cgCStar		,Str_ReadSubItem(&newNode->cgO8E, "\t"));
			Str_Trim(&newNode->cgDeck		,Str_ReadSubItem(&newNode->cgO8E, "\t"));
			Str_Trim(&newNode->cgO9E		,Str_ReadSubItem(&newNode->cgO8E, "\t"));
			if ((newNode->cgO8E.length() > 0) && (newNode->cgO8E[newNode->cgO8E.length()] == '\0'))
				newNode->cgO8E.erase(newNode->cgO8E.length() - 1);

			newNode->cgLineNumber = readNum;
			InsertRChild(lastNode,newNode);
			lastNode = newNode;
			if (!fileStream.eof()){
				newNode = new(COCAWORD_NODE);
				if (newNode == nullptr)
					return -1;
			}
		}
	};
	AddNode(newNode);
	fileStream.close();
	return(readNum);
}
//------------------------------------------------------------------------------------------//
int32 COCAWORD_NODE::UpdateDict(DICTWORD_NODE *dict9,DICTWORD_NODE *dict8){
	std::string		strWord;
	COCAWORD_NODE	*operateNode,*nextNode;
	DICTWORD_NODE	*operateNode1,*nextNode1;
	//131072
	operateNode = this;
	while(operateNode != nullptr){
		nextNode = (COCAWORD_NODE*)RTREE_NODE::GetcgRChild(operateNode);
		if (operateNode->cgO9E == "Y"){
			Str_LowerCase(&strWord,operateNode->cgO9W);
			operateNode1 = dict9;
			while(operateNode1 != nullptr){
				nextNode1 = (DICTWORD_NODE*)RTREE_NODE::GetcgRChild(operateNode1);
				if (operateNode1->cgOWordLowerCase == strWord){
					operateNode->cgO9E = operateNode1->cgOExplain;
					RTREE_LChildRChain_Traversal_LINE_nolock(DICTWORD_NODE,operateNode1,operateNode->cgO9E += operateNode_t->cgOExplain;);
					break;
				}
				operateNode1 = nextNode1;
			};
		}
		if (operateNode->cgO9E == "B")
			operateNode->cgO9E = "";
		
		if (operateNode->cgO8E == "Y"){
			Str_LowerCase(&strWord,operateNode->cgO8W);
			operateNode1 = dict8;
			while(operateNode1 != nullptr){
				nextNode1 = (DICTWORD_NODE*)RTREE_NODE::GetcgRChild(operateNode1);
				if (operateNode1->cgOWordLowerCase == strWord){
					operateNode->cgO8E = operateNode1->cgOExplain;
					RTREE_LChildRChain_Traversal_LINE_nolock(DICTWORD_NODE,operateNode1,operateNode->cgO8E += operateNode_t->cgOExplain;);
					break;
				}
				operateNode1 = nextNode1;
			};
		}
		if (operateNode->cgO8E == "B")
			operateNode->cgO8E = "";
		operateNode = nextNode;
	};
	return 1;
}
//------------------------------------------------------------------------------------------//
int32 COCAWORD_NODE::WriteToFile(const std::string &fileName){
	std::fstream	fileStream;
	std::string		strContent;
	COCAWORD_NODE	*operateNode,*nextNode;
	
	strContent = "D9_" + fileName;
	fileStream.open(strContent.c_str(),std::ios::out|std::ios::trunc|std::ios::binary);
	
	operateNode = this;
	strContent = "";
	while(operateNode != nullptr){
		nextNode = (COCAWORD_NODE*)RTREE_NODE::GetcgRChild(operateNode);
		
		strContent = operateNode->cgOWord;		strContent += "\t";
		strContent += operateNode->cgOrder;		strContent += "\t";
		strContent += operateNode->cgAttribute;	strContent += "\t";
		strContent += operateNode->cgCStar;		strContent += "\t";
		strContent += operateNode->cgDeck;		strContent += "\t";
		if (operateNode->cgO9E.length() >= 131071){//anki limit
			strContent += "##Anki limit##";
		}
		else{
			strContent += operateNode->cgO9E;
		}
		strContent += "\t\n";
		fileStream << strContent;
		
		operateNode = nextNode;
	};
	fileStream.flush();
	fileStream.close();
	
	strContent = "D8_" + fileName;
	fileStream.open(strContent.c_str(),std::ios::out|std::ios::trunc|std::ios::binary);
	
	operateNode = this;
	strContent = "";
	while(operateNode != nullptr){
		nextNode = (COCAWORD_NODE*)RTREE_NODE::GetcgRChild(operateNode);
		
		strContent = operateNode->cgOWord;		strContent += "\t";
		strContent += operateNode->cgOrder;		strContent += "\t";
		strContent += operateNode->cgAttribute;	strContent += "\t";
		strContent += operateNode->cgCStar;		strContent += "\t";
		strContent += operateNode->cgDeck;		strContent += "\t";
		if (operateNode->cgO8E.length() >= 131071){//anki limit
			strContent += "##Anki_limit##";
		}
		else{
			strContent += operateNode->cgO8E;
		}
		strContent += "\t\n";
		fileStream << strContent;
		
		operateNode = nextNode;
	};
	fileStream.flush();
	fileStream.close();
	return 1;
}
//------------------------------------------------------------------------------------------//


















//------------------------------------------------------------------------------------------//
#ifdef SWVERSION_DICT
//------------------------------------------------------------------------------------------//
int32 BIC_DICT_DO::Help(BICPAR *tBICPAR,int32 blDetail)const{
	PrintHelpItem(tBICPAR,cgCommand,"Handle Html");
	if (blDetail == 0)
		return(cgReturnCode);
	PrintHelpItem(tBICPAR,"     <-d>  <tag>"			,"Delete tags attribute.");
	PrintHelpItem(tBICPAR,"           <attr>"			,"Attribute.");
	PrintHelpItem(tBICPAR,"           <value>"			,"Attribute Value.");
	PrintHelpItem(tBICPAR,"     <-db> <tag>"			,"Delete tags which has no content.");
	PrintHelpItem(tBICPAR,"     <-dti><tag>"			,"Delete tags include content.");
	PrintHelpItem(tBICPAR,"           <attr>"			,"Attribute.");
	PrintHelpItem(tBICPAR,"           <value>"			,"Attribute Value.");
	PrintHelpItem(tBICPAR,"     <-dte><tag>"			,"Delete tags exclude content.");
	PrintHelpItem(tBICPAR,"           <attr>"			,"Attribute.");
	PrintHelpItem(tBICPAR,"           <value>"			,"Attribute Value.");
	
	PrintHelpItem(tBICPAR,"     <-s>  <tag>"			,"Set tags' attribute.");
	PrintHelpItem(tBICPAR,"           <attr>"			,"Attribute name.");
	PrintHelpItem(tBICPAR,"           <value>"			,"Attribute Value.");
	PrintHelpItem(tBICPAR,"           <nTag>"			,"New tag name.");
	PrintHelpItem(tBICPAR,"           <nAttr>"			,"New attribute name.");
	PrintHelpItem(tBICPAR,"           <nValue>"			,"New attribute Value.");
	//PrintHelpItem(tBICPAR,"     <-st> <tage>"			,"Set tags' name.");
	//PrintHelpItem(tBICPAR,"           <newTag>"			,"New tag name.");
	
	PrintHelpItem(tBICPAR,"     <-c>  <tag>"			,"Check tags' attribute.");
	PrintHelpItem(tBICPAR,"           <attr>"			,"Attribute name.");
	PrintHelpItem(tBICPAR,"           <value>"			,"Attribute Value.");

	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_DICT_DO::Command(BICPAR *tBICPAR, const std::string &par,std::string *ret)const{
	std::string		strPar,par1,par2,par3,par4,par5,par6;
	DICTWORD_NODE	*dict;

	par6 = Str_Trim(par);
	strPar = Str_SplitSubItem(&par6,' ');par6 = Str_LTrim(par6);
	par1 = Str_SplitSubItem(&par6,' ');par6 = Str_LTrim(par6);
	par2 = Str_SplitSubItem(&par6,' ');par6 = Str_LTrim(par6);
	par3 = Str_SplitSubItem(&par6,' ');par6 = Str_LTrim(par6);
	par4 = Str_SplitSubItem(&par6,' ');par6 = Str_LTrim(par6);
	par5 = Str_SplitSubItem(&par6,' ');par6 = Str_LTrim(par6);
	par6 = Str_SplitSubItem(&par6,' ');

	dict = &tBICPAR->sdtApp->m_Dict;
	if (strPar == "-d"){
		PrintStrN(tBICPAR, "Delete items: " + Str_UInt64ToString(dict->DelAttribute(par1,par2,par3)), RICH_LIN_clCyan);
	}
	else if (strPar == "-db"){
		PrintStrN(tBICPAR, "Delete items: " + Str_UInt64ToString(dict->DelTagsBlankContent(par1)), RICH_LIN_clCyan);
	}
	else if (strPar == "-dte"){
		PrintStrN(tBICPAR, "Delete items: " + Str_UInt64ToString(dict->DelTagsExcludeConetent(par1,par2,par3)), RICH_LIN_clCyan);
	}
	else if (strPar == "-dti"){
		PrintStrN(tBICPAR, "Delete items: " + Str_UInt64ToString(dict->DelTagsIncludeConetent(par1,par2,par3)), RICH_LIN_clCyan);
	}
	else if (strPar == "-s"){
		PrintStrN(tBICPAR, "Set items: " + Str_UInt64ToString(dict->SetTags(par1,par2,par3,par4,par5,par6)), RICH_LIN_clCyan);
	}
	else if (strPar == "-c"){
		PrintStrN(tBICPAR, "Check items: " + Str_UInt64ToString(dict->CheckTags(par1,par2,par3)), RICH_LIN_clCyan);
	}
	else{
		PrintDoRet(tBICPAR,"Input error");
		return(cgReturnCode);
	}
	PrintStrN(tBICPAR, "Finish.", RICH_LIN_clCyan);
	SYS_SleepMS(100);
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_DICT::Help(BICPAR *tBICPAR, int32 blDetail)const{
	PrintHelpItem(tBICPAR, cgCommand, "->Dict.");
	if (blDetail == 0)
		return(cgReturnCode);
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_DICT_READ::Help(BICPAR *tBICPAR,int32 blDetail)const{
	PrintHelpItem(tBICPAR,cgCommand,"Read dict/coca data");
	if (blDetail == 0)
		return(cgReturnCode);
	PrintHelpItem(tBICPAR,"     <d|dict|coca>"	,"Compress dict/dict/coca.");
	PrintHelpItem(tBICPAR,"     <filename>"		,"File name.");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_DICT_READ::Command(BICPAR *tBICPAR, const std::string &par,std::string *ret)const{
	DICT_THREAD		dictThread;
	DICTWORD_NODE	*dict;
	COCAWORD_NODE	*coca;
	std::string		strPar,par1;
	
	par1 = Str_Trim(par);
	strPar = Str_SplitSubItem(&par1,' ');par1 = Str_LTrim(par1);
	par1 = Str_SplitSubItem(&par1,' ');
	
	dict = &tBICPAR->sdtApp->m_Dict;
	coca = &tBICPAR->sdtApp->m_COCA;
	if (CFS_CheckFile(par1) > 0){
		if ((strPar == "d") || (strPar == "dict")){
			PrintStrN(tBICPAR, "Read dict file to Dict...", RICH_LIN_clBlue);
			PrintStrN(tBICPAR, "FileName: ", RICH_LIN_clBlue);
			PrintStr(tBICPAR, par1, RICH_LIN_clCyan);
			PrintStrN(tBICPAR, "FileSize: ", RICH_LIN_clBlue);
			PrintStr(tBICPAR, Str_Int64ToString(CFS_CheckFileSize(par1)), RICH_LIN_clCyan);
			PrintStrN(tBICPAR, "", RICH_LIN_clBlue);
			
			if (strPar == "d"){
				dictThread.Init(dict, par1,G_DICT_RM_COMPRESS_YES);
			}
			else{
				dictThread.Init(dict, par1,G_DICT_RM_COMPRESS_NO);
			}
			dictThread.Read();
			par1 = "";
			do{
				SYS_SleepMS(20);
				if (dict->sgLogFifo.GetInASCII(&par1, -1) > 0){
					PrintStr(tBICPAR, par1, RICH_LIN_clRed);
					par1 = "";
				}
			}while(dictThread.CheckReading() > 0);
			if (dict->sgLogFifo.GetInASCII(&par1, -1) > 0)
				PrintStr(tBICPAR, par1, RICH_LIN_clRed);
			
			PrintStrN(tBICPAR, "Items: ", RICH_LIN_clBlue);
			PrintStr(tBICPAR, Str_Int64ToString(dictThread.ReadNum()), RICH_LIN_clCyan);
			PrintStrN(tBICPAR, "Finish.", RICH_LIN_clBlue);
		}
		else if (strPar == "coca"){
			PrintStrN(tBICPAR, "Read items: " + Str_UInt64ToString(coca->ReadFromFile(par1)), RICH_LIN_clCyan);
		}
		else{
			PrintDoRet(tBICPAR,"Input error");
			return(cgReturnCode);
		}
		PrintStrN(tBICPAR, "Finish.", RICH_LIN_clCyan);
	}
	else{
		PrintDoRet(tBICPAR,"File does not exist");
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_DICT_SAVE::Help(BICPAR *tBICPAR,int32 blDetail)const{
	PrintHelpItem(tBICPAR,cgCommand,"Save dict");
	if (blDetail == 0)
		return(cgReturnCode);
	PrintHelpItem(tBICPAR,"     <d|dict>"		,"Compress dict/dict.");
	PrintHelpItem(tBICPAR,"     <filename>"		,"File name.");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_DICT_SAVE::Command(BICPAR *tBICPAR, const std::string &par,std::string *ret)const{
	DICTWORD_NODE	*dict;
	std::string		strPar,par1;
	
	par1 = Str_Trim(par);
	strPar = Str_SplitSubItem(&par1,' ');par1 = Str_LTrim(par1);
	par1 = Str_SplitSubItem(&par1,' ');
	
	dict = &tBICPAR->sdtApp->m_Dict;
	if (strPar.length() > 0){
		if (strPar == "d"){
			PrintStrN(tBICPAR, "Save to file:" + par1, RICH_LIN_clCyan);
			dict->SaveCompressDict(par1);
			PrintStrN(tBICPAR, "Finish.", RICH_LIN_clCyan);
		}
		else if (strPar == "dict"){
			PrintStrN(tBICPAR, "Save to file:" + par1, RICH_LIN_clCyan);
			dict->SaveDict(par1);
			PrintStrN(tBICPAR, "Finish.", RICH_LIN_clCyan);
		}
	}
	else{
		PrintDoRet(tBICPAR,"Input error");
	}
	SYS_SleepMS(100);
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_DICT_FIND::Help(BICPAR *tBICPAR,int32 blDetail)const{
	PrintHelpItem(tBICPAR,cgCommand,"Find word in dict/coca");
	if (blDetail == 0)
		return(cgReturnCode);
	PrintHelpItem(tBICPAR,"     <d|coca><word>"			,"Search in Dict.");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_DICT_FIND::Command(BICPAR *tBICPAR, const std::string &par,std::string *ret)const{
	DICTWORD_NODE	*dict;
	COCAWORD_NODE	*coca;
	std::string		strPar,par1;
	
	par1 = Str_Trim(par);
	strPar = Str_SplitSubItem(&par1,' ');par1 = Str_LTrim(par1);
	par1 = Str_SplitSubItem(&par1,' ');
	
	dict = &tBICPAR->sdtApp->m_Dict;
	coca = &tBICPAR->sdtApp->m_COCA;
	if (strPar.length() > 0){
		if (strPar == "coca"){
			PrintStrN(tBICPAR, "Find in COCA...", RICH_LIN_clBlue);
			RTREE_RChain_Traversal_LINE_nolock(COCAWORD_NODE,coca,
				if (Str_LowerCase(operateNode_t->cgOWord) == par1){
					PrintStrN(tBICPAR, "Got.\nOrder number is:" + Str_IntToString(operateNode_t->cgLineNumber), RICH_LIN_clCyan);
					break;
				}
			);
		}
		else if (strPar == "d"){
			PrintStrN(tBICPAR, "Find in Dict...", RICH_LIN_clBlue);
			RTREE_RChain_Traversal_LINE_nolock(DICTWORD_NODE,dict,
				if (Str_LowerCase(operateNode_t->cgOWord) == par1){
					PrintStrN(tBICPAR, "Got.\nOrder number is:" + Str_IntToString(operateNode_t->cgOrderNumber), RICH_LIN_clCyan);
					par1 = operateNode_t->cgOWord;
					par1 += "\n";
					operateNode_t->cgHtmlContent.Compose(&par1,"",G_SPACE_ON);
					PrintStrN(tBICPAR, par1, RICH_LIN_clCyan);
					break;
				}
			);
		}
	}
	else{
		PrintDoRet(tBICPAR,"Input error");
	}
	SYS_SleepMS(100);
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_DICT_UPDATE::Help(BICPAR *tBICPAR,int32 blDetail)const{
	PrintHelpItem(tBICPAR,cgCommand,"Update explanation");
	if (blDetail == 0)
		return(cgReturnCode);
	PrintHelpItem(tBICPAR,"     <d9|d8>"				,"Output file name.");
	PrintHelpItem(tBICPAR,"     <filename>"				,"Output file name.");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_DICT_UPDATE::Command(BICPAR *tBICPAR, const std::string &par,std::string *ret)const{
	DICTWORD_NODE	*dict;
	COCAWORD_NODE	*coca;
	std::string		strPar,par1;
	
	par1 = Str_Trim(par);
	strPar = Str_SplitSubItem(&par1,' ');par1 = Str_LTrim(par1);
	par1 = Str_SplitSubItem(&par1,' ');
	
	dict = &tBICPAR->sdtApp->m_Dict;
	coca = &tBICPAR->sdtApp->m_COCA;
	if (strPar.length() > 0){
		if (strPar == "d9"){
			PrintStrN(tBICPAR, "Update coca data...", RICH_LIN_clBlue);
			coca->UpdateDict(dict, nullptr);
			PrintStrN(tBICPAR, "Update completed.", RICH_LIN_clCyan);
			
			PrintStrN(tBICPAR, "Write to file:" + par1, RICH_LIN_clCyan);
			coca->WriteToFile(par1);
			PrintStrN(tBICPAR, "Finish.", RICH_LIN_clCyan);
		}
		else if (strPar == "d8"){
			PrintStrN(tBICPAR, "Update coca data...", RICH_LIN_clBlue);
			coca->UpdateDict(nullptr, dict);
			PrintStrN(tBICPAR, "Update completed.", RICH_LIN_clCyan);
			
			PrintStrN(tBICPAR, "Write to file:" + par1, RICH_LIN_clCyan);
			coca->WriteToFile(par1);
			PrintStrN(tBICPAR, "Finish.", RICH_LIN_clCyan);
		}
		else if (strPar == "d"){
			PrintStrN(tBICPAR, "Save to file:" + par1, RICH_LIN_clCyan);
			dict->SaveDict(par1);
			PrintStrN(tBICPAR, "Finish.", RICH_LIN_clCyan);
		}
	}
	else{
		PrintDoRet(tBICPAR,"Input error");
	}
	SYS_SleepMS(100);
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
#endif
