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
rem Description: Command file for freezing def files
rem

call clean
call wins
call arm
call attrib -r ..\bwins\*.def
call attrib -r ..\eabi\*.def
call abld freeze winscw
call abld freeze armv5
call clean
call all
