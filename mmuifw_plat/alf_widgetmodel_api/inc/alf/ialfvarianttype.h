/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  This interface defines a generic data type
*
*/



#ifndef I_ALFVARIANTTYPE_H
#define I_ALFVARIANTTYPE_H


//  INCLUDES

#include <osn/osntypes.h>
#include <alf/ialfmodelbase.h>

namespace osncore
    {
class UString;
    }

using namespace osncore;

namespace Alf
    {

// FORWARD DECLARATIONS
class IAlfContainer;
class IAlfMap;
class IAlfBranch;
class IAlfModelBase;

//INTERFACE DECLARATIONS

/**
 * An interface for variant data type.
 * Application-specific data source classes may implement
 * this interface. Alternatively, a wrapper class may
 * be implemented to convert data fields from application source
 * to some IAlfVariantType implementation (e.g., CAlfVariantType).
 *
 * @lib alfwidgetmodel.lib
 * @since S60 ?S60_version
 * @status Draft
 */
class IAlfVariantType
    {
public:
    /** Type enumeration. */
    enum Type{EBool, EInt, EUint, EReal, EString, EContainer, EMap, EBranch,ECustomData};

    /** Virtual destructor. */
    virtual ~IAlfVariantType() {}

    /**
     * Set the Data the variantData
     *
     * @param aValue The new value.
     * @since S60 ?S60_version
     */
    virtual void set(IAlfVariantType& aValue) = 0;

    /**
     * Get the data type.
     *
     * @return The data type.
     * @since S60 ?S60_version
     */
    virtual Type type() const = 0;

    /**
     * Get the data value as an bool .
     *
     * @exception osncore:AlfDataException Thrown if type() is not EBool.
     * @return bool data value.
     * @since S60 ?S60_version
     */
    virtual bool boolean() const = 0;

    /**
     * Get the data value as an integer.
     *
     * @exception osncore:AlfDataException Thrown if type() is not EInt.
     * @return Integer data value.
     * @since S60 ?S60_version
     */
    virtual int integer() const = 0;

    /**
     * Get the data value as an unsigned integer.
     *
     * @exception osncore:AlfDataException Thrown if type() is not EUInt.
     * @return Unsigned integer data value.
     * @since S60 ?S60_version
     */
    virtual uint uinteger() const = 0;

    /**
     * Get the data value as a real number.
     *
     * @exception osncore:AlfDataException Thrown if type() is not EReal.
     * @return Real data value.
     * @since S60 ?S60_version
     */
    virtual double real() const = 0;

    /**
     * Get the data value as a descriptor.
     *
     * @exception osncore:AlfDataException Thrown if type() is not EString.
     * @return string value.
     * @since S60 ?S60_version
     */
    virtual const UString& string() const = 0;

    /**
     * Get the data value as a container interface.
     *
     * @exception osncore:AlfDataException Thrown if type() is not EContainer.
     * @return Container interface.
     * @since S60 ?S60_version
     */
    virtual IAlfContainer* container() = 0;

    /**
     * Get the data value as a map interface.
     *
     * @exception osncore:AlfDataException Thrown if type() is not EMap.
     * @return Map interface.
     * @since S60 ?S60_version
     */
    virtual IAlfMap* map() = 0;

    /**
     * Get the data value as a Branch interface.
     *
     * @exception osncore:AlfDataException Thrown if type() is not EBranch.
     * @return Branch interface.
     * @since S60 ?S60_version
     */
    virtual IAlfBranch* branch() = 0;

    /**
     * Get the data value as a User Defined Data Pointer.
     *
     * @exception osncore:AlfDataException Thrown if type() is not ECustomData.
     * @return CAlfModelBase interface.
     * @since S60 ?S60_version
     */
    virtual IAlfModelBase* customData() = 0;

    };

    } // namespace Alf

#endif // I_ALFVARIANTTYPE_H

// End of File
