// Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __BITMAP_CONVERTER_H__
#define __BITMAP_CONVERTER_H__

class CBitmapConverter; // declared here
/**
*
* The Internal Technology API for clients 
* to call BitmapTransforms Library bitmap conversion
*
* @internalTechnology
*
*/
NONSHARABLE_CLASS( CBitmapConverter ): public CBase
	{
public:
	IMPORT_C static CBitmapConverter* NewL();
	IMPORT_C ~CBitmapConverter();
	IMPORT_C void Convert(TRequestStatus* aStatus, CFbsBitmap& aDstBitmap, CFbsBitmap& aSrcBitmap);
	IMPORT_C void Cancel();

private:
	CBitmapConverter();
	void ConstructL();

private:
	class CBody;
	CBody* iBody;
	};

#endif
