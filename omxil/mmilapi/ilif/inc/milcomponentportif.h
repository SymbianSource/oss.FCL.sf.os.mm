// Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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
 @prototype
*/

#ifndef MILCOMPONENTPORTIF_H
#define MILCOMPONENTPORTIF_H

#include <e32base.h>
#include "ilifbase.h"
#include "milcomponentif.h"

class MILComponentPortIf;
class MILComponentPortIfObserver;

class CMMFBuffer;
	
/**
IL Component Port interface class
*/
class MILComponentPortIf
	{
public:
	
	/**
	Asynchronous function used to get data from an output Port.
	
	@param	aBuffer	
			Reference to the buffer to be filled.
	@return An error code indicating if the function call was successful. KErrNone on success, otherwise
	        another of the system-wide error codes.
	*/		 
	virtual TInt FillThisBuffer(CMMFBuffer& aBuffer) = 0;

	/**
	Asynchronous function used to deliver data to an input Port.
	
	@param	aBuffer	
			Reference to the buffer to be emptied.
	@return An error code indicating if the function call was successful. KErrNone on success, otherwise
	        another of the system-wide error codes.
	*/		 
	virtual TInt EmptyThisBuffer(const CMMFBuffer& aBuffer) = 0;
		
	/**
	Called by an IL client to request tunnelling between the component's port represented by this
	interface and the component's port represented by the port interface passed as a parameter.
	It can also be called by an IL client to request tearing down an existing tunnel by passing a NULL
	port argument.
	
	@param  aPort 
			The port interface representing the port to be tunelled to, or NULL to disconnect tunnel.
	@return An error code indicating if the function call was successful. KErrNone on success, otherwise
	        another of the system-wide error codes.
	*/
 	virtual TInt TunnelRequest(MILComponentPortIf* aPort) = 0;

	/**
	Called by a component to get the index of this port.
	
	@return An integer specifying the index of this port.
	*/
	virtual TInt PortIndex() const = 0;

	/**
	Called by a component to get the direction of this port.
	
	@return The direction of this port
	*/
	virtual TPortDirection PortDirection() const = 0;

	/**
	Synchronous function used to instruct the port to create a buffer. 
	
	@param  aBufferSize
			The size of the buffer to be created.
	@leave  KErrNoMemory if not sufficient memory available when creating the buffer.
	@return A pointer to the newly created buffer.
	*/
	virtual CMMFBuffer* CreateBufferL(TInt aBufferSize) = 0;

	/**
	Synchronous function used to instruct the Port to use the buffer passed 
	in the function's argument. 
	
	@param  aBuffer
			A reference to the buffer to be used by the Port.
	@return An error code indicating if the function call was successful. 
			KErrNone on success, otherwise another of the system-wide error codes.
	*/	
	virtual TInt UseBuffer(CMMFBuffer& aBuffer) = 0;

	/**
	Synchronous function used to instruct the port to free the buffer passed 
	in the function's argument. 
	
	@param  aBuffer
			The buffer to be freed
	@return An error code indicating if the function call was successful. 
			KErrNone on success, otherwise another of the system-wide error codes.
	*/
	virtual TInt FreeBuffer(CMMFBuffer* aBuffer) = 0;

	/**
	Asynchronous function used to flush this port.
	
	@return An error code indicating if the function call was successful. 
			KErrNone on success, otherwise another of the system-wide error codes.
	*/
	virtual TInt FlushPort() = 0;
	
	/**
	Asynchronous function used to enable this port.
	
	@return An error code indicating if the function call was successful. 
			KErrNone on success, otherwise another of the system-wide error codes.
	*/
	virtual TInt EnablePort() = 0;

	/**
	Asynchronous function used to disable this port.
	
	@return An error code indicating if the function call was successful. 
			KErrNone on success, otherwise another of the system-wide error codes.
	*/
	virtual TInt DisablePort() = 0;
	
	/**
	Synchronous function that returns a pointer to a component owning this port. 
	
	@return A pointer to this port's component
	*/
	virtual	MILComponentIf* PortComponent() const = 0; 

	};

#endif // MILCOMPONENTPORTIF_H
