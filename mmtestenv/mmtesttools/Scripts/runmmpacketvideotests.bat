rem Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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

del e:\logs\testresults\tsi_a3f_devsound_aac*.*
del e:\logs\testresults\omxil\tsu_milif_omxilif*.*
del e:\logs\testresults\omxil\tsu_omxil_aacdecoder_01_debug*.*

md e:\logs\
md e:\logs\testresults\
md e:\logs\testresults\omxil\

md c:\tsi_a3f_devsound_aac\

rem ******************A3F Devsound PacketVideo Integration Tests ****************


copy z:\tsi_a3f_devsound_aac_play.ini c:\tsi_a3f_devsound_aac\tsi_a3f_devsound_aac_play.ini

rem Copy test data
copy e:\inttest\a3f\tsi_a3f_devsound_aac\*.* c:\tsi_a3f_devsound_aac\

testframework tsi_a3f_devsound_aac_play_debug.script
move c:\logs\testresults\*.htm e:\logs\testresults\

testframework tsi_a3f_devsound_aac_play_alloc.script
move c:\logs\testresults\*.htm e:\logs\testresults\

del c:\tsi_a3f_devsound_aac\*.*


rem *****************************************************************************

rem *********** Multimedia Integration Layer Interface PacketVideo Unit Tests ***********


testframework z:\omxil\tsu_milif_omxilif_aac.script
move c:\logs\testresults\omxil\*.htm e:\logs\testresults\omxil\

rem *****************************************************************************

rem ************************ OMX IL AAC Decoder Unit Tests ************************

md c:\omxil\
md c:\omxil\testfiles\

copy e:\unittest\omxil\testfiles\*.* c:\omxil\testfiles\

testframework \omxil\tsu_omxil_aacdecoder_01_debug.script
move c:\logs\testresults\omxil\tsu_omxil_aacdecoder_01_debug.htm e:\logs\testresults\omxil\

del c:\omxil\testfiles\*.*
del c:\omxil\*.*

rem *****************************************************************************

del c:\*.*