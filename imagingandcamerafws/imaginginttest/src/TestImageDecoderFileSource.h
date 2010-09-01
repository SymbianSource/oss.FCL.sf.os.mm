// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// This file contains testing steps for the integration test of ImageDecoder.
// The tests are for opening the image file with TMMFileSource/TMMFileHandleSource
// and decoding it.
// 
//

#ifndef __TESTIMAGEDECODERFILESOURCE_H__
#define __TESTIMAGEDECODERFILESOURCE_H__

#include "TestStepConversion.h"

const TInt KMaxMimeLength = 256;

// Positive Tests

/**
*
* Read and decode an image, a JPEG, from a file passing FileSource.
*
* @lib "TSI_ICL_FRM_00.lib"
*
* @class CTestImageDecFileSource
*
*/
class CTestImageDecFileSource : public CTestStepConversion
    {
public:
    static CTestImageDecFileSource* NewL(const TDesC& aTestName);

    // from RTestStep
    virtual TVerdict DoTestStepL();

private:
    CTestImageDecFileSource(const TDesC& aTestName);
    void OpenImageAndDecodeFromFileSourceL(const TDesC& aFileName);

    TVerdict TestStep_I_0800_0801L();
    };

/**
*
* Read and decode an image, a JPEG, from a file passing FileHandleSource.
*
* @lib "TSI_ICL_FRM_00.lib"
*
* @class CTestImageDecFileHandleSource
*
*/
class CTestImageDecFileHandleSource : public CTestStepConversion
    {
public:
    static CTestImageDecFileHandleSource* NewL(const TDesC& aTestName);

    // from RTestStep
    virtual TVerdict DoTestStepL();

private:
    CTestImageDecFileHandleSource(const TDesC& aTestName);
    void OpenImageAndDecodeFromFileHandleSourceL(const TDesC& aFileName);

    TVerdict TestStep_I_0802_0803L();
    };

/**
*
* Read and decode an image, a JPEG, from a file passing FileSource passing a bitmap created using CreateExtendedBitmap.
*
* @lib "TSI_ICL_FRM_00.lib"
*
* @class CTestImageDecFileSource
*
*/
class CTestImageDecFileSourceExtendedBitmap : public CTestStepConversion, public MFbsExtendedBitmapInitializer
    {
public:
    static CTestImageDecFileSourceExtendedBitmap* NewL(const TDesC& aTestName);

    // from RTestStep
    virtual TVerdict DoTestStepL();

private:
    CTestImageDecFileSourceExtendedBitmap(const TDesC& aTestName);
    void OpenImageAndDecodeFromFileSourceL(const TDesC& aFileName);
    TInt InitExtendedBitmap(TAny* aData, TInt aDataSize);

    TVerdict TestStep_I_0804L();
    };

#endif //__TESTIMAGEDECODERFILESOURCE_H__
