/*
* Copyright (c) 2010 ISB.
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of the "Symbian Foundation License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.symbianfoundation.org/legal/sfl-v10.html".
*
* Initial Contributors:
* ISB - Initial contribution
*
* Contributors:
*
* Description:
*
*/

#ifndef ECAMWEBCAMERABUFFER_H
#define ECAMWEBCAMERABUFFER_H

#include <e32base.h>
#include <ecam.h>


class CFbsBitmap;
class CWebCameraBuffer : public CBase
					   , public MCameraBuffer
	{
public:
	static CWebCameraBuffer* NewL(const CFbsBitmap& aBitmap, HBufC8* aData);
	static CWebCameraBuffer* NewLC(const CFbsBitmap& aBitmap, HBufC8* aData);
	static CWebCameraBuffer* NewL(CFbsBitmap* aBitmap, HBufC8* aData);
	static CWebCameraBuffer* NewLC(CFbsBitmap* aBitmap, HBufC8* aData);

public:
	virtual TInt NumFrames();
	virtual TDesC8* DataL( TInt aFrameIndex );
	virtual CFbsBitmap& BitmapL( TInt aFrameIndex );
	virtual RChunk& ChunkL();
	virtual TInt ChunkOffsetL( TInt aFrameIndex );
	virtual TInt FrameSize( TInt aFrameIndex );
	virtual void Release();
    ~CWebCameraBuffer();

private:
	void ConstructL(const CFbsBitmap& aBitmap, HBufC8* aData);
	void ConstructL(CFbsBitmap* aBitmap, HBufC8* aData);
    CWebCameraBuffer();

private:
	CFbsBitmap* iBitmap;
	HBufC8*     iImageData;
	RChunk      iChunk;

	TBool iOwnBitmap;
	TBool iOwnData;
	};  

#endif // ECAMWEBCAMERABUFFER_H
