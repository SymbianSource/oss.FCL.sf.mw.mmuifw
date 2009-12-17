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
* Description:   Defines RealValue for attributeTypes..
*
*/


#ifndef ALFREALVALUE_H
#define ALFREALVALUE_H

//INCLUDES
#include <osn/osndefines.h>
#include <memory>
#include "alf/alfattributevaluetype.h"

// FORWARD DECLARATIONS

using std::auto_ptr;

namespace Alf
    {

/**
 * A class for a real value type.
 *
 * @lib alfwidgetmodel.lib
 * @since S60 ?S60_version
 */
class AlfRealValue : public AlfAttributeValueType
    {
public:

    /**
     * Constructor.
     *
     * @since S60 ?S60_version
     * @param aValue The value of the attribute. Makes a copy of the attribute.
     * @return New object.
     */
    AlfRealValue( float aValue, TAlfUnit aUnit = EAlfUnitNormalized );

    /**
     * Destructor.
     */
    virtual ~AlfRealValue();

    /**
        * Gets the real value.
        *
        * @since S60 ?S60_version
        * @return The real value.
        */
    float virtual realValue() const;

    /**
     * Gets the type of the attribute value.
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

    float mReal;
    TAlfUnit mUnit;

    };

    } // namespace Alf

#endif // ALFREALVALUE_H
