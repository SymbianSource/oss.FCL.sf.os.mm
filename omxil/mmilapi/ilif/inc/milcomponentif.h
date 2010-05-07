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

#ifndef MILCOMPONENTIF_H
#define MILCOMPONENTIF_H

#include <e32base.h>
#include "ilifbase.h"
#include "tilstruct.h"

class MILIfObserver;
class MILComponentPortIf;

/**
IL Component interface class
*/
class MILComponentIf
	{
public:

	/**
	Synchronous method which creates the physical IL Component.
	
	@param  aComponentName
			The string identifying the name of the component.
	@param  aComponentObserver
			The class to receive asynchronous component events.
	@leave 	System wide error.
	*/
	virtual void CreateComponentL(const TDesC8& aComponentName, MILIfObserver& aComponentObserver) = 0;

	/**
	Synchronous method which creates the physical IL Component.
	
	@param  aComponentUID
			The UIF identifying a particular component implementation.
	@param  aComponentObserver
			The class to receive asynchronous component events.
	@leave 	System wide error.
	*/
	virtual void CreateComponentL(const TUid& aComponentUID, MILIfObserver& aComponentObserver) = 0;

	/**
	Called by the IL client to release the component associated to this interface
	*/
	virtual void ReleaseComponent() = 0;
	
	/**
	Synchronous method which returns the input ports a component holds. 
	The array is cleared before appending ports.
	
	@param  aComponentPorts
			The array to which the input ports will be appended.
	@return An error code indicating if the function call was successful. KErrNone on success, otherwise
	        another of the system-wide error codes.
	*/
	virtual TInt GetComponentInputPorts(RPointerArray<MILComponentPortIf>& aComponentPorts) const = 0;

	/**
	Synchronous method which returns the output ports a component holds.
	The array is cleared before appending ports.
	
	@param  aComponentPorts
			The array to which the output ports will be appended.
	@return An error code indicating if the function call was successful. KErrNone on success, otherwise
	        another of the system-wide error codes.
	*/
	virtual TInt GetComponentOutputPorts(RPointerArray<MILComponentPortIf>& aComponentPorts) const = 0;

	/**
	Synchronous method which sets the configuration for a Component.
	Note that in the omx implementation of this interface this function will be used for both SetParameter()
	and SetConfig()

	@param  aConfig
			The reference to the structure that contains the configuration data.
	@param  aComponentPort
			The component's port to be configured when needed, otherwise NULL.
	@return An error code indicating if the function call was successful. KErrNone on success, otherwise
	        another of the system-wide error codes.	
	@see TILStruct
	*/
	virtual TInt SetConfig(const TILStruct& aConfig, const MILComponentPortIf* aComponentPort = NULL) = 0;
	
	/**
	Synchronous method which gets a configuration structure from a component.
	Note that in the omx implementation of this interface this function will be used for both GetParameter()
	and GetConfig()
	
	@param  aConfig
			The reference to the structure that is to contain the configuration information.
	@param  aComponentPort
			The component's port to be queried when needed, otherwise NULL.
	@return An error code indicating if the function call was successful. KErrNone on success, otherwise
	        another of the system-wide error codes.	
	@see TILStruct
	*/
	virtual TInt GetConfig(TILStruct& aConfig, const MILComponentPortIf* aComponentPort = NULL) const = 0;	
	
	/**
	Asynchronous method which instructs the Component to start the initialization. 
	An event is sent to the component's observer on completion.
	
	@return An error code indicating if the function call was successful. KErrNone on success, otherwise
	        another of the system-wide error codes.
	@see MILIfObserver::MsgFromILComponent()
	*/
	virtual TInt Initialize() = 0;

	/**
	Asynchronous method which instructs the Component to start the execution. 
	An event is sent to the component's observer on completion.
	
	@return An error code indicating if the function call was successful. KErrNone on success, otherwise
	        another of the system-wide error codes.
	@see MILIfObserver::MsgFromILComponent()
	*/
	virtual TInt Execute() = 0; 

	/**
	Asynchronous method which pauses the current on-going task.
	An event is sent to the component's observer on completion.
	
	@return An error code indicating if the function call was successful. KErrNone on success, otherwise
	        another of the system-wide error codes.
	@see MILIfObserver::MsgFromILComponent()
	*/
	virtual TInt Pause() = 0;

	/**
	Asynchronous method which stops the current on-going task. 
	An event is sent to the component's observer on completion.

	@return An error code indicating if the function call was successful. KErrNone on success, otherwise
	        another of the system-wide error codes.
	@see MILIfObserver::MsgFromILComponent()
	*/
	virtual TInt Stop() = 0;

	/**
	Asynchronous method which moves the component back to its uninitialized state. 
	An event is sent to the component's observer on completion.

	@return An error code indicating if the function call was successful. KErrNone on success, otherwise
	        another of the system-wide error codes.
	@see MILIfObserver::MsgFromILComponent()
	*/
	virtual TInt UnInitialize() = 0;

	/**
	Synchronous method which returns the current state of the Component.
	
	@param  aState
 			The current state of the Component.
	@return An error code indicating if the function call was successful. KErrNone on success, otherwise
	        another of the system-wide error codes.
	*/
	virtual TInt GetState(TILComponentState& aState) const = 0;

	/**
	Synchronous method which sends a command to the component.
	
	@param  aCommand
			The reference to the structure that is to contain the command to be executed by the component.
	@return An error code indicating if the function call was successful. KErrNone on success, otherwise
	        another of the system-wide error codes.
	@see TILCommand
	*/
	virtual TInt SendCommand(const TILCommand& aCommand) = 0;

	/**	
	Asynchronous function used to flush all component ports. 
	An event is sent to the component's observer on completion.
	
	@return An error code indicating if the function call was successful. KErrNone on success, otherwise
	        another of the system-wide error codes.
	*/
	virtual TInt FlushAllPorts() = 0;

	/**	
	Called by the IL client to query the component about the roles it supports. 
	A component role  is a string identifying the behaviour of the component.
	
	@param  aComponentRole
			The pointer to the string returned identifying the role of the component.
	@param  aIndex
			The index of the component's role being queried.
	@return An error code indicating if the function call was successful. KErrNone on success, otherwise
	        another of the system-wide error codes.
	*/
	virtual TInt ComponentRoleEnum(TPtr8& aComponentRole, TUint32 aIndex) const = 0;
 

	/**	
	Called by the IL client to query the component's version.
	
	@param  aVersion
			The reference to the structure that is to contain the version of the component
	@return An error code indicating if the function call was successful. KErrNone on success, otherwise
	        another of the system-wide error codes.
	@see TILComponentVersion
	*/
	virtual TInt GetComponentVersion(TILComponentVersion& aVersion) const = 0;

	/**
	Synchronous method which uses a standardized OpenMAX or vendor-specific extension string 
	to set a particular component configuration.
	Note that in the OMX implementation of this interface this function will be used for both 
	SetParameter() and SetConfig().
	
	@param  aParameterName
			The string that identifies the data container.
	@param  aConfig
			The reference to the structure that allows access to the data container used for this 
			particular component configuration
	@param  aComponentPort
			The component's port to which the config command is targeted to.
	@return An error code indicating if the function call was successful. KErrNone on success, otherwise
	        another of the system-wide error codes.	
	@see TILStruct
	*/
	virtual TInt SetExtConfig(const TDesC8& aParameterName, const TILStruct& aConfig, const MILComponentPortIf* aComponentPort = NULL) = 0;
	
	/**
	Synchronous method which uses a standardized OpenMAX or vendor-specific extension string 
	to get a particular component configuration.
	
	@param  aParameterName
			The string that identifies the data container.
	@param  aConfig
			The reference to the structure to access the component's configuration data.
	@param  aComponentPort
			The component's port to which the config command is targeted to.
	@return An error code indicating if the function call was successful. KErrNone on success, otherwise
	        another of the system-wide error codes.	
	@see TILStruct
	*/
	virtual TInt GetExtConfig(const TDesC8& aParameterName, TILStruct& aConfig, const MILComponentPortIf* aComponentPort = NULL) const = 0;
	
	
	/**
	Synchronous method to set a particular instance of this interface to the specific role passed as a parameter. 
	A single implementation of this API may support various component roles. It is necessary for the 
	implementation to know what type of component is configuring so that the appropriate translation of 
	configuration parameters can be chosen.
	The role UIDs may be defined by the client application and can be mapped be the implementation of this API
	to roles the components under this layer understand.

	@param  aFormat
			The UID identifying the role to be played by the implementation of this API.
	@return An error code indicating if the function call was successful. KErrNone on success, otherwise
	        another of the system-wide error codes.	
	*/
	virtual TInt SetComponentIfRole(const TUid& aComponentRole) = 0;

	/**
	Synchronous method to set a particular instance of this interface to the specific role passed as a parameter. 
	A single implementation of this API may support various component roles. It is necessary for the 
	implementation to know what type of component is configuring so that the appropriate translation of 
	configuration parameters can be chosen.
	Usually the component role names are defined by a known standard body. For instance role names are defined
	for OpenMAX IL Standard Components by Khronos Group.
	
	@param  aComponentRole
			The string identifying the role to be played by the implementation of this API.
	@return An error code indicating if the function call was successful. KErrNone on success, otherwise
	        another of the system-wide error codes.	
	*/
	virtual TInt SetComponentIfRole(const TDesC8& aComponentRole) = 0;

	/**
	Synchronous method to query about the particular role the interface is set to.
	A single implementation of this API can support various roles, however the role of the interface
	is that of the physical component instantiated under this interface.
	The role of the interface is set by the client after the component creation by calling SetComponentIfRole.
	The role UIDs may be defined by the client application and can be mapped be the implementation of this API
	to roles the components under this layer understand.
	
	@param  aComponentRole
			The UID identifying this interface's role which is to be passed back to the caller.
	@return An error code indicating if the function call was successful. KErrNone on success, otherwise
	        another of the system-wide error codes.	
	*/
	virtual TInt GetComponentIfRole(TUid& aComponentRole) const = 0;

	/**
	Synchronous method to query about the particular role the interface is set to.
	A single implementation of this API can support various roles, however the role of the interface
	is that of the [hysical component instantiated under this interface.
	The role of the interface is set by the client after the component creation by calling SetComponentIfRole.
	Usually the component role names are defined by a known standard body. For instance role names are defined
	for OpenMAX IL Standard Components by Khronos Group.

	@param  aComponentRole
			The string identifying this interface's role which is to be passed back to the caller.
	@return An error code indicating if the function call was successful. KErrNone on success, otherwise
	        another of the system-wide error codes.	
	*/
	virtual TInt GetComponentIfRole(TDes8& aComponentRole) const = 0;

	/**
	Synchronous method used to identify a particular interface implementation. 
	It takes as a parameter the UID identifying the type of the interface requested and 
	it returns a pointer to the specified interface or NULL if it does not exist. 
	Must be cast to correct type by the user

	@param  aInterfaceId
			The UID associated with the interface requested.
	@return A pointer to the specified interface or NULL if it has not be implemented.	
	*/
	virtual TAny* CustomInterface(TUid aInterfaceId) = 0;

	};

	
#endif // MILCOMPONENTIF_H
