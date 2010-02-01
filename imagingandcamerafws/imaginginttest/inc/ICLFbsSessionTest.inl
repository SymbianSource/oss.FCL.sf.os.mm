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

#ifndef __ICLFbsSessionTest_inl
#define __ICLFbsSessionTest_inl



/*
 * Default Constructor	
 */
inline CICLFbsSessionTest::CICLFbsSessionTest()
	{ 
	}


/*
 * Default Destructor	
 */
inline CICLFbsSessionTest::~CICLFbsSessionTest()
	{ 
	RFbsSession::Disconnect(); 
	}

/*
 * Makes an actual Connection to RFbsSession
 *
 */	
inline void CICLFbsSessionTest::ConnectToSessionL()
	{ 
	User::LeaveIfError(RFbsSession::Connect()); 
	}		

/* 
 * Constructs the object and pushes it on the CleanUpStack
 * 
 * NewLC()
 *
 * @return Returns Pointer Of Type CICLFbsSessionTest
 * 
 */
inline CICLFbsSessionTest* CICLFbsSessionTest::NewLC()
	{
	CICLFbsSessionTest* self = new (ELeave) CICLFbsSessionTest;
    CleanupStack::PushL(self);
    self->ConnectToSessionL();
    return self;
	}

#endif
