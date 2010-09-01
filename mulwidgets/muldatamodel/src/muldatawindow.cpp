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
* Description:  Implementation of Data window
*
*/


//Includes
#include "muldatawindow.h"

#include <stdexcept>
#include <e32debug.h>

//Internal includes
#include "mulassert.h"
#include "mulmodelimpl.h"
#include "mullog.h"

using namespace std;

namespace Alf
    {
    
// ---------------------------------------------------------------------------
// MulDataWindow
// ---------------------------------------------------------------------------
//
MulDataWindow::MulDataWindow( MulModelImpl& aMulModel )
			  :mMulModel( aMulModel ),
			   mHighlight(KNotInitialized),
			   mItemIndex(KNotInitialized),
			   mOldItemIndex(KNotInitialized),
			   mBufferSize(KNotInitialized),
			   mWindowSize(KNotInitialized),
			   mWindowOffset(KNotInitialized),
			   mWindowTop(0),
			   mWindowBottom(0),
			   mBufferTop(0),
			   mBufferBottom(0),
			   mOldBufferTop(KNotInitialized),
			   mOldBufferBottom(KNotInitialized),
			   mRearBufferTop(0),
			   mRearBufferBottom(0),
			   mOldRearBufferTop(KNotInitialized),
			   mOldRearBufferBottom(KNotInitialized)
	{	
	}

// ---------------------------------------------------------------------------
// SetWindowSize
// ---------------------------------------------------------------------------
//
void MulDataWindow::SetWindowSize( int aWindowSize )
	{
	MUL_LOG_ENTRY_EXIT("MUL::MulDataWindow::SetWindowSize()"); 
	
	__MUL_ASSERT( aWindowSize > 0, KLInvalidArgument );
	
	if( mWindowSize != aWindowSize )
		{
		if( KNotInitialized == mBufferSize )
			{
			mBufferSize = aWindowSize;			
			}
			
		mWindowSize = aWindowSize;
		//devide window half 
		//Try to keep this much item above highlight and below highlight
		mWindowOffset = mWindowSize / 2; 
		
		//adjusting window offset
		//if window size changes then the top and bottom offset should be 
		// adjusted such that highlight remains at the centre
		//window top and bottom are inclusive
		
		mWindowTop = mItemIndex - mWindowOffset;	
		mWindowTop = mWindowTop < 0 ? 0 : mWindowTop ;		
		mWindowBottom = mWindowTop + mWindowSize - 1;
		mWindowBottom = mWindowBottom >= mMulModel.CurrentItemCount() ? (mMulModel.CurrentItemCount() -1) : mWindowBottom;	
		
		MUL_LOG_INFO2("MUL::MulDataWindow::SetWindowSize() mWindowTop:%d,mWindowBottom:%d",mWindowTop,mWindowBottom); 
		
		AdjustBuffer();
		mMulModel.DataWindowUpdated();
		}
	//else nothing needed
	}
	
// ---------------------------------------------------------------------------
// SetBufferSize
// ---------------------------------------------------------------------------
//
void MulDataWindow::SetBufferSize( int aBufferSize )
	{
	MUL_LOG_ENTRY_EXIT("MUL::MulDataWindow::SetBufferSize()"); 
	
	__MUL_ASSERT( aBufferSize > 0, KLInvalidArgument );
	
	if( mBufferSize != aBufferSize )
		{			
		mBufferSize = aBufferSize;
		
		if(mMulModel.CurrentItemCount() > 0)
			{
			AdjustBuffer();
			mMulModel.DataWindowUpdated();
			}
		}
	//else nothing needed
	}

// ---------------------------------------------------------------------------
// ~MulDataWindow
// ---------------------------------------------------------------------------
//
MulDataWindow::~MulDataWindow()
	{
	// do nothing
	}

// ---------------------------------------------------------------------------
// SetHighlight
// ---------------------------------------------------------------------------
//
void MulDataWindow::SetHighlight( int aHighlightIndex )
	{
	MUL_LOG_INFO2("MUL::MulDataWindow::SetHighlight() mHighlight:%d,modelCount:%d",aHighlightIndex,mMulModel.CurrentItemCount()); 
	
	__MUL_ASSERT( aHighlightIndex >= 0 && aHighlightIndex <= mMulModel.CurrentItemCount()-1, KLInvalidArgument );
	
	if( mHighlight != aHighlightIndex )
		{
		mHighlight = aHighlightIndex; 
	
		ScrollWindow( mHighlight );
		}
	//else same highlight
	}


// ---------------------------------------------------------------------------
// UpdateHighlight
// ---------------------------------------------------------------------------
//
void MulDataWindow::UpdateHighlight( int aHighlightIndex )
	{
	// No need to assert. We should send highlight -1 when all the items are removed.
	// Widget should handle. to show emty text visualization
	mHighlight = aHighlightIndex; 
	}
		
// ---------------------------------------------------------------------------
// ScrollWindow
// ---------------------------------------------------------------------------
//
void MulDataWindow::ScrollWindow( int aItemIndex )
	{
	MUL_LOG_INFO2("MUL::MulDataWindow::ScrollWindow() aItemIndex:%d,modelCount:%d",aItemIndex,mMulModel.CurrentItemCount()); 
	
	__MUL_ASSERT( aItemIndex >= 0 && aItemIndex <= mMulModel.CurrentItemCount()-1, KLInvalidArgument );
	
	if( mItemIndex != aItemIndex )
		{
		mOldItemIndex = mItemIndex;
		mItemIndex = aItemIndex; 
	
        if(IsWindowEnabled())
	        {
		    UpdateDataWindow();
	        }
		}
	//else same highlight
	}


// ---------------------------------------------------------------------------
// UpdateDataWindow
// ---------------------------------------------------------------------------
//
void MulDataWindow::UpdateDataWindow()
	{
#ifdef _DEBUG
	int diff = mItemIndex - mOldItemIndex;
#endif //_DEBUG 
	
	//If diffrence is negative than highlight is moved up else down
	(( mItemIndex - mOldItemIndex ) < 0 ) ? ShiftWindowUp() : ShiftWindowDown() ;
	}

// ---------------------------------------------------------------------------
// ShiftWindowDown
// ---------------------------------------------------------------------------
//
void MulDataWindow::ShiftWindowDown()
	{
	MUL_LOG_ENTRY_EXIT("MUL::MulDataWindow::ShiftWindowDown()");  
	MUL_LOG_INFO2("MUL::MulDataWindow::ShiftWindowDown() mItemIndex:%d,mWindowBottom:%d",mItemIndex,mWindowBottom); 
	
	if( mItemIndex > mWindowBottom )
		{
		mWindowTop = mItemIndex - mWindowOffset;
		mWindowBottom = mWindowTop + mWindowSize -1;
		//bottom is exceeding model count
		mWindowBottom = mWindowBottom >= mMulModel.CurrentItemCount() ? 
						mWindowBottom = mMulModel.CurrentItemCount() - 1 : mWindowBottom;
		
		mWindowTop = mWindowBottom - mWindowSize + 1;
		//top can't be negative
		mWindowTop = mWindowTop < 0 ? 0 : mWindowTop ;
		
		MUL_LOG_INFO2("MUL::MulDataWindow::ShiftWindowDown() mWindowTop:%d,mWindowBottom:%d",mWindowTop,mWindowBottom); 
		SaveOldValues();
		AdjustBuffer();
		mMulModel.DataWindowUpdated();
		}
	//else nothing needed
	}

// ---------------------------------------------------------------------------
// ShiftWindowUp
// ---------------------------------------------------------------------------
//
void MulDataWindow::ShiftWindowUp()
	{
	MUL_LOG_ENTRY_EXIT("MUL::MulDataWindow::ShiftWindowUp() Start");  
	MUL_LOG_INFO2("MUL::MulDataWindow::ShiftWindowUp() mItemIndex:%d,mWindowBottom:%d",mItemIndex,mWindowBottom); 
	
	if( mItemIndex < mWindowTop )
		{
		mWindowTop = mItemIndex - mWindowOffset;
		//top can't be negative
		mWindowTop = mWindowTop < 0 ? 0 : mWindowTop ;
		
		mWindowBottom = mWindowTop + mWindowSize -1;
		
		//bottom cant exceed model count
		mWindowBottom = mWindowBottom >= mMulModel.CurrentItemCount() ? 
						mWindowBottom = mMulModel.CurrentItemCount() - 1 : mWindowBottom;
		
		MUL_LOG_INFO2("MUL::MulDataWindow::ShiftWindowUp() mWindowTop:%d,mWindowBottom:%d",mWindowTop,mWindowBottom); 
		
		SaveOldValues();
		AdjustBuffer();
		mMulModel.DataWindowUpdated();
		}
	//else nothing needed
	}

// ---------------------------------------------------------------------------
// AdjustBuffer
// ---------------------------------------------------------------------------
//
void MulDataWindow::AdjustBuffer()
	{
	MUL_LOG_ENTRY_EXIT("MUL::MulDataWindow::AdjustBuffer()");  
	
	int modelCount = mMulModel.CurrentItemCount() -1;
		
	//remove data is new window is smaller/add is new window is bigger
	if( mWindowTop == 0 && ActualBufferSize() <= modelCount )
		{
		MUL_LOG_INFO("MUL::MulDataWindow::AdjustBuffer() mWindowTop == 0 ");  		
		
		mBufferTop = mWindowTop;
		mBufferBottom = mBufferTop + mWindowSize + mBufferSize - 1;
		
		//Create rear top and rear bottom only in looping case
		//window top is zero so data at other end should be buffered
		mRearBufferBottom = modelCount;
		mRearBufferTop = mRearBufferBottom - mBufferSize +1;		
		}
	else if( mWindowBottom == modelCount && ActualBufferSize() <= modelCount )
		{
		MUL_LOG_INFO("MUL::MulDataWindow::AdjustBuffer() mWindowBottom == modelCount ");  
		
		mBufferBottom = mWindowBottom ;
		mBufferTop = mBufferBottom - (mWindowSize + mBufferSize) + 1;
		
		//Create rear top and rear bottom only in looping case
		//window bottom is equal to model count so data at other end should be buffered
		mRearBufferTop = 0;
		mRearBufferBottom = mRearBufferTop + mBufferSize -1;
		}
	else
		{
		MUL_LOG_INFO("MUL::MulDataWindow::AdjustBuffer() else ");  
		
		mBufferTop = mWindowTop - mBufferSize;
		mBufferBottom = mWindowBottom + mBufferSize;
		
		//check if top or bottom is out of bound and update offset accordingly
		if( mBufferTop < 0 )
			{
			MUL_LOG_INFO("MUL::MulDataWindow::AdjustBuffer() mBufferTop < 0 "); 
			
			mBufferTop = 0;
			mBufferBottom = mBufferTop + ActualBufferSize()  - 1;
			//buffer bottom cant be larger then model count
			mBufferBottom = mBufferBottom > modelCount ? modelCount : mBufferBottom;
			}
		else if( mBufferBottom > modelCount )
			{
			MUL_LOG_INFO("MUL::MulDataWindow::AdjustBuffer() mBufferBottom > modelCount"); 
			
			mBufferBottom = modelCount ;
			mBufferTop = mBufferBottom - ActualBufferSize() + 1;
			//buffer top cant be less then 0
			mBufferTop = mBufferTop < 0 ? 0 : mBufferTop;
			}
		
		//in other case rear top and rear bottom is not use set to -1
		mRearBufferTop = KNotInitialized;
		mRearBufferBottom = KNotInitialized;
		}
	
	MUL_LOG_INFO2("MUL::MulDataWindow::AdjustBuffer() mOldBufferTop:%d,mOldBufferBottom:%d",mOldBufferTop,mOldBufferBottom); 
	MUL_LOG_INFO2("MUL::MulDataWindow::AdjustBuffer() mOldRearBufferBottom:%d,mOldRearBufferTop:%d",mOldRearBufferBottom,mOldRearBufferTop); 
	
	MUL_LOG_INFO2("MUL::MulDataWindow::AdjustBuffer() mBufferTop:%d,mBufferBottom:%d",mBufferTop,mBufferBottom); 
	MUL_LOG_INFO2("MUL::MulDataWindow::AdjustBuffer() mRearBufferBottom:%d,mRearBufferTop:%d",mRearBufferBottom,mRearBufferTop); 

	__MUL_ASSERT_DEBUG( mWindowTop >= 0 && mWindowBottom < mMulModel.CurrentItemCount(), _L("Invlid Window"));
	__MUL_ASSERT_DEBUG( mBufferTop >= 0 && mBufferBottom < mMulModel.CurrentItemCount(), _L("Invlid Buffer"));
	}
		
// ---------------------------------------------------------------------------
// IsItemInDataWindow
// ---------------------------------------------------------------------------
//
bool MulDataWindow::IsItemInDataWindow(int aItemIndex ) const
	{
	//__MUL_ASSERT_DEBUG( aItemIndex >= 0 && aItemIndex <= mMulModel.CurrentItemCount()-1, KLInvalidArgument );
	
	bool result( false );
	
	if(mWindowSize == KNotInitialized )
		{
		result = false;
		}
	//check that rear buffer is on or not in looping case
	else if( mRearBufferTop != KNotInitialized && mRearBufferBottom != KNotInitialized )
		{
		if( ( aItemIndex >= mBufferTop &&  aItemIndex <= mBufferBottom ) || 
			( aItemIndex >= mRearBufferTop &&  aItemIndex <= mRearBufferBottom )	)
			{
			result = true;
			}
		}
	else if( aItemIndex >= mBufferTop &&  aItemIndex <= mBufferBottom )
		{
		result = true;
		}
	return result;
	}
	

// ---------------------------------------------------------------------------
// RelativeIndex
// ---------------------------------------------------------------------------
//
int MulDataWindow::RelativeIndex( int aAbsoluteIndex ) const
	{
	__MUL_ASSERT_DEBUG( aAbsoluteIndex >= 0 && aAbsoluteIndex <= mMulModel.CurrentItemCount()-1, KLInvalidArgument );
	
	if( !IsItemInDataWindow(aAbsoluteIndex) )
	    {
	    return -1;
	    }
	
	if( mRearBufferTop != KNotInitialized && mRearBufferBottom != KNotInitialized )
		{
		if( mRearBufferBottom == mMulModel.CurrentItemCount() - 1 )
			{
			if( aAbsoluteIndex >= mRearBufferTop && aAbsoluteIndex <= mRearBufferBottom )	
				{
				int bufferSize = BottomOffset() - TopOffset() + 1;
				int rearDiff = aAbsoluteIndex - mRearBufferTop;
				int relativeIndex = bufferSize + rearDiff;
				return relativeIndex;
				}
			else
				{
				int relativeIndex = aAbsoluteIndex < TopOffset() ? 
									aAbsoluteIndex : aAbsoluteIndex - TopOffset() ;	
				return relativeIndex;	
				}		
			}
		else
			{
			if( aAbsoluteIndex >= mRearBufferTop && aAbsoluteIndex <= mRearBufferBottom )
				{
				int relativeIndex = aAbsoluteIndex < TopOffset() ? 
									aAbsoluteIndex : aAbsoluteIndex - TopOffset() ;	
				return relativeIndex;	
				}
			else
				{	
				int bufferSize = mRearBufferBottom - mRearBufferTop + 1;
				int diff = aAbsoluteIndex - TopOffset();
				int relativeIndex = bufferSize + diff;
				return relativeIndex;		
				}
			}
		}
	else
		{		
		int relativeIndex = aAbsoluteIndex < TopOffset() ? 
							aAbsoluteIndex : aAbsoluteIndex - TopOffset() ;	
		return relativeIndex;
		}
	}

// ---------------------------------------------------------------------------
// AbsoluteIndex
// ---------------------------------------------------------------------------
//
int MulDataWindow::AbsoluteIndex( int aRelativeIndex ) const
	{
	__MUL_ASSERT_DEBUG( aRelativeIndex >= 0 && aRelativeIndex <= ActualBufferSize(), _L("Invalid Relative Index"));
	
	if( mRearBufferTop != KNotInitialized && mRearBufferBottom != KNotInitialized )
		{
		if( mRearBufferBottom == mMulModel.CurrentItemCount() - 1 )
			{
			if( aRelativeIndex > BottomOffset() )
				{
				//relative index is in loop buffer
				int diff = aRelativeIndex - BottomOffset() - 1;
				int absoluteIndex = RearTopOffset() + diff;
				return absoluteIndex;
				}
			else
				{
				int absoluteIndex = TopOffset() + aRelativeIndex;
				return absoluteIndex;
				}
			}
		else
			{
			if( aRelativeIndex <= RearBottomOffset() )
				{
				//relative index is in loop buffer
				int absoluteIndex = RearTopOffset() + aRelativeIndex;
				return absoluteIndex;
				}
			else
				{
				int diff = aRelativeIndex - RearBottomOffset() - 1;
				int absoluteIndex = TopOffset() + diff;
				return absoluteIndex;
				}
			}
		}
	else
		{		
		int absoluteIndex = TopOffset() + aRelativeIndex;
		return absoluteIndex;
		}
	}

// ---------------------------------------------------------------------------
// SetVisibleWindow
// ---------------------------------------------------------------------------
//
void MulDataWindow::SetVisibleWindow(int aWindowTop, int aWindowBottom)
	{	
	mWindowTop = aWindowTop;
	mWindowBottom = aWindowBottom;
	}

// ---------------------------------------------------------------------------
// IsBufferOffsetChanged
// ---------------------------------------------------------------------------
//
bool MulDataWindow::IsBufferOffsetChanged()
	{
	if(mRearBufferBottom != mOldRearBufferBottom || mRearBufferTop != mOldRearBufferTop
		|| mBufferTop != mOldBufferTop || mBufferBottom != mOldBufferBottom)
		{
		return true;
		}
	return false;
	}
			
    } // namespace Alf

//End of file
