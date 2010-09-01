// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef _R_LOGGER_H
/** 	@file
	@internalTechnology */

#include "f32file.h"

#define _R_LOGGER_H

#if ( defined(_DEBUG) && !defined(DISABLE_LOGGING) )

#define LOG1(a) { RLogger rl; \
		if (KErrNone==rl.Open()) \
			rl.Log(__FILE__, __LINE__, _L8(a));\
		rl.Close();}

#define LOG2(a,b) { RLogger rl; \
		if (KErrNone==rl.Open()) \
			rl.Log(__FILE__, __LINE__, _L8(a),b);\
		rl.Close();}

#define LOG3(a,b,c) { RLogger rl; \
		if (KErrNone==rl.Open()) \
			rl.Log(__FILE__, __LINE__, _L8(a),b,c);\
		rl.Close();}
		
#define LOG4(a,b,c,d) { RLogger rl; \
		if (KErrNone==rl.Open()) \
			rl.Log(__FILE__, __LINE__, _L8(a),b,c,d);\
		rl.Close();}
		
#define LOG5(a,b,c,d,e) { RLogger rl; \
		if (KErrNone==rl.Open()) \
			rl.Log(__FILE__, __LINE__, _L8(a),b,c,d,e);\
		rl.Close();}				
		
class RLogger 
	{
public:
	TInt Open()
		{
		TInt err;
		if (KErrNone!=(err=iFs.Connect()))
			return err;
		_LIT(KFileName, "c:\\temp\\mylog.txt");
		if (KErrNone==(err=iFile.Open(iFs, KFileName,EFileWrite)))
			{
			TInt pos=0;
			if (KErrNone!=(err=iFile.Seek(ESeekEnd, pos)))
				iFile.Close();
			}
		else
			{
			iFs.MkDirAll(KFileName);
			err=iFile.Create(iFs,KFileName, EFileWrite);
			}

		return err;
		}

	void Close()
		{
		iFile.Close();
		iFs.Close();
		}

	void Log(const char* aFile, int aLineNo, TRefByValue<const TDesC8> aFmt,...)
		{
		TBuf8<256> Line;
		TPtrC8 FileName( reinterpret_cast<const TUint8*>(aFile));
		TPtrC8 NamePart( FileName.Length()>32? FileName.Right(32):FileName);
		TTime Now;
		Now.HomeTime();
		TDateTime FmtTime(Now.DateTime());
		Line.Format(_L8("...%S:%-5d %02d:%02d:%02d.%06d "), &NamePart, aLineNo, 
							FmtTime.Hour(), FmtTime.Minute(), FmtTime.Second(), FmtTime.MicroSecond() );
		iFile.Write(Line);
		Line.Zero();
		VA_LIST args;
		VA_START(args,aFmt);
		Line.AppendFormatList(aFmt, args);
		Line.Append('\n');
		iFile.Write(Line);
		}
protected:
	RFs		iFs;
	RFile	iFile;
	};

#else

#define LOG1(a) {(void)a;}
	
#define LOG2(a,b) {(void)a; (void)b;}

#define LOG3(a,b,c) {(void)a; (void)b; (void)c;}

#define LOG4(a,b,c,d) {(void)a; (void)b; (void)c; (void)d;}

#define LOG5(a,b,c,d,e) {(void)a; (void)b; (void)c; (void)d; (void)e;}

#endif

#endif // _R_LOGGER_H
