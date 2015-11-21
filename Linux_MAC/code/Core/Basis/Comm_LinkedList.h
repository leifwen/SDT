/*
 * Copyright (c) 2012-2014 
 * All rights reserved.
 *
 * File Name: Comm_LinkedList.h
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2013.12.02
*/
#ifndef Comm_LinkedListH
#define Comm_LinkedListH
//------------------------------------------------------------------------------------------//
#include "BasicDef.h"
#include "BasicClass.h"
#include "SYS_Lock.h"
#if 0
//------------------------------------------------------------------------------------------//
template <typename TYPE_LINKED> class	LINKEDLIST;
//------------------------------------------------------------------------------------------//
template <typename TYPE_LINKED>
class	LINKEDLIST_NODE : public BASIC_CFLAG{
	public:
		enum{RFLAG_C = 1, RFLAG_S = BASIC_CFLAG::RFLAG_S + BASIC_CFLAG::RFLAG_C};
	public:
				 LINKEDLIST_NODE(void);
		virtual	~LINKEDLIST_NODE(void){RemoveSelfFromList();};
	private:
		SYS_ThreadLock	cgLLNCS;
		static 	uint32	cgNextNodeID;			//the ID of next the same class in system.
	public:
		void	Node_Lock	(void){cgLLNCS.Lock();};
		void	Node_Unlock	(void){cgLLNCS.Unlock();};
	public:
		LINKEDLIST_NODE<TYPE_LINKED>	*next;
		LINKEDLIST_NODE<TYPE_LINKED>	*prior;
		LINKEDLIST<TYPE_LINKED>			*fLinkedList;
		uint32							linkedID;
		uint32							nodeID;//static ID,set by self
	    uint32							dNodeID;//dynamic ID,set by list class
	public:
				int32	RemoveSelfFromList	(void);
		virtual	void	SetblSelected		(void)		{SetSFlag(RFLAG_CREATE(0));};
		virtual	void	ClrblSelected		(void)		{ClrSFlag(RFLAG_CREATE(0));};
		virtual	int32	CheckblSelected		(void)const	{return(CheckSFlag(RFLAG_CREATE(0)));};
};
//------------------------------------------------------------------------------------------//
template <typename TYPE_LINKED> uint32 LINKEDLIST_NODE<TYPE_LINKED>::cgNextNodeID = 0;
//------------------------------------------------------------------------------------------//
template <typename TYPE_LINKED>
LINKEDLIST_NODE<TYPE_LINKED>::LINKEDLIST_NODE(void) : BASIC_CFLAG(){
	linkedID = 0;
	next = this;
	prior = this;
	fLinkedList = nullptr;
	++ cgNextNodeID;
	nodeID = cgNextNodeID;
    dNodeID = nodeID;
}
//------------------------------------------------------------------------------------------//
template <typename TYPE_LINKED>
int32 LINKEDLIST_NODE<TYPE_LINKED>::RemoveSelfFromList(void){
    dNodeID = cgNextNodeID;
	if (linkedID == 0)
		return 1;
	if (fLinkedList == nullptr)
		return 1;
	return(fLinkedList->Remove(this));
}
//------------------------------------------------------------------------------------------//







//------------------------------------------------------------------------------------------//
#define	LINKEDLIST_Traversal_T(TYPE_LINKED,LINKEDLIST_NODE_FUN)\
{\
	TYPE_LINKED		*tHead,*tNext;\
	LINKEDLIST<TYPE_LINKED>::List_Lock();\
	if (BASIC_CFLAG::CheckblEnabled() != 0){\
		if (LINKEDLIST<TYPE_LINKED>::cgHead != nullptr){\
			tHead = (TYPE_LINKED*)LINKEDLIST<TYPE_LINKED>::cgHead;\
			tNext = (TYPE_LINKED*)LINKEDLIST<TYPE_LINKED>::cgHead;\
			do{\
				tNext->LINKEDLIST_NODE_FUN;\
				tNext = (TYPE_LINKED*)tNext->next;\
			}while(tNext != tHead);\
		}\
	}\
	LINKEDLIST<TYPE_LINKED>::List_Unlock();\
};
//------------------------------------------------------------------------------------------//
#define	LINKEDLIST_Traversal(TYPE_LINKED,LINKEDLIST_NODE_FUN)\
{\
	TYPE_LINKED		*tHead,*tNext;\
	List_Lock();\
	if (CheckblEnabled() != 0){\
		if (cgHead != nullptr){\
			tHead = (TYPE_LINKED*)cgHead;\
			tNext = (TYPE_LINKED*)cgHead;\
			do{\
				tNext->LINKEDLIST_NODE_FUN;\
				tNext = (TYPE_LINKED*)tNext->next;\
			}while(tNext != tHead);\
		}\
	}\
	List_Unlock();\
};

//------------------------------------------------------------------------------------------//
#define	LINKEDLIST_Run_Select_R(TYPE_LINKED,LINKEDLIST_NODE_FUN,TYPE_RET)\
{\
	TYPE_LINKED		*tSelect;\
	TYPE_RET		ret;\
	ret = 0;\
	List_Lock();\
	tSelect = (TYPE_LINKED*)GetSelected();\
	if (tSelect != nullptr)\
		ret = tSelect->LINKEDLIST_NODE_FUN;\
	List_Unlock();\
	return(ret);\
};
//------------------------------------------------------------------------------------------//
#define	LINKEDLIST_Run_Select(TYPE_LINKED,LINKEDLIST_NODE_FUN)\
{\
	TYPE_LINKED		*tSelect;\
	List_Lock();\
	tSelect = (TYPE_LINKED*)GetSelected();\
	if (tSelect != nullptr)\
		tSelect->LINKEDLIST_NODE_FUN;\
	List_Unlock();\
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
template <typename TYPE_LINKED>
class	LINKEDLIST : public BASIC_CFLAG{
	public:
		enum{RFLAG_C = 0, RFLAG_S = BASIC_CFLAG::RFLAG_S + BASIC_CFLAG::RFLAG_C};
	public:
				 LINKEDLIST	(void);
		virtual	~LINKEDLIST	(void){SetEmpty();};
	private:
		SYS_ThreadLock		cgLLCS;
		static 	uint32	cgNextLinkedID;		//the ID of next the same class in system.
		uint32			cgSelfLinkedID;
	    uint32			cgNextDNodeID;
	protected:
//		mutable	int32	internalResult;		//remove 2014/05/26
		LINKEDLIST_NODE<TYPE_LINKED>		*cgHead;
	public:
		void	List_Lock		(void){cgLLCS.Lock();};
		void	List_Unlock		(void){cgLLCS.Unlock();};
	public:
		void	LSetblSelected	(void){LINKEDLIST_Traversal(TYPE_LINKED,SetblSelected());};
		void	LClrblSelected	(void){LINKEDLIST_Traversal(TYPE_LINKED,ClrblSelected());};
	public:
		int32	Add				(LINKEDLIST_NODE<TYPE_LINKED> *tListItem){return(Insert(tListItem));};
		int32	Insert			(LINKEDLIST_NODE<TYPE_LINKED> *tInsertItem,LINKEDLIST_NODE<TYPE_LINKED> *tLocationItem = nullptr,int32 blAfter = 1);
		int32	Remove			(LINKEDLIST_NODE<TYPE_LINKED> *tFirstItem,LINKEDLIST_NODE<TYPE_LINKED> *tEndItem = nullptr);
		int32	MoveUp			(LINKEDLIST_NODE<TYPE_LINKED> *tListItem,int32 blDoCycle = 0);
		int32	MoveDown		(LINKEDLIST_NODE<TYPE_LINKED> *tListItem,int32 blDoCycle = 0);
        void	SetEmpty		(void){Remove(cgHead,GetTail());};
		LINKEDLIST_NODE<TYPE_LINKED>	*GetHead(void)const{return(cgHead);};
		LINKEDLIST_NODE<TYPE_LINKED>	*GetTail(void)const;
		LINKEDLIST_NODE<TYPE_LINKED>	*GetNext(LINKEDLIST_NODE<TYPE_LINKED> *itemNode)const;
		LINKEDLIST_NODE<TYPE_LINKED>	*GetPrior(LINKEDLIST_NODE<TYPE_LINKED> *itemNode)const;
		LINKEDLIST_NODE<TYPE_LINKED>	*GetSelected(void);
		LINKEDLIST_NODE<TYPE_LINKED>	*FindByNodeID(uint32 tNodeID);
	    LINKEDLIST_NODE<TYPE_LINKED>	*FindByDNodeID(uint32 tDNodeID);
	    LINKEDLIST_NODE<TYPE_LINKED>	*SetSelectByNodeID(uint32 tNodeID);
		LINKEDLIST_NODE<TYPE_LINKED>	*SetSelectByDNodeID(uint32 tDNodeID);
};
//------------------------------------------------------------------------------------------//
template <typename TYPE_LINKED> uint32 LINKEDLIST<TYPE_LINKED>::cgNextLinkedID = 0;
//------------------------------------------------------------------------------------------//
template <typename TYPE_LINKED>
LINKEDLIST<TYPE_LINKED>::LINKEDLIST(void) : BASIC_CFLAG(){
	cgHead = nullptr;
	++ cgNextLinkedID;
	cgSelfLinkedID = cgNextLinkedID;
    cgNextDNodeID = 0;
}
//------------------------------------------------------------------------------------------//
template <typename TYPE_LINKED>
LINKEDLIST_NODE<TYPE_LINKED>* LINKEDLIST<TYPE_LINKED>::GetTail(void)const{
	if (cgHead == nullptr)
		return(nullptr);
	return(cgHead->prior);
}
//------------------------------------------------------------------------------------------//
template <typename TYPE_LINKED>
LINKEDLIST_NODE<TYPE_LINKED>* LINKEDLIST<TYPE_LINKED>::GetNext(LINKEDLIST_NODE<TYPE_LINKED> *itemNode)const{
	if (itemNode == nullptr)
		return(nullptr);
	if (itemNode == cgHead->prior)
		return(nullptr);
	return(itemNode->next);
}
//------------------------------------------------------------------------------------------//
template <typename TYPE_LINKED>
LINKEDLIST_NODE<TYPE_LINKED>* LINKEDLIST<TYPE_LINKED>::GetPrior(LINKEDLIST_NODE<TYPE_LINKED> *itemNode)const{
	if (itemNode == nullptr)
		return(nullptr);
	if (itemNode == cgHead)
		return(nullptr);
	return(itemNode->prior);
}
//------------------------------------------------------------------------------------------//
template <typename TYPE_LINKED>
int32 LINKEDLIST<TYPE_LINKED>::Insert(LINKEDLIST_NODE<TYPE_LINKED> *tInsertItem
									  ,LINKEDLIST_NODE<TYPE_LINKED> *tLocationItem
									  ,int32 blAfter){
	LINKEDLIST_NODE<TYPE_LINKED> *firstItem,*endItem;

	if (tInsertItem == nullptr)
		return 0;
	if (tInsertItem->linkedID != 0)
		return 0;

    if ((tLocationItem != nullptr) && (tLocationItem->linkedID != cgSelfLinkedID))
        tLocationItem = nullptr;
        
	cgLLCS.Lock();

	firstItem = tInsertItem;
	endItem = tInsertItem->prior;

	if (cgHead == nullptr){
		cgHead = firstItem;
	}
	else{
		if (tLocationItem == nullptr){
			tLocationItem = cgHead;
			blAfter = 0;
		}
		if (blAfter == 0){//before
			endItem->next = tLocationItem;
			firstItem->prior = tLocationItem->prior;
			tLocationItem->prior = endItem;
			firstItem->prior->next = firstItem;
		}
		else{//after
			endItem->next = tLocationItem->next;
			firstItem->prior = tLocationItem;
			tLocationItem->next = firstItem;
			endItem->next->prior = endItem;
		}
	}
    if (firstItem == endItem){
        ++ cgNextDNodeID;
        firstItem->linkedID = cgSelfLinkedID;
        firstItem->fLinkedList = this;
        firstItem->dNodeID = cgNextDNodeID;
    }
    else{
        do{
            ++ cgNextDNodeID;
            firstItem->linkedID = cgSelfLinkedID;
            firstItem->fLinkedList = this;
            firstItem->dNodeID = cgNextDNodeID;
            firstItem = firstItem->next;
        }while(firstItem != endItem);
        ++ cgNextDNodeID;
        endItem->linkedID = cgSelfLinkedID;
        endItem->fLinkedList = this;
    }
	cgLLCS.Unlock();
	return 1;
}
//------------------------------------------------------------------------------------------//
template <typename TYPE_LINKED>
int32 LINKEDLIST<TYPE_LINKED>::Remove(LINKEDLIST_NODE<TYPE_LINKED> *tFirstItem,LINKEDLIST_NODE<TYPE_LINKED> *tEndItem){
	int32	blOK;
	if (tFirstItem == nullptr)
		return 0;
	if (tEndItem == nullptr)
		tEndItem = tFirstItem;
	cgLLCS.Lock();
	blOK = 0;
	if ((cgHead != nullptr) && (tFirstItem->linkedID == cgSelfLinkedID) && (tEndItem->linkedID == cgSelfLinkedID)){
		if (cgHead == tFirstItem)
			cgHead = tEndItem->next;
		if (cgHead == tFirstItem)
			cgHead = nullptr;
		tFirstItem->prior->next = tEndItem->next;
		tEndItem->next->prior = tFirstItem->prior;

		tFirstItem->prior = tEndItem;
		tEndItem->next = tFirstItem;
		do{
			tFirstItem->linkedID = 0;
			tFirstItem->fLinkedList = nullptr;
            tFirstItem->dNodeID = tFirstItem->nodeID;
			tFirstItem = tFirstItem->next;
		}while(tFirstItem != tEndItem);
		tEndItem->linkedID = 0;
		tEndItem->fLinkedList = nullptr;
        tFirstItem->dNodeID = tFirstItem->nodeID;
		blOK = 1;
	}
    if (cgHead == nullptr)
        cgNextDNodeID = 0;
	cgLLCS.Unlock();
	return(blOK);
}
//------------------------------------------------------------------------------------------//
template <typename TYPE_LINKED>
int32 LINKEDLIST<TYPE_LINKED>::MoveUp(LINKEDLIST_NODE<TYPE_LINKED> *tListItem,int32 blDoCycle){
	LINKEDLIST_NODE<TYPE_LINKED> *priorItem;
	int32	blOK;
	if (tListItem == nullptr)
		return 0;
	cgLLCS.Lock();
	blOK = 0;
	if ((cgHead != nullptr) && (tListItem->linkedID == cgSelfLinkedID)){
		blOK = 1;
		if (tListItem != cgHead){
			priorItem = tListItem->prior;
            if (cgHead == priorItem)
                cgHead = tListItem;

            priorItem->next = tListItem->next;
            tListItem->next->prior = priorItem;
            tListItem->next = priorItem;
            
            tListItem->prior = priorItem->prior;
            priorItem->prior->next = tListItem;
            priorItem->prior = tListItem;
        
            //Remove(priorItem);
			//Insert(priorItem,tListItem);
		}
		else if (blDoCycle != 0){
			cgHead = cgHead->next;
		}
	}
	cgLLCS.Unlock();
	return(blOK);
}
//------------------------------------------------------------------------------------------//
template <typename TYPE_LINKED>
int32 LINKEDLIST<TYPE_LINKED>::MoveDown(LINKEDLIST_NODE<TYPE_LINKED> *tListItem,int32 blDoCycle){
	LINKEDLIST_NODE<TYPE_LINKED> *nextItem;
	int32	blOK;
	if (tListItem == nullptr)
		return 0;
	cgLLCS.Lock();
	blOK = 0;
	if ((cgHead != nullptr) && (tListItem->linkedID == cgSelfLinkedID)){
		blOK = 1;
		if (tListItem != cgHead->prior){
			nextItem = tListItem->next;
            if (cgHead == tListItem)
                cgHead = nextItem;
            
            tListItem->next = nextItem->next;
            nextItem->next->prior = tListItem;
            nextItem->next = tListItem;
            
            nextItem->prior = tListItem->prior;
            tListItem->prior->next = nextItem;
            tListItem->prior = nextItem;
            
			//Remove(tListItem);
			//Insert(tListItem,nextItem);
		}
		else if (blDoCycle != 0){
			cgHead = cgHead->prior;
		}
	}
	cgLLCS.Unlock();
	return(blOK);
}
//------------------------------------------------------------------------------------------//
/*template <typename TYPE_LINKED>
void LINKEDLIST<TYPE_LINKED>::SetEmpty(void){
	cgLLCS.Lock();
	while (Remove(cgHead) > 0);
	cgLLCS.Unlock();
}*/
//------------------------------------------------------------------------------------------//
template <typename TYPE_LINKED>
LINKEDLIST_NODE<TYPE_LINKED> *LINKEDLIST<TYPE_LINKED>::GetSelected(void){
	int32							blOK;
	LINKEDLIST_NODE<TYPE_LINKED>	*tHead,*tNext;

	tNext = nullptr;
	cgLLCS.Lock();
	if (cgHead != nullptr){
		tHead = cgHead;
		tNext = cgHead;
		blOK = 0;
		do{
			if (tNext->CheckblSelected() != 0){
				blOK = 1;
				break;
			}
			tNext = tNext->next;
		}while(tNext != tHead);
		if (blOK == 0)
			tNext = nullptr;
	}
	cgLLCS.Unlock();
	return(tNext);
}
//------------------------------------------------------------------------------------------//
template <typename TYPE_LINKED>
LINKEDLIST_NODE<TYPE_LINKED> *LINKEDLIST<TYPE_LINKED>::FindByNodeID(uint32 tNodeID){
	int32							blOK;
	LINKEDLIST_NODE<TYPE_LINKED>	*tHead,*tNext;

	cgLLCS.Lock();
	tNext = nullptr;
	if (cgHead != nullptr){
		tHead = cgHead;
		tNext = cgHead;
		blOK = 0;
		do{
			if (tNext->nodeID == tNodeID){
				blOK = 1;
				break;
			}
			tNext = tNext->next;
		}while(tNext != tHead);
		if (blOK == 0)
			tNext = nullptr;
	}
	cgLLCS.Unlock();
	return(tNext);
}
//------------------------------------------------------------------------------------------//
template <typename TYPE_LINKED>
LINKEDLIST_NODE<TYPE_LINKED> *LINKEDLIST<TYPE_LINKED>::FindByDNodeID(uint32 tDNodeID){
    int32							blOK;
    LINKEDLIST_NODE<TYPE_LINKED>	*tHead,*tNext;
    
    cgLLCS.Lock();
    tNext = nullptr;
    if (cgHead != nullptr){
        tHead = cgHead;
        tNext = cgHead;
        blOK = 0;
        do{
            if (tNext->dNodeID == tDNodeID){
                blOK = 1;
                break;
            }
            tNext = tNext->next;
        }while(tNext != tHead);
        if (blOK == 0)
            tNext = nullptr;
    }
    cgLLCS.Unlock();
    return(tNext);
}
//------------------------------------------------------------------------------------------//
template <typename TYPE_LINKED>
LINKEDLIST_NODE<TYPE_LINKED> *LINKEDLIST<TYPE_LINKED>::SetSelectByNodeID(uint32 tNodeID){
	LINKEDLIST_NODE<TYPE_LINKED>	*tNode;

	cgLLCS.Lock();
	tNode = FindByNodeID(tNodeID);
	if (tNode != nullptr)
		tNode->SetblSelected();
	cgLLCS.Unlock();
	return(tNode);
}
//------------------------------------------------------------------------------------------//
template <typename TYPE_LINKED>
LINKEDLIST_NODE<TYPE_LINKED> *LINKEDLIST<TYPE_LINKED>::SetSelectByDNodeID(uint32 tDNodeID){
    LINKEDLIST_NODE<TYPE_LINKED>	*tNode;
    
    cgLLCS.Lock();
    tNode = FindByDNodeID(tDNodeID);
    if (tNode != nullptr)
        tNode->SetblSelected();
    cgLLCS.Unlock();
    return(tNode);
}
//------------------------------------------------------------------------------------------//






//------------------------------------------------------------------------------------------//
template <typename TYPE_LINKED_NODE>
class LINKED_POOL_B : public LINKEDLIST<TYPE_LINKED_NODE>{
	public:
		enum{RFLAG_C = 0, RFLAG_S = LINKEDLIST<TYPE_LINKED_NODE>::RFLAG_S + LINKEDLIST<TYPE_LINKED_NODE>::RFLAG_C};
	public:
		 		 LINKED_POOL_B(void) : LINKEDLIST<TYPE_LINKED_NODE>(){;};
		virtual	~LINKED_POOL_B(void){DestroyAll();};
	private:
	public:
		TYPE_LINKED_NODE	*Create(TYPE_LINKED_NODE *tLocationNode = nullptr,int32 blAfter = 1);
		void	Destroy(TYPE_LINKED_NODE **tTYPE_LINKED_NODE);
		void	DestroyAll(void);
};
//------------------------------------------------------------------------------------------//
template <typename TYPE_LINKED_NODE>
TYPE_LINKED_NODE *LINKED_POOL_B<TYPE_LINKED_NODE>::Create(TYPE_LINKED_NODE *tLocationNode,int32 blAfter){
	TYPE_LINKED_NODE	*tTYPE_LINKED_NODE;
	
	tTYPE_LINKED_NODE = new TYPE_LINKED_NODE();
	if (tTYPE_LINKED_NODE == nullptr)
		return nullptr;
	LINKEDLIST<TYPE_LINKED_NODE>::Insert(tTYPE_LINKED_NODE,tLocationNode,blAfter);
	return(tTYPE_LINKED_NODE);
}
//------------------------------------------------------------------------------------------//
template <typename TYPE_LINKED_NODE>
void LINKED_POOL_B<TYPE_LINKED_NODE>::Destroy(TYPE_LINKED_NODE **tTYPE_LINKED_NODE){
	if (*tTYPE_LINKED_NODE == nullptr)
		return;
	LINKEDLIST<TYPE_LINKED_NODE>::Remove((LINKEDLIST_NODE<TYPE_LINKED_NODE>*)*tTYPE_LINKED_NODE);
	try{
		delete *tTYPE_LINKED_NODE;
	}
	catch(...){}
	*tTYPE_LINKED_NODE = nullptr;
}
//------------------------------------------------------------------------------------------//
template <typename TYPE_LINKED_NODE>
void LINKED_POOL_B<TYPE_LINKED_NODE>::DestroyAll(void){
	TYPE_LINKED_NODE	*tHead;
	
	LINKEDLIST<TYPE_LINKED_NODE>::List_Lock();
	while(LINKEDLIST<TYPE_LINKED_NODE>::cgHead != nullptr){
		tHead = (TYPE_LINKED_NODE*) LINKEDLIST<TYPE_LINKED_NODE>::cgHead;
		LINKEDLIST<TYPE_LINKED_NODE>::Remove((LINKEDLIST_NODE<TYPE_LINKED_NODE>*)tHead);
		try{
			delete tHead;
		}
		catch(...){}
	};
	LINKEDLIST<TYPE_LINKED_NODE>::List_Unlock();
}
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
template <typename TYPE_LINKED_NODE,typename TYPE_CREATE1,typename TYPE_CREATE2>
class LINKED_POOLEx : public LINKED_POOL_B<TYPE_LINKED_NODE>{
	public:
		enum{RFLAG_C = 0, RFLAG_S = LINKED_POOL_B<TYPE_LINKED_NODE>::RFLAG_S + LINKED_POOL_B<TYPE_LINKED_NODE>::RFLAG_C};
	public:
		 		 LINKED_POOLEx(void) : LINKED_POOL_B<TYPE_LINKED_NODE>(){;};
		virtual	~LINKED_POOLEx(void){LINKED_POOL_B<TYPE_LINKED_NODE>::DestroyAll();};
	private:
	public:
		//TYPE_LINKED_NODE	*Create(TYPE_CREATE1 tTYPE_CREATE1);
		TYPE_LINKED_NODE	*Create(TYPE_CREATE1 tTYPE_CREATE1,TYPE_CREATE2 tTYPE_CREATE2);
};
//------------------------------------------------------------------------------------------//
/*template <typename TYPE_LINKED_NODE,typename TYPE_CREATE1,typename TYPE_CREATE2>
TYPE_LINKED_NODE *LINKED_POOLEx<TYPE_LINKED_NODE,TYPE_CREATE1,TYPE_CREATE2>::Create(TYPE_CREATE1 tTYPE_CREATE1){
	TYPE_LINKED_NODE	*tTYPE_LINKED_NODE;
	
	tTYPE_LINKED_NODE = new TYPE_LINKED_NODE(tTYPE_CREATE1);
	if (tTYPE_LINKED_NODE == nullptr)
		return nullptr;
	LINKEDLIST<TYPE_LINKED_NODE>::Add(tTYPE_LINKED_NODE);
	return(tTYPE_LINKED_NODE);
}*/
//------------------------------------------------------------------------------------------//
template <typename TYPE_LINKED_NODE,typename TYPE_CREATE1,typename TYPE_CREATE2>
TYPE_LINKED_NODE *LINKED_POOLEx<TYPE_LINKED_NODE,TYPE_CREATE1,TYPE_CREATE2>::Create(TYPE_CREATE1 tTYPE_CREATE1,TYPE_CREATE2 tTYPE_CREATE2){
	TYPE_LINKED_NODE	*tTYPE_LINKED_NODE;
	
	tTYPE_LINKED_NODE = new TYPE_LINKED_NODE(tTYPE_CREATE1,tTYPE_CREATE2);
	if (tTYPE_LINKED_NODE == nullptr)
		return nullptr;
	LINKEDLIST<TYPE_LINKED_NODE>::Add((LINKEDLIST_NODE<TYPE_LINKED_NODE>*)tTYPE_LINKED_NODE);
	return(tTYPE_LINKED_NODE);
}
//------------------------------------------------------------------------------------------//
#endif
#endif
