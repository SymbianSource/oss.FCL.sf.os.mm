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

#ifndef TSI_ICL_PLUGINSUPPORT_010_STEP_H
#define TSI_ICL_PLUGINSUPPORT_010_STEP_H

#include <test/testexecuteserverbase.h>
#include "te_tsi_icl_pluginsupport_01suitestepbase.h"

class Ctsi_icl_pluginsupport_010Step : 	public CTe_tsi_icl_pluginsupport_01SuiteStepBase,
								 		public ImageProcessor::MImgProcessorObserver
	{
public:
	Ctsi_icl_pluginsupport_010Step();
	~Ctsi_icl_pluginsupport_010Step();
	virtual TVerdict doTestStepPreambleL();
	virtual TVerdict doTestStepL();
	virtual TVerdict doTestStepPostambleL();

	// From MImgProcessorObserver
	void ImageProcessorInitializingComplete(ImageProcessor::CImgProcessor& aImageProcessor, TInt aError);
	void ImageProcessorPreviewInitializingComplete(ImageProcessor::CImgProcessor& aImageProcessor, TInt aPreviewId, TInt aError);
	void ImageProcessingComplete(ImageProcessor::CImgProcessor& aImageProcessor, TInt aError);
	void ImageProcessorPreviewRenderingComplete(ImageProcessor::CImgProcessor& aImageProcessor, TInt aPreviewId, TInt aError);
	void ImageProcessorEvent(ImageProcessor::CImgProcessor& aImageProcessor, TInt aEventId, TUid aUid, TInt aId);

private:
	void PrintCallbackBehaviour();

private:
	CActiveScheduler* iActiveScheduler;
	RFs iFs;
	};

_LIT(Ktsi_icl_pluginsupport_010step,"tsi_icl_pluginsupport_010Step");

#endif // TSI_ICL_PLUGINSUPPORT_010_STEP_H
