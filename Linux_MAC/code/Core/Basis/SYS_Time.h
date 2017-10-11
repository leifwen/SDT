/*
 * Copyright (c) 2012-2014 
 * All rights reserved.
 *
 * File Name: SYS_Time.h
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
#ifndef SYS_TimeH
#define SYS_TimeH
//------------------------------------------------------------------------------------------//
#include "BasicDef.h"
//------------------------------------------------------------------------------------------//
enum G_UTC{G_UTC_OFF = 0,G_UTC_ON};
//------------------------------------------------------------------------------------------//
class TIME;
//------------------------------------------------------------------------------------------//
STDSTR		SYS_MakeTimeNow		(void);
STDSTR		SYS_MakeTime		(TIME &tDateTime);
const TIME&	SYS_Now				(TIME *tDateTime);	//UTC
double		SYS_GetSec			(const TIME &tDateTime);

const TIME&	SYS_SetDateTime		(TIME *tRetDateTime,const double &sec);
const TIME&	SYS_SetDateTime		(TIME *tRetDateTime,const TIME &tDateTime);

const TIME&	SYS_MinusDateTime	(TIME *tRetDateTime,const double &sec);
const TIME&	SYS_MinusDateTime	(TIME *tRetDateTime,const TIME &tDateTime);
const TIME&	SYS_MinusDateTime	(TIME *tRetDateTime,const TIME &tStrartDateTime,const TIME &tEndDateTime);

const TIME&	SYS_AddDateTime		(TIME *tRetDateTime,const double &sec);
const TIME&	SYS_AddDateTime		(TIME *tRetDateTime,const TIME &tDateTime);
const TIME&	SYS_AddDateTime		(TIME *tRetDateTime,const TIME &tStrartDateTime,const TIME &tEndDateTime);

void		SYS_DecodeDate1970	(const TIME &tDateTime,int32 *year,int32 *month,int32 *day,G_UTC utc = G_UTC_OFF);
void		SYS_DecodeTime1970	(const TIME &tDateTime,int32 *hour,int32 *min,int32 *sec,int32 *msec,G_UTC utc = G_UTC_OFF);
const TIME&	SYS_EncodeTime1970	(TIME *tDateTime,STDSTR sTime,G_UTC utc = G_UTC_OFF);//"YYYY/MM/DD,hh/mm/ss,zzz"

void		SYS_DecodeTimeABS	(const TIME &tDateTime,uint64 *hour,uint64 *min,uint64 *sec,uint64 *msec);
const TIME&	SYS_EncodeTimeABS	(TIME *tDateTime,STDSTR sTime);
const TIME&	SYS_EncodeTimeABS	(TIME *tDateTime,const uint32 &hour,const uint32 &min,const uint32 &sec,const uint32 &msec);

STDSTR	SYS_FormatDateTime		(const STDSTR &style,const uint32 &year,const uint32 &month,const uint32 &day,const uint32 &hour,const uint32 &min,const uint32 &sec,const uint32 &msec);
STDSTR	SYS_FormatDateTime1970	(const STDSTR &style,const TIME &tDateTime,G_UTC utc = G_UTC_OFF);
STDSTR	SYS_FormatDateTime1900	(const STDSTR &style,TIME tDateTime,G_UTC utc = G_UTC_OFF);
STDSTR	SYS_FormatTimeABS		(const STDSTR &style,const TIME &tDateTime);

int32		SYS_CompareDateTime	(const TIME &tStrartDateTime,const TIME &tEndDateTime);
//------------------------------------------------------------------------------------------//
class TIME{
	public:
				 TIME	(void){Clear();};
		virtual ~TIME	(void){;};
	public:
		static double	gCompensation;
	public:
		#ifdef CommonDefH_Unix
			struct	timeval	gDateTime;
		#endif
		#ifdef CommonDefH_VC
			uint32			gMsec;
			time_t			gDateTime;
		#endif
	public:
		void	Clear(void){
			#ifdef CommonDefH_Unix
				gDateTime.tv_sec = 0;
				gDateTime.tv_usec = 0;
			#endif
			#ifdef CommonDefH_VC
				gDateTime = 0;
				gMsec = 0;
			#endif
		};
		inline	double	GetSec			(void)																	{return(SYS_GetSec(*this));};
		inline	void	Now				(void)																	{SYS_Now(this);};
		//inline	void	Date			(void)																	{SYS_Now(this);};
		inline	void	DecodeDate		(int32 *year,int32 *month,int32 *day,G_UTC utc = G_UTC_OFF)				{SYS_DecodeDate1970(*this,year,month,day,utc);};
		inline	void	DecodeTime		(int32 *hour,int32 *min,int32 *sec,int32 *msec,G_UTC utc = G_UTC_OFF)	{SYS_DecodeTime1970(*this,hour,min,sec,msec,utc);};
		inline	void	EncodeTime		(const STDSTR &sTime,G_UTC utc = G_UTC_OFF)								{SYS_EncodeTime1970(this,sTime,utc);};
		inline	void	MinusDateTime	(const TIME &tDateTime)													{SYS_MinusDateTime(this,tDateTime);};
		inline	void	AddDateTime		(const TIME &tDateTime)													{SYS_AddDateTime(this,tDateTime);};
		inline	void	MinusDateTime	(const TIME &tStrartDateTime,const TIME &tEndDateTime)					{SYS_MinusDateTime(this,tStrartDateTime,tEndDateTime);};
		inline	void	AddDateTime		(const TIME &tStrartDateTime,const TIME &tEndDateTime)					{SYS_AddDateTime(this,tStrartDateTime,tEndDateTime);};
		inline	STDSTR	FormatDateTime	(const STDSTR &style,G_UTC utc = G_UTC_OFF)								{return(SYS_FormatDateTime1970(style,*this,utc));};
	public:
		TIME operator +(const TIME &tDateTime){
			TIME ret;
			return(SYS_AddDateTime(&ret,*this,tDateTime));
		};
		TIME operator +(const double &sec){
			TIME ret;
			ret = sec;
			return(SYS_AddDateTime(&ret,*this,ret));
		};
		const TIME &operator +=(const TIME &tDateTime)	{return(SYS_AddDateTime(this,tDateTime));};
		const TIME &operator +=(const double &sec)		{return(SYS_AddDateTime(this,sec));};
		TIME operator -(const TIME &tDateTime){
			TIME ret;
			return(SYS_MinusDateTime(&ret,*this,tDateTime));
		};
		TIME operator -(const double &sec){
			TIME ret;
			ret = sec;
			return(SYS_MinusDateTime(&ret,*this,ret));
		};
		const TIME &operator -=(const TIME &tDateTime)	{return(SYS_MinusDateTime(this,tDateTime));};
		const TIME &operator -=(const double &sec)		{return(SYS_MinusDateTime(this,sec));};
		const TIME &operator =(const TIME &tDateTime)	{return(SYS_SetDateTime(this,tDateTime));};
		const TIME &operator =(const double &sec)		{return(SYS_SetDateTime(this,sec));};

		bool operator ==(const TIME& tDateTime){return(SYS_CompareDateTime(*this,tDateTime) == 0);};
		bool operator !=(const TIME& tDateTime){return(SYS_CompareDateTime(*this,tDateTime) != 0);};
		bool operator > (const TIME& tDateTime){return(SYS_CompareDateTime(*this,tDateTime) == 1);};
		bool operator >=(const TIME& tDateTime){return(SYS_CompareDateTime(*this,tDateTime) != -1);};
		bool operator < (const TIME& tDateTime){return(SYS_CompareDateTime(*this,tDateTime) == -1);};
		bool operator <=(const TIME& tDateTime){return(SYS_CompareDateTime(*this,tDateTime) != 1);};
};
//------------------------------------------------------------------------------------------//
typedef  TIME TIME;
//------------------------------------------------------------------------------------------//
inline int32 SYS_SleepMS(int32 timeMS){
#ifdef CommonDefH_Unix
	struct	timeval	tv;
	tv.tv_sec = timeMS / 1000;
	tv.tv_usec = (timeMS % 1000) * 1000;
	return(select(0,nullptr,nullptr,nullptr,&tv));
#endif
#ifdef CommonDefH_VC
	Sleep(timeMS);
#endif
	return 0;
}
//------------------------------------------------------------------------------------------//
inline int32 SYS_DelayMS(double timeMS,int32 *blExit = nullptr){
#ifdef CommonDefH_Unix
	TIME	executeTime,nowTime;
	double	dfTim;
	nowTime.Now();
	do
	{
		executeTime.Now();
		executeTime.MinusDateTime(nowTime);
		dfTim = executeTime.GetSec() * 1000;
		if ((blExit != nullptr) && (*blExit != 0))
			break;
	}while(timeMS - dfTim > 0.5);
#endif
#ifdef CommonDefH_VC
	double	dfTim;
	if (timeMS > 99){
		TIME	executeTime,nowTime;
		nowTime.Now();
		do
		{
			executeTime.Now();
			executeTime.MinusDateTime(nowTime);
			dfTim = executeTime.GetSec() * 1000;
			if ((blExit != nullptr) && (*blExit != 0))
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
			if ((blExit != nullptr) && (*blExit != 0))
				break;
		}while(timeMS - dfTim > 0.5);
	}
#endif
	return 0;
}
//------------------------------------------------------------------------------------------//
struct SYS_TIME_S{
	double 	timeMS;
	int32	dfTim;
	TIME	DTime1;
	TIME	DTime2;
#ifdef CommonDefH_VC
	LARGE_INTEGER	litmp;
	LONGLONG		QTime1;
	LONGLONG		QTime2;
	double			dfFreq;
#endif
};
//------------------------------------------------------------------------------------------//
inline void SYS_Delay_SetTS(SYS_TIME_S *timeS,double timeMS){
	timeS->DTime1.Now();
	timeS->DTime1 += (timeMS / 1000);
	timeS->timeMS = timeMS;
#ifdef CommonDefH_VC
	QueryPerformanceFrequency(&timeS->litmp);
	timeS->dfFreq = (double)timeS->litmp.QuadPart;		// get Frequency
	QueryPerformanceCounter(&timeS->litmp);
	timeS->QTime1 = timeS->litmp.QuadPart;				// get first count.
	timeS->QTime1 += (LONGLONG)((timeMS * timeS->dfFreq) / 1000);
#endif
}
//------------------------------------------------------------------------------------------//
inline void SYS_Delay_AddTS(SYS_TIME_S *timeS,double timeMS){
	timeS->timeMS += timeMS;
	timeS->DTime1 += (timeMS / 1000);
#ifdef CommonDefH_VC
	timeS->QTime1 += (LONGLONG)((timeMS * timeS->dfFreq) / 1000);
#endif
}
//------------------------------------------------------------------------------------------//
inline int32 SYS_Delay_CheckTS(SYS_TIME_S *timeS){
	if (timeS == nullptr)
		return 0;
#ifdef CommonDefH_Unix
	timeS->DTime2.Now();
	timeS->dfTim = (timeS->DTime2 >= timeS->DTime1);
#endif
#ifdef CommonDefH_VC
	if (timeS->timeMS > 99){
		timeS->DTime2.Now();
		timeS->dfTim = (timeS->DTime2 >= timeS->DTime1);
	}
	else{
		QueryPerformanceCounter(&timeS->litmp);
		timeS->QTime2 = timeS->litmp.QuadPart;			// get last count
		timeS->dfTim = (timeS->QTime2 >= timeS->QTime1);	// get time,uint is ms
	}
#endif
	return(timeS->dfTim);
}
//------------------------------------------------------------------------------------------//
uint64	SYS_TimeToNTP		(const double &sec);
double	SYS_NTPToTime		(const uint64 &ntp);
uint64	SYS_CharToNTP		(const uint8 *buffer);
void	SYS_NTPToChar		(uint64 ntp, uint8 *buffer);
uint64	SYS_ASCIIStrToNTP	(STDSTR strIn);
STDSTR	SYS_NTPToHEXStr		(uint64 ntp);
//------------------------------------------------------------------------------------------//
#endif
