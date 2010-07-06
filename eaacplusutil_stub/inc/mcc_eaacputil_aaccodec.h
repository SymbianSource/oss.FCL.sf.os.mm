/*

* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).

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

* Description:  Class definition for the frame table functions.

 *

*/
/*

*******************************************************************************

Product     : Decoder Parser Utility

Module      : 

File        : 

Description : 

*******************************************************************************

*/





#ifndef INCLUDE_EAACPLUSUTILITY_H 

#define INCLUDE_EAACPLUSUTILITY_H



#ifdef SYMBIAN_BUILD

#include<e32def.h>

//#else /* SYMBIAN_BUILD */

//#define IMPORT_C

//#define EXPORT_C

#endif /* SYMBIAN_BUILD */



struct BaseAudioDecoder;

// FORWARD DECLARATIONS



#ifdef __cplusplus

extern "C"

{

#endif /* __cplusplus */



#ifndef INCLUDE_COMMON

typedef signed char			int8;

typedef unsigned char		uint8;

typedef short int			int16;

typedef unsigned short int	uint16;

typedef int				    int32;

typedef unsigned int		uint32;



#define E_SUCCESS		    0

#define	E_FAILURE		    (-1)



#endif



typedef struct EAacpAudioFrameInfo  

{

	int OutFrameSize;

	int NoOfSamples;

	int SamplingFrequency;

	int NoOfChannels;

	int Profile;

	int ObjectType ;		

	int OutSamplingFrequency;

	int DownSampledMode;

	int SBR;

	int PS;



} CEAacpAudioFrameInfo ; 



IMPORT_C int32 GetFrameInfo (uint8* aHeaderbuffer, 

							 int32* aHeaderSize,

							 CEAacpAudioFrameInfo *FrameInfo,

							 int32 format,

							 uint8* aDatabuffer, 

							 int32 aDataLength

							 );



IMPORT_C int32 GetFrameLength(uint8* aBuffer,

							  int32 aBufferLength,

							  int32 aFormat,

							  int32 *aFrameLength,

							  //int32 samplingFreq

							  CEAacpAudioFrameInfo *FrameInfo

							  );



IMPORT_C int32 GetAdtsHeaderFrameLength(uint8* aBuffer,

							  int32 aBufferLength,

							  int32 *aFrameLength

							  );



#ifdef __cplusplus

}

#endif



#endif /*   INCLUDE_AAC_DECODER_H   */

