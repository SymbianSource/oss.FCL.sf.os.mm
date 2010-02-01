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
rem Core-Loader Conformance Tests for test and vendor components
rem 
rem


del E:\unittest\omxil\test\*_coreconftestoutput.txt
del E:\unittest\omxil\aacdecoder\*_coreconfvendoroutput.txt
del E:\unittest\omxil\common\*_coreconfvendoroutput.txt
del E:\unittest\omxil\pcmrenderer\*_coreconfvendoroutput.txt


REM ...where core-loader conformance input files will be exported/copied
md  c:\omxil\
md  c:\omxil\test\
md  c:\omxil\aacdecoder\
md  c:\omxil\common\
md  c:\omxil\pcmrenderer\


REM copy core-loader conformance input files to H4
copy E:\unittest\omxil\test\*.*         c:\omxil\test\
copy E:\unittest\omxil\aacdecoder\*.*   c:\omxil\aacdecoder\
copy E:\unittest\omxil\common\*.*       c:\omxil\common\
copy E:\unittest\omxil\pcmrenderer\*.*  c:\omxil\pcmrenderer\


REM run core conformance tests
coreconf.exe c:\omxil\test\coreinitdeinit_coreconftestinput.txt
move c:\omxil\test\coreinitdeinit_coreconftestoutput.txt E:\unittest\omxil\test\coreinitdeinit_coreconftestoutput.txt

coreconf.exe c:\omxil\test\getcontentpipe_coreconftestinput.txt
move c:\omxil\test\getcontentpipe_coreconftestoutput.txt E:\unittest\omxil\test\getcontentpipe_coreconftestoutput.txt

coreconf.exe c:\omxil\test\setuptunnel_coreconftestinput.txt
move c:\omxil\test\setuptunnel_coreconftestoutput.txt E:\unittest\omxil\test\setuptunnel_coreconftestoutput.txt

REM run core-loader conformance tests
coreconf.exe c:\omxil\test\compload_coreconftestinput.txt
move c:\omxil\test\compload_coreconftestoutput.txt E:\unittest\omxil\test\compload_coreconftestoutput.txt

coreconf.exe c:\omxil\test\compnameenum_coreconftestinput.txt
move c:\omxil\test\compnameenum_coreconftestoutput.txt E:\unittest\omxil\test\compnameenum_coreconftestoutput.txt

coreconf.exe c:\omxil\test\compsofrole_coreconftestinput.txt
move c:\omxil\test\compsofrole_coreconftestoutput.txt E:\unittest\omxil\test\compsofrole_coreconftestoutput.txt

coreconf.exe c:\omxil\test\rolesofcomp_coreconftestinput.txt
move c:\omxil\test\rolesofcomp_coreconftestoutput.txt E:\unittest\omxil\test\rolesofcomp_coreconftestoutput.txt

REM run all conformance tests
coreconf.exe c:\omxil\test\allconftests_coreconftestinput.txt
move c:\omxil\test\allconftests_coreconftestoutput.txt E:\unittest\omxil\test\allconftests_coreconftestoutput.txt

coreconf.exe c:\omxil\test\allconftests2_coreconftestinput.txt
move c:\omxil\test\allconftests2_coreconftestoutput.txt E:\unittest\omxil\test\allconftests2_coreconftestoutput.txt

coreconf.exe c:\omxil\test\allconftests3_coreconftestinput.txt
move c:\omxil\test\allconftests3_coreconftestoutput.txt E:\unittest\omxil\test\allconftests3_coreconftestoutput.txt


REM run core conformance tests with vendor components
coreconf.exe c:\omxil\aacdecoder\coreinitdeinit_coreconfvendorinput.txt
move c:\omxil\aacdecoder\coreinitdeinit_coreconfvendoroutput.txt E:\unittest\omxil\aacdecoder\coreinitdeinit_coreconfvendoroutput.txt

coreconf.exe c:\omxil\aacdecoder\getcontentpipe_coreconfvendorinput.txt
move c:\omxil\aacdecoder\getcontentpipe_coreconfvendoroutput.txt E:\unittest\omxil\aacdecoder\getcontentpipe_coreconfvendoroutput.txt

coreconf.exe c:\omxil\aacdecoder\setuptunnel_coreconfvendorinput.txt
move c:\omxil\aacdecoder\setuptunnel_coreconfvendoroutput.txt E:\unittest\omxil\aacdecoder\setuptunnel_coreconfvendoroutput.txt

coreconf.exe c:\omxil\aacdecoder\compload_coreconfvendorinput.txt
move c:\omxil\aacdecoder\compload_coreconfvendoroutput.txt E:\unittest\omxil\aacdecoder\compload_coreconfvendoroutput.txt

coreconf.exe c:\omxil\aacdecoder\compnameenum_coreconfvendorinput.txt
move c:\omxil\aacdecoder\compnameenum_coreconfvendoroutput.txt E:\unittest\omxil\aacdecoder\compnameenum_coreconfvendoroutput.txt

coreconf.exe c:\omxil\aacdecoder\compsofrole_coreconfvendorinput.txt
move c:\omxil\aacdecoder\compsofrole_coreconfvendoroutput.txt E:\unittest\omxil\aacdecoder\compsofrole_coreconfvendoroutput.txt

coreconf.exe c:\omxil\aacdecoder\rolesofcomp_coreconfvendorinput.txt
move c:\omxil\aacdecoder\rolesofcomp_coreconfvendoroutput.txt E:\unittest\omxil\aacdecoder\rolesofcomp_coreconfvendoroutput.txt

coreconf.exe c:\omxil\aacdecoder\allconftests_coreconfvendorinput.txt
move c:\omxil\aacdecoder\allconftests_coreconfvendoroutput.txt E:\unittest\omxil\aacdecoder\allconftests_coreconfvendoroutput.txt

coreconf.exe c:\omxil\aacdecoder\allconftests2_coreconfvendorinput.txt
move c:\omxil\aacdecoder\allconftests2_coreconfvendoroutput.txt E:\unittest\omxil\aacdecoder\allconftests2_coreconfvendoroutput.txt

coreconf.exe c:\omxil\aacdecoder\allconftests3_coreconfvendorinput.txt
move c:\omxil\aacdecoder\allconftests3_coreconfvendoroutput.txt E:\unittest\omxil\aacdecoder\allconftests3_coreconfvendoroutput.txt

coreconf.exe c:\omxil\common\coreinitdeinit_coreconfvendorinput.txt
move c:\omxil\common\coreinitdeinit_coreconfvendoroutput.txt E:\unittest\omxil\common\coreinitdeinit_coreconfvendoroutput.txt

coreconf.exe c:\omxil\common\getcontentpipe_coreconfvendorinput.txt
move c:\omxil\common\getcontentpipe_coreconfvendoroutput.txt E:\unittest\omxil\common\getcontentpipe_coreconfvendoroutput.txt

coreconf.exe c:\omxil\common\setuptunnel_coreconfvendorinput.txt
move c:\omxil\common\setuptunnel_coreconfvendoroutput.txt E:\unittest\omxil\common\setuptunnel_coreconfvendoroutput.txt

coreconf.exe c:\omxil\common\compload_coreconfvendorinput.txt
move c:\omxil\common\compload_coreconfvendoroutput.txt E:\unittest\omxil\common\compload_coreconfvendoroutput.txt

coreconf.exe c:\omxil\common\compnameenum_coreconfvendorinput.txt
move c:\omxil\common\compnameenum_coreconfvendoroutput.txt E:\unittest\omxil\common\compnameenum_coreconfvendoroutput.txt

coreconf.exe c:\omxil\common\compsofrole_coreconfvendorinput.txt
move c:\omxil\common\compsofrole_coreconfvendoroutput.txt E:\unittest\omxil\common\compsofrole_coreconfvendoroutput.txt

coreconf.exe c:\omxil\common\rolesofcomp_coreconfvendorinput.txt
move c:\omxil\common\rolesofcomp_coreconfvendoroutput.txt E:\unittest\omxil\common\rolesofcomp_coreconfvendoroutput.txt

coreconf.exe c:\omxil\common\allconftests_coreconfvendorinput.txt
move c:\omxil\common\allconftests_coreconfvendoroutput.txt E:\unittest\omxil\common\allconftests_coreconfvendoroutput.txt

coreconf.exe c:\omxil\common\allconftests2_coreconfvendorinput.txt
move c:\omxil\common\allconftests2_coreconfvendoroutput.txt E:\unittest\omxil\common\allconftests2_coreconfvendoroutput.txt

coreconf.exe c:\omxil\common\allconftests3_coreconfvendorinput.txt
move c:\omxil\common\allconftests3_coreconfvendoroutput.txt E:\unittest\omxil\common\allconftests3_coreconfvendoroutput.txt

coreconf.exe c:\omxil\common\allconftests4_coreconfvendorinput.txt
move c:\omxil\common\allconftests4_coreconfvendoroutput.txt E:\unittest\omxil\common\allconftests4_coreconfvendoroutput.txt

coreconf.exe c:\omxil\common\allconftests5_coreconfvendorinput.txt
move c:\omxil\common\allconftests5_coreconfvendoroutput.txt E:\unittest\omxil\common\allconftests5_coreconfvendoroutput.txt

coreconf.exe c:\omxil\common\allconftests6_coreconfvendorinput.txt
move c:\omxil\common\allconftests6_coreconfvendoroutput.txt E:\unittest\omxil\common\allconftests6_coreconfvendoroutput.txt

coreconf.exe c:\omxil\pcmrenderer\coreinitdeinit_coreconfvendorinput.txt
move c:\omxil\pcmrenderer\coreinitdeinit_coreconfvendoroutput.txt E:\unittest\omxil\pcmrenderer\coreinitdeinit_coreconfvendoroutput.txt

coreconf.exe c:\omxil\pcmrenderer\getcontentpipe_coreconfvendorinput.txt
move c:\omxil\pcmrenderer\getcontentpipe_coreconfvendoroutput.txt E:\unittest\omxil\pcmrenderer\getcontentpipe_coreconfvendoroutput.txt

coreconf.exe c:\omxil\pcmrenderer\setuptunnel_coreconfvendorinput.txt
move c:\omxil\pcmrenderer\setuptunnel_coreconfvendoroutput.txt E:\unittest\omxil\pcmrenderer\setuptunnel_coreconfvendoroutput.txt

coreconf.exe c:\omxil\pcmrenderer\compload_coreconfvendorinput.txt
move c:\omxil\pcmrenderer\compload_coreconfvendoroutput.txt E:\unittest\omxil\pcmrenderer\compload_coreconfvendoroutput.txt

coreconf.exe c:\omxil\pcmrenderer\compnameenum_coreconfvendorinput.txt
move c:\omxil\pcmrenderer\compnameenum_coreconfvendoroutput.txt E:\unittest\omxil\pcmrenderer\compnameenum_coreconfvendoroutput.txt

coreconf.exe c:\omxil\pcmrenderer\compsofrole_coreconfvendorinput.txt
move c:\omxil\pcmrenderer\compsofrole_coreconfvendoroutput.txt E:\unittest\omxil\pcmrenderer\compsofrole_coreconfvendoroutput.txt

coreconf.exe c:\omxil\pcmrenderer\rolesofcomp_coreconfvendorinput.txt
move c:\omxil\pcmrenderer\rolesofcomp_coreconfvendoroutput.txt E:\unittest\omxil\pcmrenderer\rolesofcomp_coreconfvendoroutput.txt

coreconf.exe c:\omxil\pcmrenderer\allconftests_coreconfvendorinput.txt
move c:\omxil\pcmrenderer\allconftests_coreconfvendoroutput.txt E:\unittest\omxil\pcmrenderer\allconftests_coreconfvendoroutput.txt

coreconf.exe c:\omxil\pcmrenderer\allconftests2_coreconfvendorinput.txt
move c:\omxil\pcmrenderer\allconftests2_coreconfvendoroutput.txt E:\unittest\omxil\pcmrenderer\allconftests2_coreconfvendoroutput.txt

coreconf.exe c:\omxil\pcmrenderer\allconftests3_coreconfvendorinput.txt
move c:\omxil\pcmrenderer\allconftests3_coreconfvendoroutput.txt E:\unittest\omxil\pcmrenderer\allconftests3_coreconfvendoroutput.txt


REM delete core-loader conformance input files from H4
del c:\omxil\test\*.*
del c:\omxil\aacdecoder\*.*
del c:\omxil\common\*.*
del c:\omxil\pcmrenderer\*.*
