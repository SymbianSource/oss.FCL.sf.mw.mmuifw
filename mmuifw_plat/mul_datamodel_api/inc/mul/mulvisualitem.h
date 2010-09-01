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
* Description:  Visual Item class
 *
*/


#ifndef MULVISUALITEM_H
#define MULVISUALITEM_H

#include <osn/osndefines.h>
#include <osn/ustring.h>
#include <alf/alfvarianttype.h>
#include <memory>
#include <map>
#include <vector>
#include <e32cmn.h>

namespace osncore
    {
    class UString;
    }
using namespace osncore;
using namespace std;
    

namespace Alf
    {
    

//enum mulvisualitem
namespace mulvisualitem
    {
    
    enum TVisualAttribute
        {
        KMulIcon1 = 0,
        KMulIcon2 ,
        KMulIcon3 ,
        KMulTitle ,
        KMulTitle1 ,
        KMulTitle2 ,
        KMulDetail ,
        KMulDate ,
        KMulIndicator1 ,
        KMulIndicator2 ,
        KMulIndicator3 ,
        KMulIndicator4 ,
        KMulIndicator5,
        KMulIndicator6,
        KMulActionItem ,
        KMulEnabled ,
        KMulSoftkeyTitle ,
        KMulLatch ,
        KMulRead ,
        KMulInputText,
        KMulEmptyItem ,
        KVisualItemProperties,
        KVisualItemAttributes ,   
        KAttributeTemplate, 
        KMulMaxRange, 
		KMulMinRange,
		KMulTick,
		KMulPrimaryTick,
		KMulSecondaryTick, 
		KMulLeftText, 
		KMulRightText,  
		KMulCounter1,
		KMulCounter2,
		KMulImage,
		KMulPercent1,
		KMulPercent2,
		KMul_n,
        KLastAttribute = 100 // reserved 0 to 100
        };
    
    }//namespace mulvisualitem
    
/* Forward declarations*/
class IAlfMap;
class IAlfContainer;
class IMulVariantType;
/**
/*! @class MulVisualItem
 *  @brief Client need to use this class to add data in data model.
 *
 *  Visual item is set of attributes corresponding to an item in the widget.
 *  The attributes added to visual item get visualized on the widget
 * 
 */  
class MulVisualItem
	{
private:

	class MulAttribute;
    		
public:

	/**
	 * Flags for attribute 
	 */
	enum TAttributeFlags
		{
		EDisposable = 0x01 // attribute with this flag on can be deleted from item is it is out of visible window
		};
		
	enum TAttributeOwnership
		{
		EDoesOwn = 0, //attribute is owned by model
		EDoesNotOwn	  //attribute is not owned by model
		};
	
public:	//Constructor and Dstructor
	
	/**
	 * C++ constructor.
	 */
	OSN_IMPORT MulVisualItem();
	
	/**
	 * C++ constructor.
	 */
	OSN_IMPORT MulVisualItem( const MulVisualItem& aVisualData );
	
	 /**
 	 * Destructor.
 	 */
	OSN_IMPORT ~MulVisualItem();
   
public: // New mehtod

	/**
	 * Add new attribute to visual item
	 * 
	 * @param aName name of attribute
	 * @param aValue value of attribute
	 * @param aAttributeFlags property of this attribute
	 */
	//OSN_IMPORT void SetAttribute( mulvisualitem::TVisualAttribute aName, const UString& aValue,int aAttributeFlags = 0 );
	
	/**
	 * Add new attribute to visual item
	 * 
	 * @param aName name of attribute
	 * @param aValue value of attribute
	 * @param aAttributeFlags property of this attribute
	 */
	OSN_IMPORT void SetAttribute( mulvisualitem::TVisualAttribute aName, int aValue,int aAttributeFlags = 0 );
	
   /**
     * Add new attribute to visual item
     * 
     * @param aName name of attribute
     * @param aValue value of attribute
     * @param aAttributeFlags property of this attribute
     */
    OSN_IMPORT void SetAttribute( mulvisualitem::TVisualAttribute aName, const TDesC& aValue ,int aAttributeFlags = 0 );
	
	/**
	 * Add new attribute to visual item
	 * 
	 * @param aName name of attribute
	 * @param aValue value of attribute 
	 * @param aAttributeFlags property of this attribute
	 * @param aAttributeOwership If the value is EDoesOwn then the ownership 
	 *  of the attribute transfered to the visual item else not.
	 */
	OSN_IMPORT void SetAttribute( mulvisualitem::TVisualAttribute aName, IMulVariantType* aValue, 
								  int aAttributeFlags = 0,TAttributeOwnership aAttributeOwership = EDoesOwn );
	
	/**
	 * Return Value of specified attribute
	 * 
	 * @param aName name of attribute need to retrived
	 * @return Value of attribute
	 */
	OSN_IMPORT IMulVariantType* Attribute( mulvisualitem::TVisualAttribute aName ) const;
	
	/**
	 * Return Value of specified attribute as integer
	 * 
	 * @param aName name of attribute need to retrived
	 * @return Value of attribute
	 */
	OSN_IMPORT int AttributeAsInt( mulvisualitem::TVisualAttribute aName ) const;
	
	/**
	 * Return Value of specified attribute as String
	 * 
	 * @param aName name of attribute need to retrived
	 * @return Value of attribute
	 */
	OSN_IMPORT const TDesC& AttributeAsString( mulvisualitem::TVisualAttribute aName ) const;
	  
	 /**
	  * Return the existing attributes of the visual item
	  * @return Vector of the names of the existing attributes
	  */
	 OSN_IMPORT const std::vector<mulvisualitem::TVisualAttribute> Attributes() const;


	 /**
	  * Return the bool value if attribute is dirty(updated and not redrawn)
	  * @return bool Value of attribute dirty status
	  */
	 OSN_IMPORT bool IsDirty( mulvisualitem::TVisualAttribute aAttr ) const;
	 
	  
	  /**
	  * Return the flag of the attributes 
	  * @param aName name of attribute need to retrived
	  * @return Value of flag of the attribute 
	  */
	  int Flag( mulvisualitem::TVisualAttribute aName );
	   
	  /**
	   * Remove attribute from visual item - required by garbage collector
	   * 
	   * @param aName Attribute name to be removed
	   */
	  void RemoveAttribute( mulvisualitem::TVisualAttribute aName );

	 /**
	  * Sets the dirty flag for a Visual attribute
	  * @param aDirty sets dirty flag to true/false
	  * @return void
	  */
	 void SetDirty(mulvisualitem::TVisualAttribute aAttr, bool aDirty);
	 
	 
	 /**
	  * Resets the dirty flag for a Visual attribute from input Visual item
	  * @param const MulVisualItem& Visual item with input dirty flags
	  * @return void
	  */
	 void ResetDirtyAttribute(const MulVisualItem& aVisualItem);
	 
	  	 	  
private:

	std::map< mulvisualitem::TVisualAttribute, MulAttribute* > mVisualItem;
	// Attribute flags - indicates which attributes are dirty(need redrawing)
	// Using 2 attributes to store 36 attributes (refer TVisualAttribute)
	unsigned int mBitAttrFlag1;	// stores dirty bits of first 18 attributes
	unsigned int mBitAttrFlag2;	// stores dirty bits of second 18 attributes
	};
	
    } // namespace Alf	

#endif //MULVISUALITEM_H

//End of file
