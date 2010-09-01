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
* Description:  element exception class.
*
*/



#include <alf/alfelementexception.h>
#include "alfexceptionstrings.h"

using namespace osncore;



// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
OSN_EXPORT AlfElementException::AlfElementException( int aErrorCode ) throw() :
        AlfException( aErrorCode )
    {
    }

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
OSN_EXPORT AlfElementException::AlfElementException(
    int aErrorCode, const char* aInfo, const char* aFileAndLine ) throw() :
        AlfException( aErrorCode, aInfo, aFileAndLine )
    {
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
OSN_EXPORT AlfElementException::~AlfElementException() throw()
    {
// nothing to do!
    }

// ---------------------------------------------------------------------------
// Returns the error information in c-string style.
// ---------------------------------------------------------------------------
//
OSN_EXPORT const char* AlfElementException::what() const throw()
    {
    switch ( errorCode() )
        {
        case EInvalidElement:
            return KErrorInvalidElement;
        default:
            return KErrorUndefinedException;
        }
    }

// End of File
