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
* Description:   Class for enumValue for attributes..
*
*/


#ifndef ALFENUMVALUE_H
#define ALFENUMVALUE_H

//INCLUDES
#include <osn/osndefines.h>
#include <memory>
#include "alf/alfattributevaluetype.h"

// FORWARD DECLARATIONS

using std::auto_ptr;

namespace Alf
    {

/**
 * A class for enum value type.
 *
 * @lib alfwidgetmodel.lib
 * @since S60 ?S60_version
 */
class AlfEnumValue : public AlfAttributeValueType
    {

public:

    /**
     * Constructor.
     *
     * @since S60 ?S60_version
     * @return New object.
     */
    AlfEnumValue(int aValue, TAlfUnit aUnit = EAlfUnitPixel);

    /**
     * Destructor.
     */
    virtual ~AlfEnumValue();

    /**
        * Gets the enum value.
        *
        * @since S60 ?S60_version
        * @return The enum value.
        */
    int virtual enumValue() const;

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

    int mEnum;
    TAlfUnit mUnit;

    };

    } // namespace Alf

#endif // ALFENUMVALUE_H
