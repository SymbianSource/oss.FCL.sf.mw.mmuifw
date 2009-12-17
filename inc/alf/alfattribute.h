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
* Description:  A class for attribute.
*
*/


#ifndef ALFATTRIBUTE_H
#define ALFATTRIBUTE_H

//INCLUDES
#include <osn/osndefines.h>
#include <memory>
#include <osn/osntypes.h>
#include <alf/alfmetric.h>
#include <alf/alftimedvalue.h>
#include "alf/alfattributevaluetype.h"

using namespace osncore;

// FORWARD DECLARATIONS

namespace osncore
    {
class UString;
    }
using std::auto_ptr;

namespace Alf
    {

class AlfAttributeImpl;
class AlfAttributeValueType;

/**
 *  @class AlfAttribute alfattribute.h "alf/alfattribute.h"
 *  Class for holding attributes. 
 *  AlfAttribute is a receptacle for holding data that can be applied to  
 *  visuals/visual templates via the attribute setters. 
 *  They can also be used hold data for properties of widgets or widget parts
 *  implement the IAlfAttributeOwner interface. 
 *  An attribute can have one or more attribute value types. 
 *  Attributes can be bunched together in attribute containers.
 *  @see IAlfAttributeSetter
 *  @see IAlfAttributeOwner
 *  @see AlfAttributeValueType
 *  @see AlfAtributeContainer
 *  @lib alfwidgetmodel.lib
 *  @since S60 ?S60_version
 *  @status Draft
 */ 
class AlfAttribute
    {
public:

    /* See Declarative Hitchcock language specification
       for more detail of attribute categories */
    enum attributecategory
        {
        EStatic,     /* static attribute */
        EDynamic,    /* dynamic attribute */
        EStaticData, /* static dataattribute  */
        EDynamicData /* dynamic dataattribute */
        };

    /**
     * Constructor.
     * @exception std::bad_alloc
     *
     * @since S60 ?S60_version
     */
    OSN_IMPORT AlfAttribute();

    /**
     * Constructor.
     * @exception std::bad_alloc
     *
     * @param aName Attribute name. Makes a copy of the name.
     * @param aCategory Attribute category.
     * @return New object.
     * @since S60 ?S60_version
     */
    OSN_IMPORT AlfAttribute(const char* aName, attributecategory aCategory);

    /**
     * Constructor.
     * @exception std::bad_alloc
     *
     * @param aName Attribute name. Makes a copy of the name.
     * @param aTargetValue The target value of the child attribute.
     * @param aUnit Unit of the value.
     * @param aCategory Attribute category.
     * @return New object.
     * @since S60 ?S60_version
     */
    OSN_IMPORT AlfAttribute(const char* aName, int aTargetValue,
                            TAlfUnit aUnit = EAlfUnitPixel,
                            attributecategory aCategory = EStatic);

    /**
     * Constructor.
     * @exception std::bad_alloc
     *
     * @param aName Attribute name. Makes a copy of the name.
     * @param aTargetValue The target value of the child attribute.
     * @param aUnit Unit of the value.
     * @param aCategory Attribute category.
     * @return New object.
     * @since S60 ?S60_version
     */
    OSN_IMPORT AlfAttribute(const char* aName, float aTargetValue,
                            TAlfUnit aUnit = EAlfUnitNormalized,
                            attributecategory aCategory = EStatic);

    /**
     * Constructor.
     * @exception std::bad_alloc
     *
     * @param aName Attribute name. Makes a copy of the name.
     * @param aTargetValue The target value of the child attribute. Makes a copy of the value.
     * @param aCategory Attribute category.
     * @return New object.
     * @since S60 ?S60_version
     */
    OSN_IMPORT AlfAttribute(const char* aName, const UString& aTargetValue,
                            attributecategory aCategory = EStatic);

    /**
     * Destructor.
     */
    OSN_IMPORT virtual ~AlfAttribute();

    /**
     * Assignment operation. Assigns all the member variables
     * to the values in the reference attribute.
     * @exception std::bad_alloc
     *
     * @param aAttribute The source attribute.
     * @return Reference to the attribute.
     * @since S60 ?S60_version
     */
    OSN_IMPORT AlfAttribute& operator=(const AlfAttribute& aAttribute);

    /**
     * Clones the attribute.
     * @exception std::bad_alloc
     *
     * @return The cloned attribute. Ownership is transferred to the caller.
     * @since S60 ?S60_version
     */
    OSN_IMPORT AlfAttribute* clone();

    /**
     * Gets the name of the attribute.
     *
     * @return The name of the attribute.
     * @since S60 ?S60_version
     */
    OSN_IMPORT const char* name() const;

    /**
     * Gets the category of the attribute.
     *
     * @return The category of the attribute.
     * @since S60 ?S60_version
     */
    OSN_IMPORT attributecategory category() const;

    /**
     * Gets the type of the attribute.
     * @exception osncore::AlfAttributeException Thrown with error code osncore::EInvalidAttribute if aIndex is invalid. 
     *
     * @return The type of the attribute target value.
     * @since S60 ?S60_version
     */
    OSN_IMPORT AlfAttributeValueType::Type type(unsigned int aIndex = 0) const;

    /**
     * Gets the int target value. Use this method only if the attribute type is EInt.
     * @exception osncore::AlfAttributeException Thrown with error code osncore::EInvalidAttribute if aIndex is invalid. 
     * @param aIndex The index of the attribute value.
     * @return The integer value of the attribute.
     * @since S60 ?S60_version
     */
    OSN_IMPORT int intValue(unsigned int aIndex = 0) const;

    /**
     * Gets the real target value. Use this method only if the attribute type is EFloat.
     * @exception osncore::AlfAttributeException Thrown with error code osncore::EInvalidAttribute if aIndex is invalid. 
     *
     * @param aIndex The index of the attribute value.
     * @return The float value of the attribute.
     * @since S60 ?S60_version
     */
    OSN_IMPORT float realValue(unsigned int aIndex = 0) const;

    /**
     * Gets the string target value. Use this method only if the attribute type is EString.
     * @exception osncore::AlfAttributeException Thrown with error code osncore::EInvalidAttribute if aIndex is invalid. 
     *
     * @param aIndex The index of the attribute value.
     * @return The string value of the attribute.
     * @since S60 ?S60_version
     */
    OSN_IMPORT const UString& stringValue(unsigned int aIndex = 0) const;

    /**
     * Returns true if the attribute value is set.
     *
     * @return The value state.
     * @since S60 ?S60_version
     */
    OSN_IMPORT bool isValueSet() const;

    /**
     * Gets the unit of the value.
     * @exception osncore::AlfAttributeException Thrown with error code osncore::EInvalidAttribute if aIndex is invalid. 
     *
     * @param aIndex The index of the attribute value.
     * @return The unit.
     * @since S60 ?S60_version
     */
    OSN_IMPORT virtual TAlfUnit unit(unsigned int aIndex = 0) const;

    /**
     * Adds a new target value to the attribute.
     *
     * @param aValue New value to be added.
     * @since S60 ?S60_version
     */
    OSN_IMPORT void addTargetValue(AlfAttributeValueType* aValue);

    /**
     * Gets target value count.
     * @exception osncore::AlfAttributeException Thrown with error code osncore::EInvalidAttribute if aIndex is invalid. 
     *
     * @return Value count.
     * @since S60 ?S60_version
     */
    OSN_IMPORT unsigned int getTargetValueCount() const;

    /**
     * Sets the target value of the attribute.
     *
     * @param aIndex The index of the attribute value.
     * @param aValue The target value of the attribute.
     * @since S60 ?S60_version
     */
    OSN_IMPORT void setTargetValue(AlfAttributeValueType* aValue,
                                   unsigned int aIndex = 0);

    /**
     * Gets the target value of the attribute.
     *
     * @param aIndex The index of the attribute value.
     * @return The target value of the attribute.
     * @since S60 ?S60_version
     */
    OSN_IMPORT AlfAttributeValueType* getTargetValue(
        unsigned int aIndex = 0) const;

    /**
     * Removes the target value of the attribute.
     * @exception osncore::AlfAttributeException Thrown with error code osncore::EInvalidAttribute if aIndex is invalid. 
     *
     * @param aIndex The index of the attribute value.
     * @since S60 ?S60_version
     */
    OSN_IMPORT void removeTargetValue(unsigned int aIndex = 0);

    /**
     * Adds a new source value to the attribute.
     *
     * @param aValue New value to be added.
     * @since S60 ?S60_version
     */
    OSN_IMPORT void addSourceValue(AlfAttributeValueType* aValue);

    /**
     * Gets source value count.
     * @exception osncore::AlfAttributeException Thrown with error code osncore::EInvalidAttribute if aIndex is invalid. 
     *
     * @return The source value count.
     * @since S60 ?S60_version
     */
    OSN_IMPORT unsigned int getSourceValueCount() const;

    /**
     * Sets the source value of the attribute.
     *
     * @param aIndex The index of the attribute value.
     * @param aValue The source value of the attribute.
     * @since S60 ?S60_version
     */
    OSN_IMPORT void setSourceValue(AlfAttributeValueType* aValue,
                                   unsigned int aIndex = 0);

    /**
     * Gets the source value of the attribute.
     *
     * @param aIndex The index of the attribute value.
     * @return The source value of the attribute.
     * @since S60 ?S60_version
     */
    OSN_IMPORT AlfAttributeValueType* getSourceValue(
        unsigned int aIndex = 0) const;

    /**
     * Removes the source value of the attribute.
     * @exception osncore::AlfAttributeException Thrown with error code osncore::EInvalidAttribute if aIndex is invalid. 
     *
     * @param aIndex The index of the attribute value.
     * @since S60 ?S60_version
     */
    OSN_IMPORT void removeSourceValue(unsigned int aIndex = 0);

    /**
     * Sets the interpolation style of the attribute.
     *
     * @param aInterpolationStyle The interpolation style.
     * @since S60 ?S60_version
     */
    OSN_IMPORT void setInterpolationStyle(TAlfInterpolationStyle aInterpolationStyle);

    /**
     * Gets the interpolation style of the attribute.
     *
     * @return The interpolation style of the attribute.
     * @since S60 ?S60_version
     */
    OSN_IMPORT TAlfInterpolationStyle getInterpolationStyle() const;

    /**
     * Sets the transition time for the attribute.
     *
     * @param aTime The transition time for the attribute.
     * @since S60 ?S60_version
     */
    OSN_IMPORT void setTime(int aTime);

    /**
     * Gets the transition time for the attribute.
     *
     * @return The transition time for the attribute.
     */
    OSN_IMPORT int getTime() const;

    /**
     * Sets the mapping function id for the attribute.
     *
     * @param aId The mapping function id for the attribute.
     * @since S60 ?S60_version
     */
    OSN_IMPORT void setMappingFunctionId(int aId);

    /**
     * Gets the mapping function id for the attribute.
     *
     * @return The mapping function id for the attribute.
     * @since S60 ?S60_version
     */
    OSN_IMPORT int getMappingFunctionId() const;

    /**
     * Sets the datafield for the attribute.
     *
     * @param aDataField The datafield for the attribute.
     * @since S60 ?S60_version
     */
    OSN_IMPORT void setDataField(const char* aDataField);

    /**
     * Gets the datafield for the attribute.
     *
     * @return The datafield for the attribute.
     * @since S60 ?S60_version
     */
    OSN_IMPORT const char* getDataField() const;

    /**
     * Sets the delay before the transition.
     *
     * @param aDelay The delay before the transition.
     * @since S60 ?S60_version
     */
    OSN_IMPORT void setDelay(int aDelay);

    /**
     * Gets the delay before the transition.
     *
     * @return The delay before the transition.
     * @since S60 ?S60_version
     */
    OSN_IMPORT int getDelay() const;

    /**
     * Gets the dirty information
     *
     * @return whether dirty or not
     * @since S60 ?S60_version
     */
    OSN_IMPORT bool isDirty( ) const;

    /**
     * Sets the dirty information
     *
     * @param flag indicates whether remove or set dirtiness
     * @since S60 ?S60_version
     */
    OSN_IMPORT void setDirty(bool aFlag );

private:

    /**
    * Resets the data.
    *
     * @param aName Attribute name.
     * @param aValue The value of the attribute.
     * @param aCategory Attribute category.
     * @since S60 ?S60_version
    */
    void reset(const char* aName, AlfAttributeValueType* aValue,
               attributecategory aCategory);

    /**
     * Declaring private copy construction to prevent usage.
     *
     * @param aAttribute The attribute.
     * @since S60 ?S60_version
     */
    AlfAttribute(const AlfAttribute& attribute);

    /**
     * Helper function for cloning value type.
     * @exception std::bad_alloc
     *
     * @param aValue The source attribute value.
     * @since S60 ?S60_version
     */
    AlfAttributeValueType* cloneValueType(AlfAttributeValueType* aValue) const;

private:    // data

    // Implementation data
    auto_ptr<AlfAttributeImpl> mData;
    };

    } // namespace Alf

#endif //ALFATTRIBUTE_H