//
//  SYS_Time.cpp
//  SDT
//
//  Created by Leif Wen on 23/01/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "stdafx.h"
//------------------------------------------------------------------------------------------//
#include "SYS_Time.h"
#include "DS_STRING.h"
//------------------------------------------------------------------------------------------//
#ifdef SYS_Time_h
//------------------------------------------------------------------------------------------//
#ifndef DEFAULT_TIMEFORMATE
#define	DEFAULT_TIMEFORMATE	"[hh:mm:ss.zzz]"
#endif
//------------------------------------------------------------------------------------------//
double DTIME::gCompensation = 0.0;
//------------------------------------------------------------------------------------------//
STDSTR SYS_MakeTimeNow(void){
	DTIME	dtime;
	dtime.Now();
	return(SYS_FormatDateTime1970(DEFAULT_TIMEFORMATE,dtime));
};
//------------------------------------------------------------------------------------------//
STDSTR SYS_MakeTime(DTIME& tDateTime){
	return(SYS_FormatDateTime1970(DEFAULT_TIMEFORMATE,tDateTime));
};
//------------------------------------------------------------------------------------------//
DTIME::DTIME(void){
	Clear();
};
//------------------------------------------------------------------------------------------//
DTIME::DTIME(const DTIME& C){
	gTime.tv_sec = C.gTime.tv_sec;
	gTime.tv_usec = C.gTime.tv_usec;
};
//------------------------------------------------------------------------------------------//
DTIME& DTIME::Now(void){
#ifdef CommonDefH_Unix
	timeval t;
	gettimeofday(&t,nullptr);
	gTime.tv_sec = t.tv_sec;
	gTime.tv_usec = t.tv_usec;
#endif
#ifdef CommonDefH_VC
	SYSTEMTIME	tTime;
	gTime.tv_sec = time(nullptr);
	GetLocalTime(&tTime);
	gTime.tv_usec = tTime.wMilliseconds * 1000;
#endif
	*this += gCompensation;
	return(*this);
};
//------------------------------------------------------------------------------------------//
DTIME& DTIME::Minus(const double& second){
	struct	TIMEVAL64	dtime;
	
	dtime.tv_sec = (uint64)second;
	dtime.tv_usec = (uint64)((second - dtime.tv_sec) * 1000 * 1000);
	
	gTime.tv_sec -= dtime.tv_sec;

	if (gTime.tv_usec < dtime.tv_usec){
		-- gTime.tv_sec;
		gTime.tv_usec += 1 * 1000 * 1000;
	}
	gTime.tv_usec -= dtime.tv_usec;

	return(*this);
};
//------------------------------------------------------------------------------------------//
DTIME& DTIME::Minus(const DTIME& dtime){

	gTime.tv_sec -= dtime.gTime.tv_sec;
	
	if (gTime.tv_usec < dtime.gTime.tv_usec){
		-- gTime.tv_sec;
		gTime.tv_usec += 1 * 1000 * 1000;
	}
	gTime.tv_usec -= dtime.gTime.tv_usec;

	return(*this);
};
//------------------------------------------------------------------------------------------//
DTIME& DTIME::Add(const double& second){
	struct	TIMEVAL64	dtime;
	
	dtime.tv_sec = (uint64)second;
	dtime.tv_usec = (uint64)((second - dtime.tv_sec) * 1000 * 1000);
	
	gTime.tv_sec += dtime.tv_sec;
	gTime.tv_usec += dtime.tv_usec;
	
	if (gTime.tv_usec >= 1 * 1000 * 1000){
		++ gTime.tv_sec;
		gTime.tv_usec -= 1 * 1000 * 1000;
	}
	return(*this);
};
//------------------------------------------------------------------------------------------//
DTIME& DTIME::Add(const DTIME& dtime){
	
	gTime.tv_sec += dtime.gTime.tv_sec;
	gTime.tv_usec += dtime.gTime.tv_usec;
	
	if (gTime.tv_usec >= 1 * 1000 * 1000){
		++ gTime.tv_sec;
		gTime.tv_usec -= 1 * 1000 * 1000;
	}
	return(*this);
};
//------------------------------------------------------------------------------------------//
bool32 DTIME::Compare(const DTIME& stime,const DTIME& etime) const{
	if (stime.gTime.tv_sec > etime.gTime.tv_sec)
		return 1;
	if (stime.gTime.tv_sec < etime.gTime.tv_sec)
		return -1;
	if (stime.gTime.tv_usec > etime.gTime.tv_usec)
		return 1;
	if (stime.gTime.tv_usec < etime.gTime.tv_usec)
		return -1;
	return 0;
};
//------------------------------------------------------------------------------------------//
void SYS_DecodeDate1970(struct tm* time_tm,const struct TIMEVAL64& timeval_tm,G_UTC utc){
#ifdef CommonDefH_Unix
	TIME_SEC	sec;
	sec = timeval_tm.tv_sec;
	if (utc == G_UTC_OFF){
		localtime_r(&sec,time_tm);
	}
	else{
		gmtime_r(&sec,time_tm);
	}
#endif
#ifdef CommonDefH_VC
	TIME_SEC	sec;
	sec = timeval_tm.tv_sec;
	if (utc == G_UTC_OFF){
		localtime_s(time_tm, &sec);
	}
	else{
		gmtime_s(time_tm,&sec);
	}
#endif
};
//------------------------------------------------------------------------------------------//
const DTIME& DTIME::DecodeDate1970(int32* year,int32* month,int32* day,G_UTC utc) const{
	struct tm time_tm;
	
	SYS_DecodeDate1970(&time_tm,gTime,utc);
	*day = time_tm.tm_mday;
	*month = time_tm.tm_mon + 1;
	*year = time_tm.tm_year + 1900;
	return(*this);
};
//------------------------------------------------------------------------------------------//
const DTIME& DTIME::DecodeTime1970(int32* hour,int32* min,int32* sec,int32* msec,G_UTC utc) const{
	struct tm time_tm;
	
	SYS_DecodeDate1970(&time_tm,gTime,utc);
	
	*msec = (int32)((int64)gTime.tv_usec / 1000);
	*sec  = time_tm.tm_sec;
	*min  = time_tm.tm_min;
	*hour = time_tm.tm_hour;
	return(*this);
};
//------------------------------------------------------------------------------------------//
DTIME& DTIME::EncodeTime1970(STDSTR sTime,G_UTC utc){
	// YYYY/MM/DD,hh/mm/ss,zzz
	struct tm	time_tm;
	
	time_tm.tm_year = atoi(Str_ReadSubItem(&sTime,"/").c_str()) - 1900;
	time_tm.tm_mon  = atoi(Str_ReadSubItem(&sTime,"/").c_str()) - 1;
	time_tm.tm_mday = atoi(Str_ReadSubItem(&sTime,",").c_str());
	time_tm.tm_hour = atoi(Str_ReadSubItem(&sTime,"/").c_str());
	time_tm.tm_min  = atoi(Str_ReadSubItem(&sTime,"/").c_str());
	time_tm.tm_sec  = atoi(Str_ReadSubItem(&sTime,",").c_str());
	
	time_tm.tm_wday = 0;
	time_tm.tm_yday = 0;
	time_tm.tm_isdst = 0;
	
	gTime.tv_usec = atoi(sTime.c_str()) * 1000;
#ifdef CommonDefH_Unix
	if (utc == G_UTC_OFF){
		gTime.tv_sec = mktime(&time_tm);
	}
	else{
		gTime.tv_sec = mktime(&time_tm);
		timeb tb;
		ftime(&tb);
		gTime.tv_sec -= (tb.timezone * 60);
	}
#endif
#ifdef CommonDefH_VC
	if (utc == G_UTC_OFF){
		gTime.tv_sec = _mktime64(&time_tm);
	}
	else{
		gTime.tv_sec = _mkgmtime64(&time_tm);
	}
#endif
	return(*this);
};
//------------------------------------------------------------------------------------------//
const DTIME& DTIME::DecodeTimeABS(uint64* hour,uint64* min,uint64* sec,uint64* msec) const{
	*hour = gTime.tv_sec / (60 * 60);
	*min  = (gTime.tv_sec % (60 * 60)) / 60;
	*sec  = gTime.tv_sec % 60;
	*msec = (uint64)gTime.tv_usec / 1000;
	return(*this);
};
//------------------------------------------------------------------------------------------//
DTIME& DTIME::EncodeTimeABS(STDSTR sTime){
	// YYYY/MM/DD,hh/mm/ss,zzz
	
	gTime.tv_sec =  atoi(Str_ReadSubItem(&sTime,"/").c_str()) * 365 * 24 * 60 * 60;
	gTime.tv_sec += atoi(Str_ReadSubItem(&sTime,"/").c_str()) * 30 * 24 * 60 * 60;
	gTime.tv_sec += atoi(Str_ReadSubItem(&sTime,",").c_str()) * 24 * 60 * 60;
	gTime.tv_sec += atoi(Str_ReadSubItem(&sTime,"/").c_str()) * 60 * 60;
	gTime.tv_sec += atoi(Str_ReadSubItem(&sTime,"/").c_str()) * 60;
	gTime.tv_sec += atoi(Str_ReadSubItem(&sTime,",").c_str());
	
	gTime.tv_usec = atoi(sTime.c_str()) * 1000;
	return(*this);
};
//------------------------------------------------------------------------------------------//
DTIME& DTIME::EncodeTimeABS(const uint32& hour,const uint32& min,const uint32& sec,const uint32& msec){
	gTime.tv_sec = hour * 60 * 60 + min * 60 + sec;
	gTime.tv_usec = msec * 1000;
	return(*this);
};
//------------------------------------------------------------------------------------------//
STDSTR SYS_GetTimeString(int32 flagType,int32 flagCount
						 ,const uint32& year,const uint32& month,const uint32& day
						 ,const uint32& hour,const uint32& min,const uint32& sec,const uint32& msec){
	STDSTR	retResult,strTemp;
	retResult = "";
	if (flagCount > 0){
		switch(flagType){
			case 1:{//YY
				if (flagCount < 3){
					retResult = Str_ToStr(year % 100);
				}
				else{
					retResult = Str_ToStr(year);
				}
				break;
			}
			case 2:{//MM
				strTemp = Str_ToStr(month);
				if ((uint32)flagCount > strTemp.length())
					strTemp.insert(0,flagCount - strTemp.length(),'0');
				retResult += strTemp;
				break;
			}
			case 3:{//DD
				strTemp = Str_ToStr(day);
				if ((uint32)flagCount > strTemp.length())
					strTemp.insert(0,flagCount - strTemp.length(),'0');
				retResult += strTemp;
				break;
			}
			case 4:{//hh
				strTemp = Str_ToStr(hour);
				if ((uint32)flagCount > strTemp.length())
					strTemp.insert(0,flagCount - strTemp.length(),'0');
				retResult += strTemp;
				break;
			}
			case 5:{//mm
				strTemp = Str_ToStr(min);
				if ((uint32)flagCount > strTemp.length())
					strTemp.insert(0,flagCount - strTemp.length(),'0');
				retResult += strTemp;
				break;
			}
			case 6:{//ss
				strTemp = Str_ToStr(sec);
				if ((uint32)flagCount > strTemp.length())
					strTemp.insert(0,flagCount - strTemp.length(),'0');
				retResult += strTemp;
				break;
			}
			case 7:{//zzz
				uint32 _msec = msec;
				if (flagCount == 2)
					_msec /= 10;
				if (flagCount == 1)
					_msec /= 100;
				strTemp = Str_ToStr(_msec);
				if ((uint32)flagCount > strTemp.length())
					strTemp.insert(0,flagCount - strTemp.length(),'0');
				retResult += strTemp;
				break;
			}
			default:;
		};
	}
	return(retResult);
};
//------------------------------------------------------------------------------------------//
STDSTR SYS_FormatDateTime(const STDSTR& style
						  ,const uint32& year,const uint32& month,const uint32& day
						  ,const uint32& hour,const uint32& min,const uint32& sec,const uint32& msec){
	STDSTR		ret;
	STRSIZE		length,i;
	int32		flag,flagCount,flagType;
	uint8		charP,charData;
	
	length = style.length();
	i = 0;
	flag = 0;
	flagCount = 0;
	flagType = 0;
	ret = "";
	while(i < length){
		charP = style[i];
		if ((i + 1) < length){
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
		if (flag == 0){
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
};
//------------------------------------------------------------------------------------------//
STDSTR SYS_FormatDateTime1970(const STDSTR& style,const DTIME& dtime,G_UTC utc){
	int32	year,month,day,hour,min,sec,msec;
	
	dtime.DecodeDate1970(&year,&month,&day,utc);
	dtime.DecodeTime1970(&hour,&min,&sec,&msec,utc);
	
	return(SYS_FormatDateTime(style,year,month,day,hour,min,sec,msec));
};
//------------------------------------------------------------------------------------------//
STDSTR SYS_FormatDateTime1900(const STDSTR& style,DTIME dtime,G_UTC utc){
	STDSTR	ret;
	int32	year,month,day,hour,min,sec,msec;
	
	dtime -= (70LL * 365 + 17) * 24 * 60 * 60;
	dtime.DecodeDate1970(&year,&month,&day,utc);
	dtime.DecodeTime1970(&hour,&min,&sec,&msec,utc);
	return(SYS_FormatDateTime(style,year,month,day,hour,min,sec,msec));
};
//------------------------------------------------------------------------------------------//
STDSTR SYS_FormatTimeABS(const STDSTR& style,const DTIME& dtime){
	uint64	hour,min,sec,msec;
	
	dtime.DecodeTimeABS(&hour,&min,&sec,&msec);
	
	return(SYS_FormatDateTime(style,0,0,0,(uint32)hour,(uint32)min,(uint32)sec,(uint32)msec));
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
bool32 SYS_DelayMS(double timeMS,bool32* blExit){
#ifdef CommonDefH_Unix
	DTIME	executeTime,nowTime;
	double	dfTim;
	nowTime.Now();
	do
	{
		executeTime.Now();
		executeTime -= nowTime;
		dfTim = executeTime.GetSec() * 1000;
		if ((blExit != nullptr) && (*blExit != G_FALSE))
			break;
	}while(timeMS - dfTim > 0.5);
#endif
#ifdef CommonDefH_VC
	double	dfTim;
	if (timeMS > 99){
		DTIME	executeTime,nowTime;
		nowTime.Now();
		do
		{
			executeTime.Now();
			executeTime -= nowTime;
			dfTim = executeTime.GetSec() * 1000;
			if ((blExit != nullptr) && (*blExit != G_FALSE))
				break;
		}while(timeMS - dfTim > 0.5);
	}
	else{
		LARGE_INTEGER	litmp;
		LONGLONG	QPart1,QPart2;
		double		dfFreq;
		QueryPerformanceFrequency(&litmp);
		dfFreq = (double)litmp.QuadPart;		// get Frequency
		QueryPerformanceCounter(&litmp);
		QPart1 = litmp.QuadPart;				// get first count.
		do
		{
			QueryPerformanceCounter(&litmp);
			QPart2 = litmp.QuadPart;			// get last count
			dfTim = (double)(QPart2 - QPart1) * 1000 / dfFreq;	// get time,uint is ms
			if ((blExit != nullptr) && (*blExit != G_FALSE))
				break;
		}while(timeMS - dfTim > 0.5);
	}
#endif
	return G_FALSE;
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
uint64 SYS_ASCIIStrToNTP(STDSTR strIn){
	uint8		charData;
	uint64		ntp;
	
	charData = strIn[0];
	ntp = charData;
	charData = strIn[1];
	ntp = (ntp << 8) | charData;
	charData = strIn[2];
	ntp = (ntp << 8) | charData;
	charData = strIn[3];
	ntp = (ntp << 8) | charData;
	charData = strIn[4];
	ntp = (ntp << 8) | charData;
	charData = strIn[5];
	ntp = (ntp << 8) | charData;
	charData = strIn[6];
	ntp = (ntp << 8) | charData;
	charData = strIn[7];
	ntp = (ntp << 8) | charData;
	
	return(ntp);
};
//------------------------------------------------------------------------------------------//
STDSTR SYS_NTPToHEXStr(uint64 ntp){
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
	
	return(Str_CharToHEX(charData, 8, G_SPACE_ON));
};
//------------------------------------------------------------------------------------------//
void SYS_NTPToChar(uint64 ntp,uint8* buffer){
	
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
};
//------------------------------------------------------------------------------------------//
uint64 SYS_CharToNTP(const uint8* buffer){
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
};
//------------------------------------------------------------------------------------------//
uint64 SYS_TimeToNTP(const double& sec){
	uint64		ret, u64_sec, u64_nsec;
	
	u64_sec = (uint64)(sec);
	u64_nsec = (uint64)((sec - u64_sec) * 1000 * 1000 * 1000);
	/* Convert nanoseconds to 32-bits fraction (232 picosecond units) */
	ret = ((u64_nsec) << 32) / (1000 * 1000 * 1000);
	
	//There is 70 years (incl. 17 leap ones) offset to the Unix Epoch.
	//No leap seconds during that period since they were not invented yet.
	
	ret |= ((70LL * 365 + 17) * 24 * 60 * 60 + u64_sec) << 32;
	
	return(ret);
};
//------------------------------------------------------------------------------------------//
double SYS_NTPToTime(const uint64& ntp){
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
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif /* SYS_Time_h */
