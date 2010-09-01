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
* Description:   Implementation to know the type of attribute.
*
*/


#include <e32def.h>
#include <osn/osnnew.h>
#include <alf/alfdataexception.h>
#include <alf/alfattributeexception.h>
#include "alf/alfattributevaluetype.h"

#include "alfenumvalue.h"
#include "alfrealvalue.h"
#include "alfstringvalue.h"

namespace Alf
    {

class AlfAttributeValueTypeImpl
    {
public:
    AlfAttributeValueTypeImpl()
        {
        mValueType = NULL;
        }

    ~AlfAttributeValueTypeImpl()
        {
        delete mValueType;
        }

    AlfAttributeValueType* mValueType;
    };

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Constructor.
// ---------------------------------------------------------------------------
//
AlfAttributeValueType::AlfAttributeValueType()
    {

    }

// ---------------------------------------------------------------------------
// Constructor.
// ---------------------------------------------------------------------------
//
OSN_EXPORT AlfAttributeValueType::AlfAttributeValueType(int aValue,
        TAlfUnit aUnit)
    {
    auto_ptr<AlfEnumValue> value(new (EMM) AlfEnumValue(aValue, aUnit));
    auto_ptr<AlfAttributeValueTypeImpl> valueType(
        new (EMM) AlfAttributeValueTypeImpl());

    mData.reset(valueType.release());       // ownership transferred
    mData->mValueType = value.release();    // ownership transferred
    }

// ---------------------------------------------------------------------------
// Constructor.
// ---------------------------------------------------------------------------
//
OSN_EXPORT AlfAttributeValueType::AlfAttributeValueType(float aValue,
        TAlfUnit aUnit)
    {
    auto_ptr<AlfRealValue> value(new (EMM) AlfRealValue(aValue, aUnit));
    auto_ptr<AlfAttributeValueTypeImpl> valueType(
        new (EMM) AlfAttributeValueTypeImpl());

    mData.reset(valueType.release());       // ownership transferred
    mData->mValueType = value.release();    // ownership transferred
    }

// ---------------------------------------------------------------------------
// Constructor.
// ---------------------------------------------------------------------------
//
OSN_EXPORT AlfAttributeValueType::AlfAttributeValueType(const UString& aValue)
    {
    auto_ptr<AlfStringValue> value(new (EMM) AlfStringValue(aValue));
    auto_ptr<AlfAttributeValueTypeImpl> valueType(
        new (EMM) AlfAttributeValueTypeImpl());

    mData.reset(valueType.release());       // ownership transferred
    mData->mValueType = value.release();    // ownership transferred
    }

// ---------------------------------------------------------------------------
// Destructor.
// ---------------------------------------------------------------------------
//
OSN_EXPORT AlfAttributeValueType::~AlfAttributeValueType()
    {

    }

// ---------------------------------------------------------------------------
// Gets the enum value.
// ---------------------------------------------------------------------------
//
OSN_EXPORT int AlfAttributeValueType::enumValue() const
    {
    return intValue();
    }

// ---------------------------------------------------------------------------
// Gets the string value.
// ---------------------------------------------------------------------------
//
OSN_EXPORT const UString& AlfAttributeValueType::stringValue() const
    {
    if (type() != EString)
        {
        ALF_THROW(AlfDataException,EInvalidAttributeValue,"AlfAttributeValueType")
        }
    return mData->mValueType->stringValue();
    }

// ---------------------------------------------------------------------------
// Gets the int value.
// ---------------------------------------------------------------------------
//
OSN_EXPORT int AlfAttributeValueType::intValue() const
    {
    if (type() != EInt)
        {
        ALF_THROW(AlfDataException,EInvalidAttributeValue,"AlfAttributeValueType")
        }
    return mData->mValueType->enumValue();
    }

// ---------------------------------------------------------------------------
// Gets the real value.
// ---------------------------------------------------------------------------
//
OSN_EXPORT float AlfAttributeValueType::realValue() const
    {
    if (type() != EFloat)
        {
        ALF_THROW(AlfDataException,EInvalidAttributeValue,"AlfAttributeValueType")
        }
    return mData->mValueType->realValue();
    }

// ---------------------------------------------------------------------------
// Gets the RGB value.
// ---------------------------------------------------------------------------
//
OSN_EXPORT float AlfAttributeValueType::rgbValue() const
    {
    return realValue();
    }

// ---------------------------------------------------------------------------
// Gets the type of the atrribute value.
// ---------------------------------------------------------------------------
//
OSN_EXPORT AlfAttributeValueType::Type AlfAttributeValueType::type() const
    {
    return mData->mValueType->type();
    }

// ---------------------------------------------------------------------------
// Gets the unit.
// ---------------------------------------------------------------------------
//
OSN_EXPORT TAlfUnit AlfAttributeValueType::unit() const
    {
    return mData->mValueType->unit();
    }

    } // Alf
