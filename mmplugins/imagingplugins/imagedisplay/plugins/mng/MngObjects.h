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

#ifndef __MNGOBJECTS_H__

#define __MNGOBJECTS_H__
/** 	@file
	@internalTechnology */

#include <gdi.h>

#include "PixelFilter.h"
#include "Providers.h"
#include "MngChunks.h"
#include "ColourDefs.h"
#include "PixelConsumer.h"

class CMngPlayer;

typedef TInt TMngObjLabel;

const TMngObjLabel KFirstObjectLabel=1;


enum TMngExecCommand 
	{
	ECmdProcessNext=0,
	ECmdJumpToObject,
	ECmdStartFromBeginning,
	ECmdTerminate,
	ECmdTerminateClearOutput,
	ECmdTerminateAfter1stFrame,
	};

class TMngExecuteResult 
	{
public:
	inline TMngExecuteResult();

public:
	TMngExecCommand iCommand;
	TMngObjLabel	iObjectLabel;
	TInt			iDelayInTicks;
	};

class CMngObject:public CBase 
	{
public:
	
	inline TMngObjLabel ObjectLabel() const;
	inline void SetObjectLabel(const TMngObjLabel& aLabel);
// Object Acess interface //
	virtual void  ExecuteL(CMngPlayer& aPlayer, TMngExecuteResult& aResult)=0;
	virtual TBool IsSubFrame() const;
	virtual TBool IsLayer() const;
	virtual void OnAddToPlayerL(const CMngPlayer& aPlayer);
	virtual void Reset();
protected:
	TMngObjLabel iObjectLabel;
	};


class CMngTermination:public CMngObject 
	{
public:
	static CMngTermination* NewL(TTermChunk*& aTermChunk, TBool aMngSubframesNoLoops);  //Subframes with No Loops
	virtual void ExecuteL(CMngPlayer& aPlayer, TMngExecuteResult& aResult);
	virtual void Reset();
protected:
	inline explicit CMngTermination(const TTermChunk& aTermChunk, TBool aMngSubframesNoLoops);  //Subframes with No Loops
protected:
	TBool 					iMngSubframesNoLoops;  //Subframes with No Loops
	TMngTerminationAction	iTerminationAction;
	TMngTerminationAction	iPostIterationAction;
	TInt					iDelay;				// delay in ticks
	TInt					iIterationMax;
	TInt					iIterationCounter;
	};

class CMngKeyFrame:public CMngObject 
	{
public:
	static CMngObject* NewL(TMngChunk*& aFramChunk);
	~CMngKeyFrame();
	virtual void  ExecuteL(CMngPlayer& aPlayer, TMngExecuteResult& aResult);
	virtual TBool IsSubFrame() const;

protected:
	explicit CMngKeyFrame(TFramChunk* aFramChunk);
protected:
	TFramChunk*	iFramChunk;
	};

class CMngBackground:public CMngObject 
	{
public:
	static CMngObject* NewL(TMngChunk*& aBackChunk);
	virtual void  ExecuteL(CMngPlayer& aPlayer, TMngExecuteResult& aResult);
		
protected:
	inline explicit CMngBackground(const TBackChunk& aBackChunk);
protected:
	TRgb					iBackgroundColour;
	TMngBackroundMandatory	iBgMandatory;
	};


class CMngRawChunkHolder:public CMngObject, public MUniqueChunkDataProvider 
	{
public:
	static CMngObject* NewL(TMngChunk*& aRawChunk);
	~CMngRawChunkHolder();

	virtual void  ExecuteL(CMngPlayer& aPlayer, TMngExecuteResult& aResult);

	TInt GetChunkData(const TText8* aChunkId, const TUint8*& aDataPtr, TInt& aSizeInBites);

protected:
	inline explicit CMngRawChunkHolder(TMngRawChunk* aPaltChunk);

protected:
	TMngRawChunk* const	iChunk;
	};


class CMngImageDefinition:public CMngObject 
	{
public:
	static CMngObject* NewL(TMngChunk*& aDefiChunk);
	virtual void  ExecuteL(CMngPlayer& aPlayer, TMngExecuteResult& aResult);
	void OnAddToPlayerL(const CMngPlayer& aPlayer);

protected:
	inline explicit CMngImageDefinition(const TDefiChunk& aDefiChunk);

protected:
	TBool		iDoNotShow;
	TPoint		iOrigin;
	TRect		iClippingRect;
	};

class CMngMagnDefinition:public CMngObject, public MPixelFilter 
	{
private:
	enum TBorderIdx
		{
		EBorder1st=0,
		EBorderMiddle,
		EBorderPreLast,
		EBorderLast
		};
public:
	static CMngObject* NewL(TMngChunk*& aMagnChunk);
	~CMngMagnDefinition();
	virtual void  ExecuteL(CMngPlayer& aPlayer, TMngExecuteResult& aResult);

// from the MPixelFilter // 
	virtual void PrepareL(const TSize& aOriginalSize,CRgbPixelConsumer* aConsumer);
	virtual void Commit();
	virtual void SetPixel(const TPoint& aPosition, TUint aPixelColour);

	virtual void AddRef();
	virtual void Release();

protected:
	explicit CMngMagnDefinition(TMagnChunk& aMagnChunk);

	void CommitSimpleRepeat();
	void CommitInterpolation();

	inline TInt XRepeat(TInt aX) const;
	inline TInt YRepeat(TInt aY) const;
	
	inline TInt	RealPosX(TInt aSrcX) const;
	inline TInt	RealPosY(TInt aSrcY) const;
	
protected:
	TMagnChunk*	iMagn;
	TRgbaColour* iColorBuffer;
	TSize iImageSize;
	TInt iXRepeat[EBorderLast+1];
	TInt iYRepeat[EBorderLast+1];
	CRgbPixelConsumer* iConsumer;
	CRgbPixelConsumer::SetPixelFunc iSetPixelFunction; // function pointer to either SetRGBPixel or SetRGBAPixel
	TInt iRefCount;
	};

class CMngLoopEntry:public CMngObject 
	{
public:
	static CMngObject* NewL(TMngChunk*& aLoopChunk);
	virtual void ExecuteL(CMngPlayer& aPlayer, TMngExecuteResult& aResult);
	void Reset();

protected:
	inline explicit CMngLoopEntry(const TLoopChunk& aLoopChunk);

protected:
	TInt	iNestLevel;
	TInt	iIterationCount;
	TInt	iIterationsLeft;
	};

class CMngLoopEnd:public CMngObject 
	{
public:
	static CMngObject* NewL(TMngChunk*& aLoopChunk);
	virtual void ExecuteL(CMngPlayer& aPlayer, TMngExecuteResult& aResult);

protected:
	inline explicit CMngLoopEnd(const TEndlChunk& aLoopChunk);

protected:
	TInt	iNestLevel;
	};


class CMngLayer:public CMngObject 
	{
public:
	virtual TBool IsLayer() const;
	};


#include "MngObjects.inl"

#endif //ndef __MNGOBJECTS_H__

