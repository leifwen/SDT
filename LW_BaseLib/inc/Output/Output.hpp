//
//  Output.hpp
//  SDT
//
//  Created by Leif Wen on 30/01/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

//------------------------------------------------------------------------------------------//
#ifndef Output_hpp
#define Output_hpp
//------------------------------------------------------------------------------------------//
#include "Output.h"
#ifdef Output_h
#include "SYS_Time.h"
//------------------------------------------------------------------------------------------//
#ifndef DIVIDING_LINE
#define	DIVIDING_LINE	"----------------------------------------------------------------------\n"
#endif
//------------------------------------------------------------------------------------------//
inline void OUTPUT_CACHE::Start(void){
	outCacheThread.ThreadRun();
};
//------------------------------------------------------------------------------------------//
inline void OUTPUT_CACHE::Stop(void){
	outCacheThread.ThreadStop();
};
//------------------------------------------------------------------------------------------//









//------------------------------------------------------------------------------------------//
inline void			OUTPUT_NODE::SetDestoryByCache	(void)				{SetSFlag(ON_blDestoryByCache);};
inline bool32		OUTPUT_NODE::IsDestoryByCache	(void)const			{return(CheckSFlag(ON_blDestoryByCache));};
inline void 		OUTPUT_NODE::SetCOlType			(COLType colType)	{cgColType = colType;};
//------------------------------------------------------------------------------------------//
inline void OUTPUT_NODE::UnregisterChild(OUTPUT_NODE* son){
	if (son != nullptr)
		son->SetAddress(nullptr,0,0,0);
	DetachUpPriorNext(son,son);
};
//------------------------------------------------------------------------------------------//
inline void OUTPUT_NODE::Print(uint32 ctrl,COLORENUM col,const uint8* data,uint32 num)	{;};
//------------------------------------------------------------------------------------------//
inline void OUTPUT_NODE::DoCleanLastLine(void){;};
//------------------------------------------------------------------------------------------//
inline void OUTPUT_NODE::CleanLastLine(OUTPUT_NODE* node){
	if (node != nullptr){
		node->DoCleanLastLine();
	}
};
//------------------------------------------------------------------------------------------//
static inline OUTPUT_NODE& DoPrintStr(OUTPUT_NODE* node){return(*node);};
//------------------------------------------------------------------------------------------//
static inline OUTPUT_NODE& DoPrintStr(OUTPUT_NODE* node,const _ColData& first){
	return(*node << first);
};
//------------------------------------------------------------------------------------------//
template<typename T>static inline OUTPUT_NODE& DoPrintStr(OUTPUT_NODE* node,const T& first){
	if (node->GetUVInLength(first) > 0)
		*node << " ";
	return(*node << first);
};
//------------------------------------------------------------------------------------------//
template<typename T,typename... Args>static inline OUTPUT_NODE& DoPrintStr(OUTPUT_NODE* node,const T& first, const Args&... args){
	DoPrintStr(node,first);
	return(DoPrintStr(node,args...));
};
//------------------------------------------------------------------------------------------//
template<typename T,typename... Args> void OUTPUT_NODE::PrintStr(OUTPUT_NODE* node,const T& first,const Args&... args){
	if (node != nullptr){
		*node << Begin()
		<< COL_NormalMessage << first;
		
		DoPrintStr(node,args...)
		<< Endl();
	}
};
//------------------------------------------------------------------------------------------//
template<typename T,typename... Args> void OUTPUT_NODE::PrintStrNL(OUTPUT_NODE* node,const T& first,const Args&... args){
	if (node != nullptr){
		*node << Begin() << NL()
		<< COL_NormalMessage << first;
		
		DoPrintStr(node,args...)
		<< Endl();
	}
};
//------------------------------------------------------------------------------------------//
template<typename T,typename... Args> void OUTPUT_NODE::PrintALine(OUTPUT_NODE* node,const T& first,const Args&... args){
	if (node != nullptr){
		*node << Begin() << NL()
		<< COL_NormalMessage << first;
		
		DoPrintStr(node,args...)
		<< NL() << Endl();
	}
};
//------------------------------------------------------------------------------------------//
template<typename T,typename... Args> void OUTPUT_NODE::PrintALineDot(OUTPUT_NODE* node,const T& first,const Args&... args){
	if (node != nullptr){
		*node << Begin() << NL()
		<< COL_NormalMessage << first;
		
		DoPrintStr(node,args...)
		<< COL_NormalMessage << ".\n"
		<< Endl();
	}
};
//------------------------------------------------------------------------------------------//
template<typename... Args> void OUTPUT_NODE::PrintALineDot(OUTPUT_NODE* node,const _ColData& first,const Args&... args){
	if (node != nullptr){
		*node << Begin() << NL()
		<< first;
		
		DoPrintStr(node,args...)
		<< ".\n"
		<< Endl();
	}
};
//------------------------------------------------------------------------------------------//
template<typename... Args> void OUTPUT_NODE::PrintALineDot(OUTPUT_NODE* node,const COLORENUM& col,const Args&... args){
	if (node != nullptr){
		*node << Begin() << NL();
		*node << col;
		
		DoPrintStr(node,args...)
		<< ".\n"
		<< Endl();
	}
};
//------------------------------------------------------------------------------------------//
template<typename... Args> void OUTPUT_NODE::PrintWithTime_noNL(OUTPUT_NODE* node,const Args&... args){
	if (node != nullptr){
		*node << Begin()
		<< COL_Time_DCyan
		<< COL_NormalMessage;
		
		DoPrintStr(node,args...)
		<< Endl();
	}
};
//------------------------------------------------------------------------------------------//
template<typename... Args> void OUTPUT_NODE::PrintWithTime(OUTPUT_NODE* node,const Args&... args){
	if (node != nullptr){
		*node << Begin() << NL()
		<< COL_Time_DCyan
		<< COL_NormalMessage;
		
		DoPrintStr(node,args...)
		<< Endl();
	}
};
//------------------------------------------------------------------------------------------//
template<typename... Args> void OUTPUT_NODE::PrintWithDividingLine(OUTPUT_NODE* node,const Args&... args){
	if (node != nullptr){
		*node << Begin() << NL()
		<< COL_DivLineTime
		<< COL_NormalMessage;
		
		DoPrintStr(node,args...)
		<< NL() << Endl();
	}
};
//------------------------------------------------------------------------------------------//
template<typename... Args> void OUTPUT_NODE::PrintWithDividingLineDot(OUTPUT_NODE* node,const Args&... args){
	if (node != nullptr){
		*node << Begin() << NL()
		<< COL_DivLineTime
		<< COL_NormalMessage;
		
		DoPrintStr(node,args...)
		<< COL_NormalMessage << ".\n"
		<< Endl();
	}
};
//------------------------------------------------------------------------------------------//
template<typename... Args> void OUTPUT_NODE::PrintWithDividingLineDot(OUTPUT_NODE* node,const _ColData& first,const Args&... args){
	if (node != nullptr){
		*node << Begin() << NL()
		<< COL_DivLineTime
		<< first;
		
		DoPrintStr(node,args...)
		<< ".\n"
		<< Endl();
	}
};
//------------------------------------------------------------------------------------------//
template<typename... Args> void OUTPUT_NODE::PrintWithDividingLineDot(OUTPUT_NODE* node,const COLORENUM& col,const Args&... args){
	if (node != nullptr){
		*node << Begin() << NL()
		<< COL_DivLineTime
		<< col;
		
		DoPrintStr(node,args...)
		<< ".\n"
		<< Endl();
	}
};
//------------------------------------------------------------------------------------------//
template<typename... Args> void OUTPUT_NODE::PrintMessageDot(OUTPUT_NODE* node,const Args&... args){
	if (node != nullptr){
		*node << Begin() << DefGroup() << NL()
		<< COL_DivLineTime
		<< COL_NormalMessage;
		
		DoPrintStr(node,args...)
		<< COL_NormalMessage << ".\n"
		<< COL_DivLine_Maroon
		<< Endl();
	}
};
//------------------------------------------------------------------------------------------//
template<typename... Args> void OUTPUT_NODE::PrintMessageDot(OUTPUT_NODE* node,const _ColData& first,const Args&... args){
	if (node != nullptr){
		*node << Begin() << DefGroup() << NL()
		<< COL_DivLineTime
		<< first;
		
		DoPrintStr(node,args...)
		<< ".\n"
		<< COL_DivLine_Maroon
		<< Endl();
	}
};
//------------------------------------------------------------------------------------------//
template<typename... Args> void OUTPUT_NODE::PrintMessageDot(OUTPUT_NODE* node,const COLORENUM& col,const Args&... args){
	if (node != nullptr){
		*node << Begin() << DefGroup() << NL()
		<< COL_DivLineTime
		<< col;
		
		DoPrintStr(node,args...)
		<< ".\n"
		<< COL_DivLine_Maroon
		<< Endl();
	}
};
//------------------------------------------------------------------------------------------//
template<typename T,typename... Args> void OUTPUT_NODE::PrintWarningMessageDot(OUTPUT_NODE* node,const T& first,const Args&... args){
	if (node != nullptr){
		*node << Begin() << DefGroup() << NL()
		<< COL_DivLineTime
		<< COL_WarningMessage1
		<< first
		<< COL_WarningMessage2;

		DoPrintStr(node,args...)
		<< COL_WarningMessage1 << ".\n"
		<< COL_DivLine_Maroon
		<< Endl();
	}
};
//------------------------------------------------------------------------------------------//
template<typename... Args> void OUTPUT_NODE::PrintWarningMessageDot(OUTPUT_NODE* node,const _ColData& first,const Args&... args){
	PrintWarningMessageDot(node,first.uvin,args...);
};
//------------------------------------------------------------------------------------------//
template<typename... Args> void OUTPUT_NODE::PrintWarningMessageDot(OUTPUT_NODE* node,const COLORENUM& col,const Args&... args){
	PrintWarningMessageDot(node,args...);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif /* Output_h */
#endif /* Output_hpp */
