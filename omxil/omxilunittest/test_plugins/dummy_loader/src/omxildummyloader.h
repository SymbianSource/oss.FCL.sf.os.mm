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
 @prototype
*/

#ifndef OMXILDUMMYLOADER_H
#define OMXILDUMMYLOADER_H

#include <e32base.h>
#include <e32des8.h> 
#include <openmax/il/loader/omxilcomponentif.h>
#include <openmax/il/core/omxilsymbianloaderif.h>

class COmxILDummyLoader : public CBase
	{
public:
	class TOmxComponentInfo
		{
	public:
		TOmxComponentInfo();
	public:
		TUid iUid;
		TBuf8<OMX_MAX_STRINGNAME_SIZE> iComponentName;
		TBuf8<OMX_MAX_ROLESBUFFER_SIZE> iRoles;
		};

public:
	OMX_HANDLETYPE Handle();

	COmxILDummyLoader(OMX_HANDLETYPE aLoader);
	~COmxILDummyLoader();

	OMX_ERRORTYPE InitializeComponentLoader();
	OMX_ERRORTYPE UnInitializeComponentLoader(OMX_LOADERTYPE* loader);
	OMX_ERRORTYPE LoadComponent(		OMX_HANDLETYPE* pHandle,
										const OMX_STRING cComponentName,
										OMX_PTR pAppData,
										OMX_CALLBACKTYPE* pCallBacks );
	OMX_ERRORTYPE UnloadComponent( 		OMX_HANDLETYPE hComponent );
	OMX_ERRORTYPE ComponentNameEnum(	OMX_STRING cComponentName,
								    	OMX_U32 nNameLength,
								    	const OMX_U32 nIndex );
	OMX_ERRORTYPE GetRolesOfComponent( 	const OMX_STRING compName,
								       	OMX_U32* pNumRoles,
								       	OMX_U8** roles );
	OMX_ERRORTYPE GetComponentsOfRole( 	const OMX_STRING role,
								       	OMX_U32* pNumComps,
								       	OMX_U8** compNames );
	
	operator OMX_HANDLETYPE();

private:
	void ConstructL();

private:
	OMX_LOADERTYPE* iHandle;
	
	RArray<TOmxComponentInfo> iComponentNameList;
	RPointerArray<COmxILComponentIf> iComponents;
	};

inline COmxILDummyLoader::TOmxComponentInfo::TOmxComponentInfo():
	iUid(TUid::Uid(0)),iComponentName(0),iRoles(0) 
	{
	}


#endif // OMXILDUMMYLOADER_H
