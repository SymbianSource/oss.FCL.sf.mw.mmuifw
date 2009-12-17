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
* Description:  Base implementation of pull and push mode
 *
*/



//Includes
#include "mulpushmode.h"

#include "mulassert.h"
#include "mulcachemanager.h"
#include "mulpagedatawindow.h"
#include "mullog.h"

namespace Alf
    { 
    	  
// ---------------------------------------------------------------------------
// MulPushMode()
// ---------------------------------------------------------------------------
//
MulPushMode::MulPushMode( MulPageDataWindow& aDataWindow )
			: mDataWindow(aDataWindow)
	{   	
	mCacheManager.reset( new ( EMM ) MulCacheManager());		
	}

// ---------------------------------------------------------------------------
// ~MulPushMode()
// ---------------------------------------------------------------------------
//
MulPushMode::~MulPushMode()
	{
	// No implementation is required.
	}

// ---------------------------------------------------------------------------
// Insert
// ---------------------------------------------------------------------------
//
bool MulPushMode::Insert(int aIndex, int aCount, const MulDataPath& aPath )
	{
	__MUL_ASSERT( aIndex >= 0 && aIndex <= mCacheManager->ExpandedNodeCount() && aCount > 0, KLInvalidArgument );
	
	int modelCountBefore  = mCacheManager->ExpandedNodeCount();	
	mCacheManager->CreateNodes( aIndex, aCount, aPath );

	int modelCount = mCacheManager->ExpandedNodeCount();
		
	// if there is a change in expanded node count of model then update
	// the alf model. THis case would happen if we are inserting childnodes
	// for a node which is not expanded. Then no need to update the alf model
	if(mDataWindow.IsWindowEnabled() && modelCount != modelCountBefore)
		{
		ModelCountChanged(modelCountBefore,modelCount);
		}
		
	// The highlight is the same but its relative index is changed 
	// because the buffer offsets are changed. Even in this case widget has to update the visualization
	return false;
	}

// ---------------------------------------------------------------------------
// Remove
// ---------------------------------------------------------------------------
//
bool MulPushMode::Remove( int aIndex, int aCount,const MulDataPath& aPath)
	{
	__MUL_ASSERT( aIndex >= 0 && aCount > 0 && aIndex+aCount <= mCacheManager->ExpandedNodeCount(), KLInvalidArgument );

	int modelCountBefore  = mCacheManager->ExpandedNodeCount();

	mCacheManager->RemoveVisualItem( aIndex, aCount, aPath );
	
	int modelCount = mCacheManager->ExpandedNodeCount();
	
	// To inform the calling class whether the highlight has changed 
	// If the current highlight is more than the total number of items. Then udjust teh highlight
	bool highlightUpdated = ValidateCurrentHighlight();
	
	// if there is a change in expanded node count of model then update
	// the alf model. This case would happen if we are removing childnodes
	// for a node which is not expanded. Then no need to update the alf model			
	if(mDataWindow.IsWindowEnabled() && modelCount != modelCountBefore)
		{
		ModelCountChanged(modelCountBefore,modelCount);
		}
		
	// If the highlight is changed because it was greater than current model count or if
	// the buffer offsets are changed then return true		
	return ( highlightUpdated );
	}

// ---------------------------------------------------------------------------
// Update
// ---------------------------------------------------------------------------
//
void MulPushMode::Update(int /*aIndex*/, int /*aCount*/, const MulDataPath& /*aPath*/)
    {
    //no implementation required
    }

// ---------------------------------------------------------------------------
// Refresh
// ---------------------------------------------------------------------------
//
void MulPushMode::Refresh( int /*aCount*/, const MulDataPath& /*aPath*/)
    {
    // throw exception not implemented in push mode 
    throw std::logic_error("Not Implemented");
    }


// ---------------------------------------------------------------------------
// SetTemplate
// ---------------------------------------------------------------------------
//
void MulPushMode::SetTemplate( mulwidget::TLogicalTemplate aTemplateId )
    {
    mDefaultTemplate =  aTemplateId ;
    
    MulDataPath path;
    for( int i = 0 ; i < mCacheManager->Count() ; i++ )
        {
        MulPushMode::SetTemplate( i, mDefaultTemplate, path );           
        }
    }

// ---------------------------------------------------------------------------
// Template
// ---------------------------------------------------------------------------
//
mulwidget::TLogicalTemplate MulPushMode::Template( ) 
    {
    return mDefaultTemplate;
    }

// ---------------------------------------------------------------------------
// Data
// ---------------------------------------------------------------------------
//
const MulVisualItem* MulPushMode::Data( int aIndex, const MulDataPath& /*aPath*/) const
	{
	__MUL_ASSERT( aIndex >= 0 && aIndex < mCacheManager->ExpandedNodeCount(), KLInvalidArgument );
	
	return mCacheManager->Data( aIndex );			
	}

// ---------------------------------------------------------------------------
// SetData
// ---------------------------------------------------------------------------
//
void MulPushMode::SetData( int aIndex, std::auto_ptr<MulVisualItem> aVisualItem, 
                          const MulDataPath& aPath)
    {
    __MUL_ASSERT( aIndex >= 0 && aIndex < mCacheManager->ExpandedNodeCount(),_L("Invalid Argument"));

    MulVisualItem* visualItem = mCacheManager->Data( aIndex );
    MUL_LOG_INFO2("MUL::MulPushMode::SetData() Index:%d,item:%x",aIndex, visualItem);
    if( visualItem )
        {
        mCacheManager->UpdateVisualItem( *aVisualItem , aPath, aIndex );
        }
    else
        {
        // Validating input visual item
        ValidateVisualItem( *aVisualItem );
        mCacheManager->AddVisualItem( aVisualItem, aPath, aIndex );
        }
    }

// ---------------------------------------------------------------------------
// Count
// ---------------------------------------------------------------------------
//
int MulPushMode::Count(const MulDataPath& /*aPath*/) const
	{
	return mCacheManager->ExpandedNodeCount();
	} 

// ---------------------------------------------------------------------------
// SetTemplate
// ---------------------------------------------------------------------------
//
void MulPushMode::SetTemplate( int aItemIndex, mulwidget::TLogicalTemplate aTemplateId, 
	                    	  const MulDataPath& /*aPath*/ )
	{	
	MUL_LOG_ENTRY_EXIT("MUL::MulPushMode::SetTemplate");
	
    MulVisualItem* visualItem = mCacheManager->Data( aItemIndex );	
    if( visualItem )
        {
        mulwidget::TLogicalTemplate logicalTemplate = (mulwidget::TLogicalTemplate) visualItem->AttributeAsInt( mulvisualitem::KAttributeTemplate );
    	//if template is not same then only change it		
        if( logicalTemplate != aTemplateId  )
        	{
        	visualItem->SetAttribute( mulvisualitem::KAttributeTemplate, aTemplateId );   		
        	}
        }
	}
	
// ---------------------------------------------------------------------------
// CurrentItemCount
// ---------------------------------------------------------------------------
//  
int MulPushMode::CurrentItemCount() const
	{
	return mCacheManager->ExpandedNodeCount();		
	}

// ---------------------------------------------------------------------------
// CacheManager
// ---------------------------------------------------------------------------
//
MulCacheManager& MulPushMode::CacheManager() const
	{
	return *mCacheManager;
	}

// ---------------------------------------------------------------------------
//ValidateVisualItem
// ---------------------------------------------------------------------------
//
void MulPushMode::ValidateVisualItem( MulVisualItem& aVisualItem )
	{
	//add default template to item if template is not already set and default template is not null
	IMulVariantType* itemTemplate = aVisualItem.Attribute( mulvisualitem::KAttributeTemplate );
	if( !itemTemplate )
		{
		if( mDefaultTemplate != mulwidget::KTemplateUnknown )
			{
			//add default template to this visual item	
			aVisualItem.SetAttribute( mulvisualitem::KAttributeTemplate, mDefaultTemplate, 0 );
			}
		else
			{
			//template must be there
			throw std::invalid_argument("Default template not available.");
			}
		}
	}

// ---------------------------------------------------------------------------
// ModelCountChanged
// ---------------------------------------------------------------------------
//
void MulPushMode::ModelCountChanged(int aOldCount, int aNewCount)
	{
	MUL_LOG_ENTRY_EXIT("MUL::MulPushMode::ModelCountChanged()");
	
	MUL_LOG_INFO2("MUL::MulPushMode::ModelCountChanged() aOldCount:%d,aNewCount:%d", aOldCount, aNewCount );
	
	mDataWindow.AdjustOffset();	
	}

// ---------------------------------------------------------------------------
// ValidateCurrentHighlight
// ---------------------------------------------------------------------------
//
bool MulPushMode::ValidateCurrentHighlight()
	{
	// To inform the calling class whether the highlight has changed
	int modelCount = mCacheManager->ExpandedNodeCount();
	if( mDataWindow.Highlight() >= modelCount)
		{
		mDataWindow.UpdateHighlight( modelCount-1 );
		return true;
		}
	return false;
	}

// ---------------------------------------------------------------------------
// Item
// ---------------------------------------------------------------------------
//
const MulVisualItem& MulPushMode::Item( int aAbsoluteIndex )
    {
    MulVisualItem* visualItem = mCacheManager->Data( aAbsoluteIndex );
    if(!visualItem)
        {
        throw std::invalid_argument(KInvalidArgument);  
        }
    return *visualItem; 
    }
	
	} // namespace ends here
  
//End of file
    
