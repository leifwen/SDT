//
//  BIC_Dict.cpp
//  SDT
//
//  Created by Leif Wen on 25/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "stdafx.h"
#include "BIC_Dict.h"
//------------------------------------------------------------------------------------------//
#ifdef BIC_Dict_h
#include "SYS_File.h"
//------------------------------------------------------------------------------------------//
CMDID BIC_DICT_DO::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env,cgCommand,"Handle Html");
	if (B_ChkFLAG32(flag, CMD_blPrintSimple))
		return(cgCommandID);
	PrintHelpSubItem(env,"<-d>  <tag>"			,"Delete tags attribute");
	PrintHelpSubItem(env,"      <attr>"			,"Attribute");
	PrintHelpSubItem(env,"      <value>"		,"Attribute Value");
	PrintHelpSubItem(env,"<-db> <tag>"			,"Delete tags which has no content");
	PrintHelpSubItem(env,"<-dti><tag>"			,"Delete tags include content");
	PrintHelpSubItem(env,"      <attr>"			,"Attribute");
	PrintHelpSubItem(env,"      <value>"		,"Attribute Value");
	PrintHelpSubItem(env,"<-dte><tag>"			,"Delete tags exclude content");
	PrintHelpSubItem(env,"      <attr>"			,"Attribute");
	PrintHelpSubItem(env,"      <value>"		,"Attribute Value");
	
	PrintHelpSubItem(env,"<-s>  <tag>"			,"Set tags' attribute");
	PrintHelpSubItem(env,"      <attr>"			,"Attribute name");
	PrintHelpSubItem(env,"      <value>"		,"Attribute Value");
	PrintHelpSubItem(env,"      <nTag>"			,"New tag name");
	PrintHelpSubItem(env,"      <nAttr>"		,"New attribute name");
	PrintHelpSubItem(env,"      <nValue>"		,"New attribute Value");
	PrintHelpSubItem(env,"<-c>  <tag>"			,"Check tags' attribute");
	PrintHelpSubItem(env,"      <attr>"			,"Attribute name");
	PrintHelpSubItem(env,"      <value>"		,"Attribute Value");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_DICT_DO::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	STDSTR			par,par1,par2,par3,par4,par5,par6;
	DICTWORD_NODE	*dict = &BIC_DICT::GetDict();
	
	par6 = Str_Trim(msg);
	par = Str_SplitSubItem(&par6,' ');par6 = Str_LTrim(par6);
	par1 = Str_SplitSubItem(&par6,' ');par6 = Str_LTrim(par6);
	par2 = Str_SplitSubItem(&par6,' ');par6 = Str_LTrim(par6);
	par3 = Str_SplitSubItem(&par6,' ');par6 = Str_LTrim(par6);
	par4 = Str_SplitSubItem(&par6,' ');par6 = Str_LTrim(par6);
	par5 = Str_SplitSubItem(&par6,' ');par6 = Str_LTrim(par6);
	par6 = Str_SplitSubItem(&par6,' ');
	
	if (par == "-d"){
		PrintResult(env, "Delete items:", Str_ToStr(dict->DelAttribute(par1,par2,par3)));
	}
	else if (par == "-db"){
		PrintResult(env, "Delete items:", Str_ToStr(dict->DelTagsBlankContent(par1)));
	}
	else if (par == "-dte"){
		PrintResult(env, "Delete items:", Str_ToStr(dict->DelTagsExcludeContent(par1,par2,par3)));
	}
	else if (par == "-dti"){
		PrintResult(env, "Delete items:", Str_ToStr(dict->DelTagsIncludeContent(par1,par2,par3)));
	}
	else if (par == "-s"){
		PrintResult(env, "Set items:", Str_ToStr(dict->SetTags(par1,par2,par3,par4,par5,par6)));
	}
	else if (par == "-c"){
		PrintResult(env, "Check items:", Str_ToStr(dict->CheckTags(par1,par2,par3)));
	}
	else{
		PrintFail(env,"Input error");
		return(cgCommandID);
	}
	PrintSuccess(env);
	SYS_SleepMS(100);
	return(cgCommandID);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIC_DICT_READ::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env,cgCommand,"Read dict/coca data");
	if (B_ChkFLAG32(flag, CMD_blPrintSimple))
		return(cgCommandID);
	PrintHelpSubItem(env,"<dict|d|coca>"	,"Dict/compress dict/coca");
	PrintHelpSubItem(env,"<filename>"		,"File name");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_DICT_READ::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	DICT_THREAD		dictThread;
	DICTWORD_NODE	*dict = &BIC_DICT::GetDict();
	COCAWORD_NODE	*coca = &BIC_DICT::GetCoca();
	STDSTR			strPar,par1;
	
	par1 = Str_Trim(msg);
	strPar = Str_SplitSubItem(&par1,' ');par1 = Str_LTrim(par1);
	par1 = Str_SplitSubItem(&par1,' ');
	

	if (CFS_CheckFile(par1) > 0){
		if ((strPar == "d") || (strPar == "dict")){
			PrintResult(env, COL_clBlue, "Read dict file to Dict...");
			PrintResult(env, COL_clBlue, "FileName:",COL_clCyan, par1);
			PrintResult(env, COL_clBlue, "FileSize:",COL_clCyan, Str_ToStr(CFS_CheckFileSize(par1)).c_str());
			
			if (strPar == "d"){
				dictThread.Read(dict, par1,G_DICT_RM_COMPRESS_YES);
			}
			else{
				dictThread.Read(dict, par1,G_DICT_RM_COMPRESS_NO);
			}
			par1 = "";
			do{
				SYS_SleepMS(20);
				if (dict->GetLogArray().Get(&par1, -1).length() > 0){
					PrintStr(env, COL_clRed, par1);
					par1 = "";
				}
			}while(dictThread.IsTerminated() == 0);
			if (dict->GetLogArray().Get(&par1, -1).length() > 0)
				PrintStr(env, COL_clRed, &par1);
			
			PrintResult(env, COL_clBlue, "Items:",COL_clCyan, Str_ToStr(dictThread.ReadNum()).c_str());
		}
		else if (strPar == "coca"){
			PrintResult(env, "Read items:", Str_ToStr(coca->ReadFromFile(par1)));
		}
		else{
			PrintFail(env,"Input error");
			return(cgCommandID);
		}
		PrintSuccess(env);
	}
	else{
		PrintFail(env,"File does not exist");
	}
	return(cgCommandID);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIC_DICT_SAVE::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env,cgCommand,"Save dict");
	if (B_ChkFLAG32(flag, CMD_blPrintSimple))
		return(cgCommandID);
	PrintHelpSubItem(env,"<dict|d>"			,"Dict/compress dict");
	PrintHelpSubItem(env,"<filename>"		,"File name");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_DICT_SAVE::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	DICTWORD_NODE	*dict = &BIC_DICT::GetDict();
	STDSTR			strPar,par1;
	
	par1 = Str_Trim(msg);
	strPar = Str_SplitSubItem(&par1,' ');par1 = Str_LTrim(par1);
	par1 = Str_SplitSubItem(&par1,' ');
	
	if (strPar.length() > 0){
		if (strPar == "d"){
			PrintResult(env, COL_clCyan,"Save to file:",par1);
			dict->SaveCompressDict(par1);
		}
		else if (strPar == "dict"){
			PrintResult(env, COL_clCyan,"Save to file:",par1);
			dict->SaveDict(par1);
		}
		PrintSuccess(env);
	}
	else{
		PrintFail(env,"Input error");
	}
	SYS_SleepMS(100);
	return(cgCommandID);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIC_DICT_FIND::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env,cgCommand,"Find word in dict/coca");
	if (B_ChkFLAG32(flag, CMD_blPrintSimple))
		return(cgCommandID);
	PrintHelpSubItem(env,"<d|coca>"	,"Dict/coca");
	PrintHelpSubItem(env,"<word>"	,"Word");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_DICT_FIND::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	DICTWORD_NODE	*dict = &BIC_DICT::GetDict();
	COCAWORD_NODE	*coca = &BIC_DICT::GetCoca();
	STDSTR			strPar,par1;
	
	par1 = Str_Trim(msg);
	strPar = Str_SplitSubItem(&par1,' ');par1 = Str_LTrim(par1);
	par1 = Str_SplitSubItem(&par1,' ');
	
	if (strPar.length() > 0){
		if (strPar == "coca"){
			PrintResult(env, COL_clCyan,"Find in COCA...");
			TREE_RChain_Traversal_LINE_nolock(COCAWORD_NODE,coca,
				if (Str_LowerCase(_opNode->cgOWord) == par1){
					PrintResult(env, COL_clCyan, "Got");
					PrintResult(env, COL_clCyan, "Order number is:",Str_ToStr(_opNode->cgLineNumber));
					break;
				}
			);
		}
		else if (strPar == "d"){
			PrintResult(env, COL_clCyan,"Find in Dict...");
			TREE_RChain_Traversal_LINE_nolock(DICTWORD_NODE,dict,
				if (Str_LowerCase(_opNode->cgOWord) == par1){
					PrintResult(env, COL_clCyan, "Got");
					PrintResult(env, COL_clCyan, "Order number is:",Str_ToStr(_opNode->cgOrderNumber));
					PrintResult(env, COL_clCyan, &_opNode->cgOWord);
					PrintResult(env, COL_clCyan, &_opNode->cgHtmlContent.Compose(&par1,"",G_SPACE_ON));
					break;
				}
			);
		}
	}
	else{
		PrintFail(env,"Input error");
	}
	SYS_SleepMS(100);
	return(cgCommandID);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIC_DICT_UPDATE::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env,cgCommand,"Update explanation");
	if (B_ChkFLAG32(flag, CMD_blPrintSimple))
		return(cgCommandID);
	PrintHelpSubItem(env,"<d9|d8>"		,"Output file name.");
	PrintHelpSubItem(env,"<filename>"	,"Output file name.");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_DICT_UPDATE::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	DICTWORD_NODE	*dict = &BIC_DICT::GetDict();
	COCAWORD_NODE	*coca = &BIC_DICT::GetCoca();
	STDSTR			strPar,par1;
	
	par1 = Str_Trim(msg);
	strPar = Str_SplitSubItem(&par1,' ');par1 = Str_LTrim(par1);
	par1 = Str_SplitSubItem(&par1,' ');
	
	if (strPar.length() > 0){
		if (strPar == "d9"){
			PrintResult(env, COL_clBlue, "Update coca data...");
			coca->UpdateDict(dict, nullptr);
			PrintSuccess(env,"Finished update");
			
			PrintResult(env, COL_clCyan, "Write to file:",par1);
			coca->WriteToFile(par1);
		}
		else if (strPar == "d8"){
			PrintResult(env, COL_clBlue, "Update coca data...");
			coca->UpdateDict(nullptr, dict);
			PrintSuccess(env,"Finished update");
			
			PrintResult(env, COL_clCyan, "Write to file:",par1);
			coca->WriteToFile(par1);
		}
		else if (strPar == "d"){
			PrintResult(env, COL_clCyan, "Save to file:",par1);
			dict->SaveDict(par1);
		}
		PrintSuccess(env);
	}
	else{
		PrintFail(env,"Input error");
	}
	SYS_SleepMS(100);
	return(cgCommandID);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIC_DICT_EMPTY::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env,cgCommand,"Empty dict");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_DICT_EMPTY::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	BIC_DICT::Empty();
	PrintSuccess(env);
	return(cgCommandID);
}
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
BIC_DICT::BIC_DICT(void) : BIC_BASE_S(){
	cgCommandID = BIC_ID_DICT;
	cgCommand = "dict";
	cgConsoleName = cgCommand;
	cgHelpName = "Dict";

	Add(cgSub_read) < cgSub_save < cgSub_find < cgSub_update < cgSub_do < cgSub_empty;
};
//------------------------------------------------------------------------------------------//
BIC_DICT::~BIC_DICT(void){
	Empty();
}
//------------------------------------------------------------------------------------------//
void BIC_DICT::Empty(void){
	DICTWORD_NODE	*dict = &GetDict();
	COCAWORD_NODE	*coca = &GetCoca();
	
	{TREE_RChain_Traversal_LINE_nolock(DICTWORD_NODE,dict,_opNode->Empty(););}
	{TREE_RChain_Traversal_LINE_nolock(COCAWORD_NODE,coca,_opNode->Empty(););}
	
	DestroySubTree(dict);
	DestroySubTree(coca);
	CleanTrash(&HTML_NODE::GetTrashOwer());
};
//------------------------------------------------------------------------------------------//
DICTWORD_NODE& BIC_DICT::GetDict(void){
	static	DICTWORD_NODE	sgDict;
	return (sgDict);
};
//------------------------------------------------------------------------------------------//
COCAWORD_NODE& BIC_DICT::GetCoca(void){
	static	COCAWORD_NODE	sgCoca;
	return (sgCoca);
};
//------------------------------------------------------------------------------------------//
#endif /* BIC_Dict_h */
