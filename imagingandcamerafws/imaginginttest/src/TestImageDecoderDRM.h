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
// The enabling of agent's user interface feature is tested.
// 
//

#ifndef __TEST_IMAGEDECODERDRM_H__
#define __TEST_IMAGEDECODERDRM_H__

#include "TestStepConversion.h"

// Positive Test

/**
*
* Enable agent's user interface for errors and confirmation requests property.
* Read and decode a protected image, a JPEG, from a file for VIEW intent and 
* specifying MIME type.
*
* @lib "TSI_ICL_FRM_00.lib"
*
* @class CTestImageDecDrmEnableAgentUI
*
*/
class CTestImageDecDrmEnableAgentUI : public CTestStepConversion
    {
public:
    static CTestImageDecDrmEnableAgentUI* NewL(const TDesC& aTestName);

    // from RTestStep
    virtual TVerdict DoTestStepL();

private:
    CTestImageDecDrmEnableAgentUI(const TDesC& aTestName);

    TVerdict TestStep_I_0761L();

    void OpenImageAndDecodeFromFileSourceL(
        const TDesC& aFileName, 
        const TDesC& aUniqueId, 
        ContentAccess::TIntent aIntent);
    };

#endif //__TEST_IMAGEDECODERDRM_H__
