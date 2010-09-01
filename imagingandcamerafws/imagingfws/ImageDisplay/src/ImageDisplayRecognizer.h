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

#ifndef IMAGEDISPLAYRECOGNIZER_H
#define IMAGEDISPLAYRECOGNIZER_H

#include <imageconversion.h>

/** @file
	@internalComponent */

/**
@internalComponent

Utility class providing Mime type recognition based on file 
extension or image data using information provided in the plug-in
opaque data - file extension/identification string
*/
NONSHARABLE_CLASS( CImgDisplayMimeTypeRecognizer ) : public CBase
	{
	
public:
	IMPORT_C static CImgDisplayMimeTypeRecognizer* NewL();

	~CImgDisplayMimeTypeRecognizer();
	IMPORT_C TBool GetMimeTypeL(const TDesC8& aImageData, const TDesC& aFileName, TDes8& aMimeType);
	IMPORT_C void GetFileTypesL(RFileExtensionMIMETypeArray& aExtArray);
	
private:
	CImgDisplayMimeTypeRecognizer();
	void ConstructL();
	void GetTypeByFileNameL(const TDesC& aFileName,TDes8& aMimeType);
	
private:
	class CBody;
	CBody* iBody;	
	};


#endif //ndef IMAGEDISPLAYRECOGNIZER_H


