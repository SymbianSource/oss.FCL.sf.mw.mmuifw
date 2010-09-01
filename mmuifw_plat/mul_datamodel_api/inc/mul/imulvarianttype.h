/*
* Copyright (c) 2007-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Interface for generic data type
*
*/


#ifndef I_MULVARIANTTYPE_H
#define I_MULVARIANTTYPE_H

#include <memory>
//#include <osn/osntypes.h>
#include <e32cmn.h>

// Forward Declarations

namespace Alf
    {

// Forward Declarations
 class IAlfMap;

/*! @class IMulVariantType
 *  @brief Interface for mul model data type 
 *
 *  Application should implement this interface if application needs
 * 	to provide custom data type to model.
 *
 */
class IMulVariantType
    {
public:
    
    enum TMulType{ EUnknown ,EBool, EInt, EUint, EReal, EDes, EMap, ECustomData };

    /**
     * Create copy of variant type
     *
     * @return Copy of variant type and ownsership is transferred
     */ 
    virtual std::auto_ptr< IMulVariantType > Clone() = 0;
    
    /**
     * Get data as symbian descriptior
     * 
     * @return Descriptor data value.
     */
    virtual const TDesC& DesC() const = 0 ;
    
    /**
     * Type of data
     * 
     * @return Data type of Variant Data
     */
    virtual TMulType Type() const = 0 ;
    
    /**
     * Destructor
     */
    virtual ~IMulVariantType()
    	{
    	}
    
    /**
     * Get the data value as an bool .
     *
     * @exception osncore:AlfDataException Thrown if type() is not EBool.
     * @return bool data value.
     */
    virtual bool boolean() const = 0;

    /**
     * Get the data value as an integer.
     *
     * @exception osncore:AlfDataException Thrown if type() is not EInt.
     * @return Integer data value.
     */
    virtual int integer() const = 0;

    /**
     * Get the data value as an unsigned integer.
     *
     * @exception osncore:AlfDataException Thrown if type() is not EUInt.
     * @return Unsigned integer data value.
     */
    virtual uint uinteger() const = 0;

    /**
     * Get the data value as a real number.
     *
     * @exception osncore:AlfDataException Thrown if type() is not EReal.
     * @return Real data value.
     */
    virtual double real() const = 0;

    /**
     * Get the data value as a map interface.
     *
     * @exception osncore:AlfDataException Thrown if type() is not EMap.
     * @return Map interface.
     * @since S60 ?S60_version
     */
    virtual IAlfMap& Map() const = 0 ;
            
    };

    } // namespace Alf

#endif // I_MULVARIANTTYPE_H

//End of file

