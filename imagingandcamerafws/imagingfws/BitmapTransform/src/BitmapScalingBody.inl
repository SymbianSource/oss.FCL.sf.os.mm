// Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// This is the implementation of the BitmapTransforms Library
// 
//

#ifndef __BitmapScalingBody_inl
#define __BitmapScalingBody_inl

#include "BitmapTransformsMain.h"

/**
 *
 * Begins the bitmap re-scaling operation.
 *
 * The scaling factor is based on the relative value of the source bitmap
 * size and the explicitly supplied size. The operation is asynchronous.
 * When it is complete, successfully or otherwise, the
 * <code>TRequestStatus</code>& is set, passing the state of the operation.
 *
 * @param     "TRequestStatus* aRequestStatus"
 *
 * @param     "CFbsBitmap& aBitmap"
 *            The bitmap to be re-scaled. This reference is also the
 *            target location for the re-scaled bitmap.
 * @param     "const TSize& aDestinationSize"
 *            The requested target size for the re-scaled bitmap.
 * @param     "TBool aMaintainAspectRatio = ETrue"
 *            <code>ETrue</code> - the aspect ratio is retained;
 *            this is the default. The same scaling factor is
 *            applied in both the horizontal and vertical
 *            directions. This is the smaller of the horizontal
 *            scaling factor and the vertical scaling factor.
 *            <code>EFalse</code> - the aspect ratio need not be
 *            retained.
 *
 */
inline void CBitmapScalerBody::Scale(TRequestStatus* aRequestStatus,
									 CFbsBitmap& aBitmap,
									 const TSize& aDestinationSize,
									 TBool aMaintainAspectRatio)
	{
	iPlugin->Scale(aRequestStatus, aBitmap, aDestinationSize, aMaintainAspectRatio);
	}

inline void CBitmapScalerBody::Cancel()
	{
	iPlugin->Cancel();
	}


#endif // __BitmapScalingBody_inl
