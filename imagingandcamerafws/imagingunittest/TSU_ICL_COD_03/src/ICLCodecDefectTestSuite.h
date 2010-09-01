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
//

#if (!defined __ICL_CODEC_DEFECT_TEST_SUITE_H_)
#define __ICL_CODEC_DEFECT_TEST_SUITE_H_

#include <testframework.h>

/**
 *
 * @lib "TSU_ICL_COD_03.lib"
 *
 * @xxxx
 *
 */
class  CICLCodecDefectTestSuite : public CTestSuite
	{
public:
	EXPORT_C static CICLCodecDefectTestSuite* NewL();
	~CICLCodecDefectTestSuite();
	void InitialiseL();
	TPtrC GetVersion() const;
	void InitSystemPath();
	const TDesC& DefaultPath() const;

private:
	TFileName iDefaultPath;
	};


#endif //__ICL_CODEC_DEFECT_TEST_SUITE_H_
