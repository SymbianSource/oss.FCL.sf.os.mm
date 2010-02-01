// Copyright (c) 1998-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// MdaImageConverter.h
// Implementation of the API interfaces defined in MdaImageConverter.h
// 
//

#ifndef __MDAIMAGECONVERTERPRIV_H__
#define __MDAIMAGECONVERTERPRIV_H__

#include <fbs.h>
#include <bitdev.h>

#include <icl/imagedata.h>
#include <mda/client/resource.h>
#include <mda/client/utility.h>

#include "BitmapConverter.h"
#include <bitmaptransforms.h>

#include <mda/common/video.h>

enum 
	{
	EMDAImConvPanicNotImplemented=1,
	EMDAImConvPanicNotYetOpen
	};


class CMdaServer;
class REComSession;
class MMdaImageUtilObserver;
class TFrameDataBlock;
class TImageDataBlock;

// CMdaImageUtilityPriv
class CMdaImageUtilityPriv; // declared here
NONSHARABLE_CLASS( CMdaImageUtilityPriv ): public CBase
	{
friend class CMdaImageUtility;
friend class CMdaImageDataReadUtility;
friend class CMdaImageBitmapToBitmapUtility;

public:
	virtual ~CMdaImageUtilityPriv();
protected:
	virtual void Close();
	virtual void ConvertL(CFbsBitmap& aBitmap,TInt aFrameNumber = 0) = 0;
	virtual void ConvertL(CFbsBitmap& aBitmap,CFbsBitmap& aMaskBitmap,TInt aFrameNumber = 0) = 0;
	virtual void CancelConvertL() = 0;
	virtual void FrameInfo(TInt aFrame,TFrameInfo& aInfo) const;
	virtual TInt FrameCount() const;
protected:
	CMdaImageUtilityPriv(MMdaImageUtilObserver& aObserver, CMdaImageUtility* aParent);
	void Close(TBool aFullClose);
	void Initialize();
	virtual void DoClose();
	void SelfComplete(TInt aError);
	void SetUidsFromOrigFormatsL(TMdaClipFormat* aFormat,TMdaPackage* aCodec,TMdaPackage* aExtra);
	void OpenFsSessionL();
	void OpenEcomSessionL();
	void CloseEcomSession();
#if defined(__ECOM_POWERDOWN_WORKAROUND)
	void OpenWaitTimerL();
#endif defined(__ECOM_POWERDOWN_WORKAROUND)
	// Calls from CActive derived parent objects
	virtual void RunL()=0;
	virtual void DoCancel()=0;
protected:
	MMdaImageUtilObserver&	iObserver;
	CMdaImageUtility*		iParent;	// link back to the handle class
	TUid					iImageType;
	TUid					iImageSubType;
	TFrameDataBlock*		iFrameData;
	TImageDataBlock*		iImageData;
	RFs						iFs;
	REComSession*			iEcomSession; // not owned
#if defined(__ECOM_POWERDOWN_WORKAROUND)
	RTimer iWaitTimer;
	TInt iWaitTimeCount;
#endif defined(__ECOM_POWERDOWN_WORKAROUND)
	};

// CMdaImageDataReadUtilityPriv
class CImageDecoder;
class CMdaImageDataReadUtility;
class CMdaImageDataReadUtilityPriv; // declared here
NONSHARABLE_CLASS( CMdaImageDataReadUtilityPriv ): public CMdaImageUtilityPriv
	{
public:
	virtual ~CMdaImageDataReadUtilityPriv();
	// From CMdaImageUtility
	virtual void ConvertL(CFbsBitmap& aBitmap,TInt aFrameNumber = 0);
	virtual void ConvertL(CFbsBitmap& aBitmap,CFbsBitmap& aMaskBitmap,TInt aFrameNumber = 0);
	virtual void CancelConvertL();
	virtual void RunL();
	virtual void DoCancel();
public:
	virtual void FrameInfo(TInt aFrame,TFrameInfo& aInfo) const;
	virtual TInt FrameCount() const;
	virtual void DoClose();
	const TDesC& FrameCommentL(TInt aFrameNumber);
protected:
	enum TState
		{
		EStateClosed,
		EStateOpening,
		EStateOpeningWait,
		EStateOpen,
		EStateConverting
		};
	CMdaImageDataReadUtilityPriv(MMdaImageUtilObserver& aObserver, CMdaImageDataReadUtility* aParent);

	// Creates relevant iDecoder property - depending on subclass.
	// Called by "background" AO processing.
 	virtual void DoOpenL()=0;
private:
	inline CMdaImageDataReadUtility* Parent() const;
	void GetCommentL();

protected:
	CImageDecoder* iDecoder;
	TState iState;

	TBool iPrevConvertUnderflowed;
	TSize iDestinationSize; // used to check we don't continue with different bitmap
	TInt iDestinationHandle;
	TInt iDestinationMaskHandle; 
	TInt iKeptFrameNumber;
	TDisplayMode iDestinationDisplayMode;
	TDisplayMode iDestinationMaskDisplayMode;
	RPointerArray<HBufC> iComment;
	};


class CImageEncoder;
class CFrameImageData;
class CMdaImageDataWriteUtility;
class CMdaImageDataWriteUtilityPriv; // declared here
NONSHARABLE_CLASS( CMdaImageDataWriteUtilityPriv ): public CMdaImageUtilityPriv
	{
friend class CMdaImageDataWriteUtility;

public:
	virtual ~CMdaImageDataWriteUtilityPriv();
	// From CMdaImageUtility
	virtual void ConvertL(CFbsBitmap& aBitmap,TInt aFrameNumber = 0);
	virtual void ConvertL(CFbsBitmap& aBitmap,const TRect& aSourceRect,TInt aFrameNumber = 0);
	virtual void ConvertL(CFbsBitmap& aBitmap,CFbsBitmap& aMaskBitmap,TInt aFrameNumber = 0);
	virtual void CancelConvertL();
	CImageEncoder* Encoder() const;
protected:
	virtual void RunL();
	virtual void DoCancel();
protected:
	enum TState
		{
		EStateClosed,
		EStateCreating,
		EStateOpen,
		EStateConverting,
		EStateCopying,
		EStateCancelled
		};
	CMdaImageDataWriteUtilityPriv(MMdaImageUtilObserver& aObserver, CMdaImageDataWriteUtility* aParent);

	virtual void DoClose();

	// Creates relevant <code>iEncoder</code> property - depending on subclass.
	// Called by "background" AO processing.
	virtual void DoCreateL(TBool aFullCreate=ETrue)=0;

	// Used at end to copy any data to destination if req. Default is no-op.
	virtual void DoFinalCopyL();

	// Complete iEncoder initialisation once created
	void CompleteEncoderCreationL();
private:
	inline CMdaImageDataWriteUtility* Parent() const;
protected:
	CImageEncoder* iEncoder;
	CFrameImageData* iFrameImageData;
	TState iState;
	};

class CMdaImageFileToBitmapUtility;
class CMdaImageFileToBitmapUtilityPriv; // declared here
NONSHARABLE_CLASS( CMdaImageFileToBitmapUtilityPriv ): public CMdaImageDataReadUtilityPriv
	{
public:
	static CMdaImageFileToBitmapUtilityPriv* NewL(MMdaImageUtilObserver& aObserver, CMdaImageFileToBitmapUtility* aParent);
	virtual ~CMdaImageFileToBitmapUtilityPriv();
	void OpenL(const TDesC& aFileName,TMdaClipFormat* aFormat = NULL,TMdaPackage* aCodec = NULL,TMdaPackage* aExtra = NULL);
protected:
	CMdaImageFileToBitmapUtilityPriv(MMdaImageUtilObserver& aObserver, CMdaImageFileToBitmapUtility* aParent);
	void ConstructL();
	// from CMdaImageDataReadUtility
	void DoOpenL();
private:
	inline CMdaImageFileToBitmapUtility* Parent() const;
protected:
	HBufC *iFileName;
	};

class CMdaImageDescToBitmapUtility;
class CMdaImageDescToBitmapUtilityPriv; // declared here
NONSHARABLE_CLASS( CMdaImageDescToBitmapUtilityPriv ): public CMdaImageDataReadUtilityPriv
	{
public:
	static CMdaImageDescToBitmapUtilityPriv* NewL(MMdaImageUtilObserver& aObserver,CMdaImageDescToBitmapUtility* aParent);
	void OpenL(const TDesC8& aDescriptor,TMdaClipFormat* aFormat = NULL,TMdaPackage* aCodec = NULL,TMdaPackage* aExtra = NULL);
protected:
	CMdaImageDescToBitmapUtilityPriv(MMdaImageUtilObserver& aObserver, CMdaImageDescToBitmapUtility* aParent);
	void ConstructL();
	// from CMdaImageDataReadUtilityPriv
	void DoOpenL();
private:
	inline CMdaImageDescToBitmapUtility* Parent() const;
protected:
	const TDesC8* iDescriptor; // not owned
	};

class CMdaImageBitmapToFileUtility;
class CMdaImageBitmapToFileUtilityPriv; // declared here
NONSHARABLE_CLASS( CMdaImageBitmapToFileUtilityPriv ): public CMdaImageDataWriteUtilityPriv
	{
public:
	static CMdaImageBitmapToFileUtilityPriv* NewL(MMdaImageUtilObserver& aObserver,CMdaImageBitmapToFileUtility* aParent);
	virtual ~CMdaImageBitmapToFileUtilityPriv();
	void CreateL(const TDesC& aFileName,TMdaClipFormat* aFormat,TMdaPackage* aCodec,TMdaPackage* aExtra);
protected:
	CMdaImageBitmapToFileUtilityPriv(MMdaImageUtilObserver& aObserver, CMdaImageBitmapToFileUtility* aParent);
	void ConstructL();
	// from CMdaImageDataWriteUtility
	void DoCreateL(TBool aFullCreate=ETrue);
private:
	inline CMdaImageBitmapToFileUtility* Parent() const;
protected:
	HBufC *iFileName;
	};

class CMdaImageBitmapToDescUtility;
class CMdaImageBitmapToDescUtilityPriv; // declared here
NONSHARABLE_CLASS( CMdaImageBitmapToDescUtilityPriv ): public CMdaImageDataWriteUtilityPriv
	{
public:
	static CMdaImageBitmapToDescUtilityPriv* NewL(MMdaImageUtilObserver& aObserver,CMdaImageBitmapToDescUtility* aParent);
	void CreateL(TDes8& aDescriptor,TMdaClipFormat* aFormat,TMdaPackage* aCodec,TMdaPackage* aExtra);
	virtual ~CMdaImageBitmapToDescUtilityPriv();
protected:
	CMdaImageBitmapToDescUtilityPriv(MMdaImageUtilObserver& aObserver, CMdaImageBitmapToDescUtility* aParent);
	// from CMdaImageDataWriteUtility
	virtual void DoCreateL(TBool aFullCreate=ETrue);
	virtual void DoFinalCopyL();
private:
	inline CMdaImageBitmapToDescUtility* Parent() const;
protected:
	TDes8*	iDescriptor;
	HBufC8* iSecondBuffer; 
	};

class CMdaImageBitmapToBitmapUtilityPriv; // declared here
NONSHARABLE_CLASS( CMdaImageBitmapToBitmapUtilityPriv ): public CMdaImageUtilityPriv
	{
public:
	static CMdaImageBitmapToBitmapUtilityPriv* NewL(MMdaImageUtilObserver& aObserver, CMdaImageBitmapToBitmapUtility* aParent);
	~CMdaImageBitmapToBitmapUtilityPriv();
	void OpenL(CFbsBitmap& aBitmap);
	// from CMdaImageUtility
	virtual void ConvertL(CFbsBitmap& aBitmap, TInt aFrameNumber = 0);
	virtual void ConvertL(CFbsBitmap& aBitmap, CFbsBitmap& aMaskBitmap, TInt aFrameNumber = 0);
	virtual void CancelConvertL();
	virtual void RunL();
	virtual void DoCancel();
	// From CMdaImageUtility
	virtual void DoClose();
protected:
	CMdaImageBitmapToBitmapUtilityPriv(MMdaImageUtilObserver& aObserver, CMdaImageBitmapToBitmapUtility* aParent);
private:
	inline CMdaImageBitmapToBitmapUtility* Parent() const;

protected:
	enum TUtilState
		{
		EClosed,
		EReady,
		EOpening,
		ECopying
		};
protected:
//	RMdaSourceStreamPort iSrcPort;
//	RMdaDestinationStreamPort iDstPort;
private:
	CBitmapConverter* iBitmapCopier;
	TUtilState iState;
	CFbsBitmap* iSrcBitmap;
	};

class CMdaBitmapRotator;
class CMdaBitmapRotatorPriv; // declared here
NONSHARABLE_CLASS( CMdaBitmapRotatorPriv ): public CActive
	{
public:
	static CMdaBitmapRotatorPriv* NewL();
	virtual ~CMdaBitmapRotatorPriv();
	void RotateL(MMdaImageUtilObserver& aObserver,CFbsBitmap& aSrcBitmap,CFbsBitmap& aTgtBitmap,CMdaBitmapRotator::TRotationAngle aAngle);
	void RotateL(MMdaImageUtilObserver& aObserver,CFbsBitmap& aBitmap,CMdaBitmapRotator::TRotationAngle aAngle);
	void CancelRotation();
protected:	
	CMdaBitmapRotatorPriv();
	void ConstructL();
	// from CActive
	virtual void RunL();
	virtual void DoCancel();
	CBitmapRotator::TRotationAngle MapRotationAngle(CMdaBitmapRotator::TRotationAngle aAngle);
protected:
	MMdaImageUtilObserver*	iObserver; // not owned
	CBitmapRotator*			iBitmapRotator;
	};

class CMdaBitmapScaler;
class CMdaBitmapScalerPriv; // declared here
NONSHARABLE_CLASS( CMdaBitmapScalerPriv ): public CActive
	{
public:
	static CMdaBitmapScalerPriv* NewL();
	virtual ~CMdaBitmapScalerPriv();
	void ScaleL(MMdaImageUtilObserver& aObserver,CFbsBitmap& aSrcBitmap,CFbsBitmap& aTgtBitmap,TBool aMaintainAspectRatio = ETrue);
	void ScaleL(MMdaImageUtilObserver& aObserver,CFbsBitmap& aBitmap,TSize aDestSize,TBool aMaintainAspectRatio = ETrue);
	void CancelScaling();
protected:
	CMdaBitmapScalerPriv();
	void ConstructL();
	// from CActive
	virtual void RunL();
	virtual void DoCancel();
protected:
	MMdaImageUtilObserver*	iObserver;	// not owned
	CBitmapScaler*			iBitmapScaler;
	};

#include "MdaImageConverterPriv.inl"

#endif // __MDAIMAGECONVERTERPRIV_H__

