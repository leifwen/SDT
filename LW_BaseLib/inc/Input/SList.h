//
//  SList.h
//  SDT
//
//  Created by Leif Wen on 17/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

//------------------------------------------------------------------------------------------//
#include "DS_Tree.h"
//------------------------------------------------------------------------------------------//
#ifdef DS_Tree_h
#ifndef SList_h
#define SList_h
#ifdef SList_h
//------------------------------------------------------------------------------------------//
class SC_NODE : public TNF{
	private:
	public:
				 SC_NODE(void);
		virtual ~SC_NODE(void){;};
	public://need record
		STDSTR		StrCommand;
		bool32		blEnableSendCR;//send "\r"
		DS_RWLock	rwLock;
	private:
				STDSTR&		ExportV0_4			(uint32 ver,STDSTR* strOut);
				void		ImportV0_4			(uint32 ver,STDSTR* strIn);
	public:
				void 		Init				(void);
				STDSTR&		Export				(uint32 ver,STDSTR* strOut);
				void		Import				(uint32 ver,STDSTR* strIn);
	
		static	void		Copy				(SC_NODE *node1,const SC_NODE *node2);//copy 2 to 1
		static	STDSTR&		GetTitle			(STDSTR* retStr);
				STDSTR&		Compose				(STDSTR* retStr);
};
//------------------------------------------------------------------------------------------//
class SC_LIST : public TNFP{
	public:
				 SC_LIST(void);
		virtual ~SC_LIST(void);
	protected:
		virtual	TNF*		CreateNode			(void);
	private:
				STDSTR&		ExportV0_4			(uint32 ver,STDSTR* strOut);
				void		ImportV0_4			(uint32 ver,STDSTR* strIn);
	public:
				void 		Init				(void);
				STDSTR&		Export				(uint32 ver,STDSTR* strOut);
				void		Import				(uint32 ver,STDSTR* strIn);
	
				void		Save				(const STDSTR& fileName);
				void		Load				(const STDSTR& fileName);

				void		MoveToTrash			(TNF* tFirstNode,TNF* tEndNode = nullptr);
				void		Empty				(void);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif /* SList_h */
#endif /* SList_h */
#endif
