/*

* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).

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

* Description:  Class definition for the frame table functions.

 *

*/








// INCLUDE FILES

#include	<e32debug.h>
#include	<eaacplusutil.h>
#include    "mcc_eaacputil_aaccodec.h"



// ============================ MEMBER FUNCTIONS ===============================



// -----------------------------------------------------------------------------

// CEAacPlusUtil::CEAacPlusUtil

// C++ default constructor can NOT contain any code, that

// might leave.

// -----------------------------------------------------------------------------

//

CEAacPlusUtil::CEAacPlusUtil()

    {

    }



// -----------------------------------------------------------------------------

// CEAacPlusUtil::ConstructL

// Symbian 2nd phase constructor can leave.

// -----------------------------------------------------------------------------

//

void CEAacPlusUtil::ConstructL()

    {

	   
    }



// -----------------------------------------------------------------------------

// CEAacPlusUtil::NewL

// Two-phased constructor.

// -----------------------------------------------------------------------------

//

EXPORT_C CEAacPlusUtil* CEAacPlusUtil::NewL()

	{

	
    CEAacPlusUtil* self = new( ELeave ) CEAacPlusUtil;

    CleanupStack::PushL( self );

    self->ConstructL();

    CleanupStack::Pop(self);

    return self;

    }



// Destructor

EXPORT_C CEAacPlusUtil::~CEAacPlusUtil()

    {

    }



// -----------------------------------------------------------------------------

// CEAacPlusUtil::GetAacFormatType

// Used to get the format type.

// -----------------------------------------------------------------------------

//



EXPORT_C TInt CEAacPlusUtil::GetAacFormatType(const TUint8* aBuf, TInt aBufLen, TEAacPlusFormatType& aFormatType)

	{

	RDebug::Print(_L("CFrameTable::InitFrameTable aBuf[%d], aBufLen[%d], aFormatType[%d]"), aBuf, aBufLen, aFormatType);
    	return KErrNone;
	}



// -----------------------------------------------------------------------------

// CEAacPlusUtil::GetAacFrameInfo

// Used to get the format information.

// -----------------------------------------------------------------------------

//

EXPORT_C TInt CEAacPlusUtil::GetAacFrameInfo(const TUint8* aHdrBuf, TInt& aHdrLen, TEAacPlusFrameInfo& aFrameInfo)

    {

	RDebug::Print(_L("CFrameTable::InitFrameTable aHdrBuf[%d], aHdrLen[%d], aFrameInfo[%d]"), aHdrBuf, aHdrLen, aFrameInfo);
    	return KErrNone;

    }





// -----------------------------------------------------------------------------

// CEAacPlusUtil::GetAacFrameInfo

// Used to get the format information.

// -----------------------------------------------------------------------------

//

EXPORT_C TInt CEAacPlusUtil::GetAacFrameInfo (const TUint8* aHdrBuf, TInt& aHdrLen, TEAacPlusFrameInfo& aFrameInfo, 

                                                    TUint8* aDataBuf, TInt aDataBufLen)

    {

	RDebug::Print(_L("CFrameTable::InitFrameTable aHdrBuf[%d], aHdrLen[%d], aFrameInfo[%d], aDataBuf[%d], aDataBufLen[%d]"), aHdrBuf, aHdrLen, aFrameInfo, aDataBuf, aDataBufLen);
    	return KErrNone;

    }



// -----------------------------------------------------------------------------

// CEAacPlusUtil::GetAacFrameLength

// Used to get the size (in bytes) of the frame found at the start of aBuf.

// -----------------------------------------------------------------------------

//

EXPORT_C TInt CEAacPlusUtil::GetAacFrameLength(const TUint8* aBuf,	TInt aBufLen, TInt& aFrameLen)

	{	
RDebug::Print(_L("CFrameTable::InitFrameTable aBuf[%d], aBufLen[%d], aFrameLen[%d]"), aBuf, aBufLen, aFrameLen);
    	return KErrNone;
	}



// -----------------------------------------------------------------------------

// CEAacPlusUtil::Config

// Used to initialize the util if FrameInfo is not used to do it.

// -----------------------------------------------------------------------------

//

EXPORT_C TInt CEAacPlusUtil::Config(TEAacPlusFrameLenConfig& aFrameLenConfig)

	{

	RDebug::Print(_L("CFrameTable::InitFrameTable aFrameLenConfig[%d]"), aFrameLenConfig);
    	return KErrNone;
	}



// ========================== OTHER EXPORTED FUNCTIONS =========================



//  End of File

