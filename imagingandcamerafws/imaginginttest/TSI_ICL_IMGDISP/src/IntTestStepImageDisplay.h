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
// DecodeFileByNameByPluginUid.h
// 
//

#if (!defined __INTTESTSTEPIMAGEDISPLAY_H__)
#define __INTTESTSTEPIMAGEDISPLAY_H__

#include "TestStepImageDisplay.h"

/**
 *
 * The following classes provide test steps to exercise the ImageDisplay
 * functionality. See test specification for details of individual test cases.
 *
 */

/**
 * CDecodeFileByNameByPluginUid
 */
class CDecodeFileByNameByPluginUid : public CTestStepImgDisplayGeneric
	{
public:

	CDecodeFileByNameByPluginUid(const TImageDisplayUnitTestParams& aTestParams);

protected:
	virtual TVerdict TestL();

private:
	};

/**
 * CDecodeFileByNameByInvalidPluginUid
 */
class CDecodeFileByNameByInvalidPluginUid : public CTestStepImgDisplayGeneric
	{
public:

	CDecodeFileByNameByInvalidPluginUid(const TImageDisplayUnitTestParams& aTestParams);

protected:
	virtual TVerdict TestL();

private:
	};

/**
 * CDecodeEmptyFileByNameNoExt
 */
class CDecodeEmptyFileByNameNoExt : public CTestStepImgDisplayGeneric
	{
public:

	CDecodeEmptyFileByNameNoExt(const TImageDisplayUnitTestParams& aTestParams);

protected:
	virtual TVerdict TestL();

private:
	};

/**
 * CDecodeEmptyFileByName
 */
class CDecodeEmptyFileByName : public CTestStepImgDisplayGeneric
	{
public:

	CDecodeEmptyFileByName(const TImageDisplayUnitTestParams& aTestParams);

protected:
	virtual TVerdict TestL();

private:
	};

/**
 * CDecodeEmptyFileByMimeType
 */
class CDecodeEmptyFileByMimeType : public CTestStepImgDisplayGeneric
	{
public:

	CDecodeEmptyFileByMimeType(const TImageDisplayUnitTestParams& aTestParams);

protected:
	virtual TVerdict TestL();

private:
	};

/**
 * CDecodeEmptyFileByImageType
 */
class CDecodeEmptyFileByImageType : public CTestStepImgDisplayGeneric
	{
public:

	CDecodeEmptyFileByImageType(const TImageDisplayUnitTestParams& aTestParams);

protected:
	virtual TVerdict TestL();

private:
	};

/**
 * CDecodeBadHdrFileByNameNoExt
 */
class CDecodeBadHdrFileByNameNoExt : public CTestStepImgDisplayGeneric
	{
public:

	CDecodeBadHdrFileByNameNoExt(const TImageDisplayUnitTestParams& aTestParams);

protected:
	virtual TVerdict TestL();

private:
	};

/**
 * CDecodeBadHdrFileByName
 */
class CDecodeBadHdrFileByName : public CTestStepImgDisplayGeneric
	{
public:

	CDecodeBadHdrFileByName(const TImageDisplayUnitTestParams& aTestParams);

protected:
	virtual TVerdict TestL();

private:
	};

/**
 * CDecodeBadHdrFileByMimeType
 */
class CDecodeBadHdrFileByMimeType : public CTestStepImgDisplayGeneric
	{
public:

	CDecodeBadHdrFileByMimeType(const TImageDisplayUnitTestParams& aTestParams);

protected:
	virtual TVerdict TestL();

private:
	};

/**
 * CDecodeBadHdrFileByImageType
 */
class CDecodeBadHdrFileByImageType : public CTestStepImgDisplayGeneric
	{
public:

	CDecodeBadHdrFileByImageType(const TImageDisplayUnitTestParams& aTestParams);

protected:
	virtual TVerdict TestL();

private:
	};

/**
 * CDecodeFileClippedNoClipPlugins
 */
class CDecodeFileClippedNoClipPlugins : public CTestStepImgDisplayGeneric
	{
public:

	CDecodeFileClippedNoClipPlugins(const TImageDisplayUnitTestParams& aTestParams);

protected:
	virtual TVerdict TestL();

private:
	};

/**
 * CDecodeFileClipCancelNoClipPlugins
 */
class CDecodeFileClipCancelNoClipPlugins : public CTestStepImgDisplayGeneric
	{
public:

	CDecodeFileClipCancelNoClipPlugins(const TImageDisplayUnitTestParams& aTestParams);

protected:
	virtual TVerdict TestL();

private:
	};

/**
 * CDecodeNoExistFileByName
 */
class CDecodeNoExistFileByName : public CTestStepImgDisplayGeneric
	{
public:

	CDecodeNoExistFileByName(const TImageDisplayUnitTestParams& aTestParams);

protected:
	virtual TVerdict TestL();

private:
	};

#endif // __INTTESTSTEPIMAGEDISPLAY_H__
