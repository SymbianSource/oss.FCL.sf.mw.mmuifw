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
* Description:  bad alloc eception interface
*
*/




#ifndef ALFEXCEPTION_H_
#define ALFEXCEPTION_H_

#include <osn/osncommon.h>
#include <stdexcept>

using namespace std;

namespace osncore
    {

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

#ifdef ALF_DEBUG_EXCEPTIONS
/**
* Use this macro for throwing exceptions in the widgetmodel code.
*
* @param p1 Exception class
* @param p2 Error code
* @param p3 Debug information string
*/
#define ALF_THROW(p1,p2,p3) \
{p1 p4= p1(p2,p3, __FILE__ ":" TOSTRING(__LINE__)); throw (p4);}
#else
#define ALF_THROW(p1,p2,p3) {p1 p4= p1(p2); throw(p4);}
#endif // ALF_DEBUG_EXCEPTIONS


/**
* Error codes for Alf Widget Model. These error codes are sent within 
* AlfException class and they are common across all widgets. Widget specific 
* exceptions are defined together with widget specific exception classes. 
* For more information on widget specific errors see widget specific exception 
* classes (like CAlfButtonException).
*/
enum AlfWidgetModelException
    {
    ECommonError,
    
    /** Passed attribute had no meaning in the object 
        where it was passed to. */
    EInvalidAttribute,
    
    /* Passed attribute was valid, but the value for the 
       attribute was invalid. */
    EInvalidAttributeValue,
    
    /** Not enought information provided to affect the specified attribute. */
    EIncompleteAttributeValues,
    
    /** Passed visual was invalid */
    EInvalidVisual,
    
    /** Creation of visual failed */
    ECanNotCreateVisual,
    
    /** Passed visual value was invalid */
    EInvalidVisualValue,
    
    /** Passed Element was invalid */   
    EInvalidElement,
    
    /** Invalid data type in Alf::IAlfVariantType */
    EInvalidVariantDataType,
    
    /** Array Index out of bounds */
    EInvalidArrayIndex,
    
    /** A non-applicable operation was performed on a 
        Alf::IAlfContainer object. */
    EInvalidContainerOperation,
    
    /** A non-applicable operation was performed on a 
        Alf::IAlfMap object. */
    EInvalidMapOperation,
    
    /** A non-applicable operation was performed on a 
        Alf::IAlfBranch object. */
    EInvalidBranchOperation,
    
    /** An invalid operation was perfomed on a 
        Alf::IAlfModel object. */
    EInvalidModelOperation,
    
    /** Passed Widget was invalid */   
    EInvalidWidget,
    
    /**
     * An invalid instance ID was supplied to the factory while creating a 
     * widget. Also used if an attempt is used to create a widget with same 
     * instance ID as an existing widget.
     */ 
    EInvalidWidgetInstanceId,
    
    /**
     * No display was found from the UI Accelerator Toolkit environment in 
     * which the widget should have been constructed.
     * See Alf::IAlfWidgetFactory::createWidget() for more information.
     */
    EDisplayNotFound,

    /**
     * EInvalidArgument error code can be used when a user passes an 
     * argument which is invalid. For example, user tries  to set alignment 
     * of vertical scrollbar as top or bottom (vertical scrollbar can be left or
     * right aligned only).
     */
    EInvalidArgument,

    /**
     * Error code used when an operation would result in an invalid object 
     * hierarchy. There are multiple object hierarchies maintained in widget 
     * model that this error code can relate to. Widget objects are stored in a
     * hierarchy under a view widget
     * (see Alf::IAlfContainerWidget and Alf::IAlfWidget for more information). 
     * Widget presentation can be constructed from a  hierarchy of visual 
     * templates (see Alf::IAlfVisualTemplate).
     */
    EInvalidHierarchy
    };

/**
 *  @class AlfException alfexception.h "alf/alfexception.h"
 *  Base exception class.
 *
 *  Exceptions specify an error code, which can be one of the error codes 
 *  defined in osncore::AlfWidgetModelException. Concrete widgets derive their 
 *  own exception classes from this base class. If the exception is thrown from
 *  a concrete widget, the error code can also be one of the widget specific 
 *  error codes.
 *
 *  @lib alfwidgetutils.lib
 *  @since S60 ?S60_version
 *  @status Draft
 */
class AlfException : public exception
    {
public:
    /**
      * Constructor.
      * @param aErrorCode Error code
      */
    OSN_IMPORT  AlfException( const int aErrorCode ) throw();

    /**
      * Constructor.
      * @param aErrorCode Error code
      * @param aInfo Optional additional information or NULL
      */
    OSN_IMPORT  AlfException( int aErrorCode, const char* aInfo ) throw();

    /**
      * Constructor.
      * @param aErrorCode Error code
      * @param aInfo Optional additional information or NULL
      * @param aFileAndLine Optional file and line information or NULL
      */
    OSN_IMPORT  AlfException(
        int aErrorCode, const char* aInfo, const char* aFileAndLine ) throw();

    /**
      *  Class Destructor.
      */
    OSN_IMPORT virtual ~AlfException() throw();

    /**
      * Returns the description of the exception.
      * @return The description of the exception.
      */
    OSN_IMPORT virtual const char* what() const throw();

    /**
      * Returns the error code of the exception.
      * @return The error code of the exception.
      */
    OSN_IMPORT virtual int errorCode() const throw();

    /**
      * Returns additional information defined in the exception.
      * This can be e.g. the name of the class, which has thrown the exception.
      *
      * @return Additional information defined in the exception.
      */
    OSN_IMPORT virtual const char* info() const throw();

    /**
      * Returns a string with the filename and the line number of the source code,
      * which has thrown the exception. If the information does not exist,
      * an empty string is returned.
      *
      * @return A string with the filename and the line number of the source code,
      * which has thrown the exception.
      */
    OSN_IMPORT const char* fileAndLine() const throw();

private: // data

    int mErrorCode;
    const char* mInfo;
    const char* mFileAndLine;
    };

    }

#endif
