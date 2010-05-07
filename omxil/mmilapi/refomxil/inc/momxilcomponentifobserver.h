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

#ifndef MOMXILCOMPONENTIFOBSERVER_H
#define MOMXILCOMPONENTIFOBSERVER_H

#include <e32base.h>
#include <openmax/il/khronos/v1_x/OMX_Core.h>

class CMMFBuffer;

/**
OpenMAX component interface observer class.
This is an internal interface class that encapsulates and represents the OpenMAX callback API as defined by the OpenMAX specification.
*/
class MOmxILComponentIfObserver
	{
public:
	
	/**
	Callback method that returns the buffer that has been filled.
	
	@param 	aBuffer
	 		The buffer returned by the OpenMAX component.
	@param 	aPortIndex
	 		The OpenMAX component's port index returning the buffer.
	@return	
			KErrNone, if successful; otherwise one of the other system-wide error codes. 
	*/

	virtual TInt FillBufferDone(CMMFBuffer* aBuffer, TInt aPortIndex) = 0;

	/**
	Callback method that returns the buffer that has been emptied.
	
	@param 	aBuffer
	 		The buffer returned by the OpenMAX component.
	@param 	aPortIndex
	 		The OpenMAX component's port index returning the buffer.
	@return	
			KErrNone, if successful; otherwise one of the other system-wide error codes. 
	*/
	virtual TInt EmptyBufferDone(const CMMFBuffer* aBuffer, TInt aPortIndex) = 0;

	/**
	Callback method used by the component to notify the client application of a particular event.
	
	@param 	aEvent
	 		Event the component wants to notify the application about.
	@param 	aData1
	 		The first integer event-specific parameter.
	@param 	aData2
	 		The second integer event-specific parameter.
	@param 	aExtraInfo
			A pointer to additional event-specific data the component wants to send to the application.
			The component owns the memory.
	@return	
			KErrNone, if successful; otherwise one of the other system-wide error codes. 
	*/
	virtual TInt EventHandler(OMX_EVENTTYPE aEvent, TUint32 aData1, TUint32 aData2, TAny* aExtraInfo) = 0;
	};

#endif // MOMXILCOMPONENTIFOBSERVER_H

