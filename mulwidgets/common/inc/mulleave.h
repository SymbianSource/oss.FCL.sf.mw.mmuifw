/*
* Copyright (c) 2007-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Implementation for Leave to Throw macro
*
*/


#ifndef MULLEAVE_H
#define MULLEAVE_H

#include <stdexcept>
	
namespace Alf
	{

static const char* const KLeaveError = "Leave occured";
	
class MulLeave
    {

public:
    
    static void ThrowOnError( int aError )
        {
        switch ( aError )
            {
            case KErrNone: 
            	{
                // do nothing
                break;
            	}
                
            case KErrNoMemory:
            	{
                throw std::bad_alloc(); 
            	}
                
            // add other errors here if specific exception need to throw
            case KErrGeneral: // fall through
            default:
            	{
                throw std::logic_error( KLeaveError ); 
            	}
            }
        }
    };
	
	} // namespace Alf

#define THROW_IF_LEAVES(code) TRAPD( _leave_error, { code; } ); \
    						  Alf::MulLeave::ThrowOnError( _leave_error ); 

#endif  // MULLEAVE_H

// End of file

