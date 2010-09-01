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
rem Description: Coverflow sisx bat file
rem

@echo off
echo DoTo: update the location for certificate and private key files

set certificate_file=..\internal\certificates\rd.cer
set private_key_file=..\internal\certificates\rd-key.pem

makesis coverflow_test.pkg
signsis coverflow_test.sis coverflow_test.sisx %certificate_file% %private_key_file%

makesis grid_test.pkg
signsis grid_test.sis grid_test.sisx %certificate_file% %private_key_file%



