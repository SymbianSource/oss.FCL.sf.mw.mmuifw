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
* Description:  MulVarianttype implementation
*
*/

#ifndef MULVARIANTTYPE_H
#define MULVARIANTTYPE_H

#include <mul/imulvarianttype.h>
#include <alf/alfvarianttype.h>

#include <osn/osndefines.h>
       
namespace Alf
    {       
// Forward Declarations

/*! @class MulVariantType
 *  @brief To create data for data model
 */  
class MulVariantType : public IMulVariantType
    {
public: //Constructor and Destructor

    /**
     * The default constructor.
     */
    OSN_IMPORT MulVariantType();
    
    /**
     * Constructor for Descriptor
     * 
     * @param aDes Descriptor
     * @return new Object
     */
    OSN_IMPORT MulVariantType( const TDesC& aDes );
            
    /**
     * Constructor for variant data type creation for a integer type of data
     * @param aValue - integer to be assigned for the variant data
     * @return New object.
     */
    OSN_IMPORT MulVariantType(const int& aValue);
    
    /**
     * Constructor for variant data type creation for a Unsigned integer type of data
     * @param aValue - unsigned integer to be assigned for the variant data
     * @return New object.
     */
    OSN_IMPORT MulVariantType(const uint& aValue);
        
    /**
     * Constructor for variant data type creation for a real type of data
     * @param aValue - Real value to be assigned for the variant data
     * @return New object.
     */
    OSN_IMPORT MulVariantType(const double& aValue);
        
    /**
     * Constructor for variant data type creation for a bool type of data
     * @param aValue - Boolean value to be assigned for the variant data
     * @return New object.
     */
    OSN_IMPORT MulVariantType(const bool& aValue);
    
    /**
     * Constructor for variant data type creation for a Alf Variant type of data
     * @param aData -the variant data
     * @return New object.
     */
    OSN_IMPORT MulVariantType( IAlfMap& aData ); 
    
    /**
     * Destructor
     */
    OSN_IMPORT ~MulVariantType();
    
public: // From IMulVariantType
    
    OSN_IMPORT std::auto_ptr< IMulVariantType > Clone();
       
    OSN_IMPORT const TDesC& DesC() const;
       
    OSN_IMPORT TMulType Type() const;
       
    OSN_IMPORT bool boolean() const ;

    OSN_IMPORT int integer() const ;

    OSN_IMPORT uint uinteger() const ;

    OSN_IMPORT double real() const ;

    OSN_IMPORT IAlfMap& Map() const ;
    
private:
    
    void Construct( TMulType aType );
        
private:
    
    class MulVariantTypeImpl;
    
private:

    std::auto_ptr<MulVariantTypeImpl> mVarData;
    
    };
    
    } // namespace Alf  

#endif //I_MULVARIANTTYPE_H

//End of file
