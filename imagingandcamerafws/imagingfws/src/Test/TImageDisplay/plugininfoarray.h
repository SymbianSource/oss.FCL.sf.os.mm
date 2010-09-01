// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef PLUGININFOARRAY_H

#define PLUGININFOARRAY_H

#include <imageconversion.h>

class CPluginInfoArray : public CBase, public MDesCArray
	{
public:
	static CPluginInfoArray* NewL();
	virtual ~CPluginInfoArray();

	void Reset();

	void RefreshPluginListL();
	TUid ImageType(TInt aIndex);
	TUid SubType(TInt aIndex);

	//From MDesCArray
    TInt MdcaCount() const;
    TPtrC MdcaPoint(TInt aIndex) const;

private:
	CPluginInfoArray();
	void ConstructL();

public:
	RImageTypeDescriptionArray iPluginArray;
	};

#endif // ndef PLUGININFOARRAY_H

