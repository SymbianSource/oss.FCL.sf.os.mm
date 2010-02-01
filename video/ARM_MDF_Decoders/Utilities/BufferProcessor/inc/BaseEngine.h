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

#ifndef __PROCESS_ENGINE_H
#define __PROCESS_ENGINE_H

#include <e32base.h>

class MBaseCodec;

class MProcessEngineObserver
	{
	public:

		// Callback to indicate the input buffer is consumed
		virtual TInt InputBufferConsumed ( TAny* aInp, TInt aError ) =0;

		// Callback to indicate output buffer is ready
		virtual TInt OutputBufferReady ( TAny* aOup, TInt aError ) = 0;

		// Callback to handle the error from RunL
		virtual void HandleError ( TInt aError ) = 0;

	};

class CBaseEngine: public CBase
	{
	public:
		IMPORT_C static CBaseEngine* NewL ( MProcessEngineObserver* aObserver,
										   MBaseCodec* aCodec,
										   TBool aInPlaceProcessing,
                                           TInt aPriority = CActive::EPriorityStandard);
		virtual ~CBaseEngine ()
			{
			}
	public:
		// to add the input buffer to the Q
		virtual TInt AddInput ( TAny* ) = 0;

		// to add the output buffer to the Q
		virtual TInt AddOutput ( TAny* ) = 0;

		// starts the processing;
		virtual TInt Start () = 0;

		// stops the processing
		virtual TInt Stop () = 0;

		// Reset all the buffers.
		virtual TInt Reset() =0;
		
		// Returns the number of input buffers in process engine
		virtual TInt NumInputBuffers () = 0;

		// Returns the number of output buffers in process engine
		virtual TInt NumOutputBuffers () = 0;

		// Set the clock source and minimum time taken  for PP
		virtual TInt SetSyncOptions(TAny* aClockSource,TInt aProcessingTime = 0) = 0;
		// fetches the OutputBuffer from the process Engine OutputQ
		virtual TAny* FetchOutputBuffer()=0;

		// fetches the InputBuffer from the process Engine INputQ
		virtual TAny* FetchInputBuffer()=0;

	};

#endif //__PROCESS_ENGINE_H