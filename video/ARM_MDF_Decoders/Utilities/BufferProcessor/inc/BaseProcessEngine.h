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

#ifndef _CBASEPROCENGINE_H_
#define _CBASEPROCENGINE_H_

#include <e32base.h>
#include "codec.h"
#include "BaseEngine.h"

// Forward Declarations
class TVideoPicture;
class MMMFClockSource;

class CBaseProcessEngine : public CActive
	{
	public:
		enum TState {
		ENil = 0,
		EStart,
		EStop
		};

	public: // constructor and destructor
		//
		static CBaseProcessEngine* NewL ( MProcessEngineObserver* aObserver,
										  MBaseCodec* aCodec,
										  TBool aInPlaceProcessing,
                                          TInt aPriority = EPriorityStandard);
		// Destructor
		virtual ~CBaseProcessEngine ();
	
	private: // constructor

		// default constructor
		void ConstructL ( MProcessEngineObserver* aObserver,
						  MBaseCodec* aCodec,
						  TBool aInPlaceProcessing );
		//
		CBaseProcessEngine(TInt aPriority = EPriorityStandard);


	public: // API's 
		// To start the engine
		TInt Start ();

		// To stop the engine
		TInt Stop ();

		//Add the input buffer to the free input queue
		TInt AddInput( TAny* aBuffer );

		//Add the output buffer to the free output queue
		TInt AddOutput( TAny* aBuffer );
		
		// To check whether the engine is ready for processing
		TBool IsReadyForProcessing ();

		// Reset all the buffers.
		TInt Reset();
		
		// fetches the OutputBuffer from the process Engine OutPut Queue
		 TAny* FetchOutputBuffer();

		// fetches the InputBufer from the process Engine InPut Queue
		 TAny* FetchInputBuffer();

		
		// Returns the number of input buffers in process engine
		virtual TInt NumInputBuffers ();

		// Returns the number of output buffers in process engine
		virtual TInt NumOutputBuffers ();
		
		// Set the synchronize options
		virtual TInt SetSyncOptions(TAny* aClockSource,TInt aProcessingTime = 0);

	private: // from CActive
		void DoCancel ();

		void RunL ();

		TInt RunError ( TInt aError );

		// other functions
		void IssueRequest ();
		// to get the input
		TAny* GetInput ();
		// To get the output
		TAny* GetOutput ();
		/**
		* Function to decide if processing should be done or not
		*     
		* @return :  TInt: KErrNone
		*/
		TBool CanProcess(TVideoPicture *aPicture);

	private: // data
		RArray<TAny*>			iInputArray;
		RArray<TAny*>			iOutputArray;
		MBaseCodec*				iCodec;
		MProcessEngineObserver*	iObserver;
		TAny*					iInput;
		TAny*					iOutput;
		TState					iState;
		TInt					iPrevResult;
		TBool					iInPlaceProcessing;
		MMMFClockSource*		iClockSource;
		TInt					iTimeTakenForProcessing;
		
	};
#endif //_CBASEPROCENGINE_H_