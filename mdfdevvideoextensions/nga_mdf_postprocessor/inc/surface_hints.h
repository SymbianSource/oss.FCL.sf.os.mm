/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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
*
*/

#ifndef __SURFACE_HINTS_LOCAL_H__
#define __SURFACE_HINTS_LOCAL_H__

//- Namespace ---------------------------------------------------------------

namespace surfaceHints
{

//- Constants ---------------------------------------------------------------

/** Hint of surface the content.
    @see TSurfaceContent for possible values
*/
const TInt KSurfaceContent = 0x1;

/** Hint of the expected update rate of the surface content.
    Value for a surface containing e.g. 25 fps video the value should be 25.
    For a static UI element the value should be 0.
    @see TSurfaceUpdate
*/
const TInt KSurfaceUpdate = 0x2;

/** Hint of the surface content copy protected and can it be
    shown on external displays.
    @see TSurfaceProtection for possible values.
*/
const TInt KSurfaceProtection = 0x3;

/** Hint of the color space of the surface content.
    Value is a pointer to TColorspace stucture.
    @see TColorspace
*/
const TInt KSurfaceColorspace = 0x4;

/** Values used for the KSurfaceProtection key. The values are bitmasks and can be combined
* e.g. EAllowAnalogProtectionRequired | EAllowDigitalProtectionRequired.
*/
enum TSurfaceProtection
    {
    /**
    * Not allowed on external outputs
    */
    EAllowInternalOnly                  = 0x00000000,

    /**
    * Allowed on all external outputs
    */
    EAllowAllExternals                  = 0xFFFFFFFF,

    /**
    * Allow passing content over analog outputs,
    * e.g. composite and S-video
    */
    EAllowAnalog                        = 0x00000010,

    /**
    * Allow output over an analog output channel which has a protection
    * mechanism
    */
    EAllowAnalogProtectionRequired      = 0x00000020,

    /**
    * Allow passing content over digital outputs,
    * e.g. DVI and HDMI
    */
    EAllowDigital                       = 0x00000200,

    /**
    * Licensed product must attempt to engage HDCP to protect the content.
    * However it should be passed through to HDMI even if HDCP is not engaged or fails to engage.
    */
    EAllowDigitalProtectionRequested    = 0x00000400,

    /**
    * Licensed product is required to engage HDCP to protect the content.
    * If HDCP is not engaged or can not be engaged the content must not be passed through to HDMI.
    */
    EAllowDigitalProtectionRequired     = 0x00000800,
    };

}; //namespace surfaceHints

#endif //__SURFACE_HINTS_LOCAL_H__

// End of File

