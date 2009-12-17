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
* Description:  Interface for model
*
*/


#ifndef I_MULMODEL_H
#define I_MULMODEL_H

//Includes
#include <alf/ialfinterfacebase.h>
#include <alf/ialfmodel.h>
#include <alf/alftypes.h>
#include <mul/imulmodelbase.h>
#include <mul/imulwidgetdef.h>
#include <mul/imulwidget.h>
#include <mul/muldatapath.h>
#include <vector>
#include <memory>

namespace osncore
    {
    class UString;
    }
using namespace osncore;

namespace Alf
    {
    
namespace mulmodel
    {
    static const IfId Ident=
        {
        0, "mulmodel"
        };
   
    static const char* const KBaseElementName = "base" ;
    static const char* const KItemElementName = "item" ;
    }

// Forward Declarations
class MulVisualItem;
class MulDataPath;
class IMulModelProvider;

/*! @class IMulModel
*
*  @brief An interface for the data model.   
*
*  Client that creates the model own the model.
*  Client can use this interface to update data model.
*
*/
class IMulModel : public IMulModelBase, public IAlfModel
    {

public:

	/**
	 * Getter for the type identifier of this interface.
	 *
	 * @return A descriptor to identify the type of this interface.
	 */
	static inline const IfId& type()
	    {
	    return mulmodel::Ident;    
	    }

	/** 
	 * Virtual destructor.
	 */ 
 	virtual ~IMulModel() {}
    
    /**
     * Informs the data model that new items have been added to client's
     * internal model. Data model is expected to sync its indices as a 
     * result.
     * <b>Note:</b> This API will never give an exception.
     * 
     * @param aIndex Position where items were inserted.
     * @param aCount Number of items inserted.
     * @param aPath  Path of parent item in the data hierarchy. 
     *               Default is Null path i.e. no parent.
     */ 
    virtual void Insert( int aIndex, int aCount, 
                         const MulDataPath& aPath = MulDataPath() ) = 0 ;
        
     /**
      * Informs the data model that items have been removed from the client's.
      * internal data model. Data model is expected to sync its indices as a 
      * result.
      * <b>Note:</b> This API will never give an exception.
      * 
      * @param aIndex Position from where items have to be removed.
      * @param aCount Number of items to be removed.\
      * @param aPath  Path of parent item in the data hierarchy. 
      *               Default is Null path i.e. no parent.
      */ 
     virtual void Remove( int aIndex, int aCount, 
    		 			  const MulDataPath& aPath = MulDataPath() ) = 0;
     
     /**
      * Informs the data model that an item has been updated in the client's
      * internal model. Data model will use to input to determine whether
      * a visualization change is required. If yes, the model will request the
      * provider for updated data.
      * <b>Note:</b> This API will never give an exception.
      * 
      * @param aIndex Position from where items have to be updated.
      * @param aCount Number of items to be updated.
      * @param aPath  Path of parent item in the data hierarchy. 
      *               Default is Null path i.e. no parent.
      */ 
     virtual void Update( int aIndex, int aCount, 
                          const MulDataPath& aPath = MulDataPath() ) = 0;

    /**
      * A refresh data request. Informs that the data model has to be 
      * repopulated with mentioned number of items. So the data model
      * will remove all the existing data and issue request to the 
      * provider for new data.
      * <b>Note:</b> This API will never give an exception.
      * 
      * @param aCount Number of items to be repopulated.
      * @param aPath  Path of parent item in the data hierarchy. 
      *               Default is Null path i.e. no parent.
      */ 
     virtual void Refresh( int aCount, const MulDataPath& aPath = MulDataPath() ) = 0;
          
     /**
      * Provide data for the specified item. This is a unified API for 
      * adding & updating data. 
      *
      * @param aIndex Position of item to be updated.
      * @param aVisualItem Collection of attributes. Ownership is transfered.
      * @param aPath  Path of parent item in the data hierarchy. 
      *               Default is Null path i.e. no parent.
      * @throw Invalid Argument, in case index is out of bound or visual 
      *                         item is NULL.
      * @throw Logic Error, in case item could not be updated.
      */ 
     virtual void SetData( int aIndex, std::auto_ptr<MulVisualItem> aVisualItem, 
                            const MulDataPath& aPath = MulDataPath() ) = 0;
     
     /**
      * Get the data for item at the specified index.
      *
      * @param aIndex Position of item in the data model.
      * @param aPath  Path of parent item in the data hierarchy. 
      *               Default is Null path i.e. no parent.
      * @return A constant reference to the data at the given index.
      * @throw Invalid Argument, in case index is out of bound.
      */ 
     virtual const MulVisualItem* Data( int aIndex, 
                                		const MulDataPath& aPath = MulDataPath() ) const = 0;
     
    /**
      * Get the count of items in the model.
      *
      * @param aPath  Path of parent item in the data hierarchy. 
      *               Default is Null path i.e. no parent.
      * @return Count of items in the model.
      */ 
     virtual int Count( const MulDataPath& aPath = MulDataPath() ) const = 0; 
          
    /**
     * Set the data provider for the model. 
     * <b>Note:</b> This API will be removed in future. 
     * Clients would be able to specify the provider to the model at 
     * the time of the construction.
     * 
     * @param aDataProvider Handle to the data provider.
     * @throw Logic Error, in case same accessor object is passed again.
     */ 
     virtual void SetDataProvider( const IMulModelProvider& aDataProvider ) = 0;
     
    /**
     * Set the default logical template id for widget elements. 
     * Logical templates map to visualization of the element(s).
     * 
     * @param aTemplateId Logical template id for the widget elements.
     */ 
     virtual void SetTemplate( mulwidget::TLogicalTemplate aDefaultTemplate ) = 0;
     
     /**
      * Set the logical template id for a specific widget element. 
      * Logical templates map to visualization of the element.
      * 
      * @param aItemIndex Unique id of item of which template needs to be applied.
      * @param aTemplateId Logical template id for the widget element.
      * @param aPath  Path of parent item in the data hierarchy. 
      *               Default is Null path i.e. no parent.
      * @throw Invalid Argument, in case index is out of bound.
      */ 
     virtual void SetTemplate( int aItemIndex, mulwidget::TLogicalTemplate aDefaultTemplate ,
                                const MulDataPath& aPath = MulDataPath() )=0;
	 
    /**
     * Set the data window/cache size of the Data model.
     * This API can be used to over-ride the data window size of the data model
     * already set by the widget. This can be used by clients to set the buffer
     * size different from what the widget has specified. 
     * 
     * @param aSize Size of buffer.
     * @throw Invalid Argument, incase the buffer size is negative.
     */ 
    virtual void SetDataWindow( int aSize ) = 0;
    
    /**
     * Get the buffer size defined for the data model.
     * 
     * @return Size of data window.
     */ 
    virtual int DataWindow( ) const = 0;        
    
     /**
	  * Set highlighted item in the model.
      * 
	  * @param aItemIndex Index of item.
	  * @param aPath  Path of parent item in the data hierarchy. 
      *               Default is Null path i.e. no parent.
	  *
	  * <B>Examples:</b>
	  * @code
	  * Set index of the item to be highlighted.
	  *
	  *         mListWidget->SetHighlight(highlight);
	  * @endcode
      */ 
	virtual void SetHighlight( int aItemIndex, const MulDataPath& aPath = MulDataPath() ) = 0; 

    /**
     * Get the index of the current highlighted item from the model.
     * 
     * @return Highlighted item index.
     *
	 * <B>Examples:</b>
	 * @code
	 * Get index of the highlighted item.
	 *
	 *      int highlight = mListWidget->Highlight();
	 * @endcode
     */ 
     virtual int Highlight() = 0;
     
    /**
     * Get the index of the current highlighted item along with the path 
     * of the root item in the data hierarchy.
     * 
     * @param aPath  Path of parent item in the data hierarchy. Out parameter.
     * @return Highlighted item index.
     *
	 * <B>Examples:</b>
	 * @code
	 * Get index of the highlighted item.
	 *
	 *      int highlight = mListWidget->Highlight();
	 * @endcode
     */ 
     virtual int Highlight( MulDataPath& aPath ) = 0;     
     
    /**
	 * Function for setting the item(s) Marked or Unmarked. For mark and
	 * unmark operations, indices of items should be provided.
	 * 
	 * @param aType Type of marking operation to be performed. 
	 * @param aItemsIndex Indices of the items to be changed
	 * @throw Invalid Argument, if type is specified as mark-all and unmark-all. 
	 *
	 * <B>Examples:</b>
	 * @code
     * Mark the selected item
     *
	 *      mListWidget->SetMarkedIndices( MulWidgetDef::EMulMark, itemIndex );
	 * @endcode
	 */ 
	virtual void SetMarkedIndices( MulWidgetDef::TMulWidgetMarkingType aType ,
								   const std::vector<int>& aItemsIndex,
								   const MulDataPath& aPath = MulDataPath() ) = 0;

	/**
	 * Function for setting the item(s) Marked or Unmarked.
	 * For mark all and unmark all operations use this overloaded version.
	 * 
	 * @param aType Type of marking operation to be performed.
	 * @throw Invalid Argument, if type is specified as mark and unmark. 
     *
	 * <B>Examples:</b>
	 * @code
	 * Mark all items
	 *
	 *      mListWidget->SetSelection( MulWidgetDef::EMulMarkAll );
	 * @endcode
	 */		 
	virtual	void SetMarkedIndices(
			MulWidgetDef::TMulWidgetMarkingType aType = MulWidgetDef::EMulMarkAll
			) = 0;
			
	/**
	 * Gets the indices of the currently marked items.
	 * 
	 * @return Array of maked indices. This would return an empty container if no items are marked. 
	 *
	 * <B>Examples:</b>
	 * @code
	 * Get the array of all marked items
	 *
	 *      std::vector<int> markedItems = mListWidget->MarkIndices();
	 * @endcode
	 */
	virtual const std::vector<int>& MarkedIndices( const MulDataPath& aPath = MulDataPath() ) = 0;    
          
    };

    } // namespace Alf

#endif // I_MULMODEL_H

//End of file

