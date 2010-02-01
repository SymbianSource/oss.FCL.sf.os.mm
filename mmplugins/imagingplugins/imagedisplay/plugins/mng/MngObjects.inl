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
TMngExecuteResult::TMngExecuteResult():	iCommand(ECmdProcessNext),
										iDelayInTicks(0)
	{
	}

inline
TMngObjLabel CMngObject::ObjectLabel() const
	{
	return iObjectLabel;
	}

inline
void CMngObject::SetObjectLabel(const TMngObjLabel& aLabel)
	{
	iObjectLabel = aLabel;
	}

inline
CMngTermination::CMngTermination(const TTermChunk& aTerm, TBool aMngSubframesNoLoops):  //Subframes with No Loops
									iMngSubframesNoLoops(aMngSubframesNoLoops),  //Subframes with No Loops
									iTerminationAction(aTerm.iTerminationAction),
									iPostIterationAction(aTerm.iPostIterationAction),
									iDelay(aTerm.iDelay),
									iIterationMax(aTerm.iIterationMax),
									iIterationCounter(aTerm.iIterationMax)
	{
	}

inline
CMngBackground::CMngBackground(const TBackChunk& aBackChunk):iBgMandatory(aBackChunk.iBgMandatory)
	{
	iBackgroundColour.SetRed(aBackChunk.iRedBackground);
	iBackgroundColour.SetGreen(aBackChunk.iGreenBackground);
	iBackgroundColour.SetBlue(aBackChunk.iBlueBackground);
	}

inline
CMngRawChunkHolder::CMngRawChunkHolder(TMngRawChunk* aChunk):iChunk(aChunk)
	{
	}

inline
CMngImageDefinition::CMngImageDefinition(const TDefiChunk& aDefiChunk):iDoNotShow(aDefiChunk.iDoNotShow),
												iOrigin(TPoint(aDefiChunk.iXLocation, aDefiChunk.iYLocation)),
												iClippingRect(TRect(aDefiChunk.iLeftClippingBoundary,
																	aDefiChunk.iTopClippingBoundary,
																	aDefiChunk.iRightClippingBoundary,
																	aDefiChunk.iBottomClippingBoundary)
															  )
	{
	}

inline
CMngLoopEntry::CMngLoopEntry(const TLoopChunk& aLoopChunk):
													iNestLevel(aLoopChunk.iNestLevel),
													iIterationCount(aLoopChunk.iIterationCount),
													iIterationsLeft(aLoopChunk.iIterationCount)
	{
	}

inline
CMngLoopEnd::CMngLoopEnd(const TEndlChunk& aLoopChunk):iNestLevel(aLoopChunk.iNestLevel)
	{
	}
