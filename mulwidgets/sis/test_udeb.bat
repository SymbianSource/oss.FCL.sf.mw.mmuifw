rem
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
rem Description: bat file to create all Widget's test app with logging utility.
rem


call makesis coverflow_test_udeb.pkg
call signsis -s coverflow_test_udeb.sis coverflow_test_udeb.sisx RDTest_02.der RDTest_02.key

call makesis gesturetest.pkg
call signsis -s gesturetest.sis gesturetest.sisx RDTest_02.der RDTest_02.key

call makesis grid_test.pkg
call signsis -s grid_test.sis grid_test.sisx RDTest_02.der RDTest_02.key

call makesis mullistapp_test.pkg
call signsis -s mullistapp_test.sis mullistapp_test.sisx RDTest_02.der RDTest_02.key

call makesis slider_test.pkg
call signsis -s slider_test.sis slider_test.sisx RDTest_02.der RDTest_02.key

call makesis softkeytest.pkg
call signsis -s softkeytest.sis softkeytest.sisx RDTest_02.der RDTest_02.key

call makesis toolbar_test.pkg
call signsis -s toolbar_test.sis toolbar_test.sisx RDTest_02.der RDTest_02.key

call makesis metapane_test.pkg
call signsis -s metapane_test.sis metapane_test.sisx RDTest_02.der RDTest_02.key
