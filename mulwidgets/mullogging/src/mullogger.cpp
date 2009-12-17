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
* Description:  Logging utility for MC Photos
*
*/


// CLASS HEADER
#include "mullogger.h"

// EXTERNAL INCLUDES
#include <e32debug.h>
#include <flogger.h>

// INTERNAL INCLUDES
//#include "mullogchunk.h"

namespace MulLogger
    {
    /// constants
    const TInt KMaxLoggingBufferLength = 256;


/**
 * TDes overflow handler
 * Default overflow handler panics and we dont want to do 
 * that in logging so need to implement our own handler.
 */ 
class TDes8OverFlowHandler : public TDes8Overflow
    {
    public: // from TDes8Overflow
        /// @ref TDes8Overflow::Overflow
        void Overflow( TDes8& /*aDes*/ )
            {
            // do nothing, we cant allocate so just eat the error
            }
    };

// -----------------------------------------------------------------------------
// MulLogger::WriteFormat
// -----------------------------------------------------------------------------
EXPORT_C void WriteFormat( TRefByValue< const TDesC8 > aFmt, ... )
    {
    /// writeable buffer, in emulator this is in stack
    TBuf8< KMaxLoggingBufferLength > gWriteBuffer;
    // take the ellipsis parameter
    VA_LIST args;
    VA_START( args, aFmt );
    // create overflow handler
    TDes8OverFlowHandler overflowhandler;
    // clear old data by setting length to zero
    gWriteBuffer.SetLength( 0 );
    // append the parameters to the descriptor
    gWriteBuffer.AppendFormatList( aFmt, args, &overflowhandler );
    // end the ellipsis handling
    VA_END( args );
    
    TBuf16<256> buffer;
    buffer.Copy(gWriteBuffer);
    RDebug::Print(_L("MUL:-%S"),&buffer);
    
    } // namespace
    
} // namespace
