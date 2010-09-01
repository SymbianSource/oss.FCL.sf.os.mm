// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// This file contains test suite class declaration for image encoding integration tests.
// Test Design: //EPOC/DV3/team/2005/Multimedia/WorkingDocumentation/Test_Design/ICL_IntTestDesign_ImageDecoder.doc
// Test Spec: //EPOC/DV3/team/2005/Multimedia/WorkingDocumentation/Test_Specs/ICL_IntTestSpec_ImageEncoder.xls
// 
//


#ifndef TSI_ICL_ENCODE_SUITE_H
#define TSI_ICL_ENCODE_SUITE_H

// Test system includes
#include <testframework.h>


// Test suite class
class CTestSuiteImageEncoder : public CTestSuite
    {
public:
    static CTestSuiteImageEncoder* NewL();

    ~CTestSuiteImageEncoder();

    // from CTestSuite
    // Provides version of the suite
    TPtrC GetVersion() const;

    // from CTestSuite
    // Creates all the test steps and stores them inside CTestSuiteImageEncoder
    void InitialiseL();

private:
    CTestSuiteImageEncoder();
	};

#endif // TSI_ICL_ENCODE_SUITE_H
