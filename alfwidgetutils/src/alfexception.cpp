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
* Description:  The basic implementation for presentation elements.
*
*/



#include <alf/alfexception.h>

using namespace std;
using namespace osncore;



// ======== LOCAL FUNCTIONS ========

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
OSN_EXPORT AlfException::AlfException( const int aErrorCode ) throw()
    {
    mErrorCode = aErrorCode;
    mInfo = 0;
    mFileAndLine = 0;
    }

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
OSN_EXPORT AlfException::AlfException(
    int aErrorCode, const char* aInfo ) throw()
    {
    mErrorCode = aErrorCode;
    mInfo = aInfo;
    mFileAndLine = 0;
    }

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
OSN_EXPORT AlfException::AlfException(
    int aErrorCode, const char* aInfo, const char* aFileAndLine ) throw()
    {
    mErrorCode = aErrorCode;
    mInfo = aInfo;
    mFileAndLine = aFileAndLine;
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
OSN_EXPORT AlfException::~AlfException() throw()
    {
    // nothing to do!
    }

// ---------------------------------------------------------------------------
// Returns the error information in c-string style.
// ---------------------------------------------------------------------------
//
OSN_EXPORT const char* AlfException::what() const throw()
    {
    return "";
    }

// ---------------------------------------------------------------------------
// Returns the error information in integer type
// ---------------------------------------------------------------------------
//
OSN_EXPORT int AlfException::errorCode() const throw()
    {
    return mErrorCode;
    }

// ---------------------------------------------------------------------------
// Returns the error information in integer type
// ---------------------------------------------------------------------------
//
OSN_EXPORT const char* AlfException::info() const throw()
    {
    if ( mInfo )
        {
        return mInfo;
        }
    return "";
    }

// ---------------------------------------------------------------------------
// Returns the file and line information
// ---------------------------------------------------------------------------
//
OSN_EXPORT const char* AlfException::fileAndLine() const throw()
    {
    if ( mFileAndLine )
        {
        return mFileAndLine;
        }
    return "";
    }

// End of File
