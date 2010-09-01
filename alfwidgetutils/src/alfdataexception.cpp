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



#include <alf/alfdataexception.h>
#include "alfexceptionstrings.h"

using namespace osncore;



// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
OSN_EXPORT AlfDataException::AlfDataException( int aErrorCode ) throw() :
        AlfException( aErrorCode )
    {
    }

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
OSN_EXPORT AlfDataException::AlfDataException(
    int aErrorCode, const char* aInfo, const char* aFileAndLine ) throw() :
        AlfException( aErrorCode, aInfo, aFileAndLine )
    {
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
OSN_EXPORT AlfDataException::~AlfDataException() throw()
    {
// nothing to do!
    }

// ---------------------------------------------------------------------------
// Returns the error information in c-string style.
// ---------------------------------------------------------------------------
//
OSN_EXPORT const char* AlfDataException::what() const throw()
    {
    switch ( errorCode() )
        {
        case EInvalidVariantDataType:
            return KErrorInvalidVariantDataType;

        case EInvalidArrayIndex:
            return KErrorInvalidArrayIndex;

        case EInvalidContainerOperation:
            return KErrorInvalidContainerOperation;

        case EInvalidMapOperation:
            return KErrorInvalidMapOperation;

        case EInvalidBranchOperation:
            return KErrorInvalidBranchOperation;

        case EInvalidModelOperation:
            return KErrorInvalidModelOperation;

        default:
            return KErrorUndefinedException;
        }
    }

// End of File
