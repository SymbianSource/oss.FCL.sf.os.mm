// Copyright (c) 2001-2009 Nokia Corporation and/or its subsidiary(-ies).
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
 * TestStepRotate
 * This class provides a test step to exercise the mirror and rotate code
 * It has only the objective of not crashing
 *
 * @lib "TS_BitmapTransforms.lib"
 *
 *
 */
class CTestStepRotate : public CTestStepBitmapTransforms
	{
public:
		CTestStepRotate();
		~CTestStepRotate();
		virtual TVerdict DoTestStepL();
		
protected:
		void RotateAndMirrorL( CBitmapRotator& rotator, CFbsBitmap& srcBmp,CFbsBitmap& tgtBmp, CActiveListener& aActiveListener );
		
	};
/**
	CTestStepRotate16Ma the class is used to test all the rotation angles
	for EColor16MA mode bitmaps by using separate alpha and rgb bitmaps 
	as a source for combined bitmap
*/
class CTestStepRotate16Ma : public CTestStepRotate
	{
public:
			CTestStepRotate16Ma(const TDesC& aTestName, const TDesC& aRgbBmpName, const TDesC& aAlphaBmpName );
			~CTestStepRotate16Ma();
	virtual TVerdict DoTestStepL();
protected:
	const TPtrC iRgbBmpName;
	const TPtrC iAlphaBmpName;
	};


/**
 *
 * TestStepRotate90Degrees
 * This test step tests the rotation of a bit map n by n through 90 degrees
 * with N > 0
 * @lib "TS_BitmapTransforms.lib"
 *
 */
class CTestStepRotate90Degrees : public CTestStepBitmapTransforms
	{
public:
		CTestStepRotate90Degrees();
		~CTestStepRotate90Degrees();
		virtual enum TVerdict DoTestStepL();
		
	};


/**
 *
 * TestStepRotate180Degrees
 * This test step tests the rotation of a bit map n by n through 180 degrees
 * with N > 0
 * @lib "TS_BitmapTransforms.lib"
 *
 *
 */
class CTestStepRotate180Degrees : public CTestStepBitmapTransforms
	{
public:
		CTestStepRotate180Degrees();
		~CTestStepRotate180Degrees();
		virtual enum TVerdict DoTestStepL();
		
	};



/**
 *
 * TestStepRotate270Degrees
 * This test step tests the rotation of a bit map n by n through 90 degrees
 * with N > 0
 * @lib "TS_BitmapTransforms.lib"
 *
 */
class CTestStepRotate270Degrees : public CTestStepBitmapTransforms
	{
public:
		CTestStepRotate270Degrees();
		~CTestStepRotate270Degrees();
		virtual enum TVerdict DoTestStepL();
		
	};

/**
 *
 * CTestStepMirrorHorizontal
 * This test step tests the mirroring of a bit map n by n about the horizontal axis
 * with N > 0
 *
 * @lib "TS_BitmapTransforms.lib"
 *
 */
class CTestStepMirrorHorizontal : public CTestStepBitmapTransforms
	{
public:
		CTestStepMirrorHorizontal();
		~CTestStepMirrorHorizontal();
		virtual enum TVerdict DoTestStepL();
		
	};


/**
 *
 * TestStepMirrorVertical
 * This test step mirrors a n by n bitmap about the vertical axis
 * @lib "TS_BitmapTransforms.lib"
 *
 */
class CTestStepMirrorVertical : public CTestStepBitmapTransforms
	{
public:
		CTestStepMirrorVertical();
		~CTestStepMirrorVertical();
		virtual enum TVerdict DoTestStepL();
		
	};


/**
 *
 * TestStepZeroOnBothDimensions
 *
 * This test contructs a bitmap of zero size
 * and rotates and mirrors it
 * 
 * @lib "TS_BitmapTransforms.lib"
 *
 */
class CTestStepZeroOnBothDimensions : public CTestStepBitmapTransforms
	{
public:
	 CTestStepZeroOnBothDimensions();
	~CTestStepZeroOnBothDimensions();
	virtual enum TVerdict DoTestStepL();
	};



/**
 *
 * TestStepOneByY
 *
 * This test contructs a bitmap of size one by y and rotates it through 90 degrees clockwise
 * 
 * @lib "TS_BitmapTransforms.lib"
 */
class CTestStepOneByY : public CTestStepBitmapTransforms
	{
public:
	 CTestStepOneByY();
	~CTestStepOneByY();
	virtual enum TVerdict DoTestStepL( );
	};


/**
 *
 * TestStepXbyOne
 *
 * This test contructs a bitmap of size n by 1 and rotates it through 90 clockwise
 * 
 * @lib "TS_BitmapTransforms.lib"
 *
 */
class CTestStepXbyOne : public CTestStepBitmapTransforms
	{
public:
	 CTestStepXbyOne();
	~CTestStepXbyOne();
	virtual enum TVerdict DoTestStepL();
	};


/**
 *
 * CTestFlipOneByX 
 *
 * This test contructs a bitmap of unit size and rotates/mirrors it
 * based on the template argument It is the base class for a series of tests
 * which uses template specialisation to save code
 * 
 * @lib "TS_BitmapTransforms.lib"
 *
 */
 template <CBitmapRotator::TRotationAngle T> class CTestFlipOneByX : public CTestStepBitmapTransforms
	 {
public:
	         CTestFlipOneByX();
	virtual ~CTestFlipOneByX();
	virtual enum TVerdict DoTestStepL();
	 };


/**
 *
 * CTestStepFlipOneByN90 
 *
 * This test contructs a bitmap of unit size and rotates it through 90 degrees
 * based on the template argument
 * 
 * @lib "TS_BitmapTransforms.lib"
 *
 */

class CTestStepFlipOneByN90 : public  CTestFlipOneByX<CBitmapRotator::ERotation90DegreesClockwise> 
	{
public:
	CTestStepFlipOneByN90();
	};


/**
 *
 * CTestStepFlipOneByN180 
 *
 * This test contructs a bitmap of size 1 by N and rotates it by 180 degrees
 * 
 * @lib "TS_BitmapTransforms.lib"
 *
 *
 */

class CTestStepFlipOneByN180 : public  CTestFlipOneByX<CBitmapRotator::ERotation180DegreesClockwise> 
	{
public:
	CTestStepFlipOneByN180();
	};



/**
 *
 * CTestStepFlipOneByN270
 *
 * This test contructs a bitmap of size 1 by N and rotates it through 270 degrees clockwise
 * based on the template argument
 * 
 * @lib "TS_BitmapTransforms.lib"
 *
 */

class CTestStepFlipOneByN270 : public  CTestFlipOneByX<CBitmapRotator::ERotation270DegreesClockwise> 
	{
public:
	CTestStepFlipOneByN270();
	};


/**
 *
 * CTestStepFlipOnebyNVertical
 *
 * This test contructs a bitmap of unit 1 by N and flips it about the vertical axis
 * 
 * @lib "TS_BitmapTransforms.lib"
 */

class CTestStepFlipOnebyNVertical : public  CTestFlipOneByX<CBitmapRotator::EMirrorVerticalAxis> 
	{
public:
	CTestStepFlipOnebyNVertical();
	};


/**
 * CTestStepFlipOneByNHorizontal 
 *
 * This test contructs a bitmap of size 1 by N and flips it about the horizontal axis
 * 
 * @lib "TS_BitmapTransforms.lib"
 *
 *
 */

class CTestStepFlipOneByNHorizontal 
: public  CTestFlipOneByX<CBitmapRotator::EMirrorHorizontalAxis> 
	{
public:
	CTestStepFlipOneByNHorizontal();
	};


/**
 * CTestStepFlipOneByX
 *
 * This test contructs a bitmap of unit size n by 1
 * and rotates/mirrors based on the template argument
 * based on the template argument
 * 
 * @lib "TS_BitmapTransforms.lib"
 *
 *
 */
template <CBitmapRotator::TRotationAngle T> class CTestStepFlipXbyOne : public CTestStepBitmapTransforms
	{
 public:
	          CTestStepFlipXbyOne();
	virtual  ~CTestStepFlipXbyOne();
	virtual enum TVerdict DoTestStepL();
	};


/**
 * CTestStepFlipNByOne90
 *
 * This test contructs a bitmap of size n by 1 and rotates it through 90 degrees
 * based on the template argument
 * 
 * @lib "TS_BitmapTransforms.lib"
 *
 */

class CTestStepFlipNByOne90 : public  CTestStepFlipXbyOne<CBitmapRotator::ERotation90DegreesClockwise> 
	{
public:
	CTestStepFlipNByOne90();
	};


/**
 *
 * CTestStepFlipNByOne180 
 *
 * This test contructs a bitmap of size 1 by N and rotates it 180 degrees clockwise
 * based on the template argument
 * 
 * @lib "TS_BitmapTransforms.lib"
 *
 *
 */

class CTestStepFlipNByOne180 : public CTestStepFlipXbyOne<CBitmapRotator::ERotation180DegreesClockwise> 
	{
public:
	CTestStepFlipNByOne180();
	};



/**
 * CTestStepFlipNByOne270
 *
 * This test contructs a bitmap of size n by 1 and rotates it 270 degrees
 * 
 * @lib "TS_BitmapTransforms.lib"
 *
 *
 */

class CTestStepFlipNByOne270 : public  CTestStepFlipXbyOne<CBitmapRotator::ERotation270DegreesClockwise> 
	{
public:
	CTestStepFlipNByOne270();
	};


/**
 *
 * CTestStepFlipNByOneVertical
 *
 * This test contructs a bitmap of unit size and flips it
 * based on the template argument
 * 
 * @lib "TS_BitmapTransforms.lib"
 *
 */

class CTestStepFlipNByOneVertical : public  CTestStepFlipXbyOne<CBitmapRotator::EMirrorVerticalAxis> 
	{
public:
	CTestStepFlipNByOneVertical();
	};


/**
 *
 * CTestStepFlipNByOneHorizontal
 *
 * This test contructs a bitmap of size N by 1 and mirrors it about the horizontal axis 
 * @lib "TS_BitmapTransforms.lib"
 *
 */

class CTestStepFlipNByOneHorizontal : public  CTestStepFlipXbyOne<CBitmapRotator::EMirrorHorizontalAxis> 
	{
public:
	CTestStepFlipNByOneHorizontal();
	};


/**
 * CTestForceRotateMemoryAllocFailure
 * This test forces heap allocation errors and checks that leave functions
 * have been used
 * @lib "TS_BitmapTransforms.lib"
 *
 */

class CTestForceRotateMemoryAllocFailure : public CTestStepBitmapTransforms
	{
public:
     CTestForceRotateMemoryAllocFailure ();
	~CTestForceRotateMemoryAllocFailure ();
	virtual enum TVerdict DoTestStepL();
	void DoTestStepFbsL( const TFileName& iSourceFileName, CBitmapRotator::TRotationAngle aAngle );

private:
    TVerdict TestFailed( TInt& aError );
	};

/**
 * This test checks that CBitmapRotator->CustomCommand return KErrNotSupported
 * Unit test step for defect INC074719
 *
 */

class CTestCustomCommandCheck : public CTestStepBitmapTransforms
	{
public:
     CTestCustomCommandCheck ();
	~CTestCustomCommandCheck ();
	virtual enum TVerdict DoTestStepL();

private:
    TVerdict TestFailed( TInt& aError );
	};

/**
 *
 * TestStepRotate180DegreesMbm for: 
 * DEF120342: CBitmap Rotator, banding occurs in a bitmap rotated 180 degress
 *
 * MM_ICL_BTRANS_U_306_HP
 */
class CTestStepRotate180DegreesMbm : public CTestStepBitmapTransforms
	{
public:
		CTestStepRotate180DegreesMbm();
		~CTestStepRotate180DegreesMbm();
		virtual enum TVerdict DoTestStepL();
		
	};

/**
 *
 * TestStepRotateExtendedBitMap: 
 * 
 *
 * MM_ICL_BTRANS_U_307_HP
 */
class CTestStepRotateExtendedBitMap : public CTestStepBitmapTransforms, public MFbsExtendedBitmapInitializer
    {
public:
        CTestStepRotateExtendedBitMap();
        ~CTestStepRotateExtendedBitMap();
        virtual enum TVerdict DoTestStepL();
        void RotateAndMirrorABitmapL( const TFileName& aSourceFileName, CBitmapRotator::TRotationAngle aAngle, TDisplayMode aDisplayMode );
        TInt InitExtendedBitmap(TAny* aData, TInt aDataSize);
        
    };

#endif //(__TESTSTEP_ROTATE_H__)
