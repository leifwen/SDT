//
//  BuildInCommand.cpp
//  SDT
//
//  Created by Leif Wen on 01/05/2017.
//  Copyright © 2017 leif wen. All rights reserved.
//

//------------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "BuildInCommand.h"
#include "Comm_Convert.h"
//------------------------------------------------------------------------------------------//
#ifdef BIC_CMDH
//------------------------------------------------------------------------------------------//
int32 BuildInCommand::Do(BIC_ENV *env){
	STDSTR		strCommand;
	int32		ret;
	
	ClrblExit(env);
	PrintTitle(env,env->eda->csType,1);
	if (BI_ReadCommand(env,&strCommand) == 0)
		return(BI_RETCODE_NO);
	
	ret = BI_RETCODE_NO;
	
	if (strCommand.length() > 0){
		PrintStr(env,"\n");
		SYS_SleepMS(10);
		ret = ExecuteAsConsole(env,strCommand,"",env->eda);
	#ifdef BIC_EXE_hpp
		if (ret == BI_RETCODE_NO)
			ret = cgC_EXE_BASH.Command(env,strCommand,nullptr);
	#endif
	}
	if (env->blDisplayAuto != 0)
		env->cstdout->PrintEnable();
	if (env->blExit == BI_RETCODE_EXIT)
		return BI_RETCODE_EXIT;
	return(ret);
}
//------------------------------------------------------------------------------------------//
#endif
