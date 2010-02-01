// Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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
 
 The function copies pixels 
 
 @param "aTo" 
 pointer to destination
 @param "aFrom" 
 pointer to source
 
*/
inline void CBitmapScalerPlugin::CopyPixel(TUint8* aTo, const TUint8* aFrom) const
	{
	switch (iComponentsPerPixel)
		{
		case 4:
			*reinterpret_cast<TUint32*>(aTo) = *reinterpret_cast<const TUint32*>(aFrom);
			break;
		case 3:
			aTo[0] = aFrom[0];
			aTo[1] = aFrom[1];
			aTo[2] = aFrom[2];
			break;
		case 1:
			aTo[0] = aFrom[0];
			break;
		default:
			ASSERT(EFalse);
		}
	}
