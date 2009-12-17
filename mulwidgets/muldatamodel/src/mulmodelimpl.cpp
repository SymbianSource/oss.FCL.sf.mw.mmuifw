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


//Includes
// class headers
#include "mulmodelimpl.h"

#include <mul/mulvarianttype.h>

// Local Headers
#include "mulcachemanager.h"
#include "muldatawindow.h"
#include "mulpagedatawindow.h"
#include "mulpushmode.h"
#include "mulpullmode.h"
#include "mulassert.h"
#include "mulmodeldef.h"
#include "mullog.h"

namespace Alf
    { 
    
// ---------------------------------------------------------------------------
// MulModelImpl()
// ---------------------------------------------------------------------------
//
OSN_EXPORT MulModelImpl::MulModelImpl()
	{	
	MUL_LOG_ENTRY_EXIT("MUL::MulModelImpl::MulModelImpl()");  	
	
	//mDataWindow.reset( new ( EMM ) MulDataWindow( *this ));
	mDataWindow.reset( new ( EMM ) MulPageDataWindow( *this ));  	
	mModelHelper.reset(new (EMM) MulPushMode(*mDataWindow ));
	}

// ---------------------------------------------------------------------------
// ~MulModelImpl()
// ---------------------------------------------------------------------------
//
OSN_EXPORT MulModelImpl::~MulModelImpl()
	{
	MUL_LOG_ENTRY_EXIT("MUL::MulModelImpl::~MulModelImpl()");   
	mObserverArray.clear();
	}

// ------------------------ From IMulModel -----------------------------------

// ---------------------------------------------------------------------------
// Insert
// ---------------------------------------------------------------------------
//
void MulModelImpl::Insert(int aIndex, int aCount,const MulDataPath& aPath)
	{
	MUL_LOG_ENTRY_EXIT("MUL::MulModelImpl::Insert()"); 
	MUL_LOG_INFO2("MUL::MulModelImpl::Insert() index:%d,count:%d",aIndex,aCount);
	
	mModelHelper->Insert(aIndex, aCount,aPath);

	if( aCount > 0  &&  mDataWindow->Highlight() == KNotInitialized )
		{
		mDataWindow->UpdateHighlight(0);
		NotifyDataChange( IMulModelObserver::EHighlightChanged , 0 );
		}
				
	NotifyDataChange( IMulModelObserver::EItemsInserted, aIndex, aCount );
	}

// ---------------------------------------------------------------------------
// Remove
// ---------------------------------------------------------------------------
//
void MulModelImpl::Remove(int aIndex, int aCount, const MulDataPath& aPath)
	{
	// If the highlight has changed because of the remove then inform the widget
	if( mModelHelper->Remove(aIndex, aCount, aPath) )
		{
		NotifyDataChange( IMulModelObserver::EHighlightChanged , mDataWindow->Highlight() );
		}
	NotifyDataChange( IMulModelObserver::EItemsRemoved, aIndex, aCount );
	}

// ---------------------------------------------------------------------------
// Update
// ---------------------------------------------------------------------------
//
void MulModelImpl::Update( int aIndex, int aCount, const MulDataPath& aPath )
	{
	mModelHelper->Update( aIndex, aCount, aPath );
	}

// ---------------------------------------------------------------------------
// Refresh
// ---------------------------------------------------------------------------
//
void MulModelImpl::Refresh( int aCount, const MulDataPath& aPath )
	{	
	mModelHelper->Refresh( aCount, aPath );
	NotifyDataChange( IMulModelObserver::EModelRefreshed , aCount );
	}

// ---------------------------------------------------------------------------
// SetData
// ---------------------------------------------------------------------------
//
void MulModelImpl::SetData( int aIndex, auto_ptr<MulVisualItem> aVisualItem, const MulDataPath& aPath )
	{
	MUL_LOG_ENTRY_EXIT("MUL::MulModelImpl::SetData()");  		
	MUL_LOG_INFO1("MUL::MulModelImpl::SetData() index:%d",aIndex );
	
	mModelHelper->SetData( aIndex, aVisualItem, aPath );
	NotifyDataChange( IMulModelObserver::EItemUpdated , aIndex );
	}

// ---------------------------------------------------------------------------
// Data
// ---------------------------------------------------------------------------
//
const MulVisualItem* MulModelImpl::Data( int aIndex, const MulDataPath& aPath) const
	{
	return mModelHelper->Data( aIndex, aPath );
	}

// ---------------------------------------------------------------------------
// Count
// ---------------------------------------------------------------------------
//
int MulModelImpl::Count( const MulDataPath& aPath) const
	{
	return mModelHelper->Count( aPath );
	}

// ---------------------------------------------------------------------------
// SetDataProvider
// ---------------------------------------------------------------------------
//
void MulModelImpl::SetDataProvider( const IMulModelProvider& aDataProvider )
	{	
	mModelHelper.reset(new (EMM) MulPullMode( *mDataWindow, aDataProvider ) );	
	}

// ---------------------------------------------------------------------------
// SetTemplate
// ---------------------------------------------------------------------------
//
void MulModelImpl::SetTemplate( mulwidget::TLogicalTemplate aTemplateId )
	{
	MUL_LOG_ENTRY_EXIT("MUL::MulModelImpl::SetTemplate");
	mModelHelper->SetTemplate( aTemplateId );
	// default template updated
	NotifyTemplateChange( -1, aTemplateId );
	}

// ---------------------------------------------------------------------------
// SetTemplate
// ---------------------------------------------------------------------------
//
void MulModelImpl::SetTemplate(int aItemIndex, mulwidget::TLogicalTemplate aTemplateId, const MulDataPath& aPath )
	{
	MUL_LOG_ENTRY_EXIT("MUL::MulModelImpl::SetTemplate");
	mModelHelper->SetTemplate(aItemIndex, aTemplateId, aPath );
	NotifyTemplateChange( aItemIndex, aTemplateId );
	}

// ---------------------------------------------------------------------------
// SetDataWindow
// ---------------------------------------------------------------------------
//
void MulModelImpl::SetDataWindow( int aBufferSize )
	{
	mDataWindow->SetBufferSize( aBufferSize );
	}

// ---------------------------------------------------------------------------
// DataWindow
// ---------------------------------------------------------------------------
//
int MulModelImpl::DataWindow() const         
	{
	return mDataWindow->BufferSize();
	}
	
// ---------------------------------------------------------------------------
// SetHighlight
// ---------------------------------------------------------------------------
//
void MulModelImpl::SetHighlight( int aItemIndex, const MulDataPath& /*aPath*/ )
	{
	mDataWindow->SetHighlight( aItemIndex );
	
	NotifyDataChange( IMulModelObserver::EHighlightChanged , aItemIndex );
	}

// ---------------------------------------------------------------------------
// Highlight
// ---------------------------------------------------------------------------
//
int MulModelImpl::Highlight( MulDataPath& /*aPath*/ )
	{
	return mDataWindow->Highlight();	
	}

// ---------------------------------------------------------------------------
// Highlight
// ---------------------------------------------------------------------------
//
int MulModelImpl::Highlight()
	{
	return 	mDataWindow->Highlight();
	}

// ---------------------------------------------------------------------------
// SetMarkedIndices
// ---------------------------------------------------------------------------
//
void MulModelImpl::SetMarkedIndices( MulWidgetDef::TMulWidgetMarkingType aType,
									 const std::vector<int>& aItemsIndex,
									 const MulDataPath& aPath )

	{
	//fire update event in alf model
	mModelHelper->CacheManager().SetMarkedIndices( aType, aItemsIndex, aPath );
	
	for( int i = 0 ; i < aItemsIndex.size() ; i++ )
	    {
	    if(mDataWindow->IsItemInDataWindow(aItemsIndex[i]))
	        {
	        CreateMarkingEvent(aItemsIndex[i]);
	        }
	    }
	}

// ---------------------------------------------------------------------------
// SetMarkedIndices
// ---------------------------------------------------------------------------
//
void MulModelImpl::SetMarkedIndices(MulWidgetDef::TMulWidgetMarkingType aType)
	{
	//fire update event in alf model
	mModelHelper->CacheManager().SetMarkedIndices( aType );
	
	for( int i = mDataWindow->TopOffset() ; i <= mDataWindow->BottomOffset() ; i++ )
        {
        CreateMarkingEvent(i);
        }
	    
	for( int i = mDataWindow->RearTopOffset( ); i <= mDataWindow->RearBottomOffset() && i > 0 ; i++ )
        {
        CreateMarkingEvent(i);
        }
	}

// ---------------------------------------------------------------------------
// CreateMarkingEvent
// ---------------------------------------------------------------------------
//
void MulModelImpl::CreateMarkingEvent(int aAbsoluteIndex)
	{
	NotifyDataChange( IMulModelObserver::EMarkingChanged , aAbsoluteIndex );
	}

// ---------------------------------------------------------------------------
// NotifyStateChange
// ---------------------------------------------------------------------------
//
void MulModelImpl::NotifyStateChange( IMulModelObserver::TMulChangedState aState, 
									  std::auto_ptr<IMulVariantType> aData)
	{
	for( int i = 0; i < mObserverArray.size(); ++i )
		{
		mObserverArray[i]->ModelStateChanged( aState, *aData );
		}
	}

// ---------------------------------------------------------------------------
// NotifyDataChange
// ---------------------------------------------------------------------------
//
void MulModelImpl::NotifyDataChange( IMulModelObserver::TMulChangedState aState, 
                                      int aStartIndex, int aCount )
    {
    std::auto_ptr<IAlfMap> map( new ( EMM )AlfMap() );
    map->addItem( new (EMM) AlfVariantType(aStartIndex),UString(KStartIndex));
    map->addItem( new (EMM) AlfVariantType(aCount),UString(KCount));
    
    std::auto_ptr<IMulVariantType> data(new (EMM)MulVariantType(*map));
    NotifyStateChange( aState , data );
    }

// ---------------------------------------------------------------------------
// NotifyDataChange
// ---------------------------------------------------------------------------
//
void MulModelImpl::NotifyDataChange( IMulModelObserver::TMulChangedState aState, int aIndex )
    {    
    std::auto_ptr<IMulVariantType> data(new (EMM)MulVariantType(aIndex));
    NotifyStateChange( aState , data );
    }

// ---------------------------------------------------------------------------
// NotifyTemplateChange
// ---------------------------------------------------------------------------
//
void MulModelImpl::NotifyTemplateChange( int aIndex, mulwidget::TLogicalTemplate aTamplateId )
    {
    std::auto_ptr<IAlfMap> map( new ( EMM )AlfMap() );
    map->addItem(new (EMM) AlfVariantType(aTamplateId),osncore::UString(KDefaultTemplate));
    map->addItem(new (EMM) AlfVariantType(aIndex),osncore::UString(KIndex));
       
    std::auto_ptr<IMulVariantType> data(new (EMM)MulVariantType(*map));
    NotifyStateChange( IMulModelObserver::ETemplateChanged , data );
    }

// ---------------------------------------------------------------------------
// MarkedIndices
// ---------------------------------------------------------------------------
//
const std::vector<int>& MulModelImpl::MarkedIndices(const MulDataPath& aPath )
	{	
	return mModelHelper->CacheManager().MarkedIndices( aPath );	
	}

// ---------------------------------------------------------------------------
// IsLoopingOn
// ---------------------------------------------------------------------------
//
bool MulModelImpl::IsLoopingOn()
	{
	return ( mDataWindow->RearBottomOffset() != -1 && mDataWindow->OldRearBottomOffset() != -1 ); 
	}

// ---------------------------------------------------------------------------
// AddModelObserver
// ---------------------------------------------------------------------------
//
void MulModelImpl::AddModelObserver( IMulModelObserver* aObserver )
    {
    __MUL_ASSERT(aObserver, KLInvalidArgument );
    if( std::find(mObserverArray.begin(), mObserverArray.end(), aObserver) == mObserverArray.end() )
        {
        mObserverArray.push_back( aObserver );  
        }
    }

// ---------------------------------------------------------------------------
// RemoveModelObserver
// ---------------------------------------------------------------------------
//
void MulModelImpl::RemoveModelObserver( IMulModelObserver* aObserver )
    {
    __MUL_ASSERT(aObserver, KLInvalidArgument );
    if( std::find(mObserverArray.begin(), mObserverArray.end(), aObserver ) != mObserverArray.end() )
       {
       mObserverArray.erase( std::find( mObserverArray.begin(), mObserverArray.end(), aObserver ));  
       }
    }

// ---------------------------------------------------------------------------
// RemoveModelObserver
// ---------------------------------------------------------------------------
//
mulwidget::TLogicalTemplate MulModelImpl::Template()
    {
    return mModelHelper->Template();
    }
	
// ---------------------------------------------------------------------------
// Accessor
// ---------------------------------------------------------------------------
//
IMulModelAccessor* MulModelImpl::Accessor()
	{
	return this;
	}

// ------------------------ From IMulModelAccessor ---------------------------

// ---------------------------------------------------------------------------
// SetHighlight
// ---------------------------------------------------------------------------
//
void MulModelImpl::SetHighlight( int aItemIndex )
	{
	mDataWindow->SetHighlight( aItemIndex );
	}

// ---------------------------------------------------------------------------
// ScrollWindow
// ---------------------------------------------------------------------------
//
void MulModelImpl::ScrollWindow( int aItemIndex )
	{
	mDataWindow->ScrollWindow( aItemIndex );
	}
 
// ---------------------------------------------------------------------------
// CurrentItemCount
// ---------------------------------------------------------------------------
//
int MulModelImpl::CurrentItemCount()
	{
	return mModelHelper->CurrentItemCount();
	}
 
// ---------------------------------------------------------------------------
// SetDataWindow
// ---------------------------------------------------------------------------
//
void MulModelImpl::SetVisibleWindow(  int aWindowSize, int aPageTopIndex )
	{
	mDataWindow->SetWindowSize( aWindowSize, aPageTopIndex );
	}
// ------------------------ Member Function ----------------------------------

// ---------------------------------------------------------------------------
// DataWindowUpdated
// ---------------------------------------------------------------------------
//
void MulModelImpl::DataWindowUpdated()
	{
	mModelHelper->DataWindowUpdated();
	std::auto_ptr<IMulVariantType> dummyData;
	NotifyStateChange( IMulModelObserver::EDataWindowUpdated, dummyData );			
	}

// ---------------------------------------------------------------------------
// RelativeIndex
// ---------------------------------------------------------------------------
//
int MulModelImpl::RelativeIndex( int aAbsoluteIndex ) const
	{
	return mDataWindow->RelativeIndex(aAbsoluteIndex);
	}

// ---------------------------------------------------------------------------
// TopWindowOffset
// ---------------------------------------------------------------------------
//
int MulModelImpl::TopWindowOffset() const
	{
	return mDataWindow->TopWindowOffset();
	}

// ---------------------------------------------------------------------------
// BottomWindowOffset
// ---------------------------------------------------------------------------
//
int MulModelImpl::BottomWindowOffset() const
	{
	return mDataWindow->BottomWindowOffset();
	}

// ---------------------------------------------------------------------------
// TopOffset
// ---------------------------------------------------------------------------
//
int MulModelImpl::TopOffset() const
	{
	return mDataWindow->TopOffset();
	}

// ---------------------------------------------------------------------------
// BottomOffset
// ---------------------------------------------------------------------------
//
int MulModelImpl::BottomOffset() const
	{
	return mDataWindow->BottomOffset();
	}

// ---------------------------------------------------------------------------
// RearTopOffset
// ---------------------------------------------------------------------------
//
int MulModelImpl::RearTopOffset() const
	{
	return mDataWindow->RearTopOffset();
	}

// ---------------------------------------------------------------------------
// RearBottomOffset
// ---------------------------------------------------------------------------
//
int MulModelImpl::RearBottomOffset() const
	{
	return mDataWindow->RearBottomOffset();
	}
	
// ---------------------------------------------------------------------------
// IsItemMarked
// ---------------------------------------------------------------------------
//  
bool MulModelImpl::IsItemMarked( int aIndex )
	{
	return mModelHelper->CacheManager().IsItemMarked(aIndex);
	}

// ---------------------------------------------------------------------------
// Item
// ---------------------------------------------------------------------------
//
const MulVisualItem& MulModelImpl::Item( int aAbsoluteIndex )
	{
	return mModelHelper->Item( aAbsoluteIndex );
	}

// ---------------------------------------------------------------------------
// AbsoluteIndex
// ---------------------------------------------------------------------------
//
int MulModelImpl::AbsoluteIndex( int aRelativeIndex )
	{
	return mDataWindow->AbsoluteIndex(aRelativeIndex);
	}

// ---------------------------------------------------------------------------
// SetMarkedIndex
// ---------------------------------------------------------------------------
//
void MulModelImpl::SetMarkedIndex( MulWidgetDef::TMulWidgetMarkingType aType, int aIndex )
	{
	mModelHelper->CacheManager().SetMarkedIndex(aType,aIndex);
	}

// ------------- Function from IAlfModel -------------------------------------

// ---------------------------------------------------------------------------
// AddModelChangeObserver
// ---------------------------------------------------------------------------
//
void MulModelImpl::addModelChangeObserver( IAlfModelChangeObserver& /*aObserver*/ )
	{
	//__MUL_ASSERT_DEBUG(false, KLNotImplemented );
	}
	
// ---------------------------------------------------------------------------
// RemoveModelChangeObserver
// ---------------------------------------------------------------------------
//
void MulModelImpl::removeModelChangeObserver( IAlfModelChangeObserver& /*aObserver*/ )
	{	
	//__MUL_ASSERT_DEBUG(false, KLNotImplemented );
	}
// ---------------------------------------------------------------------------
// SetData
// ---------------------------------------------------------------------------
//
void MulModelImpl::setData( IAlfVariantType* /*aData*/ )
	{
	__MUL_ASSERT_DEBUG(false, KLNotImplemented);
	}

// ---------------------------------------------------------------------------
// UpdateData
// ---------------------------------------------------------------------------
//
void MulModelImpl::updateData( int /*aNumContainerIndices*/, 
								int* /*aContainerIndices*/, 
								IAlfVariantType* /*aData*/ )
	{
	__MUL_ASSERT_DEBUG(false, KLNotImplemented);
	}

// ---------------------------------------------------------------------------
// AddData
// ---------------------------------------------------------------------------
//
void MulModelImpl::addData( int /*aNumContainerIndices*/, 
							 int* /*aContainerIndices*/, 
							 IAlfVariantType* /*aData*/ )
	{
	__MUL_ASSERT_DEBUG(false, KLNotImplemented);
	}

// ---------------------------------------------------------------------------
// RemoveData
// ---------------------------------------------------------------------------
//
void MulModelImpl::removeData( int /*aNumContainerIndices*/, int* /*aContainerIndices*/ )
	{
	__MUL_ASSERT_DEBUG(false, KLNotImplemented);
	}

// ---------------------------------------------------------------------------
// ExecuteOperations
// ---------------------------------------------------------------------------
//
void MulModelImpl::executeOperations( AlfPtrVector<AlfModelOperation>&  /*aOperationsArray*/ )
	{
	__MUL_ASSERT_DEBUG(false, KLNotImplemented);
	}
      
// ---------------------------------------------------------------------------
// ExecuteOperation
// ---------------------------------------------------------------------------
//
void MulModelImpl::executeOperation( AlfModelOperation* /*aOperation*/ )
	{
	__MUL_ASSERT_DEBUG(false, KLNotImplemented);
	}

// ---------------------------------------------------------------------------
// ClearModel
// ---------------------------------------------------------------------------
//
void MulModelImpl::clearModel()
	{	
	__MUL_ASSERT_DEBUG(false, KLNotImplemented);
	}

// ---------------------------------------------------------------------------
// Data
// ---------------------------------------------------------------------------
//
IAlfVariantType* MulModelImpl::data() const
	{
	__MUL_ASSERT_DEBUG(false, KLNotImplemented);
	return NULL;
	}

// ---------------------------------------------------------------------------
// From class MAlfInterfaceBase.
// Getter for interfaces provided by the list model.
// ---------------------------------------------------------------------------
//
IAlfInterfaceBase* MulModelImpl::makeInterface( const IfId& /*aType*/ )
    {   
    return static_cast<IMulModel*>( this );
    }
 
	
    } // namespace Alf

//End of file
