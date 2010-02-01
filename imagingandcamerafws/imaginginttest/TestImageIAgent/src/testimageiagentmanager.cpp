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
//

#include "testimageiagentmanager.h"

using namespace ContentAccess;

_LIT(KTestJPEGIContentExtension, "JPGI");
_LIT(KTestTIFIContentExtension, "TIFI");


CTestIMAGEIAgentManager* CTestIMAGEIAgentManager::NewL()
	{
	CTestIMAGEIAgentManager* self = NewLC();
	CleanupStack::Pop(self);
	return self;
	}

CTestIMAGEIAgentManager* CTestIMAGEIAgentManager::NewLC()
	{
	CTestIMAGEIAgentManager* self=new(ELeave) CTestIMAGEIAgentManager();
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

CTestIMAGEIAgentManager::CTestIMAGEIAgentManager()
	{
	}

CTestIMAGEIAgentManager::~CTestIMAGEIAgentManager()
	{
	}

void CTestIMAGEIAgentManager::ConstructL()
	{
	}

void CTestIMAGEIAgentManager::DeleteFileL(const TDesC &aFileName)
	{
	RFs fs;
	User::LeaveIfError(fs.Connect());
	CleanupClosePushL(fs);
	User::LeaveIfError(fs.Delete(aFileName));
	CleanupStack::PopAndDestroy();
	}

TInt CTestIMAGEIAgentManager::DeleteFile(const TDesC &aFileName)
	{
	TRAPD(err, DeleteFileL(aFileName));
	return err;
	}


TInt CTestIMAGEIAgentManager::CopyFile(const TDesC& /*aSource*/, const TDesC& /*aDestination*/)
	{
	return KErrNone;
	}

TInt CTestIMAGEIAgentManager::RenameFile(const TDesC& /*aSource*/, const TDesC& /*aDestination*/)
	{
	return KErrNone;
	}

TInt CTestIMAGEIAgentManager::MkDir(const TDesC& /*aPath*/)
	{
	return KErrNone;
	}

TInt CTestIMAGEIAgentManager::MkDirAll(const TDesC& /*aPath*/)
	{
	return KErrNone;
	}

TInt CTestIMAGEIAgentManager::RmDir(const TDesC& /*aPath*/)
	{
	return KErrNone;
	}

TInt CTestIMAGEIAgentManager::GetDir(const TDesC& /*aName*/,TUint /*anEntryAttMask*/,TUint /*anEntrySortKey*/, CDir*& /*anEntryList*/) const
	{
	return KErrNone;
	}

TInt CTestIMAGEIAgentManager::GetDir(const TDesC& /*aName*/,TUint /*anEntryAttMask*/,TUint /*anEntrySortKey*/, CDir*& /*anEntryList*/,CDir*& /*aDirList*/) const
	{
	return KErrNone;
	}

TInt CTestIMAGEIAgentManager::GetDir(const TDesC& /*aName*/,const TUidType& /*anEntryUid*/,TUint /*anEntrySortKey*/, CDir*& /*aFileList*/) const
	{
	return KErrNone;
	}

TInt CTestIMAGEIAgentManager::GetAttribute(TInt /*aAttribute*/, TInt& /*aValue*/, const TVirtualPathPtr& /*aVirtualPath*/)
	{
	return KErrNone;
	}

TInt CTestIMAGEIAgentManager::GetAttributeSet(RAttributeSet& /*aAttributeSet*/, const TVirtualPathPtr& /*aVirtualPath*/)
	{
	return KErrNone;
	}

TInt CTestIMAGEIAgentManager::GetStringAttribute(TInt /*aAttribute*/, TDes& /*aValue*/, const TVirtualPathPtr& /*aVirtualPath*/)
	{
	return KErrNone;
	}

TInt CTestIMAGEIAgentManager::GetStringAttributeSet(RStringAttributeSet& /*aStringAttributeSet*/, const TVirtualPathPtr& /*aVirtualPath*/)
	{
	return KErrNone;
	}

void CTestIMAGEIAgentManager::NotifyStatusChange(const TDesC& /*aURI*/, TEventMask /*aMask*/, TRequestStatus& /*aStatus*/)
	{
	}

TInt CTestIMAGEIAgentManager::CancelNotifyStatusChange(const TDesC& /*aURI*/, TRequestStatus& /*aStatus*/)
	{
	return KErrNone;
	}

TInt CTestIMAGEIAgentManager::SetProperty(TAgentProperty /*aProperty*/, TInt /*aValue*/)
	{
	return KErrNone;
	}

void CTestIMAGEIAgentManager::DisplayInfoL(TDisplayInfo /*aInfo*/, const TVirtualPathPtr& /*aVirtualPath*/)
	{
	}

TBool CTestIMAGEIAgentManager::IsRecognizedL(const TDesC& aURI, TContentShareMode /*aShareMode*/) const
	{
	// Convert URI to uppercase
	HBufC *uriUpper = aURI.AllocL();
	TPtr uriPtr(uriUpper->Des());
	uriPtr.UpperCase();

	// Check that the file extension is .JPEGI
	TPtrC jpegextension = uriPtr.Right(KTestJPEGIContentExtension().Length());
	TInt jpegresult = jpegextension.Compare(KTestJPEGIContentExtension);
	
	// Check that the file extension is .TIFI
	TPtrC tifextension = uriPtr.Right(KTestTIFIContentExtension().Length());
	TInt tifresult = tifextension.Compare(KTestTIFIContentExtension);
		
	delete uriUpper;

	if( jpegresult == 0 || tifresult == 0 )
		{
		return ETrue;
		}
	else
		{
		return EFalse;
		}
	}

TBool CTestIMAGEIAgentManager::IsRecognizedL(RFile& /*aFile*/) const
	{
	return EFalse;
	}

TBool CTestIMAGEIAgentManager::RecognizeFileL(const TDesC& /*aFileName*/, const TDesC8& /*aBuffer*/, TDes8& /*aFileMimeType*/, TDes8& /*aContentMimeType*/) const
	{
	// don't return anything to apparc for now
	return EFalse;
	}

TInt CTestIMAGEIAgentManager::AgentSpecificCommand(TInt /*aCommand*/, const TDesC8& /*aInputBuffer*/, TDes8& /*aOutputBuffer*/)
	{
	return KErrNone;
	}

void CTestIMAGEIAgentManager::AgentSpecificCommand(TInt /*aCommand*/, const TDesC8& /*aInputBuffer*/, TDes8& /*aOutputBuffer*/, TRequestStatus& /*aStatus*/)
	{
	}

void CTestIMAGEIAgentManager::DisplayManagementInfoL()
	{
	}

void CTestIMAGEIAgentManager::PrepareHTTPRequestHeaders(RStringPool& /*aStringPool*/, RHTTPHeaders& /*aRequestHeaders*/) const
	{
	}
