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

/**
 @file
 @internalTechnology
 @prototype
*/

#ifndef ILIFBASE_INL
#define ILIFBASE_INL

inline 
TILEvent::TILEvent()
	:
	iEvent(EEventMax),
	iData1(0),
	iData2(0),
	iExtraData(0)
	{
	}

inline
TILEvent::TILEvent(
	TILEventTypes aEvent,
	TUint32 aData1,
	TUint32 aData2,
	TAny* aExtraData
	)
	:
	iEvent(aEvent),
	iData1(aData1),
	iData2(aData2),
	iExtraData(aExtraData)
	{
	}

inline 
TILCommand::TILCommand()
	:
	iCmd(ECommandInvalid),
	iData1(0),
	iExtraData(0)
	{
	}

inline
TILCommand::TILCommand(
	TILCommandTypes aCmd,
	TUint32 aData1,
	TAny* aExtraData
	)
	:
	iCmd(aCmd),
	iData1(aData1),
	iExtraData(aExtraData)
	{
	}	
		
inline TILVersion::TILVersion()
	: 
	iMajor(0), 
	iMinor(0), 
	iRev(0), 
	iStep(0)	
	{
	}

inline TILVersion::TILVersion(
	TUint8 aMajor, 
	TUint8 aMinor, 
	TUint8 aRev, 
	TUint8 aStep
	) 
	: 
	iMajor(aMajor), 
	iMinor(aMinor), 
	iRev(aRev), 
	iStep(aStep)	
	{
	}

inline TBool TILVersion::operator==(TILVersion aVersion)
	{
	return (!(operator!=(aVersion)));
	}
	
inline TBool TILVersion::operator!=(TILVersion aVersion)
	{
	return ((aVersion.iMajor != iMajor) || (aVersion.iMinor != iMinor) || (aVersion.iRev != iRev) || (aVersion.iStep != iStep));
	}

inline TILComponentVersion::TILComponentVersion(
	TPtr8& aPtr
	) 
	: 
	iComponentName(aPtr),
	iComponentVersion(), 
	iSpecVersion(), 
	iComponentUniqueID(0) 
	{
	}


#endif  // ILIFBASE_INL
