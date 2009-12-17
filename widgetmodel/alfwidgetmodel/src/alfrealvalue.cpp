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
* Description:   Real Value for attribute Value type.
*
*/


#include <osn/osnnew.h>
#include "alfrealvalue.h"

namespace Alf
    {

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Constructor.
// ---------------------------------------------------------------------------
//
AlfRealValue::AlfRealValue(float aValue, TAlfUnit aUnit)
    {
    mReal = aValue;
    mUnit = aUnit;
    }

// ---------------------------------------------------------------------------
// Destructor.
// ---------------------------------------------------------------------------
//
AlfRealValue::~AlfRealValue()
    {

    }

// ---------------------------------------------------------------------------
// Gets the real value.
// ---------------------------------------------------------------------------
//
float AlfRealValue::realValue() const
    {
    return mReal;
    }

// ---------------------------------------------------------------------------
// Gets the type of the atrribute value.
// ---------------------------------------------------------------------------
//
AlfAttributeValueType::Type AlfRealValue::type() const
    {
    return EFloat;
    }

// ---------------------------------------------------------------------------
// Gets the unit.
// ---------------------------------------------------------------------------
//
TAlfUnit AlfRealValue::unit() const
    {
    return mUnit;
    }

    } // Alf
