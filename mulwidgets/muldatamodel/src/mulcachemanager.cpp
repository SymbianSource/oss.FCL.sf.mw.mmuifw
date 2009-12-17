/*
* Copyright (c) 2007-2008 Nokia Corporation and/or its subsidiary(-ies). 
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of the License "Symbian Foundation License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.symbianfoundation.org/legal/sfl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  Cache Manager class
 *
*/


#include "mulcachemanager.h"

#include <mul/imulvarianttype.h>
#include <mul/muldatapath.h>

#include <alf/alfvarianttype.h>
#include <alf/ialfmap.h>
#include <alf/ialfcontainer.h>
#include <mul/muldatapath.h>

#include <stdexcept>
#include <osn/ustring.h>

#include "mulassert.h"
#include "mulmodeldef.h"
#include "mulmodelimpl.h"
#include "mul/mulmodelutility.h"
#include "mullog.h"

namespace Alf
	{

// ---------------------------------------------------------------------------
// Constructor 
// ---------------------------------------------------------------------------
//
MulCacheManager::MulCacheManager()
			   : mTotalCount(0)
	{
	}

// ---------------------------------------------------------------------------
// Destructor 
// ---------------------------------------------------------------------------
//
MulCacheManager::~MulCacheManager()
	{	
	MulVisualItemMap::iterator itr;
	for( itr = mVisualItemMap.begin(); itr!= mVisualItemMap.end(); ++itr )
		{
		delete itr->second;
		itr->second = NULL;
		}
	mVisualItemMap.clear();
	}

// ---------------------------------------------------------------------------
// CreateNodes 
// ---------------------------------------------------------------------------
//
void MulCacheManager::CreateNodes(int aStartIndex, int aCount, const MulDataPath& /*aPath*/)
	{
	MUL_LOG_ENTRY_EXIT("MUL::MulCacheManager::CreateNodes()");
	
	__MUL_ASSERT_DEBUG( aStartIndex >= 0 && aStartIndex <= ExpandedNodeCount() && aCount > 0 , KLInvalidArgument );

	if(mTotalCount > 0 && aStartIndex < mTotalCount )
		{
		UpdataPathInMapAfterInsert(mVisualItemMap, aStartIndex, aCount);
		UpdataPathInMapAfterInsert(mSelectionMap, aStartIndex, aCount);
		}

	mTotalCount += aCount;
	}

// ---------------------------------------------------------------------------
// AddVisualItem 
// ---------------------------------------------------------------------------
//
void MulCacheManager::AddVisualItem( std::auto_ptr<MulVisualItem> aVisualItem,
									const MulDataPath& /*aPath*/ , int aIndex)
	{
	MUL_LOG_ENTRY_EXIT("MUL::MulCacheManager::AddVisualItem()");
	__MUL_ASSERT_DEBUG( aIndex >= 0 && aIndex < ExpandedNodeCount() , KLInvalidArgument );
	
	//adding item now
	mVisualItemMap[aIndex] = aVisualItem.release();
	}

// ---------------------------------------------------------------------------
// AddVisualItem 
// ---------------------------------------------------------------------------
//
//void MulCacheManager::AddVisualItem( std::auto_ptr<MulVisualItem> aVisualItem, int aItemIndex )
//	{
//	MUL_LOG_ENTRY_EXIT("MUL::MulCacheManager::AddVisualItem()");
//	__MUL_ASSERT_DEBUG( aItemIndex >= 0 && aItemIndex < ExpandedNodeCount() , KLInvalidArgument );
//	
//	MulDataPath path ;//= mTree.FindNode( aItemIndex );
//	AddVisualItem( aVisualItem, path, aItemIndex );
//	}

// ---------------------------------------------------------------------------
// RemoveVisualItem 
// ---------------------------------------------------------------------------
//
void MulCacheManager::RemoveVisualItem( int aIndex )
	{
	MUL_LOG_ENTRY_EXIT("MUL::MulCacheManager::RemoveVisualItem()");
	
	__MUL_ASSERT_DEBUG( aIndex >= 0 && aIndex < ExpandedNodeCount() , KLInvalidArgument );
	
	//MulDataPath path = mTree.FindNode( aIndex );
	MulDataPath path;
	RemoveItemFromMap( path, aIndex );	
	}

// ---------------------------------------------------------------------------
// RemoveVisualItem 
// ---------------------------------------------------------------------------
//
void MulCacheManager::RemoveVisualItem(const MulDataPath& aPath , int aIndex )
	{
	MUL_LOG_ENTRY_EXIT("MUL::MulCacheManager::RemoveVisualItem()");
	
	__MUL_ASSERT_DEBUG( aIndex >= 0 && aIndex < ExpandedNodeCount() , KLInvalidArgument );
		
	RemoveItemFromMap( aPath, aIndex );
	mSelectionMap.erase( aIndex ); //remove marking information for this path, index
	
	UpdataPathInMapAfterRemove( mVisualItemMap, aIndex,1 );	
	UpdataPathInMapAfterRemove( mSelectionMap, aIndex,1 );	
	mTotalCount--;
	}
	
// ---------------------------------------------------------------------------
// RemoveVisualItem 
// ---------------------------------------------------------------------------
//	
void MulCacheManager::RemoveVisualItem(int aIndex, int aCount, const MulDataPath& aPath)
	{
	MUL_LOG_ENTRY_EXIT("MUL::MulCacheManager::RemoveVisualItem()");
	
	__MUL_ASSERT_DEBUG( aIndex >= 0 && aIndex+aCount <= ExpandedNodeCount() , KLInvalidArgument );
	
	for(int i = aIndex ; i < aIndex+aCount; i++ )	
		{
		RemoveItemFromMap( aPath, i );
		mSelectionMap.erase( i ); //remove marking information for this path, index
		}
	
	if(aIndex + aCount < ExpandedNodeCount() )
		{
		UpdataPathInMapAfterRemove( mVisualItemMap, aIndex, aCount);	
		UpdataPathInMapAfterRemove( mSelectionMap, aIndex, aCount );		
		}
		
	mTotalCount-= aCount;
	}

// ---------------------------------------------------------------------------
// RemoveItemFromMap 
// ---------------------------------------------------------------------------
//
void MulCacheManager::RemoveItemFromMap( const MulDataPath& /*aPath*/ , int aIndex )
	{
	MUL_LOG_ENTRY_EXIT("MUL::MulCacheManager::RemoveItemFromMap()");
	
	MulVisualItemMap::iterator findIter = mVisualItemMap.find(aIndex);
	if( findIter!= mVisualItemMap.end() )
		{
		delete findIter->second;
		findIter->second = NULL;
		mVisualItemMap.erase( aIndex );
		}		
	}
	
// ---------------------------------------------------------------------------
// UpdataPathInMapAftreRemove 
// ---------------------------------------------------------------------------
//
template <typename T> void MulCacheManager::UpdataPathInMapAfterRemove( std::map<int,T >& aMap, int aIndex, int aCount )
	{	
	MUL_LOG_ENTRY_EXIT("MUL::MulCacheManager::UpdataPathInMapAfterRemove()");
	
	//loop through all items
	//for(int i = 0 ; i < ExpandedNodeCount() ; ++i )
	for(int i = aIndex ; i < ExpandedNodeCount() ; ++i )
		{
		// if index is greater than path index then index need to be modified
		//if( i >= aIndex )
			{	
				
			if( aMap.find( i ) != aMap.end() )
				{			
				T item = aMap.find( i )->second;
				aMap.erase( i );
				aMap[i-aCount] = item; //add item in to new index
				}
			}
		}
		
	}

// ---------------------------------------------------------------------------
// UpdataPathInMapAftreInsert 
// ---------------------------------------------------------------------------
//
template <typename T> void MulCacheManager::UpdataPathInMapAfterInsert( std::map<int,T >& aMap, int aIndex, int aCount )
	{
	MUL_LOG_ENTRY_EXIT("MUL::MulCacheManager::UpdataPathInMapAfterInsert()");
	
	//for( int i = ExpandedNodeCount() ; i > 0 ; --i )
	for( int i = ExpandedNodeCount() ; i >= aIndex ; --i )
        {
        // if index is greater than path index then index need to be modified
        //if( i >= aIndex )
            {   
                
            if( aMap.find( i ) != aMap.end() )
                {           
                T item = aMap.find( i )->second;
                aMap.erase( i );
                aMap[ i + aCount] = item; //add item in to new index
                }
            }
        }
	}

// ---------------------------------------------------------------------------
// UpdateVisualItem 
// ---------------------------------------------------------------------------
//
void MulCacheManager::UpdateVisualItem( MulVisualItem& aUpdatedVisualItem, int aItemIndex  )
	{
	MUL_LOG_ENTRY_EXIT("MUL::MulCacheManager::UpdateVisualItem()");
	
	__MUL_ASSERT_DEBUG( aItemIndex >= 0 && aItemIndex < ExpandedNodeCount() , KLInvalidArgument );
	
	MulDataPath path ;
	UpdateVisualItem( aUpdatedVisualItem, path, aItemIndex );
	}

// ---------------------------------------------------------------------------
// UpdateVisualItem 
// ---------------------------------------------------------------------------
//
void MulCacheManager::UpdateVisualItem( MulVisualItem& aVisualItem,const MulDataPath& /*aPath*/, int aIndex )
	{
	MUL_LOG_ENTRY_EXIT("MUL::MulCacheManager::UpdateVisualItem()");
	
	MulVisualItem* visualItem = Data( aIndex );
	std::vector<mulvisualitem::TVisualAttribute> existingAttributes = aVisualItem.Attributes( );
	for( int i = 0 ; i < existingAttributes.size() ; ++i )
		{
		mulvisualitem::TVisualAttribute attributeName = existingAttributes[i];
		IMulVariantType* newValue = aVisualItem.Attribute( attributeName );
		if(newValue)
		    {
		    std::auto_ptr<IMulVariantType> cloneValue = newValue->Clone();
		   	int newflags = aVisualItem.Flag( attributeName );
		
		   	visualItem->SetAttribute( attributeName , cloneValue.get(), newflags );
		   	cloneValue.release();
		    }
		}
	visualItem->ResetDirtyAttribute(aVisualItem);
	}

// ---------------------------------------------------------------------------
// Data 
// ---------------------------------------------------------------------------
//
MulVisualItem* MulCacheManager::Data(const MulDataPath& /*aPath*/, int aIndex ) const
	{
	MUL_LOG_ENTRY_EXIT("MUL::MulCacheManager::Data()");
	
	//__MUL_ASSERT_DEBUG( aIndex >= 0 && aIndex < ExpandedNodeCount() , KLInvalidArgument );
	
	MulVisualItemMap::const_iterator findIter = mVisualItemMap.find(aIndex);
	if( findIter != mVisualItemMap.end() )
		{		
		return findIter->second;
		}
	else
		{
		return NULL;
		}
	}

// ---------------------------------------------------------------------------
// Data 
// ---------------------------------------------------------------------------
//
MulVisualItem* MulCacheManager::Data( int aIndex ) const
	{
	MUL_LOG_ENTRY_EXIT("MUL::MulCacheManager::Data()");
	   
	__MUL_ASSERT_DEBUG( aIndex >= 0 && aIndex < ExpandedNodeCount() , KLInvalidArgument );
		
	MulDataPath path ;
	return Data( path, aIndex );
	}

// ---------------------------------------------------------------------------
// Count 
// ---------------------------------------------------------------------------
//
int MulCacheManager::Count() const
	{
	return mVisualItemMap.size();
	}
	
// ---------------------------------------------------------------------------
// ExpandedNodeCount 
// ---------------------------------------------------------------------------
//
int MulCacheManager::ExpandedNodeCount( ) const
	{
	//return mTree.ExpandedNodeCount();
	return mTotalCount;
	}

// ---------------------------------------------------------------------------
// SetMarkedIndices
// ---------------------------------------------------------------------------
//
void MulCacheManager::SetMarkedIndices( MulWidgetDef::TMulWidgetMarkingType aType ,
									 const std::vector<int>& aItemsIndex,
									 const MulDataPath& /*aPath*/)

	{
	MUL_LOG_ENTRY_EXIT("MUL::MulCacheManager::SetMarkedIndices()");
	
	bool selection( false );
	// If selection type is not mark or unmark throw invalid argument exception
	aType == MulWidgetDef::EMulMark || aType == MulWidgetDef::EMulUnmark ?
	selection = ( aType == MulWidgetDef::EMulMark ) :
	throw std::invalid_argument( KInvalidArgument ); 
	
	// Add selected indices to vector and remove unselected
	for(int i = 0 ; i < aItemsIndex.size() ; i++ )
		{
		int  index = aItemsIndex[i];
		selection ? mSelectionMap[index] = index : mSelectionMap.erase(index);
		}	
	}
	
// ---------------------------------------------------------------------------
// SetMarkedIndices
// ---------------------------------------------------------------------------
//
void MulCacheManager::SetMarkedIndices(MulWidgetDef::TMulWidgetMarkingType aType)
	{	
	MUL_LOG_ENTRY_EXIT("MUL::MulCacheManager::SetMarkedIndices()");
	
	bool selection(false);
	// If selection type is not mark all or unmark all throw invalid argument exception
	aType == MulWidgetDef::EMulMarkAll || aType == MulWidgetDef::EMulUnmarkAll ?
	selection = ( aType == MulWidgetDef::EMulMarkAll ) :
	throw std::invalid_argument( KInvalidArgument ); 
	
	int count = ExpandedNodeCount();
	for(int i = 0 ; i < count ; ++i )
		{
		selection ? mSelectionMap[i] = i : mSelectionMap.erase(i);
		}		
	}

// ---------------------------------------------------------------------------
// MarkedIndices
// ---------------------------------------------------------------------------
//
const std::vector<int>& MulCacheManager::MarkedIndices( const MulDataPath& /*aPath*/  )
	{
	MUL_LOG_ENTRY_EXIT("MUL::MulCacheManager::MarkedIndices()");
	
	mSelectionVector.clear();
	
	int count = ExpandedNodeCount();
	for( int i=0; i < count ;++i )
		{
		MulSelectionMap::const_iterator itr = mSelectionMap.find(i);
		if(	itr != mSelectionMap.end() )
			{
			mSelectionVector.insert(i);
			}
		}
	return mSelectionVector.actualVector();	
	}

// ---------------------------------------------------------------------------
// IsItemMarked
// ---------------------------------------------------------------------------
//  
bool MulCacheManager::IsItemMarked( int aIndex ) const
	{
	MUL_LOG_ENTRY_EXIT("MUL::MulCacheManager::IsItemMarked()");
	
	__MUL_ASSERT_DEBUG( aIndex >= 0 && aIndex < ExpandedNodeCount() , KLInvalidArgument );
		
	MulSelectionMap::const_iterator itr = mSelectionMap.find( aIndex );	
	return ( itr != mSelectionMap.end() );
	}

// ---------------------------------------------------------------------------
// SetMarkedIndex
// ---------------------------------------------------------------------------
// 
void MulCacheManager::SetMarkedIndex( MulWidgetDef::TMulWidgetMarkingType aType, int aIndex )
	{
	MUL_LOG_ENTRY_EXIT("MUL::MulCacheManager::SetMarkedIndex()");
	
	__MUL_ASSERT_DEBUG( aIndex >= 0 && aIndex < ExpandedNodeCount() , KLInvalidArgument );
	
	bool selection( false );
	// If selection type is not mark or unmark throw invalid argument exception
	aType == MulWidgetDef::EMulMark || aType == MulWidgetDef::EMulUnmark ?
	selection = ( aType == MulWidgetDef::EMulMark ) :
	throw std::invalid_argument( KInvalidArgument ); 
	
	selection ? mSelectionMap[aIndex] = aIndex : mSelectionMap.erase(aIndex);
	}
	
} //end of namespace alf

//end of file
