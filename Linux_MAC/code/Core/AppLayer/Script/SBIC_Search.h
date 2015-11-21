/*
 * Copyright (c) 2012-2014 
 * All rights reserved.
 *
 * File Name: SBIC_Search.h
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2012.12.24
 * @2013.7.10 :	add'','hex,'build,support in CompareCacl
 				change to Liunx style
 				use int32 to replace bool
 				use int32 to replace uint32
*/
#ifndef SBIC_SearchH
#define SBIC_SearchH
//------------------------------------------------------------------------------------------//
#include "SBIC_B.h"
//------------------------------------------------------------------------------------------//
class SBIC_Search : public SBIC_Node{
	public:
		enum{RFLAG_C = 0, RFLAG_S = SBIC_Node::RFLAG_S + SBIC_Node::RFLAG_C};
	public:
		SBIC_Search(void) : SBIC_Node() {
			cgCommand = "search,=";
			cgReturnCode = SBI_RETCODE_SEARCH;
		};
		virtual ~SBIC_Search(void){;};
	public:
		virtual	int32	Command	(SBICPAR *tBICPAR,const std::string &par,std::string *ret)const;
		virtual	int32	Help	(SBICPAR *tBICPAR,int32 blDetail = 1)const;
	private:
		static	void		FormatString	(const std::string &strInput,std::string *returnStr);
		static	std::string	SplitLetter		(const std::string &strSearch
											 ,std::string::size_type strSearchPostion
											 ,std::string::size_type *returnSearchPostion);
		static	int32		SplitSymbol		(const std::string &strSearch
											 ,std::string::size_type strSearchPostion
											 ,std::string::size_type *returnSearchPostion
											 ,std::string::size_type *symbolLength
											 ,int32 *blOnlyQuestionMark);
		static	int32		SearchSubString	(std::string *result
											 ,const std::string &strContent
											 ,std::string::size_type strContentPostion
											 ,const std::string &strSearch
											 ,std::string::size_type strSearchPostion
											 ,std::string::size_type *returnContentPostion);
		static	int32		SearchStringOnce(std::string *result
											 ,const std::string &strContent
											 ,std::string::size_type strContentPostion
											 ,const std::string &strSearch);
	public:
		static	int32		Search			(const std::string &strContent,const std::string &strSearch,std::string *retStr);
};
//------------------------------------------------------------------------------------------//
#endif
