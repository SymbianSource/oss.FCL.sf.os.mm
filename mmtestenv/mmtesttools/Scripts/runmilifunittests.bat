rem Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
rem All rights reserved.
rem This component and the accompanying materials are made available
rem under the terms of "Eclipse Public License v1.0"
rem which accompanies this distribution, and is available
rem at the URL "http://www.eclipse.org/legal/epl-v10.html".
rem
rem Initial Contributors:
rem Nokia Corporation - initial contribution.
rem
rem Contributors:
rem
rem Description:
rem

del E:\Logs\TestResults\omxil\tsu_milif_omxilif.*
md E:\Logs
md E:\Logs\TestResults
md E:\Logs\TestResults\omxil

testframework z:\omxil\tsu_milif_omxilif.script

move C:\Logs\TestResults\omxil\*.htm E:\Logs\TestResults\omxil
del C:\*.*
