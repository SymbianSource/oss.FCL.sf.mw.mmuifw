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
* Description:  Utility class for widgets visualization in terms of LCT data
*
*/


#ifndef MULVISUALUTILITY_H_
#define MULVISUALUTILITY_H_

#include <alf/alflayout.h>
#include <osn/ustring.h>

// Avkon Includes
#include <aknlayout2hierarchy.h>
#include <AknUtils.h>
#include <AknsConstants.h>

// OpenC Includes
#include <libc/string.h>
#include <libc/stdlib.h>

#include <vector>
using namespace osncore;

namespace Alf
	{
	    
enum TMulCurrentOrientation
    {
    EMulPortrait,
    EMulLandScape
    };	
/**
 * Different visual types
 */
enum TMulLCTVisualType
    {
    EVisualImage = 0,
    EVisualText
    };

/**
 * Different attribute types
 */    
enum TMulCustomAttributeType
    {
    EAttributeNone,
    EAttributeCategory,
    EAttributeOpacity,
    EAttributeStyle,
    EAttributeFontthemecolor,
    EAttributeHAlign,
    EAttributeVAlign
    };

/**
 * Different Category types
 */    
enum TMulAttributeCategoryType
    {
    ECategoryInvalid,
    ECategoryData,
    ECategoryStaticData
    };
    
/**
 * Co-ordinate data
 */    
struct LCTData
	{
	    int mPosX;
	    int mPosY;
	    int mWidth;
	    int mHeight;
	    int mFontId;
	};

/**
 * LCT Id and variety
 */	
struct LCTPair
    {
    UString mLctId;
    int mVariety;
    };

// ---------------------------------------------------------------------------
// Class to hold  attributes name and value
// ---------------------------------------------------------------------------
//
class CustomAttributeStructure 
    {
public:

   /**
	* Constructor
	*
	* @param aName				Name of the Attribute
	* @param aTargetVal			Value the Attribute
	* @param aAttributeType		Name of the Attribute
	* @param aCategoryType		Name of the Attribute
	*
	*
	*/
    CustomAttributeStructure(const UString& aName, const UString& aTargetVal,
        TMulCustomAttributeType aAttributeType = EAttributeNone, 
        TMulAttributeCategoryType aCategoryType = ECategoryInvalid): 
            mName(aName),
            mTargetVal(aTargetVal),
            mAttributeType(aAttributeType), 
            mCategoryType(aCategoryType)
            
             {
             mLctPair.mLctId = UString("");
             mLctPair.mVariety = -1;
             } 
    
   /**
	* Constructor
	*
	* @param aName				Name of the Attribute
	* @param aLctInfo			LCT information of the custom attribute
	* @param aAttributeType		Name of the Attribute
	* @param aCategoryType		Name of the Attribute
	*
	*
	*/
    CustomAttributeStructure(const UString& aName, const LCTPair& aLctInfo,
        TMulCustomAttributeType aAttributeType = EAttributeNone, 
        TMulAttributeCategoryType aCategoryType = ECategoryInvalid): 
            mName(aName),
            mTargetVal(""),
            mAttributeType(aAttributeType), 
            mCategoryType(aCategoryType)
            
             {
             mLctPair.mLctId = aLctInfo.mLctId;
             mLctPair.mVariety = aLctInfo.mVariety;
             }   
    
   /**
	* Sets the name of the custom attribute
	*
	* @param aName	Name of the custom attribute
	* @return nothing
	*
	*/         
    void SetName(const UString& aName) {mName = aName;}
    
   /**
	* Sets the value the custom attribute
	*
	* @param aTarget	Value of the custom attribute
	* @return nothing
	*
	*/ 
    void SetTargetValue(const UString& aTarget) {mTargetVal = aTarget; }
    
   /**
	* Get the name of attribute
	*
	* 
	* @return Name of the attribute
	*
	*/
    const UString GetName() const {return mName; } 
    
   /**
	* Get the value of the attribute
	*
	* 
	* @return Value of the attribute
	*
	*/
    const UString GetTargetvalue() const {return mTargetVal;} 
     
   /**
	* Sets the attribute type the custom attribute
	*
	* @param aAttributeType		Attribute type of the custom attribute
	* @return nothing
	*
	*/ 
    void SetAttributeType(TMulCustomAttributeType aAttributeType) {mAttributeType = aAttributeType;}
    
   /**
	* Sets the Category type the custom attribute
	*
	* @param aCategoryType		Category type of the custom attribute
	* @return nothing
	*
	*/
    void SetCategoryType(TMulAttributeCategoryType aCategoryType) {mCategoryType = aCategoryType;} 
       
   /**
	* Get the attribute type of the attribute
	*
	* 
	* @return TMulCustomAttributeType
	*
	*/  
    TMulCustomAttributeType AttributeType() const {return mAttributeType;}
    
   /**
	* Get the category type of the attribute
	*
	* 
	* @return TMulAttributeCategoryType
	*
	*/ 
    TMulAttributeCategoryType CategoryType()const {return mCategoryType;}
    
   /**
	* Get the LCT information of attribute
	*
	* 
	* @return LCTPair
	*
	*/ 
    const LCTPair& GetLctPair() 
    {
    return mLctPair;
    }

private: 
 
   /**
	* Attribute name
	*/
    UString mName;
    
   /**
	* Attribute value
	*/
    UString mTargetVal;
    
   /**
	* Attribute type
	*/
    TMulCustomAttributeType mAttributeType;
    
   /**
	* Attribute  Category type
	*/
    TMulAttributeCategoryType mCategoryType;    
    
   /**
	* LCT Id and variety
	*/
    LCTPair mLctPair;
    };
    

// ---------------------------------------------------------------------------
// Class to hold a visual's attributes
// ---------------------------------------------------------------------------
//
class LCTVisualStructure
    {
public:
	
   /**
	* Default Constructor
	*
	*/
    LCTVisualStructure()
    {
    	
    }    
        
   /**
	* Destructor
	*
	*/
    ~LCTVisualStructure()
    {
    for(int i =0; i < mAttributeStructure.size(); i++)
        {
        delete mAttributeStructure[i];
        }
    mAttributeStructure.clear();
    } 
        
   /**
	* Sets the LCT Id of the visual Structure (part of LCT information of Visual Structure)
	*
	* @param aId	LCT Id of Visual Structure
	* @return nothing
	*
	*/
    void SetLCTId(const UString& aId) {mLctPair.mLctId = aId;}
    
   /**
	* Sets the Variety Id of the visual Structure (part of LCT information of Visual Structure)
	*
	* @param aId	Variety Id of Visual Structure
	* @return nothing
	*
	*/
    void SetVarietyId(int aId) {mLctPair.mVariety = aId;}
    
   /**
	* Gets the LCT Id of the visual Structure (part of LCT information of Visual Structure)
	*
	* @return nothing
	*
	*/
    const UString GetLCTId() const {return mLctPair.mLctId; } 
    
   /**
	* Gets the Variety Id of the visual Structure (part of LCT information of Visual Structure)
	*
	* @return nothing
	*
	*/
    const int GetVarietyId() const {return mLctPair.mVariety;}
     
   /**
	* Sets the type of visual this structure contains (can be either image/text)
	*
	* @param aVisualType	EVisualImage/EVisualText
	* @return nothing
	*
	*/
    void SetVisualType(const TMulLCTVisualType aVisualType) {mVisualType = aVisualType; }
    
   /**
	* Gets the visual type of the visual Structure 
	*
	* @return TMulLCTVisualType
	*
	*/
    TMulLCTVisualType VisualType()const { return mVisualType;} 
    
   /**
	* Adds a Custom attribute structure to this visual structure
	*
	* @param	aAttr	Custom Attribute structure to be added
	*					
	*
	*/    
    void AddAttribute(CustomAttributeStructure* aAttr)
    {
    if(aAttr)
        mAttributeStructure.push_back(aAttr);
    }
    
   /**
	* Gets a Custom attribute structure at a specified index in this visual structure
	*
	* @param	aIndex	Index at which the attribute struct desired
	* @return   CustomAttributeStructure, NULL if no attribute is found
	*
	*/ 
    const CustomAttributeStructure* GetAttribute(int aIndex) const
    {
    if(aIndex >=0 && aIndex < mAttributeStructure.size())
        return mAttributeStructure[aIndex];
    else
        return NULL;
    }
        
   /**
	* Gets a Custom attribute structure by name in this visual structure
	*
	* @param	aAttrName	Name of the attribute struct desired
	* @return   CustomAttributeStructure, NULL if no attribute is found
	*					
	*
	*/  
    const CustomAttributeStructure* GetAttributeByName(const UString& aAttrName) const
    {
    for (vector<CustomAttributeStructure*>::const_iterator it = mAttributeStructure.begin(); 
            it!=mAttributeStructure.end(); ++it) 
    	{
    	if(!strcmp(((*it)->GetName()).getUtf8(),aAttrName.getUtf8()))
    		return *it;
    	} 
    return NULL;        
    } 
    
   /**
	* Gets the current attribute count
	*
	* @return total number of attributes
	*
	*/
    int AttributeCount() const
    {
    return mAttributeStructure.size();
    }

   /**
	* Sets the Id of the visual Structure 
	*
	* @param aId	Identifier of Visual Structure
	* @return nothing
	*
	*/
    void SetId(const UString& aId) {mId = aId;}
    
   /**
	* Gets the Id of the visual Structure 
	*
	* @return Identifier of the visual structure
	*
	*/   
    UString GetId() const { return mId; }
    
    void SetPositions(int x,int y)
	    {
	    mX = x;
	    mY = y;
	    }
    void SetDimensions(int width,int height)
	    {
	    mW = width;
	    mH = height;
	    }  
    int PositionX() const {return mX;}
    int PositionY() const {return mY;}
    int Width() const {return mW;}
    int Height() const {return mH;}
   /**
	* Gets the LCT Information of the visual Structure 
	*
	* @return LCTPair 
	*
	*/   
    const LCTPair& GetLctPair() 
    {
    return mLctPair;
    }

        
private:

   /**
	* Class to hold  attributes name and value
	*
	*/
    UString mId;
     
	/**
	 * LCT Id and variety
	 */ 
    LCTPair mLctPair;   
    
	/**
 	 * Type of Visual
     */
    TMulLCTVisualType mVisualType;
    
    /**
	* Array of attribute structures (owned)
	*
	*/
    std::vector<CustomAttributeStructure*> mAttributeStructure;
    int mX,mY,mW,mH;
    };

// ---------------------------------------------------------------------------
// Custom data type to hold information about different visuals
// ---------------------------------------------------------------------------
//
class LCTElementStructure
    {
public:
    
   /**
	* Destructor
	*
	*/
    ~LCTElementStructure()
    {
    for(int i =0; i < mVS.size(); ++i)
        {
        delete mVS[i];            
        }
    mVS.clear(); 
    }
       
   /**
	* Adds a Visual structure to this Element structure
	*
	* @param	aVStructure		VisualStructure to be added
	* @return 	nothing				
	*
	*/  
    void AddVisualStructure(LCTVisualStructure* aVStructure)
    {
    if(aVStructure)
    mVS.push_back(aVStructure);
    }   
    
   /**
	* Gets a Visual structure at a specified index in this visual structure
	*
	* @param	aIndex	Index at which the attribute struct desired
	* @return   LCTVisualStructure, NULL if no structure is found
	*
	*/    
    const LCTVisualStructure* GetVisualStruture(int aIndex)
    {
    if(aIndex >=0 && aIndex < mVS.size())
        return mVS[aIndex];
    else
        return NULL;
    }
    
    /**
	* Gets a Visual structure with specified name
	*
	* @param	aVisualName	name of the  visual structure
	* @return   LCTVisualStructure, NULL if no structure is found
	*
	*/    
    const LCTVisualStructure* GetVisualStrutureByName(const UString& aVisualName) const
    {
    for (vector<LCTVisualStructure*>::const_iterator it = mVS.begin(); 
            it!=mVS.end(); ++it) 
    	{
    	if(!strcmp(((*it)->GetId()).getUtf8(),aVisualName.getUtf8()))
    		return *it;
    	} 
    //if attribute by this name is not found
    return NULL;        
    }
        
    /**
	* Gets a Visual structure count
	*
	* @param	nothing
	* @return   int visual structure count
	*
	*/ 
    int VisualCount()
    {
    return mVS.size();
    }
        
    /**
	* Sets the visual structure Id
	*
	* @param	aId
	* @return   nothing
	*
	*/ 
    void SetId(const UString& aId) {mId = aId;}
    
    /**
	* Gets the visual structure Id
	*
	* @param	nothing
	* @return   UString 
	*
	*/ 
    UString GetId() const { return mId; }
    
    void SetPositions(int x,int y)
	    {
	    mX = x;
	    mY = y;
	    }
    void SetDimensions(int width,int height)
	    {
	    mW = width;
	    mH = height;
	    }  
    int PositionX() const {return mX;}
    int PositionY() const {return mY;}
    int Width() const {return mW;}
    int Height() const {return mH;}
   /**
	* Sets the LCT Id
	*
	* @param	aId
	* @return   nothing
	*
	*/ 
    void SetLCTId(const UString& aId) {mLctPair.mLctId = aId;}
    
    /**
	* Sets the Variety Id
	*
	* @param	aId
	* @return   nothing
	*
	*/ 
    void SetVarietyId(int aId) {mLctPair.mVariety = aId;}
    
   /**
	* Returns the LCT Id
	*
	* @param	nothing
	* @return   UString 
	*
	*/ 
    const UString GetLCTId() const {return mLctPair.mLctId; }
    
    /**
	* Returns the Variety Id
	*
	* @param	nothing
	* @return   int
	*
	*/  
    const int GetVarietyId() const {return mLctPair.mVariety;} 
    
    /**
	* Returns the LCT Pair structure
	*
	* @param	nothing
	* @return   LCTPair
	*
	*/  
    const LCTPair& GetLctPair() 
    {
    return mLctPair;
    }

private:
	/**
	 * Identifier for the element structure
	 */
    UString mId;
    
    /**
	 * LCT information of element
	 */
    LCTPair mLctPair;
    
    /**
     * Array of children visual structure (owned)
     */
    vector<LCTVisualStructure*> mVS;    
    
    int mX,mY,mW,mH;
    };



class MulVisualUtility
	{
	public:
	    /**
	     *  Default Constructor
	     */
	    OSN_IMPORT MulVisualUtility();
	    
	    /**
	     * Destructor
	     */
	    OSN_IMPORT ~MulVisualUtility();
	   
	   /**
	    * Accessor function for the LCT data
	    * 
	    * @return	LCTData		internal data that contains the coordinate information
	    */
	    OSN_IMPORT const LCTData& data() const ;
	    
	    /**
	     *  Uses the passed LCTPair and populates the LCT related information
	     *  as its internal Data.
	     *
	     *  @param	aPair			LCTPair to be parsed
	     *  @param  aParentLayout	ParentLayout wit respect to which the 
	     *							coordinate calculation has to be made
	     *  @return nothing
	     */
	    OSN_IMPORT void parseLCTInfo(const LCTPair& aPair, CAlfLayout* aParentLayout= NULL);
	     
	    /**
	     *  Uses the passed LCTPair and populates the LCT related information
	     *  as its internal Data.
	     *
	     *  @ :: yet to implement
	     *  @return nothing
	     */
	    OSN_IMPORT void parseLCTTextInfo();
	     
	private:
	    /**
	     * Internal function to concert LCT IDs into absolute data 
	     */	
	     void readLCTData();
	     
	    /**
	     * Internal function to concert LCT IDs into absolute data for text 
	     */	
	     void readLCTTextData();
	     
	     bool isParentRelative(int aVal) const;
	
	private:
	    /**
	     * LCT data, Owned
	     */
	    auto_ptr <LCTData> mData;
	    
	    /**
	     * LCT id
	     */
	     UString mLCTId;
	     
	    /**
	     * Variety id
	     */
	     int mVarietyId;
	};

	}
 //namespace Alf

#endif /*MULVISUALUTILITY_H_*/
