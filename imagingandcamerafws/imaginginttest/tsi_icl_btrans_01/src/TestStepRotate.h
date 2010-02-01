// Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// This file contains Test step implementations for
// the CBitmapRotator class.
//

#if (!defined __TESTSTEP_ROTATE_H__)
#define __TESTSTEP_ROTATE_H__

#include <bitmaptransforms.h>




/**
 *
 * CMmIclBtransI0201Cp
 * Rotate a CFbsBitmap by 90 degrees, REPLACING the original bitmap 
 * with the result, and verify the result matches a reference bitmap
 * @lib "TSI_ICL_BTRANS_01.lib"
 *
 */
class CMmIclBtransI0201Cp : public CTestStepBitmapTransforms
	{
public:
	CMmIclBtransI0201Cp(TBool aAllocTest);

	//From CTestStepBitmapTransforms
	void Setup();
	TVerdict TestStepL();
	};

/**
 *
 * CMmIclBtransI0202Cp
 * Rotate a CFbsBitmap by 180 degrees, REPLACING the original bitmap 
 * with the result, and verify the result matches a reference bitmap
 * @lib "TSI_ICL_BTRANS_01.lib"
 *
 */
class CMmIclBtransI0202Cp : public CTestStepBitmapTransforms
	{
public:
	CMmIclBtransI0202Cp(TBool aAllocTest);

	//From CTestStepBitmapTransforms
	void Setup();
	TVerdict TestStepL();	
	};

/**
 *
 * CMmIclBtransI0203Cp
 * Rotate a CFbsBitmap by 270 degrees, REPLACING the original bitmap 
 * with the result, and verify the result matches a reference bitmap
 * @lib "TSI_ICL_BTRANS_01.lib"
 *
 */
class CMmIclBtransI0203Cp : public CTestStepBitmapTransforms
	{
public:
	CMmIclBtransI0203Cp(TBool aAllocTest);

	//From CTestStepBitmapTransforms
	void Setup();
	TVerdict TestStepL();	
	};

/**
 *
 * CMmIclBtransI0204Cp
 * Rotate a CFbsBitmap by 90 degrees, KEEPING the original bitmap 
 * with the result, and verify the result matches a reference bitmap
 * @lib "TSI_ICL_BTRANS_01.lib"
 *
 */
class CMmIclBtransI0204Cp : public CTestStepBitmapTransforms
	{
public:
	CMmIclBtransI0204Cp(TBool aAllocTest);

	//From CTestStepBitmapTransforms
	void Setup();
	TVerdict TestStepL();
	};



/**
 *
 * CMmIclBtransI0301Cp
 * Attempt to rotate a CFbsBitmap which has not been initialised or contains nothing. 
 * Verify the correct error code is returned and the original bitmap not changed
 * This step checks a bitmap that has been new'd but not Created(), it should panic with ENoSourceBitmap
 * @lib "TSI_ICL_BTRANS_01.lib"
 *
 */
class CMmIclBtransI0301Cp : public CTestStepBitmapTransforms
	{
public:
	CMmIclBtransI0301Cp(TBool aAllocTest);

	//From CTestStepBitmapTransforms
	TVerdict TestStepL();
	};


/**
 *
 * CMmIclBtransI0302Cp
 * Attempt to rotate a CFbsBitmap which contains corrupt data or data which is not 
 * in CFbsBitmap format. Verify the correct error code is returned and the original 
 * bitmap not changed
 *
 * @lib "TSI_ICL_BTRANS_01.lib"
 *
 */
class CMmIclBtransI0302Cp : public CTestStepBitmapTransforms
	{
public:
	CMmIclBtransI0302Cp(TBool aAllocTest);

	//From CTestStepBitmapTransforms
	void Setup();
	TVerdict TestStepL();
	};



	
/**
 *
 * CMmIclBtransI0303Hp
 * Attempt to rotate a CFbsBitmap which has not been initialised or contains nothing. 
 * Verify the correct error code is returned and the original bitmap not changed
 * @lib "TSI_ICL_BTRANS_01.lib"
 *
 */
class CMmIclBtransI0303Hp : public CTestStepBitmapTransforms
	{
public:
	CMmIclBtransI0303Hp(TBool aAllocTest);
	TVerdict TestStepL();
	};


/**
 *
 * CMmIclBtransI0305Cp
 * Attempt to rotate a CFbsBitmap which has not been initialised or contains nothing. 
 * Verify the correct error code is returned and the original bitmap not changed
 * This step checks a bitmap that has been Created() with 0 size
 * @lib "TSI_ICL_BTRANS_01.lib"
 *
 */
class CMmIclBtransI0305Cp : public CTestStepBitmapTransforms
	{
public:
	CMmIclBtransI0305Cp(TBool aAllocTest);

	//From CTestStepBitmapTransforms
	void Setup();
	TVerdict TestStepL();	
	};


#endif //(__TESTSTEP_ROTATE_H__)
