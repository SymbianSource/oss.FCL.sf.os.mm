// Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#if (!defined __TEST_SUITE_GIF_SCALER_H_)
#define __TEST_SUITE_GIF_SCALER_H_

#include <testframework.h>

/**
 *
 * CTestSuiteGifScaler is the test suite
 *
 * @lib "TSU_ICL_GSCALER.lib"
 *
 */
class  CTestSuiteGifScaler : public CTestSuite
	{
public:
	void InitialiseL();
	~CTestSuiteGifScaler();
	void AddTestStepL(CTestStepGifScaler* ptrTestStep );
	TPtrC GetVersion();
	const TDesC& DefaultPath() const;

private:
	void InitSystemPath();
	
private:
	TFileName iDefaultPath;
	};


/**
@panic  EBadArgument is raised a precondition on an argument is violated
*/

enum TS_GifScalerPanic
	{
	EBadArgument,
	};

/**

The function panic raises a panic from within the TS_GifScaler library
@param The argument is a member of the enumeration TS_GifScalerPanic
    
@lib TSU_ICL_GSCALER.lib
@panic The panics raised by this function are:
     EBadArgument

@xxxx

*/


GLDEF_C void Panic( TS_GifScalerPanic aError);

#endif // __TEST_SUITE_GIF_SCALER_H_
