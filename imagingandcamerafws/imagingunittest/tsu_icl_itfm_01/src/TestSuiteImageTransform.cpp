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

// EPOC includes
#include <e32base.h>

// Test system includes
#include <testframework.h>
#include <icl/imagecodecdata.h>

#include "TestStepImageTransform.h"
#include "TestSuiteImageTransform.h"
#include "TestPlugin/TestTransformer.hrh"

#define KMbm1 _S("mbmProg.mbm")
#define KPng1 _S("png1.png")
#define KText _S("test.txt")
#define KTooShort _S("TooShort.txt")
#define KDoesntExist _S("DoesntExist.jpg")
#define K_corrupt_0thIFDTag_0000 _S("0thIFDTag_0000.jpg")
#define K_corrupt_0thIFDTag_7FFF _S("0thIFDTag_7FFF.jpg")
#define K_corrupt_0thIFDTag_FFFF _S("0thIFDTag_FFFF.jpg")

#define KMimeTypeJpg _S8("image/x-epoc-mbm")
#define KMimeTypePng _S8("image/png")
#define KMimeTypeSilly _S8("image/silly")

//source image names for test MM-ICL-ITFM-U-0019-HP
#define K_IllegalExifImage_Leave1 _S("IllegalExifImage_Leave1.jpg")
#define K_IllegalExifImage_Leave2 _S("IllegalExifImage_Leave2.jpg")
#define K_IllegalExifImage_Leave3 _S("IllegalExifImage_Leave3.jpg")
#define K_IllegalExifImage_Panic1 _S("IllegalExifImage_Panic1.jpg")
#define K_IllegalExifImage_Panic2 _S("IllegalExifImage_Panic2.jpg")

const TInt32 KJpegTestPlugin = KUidTransformImplementationJpegTestPlugin;
const TInt32 KPngTestPlugin = KUidTransformImplementationPngTestPlugin;

const TInt32 KImageTypePNGUidVal = 0x101F45C9; //KImageTypePNGUid.iUid;
const TInt32 KImageTypeMBMUidVal = 0x101F45CC; //KImageTypeJPGUid.iUid;
const TInt32 KImageSubTypeSilly = 0x01020304;

// constant table of parameters for tests
const TImageTransformUnitTestParams KTestParameters[] =
	{	
		//*****************************************************
		// JPEG tests - generic
		//*****************************************************
		{
		_S("MM-ICL-ITFM-U-0011-HP"), 
		_S("this tests scaling a JPEG image from file to file"), 
		KMbm1,			// source file name
		100, 100, 		// dest size
		ETrue,			// maintain aspect ratio
		ETrue,			// Source is a filename
		ETrue,			// Destination is a filename
		NULL,			// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		EFalse,			// continue onto next test step
		ENormal			// test type
		},
		{
		_S("MM-ICL-ITFM-U-0012-HP"), 
		_S("this tests scaling a JPEG image from file to descriptor"), 
		KMbm1,			// source file name
		100, 100, 		// dest size
		ETrue,			// maintain aspect ratio
		ETrue,			// Source is a filename
		EFalse,			// Destination is a filename
		NULL,			// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		EFalse,			// continue onto next test step
		ENormal			// test type
		},
		{
		_S("MM-ICL-ITFM-U-0013-HP"), 
		_S("this tests scaling a JPEG image from descriptor to file"), 
		KMbm1,			// source file name
		100, 100, 		// dest size
		ETrue,			// maintain aspect ratio
		EFalse,			// Source is a filename
		ETrue,			// Destination is a filename
		NULL,			// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		EFalse,			// continue onto next test step
		ENormal			// test type
		},
		{
		_S("MM-ICL-ITFM-U-0014-HP"), 
		_S("this tests scaling a JPEG image from descriptor to descriptor"), 
		KMbm1,			// source file name
		100, 100, 		// dest size
		ETrue,			// maintain aspect ratio
		EFalse,			// Source is a filename
		EFalse,			// Destination is a filename
		NULL,			// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		EFalse,			// continue onto next test step
		ENormal			// test type
		},
		{
		_S("MM-ICL-ITFM-U-0015-HP"), 
		_S("this tests scaling a JPEG image specifying the MIME type"), 
		KMbm1,			// source file name
		100, 100, 		// dest size
		ETrue,			// maintain aspect ratio
		ETrue,			// Source is a filename
		ETrue,			// Destination is a filename
		KMimeTypeJpg,	// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		EFalse,			// continue onto next test step
		ENormal			// test type
		},
		{
		_S("MM-ICL-ITFM-U-0016-HP"), 
		_S("this tests scaling a JPEG image specifying the image type"), 
		KMbm1,			// source file name
		100, 100,		// dest size
		ETrue,			// maintain aspect ratio
		ETrue,			// Source is a filename
		ETrue,			// Destination is a filename
		NULL,			// Mime type
		KImageTypeMBMUidVal, 0,	// image type & subtype
		0,				// plugin UID
		EFalse,			// continue onto next test step
		ENormal			// test type
		},
		{
		_S("MM-ICL-ITFM-U-0017-HP"), 
		_S("this tests scaling a JPEG image specifying the plugin UID"), 
		KMbm1,			// source file name
		100, 100,		// dest size
		ETrue,			// maintain aspect ratio
		ETrue,			// Source is a filename
		ETrue,			// Destination is a filename
		NULL,			// Mime type
		0, 0,			// image type & subtype
		KJpegTestPlugin,// plugin UID
		EFalse,			// continue onto next test step
		ENormal			// test type
		},

		//3 files for  PDEF103555:JPEG Exif image with invalid num of IFD0 entries leaves with KErrCorrupt 
		{
		_S("MM-ICL-ITFM-U-0018-HP"), 
		_S("this tests scaling a JPEG image from file to file"), 
		K_corrupt_0thIFDTag_0000,			// source file name
		100, 100, 		// dest size
		ETrue,			// maintain aspect ratio
		ETrue,			// Source is a filename
		ETrue,			// Destination is a filename
		NULL,			// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		ETrue,			// continue onto next test step
		ENormal			// test type
		},
		{
		_S("MM-ICL-ITFM-U-0018-HP"), 
		_S("this tests scaling a JPEG image from file to file"), 
		K_corrupt_0thIFDTag_7FFF,			// source file name
		100, 100, 		// dest size
		ETrue,			// maintain aspect ratio
		ETrue,			// Source is a filename
		ETrue,			// Destination is a filename
		NULL,			// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		ETrue,			// continue onto next test step
		ENormal			// test type
		},
		{
		_S("MM-ICL-ITFM-U-0018-HP"), 
		_S("this tests scaling a JPEG image from file to file"), 
		K_corrupt_0thIFDTag_FFFF,			// source file name
		100, 100, 		// dest size
		ETrue,			// maintain aspect ratio
		ETrue,			// Source is a filename
		ETrue,			// Destination is a filename
		NULL,			// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		EFalse,			// continue onto next test step
		ENormal			// test type
		},
		{
		_S("MM-ICL-ITFM-U-0019-HP"), 
		_S("this tests for memory overwriting on image transformation"), 
		K_IllegalExifImage_Leave1,			// source file name
		100, 100, 		// dest size
		ETrue,			// maintain aspect ratio
		ETrue,			// Source is a filename
		ETrue,			// Destination is a filename
		NULL,			// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		ETrue,			// continue onto next test step
		ENormal			// test type
		},
		{
		_S("MM-ICL-ITFM-U-0019-HP"), 
		_S("this tests for memory overwriting on image transformation"), 
		K_IllegalExifImage_Leave2,			// source file name
		100, 100, 		// dest size
		ETrue,			// maintain aspect ratio
		ETrue,			// Source is a filename
		ETrue,			// Destination is a filename
		NULL,			// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		ETrue,			// continue onto next test step
		ENormal			// test type
		},
		{
		_S("MM-ICL-ITFM-U-0019-HP"), 
		_S("this tests for memory overwriting on image transformation"), 
		K_IllegalExifImage_Leave3,			// source file name
		100, 100, 		// dest size
		ETrue,			// maintain aspect ratio
		ETrue,			// Source is a filename
		ETrue,			// Destination is a filename
		NULL,			// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		ETrue,			// continue onto next test step
		ENormal			// test type
		},
		{
		_S("MM-ICL-ITFM-U-0019-HP"), 
		_S("this tests for memory overwriting on image transformation"), 
		K_IllegalExifImage_Panic1,			// source file name
		100, 100, 		// dest size
		ETrue,			// maintain aspect ratio
		ETrue,			// Source is a filename
		ETrue,			// Destination is a filename
		NULL,			// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		ETrue,			// continue onto next test step
		ENormal			// test type
		},
		{
		_S("MM-ICL-ITFM-U-0019-HP"), 
		_S("this tests for memory overwriting on image transformation"), 
		K_IllegalExifImage_Panic2,			// source file name
		100, 100, 		// dest size
		ETrue,			// maintain aspect ratio
		ETrue,			// Source is a filename
		ETrue,			// Destination is a filename
		NULL,			// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		EFalse,			// continue onto next test step
		ENormal			// test type
		},
		//*****************************************************
		// PNG tests - generic
		//*****************************************************
		{
		_S("MM-ICL-ITFM-U-0021-HP"), 
		_S("this tests scaling a PNG image from file to file"), 
		KPng1,			// source file name
		100, 100,		// dest size
		ETrue,			// maintain aspect ratio
		ETrue,			// Source is a filename
		ETrue,			// Destination is a filename
		NULL,			// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		EFalse,			// continue onto next test step
		ENormal			// test type
		},
		{
		_S("MM-ICL-ITFM-U-0025-HP"), 
		_S("this tests scaling a PNG image specifying the MIME type"), 
		KPng1,			// source file name
		100, 100,		// dest size
		ETrue,			// maintain aspect ratio
		ETrue,			// Source is a filename
		ETrue,			// Destination is a filename
		KMimeTypePng,	// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		EFalse,			// continue onto next test step
		ENormal			// test type
		},
		{
		_S("MM-ICL-ITFM-U-0026-HP"), 
		_S("this tests scaling a PNG image specifying the image type"), 
		KPng1,			// source file name
		100, 100,		// dest size
		ETrue,			// maintain aspect ratio
		ETrue,			// Source is a filename
		ETrue,			// Destination is a filename
		NULL,			// Mime type
		KImageTypePNGUidVal, 0,	// image type & subtype
		0,				// plugin UID
		EFalse,			// continue onto next test step
		ENormal			// test type
		},
		{
		_S("MM-ICL-ITFM-U-0027-HP"), 
		_S("this tests scaling a PNG image specifying the plugin UID"), 
		KPng1,			// source file name
		100, 100,		// dest size
		ETrue,			// maintain aspect ratio
		ETrue,			// Source is a filename
		ETrue,			// Destination is a filename
		NULL,			// Mime type
		0, 0,			// image type & subtype
		KPngTestPlugin,	// plugin UID
		EFalse,			// continue onto next test step
		ENormal			// test type
		},
		//*****************************************************
		// Re-Use tests
		//*****************************************************
		{
		_S("MM-ICL-ITFM-U-0031-HP"), 
		_S("this tests scaling a JPEG image from file to file, then repeating with a different size"),
		KMbm1,			// source file name
		100, 100,		// dest size
		ETrue,			// maintain aspect ratio
		ETrue,			// Source is a filename
		ETrue,			// Destination is a filename
		NULL,			// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		ETrue,			// Re-use test: continue onto next test step
		ENormal			// test type
		},
		{
		NULL, 
		NULL,			
		KMbm1,			// source file name
		160, 90,		// dest size
		EFalse,			// maintain aspect ratio
		ETrue,			// Source is a filename
		ETrue,			// Destination is a filename
		NULL,			// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		EFalse,			// continue onto next test step
		ENormal			// test type
		},

		{
		_S("MM-ICL-ITFM-U-0032-HP"), 
		_S("this tests scaling a JPEG image from file to descriptor, then repeating scale to a file"),
		KMbm1,			// source file name
		100, 100,		// dest size
		ETrue,			// maintain aspect ratio
		ETrue,			// Source is a filename
		EFalse,			// Destination is a filename
		NULL,			// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		ETrue,			// Re-use test: continue onto next test step
		ENormal			// test type
		},
		{
		NULL, 
		NULL,			
		KMbm1,			// source file name
		160, 90,		// dest size
		EFalse,			// maintain aspect ratio
		ETrue,			// Source is a filename
		ETrue,			// Destination is a filename
		NULL,			// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		EFalse,			// continue onto next test step
		ENormal			// test type
		},
		//*****************************************************
		// Alloc tests
		//*****************************************************
		{
		_S("MM-ICL-ITFM-U-0100-HP"), 
		_S("this is an alloc test of scaling a jpeg image from file to file"), 
		KMbm1,			// source file name
		100, 100,		// dest size
		ETrue,			// maintain aspect ratio
		ETrue,			// Source is a filename
		ETrue,			// Destination is a filename
		NULL,			// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		EFalse,			// continue onto next test step
		EAlloc			// test type
		},
		{
		_S("MM-ICL-ITFM-U-0101-HP"), 
		_S("this is an alloc test of scaling a png image from descriptor to descriptor"), 
		KPng1,			// source file name
		100, 100,		// dest size
		ETrue,			// maintain aspect ratio
		EFalse,			// Source is a filename
		EFalse,			// Destination is a filename
		NULL,			// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		EFalse,			// continue onto next test step
		EAlloc			// test type
		}
	};

//*****************************************************
// Specific tests
//*****************************************************
const TImageTransformUnitTestParams KSpecificTest1Parameters =
	{
	_S("MM-ICL-ITFM-U-0041-HP"), 
	_S("this is a test of SetSourceRectL() - test PNG plugin should suport this"),
	KPng1,			// source file name
	100, 100,		// dest size
	ETrue,			// maintain aspect ratio
	ETrue,			// Source is a filename
	ETrue,			// Destination is a filename
	NULL,			// Mime type
	0, 0,			// image type & subtype
	0,				// plugin UID
	EFalse,			// continue onto next test step
	ENormal			// test type
	};

const TImageTransformUnitTestParams KSpecificTest2Parameters =
	{
	_S("MM-ICL-ITFM-U-0042-HP"),
	_S("this is a test of extension API mechanism - test client can use extension API"),
	KPng1,			// source file name
	100, 100,		// dest size
	ETrue,			// maintain aspect ratio
	ETrue,			// Source is a filename
	ETrue,			// Destination is a filename
	NULL,			// Mime type
	0, 0,			// image type & subtype
	0,				// plugin UID
	EFalse,			// continue onto next test step
	ENormal			// test type
	};

const TImageTransformUnitTestParams KSpecificTest3Parameters =
	{
	_S("MM-ICL-ITFM-U-0043-HP"),
	_S("this is a test of extension API mechanism - test client can use extension API"),
	KPng1,			// source file name
	100, 100,		// dest size
	ETrue,			// maintain aspect ratio
	ETrue,			// Source is a filename
	ETrue,			// Destination is a filename
	NULL,			// Mime type
	0, 0,			// image type & subtype
	0,				// plugin UID
	EFalse,			// continue onto next test step
	ENormal			// test type
	};

//*****************************************************
// Cancel tests
//*****************************************************
const TImageTransformUnitTestParams KCancelTest1Parameters =
	{
	_S("MM-ICL-ITFM-U-0081-HP"), 
	_S("this tests starting to scale a JPEG image & then cancelling"), 
	KMbm1,			// source file name
	100, 100,		// dest size
	ETrue,			// maintain aspect ratio
	ETrue,			// Source is a filename
	ETrue,			// Destination is a filename
	NULL,			// Mime type
	0, 0,			// image type & subtype
	0,				// plugin UID
	EFalse,			// continue onto next test step
	ENormal			// test type
	};

const TImageTransformUnitTestParams KCancelTest2Parameters =
	{
	_S("MM-ICL-ITFM-U-0082-HP"), 
	_S("this tests starting to scale a JPEG image & then deleting"), 
	KMbm1,			// source file name
	100, 100,		// dest size
	ETrue,			// maintain aspect ratio
	ETrue,			// Source is a filename
	ETrue,			// Destination is a filename
	NULL,			// Mime type
	0, 0,			// image type & subtype
	0,				// plugin UID
	EFalse,			// continue onto next test step
	ENormal			// test type
	};

//*****************************************************
// Negative tests
//*****************************************************
const TImageTransformUnitTestParams KNegativeTest1Parameters =
	{
	_S("MM-ICL-ITFM-U-00C1-HP"), 
	_S("this tests scaling a JPEG image specifying an invalid plugin UID"), 
	KMbm1,			// source file name
	100, 100,		// dest size
	ETrue,			// maintain aspect ratio
	ETrue,			// Source is a filename
	ETrue,			// Destination is a filename
	NULL,			// Mime type
	0, 0,			// image type & subtype
	0x01020304,		// plugin UID
	EFalse,			// continue onto next test step
	ENormal			// test type
	};

const TImageTransformUnitTestParams KNegativeTest2Parameters =
	{
	_S("MM-ICL-ITFM-U-00C2-HP"), 
	_S("this tests scaling a JPEG image specifying an invalid image type"), 
	KMbm1,			// source file name
	100, 100,		// dest size
	ETrue,			// maintain aspect ratio
	ETrue,			// Source is a filename
	ETrue,			// Destination is a filename
	NULL,			// Mime type
	0x01020304, 0,	// image type & subtype
	NULL,			// plugin UID
	EFalse,			// continue onto next test step
	ENormal			// test type
	};

const TImageTransformUnitTestParams KNegativeTest3Parameters =
	{
	_S("MM-ICL-ITFM-U-00C3-HP"), 
	_S("this tests scaling a JPEG image specifying an invalid MIME type"), 
	KMbm1,			// source file name
	100, 100,		// dest size
	ETrue,			// maintain aspect ratio
	ETrue,			// Source is a filename
	ETrue,			// Destination is a filename
	KMimeTypeSilly,	// Mime type
	0, 0,			// image type & subtype
	0,				// plugin UID
	EFalse,			// continue onto next test step
	ENormal			// test type
	};

const TImageTransformUnitTestParams KNegativeTest4Parameters =
	{
	_S("MM-ICL-ITFM-U-00C4-HP"), 
	_S("this tests scaling a text file"), 
	KText,			// source file name
	100, 100,		// dest size
	ETrue,			// maintain aspect ratio
	ETrue,			// Source is a filename
	ETrue,			// Destination is a filename
	NULL,			// Mime type
	0, 0,			// image type & subtype
	0,				// plugin UID
	EFalse,			// continue onto next test step
	ENormal			// test type
	};

const TImageTransformUnitTestParams KNegativeTest6Parameters =
	{
	_S("MM-ICL-ITFM-U-00C6-HP"), 
	_S("this tests specifying a null image type and a non-null subtype"), 
	KMbm1,			// source file name
	100, 100,		// dest size
	ETrue,			// maintain aspect ratio
	ETrue,			// Source is a filename
	ETrue,			// Destination is a filename
	NULL,			// Mime type
	0, KImageSubTypeSilly,			// image type & subtype
	0,				// plugin UID
	EFalse,			// continue onto next test step
	ENormal			// test type
	};


const TImageTransformUnitTestParams KNegativeTest7Parameters =
	{
	_S("MM-ICL-ITFM-U-00C7-HP"), 
	_S("this tests opening a file that doesn't exist"), 
	KDoesntExist,	// source file name
	100, 100,		// dest size
	ETrue,			// maintain aspect ratio
	ETrue,			// Source is a filename
	ETrue,			// Destination is a filename
	NULL,			// Mime type
	NULL, 0,		// image type & subtype
	NULL,			// plugin UID
	EFalse,			// continue onto next test step
	ENormal			// test type
	};

const TImageTransformUnitTestParams KNegativeTest8Parameters =
	{
	_S("MM-ICL-ITFM-U-00C8-HP"), 
	_S("this tests opening a file that is too short"), 
	KTooShort,		// source file name
	100, 100,		// dest size
	ETrue,			// maintain aspect ratio
	ETrue,			// Source is a filename
	ETrue,			// Destination is a filename
	NULL,			// Mime type
	NULL, 0,		// image type & subtype
	NULL,			// plugin UID
	EFalse,			// continue onto next test step
	ENormal			// test type
	};

const TImageTransformUnitTestParams KNegativeTest9Parameters =
	{
	_S("MM-ICL-ITFM-U-00C9-HP"), 
	_S("this tests calling SetSourceRectL() when it is not supported"), 
	KMbm1,		// source file name
	100, 100,		// dest size
	ETrue,			// maintain aspect ratio
	ETrue,			// Source is a filename
	ETrue,			// Destination is a filename
	NULL,			// Mime type
	NULL, 0,		// image type & subtype
	NULL,			// plugin UID
	EFalse,			// continue onto next test step
	ENormal			// test type
	};

//*****************************************************
// Panic tests
//*****************************************************

const TImageTransformUnitTestParams KPanicTest1Parameters =
	{
	_S("MM-ICL-ITFM-U-0200-HP"), 
	_S("this is a panic test of calling Transform() without haven't called SetupL()"), 
	KMbm1,			// source file name
	100, 100,		// dest size
	ETrue,			// maintain aspect ratio
	ETrue,			// Source is a filename
	ETrue,			// Destination is a filename
	NULL,			// Mime type
	0, 0,			// image type & subtype
	0,				// plugin UID
	EFalse,			// continue onto next test step
	ENormal			// test type
	};

const TImageTransformUnitTestParams KPanicTest2Parameters =
	{
	_S("MM-ICL-ITFM-U-0201-HP"), 
	_S("this is a panic test of calling Transform() twice()"), 
	KMbm1,			// source file name
	100, 100,		// dest size
	ETrue,			// maintain aspect ratio
	ETrue,			// Source is a filename
	ETrue,			// Destination is a filename
	NULL,			// Mime type
	0, 0,			// image type & subtype
	0,				// plugin UID
	EFalse,			// continue onto next test step
	ENormal			// test type
	};

const TImageTransformUnitTestParams KPanicTest4Parameters =
	{
	_S("MM-ICL-ITFM-U-0203-HP"), 
	_S("this is a panic test of calling SetupL() without specifying the source & destination & dest size"), 
	KMbm1,			// source file name
	100, 100,		// dest size
	ETrue,			// maintain aspect ratio
	ETrue,			// Source is a filename
	ETrue,			// Destination is a filename
	NULL,			// Mime type
	0, 0,			// image type & subtype
	0,				// plugin UID
	EFalse,			// continue onto next test step
	ENormal			// test type
	};


/**
*
* Create an instance of this test suite.
*
* NewTestSuiteImageTransformL is exported at ordinal 1.
* This provides the interface to allow the Test Framework
* to create instances of this test suite.
*
* @return	"CTestSuiteImageTransform*"
*			The newly created test suite
*/
EXPORT_C CTestSuiteImageTransform* NewTestSuiteImageTransformL() 
    { 
	CTestSuiteImageTransform* self = new (ELeave) CTestSuiteImageTransform;
	CleanupStack::PushL(self);
	self->ConstructL();

	CleanupStack::Pop();
	return self;
    }

/**
*
* CTestSuiteImageTransform destructor
*
* 
*/
CTestSuiteImageTransform::~CTestSuiteImageTransform()
	{
	}

/**
*
* This test suite version string
*
*
*/
_LIT(KTxtVersion,"1.0");

/**
*
* Get test suite version.
*
* @return	"TPtrC"
*			The version string.
*
*
*/
TPtrC CTestSuiteImageTransform::GetVersion( ) const
	{
	return KTxtVersion();
	}

/**
*
* Add a test step into the suite.
*
* @param	"CTestStepImageTransform* aPtrTestStep"
*			The test step to be added.
* 
*/
void CTestSuiteImageTransform::AddTestStepL(CTestStepImageTransform* aPtrTestStep)
	{
    // [ assert pre condition that a valid test ptr has been supplied ]
	__ASSERT_ALWAYS(aPtrTestStep, Panic(EBadArgument));
	
	// [ set back wards reference to test suite ]
	aPtrTestStep->SetSuite(this); 
	
	//[ add the step to the test suite ]
	CTestSuite::AddTestStepL(aPtrTestStep);
	}

/**
*
* Initialiser for test suite.
* This creates all the test steps and stores them
* inside CTestSuiteImageTransform
* 
*/
void CTestSuiteImageTransform::InitialiseL( )
	{
	//[ set the default path ]
	InitSystemPath();

	// store the name of this test suite
	iSuiteName = _L("ImageTransform");
	
	// Add tests


	// generic tests
	TUint stepNum;
	for (stepNum=0; stepNum<(sizeof(KTestParameters)/sizeof(TImageTransformUnitTestParams)); stepNum++)
		{
		const TImageTransformUnitTestParams& testParams = KTestParameters[stepNum];
		if (testParams.iTestName != NULL)
			AddTestStepL(new(ELeave) CTestStepTransformGeneric(testParams));
		}

	// specific tests
	AddTestStepL(new(ELeave) CTestStepTransformSpecific1(KSpecificTest1Parameters));
	AddTestStepL(new(ELeave) CTestStepTransformSpecific2(KSpecificTest2Parameters));
	AddTestStepL(new(ELeave) CTestStepTransformSpecific3(KSpecificTest3Parameters));
	
	// cancel tests
	AddTestStepL(new(ELeave) CTestStepTransformCancel1(KCancelTest1Parameters));
	AddTestStepL(new(ELeave) CTestStepTransformCancel2(KCancelTest2Parameters));

	// negative tests
	AddTestStepL(new(ELeave) CTestStepTransformNegative1(KNegativeTest1Parameters));
	AddTestStepL(new(ELeave) CTestStepTransformNegative1(KNegativeTest2Parameters));
	AddTestStepL(new(ELeave) CTestStepTransformNegative1(KNegativeTest3Parameters));
	AddTestStepL(new(ELeave) CTestStepTransformNegative1(KNegativeTest4Parameters));
	AddTestStepL(new(ELeave) CTestStepTransformNegative6(KNegativeTest6Parameters));
	AddTestStepL(new(ELeave) CTestStepTransformNegative7(KNegativeTest7Parameters));
	AddTestStepL(new(ELeave) CTestStepTransformNegative8(KNegativeTest8Parameters));
	AddTestStepL(new(ELeave) CTestStepTransformNegative9(KNegativeTest9Parameters));

	// panic tests
	AddTestStepL(new(ELeave) CTestStepTransformPanic1(KPanicTest1Parameters));
	AddTestStepL(new(ELeave) CTestStepTransformPanic2(KPanicTest2Parameters));
	AddTestStepL(new(ELeave) CTestStepTransformPanic4(KPanicTest4Parameters));


	}

/**
*
* InitSystemPath
* This sets up the default path
* 
*/
void CTestSuiteImageTransform::InitSystemPath()
	{
	_LIT(KTestSourcePathOnE,"e:\\tsu_icl_itfm_01\\");
	_LIT(KTestSourcePathOnD,"d:\\tsu_icl_itfm_01\\");
	_LIT(KTestSourcePathOnC,"c:\\tsu_icl_itfm_01\\");

	TUint att;
	RFs fs;
	TInt ret = fs.Connect();
	__ASSERT_ALWAYS(ret == KErrNone,User::Panic(_L("Sys path not setup"),ret));

	//[ take the first drive where it exists or can be made ]
	if (fs.Att(KTestSourcePathOnC, att) == KErrNone)
		{
		iDefaultPath = KTestSourcePathOnC;
		}
	else if (fs.Att(KTestSourcePathOnD, att) == KErrNone)
		{
		iDefaultPath = KTestSourcePathOnD;
		}
	else if (fs.Att(KTestSourcePathOnE, att) == KErrNone)
		{
		iDefaultPath = KTestSourcePathOnE;
		}
	else
		{
		User::Panic(_L("Test files not found"), KErrNotFound);
		}

	fs.Close();
	}
	
const TDesC& CTestSuiteImageTransform::DefaultPath() const
	{
	return iDefaultPath;
	}
