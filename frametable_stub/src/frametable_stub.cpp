/*

* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).

* All rights reserved.

* This component and the accompanying materials are made available

* under the terms of "Eclipse Public License v1.0"

* which accompanies this distribution, and is available

* at the URL "http://www.eclipse.org/legal/epl-v10.html".

*

* Initial Contributors:

* Nokia Corporation - initial contribution.

*

* Contributors:

*

* Description:  Class definition for the frame table functions.

 *

*/






#include <frametable.h>
#include <e32debug.h>


CFrameTable::CFrameTable()

    {

    }

void CFrameTable::ConstructL()

    {

    }

EXPORT_C CFrameTable* CFrameTable::NewL()

    {

    CFrameTable* self = new(ELeave) CFrameTable;

    return self;

    }

EXPORT_C CFrameTable::~CFrameTable()

    {

    }

EXPORT_C TInt CFrameTable::InitFrameTable(TInt aSampleRate, TInt aSamplesPerFrame)

    {
    	 RDebug::Print(_L("CFrameTable::InitFrameTable aSampleRate[%d], aSamplesPerFrame[%d]"), aSampleRate, aSamplesPerFrame);
    	return KErrNone;
    }

EXPORT_C TInt CFrameTable::SubmitTableEntry(TUint aPos)

    {
    	RDebug::Print(_L("CFrameTable::InitFrameTable aPos[%d]"), aPos);
    	return KErrNone;
   }

TInt CFrameTable::SubmitLowResTableEntry(TBufRefPosEntry& aEntry)

    {
    	RDebug::Print(_L("CFrameTable::InitFrameTable aEntry[%d]"), aEntry);
			return KErrNone;
    }

EXPORT_C TInt CFrameTable::FindFramePosFromTime(TUint& aTimeMs, TUint& aPos)

    {
    	RDebug::Print(_L("CFrameTable::InitFrameTable aTimeMs[%d], aPos[%d]"), aTimeMs, aPos);
    	return KErrNone;
    }



EXPORT_C TInt CFrameTable::FindFrameTimeFromPos(TUint& aTimeMs, TUint& aPos)

    {
    	RDebug::Print(_L("CFrameTable::InitFrameTable aTimeMs[%d], aPos[%d]"), aTimeMs, aPos);
    	return KErrNone;
    }

EXPORT_C TInt CFrameTable::LastFramePos(TUint& aPos)

    {
    	RDebug::Print(_L("CFrameTable::InitFrameTable aPos[%d]"), aPos);
    	return KErrNone;
    }

EXPORT_C TInt CFrameTable::LastFrameTime(TUint& aTimeMs)

   {
   	RDebug::Print(_L("CFrameTable::InitFrameTable aTimeMs[%d]"), aTimeMs);
    	return KErrNone;
    }

EXPORT_C void CFrameTable::ResetTable()

    {
    	
    }

EXPORT_C void CFrameTable::ShrinkTable()

    { 
    	
    }



EXPORT_C TInt CFrameTable::SetSourceReference(TUint aTimeMs, TUint aPos)

    { 
    	RDebug::Print(_L("CFrameTable::InitFrameTable aTimeMs[%d], aPos[%d]"), aTimeMs, aPos);
    	return KErrNone;
    }



EXPORT_C TInt CFrameTable::RegisterForEvent(TFrameTableEvent aEvent, MFrameTableEventObserver* aObserver, TUint aParam)

    {
    	RDebug::Print(_L("CFrameTable::InitFrameTable aEvent[%d], aObserver[%d], aParam[%d]"), aEvent, aObserver, aParam);
			return KErrNone;
    }


EXPORT_C TInt CFrameTable::UnRegisterForEvent(TFrameTableEvent aEvent, MFrameTableEventObserver* /*aObserver*/)

    {
    	RDebug::Print(_L("CFrameTable::InitFrameTable aEvent[%d]"), aEvent);
    	return KErrNone;
    }

EXPORT_C TInt CFrameTable::Bitrate()

    {
			return KErrNone;
    }


TUint CFrameTable::CalcFrameFromTimeMs(TUint aTimeMs)

    {
    	RDebug::Print(_L("CFrameTable::InitFrameTable aTimeMs[%d]"), aTimeMs);
    	return KErrNone; 
    }



EXPORT_C TInt CFrameTable::IsSeeking(TBool& aIsSeeking)

    {
    	RDebug::Print(_L("CFrameTable::InitFrameTable aIsSeeking[%d]"), aIsSeeking);
			return KErrNone;
    }



EXPORT_C TInt CFrameTable::GetLastPosEvent(TUint& aPos)

    {
    	RDebug::Print(_L("CFrameTable::InitFrameTable aPos[%d]"), aPos);
			return KErrNone;
    }



