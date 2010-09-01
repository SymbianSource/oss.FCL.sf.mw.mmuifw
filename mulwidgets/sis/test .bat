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
rem Description: bat file to create all Widget's test app.
rem


call makesis coverflow_test.pkg
call signsis -s coverflow_test.sis coverflow_test.sisx RDTest_02.der RDTest_02.key

call makesis gesturetest.pkg
call signsis -s gesturetest.sis gesturetest.sisx RDTest_02.der RDTest_02.key

call makesis grid_test.pkg
call signsis -s grid_test.sis grid_test.sisx RDTest_02.der RDTest_02.key

call makesis mullistapp_test.pkg
call signsis -s mullistapp_test.sis mullistapp_test.sisx RDTest_02.der RDTest_02.key

call makesis ListTest.pkg
call signsis -s ListTest.sis ListTest.sisx RDTest_02.der RDTest_02.key
call makesis slider_test.pkg
call signsis -s slider_test.sis slider_test.sisx RDTest_02.der RDTest_02.key

