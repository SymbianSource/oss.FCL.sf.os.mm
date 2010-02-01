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





//  INCLUDES

#include "FrameTable.h"

#include "DebugMacros.h"

#include <e32debug.h>



// CONSTANTS

const TInt KBuffersToKeep = 3;

const TInt KLowResPeriodMs = 1000; // approximate interval for entries stored in low res table is 1 sec



// ============================ MEMBER FUNCTIONS ===============================



// -----------------------------------------------------------------------------

// CFrameTable::CFrameTable

// C++ default constructor can NOT contain any code, that

// might leave.

// -----------------------------------------------------------------------------

//

CFrameTable::CFrameTable()

    {

    }



// -----------------------------------------------------------------------------

// CFrameTable::ConstructL

// Symbian 2nd phase constructor can leave.

// -----------------------------------------------------------------------------

//

void CFrameTable::ConstructL()

    {

    DP0(_L("CFrameTable::ConstructL"));

    SetSourceReference(0,0);

    }



// -----------------------------------------------------------------------------

// CFrameTable::NewL

// Two-phased constructor.

// -----------------------------------------------------------------------------

//

EXPORT_C CFrameTable* CFrameTable::NewL()

    {

    DP0(_L("CFrameTable::NewL"));



    CFrameTable* self = new(ELeave) CFrameTable;

    CleanupStack::PushL(self);

    self->ConstructL();

    CleanupStack::Pop(self);

    return self;

    }



// -----------------------------------------------------------------------------

// CFrameTable::~CFrameTable

// -----------------------------------------------------------------------------

//

EXPORT_C CFrameTable::~CFrameTable()

    {

    DP0(_L("CFrameTable::~CFrameTable"));

    iFrameTable.Close();

    iLowResFrameTable.Close();

    }



// -----------------------------------------------------------------------------

// CFrameTable::InitFrameTable

// -----------------------------------------------------------------------------

//

EXPORT_C TInt CFrameTable::InitFrameTable(TInt aSampleRate, TInt aSamplesPerFrame)

    { // this is only intended to be used at the very beginning of play since it clears the table

    DP2(_L("CFrameTable::InitFrameTable aSampleRate[%d], aSamplesPerFrame[%d]"), aSampleRate, aSamplesPerFrame);

    TInt status = KErrNone;

    ResetTable();

    iSampleRate = aSampleRate;

    iSamplesPerFrame = aSamplesPerFrame;



    if ((iSampleRate> 0) && (iSamplesPerFrame> 0))

        {

        iMsecPerFrame = (iSamplesPerFrame*1000)/iSampleRate;

        iFramesPerLowResInterval = ((iSampleRate*KLowResPeriodMs/1000) / iSamplesPerFrame);

        iLowResIntervalMs = (iSamplesPerFrame*1000*iFramesPerLowResInterval)/iSampleRate;

        }

    else

        {

        status = KErrArgument;

        }



    return status;

    }



//  0   1   2   3   4

// -----------------------------------------------------------------------------

// CFrameTable::SubmitTableEntry

// -----------------------------------------------------------------------------

//

// The table will always have contiguous frames in it.

EXPORT_C TInt CFrameTable::SubmitTableEntry(TUint aPos)

    {

    DP2(_L("CFrameTable::SubmitTableEntry aPos[%u], event pos[%u]"), aPos, iPosEventMs);

    TInt status = KErrNone;

    TBufRefPosEntry lastEntry;

    lastEntry.iPos = 0;

    TBufRefPosEntry bufRefPos;

    TUint frameNum;



    TUint pos = aPos+iOffset; // pos = absolute position (not including any header)



    if (aPos> 0)

        {

        iCurrentFrameCount++; // not a total frame count, but count from last reference set

        DP1(_L("CFrameTable::SubmitTableEntry iCurrentFrameCount[%u]"), iCurrentFrameCount);

        }



    TUint numEntries = iFrameTable.Count(); // this table will stay short - seeking backward resets it



    if (numEntries == 0)

        { // table gets reset if seeking in source

        bufRefPos.iPos = pos;

        bufRefPos.iFrameNum = iFrameNum; // first frame num is 0 so calculations for time 0 would point to frame 0

        status = iFrameTable.Append(bufRefPos);

        ASSERT(status == KErrNone);

        numEntries = 1;

        DP2(_L("CFrameTable::SubmitTableEntry init pos[%u] framenum[%u]"), pos, iFrameNum);

        }

    else

        {

        lastEntry = iFrameTable[numEntries-1];



        if (pos> lastEntry.iPos)

            {

            frameNum = lastEntry.iFrameNum + 1;

            bufRefPos.iPos = pos;

            bufRefPos.iFrameNum = frameNum;



            DP3 (_L ("CFrameTable::SubmitTableEntry, new entry iPos[%u], iFrameNum[%u], cnt[%u]"), bufRefPos.iPos, bufRefPos.iFrameNum, numEntries+1);



            status = iFrameTable.Append(bufRefPos);

            ASSERT(status == KErrNone);

            if (iOkToShrink)

                {

                DP0(_L("CFrameTable::SubmitTableEntry removing an entry "));

                iFrameTable.Remove(0);

                }

            }

        else

            {

            status = KErrArgument;

            }

        }



    if (status == KErrNone)

        {

        SubmitLowResTableEntry(bufRefPos); // do this before events are detected below



        // total average bitrate - not instantaneous bitrate - used for duration calculation

        if (pos> iLastBytePos)

            { // don't recalculate if we rewind

            iLastBytePos = pos; // pos is absolute position - relative to the start of actual data content (not file start - no metadata) as set by the user. User maintains metadata offsets.

            TUint totalTimeMs = bufRefPos.iFrameNum*iMsecPerFrame; // total time in the table

                DP1(_L("CFrameTable::SubmitTableEntry totalTimeMs[%u]"), totalTimeMs);

            if (totalTimeMs> 0)

                {

                // cast calculation to TIn64 or it could lose precision

                TUint64 p = pos;

                p = p*8*1000;

                TUint64 t = totalTimeMs;

                iAvgBitrate = p/t;

                }

            }

        }



    // need to convert current position to time

    TInt currentTimeMs = iCurrentFrameCount*iMsecPerFrame + iTimeRefMs; // current time position being played

    DP3(_L("CFrameTable::SubmitTableEntry current calculated time ms[%u] iCurrentFrameCount[%u] timeref[%u]"), currentTimeMs, iCurrentFrameCount, iTimeRefMs);

    if (iPosEventMs> 0)

        { // used for seeking

        if (currentTimeMs >= iPosEventMs)

            {

            iPosEventMs = 0;

            DP0(_L("CFrameTable::SubmitTableEntry handle event pos reached"));

            iEventPos = pos;

            iObserver->HandleFrameTableEvent(EPosReached);

            }

        }



    if (iPlayWindowEndPosEventMs> 0)

        {

        if (currentTimeMs >= iPlayWindowEndPosEventMs)

            {

            iPlayWindowEndPosEventMs = 0;

            DP0(_L("CFrameTable::SubmitTableEntry handle event play window end pos reached"));

            iObserver->HandleFrameTableEvent(EPlayWindowEndPosReached);

            }

        }



    if (iIntervalEvent> 0)

        {

        if (iIntervalRef == 0)

            {

            iIntervalRef = currentTimeMs;

            }



        if ((currentTimeMs - iIntervalRef) >= iIntervalEvent)

            {

            iIntervalRef = currentTimeMs;

            iObserver->HandleFrameTableEvent(EDecodeInterval);

            }

        }



    DP1 (_L ("CFrameTable::SubmitTableEntry, avgBitRate[%u]"), iAvgBitrate);

    return status;

    }



TInt CFrameTable::SubmitLowResTableEntry(TBufRefPosEntry& aEntry)

    {

    TInt status = KErrNone;

    TBufRefPosEntry lastEntry;



    TUint numEntries = iLowResFrameTable.Count();

    if (numEntries == 0)

        {

        status = iLowResFrameTable.Append(aEntry);

        ASSERT(status == KErrNone);

        return status;

        }



    lastEntry = iLowResFrameTable[numEntries-1];



    ASSERT(aEntry.iFrameNum <= lastEntry.iFrameNum + iFramesPerLowResInterval);

    // note that 0 position table entries can be submitted to the table before it is initialized.

    // If this happens, iFramesPerLowResInterval would still be 0 and the same entry might be appended again

    // so we have a check to make sure that doesn't happen.

    if ((aEntry.iFrameNum == lastEntry.iFrameNum + iFramesPerLowResInterval) && 

        (iFramesPerLowResInterval != 0))

        {

        DP3 (_L ("CFrameTable::SubmitLowResTableEntry, new entry aEntry.iFrameNum[%u], lastEntry.iFrameNum[%u], iFramesPerLowResInterval[%u]"), aEntry.iFrameNum, lastEntry.iFrameNum, iFramesPerLowResInterval);

        DP3 (_L ("CFrameTable::SubmitLowResTableEntry, new entry iPos[%u], iFrameNum[%u], cnt[%u]"), aEntry.iPos, aEntry.iFrameNum, numEntries+1);



        status = iLowResFrameTable.Append(aEntry);

        ASSERT(status == KErrNone);

        }

    else

        {

        status = KErrArgument;

        }



    return status;

    }



// -----------------------------------------------------------------------------

// CFrameTable::FindFramePosFromTime

// -----------------------------------------------------------------------------

//

/*

 Looks in the table for the frame position associated with the given time.

 It will return the position and also the time for that frame.

 If not found, an error will be returned, and the time and position will be

 the closest available in the table.

 */

EXPORT_C TInt CFrameTable::FindFramePosFromTime(TUint& aTimeMs, TUint& aPos)

    {

    DP2(_L("CFrameTable::FindFramePosFromTime time[%u], pos[%u]"), aTimeMs, aPos);

    TInt stat = KErrNone;

    // TUint in msec can represent 1193 hours

    TInt tableIndex;



    // special case for time = 0

    if (aTimeMs == 0)

        {

        aPos = 0;

        DP0(_L("CFrameTable::FindFramePosFromTime time=0 pos=0"));

        return KErrNone;

        }



    TUint countlr = iLowResFrameTable.Count();

    if ((countlr == 0) || (iMsecPerFrame == 0))

        {

        DP2(_L("CFrameTable::FindFramePosFromTime not ready countlr[%u] iMsecPerFrame[%u]"), countlr, iMsecPerFrame);

        return KErrNotReady;

        }

    TUint frame = aTimeMs/iMsecPerFrame; // calc frame number

    DP1(_L("CFrameTable::FindFramePosFromTime looking for frame[%u]"), frame);



    // check hi-res table first

    TUint count = iFrameTable.Count();

    if (count == 0)

        {

        stat = KErrNotFound; // use low-res table

        }

    else

        {

        TUint frame0 = iFrameTable[0].iFrameNum; // first frame available (table can shrink)

        DP1(_L("CFrameTable::FindFramePosFromTime frame0[%u]"), frame0);

        if (frame >= frame0) // frame might be in table



            {

            tableIndex = frame - frame0; // calc index into table

            DP1(_L("CFrameTable::FindFramePosFromTime tableIndex[%u]"), tableIndex);

            if (tableIndex >= count)

                {

                stat = KErrNotFound; // use low-res table

                DP0(_L("CFrameTable::FindFramePosFromTime not found in hi-res"));

                }

            else

                {

                aPos = iFrameTable[tableIndex].iPos;

                iFrameNum = iFrameTable[tableIndex].iFrameNum; // iFrameNum provides frame num after hi-res table is reset

                aTimeMs = iFrameNum * iMsecPerFrame;

                DP4(_L("CFrameTable::FindFramePosFromTime aTimeMs[%u] iFrameNum[%u] iSamplesPerFrame[%u] iSampleRate[%u]"),

                        aTimeMs,iFrameNum,iSamplesPerFrame,iSampleRate);

                }

            }

        else

            {

            stat = KErrNotFound; // use low-res table

            }

        }

    if (stat == KErrNotFound)

        { // check low-res table

        DP0(_L("CFrameTable::FindFramePosFromTime looking in low-res table"));

        if (iFramesPerLowResInterval> 0)

    		{  // first frame is always 0 in low res table - don't need to subtract frame[0]

            tableIndex = frame/iFramesPerLowResInterval; // calc index into table

            DP3(_L("CFrameTable::FindFramePosFromTime tableIndex[%d] frame[%u] iFramesPerLowResInterval[%u]"),

                    tableIndex,frame,iFramesPerLowResInterval);

            if (tableIndex < countlr)

                {

                aPos = iLowResFrameTable[tableIndex].iPos;

                iFrameNum = iLowResFrameTable[tableIndex].iFrameNum; // iFrameNum provides frame num after hi-res table is reset

                aTimeMs = iFrameNum * iMsecPerFrame;

                stat = KErrNone;

                }

            }

        }



    if (stat == KErrNotFound)

        { // use last in low-res table

        aPos = iLowResFrameTable[countlr-1].iPos;

        iFrameNum = iLowResFrameTable[countlr-1].iFrameNum; // iFrameNum provides frame num after hi-res table is reset

        aTimeMs = iFrameNum * iMsecPerFrame;

        }

    DP4(_L("CFrameTable::FindFramePosFromTime time[%u], pos[%u], stat[%d] iFrameNum sync[%u]"), aTimeMs, aPos, stat, iFrameNum);

    return stat;

    }



EXPORT_C TInt CFrameTable::FindFrameTimeFromPos(TUint& aTimeMs, TUint& aPos)

    {

    DP1(_L("CFrameTable::FindFrameTimeFromPos pos[%u]"), aPos);

    TInt stat = KErrNotFound;

    //	TBufRefPosEntry temp; // used for array search

    //	temp.iFrameNum = 0;

    //	temp.iPos = aPos;



    //	stat = iFrameTable.SpecificFindInUnsignedKeyOrder(temp, EArrayFindMode_First);

    TUint count = iFrameTable.Count();

    for (TUint i=0; i<count; i++)

        {

        if (iFrameTable[i].iPos >= aPos)

            {

            stat = i;

            DP4(_L("CFrameTable::FindFrameTimeFromPos hi-res first[%u] last[%u] count[%u] stat[%d]"),

                    iFrameTable[0].iPos, iFrameTable[count-1].iPos, count, stat);

            break;

            }

        }

#ifdef _DEBUG 

    if(count == 0)

        {

        DP0(_L("CFrameTable::FindFrameTimeFromPos hi-res Frame Table: Empty"));

        }

#endif



    if (stat == KErrNotFound)

        {

        //		stat = iLowResFrameTable.SpecificFindInUnsignedKeyOrder(temp, EArrayFindMode_First);

        count = iLowResFrameTable.Count();

        for (TUint i=0; i<count; i++)

            {

            if (iLowResFrameTable[i].iPos >= aPos)

                {

                stat = i;

                DP4(_L("CFrameTable::FindFrameTimeFromPos lo-res first[%u] last[%u] count[%u] stat[%d]"),

                        iLowResFrameTable[0].iPos, iLowResFrameTable[count-1].iPos, count, stat);

                break;

                }

            }

#ifdef _DEBUG 

        if(count == 0)

            {

            DP0(_L("CFrameTable::FindFrameTimeFromPos lo-res Frame Table: Empty"));

            }

#endif		



        if (stat != KErrNotFound)

            {

            aPos = iLowResFrameTable[stat].iPos;

            aTimeMs = iLowResFrameTable[stat].iFrameNum * iMsecPerFrame;

            iFrameNum = iLowResFrameTable[stat].iFrameNum; // iFrameNum provides frame num after hi-res table is reset

            DP3(_L("CFrameTable::FindFrameTimeFromPos lo-res iFrameNum[%u] indx[%d] iFrameNum sync[%u]"), iLowResFrameTable[stat].iFrameNum, stat, iFrameNum);

            }

        }

    else

        {

        aPos = iFrameTable[stat].iPos;

        aTimeMs = iFrameTable[stat].iFrameNum * iMsecPerFrame;

        iFrameNum = iFrameTable[stat].iFrameNum; // iFrameNum provides frame num after hi-res table is reset

        DP3(_L("CFrameTable::FindFrameTimeFromPos hi-res iFrameNum[%u] indx[%d] iFrameNum sync[%u]"), iFrameTable[stat].iFrameNum, stat, iFrameNum);

        }



    DP3(_L("CFrameTable::FindFrameTimeFromPos time[%u] pos[%u] indxstat[%d]"), aTimeMs, aPos, stat);

    if (stat> 0)

        {

        stat = KErrNone;

        }

    return stat;

    }



// -----------------------------------------------------------------------------

// CFrameTable::LastFramePos

// -----------------------------------------------------------------------------

//

EXPORT_C TInt CFrameTable::LastFramePos(TUint& aPos)

    {

    TInt stat = KErrNotFound;

    TInt stat1 = KErrNotFound;

    TInt stat2 = KErrNotFound;

    TUint count;

    TInt pos1 = 0;

    TInt pos2 = 0;



    count = iFrameTable.Count();

    DP1(_L("CFrameTable::LastFramePos count[%u]"), count);

    if (count> 0)

        {

        pos1 = iFrameTable[count-1].iPos;

        stat1 = KErrNone;

        DP1(_L("CFrameTable::LastFramePos last pos in hi-res[%d]"), pos1);

        }



    count = iLowResFrameTable.Count();

    DP1(_L("CFrameTable::LastFramePos lr count[%u]"), count);

    if (count> 0)

        {

        pos2 = iLowResFrameTable[count-1].iPos;

        stat2 = KErrNone;

        DP1(_L("CFrameTable::LastFramePos last pos in low-res[%d]"), pos2);

        }



    if ((stat1 == KErrNone) && (stat2 == KErrNone))

        {

        aPos = Max(pos1, pos2);

        stat = KErrNone;

        }

    else if (stat1 == KErrNone)

        {

        aPos = pos1;

        stat = KErrNone;

        }

    else if (stat2 == KErrNone)

        {

        aPos = pos2;

        stat = KErrNone;

        }

    DP1(_L("CFrameTable::LastFramePos aPos[%d]"), aPos);



    return stat;

    }



// -----------------------------------------------------------------------------

// CFrameTable::LastFrameTime

// -----------------------------------------------------------------------------

//

EXPORT_C TInt CFrameTable::LastFrameTime(TUint& aTimeMs)

    {

    DP0(_L("CFrameTable::LastFrameTime"));

    TInt stat = KErrNotFound;

    TInt stat1 = KErrNotFound;

    TInt stat2 = KErrNotFound;

    TUint count;

    TUint frameNum = 0;

    TInt frameNum1 = 0;

    TInt frameNum2 = 0;



    count = iFrameTable.Count();

    if (count> 0)

        {

        frameNum1 = iFrameTable[count-1].iFrameNum;

        stat1 = KErrNone;

        }



    count = iLowResFrameTable.Count();

    if (count> 0)

        {

        frameNum2 = iLowResFrameTable[count-1].iFrameNum;

        stat2 = KErrNone;

        }



    if ((stat1 == KErrNone) && (stat2 == KErrNone))

        {

        frameNum = Max(frameNum1, frameNum2);

        stat = KErrNone;

        }

    else if (stat1 == KErrNone)

        {

        frameNum = frameNum1;

        stat = KErrNone;

        }

    else if (stat2 == KErrNone)

        {

        frameNum = frameNum2;

        stat = KErrNone;

        }



    aTimeMs = frameNum * iMsecPerFrame;

    DP2(_L("CFrameTable::LastFrameTime time[%u] frame[%u]"),aTimeMs,frameNum);



    return stat;

    }



// -----------------------------------------------------------------------------

// CFrameTable::ResetTable

// -----------------------------------------------------------------------------

//	

EXPORT_C void CFrameTable::ResetTable()

    {

    DP0(_L("CFrameTable::ResetTable"));



    iFrameTable.Reset();

    iBufCnt = 0;

    iOkToShrink = EFalse;

    }



// -----------------------------------------------------------------------------

// CFrameTable::ShrinkTable

// -----------------------------------------------------------------------------

//	

EXPORT_C void CFrameTable::ShrinkTable()

    { // gets called from controller decoder when switching to next buffer

    DP0(_L("CFrameTable::ShrinkTable"));

    if (iBufCnt < KBuffersToKeep)

        {

        iBufCnt++;

        }

    else

        {

        iOkToShrink = ETrue;

        DP0(_L("CFrameTable::ShrinkTable ok to shrink"));

        }

    }



EXPORT_C TInt CFrameTable::SetSourceReference(TUint aTimeMs, TUint aPos)

    { // if position is unknown it should always be set to 0 as the reference offset and time will be used instead

    // This is used to set the offset which is added to future frame positions submitted

    // This frame table can get reset when seeking and needs to know where it is in the source

    DP2(_L("CFrameTable::SetOffset, aTimeMs[%u], aPos[%u]"), aTimeMs, aPos);

    TInt stat = KErrNone;

    iTimeRefMs = aTimeMs;

    iCurrentFrameCount = 0;

    iIntervalRef = 0;

    iOffset = aPos;

    if (aTimeMs == 0)

        {

        iFrameNum = 0;

        }

    if (iFrameTable.Count() == 0)

        { // this is to make sure the table has at least one entry in case playback looks for a position in the buffers after seeking

        SubmitTableEntry(0);

        //		iCurrentFrameCount--; // since this is not a decoded frame

        }

    return stat;

    }



// -----------------------------------------------------------------------------

// CFrameTable::RegisterForEvent

// -----------------------------------------------------------------------------

//

EXPORT_C TInt CFrameTable::RegisterForEvent(TFrameTableEvent aEvent,

        MFrameTableEventObserver* aObserver,

        TUint aParam)

    {

    DP2(_L("CFrameTable::RegisterForEvent event[%d] param[%u]"), aEvent, aParam);



    TInt status = KErrNone;

    iObserver = aObserver;



    switch (aEvent)

        {

        case EPosReached:

        iPosEventMs = aParam;

        break;



        case EPlayWindowEndPosReached:

        iPlayWindowEndPosEventMs = aParam;

        break;



        case EDecodeInterval:



        if (aParam> 0) // msec



            {

            iIntervalRef = 0;

            iIntervalEvent = aParam;

            }

        else

        return KErrArgument;



        break;



        default:

        status = KErrUnknown;

        break;

        }



    return status;

    }



// -----------------------------------------------------------------------------

// CFrameTable::UnRegisterForEvent

// -----------------------------------------------------------------------------

//

EXPORT_C TInt CFrameTable::UnRegisterForEvent(TFrameTableEvent aEvent, MFrameTableEventObserver* /*aObserver*/)

    {

    DP1(_L("CFrameTable::UnRegisterForEvent event[%d]"), aEvent);



    TInt status = KErrNone;



    switch (aEvent)

        {

        case EPosReached:

        iPosEventMs = 0;

        break;



        case EPlayWindowEndPosReached:

        iPlayWindowEndPosEventMs = 0;

        break;



        case EDecodeInterval:

        iIntervalEvent = 0;

        break;



        default:

        status = KErrUnknown;

        break;

        }



    return status;

    }



// -----------------------------------------------------------------------------

// CFrameTable::Bitrate

// -----------------------------------------------------------------------------

//	

EXPORT_C TInt CFrameTable::Bitrate()

    {

    return iAvgBitrate;

    }



// ========================== OTHER EXPORTED FUNCTIONS =========================



// -----------------------------------------------------------------------------

// CFrameTable::CalcFrameFromTimeMs

// -----------------------------------------------------------------------------

//

TUint CFrameTable::CalcFrameFromTimeMs(TUint aTimeMs)

    { // TUint in msec can represent 1193 hours

    TUint frame = 0;



    if (iMsecPerFrame > 0)

        {

        frame = aTimeMs/iMsecPerFrame;

        }



    return frame;

    }



EXPORT_C TInt CFrameTable::IsSeeking(TBool& aIsSeeking)

    {

    aIsSeeking = (iPosEventMs> 0);

    return KErrNone;

    }



EXPORT_C TInt CFrameTable::GetLastPosEvent(TUint& aPos)

    {

    aPos = iEventPos;

    return KErrNone;

    }



// End of File



