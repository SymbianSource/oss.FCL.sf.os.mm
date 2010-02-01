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

/**
 @file
 @internalTechnology
*/

#ifndef MMFMEDIACLIENTEXTDISPLAYINTERFACE_H_
#define MMFMEDIACLIENTEXTDISPLAYINTERFACE_H_

#include <e32base.h>
#include <graphics/surface.h>

/**

Mixin class capable of handling events and requests from provider

*/
class MExtDisplayConnectionProviderCallback
    {
public:
    virtual void MedcpcExtDisplayCalculateExtentAndViewportL(TRect& aExtent, TRect& aViewport, TRect& aExternalDisplayRect) = 0;
    virtual void MedcpcExtDisplayNotifyConnected(TBool aExtDisplayConnected) = 0;
    };

/**

Base class for external display connection provider plugins.

*/
class CExtDisplayConnectionProviderInterface : public CBase
	{
public:

    /**
    Determines if external displays are supported by checking if any implementations for
    supporting external displays are available.

    @return ETrue if supported otherwise EFalse

    */
    static TBool ExternalDisplaySupportedL();
    
	/**
	Allocates and constructs a new provider plugin.

	Uses ECOM to instantiate a new provider plugin. Can leave with any of the system-wide error codes.

	@param  aCallback
	        A reference to an object that can be used by the provider to send
            events back to the client and make request on the client.
	@param  aSurfaceId
	        The surfaceId. 
    @leave  KErrNotSupported There is no support for external displays.
	@return A pointer to the new provider plugin.

	*/
	static CExtDisplayConnectionProviderInterface* NewL(MExtDisplayConnectionProviderCallback& aCallback, TSurfaceId& aSurfaceId);

    /**
    Function to be implemented by connection provider that indicates whether an external display is
    connected.
    
    @return ETrue if external display is connectde otherwise EFalse.
    */
	virtual TBool ExtDisplayConnectedL() = 0;
	    
	/**
	Default destructor.
	*/
	IMPORT_C virtual ~CExtDisplayConnectionProviderInterface();
	
protected:
	/**
	Constructor.
	*/
    IMPORT_C CExtDisplayConnectionProviderInterface();
	
    /**
     The callback handler for the provider plugin.
     */
     MExtDisplayConnectionProviderCallback* iCallback; // not owned

     /**
     The surface Id.
     */
     TSurfaceId iSurfaceId; // not owned
     
private:
	/**
	This is internal and not intended for use.
	*/
	void ConstructL(MExtDisplayConnectionProviderCallback& aCallback, TSurfaceId& aSurfaceId);
	
private:
	/**
	Required by ECOM.
	*/
	TUid iInstanceKey;
	};

#endif /* MMFMEDIACLIENTEXTDISPLAYINTERFACE_H_ */
