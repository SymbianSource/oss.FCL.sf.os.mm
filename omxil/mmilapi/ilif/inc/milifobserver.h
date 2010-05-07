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

#ifndef MILIFOBSERVER_H
#define MILIFOBSERVER_H

#include <e32base.h>
#include "ilifbase.h"

class CMMFBuffer;
class MILComponentIf;

/**
IL Component observer class
*/
class MILIfObserver
	{
public:
	/**
	Callback function to send an event/message to the component observer, i.e. the IL client.
	
	@param  aComponent
	        The IL Component which sent the callback.
	@param  aEvent
			A reference to the structure that contains the event data.
			Any implementation of this function should be able to handle expected and unexpected events.
	@return An error code indicating if the function call was successful. KErrNone on success, otherwise
	        another of the system-wide error codes.
	@see TILEvent
	*/
	virtual TInt MsgFromILComponent(const MILComponentIf* aComponent, const TILEvent& aEvent) = 0; 

	/**
	Callback function to return to the component port observer the buffer that has been delivered.
	The observer is usually the IL client or another port.

	Note that for some implementations this is essentially equivalent to EmptyBufferDone() from input 
	ports and FillBufferDone() from output ports.

	@param  aPort
		  	The interface from the port which sent the callback.
	@param  aBuffer
			The buffer returned by the IL component.
	@return An error code indicating if the function call was successful. KErrNone on success, otherwise
	        another of the system-wide error codes.
	*/
	virtual TInt BufferDelivered(const MILComponentPortIf* aPort, const CMMFBuffer* aBuffer) = 0;
	};
	
#endif // MILIFOBSERVER_H
