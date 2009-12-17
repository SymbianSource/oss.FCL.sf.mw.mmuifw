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
* Description:  Interface for model accessor
 *
*/


#ifndef I_MULMODELACCESSOR_H
#define I_MULMODELACCESSOR_H

//Includes
#include <alf/ialfmodel.h>
#include <mul/imulwidgetdef.h>
#include <mul/imulwidget.h>

#include <vector>

namespace osncore
	{
	class UString;
	}
using namespace osncore;

namespace Alf
	{

	/* Forward declarations*/
	class MulVisualItem;
	class IMulModelObserver;

	/**
	 *  An interface for accessing data model. Widgets use this interface to interact with Model.
	 *  Data model implements this interface.
	 *  @lib 
	 */
	class IMulModelAccessor : public IAlfModel
		{
	public:

	/**
	 * The function to set a particular item as highlighted.
	 * 
	 * @param aIndex Index of the item to be highlighted.
	 * @throws Invalid_argument Throws this exception if index is not a valid index.
	 */
	virtual void SetHighlight( int aItemIndex ) = 0;
	
	/**
	 * Scroll Window to mention item index 
	 * it will check that data window needs to be updated or not 
	 * and if required it update data window
	 *
	 * @aItemIndex item index
	 * @throws invaid_argument exception
	 */
	virtual void ScrollWindow( int aItemIndex ) = 0;

	/**
	 * Function to get the highlighted item index.
	 * 
	 * @return Index of the highlighted index.
	 */
	virtual int Highlight() = 0;
          
    /**
     * Function to get the count of items currently present in the model. 
     * This includes hidden items.
     * 
     * @return Count of items existing in the model.
     */ 
     virtual int CurrentItemCount() = 0;
     
    /**
     * Set visible data window for model.
     * 
     * @param aVisibleWindow No of visuals that widget can display at a time on screen.
     */ 
     virtual void SetVisibleWindow( int aWindowSize, int aPageTopIndex = -1 ) = 0;
     	 
     /**
 	 * Returns relative index from absolute index.
 	 * 
 	 * @param aAbsoluteIndex Abosulte index
 	 * @return Returns relative index from absolute index.
 	 */
	 virtual int RelativeIndex( int aAbsoluteIndex ) const = 0;
	 	 
	/**
	 * Return visible window top offset
	 * 
	 * @return Current window top offset
	 */
	 virtual int TopWindowOffset() const= 0;
	
	/**
	 * Return visible window bottom offset
	 * 
	 *  @return Current window bottom offset
	 */
	 virtual int BottomWindowOffset() const = 0;
	 	 
	/**
	 * Return visible window top offset
	 * 
	 * @return Current window top offset
	 */
	 virtual int TopOffset() const= 0;
	
	/**
	 * Return visible window bottom offset
	 * 
	 *  @return Current window bottom offset
	 */
	 virtual int BottomOffset() const = 0;

	/**
	 * Return rear buffer top offset
	 * 
	 * @return Current window top offset
	 */	
	virtual int RearTopOffset() const = 0;

	/**
	 * Return rear buffer bottom offset
	 * 
	 * @return Current window top offset
	 */	
	virtual int RearBottomOffset() const = 0;
		 	 
	 /**
	  * Check weather item at specified index is marked or not.
	  * 
	  * @param aIndex Absolute index of item
	  * @return true if seleccted else otherwise
	  */
	 virtual bool IsItemMarked( int aIndex ) = 0; 
	   
    /**
	 * Function for setting the item Marked or Unmarked. For mark and
	 * unmark operations, index of item should be provided.
	 * 
	 * @param aType Type of marking operation to be performed. 
	 * @param aItemsIndex Indices of the items to be changed
	 * @throw Invalid Argument, if type is specified as mark-all and unmark-all. 
	 */ 
	 virtual void SetMarkedIndex( MulWidgetDef::TMulWidgetMarkingType aType, int aIndex ) = 0;
	 	 
	 /**
	  * Returns the visual item at the specified absolute index
	  * 
	  * @param aIndex Absolute index of item
	  */
	 virtual const MulVisualItem& Item( int aAbsoluteIndex ) = 0;
	 
	/**
	 * Returns Absolute index from relative index
	 *
	 * @param aRelativeIndex Relative index
	 * @return Absolute index
	 */
	 virtual int AbsoluteIndex( int aRelativeIndex ) = 0;
	 
	 /**
	  * Return whather currently looping is on or not
	  * 
	  * @return true if looping is on false otherwise
	  */
	 virtual bool IsLoopingOn() = 0;
	 	 
	 /**
	  * Add model state change observer.
	  * 
	  * @param aObserver Observer- ownserhip is not transferred
	  */
	 virtual void AddModelObserver( IMulModelObserver* aObserver ) = 0;
	 
	 /**
      * Eemove model state change observer.
      * 
      * @param aObserver Observer- ownserhip is not transferred
      */
	 virtual void RemoveModelObserver( IMulModelObserver* aObserver )= 0;
	 
	 /**
      * Return default template
      * 
      * @return Default template
      */
    virtual mulwidget::TLogicalTemplate Template( ) = 0;
	 
    };


    } // namespace Alf

#endif // I_MULMODELACCESSOR_H
