#ifndef __EXIFTAGDESCRIPTIONS_H__
#define __EXIFTAGDESCRIPTIONS_H__

#include "ExifGeneralConsts.h"

// Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// ExifTag class defines the Tags found in the Exif specification.
// Comments are arranged thus:
// Tag/Format/ComponentCount/R(equired if hardware supports it),O(ptional),M(andatory)/Default value/Tag name/Tag description.
// 
//

/**
 @file
 @internalComponent 
*/

// 0th IFD (Primary image associated).						
const TUint KTag010E[6] = {0x010E, EAscii, 0x00000000, 0x00000000, EZeroth, EConditionallyMandatory}; //0x010E,2,any,R,none,ImageDescription,A character string giving the title of the image.
const TUint KTag010F[6] = {0x010F, EAscii, 0x00000000, (TUint) _S8("Symbian"), EZeroth, EMandatory}; //0x010F,2,any,M,none,Make,The manufacturer of the recording equipment.
const TUint KTag0110[6] = {0x0110, EAscii, 0x00000000, (TUint) _S8("OS 9.1"), EZeroth, EMandatory}; //0x0110,2,any,M,none,Model,The model name or model number of the equipment.
const TUint KTag0112[6] = {0x0112, EUnsignedShort, 0x00000001, 0x00000001, EZeroth, EConditionallyMandatory}; //0x0112,3,1,R,1,Orientation,"1-8. 1 = 0th row is at visual top of image, 0th column is at visual lh side. Etc"
const TUint KTag011A[6] = {0x011A, EUnsignedRational, 0x00000001, 0x00000048, EZeroth, EMandatory}; //0x011A,5,1,M,72,Xresolution,n/a (pixel no. per ResolutionUnit in the ImageWidth direction).
const TUint KTag011B[6] = {0x011B, EUnsignedRational, 0x00000001, 0x00000048, EZeroth, EMandatory}; //0x011B,5,1,M,72,Yresolution,n/a (pixel no. per ResolutionUnit in the ImageLength direction).
const TUint KTag0128[6] = {0x0128, EUnsignedShort, 0x00000001, 0x00000002, EZeroth, EMandatory}; //0x0128,3,1,M,2,ResolutionUnit,"Units of resolution as used by Xresolution and YResolution (2 = inches, 3 = cm)."
const TUint KTag012D[6] = {0x012D, EUnsignedShort, 0x00000000, 0x00000000, EZeroth, EConditionallyMandatory}; //0x012D,3,3*256,R,none,TransferFunction,transfer function for the image - not normally necessary
const TUint KTag0131[6] = {0x0131, EAscii, 0x00000000, 0x00000000, EZeroth, EOptional}; //0x0131,2,any,O,none,Software,"Records the name and version of the software/firmware of the camera or image input device used to generate the image. Eg ""Exif Software Version 1.00a\0"""
const TUint KTag0132[6] = {0x0132, EAscii, 0x00000014, 0x00000000, EZeroth, EConditionallyMandatory}; //0x0132,2,20,R,none,DateTime,"none (format is ""YYYY:MM:DD HH:MM:SS\0"" when unknown can be ""    :  :     :  :  \0"")."
const TUint KTag013B[6] = {0x013B, EAscii, 0x00000000, 0x00000000, EZeroth, EOptional}; //0x013B,2,any,O,none,Artist,"Records the name of the camera owner, photgrapher or image creator. Eg ""Camera owner, John Smith; Photographer, Michael Brown; Image Creator, Ken James\0"""
const TUint KTag013E[6] = {0x013E, EUnsignedRational, 0x00000002, 0x00000000, EZeroth, EOptional}; //0x013E,5,2,O,none,WhitePoint,The chromaticity of the white point of the image
const TUint KTag013F[6] = {0x013F, EUnsignedRational, 0x00000006, 0x00000000, EZeroth, EOptional}; //0x013F,5,6,O,none,PrimaryChromaticies,The chromaticity of the three primary colours of the image
const TUint KTag0211[6] = {0x0211, EUnsignedRational, 0x00000003, 0x00000000, EZeroth, EOptional}; //0x0211,5,3,O,none,YCbCrCoefficients,The matrix coefficients for transformation from RGB to YCbCr
const TUint KTag0213[6] = {0x0213, EUnsignedShort, 0x00000001, 0x00000000, EZeroth, EMandatory}; //0x0213,3,1,M,1,YCbCrPositioning,"1 = centered, 2 = co-sited."
const TUint KTag0214[6] = {0x0214, EUnsignedRational, 0x00000006, 0x00000000, EZeroth, EOptional}; //0x0214,5,6,O,"[0,255,0,255,0,255] when PhotometricInterpretation is RGB), [0,255,0,128,0,128] when PhotometricInterpretation is YCbCr",ReferenceBlackWhite,"the reference black point value and reference white point value. dependant upon non-JPEG data, so should it be included?"
const TUint KTag8298[6] = {0x8298, EAscii, 0x00000000, 0x00000000, EZeroth, EOptional}; //0x8298,2,any,O,none,Copyright,Copyright Information.
const TUint KTag8769[6] = {0x8769, EUnsignedLong, 0x00000001, 0x00000000, EZeroth, EMandatory}; //0x8769,4,1,M,none,Pointer to Exif IFD.,n/a
const TUint KTag8825[6] = {0x8825, EUnsignedLong, 0x00000001, 0x00000000, EZeroth, EOptional}; //0x8825,4,1,O,none,Pointer to GPS Info IFD.,n/a

// Exif Sub IFD
const TUint KTag829A[6] = {0x829A, EUnsignedRational, 0x00000001, 0x00000000, EExifSub, EConditionallyMandatory}; //0x829A,5,1,R,none,ExposureTime,Exposure time in seconds.
const TUint KTag829D[6] = {0x829D, EUnsignedRational, 0x00000001, 0x00000000, EExifSub, EOptional}; //0x829D,5,1,O,none,Fnumber,The F number.
const TUint KTag8822[6] = {0x8822, EUnsignedShort, 0x00000001, 0x00000000, EExifSub, EOptional}; //0x8822,3,1,O,0,ExposureProgram,"The class of the program used by the camera to set exposure when the picture is taken. 0,1,2,3,4,5,6,7,8"
const TUint KTag8824[6] = {0x8824, EAscii, 0x00000000, 0x00000000, EExifSub, EOptional}; //0x8824,2,any,O,none,SpectralSensitivity,The spectral sensitivity of each channel of the camera used.
const TUint KTag8827[6] = {0x8827, EUnsignedShort, 0x00000000, 0x00000000, EExifSub, EOptional}; //0x8827,3,any,O,none,ISOSpeedRatings,Indicates the ISO Speed and ISO Latitude of the camera or input device as specified in IDS 12232.
const TUint KTag8828[6] = {0x8828, EUndefined, 0x00000000, 0x00000000, EExifSub, EOptional}; //0x8828,7,any,O,none,OECF,Opto-Electric Conversion Function specified in ISO 14524.
const TUint KTag9000[6] = {0x9000, EUndefined, 0x00000004, 0x30323230, EExifSub, EMandatory}; //0x9000,7,4,M,0220 - no null (in 2.2) or 0210 (in 2.1).,ExifVersion,The version of this standard supported. Non-existence = nonconformance.
const TUint KTag9003[6] = {0x9003, EAscii, 0x00000014, (TUint) _S8("    :  :     :  :  "), EExifSub, EMandatory}; //0x9003,2,20,M,none,DateTimeOriginal,"Date and time when the orginal image data was generated. (format is ""YYYY:MM:DD HH:MM:SS\0"" when unknown can be ""    :  :     :  :  \0"")."
const TUint KTag9004[6] = {0x9004, EAscii, 0x00000014, (TUint) _S8("    :  :     :  :  "), EExifSub, EMandatory}; //0x9004,2,20,M,none,DateTimeDigitized,"Date and time when the image was stored as digital data. (format is ""YYYY:MM:DD HH:MM:SS\0"" when unknown can be ""    :  :     :  :  \0"")."
const TUint KTag9101[6] = {0x9101, EUndefined, 0x00000004, 0x00030201, EExifSub, EMandatory}; //0x9101,7,4,M,"4 5 6 0 (if RGB uncompressed - 4=R,5=G,6=B), 1 2 3 0 (other cases - 1=Y,2=Cb,3=Cr).",ComponentsConfiguration,"When compressed, the channels of each component are arranged in order from the 1st component to the 4th."
const TUint KTag9102[6] = {0x9102, EUnsignedRational, 0x00000001, 0x00000000, EExifSub, EOptional}; //0x9102,5,1,O,none,CompressedBitsPerPixel,"When compressed, the compression mode used for a compressed image is indicated in unit bits per pixel."
const TUint KTag9201[6] = {0x9201, ESignedRational, 0x00000001, 0x00000000, EExifSub, EOptional}; //0x9201,10,1,O,none,ShutterSpeedValue,Shutter speed. Unit is APEX setting.
const TUint KTag9202[6] = {0x9202, EUnsignedRational, 0x00000001, 0x00000000, EExifSub, EOptional}; //0x9202,5,1,O,none,ApertureValue,Lens aperture. The unit is the APEX value.
const TUint KTag9203[6] = {0x9203, ESignedRational, 0x00000001, 0x00000000, EExifSub, EOptional}; //0x9203,10,1,O,none,BrightnessValue,The value of brightness. The unit is the APEX value. Normally range is -99.99 to 99.99.
const TUint KTag9204[6] = {0x9204, ESignedRational, 0x00000001, 0x00000000, EExifSub, EOptional}; //0x9204,10,1,O,none,ExposureBiasValue,The exposure bias. The unit is the APEX value. Normally range is -99.99 to 99.99.
const TUint KTag9205[6] = {0x9205, EUnsignedRational, 0x00000001, 0x00000000, EExifSub, EOptional}; //0x9205,5,1,O,none,MaxApertureValue,The smallest F number of the lens. The unit is the APEX value. Normally range is -99.99 to 99.99.
const TUint KTag9206[6] = {0x9206, EUnsignedRational, 0x00000001, 0x00000000, EExifSub, EOptional}; //0x9206,5,1,O,none,SubjectDistance,"The distance to the subject, given in meters. If numerator is 0xFFFFFFFF = infinity, if numerator is 0, distance is unknown."
const TUint KTag9207[6] = {0x9207, EUnsignedShort, 0x00000001, 0x00000000, EExifSub, EOptional}; //0x9207,3,1,O,0,MeteringMode,"The metering mode. values = 0,1,2,3,4,5,6."
const TUint KTag9208[6] = {0x9208, EUnsignedShort, 0x00000001, 0x00000000, EExifSub, EOptional}; //0x9208,3,1,O,"0 (unknown. others = 1,2,3,17,18,19,20,21,22,23-254,255).",LightSource,"The kind of light source. Values = 0,1,2,3,4,9,10,11,12,13,14,15,18,19,20,21,22,23,24,255"
const TUint KTag9209[6] = {0x9209, EUnsignedShort, 0x00000001, 0x00000000, EExifSub, EConditionallyMandatory}; //0x9209,3,1,R,none,Flash,8 bits used to reflect flash use - see spec for details.
const TUint KTag920A[6] = {0x920A, EUnsignedRational, 0x00000001, 0x00000000, EExifSub, EOptional}; //0x920A,5,1,O,none,FocalLength,"Focal length of the lens, in mm."
const TUint KTag9214[6] = {0x9214, EUnsignedShort, 0x00000000, 0x00000000, EExifSub, EOptional}; //0x9214,3,2 or 3 or 4,O,none,SubjectArea,Location and area of the main subject in the overall scene.
const TUint KTag927C[6] = {0x927C, EUndefined, 0x00000000, 0x00000000, EExifSub, EOptional}; //0x927C,7,any,O,none,MakerNote,Tag for manufacturers of Exif writers to record required data.
const TUint KTag9286[6] = {0x9286, EUndefined, 0x00000000, 0x00000000, EExifSub, EOptional}; //0x9286,7,any,O,none,UserComment,"Tag for Exif users to write keywords or comments on the image.  (4 ID codes - ASCII, JIS, UNICODE, undefined)"
const TUint KTag9290[6] = {0x9290, EAscii, 0x00000000, 0x00000000, EExifSub, EOptional}; //0x9290,2,any,O,none,SubsecTime,Record fractions of seconds for the DateTime tag.
const TUint KTag9291[6] = {0x9291, EAscii, 0x00000000, 0x00000000, EExifSub, EOptional}; //0x9291,2,any,O,none,SubsecTimeOriginal,Record fractions of seconds for the DateTimeOriginal tag.
const TUint KTag9292[6] = {0x9292, EAscii, 0x00000000, 0x00000000, EExifSub, EOptional}; //0x9292,2,any,O,none,SubsecTimeDigitized,Record fractions of seconds for the DateTimeDigitized tag.
const TUint KTagA000[6] = {0xA000, EUndefined, 0x00000004, 0x30303130, EExifSub, EMandatory}; //0xA000,7,4,M,0100 - no null,FlashpixVersion,Flashpix format version.
const TUint KTagA001[6] = {0xA001, EUnsignedShort, 0x00000001, 0x0000FFFF, EExifSub, EMandatory}; //0xA001,3,1,M,"1 = sRGB, FFFF = uncalibrated.",ColorSpace,The colour space specifier.
const TUint KTagA002[6] = {0xA002, EUnsignedLong, 0x00000001, 0x00000000, EExifSub, EMandatory}; //0xA002,3 or 4,1,M,none,PixelXDimension,"When compressed, the valid width of the meaningful image."
const TUint KTagA003[6] = {0xA003, EUnsignedLong, 0x00000001, 0x00000000, EExifSub, EMandatory}; //0xA003,3 or 4,1,M,none,PixelYDimension,"When compressed, the valid height of the meaningful image."
const TUint KTagA004[6] = {0xA004, EAscii, 0x0000000D, 0x00000000, EExifSub, EOptional}; //0xA004,2,13,O,none,RelatedSoundFile,"Sound file name and extension - ""XXXXXXXX.EXT\0"""
const TUint KTagA005[6] = {0xA005, EUnsignedLong, 0x00000001, 0x00000000, EExifSub, EMandatory}; //0xA005,4,1,M,none,Interoperability IFD pointer,
const TUint KTagA20B[6] = {0xA20B, EUnsignedRational, 0x00000001, 0x00000000, EExifSub, EOptional}; //0xA20B,5,1,O,none,FlashEnergy,"The strobe energy at the time the image is captured, in BCPS."
const TUint KTagA20C[6] = {0xA20C, EUndefined, 0x00000000, 0x00000000, EExifSub, EOptional}; //0xA20C,7,any,O,none,SpatialFrequencyResponse,"Spatial frequency table and SFR values in the direction of image width, height and diagonal direction, as in ISO 12233."
const TUint KTagA20E[6] = {0xA20E, EUnsignedRational, 0x00000001, 0x00000000, EExifSub, EOptional}; //0xA20E,5,1,O,none,FocalPlaneXResolution,Number of pixels in the image width direction per FocalPlaneResolutionUnit on the the camera focal plane.
const TUint KTagA20F[6] = {0xA20F, EUnsignedRational, 0x00000001, 0x00000000, EExifSub, EOptional}; //0xA20F,5,1,O,none,FocalPlaneYResolution,Number of pixels in the image height direction per FocalPlaneResolutionUnit on the the camera focal plane.
const TUint KTagA210[6] = {0xA210, EUnsignedShort, 0x00000001, 0x00000002, EExifSub, EOptional}; //0xA210,3,1,O,2,FocalPlaneResolutionUnit,Unit for measuring FocalPlaneXResolution and FocalPlaneYResolution. Value is the same as Resolution Unit.
const TUint KTagA214[6] = {0xA214, EUnsignedShort, 0x00000002, 0x00000000, EExifSub, EOptional}; //0xA214,3,2,O,none,SubjectLocation,Location of main subject in the scene.
const TUint KTagA215[6] = {0xA215, EUnsignedRational, 0x00000001, 0x00000000, EExifSub, EOptional}; //0xA215,5,1,O,none,ExposureIndex,Exposure index selected on the camera/input device at the time the image is captured.
const TUint KTagA217[6] = {0xA217, EUnsignedShort, 0x00000001, 0x00000000, EExifSub, EOptional}; //0xA217,3,1,O,none,SensingMethod,"The image sensor on the camera/input device.   1 = not defined, 2,3,4,5,7,8"
const TUint KTagA300[6] = {0xA300, EUndefined, 0x00000001, 0x00000003, EExifSub, EOptional}; //0xA300,7,1,O,3,FileSource,"The image source. If DSC recorded the image, this value is 3."
const TUint KTagA301[6] = {0xA301, EUndefined, 0x00000001, 0x00000001, EExifSub, EOptional}; //0xA301,7,1,O,1,SceneType,The type of scene.  If DSC recorded the image value is 1.
const TUint KTagA302[6] = {0xA302, EUndefined, 0x00000000, 0x00000000, EExifSub, EOptional}; //0xA302,7,any,O,none,CFAPattern,Color Filter Array geometric pattern of the image sensor
const TUint KTagA401[6] = {0xA401, EUnsignedShort, 0x00000001, 0x00000000, EExifSub, EOptional}; //0xA401,3,1,O,0,CustomRendered,"Indicates the use of special processing on image data, such as rendering geared to output. Values are 0 (normal process) or 1 (custom process)."
const TUint KTagA402[6] = {0xA402, EUnsignedShort, 0x00000001, 0x00000000, EExifSub, EConditionallyMandatory}; //0xA402,3,1,R,none,ExposureMode,"The exposure mode set when the image was shot. Values = 0 (auto exposure), 1 (Manual exposure), 2 (Auto bracket)."
const TUint KTagA403[6] = {0xA403, EUnsignedShort, 0x00000001, 0x00000000, EExifSub, EConditionallyMandatory}; //0xA403,3,1,R,none,WhiteBalance,"The white balance mode when image was shot. Values = 0 (Auto white balance), 1 (Manual white balance)."
const TUint KTagA404[6] = {0xA404, EUnsignedRational, 0x00000001, 0x00000000, EExifSub, EOptional}; //0xA404,5,1,O,none,DigitalZoomRatio,"The digital zoom ratio when the image was shot. If numerator is 0, digital zoom not used."
const TUint KTagA405[6] = {0xA405, EUnsignedShort, 0x00000001, 0x00000000, EExifSub, EOptional}; //0xA405,3,1,O,none,FocalLengthIn35mmFilm,"Equivalent focal length assuming a 35mm film camera, in mm. value of 0 = focal length is unknown. Differs to FocalLength tag."
const TUint KTagA406[6] = {0xA406, EUnsignedShort, 0x00000001, 0x00000000, EExifSub, EConditionallyMandatory}; //0xA406,3,1,R,0,ScenceCaptureType,"The type of scene shot. Differs to SceneType tag. Values = 0 (Standard), 1 (Landscape), 2 (Portrait), 3 (Night Scene)"
const TUint KTagA407[6] = {0xA407, EUnsignedShort, 0x00000001, 0x00000000, EExifSub, EOptional}; //0xA407,3,1,O,none,GainControl,"Degree of overall image gain adjustment. Values: 0 (none), 1 (Low gain up), 2 (High gain up), 3 (Low gain down), 4 (High gain down)."
const TUint KTagA408[6] = {0xA408, EUnsignedShort, 0x00000001, 0x00000000, EExifSub, EOptional}; //0xA408,3,1,O,0,Contrast,"Direction of contrast processing applied by the camera when the image was shot. Values: 0 (Normal), 1 (Soft), 2 (Hard)."
const TUint KTagA409[6] = {0xA409, EUnsignedShort, 0x00000001, 0x00000000, EExifSub, EOptional}; //0xA409,3,1,O,0,Saturation,"Direction of saturation processing applied by the camera when the image was shot. Values: 0 (Normal), 1 (Low saturation), 2 (High saturation)."
const TUint KTagA40A[6] = {0xA40A, EUnsignedShort, 0x00000001, 0x00000000, EExifSub, EOptional}; //0xA40A,3,1,O,0,Sharpness,"Direction of sharpness processing applied by the camera when the image was shot. Values: 0 (Normal), 1 (Soft), 2 (Hard)."
const TUint KTagA40B[6] = {0xA40B, EUndefined, 0x00000000, 0x00000000, EExifSub, EOptional}; //0xA40B,7,any,O,none,DeviceSettingDescription,Information on the picture-taking conditions of a particular camera model.
const TUint KTagA40C[6] = {0xA40C, EUnsignedShort, 0x00000001, 0x00000000, EExifSub, EOptional}; //0xA40C,3,1,O,none,SubjectDistanceRange,"Distance to the subject. Values: 0 (unknown), 1 (Macro), 2 (Close view), 3 (Distant view)"
const TUint KTagA420[6] = {0xA420, EAscii, 0x00000021, 0x00000000, EExifSub, EOptional}; //0xA420,2,33,O,none,ImageUniqueID,"Identifier assigned uniquely to each image, recorded in ASCII."
const TUint KTagA500[6] = {0xA500, EUnsignedRational, 0x00000001, 0x00000000, EExifSub, EOptional}; //0xA500,5,1,O,0x16/0x0A,Gamma

// Interoperability IFD
const TUint KTag0001[6] = {0x0001, EAscii, 0x00000004, (TUint) _S8("R98"), EInterop, EMandatory}; //0x0001,2,any,M,"R98\0",InteroperabilityIndex,
const TUint KTag0002[6] = {0x0002, EUndefined, 0x00000004, 0x30303130, EInterop, EMandatory}; //0x0002,7,4,M,"0100",InteroperabilityVersion,
const TUint KTag1000[6] = {0x1000, EAscii, 0x00000000, 0x00000000, EInterop, EOptional}; //0x1000,2,any,O,none,RelatedImageFileFormat,
const TUint KTag1001[6] = {0x1001, EUnsignedLong, 0x00000000, 0x00000000, EInterop, EOptional}; //0x1001,4,any,O,none,RelatedImageWidth,
const TUint KTag1002[6] = {0x1002, EUnsignedLong, 0x00000000, 0x00000000, EInterop, EOptional}; //0x1002,4,any,O,none,RelatedImageLength,

// 1st IFD (Thumbnail associated)
const TUint KThumbTag0103[6] = {0x0103, EUnsignedShort, 0x00000001, 0x00000006, EFirst, EMandatory}; //0x0103,3,1,M,none,Compression,6 = thumb is JPEG compression. 1 = uncompressed. Not used in primary image
const TUint KThumbTag010E[6] = {0x010E, EAscii, 0x00000000, 0x00000000, EFirst, EOptional}; //0x010E,2,any,O,none,ImageDescription,A charcter string giving the title of the image.
const TUint KThumbTag010F[6] = {0x010F, EAscii, 0x00000000, 0x00000000, EFirst, EOptional}; //0x010F,2,any,O,none,Make,The manufacturer of the recording equipment.
const TUint KThumbTag0110[6] = {0x0110, EAscii, 0x00000000, 0x00000000, EFirst, EOptional}; //0x0110,2,any,O,none,Model,The model name or model number of the equipment.
const TUint KThumbTag0112[6] = {0x0112, EUnsignedShort, 0x00000001, 0x00000001, EFirst, EOptional}; //0x0112,3,1,O,1,Orientation,"1-8. 1 = 0th row is at visual top of image, 0th column is at visual lh side. Etc"
const TUint KThumbTag011A[6] = {0x011A, EUnsignedRational, 0x00000001, 0x00000048, EFirst, EMandatory}; //0x011A,5,1,M,72,Xresolution,n/a (pixel no. per ResolutionUnit in the ImageWidth direction).
const TUint KThumbTag011B[6] = {0x011B, EUnsignedRational, 0x00000001, 0x00000048, EFirst, EMandatory}; //0x011B,5,1,M,72,Yresolution,n/a (pixel no. per ResolutionUnit in the ImageLength direction).
const TUint KThumbTag0128[6] = {0x0128, EUnsignedShort, 0x00000001, 0x00000002, EFirst, EMandatory}; //0x0128,3,1,M,2,ResolutionUnit,"Units of resolution as used by Xresolution and YResolution (2 = inches, 3 = cm)."
const TUint KThumbTag012D[6] = {0x012D, EUnsignedShort, 0x00000000, 0x00000000, EFirst, EOptional}; //0x012D,3,3*256,O,none,TransferFunction,transfer function for the image - not normally necessary
const TUint KThumbTag0131[6] = {0x0131, EAscii, 0x00000000, 0x00000000, EFirst, EOptional}; //0x0131,2,any,O,none,Software,"Records the name and version of the software/firmware of the camera or image input device used to generate the image. Eg ""Exif Software Version 1.00a\0"""
const TUint KThumbTag0132[6] = {0x0132, EAscii, 0x00000014, 0x00000000, EFirst, EOptional}; //0x0132,2,20,O,none,DateTime,"none (format is ""YYYY:MM:DD HH:MM:SS\0"" when unknown can be ""    :  :     :  :  \0"")."
const TUint KThumbTag013B[6] = {0x013B, EAscii, 0x00000000, 0x00000000, EFirst, EOptional}; //0x013B,2,any,O,none,Artist,"Records the name of the camera owner, photgrapher or image creator. Eg ""Camera owner, John Smith; Photographer, Michael Brown; Image Creator, Ken James\0"""
const TUint KThumbTag013E[6] = {0x013E, EUnsignedRational, 0x00000002, 0x00000000, EFirst, EOptional}; //0x013E,5,2,O,none,WhitePoint,The chromaticity of the white point of the image
const TUint KThumbTag013F[6] = {0x013F, EUnsignedRational, 0x00000006, 0x00000000, EFirst, EOptional}; //0x013F,5,6,O,none,PrimaryChromaticies,The chromaticity of the three primary colours of the image
const TUint KThumbTag0201[6] = {0x0201, EUnsignedLong, 0x00000001, 0x00000000, EFirst, EMandatory}; //0x0201,4,1,,none,JPEGInterchangeFormat,Offset to the SOI of the JPEG compressed thumbnail data. not used in primary JPEG data
const TUint KThumbTag0202[6] = {0x0202, EUnsignedLong, 0x00000001, 0x00000000, EFirst, EMandatory}; //0x0202,4,1,,none,JPEGInterchangeFormatLength,Number of bytes of JPEG compressed thumbnail data. not used in primary JPEG data
const TUint KThumbTag0211[6] = {0x0211, EUnsignedRational, 0x00000003, 0x00000000, EFirst, EOptional}; //0x0211,5,3,O,,YCbCrCoefficients,The matrix coefficients for transformation from RGB to YCbCr
const TUint KThumbTag0213[6] = {0x0213, EUnsignedShort, 0x00000001, 0x00000001, EFirst, EOptional}; //0x0213,3,1,O,"1 (1 = centered, 2 = co-sited).",YCbCrPositioning,"1 = centered, 2 = co-sited."
const TUint KThumbTag0214[6] = {0x0214, EUnsignedRational, 0x00000006, 0x00000000, EFirst, EOptional}; //0x0214,5,6,O,"[0,255,0,255,0,255] when PhotometricInterpretation is RGB), [0,255,0,128,0,128] when PhotometricInterpretation is YCbCr",ReferenceBlackWhite,"the reference black point value and reference white point value. dependant upon non-JPEG data, so should it be included?"
const TUint KThumbTag8298[6] = {0x8298, EAscii, 0x00000000, 0x00000000, EFirst, EOptional}; //0x8298,2,any,O,none,Copyright,Copyright Information.
const TUint KThumbTag8769[6] = {0x8769, EUnsignedLong, 0x00000001, 0x00000000, EFirst, EOptional}; //0x8769,4,1,O,none,Pointer to Exif IFD.,n/a
const TUint KThumbTag8825[6] = {0x8825, EUnsignedLong, 0x00000001, 0x00000000, EFirst, EOptional}; //0x8825,4,1,O,none,Pointer to GPS Info IFD.,n/a

// GPS Sub IFD
const TUint KTagGPS0000[6] = {0x0000, EByte, 0x00000004, 0x00000000, EGpsSub, EOptional}; // 0x0000, GPS Version Id 
const TUint KTagGPS0001[6] = {0x0001, EAscii, 0x00000002, 0x00000000, EGpsSub, EOptional}; // 0x0001, GPS Latitude Ref
const TUint KTagGPS0002[6] = {0x0002, EUnsignedRational, 0x00000003, 0x00000000, EGpsSub, EOptional}; // 0x0002, GPS Latitude
const TUint KTagGPS0003[6] = {0x0003, EAscii, 0x00000002, 0x00000000, EGpsSub, EOptional}; // 0x0003, GPS Longitude Ref
const TUint KTagGPS0004[6] = {0x0004, EUnsignedRational, 0x00000003, 0x00000000, EGpsSub, EOptional}; // 0x0004, GPS Longitude
const TUint KTagGPS0005[6] = {0x0005, EByte, 0x00000001, 0x00000000, EGpsSub, EOptional}; // 0x0005, GPS Altitude Ref
const TUint KTagGPS0006[6] = {0x0006, EUnsignedRational, 0x00000001, 0x00000000, EGpsSub, EOptional}; // 0x0006, GPS Altitude
const TUint KTagGPS0007[6] = {0x0007, EUnsignedRational, 0x00000003, 0x00000000, EGpsSub, EOptional}; // 0x0007, GPS Time Stamp
const TUint KTagGPS0008[6] = {0x0008, EAscii, 0x00000000, 0x00000000, EGpsSub, EOptional}; // 0x0008, GPS Satellites
const TUint KTagGPS0009[6] = {0x0009, EAscii, 0x00000002, 0x00000000, EGpsSub, EOptional}; // 0x0009, GPS Status
const TUint KTagGPS000A[6] = {0x000A, EAscii, 0x00000002, 0x00000000, EGpsSub, EOptional}; // 0x000A, GPS Measure Mode
const TUint KTagGPS000B[6] = {0x000B, EUnsignedRational, 0x00000001, 0x00000000, EGpsSub, EOptional}; // 0x000B, GPS DOP
const TUint KTagGPS000C[6] = {0x000C, EAscii, 0x00000002, 0x00000000, EGpsSub, EOptional}; // 0x000C, GPS Speed Ref
const TUint KTagGPS000D[6] = {0x000D, EUnsignedRational, 0x00000001, 0x00000000, EGpsSub, EOptional}; // 0x000D, GPS Speed 
const TUint KTagGPS000E[6] = {0x000E, EAscii, 0x00000002, 0x00000000, EGpsSub, EOptional}; // 0x000E, GPS Track Ref
const TUint KTagGPS000F[6] = {0x000F, EUnsignedRational, 0x00000001, 0x00000000, EGpsSub, EOptional}; // 0x000F, GPS Track 
const TUint KTagGPS0010[6] = {0x0010, EAscii, 0x00000002, 0x00000000, EGpsSub, EOptional}; // 0x0010, GPS Image Direction Ref
const TUint KTagGPS0011[6] = {0x0011, EUnsignedRational, 0x00000001, 0x00000000, EGpsSub, EOptional}; // 0x0011, GPS Image Direction
const TUint KTagGPS0012[6] = {0x0012, EAscii, 0x00000000, 0x00000000, EGpsSub, EOptional}; // 0x0012, GPS Map Datum
const TUint KTagGPS0013[6] = {0x0013, EAscii, 0x00000002, 0x00000000, EGpsSub, EOptional}; // 0x0013, GPS Destination Latitude Ref
const TUint KTagGPS0014[6] = {0x0014, EUnsignedRational, 0x00000003, 0x00000000, EGpsSub, EOptional}; // 0x0014, GPS Destination Latitude
const TUint KTagGPS0015[6] = {0x0015, EAscii, 0x00000002, 0x00000000, EGpsSub, EOptional}; // 0x0015, GPS Destination Longitude Ref
const TUint KTagGPS0016[6] = {0x0016, EUnsignedRational, 0x00000003, 0x00000000, EGpsSub, EOptional}; // 0x0016, GPS Destination Longitude
const TUint KTagGPS0017[6] = {0x0017, EAscii, 0x00000002, 0x00000000, EGpsSub, EOptional}; // 0x0017, GPS Destination Bearing Ref
const TUint KTagGPS0018[6] = {0x0018, EUnsignedRational, 0x00000001, 0x00000000, EGpsSub, EOptional}; // 0x0018, GPS Destination Bearing 
const TUint KTagGPS0019[6] = {0x0019, EAscii, 0x00000002, 0x00000000, EGpsSub, EOptional}; // 0x0019, GPS Destination Distance Ref
const TUint KTagGPS001A[6] = {0x001A, EUnsignedRational, 0x00000001, 0x00000000, EGpsSub, EOptional}; // 0x001A, GPS Destination Distance
const TUint KTagGPS001B[6] = {0x001B, EUndefined, 0x00000000, 0x00000000, EGpsSub, EOptional}; // 0x001B, GPS Processing Method
const TUint KTagGPS001C[6] = {0x001C, EUndefined, 0x00000000, 0x00000000, EGpsSub, EOptional}; // 0x001C, GPS Area Information
const TUint KTagGPS001D[6] = {0x001D, EAscii, 0x0000000B, 0x00000000, EGpsSub, EOptional}; // 0x001D, GPS Date
const TUint KTagGPS001E[6] = {0x001E, EUnsignedShort, 0x00000001, 0x00000000, EGpsSub, EOptional}; // 0x001E, GPS Differential

const TInt KNumberOfTags = 135; // The number of tags.

 // Array containing all tags.
const TInt KIfd0NumberTags = 19;
const TInt KExifSubNumberTags = 58;
const TInt KInteropNumberTags = 5;
const TInt KIfd1NumberTags = 22;
const TInt KGpsSubNumberTags = 31;


static const TUint* const KIfd0TagArray[KIfd0NumberTags] = {
												KTag010E,
												KTag010F,
												KTag0110,
												KTag0112,
												KTag011A,
												KTag011B,
												KTag0128,
												KTag012D,
												KTag0131,
												KTag0132,
												KTag013B,
												KTag013E,
												KTag013F,
												KTag0211,
												KTag0213,
												KTag0214,
												KTag8298,
												KTag8769,
												KTag8825
												};


static const TUint* const KExifSubTagArray[KExifSubNumberTags] = {	
												KTag829A,
												KTag829D,
												KTag8822,
												KTag8824,
												KTag8827,
												KTag8828,
												KTag9000,
												KTag9003,
												KTag9004,
												KTag9101,
												KTag9102,
												KTag9201,
												KTag9202,
												KTag9203,
												KTag9204,
												KTag9205,
												KTag9206,
												KTag9207,
												KTag9208,
												KTag9209,
												KTag920A,
												KTag9214,
												KTag927C,
												KTag9286,
												KTag9290,
												KTag9291,
												KTag9292,
												KTagA000,
												KTagA001,
												KTagA002,
												KTagA003,
												KTagA004,
												KTagA005,
												KTagA20B,
												KTagA20C,
												KTagA20E,
												KTagA20F,
												KTagA210,
												KTagA214,
												KTagA215,
												KTagA217,
												KTagA300,
												KTagA301,
												KTagA302,
												KTagA401,
												KTagA402,
												KTagA403,
												KTagA404,
												KTagA405,
												KTagA406,
												KTagA407,
												KTagA408,
												KTagA409,
												KTagA40A,
												KTagA40B,
												KTagA40C,
												KTagA420,
												KTagA500
												};

static const TUint* const KInteropTagArray[KInteropNumberTags] = {
												KTag0001,
												KTag0002,
												KTag1000,
												KTag1001,
												KTag1002
												};


static const TUint* const KIfd1TagArray[KIfd1NumberTags] = {												
                                                KThumbTag0103,
												KThumbTag010E,
												KThumbTag010F,
												KThumbTag0110,
												KThumbTag0112,
												KThumbTag011A,
												KThumbTag011B,
												KThumbTag0128,
												KThumbTag012D,
												KThumbTag0131,
												KThumbTag0132,
												KThumbTag013B,
												KThumbTag013E,
												KThumbTag013F,
												KThumbTag0201,
												KThumbTag0202,
												KThumbTag0211,
												KThumbTag0213,
												KThumbTag0214,
												KThumbTag8298,
												KThumbTag8769,
												KThumbTag8825
												};

static const TUint* const KGpsSubTagArray[KGpsSubNumberTags] = {
												KTagGPS0000,
												KTagGPS0001,
												KTagGPS0002,
												KTagGPS0003,
												KTagGPS0004,
												KTagGPS0005,
												KTagGPS0006,
												KTagGPS0007,
												KTagGPS0008,
												KTagGPS0009,
												KTagGPS000A,
												KTagGPS000B,
												KTagGPS000C,
												KTagGPS000D,
												KTagGPS000E,
												KTagGPS000F,
												KTagGPS0010,
												KTagGPS0011,
												KTagGPS0012,
												KTagGPS0013,
												KTagGPS0014,
												KTagGPS0015,
												KTagGPS0016,
												KTagGPS0017,
												KTagGPS0018,
												KTagGPS0019,
												KTagGPS001A,
												KTagGPS001B,
												KTagGPS001C,
												KTagGPS001D,
												KTagGPS001E
												};


#endif	// __EXIFTAGDESCRIPTIONS_H__
