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
// DecodeFileByNameByPluginUid.h
// 
//

#if (!defined __MNGPITESTSTEPIMAGEDISPLAY_H__)
#define __MNGPITESTSTEPIMAGEDISPLAY_H__

#include "TestStepImageDisplay.h"

/**
 *
 * The following classes provide test steps to exercise the ImageDisplay
 * functionality. See test specification for details of individual test cases.
 *
 */

/*
 * CScaleToRecommendedSize
 */
class CScaleToRecommendedSize : public CTestStepImgDisplayGeneric
	{
public:

	CScaleToRecommendedSize(const TImageDisplayUnitTestParams& aTestParams);

protected:
	virtual TVerdict TestL();
	TBool ConsumeFrame(const CFbsBitmap& aBitmap);

private:
	};

/*
 * CScaleToNonRecommendedSize
 */
class CScaleToNonRecommendedSize : public CTestStepImgDisplayGeneric
	{
public:

	CScaleToNonRecommendedSize(const TImageDisplayUnitTestParams& aTestParams);

protected:
	virtual TVerdict TestL();
	TBool ConsumeFrame(const CFbsBitmap& aBitmap);

private:
	};

#endif // __MNGPITESTSTEPIMAGEDISPLAY_H__
