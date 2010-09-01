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
rem Description: Command file for gesturehelper test build for winscw platform
rem

call bldmake bldfiles
call abld test build winscw udeb
call move /y \epoc32\release\winscw\udeb\t_gesture*.dll \epoc32\release\winscw\udeb\z\sys\bin\
