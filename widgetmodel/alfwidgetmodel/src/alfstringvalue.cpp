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
* Description:   StringType attribute Value.
*
*/


#include <osn/osnnew.h>
#include <alf/alfdataexception.h>

#include "alfstringvalue.h"

namespace Alf
    {

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Constructor.
// ---------------------------------------------------------------------------
//
AlfStringValue::AlfStringValue(const UString& aValue)
    {
    mString = aValue;
    }

// ---------------------------------------------------------------------------
// Destructor.
// ---------------------------------------------------------------------------
//
AlfStringValue::~AlfStringValue()
    {

    }

// ---------------------------------------------------------------------------
// Gets the string value.
// ---------------------------------------------------------------------------
//
const UString& AlfStringValue::stringValue() const
    {
    return mString;
    }

// ---------------------------------------------------------------------------
// Gets the type of the atrribute value.
// ---------------------------------------------------------------------------
//
AlfAttributeValueType::Type AlfStringValue::type() const
    {
    return EString;
    }

// ---------------------------------------------------------------------------
// Gets the unit.
// ---------------------------------------------------------------------------
//
TAlfUnit AlfStringValue::unit() const
    {
    ALF_THROW(AlfDataException,EInvalidAttributeValue,"AlfStringValue")
    }

    } // Alf
