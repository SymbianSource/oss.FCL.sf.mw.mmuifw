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
* Description:  Implementation of Pull Mode
*
*/


//Includes
#include "mulpullmode.h"

#include <mul/imulmodelprovider.h>


#include "mulassert.h"
#include "mulcachemanager.h"
#include "muldatawindow.h"
#include "mulpagedatawindow.h"
#include "mul/mulmodelutility.h"
#include "mullog.h"
#include "mulasyncdataprovider.h"

namespace Alf
    { 
    
// ---------------------------------------------------------------------------
// MulPullMode()
// ---------------------------------------------------------------------------
//
MulPullMode::MulPullMode(MulPageDataWindow& aDataWindow,
						 const IMulModelProvider& aModelProvider)
			:MulPushMode( aDataWindow )
	{	
	//mModelProvider = const_cast<IMulModelProvider*>( &aModelProvider );
	MUL_LOG_ENTRY_EXIT("MUL::MulPullModeImpl::MulPullModeImpl()");
	mModelProvider.reset( new (ELeave) MulAsyncDataProvider( const_cast<IMulModelProvider&>(aModelProvider), aDataWindow ));		
	}

// ---------------------------------------------------------------------------
// ~MulPullMode()
// ---------------------------------------------------------------------------
//
MulPullMode::~MulPullMode()
	{
	// No implementation is required.
	MUL_LOG_ENTRY_EXIT("MUL::MulPullModeImpl::~MulPullModeImpl()");
	}

// ------------------------ From IMulModel -----------------------------------

// ---------------------------------------------------------------------------
// Update
// ---------------------------------------------------------------------------
//
void MulPullMode::Update(int aIndex, int aCount, const MulDataPath& /*aPath*/)
	{
	// throw exception if index is out of bound
	__MUL_ASSERT( aIndex >= 0 && aCount > 0 && aIndex+aCount <= mCacheManager->ExpandedNodeCount(), KLInvalidArgument );
	
	int startIndex = aIndex;
	int range = 0;
	bool firstTime = true;
	for(int i = aIndex; i < aCount+aIndex ; ++i)
		{
		if( mDataWindow.IsItemInDataWindow(i) )
			{
			range++;
			firstTime = true;
			}
		else
			{
			if(firstTime)
				{
				firstTime = false;
				if(range>0)
					{
					ProvideData(startIndex,range);
					range = 0;						
					}
				}
			startIndex = i + 1;			
			}
		}
	ProvideData(startIndex,range);	
	}

// ---------------------------------------------------------------------------
// Refresh
// ---------------------------------------------------------------------------
//
void MulPullMode::Refresh( int aCount, const MulDataPath& aPath )
	{
	int modelCountBefore  = mCacheManager->ExpandedNodeCount();
	
	// remove all old nodes
    for(int currentIndex = 0; currentIndex < modelCountBefore; currentIndex++)	
		{
		mCacheManager->RemoveVisualItem(aPath,0);			
		}
    
    //mCacheManager->RemoveVisalItem(0, aCount, aPath );
	
    // insert new nodes	
	mCacheManager->CreateNodes( 0, aCount, aPath );	

	int modelCount = mCacheManager->ExpandedNodeCount();		
	// update alf model accordingly
	if(mDataWindow.IsWindowEnabled())
		{
		if(modelCountBefore == modelCount)
			{
			Update(0,aCount,aPath);
			}
		else
			{
			ModelCountChanged(modelCountBefore,modelCount);	
			}		
		}				
	}
		
// ---------------------------------------------------------------------------
// SetData
// ---------------------------------------------------------------------------
//
void MulPullMode::SetData(int aIndex, auto_ptr<MulVisualItem> aVisualItem,const MulDataPath& aPath)
	{
	MUL_LOG_ENTRY_EXIT("MUL::MulPullModeImpl::SetData()");
	
	__MUL_ASSERT( aIndex >= 0 && aIndex < Count(aPath),_L("Invalid Argument."));
		
	//Set data to model
	if( mDataWindow.IsItemInDataWindow( aIndex ) )
		{		
		MulPushMode::SetData( aIndex, aVisualItem, aPath );
		}
	}

// ---------------------------------------------------------------------------
// SetTemplate
// ---------------------------------------------------------------------------
//
void MulPullMode::SetTemplate( mulwidget::TLogicalTemplate aTemplateId )
	{
	mDefaultTemplate = aTemplateId;

	MulDataPath path ;
	if( mDataWindow.IsWindowEnabled() )
		{
		int topOffset = mDataWindow.TopOffset();
		int bottomOffset = mDataWindow.BottomOffset();
		for( int i = topOffset ; i <= bottomOffset ; i++ )
			{
			MulPushMode::SetTemplate( i, mDefaultTemplate, path);			
			}
		
		int rearTopOffset = mDataWindow.RearTopOffset();
		int rearBottomOffset = mDataWindow.RearBottomOffset();
        for( int i = rearTopOffset; i <= rearBottomOffset && i >= 0 ; i++ )
			{	
			MulPushMode::SetTemplate( i, mDefaultTemplate, path);
			}
		}
	}

// ---------------------------------------------------------------------------
// WindowSiftedDown
// ---------------------------------------------------------------------------
//
void MulPullMode::WindowSiftedDown()
	{
	MUL_LOG_ENTRY_EXIT("MUL::MulPullModeImpl::WindowSiftedDown"); 	
		
	// If the complete datawindow is invalid.   	
//	if(mDataWindow.TopOffset() - mDataWindow.OldTopOffset() > mDataWindow.ActualBufferSize() 
//		&& mDataWindow.OldTopOffset() != KNotInitialized)
	//if(mDataWindow.TopOffset() - mDataWindow.OldTopOffset() > mDataWindow.ActualBufferSize() )
	int oldTopOffset = mDataWindow.OldTopOffset();
	int oldBottomOffset = mDataWindow.OldBottomOffset();
	int topOffset = mDataWindow.TopOffset();
	int bottomOffset = mDataWindow.BottomOffset();
	int oldRearTopOffset = mDataWindow.OldRearTopOffset();
	int oldRearBottomOffset = mDataWindow.OldRearBottomOffset();
	
	 if(  ( oldTopOffset == KNotInitialized && oldBottomOffset == KNotInitialized ) ||
	      ( topOffset - oldTopOffset >= mDataWindow.ActualBufferSize()) ||
	      ( oldRearTopOffset != KNotInitialized && 
	      ( topOffset - oldTopOffset >= ( mDataWindow.WindowSize() + mDataWindow.BufferSize() ) ) 
	    ) ) 
		{
		//remove old data from  cache
        for( int i = oldTopOffset ; i <= oldBottomOffset && i >= 0 ; i++ )
            {
            mCacheManager->RemoveVisualItem( i );          
            }
        
        for( int i = oldRearTopOffset; i <= oldRearBottomOffset && i >= 0 ; i++ )
            {   
            mCacheManager->RemoveVisualItem( i );
            }
        
		int startIndex = topOffset;
		int range = bottomOffset - startIndex + 1;
		
		MUL_LOG_INFO2("MUL::MulPullModeImpl::WindowSiftedUp ProvideData startIndex:%d,range:%d",startIndex,range);
    	ProvideData(startIndex,range);
		} 
	else
		{
		//remove extra items from top and add new items at bottom
		for( int i = oldTopOffset ; i >= 0 && i < topOffset ; i++ )
			{       
			//remove item from top
			mCacheManager->RemoveVisualItem( i );
			}

		int startIndex = oldBottomOffset +1 ;
		int range = ( bottomOffset - oldBottomOffset );				
		MUL_LOG_INFO2("MUL::MulPullModeImpl::DataWindowUpdated ProvideData startIndex:%d,range:%d",startIndex,range); 		       
    	ProvideData(startIndex,range);				
		}
	}

// ---------------------------------------------------------------------------
// WindowSiftedUp
// ---------------------------------------------------------------------------
//
void MulPullMode::WindowSiftedUp()
	{
	MUL_LOG_ENTRY_EXIT("MUL::MulPullModeImpl::WindowSiftedUp "); 
	// If the complete datawindow is invalid.  	
//	if(mDataWindow.OldBottomOffset() - mDataWindow.BottomOffset() > mDataWindow.ActualBufferSize()
//		&& mDataWindow.OldBottomOffset() != KNotInitialized)

	  if( mDataWindow.OldBottomOffset() - mDataWindow.BottomOffset() >= mDataWindow.ActualBufferSize() ||
	    ( mDataWindow.OldRearBottomOffset() != KNotInitialized && 
	      ( mDataWindow.OldBottomOffset() - mDataWindow.BottomOffset() >= ( mDataWindow.WindowSize() + mDataWindow.BufferSize() ) ) 
	    ) )	
		{
		//remove old data from  cache
        for( int i = mDataWindow.OldTopOffset() ; i <= mDataWindow.OldBottomOffset() && i >= 0 ; i++ )
            {
            mCacheManager->RemoveVisualItem( i );          
            }
        
        for( int i = mDataWindow.OldRearTopOffset( ); i <= mDataWindow.OldRearBottomOffset() && i >= 0 ; i++ )
            {   
            mCacheManager->RemoveVisualItem( i );
            }
		        
		int startIndex = mDataWindow.TopOffset();
		int range = mDataWindow.BottomOffset() - startIndex + 1;
		
		MUL_LOG_INFO2("MUL::MulPullModeImpl::WindowSiftedUp ProvideData startIndex:%d,range:%d",startIndex,range);
		ProvideData(startIndex,range);
		}  
	else
		{
		//remove extra items from bottom and add at top
		for( int i = mDataWindow.BottomOffset() ; i < mDataWindow.OldBottomOffset() ; i++ )
			{		
			mCacheManager->RemoveVisualItem(i+1);
			}

		int startIndex = mDataWindow.TopOffset();
		int range = mDataWindow.OldTopOffset() - mDataWindow.TopOffset();

		MUL_LOG_INFO2("MUL::MulPullModeImpl::WindowSiftedUp ProvideData startIndex:%d,range:%d",startIndex,range); 		       
		ProvideData(startIndex,range);			
		}  			
	}

// ---------------------------------------------------------------------------
// ProvideData
// ---------------------------------------------------------------------------
//
void MulPullMode::ProvideData(int aStartIndex, int aRange)
	{
	 //call provide data for visible window first then for buffer
	if( aRange > 0 )
	    {
	    // check if the current higlighted item is there in the data asked for
	    if( mDataWindow.Highlight() >= aStartIndex &&
	            mDataWindow.Highlight() < aStartIndex+aRange )
	        {
	        mModelProvider->ProvideData( mDataWindow.Highlight(), 1 ,MulDataPath(),true );
	        }
	    //__MUL_ASSERT( ( aRange % 18 ) == 0, _L("Invalid Renage") );
	    //check that visible data is there in provide data call
	    if( aStartIndex < mDataWindow.TopWindowOffset() && 
	        aStartIndex+aRange > mDataWindow.BottomWindowOffset() )
	        {
	            {
	            //request visible item
                int startIndex = mDataWindow.TopWindowOffset();
                int range = ( mDataWindow.BottomWindowOffset() - mDataWindow.TopWindowOffset() ) + 1 ; 
                
                MUL_LOG_INFO2("MUL::MulPullModeImpl::ProvideData visible buffer startIndex:%d,range:%d",startIndex,range);
                mModelProvider->ProvideData(startIndex, range ,MulDataPath() );
                }
	        
    	        {
    	        //request item from top
    	        int startIndex = aStartIndex;
    	        int range = mDataWindow.TopWindowOffset() - startIndex; 
    	        
    	        MUL_LOG_INFO2("MUL::MulPullModeImpl::ProvideData top buffer startIndex:%d,range:%d",startIndex,range);
    	        mModelProvider->ProvideData(startIndex, range ,MulDataPath() );
    	        }
    	        
    	        {
    	        //request item from bottom
                int startIndex = mDataWindow.BottomWindowOffset() + 1 ;
                int range = ( aStartIndex + aRange - 1) - mDataWindow.BottomWindowOffset(); 

                MUL_LOG_INFO2("MUL::MulPullModeImpl::ProvideData bottom buffer startIndex:%d,range:%d",startIndex,range);
                mModelProvider->ProvideData(startIndex, range ,MulDataPath() );
                }
	        }
	    else
	        {
	        //there is not provide data pending for visible window
	        mModelProvider->ProvideData(aStartIndex, aRange ,MulDataPath() );
	        }
	    }
	}

// ---------------------------------------------------------------------------
// ModelCountChanged
// ---------------------------------------------------------------------------
//
void MulPullMode::ModelCountChanged(int aOldCount, int aNewCount)
    {
    MUL_LOG_ENTRY_EXIT("MUL::MulPullMode::AdjustOffsetAndData()");
    
    MUL_LOG_INFO1("MUL::MulPullMode::AdjustAlfModelData() modelCount:%d", aNewCount );
    
    MulPushMode::ModelCountChanged( aOldCount, aNewCount );
    
    // fetch data for the new buffers
    if( mDataWindow.RearTopOffset() != KNotInitialized &&
        mDataWindow.RearBottomOffset() != KNotInitialized )
        {   
        if(mDataWindow.RearTopOffset() == 0)
            {
            //updating all buffer
            ProvideData( mDataWindow.RearTopOffset() , mDataWindow.RearBottomOffset() - 
                mDataWindow.RearTopOffset() + 1);
            
            ProvideData( mDataWindow.TopOffset() , mDataWindow.BottomOffset() -
                mDataWindow.TopOffset() + 1);
            }
        else
            {
            ProvideData( mDataWindow.TopOffset() , mDataWindow.BottomOffset() -
                mDataWindow.TopOffset() + 1);
                
            ProvideData( mDataWindow.RearTopOffset() , mDataWindow.RearBottomOffset() - 
                mDataWindow.RearTopOffset() + 1);                               
            }       
        }
    else
        {
        ProvideData( mDataWindow.TopOffset() , mDataWindow.BottomOffset() -
             mDataWindow.TopOffset() + 1);  
        }
    }

// ---------------------------------------------------------------------------
// DataWindowUpdated
// ---------------------------------------------------------------------------
//    
void MulPullMode::DataWindowUpdated()
    {
    //here sequence of Function call are important dont change function call sequences
    //UpdateLoopBuffer must be called first 
    if( mDataWindow.RearTopOffset() != KNotInitialized &&
        mDataWindow.OldRearTopOffset() != KNotInitialized ) //there is looping on
        {
        UpdateLoopBuffer();
        //returning from here only dont need to do any thing else
        return ;
        }
    //window shifted down
    if( mDataWindow.OldBottomOffset() < mDataWindow.BottomOffset() )
        {
        WindowSiftedDown();     
        }       
    //window shifted up
    else if( mDataWindow.OldTopOffset() > mDataWindow.TopOffset())
        {
        WindowSiftedUp();       
        }

     //add buffer in looping case
    if(mDataWindow.RearTopOffset() != KNotInitialized && mDataWindow.RearBottomOffset() != KNotInitialized) //rear offset changed
        {
        // here ask data from top to bottom
        int startIndex = mDataWindow.RearTopOffset()  ;
        int range = mDataWindow.RearBottomOffset() - mDataWindow.RearTopOffset() + 1;
                
        MUL_LOG_INFO2("MUL::MulPullMode::DataWindowUpdated startIndex:%d,range:%d",startIndex,range);              
        ProvideData(startIndex,range); 
        }       
    }

// ---------------------------------------------------------------------------
// UpdateLoopBuffer
// ---------------------------------------------------------------------------
//
void MulPullMode::UpdateLoopBuffer()
    {
    MUL_LOG_ENTRY_EXIT("MUL::MulPushMode::UpdateLoopBuffer");   
    if( mDataWindow.RearTopOffset() < mDataWindow.OldRearTopOffset() )
        {
        int startIndex = mDataWindow.TopOffset();
        int range = mDataWindow.OldRearTopOffset() - startIndex;
    
        MUL_LOG_INFO2("MUL::MulPushMode::UpdateLoopBuffer startIndex:%d,range:%d",startIndex,range);               
        ProvideData(startIndex,range);
        }
    else
        {
        int startIndex = mDataWindow.OldRearBottomOffset()+1 ;
        int range = mDataWindow.BottomOffset() - startIndex + 1;
         
        MUL_LOG_INFO2("MUL::MulPushMode::UpdateLoopBuffer startIndex:%d,range:%d",startIndex,range);               
        ProvideData(startIndex,range);
        }
    }

// ---------------------------------------------------------------------------
// Item
// ---------------------------------------------------------------------------
//
const MulVisualItem& MulPullMode::Item( int aAbsoluteIndex )
    {
    MulVisualItem* visualItem = mCacheManager->Data( aAbsoluteIndex );
    if( !visualItem )
        {
        if( mDataWindow.IsItemInDataWindow(aAbsoluteIndex) )
            {
            std::auto_ptr<MulVisualItem> dummyItem = MulModelUtility::CreateVisualItem( Template() );
            visualItem = dummyItem.get();
            mCacheManager->AddVisualItem(dummyItem, MulDataPath() ,aAbsoluteIndex );  
            }
        else
            {
            throw std::invalid_argument(KInvalidArgument);  
            }
        }
    return *visualItem; 
    }

				
    } // namespace Alf

//End of file
