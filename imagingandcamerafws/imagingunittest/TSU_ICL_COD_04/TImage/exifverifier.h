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

#ifndef EXIFVERIFIER_H
#define EXIFVERIFIER_H

#include <iclexif.h>
#include "exifreaderwriter.h"

class CExifVerifier : public CBase
	{
public:
	enum TFileType 
		{
		EJpegFile			
		};
		
	enum TVerifyResult 
		{
		EOk,
		EOffsetOutOfRange,
		EOffsetNotWordAligned,
		EExifBlockNotPresent
		};
public:
	~CExifVerifier();
	static CExifVerifier* NewL(const TDesC& aFileName, TFileType aFileType);
	
	TVerifyResult VerifyIfdOffsets();
	MExifMetadata* ExifMetadata();
		
private: // functions
	void ConstructL(const TDesC& aFileName, TFileType aFileType);
	
	TVerifyResult CheckOffset(TUint offsetValue) const;
	
private:
	CJPEGExifDecoder* iDecoder;
	CExifReaderWriter* iExifReader;
	RFs iFs;
	};

#endif // EXIFVERIFIER_H
