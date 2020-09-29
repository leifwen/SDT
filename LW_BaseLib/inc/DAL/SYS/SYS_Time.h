//
//  SYS_Time.h
//  SDT
//
//  Created by Leif Wen on 23/01/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

//------------------------------------------------------------------------------------------//
#ifndef SYS_Time_h
#define SYS_Time_h
#ifdef SYS_Time_h
//------------------------------------------------------------------------------------------//
#include "BasicDefine.h"
#include <time.h>
#ifdef CommonDefH_Unix
	#include <sys/timeb.h>
	#include <sys/time.h>
#endif
//------------------------------------------------------------------------------------------//
enum G_UTC{G_UTC_OFF = 0,G_UTC_ON};
//------------------------------------------------------------------------------------------//
#ifdef CommonDefH_MAC
	typedef __darwin_time_t			TIME_SEC;
	typedef __darwin_suseconds_t	TIME_USEC;
#endif

#ifdef CommonDefH_Linux
	typedef time_t			TIME_SEC;
	typedef suseconds_t		TIME_USEC;
#endif

#ifdef CommonDefH_VC
	typedef time_t			TIME_SEC;
	typedef uint32			TIME_USEC;
#endif
//------------------------------------------------------------------------------------------//
struct	TIMEVAL64{
	uint64	tv_sec;	        /* seconds */
	uint64	tv_usec;        /* and microseconds */
};
//------------------------------------------------------------------------------------------//
class DTIME{
	public:
				 DTIME	(void);
				 DTIME	(const DTIME& C);
		virtual ~DTIME	(void){;};
	public:
		static double		gCompensation;
	private:
		struct	TIMEVAL64	gTime;
	public:
		inline			void	Clear		(void);
						DTIME&	Now			(void);
		inline			double	GetSec		(void)const;
		inline			double	GetMSec		(void)const;
		inline			DTIME&	Set			(const double&	second);
		inline			DTIME&	Set			(const DTIME&	dtime);
						DTIME&	Minus		(const double&	second);
						DTIME&	Minus		(const DTIME&	dtime);
						DTIME&	Add			(const double&	second);
						DTIME&	Add			(const DTIME&	dtime);
						bool32	Compare		(const DTIME&	stime,const DTIME& etime)const;
	public:
		inline			DTIME	operator -	(const double&	second);
		inline			DTIME	operator -	(const DTIME&	dtime);
		inline			DTIME	operator +	(const double&	second);
		inline			DTIME	operator +	(const DTIME&	dtime);
		inline	const	DTIME&	operator -=	(const double&	second);
		inline	const	DTIME&	operator -=	(const DTIME&	dtime);
		inline	const	DTIME&	operator +=	(const double&	second);
		inline	const	DTIME&	operator +=	(const DTIME&	dtime);
		inline	const	DTIME&	operator =	(const double&	second);
		inline	const	DTIME&	operator =	(const DTIME& 	dtime);
	public:
		inline			bool	operator ==	(const DTIME&	dtime) const;
		inline			bool	operator !=	(const DTIME&	dtime) const;
		inline			bool	operator >	(const DTIME&	dtime) const;
		inline			bool	operator >=	(const DTIME&	dtime) const;
		inline			bool	operator <	(const DTIME&	dtime) const;
		inline			bool	operator <=	(const DTIME&	dtime) const;
	public:
		const	DTIME& DecodeDate1970	(int32* year,int32* month,int32* day            ,G_UTC utc = G_UTC_OFF) const;
		const	DTIME& DecodeTime1970	(int32* hour,int32* min  ,int32* sec,int32* msec,G_UTC utc = G_UTC_OFF) const;
				DTIME& EncodeTime1970	(STDSTR sTime                                   ,G_UTC utc = G_UTC_OFF);// YYYY/MM/DD,hh/mm/ss,zzz
		const	DTIME& DecodeTimeABS	(uint64* hour,uint64* min,uint64* sec,uint64* msec) const;
				DTIME& EncodeTimeABS	(STDSTR sTime);											// YYYY/MM/DD,hh/mm/ss,zzz
				DTIME& EncodeTimeABS	(const uint32& hour,const uint32& min,const uint32& sec,const uint32& msec);

		inline	STDSTR Format			(const STDSTR& style,G_UTC utc = G_UTC_OFF)const;
	public:
};
//------------------------------------------------------------------------------------------//
STDSTR	SYS_MakeTimeNow			(void);
STDSTR	SYS_MakeTime			(DTIME& dtime);
STDSTR	SYS_GetTimeString		(int32 flagType,int32 flagCount
								 ,const uint32& year,const uint32& month,const uint32& day
								 ,const uint32& hour,const uint32& min,const uint32& sec,const uint32& msec);
STDSTR	SYS_FormatDateTime1970	(const STDSTR& style,const DTIME& dtime,G_UTC utc = G_UTC_OFF);
STDSTR	SYS_FormatDateTime1900	(const STDSTR& style,DTIME dtime,G_UTC utc = G_UTC_OFF);
STDSTR	SYS_FormatTimeABS		(const STDSTR& style,const DTIME& dtime);
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
inline	bool32 SYS_SleepMS(int32 timeMS);
		bool32 SYS_DelayMS(double timeMS,bool32* blExit = nullptr);
//------------------------------------------------------------------------------------------//
struct SYS_TIME_S{
	double 	timeMS;
	DTIME	DTime1;
	DTIME	DTime2;
#ifdef CommonDefH_VC
	LARGE_INTEGER	litmp;
	LONGLONG		QTime1;
	LONGLONG		QTime2;
	double			dfFreq;
#endif
};
//------------------------------------------------------------------------------------------//
inline		 void		SYS_StopWatch_Start	(SYS_TIME_S* timeS);
inline const double&	SYS_StopWatch_Stop	(SYS_TIME_S* timeS);
//------------------------------------------------------------------------------------------//
inline		 void		SYS_Delay_SetTS		(SYS_TIME_S* timeS,double timeMS);
inline		 void		SYS_Delay_AddTS		(SYS_TIME_S* timeS,double timeMS);
inline		 bool32		SYS_Delay_IsTimeout	(SYS_TIME_S* timeS);
//------------------------------------------------------------------------------------------//
uint64	SYS_TimeToNTP		(const double& sec);
double	SYS_NTPToTime		(const uint64& ntp);
uint64	SYS_CharToNTP		(const uint8* buffer);
void	SYS_NTPToChar		(uint64 ntp, uint8* buffer);
uint64	SYS_ASCIIStrToNTP	(STDSTR strIn);
STDSTR	SYS_NTPToHEXStr		(uint64 ntp);
//------------------------------------------------------------------------------------------//
#include "SYS_Time.hpp"
#endif /* SYS_Time_h */
#endif /* SYS_Time_h */
