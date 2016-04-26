/*
 * Copyright (c) 2012-2014
 * All rights reserved.
 *
 * File Name: Comm_Tree.h
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2015.09.10
 */
//------------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "Comm_Tree.h"
//------------------------------------------------------------------------------------------//
std::atomic_uint RTREE_NODE::cgID_NextNode = ATOMIC_VAR_INIT(0);
//------------------------------------------------------------------------------------------//
RTREE_NODE::RTREE_NODE(void) : BASIC_CFLAG(){
	cgID_NextNode ++;
	nodeID = cgID_NextNode.load();
	
	cgID_NextDRNode = 0;
	dRNodeID = 0;
	
	cgTrash = nullptr;
	
	cgblVirtualHead = 1;
	cgVirtualRoot = this;
	cgFather = nullptr;
	cgLChild = nullptr;
	cgRChild = nullptr;
	
	cgCNType = CN_None;
	cgCoupleNode = nullptr;
	Enable();
	selfName = "RTREE_NODE";
}
//------------------------------------------------------------------------------------------//
RTREE_NODE	*RTREE_NODE::GetFather_nolock(RTREE_NODE *tTreeNode){
	RTREE_NODE	*ret;
	
	if (tTreeNode == nullptr)
		return(nullptr);
	
	ret = tTreeNode->cgFather;
	if (tTreeNode->cgblVirtualHead == 0)
		ret = tTreeNode->cgVirtualRoot->cgFather;
	return(ret);
}
//------------------------------------------------------------------------------------------//
RTREE_NODE	*RTREE_NODE::GetFather(RTREE_NODE *tTreeNode){
	RTREE_NODE	*ret,*node;
	
	if (tTreeNode == nullptr)
		return(nullptr);
	
	ret = nullptr;
	tTreeNode->Spin_Link_Lock();
	ret = tTreeNode->cgFather;
	
	if (tTreeNode->cgblVirtualHead == 0){
		node = tTreeNode->cgVirtualRoot;
		node->Spin_Link_Lock();
		ret = node->cgFather;
		node->Spin_Link_Unlock();
	}
	tTreeNode->Spin_Link_Unlock();
	return(ret);
}
//------------------------------------------------------------------------------------------//
RTREE_NODE	*RTREE_NODE::GetFirstChild_nolock(RTREE_NODE *tTreeNode){
	if (tTreeNode == nullptr)
		return(nullptr);
	return(tTreeNode->cgLChild);
}
//------------------------------------------------------------------------------------------//
RTREE_NODE	*RTREE_NODE::GetFirstChild(RTREE_NODE *tTreeNode){
	RTREE_NODE	*ret;
	
	if (tTreeNode == nullptr)
		return(nullptr);
	
	ret = nullptr;
	tTreeNode->Spin_Link_Lock();
	ret = tTreeNode->cgLChild;
	tTreeNode->Spin_Link_Unlock();
	return(ret);
}
//------------------------------------------------------------------------------------------//
RTREE_NODE	*RTREE_NODE::GetLastChild_nolock(RTREE_NODE *tTreeNode){
	RTREE_NODE	*ret,*child;
	
	if (tTreeNode == nullptr)
		return(nullptr);
	
	ret = nullptr;
	try{
		child = tTreeNode->cgLChild;
		if (child != nullptr)
			ret = child->cgVirtualRoot;
	}
	catch(...){
		ret = nullptr;
	}
	return(ret);
}
//------------------------------------------------------------------------------------------//
RTREE_NODE	*RTREE_NODE::GetLastChild(RTREE_NODE *tTreeNode){
	RTREE_NODE	*ret,*child;
	
	if (tTreeNode == nullptr)
		return(nullptr);
	
	ret = nullptr;
	tTreeNode->Spin_Link_Lock();
	child = tTreeNode->cgLChild;
	if (child != nullptr){
		child->Spin_Link_Lock();
		ret = child->cgVirtualRoot;
		child->Spin_Link_Unlock();
	}
	tTreeNode->Spin_Link_Unlock();
	return(ret);
}
//------------------------------------------------------------------------------------------//
RTREE_NODE	*RTREE_NODE::GetFirstBrother_nolock(RTREE_NODE *tTreeNode){
	RTREE_NODE	*ret;
	
	if (tTreeNode == nullptr)
		return(nullptr);
	
	ret = tTreeNode;
	if (tTreeNode->cgblVirtualHead == 0)
		ret = tTreeNode->cgVirtualRoot;
	return(ret);
}
//------------------------------------------------------------------------------------------//
RTREE_NODE	*RTREE_NODE::GetFirstBrother(RTREE_NODE *tTreeNode){
	RTREE_NODE	*ret;
	
	if (tTreeNode == nullptr)
		return(nullptr);
	
	ret = nullptr;
	tTreeNode->Spin_Link_Lock();
	ret = tTreeNode;
	if (tTreeNode->cgblVirtualHead == 0)
		ret = tTreeNode->cgVirtualRoot;
	tTreeNode->Spin_Link_Unlock();
	return(ret);
}
//------------------------------------------------------------------------------------------//
RTREE_NODE	*RTREE_NODE::GetLastBrother_nolock(RTREE_NODE *tTreeNode){
	RTREE_NODE	*ret,*node;
	
	if (tTreeNode == nullptr)
		return(nullptr);
	
	ret = nullptr;
	node = tTreeNode->cgVirtualRoot;
	ret = node;
	if (tTreeNode->cgblVirtualHead == 0)
		ret = node->cgVirtualRoot;
	return(ret);
}
//------------------------------------------------------------------------------------------//
RTREE_NODE	*RTREE_NODE::GetLastBrother(RTREE_NODE *tTreeNode){
	RTREE_NODE	*ret,*node;
	
	if (tTreeNode == nullptr)
		return(nullptr);
	
	ret = nullptr;
	tTreeNode->Spin_Link_Lock();
	node = tTreeNode->cgVirtualRoot;
	ret = node;
	if (tTreeNode->cgblVirtualHead == 0){
		node->Spin_Link_Lock();
		ret = node->cgVirtualRoot;
		node->Spin_Link_Unlock();
	}
	tTreeNode->Spin_Link_Unlock();
	return(ret);
}
//------------------------------------------------------------------------------------------//
RTREE_NODE	*RTREE_NODE::GetNextBrother_nolock(RTREE_NODE *tTreeNode){
	if (tTreeNode == nullptr)
		return(nullptr);
	
	return(tTreeNode->cgRChild);
}
//------------------------------------------------------------------------------------------//
RTREE_NODE	*RTREE_NODE::GetNextBrother(RTREE_NODE *tTreeNode){
	RTREE_NODE	*ret;
	
	if (tTreeNode == nullptr)
		return(nullptr);
	
	ret = nullptr;
	tTreeNode->Spin_Link_Lock();
	ret = tTreeNode->cgRChild;
	tTreeNode->Spin_Link_Unlock();
	return(ret);
}
//------------------------------------------------------------------------------------------//
RTREE_NODE	*RTREE_NODE::GetPriorBrother_nolock(RTREE_NODE *tTreeNode){
	RTREE_NODE	*ret;
	
	if (tTreeNode == nullptr)
		return(nullptr);
	
	ret = nullptr;
	if (tTreeNode->cgblVirtualHead == 0)
		ret = tTreeNode->cgFather;
	return(ret);
}
//------------------------------------------------------------------------------------------//
RTREE_NODE	*RTREE_NODE::GetPriorBrother(RTREE_NODE *tTreeNode){
	RTREE_NODE	*ret;
	
	if (tTreeNode == nullptr)
		return(nullptr);
	
	ret = nullptr;
	tTreeNode->Spin_Link_Lock();
	if (tTreeNode->cgblVirtualHead == 0)
		ret = tTreeNode->cgFather;
	tTreeNode->Spin_Link_Unlock();
	return(ret);
}
//------------------------------------------------------------------------------------------//
void RTREE_NODE::UpdateSubInstertTree_nolock(RTREE_NODE *tInsertNode,RTREE_NODE *tVirtualRoot){
	RTREE_NODE *loopNode,*opNode;
	
	tVirtualRoot->cgID_NextDRNode ++;
	tInsertNode->cgblVirtualHead = 0;
	tInsertNode->dRNodeID.store(tVirtualRoot->cgID_NextDRNode.load());
	tInsertNode->cgVirtualRoot = tVirtualRoot;
	
	loopNode = tInsertNode->cgRChild;
	while(loopNode != nullptr){
		opNode = loopNode;
		opNode->Spin_Link_Lock();
		
		tVirtualRoot->cgID_NextDRNode ++;
		opNode->cgblVirtualHead = 0;
		opNode->dRNodeID.store(tVirtualRoot->cgID_NextDRNode.load());
		opNode->cgVirtualRoot = tVirtualRoot;
		
		loopNode = opNode->cgRChild;
		opNode->Spin_Link_Unlock();
	}
}
//------------------------------------------------------------------------------------------//
int32 RTREE_NODE::InsertRChild(RTREE_NODE *tFatherNode,RTREE_NODE *tInsertNode){
	RTREE_NODE	*rootNode,*endNode,*rChild;
	int32 blok;
	
	if ((tFatherNode == nullptr) || (tInsertNode == nullptr) || (tFatherNode == tInsertNode))
		return 0;
	
	blok = 0;
	
	tInsertNode->Spin_Link_Lock();
	if (tInsertNode->cgFather == nullptr){
	RESTART:;
		tFatherNode->Spin_Link_Lock();
		if (tFatherNode->cgblVirtualHead != 0){
			rootNode = tFatherNode;
		}
		else{
			rootNode = tFatherNode->cgVirtualRoot;
			if (rootNode->Spin_Link_Try() == 0){
				tFatherNode->Spin_Link_Unlock();
				goto RESTART;
			}
		}
		
		endNode = tInsertNode->cgVirtualRoot;
		UpdateSubInstertTree_nolock(tInsertNode,rootNode);
		
		{//insert
			tInsertNode->cgFather = tFatherNode;
			rChild = tFatherNode->cgRChild;
			if (rChild == nullptr){
				rootNode->cgVirtualRoot = endNode;
			}
			else{
				if (endNode != tInsertNode)
					endNode->Spin_Link_Lock();
				endNode->cgRChild = rChild;
				if (endNode != tInsertNode)
					endNode->Spin_Link_Unlock();
				
				rChild->Spin_Link_Lock();
				rChild->cgFather = endNode;
				rChild->Spin_Link_Unlock();
			}
			tFatherNode->cgRChild = tInsertNode;
			blok = 1;
		}
		if (tFatherNode->cgblVirtualHead == 0)
			rootNode->Spin_Link_Unlock();
		tFatherNode->Spin_Link_Unlock();
	}
	tInsertNode->Spin_Link_Unlock();
	return(blok);
}
//------------------------------------------------------------------------------------------//
int32 RTREE_NODE::InsertLChild(RTREE_NODE *tFatherNode,RTREE_NODE *tInsertNode,G_LOCK_VAILD blLock){
	RTREE_NODE	*lChild,*locateNode,*endNode;
	int32		blok;
	
	if ((tFatherNode == nullptr) || (tInsertNode == nullptr) || (tFatherNode == tInsertNode))
		return 0;
	
	blok = 0;
	lChild = nullptr;
	
	tInsertNode->Spin_Link_Lock();
	if (tInsertNode->cgFather == nullptr){
		tFatherNode->Spin_Link_Lock(blLock);
		lChild = tFatherNode->cgLChild;
		
		if (lChild == nullptr){
			tInsertNode->cgFather = tFatherNode;
			tFatherNode->cgLChild = tInsertNode;
		}
		else{
			lChild->Spin_Link_Lock(blLock);
			locateNode = lChild->cgVirtualRoot;
			endNode = tInsertNode->cgVirtualRoot;
			
			UpdateSubInstertTree_nolock(tInsertNode,lChild);
			
			{//insert
				tInsertNode->cgFather = locateNode;
				lChild->cgVirtualRoot = endNode;
				if (locateNode != lChild)
					locateNode->Spin_Link_Lock();
				locateNode->cgRChild = tInsertNode;
				if (locateNode != lChild)
					locateNode->Spin_Link_Unlock();
			}
			lChild->Spin_Link_Unlock(blLock);
		}
		blok = 1;
		tFatherNode->Spin_Link_Unlock(blLock);
	}
	tInsertNode->Spin_Link_Unlock();
	return(blok);
}
//------------------------------------------------------------------------------------------//
void RTREE_NODE::UpdateSubRChain_nolock(RTREE_NODE *tSubRootNode){
	RTREE_NODE *loopNode,*opNode;
	
	tSubRootNode->cgblVirtualHead = 1;
	tSubRootNode->cgID_NextDRNode = 0;
	tSubRootNode->dRNodeID = 0;
	tSubRootNode->cgFather = nullptr;
	opNode = tSubRootNode;
	loopNode = tSubRootNode->cgRChild;
	while(loopNode != nullptr){
		opNode = loopNode;
		opNode->Spin_Link_Lock();
		
		tSubRootNode->cgID_NextDRNode ++;
		opNode->cgblVirtualHead = 0;
		opNode->dRNodeID.store(tSubRootNode->cgID_NextDRNode.load());
		opNode->cgVirtualRoot = tSubRootNode;
		
		loopNode = opNode->cgRChild;
		opNode->Spin_Link_Unlock();
	};
	tSubRootNode->cgVirtualRoot = opNode;
}
//------------------------------------------------------------------------------------------//
RTREE_NODE *RTREE_NODE::BreakRChild(RTREE_NODE *tFatherNode){
	RTREE_NODE	*rootNode,*rChild;
	
	if (tFatherNode == nullptr)
		return(nullptr);
	
RESTART:;
	tFatherNode->Spin_Link_Lock();
	rChild = tFatherNode->cgRChild;
	if (rChild != nullptr){
		if (tFatherNode->cgblVirtualHead != 0){
			rootNode = tFatherNode;
		}
		else{
			rootNode = tFatherNode->cgVirtualRoot;
			if (rootNode->Spin_Link_Try() == 0){
				tFatherNode->Spin_Link_Unlock();
				goto RESTART;
			}
		}
		
		rootNode->cgVirtualRoot = tFatherNode;
		tFatherNode->cgRChild = nullptr;
		rChild->Spin_Link_Lock();
		UpdateSubRChain_nolock(rChild);
		rChild->Spin_Link_Unlock();
		if (tFatherNode->cgblVirtualHead == 0)
			rootNode->Spin_Link_Unlock();
	}
	tFatherNode->Spin_Link_Unlock();
	return(rChild);
}
//------------------------------------------------------------------------------------------//
RTREE_NODE *RTREE_NODE::BreakLChild(RTREE_NODE *tFatherNode){
	RTREE_NODE	*lChild;
	
	if (tFatherNode == nullptr)
		return(nullptr);
	
	lChild = nullptr;
	tFatherNode->Spin_Link_Lock();
	if (tFatherNode->cgLChild != nullptr){
		lChild = tFatherNode->cgLChild;
		tFatherNode->cgLChild = nullptr;
		lChild->Spin_Link_Lock();
		lChild->cgFather = nullptr;
		lChild->Spin_Link_Unlock();
	}
	tFatherNode->Spin_Link_Unlock();
	
	return(lChild);
}
//------------------------------------------------------------------------------------------//
void RTREE_NODE::RemoveFromFather(RTREE_NODE *tTreeNode){
	RTREE_NODE	*rootNode,*fatherNode;
	
	if (tTreeNode == nullptr)
		return;
	
RESTART:;
	tTreeNode->Spin_Link_Lock();
	fatherNode = tTreeNode->cgFather;
	if (fatherNode != nullptr){
		if (fatherNode->Spin_Link_Try() == 0){
			tTreeNode->Spin_Link_Unlock();
			goto RESTART;
		}

		if (tTreeNode->cgblVirtualHead != 0){
			fatherNode->cgLChild = nullptr;
			tTreeNode->cgFather = nullptr;
		}
		else{
			if (fatherNode->cgblVirtualHead != 0){
				rootNode = fatherNode;
			}
			else{
				rootNode = fatherNode->cgVirtualRoot;
				if (rootNode->Spin_Link_Try() == 0){
					fatherNode->Spin_Link_Unlock();
					tTreeNode->Spin_Link_Unlock();
					goto RESTART;
				}
			}
			rootNode->cgVirtualRoot = fatherNode;
			fatherNode->cgRChild = nullptr;
			UpdateSubRChain_nolock(tTreeNode);
			if (fatherNode->cgblVirtualHead == 0)
				rootNode->Spin_Link_Unlock();
		}
		fatherNode->Spin_Link_Unlock();
	}
	tTreeNode->Spin_Link_Unlock();
}
//------------------------------------------------------------------------------------------//
RTREE_NODE *RTREE_NODE::RemoveNodesInRChain(RTREE_NODE *tFirstNode,RTREE_NODE *tEndNode,G_LOCK_VAILD blLock){
	RTREE_NODE	*retSubRoot,*rootNode,*fatherNode,*priorNode,*nextNode,*opNode,*loopNote;
	
	if (tFirstNode == nullptr)
		return(nullptr);
	
	if (tEndNode == nullptr)
		tEndNode = tFirstNode;
	
	retSubRoot = nullptr;
	
RESTART:;
	tFirstNode->Spin_Link_Lock();
	
	if (tFirstNode->cgblVirtualHead == 0){
		rootNode = tFirstNode->cgVirtualRoot;
		if (rootNode->Spin_Link_Try(blLock) == 0){
			tFirstNode->Spin_Link_Unlock();
			goto RESTART;
		}
		
		if (tEndNode != tFirstNode)
			tEndNode->Spin_Link_Lock();
		nextNode = tEndNode->cgRChild;
		
		priorNode = tFirstNode->cgFather;
		if (priorNode != rootNode)
			priorNode->Spin_Link_Lock();
		priorNode->cgRChild = nextNode;
		if (priorNode != rootNode)
			priorNode->Spin_Link_Unlock();
		
		if (nextNode != nullptr){
			nextNode->Spin_Link_Lock();
			nextNode->cgFather = priorNode;
			nextNode->Spin_Link_Unlock();
		}
		else{
			rootNode->cgVirtualRoot = priorNode;
		}
		tEndNode->cgRChild = nullptr;
		if (tEndNode != tFirstNode)
			tEndNode->Spin_Link_Unlock();
		UpdateSubRChain_nolock(tFirstNode);
		rootNode->Spin_Link_Unlock(blLock);
	}
	else{
		if (tEndNode != tFirstNode)
			tEndNode->Spin_Link_Lock();
		nextNode = tEndNode->cgRChild;

		fatherNode = tFirstNode->cgFather;
		if (fatherNode != nullptr){
			if (fatherNode->Spin_Link_Try(blLock) == 0){
				if (tEndNode != tFirstNode)
					tEndNode->Spin_Link_Unlock();
				tFirstNode->Spin_Link_Unlock();
				goto RESTART;
			}
			fatherNode->cgLChild = nextNode;
			fatherNode->Spin_Link_Unlock(blLock);
		}
		
		tFirstNode->cgFather = nullptr;
		tFirstNode->cgVirtualRoot = tEndNode;
		tEndNode->cgRChild = nullptr;
		if (tEndNode != tFirstNode)
			tEndNode->Spin_Link_Unlock();

		if (nextNode != nullptr){
			nextNode->Spin_Link_Lock();
			if (fatherNode == nullptr){
				UpdateSubRChain_nolock(nextNode);
				retSubRoot = nextNode;
			}
			else{
				nextNode->cgblVirtualHead = 1;
				nextNode->cgID_NextDRNode.store(tFirstNode->cgID_NextDRNode.load());
				nextNode->cgFather = fatherNode;
				opNode = nextNode;
				loopNote = nextNode->cgRChild;
				while(loopNote != nullptr){
					opNode = loopNote;
					opNode->Spin_Link_Lock();
					opNode->cgblVirtualHead = 0;
					opNode->cgVirtualRoot = nextNode;
					loopNote = opNode->cgRChild;
					opNode->Spin_Link_Unlock();
				};
				nextNode->cgVirtualRoot = opNode;
			}
			nextNode->Spin_Link_Unlock();
		}
	}
	tFirstNode->Spin_Link_Unlock();
	return(retSubRoot);
}
//------------------------------------------------------------------------------------------//
void RTREE_NODE::MoveNodesUpInRChain(RTREE_NODE *tFirstNode,RTREE_NODE *tEndNode){
	RTREE_NODE	*rootNode,*priorNodefather,*priorNode,*nextNode;
	RTREE_NODE	*loopNode,*opNode;
	int32		blret;
	
	if (tFirstNode == nullptr)
		return;
	if (tEndNode == nullptr)
		tEndNode = tFirstNode;

	do{
		blret = 1;
		tFirstNode->Spin_Link_Lock();
		if (tFirstNode->cgblVirtualHead == 0){
			blret = 0;
			priorNode = tFirstNode->cgFather;
			if (priorNode->Spin_Link_Try() > 0){
				do{
					rootNode = tFirstNode->cgVirtualRoot;
					if (rootNode != priorNode){
						if (rootNode->Spin_Link_Try() == 0)
							break;
					}
					{
						if (rootNode->cgVirtualRoot == tEndNode)
							rootNode->cgVirtualRoot = priorNode;
						
						if (tEndNode != tFirstNode)
							tEndNode->Spin_Link_Lock();
						{
							nextNode = tEndNode->cgRChild;
							if (nextNode != nullptr){
								nextNode->Spin_Link_Lock();
								nextNode->cgFather = priorNode;
								nextNode->Spin_Link_Unlock();
							}
							
							priorNodefather = priorNode->cgFather;
							priorNode->cgFather = tEndNode;
							priorNode->cgRChild = nextNode;
							tFirstNode->cgFather = priorNodefather;
							tEndNode->cgRChild = priorNode;
						}
						if (tEndNode != tFirstNode)
							tEndNode->Spin_Link_Unlock();
						
						if (priorNode->cgblVirtualHead == 0){
							if (priorNodefather != rootNode)
								priorNodefather->Spin_Link_Lock();
							priorNodefather->cgRChild = tFirstNode;
							if (priorNodefather != rootNode)
								priorNodefather->Spin_Link_Unlock();
						}
						else{
							if (priorNodefather != nullptr){
								priorNodefather->Spin_Link_Lock();
								priorNodefather->cgLChild = tFirstNode;
								priorNodefather->Spin_Link_Unlock();
							}
							tFirstNode->cgblVirtualHead = 1;
							tFirstNode->cgID_NextDRNode.store(priorNode->cgID_NextDRNode.load());
							
							opNode = tFirstNode;
							loopNode = tFirstNode->cgRChild;
							while(loopNode != nullptr){
								opNode = loopNode;
								opNode->Spin_Link_Lock(((opNode == priorNode) || (opNode == rootNode))?G_LOCK_OFF:G_LOCK_ON);
								opNode->cgblVirtualHead = 0;
								opNode->cgVirtualRoot = tFirstNode;
								loopNode = opNode->cgRChild;
								opNode->Spin_Link_Unlock(((opNode == priorNode) || (opNode == rootNode))?G_LOCK_OFF:G_LOCK_ON);
							};
							tFirstNode->cgVirtualRoot = opNode;
						}
					}
					if (rootNode != priorNode)
						rootNode->Spin_Link_Unlock();
					blret = 1;
				}while(0);
				priorNode->Spin_Link_Unlock();
			}
		}
		tFirstNode->Spin_Link_Unlock();
	}while(blret == 0);
}
//------------------------------------------------------------------------------------------//
void RTREE_NODE::MoveNodesDownInRChain(RTREE_NODE *tFirstNode,RTREE_NODE *tEndNode){
	RTREE_NODE	*rootNode,*priorNode,*nextNode,*nextNodeRChild;
	RTREE_NODE	*loopNode,*opNode;
	int32		blret;

	if (tFirstNode == nullptr)
		return;
	if (tEndNode == nullptr)
		tEndNode = tFirstNode;
	
	do{
		blret = 1;
		tEndNode->Spin_Link_Lock();
		nextNode = tEndNode->cgRChild;
		if (nextNode != nullptr){
			blret = 0;
			if (nextNode->Spin_Link_Try() > 0){
				do{
					rootNode = nextNode->cgVirtualRoot;
					if (rootNode != tEndNode){
						if (rootNode->Spin_Link_Try() == 0)
							break;
					}
					{
						if (rootNode->cgVirtualRoot == nextNode)
							rootNode->cgVirtualRoot = tEndNode;
						
						if ((tFirstNode != tEndNode) && (tFirstNode != rootNode))
							tFirstNode->Spin_Link_Lock();
						{
							nextNodeRChild = nextNode->cgRChild;
							if (nextNodeRChild != nullptr){
								nextNodeRChild->Spin_Link_Lock();
								nextNodeRChild->cgFather = tEndNode;
								nextNodeRChild->Spin_Link_Unlock();
							}
							
							priorNode = tFirstNode->cgFather;
							
							nextNode->cgFather = priorNode;
							nextNode->cgRChild = tFirstNode;
							tFirstNode->cgFather = nextNode;
							tEndNode->cgRChild = nextNodeRChild;
							
							if (tFirstNode->cgblVirtualHead == 0){
								if (priorNode != rootNode)
									priorNode->Spin_Link_Lock();
								priorNode->cgRChild = nextNode;
								if (priorNode != rootNode)
									priorNode->Spin_Link_Unlock();
							}
							else{
								if (priorNode != nullptr){
									priorNode->Spin_Link_Lock();
									priorNode->cgLChild = nextNode;
									priorNode->Spin_Link_Unlock();
								}
								nextNode->cgblVirtualHead = 1;
								nextNode->cgID_NextDRNode.store(tFirstNode->cgID_NextDRNode.load());
								
								opNode = nextNode;
								loopNode = nextNode->cgRChild;
								while(loopNode != nullptr){
									opNode = loopNode;
									opNode->Spin_Link_Lock(((opNode == rootNode) || (opNode == tFirstNode) || (opNode == tEndNode))?G_LOCK_OFF:G_LOCK_ON);
									opNode->cgblVirtualHead = 0;
									opNode->cgVirtualRoot = nextNode;
									loopNode = opNode->cgRChild;
									opNode->Spin_Link_Unlock(((opNode == rootNode) || (opNode == tFirstNode) || (opNode == tEndNode))?G_LOCK_OFF:G_LOCK_ON);
								};
								nextNode->cgVirtualRoot = opNode;
							}
						}
						if ((tFirstNode != tEndNode) && (tFirstNode != rootNode))
							tFirstNode->Spin_Link_Unlock();
					}
					if (rootNode != tEndNode)
						rootNode->Spin_Link_Unlock();
					blret = 1;
				}while(0);
				nextNode->Spin_Link_Unlock();
			}
		}
		tEndNode->Spin_Link_Unlock();
	}while(blret == 0);
}
//------------------------------------------------------------------------------------------//
void RTREE_NODE::MoveNodesAfterInRChain(RTREE_NODE *tFirstNode,RTREE_NODE *tEndNode,RTREE_NODE *tAfterNode){
//	RTREE_NODE	*rootNode,*priorNode,tFirstNode,tEndNode,*nextNode,tAfterNode,*afterNodeRChild;
//	RTREE_NODE	*rootNode,tAfterNode,*afterNodeRChild,*priorNode,tFirstNode,tEndNode,*nextNode;

	RTREE_NODE	*rootNode,*priorNode,*nextNode,*afterNodeRChild;
	RTREE_NODE	*loopNode,*opNode;
	int32		blret;
	if (tFirstNode == nullptr)
		return;
	if (tEndNode == nullptr)
		tEndNode = tFirstNode;
	do{
		blret = 1;
		tEndNode->Spin_Link_Lock();
		nextNode = tEndNode->cgRChild;
		if (tAfterNode == nullptr)
			tAfterNode = nextNode;
		if ((nextNode != nullptr) && (tAfterNode != tFirstNode) && (tAfterNode != tEndNode)){
			blret = 0;
			if (nextNode->Spin_Link_Try() > 0){
				if (tAfterNode->Spin_Link_Try((tAfterNode == nextNode)?G_LOCK_OFF:G_LOCK_ON) > 0){
					do{
						rootNode = nextNode->cgVirtualRoot;
						if (rootNode->Spin_Link_Try((rootNode == tEndNode)?G_LOCK_OFF:G_LOCK_ON) == 0)
							break;
						{
							if (rootNode->cgVirtualRoot == tAfterNode)
								rootNode->cgVirtualRoot = tEndNode;
							
							tFirstNode->Spin_Link_Lock(((tFirstNode == tEndNode) || (tFirstNode == rootNode))?G_LOCK_OFF:G_LOCK_ON);
							{
								priorNode = tFirstNode->cgFather;
								
								if (priorNode != tAfterNode){
									afterNodeRChild = tAfterNode->cgRChild;
									if (afterNodeRChild != nullptr){
										afterNodeRChild->Spin_Link_Lock();
										afterNodeRChild->cgFather = tEndNode;
										afterNodeRChild->Spin_Link_Unlock();
									}
									
									tAfterNode->cgRChild = tFirstNode;
									tFirstNode->cgFather = tAfterNode;
									tEndNode->cgRChild = afterNodeRChild;
									
									nextNode->cgFather = priorNode;
									
									if (tFirstNode->cgblVirtualHead == 0){
										if (priorNode != rootNode)
											priorNode->Spin_Link_Lock();
										priorNode->cgRChild = nextNode;
										if (priorNode != rootNode)
											priorNode->Spin_Link_Unlock();
									}
									else{
										if (priorNode != nullptr){
											priorNode->Spin_Link_Lock();
											priorNode->cgLChild = nextNode;
											priorNode->Spin_Link_Unlock();
										}
										nextNode->cgblVirtualHead = 1;
										nextNode->cgID_NextDRNode.store(tFirstNode->cgID_NextDRNode.load());
										
										opNode = nextNode;
										loopNode = nextNode->cgRChild;
										while(loopNode != nullptr){
											opNode = loopNode;
											opNode->Spin_Link_Lock(((opNode == rootNode) || (opNode == tFirstNode) || (opNode == tEndNode) || (opNode == tAfterNode))
																   ?G_LOCK_OFF:G_LOCK_ON);
											opNode->cgblVirtualHead = 0;
											opNode->cgVirtualRoot = nextNode;
											loopNode = opNode->cgRChild;
											opNode->Spin_Link_Unlock(((opNode == rootNode) || (opNode == tFirstNode) || (opNode == tEndNode) || (opNode == tAfterNode))
																	 ?G_LOCK_OFF:G_LOCK_ON);
										};
										nextNode->cgVirtualRoot = opNode;
									}
								}
							}
							tFirstNode->Spin_Link_Unlock(((tFirstNode == tEndNode) || (tFirstNode == rootNode))?G_LOCK_OFF:G_LOCK_ON);
						}
						rootNode->Spin_Link_Unlock((rootNode == tEndNode)?G_LOCK_OFF:G_LOCK_ON);
						blret = 1;
					}while(0);
					tAfterNode->Spin_Link_Unlock((tAfterNode == nextNode)?G_LOCK_OFF:G_LOCK_ON);
				}
				nextNode->Spin_Link_Unlock();
			}
		}
		tEndNode->Spin_Link_Unlock();
	}while(blret == 0);
}
//------------------------------------------------------------------------------------------//
void RTREE_NODE::DestroyTree(RTREE_NODE *tTreeNode){
	
	if (tTreeNode == nullptr)
		return;

	DestroyTree(RTREE_NODE::BreakRChild(tTreeNode));
	DestroyTree(RTREE_NODE::BreakLChild(tTreeNode));
	
	try{
		delete tTreeNode;
	}
	catch(...){}
	tTreeNode = nullptr;
}
//------------------------------------------------------------------------------------------//
void RTREE_NODE::DestroySubTree(RTREE_NODE *tTreeNode){
	
	if (tTreeNode == nullptr)
		return;
	
	DestroyTree(RTREE_NODE::BreakRChild(tTreeNode));
	DestroyTree(RTREE_NODE::BreakLChild(tTreeNode));
}
//------------------------------------------------------------------------------------------//
RTREE_NODE *RTREE_NODE::GetNewNode(RTREE_NODE *tTrashOwner){
	RTREE_NODE *nNode,*cNode;
	if (tTrashOwner == nullptr)
		return(nullptr);
	if (tTrashOwner->cgTrash == nullptr)
		return(tTrashOwner->CreateNode());
	nNode = GetLastChild(tTrashOwner->cgTrash);
	if (nNode == nullptr)
		return(tTrashOwner->CreateNode());
	RemoveNodesInRChain(nNode);
	cNode = BreakLChild(nNode);
	InsertLChild(tTrashOwner->cgTrash,cNode);
	return(nNode);
}
//------------------------------------------------------------------------------------------//
void RTREE_NODE::CreateTrash(RTREE_NODE *tTrashOwner){
	if (tTrashOwner == nullptr)
		return;
	if (tTrashOwner->cgTrash == nullptr)
		tTrashOwner->cgTrash = new RTREE_NODE;
}
//------------------------------------------------------------------------------------------//
void RTREE_NODE::CleanTrash(RTREE_NODE *tTrashOwner){
	RTREE_NODE *deltree;
	if (tTrashOwner == nullptr)
		return;
	deltree = BreakLChild(tTrashOwner->cgTrash);
	DestroyTree(deltree);
}
//------------------------------------------------------------------------------------------//
void RTREE_NODE::DestroyTrash(RTREE_NODE *tTrashOwner){
	if (tTrashOwner == nullptr)
		return;
	DestroyTree(tTrashOwner->cgTrash);
	tTrashOwner->cgTrash = nullptr;
}
//------------------------------------------------------------------------------------------//
RTREE_NODE *RTREE_NODE::MoveNodeToTrash(RTREE_NODE *tTreeNode,RTREE_NODE *tTrashOwner){
	RTREE_NODE *ret;
	if (tTreeNode == nullptr)
		return(nullptr);
	
	ret = RemoveNodesInRChain(tTreeNode);
	if (tTrashOwner == nullptr){
		DestroyTree(tTreeNode);
	}
	else{
		InsertLChild(tTrashOwner->cgTrash,tTreeNode);
	}
	return(ret);
}
//------------------------------------------------------------------------------------------//
RTREE_NODE *RTREE_NODE::MoveNodesToTrash(RTREE_NODE *tFirstNode,RTREE_NODE *tEndNode,RTREE_NODE *tTrashOwner){
	RTREE_NODE *ret;
	if (tFirstNode == nullptr)
		return(nullptr);
	
	ret = RemoveNodesInRChain(tFirstNode,tEndNode);
	if (tTrashOwner == nullptr){
		DestroyTree(tFirstNode);
	}
	else{
		InsertLChild(tTrashOwner->cgTrash,tFirstNode);
	}
	return(ret);
}
//------------------------------------------------------------------------------------------//
void RTREE_NODE::MoveTreeToTrash(RTREE_NODE *tTreeNode,RTREE_NODE *tTrashOwner){
	if (tTreeNode == nullptr)
		return;
	RemoveFromFather(tTreeNode);
	if (tTrashOwner == nullptr){
		DestroyTree(tTreeNode);
	}
	else{
		InsertLChild(tTrashOwner->cgTrash,tTreeNode);
	}
}
//------------------------------------------------------------------------------------------//
RTREE_NODE *FindByNodeID_lock(RTREE_NODE *tTreeNode,uint32 tNodeID){
	uint32	id;
	RTREE_NODE *ret;
	
	if (tTreeNode == nullptr)
		return(nullptr);
	
	ret = nullptr;
	tTreeNode->Spin_Link_Lock();
	
	id = RTREE_NODE::GetnodeID(tTreeNode).load();
	
	if (id == tNodeID){
		ret = tTreeNode;
	}
	else{
		ret = FindByNodeID_lock(RTREE_NODE::GetcgRChild(tTreeNode),tNodeID);
		if (ret == nullptr)
			ret = FindByNodeID_lock(RTREE_NODE::GetcgLChild(tTreeNode),tNodeID);
	}
	
	tTreeNode->Spin_Link_Unlock();
	return(ret);
}
//------------------------------------------------------------------------------------------//
RTREE_NODE *RTREE_NODE::FindInTreeByNodeID(RTREE_NODE *tTreeNode,uint32 tNodeID){
	uint32	id;
	RTREE_NODE *ret,*rootNode;
	
	if (tTreeNode == nullptr)
		return(nullptr);
	
	ret = nullptr;
	rootNode = nullptr;
	tTreeNode->Spin_Link_Lock();
	
	id = tTreeNode->nodeID.load();
	
	if (id == tNodeID){
		ret = tTreeNode;
	}
	else{
		if (tTreeNode->cgblVirtualHead == 0){
			rootNode = tTreeNode->cgVirtualRoot;
			rootNode->Spin_Link_Lock();
		}
		
		ret = FindByNodeID_lock(tTreeNode->cgRChild,tNodeID);
		if (ret == nullptr)
			ret = FindByNodeID_lock(tTreeNode->cgLChild,tNodeID);
		if (tTreeNode->cgblVirtualHead == 0)
			rootNode->Spin_Link_Unlock();
	}
	
	tTreeNode->Spin_Link_Unlock();
	return(ret);
}
//------------------------------------------------------------------------------------------//
/*RTREE_NODE *RTREE_NODE::FindInRChainByDRNodeID(RTREE_NODE *tTreeNode,uint32 tDRNodeID){
	RTREE_NODE *rootNode,*ret,*fatherNode;
	
	if (tTreeNode == nullptr)
		return(nullptr);
	
	rootNode = GetFirstBrother(tTreeNode);
	rootNode->Spin_Link_Lock();
	fatherNode = rootNode->cgFather;
	rootNode->Spin_Link_Unlock();
	
	if (fatherNode != nullptr)
		fatherNode->Spin_Link_Lock();
	
	RTREE_RChain_Find(RTREE_NODE,rootNode,ret,(operateNode_t->dRNodeID.load() == tDRNodeID));
	
	if (fatherNode != nullptr)
		fatherNode->Spin_Link_Unlock();
	return(ret);
}*/
//------------------------------------------------------------------------------------------//
RTREE_NODE *RTREE_NODE::FindInLChildRChainByDRNodeID(RTREE_NODE *tTreeNode,uint32 tDRNodeID){
	RTREE_NODE *ret;
	ret = nullptr;
	RTREE_LChildRChain_Find(RTREE_NODE,tTreeNode,ret,(operateNode_t->dRNodeID.load() == tDRNodeID));
	return(ret);
}
//------------------------------------------------------------------------------------------//
RTREE_NODE *RTREE_NODE::FindInLChildRChainByDRNodeID_nolock(RTREE_NODE *tTreeNode,uint32 tDRNodeID){
	RTREE_NODE	*tSelect;
	tSelect = nullptr;
	if (tTreeNode == nullptr)
		return(nullptr);
	RTREE_LChildRChain_Traversal_LINE_nolock(RTREE_NODE,tTreeNode,
		if (operateNode_t->dRNodeID.load() == tDRNodeID){
			tSelect = operateNode_t;
			break;
		}
	);
	return(tSelect);}
//------------------------------------------------------------------------------------------//
/*RTREE_NODE *RTREE_NODE::GetSelectedInRChain(RTREE_NODE *tTreeNode){
	RTREE_NODE *rootNode,*ret,*fatherNode;
	
	if (tTreeNode == nullptr)
		return(nullptr);
	
	rootNode = GetFirstBrother(tTreeNode);
	rootNode->Spin_Link_Lock();
	fatherNode = rootNode->cgFather;
	rootNode->Spin_Link_Unlock();
	
	if (fatherNode != nullptr)
		fatherNode->Spin_Link_Lock();
	
	RTREE_RChain_Find(RTREE_NODE,rootNode,ret,(operateNode_t->CheckblSelected() != 0));
	
	if (fatherNode != nullptr)
		fatherNode->Spin_Link_Unlock();
	return(ret);
}*/
//------------------------------------------------------------------------------------------//
RTREE_NODE *RTREE_NODE::GetSelectedInLChildRChain(RTREE_NODE *tTreeNode){
	RTREE_NODE	*tSelect;
	tSelect = nullptr;
	RTREE_LChildRChain_Find(RTREE_NODE,tTreeNode,tSelect,(operateNode_t->CheckblSelected() != 0));\
	return(tSelect);
}
//------------------------------------------------------------------------------------------//
RTREE_NODE *RTREE_NODE::GetSelectedInLChildRChain_nolock(RTREE_NODE *tTreeNode){
	RTREE_NODE	*tSelect;
	tSelect = nullptr;
	if (tTreeNode == nullptr)
		return(nullptr);
	RTREE_LChildRChain_Traversal_LINE_nolock(RTREE_NODE,tTreeNode->cgLChild,
		if (operateNode_t->CheckblSelected() != 0){
			tSelect = operateNode_t;
			break;
		}
	);
	return(tSelect);
}
//------------------------------------------------------------------------------------------//
void RTREE_NODE::LinkCoupleNode(RTREE_NODE *slaveNode,G_LOCK_VAILD blLock){
	if (slaveNode == nullptr)
		return;
	Spin_InUse_set(blLock);
		UnlinkCoupleNode(G_LOCK_OFF);
		cgCoupleNode = slaveNode;
		cgCNType = CN_1;
		slaveNode->Spin_InUse_set();
			slaveNode->UnlinkCoupleNode(G_LOCK_OFF);
			slaveNode->cgCoupleNode = this;
			slaveNode->cgCNType = CN_0;
		slaveNode->Spin_InUse_clr();
	Spin_InUse_clr(blLock);
}
//------------------------------------------------------------------------------------------//
void RTREE_NODE::UnlinkCoupleNode(G_LOCK_VAILD blLock){
	Spin_InUse_set(blLock);
	if (cgCoupleNode != nullptr){
		cgCoupleNode->Spin_InUse_set();
		cgCoupleNode->cgCoupleNode = nullptr;
		cgCoupleNode->cgCNType = CN_None;
		cgCoupleNode->Spin_InUse_clr();
		cgCoupleNode = nullptr;
		cgCNType = CN_None;
	}
	Spin_InUse_clr(blLock);
}
//------------------------------------------------------------------------------------------//
