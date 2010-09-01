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
* Description:   Implemenation file for enumValue for variants.
*
*/


#include <osn/osnnew.h>
#include "alfenumvalue.h"

namespace Alf
    {

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Constructor.
// ---------------------------------------------------------------------------
//
AlfEnumValue::AlfEnumValue(int aValue, TAlfUnit aUnit)
    {
    mEnum = aValue;
    mUnit = aUnit;
    }

// ---------------------------------------------------------------------------
// Destructor.
// ---------------------------------------------------------------------------
//
AlfEnumValue::~AlfEnumValue()
    {

    }

// ---------------------------------------------------------------------------
// Gets the enum value.
// ---------------------------------------------------------------------------
//
int AlfEnumValue::enumValue() const
    {
    return mEnum;
    }

// ---------------------------------------------------------------------------
// Gets the type of the atrribute value.
// ---------------------------------------------------------------------------
//
AlfAttributeValueType::Type AlfEnumValue::type() const
    {
    return EInt;
    }

// ---------------------------------------------------------------------------
// Gets the unit.
// ---------------------------------------------------------------------------
//
TAlfUnit AlfEnumValue::unit() const
    {
    return mUnit;
    }

    } // Alf
