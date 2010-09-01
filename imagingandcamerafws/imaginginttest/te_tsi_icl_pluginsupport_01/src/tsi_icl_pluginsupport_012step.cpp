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

#include "tsi_icl_pluginsupport_012step.h"
#include "te_tsi_icl_pluginsupport_01suitedefs.h"

_LIT(KSourceFilename, "c:\\te_tsi_icl_pluginsupport_01\\imagetransform_source.jpeg");
_LIT(KDestinationFilename, "c:\\te_tsi_icl_pluginsupport_01\\imagetransform_destination.jpeg");

Ctsi_icl_pluginsupport_012Step::~Ctsi_icl_pluginsupport_012Step()
/**
 * Destructor
 */
	{
	delete iActiveScheduler;
	iFs.Close();
	}

Ctsi_icl_pluginsupport_012Step::Ctsi_icl_pluginsupport_012Step()
/**
 * Constructor
 */
	{
	SetTestStepName(Ktsi_icl_pluginsupport_012step);
	}

TVerdict Ctsi_icl_pluginsupport_012Step::doTestStepPreambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{
	iActiveScheduler = new (ELeave) CActiveScheduler;
	CActiveScheduler::Install(iActiveScheduler);
	
	User::LeaveIfError(iFs.Connect());

	SetTestStepResult(EPass);
	return TestStepResult();
	}


TVerdict Ctsi_icl_pluginsupport_012Step::doTestStepL()
/**
 * @return - TVerdict code
 * Override of base class pure virtual
 * Our implementation only gets called if the base class doTestStepPreambleL() did
 * not leave. That being the case, the current test result value will be EPass.
 */
	{
	if (TestStepResult()==EPass)
		{
		INFO_PRINTF1(_L("*** Simple instantiation test for CImageTransform plugin extensions ***"));

#ifdef ADAPTATION_PLUGINS_PRESENT

		INFO_PRINTF1(_L("\tTest .mmp macro ADAPTATION_PLUGINS_PRESENT defined"));

#else // ADAPTATION_PLUGINS_PRESENT

		INFO_PRINTF1(_L("\tTest .mmp macro ADAPTATION_PLUGINS_PRESENT not defined"));

#endif // ADAPTATION_PLUGINS_PRESENT		
		
		__UHEAP_MARK;
		
		CImageTransform* imageTransform = NULL;
		TRAPD(err, imageTransform = CImageTransform::NewL(iFs));
		if(err != KErrNone)
			{
			INFO_PRINTF2(_L("\tA CImageTransform::NewL() failed with error code %d."), err);
			User::Leave(err);
			}
		
		CleanupStack::PushL(imageTransform);
	
		RArray<CImageTransform::TTransformations> transformationsArray;
		CleanupClosePushL(transformationsArray);
		
		transformationsArray.AppendL(CImageTransform::ESqueeze);
		transformationsArray.AppendL(CImageTransform::EOrientation);
		transformationsArray.AppendL(CImageTransform::EOverlay);
				
		for(TInt i = 0; i < transformationsArray.Count(); i++)
			{
			TRAP(err, imageTransform->SetSourceFilenameL(KSourceFilename));
			if(err != KErrNone)
				{
				INFO_PRINTF2(_L("\tA CImageTransform::SetSourceFilenameL() failed with error code %d."), err);
				User::Leave(err);
				}
		
			TRAP(err, imageTransform->SetDestFilenameL(KDestinationFilename));
			if(err != KErrNone)
				{
				INFO_PRINTF2(_L("\tA CImageTransform::SetDestFilenameL() failed with error code %d."), err);
				User::Leave(err);
				}	
									
			TRAP(err, imageTransform->SetTransformationsL(transformationsArray[i]));
			if(err != KErrNone)
				{
				INFO_PRINTF2(_L("\tA CImageTransform::SetTransformationsL() failed with error code %d."), err);
				User::Leave(err);
				}
			
			TRAP(err, imageTransform->SetupL());
			if(err != KErrNone)
				{
				INFO_PRINTF2(_L("\tA CImageTransform::SetupL() failed with error code %d."), err);
				
#ifdef ADAPTATION_PLUGINS_PRESENT
				
				INFO_PRINTF1(_L("\t\tThis is UNEXPECTED behaviour with this configuration (see the macro: ADAPTATION_PLUGINS_PRESENT)."));
				User::Leave(err);
			
#else // ADAPTATION_PLUGINS_PRESENT

				INFO_PRINTF1(_L("\t\tThis is EXPECTED behaviour with this configuration (see the macro: ADAPTATION_PLUGINS_PRESENT)."));
				// PASS!

#endif // ADAPTATION_PLUGINS_PRESENT
				}
			else
				{
				INFO_PRINTF1(_L("\tA CImageTransform::SetupL() succeeded."));
				
#ifdef ADAPTATION_PLUGINS_PRESENT

				INFO_PRINTF1(_L("\t\tThis is EXPECTED behaviour with this configuration (see the macro: ADAPTATION_PLUGINS_PRESENT)."));
				
				const TUid KSqueezeExtensionUid 	= {KUidSqueezeTransformExtension};
				const TUid KOrientationExtensionUid	= {KUidOrientationTransformExtension};
				const TUid KOverlayExtensionUid 	= {KUidOverlayTransformExtension};
				
				CImageTransformPluginExtension* pluginExtension = NULL;
			
				switch(transformationsArray[i])
					{
					case CImageTransform::ESqueeze:
						INFO_PRINTF1(_L("\t\tSqueeze plugin extension."));
						pluginExtension = 
							static_cast<CSqueezeTransformExtension*>(imageTransform->Extension(KSqueezeExtensionUid, err));
						break;
					case CImageTransform::EOrientation:
						INFO_PRINTF1(_L("\t\tOrientation plugin extension."));
						pluginExtension = 
							static_cast<CSqueezeTransformExtension*>(imageTransform->Extension(KOrientationExtensionUid, err));
						break;
					case CImageTransform::EOverlay:
						INFO_PRINTF1(_L("\t\tOverlay plugin extension."));
						pluginExtension = 
							static_cast<COverlayTransformExtension*>(imageTransform->Extension(KOverlayExtensionUid, err));
						break;
					default:
						INFO_PRINTF1(_L("\t\tUnsupported TTransformations type"));
						User::Leave(KErrNotSupported);
					}
				
				if(pluginExtension && err == KErrNone)
					{
					INFO_PRINTF1(_L("\t\t\tThe plugin extension was obtained successfully."));
					INFO_PRINTF1(_L("\t\t\tThis is EXPECTED behaviour with this configuration (see the macro: ADAPTATION_PLUGINS_PRESENT)."));
					SetTestStepResult(EPass);
					}
				else
					{
					INFO_PRINTF1(_L("\t\t\tThe plugin extension was not obtained successfully."));
					INFO_PRINTF1(_L("\t\t\tThis is UNEXPECTED behaviour with this configuration (see the macro: ADAPTATION_PLUGINS_PRESENT)."));
					SetTestStepResult(EFail);
					}
					
#else // ADAPTATION_PLUGINS_PRESENT

				INFO_PRINTF1(_L("\t\t\tThis is UNEXPECTED behaviour with this configuration (see the macro: ADAPTATION_PLUGINS_PRESENT)."));
				User::Leave(KErrGeneral);

#endif // ADAPTATION_PLUGINS_PRESENT
				}		
			
			imageTransform->Reset(); // reset for next plugin
			}
		
		CleanupStack::PopAndDestroy(&transformationsArray);
		CleanupStack::PopAndDestroy(imageTransform);
		imageTransform = NULL;
		
		REComSession::FinalClose();
		
		__UHEAP_MARKEND;
		}

	return TestStepResult();
	}


/**
 * @return - TVerdict code
 * Override of base class virtual
 */
TVerdict Ctsi_icl_pluginsupport_012Step::doTestStepPostambleL()
	{
	return TestStepResult();
	}

