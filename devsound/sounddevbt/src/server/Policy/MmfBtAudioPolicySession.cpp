// Copyright (c) 2000-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "MmfBtAudioPolicySession.h"
#include "MmfBtPolicyClientServer.h"
#include "MmfBtAudioPolicyServer.h"
#include "MmfBtDevSoundInfo.h"



CMMFAudioPolicySession* CMMFAudioPolicySession::NewL()
	{
	CMMFAudioPolicySession* self = new(ELeave) CMMFAudioPolicySession();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}

void CMMFAudioPolicySession::ConstructL()
	{
	iAudioPolicyRequest = new (ELeave) CMMFAudioPolicyRequest();
	}

CMMFAudioPolicySession::CMMFAudioPolicySession() : iEventsQue(_FOFF(TMMFAudioPolicyEventHolder,iLink))
	{
	}

void CMMFAudioPolicySession::CreateL(const CMmfIpcServer& aServer)
	{
	iServer = STATIC_CAST(CMMFAudioPolicyServer*, (CONST_CAST(CMmfIpcServer*, &aServer)));
	iServer->IncrementSessionCount();
	CMmfIpcSession::CreateL(aServer);
	iServer->IncrementSessionId();
	iPolicySessionId = iServer->PolicySessionId();
	// Get ptr to AudioPolicy from the server
	iAudioPolicy = iServer->AudioPolicy();
	(iAudioPolicy->AudioPolicyRequestArray())->SetReserveL(iServer->PolicySessionCount());
	}

CMMFAudioPolicyServer* CMMFAudioPolicySession::Server()
	{
	return STATIC_CAST(CMMFAudioPolicyServer*, iServer);
	}

CMMFAudioPolicySession::~CMMFAudioPolicySession()
	{
	delete iEventReceiver;
	delete iAudioPolicyRequest;
	iEventReceiver=NULL;
	if (!iEventsQue.IsEmpty())
		{
		TMMFAudioPolicyEventHolder* heldEvent = iEventsQue.First();
		iEventsQue.Remove(*heldEvent);
		delete heldEvent;
		}
	iAudioPolicyPrioritySettings.iState = EMMFStateClosed;
	// Have session's CMMFAudioPolicyRequest object removed from list
	iAudioPolicy->RemoveFromList(iPolicySessionId);
	iServer->DecrementSessionCount();
	iServer->StopNotificationTimer();
	}

void CMMFAudioPolicySession::ServiceL(const RMmfIpcMessage& aMessage)
	{
	TBool complete = EFalse;
	switch(aMessage.Function())
		{
	case EMMFPolicyMakeRequest:
		MakeRequestL(aMessage);
		aMessage.Complete(KErrNone);
		break;
	case EMMFPolicySetDevSoundInfo:
		complete = SetDevSoundInfoL(aMessage);
		break;
	case EMMFPolicyUpdateState:
		complete = UpdateStateL(aMessage);
		break;
	case EMMFPolicyReceiveEvents:
		complete = ReceiveEventsL(aMessage);
		break;
	case EMMFPolicyCancelReceiveEvents:
		complete = CancelReceiveEvents();
		break;
	case EMMFPolicyGetPlayFormatsSupported:
		complete = GetPlayFormatsSupportedL(aMessage);
		break;
	case EMMFPolicyGetRecordFormatsSupported:
		complete = GetRecordFormatsSupportedL(aMessage);
		break;
	case EMMFPolicyGetPlayFormat:
		complete = GetPlayFormatL(aMessage);
		break;
	case EMMFPolicyGetRecordFormat:
		complete = GetRecordFormatL(aMessage);
		break;
	case EMMFPolicyLaunchRequests:
		complete = LaunchRequest();
		break;
	case EMMFPolicyRequestResourceNotification:
		complete = RequestResourceNotificationL(aMessage);
		break;
	case EMMFPolicyCancelRequestResourceNotification:
		complete = CancelRequestResourceNotificationL(aMessage);
		break;
	case EMMFPolicyStopNotification:
		complete = StopNotificationL(aMessage);
		break;
	case EMMFPolicyGetResourceNotificationEvent:
		complete = GetResourceNotificationEventL(aMessage);
		break;
	default:
		User::Leave(KErrNotSupported);
		break;
		}
	if(complete)
		{
		aMessage.Complete(KErrNone);
		}
	}

void CMMFAudioPolicySession::MakeRequestL(const RMmfIpcMessage& aMessage)
	{
	TMMFAudioPolicyPrioritySettingsPckg settingsPckg;
	MmfMessageUtil::ReadL(aMessage, 0, settingsPckg);

	iAudioPolicyRequest->SetPriority(settingsPckg().iPriority);
	iAudioPolicyRequest->SetPref(settingsPckg().iPref);
	iAudioPolicyRequest->SetState(settingsPckg().iState);

	iAudioPolicyRequest->SetCapabilities(settingsPckg().iCapabilities);
	
	// Set session Id in Request
	iAudioPolicyRequest->SetPolicySessionId(iPolicySessionId);
	iAudioPolicy->MakeRequest(iAudioPolicyRequest);
	}

TBool CMMFAudioPolicySession::UpdateStateL(const RMmfIpcMessage& aMessage)
	{
	TMMFAudioPolicyPrioritySettingsPckg settingsPckg;
	MmfMessageUtil::ReadL(aMessage, 0, settingsPckg);

	iAudioPolicyRequest->SetState(settingsPckg().iState);
	iAudioPolicy->ModifyEntry(iPolicySessionId, iAudioPolicyRequest); 
	return ETrue;
	}

void CMMFAudioPolicySession::SendEventToClient(TMMFAudioPolicyEvent& aEvent)
	{
	if (iEventReceiver)
		{
		iEventReceiver->SendEvent(aEvent);
		delete iEventReceiver;
		iEventReceiver=NULL;
		}
	else 
		{
		TMMFAudioPolicyEventHolder* heldEvent = new TMMFAudioPolicyEventHolder(aEvent);
		iEventsQue.AddLast(*heldEvent);
		}
	}

TBool CMMFAudioPolicySession::SetDevSoundInfoL(const RMmfIpcMessage& aMessage)
	{
	TMMFDevSoundInfoPckg devSoundInfoPckg;
	MmfMessageUtil::ReadL(aMessage, 0, devSoundInfoPckg);
	iDevSoundId = devSoundInfoPckg().iDevSoundId;
	return ETrue;
	}

TBool CMMFAudioPolicySession::ReceiveEventsL(const RMmfIpcMessage& aMessage)
	{
	if (iEventReceiver)
		User::Leave(KErrAlreadyExists);
	iEventReceiver = CMMFAudioPolicyEventReceiver::NewL(aMessage);
	//send the next cached event (if any) to the client
	if (!iEventsQue.IsEmpty())
		{
		TMMFAudioPolicyEventHolder* heldEvent = iEventsQue.First();
		iEventReceiver->SendEvent(heldEvent->iEvent);
		iEventsQue.Remove(*heldEvent);
		delete iEventReceiver;
		iEventReceiver=NULL;
		}
	return EFalse;
	}

TBool CMMFAudioPolicySession::CancelReceiveEvents()
	{
	delete iEventReceiver;
	iEventReceiver = NULL;
	return ETrue;
	}

TBool CMMFAudioPolicySession::GetPlayFormatsSupportedL(const RMmfIpcMessage& aMessage)
	{
	RMdaDevSound::TSoundFormatsSupportedBuf aPlayFormatsSupported;
	aPlayFormatsSupported = iAudioPolicy->MdaHwInfo()->GetPlayFormatsSupported();
	MmfMessageUtil::WriteL(aMessage, 0, aPlayFormatsSupported);
	return ETrue;
	}

TBool CMMFAudioPolicySession::GetRecordFormatsSupportedL(const RMmfIpcMessage& aMessage)
	{
	RMdaDevSound::TSoundFormatsSupportedBuf aRecordFormatsSupported;
	aRecordFormatsSupported = iAudioPolicy->MdaHwInfo()->GetRecordFormatsSupported();
	MmfMessageUtil::WriteL(aMessage, 0, aRecordFormatsSupported);
	return ETrue;
	}

TBool CMMFAudioPolicySession::GetPlayFormatL(const RMmfIpcMessage& aMessage)
	{
	RMdaDevSound::TCurrentSoundFormatBuf aPlayFormat;
	aPlayFormat = iAudioPolicy->MdaHwInfo()->GetPlayFormat();
	MmfMessageUtil::WriteL(aMessage, 0, aPlayFormat);
	return ETrue;
	}

TBool CMMFAudioPolicySession::GetRecordFormatL(const RMmfIpcMessage& aMessage)
	{
	RMdaDevSound::TCurrentSoundFormatBuf aRecordFormat;
	aRecordFormat = iAudioPolicy->MdaHwInfo()->GetRecordFormat();
	MmfMessageUtil::WriteL(aMessage, 0, aRecordFormat);
	return ETrue;
	}

TBool CMMFAudioPolicySession::LaunchRequest()
	{
	iAudioPolicy->LaunchRequest();
	return ETrue;
	}
	
TBool CMMFAudioPolicySession::RequestResourceNotificationL(const RMmfIpcMessage& aMessage)
	{
	TMMFAudioPolicyResourceNotificationSettingsPckg notificationPckg;
	MmfMessageUtil::ReadL(aMessage, 0, notificationPckg);
	iAudioPolicyRequest->SetNotificationEvent(notificationPckg().iNotificationUid);
	iAudioPolicy->SetNotification(iPolicySessionId,notificationPckg().iNotificationUid);
	return ETrue;
	}

TBool CMMFAudioPolicySession::CancelRequestResourceNotificationL(const RMmfIpcMessage& aMessage)
	{
	if(iServer->IsTimerActive() == EFalse)
		{
		iServer->StartNotificationTimer();
		}
	TMMFAudioPolicyResourceNotificationSettingsPckg notificationPckg;
	MmfMessageUtil::ReadL(aMessage, 0, notificationPckg);
	iAudioPolicyRequest->ResetNotificationEvent(notificationPckg().iNotificationUid);
	iAudioPolicy->SetNotification(iPolicySessionId,KNullUid);
	return ETrue;
	}

TBool CMMFAudioPolicySession::StopNotificationL(const RMmfIpcMessage& /*aMessage*/)
	{
	iServer->StopNotificationTimer();
	return ETrue;
	}
	
TBool CMMFAudioPolicySession::GetResourceNotificationEventL(const RMmfIpcMessage& aMessage)
	{
	TMMFAudioPolicyResourceNotificationSettingsPckg notificationPckg;
	notificationPckg().iNotificationUid = iAudioPolicyRequest->NotificationEvent();
	MmfMessageUtil::WriteL(aMessage, 0, notificationPckg);
	return ETrue;
	}
	
CMMFAudioPolicyEventReceiver* CMMFAudioPolicyEventReceiver::NewL(const RMmfIpcMessage& aMessage)
	{
	return new(ELeave) CMMFAudioPolicyEventReceiver(aMessage);
	}

CMMFAudioPolicyEventReceiver::~CMMFAudioPolicyEventReceiver()
	{
	if (iNeedToCompleteMessage)
		iMessage.Complete(KErrDied);
	}

void CMMFAudioPolicyEventReceiver::SendEvent(const TMMFAudioPolicyEvent& aEvent)
	{
	TMMFAudioPolicyEventPckg eventpckg(aEvent);
	TInt err = MmfMessageUtil::Write(iMessage, 0, eventpckg);
	iMessage.Complete(err);
	iNeedToCompleteMessage = EFalse;			
	}
	
void CMMFAudioPolicyEventReceiver::SendEvent(const TMMFEvent& aEvent)
	{
	TMMFEventPckg eventpckg(aEvent);
	TInt err = MmfMessageUtil::Write(iMessage, 0, eventpckg);
	iMessage.Complete(err);
	iNeedToCompleteMessage = EFalse;
	}

CMMFAudioPolicyEventReceiver::CMMFAudioPolicyEventReceiver(const RMmfIpcMessage& aMessage) : 
	iMessage(aMessage), iNeedToCompleteMessage(ETrue)
	{
	}
