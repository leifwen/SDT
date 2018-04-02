//
//  Script.h
//  SDT
//
//  Created by Leif Wen on 22/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "Device.h"
#include "BIF_CMD.h"
#include "Commu_AEXE.h"
#if defined Device_h && defined BIF_CMD_h && defined GList_h
//------------------------------------------------------------------------------------------//
#ifndef Script_h
#define Script_h
#ifdef Script_h
//------------------------------------------------------------------------------------------//
#define TELIT_AT_RESPONSE \
"[\r\nOK\r\n\
|| \r\nCONNECT\r\n\
|| \r\nRING\r\n\
|| \r\nNO CARRIER\r\n\
|| \r\nERROR\r\n\
|| +CME ERROR:\
|| \r\n+CME ERROR:\
|| \r\nCONNECT *\r\n\
|| \r\nNO DIALTONE\r\n\
|| \r\nBUSY\r\n\
|| \r\nNO ANSWER\r\n]"
//------------------------------------------------------------------------------------------//
class SCRIPT : public DEVICE_EXE_FRAME{
	protected:
		enum	{RFLAG_C = 1, RFLAG_S = DEVICE_EXE_FRAME::RFLAG_S + DEVICE_EXE_FRAME::RFLAG_C};
		enum	{SRC_blATResponse = RFLAG_CREATE(0)};
	public:
				 SCRIPT(uint32 size);
		virtual ~SCRIPT(void);
	private:
		virtual	void	Init				(const DEVICE* dev);
		virtual	void	SetblIsTerminated	(void);
		virtual	void	ClrblIsTerminated	(void);
				void	PrintB				(CMD_ENV* env,const STDSTR& str);
				void	PrintP				(CMD_ENV* env,const STDSTR& str);
				void	PrintM				(CMD_ENV* env,const STDSTR& strM,const STDSTR& strB = "");
	private:
		SYS_Thread<SCRIPT>	eGroupListThread;
		SYS_Thread<SCRIPT>	eGroupThread;
		SYS_Thread<SCRIPT>	eCommandThread;
	private:
		CMD_ENV			cgENV;
		BIF_CMD			cgBIFCMD;
	
		BIF_REPLACE		cgSubC_Replace;
#ifdef Commu_AEXE_h
		AEXEPOOL		cgAExePool;
#endif
		STDSTR			cgATCondition;
		GC_LIST			cgMGroupList;
		COMMAND_GROUP	cgMGroup;
		COMMAND_NODE	cgMCommand;
	private:
				bool32	EGroupListThreadFun	(void* p);
				bool32	EGroupThreadFun		(void* p);
				bool32	ECommandThreadFun	(void* p);
	
				void	PrintGroupResult	(OUTPUT_NODE* cOut,COMMAND_GROUP* tGroup,const STDSTR& strGroupName);
				void    ShowResult			(COMMAND_GROUP *tGroup,const STDSTR& tExecuteTime);
				void	ShowResult			(GC_LIST* tGroupList,const STDSTR& tExecuteTime);
	
				bool32	ExecuteGroupList	(GC_LIST* tGroupList,const int32& runTotalTimes);
				bool32	ExecuteGroup		(COMMAND_GROUP* tGroup,const int32& runTotalTimes,bool32 blType);
				bool32	ExecuteCommand		(COMMAND_NODE* tCommand,int32 frameTimeout);
	public:
				void	SetblPrintSBICinfo	(void);
				void	ClrblPrintSBICinfo	(void);
				bool32	CheckPrintSBICinfo	(void);

				void	SetblCommandExplain	(void);
				void	ClrblCommandExplain	(void);
				bool32	CheckCommandExplain	(void);
		
				void	SetblATResponse		(void);
				void	ClrblATResponse		(void);
				bool32	CheckATResponse		(void);

	public:
		virtual	void	Stop				(void);
				void	Help				(OUTPUT_NODE* oDevNode);
	public:
				bool32	Execute				(const DEVICE* dev,const GC_LIST* tGroupList);
				bool32	Execute				(const DEVICE* dev,const COMMAND_GROUP* tGroup);
				bool32	Execute				(const DEVICE* dev,const COMMAND_NODE* tCommand);
};
//------------------------------------------------------------------------------------------//
#else
#undef SWVERSION_SCRIPT
#endif /* Script_h */
#endif /* Script_h */
#else
#undef SWVERSION_SCRIPT
#endif /* Device_h */
