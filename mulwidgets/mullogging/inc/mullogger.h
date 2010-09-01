/*
* Copyright (c) 2006-2007 Nokia Corporation and/or its subsidiary(-ies). 
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  Logging utility for MUL
*
*/


#ifndef T_MULLOGGER_H
#define T_MULLOGGER_H

// CLASS HEADER
#include "mullogger.h"

// EXTERNAL INCLUDES
#include <e32std.h>

namespace MulLogger
    {
    /**
     * Logging utility, writes the given log to a shared chunk if the chunk
     * exists or to RDebug if not.
     * Note! due to performance reasons only 8 bit descriptor allowed
     * @param The descriptor containing the format string. 
     * @param The arguments to append to the format string (ellipsis notation)
     */
    IMPORT_C void WriteFormat( TRefByValue<const TDesC8> aFmt, ... );
    }

#endif // T_MULLOGGER_H
