// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
// TestStepMeasurePerformance.h
// This file contains Test step implementations for
// ICL performance assessment
//

#if (!defined __TESTSTEP_ICLPERFORMANCE_H__)
#define __TESTSTEP_ICLPERFORMANCE_H__


#include "TestStepIclPfm.h"
#include <bitmaptransforms.h>


enum TIclProfileTestType
	{
	ENormal = 0,
	EWait = 1
	};

class TPerformanceTestParams
	{
public:
    enum TDriveType
        {
        EDstMediaRamDrive,
        EDstMediaMMCDrive
        };
public:
	const TText*       		iSrcFileName;		// source file name
	const TText*       		iRefFileName;		// reference file name
	TDisplayMode			iDisplayMode;		// decoding display mode
	TInt					iReductionFactor;	// reduction factor
	TInt					iMaxDecodeTime;		// in microseconds
	CImageDecoder::TOptions	iDecoderOptions;	// TOptions parameter
	const TText*       		iConversionType;	// conversion type
	TBool					iUseMemBuffer;		// use memory buffer
	TInt                    iEncoderQuality;    // encoder quality settings
	TInt                    iEncoderFormat;     // sampling format
	TInt                    iDstMediaType;      // destination media type
	};

struct TIclProfileTest
	{
	const TText* iTestName;
	TImageFormat iFormat;
	TIclProfileTestType iTestType;
	const TPerformanceTestParams* iPerfTstParams;
	};


class  CActiveListener;


/**
  *  CMmIclPfmTest01
  */

class CMmIclPfmTest001 : public CTestStepIclPerformance
	{
public:
	CMmIclPfmTest001();
	virtual  TVerdict DoTestStepL();
	
	TVerdict DoProfileL(const TIclProfileTest aProfileTest);
	void DecodeFileToBitmapL(const TIclProfileTest aProfileTest);
protected:
	TUint iDecoderOptions;

	};

#endif // __TESTSTEP_ICLPERFORMANCE_H__
