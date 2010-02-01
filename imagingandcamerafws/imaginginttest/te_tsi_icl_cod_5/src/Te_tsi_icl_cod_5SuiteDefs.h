/**
* Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:
* This file define all the common values thoughout your test project
* 
*
*/



/**
 @file Te_tsi_icl_cod_5SuiteDefs.h
 @internalTechnology
*/
#if (!defined __TE_TSI_ICL_COD_5_SUITEDEFS_H__)
#define __TE_TSI_ICL_COD_5_SUITEDEFS_H__

#include <hal.h> // for performance figures
#include <ecom/ecom.h> // for FinalClose()
#include <icl/icl_uids.hrh>
#include <icl/mngimagedisplay.hrh>

_LIT(KTe_tsi_icl_cod_5SuiteDecoderUid,"decoderUid");
_LIT(KTe_tsi_icl_cod_5SuiteSourceFile,"sourceFile");
_LIT(KTe_tsi_icl_cod_5Suite16MAPTest,"isEColor16MAPTest");
_LIT(KTe_tsi_icl_cod_5Suite16MAPRefFile,"16MAPRefFile");
_LIT(KTe_tsi_icl_cod_5Suite16MARefFile,"16MARefFile");
_LIT(KTe_tsi_icl_cod_5Suite16MURefFile,"16MURefFile");
_LIT(KTe_tsi_icl_cod_5Suite16MRefFile,"16MRefFile");
_LIT(KTe_tsi_icl_cod_5Suite8BitMaskRefFile,"8bitMaskRefFile");
_LIT(KTe_tsi_icl_cod_5SuiteEncoderUid,"encoderUid");
_LIT(KTe_tsi_icl_cod_5SuiteEncodeRefFile,"EncodeRefFile");
_LIT(KTe_tsi_icl_cod_5SuiteFramesToProcess,"frameCount");
_LIT(KTe_tsi_icl_cod_5SuiteMaskPresent,"mask");
_LIT(KTe_tsi_icl_cod_5SuiteAllocTestIsScalingTest,"isScalingTest");
_LIT(KTe_tsi_icl_cod_5SuiteAllocReductionFactor,"reductionFactor");
_LIT(KTe_tsi_icl_cod_5SuiteWidth,"width");
_LIT(KTe_tsi_icl_cod_5SuiteHeight,"height");

// For test step panics
_LIT(KTe_tsi_icl_cod_5SuitePanic,"Te_tsi_icl_cod_5Suite");

// File saving/identification
_LIT(KMbmExtension, ".mbm");
_LIT(KBmpExtension, ".bmp");
_LIT(KJpgExtension, ".jpg");
_LIT(KGifExtension, ".gif");
_LIT(KPngExtension, ".png");
_LIT(KFrameMbmExtension, "_frame_%d.mbm");
_LIT(KMask, "_mask");
_LIT(KReductionFactorMbmExtension, "_reduction_factor_%d.mbm");

const TUint KOneMicroSecond = 1000000; // for performance figures

// For scaling tests
const TInt KHalfSize = 1;
const TInt KQuarterSize = 2;
const TInt KEighthSize = 3;

#endif
