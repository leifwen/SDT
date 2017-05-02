/*
 * Copyright (c) 2012-2014 
 * All rights reserved.
 *
 * File Name: CommandSlist.h
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2012.12.27
 *
*/

//------------------------------------------------------------------------------------------//
#include "Comm_Tree.h"
//------------------------------------------------------------------------------------------//
#ifdef Comm_TreeH
#ifndef CommandSlistH
#define CommandSlistH
#ifdef CommandSlistH
//------------------------------------------------------------------------------------------//
class SC_NODE : public TREE_NODE{
	private:
	public:
				 SC_NODE(void) : TREE_NODE(){Init();SetSelfName("SC_NODE");};
		virtual ~SC_NODE(void){;};
	public://need record
		STDSTR	StrCommand;
		int32	blEnableSendCR;//send "\r"
	public:
		void 	Init(void);
		STDSTR	&CreateNodeStrV0_2(STDSTR *retStr);
		STDSTR	&CreateNodeStrV0_3(STDSTR *retStr){return(CreateNodeStrV0_2(retStr));};
		STDSTR	&CreateNodeStrV0_4(STDSTR *retStr);
		STDSTR	&CreateNodeStrV0_5(STDSTR *retStr){return(CreateNodeStrV0_4(retStr));};
		STDSTR	&CreateNodeStrV0_6(STDSTR *retStr){return(CreateNodeStrV0_4(retStr));};
		void	SetNodeV0_2(STDSTR *strIn);
		void	SetNodeV0_3(STDSTR *strIn){SetNodeV0_2(strIn);};
		void	SetNodeV0_4(STDSTR *strIn);
		void	SetNodeV0_5(STDSTR *strIn){SetNodeV0_4(strIn);};
		void	SetNodeV0_6(STDSTR *strIn){SetNodeV0_4(strIn);};
	public:
		static void	Copy(SC_NODE *node1,const SC_NODE *node2);//copy 2 to 1
	public:
		static	STDSTR&	GetTitle(STDSTR *retStr);
				STDSTR&	Compose	(STDSTR *retStr);
};
//------------------------------------------------------------------------------------------//
class SC_LIST : public TREE_NODE{
	public:
				 SC_LIST(void){CreateTrash(this);SetSelfName("SC_LIST");};
		virtual ~SC_LIST(void){DestroyAll();};
	protected:
		inline	virtual	TNF*	CreateNode	(void){return(SetSubNodeSelfName(new SC_NODE));};
	public:
		inline	void		Empty		(void){CleanChild(this);};
		inline	SC_NODE*	GetNewNode	(void){return((SC_NODE*)TREE_NODE::GetNewNode(this));};
		STDSTR	&CreateSCListStrV0_2(STDSTR *retStr);
		STDSTR	&CreateSCListStrV0_3(STDSTR *retStr){return(CreateSCListStrV0_2(retStr));};
		STDSTR	&CreateSCListStrV0_4(STDSTR *retStr);
		STDSTR	&CreateSCListStrV0_5(STDSTR *retStr){return(CreateSCListStrV0_4(retStr));};
		STDSTR	&CreateSCListStrV0_6(STDSTR *retStr){return(CreateSCListStrV0_4(retStr));};
		void	SetSCListV0_2(STDSTR *strIn);
		void	SetSCListV0_3(STDSTR *strIn){SetSCListV0_2(strIn);};
		void	SetSCListV0_4(STDSTR *strIn);
		void	SetSCListV0_5(STDSTR *strIn){SetSCListV0_4(strIn);};
		void	SetSCListV0_6(STDSTR *strIn){SetSCListV0_4(strIn);};
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif
#endif
#endif
