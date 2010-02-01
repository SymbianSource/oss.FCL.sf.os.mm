/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:
*
*/

#ifndef INCLUDE_COMMON
#define INCLUDE_COMMON


typedef signed char          int8;
typedef unsigned char        uint8;
typedef short int            int16;
typedef unsigned short int   uint16;
typedef int                  int32;
typedef unsigned int         uint32;

typedef float                flt32;
typedef double               flt64;

typedef unsigned char        tBool;
typedef signed int           tError;


#define E_TRUE               1
#define E_FALSE              0

#define E_ON                 1
#define E_OFF                0

#define E_DEBUG              1
#define E_RELEASE            0

#define E_SUCCESS            0
#define E_FAILURE           (-1)
#define E_OUT_OF_MEMORY     (-2)
#define E_OUT_OF_RANGE      (-3)
#define E_FILE_CREATE_FAIL  (-4)
#define E_UNDEFINED_FLAG    (-5)
#define E_ERROR_ARGUMENT	  (-16)

#define E_USER_ERROR_BASE   (-1000)

#define mIsSuccess(code)    ((code)>=0)
#define mIsFailure(code)    ((code)<0)

/* This is the data structure for the output video frame buffer */
typedef struct BaseVideoFrame
{
    uint8  *lum;       /* Luminance pointer */
		uint8  *cb;        /* Cb pointer */
		uint8  *cr;        /* Cr pointer */
		uint16  width;     /* Width of the frame */
		uint16  height;    /* Height of the frame */
		uint32  timeStamp; /* Playback Timestamp */

} tBaseVideoFrame;

typedef enum {YUV422INT_LE, YUV422INT_BE} EBufferLayout422; 

#endif  /* INCLUDE_COMMON */
