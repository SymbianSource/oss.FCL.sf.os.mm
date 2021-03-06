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

#ifndef VIDEOPLAYFILE_COV_H
#define VIDEOPPLAYFILE_COV_H

#include "tsu_mdf_videocodecs_step.h"
#include <mmf/devvideo/devvideoplay.h>


class RTestStepVideoCodecs;
class CPlayVideoFileCov;


// For Play tests
class CPlayVideoFileCov : public CActive, public MMMFDevVideoPlayProxy
	{
public:

enum TPlayVideoFileState
	{
	EHwDeviceInit,
	EHwDeviceStartDecode,
	EHwDeviceAllowToComplete,
	EHwDeviceEncodeCheckData,		
	EHwDeviceDone,
	EHwDeviceError
	};	
	
public:
	static CPlayVideoFileCov* NewL(RTestStepVideoCodecs* aParent);
	~CPlayVideoFileCov();

	void LoadCodecL();
	void SetDSA(TBool aUseDSA);
	void StartPlaybackL();
	void SetState(TPlayVideoFileState aState);
	void CleanupAndSetDeviceError(TPtrC16 aText);	
	void StartDirectScreenAccessL();	
	
	// From CActive
	void RunL();
	void DoCancel();
    
    // From MMMFDevVideoPlayProxy	
	virtual void MdvppNewPicture(TVideoPicture* aPicture);
	virtual void MdvppNewBuffers();
	virtual void MdvppReturnPicture(TVideoPicture* aPicture);
	virtual void MdvppSupplementalInformation(const TDesC8& aData, 
		const TTimeIntervalMicroSeconds& aTimestamp, const TPictureId& aPictureId);
	virtual void MdvppPictureLoss();
	virtual void MdvppPictureLoss(const TArray<TPictureId>& aPictures);
	virtual void MdvppSliceLoss(TUint aFirstMacroblock, TUint aNumMacroblocks, const TPictureId& aPicture);
	virtual void MdvppReferencePictureSelection(const TDesC8& aSelectionData);
	virtual void MdvppTimedSnapshotComplete(TInt aError, TPictureData* aPictureData, 
		const TTimeIntervalMicroSeconds& aPresentationTimestamp, const TPictureId& aPictureId);
	virtual void MdvppFatalError(CMMFVideoHwDevice* aDevice, TInt aError);
	virtual void MdvppInitializeComplete(CMMFVideoHwDevice* aDevice, TInt aError);
	virtual void MdvppStreamEnd();
	
	//Wrappers for functions which panic
	void  CancelTimedSnapshot();
	void  GetTimedSnapshot1();
	void  GetTimedSnapshot2();
	void  GetSupportedSnapshotFormats();

	
	//Function for negative tests
	
	void NegTests();
	void InitializeCov1();
	void InitializeCov2();
	
	
	
	
private:
	CPlayVideoFileCov(RTestStepVideoCodecs* aParent);
	void ConstructL();	
	void StartDecodeL();
	void StartDecodePauseResumeL();
	void InitializeL();		
	void ReadNextBufferL();
	void OpenTestFileL();
    
private:
	//DevVideoPlay
	CMMFVideoDecodeHwDevice* iVideoHwDevice;
	
	TInt iError;
	
	// data section
	RFs iFs;
	RFile iFile;
	TPlayVideoFileState iState;	
	RTestStepVideoCodecs* iParent;
	TBool iStoppedReceived;
	TBool iUseDSA;
	
	TSize iFrameSize;
	CFbsScreenDevice* iScreenDevice;
	};

#endif // VIDEOPLAYFILE_COV_H


