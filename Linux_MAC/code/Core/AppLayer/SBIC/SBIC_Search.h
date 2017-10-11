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
//------------------------------------------------------------------------------------------//
#include "SBIC_B.h"
//------------------------------------------------------------------------------------------//
#ifdef SBIC_BH
#ifndef SBIC_SearchH
#define SBIC_SearchH
#ifdef SBIC_SearchH
//------------------------------------------------------------------------------------------//
class SBIC_Search : public SBIC_Node{
	public:
		SBIC_Search(void) : SBIC_Node() {
			cgCommand = "search,=";
			cgReturnCode = SBI_RETCODE_SEARCH;
		};
		virtual ~SBIC_Search(void){;};
	public:
		virtual	int32	Command	(SBIC_ENV *env,const STDSTR &par,void *retTrue)const;
		virtual	int32	Help	(SBIC_ENV *env,int32 blDetail = 1)const{
			PrintB(env,".CMD = Search=<Expression> -->Search expression in received data, and save the result.");
			PrintB(env,"  Command = <'Search=<Expression>>[//COMMENT]");
			PrintP(env,"  Notes: Expression's operators is * and ?, and only support ASCII format.");
			PrintP(env,"     1.* means any character, ? means one character, support multiple * and ? combination.");
			PrintP(env,"     2.The results returned are the data replaced by * and ?.");
			PrintP(env,"     3.Only record the first match of the results.");
			PrintP(env,"     4.Use 'RET (x) to get the results.");
			PrintP(env,"     5.support \\0xhh, \\0Xhh, \\a, \\b, \\f, \\n, \\r, \\t, \\v, \\\\, \\', \\\", \\0, \\/, \\*, \\?.");
			PrintP(env,"   eg:");
			PrintP(env,"    Received data:HappyNewYear");
			PrintP(env,"      Command = 'Search=Happy*Year     //Search Happy*Year, and record New as the result");
			PrintP(env,"      'RET(1)=New");
			PrintP(env,"    Received data:Name: Leif Wen.MP: +86-138-6062-4143.");
			PrintP(env,"      Command = 'Search=Name: *.MP: *. //Search Name: *.MP: *., and record Leif Wen and +86-138-6062-4143 as result");
			PrintP(env,"      'RET(1)=Leif Wen");
			PrintP(env,"      'RET(2)=+86-138-6062-4143");
			return(cgReturnCode);
		};
	private:
		static	void	FormatString	(STDSTR *returnStr,const STDSTR &strIn);
		static	STDSTR	SplitLetter		(STDSTR::size_type *offset,const STDSTR &strSearch);
		static	int32	SplitSymbol		(const STDSTR &strSearch
											 ,STDSTR::size_type *offset
											 ,STDSTR::size_type *symbolLength
											 ,int32 *blOnlyQuestionMark);
		static	int32	SearchSubString	(STDSTR *result
											 ,const STDSTR &strContent
											 ,STDSTR::size_type strContentPostion
											 ,const STDSTR &strSearch
											 ,STDSTR::size_type strSearchPostion
											 ,STDSTR::size_type *returnContentPostion);
		static	int32	SearchStringOnce(STDSTR *result
											 ,const STDSTR &strContent
											 ,STDSTR::size_type strContentPostion
											 ,const STDSTR &strSearch);
	public:
		static	int32	Search			(STDSTR *retStr,const STDSTR &strContent,const STDSTR &searchString);
};
//------------------------------------------------------------------------------------------//
#endif
#endif
#endif
