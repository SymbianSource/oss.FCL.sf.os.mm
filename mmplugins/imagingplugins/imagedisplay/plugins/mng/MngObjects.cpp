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
#include "MngPlayer.h"
#include "MngPanic.h"
#include "MngRenderer.h"

#include "MngObjects.h"

/*virtual*/
void CMngObject::Reset()
	{
	}

/*virual*/
TBool CMngObject::IsSubFrame() const
	{
	return EFalse;
	}

/*virual*/
TBool CMngObject::IsLayer() const
	{
	return EFalse;
	}

/*virual*/
void CMngObject::OnAddToPlayerL(const CMngPlayer& /*aPlayer*/)
	{
	}

/*virual*/
TBool CMngLayer::IsLayer() const
	{
	return ETrue;
	}

/*static*/
CMngTermination* CMngTermination::NewL(TTermChunk*& aTermChunk, TBool aMngSubframesNoLoops)  //Subframes with No Loops
	{
	TTermChunk* pChunk=aTermChunk;
	CleanupDeletePushL(pChunk);
	aTermChunk = NULL;
	CMngTermination* self=new (ELeave) CMngTermination(*pChunk, aMngSubframesNoLoops); //Subframes with No Loops
	CleanupStack::PopAndDestroy(pChunk);
	return self;
	}

void CMngTermination::ExecuteL(CMngPlayer& aPlayer, TMngExecuteResult& aResult)
	{
	TMngTerminationAction Action=iPostIterationAction;
	
	//Subframes with No Loops
	if(iMngSubframesNoLoops)
		{
		iIterationCounter = 1;
		Action =EMngShowLastFrameIndef;	
		}

	ASSERT(iIterationCounter > -1);
	iIterationCounter -= (iIterationCounter>0 && iIterationCounter!=KMngInfinity);
	aResult.iDelayInTicks = iDelay;
	if (iIterationCounter)
		{
		Action=iTerminationAction;
		}
	switch (Action)
		{
		case EMngShowLastFrameIndef: 
			aResult.iCommand=ECmdTerminate;
			break;
		case EMngCeaseDisplAnything: 
			aResult.iCommand=ECmdTerminateClearOutput;
			break;
		case EMngShowFirstFrmAfterTerm:
			aResult.iCommand=ECmdTerminateAfter1stFrame;
			aPlayer.SetTerminationFrameNum(1);
			break;
		case EMngRepeatImmidiately:
			aResult.iCommand=ECmdStartFromBeginning;
			break;
		default:
			User::Leave(KErrCorrupt);
		}
	}

void CMngTermination::Reset()
	{
	iIterationCounter = iIterationMax;
	}

/*static*/
CMngObject* CMngKeyFrame::NewL(TMngChunk*& aFramChunk)
	{
	CleanupDeletePushL(aFramChunk);
	TFramChunk* FrameChunk=static_cast<TFramChunk*>(aFramChunk);
	aFramChunk=NULL;
	CMngKeyFrame* self=new (ELeave) CMngKeyFrame(FrameChunk);
	CleanupStack::Pop();
	return self;
	}

inline
CMngKeyFrame::CMngKeyFrame(TFramChunk* aFramChunk):iFramChunk(aFramChunk)
	{
	}

CMngKeyFrame::~CMngKeyFrame()
	{
	delete iFramChunk;
	}

void CMngKeyFrame::ExecuteL(CMngPlayer& aPlayer, TMngExecuteResult& /*aResult*/)
	{
	aPlayer.SetFramingMode(iFramChunk->iFramingMode);
	if (EMngFramDoNotChange != iFramChunk->iChangeInterFrmDelay)
		{
		aPlayer.SetInterFrameDelay(iFramChunk->iChangeInterFrmDelay, iFramChunk->iInterframeDelay);
		}

	if (EMngFramDoNotChange  != iFramChunk->iChangeClippingBoundaries)
		{
		aPlayer.SetLayerClipping(iFramChunk->iChangeClippingBoundaries, 
									TRect(iFramChunk->iLeftLayerClippingB, iFramChunk->iTopLayerClippingB,
										  iFramChunk->iRightLayerClippingB, iFramChunk->iBottomLayerClippingB),
									iFramChunk->iAddLayerClipping);
		}
	}

TBool CMngKeyFrame::IsSubFrame() const
	{
	return ETrue;
	}

/*static*/
CMngObject* CMngBackground::NewL(TMngChunk*& aBackChunk)
	{
	TBackChunk* BackChunk=static_cast<TBackChunk*>(aBackChunk);
	aBackChunk = NULL;
	CleanupDeletePushL(BackChunk);
	CMngBackground* self=new (ELeave) CMngBackground(*BackChunk);
	CleanupStack::PopAndDestroy(BackChunk);
	return self;
	}

void CMngBackground::ExecuteL(CMngPlayer& aPlayer, TMngExecuteResult& /*aResult*/)
	{
	aPlayer.Renderer()->SetBackground(iBackgroundColour, (iBgMandatory&EMngTermBgColorMandatoryImgAdvisory) );
	}

/*static*/
CMngObject* CMngRawChunkHolder::NewL(TMngChunk*& aChunk)
	{
	TMngRawChunk* RawChunk=static_cast<TMngRawChunk*>(aChunk);
	aChunk = NULL;
	CleanupDeletePushL(RawChunk);
	CMngRawChunkHolder* self=new (ELeave) CMngRawChunkHolder(RawChunk);
	CleanupStack::Pop(RawChunk);
	return self;
	}

void CMngRawChunkHolder::ExecuteL(CMngPlayer& aPlayer, TMngExecuteResult& /*aResult*/)
	{
	aPlayer.AddUniqueChunkProviderL(iChunk->iHeader.iChunkId, *this);
	}

TInt CMngRawChunkHolder::GetChunkData(const TText8* aChunkId, const TUint8*& aDataPtr, TInt& aSizeInBites)
	{
	TInt res=KErrNone;
	if (iChunk->iHeader.iChunkId == TPtrC8(aChunkId, sizeof(TChunkId)) )
		{
		aDataPtr = iChunk->Data();
		aSizeInBites = iChunk->iHeader.iLength;
		}
	else
		{
		res = KErrNotFound;
		aSizeInBites = 0;
		aDataPtr = NULL;
		}
	return res;
	}

CMngRawChunkHolder::~CMngRawChunkHolder()
	{
	delete iChunk;
	}

/*static*/
CMngObject* CMngImageDefinition::NewL(TMngChunk*& aDefiChunk)
	{
	TDefiChunk* DefiChunk=static_cast<TDefiChunk*>(aDefiChunk);
	aDefiChunk = NULL;
	CleanupDeletePushL(DefiChunk);
	CMngImageDefinition* self = new (ELeave) CMngImageDefinition(*DefiChunk);
	CleanupStack::PopAndDestroy(DefiChunk);
	return self;
	}

void CMngImageDefinition::ExecuteL(CMngPlayer& aPlayer, TMngExecuteResult& /*aResult*/)
	{
	CMngRenderer* Renderer = aPlayer.Renderer();
	Renderer->SetOutputEnabled(!iDoNotShow);
	Renderer->SetImageOrigin(iOrigin);
	Renderer->SetImageClipRect(iClippingRect);
	}

void CMngImageDefinition::OnAddToPlayerL(const CMngPlayer& aPlayer)
	{
	if (iClippingRect.iBr.iX==-1 && iClippingRect.iTl.iX==0)
		{
		iClippingRect.iBr.iX= 0;
		iClippingRect.iBr	= aPlayer.FrameSize().AsPoint();
		}
	}

/*static*/
CMngObject* CMngLoopEntry::NewL(TMngChunk*& aLoopChunk)
	{
	TLoopChunk* LoopChunk=static_cast<TLoopChunk*>(aLoopChunk);
	aLoopChunk = NULL;
	CleanupDeletePushL(LoopChunk);
	CMngLoopEntry* self = new (ELeave) CMngLoopEntry(*LoopChunk);
	CleanupStack::PopAndDestroy(LoopChunk);
	return self;
	}

void CMngLoopEntry::ExecuteL(CMngPlayer& aPlayer, TMngExecuteResult& /*aResult*/)
	{
	ASSERT(iIterationsLeft>-1);
	TLoopStackItem item;
	item.iLoopObjectLabel		= ObjectLabel();
	item.iNestLevel				= iNestLevel;
	if (iIterationsLeft > 0)
		{
		--iIterationsLeft;
		}
	if (iIterationsLeft > 0)
		{
		item.iMoreIterationsNeeded = ETrue;
		}
	else
		{
		iIterationsLeft = iIterationCount;
		item.iMoreIterationsNeeded = EFalse;
		}
	aPlayer.LoopStack().PushL(item);
	}

void CMngLoopEntry::Reset()
	{
	iIterationsLeft = iIterationCount;
	}

/*static*/
CMngObject* CMngLoopEnd::NewL(TMngChunk*& aEndlChunk)
	{
	TEndlChunk* EndlChunk=static_cast<TEndlChunk*>(aEndlChunk);
	aEndlChunk = NULL;
	CleanupDeletePushL(EndlChunk);
	CMngLoopEnd* self = new (ELeave) CMngLoopEnd(*EndlChunk);
	CleanupStack::PopAndDestroy(EndlChunk);
	return self;
	}

void CMngLoopEnd::ExecuteL(CMngPlayer& aPlayer, TMngExecuteResult& aResult)
	{
	TLoopStackItem item;
	if (KErrNone != aPlayer.LoopStack().Pop(item))
		{
		User::Leave(KErrCorrupt);
		}
	if (item.iNestLevel != iNestLevel)
		{
		User::Leave(KErrCorrupt);
		}
	if (item.iMoreIterationsNeeded)
		{
		aResult.iCommand	= ECmdJumpToObject;
		aResult.iObjectLabel= item.iLoopObjectLabel;
		}
	}
