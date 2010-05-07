// Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef TSU_OMXIL_DUMMYCONTENTPIPE_SUITE_H_
#define TSU_OMXIL_DUMMYCONTENTPIPE_SUITE_H_

#include <testframework.h>

/**
 *
 * CTestSuite, which is the base class for 
 * the CTestSuite_OMXIL_DummyContentPipe.
 *
 */
class  CTestSuite_OMXIL_DummyContentPipe : public CTestSuite
	{
	public:
	
		void InitialiseL( void );
		virtual ~CTestSuite_OMXIL_DummyContentPipe();
		virtual TPtrC GetVersion( void ) const;
	};


#endif /* TSU_OMXIL_DUMMYCONTENTPIPE_SUITE_H_ */
