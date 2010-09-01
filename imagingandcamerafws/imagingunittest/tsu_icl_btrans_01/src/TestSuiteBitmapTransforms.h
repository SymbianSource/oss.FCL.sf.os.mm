// Copyright (c) 2001-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#if (!defined __TEST_SUITE_BITMAP_TRANSFORMS_H_)
#define __TEST_SUITE_BITMAP_TRANSFORMS_H_

#include <testframework.h>

/**
 *
 * CTestSuiteBitmapTransforms is the test suite
 *
 * @lib "TS_BitmapTransforms.lib"
 *
 */
class  CTestSuiteBitmapTransforms : public CTestSuite
	{
private:
	void InitSystemPath();
		
public:
	void InitialiseL( void );
	virtual ~CTestSuiteBitmapTransforms();
	void AddTestStepL( CTestStepBitmapTransforms* ptrTestStep );
	TPtrC GetVersion( void );
	const TDesC& DefaultPath() const;

private:
	TFileName iDefaultPath;		
	};


/**
@panic  EBadArgument is raised a precondition on an argument is violated
*/

enum TS_BitmapTransformsPanic
	{
	EBadArgument
	};

/**

The function panic raises a panic from within the TS_BitmapTransforms library
@param The argument is a member of the enumeration TS_BitmapTransformsPanic
    
@lib TS_BitmapTransfroms.lib
@panic The panics raised by this function are:
     EBadArgument

@xxxx

*/


GLDEF_C void Panic( TS_BitmapTransformsPanic aError);

#endif /* __TEST_SUITE_BITMAP_TRANSFORMS_H_  */
