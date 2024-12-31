﻿#define __HANDLETIME_C__
#include "../systeminc/version.h"
#include "../systeminc/system.h"
#include <time.h>
#include "../systeminc/handletime.h"

//#ifdef _STONDEBUG_
//(180)企??希??1吋
//#define LSTIME_SECONDS_PER_DAY 180
//#else
//(750X12)企????刨??1吋
#define LSTIME_SECONDS_PER_DAY 5400 /* LSTIME?吋?????企? */
//#endif
/*
  LSTIME_SECONDS_PER_DAY ?阪???????????阪?????????

  π            LS????吋?????????
  9000 (火?)   2.5 [hour]
  900           0.25[hour] = 15[min]
  90            0.025[hour] = 1.5[min] = 90[sec]
  9             9[sec]

*/

#define LSTIME_HOURS_PER_DAY 1024 /* LSTIME?吋???LSTIME???? */
#define LSTIME_DAYS_PER_YEAR 100 /* LSTIME?夙???LSTIME?吋? */


// ?????????
LSTIME SaTime;
long serverTime;
long FirstTime;
int SaTimeZoneNo;	// ?????┼
BOOL TimeZonePalChangeFlag; // ??┼??????????????
#if 0
/*------------------------------------------------------------
 * ??????????????????
 * ??
 *  ??
 * 刺?π
 *  ?    TRUE(1)
 *  ?    FALSE(0)
 ------------------------------------------------------------*/
BOOL setNewTime( void )
{
    if( gettimeofday( &NowTime, (struct timezone*)NULL) != 0 )
        return FALSE;
    NowTime.tv_sec += DEBUG_ADJUSTTIME;
    return TRUE;
}
#endif

/*******************************************************************
??Ρ????byHiO 1998/12/4 18:37
*******************************************************************/
static long era = (long)912766409 + 5400;
									/* SA??????? */
									/* LS?夙?叉?????????π?
                                    ???????可?????????*/

/*******************************************************************
	???????LS?????
	long t : time??
	LSTIME *lstime : LSTIME?﹨▍??????
*******************************************************************/
void RealTimeToSATime( LSTIME *lstime )
{
	long lsseconds; /* LS?夙???企? */
    long lsdays; /* LS?夙???吋? */

	//cary 坋拻
	lsseconds = (TimeGetTime()-FirstTime)/1000 + serverTime - era;

    /* ?夙???企??1夙???企??????夙??? */
	lstime->year = (int)( lsseconds/(LSTIME_SECONDS_PER_DAY*LSTIME_DAYS_PER_YEAR) );

    lsdays = lsseconds/LSTIME_SECONDS_PER_DAY;/* ???夙???吋????? */
	lstime->day  = lsdays % LSTIME_DAYS_PER_YEAR;/* 夙????吋?????????吋*/


    /*(750*12)企?1吋*/
    lstime->hour = (int)(lsseconds % LSTIME_SECONDS_PER_DAY )
/* ???????吋???????企????? */
        * LSTIME_HOURS_PER_DAY / LSTIME_SECONDS_PER_DAY;
    /* ?吋????企????????吋????????????????
     ????????*/

	return;
}

/*******************************************************************
	LS????????????
	LSTIME *lstime : LSTIME?﹨▍??????
	long *t : ????????
*******************************************************************/
void LSTimeToRealTime( LSTIME *lstime, long *t)
{
	*t=(long)(
        ( lstime->hour*LSTIME_DAYS_PER_YEAR+lstime->day) /* ?? */
               *LSTIME_HOURS_PER_DAY

        +     lstime->year)
        /*??????????????????nakamura      */


        *450;
	return;
}

/*******************************************************************
	LS????????希?台?
	??π int : ?0??1??2??3
	LSTIME *lstime : LSTIME?﹨▍??????
*******************************************************************/
LSTIME_SECTION getLSTime (LSTIME *lstime)
{
	if (NIGHT_TO_MORNING < lstime->hour
        && lstime->hour <= MORNING_TO_NOON)
		return LS_MORNING;
	else if(NOON_TO_EVENING < lstime->hour
            && lstime->hour <= EVENING_TO_NIGHT)
		return LS_EVENING;
	else if(EVENING_TO_NIGHT < lstime->hour
            && lstime->hour <= NIGHT_TO_MORNING)
		return LS_NIGHT;
	else
		return LS_NOON;
}

// ??┼????????????? ***********************************/
void TimeZoneProc( void )
{
	int	timeZoneNo;
	//??????????希?台?
	timeZoneNo = getLSTime ( &SaTime );
	// ??┼?阪????
	if( SaTimeZoneNo != timeZoneNo ){
		SaTimeZoneNo = timeZoneNo;	// ??┼?
		// ??┼????????????
		if( TimeZonePalChangeFlag == TRUE ){
			PaletteChange( SaTimeZoneNo, PAL_CHANGE_TIME );// ????????
		}
	}
}
	
