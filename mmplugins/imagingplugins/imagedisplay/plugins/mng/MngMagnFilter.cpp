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

/** 	@file
	@internalTechnology */
#include "MngObjects.h"
#include "MngPlayer.h"

#include "PixelConsumer.h"

class TPixelInterpolator {
public:
	enum TMethodFlags 
			{	
			EInterpolateAlpha	=1, 
			EInterpolateColour	=2,
			ENoBreakCounter		=4
			};
	inline TPixelInterpolator(TInt aMethod, TRgbaColour aPixelValue, TRgbaColour aTargetPixel, TInt aNumberOfSteps);
	inline void NextStep();
	inline TRgbaColour CurrentValue() const;
protected:
	TUint	iRed;
	TUint	iGreen;
	TUint	iBlue;
	TUint	iAlpha;
	TInt	iRedStep;
	TInt	iGreenStep;
	TInt	iBlueStep;
	TInt	iAlphaStep;
	TInt	iMethod;
	TRgbaColour iCurrent;
	TRgbaColour iTarget;
	TInt iBreakCnt;
};

inline
TPixelInterpolator::TPixelInterpolator(TInt aMethod, TRgbaColour aPixelValue, TRgbaColour aTargetPixel, TInt aNumberOfSteps):
	iRed	((aPixelValue & KRedMask)<<16-KRedShift),
	iGreen	((aPixelValue & KGreenMask)<<16-KGreenShift),
	iBlue	((aPixelValue & KBlueMask)<<16-KBlueShift),
	iAlpha	((aPixelValue & KAlphaMask)>>32-KAlphaShift),
	iAlphaStep(0),
	iMethod(aMethod),
	iCurrent(aPixelValue),
	iTarget(aTargetPixel)
	{

	iBreakCnt=(aNumberOfSteps>>1)+((aNumberOfSteps&1)==0);
	if (iMethod & EInterpolateColour)
		{
		iRedStep	= (TInt(iRed)	- TInt((aTargetPixel & KRedMask)<<16-KRedShift)) / aNumberOfSteps;
		iGreenStep	= (TInt(iGreen)	- TInt((aTargetPixel & KGreenMask)<<16-KGreenShift)) / aNumberOfSteps;
		iBlueStep	= (TInt(iBlue)	- TInt((aTargetPixel & KBlueMask)<<16-KBlueShift)) / aNumberOfSteps;
		}
	else
		{
		iBlueStep = iGreenStep = iRedStep = 0;
		}

	if (iMethod & EInterpolateAlpha)
		{
		iAlphaStep	= (TInt(iAlpha)	- TInt((aTargetPixel & KAlphaMask)>>32-KAlphaShift)) / aNumberOfSteps;
		}
	}

inline
void TPixelInterpolator::NextStep()
	{
	iBreakCnt -=((iMethod&ENoBreakCounter)==0);
	if (iMethod & EInterpolateColour)
		{
		iRed	-= iRedStep;
		iGreen	-= iGreenStep;
		iBlue	-= iBlueStep;
		iCurrent=  ((iRed>>16-KRedShift) &KRedMask) | ((iGreen>>16-KGreenShift)&KGreenMask) | ((iBlue>>16-KBlueShift)&KBlueMask) 
					| (iCurrent & KAlphaMask);

		}
	else
		{
		if (iBreakCnt == 0)
			{
			iCurrent=(iCurrent & KAlphaMask) | (iTarget&KRgbMask);
			}
		}

	if (iMethod & EInterpolateAlpha)
		{
		iAlpha	-= iAlphaStep;
		iCurrent = ((iAlpha<<8) &KAlphaMask) | (iCurrent&KRgbMask);
		}
	else
		{
		if (iBreakCnt == 0)
			{
			iCurrent=(iTarget & KAlphaMask) | (iCurrent&KRgbMask);
			}
		}
	}
											
inline
TRgbaColour TPixelInterpolator::CurrentValue() const
	{
	return iCurrent;
	}


/*static*/
CMngObject* CMngMagnDefinition::NewL(TMngChunk*& aMagnChunk)
	{
	ASSERT(KRedShift==16 && KBlueShift==0 && KGreenShift==8 && KAlphaShift==24);
	
	TMagnChunk* MagnChunk=static_cast<TMagnChunk*>(aMagnChunk);
	aMagnChunk = NULL;
	CleanupDeletePushL(MagnChunk);
	CMngMagnDefinition* self = new (ELeave) CMngMagnDefinition(*MagnChunk);
	CleanupStack::Pop(MagnChunk);
	return self;
	}

inline
CMngMagnDefinition::CMngMagnDefinition(TMagnChunk& aMagnChunk):iMagn(&aMagnChunk)
	{
	}

CMngMagnDefinition::~CMngMagnDefinition()
	{
	delete [] iColorBuffer;
	delete iMagn;
	}

void CMngMagnDefinition::ExecuteL(CMngPlayer& aPlayer, TMngExecuteResult& /*aResult*/)
	{
	aPlayer.SetPixelFilter( (iMagn->iHeader.iLength? this : NULL ) );
	}

void CMngMagnDefinition::PrepareL(const TSize& aOriginalSize,CRgbPixelConsumer* aConsumer)
	{
	iConsumer	= aConsumer;
	
	if (iConsumer->PixelSize() == KRgbPixelSize)
		{
		iSetPixelFunction = &CRgbPixelConsumer::SetRGBPixel;
		}
	else if (iConsumer->PixelSize() == KRgbaPixelSize)
		{
		iSetPixelFunction = &CRgbPixelConsumer::SetRGBAPixel;
		}
	else
		{
		User::Leave(KErrNotSupported);
		}
	
	TInt bufferSize = aOriginalSize.iWidth*aOriginalSize.iHeight;
	if (iColorBuffer==NULL || bufferSize > iImageSize.iWidth*iImageSize.iHeight)
		{
		delete [] iColorBuffer;
		iColorBuffer = NULL;
		iColorBuffer = new (ELeave) TRgbaColour[bufferSize];
		}
	iImageSize	= aOriginalSize;

	if (iMagn->iXMagnMethod != EMngMagnNoMagnification)
		{
		const TInt KJustRepl=(iMagn->iXMagnMethod == EMngMagnPixelReplication);
		iXRepeat[EBorder1st]	=iMagn->iLeftXMagnFactor ;
		iXRepeat[EBorderMiddle]	=iMagn->iXMagnFactor ;
		iXRepeat[EBorderPreLast]=iMagn->iRightXMagnFactor;
		iXRepeat[EBorderLast]	=KJustRepl? iMagn->iRightXMagnFactor: 1;
		}
	else
		{
		iXRepeat[EBorder1st] = iXRepeat[EBorderMiddle] = iXRepeat[EBorderPreLast] = iXRepeat[EBorderLast] =1;
		}

	if (iMagn->iYMagnMethod != EMngMagnNoMagnification)
		{
		const TInt KJustRepl=(iMagn->iYMagnMethod == EMngMagnPixelReplication);
		iYRepeat[EBorder1st]	=iMagn->iTopYMagnFactor;
		iYRepeat[EBorderMiddle]	=iMagn->iYMagnFactor;
		iYRepeat[EBorderPreLast]=iMagn->iBottomYMagnFactor;
		iYRepeat[EBorderLast]	=KJustRepl? iMagn->iBottomYMagnFactor: 1;
		}
	else
		{
		iYRepeat[EBorder1st] = iYRepeat[EBorderMiddle] = iYRepeat[EBorderPreLast] = iYRepeat[EBorderLast] =1;
		}
	}

void CMngMagnDefinition::Commit()
	{
	if (iMagn->iYMagnMethod < EMngMagnLinearInterpolation && iMagn->iXMagnMethod < EMngMagnLinearInterpolation)
		{
		CommitSimpleRepeat();
		}
	else
		{
		CommitInterpolation();
		}
	}

void CMngMagnDefinition::CommitInterpolation()
	{
	const TRgbaColour* pPixel=iColorBuffer;
	TPoint realPos(0,0);
	
	const TInt yMethod=(iMagn->iYMagnMethod!=EMngMagnAlphaAndColorReplication &&
						 iMagn->iYMagnMethod!=EMngMagnLinearColorAplhaReplication)*TPixelInterpolator::EInterpolateAlpha 
						| (iMagn->iYMagnMethod!=EMngMagnAlphaAndColorReplication &&
							iMagn->iYMagnMethod!=EMngMagnLinearAlphaClosestColor)*TPixelInterpolator::EInterpolateColour
						| (iMagn->iYMagnMethod < EMngMagnLinearInterpolation)*TPixelInterpolator::ENoBreakCounter; 

	const TInt xMethod=(iMagn->iXMagnMethod!=EMngMagnAlphaAndColorReplication &&
						 iMagn->iXMagnMethod!=EMngMagnLinearColorAplhaReplication)*TPixelInterpolator::EInterpolateAlpha 
						| (iMagn->iXMagnMethod!=EMngMagnAlphaAndColorReplication &&
							iMagn->iXMagnMethod!=EMngMagnLinearAlphaClosestColor)*TPixelInterpolator::EInterpolateColour
						| (iMagn->iXMagnMethod < EMngMagnLinearInterpolation)*TPixelInterpolator::ENoBreakCounter; 
	
	for (TInt yCnt=0; yCnt < iImageSize.iHeight ; ++yCnt)
		{
		const TInt CurrentY=realPos.iY;
		const TRgbaColour* pNextYPixel=pPixel+iImageSize.iWidth*(iMagn->iYMagnMethod>EMngMagnPixelReplication && (yCnt < iImageSize.iHeight - 1));
		for (TInt xCnt=0; xCnt < iImageSize.iWidth; ++xCnt)
			{
			const TInt xRepeatCnt=XRepeat(xCnt);
			TInt yRepeatCnt=YRepeat(yCnt);
			TPixelInterpolator yInterpolator(yMethod, *pPixel, *pNextYPixel, yRepeatCnt);
			const TInt nextXDelta=(xCnt < iImageSize.iWidth-1 && iMagn->iXMagnMethod>EMngMagnPixelReplication);
			TPixelInterpolator yInterpolatorNx(yMethod, pPixel[nextXDelta], pNextYPixel[nextXDelta], yRepeatCnt);
			do	{
				TInt xRepeat=xRepeatCnt;
				TPixelInterpolator xInterpolator(xMethod, yInterpolator.CurrentValue(), yInterpolatorNx.CurrentValue(), xRepeat);

				iConsumer->SetPos(realPos);
				do	{
					CALL_SETPIXEL_FUNCTION(*iConsumer, iSetPixelFunction)(xInterpolator.CurrentValue());
					xInterpolator.NextStep();
					} while (--xRepeat);
				yInterpolator.NextStep();
				yInterpolatorNx.NextStep();
				++realPos.iY;
				} while (--yRepeatCnt);
			realPos.iX +=xRepeatCnt;
			realPos.iY	=CurrentY;
			++pPixel;
			++pNextYPixel;
			}
		realPos.iY = CurrentY + YRepeat(yCnt);
		realPos.iX = 0;
		} 
	}

void CMngMagnDefinition::CommitSimpleRepeat()
	{
	const TRgbaColour* pPixel=iColorBuffer;
	TPoint realPos(0,0);
	
	for (TInt yCnt=0; yCnt < iImageSize.iHeight; ++yCnt)
		{
		const TInt CurrentY=realPos.iY;
		for (TInt xCnt=0; xCnt < iImageSize.iWidth; ++xCnt)
			{
			const TInt xRepeatCnt=XRepeat(xCnt);
			TInt yRepeatCnt=YRepeat(yCnt);
			
			do	{
				TInt xRepeat=xRepeatCnt;
				iConsumer->SetPos(realPos);
				do	{
					CALL_SETPIXEL_FUNCTION(*iConsumer, iSetPixelFunction)(*pPixel);
					} while (--xRepeat);
				++realPos.iY;
				} while (--yRepeatCnt);
			realPos.iX +=xRepeatCnt;
			realPos.iY	=CurrentY;
			++pPixel;
			}
		realPos.iY = CurrentY + YRepeat(yCnt);
		realPos.iX = 0;
		} 
	}

void CMngMagnDefinition::SetPixel(const TPoint& aPosition, TUint aPixelColour) 
	{
	const TInt Offset=aPosition.iX + aPosition.iY*iImageSize.iWidth;
	ASSERT(Offset < iImageSize.iWidth*iImageSize.iHeight);
	iColorBuffer[Offset] = aPixelColour;
	}

void CMngMagnDefinition::AddRef()
	{
	++iRefCount;
	}

void CMngMagnDefinition::Release()
	{
	if (--iRefCount == 0)
		{
		delete [] iColorBuffer;
		iColorBuffer = NULL;
		}
	}

inline 
TInt CMngMagnDefinition::XRepeat(TInt aX) const
	{
	if (aX==0)
		{
		return iXRepeat[EBorder1st];
		}
	if (aX==iImageSize.iWidth-1)
		{
		return iXRepeat[EBorderLast];
		}
	if (aX==iImageSize.iWidth-2)
		{
		return iXRepeat[EBorderPreLast];
		}
	return iXRepeat[EBorderMiddle];
	}

inline 
TInt CMngMagnDefinition::YRepeat(TInt aY) const
	{
	if (aY==0)
		{
		return iYRepeat[EBorder1st];
		}
	if (aY==iImageSize.iHeight-1)
		{
		return iYRepeat[EBorderLast];
		}
	if (aY==iImageSize.iHeight-2)
		{
		return iYRepeat[EBorderPreLast];
		}
	return iYRepeat[EBorderMiddle];
	}

