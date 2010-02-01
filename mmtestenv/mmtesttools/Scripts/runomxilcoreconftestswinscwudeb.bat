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
rem this bat file is used to run all core conformance tests on emulator and in UDEB mode only
rem because for vendor components, the binaries are not available in UREL mode and tests will fail
rem this bat file is exported to \epoc32\winscw\c\omxil and can be run from this location
rem or can also be run from \TestTools\Scripts
rem this bat file will not be in the ROM; for testing on hw, use runomxilcoreconftests.bat
rem 
rem

cd \epoc32\release\winscw\udeb

coreconf.exe \omxil\test\allconftests_coreconftestinput.txt
coreconf.exe \omxil\test\allconftests2_coreconftestinput.txt
coreconf.exe \omxil\test\allconftests3_coreconftestinput.txt
coreconf.exe \omxil\test\compload_coreconftestinput.txt
coreconf.exe \omxil\test\compnameenum_coreconftestinput.txt
coreconf.exe \omxil\test\compsofrole_coreconftestinput.txt
coreconf.exe \omxil\test\coreinitdeinit_coreconftestinput.txt
coreconf.exe \omxil\test\getcontentpipe_coreconftestinput.txt
coreconf.exe \omxil\test\rolesofcomp_coreconftestinput.txt
coreconf.exe \omxil\test\setuptunnel_coreconftestinput.txt

coreconf.exe \omxil\aacdecoder\allconftests_coreconfvendorinput.txt
coreconf.exe \omxil\aacdecoder\allconftests2_coreconfvendorinput.txt
coreconf.exe \omxil\aacdecoder\allconftests3_coreconfvendorinput.txt
coreconf.exe \omxil\aacdecoder\compload_coreconfvendorinput.txt
coreconf.exe \omxil\aacdecoder\compnameenum_coreconfvendorinput.txt
coreconf.exe \omxil\aacdecoder\compsofrole_coreconfvendorinput.txt
coreconf.exe \omxil\aacdecoder\coreinitdeinit_coreconfvendorinput.txt
coreconf.exe \omxil\aacdecoder\getcontentpipe_coreconfvendorinput.txt
coreconf.exe \omxil\aacdecoder\rolesofcomp_coreconfvendorinput.txt
coreconf.exe \omxil\aacdecoder\setuptunnel_coreconfvendorinput.txt

coreconf.exe \omxil\common\allconftests_coreconfvendorinput.txt
coreconf.exe \omxil\common\allconftests2_coreconfvendorinput.txt
coreconf.exe \omxil\common\allconftests3_coreconfvendorinput.txt
coreconf.exe \omxil\common\allconftests4_coreconfvendorinput.txt
coreconf.exe \omxil\common\allconftests5_coreconfvendorinput.txt
coreconf.exe \omxil\common\allconftests6_coreconfvendorinput.txt
coreconf.exe \omxil\common\compload_coreconfvendorinput.txt
coreconf.exe \omxil\common\compnameenum_coreconfvendorinput.txt
coreconf.exe \omxil\common\compsofrole_coreconfvendorinput.txt
coreconf.exe \omxil\common\coreinitdeinit_coreconfvendorinput.txt
coreconf.exe \omxil\common\getcontentpipe_coreconfvendorinput.txt
coreconf.exe \omxil\common\rolesofcomp_coreconfvendorinput.txt
coreconf.exe \omxil\common\setuptunnel_coreconfvendorinput.txt

coreconf.exe \omxil\pcmrenderer\allconftests_coreconfvendorinput.txt
coreconf.exe \omxil\pcmrenderer\allconftests2_coreconfvendorinput.txt
coreconf.exe \omxil\pcmrenderer\allconftests3_coreconfvendorinput.txt
coreconf.exe \omxil\pcmrenderer\compload_coreconfvendorinput.txt
coreconf.exe \omxil\pcmrenderer\compnameenum_coreconfvendorinput.txt
coreconf.exe \omxil\pcmrenderer\compsofrole_coreconfvendorinput.txt
coreconf.exe \omxil\pcmrenderer\coreinitdeinit_coreconfvendorinput.txt
coreconf.exe \omxil\pcmrenderer\getcontentpipe_coreconfvendorinput.txt
coreconf.exe \omxil\pcmrenderer\rolesofcomp_coreconfvendorinput.txt
coreconf.exe \omxil\pcmrenderer\setuptunnel_coreconfvendorinput.txt
