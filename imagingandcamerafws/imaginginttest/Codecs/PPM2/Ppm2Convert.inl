
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
//

inline TBool CPpmDecoder::Compressed() const
	{
	return (iFlags&EFlagCompressed);
	}

inline void CPpmDecoder::SetCompressed(TBool aCompressed)
	{
	if (aCompressed)
		{
		iFlags |= EFlagCompressed;
		}		
	else
		{
		iFlags &= ~EFlagCompressed;
		}		
	}

inline TBool CPpmDecoder::BytesValid() const
	{
	return (iFlags&EFlagBytesValid);
	}

inline void CPpmDecoder::SetBytesValid(TBool aBytesValid)
	{
	if (aBytesValid)
		{
		iFlags |= EFlagBytesValid;
		}		
	else
		{
		iFlags &= ~EFlagBytesValid;
		}		
	}

inline TInt CPpmDecoder::DataShift() const
	{
	return iDataShift;
	}
