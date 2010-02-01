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

#include "icl/ExifJpegTransformExtension.h"
#include "ExifJpegTransform.h"
#include <icl/icl_uids.hrh>
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include <icl/icl_uids_const.hrh>
#include <icl/icl_uids_def.hrh>
#endif


const TUid KExifJpegTransformExtension = { KUidTransformJpegExtension };

const TUid CJpegTransformExtension::Uid() const
	{
	return KExifJpegTransformExtension;
	}

MExifMetadata* CJpegTransformExtension::ExifMetadata()
	{
	return iOwner->ExifMetadata();	
	}

CJpegTransformExtension* CJpegTransformExtension::NewL(const CExifJpegTransform* aOwner)
	{
	return new (ELeave) CJpegTransformExtension(aOwner);
	}
	
CJpegTransformExtension::CJpegTransformExtension(const CExifJpegTransform* aOwner)
	: iOwner(aOwner)
	{
	
	}

