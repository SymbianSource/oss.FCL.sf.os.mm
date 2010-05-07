/*
  OMX_Symbian_ImageExt.h
  
  Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies). 
  All rights reserved.

  This program and the accompanying materials are made available 
  under the terms of the Eclipse Public License v1.0 which accompanies 
  this distribution, and is available at 
  http://www.eclipse.org/legal/epl-v10.html

  Initial Contributors:
  Nokia Corporation - initial contribution.
*/

/** @file
@brief Symbian OpenMAX IL Extension Data Structures in the Image Domain.

This file contains the extension structures for the Symbian IL extensions that
pertain to imaaging components.

@publishedDeviceAbstraction
*/

#ifndef OMX_Symbian_ImageExt_h
#define OMX_Symbian_ImageExt_h

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Each OMX header must include all required header files to allow the
 * header to compile without errors.  The includes below are required
 * for this header file to compile successfully 
 */
#include <OMX_Types.h>
#include <OMX_Image.h>

/* Extensions to standard enums */
/* ---------------------------- */

 
/**
 * Offset to Symbian extensions
 */
#define OMX_SYMBIAN_EXTENSION_START_OFFSET 0x00A00000


/** 
 * Focus Control extensions
 */
typedef enum OMX_SYMBIAN_IMAGE_FOCUSCONTROLTYPE {
    OMX_SYMBIAN_IMAGE_FocusControlRestPosition = OMX_IMAGE_FocusControlVendorStartUnused + OMX_SYMBIAN_EXTENSION_START_OFFSET,
                                               /**< Drives focus to rest position for minimal power consumption but should still be usable at least for view finder **/
    OMX_SYMBIAN_IMAGE_FocusControlMax = 0x7FFFFFFF
} OMX_SYMBIAN_IMAGE_FOCUSCONTROLTYPE;

/** 
 * Flash Control extensions
 */
typedef enum OMX_SYMBIAN_IMAGE_FLASHCONTROLTYPE {
    OMX_SYMBIAN_IMAGE_FlashControlSlowFrontSync = OMX_IMAGE_FlashControlVendorStartUnused + OMX_SYMBIAN_EXTENSION_START_OFFSET,
    OMX_SYMBIAN_IMAGE_FlashControlSlowRearSync,
    OMX_SYMBIAN_IMAGE_FlashControlOnTest,               /**< Allows production testing of the flash with a higher flash intensity 
                                                                   than the torch (may be that of a normal flash) and test (some) HW that 
                                                                   it turns OFF flash when automatic fail-safe happens (or capacitor 
                                                                   discharge in case of Xenon */
    OMX_SYMBIAN_IMAGE_FlashControlTorchOnTest,          /**< Turns on flash in torch mode */
    OMX_SYMBIAN_IMAGE_FlashControlIndicatorLightOnTest, /**< Turns on indicator light */
    OMX_SYMBIAN_IMAGE_FlashControlMax = 0x7FFFFFFF
} OMX_SYMBIAN_IMAGE_FLASHCONTROLTYPE;


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* OMX_Symbian_ImageExt_h */
