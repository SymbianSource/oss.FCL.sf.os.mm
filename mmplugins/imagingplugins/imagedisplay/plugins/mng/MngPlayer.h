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

#ifndef __MNGPLAYER_H__

#define __MNGPLAYER_H__
/** 	@file
	@internalTechnology */

#include "MngObjects.h"
#include "Providers.h"


class CMngPlayer;
class CMngProcessor;
class CPngProcessor;
class CMngFileReadStream;
class CMngRenderer;
class TMhdrChunk;
class CMngTermination;
class MMngPlayerObserver;
class MPixelFilter;
class MImageStreamDecoderFactory;
class MUniqueChunkDataProvider;

class RMngObjectList: public RPointerArray<CMngObject> 
	{
public:
		typedef TInt TObjectIterator;

		void Destroy();
		void AppendL(CMngObject*& aObject);

		TObjectIterator Find(TMngObjLabel aLabel);
		TObjectIterator Begin() const;
		TObjectIterator End() const;
private:
		
	};

class CDelayTimer: public CTimer 
	{
public:
	static CDelayTimer* NewL(CMngPlayer& aPlayer);
	void RunNow();
protected:
	inline CDelayTimer(CMngPlayer& aPlayer);
	void RunL();
	CMngPlayer& iPlayer;
	};

class TLoopStackItem 
	{
public:
	TInt			iNestLevel;
	TMngObjLabel	iLoopObjectLabel;
	TBool			iMoreIterationsNeeded;
	};

class RLoopStack: protected RArray<TLoopStackItem> 
	{
	friend class CMngPlayer;
public:
	inline void PushL(const TLoopStackItem& aItem);
	inline TInt Pop(TLoopStackItem& aItem);
	inline TBool IsEmpty() const;
	};

class TProviderEntry 
	{
public:
	inline TProviderEntry();
	inline TProviderEntry(const TChunkId& aId, MUniqueChunkDataProvider& aProvider);
											
	TChunkId					iId;
	MUniqueChunkDataProvider*   iProvider;	
	};

class RChunkProviders: protected RArray<TProviderEntry> 
	{
	friend class CMngPlayer;
	static TInt OrderFunc(const TProviderEntry& aLeft, const TProviderEntry& aRight);
	typedef TLinearOrder<TProviderEntry> TMyOrder;
public:
	TInt Add(const TProviderEntry& aItem);
	void RemoveProvider(const TChunkId& aId);
	MUniqueChunkDataProvider* Find(const TChunkId& aId);
	void Clear();
	};

class CMngPlayer: protected CActive, public MUniqueChunkDataProvider  
	{
public:

	typedef TUint TObserverNotifyEvent;

public:
		
	static CMngPlayer*	NewL(CMngFileReadStream* aReadStream, MMngPlayerObserver& aObserver, TBool aMngSubframesNoLoops);   //Subframes with No Loops;
	~CMngPlayer();

// external interface to the MngImageDisplayPlugin //
	void StopDecode();
	void Pause();
	void Decode();
	inline void SetInternalDelayOn(TBool aIsOn);
	/*
	Assumes passing of the ownership
	*/
	void SetMngHeader(TMhdrChunk*& aHeaderChunk); 
	inline TSize FrameSize() const;
	inline TInt NumFrames() const;

	inline TBool IsAlphaNeeded() const;
	
	inline void SetRequiredDisplayModes(TDisplayMode aDestDisplayMode, TDisplayMode aMaskDisplayMode);
	/**
		returns this frame delay, the value is valid only during observer callback
	*/
	inline TTimeIntervalMicroSeconds32 ThisFrameDelay() const;
// internal interface to MNG objects	//
	void InitL(TMhdrChunk*& aHeaderChunk);
	
	void AppendObjectL(CMngObject*& aObject);

	inline CMngRenderer* Renderer() const;

	void SetTerminationCtrlL(CMngTermination* aTermination);
	inline void SetTerminationFrameNum(TInt aFrameNumber);
	void SetPixelFilter(MPixelFilter* aPixelFilter);

	inline void SetFramingMode(TMngFramingMode	aFramingMode);
	void SetInterFrameDelay(TMngFramChangeNextFrameOption aOption, TInt aTicks);
	void SetLayerClipping(TMngFramChangeNextFrameOption aOption, const TRect& aClipRect, TBool aIsAdditive);
	
	void AddUniqueChunkProviderL(const TChunkId& aChunkId,  MUniqueChunkDataProvider& aProvider);

	inline RLoopStack& LoopStack();

	MImageStreamDecoderFactory& DecoderFactory();

// execution timer interface//
	void NotifyObserverGate();	

	//to set destination bitmap
	inline void SetDestinationBitmap(CFbsBitmap* aDestination);
	//to get destination bitmap
	inline CFbsBitmap* GetDestinationBitmap();
protected:
	CMngPlayer(MMngPlayerObserver& aObserver, TBool aMngSubframesNoLoops);   //Subframes with No Loops;

	void ConstructL(CMngFileReadStream* aReadStream);

// from the MUniqueChunkDataProvider //
	TInt GetChunkData(const TText8* aChunkId, const TUint8*& aPalettePtr, TInt& aSizeInBites);
// 

	void OnIteration();
	TBool DoIterationL();

	void OnEndOfSubFrame();
	void OnEndOfFrame(TBool aAlwaysNotify=EFalse, TBool aIsLastFrame=EFalse);
	void OnNewLayer();

	void DoCancel();
	void RunL();
	TInt RunError(TInt aError);
	void RunAgain();

	void StartPlay();

	void NotifyObserver(TObserverNotifyEvent aEvent, TInt aArgument, TTimeIntervalMicroSeconds32 aNotifyDelay);
	
// Utility functions //
	inline TTimeIntervalMicroSeconds32 Ticks2TimeInterval(TInt aTicks) const;

protected:
	TBool 				iMngSubframesNoLoops;   //Subframes with No Loops
	MMngPlayerObserver& iObserver;
	RMngObjectList		iObjectList;
	RLoopStack			iLoopStack;
	RChunkProviders		iChunkProviders;
	CMngFileReadStream*	iMngStream;
	CMngProcessor*		iMngProcessor;
	CMngRenderer*		iMngRenderer;
	CPngProcessor*		iPngProcessor;
	CMngTermination*	iTerminationCtrl;
	TMhdrChunk*			iMngHeader;
	TInt				iTickLength;
	CDelayTimer*		iExecTimer;
	TObserverNotifyEvent iNotifyEvent;
	TInt				iObserverNotifyArg;
	TTimeIntervalMicroSeconds32 iNominalDelayBeforeThisFrame;
	TBool				iPerformFrameDelayInternally;
	TBool				iIsPaused;
	TBool				iOutstandingNotify;
	TBool				iGotHeaderFromStream;
	TDisplayMode		iDestDisplayMode;
	TDisplayMode		iMaskDisplayMode;
// Mng operational parameters //
	TInt							iPlayerErrorCode;
	enum TPlayerState 
		{
		EStJustBorn=0, 
		EStReadingFormat, 
		EStReadyToPlay, 
		EStPlaying,
		EStStopped,
		EStNeedsRepeat,
		}							iState;
	TInt							iInterframeDelay;			// current effective interframe delay
	TInt							i1stFrameInterframeDelay;	// interframe delay fro the 1st frame of repeat
	TMngFramingMode					iFramingMode;
	TMngFramChangeNextFrameOption	iChangeInterFrmDelay;
	TInt							iNextInterFrameDelay;	
	TInt							iDefaultInterFrmDelay;		// default interframe delay
	TMngFramChangeNextFrameOption	iChangeClippingBoundaries;
	TRect							iCurrentClippingBnd;
	TRect							iDefaultClippingBnd;
	TMngFramChangeNextFrameOption	iChangeSyncIdList;
	RMngObjectList::TObjectIterator iCurrentObject;
	MImageStreamDecoderFactory*		iDecoderFactory;
	TInt							iFrameNumberToTerminate;
	TBool							iFrameBeingRendered;
// play statistics //
	TInt							iDrawnLayerNum;			// Layer number within the whole sequence
	TInt							iDrawnFrameNum;			// Frame number within the whole sequence
	TInt							iSubframeLayerNum;		// Layer number within the current subrame
	TTime							iEndOfPreviuosFrame;	// Time point when the previous frame has been completed
	TBool							iEndOfFrameNotification;
#ifdef _DEBUG
	TTime							iNotifyStart;
	TTime							iLastNotify;
#endif
private:
	TBool							iNeedsChange;
	CFbsBitmap*	iCurrentFrame; //not owned
	};

#include "MngPlayer.inl"

#endif // ndef __MNGPLAYER_H__
