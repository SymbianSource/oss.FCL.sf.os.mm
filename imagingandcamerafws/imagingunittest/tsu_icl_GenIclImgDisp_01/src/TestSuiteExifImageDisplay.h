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

#if (!defined TESTSUITEEXIFIMAGEDISPLAY_H)
#define TESTSUITEEXIFIMAGEDISPLAY_H

#include <testframework.h>

class CTestStepIclImageDisplay;

/**
 *
 * CTestSuiteExifImageDisplay is the test suite
 *
 *
 */
class  CTestSuiteExifImageDisplay : public CTestSuite
	{
public:
	void InitialiseL();
	~CTestSuiteExifImageDisplay();
	void AddTestStepL(CTestStepIclImageDisplay* ptrTestStep );
	//TPtrC GetVersion();
	virtual TPtrC GetVersion() const;
	void DefaultPath(TFileName& aName);

private:
	void InitSystemPath();
	TBool iFBSSession;
	TFileName iDefaultPath;
	};


/**

The function panic raises a panic from within the TS_ImageTransform library
@param The argument is a member of the enumeration TS_ImageTransformPanic
    
@lib TSU_ICL_ImageTransform.lib
@panic The panics raised by this function are:
     EBadArgument

*/
enum TS_ExifImageDisplayPanic
	{
	EBadArgument	=1,
	EAPIBroken,
	};

void Panic( TS_ExifImageDisplayPanic aError);



class CCallbackTimer : public CTimer
   {
public:
   static CCallbackTimer* NewLC();
	~CCallbackTimer();
   void After(TTimeIntervalMicroSeconds32 aCancelDelay, TCallBack aCallback);

private:
	CCallbackTimer();
   // from CActive
    void RunL();

private:
    TTimeIntervalMicroSeconds32 iCancelDelay;
    TCallBack iCallback; 
    };

#endif // TESTSUITEEXIFIMAGEDISPLAY_H
