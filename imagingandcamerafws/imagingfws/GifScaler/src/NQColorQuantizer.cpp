// Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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

/* NeuQuant Neural-Net Quantization Algorithm
 * ------------------------------------------
 *
 * Copyright (c) 1994 Anthony Dekker
 *
 * NEUQUANT Neural-Net quantization algorithm by Anthony Dekker, 1994.
 * See "Kohonen neural networks for optimal colour quantization"
 * in "Network: Computation in Neural Systems" Vol. 5 (1994) pp 351-367.
 * for a discussion of the algorithm.
 * See also  http://members.ozemail.com.au/~dekker/NEUQUANT.HTML
 *
 * Any party obtaining a copy of these files from the author, directly or
 * indirectly, is granted, free of charge, a full and unrestricted irrevocable,
 * world-wide, paid up, royalty-free, nonexclusive right and license to deal
 * in this software and documentation files (the "Software"), including without
 * limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons who receive
 * copies from any such party to do so, with the only requirement being
 * that this copyright notice remain intact.
 */

#include <hal.h>
#include "NQColorQuantizer.h"
#include "GifScalerMain.h"

// Initialise network in range (0,0,0) to (255,255,255) and set parameters
CColorQuantizer* CNQColorQuantizer::NewL(CPalette* aPalette, TInt aNumPaletteEntries, TInt aSampleFactor, TUint8 aTransparencyThreshold)
	{
	CNQColorQuantizer* self = new(ELeave) CNQColorQuantizer(aPalette, aNumPaletteEntries, aSampleFactor, aTransparencyThreshold);
	CleanupStack::PushL(self);
	self->ConstructL();

	CleanupStack::Pop(self);
	return self;
	}

CNQColorQuantizer::CNQColorQuantizer(CPalette* aPalette, TInt aNumPaletteEntries, TInt aSampleFactor, TUint8 aTransparencyThreshold)
: CColorQuantizer(aPalette, aTransparencyThreshold), iNetsize(aNumPaletteEntries), iSampleFactor(aSampleFactor)
	{
	}

CNQColorQuantizer::~CNQColorQuantizer()
	{
	// Parent object should have cancelled the quantization,
	// so assert that we are not active here.
	ASSERT(!IsActive());

	ASSERT(iCurrentState == EInactiveState);
	}


const TUint KMaxProcessTime = 200000; // limit processing to 200ms for liveliness.
void CNQColorQuantizer::ConstructL()
	{
	ASSERT(iCurrentState == EInactiveState);
	
	TInt systemTickPeriod;
	User::LeaveIfError(HAL::Get(HAL::ESystemTickPeriod, systemTickPeriod));

	iMaxProcessTicks = (KMaxProcessTime / systemTickPeriod);
	}

void CNQColorQuantizer::Quantize(TRequestStatus* aStatus, CFbsBitmap& aSource, CFbsBitmap& aDestination, CFbsBitmap* aDestinationMask, TInt aColorPixels)
	{
	// Current state must be inactive.
	ASSERT(iCurrentState == EInactiveState);
	ASSERT(aStatus != NULL);
	iRequestStatus = aStatus;
	*iRequestStatus = KRequestPending;

	ASSERT(aSource.SizeInPixels() == aDestination.SizeInPixels());
	ASSERT(aDestination.DisplayMode() == EColor256);
	InitBitmapSettings(aSource, aDestination, aDestinationMask, aColorPixels);
	
	// Initialise quantization settings.
	TInt totalPixels = iBitmapSize.iWidth*iBitmapSize.iHeight;
	iCurrentSampleFactor = iSampleFactor;
	if (totalPixels<KPrime4)
		{
		iCurrentSampleFactor = 1;
		if ((totalPixels%KSmallPrime1) != 0)
			iStep = KSmallPrime1;
		else if ((totalPixels%KSmallPrime2) != 0)
			iStep = KSmallPrime2;
		else if ((totalPixels%KSmallPrime3) != 0)
			iStep = KSmallPrime3;
		else
			iStep = KSmallPrime4;
		}
	else if ((totalPixels%KPrime1) != 0)
		iStep = KPrime1;
	else if ((totalPixels%KPrime2) != 0)
		iStep = KPrime2;
	else if ((totalPixels%KPrime3) != 0)
		iStep = KPrime3;
	else
		iStep = KPrime4;

	// Set the number of pixels to sample.
	iSamplepixels = iColorPixels/iCurrentSampleFactor;

	// Reset the current pixel position.
	iCurrentPixel = 0;
	iPixelPt.iX = 0;
	iPixelPt.iY = 0;

	// Initialise the network.
	InitNet();

	// Start the quantizing process.
	iCurrentState = ENetworkLearn;
	SelfComplete(KErrNone);
	}

void CNQColorQuantizer::RunL()
	{
	if (iStatus.Int() != KErrNone)
		iCurrentState = ECleanUpPending;
	
	switch (iCurrentState)
		{
		case ENetworkLearn:
			{
			// Train the neural network.
			TBool learningComplete = Learn();

			// Move to the next state if learning is complete.
			if (learningComplete)
				iCurrentState = EUnbiasNet;

			SelfComplete(KErrNone);
			break;
			}

		case EUnbiasNet:
			// Training done,
			// so Unbias the net, output the map, and build the net index. 
			UnbiasNetAndOutputColorMap();
			InxBuild();
			
			// Begin writing the quantized bitmap.
			iPixelPt.iX = 0;
			iPixelPt.iY = 0;
			iCurrentState = ECreateDestBitmap;
			SelfComplete(KErrNone);
			break;

		case ECreateDestBitmap:
			{
			// Write the quantized bitmap.
			TBool bitmapComplete;
			if (iDestinationMask)
				bitmapComplete = WriteQuantizedBitmapAndAddTransparency();
			else 
				bitmapComplete = WriteQuantizedBitmap();
			
			if (bitmapComplete)
				{
				// Writing complete,
				// so move to the cleanup state.
				iCurrentState = ECleanUpState;
				}
			else
				{
				// Not finished yet,
				// so don't change state.
				SelfComplete(KErrNone);
				}
			}

			break;

		case ECleanUpPending:
			// All operations are complete,
			// so move to the cleanup state.
			iCurrentState = ECleanUpState;
			break;

		default:
			ASSERT(0);
		}
	
	if (iCurrentState == ECleanUpState)
		{
		CleanUp();
		iCurrentState = EQuantizingComplete;
		}

	if (iCurrentState == EQuantizingComplete)
		{
		User::RequestComplete(iRequestStatus, iStatus.Int());
		iCurrentState = EInactiveState;
		}
	}

void CNQColorQuantizer::CleanUp()
	{
	}

void CNQColorQuantizer::DoCancel()
	{
	CleanUp();

	iCurrentState = EInactiveState;
	CColorQuantizer::DoCancel();
	}

TBool CNQColorQuantizer::WriteQuantizedBitmap()
	{
	TUint currentTick = User::TickCount();
	const TUint tickLimit = currentTick + iMaxProcessTicks;

	// Lock the heap.
	TInt scanlineLength = CFbsBitmap::ScanLineLength(iBitmapSize.iWidth, iDestination->DisplayMode());
	TUint8* basePos = reinterpret_cast<TUint8*>(iDestination->DataAddress());
	basePos += scanlineLength*iPixelPt.iY;

	TRgb pixel;
	for ( ; (iPixelPt.iY<iBitmapSize.iHeight) ; iPixelPt.iY++)
		{
		iPixelPt.iX = 0;
		for ( ; iPixelPt.iX<iBitmapSize.iWidth ; iPixelPt.iX++)
			{
			iBitmap->GetPixel(pixel, iPixelPt);
			basePos[iPixelPt.iX] = TUint8(InxSearch(pixel.Blue(), pixel.Green(), pixel.Red()));
			}

		basePos += scanlineLength;

		currentTick = User::TickCount();
		if (currentTick>=tickLimit)
			break;
		}

	return (iPixelPt.iY == iBitmapSize.iHeight);
	}

TBool CNQColorQuantizer::WriteQuantizedBitmapAndAddTransparency()
	{
	TUint currentTick = User::TickCount();
	const TUint tickLimit = currentTick + iMaxProcessTicks;

	// Lock the heap.
	TInt scanlineLength = CFbsBitmap::ScanLineLength(iBitmapSize.iWidth, iDestination->DisplayMode());
	TUint8* basePos = reinterpret_cast<TUint8*>(iDestination->DataAddress());
	basePos += scanlineLength*iPixelPt.iY;

	TInt scanlineLengthMask = CFbsBitmap::ScanLineLength(iBitmapSize.iWidth, iDestinationMask->DisplayMode());
	TUint8* basePosMask = reinterpret_cast<TUint8*>(iDestinationMask->DataAddress());
	basePosMask += scanlineLengthMask*iPixelPt.iY;

	// The two scanline lengths should be the same.
	ASSERT(scanlineLength == scanlineLengthMask);

	TRgb pixel;
	for ( ; (iPixelPt.iY<iBitmapSize.iHeight) ; iPixelPt.iY++)
		{
		iPixelPt.iX = 0;
		for ( ; iPixelPt.iX<iBitmapSize.iWidth ; iPixelPt.iX++)
			{
			TBool pixelTransparent = (basePosMask[iPixelPt.iX]<=iTransparencyThreshold);
			if (pixelTransparent)
				basePos[iPixelPt.iX] = KTransparencyIndex;
			else
				{
				iBitmap->GetPixel(pixel, iPixelPt);
				basePos[iPixelPt.iX] = TUint8(InxSearch(pixel.Blue(), pixel.Green(), pixel.Red()));
				}
			}

		basePos += scanlineLength;
		basePosMask += scanlineLengthMask;

		currentTick = User::TickCount();
		if (currentTick>=tickLimit)
			break;
		}

	return (iPixelPt.iY == iBitmapSize.iHeight);
	}

// Unbias network to give byte values 0..255 and record position i to prepare for sort
void CNQColorQuantizer::UnbiasNetAndOutputColorMap()
	{
	for (TInt i = 0; i<iNetsize; i++)
		{
		for (TInt j = 0; j<3; j++)
			{
			/* OLD CODE: network[i][j] >>= netbiasshift; */
			/* Fix based on bug report by Juergen Weigert jw@suse.de */
			TInt temp = (iNetwork[i][j] + (1 << (KNetbiasshift - 1))) >> KNetbiasshift;
			if (temp > 255)
				temp = 255;
			
			iNetwork[i][j] = temp;
			}
		
		// Record color no.
		iNetwork[i][3] = i;

		// Write the color map entry.
		iPalette->SetEntry(i, TRgb(iNetwork[i][2], iNetwork[i][1], iNetwork[i][0]));
		}
	}

// Insertion sort of network and building of netindex[0..255] (to do after unbias)
void CNQColorQuantizer::InxBuild()
	{
	TInt previouscol = 0;
	TInt startpos = 0;
	TInt j;
	for (TInt i = 0; i<iNetsize; i++)
		{
		TInt* p = iNetwork[i];
		TInt* q;

		TInt smallpos = i;
		TInt smallval = p[1]; // Index on g

		// Find smallest in i..netsize-1
		for (j = i+1; j<iNetsize; j++)
			{
			q = iNetwork[j];
			if (q[1] < smallval) // Index on g
				{
				smallpos = j;
				smallval = q[1]; // Index on g
				}
			}
		q = iNetwork[smallpos];

		// Swap p (i) and q (smallpos) entries
		if (i != smallpos)
			{
			j = q[0];   q[0] = p[0];   p[0] = j;
			j = q[1];   q[1] = p[1];   p[1] = j;
			j = q[2];   q[2] = p[2];   p[2] = j;
			j = q[3];   q[3] = p[3];   p[3] = j;
			}

		// Smallval entry is now in position i
		if (smallval != previouscol)
			{
			iNetindex[previouscol] = (startpos+i)>>1;
			for (j = previouscol+1; j<smallval; j++)
				iNetindex[j] = i;

			previouscol = smallval;
			startpos = i;
			}
		}

	iNetindex[previouscol] = (startpos+(iNetsize-1))>>1;
	for (j = previouscol+1; j<KPalettesize; j++)
		iNetindex[j] = iNetsize-1;
	}


// Search for BGR values 0..255 (after net is unbiased) and return colour index
TInt CNQColorQuantizer::InxSearch(TInt aB, TInt aG, TInt aR)
	{
	TInt bestd = 1000; // Biggest possible dist is 256*3
	TInt best = -1;
	TInt i = iNetindex[aG];	// Index on g
	TInt j = i-1; // Start at netindex[g] and work outwards
	while ((i<iNetsize) || (j>=0))
		{
		if (i<iNetsize)
			{
			TInt* p = iNetwork[i];
			TInt dist = p[1] - aG; // inx key
			if (dist >= bestd)
				i = iNetsize; // stop iter
			else
				{
				i++;
				if (dist<0)
					dist = -dist;
				
				TInt a = p[0] - aB;
				if (a<0)
					a = -a;
				dist += a;
				
				if (dist<bestd)
					{
					a = p[2] - aR;
					if (a<0)
						a = -a;
					dist += a;
					
					if (dist<bestd)
						{
						bestd = dist;
						best = p[3];
						}
					}
				}
			}

		if (j>=0)
			{
			TInt* p = iNetwork[j];
			TInt dist = aG - p[1]; // inx key - reverse dif
			if (dist >= bestd)
				j = -1; // stop iter
			else
				{
				j--;
				if (dist<0)
					dist = -dist;
				
				TInt a = p[0] - aB;
				if (a<0)
					a = -a;
				dist += a;
				
				if (dist<bestd)
					{
					a = p[2] - aR;
					if (a<0)
						a = -a;
					dist += a;
					
					if (dist<bestd)
						{
						bestd = dist;
						best = p[3];
						}
					}
				}
			}
		}
	
	return best;
	}

// Search for biased BGR values
TInt CNQColorQuantizer::Contest(TInt aB, TInt aG, TInt aR)
	{
	// Finds closest neuron (min dist) and updates freq
	// Finds best neuron (min dist-bias) and returns position
	// For frequently chosen neurons, freq[i] is high and bias[i] is negative
	// bias[i] = gamma*((1/netsize)-freq[i])

	TInt bestd = ~((TUint32)1<<31);
	TInt bestbiasd = bestd;
	TInt bestpos = -1;
	TInt bestbiaspos = bestpos;
	TInt* p = iBias;
	TInt* f = iFreq;
	for (TInt i = 0; i<iNetsize; i++)
		{
		TInt* n = iNetwork[i];
		TInt dist = n[0] - aB;
		if (dist<0)
			dist = -dist;
		
		TInt a = n[1] - aG;
		if (a<0)
			a = -a;
		
		dist += a;
		a = n[2] - aR;
		if (a<0)
			a = -a;
		
		dist += a;
		if (dist<bestd)
			{
			bestd = dist;
			bestpos = i;
			}
		
		TInt biasdist = dist - ((*p)>>(KIntbiasshift-KNetbiasshift));
		if (biasdist<bestbiasd)
			{
			bestbiasd = biasdist;
			bestbiaspos = i;
			}
		
		TInt betafreq = (*f >> KBetashift);
		*f++ -= betafreq;
		*p++ += (betafreq<<KGammashift);
		}

	/*
	bestpos is initialised to -1 and should be reassigned
	a value from 0 to iNetSize above.  This ASSERT prevents
	incorrect index access (i.e. negative value).
	*/
	__ASSERT_ALWAYS(bestpos >= 0, Panic(EValueOutOfRange));
	// Ensure non-negative return
	__ASSERT_ALWAYS(bestbiaspos >= 0, Panic(EValueOutOfRange));
	iFreq[bestpos] += KBeta;
	iBias[bestpos] -= KBetagamma;
	return bestbiaspos;
}


// Move neuron i towards biased (b,g,r) by factor alpha
void CNQColorQuantizer::Altersingle(TInt aAlpha, TInt aI, TInt aB, TInt aG, TInt aR)
{
	TInt* n = iNetwork[aI]; // Alter hit neuron
	*n -= (aAlpha*(*n - aB)) / KInitalpha;
	n++;

	*n -= (aAlpha*(*n - aG)) / KInitalpha;
	n++;

	*n -= (aAlpha*(*n - aR)) / KInitalpha;
}


// Move adjacent neurons by precomputed alpha*(1-((i-j)^2/[r]^2)) in radpower[|i-j|]
void CNQColorQuantizer::Alterneigh(TInt aRad, TInt aI, TInt aB, TInt aG, TInt aR)
{
	TInt lo = aI-aRad;
	if (lo<-1)
		lo = -1;
	TInt hi = aI+aRad;
	if (hi>iNetsize)
		hi = iNetsize;

	TInt j = aI+1;
	TInt k = aI-1;
	TInt* q = iRadpower;
	while ((j<hi) || (k>lo))
		{
		TInt a = (*(++q));
		if (j<hi)
			{
			TInt* p = iNetwork[j];
			*p -= (a*(*p - aB)) / KAlpharadbias;
			p++;

			*p -= (a*(*p - aG)) / KAlpharadbias;
			p++;

			*p -= (a*(*p - aR)) / KAlpharadbias;
			j++;
		}

		if (k>lo)
			{
			TInt* p = iNetwork[k];
			*p -= (a*(*p - aB)) / KAlpharadbias;
			p++;

			*p -= (a*(*p - aG)) / KAlpharadbias;
			p++;

			*p -= (a*(*p - aR)) / KAlpharadbias;
			k--;
			}
		}
	}

void CNQColorQuantizer::InitNet()
	{
	TInt i;
	for (i = 0; i<iNetsize; i++)
		{
		TInt* p = iNetwork[i];
		p[0] = p[1] = p[2] = (i << (KNetbiasshift+8))/iNetsize;
		iFreq[i] = KIntbias/iNetsize;	// 1/netsize
		}

	iAlpha = KInitalpha;
	iRadius = (iNetsize>>3)*KRadiusbias;
	
	TInt rad = iRadius >> KRadiusbiasshift;
	if (rad <= 1)
		rad = 0;

	for (i = 0; i<rad; i++) 
		iRadpower[i] = iAlpha*(((rad*rad - i*i)*KRadbias)/(rad*rad));
	}

// Main Learning Loop
TBool CNQColorQuantizer::Learn()
{
	TUint currentTick = User::TickCount();
	const TUint tickLimit = currentTick + iMaxProcessTicks;
	
	// Decreasing alpha factor, biased by 10 bits.
	TInt alphadec = 30 + ((iCurrentSampleFactor-1)/3);
	TInt delta = iSamplepixels/KNcycles;

	TInt rad = iRadius >> KRadiusbiasshift;
	if (rad <= 1)
		rad = 0;

	TInt scanlineLengthMask = 0;
	TUint8* maskBaseAddr = NULL;
	if (iDestinationMask)
		{
		// Lock the heap.
		// Retrieve the mask base address.
		scanlineLengthMask = CFbsBitmap::ScanLineLength(iBitmapSize.iWidth, iDestinationMask->DisplayMode());
		maskBaseAddr = reinterpret_cast<TUint8*>(iDestinationMask->DataAddress());
		}

	TRgb pixel;
	while (iCurrentPixel<iSamplepixels)
		{
		TUint8* basePosMask = maskBaseAddr + scanlineLengthMask*iPixelPt.iY;
		TBool pixelTransparent = iDestinationMask && (basePosMask[iPixelPt.iX]<=iTransparencyThreshold);
		if (pixelTransparent)
			{
			iPixelPt.iX += iStep;
			while (iPixelPt.iX>=iBitmapSize.iWidth)
				{
				iPixelPt.iX -= iBitmapSize.iWidth;
				iPixelPt.iY++;

				if (iPixelPt.iY>=iBitmapSize.iHeight)
					iPixelPt.iY-= iBitmapSize.iHeight;
				}

			continue;
			}
		
		iBitmap->GetPixel(pixel, iPixelPt);
		TInt b = pixel.Blue() << KNetbiasshift;
		TInt g = pixel.Green() << KNetbiasshift;
		TInt r = pixel.Red() << KNetbiasshift;
		TInt j = Contest(b, g, r);

		Altersingle(iAlpha, j, b, g, r);
		if (rad)
			Alterneigh(rad, j, b, g, r); // Alter neighbours

		iPixelPt.iX += iStep;
		while (iPixelPt.iX>=iBitmapSize.iWidth)
			{
			iPixelPt.iX -= iBitmapSize.iWidth;
			iPixelPt.iY++;

			if (iPixelPt.iY>=iBitmapSize.iHeight)
				iPixelPt.iY-= iBitmapSize.iHeight;
			}
	
		iCurrentPixel++;
		if (iCurrentPixel%delta == 0)
			{	
			iAlpha -= iAlpha / alphadec;
			iRadius -= iRadius / KRadiusdec;
			rad = iRadius >> KRadiusbiasshift;
			if (rad <= 1)
				rad = 0;
			
			for (j = 0; j<rad; j++) 
				iRadpower[j] = iAlpha*(((rad*rad - j*j)*KRadbias)/(rad*rad));

			currentTick = User::TickCount();
			if (currentTick>=tickLimit)
				break;
			}
		}

	return (iCurrentPixel == iSamplepixels);
	}

