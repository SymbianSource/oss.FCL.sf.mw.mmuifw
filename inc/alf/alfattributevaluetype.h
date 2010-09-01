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
* Description:  attribute value type header.
*
*/


#ifndef ALFATTRIBUTEVALUETYPE_H
#define ALFATTRIBUTEVALUETYPE_H

#include <osn/osndefines.h>
#include <memory>
#include <alf/alfmetric.h>

namespace osncore
    {
class UString;
    }
using namespace osncore;
using std::auto_ptr;

namespace Alf
    {

class AlfAttributeValueTypeImpl;

/**
 *  @class AlfAttributeValueType alfattributevaluetype.h "alf/alfattributevaluetype.h"
 *  A class for attribute value type. Attributes are constructed using 
 *  AlfAttributeValueType objects. AlfAttributeValueType can store integer,float,string,
 *  enum and RGB data.   
 *  @see AlfAttribute
 * 
 *  @lib alfwidgetmodel.lib
 *  @since S60 ?S60_version
 *  @status Draft
 */
class AlfAttributeValueType
    {

public:

    /** Type enumeration. */
    enum Type
        {
        EInt, EFloat, EString
        };

    /**
     * Constructor.
     * @exception std::bad_alloc
     *
     * @param aValue The value of the attribute.
     * @param aUnit Unit of the value.
     */
    OSN_IMPORT AlfAttributeValueType(int aValue,
                                     TAlfUnit aUnit = EAlfUnitPixel);

    /**
     * Constructor.
     * @exception std::bad_alloc
     *
     * @param aValue The value of the attribute.
     * @param aUnit Unit of the value.
     */
    OSN_IMPORT AlfAttributeValueType(float aValue,
                                     TAlfUnit aUnit = EAlfUnitNormalized);

    /**
     * Constructor.
     * @exception std::bad_alloc     
     *
     * @param aValue The value of the attribute. Takes a copy of the object.
     */
    OSN_IMPORT AlfAttributeValueType(const UString& aValue);

    /**
     * Destructor.
     */
    OSN_IMPORT virtual ~AlfAttributeValueType();

    /**
     * Gets the enum value.
     * @exception osncore::AlfDataException Thrown with error code osncore::EInvalidAttributeValue
     *                                      if the attribute value type is not of enum type. 
     *
     * @return The integer value.
     */
    OSN_IMPORT virtual int enumValue() const;

    /**
     * Gets the string value.
     * @exception osncore::AlfDataException Thrown with error code osncore::EInvalidAttributeValue
     *                                      if the attribute value type is not of string type. 
     *
     * @return The string value.
     */
    OSN_IMPORT virtual const UString& stringValue() const;

    /**
     * Gets the integer value.
     * @exception osncore::AlfDataException Thrown with error code osncore::EInvalidAttributeValue
     *                                      if the attribute value type is not of integer type. 
     *
     * @return The integer value.
     */
    OSN_IMPORT virtual int intValue() const;

    /**
     * Gets the real value.
     * @exception osncore::AlfDataException Thrown with error code osncore::EInvalidAttributeValue
     *                                      if the attribute value type is not of float type. 
     *
     * @return The real value.
     */
    OSN_IMPORT virtual float realValue() const;

    /**
     * Gets the RGB value.
     * @exception osncore::AlfDataException Thrown with error code osncore::EInvalidAttributeValue
     *                                      if the attribute value type is not of float type. 
     *
     * @return The RGB value.
     */
    OSN_IMPORT virtual float rgbValue() const;

    /**
     * Gets the type of the attribute value.
     *
     * @return The type of the attribute value.
     */
    OSN_IMPORT virtual Type type() const;

    /**
     * Gets the unit of the value.
     *
     * @return The unit of the value.
     */
    OSN_IMPORT virtual TAlfUnit unit() const;

protected:

    /**
     * Constructor.
     */
    AlfAttributeValueType();

private:    // data

    auto_ptr<AlfAttributeValueTypeImpl> mData;

    };

    } // namespace Alf

#endif // ALFATTRIBUTEVALUETYPE_H
