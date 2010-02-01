
// Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// ICLCodecTestStep.h
// ICL Codec Tests - see "V:\Dev\GT\GT 0137 Multimedia codec - Msrvr\Testing\Unit Test Specifications\Typhoon\"
// "SGL.GT0137.xxx Rev0.01 MMF ICL COD Unit Test Specification.xls"
// 
//

#ifndef __ICLMDAVIDTESTSTEP_H__
#define __ICLMDAVIDTESTSTEP_H__

#include <testframework.h>
#include "TMDACORE.H"

//

class CIclMdaVidTestStep : public CTestStep, public MDummyConsole
	{
public:
	static CIclMdaVidTestStep* NewL(const TDesC& aTestName);
	~CIclMdaVidTestStep();
protected:
	CIclMdaVidTestStep(const TDesC& aTestName);
	void ConstructL();
	// from CDummyConsole
protected:
	void Printf(const TDesC& aDes);
	void Printf(const TDesC& aDes, const TInt aNum);
	// from CTestStep
protected:
	TVerdict OpenL();
	void Close();
	TVerdict DoTestStepL();
protected:
	CActiveScheduler* iScheduler;
	CMdaCoreTestEngine* iEngine;
	};

#endif // __ICLMDAVIDTESTSTEP_H__

