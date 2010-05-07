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

#ifndef OMXILFILESOURCEPROCESSINGFUNCTION_H
#define OMXILFILESOURCEPROCESSINGFUNCTION_H

#include "omxilprocessingfunction.h"
#include <e32base.h>
#include <e32msgqueue.h>
#include <f32file.h>


class COmxILFileSourceProcessingFunction : public COmxILProcessingFunction
	{
public:
	static COmxILFileSourceProcessingFunction* NewL(MOmxILCallbackNotificationIf& aCallbacks);
	~COmxILFileSourceProcessingFunction();

	OMX_ERRORTYPE StateTransitionIndication(COmxILFsm::TStateIndex aNewState);
	OMX_ERRORTYPE BufferFlushingIndication(TUint32 aPortIndex, OMX_DIRTYPE aDirection);
	OMX_ERRORTYPE ParamIndication(OMX_INDEXTYPE aParamIndex, const TAny* apComponentParameterStructure);
	OMX_ERRORTYPE ConfigIndication(OMX_INDEXTYPE aConfigIndex, const TAny* apComponentConfigStructure);
	OMX_ERRORTYPE BufferIndication(OMX_BUFFERHEADERTYPE* apBufferHeader, OMX_DIRTYPE aDirection);
	OMX_BOOL BufferRemovalIndication(OMX_BUFFERHEADERTYPE* apBufferHeader, OMX_DIRTYPE aDirection);
	
	MOmxILCallbackNotificationIf& GetCallbacks();

	/** can return NULL if parameter has not been set. */
	const HBufC* FileName() const;
	const HBufC8* Uri() const;

private:
	COmxILFileSourceProcessingFunction(MOmxILCallbackNotificationIf& aCallbacks);
	void ConstructL();
	
	void InitFileAndUriL();
	OMX_ERRORTYPE SetFileName(const OMX_PARAM_CONTENTURITYPE* aContentUriType);
			
private:
	RPointerArray<OMX_BUFFERHEADERTYPE> iBuffersToFill;
	OMX_STATETYPE iState;
	HBufC8* iUri;
	HBufC* iFileName;

private:
	class CPFHelper;
	class CFileSourceAccess : public CActive
		{
	friend class CPFHelper;
	public:
		static CFileSourceAccess* NewL(COmxILFileSourceProcessingFunction& aParent);
		~CFileSourceAccess();

		// from CActive
		void RunL();
		void DoCancel();

		TInt Execute();
		void Pause();
		void Stop();
		void Idle();
		
		TInt ProcessNextBuffer();

	private:
		CFileSourceAccess(COmxILFileSourceProcessingFunction& aParent);
		void ConstructL();
		
	private:
		COmxILFileSourceProcessingFunction& iParent;
		OMX_BUFFERHEADERTYPE* iCurrentBuffer;
		
		RFs iFs;
		RFile iFileHandle;
		TInt iBufferOffset;
		TPtr8 iReadBuffer;
		
		} *iFileSourceAccess;

	class CPFHelper : public CActive
		{
	public:
		static CPFHelper* NewL(COmxILFileSourceProcessingFunction& aParent, CFileSourceAccess& aFileSourceAccess);
		~CPFHelper();
		
		static OMX_ERRORTYPE ConvertSymbianErrorType(TInt aError);

		// from CActive
		void RunL();
		void DoCancel();

		TInt ExecuteAsync();
		TInt StopAsync();
		TInt PauseAsync();
		void StopSync();
		TInt IdleAsync();
		TInt BufferIndication(OMX_BUFFERHEADERTYPE* apBufferHeader);

		enum TMessageType 
			{
			EExecuteCommand,
			EStopCommand,
			EPauseCommand,
			ECancelCommand,
			EBufferIndication,
			EIdleCommand
			};

		class TProcMessage
			{
		public:
			TMessageType iType;
			TAny* iPtr;
			};
			
		RMsgQueue<TProcMessage> iMsgQueue;
	
	private:
		CPFHelper(COmxILFileSourceProcessingFunction& aParent, CFileSourceAccess& aFileSourceAccess);
		void ConstructL();
		
		TInt ProcessQueue();

	private:
		COmxILFileSourceProcessingFunction& iParent;
		CFileSourceAccess& iFileSourceAccess;
		} *iPFHelper;			
	};

#endif // OMXILFILESOURCEPROCESSINGFUNCTION_H

