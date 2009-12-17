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
* Description:  Visual item class
*
*/


#include <mul/mulvisualitem.h>

#include <mul/imulvarianttype.h>
#include <mul/mulvarianttype.h>

#include <osn/ustring.h>
#include <stdexcept>

#include <e32math.h>	// for Pow function
#include "mul/mulmodelutility.h"

using namespace osncore;
using namespace std;

namespace Alf
    {
// ---------------------------------------------------------------------------
// Defination of class MulAttribute
// ---------------------------------------------------------------------------
//
class MulVisualItem::MulAttribute
    {
public: //Constructor and destructor

	/**
	 * C++ constructor.
	 */
	MulAttribute():mValue(NULL),mFlag(0)
		{
		}
		
	/**
	 * C++ constructor.
	 *
	 * @param aValue value of the attribute
	 * @param aAttributeFlags property of this attribute
	 */
	 MulAttribute( IMulVariantType* aValue, int aAttributeFlags = 0 )
	 	{
	 	mValue = aValue;
	 	mFlag = aAttributeFlags;
	 	}

	/**
	 * Copy constructor.
	 *
	 * @param aAttribute existing attribute
	 */
	MulAttribute( const MulAttribute& aAttribute )
	 	{
	 	mValue = aAttribute.mValue->Clone().release();
	 	mFlag = aAttribute.mFlag;
	 	}

	/**
	 * Destructor.
	 */
	~MulAttribute()
		{
		delete mValue;
		}	
	
public: //data
     
	IMulVariantType* mValue ; //own    
    int mFlag ; 
    
    };
    
// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
OSN_EXPORT MulVisualItem::MulVisualItem( )
	{				
	// Clearing dirty bit flags to 0;
	mBitAttrFlag1 = 0;
	mBitAttrFlag2 = 0;
	}

// ---------------------------------------------------------------------------
// Copy Constructor
// ---------------------------------------------------------------------------
//
OSN_EXPORT MulVisualItem::MulVisualItem( const MulVisualItem& aVisualItem )
	{	
	map<mulvisualitem::TVisualAttribute,MulAttribute*> mapToCopy = aVisualItem.mVisualItem;
	map<mulvisualitem::TVisualAttribute,MulAttribute*>::iterator itr;
	for( itr = mapToCopy.begin(); itr!= mapToCopy.end(); ++itr )
		{
		mVisualItem[itr->first] = new (EMM) MulAttribute( *itr->second );
		}
	mBitAttrFlag1 = aVisualItem.mBitAttrFlag1;
	mBitAttrFlag2 = aVisualItem.mBitAttrFlag2;
	}

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
OSN_EXPORT MulVisualItem::~MulVisualItem()
	{
	map<mulvisualitem::TVisualAttribute,MulAttribute* >::iterator itr;
	for( itr = mVisualItem.begin(); itr!= mVisualItem.end(); ++itr )
		{
		delete itr->second;
		itr->second = NULL;
		}
	mVisualItem.clear();
	}

// ---------------------------------------------------------------------------
// Add new attributes to visual item
// ---------------------------------------------------------------------------
//
//OSN_EXPORT void MulVisualItem::SetAttribute( mulvisualitem::TVisualAttribute aName, 
//											 const UString& aValue, int aAttributeFlags )
//	{
//	auto_ptr<MulVariantType> variantdata(new (EMM) MulVariantType( aValue ));
//	
//	SetAttribute( aName, variantdata.release(),aAttributeFlags,EDoesOwn);
//	}

// ---------------------------------------------------------------------------
// Add new attributes to visual item
// ---------------------------------------------------------------------------
//
OSN_EXPORT void MulVisualItem::SetAttribute( mulvisualitem::TVisualAttribute aName, 
                                             const TDesC& aValue, int aAttributeFlags )
    {
    auto_ptr<MulVariantType> variantdata(new (EMM) MulVariantType( aValue ));
    
    SetAttribute( aName, variantdata.release(),aAttributeFlags,EDoesOwn);
    }


// ---------------------------------------------------------------------------
// Add new attributes to visual item
// ---------------------------------------------------------------------------
//
OSN_EXPORT void MulVisualItem::SetAttribute( mulvisualitem::TVisualAttribute aName, 
											  int aValue,int aAttributeFlags )
	{	
	auto_ptr<MulVariantType> variantdata(new ( EMM ) MulVariantType( aValue ));
		
	SetAttribute(aName,variantdata.release(),aAttributeFlags,EDoesOwn);	
	}


// ---------------------------------------------------------------------------
// Add new attributes to visual item
// ---------------------------------------------------------------------------
//
OSN_EXPORT void MulVisualItem::SetAttribute( mulvisualitem::TVisualAttribute aName, IMulVariantType* aValue, 
		  									   int aAttributeFlags ,
		  									   TAttributeOwnership /*aAttributeOwership*/ )
	{
	map<mulvisualitem::TVisualAttribute,MulAttribute* >::iterator findIter = mVisualItem.find( aName );
	if( findIter != mVisualItem.end() )
		{
		delete findIter->second;
		findIter->second = NULL;
		mVisualItem.erase( aName );
		}
	mVisualItem[aName] = new (EMM) MulAttribute(aValue,aAttributeFlags );
	SetDirty(aName, true);
	}


// ---------------------------------------------------------------------------
// Is the input attribute dirty, to be queried before a redraw
// ---------------------------------------------------------------------------
//
OSN_EXPORT bool MulVisualItem::IsDirty(mulvisualitem::TVisualAttribute aAttr) const
{
	int i = (int)aAttr;
	TReal base  = 2.0;
	TReal r = 0.0;
	TReal val = 0.0;
	TInt err = KErrNone;

	// If mBitAttrflag is binary 101, AND'ing it with 100(attribute 3) will 
	// set 100 to return true for attribute 3 being dirty

	// if attribute index <18 check against mBitAttrFlag1, else check
	// against mBitAttrFlag2
	if(i < 18)
		{
		val = i;
		err = Math::Pow(r, base, val);
		if(KErrNone != err)
			return true;
			
		return mBitAttrFlag1 & (int)r;
		}
	else
		{
		val = (int)(mulvisualitem::KMul_n) - i;
		err = Math::Pow(r, base, val);
		if(KErrNone != err)
			return true;
		
		return mBitAttrFlag2 & (int)r;
		}
    
}
 

// ---------------------------------------------------------------------------
// Set the attribute as dirty in this Visual item based on input flag
// ---------------------------------------------------------------------------
//
void MulVisualItem::SetDirty(mulvisualitem::TVisualAttribute aAttr, bool aDirty)
{
	int i = (int)aAttr;
	TReal base  = 2.0;
	TReal r = 0.0;
	TReal val = i;
	TInt err = KErrNone;
	// If mBitAttrflag is binary 001, OR'ing it with 100(attribute 3) will 
	// set 101 to set bit for attribute 3 as 1 in dirty flag
	if(aDirty)
		{
		// if attribute index <18 operate on mBitAttrFlag1
		// else operate against mBitAttrFlag1
		if(i < 18)
			{ 
			err = Math::Pow(r,base,val);
			mBitAttrFlag1 |= (int)r;
			}
			else
			{
			err = Math::Pow(r,base,(TReal)((int)(mulvisualitem::KMul_n) - i));
			mBitAttrFlag2 |= (int)r;
			}
		}
	else
	{
		// if attribute index <18 operate on mBitAttrFlag1
		// else operate against mBitAttrFlag1
		if(i < 18)
		{
			err = Math::Pow(r,base,val);
			mBitAttrFlag1 &= ~(int)r;
		}
		else
		{
			err = Math::Pow(r, base,(TReal)((int)(mulvisualitem::KMul_n) - i));
			mBitAttrFlag2 &= ~(int)r;
		}
	}
	if(KErrNone != err)
		throw std::invalid_argument("Error setting dirty flag for attribute"); 
}


// ---------------------------------------------------------------------------
// ResetDirtyAttribute
// ---------------------------------------------------------------------------
//
void MulVisualItem::ResetDirtyAttribute(const MulVisualItem& aVisualItem)
{
	mBitAttrFlag1 = aVisualItem.mBitAttrFlag1;
	mBitAttrFlag2 = aVisualItem.mBitAttrFlag2;
}
// ---------------------------------------------------------------------------
// Attribute
// ---------------------------------------------------------------------------
//
OSN_EXPORT IMulVariantType* MulVisualItem::Attribute( mulvisualitem::TVisualAttribute aName ) const
	{
	map<mulvisualitem::TVisualAttribute,MulAttribute* >::const_iterator findIter = mVisualItem.find( aName );
	if( findIter != mVisualItem.end() )
		{		
		MulAttribute* attribute = findIter->second;
		IMulVariantType* attributeValue = attribute->mValue;
		return  attributeValue;
		}
	else
		{
		//throw std::invalid_argument("Invalid attribute name.");
		return NULL;
		}
	}

// ---------------------------------------------------------------------------
// AttributeAsInt
// ---------------------------------------------------------------------------
//
OSN_EXPORT int MulVisualItem::AttributeAsInt( mulvisualitem::TVisualAttribute aName ) const 
	{
	IMulVariantType* attributeValue = Attribute(aName);
	if( attributeValue ) 
	    {
	    return attributeValue->integer();
	    }
	throw std::invalid_argument("Invalid attribute name."); 
	}

// ---------------------------------------------------------------------------
// AttributeAsString
// ---------------------------------------------------------------------------
//
OSN_EXPORT const TDesC& MulVisualItem::AttributeAsString( mulvisualitem::TVisualAttribute aName ) const
	{
	IMulVariantType* attributeValue = Attribute(aName);
    if( attributeValue ) 
        {
        return attributeValue->DesC();
        }
	throw std::invalid_argument("Invalid attribute name."); 
	}

// ---------------------------------------------------------------------------
// ExistingAttributes
// ---------------------------------------------------------------------------
//
OSN_EXPORT const std::vector<mulvisualitem::TVisualAttribute> MulVisualItem::Attributes() const 
	{
	map<mulvisualitem::TVisualAttribute,MulAttribute* >::const_iterator itr;
	std::vector<mulvisualitem::TVisualAttribute> attributesName;
	for( itr = mVisualItem.begin(); itr!= mVisualItem.end(); ++itr )
		{
		attributesName.push_back( itr->first );
		}
	return attributesName;
	}

// ---------------------------------------------------------------------------
// Flag
// ---------------------------------------------------------------------------
//
int MulVisualItem::Flag( mulvisualitem::TVisualAttribute aName ) 
    {
     MulAttribute* attribute = mVisualItem.find( aName )->second;
     return attribute->mFlag;
    }

// ---------------------------------------------------------------------------
// RemoveAttribute
// ---------------------------------------------------------------------------
//
void MulVisualItem::RemoveAttribute( mulvisualitem::TVisualAttribute aName )
    {
    map<mulvisualitem::TVisualAttribute,MulAttribute* >::iterator findIter = mVisualItem.find( aName );
    if( findIter != mVisualItem.end() )
		{
		delete findIter->second;
		findIter->second = NULL;
		mVisualItem.erase( aName );
		}
    }

    }// namespace Alf	

//End of file
