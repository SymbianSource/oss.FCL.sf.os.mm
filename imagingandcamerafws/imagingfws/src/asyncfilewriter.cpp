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

#if defined(SYMBIAN_ENABLE_ENCODER_ASYNC_WRITES)

#include <f32file.h>

#include "ImageClientMain.h"

#include "asyncfilewriter.h"

/**
   @file
   @internalComponent
*/

/**
	Simple buffer manager can be used for checking in/out memory buffers
*/
class CBufferPool; // declared here
NONSHARABLE_CLASS( CBufferPool ): public CBase
    {
private:    
    enum
        {
        // defines maximum number of managed buffers, each would be aBufSize size
        EMaxNumOfBuffers = 3
        };
public: 
    static CBufferPool* NewL(TInt aBufSize);
    ~CBufferPool();
    
    TPtr8 TakeBuffer();
    void ReleaseBuffer(const TPtrC8& aPtr);
    inline TBool HasSpareBuffers() const;
    
private:
    explicit CBufferPool(TInt aSubBufSize);
    void ConstructL();

    TText8* iBufPtr[EMaxNumOfBuffers];	// array of buffer pointers 
    TBool iIsBufTaken[EMaxNumOfBuffers];// array of ETrue if the buffers is taken
    TInt iNumOfTaken;					// number of checked out buffers
    TInt iSubBufSize;					// size of a single buffer
    TText8* iBuffer;					// single contiguous buffer
    };

/*static*/
CBufferPool* CBufferPool::NewL(TInt aBufSize)
    {
    CBufferPool* self = new (ELeave) CBufferPool(aBufSize);
    CleanupStack::PushL(self);
    self->ConstructL();
	CleanupStack::Pop(self);
    return self;
    }

CBufferPool::CBufferPool(TInt aSubBufSize):
							iSubBufSize( Align4(aSubBufSize) )
    {
    }
    
void CBufferPool::ConstructL()
	{
	// allocate a buffer which is guaranteed to be aligned per TUint size boundary,
	// so add sizeof(TUint) as precaution
	//
    iBuffer = reinterpret_cast<TText8*>( 
                                    Align4( User::AllocL( iSubBufSize * EMaxNumOfBuffers + sizeof(TUint) ) ) 
                               );
    TText8* bufPtr = iBuffer;
    for (TInt i=0; i < EMaxNumOfBuffers; i++)
        {
        iBufPtr[ i ] = bufPtr;
        bufPtr += iSubBufSize;
        }
	}

CBufferPool::~CBufferPool()
    {
    User::Free( iBuffer );
    }
    
inline 
TBool CBufferPool::HasSpareBuffers() const
	{
	return (iNumOfTaken < EMaxNumOfBuffers);
	}
    
TPtr8 CBufferPool::TakeBuffer()
    {
    for (TInt i=0; i < EMaxNumOfBuffers; i++)
        {
        if ( !iIsBufTaken[i])
            {
            iIsBufTaken[i] = ETrue;
            ++iNumOfTaken;
            return TPtr8(iBufPtr[i], iSubBufSize, iSubBufSize);
            }
        }
    Panic(EBufPoolNoMoreBuffers);
    return TPtr8(NULL, 0, 0);
    }
    
void CBufferPool::ReleaseBuffer(const TPtrC8& aPtr)
    {
    for (TInt i=0; i < EMaxNumOfBuffers; i++)
        {
        if ( iBufPtr[i] == aPtr.Ptr() )
            {
            ASSERT(iIsBufTaken[i]);
            --iNumOfTaken;
            iIsBufTaken[i] = EFalse;
            return;
            }
        }
    Panic(EBufPoolInvalidBuffer);
    }
    
    
#ifdef WRITER_EMULATE_SLOW_MEDIA
/*static*/
TInt CAsyncFileWriter::TimerGate(TAny* aPtr)
	{
	CAsyncFileWriter* self=reinterpret_cast<CAsyncFileWriter*>(aPtr);
	self->iFile->Write(self->iFilePos[self->iCurrentBuf], self->iBufPtr[self->iCurrentBuf], self->iStatus);	
	self->SetActive();
	return KErrNone;
	}
	
class CCallbackTimer; //declared here
NONSHARABLE_CLASS( CCallbackTimer ): public CTimer
	{
public:
	static CCallbackTimer* NewL();
	
	~CCallbackTimer();
	void After(TTimeIntervalMicroSeconds32 aCancelDelay, TCallBack aCallback);

private:
	void DoCancel();
	CCallbackTimer();
	// from CActive
	void RunL();

private:
	TCallBack iCallback; 
	};

CCallbackTimer::~CCallbackTimer()
    {
    Cancel();
    }
    
void CCallbackTimer::DoCancel()
	{
	CTimer::DoCancel();
	iCallback.CallBack();
	}
	
void CCallbackTimer::After(TTimeIntervalMicroSeconds32 aCancelDelay, TCallBack aCallback)
    {
    ASSERT( !IsActive() );
   	iCallback = aCallback;   
   	CTimer::After(aCancelDelay);
	}

void CCallbackTimer::RunL()
    {
    iCallback.CallBack();
    }

CCallbackTimer* CCallbackTimer::NewL()
    {
   	CCallbackTimer* self=new (ELeave) CCallbackTimer();
   	CleanupStack::PushL(self);
   	self->ConstructL();
   	CleanupStack::Pop();
    return self;
    }

CCallbackTimer::CCallbackTimer()
   					:CTimer(CActive::EPriorityUserInput)
	{
   	CActiveScheduler::Add(this);
	}
  
#endif // WRITER_EMULATE_SLOW_MEDIA    

CAsyncFileWriter* CAsyncFileWriter::NewL(MBufferWrittenObserver& aObserver, RFile& aFile, TInt aBufferSize)
    {
    CAsyncFileWriter* self=new (ELeave) CAsyncFileWriter(aObserver, aFile);
    CleanupStack::PushL( self );
    self->ConstructL( aBufferSize );
    CleanupStack::Pop( self );
    return self;
    }

CAsyncFileWriter::CAsyncFileWriter(MBufferWrittenObserver& aObserver, RFile& aFile):
                                                    CActive(EPriorityNormal),
                                                    iObserver(aObserver),
                                                    iFile(&aFile)
                                    
    {
    CActiveScheduler::Add(this);
    }
    
void CAsyncFileWriter::ConstructL(TInt aBufferSize)    
    {
    iBufferPool = CBufferPool::NewL( aBufferSize );
    
#ifdef WRITER_EMULATE_SLOW_MEDIA    
	iCallbackTimer = CCallbackTimer::NewL();    
#endif	
    }

CAsyncFileWriter::~CAsyncFileWriter()
    {
    Cancel();
    delete iBufferPool;
    
#ifdef WRITER_EMULATE_SLOW_MEDIA
	delete iCallbackTimer;
#endif    
    }
    
void CAsyncFileWriter::WriteBufferL(const TDesC8& aBuf,TInt aPos)
    {
    if (iNumOfPendingBuffs == KMaxNumOfBuffers)
        {
		Panic( EAsyncWrtrQOverflow );
        }
    // place the buffer into Q
    iBufPtr[ (iCurrentBuf + iNumOfPendingBuffs ) & (KMaxNumOfBuffers - 1)  ].Set(aBuf);
    iFilePos[ (iCurrentBuf + iNumOfPendingBuffs ) & (KMaxNumOfBuffers - 1)  ] = aPos;
    if (++iNumOfPendingBuffs == 1)
        {
        ASSERT( !IsActive() );
        User::LeaveIfError( WriteNextBuffer() );
        }
    if (!iBufferPool->HasSpareBuffers())
    	{
    	TInt result = WaitForCurrentBuffer();
    	User::LeaveIfError( result );
    	HandleBufferCompletion( KErrNone );
    	}
    }
    
TInt CAsyncFileWriter::WaitForCurrentBuffer()
    {
#ifdef WRITER_EMULATE_SLOW_MEDIA
// cancel delay emulation timer and kick off real writing immediately
    iCallbackTimer->Cancel();
    if (!IsActive())
        {
        TimerGate(this);
        }
#endif

    Cancel();   // that would mean waiting for buffer to be written by the FServer
                // as we have to clear RequestStatus which can be done only 
                // by CActive
    return iStatus.Int();
    }
    
TInt CAsyncFileWriter::WriteNextBuffer()
    {
    TInt destSize = 0;
    TInt error = iFile->Size(destSize);
    if (error != KErrNone)
        {
        return error;
        }
      //If we start writing past EOF
    if (iFilePos[iCurrentBuf] > destSize)
        {
        error = iFile->SetSize( iFilePos[iCurrentBuf] );
        }
    if (error != KErrNone)
        {
        return error;
        }
#ifdef WRITER_EMULATE_SLOW_MEDIA
	if (!iEmulateSlowMedia || iDelayOff)
	{
#endif

    iFile->Write(iFilePos[iCurrentBuf], iBufPtr[iCurrentBuf], iStatus);
	SetActive();
	
#ifdef WRITER_EMULATE_SLOW_MEDIA
	}
	if (iEmulateSlowMedia && !iDelayOff)
		{
		iCallbackTimer->After(10000, TCallBack(TimerGate, this));	
		}
#endif

    return KErrNone;
    }

void CAsyncFileWriter::CheckBufferOut(TPtr8& aBuffer)
    {
    aBuffer.Set( iBufferPool->TakeBuffer() );
    }
    
void CAsyncFileWriter::CheckBufferIn(const TPtrC8& aBuffer)
    {
    iBufferPool->ReleaseBuffer( aBuffer );
    }
    
TInt CAsyncFileWriter::FlushBuffers()
    {
#ifdef WRITER_EMULATE_SLOW_MEDIA    
	iDelayOff = ETrue;
	iCallbackTimer->Cancel();
#endif

    TInt result = KErrNone;
    while ( iNumOfPendingBuffs > 0 && result == KErrNone)
        {
        result = WaitForCurrentBuffer();
        
        CheckBufferIn(iBufPtr[iCurrentBuf]);
        
        if ( result != KErrNone)
            {
            break;
            }
        if (--iNumOfPendingBuffs != 0)
            {
            iCurrentBuf = (iCurrentBuf + 1) & (KMaxNumOfBuffers - 1);
            result = WriteNextBuffer();
            }        
        }
    return result;
    }

void CAsyncFileWriter::HandleBufferCompletion(TInt aError)
    {
    if ( aError != KErrNone)
        {
        iObserver.BufferWritten(KNullDesC8(), aError );
        return;
        }

    TPtrC8 written( iBufPtr[iCurrentBuf] );
    iBufPtr[iCurrentBuf].Set( KNullDesC8 );
    
    TInt result = KErrNone;
    if (--iNumOfPendingBuffs != 0)
        {
        iCurrentBuf = (iCurrentBuf + 1) & (KMaxNumOfBuffers - 1);
        result = WriteNextBuffer();
        }

    iObserver.BufferWritten( (result == KErrNone ? written : iBufPtr[iCurrentBuf]) , result);
    }

void CAsyncFileWriter::RunL()
    {
    HandleBufferCompletion(iStatus.Int());
    }
    
void CAsyncFileWriter::DoCancel()
    {
    // we do nothing here since the File Server will complete our request
    // i.e. asynchronous File Server operations can't be cancelled
    }


#endif // defined(SYMBIAN_ENABLE_ENCODER_ASYNC_WRITES)

