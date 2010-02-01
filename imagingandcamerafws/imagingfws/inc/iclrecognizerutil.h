// Copyright (c) 2001-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// This is the public client API for the Image Conversion Library
// 
//

/**
 @file
 @internalComponent
 @prototype
*/
#ifndef ICLRECOGNIZERUTIL_H
#define ICLRECOGNIZERUTIL_H

/**
@internalComponent

ICL utility class used by ICL recognizer
Maintains an array of CImplementationInformation objects
so that data headers can be speedily matched against.
Updates the array when notified by ECOM of a change to the global
interface implementation registration data.
*/
NONSHARABLE_CLASS( CIclRecognizerUtil ) : public CBase
	{
public:
	IMPORT_C static CIclRecognizerUtil* NewL();
	~CIclRecognizerUtil();
	IMPORT_C TBool GetMimeTypeL(const TDesC8& aImageData, const TDesC& aFileName, TDes8& aMimeType);

private:
	CIclRecognizerUtil();
	void ConstructL();

private:
	class CBody;
	CBody* iBody;	
	};

#endif // ICLRECOGNIZERUTIL_H

