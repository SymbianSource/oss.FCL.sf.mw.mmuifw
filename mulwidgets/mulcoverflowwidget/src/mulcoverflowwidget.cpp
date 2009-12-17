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
* Description:  Widget Implementation
*
*/

//  Include Files  
#include <alf/alfenv.h>
#include <alf/alfwidgetenvextension.h>

// Class headers
#include "mulcoverflowwidget.h"    

// Mul Headers 
#include "imulmodelaccessor.h"
#include "mulassert.h"
#include "mullog.h" //for logs
#include <mul/imulsliderwidget.h>

// Local Headers
#include "mulbaseelement.h"
#include "mulcoverflowcontrol.h"

namespace Alf
    {

// ---------------------------------------------------------------------------
// MulCoverFlowWidget
// ---------------------------------------------------------------------------
//
MulCoverFlowWidget::MulCoverFlowWidget(  CAlfEnv& aEnv, 
                                   const char* aWidgetId, IAlfContainerWidget& aContainerWidget):AlfWidget(aWidgetId,aContainerWidget,aEnv)
    {
    mMulModelAccessor = NULL;
    MUL_LOG_ENTRY_EXIT("aakash::MulCoverFlowWidget::MulCoverFlowWidget");
    mWidgetPropertyFlag  = 0; //Initialization
        
    //Create default control 
    mCoverflowControl = new (EMM) MulCoverFlowControl( aEnv );
    setControl( mCoverflowControl,true );

    // create Base element
    mBaseElement = new (EMM) MulBaseElement( *mCoverflowControl, KBase );
    mTakesModelOwnership = false;
    
    }

// ---------------------------------------------------------------------------
// ~MulCoverFlowWidget
// ---------------------------------------------------------------------------
//
MulCoverFlowWidget::~MulCoverFlowWidget()
    {
    MUL_LOG_ENTRY_EXIT("aakash::MulCoverFlowWidget::~MulCoverFlowWidget");
    // Control will be destroyed in the Widget deletion sequence. In that sequence 
	// Control's owner widget will be set to NULL. So,We cannot access the widget in
	// the Coverflow control's destructor. So,This will be the right place to remove the
	// model Observer.

	if( control() && model())
	    {
	    ((IMulModelAccessor*)model())->RemoveModelObserver(static_cast<MulCoverFlowControl*>( control()));
	    }
	if (mMulModelAccessor && mTakesModelOwnership)
		{
		delete mMulModelAccessor;
		}

	IAlfWidgetFactory& widgetFactory = AlfWidgetEnvExtension::widgetFactory(*(CAlfEnv::Static()));
	IAlfWidget* sliderWidget = widgetFactory.findWidget( KNameSliderWidget );
		
	if(control())    
	    {
		(static_cast<MulCoverFlowControl*>(control()))->DestroySlider();   
	    }   

    if(sliderWidget)
    	{
    	widgetFactory.destroyWidget(sliderWidget);
    	}      	
    }

// ---------------------------------------------------------------------------
// parent
// ---------------------------------------------------------------------------
//
IAlfContainerWidget* MulCoverFlowWidget::parent() const
	{
	return AlfWidget::parent (); //return container widget
	}
    
// ---------------------------------------------------------------------------
// control
// ---------------------------------------------------------------------------
//
CAlfWidgetControl* MulCoverFlowWidget::control() const
    {
    //Call the base class method
    return AlfWidget::control();
    }
    
// ---------------------------------------------------------------------------
// setControl
// ---------------------------------------------------------------------------
//
void MulCoverFlowWidget::setControl( CAlfWidgetControl* aControl,bool aDeletePreviousControl  )
    {
    MUL_LOG_ENTRY_EXIT("aakash::MulCoverFlowWidget::setControl");
    AlfWidget::setControl( aControl,aDeletePreviousControl );
    }

// ---------------------------------------------------------------------------
// model
// ---------------------------------------------------------------------------
//
IAlfModel* MulCoverFlowWidget::model()
    {
    return mMulModelAccessor;
    }

// ---------------------------------------------------------------------------
// setModel
// ---------------------------------------------------------------------------
//
void MulCoverFlowWidget::setModel( IAlfModel* /*aModel*/ , bool /*aTakeOwnership*/ )
    {
    }

// ---------------------------------------------------------------------------
// widgetName
// ---------------------------------------------------------------------------
//
const char* MulCoverFlowWidget::widgetName() const
    {
    return AlfWidget::widgetName();
    }

 
// ---------------------------------------------------------------------------
// SetBackground
// ---------------------------------------------------------------------------
// 
void MulCoverFlowWidget::SetBackground(const TRgb& aColor)
	{
	MUL_LOG_ENTRY_EXIT("aakash::MulCoverFlowWidget::SetBackground");

	mBaseElement->SetBackground( aColor );		
	}
    
// ---------------------------------------------------------------------------
// SetBackground
// ---------------------------------------------------------------------------
//
void MulCoverFlowWidget::SetBackground(const TAknsItemID& aItemID) 
	{
	MUL_LOG_ENTRY_EXIT("aakash::MulCoverFlowWidget::SetBackground");
	
	mBaseElement->SetBackground( aItemID );
	}
// ---------------------------------------------------------------------------
// SetFlags
// ---------------------------------------------------------------------------
//
void MulCoverFlowWidget::SetFlags( unsigned int aAnyFlag )
    {
    MUL_LOG_ENTRY_EXIT("aakash::MulCoverFlowWidget::SetFlags");
    mWidgetPropertyFlag |= aAnyFlag;
    
    // When the IMulMultiItemWidget::EMulWidgetFlagFastScroll flag is modified we should
    // set the enable holding flag to gesture helper to recieve the hold events.
    if(aAnyFlag == IMulMultiItemWidget::EMulWidgetFlagFastScroll)
	    {
	    mCoverflowControl->SetHoldingEnabled(); 	
	    }
	else if(aAnyFlag == IMulMultiItemWidget::EMulWidgetDoubleTap)
		{
		mCoverflowControl->SetDoubleTapEnabled(true); 	
		}
    }

// ---------------------------------------------------------------------------
// ClearFlags
// ---------------------------------------------------------------------------
//
void MulCoverFlowWidget::ClearFlags( unsigned int aAnyFlag )
    {
    MUL_LOG_ENTRY_EXIT("aakash::MulCoverFlowWidget::ClearFlags");
    mWidgetPropertyFlag &= ~aAnyFlag;
    if(aAnyFlag == IMulMultiItemWidget::EMulWidgetDoubleTap)
		{
		mCoverflowControl->SetDoubleTapEnabled(false); 	
		}
	else if(aAnyFlag == IMulMultiItemWidget::EMulWidgetFlagFastScroll)
	    {
	    mCoverflowControl->SetHoldingEnabled(); 	
	    }
    }

// ---------------------------------------------------------------------------
// IsFlagSet
// ---------------------------------------------------------------------------
//
bool MulCoverFlowWidget::IsFlagSet( TMulWidgetPropertiesFlag aAnyFlag ) const
    {
    MUL_LOG_ENTRY_EXIT("aakash::MulCoverFlowWidget::IsFlagSet");

    return (mWidgetPropertyFlag & aAnyFlag);

    }

// ---------------------------------------------------------------------------
// GetFlags
// ---------------------------------------------------------------------------
//
unsigned int MulCoverFlowWidget::GetFlags() const
    {
    MUL_LOG_ENTRY_EXIT("aakash::MulCoverFlowWidget::GetFlags");    
    return mWidgetPropertyFlag;
    }

// ---------------------------------------------------------------------------
// SetRect
// ---------------------------------------------------------------------------
//    
void MulCoverFlowWidget::SetRect( int /*aLeftMargin*/, int /*aTopmargin*/, int /*aHeight*/,
                                  int /*aWidth*/, int /*aTransitionTime*/ )
    {
    //@todo need to be removed once its removed from imulwidget.
    }
// ---------------------------------------------------------------------------
// ShowWidget
// ---------------------------------------------------------------------------
//
void MulCoverFlowWidget::ShowWidget( bool aShow, int aTransitionTime ) 
    {
    MUL_LOG_ENTRY_EXIT("aakash::MulCoverFlowWidget::ShowWidget");
    mBaseElement->ShowWidget( aShow,aTransitionTime );
    
    // Reset the flag . Event handling gets disabled once the flag is reset to false.
    mCoverflowControl->ResetShowWidgetFlag( aShow );
    }

// ---------------------------------------------------------------------------
// SetModel
// ---------------------------------------------------------------------------
//
void MulCoverFlowWidget::SetModel( IMulModelAccessor* aAccessor, bool aFlag )
    {
    MUL_LOG_ENTRY_EXIT("aakash::MulCoverFlowWidget::SetModel");
    
    //remove from observing change from old model
	if ( mMulModelAccessor != aAccessor )
        {
        if (mMulModelAccessor && control())
            {
            ((IMulModelAccessor*)model())->RemoveModelObserver(static_cast<MulCoverFlowControl*>( control()));
            }
        // Release the old model.
        if (mTakesModelOwnership)
            {
            delete mMulModelAccessor;
            }
        // Set the new model.
        mMulModelAccessor = aAccessor;
        mTakesModelOwnership=aFlag;        
        }
	else // just copy the ownership flag 
        { 
        if (mMulModelAccessor != NULL)
            {
            mTakesModelOwnership=aFlag;
            }
        }
    
    IMulSliderWidget* sliderWidget = static_cast<IMulSliderWidget*>(mCoverflowControl->GetSliderWidget());
    // cancel all previous commands (like marquee and bounce)
	mCoverflowControl->Env().CancelCustomCommands(mCoverflowControl);
    
    if( aAccessor )
    	{
    	mMulModelAccessor = aAccessor;
        MUL_LOG_INFO("MUL::MulCoverFlowWidget::SetModel()::SetVisibleWindow "); 
        aAccessor->AddModelObserver(mCoverflowControl);
		// Notify the control regarding model change.
		mCoverflowControl->ModelChanged(aAccessor);

        if( aAccessor->CurrentItemCount() <= 0 )
        	{
        	if(sliderWidget)
	    		{
	    		mBaseElement->ShowSlider(false);	
	    		}		
        	}
    	}
    else // For Null Model case
    	{
    	mCoverflowControl->RecycleVisuals();
    	mBaseElement->ShowEmptyText( true );

    	if( sliderWidget )
    		{
    		mBaseElement->ShowSlider(false);		
    		}
    	}
    }

// ---------------------------------------------------------------------------
// GetIconSize
// ---------------------------------------------------------------------------
//	
void MulCoverFlowWidget::GetIconSize( mulwidget::TLogicalTemplate aTemplateId, 
		    mulvisualitem::TVisualAttribute aAttribute, int& aHeight, int& aWidth)
    {
    MUL_LOG_ENTRY_EXIT("aakash::MulCoverFlowWidget::GetIconSize");
    if((aAttribute == mulvisualitem::KMulIcon1) || 
            (aAttribute == mulvisualitem::KMulIndicator2))
        {
        mBaseElement->GetIconSize(aTemplateId,aAttribute,aHeight,aWidth);
        }
    else
        {
        __MUL_ASSERT( false,KInvalidArgument);
        }
    }
 
// ---------------------------------------------------------------------------
// GetIconSize
// ---------------------------------------------------------------------------
//	
void MulCoverFlowWidget::GetIconSize(mulvisualitem::TVisualAttribute aAttribute, 
        int& aHeight, int& aWidth)
    {
    MUL_LOG_ENTRY_EXIT("aakash::MulCoverFlowWidget::GetIconSize");
	__MUL_ASSERT( mMulModelAccessor != 0,KTemplateNotFound);
	if((aAttribute == mulvisualitem::KMulIcon1) || 
	        (aAttribute == mulvisualitem::KMulIndicator2))
	    {
	    mBaseElement->GetIconSize(mMulModelAccessor->Template(),aAttribute,
	            aHeight,aWidth);
	    }
	else
	    {
	    __MUL_ASSERT( false,KInvalidArgument);
	    }
    }

 // ---------------------------------------------------------------------------
 // makeInterface
 // ---------------------------------------------------------------------------
 //
IAlfInterfaceBase* MulCoverFlowWidget::makeInterface( const IfId& /*aType*/ )
    {
    return static_cast<IMulCoverFlowWidget*>( this );
    }

// ---------------------------------------------------------------------------
// setChildFocus
// ---------------------------------------------------------------------------
//
void MulCoverFlowWidget::setChildFocus( bool /*aFocus*/ )
    {
	// pure virtual function of base Class.
	// not usefull for this widget
    }

void MulCoverFlowWidget::setPresentation(const char* /*aFilePath*/)
	{
	// pure virtual function of base Class.
	// not usefull for this widget
	}
    
// ---------------------------------------------------------------------------
// AddEventHandler
// ---------------------------------------------------------------------------
//
void MulCoverFlowWidget::AddEventHandler( IAlfWidgetEventHandler& aObserver )
	{
	control()->addEventHandler( &aObserver );
	}
    
// ---------------------------------------------------------------------------
// RemoveEventHandler
// ---------------------------------------------------------------------------
//
void MulCoverFlowWidget::RemoveEventHandler( IAlfWidgetEventHandler& aObserver )
	{
	control()->removeEventHandler( aObserver );
	}
// ---------------------------------------------------------------------------
// ContainerLayout 
// ---------------------------------------------------------------------------
//
const CAlfLayout& MulCoverFlowWidget::ContainerLayout()
	{
	MUL_LOG_ENTRY_EXIT("aakash::MulCoverFlowWidget::ContainerLayout");
	CAlfLayout* layout=(CAlfLayout*)mBaseElement->findVisual(0);
	return *layout;
	} 	
// ---------------------------------------------------------------------------
// SetEmptyText
// ---------------------------------------------------------------------------
//
void MulCoverFlowWidget::SetEmptyText( const UString& aDefaultText)
    {
    MUL_LOG_ENTRY_EXIT("aakash::MulCoverFlowWidget::SetEmptyText");
    mBaseElement->SetEmptyText(aDefaultText);
    }

// ---------------------------------------------------------------------------
// SetMarkMode 
// ---------------------------------------------------------------------------
//
void MulCoverFlowWidget::SetMarkMode(bool /*aFlag*/)
	{
    // Marking mode is not supported by coverflow widget.
	}

// ---------------------------------------------------------------------------
// IsMarkMode 
// ---------------------------------------------------------------------------
//		
bool MulCoverFlowWidget::IsMarkMode()const 
	{
	// Marking mode is not supported by coverflow widget.
	return false;
	}

// ---------------------------------------------------------------------------
// SetReorderMode 
// ---------------------------------------------------------------------------
//
void MulCoverFlowWidget::SetReorderMode(TMulReOrderMode /*aReorderMode*/)
	{
	// reorder mode is not supported by coverflow widget.	
	}

// ---------------------------------------------------------------------------
// IsReorderMode 
// ---------------------------------------------------------------------------
//		
bool MulCoverFlowWidget::IsReorderModeOn() const
	{
	// reorder mode is not supported by coverflow widget.	
	return false;
	}
		
// ---------------------------------------------------------------------------
// SetOverlay 
// ---------------------------------------------------------------------------
//
void MulCoverFlowWidget::SetOverlay(TMulAttributeDataType /*aType*/, const UString& /*aAttribute*/, 
	const UString& /*aFormat*/)
	{
	// No implementation .
	}

// ---------------------------------------------------------------------------
// SetAnimationTime 
// ---------------------------------------------------------------------------
//	
void MulCoverFlowWidget::SetAnimationTime( TMulVisualEffect aVisualEffect, int aTransitionTime ) 
    {
    MUL_LOG_ENTRY_EXIT("aakash::MulCoverFlowWidget::SetAnimationTime");
    // EHighlightTransition is used to set coverflow step scroll animation time.
     if( aVisualEffect == EHighlightTransition )
        {
        mBaseElement->SetScrollAnimationTime(aTransitionTime);
        }
    }
// ---------------------------------------------------------------------------
// SetUIOnOFF
// ---------------------------------------------------------------------------
//    
void MulCoverFlowWidget::SetUIMode(bool UIOn, int aTransition )
    {
    MUL_LOG_ENTRY_EXIT("aakash::MulCoverFlowWidget::SetUIMode");
    mBaseElement->UIOnOffMode( UIOn,aTransition );    
    }

// ---------------------------------------------------------------------------
// SetVisualColor 
// ---------------------------------------------------------------------------
//
void MulCoverFlowWidget::SetVisualColor( TMulVisualType aVisualType , const TRgb& aColor )
	{
	MUL_LOG_ENTRY_EXIT("aakash::MulCoverFlowWidget::SetVisualColor");
    switch(aVisualType)
        {
        case EItemBackgroundBrush:
            {
            mBaseElement->SetItemBrush(aColor); 
            }
        break;
        case ECoverflowCounterVisual:
            {
            mBaseElement->SetTextColor(ECounterVisual,aColor);    
            }
        break;
        case ETextVisual:
            {
            mBaseElement->SetTextColor(ETilteDetailTextVisual,aColor);    
            }
        break;
        default:
        break;  	        

        }
	}

// ---------------------------------------------------------------------------
// SetDefaultImage 
// ---------------------------------------------------------------------------
//
void MulCoverFlowWidget::SetDefaultImage(int aTextureId)
	{
	MUL_LOG_ENTRY_EXIT("aakash::MulCoverFlowWidget::SetDefaultImage");
	mCoverflowControl->SetDefaultImage(aTextureId);	
	}

// ---------------------------------------------------------------------------
// RotateImage 
// ---------------------------------------------------------------------------
//
void MulCoverFlowWidget::RotateImage(TMulRotation aDirection,TSize aImageSize,int aAnimationTime)
	{
	MUL_LOG_ENTRY_EXIT("aakash::MulCoverFlowWidget::RotateImage");	
	mBaseElement->RotateImage(aDirection,aImageSize,aAnimationTime);
	}

// ---------------------------------------------------------------------------
// Gesturehelper 
// ---------------------------------------------------------------------------
//
GestureHelper::CGestureHelper* MulCoverFlowWidget::Gesturehelper()
    {
    return mCoverflowControl->Gesturehelper(); 
    }

    } //namespace alf
    
//End of File
