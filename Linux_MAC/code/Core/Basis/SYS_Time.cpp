/*
 * Copyright (c) 2012-2014
 * All rights reserved.
 *
 * File Name: SYS_Time.cpp
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2012.02.09
 * @2013.12.11 : update Thread to make more safe.
 * @2013.06.30 : change to Liunx style
 add init method
 use int32 to replace bool
 use int32 to replace uint32
 @2014.05.24 : form PublicSYS.h
 */
//------------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "SYS_Time.h"
#include "Comm_Convert.h"
//------------------------------------------------------------------------------------------//
#include <time.h>
#ifdef CommonDefH_Unix
#include <sys/timeb.h>
#include <sys/time.h>
#endif
//------------------------------------------------------------------------------------------//
#ifndef DEFAULT_TIMEFORMATE
#define	DEFAULT_TIMEFORMATE	"hh:mm:ss.zzz"
#endif
//------------------------------------------------------------------------------------------//
double SYS_DateTime::gCompensation = 0.0;
//------------------------------------------------------------------------------------------//
std::string SYS_MakeTimeNow(void){
	SYS_DateTime	dtime;
	dtime.Now();
	return(SYS_FormatDateTime1970(DEFAULT_TIMEFORMATE,dtime));
}
//------------------------------------------------------------------------------------------//
std::string SYS_MakeTime(SYS_DateTime &tDateTime){
	return(SYS_FormatDateTime1970(DEFAULT_TIMEFORMATE,tDateTime));
}
//------------------------------------------------------------------------------------------//
const SYS_DateTime &SYS_Now(SYS_DateTime *tDateTime){
#ifdef CommonDefH_Unix
	gettimeofday(&(tDateTime->gDateTime),nullptr);
#endif
#ifdef CommonDefH_VC
	SYSTEMTIME	tTime;
	tDateTime->gDateTime = time(nullptr);
	GetLocalTime(&tTime);
	tDateTime->gMsec = tTime.wMilliseconds;
#endif
	if (tDateTime->gCompensation > 0)
		*tDateTime += tDateTime->gCompensation;
	if (tDateTime->gCompensation < 0)
		*tDateTime -= (0 - tDateTime->gCompensation);
	return(*tDateTime);
}
//------------------------------------------------------------------------------------------//
double SYS_GetSec(const SYS_DateTime &tDateTime){
#ifdef CommonDefH_Unix
	return((double)tDateTime.gDateTime.tv_sec + (double)tDateTime.gDateTime.tv_usec / 1000000.0);
#endif
#ifdef CommonDefH_VC
	return((double)tDateTime.gDateTime + (double)tDateTime.gMsec / 1000.0);
#endif
}
//------------------------------------------------------------------------------------------//
const SYS_DateTime &SYS_SetDateTime(SYS_DateTime *tRetDateTime,const double &sec){
#ifdef CommonDefH_MAC
	tRetDateTime->gDateTime.tv_sec = (__darwin_time_t)sec;
	tRetDateTime->gDateTime.tv_usec = (__darwin_suseconds_t)((sec - tRetDateTime->gDateTime.tv_sec) * 1000 * 1000);
#endif
#ifdef CommonDefH_Linux
	tRetDateTime->gDateTime.tv_sec = (time_t)sec;
	tRetDateTime->gDateTime.tv_usec = (suseconds_t)((sec - tRetDateTime->gDateTime.tv_sec) * 1000 * 1000);
#endif
#ifdef CommonDefH_VC
	tRetDateTime->gDateTime = (time_t)sec;
	tRetDateTime->gMsec = (uint32)((sec - tRetDateTime->gDateTime) * 1000);
#endif
	return(*tRetDateTime);
}
//------------------------------------------------------------------------------------------//
const SYS_DateTime &SYS_SetDateTime(SYS_DateTime *tRetDateTime,const SYS_DateTime &tDateTime){
#ifdef CommonDefH_Unix
	tRetDateTime->gDateTime.tv_sec = tDateTime.gDateTime.tv_sec;
	tRetDateTime->gDateTime.tv_usec = tDateTime.gDateTime.tv_usec;
#endif
#ifdef CommonDefH_VC
	tRetDateTime->gDateTime = tDateTime.gDateTime;
	tRetDateTime->gMsec = tDateTime.gMsec;
#endif
	return(*tRetDateTime);
}
//------------------------------------------------------------------------------------------//
const SYS_DateTime &SYS_MinusDateTime(SYS_DateTime *tRetDateTime,const double &sec){
#ifdef CommonDefH_Unix
#ifdef CommonDefH_MAC
	__darwin_time_t			tSec;
	__darwin_suseconds_t	tuSec;
	tSec = (__darwin_time_t)sec;
	tuSec = (__darwin_suseconds_t)((sec - tSec) * 1000 * 1000);
#endif
#ifdef CommonDefH_Linux
	time_t			tSec;
	suseconds_t		tuSec;
	tSec = (time_t)sec;
	tuSec = (suseconds_t)((sec - tSec) * 1000 * 1000);
#endif
	tRetDateTime->gDateTime.tv_sec -= tSec;
	
	if (tRetDateTime->gDateTime.tv_usec < tuSec){
		-- tRetDateTime->gDateTime.tv_sec;
		tRetDateTime->gDateTime.tv_usec += 1 * 1000 * 1000;
	}
	tRetDateTime->gDateTime.tv_usec -= tuSec;
#endif
#ifdef CommonDefH_VC
	time_t			tSec;
	uint32			tuSec;
	tSec = (time_t)sec;
	tuSec = (uint32)((sec - tSec) * 1000);

	tRetDateTime->gDateTime -= tSec;
	if (tRetDateTime->gMsec < tuSec){
		-- tRetDateTime->gDateTime;
		tRetDateTime->gMsec += 1 * 1000;
	}
	tRetDateTime->gMsec -= tuSec;
#endif
	return(*tRetDateTime);
}
//------------------------------------------------------------------------------------------//
const SYS_DateTime &SYS_MinusDateTime(SYS_DateTime *tRetDateTime,const SYS_DateTime &tDateTime){
#ifdef CommonDefH_Unix
	tRetDateTime->gDateTime.tv_sec -= tDateTime.gDateTime.tv_sec;
	if (tRetDateTime->gDateTime.tv_usec < tDateTime.gDateTime.tv_usec){
		-- tRetDateTime->gDateTime.tv_sec;
		tRetDateTime->gDateTime.tv_usec += 1 * 1000 * 1000;
	}
	tRetDateTime->gDateTime.tv_usec -= tDateTime.gDateTime.tv_usec;
#endif
#ifdef CommonDefH_VC
	tRetDateTime->gDateTime -= tDateTime.gDateTime;
	if (tRetDateTime->gMsec < tDateTime.gMsec){
		-- tRetDateTime->gDateTime;
		tRetDateTime->gMsec += 1 * 1000;
	}
	tRetDateTime->gMsec -= tDateTime.gMsec;
#endif
	return(*tRetDateTime);
}
//------------------------------------------------------------------------------------------//
const SYS_DateTime &SYS_MinusDateTime(SYS_DateTime *tRetDateTime,const SYS_DateTime &tStrartDateTime,const SYS_DateTime &tEndDateTime){
#ifdef CommonDefH_Unix
#ifdef CommonDefH_MAC
	__darwin_suseconds_t tv_usec;
#endif
#ifdef CommonDefH_Linux
	suseconds_t tv_usec
#endif
	tRetDateTime->gDateTime.tv_sec = tStrartDateTime.gDateTime.tv_sec - tEndDateTime.gDateTime.tv_sec;
	tv_usec = tStrartDateTime.gDateTime.tv_usec;
	if (tv_usec < tEndDateTime.gDateTime.tv_usec){
		-- tRetDateTime->gDateTime.tv_sec;
		tv_usec += 1 * 1000 * 1000;
	}
	tRetDateTime->gDateTime.tv_usec = tv_usec - tEndDateTime.gDateTime.tv_usec;
#endif
#ifdef CommonDefH_VC
	uint32 gMsec;
	tRetDateTime->gDateTime = tStrartDateTime.gDateTime - tEndDateTime.gDateTime;
	gMsec = tStrartDateTime.gMsec;
	if (gMsec < tEndDateTime.gMsec){
		-- tRetDateTime->gDateTime;
		gMsec += 1 * 1000;
	}
	tRetDateTime->gMsec = gMsec - tEndDateTime.gMsec;
#endif
	return(*tRetDateTime);
}
//------------------------------------------------------------------------------------------//
const SYS_DateTime &SYS_AddDateTime(SYS_DateTime *tRetDateTime,const double &sec){
#ifdef CommonDefH_Unix
#ifdef CommonDefH_MAC
	__darwin_time_t			tSec;
	__darwin_suseconds_t	tuSec;
	tSec = (__darwin_time_t)sec;
	tuSec = (__darwin_suseconds_t)((sec - tSec) * 1000 * 1000);
#endif
#ifdef CommonDefH_Linux
	time_t			tSec;
	suseconds_t		tuSec;
	tSec = (time_t)sec;
	tuSec = (suseconds_t)((sec - tSec) * 1000 * 1000);
#endif
	tRetDateTime->gDateTime.tv_sec += tSec;
	tRetDateTime->gDateTime.tv_usec += tuSec;
	if (tRetDateTime->gDateTime.tv_usec >= 1 * 1000 * 1000){
		++ tRetDateTime->gDateTime.tv_sec;
		tRetDateTime->gDateTime.tv_usec -= 1 * 1000 * 1000;
	}
#endif
#ifdef CommonDefH_VC
	time_t			tSec;
	uint32			tuSec;
	tSec = (time_t)sec;
	tuSec = (uint32)((sec - tSec) * 1000);
	
	tRetDateTime->gDateTime += tSec;
	tRetDateTime->gMsec += tuSec;
	if (tRetDateTime->gMsec >= 1 * 1000){
		++ tRetDateTime->gDateTime;
		tRetDateTime->gMsec -= 1 * 1000;
	}
#endif
	return(*tRetDateTime);
}
//------------------------------------------------------------------------------------------//
const SYS_DateTime &SYS_AddDateTime(SYS_DateTime *tRetDateTime,const SYS_DateTime &tDateTime){
#ifdef CommonDefH_Unix
	tRetDateTime->gDateTime.tv_sec += tDateTime.gDateTime.tv_sec;
	tRetDateTime->gDateTime.tv_usec += tDateTime.gDateTime.tv_usec;
	if (tRetDateTime->gDateTime.tv_usec >= 1 * 1000 * 1000){
		++ tRetDateTime->gDateTime.tv_sec;
		tRetDateTime->gDateTime.tv_usec -= 1 * 1000 * 1000;
	}
#endif
#ifdef CommonDefH_VC
	tRetDateTime->gDateTime += tDateTime.gDateTime;
	tRetDateTime->gMsec += tDateTime.gMsec;
	if (tRetDateTime->gMsec >= 1 * 1000){
		++ tRetDateTime->gDateTime;
		tRetDateTime->gMsec -= 1 * 1000;
	}
#endif
	return(*tRetDateTime);
}
//------------------------------------------------------------------------------------------//
const SYS_DateTime &SYS_AddDateTime(SYS_DateTime *tRetDateTime,const SYS_DateTime &tStrartDateTime,const SYS_DateTime &tEndDateTime){
#ifdef CommonDefH_Unix
#ifdef CommonDefH_MAC
	__darwin_suseconds_t tv_usec;
#endif
#ifdef CommonDefH_Linux
	suseconds_t tv_usec
#endif
	tRetDateTime->gDateTime.tv_sec = tStrartDateTime.gDateTime.tv_sec + tEndDateTime.gDateTime.tv_sec;
	tv_usec = tStrartDateTime.gDateTime.tv_usec + tEndDateTime.gDateTime.tv_usec;
	if (tv_usec >= 1 * 1000 * 1000){
		++ tRetDateTime->gDateTime.tv_sec;
		tv_usec -= 1 * 1000 * 1000;
	}
	tRetDateTime->gDateTime.tv_usec = tv_usec;
#endif
#ifdef CommonDefH_VC
	int32 gMsec;
	tRetDateTime->gDateTime = tStrartDateTime.gDateTime + tEndDateTime.gDateTime;
	gMsec = tStrartDateTime.gMsec + tEndDateTime.gMsec;
	if (gMsec >= 1 * 1000){
		++ tRetDateTime->gDateTime;
		gMsec -= 1 * 1000;
	}
	tRetDateTime->gMsec = gMsec;
#endif
	return(*tRetDateTime);
}
//------------------------------------------------------------------------------------------//
void SYS_DecodeDate1970(const SYS_DateTime &tDateTime,int32 *year,int32 *month,int32 *day,G_UTC_VAILD utc){
	struct tm time_tm;
	
#ifdef CommonDefH_Unix
	if (utc == G_UTC_OFF){
		localtime_r(&(tDateTime.gDateTime.tv_sec),&time_tm);  //½«Ãë×ª»»³Éstruct tmµÄÐÎÊ½
	}
	else{
		gmtime_r(&(tDateTime.gDateTime.tv_sec),&time_tm);  //½«Ãë×ª»»³Éstruct tmµÄÐÎÊ½
	}
#endif
#ifdef CommonDefH_VC
	if (utc == G_UTC_OFF){
		localtime_s(&time_tm,&(tDateTime.gDateTime));		//½«Ãë×ª»»³Éstruct tmµÄÐÎÊ½
	}
	else{
		gmtime_s(&time_tm,&(tDateTime.gDateTime));		//½«Ãë×ª»»³Éstruct tmµÄÐÎÊ½
	}
#endif
	*day = time_tm.tm_mday;					/*ÈÕ - È¡ÖµÇø¼äÎª[1, 31]*/
	*month = time_tm.tm_mon + 1;			/*ÔÂ·Ý - È¡ÖµÇø¼äÎª[0, 11]*/
	*year = time_tm.tm_year + 1900;			/*Äê·Ý - ÆäÖµÎª1900ÄêÖÁ½ñÄêÊý*/
}
//------------------------------------------------------------------------------------------//
void SYS_DecodeTime1970(const SYS_DateTime &tDateTime,int32 *hour,int32 *min,int32 *sec,int32 *msec,G_UTC_VAILD utc){
	struct tm time_tm;
	
#ifdef CommonDefH_Unix
	if (utc == G_UTC_OFF){
		localtime_r(&(tDateTime.gDateTime.tv_sec),&time_tm);  //½«Ãë×ª»»³Éstruct tmµÄÐÎÊ½
	}
	else{
		gmtime_r(&(tDateTime.gDateTime.tv_sec),&time_tm);  //½«Ãë×ª»»³Éstruct tmµÄÐÎÊ½
	}
	*msec = (int32)((int64)tDateTime.gDateTime.tv_usec / 1000);
#endif
#ifdef CommonDefH_VC
	if (utc == G_UTC_OFF){
		localtime_s(&time_tm,&(tDateTime.gDateTime));		//½«Ãë×ª»»³Éstruct tmµÄÐÎÊ½
	}
	else{
		gmtime_s(&time_tm,&(tDateTime.gDateTime));		//½«Ãë×ª»»³Éstruct tmµÄÐÎÊ½
	}
	*msec = tDateTime.gMsec;
#endif
	*sec = time_tm.tm_sec;		/*Ãë - È¡ÖµÇø¼äÎª[0, 59]*/
	*min = time_tm.tm_min;		/*·Ö - È¡ÖµÇø¼äÎª[0, 59]*/
	*hour = time_tm.tm_hour;	/*Ê± - È¡ÖµÇø¼äÎª[0, 23]*/
}
//------------------------------------------------------------------------------------------//
const SYS_DateTime &SYS_EncodeTime1970(SYS_DateTime *tDateTime,std::string sTime,G_UTC_VAILD utc){
	// YYYY/MM/DD,hh/mm/ss,zzz
	struct tm	time_tm;
	
	time_tm.tm_year = atoi(Str_ReadSubItem(&sTime,"/").c_str()) - 1900;
	time_tm.tm_mon = atoi(Str_ReadSubItem(&sTime,"/").c_str()) - 1;
	time_tm.tm_mday = atoi(Str_ReadSubItem(&sTime,",").c_str());
	time_tm.tm_hour = atoi(Str_ReadSubItem(&sTime,"/").c_str());
	time_tm.tm_min = atoi(Str_ReadSubItem(&sTime,"/").c_str());
	time_tm.tm_sec = atoi(Str_ReadSubItem(&sTime,",").c_str());
	
	time_tm.tm_wday = 0;
	time_tm.tm_yday = 0;
	time_tm.tm_isdst = 0;
#ifdef CommonDefH_Unix
	if (utc == G_UTC_OFF){
		tDateTime->gDateTime.tv_sec = mktime(&time_tm);
	}
	else{
		tDateTime->gDateTime.tv_sec = mktime(&time_tm);
		timeb tb;
		ftime(&tb);
		tDateTime->gDateTime.tv_sec -= (tb.timezone * 60);
	}
	tDateTime->gDateTime.tv_usec = atoi(sTime.c_str()) * 1000;
#endif
#ifdef CommonDefH_VC
	if (utc == G_UTC_OFF){
		tDateTime->gDateTime = _mktime64(&time_tm);
	}
	else{
		tDateTime->gDateTime = _mkgmtime64(&time_tm);
	}
	tDateTime->gMsec = atoi(sTime.c_str());
#endif
	return(*tDateTime);
}
//------------------------------------------------------------------------------------------//
void SYS_DecodeTimeABS(const SYS_DateTime &tDateTime,uint64 *hour,uint64 *min,uint64 *sec,uint64 *msec){
#ifdef CommonDefH_Unix
	*hour = tDateTime.gDateTime.tv_sec / (60 * 60);
	*min = (tDateTime.gDateTime.tv_sec % (60 * 60)) / 60;
	*sec = tDateTime.gDateTime.tv_sec % 60;
	*msec = (uint64)tDateTime.gDateTime.tv_usec / 1000;
#endif
#ifdef CommonDefH_VC
	*hour = (tDateTime.gDateTime / (60 * 60));
	*min = (tDateTime.gDateTime % (60 * 60)) / 60;
	*sec = (tDateTime.gDateTime % 60);
	*msec = tDateTime.gMsec;
#endif
}
//------------------------------------------------------------------------------------------//
const SYS_DateTime &SYS_EncodeTimeABS(SYS_DateTime *tDateTime,std::string sTime){
	// YYYY/MM/DD,hh/mm/ss,zzz
	uint32 year,mon,mday,hour,min,sec;
	
	year = atoi(Str_ReadSubItem(&sTime,"/").c_str()) * 365 * 24 * 60 * 60;
	mon = atoi(Str_ReadSubItem(&sTime,"/").c_str()) * 30 * 24 * 60 * 60;
	mday = atoi(Str_ReadSubItem(&sTime,",").c_str()) * 24 * 60 * 60;
	hour = atoi(Str_ReadSubItem(&sTime,"/").c_str()) * 60 * 60;
	min = atoi(Str_ReadSubItem(&sTime,"/").c_str()) * 60;
	sec = atoi(Str_ReadSubItem(&sTime,",").c_str());
	
#ifdef CommonDefH_Unix
	tDateTime->gDateTime.tv_sec = year + mon + mday + hour + min + sec;
	tDateTime->gDateTime.tv_usec = atoi(sTime.c_str()) * 1000;
#endif
#ifdef CommonDefH_VC
	tDateTime->gDateTime = year + mon + mday + hour + min + sec;
	tDateTime->gMsec = atoi(sTime.c_str());
#endif
	return(*tDateTime);
}
//------------------------------------------------------------------------------------------//
const SYS_DateTime &SYS_EncodeTimeABS(SYS_DateTime *tDateTime,const uint32 &hour,const uint32 &min,const uint32 &sec,const uint32 &msec){
#ifdef CommonDefH_Unix
	tDateTime->gDateTime.tv_sec = hour * 60 * 60 + min * 60 + sec;
	tDateTime->gDateTime.tv_usec = msec * 1000;
#endif
#ifdef CommonDefH_VC
	tDateTime->gDateTime = hour * 60 * 60 + min * 60 + sec;
	tDateTime->gMsec = msec;
#endif
	return(*tDateTime);
}
//------------------------------------------------------------------------------------------//
int32 SYS_CompareDateTime(const SYS_DateTime &tStrartDateTime,const SYS_DateTime &tEndDateTime){
#ifdef CommonDefH_Unix
	if (tStrartDateTime.gDateTime.tv_sec > tEndDateTime.gDateTime.tv_sec)
		return 1;
	if (tStrartDateTime.gDateTime.tv_sec < tEndDateTime.gDateTime.tv_sec)
		return -1;
	if (tStrartDateTime.gDateTime.tv_usec > tStrartDateTime.gDateTime.tv_usec)
		return 1;
	if (tStrartDateTime.gDateTime.tv_usec < tStrartDateTime.gDateTime.tv_usec)
		return -1;
	return 0;
#endif
#ifdef CommonDefH_VC
	if (tStrartDateTime.gDateTime > tEndDateTime.gDateTime)
		return 1;
	if (tStrartDateTime.gDateTime < tEndDateTime.gDateTime)
		return -1;
	if (tStrartDateTime.gMsec > tEndDateTime.gMsec)
		return 1;
	if (tStrartDateTime.gMsec < tEndDateTime.gMsec)
		return -1;
	return 0;
#endif
}
//------------------------------------------------------------------------------------------//
std::string SYS_GetTimeString(int32 flagType,int32 flagCount
							  ,const uint32 &year,const uint32 &month,const uint32 &day
							  ,const uint32 &hour,const uint32 &min,const uint32 &sec,const uint32 &msec){
	std::string	retResult,strTemp;
	retResult = "";
	if (flagCount > 0){
		switch(flagType){
			case 1:{//YY
				if (flagCount < 3){
					retResult = Str_IntToString(year % 100);
				}
				else{
					retResult = Str_IntToString(year);
				}
				break;
			}
			case 2:{//MM
				strTemp = Str_IntToString(month);
				if ((uint32)flagCount > strTemp.length()){
					strTemp.insert(0,flagCount - strTemp.length(),'0');
				}
				retResult += strTemp;
				break;
			}
			case 3:{//DD
				strTemp = Str_IntToString(day);
				if ((uint32)flagCount > strTemp.length()){
					strTemp.insert(0,flagCount - strTemp.length(),'0');
				}
				retResult += strTemp;
				break;
			}
			case 4:{//hh
				strTemp = Str_IntToString(hour);
				if ((uint32)flagCount > strTemp.length()){
					strTemp.insert(0,flagCount - strTemp.length(),'0');
				}
				retResult += strTemp;
				break;
			}
			case 5:{//mm
				strTemp = Str_IntToString(min);
				if ((uint32)flagCount > strTemp.length()){
					strTemp.insert(0,flagCount - strTemp.length(),'0');
				}
				retResult += strTemp;
			}break;
			case 6:{//ss
				strTemp = Str_IntToString(sec);
				if ((uint32)flagCount > strTemp.length()){
					strTemp.insert(0,flagCount - strTemp.length(),'0');
				}
				retResult += strTemp;
				break;
			}
			case 7:{//zzz
				strTemp = Str_IntToString(msec);
				if ((uint32)flagCount > strTemp.length()){
					strTemp.insert(0,flagCount - strTemp.length(),'0');
				}
				retResult += strTemp;
				break;
			}
			default:;
		};
	}
	return(retResult);
}
//------------------------------------------------------------------------------------------//
std::string SYS_FormatDateTime(const std::string &style
							   ,const uint32 &year,const uint32 &month,const uint32 &day
							   ,const uint32 &hour,const uint32 &min,const uint32 &sec,const uint32 &msec){
	//style Îª¸ñÊ½»¯×Ö·û´®,ÀýÈçYY/MM/DD,hh:mm:ss(zzz)\r\n";
	//YYÄê,MMÔÂ·Ý,DDÈÕÆÚ,hhÐ¡Ê±,mm·ÖÖÓ,ssÃë,zzzºÁÃë\r\n";
	std::string	ret;
	std::string::size_type length,i;
	int32	flag,flagCount,flagType;
	uint8	charP,charData;
	
	length = style.length();
	i = 0;
	flag = 0;
	flagCount = 0;
	flagType = 0;
	ret = "";
	while(i < length){
		charP = style[i];
		if ((i + 1) < length){//¹ýÂË\\,\"
			if (charP == '\\'){
				ret += SYS_GetTimeString(flagType,flagCount,year,month,day,hour,min,sec,msec);
				charData = style[i + 1];
				if (charData == '\\'){
					ret += "\\";
					i += 2;
					continue;
				}
				if (charData == '\"'){
					ret += "\"";
					i += 2;
					continue;
				}
			}
		}
		if (flag == 0){//Ã»ÓÐ½øÈë""ÐòÁÐ,¼ì²éYY,MM,DD,hh,mm,ss,zzz¹Ø¼ü×Ö
			switch(charP){
				case '\"':{
					flag = 1;
					++ i;
					continue;
				}
				case 'Y':{
					if (flagType != 1){
						ret += SYS_GetTimeString(flagType,flagCount,year,month,day,hour,min,sec,msec);
						flagType = 1;
						flagCount = 0;
					}
					++ flagCount;
					++ i;
					continue;
				}
				case 'M':{
					if (flagType != 2){
						ret += SYS_GetTimeString(flagType,flagCount,year,month,day,hour,min,sec,msec);
						flagType = 2;
						flagCount = 0;
					}
					++ flagCount;
					++ i;
					continue;
				}
				case 'D':{
					if (flagType != 3){
						ret += SYS_GetTimeString(flagType,flagCount,year,month,day,hour,min,sec,msec);
						flagType = 3;
						flagCount = 0;
					}
					++ flagCount;
					++ i;
					continue;
				}
				case 'h':{
					if (flagType != 4){
						ret += SYS_GetTimeString(flagType,flagCount,year,month,day,hour,min,sec,msec);
						flagType = 4;
						flagCount = 0;
					}
					++ flagCount;
					++ i;
					continue;
				}
				case 'm':{
					if (flagType != 5){
						ret += SYS_GetTimeString(flagType,flagCount,year,month,day,hour,min,sec,msec);
						flagType = 5;
						flagCount = 0;
					}
					++ flagCount;
					++ i;
					continue;
				}
				case 's':{
					if (flagType != 6){
						ret += SYS_GetTimeString(flagType,flagCount,year,month,day,hour,min,sec,msec);
						flagType = 6;
						flagCount = 0;
					}
					++ flagCount;
					++ i;
					continue;
				}
				case 'z':{
					if (flagType != 7){
						ret += SYS_GetTimeString(flagType,flagCount,year,month,day,hour,min,sec,msec);
						flagType = 7;
						flagCount = 0;
					}
					++ flagCount;
					++ i;
					continue;
				}
				default:;
			};
			if (flagType != 0){
				ret += SYS_GetTimeString(flagType,flagCount,year,month,day,hour,min,sec,msec);
				flagType = 0;
				flagCount = 0;
			}
		}
		else{
			if (charP == '\"'){
				flag = 0;
				flagCount = 0;
				flagType = 0;
				++ i;
				continue;
			}
		}
		++ i;
		ret += charP;
	}
	ret += SYS_GetTimeString(flagType,flagCount,year,month,day,hour,min,sec,msec);
	return(ret);
}
//------------------------------------------------------------------------------------------//
std::string SYS_FormatDateTime1970(const std::string &style,const SYS_DateTime &tDateTime,G_UTC_VAILD utc){
	//style Îª¸ñÊ½»¯×Ö·û´®,ÀýÈçYY/MM/DD,hh:mm:ss(zzz)\r\n";
	//YYÄê,MMÔÂ·Ý,DDÈÕÆÚ,hhÐ¡Ê±,mm·ÖÖÓ,ssÃë,zzzºÁÃë\r\n";
	int32	year,month,day,hour,min,sec,msec;
	
	SYS_DecodeDate1970(tDateTime,&year,&month,&day,utc);
	SYS_DecodeTime1970(tDateTime,&hour,&min,&sec,&msec,utc);
	
	return(SYS_FormatDateTime(style,year,month,day,hour,min,sec,msec));
}
//------------------------------------------------------------------------------------------//
std::string SYS_FormatDateTime1900(const std::string &style,SYS_DateTime tDateTime,G_UTC_VAILD utc){
	std::string	ret;
	int32	year,month,day,hour,min,sec,msec;
	
	tDateTime -= (70LL * 365 + 17) * 24 * 60 * 60;
	SYS_DecodeDate1970(tDateTime,&year,&month,&day,utc);
	SYS_DecodeTime1970(tDateTime,&hour,&min,&sec,&msec,utc);
	return(SYS_FormatDateTime(style,year,month,day,hour,min,sec,msec));
}
//------------------------------------------------------------------------------------------//
std::string SYS_FormatTimeABS(const std::string &style,const SYS_DateTime &tDateTime){
	uint64	hour,min,sec,msec;
	
	SYS_DecodeTimeABS(tDateTime,&hour,&min,&sec,&msec);
	
	return(SYS_FormatDateTime(style,0,0,0,(uint32)hour,(uint32)min,(uint32)sec,(uint32)msec));
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
uint64 SYS_ASCIIStrToNTP(std::string strInput){
	uint8		charData;
	uint64		ntp;
	
	charData = strInput[0];
	ntp = charData;
	charData = strInput[1];
	ntp = (ntp << 8) | charData;
	charData = strInput[2];
	ntp = (ntp << 8) | charData;
	charData = strInput[3];
	ntp = (ntp << 8) | charData;
	charData = strInput[4];
	ntp = (ntp << 8) | charData;
	charData = strInput[5];
	ntp = (ntp << 8) | charData;
	charData = strInput[6];
	ntp = (ntp << 8) | charData;
	charData = strInput[7];
	ntp = (ntp << 8) | charData;
	
	return(ntp);
}
//------------------------------------------------------------------------------------------//
std::string SYS_NTPToHEXStr(uint64 ntp){
	uint8			charData[8];
	
	charData[7] = (uint8)ntp;
	ntp = ntp >> 8;
	charData[6] = (uint8)ntp;
	ntp = ntp >> 8;
	charData[5] = (uint8)ntp;
	ntp = ntp >> 8;
	charData[4] = (uint8)ntp;
	ntp = ntp >> 8;
	charData[3] = (uint8)ntp;
	ntp = ntp >> 8;
	charData[2] = (uint8)ntp;
	ntp = ntp >> 8;
	charData[1] = (uint8)ntp;
	ntp = ntp >> 8;
	charData[0] = (uint8)ntp;
	
	return(Str_CharToHEXStr(charData, 8, G_SPACE_ON));
}
//------------------------------------------------------------------------------------------//
void SYS_NTPToChar(uint64 ntp,uint8 *buffer){
	
	buffer += 7;
	
	*buffer = (uint8)ntp;
	ntp = ntp >> 8;
	--buffer;
	*buffer = (uint8)ntp;
	ntp = ntp >> 8;
	--buffer;
	*buffer = (uint8)ntp;
	ntp = ntp >> 8;
	--buffer;
	*buffer = (uint8)ntp;
	ntp = ntp >> 8;
	--buffer;
	*buffer = (uint8)ntp;
	ntp = ntp >> 8;
	--buffer;
	*buffer = (uint8)ntp;
	ntp = ntp >> 8;
	--buffer;
	*buffer = (uint8)ntp;
	ntp = ntp >> 8;
	--buffer;
	*buffer = (uint8)ntp;
}
//------------------------------------------------------------------------------------------//
uint64 SYS_CharToNTP(const uint8 *buffer){
	uint64		ntp;
	
	ntp = *buffer;
	++buffer;
	ntp = (ntp << 8) | *buffer;
	++buffer;
	ntp = (ntp << 8) | *buffer;
	++buffer;
	ntp = (ntp << 8) | *buffer;
	++buffer;
	ntp = (ntp << 8) | *buffer;
	++buffer;
	ntp = (ntp << 8) | *buffer;
	++buffer;
	ntp = (ntp << 8) | *buffer;
	++buffer;
	ntp = (ntp << 8) | *buffer;
	
	return(ntp);
}
//------------------------------------------------------------------------------------------//
uint64 SYS_TimeToNTP(const double &sec){
	uint64		ret, u64_sec, u64_nsec;
	
	u64_sec = (uint64)(sec);
	u64_nsec = (uint64)((sec - u64_sec) * 1000 * 1000 * 1000);
	/* Convert nanoseconds to 32-bits fraction (232 picosecond units) */
	ret = ((u64_nsec) << 32) / (1000 * 1000 * 1000);
	
	//There is 70 years (incl. 17 leap ones) offset to the Unix Epoch.
	//No leap seconds during that period since they were not invented yet.
	
	ret |= ((70LL * 365 + 17) * 24 * 60 * 60 + u64_sec) << 32;
	
	return(ret);
}
//------------------------------------------------------------------------------------------//
double SYS_NTPToTime(const uint64 &ntp){
	uint64		u64_sec, u64_nsec;
	double		ret;
	//There is 70 years (incl. 17 leap ones) offset to the Unix Epoch.
	//No leap seconds during that period since they were not invented yet.
	if (ntp == 0)
		return 0.0;
	u64_sec = (ntp >> 32) - (70LL * 365 + 17) * 24 * 60 * 60;
	u64_nsec = ((ntp & (uint64)0xffffffff) * 1000 * 1000 * 1000) >> 32;
	ret = (double)u64_sec + (double)u64_nsec / (1000.0 * 1000.0 * 1000.0);
	return(ret);
}
//------------------------------------------------------------------------------------------//
