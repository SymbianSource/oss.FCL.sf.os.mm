/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  
*
*/

#ifndef __BASEENGINEIMPLEMENTATION_H
#define __BASEENGINEIMPLEMENTATION_H

#include <e32def.h>
#include "BaseEngine.h"

//Forward declarations
class MProcessEngineObserver;
class CBaseProcessEngine;

class CBaseEngineImplementation : public CBaseEngine
	{
	public:
		static CBaseEngineImplementation* NewL ( MProcessEngineObserver* aObserver,
												 MBaseCodec* aCodec,
												 TBool aInPlaceProcessing,
                                                 TInt aPriority =CActive::EPriorityStandard);

		~CBaseEngineImplementation ();
	private:
		void ConstructL ( MProcessEngineObserver* aObserver,
						  MBaseCodec* aCodec,
						  TBool aInPlaceProcessing,
                          TInt aPriority);

		CBaseEngineImplementation ();

	private:// from BaseEngine
		// to add the input buffer to the Q
		TInt AddInput (TAny*);

		// to add the output buffer to the Q
		TInt AddOutput (TAny*);

		// starts the processing;
		TInt Start ();

		// stops the processing
		TInt Stop ();

		// Reset all the buffers.
		TInt Reset();
		// fetches the OutputBuffer from the process Engine OutPut Queue
		 TAny* FetchOutputBuffer();
		
		// fetches the InputBuffer from the process Engine InPut Queue
		 TAny* FetchInputBuffer();
		
		// Returns the number of input buffers in process engine
		virtual TInt NumInputBuffers ();

		// Returns the number of output buffers in process engine
		virtual TInt NumOutputBuffers ();

		// Set the synchronize options
		virtual TInt SetSyncOptions(TAny *, TInt aProcessingTime = 0);

	private: // data
		CBaseProcessEngine*		iEngineHandle;
	};

#endif //__BASEENGINEIMPLEMENTATION_H