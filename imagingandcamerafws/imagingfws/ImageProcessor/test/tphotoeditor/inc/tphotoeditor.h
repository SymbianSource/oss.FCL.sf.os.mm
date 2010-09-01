/**
* Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:
*
*/



#ifndef TPHOTOEDITOR_H
#define TPHOTOEDITOR_H

#include <eikappui.h>
#include <eikapp.h>
#include <eikdoc.h>
#include <techview/eikinfo.h>
#include <techview/eikcapc.h>
#include <imageprocessor/imageprocessor.h>
#include <imageprocessor/imageprocessorobserver.h>
#include <imageprocessor/imageprocessorpreview.h>
#include <imageprocessor/imageprocessoreffect.h>
#include <imageprocessor/imageprocessoroverlay.h>
#include <imageprocessor/imageprocessorinputinfo.h>
#include <imageprocessor/imageprocessoroutputinfo.h>
#include <imageprocessor/imageprocessorprogressinfo.h>
#include <panorama/panorama.h>

#include "tphotoeditordialogs.h"
#include <tphotoeditor.rsg>
#include "tphotoeditor.hrh"


//#define __PHOTOEDIT_PROFILING
#define __ENABLE_DEBUG_OUTPUT

const TUid KUidTPhotoEditor = { 0x10285E58 }; 

const TInt KMainPreviewId = 0;
const TInt KNumberOfMainPreviews = 1;
const TInt KFirstThumbnailId = KMainPreviewId + KNumberOfMainPreviews;
const TInt KShortestPossiblePath = 4;
const TDisplayMode KDefaultDisplayMode = EColor16MA;

#ifdef __WINS__
	const TSize KThumbnailSize(80, 60);
	const TInt KHorizontalSpacing = 20;
	const TInt KMinimumThumbnailToLeftBorderDistance = 20;
#else
	const TSize KThumbnailSize(80, 60);
	const TInt KHorizontalSpacing = 15;
	const TInt KMinimumThumbnailToLeftBorderDistance = 15;
#endif

const TReal KAspectRatioWidth = 4;	
const TReal KAspectRatioHeight = 3;	
const TInt KVerticalSpacing = 10;
const TInt KMaxNumberOfFilesInFolder = 1000;

//only for test image.content purpose
_LIT(KRightsUniqueId001, "ICLTestjfif444Id001"); // Rights count 5
_LIT(KDefPath, "C:\\");

//Currently supports these 4 input formats
_LIT(KJPGFormat, "jpg");
_LIT(KPNGFormat, "png");
_LIT(KGIFFormat, "gif");
_LIT(KWBMPFormat, "wbmp");

//possible output formats
_LIT(KBMPFormat, "bmp");
_LIT(KCUSTOMFormat, "custom");


static const TReal32 KDefaultZoom = 1.0f;
static const TReal32 KZoomStep = 0.3f;
static const TReal32 KDefaultPanX = 0.0f;
static const TReal32 KDefaultPanY = 0.0f;
static const TReal32 KPanStep = 0.1f;

class TPreviewData
	{
public:
	TPreviewData();
	~TPreviewData();
	TBool IsInitialized();
	void AsyncRenderL();
	void SyncRenderL();
		
public:
	ImageProcessor::TPreview* iPreview;	// Not owned.
	CFbsBitmap* iBitmap;
	TPoint iLocation;
	TReal32 iZoomFactor;
	TReal32 iPanX;
	TReal32 iPanY;
	HBufC* iFileName;	// Not owned.
	TInt iPreviewId;
	};




//
// CTPhotoEditorAppView for creating a window and to draw the text
//
class CTPhotoEditorAppView : public CCoeControl
    {    
public:
	enum TZoomMode
		{
		EZoomReset,
		EZoomIn,
		EZoomOut
		};
	
	enum TPanMode
		{
		EPanReset,
		EPanUp,
		EPanDown,
		EPanLeft,
		EPanRight
		};

	virtual ~CTPhotoEditorAppView();
	
	// Whitens a portion of the screen.
	void ClearBitmap(const TPoint& aPos, const TSize& aSize);
	
	// Blits a bitmap to the screen.
	void DrawBitmap(CFbsBitmap* aBitmap, const TPoint* aPos);
	
	// Blits a preview to the screen.
	void DrawBitmap(TPreviewData* aData);
	
	// Draws a rectangle on the screen. TODO: This is specific to Gallery?
	void DrawRectangle(const TPoint& aPos, const TSize& aSize, const TBool aClear) const;

protected:
	void AppendPreviewL(ImageProcessor::TPreview* aPreview, CFbsBitmap* aBitmap, const TPoint& aLocation, TInt aPreviewId);
	virtual void PreviewBlittedL(TInt /*aPreviewId*/) {}
	
public:
	void BlitPreviewToScreenL(TInt aPreviewId);
	virtual void PreviewInitializedL(TInt aPreviewId);
	virtual void PreviewRenderedL(TInt aPreviewId);
	virtual void RefreshMainPreviewL(TBool aClearFirst = EFalse);
	void RedrawPreviewL(TPreviewData* aPreviewData, TBool aClearFirst = EFalse);
	RPointerArray<TPreviewData> GetPreviewsL();	
		
	virtual void NavigateL(TBool /*aForward*/)
		{
		User::Leave(KErrNotSupported);
		}
	
	void ZoomL(TZoomMode aDirection, TBool aRedraw = ETrue);
	void PanL(TPanMode aPanMode, TBool aRedraw = ETrue);
	void ReFreshScreenL(TBool aClearFirst = EFalse);
	
protected:
	CTPhotoEditorAppView(const TSize& aScreenSize);
	void ConstructL(const TRect& aRect);
	void Draw(const TRect& /*aRect*/) const;
	void CheckProcessingMode();
	
public:
	TSize& ScreenSize();
	void ClearScreen();

protected:
	TInt iNumberOfPreviews;			// The number of previews that will be drawn.
	TInt iMainPreviewIndex;
	TBool iAsync;
	ImageProcessor::CImgProcessor* iImageProcessor;	// Not owned.
	RPointerArray<TPreviewData> iPreviews;
	TInt iNavigationOffset;
	TReal32 iZoomFactor;

private:
	HBufC* iText;
	TSize iScreenSize;
	TPreviewData *iData;
	};

//
//
//
class CTPhotoEditorFileView : public CTPhotoEditorAppView
	{
public:
	static CTPhotoEditorFileView* NewL(const TRect& aRect, const TSize& aScreenSize);
	~CTPhotoEditorFileView();
	void LoadFileL(const TDesC& aFile, ImageProcessor::CImgProcessor* aImageProcessor);
	void LoadFileL(TMMFileSource& aSource, ImageProcessor::CImgProcessor* iImageProcessor);

private:
	CTPhotoEditorFileView(const TSize& aScreenSize);
	void ReSetImageProcessor(ImageProcessor::CImgProcessor* aImageProcessor);
	void ConstructL(const TRect& aRect);
	void DoLoadFileL(const TDesC& aFileName);

	HBufC* iFileName;
	};

//
//
//
class CTPhotoEditorGalleryView : public CTPhotoEditorAppView
	{
public:
	enum TRenderMode
		{
		ERenderModeNone,
		ERenderModeGallery,
		ERenderModeNavigate
		};
	
	static CTPhotoEditorGalleryView* NewL(const TRect& aRect, const TSize& aScreenSize);
	~CTPhotoEditorGalleryView();
	void LoadGalleryL(RPointerArray<HBufC>& aFiles, ImageProcessor::CImgProcessor& aProcessor);
	void PreviewBlittedL(TInt aPreviewId);
	void NavigateL(TBool aForward);
	void SetSavedFilesConfigurePreviewL(RPointerArray<HBufC>& aFiles, const TDesC& aSavedImage);
	void ConfigurePreviewsL(TBool aNavigateForward = EFalse);
	void DrawNextPreviewL();
			
	void Cancel()
		{
		RDebug::Printf("Cancelling");
		
		for (TInt i = 0; i < iNumberOfPreviews; i++)
			{
			iPreviews[i]->iPreview->Cancel();
			}
		}
	
private:
	CTPhotoEditorGalleryView(const TSize& aScreenSize);
	void ConstructL(const TRect& aRect);
	HBufC* GetFileAt(TInt aIndex);


	TInt iMaxNumberOfPreviews;		// The number of previews the screen can realistically show.
	TSize iMainPreviewSize;
	RArray<TPoint> iCanvasPoints;	// Where to draw the previews on the screen.
	RPointerArray<HBufC>* iFiles;	// Not owned.
	TInt iCurrentFile;				// Position in the iFiles array.
	TInt iBitmapToUse;				// The bitmap index that the preview will be rendered to.
	TRenderMode iMode;
	HBufC* iSavedImage;
	TBool iImageSaved;
	};

//
// CTPhotoEditorAppUi handles the system events and menu events.
//
class CTPhotoEditorAppUi : public CEikAppUi, ImageProcessor::MImgProcessorObserver
    {
public:
	/**
	Flags for the view type 
	*/
	enum TViewType
		{
		EViewFile,
		EViewGallery
		};
	
    void ConstructL();
	~CTPhotoEditorAppUi();

public:
	// Callbacks via MImgProcessorObserver interface.
	void ImageProcessorInitializingComplete(ImageProcessor::CImgProcessor& aImageProcessor, TInt aError);
	void ImageProcessorPreviewInitializingComplete(ImageProcessor::CImgProcessor& aImageProcessor, TInt aPreviewId, TInt aError);
	void ImageProcessingComplete(ImageProcessor::CImgProcessor& aImageProcessor, TInt aError); 
	void ImageProcessorPreviewRenderingComplete(ImageProcessor::CImgProcessor& aImageProcessor, TInt aPreviewId, TInt aError);
	void ImageProcessorEvent(ImageProcessor::CImgProcessor& aImageProcessor, TInt aEventId, TUid aUid, TInt aId);	
	void ClearBitmap(const TPoint& aPos, const TSize& aSize);
	void ReSetImageProcessorL();

private:
	inline TBool IsFileView()
		{
		return (iAppView && (iAppView == iFileView));
		}
	
	inline TBool IsGalleryView()
		{
		return (iAppView && (iAppView == iGalleryView));
		}
	
	void SaveWorkingDirectoryL(const TDesC& aDir);
	
	void ErrorMessage(const TDesC& aMessage, TInt aError);
	void DynInitMenuPaneL(TInt aResourceId, CEikMenuPane* aMenuPane);
	TBool CheckHotKeyNotDimmedL(TInt aCommandId);
	
	// Inherited from class CEikAppUi for handling menu events
	void HandleCommandL(TInt aCommand);

	// From CCoeAppUi to handle system events
	void HandleSystemEventL(const TWsEvent& aEvent);	
	void ExecuteDialogL(CEikDialog* dialog, TInt aDialogId, ImageProcessor::TEffect& aEffect);

public:
	void RefreshL(TBool aClearFirst = EFalse);
	void DimMenuItems(CEikMenuPane* aMenuPane, const TInt aFromCmd, const TInt aToCmd);
	void OpenFileL();
	void OpenDRMFileL();
	void OpenGalleryL();
	void FilterFolderL(const TDesC& aFolderName);
	void QuerySaveChangesL();
	void SaveAsL();
	void InsertFrameL();
	void InsertClipartL();
	void InsertDRMClipartL();
	void Cancel();
	void UndoL();
	void UndoAllL();
	void DoNavigateL(TBool aFlag);
	
	// View
	void SwitchViewL(TInt aViewId = KErrNone);
	
	// Geometrical Operation
	static ImageProcessor::CImgProcessor::TOperation CommandToOperation(TInt aCommand, TInt& aResourceId);
	void ExecuteOperationL(TInt aCommand);
	
	// Effects
	template<class T>
	void ApplyEffectL(TUid aUid, TInt aResourceId = KErrNotFound);
	void ExecuteEffectL(TInt aCommand);
	void ApplyNumberInputEffectL(TInt aCommand);
	void ApplyModeInputEffectL(TInt aCommand);

	void FillParameterListL(CDesCArrayFlat& aList, TInt aMin, TInt aMax);
private:
	void CartoonL();
	void RotationL();
	void RgbColorAdjustL();
	void BlurL();
	void ColorizationL();
	void SpotlightL();
	void StampL();
	void SketchL();
	void CustomTransformationL();
	void LevelsL();
	void WhiteBalanceL();
	void ReDrawAllPreviewL();
		   
private:
	TViewType iViewType;
	
	CTPhotoEditorAppView* iAppView;	// Not to be deleted.
	CTPhotoEditorFileView* iFileView;
	CTPhotoEditorGalleryView* iGalleryView;
	
	TSize iScreenSize;
	TSize iMainPreviewSize;
	TInt iNumberofPreviews;
	TInt iNumberofDisplayedPreviews;
	
	ImageProcessor::CImgProcessor* iImageProcessor;
	TBool iIsGalleryView;
	
	ImageProcessor::TProgressInfo* iProgressInfo;
	TInt iProgress;
	
	RPointerArray<HBufC> iFolderFiles;
	HBufC* iRootDirectory;
	TBool isImageSaved;
	TBool isNavigate;
	
#if defined(__PHOTOEDIT_PROFILING)
	protected:
		TUint iProcessStart;
		TUint iProcessEnd;
		TBool iFlag;
#endif
    };


//
// CTPhotoEditorDocument for creating the application UI
//
class CTPhotoEditorDocument : public CEikDocument
	{
public:
	static CTPhotoEditorDocument* NewL(CEikApplication& aApp);
	CTPhotoEditorDocument(CEikApplication& aApp);
	void ConstructL();
	
private: 
	// Inherited from CEikDocument for creating the AppUI
	CEikAppUi* CreateAppUiL();
	};


//
// CTPhotoEditorApplication creates a new instance of the document associated with this application
//
class CTPhotoEditorApplication : public CEikApplication
	{
private: 
	CApaDocument* CreateDocumentL();
	TUid AppDllUid() const;
	};	   
	
#endif
