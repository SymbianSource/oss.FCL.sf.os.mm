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

#ifndef __EXIFJPEGTRANFORM_H__
#define __EXIFJPEGTRANFORM_H__

#include <e32base.h>
#include <icl/imagetransformplugin.h>

class CExifJpegTransformExtension;
class MExifMetadata;
class CExifEditUtility;
class MExifSource;
class MExifDest;
class CExifJpegTransform : public CImageTransformPlugin
	{

public:
	// Public factory functions:
	static CExifJpegTransform* NewL();
	
	~CExifJpegTransform();
	
	void Transform(TRequestStatus& aStatus);
	void OpenL();
	void CancelTransform();
	CImageTransformPluginExtension* Extension() const;
	
	MExifMetadata* ExifMetadata() const ;

private:
	
	CExifJpegTransform(); 
	void ConstructL();
	
private:
	CExifEditUtility* iExifEdit;
	RFs iFs;
	CImageTransformPluginExtension* iExtension;
	MExifSource* iSource;
	MExifDest* iDest;
	};

#endif // __EXIFJPEGTRANFORM_H__
