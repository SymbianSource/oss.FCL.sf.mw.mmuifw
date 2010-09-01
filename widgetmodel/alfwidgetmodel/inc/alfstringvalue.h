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
* Description:   Declares StringType attribute Value.
*
*/


#ifndef ALFSTRINGVALUE_H
#define ALFSTRINGVALUE_H

//INCLUDES
#include <osn/osndefines.h>
#include <memory>
#include <osn/ustring.h>
#include "alf/alfattributevaluetype.h"

// FORWARD DECLARATIONS

namespace osncore
    {
class UString;
    }
using namespace osncore;
using std::auto_ptr;

namespace Alf
    {

/**
 * A class for a string value type.
 *
 * @lib alfwidgetmodel.lib
 * @since S60 ?S60_version
 */
class AlfStringValue : public AlfAttributeValueType
    {
public:

    /**
     * Constructor.
     *
     * @since S60 ?S60_version
     * @return New object.
     */
    AlfStringValue( const UString& aValue );

    /**
     * Destructor.
     */
    virtual ~AlfStringValue();

    /**
        * Gets the string value.
        *
        * @since S60 ?S60_version
        * @return The string value.
        */
    const virtual UString& stringValue() const;

    /**
     * Gets the type of the atrribute value.
     *
     * @since S60 ?S60_version
     * @return The type of the value.
     */
    virtual Type type() const;

    /**
     * Gets the unit.
     *
     * @since S60 ?S60_version
     * @return The unit.
     */
    virtual TAlfUnit unit() const;

private:    // data

    UString mString;
    };

    } // namespace Alf

#endif // ALFSTRINGVALUE_H
