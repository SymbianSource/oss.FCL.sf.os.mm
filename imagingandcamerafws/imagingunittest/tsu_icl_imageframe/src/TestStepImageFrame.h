
// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef TESTSTEPIMAGEFRAME_H
#define TESTSTEPIMAGEFRAME_H

#include <testframework.h>
#include <imageframe.h>

class TFrameFormat;
class TFrameLayout;
class CImageFrame;
class TDataUnit;

// class, which methods are used to provide various utility functionality
class RImageFrameUtil
	{
public:
    static TBool FormatsEqual(const TFrameFormat& aFormat1, const TFrameFormat& aFormat2);
    static TBool LayoutsEqual(const TFrameLayout& aLayout1, const TFrameLayout& aLayout2);
    static TBool CheckUniformAssignment(CImageFrame& aFrame, TInt aValue, TInt aBufferSize, TBool aForward);
	static void SetBlockToUniformValue(TDataUnit& aDataUnit, TInt aValue);
	static TBool CompareBlockValues(TDataUnit& aDataUnit1, TDataUnit& aDataUnit2);
	static void InitPointerValues(TDataUnit* aDataUnit, TDataUnit** aDataUnitPtr, TInt aBlocks);
    static TInt LayoutPlanes(const TFrameFormat& aFormat);
    static void InitLayoutValues(TFrameLayout& aLayout,TFrameFormat& aFormat, TSize& aSize, TInt& aRecommendedSize);
	static void InitPointerValuesL(TDataUnit* aDataUnit, RArray<const TDataUnit*> &aDataUnitPtr, TInt aBlocks);
};

// base class for all tests, it embeds the alloc tests step
class RImageFrameTest : public  RTestStep
	{
protected:
	// from RTestStep;
	TVerdict DoTestStepL();
    virtual TVerdict DoTestL();
    TVerdict ProbeAllocTestsL();
	RImageFrameTest(TBool aAllocTest);
    TInt LayoutPlanes(const TFrameFormat& aFormat);
    void InitLayoutValues(TFrameLayout& aLayout,TFrameFormat& aFormat, TSize& aSize, TInt& aRecommendedSize);

protected:
	TBool iAllocTest;
	};
	
class RImageFrameDecDesc : public  RImageFrameTest
	{
public:
	static RImageFrameDecDesc* NewL(TBool aAllocTest);
	
protected:
	// from RImageFrameTest;
	TVerdict DoTestL();

	RImageFrameDecDesc(TBool aAllocTest);
	};	
	
class RImageFrameDecChunk : public  RImageFrameTest
	{
public:
	static RImageFrameDecChunk* NewL(TBool aAllocTest);
protected:
	// from RImageFrameTest;
	TVerdict DoTestL();
private:
	RImageFrameDecChunk(TBool aAllocTest);
	};
	
class RImageFrameEncDesc : public RImageFrameTest 
	{
public:
	static RImageFrameEncDesc* NewL(TBool aAllocTest);
protected:
	// from RImageFrameTest;
	TVerdict DoTestL();
private:
	RImageFrameEncDesc(TBool aAllocTest);
	};
	
class RImageFrameEncChunk : public RImageFrameTest
	{
public:
	static RImageFrameEncChunk* NewL(TBool aAllocTest);
protected:
	// from RImageFrameTest;
	TVerdict DoTestL();
private:
	RImageFrameEncChunk(TBool aAllocTest);
	};	
	
class RYuvMonoProcDesc : public RImageFrameTest
	{
public:
	static RYuvMonoProcDesc* NewL(TBool aAllocTest);
protected:
	// from RImageFrameTest;
	TVerdict DoTestL();
private:
	RYuvMonoProcDesc(TBool aAllocTest);
	};

class RYuvMonoProcChunk : public RImageFrameTest
	{
public:
	static RYuvMonoProcChunk* NewL(TBool aAllocTest);
protected:
	// from RImageFrameTest;
	TVerdict DoTestL();
private:
	RYuvMonoProcChunk(TBool aAllocTest);
	};		

class RYuv422IntlProcDesc : public RImageFrameTest
	{
public:
	static RYuv422IntlProcDesc* NewL(TBool aAllocTest);
protected:
	// from RImageFrameTest;
	TVerdict DoTestL();
private:
	RYuv422IntlProcDesc(TBool aAllocTest);
	};

class RYuv422IntlProcChunk : public RImageFrameTest
	{
public:
	static RYuv422IntlProcChunk* NewL(TBool aAllocTest);
protected:
	// from RImageFrameTest;
	TVerdict DoTestL();
private:
	RYuv422IntlProcChunk(TBool aAllocTest);
	};		
	
//YUV420PlanarProcessor (Reversed) for descriptor test
class RYuv420PlnProcDesc : public RImageFrameTest
	{
public:
	static RYuv420PlnProcDesc* NewL(TBool aAllocTest);
protected:
	// from RImageFrameTest;
	TVerdict DoTestL();
private:
	RYuv420PlnProcDesc(TBool aAllocTest);
	};

//YUV420PlanarProcessor for chunk test
class RYuv420PlnProcChunk : public RImageFrameTest
	{
public:
	static RYuv420PlnProcChunk* NewL(TBool aAllocTest);
protected:
	// from RImageFrameTest;
	TVerdict DoTestL();
private:
	RYuv420PlnProcChunk(TBool aAllocTest);
	};	
	
//YUV420PlanarProcessor (Reversed) for descriptor test
class RYuv420PlnRevProcDesc : public RImageFrameTest
	{
public:
	static RYuv420PlnRevProcDesc* NewL(TBool aAllocTest);
protected:
	// from RImageFrameTest;
	TVerdict DoTestL();
private:
	RYuv420PlnRevProcDesc(TBool aAllocTest);
	};

//YUV420PlanarProcessor (Reversed) for chunk test
class RYuv420PlnRevProcChunk : public RImageFrameTest
	{
public:
	static RYuv420PlnRevProcChunk* NewL(TBool aAllocTest);
protected:
	// from RImageFrameTest;
	TVerdict DoTestL();
private:
	RYuv420PlnRevProcChunk(TBool aAllocTest);
	};		

// test to demostrate how all works when a descriptor or chunk contains more than one image frame - descriptor case	
class RImageFrameDescMultiFrame : public RTestStep
	{
public:
	static RImageFrameDescMultiFrame* NewL(TBool aAllocTest);
protected:
	// from RTestStep;
	TVerdict DoTestStepL();
private:
	RImageFrameDescMultiFrame(TBool aAllocTest);
	};

// test to demostrate how all works when a descriptor or chunk contains more than one image frame - chunk case
class RImageFrameRChunkMultiFrame : public RTestStep
	{
public:
	static RImageFrameRChunkMultiFrame* NewL(TBool aAllocTest);
protected:
	// from RTestStep;
	TVerdict DoTestStepL();
private:
	RImageFrameRChunkMultiFrame(TBool aAllocTest);
	};	

// test to verify the functionality of TFormatClass for all different recognised format codes
class RFormatTest : public RTestStep
	{
public:
	static RFormatTest* NewL(TBool aAllocTest);
protected:
	// from RTestStep;
	TVerdict DoTestStepL();
private:
	RFormatTest(TBool aAllocTest);
	};

// test to verify the clipping functionality of the image processor 
class RImageFrameClip : public RTestStep
	{
public:
	static RImageFrameClip* NewL(TBool aAllocTest);
protected:
	// from RTestStep;
	TVerdict DoTestStepL();
private:
	RImageFrameClip(TBool aAllocTest);
	};	
	

#endif // TESTSTEPIMAGEFRAME_H

