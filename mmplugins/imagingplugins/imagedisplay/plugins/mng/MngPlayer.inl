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

/** 	@file
	@internalTechnology */
inline 
CDelayTimer::CDelayTimer(CMngPlayer& aPlayer):CTimer(EPriorityNormal),iPlayer(aPlayer)
		{
		CActiveScheduler::Add(this);
		}

inline
TTimeIntervalMicroSeconds32 CMngPlayer::Ticks2TimeInterval(TInt aTicks) const
	{
	return TTimeIntervalMicroSeconds32(aTicks * iTickLength);
	}

inline 
void CMngPlayer::SetInternalDelayOn(TBool aIsOn)
	{
	iPerformFrameDelayInternally = aIsOn;
	}

inline 
TTimeIntervalMicroSeconds32 CMngPlayer::ThisFrameDelay() const
	{
	return iNominalDelayBeforeThisFrame;
	}

inline 
TInt CMngPlayer::NumFrames() const
	{
	ASSERT(iMngHeader);
	return iMngHeader->iNominalFrameCount;
	}

inline
CMngRenderer* CMngPlayer::Renderer() const
	{
	return iMngRenderer;
	}

inline
TSize CMngPlayer::FrameSize() const
	{
	ASSERT(iMngHeader);
	return TSize(iMngHeader->iFrameWidth, iMngHeader->iFrameHeight);
	}

inline
void CMngPlayer::SetTerminationFrameNum(TInt aFrameNumber)
	{
	iFrameNumberToTerminate = aFrameNumber;
	}

inline 
void CMngPlayer::SetFramingMode(TMngFramingMode	aFramingMode)
	{
	if (aFramingMode != EMngFramDoNotChangeMode)
		{
		iFramingMode = aFramingMode;
		}
	}

inline
TBool CMngPlayer::IsAlphaNeeded() const
	{
	ASSERT(iMngHeader);
	return iMngHeader->iProfileValidity==0 || (iMngHeader->iInternalTransparencyPresence || 
			(iMngHeader->iBkgAndSemiTransparencyValidity && 
				(iMngHeader->iSemiTransparencyPresence || iMngHeader->iBkgTransparencyPresence)
			));
	}

inline
void CMngPlayer::SetRequiredDisplayModes(TDisplayMode aDestDisplayMode, TDisplayMode aMaskDisplayMode)
	{
	iDestDisplayMode = aDestDisplayMode;
	iMaskDisplayMode = aMaskDisplayMode;
	}

inline
void CMngPlayer::SetDestinationBitmap(CFbsBitmap* aDestination)
	{
	iCurrentFrame = aDestination;
	}

inline
CFbsBitmap* CMngPlayer::GetDestinationBitmap()
	{
	return iCurrentFrame;
	}

inline
RMngObjectList::TObjectIterator RMngObjectList::End() const
	{
	return Count();
	}

inline
RMngObjectList::TObjectIterator RMngObjectList::Begin() const
	{
	return TObjectIterator(0);
	}

inline
RLoopStack& CMngPlayer::LoopStack()
	{
	return iLoopStack;
	}

inline
void RLoopStack::PushL(const TLoopStackItem& aItem)
	{
	User::LeaveIfError( Append(aItem) );
	}

inline 
TInt RLoopStack::Pop(TLoopStackItem& aItem)
	{
	TInt idx=Count()-1;
	if (idx < 0 )
		{
		return KErrNotFound;
		}
	aItem = (*this)[idx];
	Remove(idx);
	return KErrNone;
	}

inline 
TBool RLoopStack::IsEmpty() const
	{
	return (Count() == 0);
	}

inline
TProviderEntry::TProviderEntry():iProvider(NULL)
		{
		}

inline
TProviderEntry::TProviderEntry(const TChunkId& aId, MUniqueChunkDataProvider& aProvider):iId(aId), 
																							iProvider(&aProvider)
	{
	}
