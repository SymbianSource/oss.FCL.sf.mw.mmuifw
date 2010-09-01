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
* Description:  Container widget exception class.
*
*/


#ifndef ALFCONTAINERWIDGETEXCEPTION_H
#define ALFCONTAINERWIDGETEXCEPTION_H

// INCLUDES
#include <alf/alfexception.h>
#include <osn/ustring.h>

namespace Alf
    {

/**
 * Error codes used with AlfContainerWidgetException.
 *
 * There is an offset in AlfContainerWidgetException error codes, so that
 * AlfContainerWidget specific exceptions can be separated from
 * osncore::AlfException base exceptions and exceptions of other widgets.
 */
enum TAlfContainerWidgetErrorCode
    {
    /**
     * Error occured in construction or initialization of the container
     * widget.
     */
    EInitializationError = 500
    };

/**
 * AlfContainerWidgetException is a base class for all container widget
 * exceptions.
 *
 */
class AlfContainerWidgetException : public osncore::AlfException
    {
public:
    // Constructors and destructor

    /**
     * Constructor with AlfContainerWidget specific error code.
     *
     * Error code has to be one of the error codes specified in
     * Alf::TAlfContainerWidgetErrorCode or a system-wide error code
     * represented by negative integer.
     *
     * @see osncore::AlfException::errorCode()
     *
     * @param aError Error code to describe the occured exception.
     */
    AlfContainerWidgetException(int aError) throw();

    /**
     * Constructor with AlfContainerWidget specific error code
     * and additional information.
     *
     * Error code has to be one of the error codes specified in
     * Alf::TAlfContainerWidgetErrorCode or a system-wide error code
     * represented by negative integer.
     *
     * @see osncore::AlfException::errorCode()
     *
     * @param aError Error code to describe the occured exception.
     * @param aInfo Optional additional information or NULL.
        */
    AlfContainerWidgetException(int aError, const char* aInfo) throw();

    /**
     * Constructor with AlfContainerWidget specific error code,
     * additional information, source file and line of code.
     *
     * Error code has to be one of the error codes specified in
     * Alf::TAlfContainerWidgetErrorCode or a system-wide error code
     * represented by negative integer.
     *
     * @see osncore::AlfException::errorCode()
     *
     * @param aError Error code to describe the occured exception.
     * @param aInfo Optional additional information or NULL.
        * @param aFileAndLine Optional file and line information or NULL.
        */
    AlfContainerWidgetException(int aError, const char* aInfo, const char* aFileAndLine) throw();

    /**
     * Default destructor.
     */
    virtual ~AlfContainerWidgetException() throw();

    /**
     * Returns the description of the exception.
     * @ret The description of the exception.
     */
    const char* what() const throw();

private:

    /**
     * Error description
     */
    osncore::UString mDescription;

    };

    } // Namespace Alf

#endif // ALFCONTAINERWIDGETEXCEPTION_H
