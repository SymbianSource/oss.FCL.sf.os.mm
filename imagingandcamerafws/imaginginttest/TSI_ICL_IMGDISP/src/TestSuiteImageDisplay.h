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

#if (!defined __TESTSUITEIMAGEDISPLAY_H__)
#define __TESTSUITEIMAGEDISPLAY_H__

#include <testframework.h>


static const TInt KTesPluginUidValue=0x101F7C4E;
static const TUid KTesPluginUid={KTesPluginUidValue};

/**
 *
 * CTestSuiteImageDisplay is the test suite
 *
 * @lib "TSU_ICL_ImageTransform.lib"
 *
 */
class  CTestSuiteImageDisplay : public CTestSuite
	{
public:
	void InitialiseL();
	~CTestSuiteImageDisplay();
	void AddTestStepL(CTestStepImageDisplay* ptrTestStep );
	//TPtrC GetVersion();
	virtual TPtrC GetVersion() const;
	void DefaultPath(TFileName& aName);

private:
	void InitSystemPath();
	TBool iFBSSession;
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


void Panic( TS_ImageTransformPanic aError);

#endif // __TESTSUITEIMAGEDISPLAY_H__
