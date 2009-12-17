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
* Description:  Utility class for data model
*
*/


#ifndef MULMODELUTILITY_H_
#define MULMODELUTILITY_H_

#include <alf/alfvarianttype.h>
#include <osn/osndefines.h>
#include <e32cmn.h>
#include <osn/ustring.h>

#include "mul/imulwidget.h"

namespace osncore
    {
    class UString;
    }
using namespace osncore;

namespace Alf
	{
	
//Forward Declaration
class MulVisualItem;
class MulModelUtility
    {
public:

	/**
	 * Create new Variant data from existing variant data.
	 * 
	 * @param aValue Existing variant data;
	 * 
	 * @reutrn new variant data, Ownership is transferred
	 */
    OSN_IMPORT static IAlfVariantType* CreateVariantType( IAlfVariantType& aValue );
    
    /**
	 * Convert map of visual item to map of alf map.
	 * Ite create new Map and while returning new map owner ship of map is transfered
	 * 
	 * @param aVisualItem Map of visual item
	 * @return Map
	 */
     OSN_IMPORT static std::auto_ptr<IAlfMap> ConvertVisualItemToMap( MulVisualItem& aVisualItem );
	
	/**
     * 
     * Convert AlfMap to visual item
     * 
     * @param aMap IAlfMAP
     * @return MulVisualItem ownership is trsnaferred
     */
    OSN_IMPORT static std::auto_ptr<MulVisualItem> ConvertMapToVisualItem( IAlfMap& aMap );
    
	/**
	 * Check the prefix of the name to see whether they can be added in alfmodel
	 * 
	 * @param aName Name whose prefix is to be checked
	 */
	static bool CheckNamePrefix( UString aName ) ;
    
    /**
     * Convert UString to CAlfString
     * 
     * @param aUString UString instance to convert to CAlfSTring

     * @return Converted CAlfString
     */
    OSN_IMPORT static UString UStringToAlfString(const UString& aUString);
    
    /**
    * Convsert CAlfString to UString
    * 
    * @param aUString CAlfSTring instance to convert to UString

    * @return Converted UString
    */
    OSN_IMPORT static UString AlfStringToUString(const UString& aAlfString);
    
    /**
     * Covert integer to AlfString
     * 
     * @param aNumber number to convert
     * @return AlfString
     */
    OSN_IMPORT static UString IntToUString( int aNumber );
    
    /**
     * Convert TDes to UString
     *
     * @param aDes a TDes type string to be converted
     * @param aString the converted Ustring
     */
    OSN_IMPORT static void ConvertTDesToUString (const TDesC& aDes, UString& aString);
    
    /**
     * Convert TDes8 to UString
     *
     * @param aDes a TDes8 type string to be converted
     * @param aString the converted Ustring
     */
    OSN_IMPORT static void ConvertTDes8ToUString (const TDesC8& aDes, UString& aString);
    
    /**
     * Convert UString to TDes
     *     
     * @param aString a UString type string to be converted
     * @param aDes the TDes type converted string
     */
    OSN_IMPORT static void ConvertUStringToTDes (const UString& aString, TDes& aDes);    
    
    /**
     * Convert UString to TDes8
     *     
     * @param aString a UString type string to be converted
     * @param aDes the TDes8 type converted string
     */
     OSN_IMPORT static void ConvertUStringToTDes8 (const UString& aString, TDes8& aDes);    
    
    /**
     * Create a visualitem and set its default template
     *     
     * @param aDefaultTemplate defaultTemplate for that visual item
     */
    OSN_IMPORT static std::auto_ptr<MulVisualItem> CreateVisualItem( mulwidget::TLogicalTemplate aDefaultTemplate ); 
    
                   
    };
	    
    } //namespace Alf

#endif /*MULMODELUTILITY_H_*/

//End of file


