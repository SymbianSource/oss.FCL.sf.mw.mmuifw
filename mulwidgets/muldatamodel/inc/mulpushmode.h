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
* Description:  Implementation of model interface base
 *
*/


#ifndef MULPUSHMODE_H
#define MULPUSHMODE_H

//Includes
#include <osn/ustring.h>

#include <mul/muldatapath.h>

#include "mul/imulwidget.h"


namespace Alf
    {

//Forward Declaration

class MulVisualItem;
class MulCacheManager;
class MulPageDataWindow;
class MulAlfModelImpl;
	

/**
 *  Base implementation class for pull/push model.
 *  @lib 
 */
class MulPushMode
    {
    
public: 

	/**
	 * C++ constructor.
	 * 
	 * @param aDataWindow DataWindow for model
	 */
    MulPushMode( MulPageDataWindow& aDataWindow );
	
	/**
	 * Destructor
	 */
	virtual ~MulPushMode();
   	
	/**
	 * Informs the data model that new items have been added to client's
	 * internal model. Data model is expected to sync its indices as a 
	 * result.
	 * 
	 * @param aIndex Position where items were inserted.
	 * @param aCount Number of items inserted.
     * @return true if highlight is modified else false.	 
	 */ 
	bool Insert( int aIndex, int aCount, 
                         const MulDataPath& aPath = MulDataPath() );
	
    /**
     * Informs the data model that items have been removed from the client's.
     * internal data model.Data model is expected to sync its indices as a 
     * result.
     * 
     * @param aIndex Position from where items have to be removed.
     * @param aCount Number of items to be removed.
     * @return true if highlight is modified else false.
     */ 
    bool Remove( int aIndex, int aCount, 
                         const MulDataPath& aPath = MulDataPath() );
	
    /**
     * Informs the data model that an item has been updated in the client's
     * internal model. Data model will use to input to determine whether
     * a visualization change is required.
     * 
     * @param aIndex Position from where items have to be updated.
     * @param aCount Number of items to be updated.
     */ 
	virtual void Update( int aIndex, int aCount, 
                         const MulDataPath& aPath = MulDataPath() );

 
    /**
      * A refresh data request. Informs that the data model has to be 
      * repopulated with mentioned number of items. So the data model
      * will remove all the existing data and issue request to the 
      * provider for new data.
      * 
      * @param aCount Number of items to be repopulated.
      * @param aPath  Path of parent item in the data hierarchy. 
      *               Default is Null path i.e. no parent.
      */ 						 
	virtual void Refresh( int aCount, const MulDataPath& aPath = MulDataPath());						 
	
	/**
     * Provide data for the specified item. This is a unified API for 
     * adding & updating data. 
     *
     * @param aIndex Position of item to be updated.
     * @param aVisualItem Collection of attributes. Ownership is transfered.
     * @throw Invalid Argument, in case index is out of bound or visual 
     *                         item is NULL.
     * @throw Logic Error, in case item could not be updated.
     */ 
	virtual void SetData( int aIndex, 
						  std::auto_ptr<MulVisualItem> aVisualItem, 
                          const MulDataPath& aPath);
	
	 /**
	  * Get the data for item at the specified index.
	  *
	  * @param aIndex Position of item in the data model.
	  * @return A constant reference to the data at the given index.
	  */ 
	const MulVisualItem* Data( int aIndex, const MulDataPath& aPath ) const;

	 /**
	  * Get the count of items in the model.
	  *
	  * @return Count of items in the model.
	  */ 
	int Count(const MulDataPath& aPath ) const; 

	/**
     * Set the default logical template id for widget elements. 
     * Logical templates map to visualization of the element(s).
     * 
     * @param aTemplateId Logical template id for the widget elements.
     */ 
	virtual void SetTemplate( mulwidget::TLogicalTemplate aTemplateId ) ;
	
    /**
     * Returns the default template. 
     * 
     * @return Logical template id for the widget elements.
     */ 
	mulwidget::TLogicalTemplate Template( ) ;

	/**
     * Set the logical template id for a specific widget element. 
     * Logical templates map to visualization of the element.
     * 
     * @param aItemIndex Unique id of item of which template needs to be applied.
     * @param aTemplateId Logical template id for the widget element.
     * @throw Invalid Argument, in case index is out of bound.
     */ 
	void SetTemplate( int aItemIndex, mulwidget::TLogicalTemplate aTemplateId, 
	                    	  const MulDataPath& aPath );
	 
	/**
     * Returns the total number of items in model.
     */ 
	int CurrentItemCount() const;	                    	
	    
	/**
	 * When ever data window update data window, it will use this method to notify
	 * Data model to update data window and update alf model
	 */
	virtual void DataWindowUpdated(){ };
		
	/**
	 * Return cachemanager 
	 * 
	 * @return cachae manager
	 */
	MulCacheManager& CacheManager() const;
	
	/**
	 * Get Visualte item from absolute index
	 * 
	 * @param aAbsoluteIndex Absolute index
	 * @return VisualItem at index
	 */
	virtual const MulVisualItem& Item( int aAbsoluteIndex );
	
	/**
	 * Check that visual item has Templatte or not
	 * If visualte item is not poper exception will be throws
	 * 
	 * @param aVisualItem Visual item to be tested
	 */
	void ValidateVisualItem( MulVisualItem& aVisualItem );
	
	/**
	 * Modify the data alfmodel to handle remove, insert, or expand/colapse operation
	 * 
	 * @param, aOldCount Total number of items in Alf model before the current operation
	 */	
	virtual void ModelCountChanged(int aOldCount, int aNewCount);		
		
private:
	
	/**
	 * If current highlight is greater than the number of items in model then update the highlight.
	 *
	 * @return Ture if highlight value is modified else false
	 */		
	bool ValidateCurrentHighlight();
		
protected: // data members	
	
	std::auto_ptr<MulCacheManager>	mCacheManager; //own
	MulPageDataWindow& mDataWindow;	// Not own
	//UString	 mDefaultTemplate; //default template for data	
	mulwidget::TLogicalTemplate mDefaultTemplate;

    };
    
    } // namespace Alf

#endif // MULPUSHMODE_H
    
//End of file
    
