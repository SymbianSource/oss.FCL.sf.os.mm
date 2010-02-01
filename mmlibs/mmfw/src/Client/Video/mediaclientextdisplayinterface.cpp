// Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include <mmf/plugin/mmfmediaclientextdisplayinterface.h>
#include <mmf/plugin/mmfmediaclientextdisplayinterface.hrh>
#include <e32cmn.h> 
#include <ecom/ecom.h>

TBool CExtDisplayConnectionProviderInterface::ExternalDisplaySupportedL()
    {
    RImplInfoPtrArray implInfoArray;
    REComSession::ListImplementationsL(TUid::Uid(KMediaDisplayExtProviderInterfaceUid), implInfoArray);

    TBool ret = EFalse;
    if(implInfoArray.Count() > 0)
        {
        ret = ETrue;
        }
    
    implInfoArray.Close();
    return ret;
    }

CExtDisplayConnectionProviderInterface* CExtDisplayConnectionProviderInterface::NewL(MExtDisplayConnectionProviderCallback& aCallback, TSurfaceId& aSurfaceId)
    {
    RImplInfoPtrArray implInfoArray;
    CleanupClosePushL(implInfoArray);
    REComSession::ListImplementationsL(TUid::Uid(KMediaDisplayExtProviderInterfaceUid), implInfoArray);
    
    if(implInfoArray.Count() == 0)
        {
        User::Leave(KErrNotSupported);
        }
    
    CExtDisplayConnectionProviderInterface* self = 
        REINTERPRET_CAST(CExtDisplayConnectionProviderInterface*, REComSession::CreateImplementationL(
                implInfoArray[0]->ImplementationUid(), _FOFF(CExtDisplayConnectionProviderInterface, iInstanceKey)));
    CleanupStack::PushL(self);
    
    self->ConstructL(aCallback, aSurfaceId);
    CleanupStack::Pop(self);
    
    CleanupStack::PopAndDestroy(); // implInfoArray
    return self;
    }

void CExtDisplayConnectionProviderInterface::ConstructL(MExtDisplayConnectionProviderCallback& aCallback, TSurfaceId& aSurfaceId)
    {
    iCallback = &aCallback;
    iSurfaceId = aSurfaceId;
    }

EXPORT_C CExtDisplayConnectionProviderInterface::~CExtDisplayConnectionProviderInterface()
    {
    REComSession::DestroyedImplementation(iInstanceKey);
    }

EXPORT_C CExtDisplayConnectionProviderInterface::CExtDisplayConnectionProviderInterface()
    {
    }
