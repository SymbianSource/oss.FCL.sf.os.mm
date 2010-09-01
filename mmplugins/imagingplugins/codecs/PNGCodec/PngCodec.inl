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

inline
void CPngReadSubCodec::SetRgbaMode(TBool aMode)
	{
	iRgbaMode = aMode;
	}

inline
void CPngReadSubCodec::SetAlphaMode(TBool aMode)
	{
	iAlphaMode = aMode;
	}
	
inline 
void CPngReadSubCodec::Start()
    {
	if(iFastProc && !iRgbaMode)
		iFastProc->Begin();
    }

inline 
void CPngReadSubCodec::Complete()
    {
	if(iFastProc && !iRgbaMode)
		iFastProc->End();
    }

inline 
void CPngReadCodec::Complete()
    {
	if(iDecoder)
		iDecoder->Complete();
    }

inline
CImageProcessor* CPngReadCodec::MaskProcessor() const
	{
	return iMaskProc;
	}

inline
CImageProcessor* CPngReadCodec::ImageProcessor() const
	{
	return iImageProc;
	}

inline
CFastProcessor* CPngReadCodec::FastProcessor() const
	{
	return iFastProc;
	}

inline TInt CPngReadSubCodec::ClampValue(TInt aValue, TInt aMinValue, TInt aMaxValue) const
	{
	if(aMaxValue<aMinValue)
		{
		return aMinValue;
		}
	return (aValue < aMinValue) ? aMinValue : (aValue > aMaxValue) ? aMaxValue : aValue;
	}


inline TInt CPngWriteSubCodec::BufferSize() const
	{ 
	return iScanlineBufferSize; 
	}

inline CPalette* CPngWriteSubCodec::Palette() const
	{ 
	return iPalette; 
	}
    
