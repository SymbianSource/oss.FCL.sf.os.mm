// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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
//

/**
 @file Te_tsi_icl_cod_5SuiteStepBase.cpp
 @internalTechnology
*/

#include "Te_tsi_icl_cod_5SuiteStepBase.h"
#include "Te_tsi_icl_cod_5SuiteDefs.h"

// Device driver constants

TVerdict CTe_tsi_icl_cod_5SuiteStepBase::doTestStepPreambleL()
/**
 * @return - TVerdict
 * Implementation of CTestStep base class virtual
 * It is used for doing all initialisation common to derived classes in here.
 * Make it being able to leave if there are any errors here as there's no point in
 * trying to run a test step if anything fails.
 * The leave will be picked up by the framework.
 */
	{
	// process some common pre setting to test steps then set SetTestStepResult to EFail or Epass.
	SetTestStepResult(EPass);
	return TestStepResult();
	}

TVerdict CTe_tsi_icl_cod_5SuiteStepBase::doTestStepPostambleL()
/**
 * @return - TVerdict
 * Implementation of CTestStep base class virtual
 * It is used for doing all after test treatment common to derived classes in here.
 * Make it being able to leave
 * The leave will be picked up by the framework.
 */
	{
	// process some common post setting to test steps then set SetTestStepResult to EFail or Epass.
	//SetTestStepResult(EPass);  // or EFail
	return TestStepResult();
	}

CTe_tsi_icl_cod_5SuiteStepBase::~CTe_tsi_icl_cod_5SuiteStepBase()
	{
	}

CTe_tsi_icl_cod_5SuiteStepBase::CTe_tsi_icl_cod_5SuiteStepBase()
	{
	}

TUint CTe_tsi_icl_cod_5SuiteStepBase::CompareBitmaps(const CFbsBitmap* aBitmap1, const CFbsBitmap* aBitmap2)
	{
	INFO_PRINTF1(_L("Comparing bitmap with reference image"));
	const TSize bitmap1Size = aBitmap1->SizeInPixels();
	const TSize bitmap2Size = aBitmap2->SizeInPixels();
    
    if (bitmap1Size != bitmap2Size)
        {
        INFO_PRINTF5(_L("Size mismatch! - (%d, %d) != (%d, %d)"), 
            bitmap1Size.iWidth, bitmap1Size.iHeight, 
            bitmap2Size.iWidth, bitmap2Size.iHeight);
        return 1;
        }

    TUint pixelCount = 0;   // all pixels in picture
    TUint identicalPixels = 0; // only identical pixels
    TUint differentPixels = 0;
    
    // compare bitmaps pixel by pixel
    for (TInt y = 0; y < bitmap1Size.iHeight; y++)
        {
        for (TInt x = 0; x < bitmap1Size.iWidth; x++)
            {
            TPoint position(x, y);
            TRgb bitmap1pixel;
            TRgb bitmap2pixel;
            aBitmap1->GetPixel(bitmap1pixel, position);
            aBitmap2->GetPixel(bitmap2pixel, position);
            
            pixelCount++;

            if(bitmap1pixel.Internal() == bitmap2pixel.Internal())
            	{
            	identicalPixels++;
            	}
            else
            	{
            	differentPixels++;
            	}     
            }
        }
    
    if (pixelCount == identicalPixels)
        {
        INFO_PRINTF1(_L("Bitmaps are fully identical."));
        }
    else
    	{
    	INFO_PRINTF2(_L("Bitmaps have have %d pixels difference."), differentPixels);
    	}

    return differentPixels;
    }

