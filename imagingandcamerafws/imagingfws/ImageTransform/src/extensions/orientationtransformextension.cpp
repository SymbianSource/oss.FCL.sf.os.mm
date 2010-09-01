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

#include "orientationtransformextension.h"

/**
This function sets the orientation of the image
@param  "aOrientation"		
		The orientation of the image
*/
EXPORT_C void COrientationTransformExtension::SetOrientationL(TOrientation aOrientation ) 
	{
	DoSetOrientationL(aOrientation);
	}

/**
Reserved for future-proofing
*/
EXPORT_C void COrientationTransformExtension::ReservedVirtual5() 
	{
	}

