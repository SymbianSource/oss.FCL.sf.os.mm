/*
  OMX_Symbian_IVCommonExt.h
  
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
@brief Symbian OpenMAX IL Extension Data Structures Common for Imaging and Video.

This file contains the extension structures for the Symbian IL extensions that
are common to Imaging and Video domains.

@publishedDeviceAbstraction
*/

#ifndef OMX_Symbian_IVCommonExt_h
#define OMX_Symbian_IVCommonExt_h

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Each OMX header must include all required header files to allow the
 * header to compile without errors.  The includes below are required
 * for this header file to compile successfully 
 */
#include <OMX_Types.h>
#include <OMX_IVCommon.h>
#include <OMX_Image.h>


/* Extensions to standard enums */
/* ---------------------------- */

 
/**
 * Offset to Symbian extensions
 */
#define OMX_SYMBIAN_EXTENSION_START_OFFSET 0x00A00000


/** 
 * Exposure Control extensions
 */
typedef enum OMX_SYMBIAN_EXPOSURECONTROLTYPE {
    OMX_SYMBIAN_ExposureControlCenter = OMX_ExposureControlVendorStartUnused + OMX_SYMBIAN_EXTENSION_START_OFFSET,
    OMX_SYMBIAN_ExposureControlVeryLong,
    OMX_SYMBIAN_ExposureControlHwFunctionalTesting, 
    OMX_SYMBIAN_ExposureControlMax = 0x7FFFFFFF
} OMX_SYMBIAN_EXPOSURECONTROLTYPE;

/** 
 * Image Filter extensions
 */
typedef enum OMX_SYMBIAN_IMAGEFILTERTYPE {
    OMX_SYMBIAN_ImageFilterSepia = OMX_ImageFilterVendorStartUnused + OMX_SYMBIAN_EXTENSION_START_OFFSET,
    OMX_SYMBIAN_ImageFilterGrayScale,
    OMX_SYMBIAN_ImageFilterNatural,
    OMX_SYMBIAN_ImageFilterVivid,
    OMX_SYMBIAN_ImageFilterMax = 0x7FFFFFFF
} OMX_SYMBIAN_IMAGEFILTERTYPE;


/* Extended generic types */
/* ---------------------- */
 

/** 
 * Generic point type
 * Point coordinates in 2D space
*/
typedef struct OMX_SYMBIAN_POINTTYPE {
    OMX_S32 nX; 
    OMX_S32 nY;
} OMX_SYMBIAN_POINTTYPE;

/** 
 * Generic size type
 * Size of rectangle in 2D space
*/
typedef struct OMX_SYMBIAN_SIZETYPE {
    OMX_S32 nWidth;
    OMX_S32 nHeight;
} OMX_SYMBIAN_SIZETYPE;

/** 
 * Generic rectangle type.
 * Rectangle represented by top left corner coordinates
 * and the size of the rectangle
 * There exist also a rectangle config defined in the OMX IL standard.
 * The purpose of this addition is to provide a generic rectangle type to 
 * be used by other configs and params.
*/
typedef struct OMX_SYMBIAN_RECTTYPE {
    OMX_SYMBIAN_POINTTYPE sTopLeft;
    OMX_SYMBIAN_SIZETYPE sSize;    
} OMX_SYMBIAN_RECTTYPE;

/** 
 * Relative rectangle type.
 * The purpose of this type is to represent a rectangular
 * region relative to certain reference size.
 * 
 * Example:
 * The location of an object detected in camera scene can be represendted
 * with relative rect. Let the object be located in rectangular are which has
 * top left coordintes (10,20) and size (90,120). The object detection algorithm
 * has used 320x240 sized image for analysis and that will be used as the 
 * reference size in the relative rect. The object location in 2 megapixel 
 * (1600x1200 pixels) captured image can be calculated by scaling the top left 
 * X-coordinate and rectangle width with reference size's width's faction 1600/320=5. 
 * Likewise for the Y-coordinate and height 1200/240=5. In the captured image 
 * the object is located inside a rectangular region that has top left coordinates 
 * (10*5, 20*5) = (50, 100) and size (90*5, 120*5) = (450, 600).      
*/
typedef struct OMX_SYMBIAN_RELATIVERECTTYPE {
    OMX_SYMBIAN_RECTTYPE sRect;      /**< Rectangle represented as concerete top left coordinates and size */
    OMX_SYMBIAN_SIZETYPE sReference; /**< Reference size. Since sRect is relative to this reference size
                                          it can be scaled up or down to represent the same rectangular 
                                          region in relation to bigger or smaller reference sizes */
} OMX_SYMBIAN_RELATIVERECTTYPE;



/* Extended generic config and param structs */
/* ----------------------------------------- */
 

/** 
 * Generic signed 32-bit integer config struct.
*/
typedef struct OMX_SYMBIAN_CONFIG_S32TYPE { 
    OMX_U32 nSize;              /**< Size of the structure in bytes */
    OMX_VERSIONTYPE nVersion;   /**< OMX specification version information */
    OMX_U32 nPortIndex;         /**< Port that this structure applies to */
    OMX_S32 nValue;             /**< signed 32-bit value */
} OMX_SYMBIAN_CONFIG_S32TYPE; 

/** 
 * Generic unsigned 32-bit integer param struct.
*/
typedef OMX_SYMBIAN_CONFIG_S32TYPE OMX_SYMBIAN_PARAM_S32TYPE; 

/** 
 * Generic unsigned 32-bit integer config struct.
*/
typedef struct OMX_SYMBIAN_CONFIG_U32TYPE {
    OMX_U32 nSize;              /**< Size of the structure in bytes */
    OMX_VERSIONTYPE nVersion;   /**< OMX specification version information */
    OMX_U32 nPortIndex;         /**< Port that this structure applies to */
    OMX_U32 nValue;             /**< Unsigned 32-bit value */
} OMX_SYMBIAN_CONFIG_U32TYPE; 

/** 
 * Generic unsigned 32-bit integer param struct.
*/
typedef OMX_SYMBIAN_CONFIG_U32TYPE OMX_SYMBIAN_PARAM_U32TYPE; 

/** 
 * Port specific generic boolean config struct
 * OMX spec already has boolean config but
 * without port index
*/
typedef struct OMX_SYMBIAN_CONFIG_BOOLEANTYPE {
    OMX_U32 nSize;              /**< Size of the structure in bytes */
    OMX_VERSIONTYPE nVersion;   /**< OMX specification version information */
    OMX_U32 nPortIndex;         /**< Port that this structure applies to */
    OMX_BOOL bEnabled;          /**< Boolean value */
} OMX_SYMBIAN_CONFIG_BOOLEANTYPE; 

/** 
 * Generic point config struct
*/
typedef struct OMX_SYMBIAN_CONFIG_POINTTYPE {
    OMX_U32 nSize;              /**< Size of the structure in bytes */
    OMX_VERSIONTYPE nVersion;   /**< OMX specification version information */
    OMX_U32 nPortIndex;         /**< Port that this structure applies to */
    OMX_SYMBIAN_POINTTYPE sPoint; /**< Point coordinates */
} OMX_SYMBIAN_CONFIG_POINTTYPE; 



/* Extended params and configs */
/* --------------------------- */


/** 
 * High level control type classification.
*/
typedef enum OMX_SYMBIAN_HIGHLEVELCONTROLTYPE {
    OMX_SYMBIAN_ControlOff,  /**< Explicitly turn feature off */ 
    OMX_SYMBIAN_ControlOn,   /**< Explicitly turn feature on */
    OMX_SYMBIAN_ControlAuto, /**< Let feature to be turned on or off automatically */
    OMX_SYMBIAN_ControlMax = 0x7FFFFFFF
} OMX_SYMBIAN_HIGHLEVELCONTROLTYPE;

/** 
 * Generic high level control config struct.
 *
*/
typedef struct OMX_SYMBIAN_CONFIG_HIGHLEVELCONTROLTYPE {
    OMX_U32 nSize;              /**< Size of the structure in bytes */
    OMX_VERSIONTYPE nVersion;   /**< OMX specification version information */
    OMX_U32 nPortIndex;         /**< Port that this structure applies to */
    OMX_SYMBIAN_HIGHLEVELCONTROLTYPE eControl; /**< High level control for given feature */
} OMX_SYMBIAN_CONFIG_HIGHLEVELCONTROLTYPE;



/** 
 * Zoom Factor config struct.
 * 
 * Note: Zoom factor is not the same as scale factor.
 * The zoom factor actually tells how much field of view
 * is decreased i.e. when zoom factor is 2x the 
 * FoV is decreased to half (1/2) of the original.
 * The scaling factor caused by decreasing the FoV is 
 * dependent on the output resolution.
*/
typedef struct OMX_SYMBIAN_CONFIG_ZOOMFACTORTYPE { 
    OMX_U32 nSize;              /**< Size of the structure in bytes */
    OMX_VERSIONTYPE nVersion;   /**< OMX specification version information */
    OMX_U32 nPortIndex;         /**< Port that this structure applies to */
    OMX_U32 xZoomFactor; /**< Zoom factor in unsigned Q16 format */
}OMX_SYMBIAN_CONFIG_ZOOMFACTORTYPE;

/** 
 * Enumeration of possible focus ranges 
 */
typedef enum OMX_SYMBIAN_FOCUSRANGETYPE {
    OMX_SYMBIAN_FocusRangeAuto,       /**< Focus range automatically decided by implementation */
    OMX_SYMBIAN_FocusRangeHyperfocal, /**< Focus range hyperfocal */
    OMX_SYMBIAN_FocusRangeSuperMacro, /**< Focus range supermacro */
    OMX_SYMBIAN_FocusRangeMacro,      /**< Focus range macro */
    OMX_SYMBIAN_FocusRangeInfinity,   /**< Focus range infinity */
    OMX_SYMBIAN_FocusRangeMax = 0x7FFFFFFF
} OMX_SYMBIAN_FOCUSRANGETYPE;

/** 
 * Focus Range config struct.
*/
typedef struct OMX_SYMBIAN_CONFIG_FOCUSRANGETYPE {
    OMX_U32 nSize;              /**< Size of the structure in bytes */
    OMX_VERSIONTYPE nVersion;   /**< OMX specification version information */
    OMX_U32 nPortIndex;         /**< Port that this structure applies to */
    OMX_SYMBIAN_FOCUSRANGETYPE eFocusRange; /**< Focus range */
} OMX_SYMBIAN_CONFIG_FOCUSRANGETYPE;

/** 
 * Focus Status.
*/
typedef struct OMX_SYMBIAN_CONFIG_EXTFOCUSSTATUSTYPE { 
    OMX_U32 nSize;              /**< Size of the structure in bytes */
    OMX_VERSIONTYPE nVersion;   /**< OMX specification version information */
    OMX_U32 nPortIndex;         /**< Port that this structure applies to */
    OMX_FOCUSSTATUSTYPE eFocusStatus; /**< Focus status */
} OMX_SYMBIAN_CONFIG_EXTFOCUSSTATUSTYPE;

/** 
 * Enumeration of possible flicker removal values 
 */
typedef enum OMX_SYMBIAN_FLICKERREMOVALTYPE {
    OMX_SYMBIAN_FlickerRemovalOff,  /**< Flicker removal disabled */
    OMX_SYMBIAN_FlickerRemovalAuto, /**< Automatically detected AC frequency */
    OMX_SYMBIAN_FlickerRemoval50,   /**< Flicker removal for 50 Hz AC frequency */
    OMX_SYMBIAN_FlickerRemoval60,   /**< Flicker removal for 60 Hz AC frequency */
    OMX_SYMBIAN_FlickerRemovalMax = 0x7FFFFFFF
} OMX_SYMBIAN_FLICKERREMOVALTYPE;

/** 
 * Flicker Removal.
*/
typedef struct OMX_SYMBIAN_CONFIG_FLICKERREMOVALTYPE {
    OMX_U32 nSize;              /**< Size of the structure in bytes */
    OMX_VERSIONTYPE nVersion;   /**< OMX specification version information */
    OMX_U32 nPortIndex;         /**< Port that this structure applies to */
   OMX_SYMBIAN_FLICKERREMOVALTYPE eFlickerRemoval; /**< Flicker removal mode */
}OMX_SYMBIAN_CONFIG_FLICKERREMOVALTYPE;

/** 
 * Enumeration of possible power versus image quality choices
 */
typedef enum OMX_SYMBIAN_QUALITYHINTTYPE {
    OMX_SYMBIAN_QualityNotSpecified = 0, /**< Not specified, component decides by itself */
    OMX_SYMBIAN_QualityLow,              /**< Power consumption priorized over quality */
    OMX_SYMBIAN_QualityBalanced,         /**< Balanced between quality and power consumption */
    OMX_SYMBIAN_QualityHigh,             /**< High quality priorized over power consumption */
    OMX_SYMBIAN_QualityMax = 0x7FFFFFFF
} OMX_SYMBIAN_QUALITYHINTTYPE;

/** 
 * Power versus quality hint provided by IL client to implementation.
*/
typedef struct OMX_SYMBIAN_CONFIG_HINTPOWERVSQUALITYTYPE {
    OMX_U32 nSize;              /**< Size of the structure in bytes */
    OMX_VERSIONTYPE nVersion;   /**< OMX specification version information */
    OMX_U32 nPortIndex;         /**< Port that this structure applies to */
    OMX_SYMBIAN_QUALITYHINTTYPE eQHint; /**< Power vs quality hint */
} OMX_SYMBIAN_CONFIG_HINTPOWERVSQUALITYTYPE;

/** 
 * Enumeration of possible depth of field choices
 */
typedef enum OMX_SYMBIAN_DOFHINTTYPE {
    OMX_SYMBIAN_DoFNotSpecified = 0, /**< Not specified, component decides by itself */
    OMX_SYMBIAN_DoFSmall,            /**< Small DoF */
    OMX_SYMBIAN_DoFMedium,           /**< Medium DoF */  
    OMX_SYMBIAN_DoFLarge,            /**< Large DoF */
    OMX_SYMBIAN_DoFMax = 0x7FFFFFFF
} OMX_SYMBIAN_DOFHINTTYPE;

/** 
 * Depth of field hint provided by IL client to implementation.
 *
*/
typedef struct OMX_SYMBIAN_CONFIG_HINTDOFTYPE {
    OMX_U32 nSize;              /**< Size of the structure in bytes */
    OMX_VERSIONTYPE nVersion;   /**< OMX specification version information */
    OMX_U32 nPortIndex;         /**< Port that this structure applies to */
    OMX_SYMBIAN_DOFHINTTYPE eDoFHint; /**< DoF hint */
} OMX_SYMBIAN_CONFIG_HINTDOFTYPE;

/** 
 * HW information retrieval struct.
*/
typedef struct OMX_SYMBIAN_CONFIG_HWINFOTYPE {
    OMX_U32 nSize;              /**< Size of the structure in bytes */
    OMX_VERSIONTYPE nVersion;   /**< OMX specification version information */
    OMX_U32 nPortIndex;         /**< Port that this structure applies to */
    OMX_U32 nVersion1;          /**< For camera sensor filled like this:                                    
                                     Bits 0...7 Sensor Manufacturer (8 bits). 
                                     Bits 8...15 Sensor Version/Revision (8 bit). 
                                     Bits 16...31 Sensor Model (16 bits).
                                     For flashgun filled like this:
                                     Bits 0-7    Flash IC Info
                                     Bits 8-15   Flash IC revision
                                     Bits 16-17  Flash Module Info A as present in Module Info Reg.A of flash HW 
                                     (Only valid for some Xenon flash, leave as 0 for LED and even for Xenon if not available)
                                     Bits 24-31  Flash Module Info B as present in Module Info Reg.B of flash HW 
                                     (Only valid for some Xenon flash, leave as 0 for LED and even for Xenon if not available) */
    OMX_U32 nVersion2;          /**< Not filled for camera
                                     For flashgun filled in same way as nVersion1 but containin information for secondary flash IC connected to same camera. 
                                     Leave as 0s if there is only single flash IC*/
    OMX_STRING cInfoString;     /**< Additional information string */
} OMX_SYMBIAN_CONFIG_HWINFOTYPE;

/** 
 * Capture mode extension.
 * 
 * Capture mode is extended to allow additional capture modes
 * like time nudge and bracketing
*/
typedef struct OMX_SYMBIAN_CONFIG_EXTCAPTUREMODETYPE {
    OMX_U32 nSize;              /**< Size of the structure in bytes */
    OMX_VERSIONTYPE nVersion;   /**< OMX specification version information */
    OMX_U32 nPortIndex;         /**< Port that this structure applies to */
    OMX_U32 nFrameBefore;       /**< Number of frames to be stored before capture bit is set */ 
    OMX_BOOL bPrepareCapture;   /**< Enables capturing frames into ring buffer before capture bit is set */
    OMX_BOOL bEnableBracketing; /**< Enables bracketing mode where settings are changed for each captured frame */
}OMX_SYMBIAN_CONFIG_EXTCAPTUREMODETYPE;

/** 
 * Enumeration of possible rotation angles
 */
typedef enum OMX_SYMBIAN_ROTATIONANGLETYPE {
    OMX_SYMBIAN_RotationCW0 = 0, /**< Rotation angle zero degrees clockwise */
    OMX_SYMBIAN_RotationCW90,    /**< Rotation angle 90 degrees clockwise */
    OMX_SYMBIAN_RotationCW180,   /**< Rotation angle 180 degrees clockwise */
    OMX_SYMBIAN_RotationCW270,   /**< Rotation angle 270 degrees clockwise */
    OMX_SYMBIAN_RotationMax = 0x7FFFFFFF
} OMX_SYMBIAN_ROTATIONANGLETYPE;

/** 
 * Enumeration of possible image orientation values 
 * The orientation values are same as the ones defined
 * for Exif orientation tag.
 */
typedef enum OMX_SYMBIAN_ORIENTATIONORIGINTYPE
{
    OMX_SYMBIAN_OrientationNotSpecified = 0x0,    /**< Orientatio of frame is not specified */
    OMX_SYMBIAN_OrientationRowTopColumnLeft,      /**< Frame is in normal orientation */
    OMX_SYMBIAN_OrientationRowTopColumnRight,     /**< Frame is mirrored */
    OMX_SYMBIAN_OrientationRowBottomColumnRight,  /**< Frame is 180 degrees CW rotated (=mirrored and flipped) */
    OMX_SYMBIAN_OrientationRowBottomColumnLeft,   /**< Frame is flipped */
    OMX_SYMBIAN_OrientationRowLeftColumnTop,      /**< Frame is 90 degrees CW rotated and mirrored */
    OMX_SYMBIAN_OrientationRowRightColumnTop,     /**< Frame is 270 degrees CW rotated */
    OMX_SYMBIAN_OrientationRowRightColumnBottom,  /**< Frame is 270 degrees CW rotated and mirrored */
    OMX_SYMBIAN_OrientationRowLeftColumnBottom,   /**< Frame is 90 degrees CW rotated */
    OMX_SYMBIAN_OrientationMax = 0x7FFFFFFF
} OMX_SYMBIAN_ORIENTATIONORIGINTYPE;

/** 
 * Orientation Configuration.
*/
typedef struct OMX_SYMBIAN_CONFIG_ORIENTATIONTYPE {
    OMX_U32 nSize;              /**< Size of the structure in bytes */
    OMX_VERSIONTYPE nVersion;   /**< OMX specification version information */
    OMX_U32 nPortIndex;         /**< Port that this structure applies to */
    OMX_SYMBIAN_ORIENTATIONORIGINTYPE eOrientation; /**< Frame orientation */
} OMX_SYMBIAN_CONFIG_ORIENTATIONTYPE;

/**
 * Maximum number of ROIs in config
 */
#define OMX_SYMBIAN_MAX_NUMBER_OF_ROIS 10

/** 
 * Tracking object type classification.
*/
typedef enum OMX_SYMBIAN_ROIOBJECTTYPE {
    OMX_SYMBIAN_RoiObjectNone,       /**< Unspecified object type */
    OMX_SYMBIAN_RoiObjectTypeFace,   /**< Object type face */
    OMX_SYMBIAN_RoiObjectTypeObject, /**< Object type generic object */
    OMX_SYMBIAN_RoiObjectMax = 0x7FFFFFFF
} OMX_SYMBIAN_ROIOBJECTTYPE;

/**
 * Pricipal 3D orientation type
 */
typedef struct OMX_SYMBIAN_3DORIENTATIONYTYPE {
    OMX_U32 nYaw;   /**< Yaw rotation angle in degrees, 0xFFFFFFFF if unknown */
    OMX_U32 nPitch; /**< Pitch rotation angle in degrees, 0xFFFFFFFF if unknown */
    OMX_U32 nRoll;  /**< Roll rotation angle in degrees, 0xFFFFFFFF if unknown */
} OMX_SYMBIAN_3DORIENTATIONYTYPE;

/**
 * Object information structure
 */
typedef struct OMX_SYMBIAN_ROIOBJECTINFOTYPE{
    OMX_SYMBIAN_RELATIVERECTTYPE sROI;           /**< Region where the object is located in the scene */
    OMX_U32 nROIID;                              /**< Identification number for ROI if available, zero by default */ 
    OMX_U32 nPriority;                           /**< ROI priority, 0 being the highest priority */
    OMX_SYMBIAN_ROIOBJECTTYPE eObjectType;       /**< Type of object in this ROI */
    OMX_SYMBIAN_3DORIENTATIONYTYPE nOrientation; /**< 3D orientation of the object in the ROI */
} OMX_SYMBIAN_ROIOBJECTINFOTYPE;

/** 
 * Regions of interest config
 * 
 * Used to indicate number of regions of interest within the image
 * Also some additional information about the object in the ROI
 * can be embedded but is not mandatory
*/
typedef struct OMX_SYMBIAN_CONFIG_ROITYPE {    
    OMX_U32 nSize;              /**< Size of the structure in bytes */
    OMX_VERSIONTYPE nVersion;   /**< OMX specification version information */
    OMX_U32 nPortIndex;         /**< Port that this structure applies to */
    OMX_U32 nNumberOfROIs;      /**< Number of ROIs included in this config */
    OMX_SYMBIAN_ROIOBJECTINFOTYPE sROIs[OMX_SYMBIAN_MAX_NUMBER_OF_ROIS]; /**< Array of ROIs */ 
} OMX_SYMBIAN_CONFIG_ROITYPE; 

/** 
 * Enumeration of possible xenon flash status values 
 */
typedef enum OMX_SYMBIAN_XENONFLASHSTATUSTYPE
{
    OMX_SYMBIAN_XenonFlashUnknown  = 0x0,   /**< Xenon flash status is unknown */
    OMX_SYMBIAN_XenonFlashDischarged,       /**< Xenon flash is discharged */
    OMX_SYMBIAN_XenonFlashCharging,         /**< Xenon flash is charging */
    OMX_SYMBIAN_XenonFlashReady,            /**< Xenon flash is ready to be used */
    OMX_SYMBIAN_XenonFlashNotAvailable,     /**< Xenon flash can not be used at the moment
                                                 (temporarily overheated etc...) */
    OMX_SYMBIAN_XenonFlashMax = 0x7FFFFFFF
} OMX_SYMBIAN_XENONFLASHSTATUSTYPE;

/** 
 * Xenon flash status Config.
*/
typedef struct OMX_SYMBIAN_CONFIG_XENONFLASHSTATUSTYPE { 
    OMX_U32 nSize;              /**< Size of the structure in bytes */
    OMX_VERSIONTYPE nVersion;   /**< OMX specification version information */
    OMX_U32 nPortIndex;         /**< Port that this structure applies to */
    OMX_SYMBIAN_XENONFLASHSTATUSTYPE eFlashStatus; /**< Xenon flash status */
} OMX_SYMBIAN_CONFIG_XENONFLASHSTATUSTYPE;


/** 
 * Enumeration of possible pre capture exposure time estimates
 * 
 * This enumeration is used to implement pre-capture warning about
 * low or high exposure time which could lead to bad quality images
 */
typedef enum OMX_SYMBIAN_PRECAPTUREEXPOSURETIMETYPE
{
    OMX_SYMBIAN_PreCaptureExposureNoneOrOngoing = 0x0, /**< Exposure time not known or exposure already started */
    OMX_SYMBIAN_PreCaptureExposureNormal,              /**< Exposure time is within normal limits */
    OMX_SYMBIAN_PreCaptureExposureShort,               /**< Exposure time is short, risk of underexposing the images */
    OMX_SYMBIAN_PreCaptureExposureLong,                /**< Exposure time is long, risk of blurry image */
    OMX_SYMBIAN_PreCaptureExposureMax = 0x7FFFFFFF
} OMX_SYMBIAN_PRECAPTUREEXPOSURETIMETYPE;

/** 
 * Pre-Capture Exposure Time Config.
*/
typedef struct OMX_SYMBIAN_CONFIG_PRECAPTUREEXPOSURETIMETYPE {
    OMX_U32 nSize;              /**< Size of the structure in bytes */
    OMX_VERSIONTYPE nVersion;   /**< OMX specification version information */
    OMX_U32 nPortIndex;         /**< Port that this structure applies to */
    OMX_SYMBIAN_PRECAPTUREEXPOSURETIMETYPE eExposureTime; /**< Estimated exposure time */
} OMX_SYMBIAN_CONFIG_PRECAPTUREEXPOSURETIMETYPE;

/** 
 * Enumeration of possible bracket mode values 
 */
typedef enum OMX_SYMBIAN_BRACKETMODETYPE {
    OMX_SYMBIAN_BracketExposureRelativeInEV, /**< Exposure value is changed relative to 
                                                        the value set by automatic exposure. 
                                                        nBracketValues are in Q16. Increment is additive. */
    OMX_SYMBIAN_BracketMax = 0x7FFFFFFF
} OMX_SYMBIAN_BRACKETMODETYPE;

/** 
 * Bracketing Config.
*/
typedef struct OMX_SYMBIAN_CONFIG_BRACKETINGTYPE {
    OMX_U32 nSize;              /**< Size of the structure in bytes */
    OMX_VERSIONTYPE nVersion;   /**< OMX specification version information */
    OMX_U32 nPortIndex;         /**< Port that this structure applies to */
    OMX_SYMBIAN_BRACKETMODETYPE eBracketMode; /**< Bracketing mode - what values are changed during bracket capture */
    OMX_U32 nNbrBracketingValues;             /**< Number of bracketing values */
    OMX_U32 nBracketValues[5];                /**< Bracketing values in an array */
} OMX_SYMBIAN_CONFIG_BRACKETINGTYPE;

/** 
 * Enumeration of possible levels of RAW processing 
 */
typedef enum OMX_SYMBIAN_RAWIMAGEPRESETTYPE {
    OMX_SYMBIAN_RawImageUnprocessed, /**< RAW data is unprocessed */
    OMX_SYMBIAN_RawImageProcessed,   /**< RAW data has some processing enabled */
    OMX_SYMBIAN_RawImageMax = 0x7FFFFFFF
} OMX_SYMBIAN_RAWIMAGEPRESETTYPE;

/** 
 * RAW Preset Config.
*/
typedef struct OMX_SYMBIAN_CONFIG_RAWPRESETTYPE {
    OMX_U32 nSize;              /**< Size of the structure in bytes */
    OMX_VERSIONTYPE nVersion;   /**< OMX specification version information */
    OMX_U32 nPortIndex;         /**< Port that this structure applies to */
    OMX_SYMBIAN_RAWIMAGEPRESETTYPE ePreset; /**< RAW processing preset */
} OMX_SYMBIAN_CONFIG_RAWPRESETTYPE;

/** 
 * Image Parameters Lock.
*/
typedef struct OMX_SYMBIAN_CONFIG_LOCKTYPE {
    OMX_U32 nSize;              /**< Size of the structure in bytes */
    OMX_VERSIONTYPE nVersion;   /**< OMX specification version information */
    OMX_U32 nPortIndex;         /**< Port that this structure applies to */
    OMX_BOOL bLock;           /**< Setting this true will lock the setting and 
                                   its value is not automatically updated anymore */
    OMX_BOOL bAtCapture;      /**< Setting this true will lock the setting value only
                                   when next capture occurs */
} OMX_SYMBIAN_CONFIG_LOCKTYPE;

/** 
 * Image Flash Control Config.
 * 
 * Extended because in standard flash control is a param
*/
typedef struct OMX_SYMBIAN_CONFIG_FLASHCONTROLTYPE {
    OMX_U32 nSize;              /**< Size of the structure in bytes */
    OMX_VERSIONTYPE nVersion;   /**< OMX specification version information */
    OMX_U32 nPortIndex;         /**< Port that this structure applies to */
    OMX_IMAGE_FLASHCONTROLTYPE eFlashControl; /**< Flash mode setting */
}OMX_SYMBIAN_CONFIG_FLASHCONTROLTYPE;

/** 
 * Enumeration of possible focus region control values
 */
typedef enum OMX_SYMBIAN_FOCUSREGIONCONTROL {
    OMX_SYMBIAN_FocusRegionAuto,           /**< Focus region decided by AF algorithm */
    OMX_SYMBIAN_FocusRegionManual,         /**< Manual focus region selected by user */
    OMX_SYMBIAN_FocusRegionFacePriority,   /**< ROI with priority face (if available) should be used as focus region, otherwise automatically selected by AF algorithm */
    OMX_SYMBIAN_FocusRegionObjectPriority, /**< ROI with priority object (if available) should be used as focus region, otherwise automatically selected by AF algorithm */
    OMX_SYMBIAN_FocusRegionMax = 0x7FFFFFFF
} OMX_SYMBIAN_FOCUSREGIONCONTROL;

/** 
 * Focus Region Control Config.
*/
typedef struct OMX_SYMBIAN_CONFIG_FOCUSREGIONTYPE {
    OMX_U32 nSize;              /**< Size of the structure in bytes */
    OMX_VERSIONTYPE nVersion;   /**< OMX specification version information */
    OMX_U32 nPortIndex;         /**< Port that this structure applies to */
    OMX_SYMBIAN_FOCUSREGIONCONTROL eFocusRegionControl; /**< Focus region selection */
    OMX_SYMBIAN_RELATIVERECTTYPE sFocusRegion;          /**< Focus region if focus region control is manual */
} OMX_SYMBIAN_CONFIG_FOCUSREGIONTYPE;

/**
 * Enumeration of possible camera self tests
 */
typedef enum OMX_SYMBIAN_CAMERASELFTESTTYPE
{
    OMX_SYMBIAN_CameraSelfTestNone = 0,
    OMX_SYMBIAN_CameraSelfTestSensor,                /**< Verifies command and data bus connections of camera sensor */
    OMX_SYMBIAN_CameraSelfTestHwa,                   /**< Verifies command bus and data bus connections between host and camera HWA */
    OMX_SYMBIAN_CameraSelfTestFocus,                 /**< Verifies that focus lens is able to move. This test typically requires HW's 
                                                                ability to measure lens position. Test can be done e.g. by moving lens to 
                                                                two different positions and reading back the lens position */

    OMX_SYMBIAN_CamearSelfTestOpticalZoom,           /**< Verifies that optical zoom lens is able to move. This test typically requires 
                                                                HW's ability to measure lens position. Test can be done e.g. by moving lens to 
                                                                two different positions and reading back the lens position */
    OMX_SYMBIAN_CameraSelfTestFlashDriverPrimaryL,   /**< tests connections to primary flash driver IC */
    OMX_SYMBIAN_CameraSelfTestFlashDriverSecondaryL, /**< tests connections to secondary flash driver IC */
    OMX_SYMBIAN_CameraSelfTestFlashL,                /**< tests capture flash without using strobe signal from camera */
    OMX_SYMBIAN_CameraSelfTestFlashWithStrobe,       /**< tests capture flash using strobe signal from camera: ONLY 
                                                                this one needs to be done in idle state from flash tests cases */
    OMX_SYMBIAN_CameraSelfTestVideoLightL,           /**< tests video light */
    OMX_SYMBIAN_CameraSelfTestAfLightL,              /**< tests AF assistance light */
    OMX_SYMBIAN_CameraSelfTestIndicatorLightL,       /**< tests capture indicator light */
    OMX_SYMBIAN_CameraSelfTestFlashTorchL,           /**< tests flash in torch mode */
    OMX_SYMBIAN_CameraSelfTestHwaControl,	          /**< Verifies HWA command bus connection */
    OMX_SYMBIAN_CameraSelfTestHwaOTP,		          /**< Sanity check for HWA OTP programming */
    OMX_SYMBIAN_CameraSelfTestHwaSDRAM,	          /**< Verifies HWA SDRAM bus connections */
    OMX_SYMBIAN_CameraSelfTestHwaSleep,		      /**< Verifies HWA ability to sleep/wakeup */
    OMX_SYMBIAN_CameraSelfTestHwaPmControl,	      /**< Verifies HWA power managment control */
    OMX_SYMBIAN_CameraSelfTestSensorControl,	      /**< Verifies sensor command bus connection */
    OMX_SYMBIAN_CameraSelfTestSensorNVM,	          /**< Verifies sensor non-volatile memory is readable */
    OMX_SYMBIAN_CameraSelfTestLensDrvControl,	      /**< Verifies lens driver IC command bus connection */
    OMX_SYMBIAN_CameraSelfTestShutter,		          /**< Verifies shutter operation */
    OMX_SYMBIAN_CameraSelfTestNDFilter,		      /**< Verifies ND filter */
    OMX_SYMBIAN_CameraSelfTestGlobalReset,	          /**< Verifies global reset signal (from camera to host) connection */
    OMX_SYMBIAN_CameraSelfTestMax = 0x7FFFFFFF
} OMX_SYMBIAN_CAMERASELFTESTTYPE;

/**
 * Self test type and result struct
 */
typedef struct OMX_SYMBIAN_CAMERASELFTESTRESULTTYPE {
    OMX_SYMBIAN_CAMERASELFTESTTYPE eTestId;   /**< Test id */
    OMX_ERRORTYPE eResult;                         /**< Test result */
} OMX_SYMBIAN_CAMERASELFTESTRESULTTYPE;

#define OMX_SYMBIAN_MAX_SELF_TESTS_PER_REQUEST 20

/**
 * Self test select config 
 */
typedef struct OMX_SYMBIAN_CONFIG_CAMERASELFTESTSELECTTYPE { 
    OMX_U32 nSize;              /**< Size of the structure in bytes */
    OMX_VERSIONTYPE nVersion;   /**< OMX specification version information */
    OMX_U32 nPortIndex;         /**< Port that this structure applies to */
    OMX_U32 nFlashHwFaultRegister1;
    OMX_U32 nFlashHwFaultRegister2;
    OMX_U16 nTestSelectMaxSizeUsed;
    OMX_SYMBIAN_CAMERASELFTESTRESULTTYPE eSelfTests[OMX_SYMBIAN_MAX_SELF_TESTS_PER_REQUEST]; /**< Self test results */
} OMX_SYMBIAN_CONFIG_CAMERASELFTESTSELECTTYPE; 

/**
 * Enumeration of functional tests
 */
typedef enum OMX_SYMBIAN_CAMERAHWFUNCTEST {
    OMX_SYMBIAN_CameraFuncTestNone = 0,
    OMX_SYMBIAN_CameraFuncTestDeadPixels,
    OMX_SYMBIAN_CameraFuncTestCouplet,
    OMX_SYMBIAN_CameraFuncTestBrightness,
    OMX_SYMBIAN_CameraFuncTestVignetting,
    OMX_SYMBIAN_CameraFuncTestNdFilter,
    OMX_SYMBIAN_CameraFuncTestFocus,
    OMX_SYMBIAN_CameraFuncTestIris,
    OMX_SYMBIAN_CameraFuncTestFlash,
    OMX_SYMBIAN_CameraFuncTestIndicatorLed,
    OMX_SYMBIAN_CameraFuncTestVideoLight,
    OMX_SYMBIAN_CameraFuncTestBlemish,
    OMX_SYMBIAN_CameraFuncTestShutter,
    OMX_SYMBIAN_CameraFuncTestMax = 0x7FFFFFFF
} OMX_SYMBIAN_CAMERAHWFUNCTEST;

#define OMX_SYMBIAN_MAX_FUNC_TESTS_PER_REQUEST 20

/**
 * Functional test selection config
 */
typedef struct OMX_SYMBIAN_CONFIG_CAMERAHWFUNCTESTSELECTTYPE { 
    OMX_U32 nSize;                  /**< Size of the structure in bytes */
    OMX_VERSIONTYPE nVersion;       /**< OMX specification version information */
    OMX_U32 nPortIndex;             /**< Port that this structure applies to */
    OMX_U16 nTestSelectMaxSizeUsed; /**< Number of entries in eFuncTests */
    OMX_SYMBIAN_CAMERAHWFUNCTEST eFuncTests[OMX_SYMBIAN_MAX_FUNC_TESTS_PER_REQUEST]; /**< Array of selected tests */ 
    OMX_BOOL bTestImageOutput;      /**< If set OMX_TRUE, NRW format test image is outputted in addition to test results */
}OMX_SYMBIAN_CONFIG_CAMERAHWFUNCTESTSELECTTYPE;

/**
 * Enumeration of scene modes
 */
typedef enum OMX_SYMBIAN_SCENEMODETYPE {
    OMX_SYMBIAN_SceneAuto = 0,      /**< Camera settings automatically selected by camera */
    OMX_SYMBIAN_ScenePortrait,      /**< Use camera settings suitable for taking portrait images */
    OMX_SYMBIAN_SceneLandscape,     /**< Use camera settings suitable for taking landscae images */
    OMX_SYMBIAN_SceneNight,         /**< Use camera settings suitable for taking low light images */
    OMX_SYMBIAN_SceneNightPortrait, /**< Use camera settings suitable for taking portrait images in low light */
    OMX_SYMBIAN_SceneSport,         /**< Use camera settings suitable for taking images when there is fast movement in the scene */
    OMX_SYMBIAN_SceneMacro,         /**< Use camera settings suitable for taking close-up images */
    OMX_SYMBIAN_SceneMax = 0x7FFFFFFF
} OMX_SYMBIAN_SCENEMODETYPE;

/**
 * Scene mode config
 */
typedef struct OMX_SYMBIAN_CONFIG_SCENEMODETYPE {
    OMX_U32 nSize;              /**< Size of the structure in bytes */
    OMX_VERSIONTYPE nVersion;   /**< OMX specification version information */
    OMX_U32 nPortIndex;         /**< Port that this structure applies to */
    OMX_SYMBIAN_SCENEMODETYPE eSceneType; /**< Scene mode selection */
} OMX_SYMBIAN_CONFIG_SCENEMODETYPE;

/**
 * RGB histogram returned using this config
 */
typedef struct OMX_SYMBIAN_CONFIG_RGBHISTOGRAM {
    OMX_U32 nSize;              /**< Size of the structure in bytes */
    OMX_VERSIONTYPE nVersion;   /**< OMX specification version information */
    OMX_U32 nPortIndex;         /**< Port that this structure applies to */
    OMX_U32 nRed[256];          /**< Histogram for red color */
    OMX_U32 nGreen[256];        /**< Histogram for green color */
    OMX_U32 nBlue[256];         /**< Histogram for blue color */
} OMX_SYMBIAN_CONFIG_RGBHISTOGRAM;

/**
 * Histogram control config
 */
typedef struct OMX_SYMBIAN_CONFIG_HISTOGRAMCONTROLTYPE {
    OMX_U32 nSize;              /**< Size of the structure in bytes */
    OMX_VERSIONTYPE nVersion;   /**< OMX specification version information */
    OMX_U32 nPortIndex;         /**< Port that this structure applies to */
    OMX_BOOL bMeasure;          /**< Set OMX_TRUE to start histogram measurement */
    OMX_U32 nBins;              /**< Number of bins */
    OMX_U32 nBytesPerBin;       /**< Number of bytes / bin */
} OMX_SYMBIAN_CONFIG_HISTOGRAMCONTROLTYPE;

/**
 * Region of interest selection config 
 */
typedef struct OMX_SYMBIAN_CONFIG_ROISELECTIONTYPE {
    OMX_U32 nSize;                   /**< Size of the structure in bytes */
    OMX_VERSIONTYPE nVersion;        /**< OMX specification version information */
    OMX_U32 nPortIndex;              /**< Port that this structure applies to */
    OMX_BOOL bReset;                 /**< OMX_TRUE = reset all selections done. Rest of the fields are not used */ 
    OMX_BOOL bSelect;                /**< OMX_TRUE = select / OMX_FALSE = deselect */
    OMX_SYMBIAN_RELATIVERECTTYPE sROI;     /**< Indicates the region where the object to be selected is located in the scene */
    OMX_U32 nROIID;                              /**< This field can contain the ID of object in order to assist  
                                                      object selection. 0 means undefined and is the default value. */
    OMX_SYMBIAN_ROIOBJECTTYPE eObjectType; /**< Indicates the type of object to be selected. */
} OMX_SYMBIAN_CONFIG_ROISELECTIONTYPE;

/**
 * Classification of algorithm comlexity values
 */
typedef enum OMX_SYMBIAN_ALGORITHMCOMPLEXITYTYPE {
    OMX_SYMBIAN_AlgComplexityLow,
    OMX_SYMBIAN_AlgComplexityMedium,
    OMX_SYMBIAN_AlgComplexityHigh,
    OMX_SYMBIAN_AlgComplexityMax = 0x7FFFFFFF
} OMX_SYMBIAN_ALGORITHMCOMPLEXITYTYPE;

/**
 * Algorithm complexity config
 */
typedef struct OMX_SYMBIAN_CONFIG_ALGORITHMCOMPLEXITYTYPE {
    OMX_U32 nSize;              /**< Size of the structure in bytes */
    OMX_VERSIONTYPE nVersion;   /**< OMX specification version information */
    OMX_U32 nPortIndex;         /**< Port that this structure applies to */
    OMX_SYMBIAN_ALGORITHMCOMPLEXITYTYPE eComplexity; /**< Algorithm complexity to use */
} OMX_SYMBIAN_CONFIG_ALGORITHMCOMPLEXITYTYPE;

/** Color format extensions. */
typedef enum OMX_SYMBIAN_COLOR_FORMATTYPE {
    OMX_SYMBIAN_COLOR_FormatYUV420MBPackedSemiPlanar = OMX_COLOR_FormatVendorStartUnused + OMX_SYMBIAN_EXTENSION_START_OFFSET,
    OMX_SYMBIAN_COLOR_FormatRawBayer12bit,
    OMX_SYMBIAN_COLOR_FormatMax = 0x7FFFFFFF
} OMX_SYMBIAN_COLOR_FORMATTYPE;

/** Primary color enumeration. */
typedef enum OMX_SYMBIAN_COLORPRIMARYTYPE {
    OMX_SYMBIAN_ColorPrimaryFullRange,
    OMX_SYMBIAN_ColorPrimaryBT601,
    OMX_SYMBIAN_ColorPrimaryBT709,
    OMX_SYMBIAN_ColorMax = 0x7FFFFFFF
} OMX_SYMBIAN_COLORPRIMARYTYPE;

/** YUV data color range.

The index specified for this structure is retrieved using 
OMX_GetExtensionIndex() with the extension string 
"OMX.Symbian.Index.Param.Common.ColorPrimary".
*/
typedef struct OMX_SYMBIAN_PARAM_COLORPRIMARYTYPE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_SYMBIAN_COLORPRIMARYTYPE eColorPrimary;
} OMX_SYMBIAN_PARAM_COLORPRIMARYTYPE;


/** Pixel Aspect Ratio.

The index specified for this structure is retrieved using 
OMX_GetExtensionIndex() with the extension string 
"OMX.Symbian.Index.Param.Common.PixelAspectRatio".
*/
typedef struct OMX_SYMBIAN_PARAM_PIXELASPECTRATIOTYPE
{
    OMX_U32 nSize;              /**< Size of the structure in bytes */
    OMX_VERSIONTYPE nVersion;   /**< OMX specification version information */
    OMX_U32 nPortIndex;         /**< Port that this structure applies to */
    OMX_U8 nHorizontal;         /**< Width of pixel. nHorizontal is 4 is For 4:3 PAR */
    OMX_U8 nVertical;           /**< Height of pixel. nVertical is 3 is For 4:3 PAR */
    OMX_TICKS nTimestamp;       /**< Time when PAR change should happen */
} OMX_SYMBIAN_PARAM_PIXELASPECTRATIOTYPE;


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* OMX_Symbian_IVCommonExt_h */
