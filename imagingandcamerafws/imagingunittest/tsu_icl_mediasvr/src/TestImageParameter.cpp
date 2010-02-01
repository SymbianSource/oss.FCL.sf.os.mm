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
// This file contains the TTestImage class which is used to store the name of images and format
// parameters for tests
// 
//

#include <mdaimageconverter.h>
#include "TestImageParameter.h"
/**
  *
  * Constructor.
  *
  */
TTestImage::TTestImage()
	{
	}

/**
  *
  * Constructor. calls SetTestImage to set the passed parameters
  *
  *	@param		"const TDesC& aSourceFile"
  *				The source file to be used for the test
  * @param		"const TMdaClipFormat* aFormat"
  *				The format of the file
  * @param		"const TMdaPackage* aCodec"
  *				The codec used
  *
  */

TTestImage::TTestImage(const TDesC& aSourceFile,const TMdaClipFormat* aFormat,const TMdaPackage* aCodec) 
	{
	SetTestImage(aSourceFile , aFormat, aCodec);
	}

/**
  *
  * Constructor. calls SetTestImage to set the passed parameters
  *
  *	@param		"const TDesC& aSourceFile"
  *				The source file to be used for the test
  * @param		"const TMdaClipFormat* aFormat"
  *				The format of the file
  * @param		"const TMdaPackage* aCodec"
  *				The codec used
  *	@param		"const TDesC& aRefFile"
  *				A reference file to compare the output of the test to
  *
  */
TTestImage::TTestImage(const TDesC& aSourceFile,const TMdaClipFormat* aFormat,const TMdaPackage* aCodec, const TDesC& aRefFile) 
	{
	SetTestImage(aSourceFile,aFormat,aCodec,aRefFile);
	}

/**
  *
  * Stores pointers to the parameters given
  *
  *	@param		"const TDesC& aSourceFile"
  *				The source file to be used for the test
  * @param		"const TMdaClipFormat * aFormat"
  *				The format of the file
  * @param		"const TMdaPackage * aCodec"
  *				The codec used
  *
  */
void TTestImage::SetTestImage(const TDesC& aSourceFile,const TMdaClipFormat* aFormat,const TMdaPackage* aCodec) 
	{
	iSourceFile = CONST_CAST(TDesC*, &aSourceFile);
	iFormat = CONST_CAST(TMdaClipFormat*, aFormat);
	iCodec = CONST_CAST(TMdaPackage*,aCodec);
	}

/**
  *
  * Stores pointers to the parameters given
  *
  *	@param		"const TDesC& aSourceFile"
  *				The source file to be used for the test
  * @param		"const TMdaClipFormat* aFormat"
  *				The format of the file
  * @param		"const TMdaPackage* aCodec"
  *				The codec used
  *	@param		"const TDesC& aRefFile"
  *				A reference file to compare the output of the test to
  *
  */
void TTestImage::SetTestImage(const TDesC& aSourceFile,const TMdaClipFormat* aFormat,const TMdaPackage* aCodec, const TDesC& aRefFile) 
	{
	iSourceFile = CONST_CAST(TDesC*,&aSourceFile);
	iFormat = CONST_CAST(TMdaClipFormat*,aFormat);
	iCodec = CONST_CAST(TMdaPackage*,aCodec);
	iRefFile = CONST_CAST(TDesC*, &aRefFile);
	}

/**
  *
  * Copy Constructor.
  *
  */
TTestImage::TTestImage(const TTestImage& aTestImage) :
		iSourceFile(CONST_CAST(TDesC*,aTestImage.iSourceFile)),
		iFormat(aTestImage.iFormat),
		iCodec(aTestImage.iCodec)
	{
	}
