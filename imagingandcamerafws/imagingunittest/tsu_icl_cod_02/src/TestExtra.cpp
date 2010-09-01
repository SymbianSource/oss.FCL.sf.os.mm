
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
// TextExtra.cpp - testframework version
// 
//

#include "TestExtra.h"

_LIT(KTextFailedTest,"!!! TEST FAILED !!!");
//const TInt KMaxMsgLength=256;		// EABI warning removal

//
//

RTestExtra::RTestExtra(const TDesC& /*aTitle*/)
	{}

// TestFailed - all versions - prints out string and waits for user to acknowledge.
void RTestExtra::TestFailed(TInt aError)  
	{
	TBuf<64> msg(KTextFailedTest);
	msg.Append(' ');
	msg.AppendNum(aError);
	TestFailed(_L("%S"), &msg);
	}

void RTestExtra::TestFailed(TRefByValue<const TDesC> aFmt,...)
	{
	VA_LIST list;
	VA_START(list, aFmt);

	PrintList(aFmt, list);

	VA_END(list);

	*iVerdict = EFail;
	}

void RTestExtra::SetLog(CLog *aLog)
	{
	iLog = aLog;
	}

void RTestExtra::SetLinkToVerdict(TVerdict& aVerdict)
	{
	iVerdict = &aVerdict;
	}

void RTestExtra::Printf(TRefByValue<const TDesC> aFmt,...)
	{
	VA_LIST list;
	VA_START(list, aFmt);

	PrintList(aFmt, list);

	VA_END(list);
	}

void RTestExtra::PrintList(TRefByValue<const TDesC> aFmt,VA_LIST aList)
	{
#if 1
	// TODO this code should work but seems broken
	iLog->Log(aFmt, aList);
#else
	const TInt KMaxMsgLength=256;
	TBuf<KMaxMsgLength> buffer;
	buffer.Zero();
	buffer.AppendFormatList(aFmt, aList);

	iLog->Log(_L("%S"), &buffer);
#endif
	}

void RTestExtra::Close()
	{
	}

