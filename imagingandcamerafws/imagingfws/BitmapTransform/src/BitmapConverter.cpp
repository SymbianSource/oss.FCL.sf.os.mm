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
//

#include "BitmapTransforms.h"
#include "BitmapTransformsMain.h"
#include "BitmapTransformsBody.h"
#include "BitmapConverter.h"

/*
*
* Creates a CBitmapConverter
* 
* NewL
* @return CBitmapConverter*
* @internalTechnology
*
*/
EXPORT_C CBitmapConverter* CBitmapConverter::NewL()
	{
	CBitmapConverter* self = new(ELeave) CBitmapConverter();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

/*
*
* ~CBitmapConverter
* @internalTechnology
*
*/
EXPORT_C CBitmapConverter::~CBitmapConverter()
	{
	Cancel();
	delete iBody;
	}

/*
*
* CBitmapConverter
*
*/
CBitmapConverter::CBitmapConverter()
	{
	}

/*
*
* ConvertL
* @param aStatus
* @param aDstBitmap
* @param aSrcBitmap
* @internalTechnology
* @pre aRequestStatus is not NULL
* @pre aSrcBitmap Handle is not zero
* @pre aDstBitmap Handle is not zero 
* @pre  iBody is not NULL
* @post iBody is not NULL
*
*/
EXPORT_C void CBitmapConverter::Convert(TRequestStatus* aStatus, CFbsBitmap& aDstBitmap, CFbsBitmap& aSrcBitmap)
	{
	//[ preconditions ]
	__ASSERT_DEBUG( (aStatus != NULL), Panic( ENullArgument ) );
	//[ panic if the src has not been created]
	__ASSERT_DEBUG( (aSrcBitmap.Handle() != 0), Panic( ENoSourceBitmap ) );
   	//[assert the invariant ]
	__ASSERT_DEBUG( iBody, Panic( EBadInvariant ) );

	//[ convert aSrcBitmap to aDstBitmap ]
    iBody->Convert( aStatus, aDstBitmap, aSrcBitmap );  
	
	//[assert the invariant ]
	__ASSERT_DEBUG( iBody, Panic( EBadInvariant ) );
	}

/*
*
* Cancels an outstanding operation
* Cancel
* @internalTechnology
*
*/
EXPORT_C void CBitmapConverter::Cancel()
	{
	if(iBody)
		iBody->Cancel();
	}

/*
*
* Constructs a CBitmapConverter
*
* ConstructL
* @post invariant is true
*
*/
void CBitmapConverter::ConstructL()
	{
	iBody = CBitmapConverter::CBody::NewL();
	//[assert the invariant ]
	__ASSERT_DEBUG( iBody, Panic( EBadInvariant ) );
	}
