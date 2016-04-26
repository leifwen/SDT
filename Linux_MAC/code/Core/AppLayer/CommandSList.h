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

#ifndef CommandSlistH
#define CommandSlistH
//------------------------------------------------------------------------------------------//
#include "Comm_Tree.h"
//------------------------------------------------------------------------------------------//
class SC_NODE : public RTREE_NODE{
	private:
	public:
				 SC_NODE(void) : RTREE_NODE(){Init();};
		virtual ~SC_NODE(void){;};
	public://need record
		std::string			StrCommand;
		int32				blEnableSendCR;//send "\r"
	public:
		void 		Init(G_LOCK_VAILD blLock = G_LOCK_ON);
		std::string	CreateNodeStrV0_2(void);
		std::string	CreateNodeStrV0_3(void){return(CreateNodeStrV0_2());};
		std::string	CreateNodeStrV0_4(void);
		std::string	CreateNodeStrV0_5(void){return(CreateNodeStrV0_4());};
		void		SetNodeV0_2(std::string *strInput);
		void		SetNodeV0_3(std::string *strInput){SetNodeV0_2(strInput);};
		void		SetNodeV0_4(std::string *strInput);
		void		SetNodeV0_5(std::string *strInput){SetNodeV0_4(strInput);};
		static void			Copy(SC_NODE *node2,SC_NODE *node1);//copy 2 to 1
};
//------------------------------------------------------------------------------------------//
class SC_LIST : public RTREE_NODE{
	public:
				 SC_LIST(void){CreateTrash(this);};
		virtual ~SC_LIST(void){DestroyAll();};
	public:
		std::string	CreateSCListStrV0_2(void);
		std::string	CreateSCListStrV0_3(void){return(CreateSCListStrV0_2());};
		std::string	CreateSCListStrV0_4(void);
		std::string	CreateSCListStrV0_5(void){return(CreateSCListStrV0_4());};
		void		SetSCListV0_2(std::string *strInput);
		void		SetSCListV0_3(std::string *strInput){SetSCListV0_2(strInput);};
		void		SetSCListV0_4(std::string *strInput);
		void		SetSCListV0_5(std::string *strInput){SetSCListV0_4(strInput);};
	public:
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif

