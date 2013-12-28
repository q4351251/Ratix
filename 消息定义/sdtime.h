/******************************************************************************
            Copyright (C) Shanda Corporation. All rights reserved.

 This file defines the time value type used by SGDP library.

******************************************************************************/

#ifndef SGDP_TIME_H_20070614
#define SGDP_TIME_H_20070614

#include "sdtype.h"

namespace SGDP
{
    typedef struct 
    {
        UINT32 m_dwSec;     ///< seconds
        UINT32 m_dwUsec;    ///< microsecond(1/1,000,000 second)
    }STimeVal;

    typedef struct
    {
        INT32 m_nSec;       ///< Seconds.     [0-60]
        INT32 m_nMin;       ///< Minutes.     [0-59]
        INT32 m_nHour;      ///< Hours.       [0-23]
        INT32 m_nMday;      ///< Day.         [1-31]
        INT32 m_nMon;       ///< Month.       [0-11]
        INT32 m_nYear;      ///< Year
        INT32 m_nWday;      ///< Day of week. [0-6]
    }SLocalTime;
}

#endif
