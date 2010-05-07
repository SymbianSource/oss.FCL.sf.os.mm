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

inline OMX_DIRTYPE
COmxILPort::Direction() const
	{
	return iParamPortDefinition.eDir;
	}

inline TBool
COmxILPort::IsEnabled() const
	{
	return iParamPortDefinition.bEnabled;
	}

inline TBool
COmxILPort::IsPopulated() const
	{
	return (OMX_TRUE == iParamPortDefinition.bPopulated ? ETrue : EFalse);
	}

inline TBool
COmxILPort::IsDePopulated() const
	{
	return (iBufferHeaders.Count() == 0 ? ETrue : EFalse);
	}

inline TBool
COmxILPort::IsTunnelled() const
	{
	return (iTunnelledComponent != 0 ? ETrue : EFalse);
	}

inline TBool
COmxILPort::IsTunnelledAndBufferSupplier() const
	{
	TBool retValue = (
		iTunnelledComponent &&
		(((iParamCompBufferSupplier.eBufferSupplier ==
		   OMX_BufferSupplyInput)
		  &&
		  (iParamPortDefinition.eDir == OMX_DirInput))
		 ||
		 ((iParamCompBufferSupplier.eBufferSupplier == OMX_BufferSupplyOutput)
		  &&
		  (iParamPortDefinition.eDir == OMX_DirOutput)))
		);

	return retValue;

	}

inline TBool
COmxILPort::IsTransitioningToEnabled() const
	{
	return (iTransitionState == EPortTransitioningToEnabled ? ETrue : EFalse);
	}

inline TBool
COmxILPort::IsTransitioningToDisabled() const
	{
	return (iTransitionState == EPortTransitioningToDisabled ? ETrue : EFalse);
	}

inline OMX_U32
COmxILPort::Index() const
	{
	return iParamPortDefinition.nPortIndex;
	}

inline OMX_PORTDOMAINTYPE
COmxILPort::Domain() const
	{
	return iParamPortDefinition.eDomain;
	}

inline OMX_U32
COmxILPort::Count() const
	{
	return iBufferHeaders.Count();
	}

inline OMX_BUFFERHEADERTYPE* const&
COmxILPort::operator[](TInt anIndex) const
	{
	return this->operator[](anIndex);
	}

inline OMX_BUFFERHEADERTYPE*&
COmxILPort::operator[](TInt anIndex)
	{
	__ASSERT_ALWAYS((anIndex>=0 && anIndex<iBufferHeaders.Count()),
					User::Panic(KOmxILPortPanicCategory, 1));
	return iBufferHeaders[anIndex];
	}


inline OMX_U32
COmxILPort::BufferMarkPropagationPort() const
	{
	return iBufferMarkPropagationPortIndex;
	}


template<typename T>
inline OMX_ERRORTYPE
COmxILPort::GetParamStructureFromTunnel(
	T& aComponentConfigStructure, OMX_INDEXTYPE aParamIndex) const
	{

	__ASSERT_ALWAYS(iTunnelledComponent,
					User::Panic(KOmxILPortPanicCategory, 1));

	aComponentConfigStructure.nSize		 = sizeof(T);
	aComponentConfigStructure.nVersion	 = TOmxILSpecVersion();
	aComponentConfigStructure.nPortIndex = iTunnelledPort;

	if (OMX_ErrorNone !=
		OMX_GetParameter(iTunnelledComponent,
						 aParamIndex,
						 &aComponentConfigStructure) )
		{
		return OMX_ErrorUndefined;
		}

	return OMX_ErrorNone;

	}



inline
COmxILPort::TBufferMarkInfo::TBufferMarkInfo(
	const OMX_MARKTYPE*& apMark,
	TBool aOwnMark /* = ETrue */)
	:
	ipMarkTargetComponent(apMark->hMarkTargetComponent),
	ipMarkData(apMark->pMarkData),
	iOwnMark(aOwnMark)
	{
	__ASSERT_DEBUG(ipMarkTargetComponent,
				   User::Panic(KOmxILPortPanicCategory, 1));
	}

inline
COmxILPort::TBufferMarkInfo::TBufferMarkInfo(
	OMX_HANDLETYPE& apMarkTargetComponent,
	OMX_PTR& apMarkData,
	TBool aOwnMark /* = ETrue */)
	:
	ipMarkTargetComponent(apMarkTargetComponent),
	ipMarkData(apMarkData),
	iOwnMark(aOwnMark)
	{
	__ASSERT_DEBUG(ipMarkTargetComponent,
				   User::Panic(KOmxILPortPanicCategory, 1));
	}

inline
COmxILPort::TBufferMarkInfoQue::TBufferMarkInfoQue(TInt aOffset)
	:
	Tq(aOffset),
	iNumElements(0)
	{
	}

inline COmxILPort::TBufferMarkInfo*
COmxILPort::TBufferMarkInfoQue::First() const
	{
	return Tq::First();
	}

inline void
COmxILPort::TBufferMarkInfoQue::AddLast(COmxILPort::TBufferMarkInfo& aRef)
	{
	Tq::AddLast(aRef);
	++iNumElements;
	}

inline void
COmxILPort::TBufferMarkInfoQue::Remove(COmxILPort::TBufferMarkInfo& aRef)
	{
	Tq::Remove(aRef);
	--iNumElements;
	}

inline TInt
COmxILPort::TBufferMarkInfoQue::Elements() const
	{
	return iNumElements;
	}

inline void
COmxILPort::TBufferMarkInfoQue::ResetAndDestroy()
	{
	while (!Tq::IsEmpty())
		{
		COmxILPort::TBufferMarkInfo* pMark = Tq::First();
		__ASSERT_DEBUG(pMark, User::Panic(KOmxILPortPanicCategory, 1));
		Tq::Remove(*pMark);
		delete pMark;
		}
	iNumElements = 0;
	}


inline
COmxILPort::TBufferInfo::TBufferInfo(
	OMX_BUFFERHEADERTYPE* apHeader,
	THeaderLocationProperty aLocation,
	THeaderOwnershipProperty aOwnership,
	OMX_U8* apBuffer,
	OMX_PTR apApp,
	OMX_PTR apPlatform,
	OMX_PTR apPort)
	:
	ipHeader(apHeader),
	iBufferProperties(0),
	ipBuffer(apBuffer),
	ipApp(apApp),
	ipPlatform(apPlatform),
	ipPort(apPort)
	{
	__ASSERT_DEBUG(ipHeader, User::Panic(KOmxILPortPanicCategory, 1));
	iBufferProperties = aLocation | aOwnership;
	}


// This constructor should only be used for array look-ups
inline
COmxILPort::TBufferInfo::TBufferInfo(
	OMX_BUFFERHEADERTYPE* apHeader)
	:
	ipHeader(apHeader),
	iBufferProperties(0),
	ipBuffer(0),
	ipApp(0),
	ipPlatform(0),
	ipPort(0)
	{
	__ASSERT_DEBUG(ipHeader, User::Panic(KOmxILPortPanicCategory, 1));
	iBufferProperties = EBufferAtHome | EBufferOwned;
	}


inline const OMX_BUFFERHEADERTYPE*
COmxILPort::TBufferInfo::GetHeader() const
	{
	return ipHeader;
	}

inline OMX_BUFFERHEADERTYPE*
COmxILPort::TBufferInfo::GetHeader()
	{
	return ipHeader;
	}

inline
COmxILPort::TBufferInfo::operator OMX_BUFFERHEADERTYPE*&()
	{
	return ipHeader;
	}

inline
COmxILPort::TBufferInfo::operator OMX_BUFFERHEADERTYPE* const&() const
	{
	return ipHeader;
	}

inline OMX_U8*
COmxILPort::TBufferInfo::GetBufferPointer() const
	{
	return ipBuffer;
	}

inline OMX_U8*
COmxILPort::TBufferInfo::GetBufferPointer()
	{
	return ipBuffer;
	}

inline OMX_PTR
COmxILPort::TBufferInfo::GetPortPointer() const
	{
	return ipPort;
	}

inline OMX_PTR
COmxILPort::TBufferInfo::GetPortPointer()
	{
	return ipPort;
	}

inline OMX_PTR
COmxILPort::TBufferInfo::GetPlatformPointer() const
	{
	return ipPlatform;
	}

inline OMX_PTR
COmxILPort::TBufferInfo::GetPlatformPointer()
	{
	return ipPlatform;
	}

inline OMX_PTR
COmxILPort::TBufferInfo::GetAppPointer() const
	{
	return ipApp;
	}

inline OMX_PTR
COmxILPort::TBufferInfo::GetAppPointer()
	{
	return ipApp;
	}

inline void
COmxILPort::TBufferInfo::SetBufferAtHome()
	{
	iBufferProperties |= EBufferAtHome;
	}

inline void
COmxILPort::TBufferInfo::SetBufferAway()
	{
	iBufferProperties &= EBufferAwayMask;
	}

inline void
COmxILPort::TBufferInfo::SetBufferOwned()
	{
	iBufferProperties |= EBufferOwned;
	}

inline void
COmxILPort::TBufferInfo::SetBufferNotOwned()
	{
	iBufferProperties &= EBufferNotOwnedMask;
	}

inline TBool
COmxILPort::TBufferInfo::IsBufferAtHome() const
	{
	return ((iBufferProperties & EBufferAtHome) != 0x0 ? ETrue : EFalse);
	}

inline TBool
COmxILPort::TBufferInfo::IsBufferOwned() const
	{
	return ((iBufferProperties & EBufferOwned) != 0x0 ? ETrue : EFalse);
	}


