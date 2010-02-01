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

#include "squeezetransformextension.h"

/**
This function sets the desired maximum destination data size in bytes.
@param  "aMaxDestDataSize"		
		The desired max destination data size in bytes
*/
EXPORT_C void CSqueezeTransformExtension::SetDestSizeInBytes(TUint aMaxDestDataSize ) 
	{
	DoSetDestSizeInBytes(aMaxDestDataSize);
	}
/**
This function sets the advanced squeeze mode params
@param  "aSqueezeAutoResizeParams"		
		The params for advaced auto resize
*/
EXPORT_C void CSqueezeTransformExtension::SetAdvancedSqueezeModeL(TAdvancedSqueezeParams* aSqueezeAutoResizeParams )
	{
	DoSetAdvancedSqueezeModeL(aSqueezeAutoResizeParams);
	}

/**
Reserved for future-proofing
*/
EXPORT_C void CSqueezeTransformExtension::ReservedVirtual5() 
	{
	}

/** 
Constructor: Set the default values for some squeeze params	
*/
EXPORT_C TAdvancedSqueezeParams::TAdvancedSqueezeParams()
	{
	iResizeAction = EAutoResizeActionMiddleCourse;
	iMinEncodingQuality = 0.6f;
	iSamplingUid = KUidSamplingColor420;
	}

