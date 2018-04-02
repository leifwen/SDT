//------------------------------------------------------------------------------------------//
int32 CMainFrame::CheckCOMInput(std::string strInput,std::string *strOutput){
	std::string	result,strData;
	uint32		comNumber;
	
	result = Str_Trim(strInput);	
	strData = Str_UpperCase(result.substr(0,3));
	if (strData == "COM"){
		result = result.substr(3);
		comNumber = atoi(result.c_str());
		if ((comNumber > 0) && (comNumber <= 256)){
			*strOutput = Str_UpperCase(Str_Trim(strInput));
			return 1;
		}
	}
	*strOutput = "";
	return 0;
}
//------------------------------------------------------------------------------------------//
int32 CMainFrame::CheckTCPInput(std::string strInput,std::string *strOutput){
	std::string	result,strData;
	
	result = Str_Trim(strInput);	
	strData = Str_UpperCase(result.substr(0,4));
	if (strData == "TCP:"){
		*strOutput = Str_UpperCase(Str_Trim(result.substr(4)));
		return 1;
	}
	*strOutput = "";
	return 0;
}
//------------------------------------------------------------------------------------------//
int32 CMainFrame::CheckUDPInput(std::string strInput,std::string *strOutput){
	std::string	result,strData;
	
	result = Str_Trim(strInput);	
	strData = Str_UpperCase(result.substr(0,4));
	if (strData == "UDP:"){
		*strOutput = Str_UpperCase(Str_Trim(result.substr(4)));
		return 1;
	}
	*strOutput = "";
	return 0;
}
//------------------------------------------------------------------------------------------//
int32 CMainFrame::CheckTCPServerInput(std::string strInput,std::string *strOutput){
	*strOutput = "";
	if (Str_UpperCase(Str_Trim(strInput)) == "TCP SERVER")
		return 1;
	return 0;
}
//------------------------------------------------------------------------------------------//
int32 CMainFrame::CheckUDPServerInput(std::string strInput,std::string *strOutput){
	*strOutput = "";
	if (Str_UpperCase(Str_Trim(strInput)) == "UDP SERVER")
		return 1;
	return 0;
}
//------------------------------------------------------------------------------------------//
uint32 CMainFrame::CheckIPComInput(std::string strInput, std::string *strOutput){
	std::string strTemp;
#ifdef SWVERSION_COM
	if (CheckCOMInput(strInput,strOutput) != 0)
		return(OPEN_COM);
#endif
#ifdef	SWVERSION_TCPClient
	if (CheckTCPInput(strInput,strOutput) != 0)
		return(OPEN_TCP);
#endif
#ifdef	SWVERSION_UDPCLient
	if (CheckUDPInput(strInput,strOutput) != 0)
		return(OPEN_UDP);
#endif
#ifdef	SWVERSION_TCPServer
	if (CheckTCPServerInput(strInput,strOutput) != 0)
		return(OPEN_TCPS);
#endif
#ifdef	SWVERSION_UDPServer
	if (CheckUDPServerInput(strInput,strOutput) != 0)
		return(OPEN_UDPS);
#endif
	*strOutput = strInput;
	return(OPEN_None);
}
//------------------------------------------------------------------------------------------//