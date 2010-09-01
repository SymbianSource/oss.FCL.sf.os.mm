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
// This file contains testing steps for the unit test of the ImageDecoder.
// An image file is opened for DRM intent.
// 
//

#ifndef __TEST_IMAGEDECODERDRM_H__
#define __TEST_IMAGEDECODERDRM_H__

#include "ICLFrameWorkTestStep.h"

// Positive Tests

/**
*
* Base class to open and decode a protected JPEG image, 
* opened by passing TMMFileSource and TMMFileHandleSource.
*
* @lib "TSU_ICL_FRM_01.lib"
*
* @class RTImageOpenAndDecode
*
*/
class RTImageOpenAndDecode : public RICLFrameworkTestStep
    {
public:
    void OpenImageAndDecodeFromFileSourceL(
        const TDesC& aFileName, 
        const TDesC& aUniqueId, 
        ContentAccess::TIntent aIntent,
        TBool aCompareToReference = ETrue);

    void OpenImageAndDecodeFromFileHandleSourceL(
        const TDesC& aFileName, 
        const TDesC& aUniqueId, 
        ContentAccess::TIntent aIntent);

    const TDesC8& GetMimeType();
    TFileName GetRefFilename();

    // from RTestStep
    virtual TVerdict OpenL();
	virtual void Close();

public:
    RFs iFileSession;
    TFileName iFileName;

private:
    CActiveScheduler* iActiveScheduler;
    };

/**
*
* Open a protected JPEG image having rights, from a file for 
* VIEW intent and specifying MIME type. Decode. (TMMFileSource).
*
* static CImageDecoder* FileNewL(RFs& aFs, const TMMSource& aFileSource, 
* const TDesC8& aMIMEType, const TOptions aOptions = EOptionNone);
*
* @lib "TSU_ICL_FRM_01.lib"
*
* @class RTImageDecDRMViewMthd1
*
*/
class RTImageDecDRMViewMthd1 : public RTImageOpenAndDecode
    {
public:
    static RTImageDecDRMViewMthd1* NewL(CTestSuite* aSuite);

    // from RTestStep
    virtual TVerdict DoTestStepL();

private:
    RTImageDecDRMViewMthd1(CTestSuite* aSuite);
    };


/**
*
* Open a protected JPEG image having rights, from a file for 
* PRINT intent and specifying MIME type. Decode. (TMMFileSource).
*
* static CImageDecoder* FileNewL(RFs& aFs, const TMMSource& aFileSource, 
* const TDesC8& aMIMEType, const TOptions aOptions = EOptionNone);
*
* @lib "TSU_ICL_FRM_01.lib"
*
* @class RTImageDecDRMPrintMthd1
*
*/
class RTImageDecDRMPrintMthd1 : public RTImageOpenAndDecode
    {
public:
    static RTImageDecDRMPrintMthd1* NewL(CTestSuite* aSuite);

    // from RTestStep
    virtual TVerdict DoTestStepL();

private:
    RTImageDecDRMPrintMthd1(CTestSuite* aSuite);
    };


/**
*
* Alloc test to open a protected JPEG image having rights, 
* from a file for VIEW intent and specifying MIME type. Decode. (TMMFileSource).
*
* static CImageDecoder* FileNewL(RFs& aFs, const TMMSource& aFileSource, 
* const TDesC8& aMIMEType, const TOptions aOptions = EOptionNone);
*
* @lib "TSU_ICL_FRM_01.lib"
*
* @class RTImageDecDRMViewAllocMthd1
*
*/
class RTImageDecDRMViewAllocMthd1 : public RTImageOpenAndDecode
    {
public:
    static RTImageDecDRMViewAllocMthd1* NewL(CTestSuite* aSuite);

    // from RTestStep
    virtual TVerdict DoTestStepL();

private:
    RTImageDecDRMViewAllocMthd1(CTestSuite* aSuite);
    };


/**
*
* Open a protected JPEG image having rights, from a file for VIEW intent. 
* Decode. (All defaults. TMMFileHandleSource).
*
* static CImageDecoder* FileNewL(RFs& aFs, const TMMSource& aFileSource, 
* const TOptions aOptions = EOptionNone, const TUid aImageType = KNullUid, 
* const TUid aImageSubType = KNullUid, const TUid aDecoderUid = KNullUid);
*
* @lib "TSU_ICL_FRM_01.lib"
*
* @class RTImageDecDRMViewMthd2
*
*/
class RTImageDecDRMViewMthd2 : public RTImageOpenAndDecode
    {
public:
    static RTImageDecDRMViewMthd2* NewL(CTestSuite* aSuite);

    // from RTestStep
    virtual TVerdict DoTestStepL();

private:
    RTImageDecDRMViewMthd2(CTestSuite* aSuite);
    };


/**
*
* Open a protected JPEG image having rights, from a file for VIEW intent 
* and specifying image type Uid. Decode. (TMMFileSource).
*
* static CImageDecoder* FileNewL(RFs& aFs, const TMMSource& aFileSource, 
* const TOptions aOptions = EOptionNone, const TUid aImageType = KNullUid, 
* const TUid aImageSubType = KNullUid, const TUid aDecoderUid = KNullUid);
*
* @lib "TSU_ICL_FRM_01.lib"
*
* @class RTImageDecDRMViewImgTypeMthd2
*
*/
class RTImageDecDRMViewImgTypeMthd2 : public RTImageOpenAndDecode
    {
public:
    static RTImageDecDRMViewImgTypeMthd2* NewL(CTestSuite* aSuite);

    // from RTestStep
    virtual TVerdict DoTestStepL();

private:
    RTImageDecDRMViewImgTypeMthd2(CTestSuite* aSuite);
    };


/**
*
* Open a protected JPEG image having rights, from a file for VIEW intent 
* and specifying decoder Uid. Decode. (TMMFileSource).
*
* static CImageDecoder* FileNewL(RFs& aFs, const TMMSource& aFileSource, 
* const TOptions aOptions = EOptionNone, const TUid aImageType = KNullUid, 
* const TUid aImageSubType = KNullUid, const TUid aDecoderUid = KNullUid);
*
* @lib "TSU_ICL_FRM_01.lib"
*
* @class RTImageDecDRMViewDecTypeMthd2
*
*/
class RTImageDecDRMViewDecTypeMthd2 : public RTImageOpenAndDecode
    {
public:
    static RTImageDecDRMViewDecTypeMthd2* NewL(CTestSuite* aSuite);

    // from RTestStep
    virtual TVerdict DoTestStepL();

private:
    RTImageDecDRMViewDecTypeMthd2(CTestSuite* aSuite);
    };


/**
*
* Alloc test to open a protected JPEG image having rights, 
* from a file for VIEW intent. Decode. (All defaults. TMMFileHandleSource).
*
* static CImageDecoder* FileNewL(RFs& aFs, const TMMSource& aFileSource, 
* const TOptions aOptions = EOptionNone, const TUid aImageType = KNullUid, 
* const TUid aImageSubType = KNullUid, const TUid aDecoderUid = KNullUid);
*
* @lib "TSU_ICL_FRM_01.lib"
*
* @class RTImageDecDRMViewAllocMthd2
*
*/
class RTImageDecDRMViewAllocMthd2 : public RTImageOpenAndDecode
    {
public:
    static RTImageDecDRMViewAllocMthd2* NewL(CTestSuite* aSuite);

    // from RTestStep
    virtual TVerdict DoTestStepL();

private:
    RTImageDecDRMViewAllocMthd2(CTestSuite* aSuite);
    };


/**
*
* Open a protected JPEG image having rights, from a file for PRINT intent 
* and specifying MIME type with Unique Id. Decode. (TMMFileSource).
*
* static CImageDecoder* FileNewL(RFs& aFs, const TMMSource& aFileSource, 
* const TDesC8& aMIMEType, const TOptions aOptions = EOptionNone);
*
* @lib "TSU_ICL_FRM_01.lib"
*
* @class RTImageDecDRMViewUniqueIdMthd1
*
*/
class RTImageDecDRMViewUniqueIdMthd1 : public RTImageOpenAndDecode
    {
public:
    static RTImageDecDRMViewUniqueIdMthd1* NewL(CTestSuite* aSuite);

    // from RTestStep
    virtual TVerdict DoTestStepL();

private:
    RTImageDecDRMViewUniqueIdMthd1(CTestSuite* aSuite);
    };


// Negative Tests

/**
*
* Open a protected JPEG image having NO rights, 
* from a file for VIEW intent and specifying MIME type. (TMMFileSource).
*
* static CImageDecoder* FileNewL(RFs& aFs, const TMMSource& aFileSource, 
* const TDesC8& aMIMEType, const TOptions aOptions = EOptionNone);
*
* @lib "TSU_ICL_FRM_01.lib"
*
* @class RTImageDecOpenDRMNoRightsMthd1
*
*/
class RTImageDecOpenDRMNoRightsMthd1 : public RTImageOpenAndDecode
    {
public:
    static RTImageDecOpenDRMNoRightsMthd1* NewL(CTestSuite* aSuite);

    // from RTestStep
    virtual TVerdict DoTestStepL();

private:
    RTImageDecOpenDRMNoRightsMthd1(CTestSuite* aSuite);
    };


/**
*
* Open a protected JPEG image having rights to VIEW, 
* from a file for UNKNOWN intent and specifying MIME type. (TMMFileSource).
*
* static CImageDecoder* FileNewL(RFs& aFs, const TMMSource& aFileSource, 
* const TDesC8& aMIMEType, const TOptions aOptions = EOptionNone);
*
* @lib "TSU_ICL_FRM_01.lib"
*
* @class RTImageDecOpenDRMUnknownMthd1
*
*/
class RTImageDecOpenDRMUnknownMthd1 : public RTImageOpenAndDecode
    {
public:
    static RTImageDecOpenDRMUnknownMthd1* NewL(CTestSuite* aSuite);

    // from RTestStep
    virtual TVerdict DoTestStepL();

private:
    RTImageDecOpenDRMUnknownMthd1(CTestSuite* aSuite);
    };


/**
*
* Open a protected JPEG image having rights, from a file for VIEW intent 
* with rights count set to 3 and specifying MIME type. Decode three times. 
* Open image for the fourth time. (TMMFileSource).
*
* static CImageDecoder* FileNewL(RFs& aFs, const TMMSource& aFileSource, 
* const TDesC8& aMIMEType, const TOptions aOptions = EOptionNone);
*
* @lib "TSU_ICL_FRM_01.lib"
*
* @class RTImageDecOpenDRMRightsCountMthd1
*
*/
class RTImageDecOpenDRMRightsCountMthd1 : public RTImageOpenAndDecode
    {
public:
    static RTImageDecOpenDRMRightsCountMthd1* NewL(CTestSuite* aSuite);

    // from RTestStep
    virtual TVerdict DoTestStepL();

private:
    RTImageDecOpenDRMRightsCountMthd1(CTestSuite* aSuite);
    };


/**
*
* Open a protected JPEG image having NO rights, from a file for VIEW intent. 
* (All defaults. TMMFileHandleSource).
*
* static CImageDecoder* FileNewL(RFs& aFs, const TMMSource& aFileSource, 
* const TOptions aOptions = EOptionNone, const TUid aImageType = KNullUid, 
* const TUid aImageSubType = KNullUid, const TUid aDecoderUid = KNullUid);
*
* @lib "TSU_ICL_FRM_01.lib"
*
* @class RTImageDecOpenDRMNoRightsMthd2
*
*/
class RTImageDecOpenDRMNoRightsMthd2 : public RTImageOpenAndDecode
    {
public:
    static RTImageDecOpenDRMNoRightsMthd2* NewL(CTestSuite* aSuite);

    // from RTestStep
    virtual TVerdict DoTestStepL();

private:
    RTImageDecOpenDRMNoRightsMthd2(CTestSuite* aSuite);
    };


/**
*
* Open a protected JPEG image having rights, from a file for UNKNOWN intent. 
* (All defaults. TMMFileHandleSource).
*
* static CImageDecoder* FileNewL(RFs& aFs, const TMMSource& aFileSource, 
* const TOptions aOptions = EOptionNone, const TUid aImageType = KNullUid, 
* const TUid aImageSubType = KNullUid, const TUid aDecoderUid = KNullUid);
*
* @lib "TSU_ICL_FRM_01.lib"
*
* @class RTImageDecOpenDRMUnknownMthd2
*
*/
class RTImageDecOpenDRMUnknownMthd2 : public RTImageOpenAndDecode
    {
public:
    static RTImageDecOpenDRMUnknownMthd2* NewL(CTestSuite* aSuite);

    // from RTestStep
    virtual TVerdict DoTestStepL();

private:
    RTImageDecOpenDRMUnknownMthd2(CTestSuite* aSuite);
    };


/**
*
* Open a protected JPEG image having rights, from a file for VIEW intent 
* with rights count set to 3 and specifying MIME type. Decode three times. 
* Open image for the fourth time. (All defaults. TMMFileHandleSource).
*
* static CImageDecoder* FileNewL(RFs& aFs, const TMMSource& aFileSource, 
* const TDesC8& aMIMEType, const TOptions aOptions = EOptionNone);
*
* @lib "TSU_ICL_FRM_01.lib"
*
* @class RTImageDecOpenDRMRightsCountMthd2
*
*/
class RTImageDecOpenDRMRightsCountMthd2 : public RTImageOpenAndDecode
    {
public:
    static RTImageDecOpenDRMRightsCountMthd2* NewL(CTestSuite* aSuite);

    // from RTestStep
    virtual TVerdict DoTestStepL();

private:
    RTImageDecOpenDRMRightsCountMthd2(CTestSuite* aSuite);
    };


/**
*
* Open a protected JPEG image having rights, from a file for VIEW intent 
* and specifying MIME type with INVALID Unique Id. (TMMFileSource).
*
* static CImageDecoder* FileNewL(RFs& aFs, const TMMSource& aFileSource, 
* const TDesC8& aMIMEType, const TOptions aOptions = EOptionNone);
*
* @lib "TSU_ICL_FRM_01.lib"
*
* @class RTImageDecDRMViewInvalIdMthd1
*
*/
class RTImageDecDRMViewInvalIdMthd1 : public RTImageOpenAndDecode
    {
public:
    static RTImageDecDRMViewInvalIdMthd1* NewL(CTestSuite* aSuite);

    // from RTestStep
    virtual TVerdict DoTestStepL();

private:
    RTImageDecDRMViewInvalIdMthd1(CTestSuite* aSuite);
    };
    
/**
*
* INC061633: NTT - CImageDecoder: KNullDesC should be KDefaultContentObject 
*
*/    
class RTImageDecDRMDefaContentTest : public RTImageOpenAndDecode
    {
public:
    static RTImageDecDRMDefaContentTest* NewL(CTestSuite* aSuite);

    // from RTestStep
    virtual TVerdict DoTestStepL();

private:
    RTImageDecDRMDefaContentTest(CTestSuite* aSuite);
    };
    

/**
*
* Open a file via CAF and decode
* *
* @lib "TSU_ICL_FRM_01.lib"
*
* @class RTImageDecDRMViewInvalIdMthd1
*
*/
class RTImageDecDRMViaCAF : public RTImageOpenAndDecode
    {
public:
    static RTImageDecDRMViaCAF* NewL(CTestSuite* aSuite);

    // from RTestStep
    virtual TVerdict DoTestStepL();

private:
    RTImageDecDRMViaCAF(CTestSuite* aSuite);
    };
    
    
/**
*
* DEF088101: ICL CAF result of ExecuteIntent not checked for conversion
*
*/
class RTImageDecOpenDRMConsumeRights : public RTImageOpenAndDecode
    {
public:
    static RTImageDecOpenDRMConsumeRights* NewL(CTestSuite* aSuite);

    // from RTestStep
    virtual TVerdict DoTestStepL();

private:
    RTImageDecOpenDRMConsumeRights(CTestSuite* aSuite);
    };


/**
*
* DEF090885: Rights can be cosumed prior to successful decoding of an image.
*
* Open a protected MNG image with multiple frames from a file with a VIEW intent 
* count of three. Decode once successfully, once with a decode error
* (No Memory), then 3 more times without memory errors.  Verify that the decode 
* error did not consume a viewing right so that only the last attempt to view
* fails due to no rights. 
*
*/
class RTImageDecOpenDRMConsumeRightsOnError : public RTImageOpenAndDecode
    {
public:
    static RTImageDecOpenDRMConsumeRightsOnError* NewL(CTestSuite* aSuite);

    // from RTestStep
    virtual TVerdict DoTestStepL();

private:
    RTImageDecOpenDRMConsumeRightsOnError(CTestSuite* aSuite);

    void OpenImageAndDecodeFromFileSourceL(
        const TDesC& aFileName, 
        const TDesC& aUniqueId, 
        ContentAccess::TIntent aIntent,
        TBool aFail);
    };

/**
*
* DEF090885: Rights can be cosumed prior to successful decoding of an image.
*
* Open a protected JPEG image with from a file with a VIEW intent 
* count of three. Decode using ConvertFrame once successfully, once with a decode error
* (KErrUnsupported), then 3 more times without errors.  Verify that the decode 
* error did not consume a viewing right so that only the last attempt to view
* fails due to no rights. 
*
*/
class RTImageDecOpenDRMConsumeRightsOnError2 : public RTImageOpenAndDecode
    {
public:
    static RTImageDecOpenDRMConsumeRightsOnError2* NewL(CTestSuite* aSuite);

    // from RTestStep
    virtual TVerdict DoTestStepL();

private:
    RTImageDecOpenDRMConsumeRightsOnError2(CTestSuite* aSuite);

    void OpenImageAndDecodeFromFileSourceL(
        const TDesC& aFileName, 
        const TDesC& aUniqueId, 
        ContentAccess::TIntent aIntent,
        TBool aFail);
    };
    
/**
*
* DEF090885: Rights can be cosumed prior to successful decoding of an image.
*
* Open a protected animated GIF image with from a file with a VIEW intent 
* count of three. Verify that intent is executed only once per image convert (not
* once per frame convert). 
*
*/
class RTImageDecOpenDRMConsumeRightsOnError3 : public RTImageOpenAndDecode
    {
public:
    static RTImageDecOpenDRMConsumeRightsOnError3* NewL(CTestSuite* aSuite);

    // from RTestStep
    virtual TVerdict DoTestStepL();

private:
    RTImageDecOpenDRMConsumeRightsOnError3(CTestSuite* aSuite);
    };

#endif //__TEST_IMAGEDECODERDRM_H__
