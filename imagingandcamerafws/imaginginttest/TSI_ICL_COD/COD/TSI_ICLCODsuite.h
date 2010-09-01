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
// This contains CTsiIclCodSuite 
// 
//

#ifndef __TSI_ICLCODSUITE_H_
#define __TSI_ICLCODSUITE_H_

/**
 *
 * @class CTsiIclCodSuite
 *
 */
class  CTsiIclCodSuite : public CTestSuite
	{
public:
	void InitialiseL();
	virtual ~CTsiIclCodSuite();
	TPtrC GetVersion();
	};

#endif // __TSI_ICLCODSUITE_H_

