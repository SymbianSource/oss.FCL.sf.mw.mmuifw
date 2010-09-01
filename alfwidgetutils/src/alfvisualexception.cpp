/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  The implementation for presentation elements.
*
*/



#include <alf/alfvisualexception.h>
#include "alfexceptionstrings.h"

using namespace osncore;



// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
OSN_EXPORT AlfVisualException::AlfVisualException( int aErrorCode ) throw() :
        AlfException( aErrorCode )
    {
    }

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
OSN_EXPORT AlfVisualException::AlfVisualException(
    int aErrorCode, const char* aInfo, const char* aFileAndLine ) throw() :
        AlfException( aErrorCode, aInfo, aFileAndLine )
    {
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
OSN_EXPORT AlfVisualException::~AlfVisualException() throw()
    {
// nothing to do!
    }

// ---------------------------------------------------------------------------
// Returns the error information in c-string style.
// ---------------------------------------------------------------------------
//
OSN_EXPORT const char* AlfVisualException::what() const throw()
    {
    switch ( errorCode() )
        {
        case EInvalidVisual:
            return KErrorInvalidVisual;
        case EInvalidVisualValue:
            return KErrorInvalidVisualValue;
        default:
            return KErrorUndefinedException;
        }
    }

// End of File
