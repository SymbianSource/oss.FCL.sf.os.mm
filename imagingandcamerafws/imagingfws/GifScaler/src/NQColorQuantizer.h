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

/* NeuQuant Neural-Net Quantization Algorithm Interface
 * ----------------------------------------------------
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

#ifndef __NQCOLORQUANTIZER_H__
#define __NQCOLORQUANTIZER_H__

#include <e32std.h>
#include <e32base.h>
#include <fbs.h>
#include "ColorQuantizer.h"

// Size of palette. (8bpp = 256 colors)
const TInt KPalettesize = 256;

// Four small primes, near 40
const TInt KSmallPrime1 = 37;
const TInt KSmallPrime2 = 41;
const TInt KSmallPrime3 = 43;
const TInt KSmallPrime4 = 47;

// Four primes near 500 - assume no image has a length so large
// that it is divisible by all four primes.
const TInt KPrime1 = 487;
const TInt KPrime2 = 491;
const TInt KPrime3 = 499;
const TInt KPrime4 = 503;

// Bias for colour values.
const TInt KNetbiasshift = 4;

// No. of learning cycles.
const TInt KNcycles = 100;

// Defs for freq and bias.
// Bias for fractions.
const TInt KIntbiasshift = 16;
const TInt KIntbias = 1<<KIntbiasshift;

// Gamma = 1024.
const TInt KGammashift = 10;
const TInt KGamma = 1<<KGammashift;

// Beta = 1/1024.
const TInt KBetashift = 10;
const TInt KBeta = KIntbias>>KBetashift;
const TInt KBetagamma = KIntbias<<(KGammashift-KBetashift);

// At 32.0 biased by 6 bits.
const TInt KRadiusbiasshift = 6;
const TInt KRadiusbias = 1<<KRadiusbiasshift;

// Factor of 1/30 each cycle.
const TInt KRadiusdec = 30;

// Defs for decreasing alpha factor.
// Alpha starts at 1.0.
const TInt KAlphabiasshift = 10;
const TInt KInitalpha = 1<<KAlphabiasshift;

// Radbias and alpharadbias used for radpower calculation.
const TInt KRadbiasshift = 8;
const TInt KRadbias = 1<<KRadbiasshift;
const TInt KAlpharadbshift = KAlphabiasshift+KRadbiasshift;
const TInt KAlpharadbias = 1<<KAlpharadbshift;

// Pixel represented as BGRc
typedef TInt TPixel[4];


// Quantization engine.
class CNQColorQuantizer; // declared here
NONSHARABLE_CLASS( CNQColorQuantizer ): public CColorQuantizer
{
public:
	// Current state.
	enum TQuantizeState
		{
		EInactiveState,					// Quantization is inactive.
		ENetworkLearn,					// Network training.
		EUnbiasNet,						// Unbias the net, etc.
		ECreateDestBitmap,				// Write the quantized pixel data.
		ECleanUpPending,				// Quantizing etc. complete, so start cleanup.
		ECleanUpState,					// Cleaning up.
		EQuantizingComplete
		};

public:
	// Initialise network in range (0,0,0) to (255,255,255) and set parameters
	static CColorQuantizer* NewL(CPalette* aPalette, TInt aNumPaletteEntries, TInt aSampleFactor, TUint8 aTransparencyThreshold);
	~CNQColorQuantizer();

	// From MColorQuantizer.
	void Quantize(TRequestStatus* aStatus, CFbsBitmap& aSource, CFbsBitmap& aDestination, CFbsBitmap* aDestinationMask, TInt aColorPixels);

private:
	CNQColorQuantizer(CPalette* aPalette, TInt aNumPaletteEntries, TInt aSampleFactor, TUint8 aTransparencyThreshold);
	void ConstructL();

	// Write the quantized bitmap.
	TBool WriteQuantizedBitmap();

	// Write the quantized bitmap and add transparency.
	TBool WriteQuantizedBitmapAndAddTransparency();
	
	// From CActive
	void RunL();
	void DoCancel();

	// Cleanup fn.
	// Used after quantization is complete.
	void CleanUp();
	
	// Init network.
	void InitNet();
	
	// Main Learning Loop
	TBool Learn();

	// Unbias network to give byte values 0..255 and record position i to prepare for sort
	void UnbiasNetAndOutputColorMap();

	// Insertion sort of network and building of netindex[0..255] (to do after unbias)
	void InxBuild();

	// Search for BGR values 0..255 (after net is unbiased) and return colour index
	TInt InxSearch(TInt aB, TInt aG, TInt aR);

	// Search for biased BGR values
	TInt Contest(TInt aB, TInt aG, TInt aR);

	// Move neuron i towards biased (b,g,r) by factor alpha
	void Altersingle(TInt aAlpha, TInt aI, TInt aB, TInt aG, TInt aR);

	// Move adjacent neurons by precomputed alpha*(1-((i-j)^2/[r]^2)) in radpower[|i-j|]
	void Alterneigh(TInt aRad, TInt aI, TInt aB, TInt aG, TInt aR);

private:
	// Current state of the quantization process.
	TQuantizeState iCurrentState;

	// The number of neurons in the network.
	// (and the number of colors to quantize down to)
	TInt iNetsize;

	// Sampling factor 1..30
	// (Set by NewL call)
	TInt iSampleFactor;

	// Sampling factor 1..30
	// (The actual sample factor used for the quantization.
	// This may be different from iSampleFactor)
	TInt iCurrentSampleFactor;

	// Total number of pixels to sample.
	TInt iSamplepixels;

	// Number of pixels to step by.
	TInt iStep;

	// Maximum processing ticks for each RunL() call.
	TUint iMaxProcessTicks;
	
	// Alpha factor.
	TInt iAlpha;

	// Radius factor.
	TInt iRadius;

	// The network itself.
	TPixel iNetwork[KPalettesize];

	// For network lookup.
	TInt iNetindex[KPalettesize];

	// Bias and freq arrays for learning.
	TInt iBias[KPalettesize];
	TInt iFreq[KPalettesize];

	// Radpower for precomputation.
	TInt iRadpower[KPalettesize];
};

#endif

