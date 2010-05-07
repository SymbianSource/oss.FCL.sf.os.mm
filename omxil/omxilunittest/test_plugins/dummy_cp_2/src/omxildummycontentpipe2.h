// Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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

/**
 @file
 @internalComponent
 @prototype
*/

#ifndef OMXILDUMMYCONTENTPIPE2_H
#define OMXILDUMMYCONTENTPIPE2_H

#include <openmax/il/core/omxilsymbiancontentpipeif.h>

NONSHARABLE_CLASS(COmxILDummyContentPipe2) : public CP_PIPETYPE
	{
public:
	static COmxILDummyContentPipe2* NewL();
	~COmxILDummyContentPipe2();
	
	static CPresult Open(CPhandle* hContent, CPstring szURI, CP_ACCESSTYPE eAccess);

	static CPresult Close(CPhandle hContent);

	static CPresult Create(CPhandle* hContent, CPstring szURI);

	static CPresult CheckAvailableBytes(CPhandle hContent, CPuint nBytesRequested, CP_CHECKBYTESRESULTTYPE* eResult);

	static CPresult SetPosition(CPhandle hContent, CPint nOffset, CP_ORIGINTYPE eOrigin);

	static CPresult GetPosition(CPhandle hContent, CPuint* pPosition);

	static CPresult Read(CPhandle hContent, CPbyte* pData, CPuint nSize);

	static CPresult ReadBuffer(CPhandle hContent, CPbyte** ppBuffer, CPuint* nSize, CPbool bForbidCopy);

	static CPresult ReleaseReadBuffer(CPhandle hContent, CPbyte* pBuffer);

	static CPresult Write(CPhandle hContent, CPbyte* data, CPuint nSize);

	static CPresult GetWriteBuffer(CPhandle hContent, CPbyte** ppBuffer, CPuint nSize);

	static CPresult WriteBuffer(CPhandle hContent, CPbyte* pBuffer, CPuint nFilledSize);

	static CPresult RegisterCallback(CPhandle hContent, CPresult (*ClientCallback)(CP_EVENTTYPE eEvent, CPuint iParam));
            
private:
	COmxILDummyContentPipe2();
	
	class COmxILDummyContentHandle : public CBase
		{
	public:
		static TInt Open(COmxILDummyContentHandle*& aHandle, const TDesC8& aUri, CP_ACCESSTYPE aAccess);
		
		TInt Close();
		
		static TInt Create(COmxILDummyContentHandle*& aHandle, const TDesC8& aUri);
		
		TInt CheckAvailableBytes(TUint aBytesRequested, CP_CHECKBYTESRESULTTYPE* aResult);
		
		TInt SetPosition(TInt aOffset, CP_ORIGINTYPE aOrigin);
		
		TInt GetPosition(TUint* aPosition);
		
		TInt Read(TUint8* aData, TUint aSize); 
		
		TInt ReadBuffer(TUint8** aBuffer, TUint* aSize, TBool aForbidCopy);
		
		TInt ReleaseReadBuffer(TUint8* aBuffer);
		
		TInt Write(TUint8* aData, TUint aSize); 
		
		TInt GetWriteBuffer(TUint8** aBuffer, TUint aSize);
		
		TInt WriteBuffer(TUint8* aBuffer, TUint aFilledSize);
		
		TInt RegisterCallback(TUint (*ClientCallback)(CP_EVENTTYPE aEvent, TUint aParam));
		
		~COmxILDummyContentHandle();
		
	private:
		COmxILDummyContentHandle();
		
	private:
		TUint iPosition;
		};
	};

#endif // OMXILDUMMYCONTENTPIPE2_H
