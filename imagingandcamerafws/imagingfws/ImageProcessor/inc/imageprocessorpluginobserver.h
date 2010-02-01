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
 @publishedPartner 
 @released 
*/

#ifndef __IMAGE_PROCESSOR_PLUGIN_OBSERVER_H__
#define __IMAGE_PROCESSOR_PLUGIN_OBSERVER_H__

#include <e32base.h>
#include <imageprocessor/imageprocessor.h>

namespace ImageProcessor
	{

/**
Plugin API for the plugin of the observer of ImageProcessor.

@publishedPartner
@released
*/	
class MImgProcessorPluginObserver
	{
public:   
	/**
	The derived implementation class must provide an implementation to perform actions when any event of ImageProcessor finished
	*/
	virtual void PostEvent(TInt aEventId, TUid aUid, TInt aId, TInt aError) = 0;
	};

	}

#endif //__IMAGE_PROCESSOR_PLUGIN_OBSERVER_H__
