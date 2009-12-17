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
* Description:  Page by Page Data Window Implemenation
*
*/


#ifndef MULPAGEDATAWINDOW_H_
#define MULPAGEDATAWINDOW_H_

#include "mulmodeldef.h"

namespace Alf
	{
	
//Forward Declaration
class MulModelImpl;

/**
 * Implametation of Data Window Class
 * This class maintain offset for buffer item that are required by widget,
 * so only that much item remain present in alf model instance.It update buffer items
 * when highlight changed and notify model to update alf model if items are not present
 * in Alf model.
 * 
 */
class MulPageDataWindow
	{
public: //Constructor and Destructor
	
	/**
	 * C++ constructor
	 * 
	 * @param aMulModel refrece of model implementation
	 */
	MulPageDataWindow( MulModelImpl& aMulModel );

	/**
	 * Descturctor
	 */
	~MulPageDataWindow();
	
public: //New Method

	/**
	 * Set window size
	 * 
	 * @param aWindowSize No of items that are visible on screen
	 */
	void SetWindowSize( int aWindowSize, int aPageTopIndex = KNotInitialized );
	
	/**
	 * Set Buffer size
	 * Here buffser size mean number of items that will be above and below window
	 * By default buffer size is equal to window size if following api is not called.
	 *
	 * @param aBufferSize Size of buffer
	 */
	void SetBufferSize( int aBufferSize );
	
	/**
	 * Change highlighted index. 
	 * Changing highlight will check that data window needs to be updated or not 
	 * and if required it update data window
	 *
	 * @aHighlightIndex New highlighted item in Data window
	 * @throws invaid_argument exception
	 */
	void SetHighlight( int aHighlightIndex );
	
	/**
	 * Scroll Window to mention item index 
	 * it will check that data window needs to be updated or not 
	 * and if required it update data window
	 *
	 * @aItemIndex item index
	 * @throws invaid_argument exception
	 */
	void ScrollWindow( int aPageTopIndex );
	
	/**
	 * Return Highlighted item's index
	 * 
	 * @return Highlighted item's index
	 */
	int Highlight() const
		{
		return mHighlight;
		}
	
	/**
	 * Return previous top offset
	 * 
	 * @return Previos top offset
	 */
	int OldTopOffset() const
		{
		return mOldBufferTop;
		}
	
	/**
	 * Return previous bottom offset
	 * 
	 * @return Previos bottom offset
	 */
	int OldBottomOffset() const
		{
		return mOldBufferBottom;
		}
	
	/**
	 * Return current top offset
	 * 
	 * @return Current top offset
	 */
	int TopOffset() const
		{
		return mBufferTop;
		}
	
	/**
	 * Return current bottom offset
	 * 
	 * @return Current bottom offset
	 */
	int BottomOffset() const
		{
		return mBufferBottom;
		}
	
	/**
	 * Return visible window top offset
	 * 
	 * @return Current window top offset
	 */
	int TopWindowOffset() const
		{
		return mWindowTop;
		}
	
	/**
	 * Return visible window bottom offset
	 * 
	 *  @return Current window bottom offset
	 */
	int BottomWindowOffset() const
		{
		return mWindowBottom;
		}
		
	/**
	 * Return the Old Rear Top offset
	 * 
	 * @return Old Rear Top offset
	 */
	int OldRearTopOffset() const
		{
		return mOldRearBufferTop;
		}
	
	/**
	 * Return the Old Rear Bottom offset
	 * 
	 * @return Old Rear Bottom offset
	 */
	int OldRearBottomOffset() const
		{
		return mOldRearBufferBottom;
		}		
	
	/**
	 * Return the Rear Top offset
	 * 
	 * @return Rear Top offset
	 */
	int RearTopOffset() const
		{
		return mRearBufferTop;
		}
	
	/**
	 * Return the Rear Bottom offset
	 * 
	 * @return Rear Bottom offset
	 */
	int RearBottomOffset() const
		{
		return mRearBufferBottom;
		}
	
	/**
	 * Return buffer size
	 * 
	 * @return Current buffer size
	 */
	int BufferSize() const
		{
		return mBufferSize;
		}
		
	/**
	 * Return window size
	 * 
	 * @return Current window size
	 */
	int WindowSize() const
		{
		return mWindowSize;
		}		
	
	/**
	 * Check that items falls in data window or not
	 * 
	 * @param aItemIndex index of item
	 * @return true if item is in data window otherwise false
	 */
	bool IsItemInDataWindow(int aItemIndex ) const;
	
	
	/**
     * Check that items falls in visible window or not
     * 
     * @param aItemIndex index of item
     * @return true if item is in visible window otherwise false
     */
	bool IsItemInVisibleWindow(int aItemIndex ) const;
		
	/**
	 * Returns Relative index in datawindow from Absolute index
	 *
	 * @param aAbsoluteIndex Absolute index
	 * @return Relative index
	 */
	int RelativeIndex( int aAbsoluteIndex ) const;
	
	/**
	 * Returns Absoulte index from relative index
	 *
	 * @param aRelativeIndex Relative index
	 * @return Absolute index
	 */
	int AbsoluteIndex( int aRelativeIndex ) const;
	
	/**
	 * Check that valid window size it set or not.
	 * 
	 * @return true - valid window size is set 
	 * 		   false - otherwisze
	 */
	bool IsWindowEnabled() const
		{
		return ( mWindowSize != KNotInitialized );
		}

	/**
	 * Adjust buffer offset according to window offset
	 */
	void AdjustBuffer();
	
	/**
	 * Return the complete buffer size, i.e. both top, bottom buffer and the window
	 */	
	int ActualBufferSize() const
		{
		return (2 * mBufferSize) + mWindowSize;	
		}
		
	/**
	 * Change highlighted index. 
	 * This change will not check that data window needs to be updated or not 
	 *
	 * @aHighlightIndex New highlighted item in Data window
	 */
	void UpdateHighlight( int aHighlightIndex );
	
	/**
	 * Adjust Window and buffer offset after insert or remove
	 * 
	 */
	void AdjustOffset();
		
	/**
	 * Save old values
	 */
	void SaveOldValues()
		{
		mOldBufferTop = mBufferTop;
		mOldBufferBottom = mBufferBottom;
		
		mOldRearBufferTop = mRearBufferTop;
		mOldRearBufferBottom = mRearBufferBottom;
		}
		
	/**
	 * Compares the current and old buffer values and returns true if teh values are cahnged
	 */
	bool IsBufferOffsetChanged();					

private://New Method
	
	/**
	 * Check that visible window need to be updated or not.
	 * If required it update visible window.
	 */
	void UpdateDataWindow();
	
	/**
	 * Data window is updated and window need to sifted down
	 */
	//void ShiftWindowDown();
	
	/**
	 * Data window is updated and window need to sifted up
	 */
	//void ShiftWindowUp();
			
private: // Data

	MulModelImpl& mMulModel; //not own
	
	int mHighlight;	
	
	int mPageTopIndex;
	int mOldPageTopIndex;
	
	int mBufferSize;
	
	int mWindowSize;
	
	int mWindowTop;
	int mWindowBottom;
	
	int mBufferTop;
	int mBufferBottom;
	
	int mOldBufferTop;
	int mOldBufferBottom;
		
	int mRearBufferTop;
	int mRearBufferBottom;
	
	int mOldRearBufferTop;
	int mOldRearBufferBottom;
	
	};

	} //namespace Alf

#endif //MULPAGEDATAWINDOW_H_

//End of file

