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
* Description:  View widget exception class.
*
*/


#ifndef ALFVIEWWIDGETEXCEPTION_H
#define ALFVIEWWIDGETEXCEPTION_H

// INCLUDES
#include <alf/alfexception.h>
#include <osn/ustring.h>

namespace Alf
    {

/**
 * Error codes used with AlfViewWidgetException.
 *
 * There is an offset in AlfViewWidgetException error codes, so that
 * AlfViewWidget specific exceptions can be separated from
 * osncore::AlfException base exceptions and exceptions of other widgets.
 */
enum TAlfViewWidgetErrorCode
    {
    /**
     * Error occured in construction or initialization of the view
     * widget.
     */
    EInitializationError = 600,

    /**
     * Handle to status pane not found
     */
    EStatusPaneError,

    /**
     * Handle to control pane not found
     */
    EControlPaneError,
    
    /**
     * Control group id specified in the viewwidget constructor
     * already reserved.
     */
    EControlGroupAlreadyExists
    };

/**
 * AlfViewWidgetException is a base class for all view widget
 * exceptions.
 *
 */
class AlfViewWidgetException : public osncore::AlfException
    {
public:
    // Constructors and destructor

    /**
     * Constructor with AlfViewWidget specific error code.
     *
     * Error code has to be one of the error codes specified in
     * Alf::TAlfViewWidgetErrorCode or a system-wide error code
     * represented by negative integer.
     *
     * @see osncore::AlfException::errorCode()
     *
     * @param aError Error code to describe the occured exception.
     */
    AlfViewWidgetException(int aError) throw();

    /**
     * Constructor with AlfViewWidget specific error code
     * and additional information.
     *
     * Error code has to be one of the error codes specified in
     * Alf::TAlfViewWidgetErrorCode or a system-wide error code
     * represented by negative integer.
     *
     * @see osncore::AlfException::errorCode()
     *
     * @param aError Error code to describe the occured exception.
     * @param aInfo Optional additional information or NULL.
        */
    AlfViewWidgetException(int aError, const char* aInfo) throw();

    /**
     * Constructor with AlfViewWidget specific error code,
     * additional information, source file and line of code.
     *
     * Error code has to be one of the error codes specified in
     * Alf::TAlfViewWidgetErrorCode or a system-wide error code
     * represented by negative integer.
     *
     * @see osncore::AlfException::errorCode()
     *
     * @param aError Error code to describe the occured exception.
     * @param aInfo Optional additional information or NULL.
        * @param aFileAndLine Optional file and line information or NULL.
        */
    AlfViewWidgetException(int aError, const char* aInfo, const char* aFileAndLine) throw();

    /**
     * Default destructor.
     */
    virtual ~AlfViewWidgetException() throw();

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

#endif // ALFVIEWWIDGETEXCEPTION_H
