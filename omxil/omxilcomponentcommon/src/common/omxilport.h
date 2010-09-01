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
 @internalComponent
*/

#ifndef OMXILPORT_H
#define OMXILPORT_H

#include <e32base.h>

#include <openmax/il/khronos/v1_x/OMX_Component.h>
#include "omxilspecversion.h"


/**
   Base Port Panic category
*/
_LIT(KOmxILPortPanicCategory, "OmxILPort");


// Forward declarations
class TOmxILCommonPortData;

/**
   OpenMAX IL component port base class.
*/
class COmxILPort : public CBase
	{

public:

	static const TInt	KMaxBufferMarksQueueSize			= 10;
	static const TInt	KBufferMarkPropagationPortNotNeeded = 0x7FFFFFFF;

public:

	IMPORT_C ~COmxILPort();

	/**
	   This method is used to retrieve the local OpenMAX IL parameter indexes
	   managed by the specific port. The implementation should collect indexes
	   from the parent class. Local indexes should be appended in order to the
	   list received as parameter. The implementation should handle index
	   duplication as a non-error situation (i.e., the resulting list will
	   contain unique indexes).

	   @param aIndexArray An array of parameter indexes where the local
	   indexes are to be appended.

	   @return OMX_ERRORTYPE
	*/
	IMPORT_C virtual OMX_ERRORTYPE GetLocalOmxParamIndexes(
		RArray<TUint>& aIndexArray) const = 0;

	/**
	   This method is used to retrieve the local OpenMAX IL config indexes
	   managed by the specific port. The implementation should collect indexes
	   from the parent class. Local indexes should be appended in order to the
	   list received as parameter. The implementation should handle index
	   duplication as a non-error situation (i.e., the resulting list will
	   contain unique indexes).

	   @param aIndexArray An array of config indexes where the local indexes
	   are to be appended.

	   @return OMX_ERRORTYPE
	*/
	IMPORT_C virtual OMX_ERRORTYPE GetLocalOmxConfigIndexes(
		RArray<TUint>& aIndexArray) const = 0;

	/**
	   Port-specific version of the OpenMAX IL GetParameter API.

	   @param aParamIndex The index of the structure that is to be filled.
	   @param apComponentParameterStructure A pointer to the IL structure.

	   @return OMX_ERRORTYPE
	*/
	IMPORT_C virtual OMX_ERRORTYPE GetParameter(
		OMX_INDEXTYPE aParamIndex,
		TAny* apComponentParameterStructure) const = 0;

	/**
	   Port-specific version of the OpenMAX IL SetParameter API.

	   @param aParamIndex The index of the structure that is to be set.
	   @param apComponentParameterStructure A pointer to the IL structure.
	   @param aUpdateProcessingFunction A flag to signal that the component's
	   processing function needs to be updated with this structure.

	   @return OMX_ERRORTYPE
	*/
	IMPORT_C virtual OMX_ERRORTYPE SetParameter(
		OMX_INDEXTYPE aParamIndex,
		const TAny* apComponentParameterStructure,
		TBool& aUpdateProcessingFunction) = 0;

	IMPORT_C virtual OMX_ERRORTYPE GetConfig(
		OMX_INDEXTYPE aConfigIndex,
		TAny* apComponentConfigStructure) const;

	IMPORT_C virtual OMX_ERRORTYPE SetConfig(
		OMX_INDEXTYPE aConfigIndex,
		const TAny* apComponentConfigStructure,
		TBool& aUpdateProcessingFunction);

	IMPORT_C virtual OMX_ERRORTYPE GetExtensionIndex(
		OMX_STRING aParameterName,
		OMX_INDEXTYPE* apIndexType) const;

	IMPORT_C virtual OMX_ERRORTYPE PopulateBuffer(
		OMX_BUFFERHEADERTYPE** appBufferHdr,
		const OMX_PTR apAppPrivate,
		OMX_U32 aSizeBytes,
		OMX_U8* apBuffer,
		TBool& portPopulationCompleted);

	IMPORT_C virtual OMX_ERRORTYPE FreeBuffer(
		OMX_BUFFERHEADERTYPE* apBufferHeader,
		TBool& portDepopulationCompleted);

	IMPORT_C virtual OMX_ERRORTYPE TunnelRequest(
		OMX_HANDLETYPE aTunneledComp,
		OMX_U32 aTunneledPort,
		OMX_TUNNELSETUPTYPE* apTunnelSetup);

	IMPORT_C virtual OMX_ERRORTYPE PopulateTunnel(
		TBool& portPopulationCompleted);

	IMPORT_C virtual OMX_ERRORTYPE FreeTunnel(
		TBool& portDepopulationCompleted);

	IMPORT_C virtual TBool SetBufferSent(
		OMX_BUFFERHEADERTYPE* apBufferHeader,
		TBool& aBufferMarkedWithOwnMark);

	IMPORT_C virtual TBool SetBufferReturned(
		OMX_BUFFERHEADERTYPE* apBufferHeader);

	IMPORT_C virtual void SetTransitionToDisabled();

	IMPORT_C virtual void SetTransitionToEnabled();

	IMPORT_C virtual void SetTransitionToDisabledCompleted();

	IMPORT_C virtual void SetTransitionToEnabledCompleted();

	IMPORT_C virtual OMX_ERRORTYPE StoreBufferMark(
		const OMX_MARKTYPE* pMark);

	IMPORT_C virtual OMX_ERRORTYPE SetComponentRoleDefaults(
		TUint aComponentRoleIndex);

	IMPORT_C virtual OMX_ERRORTYPE DoPortReconfiguration(
		TUint aPortSettingsIndex,
		const TDesC8& aPortSettings,
		OMX_EVENTTYPE& aEventForILClient);

	inline OMX_DIRTYPE Direction() const;

	inline TBool IsEnabled() const;

	inline TBool IsPopulated() const;

	inline TBool IsDePopulated() const;

	inline TBool IsTunnelled() const;

	inline TBool IsTunnelledAndBufferSupplier() const;

	IMPORT_C virtual TBool HasAllBuffersAtHome() const;

	inline TBool IsTransitioningToEnabled() const;

	inline TBool IsTransitioningToDisabled() const;

	IMPORT_C virtual TBool IsBufferAtHome(
		OMX_BUFFERHEADERTYPE* apHeaderHeader) const;

	inline OMX_U32 Index() const;

	inline OMX_PORTDOMAINTYPE Domain() const;

	inline OMX_U32 Count() const;

	inline OMX_BUFFERHEADERTYPE* const& operator[](TInt anIndex) const;

	inline OMX_BUFFERHEADERTYPE*& operator[](TInt anIndex);

	inline OMX_U32 BufferMarkPropagationPort() const;

protected:

	class TBufferMarkInfo;

	/**
	   Buffer mark info list
	*/
	class TBufferMarkInfoQue :
		public TSglQue<TBufferMarkInfo>
		{

	public:

		// Convenience typedef....
		typedef TSglQue<TBufferMarkInfo> Tq;

	public:

		inline explicit TBufferMarkInfoQue(TInt aOffset);

		inline TBufferMarkInfo* First() const;

		inline void AddLast(TBufferMarkInfo& aRef);

		inline void Remove(TBufferMarkInfo& aRef);

		inline TInt Elements() const;

		inline void ResetAndDestroy();

	private:

		TBufferMarkInfoQue();
		void AddFirst(TBufferMarkInfo& aRef);
		TBool IsFirst(const TBufferMarkInfo* aPtr) const;
		TBool IsLast(const TBufferMarkInfo* aPtr) const;

		TBufferMarkInfo* Last() const;

	private:

		TInt iNumElements;

		};


	class TBufferInfo;

protected:

	enum TPortIntermediateState
		{
		EPortNotTransitioning,
		EPortTransitioningToDisabled,
		EPortTransitioningToEnabled
		};


protected:

	IMPORT_C explicit COmxILPort(const TOmxILCommonPortData& aCommonPortData);

	/**
	   When the IL Client calls SetParameter with index
	   OMX_IndexParamPortDefinition, this method is called to set the fields in
	   the OMX_PARAM_PORTDEFINITIONTYPE (@see iParamPortDefinition) structure
	   that are specific to this port's domain.

	   @param aPortDefinition The OpenMAX IL structure received from the IL Client.
	   @param aUpdateProcessingFunction A flag to signal that the component's
	   processing function needs to be updated with the this structure.

	   @return OMX_ERRORTYPE
	*/
	virtual OMX_ERRORTYPE SetFormatInPortDefinition(
		const OMX_PARAM_PORTDEFINITIONTYPE& aPortDefinition,
		TBool& aUpdateProcessingFunction) = 0;

	/**
	   On a tunnel request, this method is called to check the compatibility of
	   the fields in the OMX_PARAM_PORTDEFINITIONTYPE structure of the
	   tunnelled component that are specific to this port's domain.

	   @param aPortDefinition The OpenMAX IL structure received from a
	   tunnelled component.

	   @return ETrue if compatible, EFalse otherwise.
	*/
	virtual TBool IsTunnelledPortCompatible(
		const OMX_PARAM_PORTDEFINITIONTYPE& aPortDefinition) const = 0;

	IMPORT_C virtual OMX_ERRORTYPE DoBufferAllocation(
		OMX_U32 aSizeBytes,
		OMX_U8*& apPortSpecificBuffer,
		OMX_PTR& apPortPrivate,
		OMX_PTR& apPlatformPrivate,
		OMX_PTR apAppPrivate = 0);

	IMPORT_C virtual void DoBufferDeallocation(
		OMX_PTR apPortSpecificBuffer,
		OMX_PTR apPortPrivate,
		OMX_PTR apPlatformPrivate,
		OMX_PTR apAppPrivate = 0);

	IMPORT_C virtual OMX_ERRORTYPE DoBufferWrapping(
		OMX_U32 aSizeBytes,
		OMX_U8* apBuffer,
		OMX_PTR& apPortPrivate,
		OMX_PTR& apPlatformPrivate,
		OMX_PTR apAppPrivate = 0);

	IMPORT_C virtual void DoBufferUnwrapping(
		OMX_PTR apPortSpecificBuffer,
		OMX_PTR apPortPrivate,
		OMX_PTR apPlatformPrivate,
		OMX_PTR apAppPrivate = 0);

	IMPORT_C virtual OMX_ERRORTYPE DoOmxUseBuffer(
		OMX_HANDLETYPE aTunnelledComponent,
		OMX_BUFFERHEADERTYPE** appBufferHdr,
		OMX_U32 aTunnelledPortIndex,
		OMX_PTR apPortPrivate,
		OMX_PTR apPlatformPrivate,
		OMX_U32 aSizeBytes,
		OMX_U8* apBuffer);

	template<typename T>
	inline OMX_ERRORTYPE GetParamStructureFromTunnel(
		T& aComponentConfigStructure, OMX_INDEXTYPE aParamIndex) const;

	IMPORT_C OMX_ERRORTYPE StoreBufferMark(
		OMX_HANDLETYPE& ipMarkTargetComponent,
		OMX_PTR& ipMarkData);

	IMPORT_C void CleanUpPort();

protected:

	// Tunnelled component
	OMX_HANDLETYPE iTunnelledComponent;

	// Tunnelled component's port
	OMX_U32 iTunnelledPort;

	// Buffer headers store
	RArray<TBufferInfo> iBufferHeaders;

	// Buffer marks store
	TBufferMarkInfoQue iBufferMarks;

	// Port's intermediate state to enabled or disabled
	TPortIntermediateState iTransitionState;

	// Associated port that will be used to propagate buffer marks
	OMX_U32 iBufferMarkPropagationPortIndex;

	// OpenMAX IL port definition structure
	OMX_PARAM_PORTDEFINITIONTYPE iParamPortDefinition;

	// OpenMAX IL buffer supplier structure
	OMX_PARAM_BUFFERSUPPLIERTYPE iParamCompBufferSupplier;

	};


/**
   Structure used to pass the common information that an OpenMAX IL port needs
   at instantiation time.
*/
class TOmxILCommonPortData
	{

public:

	// OMX specification version information
	OMX_VERSIONTYPE iOmxVersion;
	// Port number the structure applies to
	OMX_U32 iPortIndex;
	// Direction (input or output) of this port
	OMX_DIRTYPE iDirection;
    // The minimum number of buffers this port requires
	OMX_U32 iBufferCountMin;
	// Minimum size, in bytes, for buffers to be used for this port
	OMX_U32 iBufferSizeMin;
	// Domain of the port
	OMX_PORTDOMAINTYPE iPortDomain;
	// Buffers contiguous requirement (true or false)
	OMX_BOOL iBuffersContiguous;
	// Buffer aligment requirements
	OMX_U32 iBufferAlignment;
	// Port supplier preference when tunneling between two ports
	OMX_BUFFERSUPPLIERTYPE iBufferSupplier;

	// \brief This is the associated port that will be used to propagate buffer marks
	// found in incoming buffer headers.
	//
	// In general, for any output port, this parameter should be configured
	// with constant COmxILPort::KBufferMarkPropagationPortNotNeeded except
	// for an output port of a source component, in which case it must be used
	// the same index of the port that this structure applies to. For an input
	// port of a sink component,
	// COmxILPort::KBufferMarkPropagationPortNotNeeded must also be used.
	OMX_U32 iBufferMarkPropagationPortIndex;

	// Constructor
	IMPORT_C TOmxILCommonPortData(
		OMX_VERSIONTYPE aOmxVersion,
		OMX_U32 aPortIndex,
		OMX_DIRTYPE aDirection,
		OMX_U32 aBufferCountMin,
		OMX_U32 aBufferSizeMin,
		OMX_PORTDOMAINTYPE aPortDomain,
		OMX_BOOL aBuffersContiguous,
		OMX_U32 aBufferAlignment,
		OMX_BUFFERSUPPLIERTYPE aBufferSupplier,
		OMX_U32 aBufferMarkPropagationPortIndex);

	};

/**
   Buffer mark info structure
*/
class COmxILPort::TBufferMarkInfo
	{

public:

	inline explicit TBufferMarkInfo(
		const OMX_MARKTYPE*& apMark,
		TBool aOwnMark = ETrue);

	inline TBufferMarkInfo(
		OMX_HANDLETYPE& apMarkTargetComponent,
		OMX_PTR& apMarkData,
		TBool aOwnMark = ETrue);

public:

	OMX_HANDLETYPE ipMarkTargetComponent;
	OMX_PTR ipMarkData;
	TBool iOwnMark;
	TSglQueLink iLink;

	};

/**
   Buffer info structure
*/
class COmxILPort::TBufferInfo
	{

public:

	enum THeaderLocationProperty
		{
		EBufferAtHome	= 0x01,
		EBufferAway		= 0x00
		};

	enum THeaderOwnershipProperty
		{
		EBufferOwned	= 0x02,
		EBufferNotOwned = 0x00
		};

public:

	inline TBufferInfo(OMX_BUFFERHEADERTYPE* apHeader,
					   THeaderLocationProperty aLocation,
					   THeaderOwnershipProperty aOwnership,
					   OMX_U8* apBuffer,
					   OMX_PTR apApp,
					   OMX_PTR apPlatform,
					   OMX_PTR apPort);

	inline explicit TBufferInfo(OMX_BUFFERHEADERTYPE* apHeader);

	IMPORT_C static TBool Compare(const TBufferInfo& aBi1,
								  const TBufferInfo& aBi2);

	inline const OMX_BUFFERHEADERTYPE* GetHeader() const;

	inline OMX_BUFFERHEADERTYPE* GetHeader();

	inline operator OMX_BUFFERHEADERTYPE*&();

	inline operator OMX_BUFFERHEADERTYPE* const&() const;

	inline OMX_U8* GetBufferPointer() const;

	inline OMX_U8* GetBufferPointer();

	inline OMX_PTR GetPortPointer() const;

	inline OMX_PTR GetPortPointer();

	inline OMX_PTR GetPlatformPointer() const;

	inline OMX_PTR GetPlatformPointer();

	inline OMX_PTR GetAppPointer() const;

	inline OMX_PTR GetAppPointer();

	inline void SetBufferAtHome();

	inline void SetBufferAway();

	inline void SetBufferOwned();

	inline void SetBufferNotOwned();

	inline TBool IsBufferAtHome() const;

	inline TBool IsBufferOwned() const;

private:

	enum THeaderPropertyMask
		{
		EBufferAwayMask		= 0xFE,
		EBufferNotOwnedMask = 0xFD
		};

private:

	OMX_BUFFERHEADERTYPE* ipHeader;
	TUint8 iBufferProperties;
	OMX_U8* ipBuffer;
	OMX_PTR ipApp;
	OMX_PTR ipPlatform;
	OMX_PTR ipPort;

	};

#include "omxilport.inl"

#endif // OMXILPORT_H
