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
 * @file
 * @internalTechnology
 */

#include <uri8.h>
#include "omxilcallbackmanager.h"
#include "omxilfilesourceprocessingfunction.h"

const TInt KMaxMsgQueueEntries = 25;


COmxILFileSourceProcessingFunction* COmxILFileSourceProcessingFunction::NewL(MOmxILCallbackNotificationIf& aCallbacks)
	{
	COmxILFileSourceProcessingFunction* self = new (ELeave) COmxILFileSourceProcessingFunction(aCallbacks);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

COmxILFileSourceProcessingFunction::COmxILFileSourceProcessingFunction(MOmxILCallbackNotificationIf& aCallbacks)
: COmxILProcessingFunction(aCallbacks)
	{
	}

void COmxILFileSourceProcessingFunction::ConstructL()
	{
	iState = OMX_StateLoaded;
	iFileSourceAccess = CFileSourceAccess::NewL(*this);
	iPFHelper = CPFHelper::NewL(*this, *iFileSourceAccess);
	}

COmxILFileSourceProcessingFunction::~COmxILFileSourceProcessingFunction()
	{
	if(iPFHelper &&
	   (iState == OMX_StateInvalid  ||
	    iState == OMX_StateExecuting ||
	    iState == OMX_StatePause))
		{
		iPFHelper->StopSync();
		}

	delete iPFHelper;
	delete iFileSourceAccess;
	delete iUri;
	delete iFileName;

	// Buffer headers are not owned by the processing function
    iBuffersToFill.Close();
	}

OMX_ERRORTYPE COmxILFileSourceProcessingFunction::StateTransitionIndication(COmxILFsm::TStateIndex aNewState)
	{
	OMX_ERRORTYPE err = OMX_ErrorNone;
	switch(aNewState)
		{
		case COmxILFsm::EStateExecuting:
			{
			if (iPFHelper->ExecuteAsync() != KErrNone)
				{
				err = OMX_ErrorInsufficientResources;
				}
			}
			break;

		case COmxILFsm::EStateInvalid:
			{
            if (iPFHelper && iPFHelper->StopAsync() != KErrNone)
                {
                err = OMX_ErrorInsufficientResources;
                }

			}
			break;

		case COmxILFsm::EStatePause:
			{
			iPFHelper->PauseAsync();
			}
			break;

		case COmxILFsm::EStateIdle:
			{			
			iPFHelper->IdleAsync();
			}
			break;

		case COmxILFsm::EStateLoaded:
		case COmxILFsm::EStateWaitForResources:
			{
			if (iPFHelper && iPFHelper->StopAsync() != KErrNone)
				{
				err = OMX_ErrorInsufficientResources;
				}
			}
			break;

		case COmxILFsm::ESubStateExecutingToIdle:
			{
			iPFHelper->StopAsync();
			}
			break;
		case COmxILFsm::ESubStatePauseToIdle:
	    case COmxILFsm::ESubStateLoadedToIdle:
	    case COmxILFsm::ESubStateIdleToLoaded:
			break;

		default:
			{
			err = OMX_ErrorIncorrectStateTransition;
			}
		};

	return err;
	}

OMX_ERRORTYPE COmxILFileSourceProcessingFunction::BufferFlushingIndication(TUint32 aPortIndex, OMX_DIRTYPE aDirection)
	{
    OMX_ERRORTYPE err = OMX_ErrorNone;
    if ((aPortIndex == OMX_ALL && aDirection == OMX_DirMax) ||
        (aPortIndex == 0 && aDirection == OMX_DirOutput))
        {
        // Send BufferDone notifications for each bufer...
        for (TUint i=0, bufferCount=iBuffersToFill.Count(); i<bufferCount; ++i)
            {
            OMX_BUFFERHEADERTYPE* pBufferHeader = iBuffersToFill[i];
            iCallbacks.BufferDoneNotification(pBufferHeader,
                                              pBufferHeader->nOutputPortIndex,
                                              OMX_DirOutput);
            }
        // Empty buffer lists...
        iBuffersToFill.Reset();
        }
    else
        {
        err = OMX_ErrorBadParameter;
        }
    return err;
    }

OMX_ERRORTYPE COmxILFileSourceProcessingFunction::ParamIndication(OMX_INDEXTYPE aParamIndex, const TAny* apComponentParameterStructure)
	{
	OMX_ERRORTYPE err = OMX_ErrorNone;
	switch(aParamIndex)
		{
		case OMX_IndexParamPortDefinition:
			{
			//const OMX_PARAM_PORTDEFINITIONTYPE* portDefinition = static_cast<const OMX_PARAM_PORTDEFINITIONTYPE*>(apComponentParameterStructure);
			//nothing to do
			//
			//the number of buffers may change depending on capture mode (single shot vs burst mode)
			//in that case, we need to do something for PF...
			break;
			}
		case OMX_IndexParamContentURI:
			{
			const OMX_PARAM_CONTENTURITYPE* contentUriType = reinterpret_cast<const OMX_PARAM_CONTENTURITYPE*>(apComponentParameterStructure);
			err = SetFileName(contentUriType);
			break;
			}
		default:
			{
			err = OMX_ErrorUnsupportedIndex;
			}
		}
	return err;
	}

OMX_ERRORTYPE COmxILFileSourceProcessingFunction::ConfigIndication(OMX_INDEXTYPE /*aConfigIndex*/, const TAny* /*apComponentConfigStructure*/)
	{
	return OMX_ErrorNone;
	}

OMX_ERRORTYPE COmxILFileSourceProcessingFunction::BufferIndication(OMX_BUFFERHEADERTYPE* apBufferHeader, OMX_DIRTYPE aDirection)
	{
	if (aDirection == OMX_DirOutput)
    	{
    	if (iPFHelper->BufferIndication(apBufferHeader) != KErrNone)
    		{
    		return OMX_ErrorInsufficientResources;
    		}
		}
    else
    	{
    	return OMX_ErrorBadParameter;
    	}

    return OMX_ErrorNone;
	}

OMX_BOOL COmxILFileSourceProcessingFunction::BufferRemovalIndication(OMX_BUFFERHEADERTYPE* apBufferHeader, OMX_DIRTYPE /*aDirection*/)
	{
	OMX_BOOL headerDeletionResult = OMX_TRUE;

	TInt headerIndexInArray = KErrNotFound;
	if (KErrNotFound !=(headerIndexInArray = iBuffersToFill.Find(apBufferHeader)))
		{
		iBuffersToFill.Remove(headerIndexInArray);
		}
	else
		{
		headerDeletionResult = OMX_FALSE;
		}

    return headerDeletionResult;
	}

MOmxILCallbackNotificationIf& COmxILFileSourceProcessingFunction::GetCallbacks()
	{
	return iCallbacks;
	}

OMX_ERRORTYPE COmxILFileSourceProcessingFunction::SetFileName(const OMX_PARAM_CONTENTURITYPE* aContentUriType)
	{
	ASSERT(aContentUriType);
	delete iFileName;
	iFileName = NULL;
	delete iUri;
	iUri = NULL;

	TInt sizeOfUri = aContentUriType->nSize - _FOFF(OMX_PARAM_CONTENTURITYPE, contentURI); //Actual size of URI
	if (sizeOfUri <= 0)
		{
		return OMX_ErrorBadParameter;
		}

	// Don't include the zero character at the end.
	TPtrC8 uriDes(aContentUriType->contentURI,sizeOfUri);

	TInt err = KErrNone;
	do
		{
		TUriParser8 parser;
		err = parser.Parse(uriDes);
		if (err != KErrNone)
			{
			break;
			}

		TRAP(err, iFileName = parser.GetFileNameL());
		if (err != KErrNone)
			{
			break;
			}

		// Remove Null charcter '\0' if any.
		TPtr filePtr(iFileName->Des());
		TInt index = filePtr.LocateReverse('\0');
		if (index != KErrNotFound && index == filePtr.Length()-1)
		    {
		    filePtr.Delete(index,1);
		    }

		uriDes.Set(reinterpret_cast<const TUint8 *>(aContentUriType), aContentUriType->nSize );
        iUri = uriDes.Alloc();
        if (!iUri)
            {
            err = KErrNoMemory;
            break;
            }
        
		return OMX_ErrorNone;
		}
	while (EFalse);

	// Something failed.
	ASSERT(err != KErrNone);
	delete iFileName;
	iFileName = NULL;
	delete iUri;
	iUri = NULL;
	return (err == KErrNoMemory ? OMX_ErrorInsufficientResources : OMX_ErrorBadParameter);
	}


const HBufC* COmxILFileSourceProcessingFunction::FileName() const
	{
	return iFileName;
	}

const HBufC8* COmxILFileSourceProcessingFunction::Uri() const
	{
	return iUri;
	}

COmxILFileSourceProcessingFunction::CFileSourceAccess* COmxILFileSourceProcessingFunction::CFileSourceAccess::NewL(COmxILFileSourceProcessingFunction& aParent)
	{
	CFileSourceAccess* self = new (ELeave) CFileSourceAccess(aParent);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

COmxILFileSourceProcessingFunction::CFileSourceAccess::CFileSourceAccess(COmxILFileSourceProcessingFunction& aParent)
	: CActive(EPriorityStandard),
	iParent(aParent),
	iBufferOffset(0),
	iReadBuffer(0,0)
	{
	CActiveScheduler::Add(this);
	}

void COmxILFileSourceProcessingFunction::CFileSourceAccess::ConstructL()
	{
	User::LeaveIfError(iFs.Connect());
	}

COmxILFileSourceProcessingFunction::CFileSourceAccess::~CFileSourceAccess()
	{
	Cancel();

	iFileHandle.Close();
	iFs.Close();
	}

void COmxILFileSourceProcessingFunction::CFileSourceAccess::RunL()
	{
	// The buffer is not on the list implies that they have already been flushed/spotted
	// via BufferFlushingIndication/BufferRemovalIndication
	TInt index = iParent.iBuffersToFill.Find(iCurrentBuffer);
	if (KErrNotFound != index)
		{
		switch(iStatus.Int())
			{
			case KErrNone:
				{
				if (iReadBuffer.Length()==0) //the end of the file
				    {
				    iFileHandle.Close();
				    iCurrentBuffer->nFlags |= OMX_BUFFERFLAG_EOS;
				    iParent.GetCallbacks().EventNotification(OMX_EventBufferFlag, iCurrentBuffer->nOutputPortIndex, OMX_BUFFERFLAG_EOS, NULL);
				    }
				iCurrentBuffer->nFilledLen=iReadBuffer.Length();
				iCurrentBuffer->nOffset = 0;
				break;
				}
			default:
				{
				User::Leave(iStatus.Int());
				}
			};

		iParent.GetCallbacks().BufferDoneNotification(iCurrentBuffer,iCurrentBuffer->nOutputPortIndex,OMX_DirOutput);
		iParent.iBuffersToFill.Remove(index);
		iCurrentBuffer = NULL;
		if(iFileHandle.SubSessionHandle() != 0)
			{
			ProcessNextBuffer();
			}
		}
	}

TInt COmxILFileSourceProcessingFunction::CFileSourceAccess::ProcessNextBuffer()
	{
	TInt err = KErrNone;
	if ((iParent.iBuffersToFill.Count() > 0) && !IsActive() && iParent.iState == OMX_StateExecuting)
		{
		iCurrentBuffer = iParent.iBuffersToFill[0];
		iReadBuffer.Set(iCurrentBuffer->pBuffer, iCurrentBuffer->nAllocLen, iCurrentBuffer->nAllocLen);
        if (iFileHandle.SubSessionHandle() == 0)
            { 
            SetActive();
            TRequestStatus* status(&iStatus);
            User::RequestComplete(status, KErrNone);
            }
        else
            {
            iFileHandle.Read(iReadBuffer, iCurrentBuffer->nAllocLen, iStatus);
            SetActive();
            }					
		}

	return err;
	}

void COmxILFileSourceProcessingFunction::CFileSourceAccess::DoCancel()
	{
	if (iFileHandle.SubSessionHandle() != 0)
	    {
	    iFileHandle.Close();
	    }
	iCurrentBuffer = NULL;
	}

TInt COmxILFileSourceProcessingFunction::CFileSourceAccess::Execute()
	{
	iParent.iState = OMX_StateExecuting;
	return ProcessNextBuffer();
	}

void COmxILFileSourceProcessingFunction::CFileSourceAccess::Pause()
	{
	iParent.iState = OMX_StatePause;
	}

void COmxILFileSourceProcessingFunction::CFileSourceAccess::Idle()
    {
    iParent.iState = OMX_StateIdle;
    iParent.iBuffersToFill.Reset();
    }



void COmxILFileSourceProcessingFunction::CFileSourceAccess::Stop()
	{
	if(iParent.iState == OMX_StateExecuting || iParent.iState == OMX_StatePause)
		{
		Cancel();
		iParent.iState = OMX_StateIdle;
		}
	}

COmxILFileSourceProcessingFunction::CPFHelper* COmxILFileSourceProcessingFunction::CPFHelper::NewL(COmxILFileSourceProcessingFunction& aParent, CFileSourceAccess& aFileSourceAccess)
	{
	CPFHelper* self = new (ELeave) CPFHelper(aParent, aFileSourceAccess);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

COmxILFileSourceProcessingFunction::CPFHelper::CPFHelper(COmxILFileSourceProcessingFunction& aParent, CFileSourceAccess& aFileSourceAccess)
: CActive(EPriorityStandard),
  iParent(aParent),
  iFileSourceAccess(aFileSourceAccess)
	{
	CActiveScheduler::Add(this);
	}

void COmxILFileSourceProcessingFunction::CPFHelper::ConstructL()
	{
	User::LeaveIfError(iMsgQueue.CreateLocal(KMaxMsgQueueEntries));
	SetActive();
	iMsgQueue.NotifyDataAvailable(iStatus);
	}

COmxILFileSourceProcessingFunction::CPFHelper::~CPFHelper()
	{
	Cancel();
	iMsgQueue.Close();
	}



void COmxILFileSourceProcessingFunction::CPFHelper::RunL()
	{
	TInt err = ProcessQueue();
	if (err != KErrNone)
		{
		iParent.GetCallbacks().ErrorEventNotification( ConvertSymbianErrorType(err) );
		}

	// setup for next callbacks
	SetActive();
	iMsgQueue.NotifyDataAvailable(iStatus);
	}

void COmxILFileSourceProcessingFunction::CPFHelper::DoCancel()
	{
	if (iMsgQueue.Handle())
		{
		ProcessQueue();	
		iMsgQueue.CancelDataAvailable();
		}
	}

TInt COmxILFileSourceProcessingFunction::CPFHelper::ProcessQueue()
	{
	TProcMessage msg;
	TInt err = KErrNone;

	while (iMsgQueue.Receive(msg) == KErrNone)
		{
		switch (msg.iType)
			{			
			case EExecuteCommand:
				{
                const HBufC* fileName = iParent.FileName();
	            if (fileName && iFileSourceAccess.iFileHandle.SubSessionHandle() == 0)
                    {
                    err = iFileSourceAccess.iFileHandle.Open(iFileSourceAccess.iFs, *fileName, EFileRead | EFileShareReadersOnly);
                    }
	               else
	                    {
//#define KHRONOS_CONFORMANCE 
// The KHRONOS_CONFORMANCE is required to pass the following khronos conformance component tests (FlushTest,
// BaseMultiThreadedTest, StateTransitionTest, PortDisableEnableTest, IncompleteStopTest and PortCommunicationTest)
// that expects no error to be returned when a filesource filename is not set. 
	               
#ifdef KHRONOS_CONFORMANCE  
// do nothing, don't report any error
#else
	                    err =  KErrPathNotFound;
#endif
	                    }
                if ( err == KErrNone)
                    {
                    err = iFileSourceAccess.Execute();
                    }
				break;
				}

			case EStopCommand:
				{
				iFileSourceAccess.Stop();
				break;
				}

			case EPauseCommand:
				{
				iFileSourceAccess.Pause();
				break;
				}
				
			case EIdleCommand:
			    {
			    iFileSourceAccess.Idle();
			    break;
			    }
			case EBufferIndication:
				{
				OMX_BUFFERHEADERTYPE* bufferHeader = reinterpret_cast<OMX_BUFFERHEADERTYPE*>(msg.iPtr);

				if ( bufferHeader && (iParent.iState == OMX_StateExecuting || 
				        iParent.iState == OMX_StatePause || iParent.iState == OMX_StateIdle) )
					{
					err = iParent.iBuffersToFill.Append(bufferHeader);
					if(err == KErrNone)
						{
						if(iParent.iState != OMX_StateIdle)
						    {
						    err = iFileSourceAccess.ProcessNextBuffer();
						    }
						}
					else
					    {
					    // to prevent potential buffer leakage if the Append operation fails 
					    iParent.GetCallbacks().BufferDoneNotification(bufferHeader, bufferHeader->nOutputPortIndex,OMX_DirOutput);
					    }
					}
				break;
				}
			default:
				{
				break;
				}
			}

		if (err)
			{
			break;
			}
		}
	return err;
	}

TInt COmxILFileSourceProcessingFunction::CPFHelper::ExecuteAsync()
	{
	TProcMessage message;
	message.iType = EExecuteCommand;
	return iMsgQueue.Send(message);
	}

TInt COmxILFileSourceProcessingFunction::CPFHelper::StopAsync()
	{
	TProcMessage message;
	message.iType = EStopCommand;
	return iMsgQueue.Send(message);
	}

TInt COmxILFileSourceProcessingFunction::CPFHelper::PauseAsync()
    {
    TProcMessage message;
    message.iType = EPauseCommand;
    return iMsgQueue.Send(message);
    }


TInt COmxILFileSourceProcessingFunction::CPFHelper::BufferIndication(OMX_BUFFERHEADERTYPE* apBufferHeader)
	{
	TProcMessage message;
	message.iType = EBufferIndication;
	message.iPtr = apBufferHeader;
	return iMsgQueue.Send(message);
	}

void COmxILFileSourceProcessingFunction::CPFHelper::StopSync()
    {
    // Cancel to process the existing queue before handling this command
    Cancel();
    iFileSourceAccess.Stop();

    // setup for next callbacks
    SetActive();
    iMsgQueue.NotifyDataAvailable(iStatus);
    }

TInt COmxILFileSourceProcessingFunction::CPFHelper::IdleAsync()
    {
    TProcMessage message;
    message.iType = EIdleCommand;
    return iMsgQueue.Send(message);
    }


/**
 Converts a Symbian error code to an OpenMAX error code.
 @param     aError The Symbian error code.
 @return    The OpenMAX error code.
 */
OMX_ERRORTYPE COmxILFileSourceProcessingFunction::CPFHelper::ConvertSymbianErrorType(TInt aError)
    {
    // In the current implementation this function is only used for the return code in the 
    // callback methods. Currently the only expected errors KErrNone and KErrOverflow.
    
    OMX_ERRORTYPE err = OMX_ErrorNone;
    switch (aError)
        {
    case KErrNone:
        err = OMX_ErrorNone;
        break;
    case KErrOverflow:
    case KErrNoMemory:
        err = OMX_ErrorInsufficientResources;
        break;
    case KErrNotSupported:
        err = OMX_ErrorNotImplemented;
        break;
    case KErrNotReady:
        err = OMX_ErrorNotReady;
        break;
    case KErrGeneral:
    default:
        err = OMX_ErrorUndefined;
        }
    return err;
    }
