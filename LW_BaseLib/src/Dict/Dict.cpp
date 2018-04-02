//
//  Dict.cpp
//  SDT
//
//  Created by Leif Wen on 25/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "stdafx.h"
#include "Dict.h"
#ifdef Dict_h
//------------------------------------------------------------------------------------------//
#include <fstream>
#include "DS_STRING.h"
//------------------------------------------------------------------------------------------//
DICTWORD_NODE::DICTWORD_NODE(void) : TNFP(){
	Init();
};
//------------------------------------------------------------------------------------------//
DICTWORD_NODE::~DICTWORD_NODE(void){
	Empty();
};
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
ARRAY& DICTWORD_NODE::GetLogArray(void){
	static ARRAY sgLogArray;
	return(sgLogArray);
};
//------------------------------------------------------------------------------------------//
void DICTWORD_NODE::Empty(void){
	TNF *oNode,*next;
	
	CleanChild(&cgHtmlContent, &cgHtmlContent);
	oNode = BreakChild(this);
	
	next = oNode;
	while(next != nullptr){
		CleanChild(&((DICTWORD_NODE*)next)->cgHtmlContent, &((DICTWORD_NODE*)next)->cgHtmlContent);
		next = GetNext(InsertAfter_nolock(next,BreakChild_nolock(next),G_FALSE));
	};

	MoveNodesToTrash(this, oNode, GetHead(GetTail(oNode)));
	CleanTrash(this);
};
//------------------------------------------------------------------------------------------//
bool32 DICTWORD_NODE::ReadCompressDict(const STDSTR& fileName){
	std::fstream	fileStream;
	bool32			readNum;
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
			Str_CharToStr(&ndNode->cgOExplain,buffer,num,G_ASCII,G_SPACE_OFF,G_ESCAPE_OFF);
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
				InsertAfter_nolock(ldNode,ndNode);
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
bool32 DICTWORD_NODE::ReadDict(const STDSTR& fileName){
	//split by '\b'
	std::fstream	fileStream;
	uint64			num;
	int32			count,result;
	uint8			buffer[1024 * 8];
	HTML_NODE		*htmlNode;
	DICTWORD_NODE	*ndNode,*ldNode;
	ARRAY			array;
	
	array.InitSize(1024 * 1024 * 16);
	cgHtmlContent.GetLogArray().InitSize(1024 * 1024);
	GetLogArray().InitSize(1024 * 1024);
	
	fileStream.open(fileName.c_str(),std::ios::in|std::ios::binary);
	fileStream.clear();
	count = 0;
	ndNode = this;
	ldNode = ndNode;
	while(!fileStream.eof()){
		array.Empty();
		array.Put("<WORD>", G_ESCAPE_OFF);
		do{
			fileStream.getline((char*)buffer, sizeof(buffer),'\b');
			num = fileStream.gcount();
			array.Put(buffer, (uint32)num);
			if (fileStream.fail() && !fileStream.bad() && !fileStream.eof()){
				fileStream.clear();
				continue;
			}
			if (!fileStream.eof())
				array.In_RollBack(1);//remove '\b'
			if (array.Used() == sizeof("<WORD>"))
				continue;
			array.Put("</WORD>", G_ESCAPE_OFF);
			break;
		}while(!fileStream.eof());
		if (array.Used() > sizeof("<WORD></WORD>")){
			++ count;
			cgHtmlContent.GetLogArray().Empty();
			
			result = ndNode->cgHtmlContent.Resolve(&array);
			ndNode->cgHtmlContent.cgTagsType = HTML_NODE::HTML_None;
			htmlNode = (HTML_NODE*)GetDown(&ndNode->cgHtmlContent);
			
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
					InsertAfter_nolock(ldNode,ndNode);
					ldNode = ndNode;
				}
				if (result <= 0){
					GetLogArray().Put("[E][dict] read fail.[word : " + ndNode->cgOWord + " ]\n", G_ESCAPE_OFF);
					cgHtmlContent.GetLogArray().Out(GetLogArray().Put(cgHtmlContent.GetLogArray(), -1));
				}
				else{
					//GetLogArray()("[O][dict] read new.[word : " + ndNode->cgOWord + " ]\n", G_ESCAPE_OFF);
					//cgHtmlContent.GetLogArray().Out(GetLogArray().Put(cgHtmlContent.GetLogArray(), -1));
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
bool32 DICTWORD_NODE::SaveDict(const STDSTR& fileName){
	std::fstream	fileStream;
	STDSTR			strContent;
	DICTWORD_NODE	*oNode,*nextNode;
	
	fileStream.open(fileName.c_str(),std::ios::out|std::ios::trunc|std::ios::binary);
	
	oNode = this;
	strContent = "";
	while(oNode != nullptr){
		nextNode = (DICTWORD_NODE*)GetNext(oNode);
		strContent = '\b';
		strContent += oNode->cgOWord;
		strContent += "\n";
		oNode->cgHtmlContent.Compose(&strContent,"",G_SPACE_ON);
		TREE_LChildRChain_Traversal_LINE_nolock(DICTWORD_NODE,oNode,
			strContent += '\b';
			strContent += _opNode->cgOWord;
			strContent += "\n";
			_opNode->cgHtmlContent.Compose(&strContent,"",G_SPACE_ON);
		);
		fileStream << strContent;
		oNode = nextNode;
	};
	fileStream.flush();
	fileStream.close();
	return G_TRUE;
}
//------------------------------------------------------------------------------------------//
bool32 DICTWORD_NODE::SaveCompressDict(const STDSTR& fileName){
	std::fstream	fileStream;
	STDSTR			strContent;
	DICTWORD_NODE	*oNode,*nextNode;
	
	fileStream.open(fileName.c_str(),std::ios::out|std::ios::trunc|std::ios::binary);
	
	oNode = this;
	strContent = "";
	while(oNode != nullptr){
		nextNode = (DICTWORD_NODE*)GetNext(oNode);
		strContent = oNode->cgOWord;
		strContent += "\t";
		oNode->cgHtmlContent.Compose(&strContent,"",G_SPACE_OFF);
		strContent += '\n';
		TREE_LChildRChain_Traversal_LINE_nolock(DICTWORD_NODE, oNode,
			strContent += _opNode->cgOWord;
			strContent += "\t";
			_opNode->cgHtmlContent.Compose(&strContent,"",G_SPACE_OFF);
			strContent += '\n';
		);
		fileStream << strContent;
		oNode = nextNode;
	};
	fileStream.flush();
	fileStream.close();
	return G_TRUE;
}
//------------------------------------------------------------------------------------------//
uint64 DICTWORD_NODE::CheckTags(const STDSTR& tagName,const STDSTR& attrName,const STDSTR& attrValue){
	DICTWORD_NODE	*oNode,*nextNode;
	uint64	count;
	
	count = 0;
	oNode = this;
	while(oNode != nullptr){
		nextNode = (DICTWORD_NODE*)GetNext(oNode);
		count += oNode->cgHtmlContent.CheckTags(tagName, attrName, attrValue);
		TREE_LChildRChain_Traversal_LINE_nolock(DICTWORD_NODE,oNode,
			count += _opNode->cgHtmlContent.CheckTags(tagName, attrName, attrValue)
		);
		oNode = nextNode;
	};
	return(count);
}
//------------------------------------------------------------------------------------------//
uint64 DICTWORD_NODE::SetTags(const STDSTR & tagName,const STDSTR & attrName,const STDSTR & attrValue
							  ,const STDSTR & newTagName,const STDSTR & newAttrName,const STDSTR & newAttrValue){
	DICTWORD_NODE	*oNode,*nextNode;
	uint64	count;
	
	count = 0;
	oNode = this;
	while(oNode != nullptr){
		nextNode = (DICTWORD_NODE*)GetNext(oNode);
		count += oNode->cgHtmlContent.SetTags(tagName, attrName, attrValue, newTagName, newAttrName, newAttrValue);
		TREE_LChildRChain_Traversal_LINE_nolock(DICTWORD_NODE,oNode,
			count += _opNode->cgHtmlContent.SetTags(tagName, attrName, attrValue, newTagName, newAttrName, newAttrValue)
		);
		oNode = nextNode;
	};
	return(count);
}
//------------------------------------------------------------------------------------------//
uint64 DICTWORD_NODE::DelAttribute(const STDSTR& tagName,const STDSTR& attrName,const STDSTR& attrValue){
	DICTWORD_NODE	*oNode,*nextNode;
	uint64	count;
	
	count = 0;
	oNode = this;
	while(oNode != nullptr){
		nextNode = (DICTWORD_NODE*)GetNext(oNode);
		count += oNode->cgHtmlContent.DelAttribute(tagName, attrName, attrValue);
		TREE_LChildRChain_Traversal_LINE_nolock(DICTWORD_NODE,oNode,
			count += _opNode->cgHtmlContent.DelAttribute(tagName, attrName, attrValue)
		);
		oNode = nextNode;
	};
	return(count);
}
//------------------------------------------------------------------------------------------//
uint64 DICTWORD_NODE::DelTagsBlankContent(const STDSTR& tagName){
	DICTWORD_NODE	*oNode,*nextNode;
	uint64	count;
	
	count = 0;
	oNode = this;
	while(oNode != nullptr){
		nextNode = (DICTWORD_NODE*)GetNext(oNode);
		count += oNode->cgHtmlContent.DelTagsBlankContent(tagName);
		TREE_LChildRChain_Traversal_LINE_nolock(DICTWORD_NODE,oNode,
			count += _opNode->cgHtmlContent.DelTagsBlankContent(tagName)
		);
		oNode = nextNode;
	};
	return(count);
}
//------------------------------------------------------------------------------------------//
uint64 DICTWORD_NODE::DelTagsExcludeContent(const STDSTR& tagName,const STDSTR& attrName,const STDSTR& attrValue){
	DICTWORD_NODE	*oNode,*nextNode;
	uint64	count;
	
	count = 0;
	oNode = this;
	while(oNode != nullptr){
		nextNode = (DICTWORD_NODE*)GetNext(oNode);
		count += oNode->cgHtmlContent.DelTagsExcludeContent(tagName,attrName,attrValue);
		TREE_LChildRChain_Traversal_LINE_nolock(DICTWORD_NODE,oNode,
			count += _opNode->cgHtmlContent.DelTagsExcludeContent(tagName,attrName,attrValue)
		);
		oNode = nextNode;
	};
	return(count);
}
//------------------------------------------------------------------------------------------//
uint64 DICTWORD_NODE::DelTagsIncludeContent(const STDSTR& tagName,const STDSTR& attrName,const STDSTR& attrValue){
	DICTWORD_NODE	*oNode,*nextNode;
	uint64	count;
	
	count = 0;
	oNode = this;
	while(oNode != nullptr){
		nextNode = (DICTWORD_NODE*)GetNext(oNode);
		count += oNode->cgHtmlContent.DelTagsIncludeContent(tagName,attrName,attrValue);
		TREE_LChildRChain_Traversal_LINE_nolock(DICTWORD_NODE,oNode,
			count += _opNode->cgHtmlContent.DelTagsIncludeContent(tagName,attrName,attrValue)
		);
		oNode = nextNode;
	};
	return(count);
}
//------------------------------------------------------------------------------------------//











//------------------------------------------------------------------------------------------//
DICT_THREAD::DICT_THREAD(void) : SYS_AThread(){
	cgDict = nullptr;
};
//------------------------------------------------------------------------------------------//
void DICT_THREAD::Execute(void* exep){
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
//------------------------------------------------------------------------------------------//
const uint32& DICT_THREAD::ReadNum(void){
	
	return(cgReadNum);
};
//------------------------------------------------------------------------------------------//
void DICT_THREAD::Read(DICTWORD_NODE* dict,const STDSTR& filename,G_DICT_RM blCompose){
	cgDict = dict;
	cgFilename = filename;
	cgblCompress = blCompose;
	ThreadRun();
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
COCAWORD_NODE::COCAWORD_NODE(void) : TNFP(){
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
//------------------------------------------------------------------------------------------//
COCAWORD_NODE::~COCAWORD_NODE(void){
	Empty();
};
//------------------------------------------------------------------------------------------//
void COCAWORD_NODE::Empty(void){
	TNF *oNode,*next;
	
	oNode = BreakChild(this);
	next = oNode;
	while(next != nullptr)
		next = GetNext(InsertAfter_nolock(next,BreakChild_nolock(next),G_FALSE));
	
	MoveNodesToTrash(this, oNode, GetHead(GetTail(next)));
	CleanTrash(this);
};
//------------------------------------------------------------------------------------------//
int32 COCAWORD_NODE::ReadFromFile(const STDSTR& fileName){
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
			Str_CharToStr(&newNode->cgO8E,buffer,num,G_ASCII,G_SPACE_OFF,G_ESCAPE_OFF);
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
			InsertAfter_nolock(lastNode,newNode);
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
bool32 COCAWORD_NODE::WriteToFile(const STDSTR& fileName){
	std::fstream	fileStream;
	STDSTR			strContent;
	COCAWORD_NODE	*oNode,*nextNode;
	
	strContent = "D9_" + fileName;
	fileStream.open(strContent.c_str(),std::ios::out|std::ios::trunc|std::ios::binary);
	
	oNode = this;
	strContent = "";
	while(oNode != nullptr){
		nextNode = (COCAWORD_NODE*)GetNext(oNode);
		
		strContent = oNode->cgOWord;		strContent += "\t";
		strContent += oNode->cgOrder;		strContent += "\t";
		strContent += oNode->cgAttribute;	strContent += "\t";
		strContent += oNode->cgCStar;		strContent += "\t";
		strContent += oNode->cgDeck;		strContent += "\t";
		if (oNode->cgO9E.length() >= 131071){//anki limit
			strContent += "##Anki limit##";
		}
		else{
			strContent += oNode->cgO9E;
		}
		strContent += "\t\n";
		fileStream << strContent;
		
		oNode = nextNode;
	};
	fileStream.flush();
	fileStream.close();
	
	strContent = "D8_" + fileName;
	fileStream.open(strContent.c_str(),std::ios::out|std::ios::trunc|std::ios::binary);
	
	oNode = this;
	strContent = "";
	while(oNode != nullptr){
		nextNode = (COCAWORD_NODE*)GetNext(oNode);
		
		strContent = oNode->cgOWord;		strContent += "\t";
		strContent += oNode->cgOrder;		strContent += "\t";
		strContent += oNode->cgAttribute;	strContent += "\t";
		strContent += oNode->cgCStar;		strContent += "\t";
		strContent += oNode->cgDeck;		strContent += "\t";
		if (oNode->cgO8E.length() >= 131071){//anki limit
			strContent += "##Anki_limit##";
		}
		else{
			strContent += oNode->cgO8E;
		}
		strContent += "\t\n";
		fileStream << strContent;
		
		oNode = nextNode;
	};
	fileStream.flush();
	fileStream.close();
	return G_TRUE;
}
//------------------------------------------------------------------------------------------//
int32 COCAWORD_NODE::UpdateDict(DICTWORD_NODE* dict9,DICTWORD_NODE* dict8){
	STDSTR			strWord;
	COCAWORD_NODE	*oNode,*nextNode;
	DICTWORD_NODE	*oNode1,*nextNode1;
	//131072
	oNode = this;
	while(oNode != nullptr){
		nextNode = (COCAWORD_NODE*)GetNext(oNode);
		if (oNode->cgO9E == "Y"){
			Str_LowerCase(&strWord,oNode->cgO9W);
			oNode1 = dict9;
			while(oNode1 != nullptr){
				nextNode1 = (DICTWORD_NODE*)GetNext(oNode1);
				if (oNode1->cgOWordLowerCase == strWord){
					oNode->cgO9E = oNode1->cgOExplain;
					TREE_LChildRChain_Traversal_LINE_nolock(DICTWORD_NODE,oNode1,oNode->cgO9E += _opNode->cgOExplain;);
					break;
				}
				oNode1 = nextNode1;
			};
		}
		if (oNode->cgO9E == "B")
			oNode->cgO9E = "";
		
		if (oNode->cgO8E == "Y"){
			Str_LowerCase(&strWord,oNode->cgO8W);
			oNode1 = dict8;
			while(oNode1 != nullptr){
				nextNode1 = (DICTWORD_NODE*)GetNext(oNode1);
				if (oNode1->cgOWordLowerCase == strWord){
					oNode->cgO8E = oNode1->cgOExplain;
					TREE_LChildRChain_Traversal_LINE_nolock(DICTWORD_NODE,oNode1,oNode->cgO8E += _opNode->cgOExplain;);
					break;
				}
				oNode1 = nextNode1;
			};
		}
		if (oNode->cgO8E == "B")
			oNode->cgO8E = "";
		oNode = nextNode;
	};
	return G_TRUE;
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif /* Dict_h */
