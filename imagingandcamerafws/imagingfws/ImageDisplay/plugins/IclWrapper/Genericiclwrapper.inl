// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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

inline 
void CGenericImageDisplayPlugin::SetStatusFlag(TUint aFlagOption)
	{
	iPluginStatus |= aFlagOption;
	}

inline 
void CGenericImageDisplayPlugin::ClearStatusFlag(TUint aFlagOption)
	{
	iPluginStatus &= ~TUint(aFlagOption);
	}

inline 
TBool CGenericImageDisplayPlugin::IsStatusSet(TUint aFlagOption) const
	{
	return (iPluginStatus & aFlagOption)==aFlagOption;
	}

inline 
const TSize& CGenericImageDisplayPlugin::MaxUnscaledSize() const
	{
	return iMaxUnscaledSize;
	}

inline 
TDisplayMode CGenericImageDisplayPlugin::EffectiveDispMode() const
	{
	ASSERT(iDecoder);
	return DisplayMode() == ENone ? iDecoder->FrameInfo().iFrameDisplayMode : DisplayMode();
	}


inline 
TBool CGenericImageDisplayPlugin::ThumbnailRequested() const
	{
	return iImageIsThumbnail;
	}

inline
CDelayedCallback::CDelayedCallback(MCallbackClient& aClient):
										CTimer(EPriorityNormal),
										iClient(aClient)
	{
	CActiveScheduler::Add(this);
	}

inline
TExtTie::TExtTie(CGenericImageDisplayPlugin& aImplementor):iImplementor(aImplementor)
	{
	}

