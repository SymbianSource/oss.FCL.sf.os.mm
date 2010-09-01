// Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef ASYNCFILEWRITER_H

#include <e32base.h>

#define ASYNCFILEWRITER_H
/**
   @file
   @internalComponent
*/

#if defined(SYMBIAN_ENABLE_ENCODER_ASYNC_WRITES)

/**
	Callback interface which is used to notify a client that a buffer has been 
	written
*/
class MBufferWrittenObserver
    {
public:
    virtual void BufferWritten(const TPtrC8& aWritten, TInt aErrorCode)=0;
    };

class CBufferPool;
class RFile;

// can be used to emulate slow media on emulator
// for debugging/unittest purpose
#if ( defined(__WINS__) || defined(_DEBUG) )
# define WRITER_EMULATE_SLOW_MEDIA
#endif

#ifdef WRITER_EMULATE_SLOW_MEDIA
const TUid KUidIclTestEmulateSlowMedia = { 0x10273823 };
class CCallbackTimer;
#endif

/**
	Asynchronous file writer which manages memory buffers and provides with 
	asynchronous file writing service, which gives performance increase 
	in case of writing large files (> 64K) onto slow/high latency media
*/
class CAsyncFileWriter; //declared here
NONSHARABLE_CLASS( CAsyncFileWriter ): public CActive
    {
private:    
    enum
    	{
    	// defines maximum number of pending buffers which can be held in the Q, must be power of 2
    	KMaxNumOfBuffers = 4,
    	};
public:
            
    static CAsyncFileWriter* NewL(MBufferWrittenObserver& aObserver, RFile& aFile, TInt aBufferSize);
    
    ~CAsyncFileWriter();
    
    /**
        To check a buffer out of the buffer pool, 
        @return buffer pointer
    */
    void CheckBufferOut(TPtr8& aBuffer);
    /**
        To return a buffer back to the buffer pool, 
        @param  buffer pointer
    */    
    void CheckBufferIn(const TPtrC8& aBuffer);
    
    /**
        To put a buffer into file writing Q
    */
    void WriteBufferL(const TDesC8& aBuf, TInt aFilePos);
    /**
        To write all the pending buffers synchronously
    */
    TInt FlushBuffers();
    
private:
    CAsyncFileWriter(MBufferWrittenObserver& aObserver, RFile& aFile);
    
    void ConstructL(TInt aBufferSize);
    
    void RunL();
    void DoCancel();
    TInt WriteNextBuffer();
    TInt WaitForCurrentBuffer();
    void HandleBufferCompletion(TInt aError);
    
    TPtrC8 iBufPtr[KMaxNumOfBuffers];	// array of pending buffer pointers
    TInt iFilePos[KMaxNumOfBuffers];	// array of positions within the destination file
    TInt iCurrentBuf;					// index of the current buffer being written
    TInt iNumOfPendingBuffs;			// number of buffer pointer in the Q
    MBufferWrittenObserver& iObserver;	// observer interface pointer
    RFile* const iFile;					// destination file handle
    CBufferPool* iBufferPool;			// memory buffer manager
    
#ifdef WRITER_EMULATE_SLOW_MEDIA				// can be used to emulate slow media (for testing)
	static TInt TimerGate(TAny* aPtr);
	CCallbackTimer*	iCallbackTimer;
	TBool iDelayOff;
public:	
	TBool iEmulateSlowMedia;
#endif
    };

#endif // defined(SYMBIAN_ENABLE_ENCODER_ASYNC_WRITES)

#endif // ndef ASYNCFILEWRITER_H

