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
*/

#ifndef DUMMYPCMRENDERER_H
#define DUMMYPCMRENDERER_H

#include <e32msgqueue.h>
#include <mmf/server/mmfhwdevicesetup.h>
#include <mdf/mdfcommon.h>

#include "milcomponentportif.h"
#include "milifobserver.h"
#include "ilcomponentif.h"

#ifdef SYMBIAN_MDF_SHAREDCHUNK_SOUNDDRIVER
    #include "mdasoundadapter.h"
#else
    #include <mdasound.h>
#endif

class CMMFBuffer;

/* 
PCM sink renderer component (this is just a simple non-omx component for testing)
This component is used together with the omxilvorbdechwdevice and the OMXVorbisDecoder 
for testing when requesting a HwDevice supporting VRB2 fourCC.
*/
class CDummyPcmRenderer : public CILComponentIf
    {
public:    
    /* 
    PCM sink input port
    */
    class CInputPort: public CActive, 
                      public MILComponentPortIf, 
                      public MPlayCustomInterface
        {
    friend class CDummyPcmRenderer;

    public:    
        static CInputPort* NewL(CDummyPcmRenderer& aParent);
        
        // MILComponentPortIf
        TInt FillThisBuffer(CMMFBuffer& aBuffer);
        TInt EmptyThisBuffer(const CMMFBuffer& aBuffer);
        TInt TunnelRequest(MILComponentPortIf* aPort);
        TInt DisconnectTunnel(MILComponentPortIf* aPort);
        TInt PortIndex() const;
        TPortDirection PortDirection() const;
        CMMFBuffer* CreateBufferL(TInt aBufferSize);
        TInt UseBuffer(CMMFBuffer& aBuffer);
        TInt FreeBuffer(CMMFBuffer* aBuffer);
        TInt FlushPort();
        TInt EnablePort();
        TInt DisablePort();
        MILComponentIf* PortComponent() const; 
    
        // from MPlayCustomInterface
        void SetVolume(TUint aVolume);
        TUint Volume();
        TUint BytesPlayed();
        void SetVolumeRamp(const TTimeIntervalMicroSeconds& aRampDuration);
        TTimeIntervalMicroSeconds& VolumeRamp();
        TInt SampleRate();
        TInt Channels();
                
        // from CActive
        void RunL();
        void DoCancel();
        
        // Internal
        TUint32 BufferSize() const;
        void Execute();
        void Pause();
        void Stop();
        ~CInputPort();
        
    private:
        explicit CInputPort(CDummyPcmRenderer& aParent);

    private:
        CDummyPcmRenderer&        iParent;
        MILComponentPortIf*       iPortConnectedTo;
        CMMFBuffer*               iCurrentBuffer;
        RPointerArray<CMMFBuffer> iBuffers;
        TInt                      iSampleRate;
        TInt                      iChannels;
        TBool                     iInterleaved;    
        TUint                     iVolume;
        TTimeIntervalMicroSeconds iRampDuration;
        TUint                     iBytesPlayed;
        TBool                     iPaused;
        TBool                     iStopped;
        TUint                     iBufferSize;
        };
public:
    static CDummyPcmRenderer* NewL();

    // from MILComponentIf
    void CreateComponentL(const TDesC8& aComponentName, MILIfObserver& aComponentIfObserver);
    void CreateComponentL(const TUid& aUid, MILIfObserver& aComponentIfObserver);
    void ReleaseComponent();
    TInt GetComponentInputPorts(RPointerArray<MILComponentPortIf>& aComponentPorts) const;
    TInt GetComponentOutputPorts(RPointerArray<MILComponentPortIf>& aComponentPorts) const;
    TInt SetConfig(const TILStruct& aConfig, const MILComponentPortIf* aComponentPort);
    TInt GetConfig(TILStruct& aConfig, const MILComponentPortIf* aComponentPort) const;    
    TInt Initialize();
    TInt Execute(); 
    TInt Pause();
    TInt Stop();
    TInt UnInitialize();
    TInt GetState(TILComponentState& aState) const;
    TInt SendCommand(const TILCommand& aCommand);
    TInt FlushAllPorts();
    TInt ComponentRoleEnum(TPtr8& aComponentRole, TUint32 aIndex) const;
    TInt GetComponentVersion(TILComponentVersion& aVersion) const;
    TInt SetComponentIfRole(const TUid& aFormat);
    TInt SetComponentIfRole(const TDesC8& aComponentRole);
    TInt GetComponentIfRole(TUid& aFormat) const;
    TInt GetComponentIfRole(TDes8& aComponentRole) const;
    TInt SetExtConfig(const TDesC8& aParameterName, const TILStruct& aConfig, const MILComponentPortIf* aComponentPort);
    TInt GetExtConfig(const TDesC8& aParameterName, TILStruct& aConfig, const MILComponentPortIf* aComponentPort) const;
    TAny* CustomInterface(TUid aUid);    
    

    ~CDummyPcmRenderer();
    
private:
    CDummyPcmRenderer();    
 
    RMdaDevSound& SoundDevice();
    MILIfObserver* Observer() const;
    
private:
    RMdaDevSound        iSoundDevice;
    TILComponentState   iState;

    CInputPort*         iInputPort;		
	MILIfObserver*      iObserver;	// Not owned
    };
    
#endif // DUMMYPCMRENDERER_H
