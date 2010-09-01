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
* Description:  Implementation of model interface
 *
*/


#ifndef MULMODELIMPL_H
#define MULMODELIMPL_H

//Includes
#include <mul/imulmodel.h>

#include <osn/ustring.h>
#include <memory>
#include <vector>

#include "mul/imulmodelaccessor.h"
#include "imulmodelobserver.h"

namespace osncore
    {
    class UString;
    }
    
using namespace osncore;

namespace Alf
    {

//Forward Declaration

class MulDataWindow;
class MulPushMode;
class MulPageDataWindow;

/**
 *  Implementation class for Data model.
 *  @lib 
 */
class MulModelImpl : public IMulModel,
					 public IMulModelAccessor
    {
    
public:
	
	/**
	 * C++ constructor.
	 */
	OSN_IMPORT MulModelImpl();
	
	/**
	 * Destructor.
	 */
	OSN_IMPORT virtual ~MulModelImpl();
    
public: //from IMulModel
   	
	void Insert(int aIndex, int aCount, 
                const MulDataPath& aPath = MulDataPath());
	
    void Remove( int aIndex, int aCount, 
    			 const MulDataPath& aPath = MulDataPath() );
	
	void Update( int aIndex, int aCount, 
				 const MulDataPath& aPath = MulDataPath() );
	
	void Refresh( int aCount, const MulDataPath& aPath = MulDataPath() );
	     
	void SetData( int aIndex, std::auto_ptr<MulVisualItem> aVisualItem, 
                  const MulDataPath& aPath = MulDataPath() );
	
	const MulVisualItem* Data( int aIndex, 
	                    	   const MulDataPath& aPath = MulDataPath() ) const;

	int Count( const MulDataPath& aPath = MulDataPath() ) const; 

	void SetDataProvider( const IMulModelProvider& aDataProvider );

	void SetTemplate( mulwidget::TLogicalTemplate aTemplateId );

	void SetTemplate( int aItemIndex, mulwidget::TLogicalTemplate aTemplateId, 
	                  const MulDataPath& aPath = MulDataPath() );

	void SetDataWindow( int aSize );

	int DataWindow( ) const;        

	void SetHighlight( int aItemIndex, const MulDataPath& aPath = MulDataPath() ); 

	int Highlight() ;

	int Highlight( MulDataPath& aPath );     

	void SetMarkedIndices( MulWidgetDef::TMulWidgetMarkingType aType ,
						   const std::vector<int>& aItemsIndex,
						   const MulDataPath& aPath = MulDataPath() );

	void SetMarkedIndices(
			MulWidgetDef::TMulWidgetMarkingType aType = MulWidgetDef::EMulMarkAll );

	const std::vector<int>& MarkedIndices( const MulDataPath& aPath = MulDataPath() );

public: //from IMulModelBase
	    
	IMulModelAccessor* Accessor(); 

public: //from IMulModelAccessor
	 
	void SetHighlight( int aItemIndex );
	
	void ScrollWindow( int aItemIndex );
	 		      	
	int CurrentItemCount();
	 	
	void SetVisibleWindow( int aWindowSize, int aPageTopIndex = -1 );
		 		
	int RelativeIndex( int aAbsoluteIndex ) const;
 	 
	int TopWindowOffset() const;
	
	int BottomWindowOffset() const;
	
	int TopOffset() const;
			
	int BottomOffset() const;
	
	int RearTopOffset() const;
	
	int RearBottomOffset() const;
	
	bool IsItemMarked( int aIndex ); 
	
	const MulVisualItem& Item( int aAbsoluteIndex );

    int AbsoluteIndex( int aRelativeIndex );
    
    void SetMarkedIndex( MulWidgetDef::TMulWidgetMarkingType aType, int aIndex );
    
    bool IsLoopingOn();
	
    void AddModelObserver( IMulModelObserver* aObserver );
    
    void RemoveModelObserver( IMulModelObserver* aObserver );
    
    mulwidget::TLogicalTemplate Template();
    
private: //from IAlfModel

	void addModelChangeObserver( IAlfModelChangeObserver& aObserver );
	
	void removeModelChangeObserver( IAlfModelChangeObserver& aObserver );
	
	void setData( IAlfVariantType* aData );
	
	void updateData(int aNumContainerIndices,int* aContainerIndices, IAlfVariantType* aData );
	
	void addData(int aNumContainerIndices,int* aContainerIndices, IAlfVariantType* aData );
	
	void removeData(int aNumContainerIndices,int* aContainerIndices );
	  
	void executeOperations(AlfPtrVector<AlfModelOperation>&  aOperationsArray);
	      
	void executeOperation(AlfModelOperation* aOperation); 
	
	void clearModel(); 
	
	IAlfVariantType* data() const;
	
private: // from base class MAlfInterfaceBase

    /**
     * Getter for interfaces provided by the list model.
     *
     * @param aType A descriptor to identify the type of the queried interface.
     * @return The queried interface, or NULL if the interface is not
     *         supported or available.
     */
    virtual IAlfInterfaceBase* makeInterface( const IfId& aType );
    
public: //new method

	/**
	 * When ever data window update data window, it will use this method to notify
	 * Data model to update data window and update alf model
	 */
	void DataWindowUpdated();
    			
private: //New method
	
	/**
	 * Create Update event for selection change to notify Widget
	 * for selection change. Widget need to use IsItemMarked API for checking 
	 * whether item is marked or unmarked.
	 * 
	 * @param aAbsoluteIndex Absolute index
	 */
	void CreateMarkingEvent(int aAbsoluteIndex);
		
	/**
	 * Notify observer about state changed
	 * 
	 * @param aState Highlight or making changed
	 * @param aData Index of new highlight or Index of marked or unmarked item.
	 *				( Item is marked or not should be checked using IMulModelAccessor interface)	  
	 */
	void NotifyStateChange( IMulModelObserver::TMulChangedState aState, std::auto_ptr<IMulVariantType> aData);
	
	/**
     * Notify observer about data change event
     * 
     * @param aState Event to notify
     * @param aStartIndex Start index of.
     * @param aCount No of item affected      
     */
	void NotifyDataChange( IMulModelObserver::TMulChangedState aState, int aStartIndex, int aCount );
	
	/**
     * Notify observer about data change event
     * 
     * @param aState Event to notify
     * @param aIndex Start index of.
     */
    void NotifyDataChange( IMulModelObserver::TMulChangedState aState, int aIndex );
    
    /**
     * Notify observer about template change
     * 
     * @param aIndex index of item.
     * @param aTamplateId changed template
     */
    void NotifyTemplateChange( int aIndex, mulwidget::TLogicalTemplate aTamplateId );
				
private: //data
		
	//std::auto_ptr<MulDataWindow>			mDataWindow; //own
	std::auto_ptr<MulPageDataWindow>		mDataWindow; //own
	std::auto_ptr<MulPushMode> 		        mModelHelper; //own
	std::vector<IMulModelObserver*>			mObserverArray;
    };
    
    } // namespace Alf

#endif //MULMODELIMPL_H
    
//End of file
    
