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

#ifndef JPEGEXIFPLUGIN_H
#define JPEGEXIFPLUGIN_H

#include <iclexif.h>

/**
@internalTechnology
*/
class CJPEGExifDecoder::CBody : public CBase
	{
public:
	IMPORT_C static CJPEGExifDecoder::CBody* NewL(CJPEGExifDecoder& aHandle);

	IMPORT_C MExifMetadata* ExifMetadata();
	
private:
	CBody(CJPEGExifDecoder& aHandle);
	CJPEGExifDecoder& iHandle;
	};
	

/**
@internalTechnology
*/
class CJPEGExifEncoder::CBody : public CBase
	{
public:
	IMPORT_C static CJPEGExifEncoder::CBody* NewL(CJPEGExifEncoder& aHandle);

	IMPORT_C MExifMetadata* ExifMetadata();
	
private:
	CBody(CJPEGExifEncoder& aHandle);
	CJPEGExifEncoder& iHandle;
	};


#endif // JPEGEXIFPLUGIN_H

