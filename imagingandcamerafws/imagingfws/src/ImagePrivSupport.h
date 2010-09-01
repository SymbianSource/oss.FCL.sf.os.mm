// Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// Not for public use
// Provides information to the main ICL body classes about their environment, plus some env sensitive ops
// 
//

#ifndef __ImagePrivSupport_h
#define __ImagePrivSupport_h

#include <e32base.h>

// provide info to the decoder about its interface
class MImageDecoderPrivSupport
	{
public:
	/*
	 * Must use local buffer, even when decoding from descriptor
	 */
	virtual TBool MustUseBufferWithDescriptor() const=0;
	/*
	 * Copy from descriptor to indicated buffer
	 */
	virtual void CopyBufferToDescriptor(TDes8& aBuffer, const TDesC8& aSource, TInt aPosition, TInt aSize)=0;
	/**
	 *  Indicate if running in a separate thread
	 */
	virtual TBool AmInThread() const=0;
	/**
	 *  Indicate if a decode should abort early (ie. following a Cancel). Always false unless in a thread.
	 */
	virtual TBool ShouldAbort() const=0;
	/**
	 * Mutual exclusion lock - use on the frame tables to avoid update problems with threaded decode
	 * For non-threaded implementation will be a no-op.
	 */
	virtual void Lock()=0;
	/**
	 * Mutual exclusion unlock - opposit of Lock()
	 * For non-threaded implementation will be a no-op.
	 */
	virtual void Unlock()=0;
	/**
	 * Save the framecount - should be called whenever the framecount is successfully incremented
	 * Optionally used to avoid checking the framecount directly during threading. This call should
	 * only be made when we having finished setting up the frame's details.
	 * For non-threaded implementation will be a no-op.
	 */
	virtual void SaveFrameCount(TInt aFrameCount)=0;
	};

// provide info to the decoder about its interface
class MImageEncoderPrivSupport
	{
public:
	/**
	 *  Indicate if running in a separate thread
	 */
	virtual TBool AmInThread() const=0;
	/**
	 *  Indicate if a decode should abort early (ie. following a Cancel). Always false unless in a thread.
	 */
	virtual TBool ShouldAbort() const=0;
	};

#endif  // __ImagePrivSupport_h

