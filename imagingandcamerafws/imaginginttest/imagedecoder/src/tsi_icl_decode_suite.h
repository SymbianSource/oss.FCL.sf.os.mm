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
// This file contains test suite class declaration for image decoding integration tests.
// Test Design: //EPOC/DV3/team/2005/Multimedia/WorkingDocumentation/Test_Design/ICL_IntTestDesign_ImageDecoder.doc
// Test Spec: //EPOC/DV3/team/2005/Multimedia/WorkingDocumentation/Test_Specs/ICL_IntTestSpec_ImageDecoder.xls
// 
//


#ifndef TSI_ICL_DECODE_SUITE_H
#define TSI_ICL_DECODE_SUITE_H


// Test system includes
#include <testframework.h>


// Test suite class
class CTestSuiteImageDecoder : public CTestSuite
    {
public:
    static CTestSuiteImageDecoder* NewL();

    ~CTestSuiteImageDecoder();

    // from CTestSuite
    // Provides version of the suite
    TPtrC GetVersion() const;

    // from CTestSuite
    // Creates all the test steps and stores them inside CTestSuiteImageDecoder
    void InitialiseL();

private:
    CTestSuiteImageDecoder();
	};


#endif // TSI_ICL_DECODE_SUITE_H
