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
* Description:  attribute exception
*
*/




#ifndef ALFATTRIBUTEEXCEPTION_H_
#define ALFATTRIBUTEEXCEPTION_H_

#include <alf/alfexception.h>

using namespace std;

namespace osncore
    {


/**
 *  UI library attribute exception class.
 *
 *  @lib core.lib
 *  @since S60 ?S60_version
 */
class AlfAttributeException : public AlfException
    {
public:
    /**
    * Constructor.
    * @param aErrorCode Error code
    */
    OSN_IMPORT AlfAttributeException( const int aErrorCode ) throw();

    /**
      * Constructor.
      * @param aErrorCode Error code
      * @param aInfo Optional additional information or NULL
      * @param aFileAndLine Optional file and line information or NULL
      */
    OSN_IMPORT  AlfAttributeException(
        int aErrorCode, const char* aInfo, const char* aFileAndLine ) throw();

    /**
    *  Class Destructor.
    */
    OSN_IMPORT virtual ~AlfAttributeException() throw();

    /**
    * Returns the description of the exception.
    * @return The description of the exception.
    */
    OSN_IMPORT virtual const char* what() const throw();
    };

    }

#endif //ALFATTRIBUTEEXCEPTION_H_

// End of File
