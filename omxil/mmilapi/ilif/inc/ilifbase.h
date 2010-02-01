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

#ifndef ILIFBASE_H
#define ILIFBASE_H

#include <e32base.h>

	
/** 
 Enumeration used to indicate IL Component internal state. 
*/	
enum TILComponentState
	{
	/** 
	Component invalid state 
	 */
	EComponentInvalid = 0,
	/** 
	Component loaded state 
	*/
	EComponentLoaded,
	/** 
	Component idle state 
	*/
	EComponentIdle,
	/** 
	Component executing state 
	*/
	EComponentExecuting,
	/** 
	Component paused state 
	*/
	EComponentPaused,
	/** 
	Component waiting for resources state 
	*/
	EComponentWaitingForResources,
	/** 
	Component loading state 
	*/
	EComponentLoading,		
	/** 
	Component initializing state 
	*/
	EComponentInitializing,		

	EComponentMax = 0X7FFFFFFF	
	};


/**
Enumeration used to indicate if a port is an input or an output port
*/
enum TPortDirection
{
	/** 
	Port is an input port 
	*/
    EDirInput,
	/** 
	Port is an output port 
	*/
    EDirOutput
};


/** 
 Enumeration used to define the various types of IL Events 
 This enumeration matches the possible component events as defined in the 
 OpenMAX v1.1 specification
 */	
enum TILEventTypes
	{
	/** 
	Component has completed the execution of a command.
	 */
	EEventCmdComplete = 0,
	/** 
	Component has detected an error condition.
	*/
	EEventError,
	/** 
	A buffer mark has reached the target component, and the IL client has
	received this event with the private data pointer of the mark.
	*/
	EEventMark,
	/** 
	Component has changed port settings. For example, the component has
	changed port settings resulting from bit stream parsing. 
	*/
	EEventPortSettingsChanged,
	/** 
	The event that a component sends when it detects the end of a stream.
	*/
	EEventBufferFlag,
	/** 
	The component has been granted resources and is transitioning from the
	OMX_StateWaitForResources state to the OMX_StateIdle state.
	*/
	EEventResourcesAcquired,
	/** 
	The component has been resumed (i.e. no longer suspended) due to
	reacquisition of resources.
	*/
	EEventDynamicResourcesAvailable,		

	EEventMax = 0X7FFFFFFF
	};
	

/**
Event structure representing the events and data sent from the IL component to the IL client. 
*/
class TILEvent
	{
public:
	inline TILEvent();

	inline TILEvent(TILEventTypes aEvent, TUint32 aData1, TUint32 aData2, TAny* aExtraData);

public:
	/** 
	Event the component wants to notify the application about.
	*/
	TILEventTypes iEvent;
	/** 
	The first integer event-specific parameter.
	*/
	TUint32 iData1;
	/** 
	The second integer event-specific parameter.
	*/
	TUint32 iData2;
	/** 
	A pointer to additional event-specific data the component wants to send to the application.
	The component owns the memory.
	*/
	TAny* iExtraData;
	};

/** 
 Enumeration used to define commands sent to the IL component
 This enumeration defines commands as defined in the OpenMAX v1.1 specification
 that are not alredy covered by the Symbian IL API
 */	
enum TILCommandTypes
	{
	/** 
	Change the component state command.
	 */
    ECommandStateSet,    
 	/** 
	Flush the data queue(s) of a component.
	 */
    ECommandFlush,       
	/** 
	Disable a port on a component.
	 */
    ECommandPortDisable, 
	/** 
	Enable a port on a component.
	 */
    ECommandPortEnable,  
	/** 
	Mark a component/buffer for observation.
	 */
	ECommandMarkBuffer, 
	/** 
	Invalid command.
	 */
	ECommandInvalid = 0X7FFFFFFF 
	};
	
/**
Command structure representing the command's data sent to the IL component by the IL client
*/
class TILCommand
	{
public:
	inline TILCommand();

	inline TILCommand(TILCommandTypes iCmd, TUint32 aData1, TAny* aExtraData);

public:
	/** 
	Command that the application wants to send to the component.
	*/
	TILCommandTypes iCmd;
	/** 
	The integer command-specific parameter.
	*/
	TUint32 iData1;
	/** 
    A pointer to additional command-specific data the application wants to send to the component.
	The application owns the memory.
	*/
	TAny* iExtraData;
	};
	

/**
Structure encapsulating the various fields needed to represent a version of some content.
*/
class TILVersion
	{
public:
	inline TILVersion();

	inline TILVersion(TUint8 aMajor, TUint8 aMinor, TUint8 aRev, TUint8 aStep); 
		
	inline TBool operator==(TILVersion aVersion);
	
	inline TBool operator!=(TILVersion aVersion);

public:
	/** 
	Integer representing the Major version number.
	*/
	TUint8	iMajor;
	/** 
	Integer representing the Minor version number.
	*/
	TUint8	iMinor;
	/** 
	Integer representing the Revision version number.
	*/
	TUint8	iRev;
	/** 
	Integer representing the Step version number.
	*/
	TUint8	iStep;
	};

/**
Structure encapsulating the various fields needed to represent a version of an IL component.
This structure is based on the arguments required by the OMX_GetComponentVersion OMX API MACRO.  
*/
class TILComponentVersion
	{
public:
	inline TILComponentVersion(TPtr8& aPtr);

public:
	/** 
	The component name to which the structure applies.
	*/
	TPtr8 iComponentName;	
	/** 
	The version of the component to which structure applies.
	*/
	TILVersion iComponentVersion;
	/** 
	The version of the specification that was used to build this component.
	*/
	TILVersion iSpecVersion;
	/** 
	The unique identifier for the particular instance of the component to which structure applies.
	*/
	TUint32 iComponentUniqueID;
	};


#include "ilifbase.inl"

#endif // ILIFBASE_H
