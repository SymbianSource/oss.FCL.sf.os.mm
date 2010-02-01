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

#ifndef __ICL_CLIENT_RECOGNIZER_H__
#define __ICL_CLIENT_RECOGNIZER_H__

#include <e32base.h>
#include <apmrec.h>
#include <apmstd.h>

#include <imageconversion.h>
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include <iclrecognizerutil.h>
#endif

const int KRecIclDllUidValue = 0x101f7c0b;

class CImgDisplayMimeTypeRecognizer;

/**
 * @internalComponent
 *
 * Class owned by the CApaIclRecognizer recognizer class.
 * maintains a list of MIME-types supported and
 * an instance of the CIclRecognizerUtil class which contains
 * ECOM-derived information about the header data to match against.
 * Both the ImageDecoder and ImageDisplay plug-ins are used to determine
 * Mime type. ImageDecoder plug-ins are used first.
 * 
 */
class CIclRecognizer : public CBase
	{
public:
	enum TMatchMethod
		{
		ENotMatched,
		EByName,
		EBySignature
		};
public:
	static CIclRecognizer* NewL();	
	virtual ~CIclRecognizer();

	void BuildListL();

	TMatchMethod MatchL(const TDesC8& aBuffer, const TDesC& aFileName);
	
	const TDesC8& MimeString() const;

	const TDesC8& SupportedDataTypeL(TInt aIndex) const;
	TInt NumMimeTypes() const;

private:
	CIclRecognizer();
	void ConstructL();

private:
	RFileExtensionMIMETypeArray iFileExtensionMIMETypeArray;	
	TBuf8<KMaxDataTypeLength> iMimeString;
	CIclRecognizerUtil* iIclRecognizerUtil;	// owned
	CImgDisplayMimeTypeRecognizer* iImgDisplayRecognizer;
	};

/**
 * @internalAll
 *
 * Concrete implementation of the recognizer API
 * Used to recognize non-native files supported by the
 * Image conversion library. e.g .BMP files etc.
 */
class CApaIclRecognizer : public CApaDataRecognizerType 
	{
public: 
	static CApaIclRecognizer* NewL();		
	~CApaIclRecognizer();

	TDataType SupportedDataTypeL(TInt aIndex) const;
	TUint PreferredBufSize();	
	void DoRecognizeL(const TDesC& aName, const TDesC8& aBuffer);

private: 
	CApaIclRecognizer();
	void ConstructL();

private:
	CIclRecognizer* iIclRecognizer;
	};


#endif
