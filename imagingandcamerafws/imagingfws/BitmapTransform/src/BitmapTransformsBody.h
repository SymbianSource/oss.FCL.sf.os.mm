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

#ifndef __BITMAPTRANSFORMSBODY_H__
#define __BITMAPTRANSFORMSBODY_H__

#include <e32base.h>
#include <fbs.h>
#include <bitdev.h>

#include "BitmapTransforms.h" // required for enums
#include "BitmapConverter.h"

class CBitmapRotator;
class CBitmapScaler;
class MBitmapRotatorPlugin;
class MBitmapScalerPlugin;

class CBitmapRotatorBody; // declared here
/**
*
* The private API for clients to call the BitmapTransforms Library bitmap rotation
* @internalTechnology
*            
*/

NONSHARABLE_CLASS( CBitmapRotatorBody ): public CBase
	{
	friend class CBitmapRotator;
private:
	typedef CBitmapRotator::TRotationAngle TRotationAngle;

private:
	static CBitmapRotatorBody* NewL();
	~CBitmapRotatorBody();

	void Rotate(TRequestStatus* aRequestStatus, CFbsBitmap& aSrcBitmap, CFbsBitmap& aTgtBitmap, TRotationAngle aAngle);
	void Rotate(TRequestStatus* aRequestStatus, CFbsBitmap& aBitmap, TRotationAngle aAngle);
	TInt CustomCommand(TUid aUid, TAny* aParam);
	void Cancel();

	CBitmapRotatorBody();
	void ConstructL();

private:
	MBitmapRotatorPlugin* iPlugin;
	TUid				  iDestructorKey;
	};

// class CBitmapScalerBody; // declared here
/**
*
* Body of CBitmapScaler
*
* @internalTechnology
*
*/
NONSHARABLE_CLASS( CBitmapScalerBody ): public CBase
	{
	friend class CBitmapScaler;
private:
	static CBitmapScalerBody* NewL();
	~CBitmapScalerBody();

	void Scale(TRequestStatus* aRequestStatus, CFbsBitmap& aSrcBitmap, CFbsBitmap& aTgtBitmap, TBool aMaintainAspectRatio = ETrue);
	void Scale(TRequestStatus* aRequestStatus, CFbsBitmap& aBitmap, const TSize& aDestinationSize, TBool aMaintainAspectRatio = ETrue);
	void Cancel();
	TInt CustomCommand(TUid aUid, TAny* aParam);

	CBitmapScalerBody();
	void ConstructL();

private:
	MBitmapScalerPlugin* iPlugin;
	TUid				 iDestructorKey;
	};

/**
*
* Body of CBitmapConverter
* Part of the handle body idiom which separates
* interface from implementation
*
* @internalTechnology
*
*/

class CImageProcessor;
NONSHARABLE_CLASS( CBitmapConverter::CBody ): public CActive
	{
	friend class CBitmapConverter;
public:
	static CBody* NewL();
	~CBody();
	void Convert(TRequestStatus* aStatus, CFbsBitmap& aDstBitmap, CFbsBitmap& aSrcBitmap);
protected:
	// from CActive
	virtual void RunL();
	virtual void DoCancel();
private:
	CBody();
	void ConstructL();
	void DoConvert();
	void SelfComplete(TInt aError);
	void RequestComplete(TInt aReason);
private:
	TRgb* iPixelBuffer;	
	TPtr8 iPixelDes;
	TSize iSrcSize;
	CImageProcessor* iImageProc;
	CFbsBitmap* iSrcBitmap;
	TRequestStatus* iCopyStatus;
	TPoint iPos;
	};

#include "BitmapRotationBody.inl"
#include "BitmapScalingBody.inl"

#endif  // __BITMAPTRANSFORMSBODY_H__

