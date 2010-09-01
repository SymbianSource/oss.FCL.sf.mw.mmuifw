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
* Description:  widget exception
*
*/




#ifndef ALFWIDGETEXCEPTION_H_
#define ALFWIDGETEXCEPTION_H_

#include <alf/alfexception.h>

using namespace std;

namespace osncore
    {

/**
 *  @class AlfWidgetException alfwidgetexception.h "alf/alfwidgetexception.h"
 *  Widget exception class.
 *
 *  @lib alfwidgetutils.lib
 *  @since S60 ?S60_version
 *  @status Draft
 */
class AlfWidgetException : public AlfException
    {
public:
    /**
    * Constructor.
    * @param aErrorCode Error code
    */
    OSN_IMPORT AlfWidgetException( const int aErrorCode ) throw();

    /**
      * Constructor.
      * @param aErrorCode Error code
      * @param aInfo Optional additional information or NULL
      * @param aFileAndLine Optional file and line information or NULL
      */
    OSN_IMPORT  AlfWidgetException(
        int aErrorCode, const char* aInfo, const char* aFileAndLine ) throw();

    /**
    *  Class Destructor.
    */
    OSN_IMPORT virtual ~AlfWidgetException() throw();

    /**
    * Returns the description of the exception.
    * @ret The description of the exception.
    */
    OSN_IMPORT virtual const char* what() const throw();
    };

    }

#endif //ALFWIDGETEXCEPTION_H_

// End of File
