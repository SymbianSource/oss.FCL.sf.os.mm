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
//

#ifndef __IMAGECODEC_DEF_H__
#define __IMAGECODEC_DEF_H__

#include <e32cmn.h>
/**
@internalComponent


Class used to shadow the descriptor supplying the image data.

It maintains a flag to determine whether the descriptor contains a
filename or the image data itself.

This class is not intended for public use.
*/
class TImageParameterData
	{
public:

	/**
	Flag used to determine what the descriptor contains.
	*/
	enum TImageParameterDataFlag
		{
		/** Field not set.
		*/
		ENull		= 0x00000000,

		/** Descriptor is a filename.
		*/
		EFilename	= 0x00000001,

		/** Descriptor contains data.
		*/
		EData		= 0x00000002
		};

	TImageParameterData();
	void SetFilenamePtr(const TDesC* aSourceFilenamePtr);
	void SetDataPtr(const TDesC8* aSourceDataPtr);
	IMPORT_C TBool IsFilename() const;
	IMPORT_C const TDesC* SourceFilenamePtr() const;
	IMPORT_C const TDesC8* SourceDataPtr() const;
	TBool IsDataTypeDefined() const;

private:
	TImageParameterDataFlag iImageParameterDataFlag;
	union
		{
		const TDesC* iSourceFilenamePtr;
		const TDesC8* iSourceDataPtr;
		};
	};



#endif // __IMAGECODEC_H__
