
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

#ifndef __TSI_ICLCODSTEP_H__
#define __TSI_ICLCODSTEP_H__

#include <e32base.h>
#include <f32file.h>
#include <testframework.h>

/**
 *
 * Enum to define which pic type we are testing.
 * From this will be derived the test step name
 *
 */
enum TIclCodPicType
{
	EPicTypeBMP = 1,
	EPicTypePNG,
	EPicTypeJPG
};

/**
 *
 * CActiveListener provides the asynchronous operation
 * of an active object
 *
 * @class CActiveListener
 *
 */
class CActiveListener : public CActive
	{
public:
	CActiveListener() : CActive(CActive::EPriorityIdle) { CActiveScheduler::Add(this); }
	void InitialiseActiveListener() { iStatus = KRequestPending; SetActive(); }
	// From CActive
	virtual void RunL() { CActiveScheduler::Stop(); }
	virtual void DoCancel() {};
	};

/**
 * It's a base class for all test steps.
 *
 * @class CTestMmfVclntStep 
 * @brief Test class that enables tests.
 *
 */
class RIclCodStep : public RTestStep
	{
public:
	RIclCodStep(const TIclCodPicType aType, const TInt aNum);

	//From RTest
	TVerdict OpenL();
	TVerdict DoTestStepL();
	void Close();

private:
	CActiveScheduler* iScheduler;
	CActiveListener* iListener;
	RFs iFs;
	TFileName iFileName;
	TInt iError;
	};

#endif // __TSI_ICLCODSTEP_H__

