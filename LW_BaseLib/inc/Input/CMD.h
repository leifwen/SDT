//
//  CMD.h
//  SDT
//
//  Created by Leif Wen on 06/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

//------------------------------------------------------------------------------------------//
#include "DS_BaseClass.h"
#include "DS_Tree.h"
//------------------------------------------------------------------------------------------//
#ifdef DS_Tree_h
#ifndef CMD_h
#define CMD_h
#ifdef CMD_h
//------------------------------------------------------------------------------------------//
#ifndef Output_h
class OUTPUT_NODE;
#endif

#define CMD_SETVAR(name,vid,vidType)\
static	inline	void		name		(CMD_ENV* env,vidType var){CMD_ENV::SetVar(env,vid,(void*)var);};
#define CMD_VAR(name,vid,retType)\
static	inline	retType&	name		(CMD_ENV* env){return(*CMD_ENV::GetVar(env,vid,(retType*)nullptr));};
//------------------------------------------------------------------------------------------//
class CMD_ENV : public BASE_FLAG{
	public:
		enum	{RFLAG_C = 2, RFLAG_S = BASE_FLAG::RFLAG_S + BASE_FLAG::RFLAG_C};
		enum	{ENV_blExitLock = RFLAG_CREATE(0),ENV_blExit = RFLAG_CREATE(1),};
	public:
		enum	{CMD_VID_STDOUT = 0,CMD_VID_NEXT,};
	public:
				 CMD_ENV(void);
				~CMD_ENV(void);
	private:
		void			UnInitQuantity(void);
	public:
		inline	void	ClearEnvAllFlag	(void);
		inline	void	SetEnvFlag		(const uint64& rStatus);
		inline	void	ClrEnvFlag		(const uint64& rStatus);
		inline	bool32	CheckEnvFlag	(const uint64& rStatus)const;
	public:
		inline	void	SetblExitForce	(void);
		inline	void	SetblExit		(void);
		inline	void	ClrblExit		(void);
		inline	bool32	IsExit			(void)const;
	private:
		void**			buffer;
		uint32			bufferSize;
	public:
												void			InitQuantity(uint32	num);
								static	inline  void			Init		(CMD_ENV* env);
												void			CopyInit	(const CMD_ENV* env);
								static	inline	void			SetVar		(CMD_ENV* env,uint32 vid,void* var);
		template<typename VART> static	inline	VART*			GetVar		(CMD_ENV* env,uint32 vid,VART* varnullptr);
		template<typename VART> static	inline	void			NewVar		(CMD_ENV* env,uint32 vid,VART* varnullptr);
		template<typename VART> static	inline	void			DelVar		(CMD_ENV* env,uint32 vid,VART* varnullptr);
	public:
								static	inline	void			SetSTDOUT	(CMD_ENV* env,OUTPUT_NODE* var);
								static	inline	OUTPUT_NODE*	GetSTDOUT	(CMD_ENV* env);
};
//------------------------------------------------------------------------------------------//
class CMD_BASE : public TNFP{
	public:
				 CMD_BASE(void) : TNFP(){;};
		virtual ~CMD_BASE(void){;};
	public:
		static	inline		void		ClearEnvAllFlag				(CMD_ENV* env);
		static	inline		void		SetEnvFlag					(CMD_ENV* env,const uint64& rStatus);
		static	inline		void		ClrEnvFlag					(CMD_ENV* env,const uint64& rStatus);
		static	inline		bool32		CheckEnvFlag				(CMD_ENV* env,const uint64& rStatus);
	public:
		static	inline		void		SetblExitForce				(CMD_ENV* env);
		static	inline		void		SetblExit					(CMD_ENV* env);
		static	inline		void		ClrblExit					(CMD_ENV* env);
		static	inline		bool32		IsExit						(CMD_ENV* env);
	
		static				void		DelayMS						(CMD_ENV* env,uint32 timeMS);
	public:
		static	inline		void		SplitPar1					(STDSTR& strPar1,STDSTR& strPar2,const STDSTR& par,const STDSTR& split = " ");
		static	inline		void		SplitPar1					(STDSTR* strPar1,STDSTR* strPar2,const STDSTR& split = " ");
		static				STDSTR		DelComment					(const STDSTR& strIn);
		static				uint32		GetMS						(const STDSTR& strIn);
		static				STDSTR		GetMSSTR					(uint32 ms);
	public:
		static inline	OUTPUT_NODE*	GetSTDOUT					(CMD_ENV* env);
	
		static inline		bool32		IsPrintEnable				(CMD_ENV* env);
		static inline		void 		PrintEnable					(CMD_ENV* env);
		static inline		void 		PrintDisable				(CMD_ENV* env);
		static inline		void 		CleanLastLine				(CMD_ENV* env);
	
		template<typename... Args>	static void 	PrintStr				(CMD_ENV* env,const Args&... args);
		template<typename... Args>	static void 	PrintStrNL				(CMD_ENV* env,const Args&... args);
		template<typename... Args>	static void 	PrintALine				(CMD_ENV* env,const Args&... args);
		template<typename... Args>	static void 	PrintALineDot			(CMD_ENV* env,const Args&... args);
		template<typename... Args>	static void		PrintWithTime			(CMD_ENV* env,const Args&... args);
		template<typename... Args>	static void		PrintWithTime_noNL		(CMD_ENV* env,const Args&... args);
		template<typename... Args>	static void 	PrintWithDividingLine	(CMD_ENV* env,const Args&... args);
};
//------------------------------------------------------------------------------------------//
typedef	int32	CMDID;
//------------------------------------------------------------------------------------------//
class CMD_NODE : public CMD_BASE{
	protected:
		enum	{RFLAG_C = 2, RFLAG_S = CMD_BASE::RFLAG_S + CMD_BASE::RFLAG_C};
		enum	{CMD_blTrySubCMD = RFLAG_CREATE(0),CMD_blHidenHelp = RFLAG_CREATE(1)};
	public:
		enum	{CMD_ID_EXIT = -1,CMD_ID_NO = 0,CMD_ID_HELP,CMD_ID_NEXT,};
	public:
				 CMD_NODE(void);
		virtual ~CMD_NODE(void){;};
	protected:
		CMDID			cgCommandID;
		STDSTR			cgCommand;
	public:
						void	SetHidenHelp			(void);
						void	ClrHidenHelp			(void);
	public:
				virtual	CMDID	TraversalChildExecute	(CMD_ENV* env,const uint32& mID,const STDSTR& msg,void* p)const;
				virtual	CMDID	TraversalChildHelp		(CMD_ENV* env,uint32 flag)const;
	protected:
						CMDID	ResolveIDFun			(STDSTR** retMsg,const STDSTR& rawIn)const;
						CMDID	ResolveIDCMD			(STDSTR** retMsg,const STDSTR& rawIn)const;
				virtual	CMDID	ResolveID				(STDSTR** retMsg,const STDSTR& rawIn)const;
				virtual	CMDID	MessageProcessing		(CMD_ENV* env,const uint32& mID,const STDSTR& msg,void* p)const;
	public:
				virtual	CMDID	Dispose					(CMD_ENV* env,const STDSTR& rawIn,void* p)const;
				virtual	CMDID	Execute					(CMD_ENV* env,const uint32& mID,const STDSTR& msg,void* p)const;
				virtual	CMDID	Command					(CMD_ENV* env,const STDSTR& msg,void* p)const;
				virtual	CMDID	Help					(CMD_ENV* env,uint32 flag)const;
};
//------------------------------------------------------------------------------------------//
template <typename T_FUN_CLASS> class CMD_NODE_T : public CMD_NODE{
	typedef CMDID(T_FUN_CLASS::*T_FUN)(CMD_ENV*,const uint32&,const STDSTR&,void*);
	public:
				 CMD_NODE_T(void);
		virtual ~CMD_NODE_T(void){;};
	protected:
		T_FUN_CLASS*	cgFunClass;
		T_FUN			cgFun;
	public:
		void	InitFun				(T_FUN_CLASS* funClass,T_FUN fun);
	protected:
		CMDID	MessageProcessing	(CMD_ENV *env,const uint32& mID,const STDSTR& msg,void* p)const;
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#define CMD_CLASSTYPE(_NAME,_PUB,_ID,_CMD)\
	class _NAME : public _PUB{\
		public:\
					 _NAME(void) : _PUB() {cgCommandID = _ID;cgCommand = _CMD;};\
			virtual ~_NAME(void){;};\
		public:\
			virtual	CMDID	Command		(CMD_ENV* env,const STDSTR& msg,void* p)const;\
			virtual	CMDID	Help		(CMD_ENV* env,uint32 flag)const;\
	};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#include "CMD.hpp"
#endif /* CMD_h */
#endif /* CMD_h */
#endif
