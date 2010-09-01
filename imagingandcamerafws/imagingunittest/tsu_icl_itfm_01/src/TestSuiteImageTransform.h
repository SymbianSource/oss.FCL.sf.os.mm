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
//

#if (!defined __TEST_SUITE_IMAGE_CONVERT_H_)
#define __TEST_SUITE_IMAGE_CONVERT_H_

#include <testframework.h>

/**
 *
 * CTestSuiteImageTransform is the test suite
 *
 * @lib "TSU_ICL_ImageTransform.lib"
 *
 */
class  CTestSuiteImageTransform : public CTestSuite
	{
public:
	void InitialiseL();
	~CTestSuiteImageTransform();
	void AddTestStepL(CTestStepImageTransform* ptrTestStep );
	//TPtrC GetVersion();
	virtual TPtrC GetVersion() const;
	const TDesC& DefaultPath() const;

private:
	void InitSystemPath();
	
private:
	TFileName iDefaultPath;
	};


/**
@panic  EBadArgument is raised if a precondition on an argument is violated
*/

enum TS_ImageTransformPanic
	{
	EBadArgument,
	};

/**

The function panic raises a panic from within the TS_ImageTransform library
@param The argument is a member of the enumeration TS_ImageTransformPanic
    
@lib TSU_ICL_ImageTransform.lib
@panic The panics raised by this function are:
     EBadArgument

*/


GLDEF_C void Panic( TS_ImageTransformPanic aError);

#endif // __TEST_SUITE_IMAGE_CONVERT_H_
