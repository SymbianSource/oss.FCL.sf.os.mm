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

#ifndef PLUGINEXTENSIONMANAGER_H
#define PLUGINEXTENSIONMANAGER_H

#include <e32base.h>
#include <icl/imageconversionextensionintf.h>
#include <icl/imageprocessor.h>
#include "pluginextensionmanager.h"

/**
@file
@internalTechnology

Overview:
A set of internal classes to providing generic support for framework extension
functionality.  The CPluginExtensionManager contains the extensions (Operation
and Scaler) and is owned by the concrete plugin.  It also has access to the
read codec via the MReadCodecExtension interface, which should be implemented
by the concrete read codec.  Thus, CPluginExtensionManager has access to the
relevant classes in order to handle the call to GetDestinationSize().
*/

class MReadCodecExtension;
class COperationExtension;
class CScalerExtension;
class CImageProcessorExtension;

/**
Generic extension container for codec plugin use.
*/
NONSHARABLE_CLASS ( CPluginExtensionManager ) : public CBase
	{
public:
	IMPORT_C static CPluginExtensionManager* NewL(MReadCodecExtension* aReadCodec);
	IMPORT_C ~CPluginExtensionManager();
	IMPORT_C void ResetCodecExtension(MReadCodecExtension* aReadCodec);
	IMPORT_C static TInt ConvertScalingCoeffToReductionFactor(TInt aScalingCoeff);
	
	IMPORT_C void GetExtensionL(TUid aExtUid, MImageConvExtension*& aExtPtr);
	IMPORT_C void CreateExtensionForAutoRotateL();
	IMPORT_C TInt GetDestinationSize(TSize& aOriginalSize);
	IMPORT_C void SetClippingRectL(const TRect* aClipRect, const RPointerArray<TFrameInfo>& aFrameInfo);

	IMPORT_C TBool ClippingRectExtensionRequested() const;
	IMPORT_C TBool ScalerExtensionRequested() const;
	IMPORT_C TBool OperationExtensionRequested() const;
	
	IMPORT_C TRect ClippingRect();
	IMPORT_C TInt GetScalingCoefficient(TInt& aScalingCoeff, const TSize* aOriginalSize=NULL) const;
	IMPORT_C TInt GetScalerDesiredSize(TSize& aDesiredSize) const;
	IMPORT_C TInt GetScalerQuality(TImageConvScaler::TScalerQuality& aQuality) const;
	IMPORT_C TBool ScalerMaintainAspectRatio() const;
	IMPORT_C TTransformOptions Operation() const;
	IMPORT_C TTransformOptions OperationL(TUint16 aAutoRotationFlag) const;
	IMPORT_C TBool DimensionsSwapped() const;

	IMPORT_C TInt ValidateConvertData(TInt& aScalingCoeff, const TSize& aOriginalSize, TSize& aDestinationSize);
	IMPORT_C void TransferExtensionDataL(CImageProcessorExtension* aImageProcessor);
	
private:
	CPluginExtensionManager(MReadCodecExtension* aReadCodec);

private:
	COperationExtension* iOperationExtension;
	CScalerExtension* iScalerExtension;
	TRect iClippingRect;
	MReadCodecExtension* iReadCodec;
	};

/**
Generic plugin codec operation extension
*/
NONSHARABLE_CLASS ( COperationExtension ) : public MImageConvOperation
	{
friend class CPluginExtensionManager;	
	
public:
	static COperationExtension* NewL();
	~COperationExtension();

	// From MImageConvOperation
	TUid Uid() const;
	void IncrementRef();
	void Release();
	TUint Capabilities() const;
	void AddOperationL(TImageConvOperation::TOperation aOperation);
	void ClearOperationStack();

private:
	void InsertOperationL(TImageConvOperation::TOperation aOperation, TInt aPos);
	COperationExtension();
	TBool HasBeenSet();
	TBool DimensionsSwapped() const;
	TTransformOptions Operation() const;
	
private:
	TUint iRefCount;
	TUint iCapabilities;
	RArray<TImageConvOperation::TOperation> iOperationStack;
	};

/**
Generic plugin codec scaler extension
*/
NONSHARABLE_CLASS ( CScalerExtension ) : public MImageConvScaler
	{
friend class CPluginExtensionManager;
	
public:
	enum TScalerCmd
		{
		EScalerExtFullSize, // = 0
		EScalerExtUseScalingCoeff,
		EScalerExtUseDesiredSize
		};	

public:
	static CScalerExtension* NewL();
	~CScalerExtension();

	// From MImageConvScaler
	TUid Uid() const;
	void IncrementRef();
	void Release();
	void GetCapabilities(TScalerCaps& aCaps) const;
	void SetScalingL(const TSize& aDesiredSize, TImageConvScaler::TScalerQuality aQuality, TBool aLockAspectRatio);
	void SetScalingL(TInt aScalingCoeff, TImageConvScaler::TScalerQuality aScalingQuality);

private:
	CScalerExtension();
	TScalerCmd ScaleCmd() const;
	TInt GetScalingCoefficient(TInt& aScalingCoeff) const;
	TInt GetDesiredSize(TSize& aDesiredSize) const;
	TBool ScalingCoefficientSupported(TInt aScalingCoeff) const;
	TInt GetScalerQuality(TImageConvScaler::TScalerQuality& aQuality) const;
	TBool LockAspectRatioSet() const;

private:
	TUint iRefCount;
	TScalerCmd iCommand;
	TScalerCaps iCapabilities;
	TInt iScalingCoefficient;
	TSize iDesiredSize;
	TImageConvScaler::TScalerQuality iQuality;
	TBool iLockAspectRatio;
	};

#endif // PLUGINEXTENSIONMANAGER_H

