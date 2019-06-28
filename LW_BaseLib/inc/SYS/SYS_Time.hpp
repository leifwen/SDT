//
//  SYS_Time.hpp
//  SDT
//
//  Created by Leif Wen on 23/01/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

//------------------------------------------------------------------------------------------//
#ifndef SYS_Time_hpp
#define SYS_Time_hpp
//------------------------------------------------------------------------------------------//
#include "SYS_Time.h"
#ifdef SYS_Time_h
//------------------------------------------------------------------------------------------//
void DTIME::Clear(void){
	gTime.tv_sec = 0;
	gTime.tv_usec = 0;
};
//------------------------------------------------------------------------------------------//
double DTIME::GetSec(void)const{
	return((double)gTime.tv_sec + (double)gTime.tv_usec / 1000000.0);
};
//------------------------------------------------------------------------------------------//
double DTIME::GetMSec(void)const{
	return((double)gTime.tv_sec * 1000.0 + (double)gTime.tv_usec / 1000.0);
};
//------------------------------------------------------------------------------------------//
inline DTIME& DTIME::Set(const double& second){
	gTime.tv_sec = (uint64)second;
	gTime.tv_usec = (uint64)((second - gTime.tv_sec) * 1000 * 1000);
	return(*this);
};
//------------------------------------------------------------------------------------------//
inline DTIME& DTIME::Set(const DTIME& dtime){
	gTime.tv_sec = dtime.gTime.tv_sec;
	gTime.tv_usec = dtime.gTime.tv_usec;
	return(*this);
};
//------------------------------------------------------------------------------------------//
inline STDSTR DTIME::Format(const STDSTR &style,G_UTC utc)const{
	
	return(SYS_FormatDateTime1970(style,*this,utc));
};
//------------------------------------------------------------------------------------------//
inline			DTIME  DTIME::operator -	(const double &second)	{DTIME dt = *this;return(dt.Minus(second));};
inline			DTIME  DTIME::operator -	(const DTIME &dtime)	{DTIME dt = *this;return(dt.Minus(dtime));};
inline			DTIME  DTIME::operator +	(const double &second)	{DTIME dt = *this;return(dt.Add(second));};
inline			DTIME  DTIME::operator +	(const DTIME &dtime)	{DTIME dt = *this;return(dt.Add(dtime));};

inline	const	DTIME& DTIME::operator -=	(const double &second)	{return(Minus(second));};
inline	const	DTIME& DTIME::operator -=	(const DTIME &dtime)	{return(Minus(dtime));};
inline	const	DTIME& DTIME::operator +=	(const double &second)	{return(Add(second));};
inline	const	DTIME& DTIME::operator +=	(const DTIME &dtime)	{return(Add(dtime));};
inline	const	DTIME& DTIME::operator =	(const double &second)	{return(Set(second));};
inline	const	DTIME& DTIME::operator =	(const DTIME &dtime)	{return(Set(dtime));};

inline	bool DTIME::operator ==(const DTIME& dtime) const{return(Compare(*this,dtime) == 0);};
inline	bool DTIME::operator !=(const DTIME& dtime) const{return(Compare(*this,dtime) != 0);};
inline	bool DTIME::operator > (const DTIME& dtime) const{return(Compare(*this,dtime) > 0);};
inline	bool DTIME::operator >=(const DTIME& dtime) const{return(Compare(*this,dtime) >= 0);};
inline	bool DTIME::operator < (const DTIME& dtime) const{return(Compare(*this,dtime) < 0);};
inline	bool DTIME::operator <=(const DTIME& dtime) const{return(Compare(*this,dtime) <= 0);};
//------------------------------------------------------------------------------------------//
inline bool32 SYS_SleepMS(int32 timeMS){
#ifdef CommonDefH_Unix
	struct	timeval	tv;
	tv.tv_sec = timeMS / 1000;
	tv.tv_usec = (timeMS % 1000) * 1000;
	return(select(0,nullptr,nullptr,nullptr,&tv));
#endif
#ifdef CommonDefH_VC
	Sleep(timeMS);
#endif
	return G_FALSE;
};
//------------------------------------------------------------------------------------------//
inline void SYS_StopWatch_Start(SYS_TIME_S* timeS){
	timeS->DTime1.Now();
	timeS->timeMS = 0;
};
//------------------------------------------------------------------------------------------//
inline const double& SYS_StopWatch_Stop(SYS_TIME_S* timeS){
	timeS->DTime2.Now();
	timeS->DTime2 -= timeS->DTime1;
	timeS->timeMS = timeS->DTime2.GetMSec();
	return(timeS->timeMS);
};
//------------------------------------------------------------------------------------------//
inline void SYS_Delay_SetTS(SYS_TIME_S* timeS,double timeMS){
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
};
//------------------------------------------------------------------------------------------//
inline void SYS_Delay_AddTS(SYS_TIME_S* timeS,double timeMS){
	timeS->timeMS += timeMS;
	timeS->DTime1 += (timeMS / 1000);
#ifdef CommonDefH_VC
	timeS->QTime1 += (LONGLONG)((timeMS * timeS->dfFreq) / 1000);
#endif
};
//------------------------------------------------------------------------------------------//
inline bool32 SYS_Delay_IsTimeout(SYS_TIME_S* timeS){
	bool32	blIsTimeout;
	if (timeS == nullptr)
		return G_FALSE;
#ifdef CommonDefH_Unix
	timeS->DTime2.Now();
	blIsTimeout = (timeS->DTime2 >= timeS->DTime1);
#endif
#ifdef CommonDefH_VC
	if (timeS->timeMS > 99){
		timeS->DTime2.Now();
		blIsTimeout = (timeS->DTime2 >= timeS->DTime1);
	}
	else{
		QueryPerformanceCounter(&timeS->litmp);
		timeS->QTime2 = timeS->litmp.QuadPart;				// get last count
		blIsTimeout = (timeS->QTime2 >= timeS->QTime1);	// get time,uint is ms
	}
#endif
	return(blIsTimeout);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif /* SYS_Time_h */
#endif /* SYS_Time_hpp */
