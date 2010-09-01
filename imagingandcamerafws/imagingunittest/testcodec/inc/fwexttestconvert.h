// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef FWEXTTESTCONVERT_H
#define FWEXTTESTCONVERT_H

#include <e32base.h>
#include <icl/imageplugin.h>
#include <icl/imageconversionextension.h>
#include <icl/imageconversionextensionintf.h>

// forward declaration
class CFwExtTestDecoderPlugin;
class CFwExtTestEncoderPlugin;
class CFwExtReadCodec;
class CFwExtWriteCodec;

// Concrete Decode Transform Extension class
class CDecodeOperationExtension : public CBase,
								  public MImageConvOperation
	{
public:
	static CDecodeOperationExtension* NewL(CFwExtTestDecoderPlugin* aParent);
	~CDecodeOperationExtension();

	// From MImageConvOperation
	TUid Uid() const;
	void IncrementRef();
	void Release();
	TUint Capabilities() const;
	void AddOperationL(TImageConvOperation::TOperation aOperation);
	void ClearOperationStack();
	
protected:
	CDecodeOperationExtension(CFwExtTestDecoderPlugin* aParent);
	
private:
	CFwExtTestDecoderPlugin* iParent;
	TInt iReferenceCount;
	TUint iOperationCaps;
	TUint iOperations;
	};

// Concrete Scaler Extension class
class CScalerExtension : public CBase,
						 public MImageConvScaler
	{
public:
	static CScalerExtension* NewL(CFwExtTestDecoderPlugin* aParent);
	~CScalerExtension();

	// From MImageConvScaler
	TUid Uid() const;
	void IncrementRef();
	void Release();
	void GetCapabilities(TScalerCaps& aCaps) const;
	void SetScalingL(const TSize& aDesiredSize, TImageConvScaler::TScalerQuality aQuality, TBool aLockAspectRatio);
	void SetScalingL(TInt aScalingCoeff, TImageConvScaler::TScalerQuality aScalingQuality);
	
protected:
	CScalerExtension(CFwExtTestDecoderPlugin* aParent);
	void ConstructL();
	
private:
	CFwExtTestDecoderPlugin* iParent;
	TInt iReferenceCount;
	TScalerCaps iScalerCaps;

	};

// Concrete Streamed Decode Extension class
class CStreamedDecodeExtension : public CBase,
								 public MImageConvStreamedDecode
	{
public:
	static CStreamedDecodeExtension* NewL(CFwExtTestDecoderPlugin* aParent);
	~CStreamedDecodeExtension();

	// From MImageConvStreamedDecode
	TUid Uid() const;
	void IncrementRef();
	void Release();
	void GetSupportedFormatsL(RArray<TUid>& aFormats, TUid& aOptimalFormat) const;
	void GetCapabilities(TUid aFormat, TInt aFrameNumber, TDecodeStreamCaps& aCaps) const;
	TInt GetBufferSize(TUid aFormat, TSize& aBlockSizeInPixels, TInt aNumBlocks) const;
	void InitFrameL(TUid aFormat, TInt aFrameNumber, TDecodeStreamCaps::TNavigation aNavigation);
	void GetBlocks(TRequestStatus* aStatus, CImageFrame* aFrame, TInt aSeqPosition, TInt aNumBlocksToGet, TInt* aNumBlocksRead);
	void GetNextBlocks(TRequestStatus* aStatus, CImageFrame* aFrame, TInt aNumBlocksToGet, TInt* aNumBlocksRead, TBool* aHaveMoreBlocks);

protected:
	CStreamedDecodeExtension(CFwExtTestDecoderPlugin* aParent);
	
private:
	CFwExtTestDecoderPlugin* iParent;
	TInt iReferenceCount;
	};

// Concrete Prepare Extension class for decoder plugin
class CPrepareDecodeExtension : public CBase,
								 public MImageConvPrepare
	{
public:
	static CPrepareDecodeExtension* NewL(CFwExtTestDecoderPlugin* aParent);
	~CPrepareDecodeExtension();

	// From MImageConvPrepare
	TUid Uid() const;
	void IncrementRef();
	void Release();
	void Prepare(TRequestStatus* aStatus);

protected:
	CPrepareDecodeExtension(CFwExtTestDecoderPlugin* aParent);
	
private:
	CFwExtTestDecoderPlugin* iParent;
	TInt iReferenceCount;
	};
	

// Concrete Prepare Extension class for decoder plugin
class CPrepareEncodeExtension : public CBase,
								 public MImageConvPrepare
	{
public:
	static CPrepareEncodeExtension* NewL(CFwExtTestEncoderPlugin* aParent);
	~CPrepareEncodeExtension();

	// From MImageConvPrepare
	TUid Uid() const;
	void IncrementRef();
	void Release();
	void Prepare(TRequestStatus* aStatus);

protected:
	CPrepareEncodeExtension(CFwExtTestEncoderPlugin* aParent);
	
private:
	CFwExtTestEncoderPlugin* iParent;
	TInt iReferenceCount;
	};

// Concrete Encode Transform Extension class
class CEncodeOperationExtension : public CBase,
								  public MImageConvOperation
	{
public:
	static CEncodeOperationExtension* NewL(CFwExtTestEncoderPlugin* aParent);
	~CEncodeOperationExtension();

	// From MImageConvOperation
	TUid Uid() const;
	void IncrementRef();
	void Release();
	TUint Capabilities() const;
	void AddOperationL(TImageConvOperation::TOperation aOperation);
	void ClearOperationStack();
	
protected:
	CEncodeOperationExtension(CFwExtTestEncoderPlugin* aParent);
	
private:
	CFwExtTestEncoderPlugin* iParent;
	TInt iReferenceCount;
	TUint iOperationCaps;
	TUint iOperations;
	};	

// Concrete Streamed Encode Extension class
class CStreamedEncodeExtension : public CBase,
								 public MImageConvStreamedEncode
	{
public:
	static CStreamedEncodeExtension* NewL(CFwExtTestEncoderPlugin* aParent);
	~CStreamedEncodeExtension();

	// From MImageConvStreamedEncode
	TUid Uid() const;
	void IncrementRef();
	void Release();
	void GetSupportedFormatsL(RArray<TUid>& aFormats, TUid& aOptimalFormat) const;
	void GetCapabilities(TUid aFormat, TEncodeStreamCaps& aCaps) const;
	void InitFrameL(TUid aFormat, TInt aFrameNumber, const TSize& aFrameSizeInPixels, const TSize& aBlockSizeInPixels, TEncodeStreamCaps::TNavigation aNavigation, const CFrameImageData* aFrameImageData);
	void AppendBlocks(TRequestStatus* aStatus, const CImageFrame& aBlocks, TInt aNumBlocksToAdd);
	void AddBlocks(TRequestStatus* aStatus, const CImageFrame& aBlocks, const TInt& aSeqPosition);
	void Complete(TRequestStatus* aStatus); 

protected:
	CStreamedEncodeExtension(CFwExtTestEncoderPlugin* aParent);
	
private:
	CFwExtTestEncoderPlugin* iParent;
	TInt iReferenceCount;
	};

// Decoder Plugin
class CFwExtTestDecoderPlugin : public CImageDecoderPlugin
	{
friend class CDecodeOperationExtension;
friend class CScalerExtension;
friend class CStreamedDecodeExtension;
friend class CPrepareDecodeExtension;
	
public:
	static CFwExtTestDecoderPlugin* NewL();
	~CFwExtTestDecoderPlugin();
	
	// From CImageDecoderPlugin
	void ImageType(TInt aFrameNumber, TUid& aImageType, TUid& aImageSubType) const;
	void ScanDataL();
	CFrameInfoStrings* FrameInfoStringsL(RFs& aFs, TInt aFrameNumber = 0);
	void GetExtensionL(TUid aExtUid, MImageConvExtension*& aExtPtr);
	TInt GetDestinationSize(TSize& aSize, TInt aFrameNumber = 0);
	void SetClippingRectL(const TRect* aClipRect);
	void HandleCustomSyncL(TInt aParam);
	void InitCustomAsyncL(TInt aParam);
	
protected:
	CFwExtTestDecoderPlugin();
	
private:
	// Codec
	CFwExtReadCodec* iReadCodec; // not owned
	
	// Actual extensions
	CDecodeOperationExtension* iOperationExt;
	CScalerExtension* iScalerExt;
	CStreamedDecodeExtension* iStreamedDecodeExt;
	CPrepareDecodeExtension* iPrepareExt; 
		
	// Pointers to test extension shared with test code
	CDecodeOperationExtension* iTestOperationExtPtr; // owned
	CScalerExtension* iTestScalerExtPtr; // owned
	CStreamedDecodeExtension* iTestStreamedDecodeExtPtr; // owned
	CPrepareDecodeExtension* iTestPrepareExtPtr; //owned
	
	TSize iDestSize;
	};

// Encoder Plugin
class CFwExtTestEncoderPlugin : public CImageEncoderPlugin
	{
friend class CEncodeOperationExtension;
friend class CStreamedEncodeExtension;
friend class CPrepareEncodeExtension;
	
public:
	static CFwExtTestEncoderPlugin* NewL();
	~CFwExtTestEncoderPlugin();
	
	// From CImageEncoderPlugin
	void PrepareEncoderL(const CFrameImageData* aFrameImageData);
	void UpdateHeaderL();
	void HandleCustomSyncL(TInt aParam);
	void GetExtensionL(TUid aExtUid, MImageConvExtension*& aExtPtr);
	
protected:
	CFwExtTestEncoderPlugin();
	void SetCodec(CFwExtWriteCodec* aWriteCodec);
	
private:
	// Codec
	CFwExtWriteCodec* iWriteCodec; // not owned
	
	// Actual extensions
	CEncodeOperationExtension* iOperationExt;
	CStreamedEncodeExtension* iStreamedEncodeExt;
	CPrepareEncodeExtension* iPrepareExt; 
	
	// Pointers to test extension shared with test code
	CEncodeOperationExtension* iTestOperationExtPtr; // owned
	CStreamedEncodeExtension* iTestStreamedEncodeExtPtr; // owned
	CPrepareEncodeExtension* iTestPrepareExtPtr; //owned
	};

#endif // FWEXTTESTCONVERT_H