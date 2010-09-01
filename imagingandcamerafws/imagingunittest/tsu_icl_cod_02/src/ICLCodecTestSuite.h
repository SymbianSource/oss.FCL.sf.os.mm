
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
// ICL Codec Tests - see ../UnitTest/ICL/Documentation/ICL Codecs Unit Test Specification.xls
// 
//

#ifndef __ICLCodecTestSuite_h__
#define __ICLCodecTestSuite_h__

class CICLCodecSuite : public CTestSuite
	{
public:
	static CICLCodecSuite* NewL();
	~CICLCodecSuite();
	const TDesC& DefaultPath() const;
	
protected:
	CICLCodecSuite();

	void InitSystemPath();
	// frm CTestSuite
	void InitialiseL();
	TPtrC GetVersion();
	
private:
	void DummyCalls();
	
private:
	TFileName iDefaultPath;
	};

#endif //__ICLCodecTestSuite_h__

