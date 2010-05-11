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
* Description:  Widget Control Implementation
 *
*/


//  Include Files

// Class Headers
#include "mulcoverflowcontrol.h"

#include <AknUtils.h>
// Alf Headers
#include <alf/alfenv.h>
// for dosetimage func
#include <alf/alfimageloaderutil.h>
#include <alf/alfbitmapprovider.h>
#include <alf/alfdisplay.h>
#include <alf/alfresourcepool.h> //for resource pooling
#include <alf/alfwidgetenvextension.h> //for resource pooling
#include <alf/alfutil.h>
#include <alf/alfimagevisual.h>

// Mul Headers
#include <mul/mulevent.h>
#include <mul/imulsliderwidget.h>


// Gesture Helper 
#include <gesturehelper.h>

// Local Headers
#include "mulbaseelement.h"
#include "mulcoverflowao.h"
#include "mulcoverflowtemplate.h" 
#include "mulcoverflowwidget.h"
#include "mulleave.h" //for leaving function
#include "mullog.h" //for logs
#include "mulassert.h"
#include "imulmodelaccessor.h"

//Gesture Helper
using namespace GestureHelper;

namespace Alf
	{
		
struct TMulCoverFlowControlImpl
	{
	TMulCoverFlowControlImpl()
		{
		mHighlightIndex = -1;
		mScreenWidth = 0;
		mScreenHeight = 0;
		mGestureSpeed  = 0;
		mCoverFlow2DTemplate = NULL;
		mIsWidgetVisible = true; //by default widget is visible .
		mBaseElement = NULL;
		mFastScroll = false;
		mNumVisibleItem = 0;
		mDefaultTextureId = -1;	
		mBounceInProgress = false;
		mDirectionChanged = false;
		mDontBounce = false;
		mNumberOfRepeatEvents = 0;
		mIsMirrored = false;
		}
			
    /// @bug critical:avanhata:4/7/2008 you should never cache a property that is a property of
    /// another class. The model exists to own this type of information. I gave this review comment
    /// as "critical", since it is exactly these cached member variables that will 
    /// yield most of bugs and complexity. Refactor it away.
	int mHighlightIndex;
    
	int mScreenWidth;
    int	mScreenHeight;
	bool mIsWidgetVisible;
	MulBaseElement* mBaseElement;	
	auto_ptr<MulCoverFlowTemplate> mCoverFlow2DTemplate;  // 2D cover flow template //owns 
	TReal32 mGestureSpeed;
	bool mFastScroll; 		// Flag to indicate wheher in fast scroll mode or not
	int mNumVisibleItem;
	int mDefaultTextureId;
	bool mBounceInProgress;
	int mReferencePoint;
	bool mDirectionChanged;
	int mCurrGestureDir; 
	bool mDontBounce;
	int mNumberOfRepeatEvents;
	bool mIsLandscape;
	bool mIsMirrored;
	};
	
// ---------------------------------------------------------------------------
// MulCoverFlowControl
// ---------------------------------------------------------------------------
//
MulCoverFlowControl::MulCoverFlowControl(CAlfEnv& aEnv	)
	{
	//Call the base class method
	MUL_LOG_ENTRY_EXIT("MUL:MulCoverFlowControl::MulCoverFlowControl");
	CAlfWidgetControl::construct ( aEnv);

	mData.reset( new (EMM)TMulCoverFlowControlImpl );
    
	mFeedback = MTouchFeedback::Instance(); 
	
    if (AknLayoutUtils::LayoutMirrored())
    	{
    	mData->mIsMirrored = true;
    	}
    else
		{
		mData->mIsMirrored = false;
		}
		
	mData->mIsLandscape = IsLandscape();

    THROW_IF_LEAVES
		(
        /// @bug critical:avanhata:30/9/2008 this is a symbian code section. don't call throwing osn code here. 
        /// Throwing within a TRAP is at best undefined
		mCoverFlowAo.reset( new(EMM) MulCoverFlowAo( *this ) );

		mHelper.reset(GestureHelper::CGestureHelper::NewL( *this ));
		mHelper->InitAlfredPointerCaptureL( aEnv, aEnv.PrimaryDisplay(), KGroupId ); 
		mHelper->SetHoldingEnabled( EFalse );
		)
	}
		
// ---------------------------------------------------------------------------
// ~MulCoverFlowControl
// ---------------------------------------------------------------------------
//	
MulCoverFlowControl::~MulCoverFlowControl()
	{
	MUL_LOG_ENTRY_EXIT("MUL:MulCoverFlowControl::~MulCoverFlowControl");

    if( ModelAccessor() != NULL )
        {
        ModelAccessor()->RemoveModelObserver(this);
        }

	DestroySlider();      

	}
    
// ---------------------------------------------------------------------------
// makeInterface
// ---------------------------------------------------------------------------
//          
IAlfInterfaceBase* MulCoverFlowControl::makeInterface( const IfId& aType )
    {
    MUL_LOG_ENTRY_EXIT("MUL:MulCoverFlowControl::makeInterface");
    UString param(aType.mImplementationId);
    return CAlfWidgetControl::makeInterface( aType );
    }

// ---------------------------------------------------------------------------
// ResetShowWidgetFlag
// ---------------------------------------------------------------------------
//  
void MulCoverFlowControl::ResetShowWidgetFlag( bool aNewResetValue )
    {
    MUL_LOG_ENTRY_EXIT("MUL:MulCoverFlowControl::ResetShowWidgetFlag");
    mData->mIsWidgetVisible = aNewResetValue;
    if( aNewResetValue)
        {
    	mHelper->AddObserver(this);
        } 
    }

// ---------------------------------------------------------------------------
// Template2D
// ---------------------------------------------------------------------------
//
MulCoverFlowTemplate*  MulCoverFlowControl::Template2D()
	{
	MUL_LOG_ENTRY_EXIT("MUL:MulCoverFlowControl::Template2D");
	return mData->mCoverFlow2DTemplate.get();
	}
		
// ---------------------------------------------------------------------------
// IsFastScrollMode
// ---------------------------------------------------------------------------
//	
bool MulCoverFlowControl::IsFastScrollMode()
	{
	MUL_LOG_ENTRY_EXIT("MUL:MulCoverFlowControl::IsFastScrollMode");
	return mData->mFastScroll;
	}	
	

//-----------------------Event Handling API's -------------------------------
// ---------------------------------------------------------------------------
// handleEvent
// ---------------------------------------------------------------------------
//	
AlfEventStatus MulCoverFlowControl::handleEvent(const TAlfEvent& aEvent)
	{
    MUL_LOG_ENTRY_EXIT("MUL:MulCoverFlowControl::handleEvent");
    if( aEvent.IsCustomEvent() )
        {
        return HandleCustomEvent( aEvent );
        } 
    else if( aEvent.IsKeyEvent() && mData->mIsWidgetVisible )
        {
        if(TotalModelCount() <= KMinNumItemForFastScroll || 
        	aEvent.KeyEvent().iRepeats == 0 )
	        {
	        return HandleKeyEvent( aEvent );	
	        }
	    else
	    	{	        
		    return HandleRepeatKeyEvent( aEvent );	
	    	}
        }      
 	else if( aEvent.IsPointerEvent() && mData->mIsWidgetVisible )
        {                 
		MUL_LOG_INFO("MUL::MulCoverFlowControl: handleEvent ::pointer event");
		//commented as gives warning in armv5 
		//THROW_IF_LEAVES
		     //(
		     TBool result = mHelper->OfferEventL(aEvent);
		     MUL_LOG_INFO1("MUL::MulCoverFlowControl: handleEvent::result %d ",result);
		     return result ? EEventConsumed :EEventNotHandled ;            
		    // )            
        }    
   	return EEventNotHandled;
    }
// ---------------------------------------------------------------------------
// HandleCustomEvent
// ---------------------------------------------------------------------------
//  
AlfEventStatus MulCoverFlowControl::HandleCustomEvent( const TAlfEvent& aEvent )
    {
    MUL_LOG_ENTRY_EXIT("MUL:MulCoverFlowControl::HandleCustomEvent");
    AlfEventStatus eventConsumed = EEventNotHandled;
    switch( aEvent.CustomParameter() )
        {
        case ECustomEventBounceBack:
            {
            MUL_LOG_INFO("MUL::MulCF:Handle custom event Compensate Bounce");
            mFeedback->InstantFeedback(ETouchFeedbackBasic); 
            mData->mBaseElement->StopDoodling(/*3*KBounceTime*/500);
            if ((TotalModelCount() > 1 && mData->mNumVisibleItem == 1) || 
            	(TotalModelCount() > 2))
	            {
	            Env().Send(TAlfCustomEventCommand(ECustomEventScroll, this),500/*3*KBounceTime*/ + 30); 	
	            }
	        else
	        	{
	        	//start marquee in case of text is visible.
	        	if(mData->mBaseElement->IsTextVisibile())
	        	    {
	        	    //start marquee after 1 sec (after doodle completed)
	        	    Env().Send(TAlfCustomEventCommand(
	        	            ECustomEventMarqueeStart, this),500/*3*KBounceTime*/ + KMarqueeTime1000);
	        	    }
	        	// if only one item is there then no need to scroll.
	        	mData->mBounceInProgress = false;	
	        	}
            eventConsumed = EEventConsumed;
            break;
            } 
        case ECustomEventScroll:
            {
            MUL_LOG_INFO("MUL::MulCF:Handle custom event Compensate Bounce");
            Env().Send(TAlfCustomEventCommand(ECustomEventBounceCompleted,this),600); 
            
            if (mData->mHighlightIndex == 0)
	            {
	            DoSetFocusIndex(TotalModelCount() -1,/*4*KBounceTime*/800);	
	            }
	        else
	        	{
	        	DoSetFocusIndex(0,800);	
	        	}
	        SetSliderTickPosition();
            eventConsumed = EEventConsumed;
            break;
            } 
        case ECustomEventBounceCompleted:
        	{
        	mData->mBounceInProgress = false;
			eventConsumed = EEventConsumed;
            break;        		
        	}
        case ECustomEventMarqueeStart:
        	{
        	mData->mBaseElement->StartMarquee(mulvisualitem::KMulTitle);
			eventConsumed = EEventConsumed;
			break;
        	}
        case ECustomEventTitleMarqueeFinished:
            {
            mData->mBaseElement->StopMarquee(mulvisualitem::KMulTitle);
            eventConsumed = EEventConsumed;
            break;
            }
        case ECustomEventDetailMarqueeStart:
            {
            mData->mBaseElement->StartMarquee(mulvisualitem::KMulDetail);
            eventConsumed = EEventConsumed;
            break;
            }
        case ECustomEventMarqueeFinished:
           {
           mData->mBaseElement->StopMarquee(mulvisualitem::KMulDetail);
    	   eventConsumed = EEventConsumed;
    	   break;
           }
        default: break;
        }  
    return eventConsumed;
    }

// ---------------------------------------------------------------------------
// HandleKeyEvent
// ---------------------------------------------------------------------------
//  
AlfEventStatus MulCoverFlowControl::HandleKeyEvent( const TAlfEvent& aEvent )
    {
    MUL_LOG_ENTRY_EXIT("MUL:MulCoverFlowControl::HandleKeyEvent");
    AlfEventStatus eventConsumed = EEventNotHandled;
   	if( aEvent.Code() == EEventKey)
     	{
     	switch ( aEvent.KeyEvent().iCode )
	        {
	        case EKeyEnter:
            // Event when selection key is pressed .( key event )
	        case EKeyDeviceA:
	        case EKeyDevice3: 
	            {
	            if(aEvent.KeyEvent().iRepeats == 0)
		            {
		            if(TotalModelCount() <= 0 && mData->mBaseElement->IsEmptyText() )           
			            {
			            SetSelection(mData->mHighlightIndex);
			            eventConsumed = EEventConsumed;
			            }
			        else if( TotalModelCount() > 0 )
			        	{
			        	SetSelection(mData->mHighlightIndex);
			            eventConsumed = EEventConsumed;	
			        	}
			        break;	
		            }
	            }
	        case EKeyRightArrow:
	            {
	            // Send event to inform that the Right Key is pressed
				if (mData->mIsMirrored)
					{
					HandleNavigationEvent ( EEventScrollLeft );
					}
				else
					{
					HandleNavigationEvent ( EEventScrollRight );
					}
	            eventConsumed = EEventConsumed;
	            break;
	            }
	        
	        case EKeyLeftArrow:
	            {
	            // Send event to inform that the Left Key is pressed
				if (mData->mIsMirrored)
					{
					HandleNavigationEvent ( EEventScrollRight );
					}
				else
					{
					HandleNavigationEvent ( EEventScrollLeft );
					}
	            eventConsumed = EEventConsumed;
	            break;
	            }
	        case EKeyBackspace:
	        //case EKeyDelete:
	            {
	            CAlfWidgetControl::processEvent( TAlfEvent( ETypeRemove,mData->mHighlightIndex));
	            eventConsumed = EEventConsumed;
	            break;
	            }
	        default:
	            {
	            break;
	            }
	     	}
    	}
        
   else if(aEvent.Code() == EEventKeyUp) 
	   {
	   mData->mNumberOfRepeatEvents = 0;
	   switch ( aEvent.KeyEvent().iScanCode )
	        {
			case EStdKeyRightArrow:
			case EStdKeyLeftArrow:
				{
				if(mData->mFastScroll)
					{
					HandleEnhancedStop();
			   		eventConsumed = EEventConsumed;	
					}				
		   		break;
				}
			default:
				{
				break;	
				}
	        }
	   }         
     
    return eventConsumed;
    }

// ---------------------------------------------------------------------------	
// HandleRepeatKeyEvent
// ---------------------------------------------------------------------------
//
AlfEventStatus MulCoverFlowControl::HandleRepeatKeyEvent( const TAlfEvent& aEvent )
    {
    MUL_LOG_ENTRY_EXIT("MUL:MulCoverFlowControl::HandleHoldGestureEvents");

	AlfEventStatus eventConsumed = EEventNotHandled;
   	if( aEvent.Code() == EEventKey)
     	{
     	switch ( aEvent.KeyEvent().iCode )
	        {
	        case EKeyRightArrow:
	            {
				if (mData->mIsMirrored)
					{
					HandleFastScrollWithKeyEvents(1);
					}
				else
					{
					HandleFastScrollWithKeyEvents(-1);
					}
	            eventConsumed = EEventConsumed;
	            break;
	            }
	        
	        case EKeyLeftArrow:
	            {
				if (mData->mIsMirrored)
					{
					HandleFastScrollWithKeyEvents(-1);
					}
				else
					{
					HandleFastScrollWithKeyEvents(1);
					}
	            eventConsumed = EEventConsumed;
	            break;
	            }
	        default:
	            {
	            break;
	            }
	     	}
    	}
	return eventConsumed;
    }

// ---------------------------------------------------------------------------	
// HandleFastScrollWithKeyEvents
// ---------------------------------------------------------------------------
//
void MulCoverFlowControl::HandleFastScrollWithKeyEvents(int aDirection)
    {
    MUL_LOG_ENTRY_EXIT("MUL:MulCoverFlowControl::HandleFastScrollWithKeyEvents");
    mData->mNumberOfRepeatEvents++;
    if(mData->mNumberOfRepeatEvents == 1 )
        {
        CAlfWidgetControl::processEvent( TAlfEvent( ETypeFastScroll,EScrollStart ));
		mData->mFastScroll = true;
		mCoverFlowAo->StartMoving(1, aDirection,true,200);	
        }
    else if(mData->mNumberOfRepeatEvents == KNumberOfRepeatsForMediumSpeed && mData->mFastScroll )
        {
        mCoverFlowAo->StartMoving(1, aDirection,true,150);	
        }
    else if(mData->mNumberOfRepeatEvents == KNumberOfRepeatsForFastSpeed && mData->mFastScroll )
        {
        mCoverFlowAo->StartMoving(1, aDirection,true,100);	
        }
    }      
    
// ---------------------------------------------------------------------------
// HandleNavigationEvent
// ---------------------------------------------------------------------------
//
void MulCoverFlowControl::HandleNavigationEvent( int aEvent )
	{
	MUL_LOG_ENTRY_EXIT("MUL:MulCoverFlowControl::HandleNavigationEvent");
	
	// During bounce ignore all the events
	if (mData->mBounceInProgress)
		{
		return;	
		}
		
	int highlightIndex = mData->mHighlightIndex;
	int totalcount = TotalModelCount();
	if(totalcount <= 0 && highlightIndex == -1 )
		{
		return;
		}
	//stop marquee if the navigation is happening.
	if(mData->mBaseElement->IsTextVisibile())
	    {
	    mData->mBaseElement->StopMarquee(mulvisualitem::KMulTitle);
	    mData->mBaseElement->StopMarquee(mulvisualitem::KMulDetail);
	    }
	Env().CancelCustomCommands(this);
	
	//Update the highlight index
	switch( aEvent )
		{
		case EEventScrollRight: 
			{
			// boundary check
			if( highlightIndex == totalcount - 1 )
				{
				if( !mData->mBounceInProgress && !mData->mDontBounce)
					{
					mData->mBounceInProgress = true;
					mData->mBaseElement->StartBounce(KBounceRight);
					}
				else
					{
					mData->mBounceInProgress = false;
					mData->mDontBounce = false;
					highlightIndex = 0;	
					}
				}
			else 
				{
				highlightIndex++;
				}
				
			mData->mBaseElement->SetScrollDir(EItemScrollRight);	
			}
		break;
		
		case EEventScrollLeft:
			{
			//boundary check
			if( highlightIndex == 0 )
				{
				if( !mData->mBounceInProgress && !mData->mDontBounce)
					{
					mData->mBounceInProgress = true;
				    mData->mBaseElement->StartBounce(KBounceLeft);
					}
				else
					{
					mData->mBounceInProgress = false;
					mData->mDontBounce = false;
					highlightIndex = totalcount - 1;	
					}
				}
			else
				{
				highlightIndex--;
				}	
			}
		mData->mBaseElement->SetScrollDir(EItemScrollLeft);	
		break;
		}
		
	// to handle bounce checks
	if( !mData->mBounceInProgress )	
		{ 
		DoSetFocusIndex( highlightIndex ); 	
		SetSliderTickPosition();
		}	   
	}
// ---------------------------------------------------------------------------
// SendEventToBaseElement
// ---------------------------------------------------------------------------
//
void MulCoverFlowControl::SendEventToBaseElement( const TAlfEvent& aEvent )
    {
    MUL_LOG_ENTRY_EXIT("MUL:MulCoverFlowControl::SendEventToBaseElement");
    mData->mBaseElement->offerEvent(  *this, aEvent);
    }

//----------------------------Highlight and Selection API's -----------------

// ---------------------------------------------------------------------------
// SetHighlightIndex
// ---------------------------------------------------------------------------
//          
void MulCoverFlowControl::SetHighlightIndex( int aIndex, bool aUpdateSlider)
    {
    MUL_LOG_ENTRY_EXIT("MUL:MulCoverFlowControl::SetHighlightIndex");
    // Throw Out of Bound Exception if index is negative
   __MUL_ASSERT (aIndex > -1 && aIndex < TotalModelCount(), KOutOfBound); 
    //set the highlight in model
    if(aIndex > mData->mHighlightIndex)
        {
        mData->mBaseElement->SetScrollDir(EItemScrollRight);	    
        }
    else
        {
        mData->mBaseElement->SetScrollDir(EItemScrollLeft);    
        }    
    DoSetFocusIndex( aIndex );  
    if (aUpdateSlider)
        {
        SetSliderTickPosition();	
        }
    }   
// ---------------------------------------------------------------------------
// DoSetFocusIndex
// ---------------------------------------------------------------------------
//  
void MulCoverFlowControl::DoSetFocusIndex( int aNewIndex, int aAnimationTime )
    {
    MUL_LOG_ENTRY_EXIT("MUL:MulCoverFlowControl::DoSetFocusIndex");
    
    if(aNewIndex < 0 )
	    {
	    return;	
	    }

    int oldindex = mData->mHighlightIndex;
    IMulModelAccessor* accessor = static_cast<IMulModelAccessor*>(widget()->model());
    if(accessor)
	    {
	    if( oldindex == aNewIndex )
	        {
            accessor->SetHighlight( aNewIndex ); 
            
            // When one item case to cancel the drag effect
            mData->mBaseElement->StopDoodling(200);            
	        return;
	        }
	    // handling of cancelling bounce in 2 items case
	    if(TotalModelCount() == 2)
		    {
		    if((mData->mBaseElement->ScrollDir() == EItemScrollRight && aNewIndex == 1 && oldindex == 2)||
		    	(mData->mBaseElement->ScrollDir() == EItemScrollLeft && aNewIndex == 2 && oldindex == 1))
			    {
			    mData->mBaseElement->StopDoodling(200); 
			    return;	
			    }
		    }
	 
	    accessor->SetHighlight( aNewIndex ); 
	    accessor->ScrollWindow( WindowTop(aNewIndex) );  

	    // Set the new focused item 
	    mData->mHighlightIndex = aNewIndex; 
	     
	   // Throw Out of Bound Exception if index is negative
	   __MUL_ASSERT( mData->mHighlightIndex > -1 , KOutOfBound ); 
	   mData->mBaseElement->offerEvent(*this,TAlfEvent(ETypeHighlight,aAnimationTime));
	    
	    if( mData->mIsWidgetVisible )
	        {
	        // If the widget is in hidden mode no event will be sent to the application/client
	        CAlfWidgetControl::processEvent( TAlfEvent( ETypeHighlight,mData->mHighlightIndex));
	        }
	    }
    }

// ----------------------------------------------------------------------------
// SetSelection
// ----------------------------------------------------------------------------
//
void MulCoverFlowControl::SetSelection(int aHighlight)    
    {
    MUL_LOG_ENTRY_EXIT("MUL:MulCoverFlowControl::SetSelection");
    CAlfWidgetControl::processEvent ( TAlfEvent ( ETypeSelect, aHighlight));
    }

// ---------------------------------------------------------------------------
// HighlightIndex
// ---------------------------------------------------------------------------
//  
int MulCoverFlowControl::HighlightIndex()
    {
    MUL_LOG_ENTRY_EXIT("MUL:MulCoverFlowControl::HighlightIndex");
    return mData->mHighlightIndex; 
    }  
      
// ---------------------------------------------------------------------------
// SetSliderTickPosition
// ---------------------------------------------------------------------------
//
void MulCoverFlowControl::SetSliderTickPosition( )
    {
    MUL_LOG_ENTRY_EXIT("MUL:MulCoverFlowControl::SetSliderTickPosition");
    if (mData->mCoverFlow2DTemplate->IsSliderVisible())
	    {
	    IMulSliderModel* mulSliderModel = GetSliderModel();	
	    if( mulSliderModel && mData->mHighlightIndex >= KInitialvalueZero )
	        {
	        mulSliderModel->SetPrimaryValue( mData->mHighlightIndex );   
	        }
	    }
    }
  
//----------------------Model Change Related API's----------------------------
// ----------------------------------------------------------------------------
// ModelStateChanged
// ----------------------------------------------------------------------------
//
void MulCoverFlowControl::ModelStateChanged( TMulChangedState aState, IMulVariantType& aData  )
    {  
    MUL_LOG_ENTRY_EXIT("MUL:MulCoverFlowControl::ModelStateChanged");
    switch( aState )
        {
        case EItemUpdated:
        	{
        	int index = aData.integer();
        	if(TotalModelCount() < 2*mData->mNumVisibleItem+1)
	        	{
	        	HandleLessItemsUpdate(index);	
	        	}
        	else
	        	{
	        	int relativeIndex = 0;
	        	if (IsIndexInVisibleWindow(index, relativeIndex))
		        	{
		        	UpdateCoverflowItem(index,relativeIndex);
		        	mData->mBaseElement->CancelRotationOnUpdate(relativeIndex);
		        	}	
	        	}
        	break;	
        	} 
       case EHighlightChanged:
            {
            if (mData->mHighlightIndex != aData.integer())
	            {
	            mData->mHighlightIndex = aData.integer();
	            IMulModelAccessor* accessor = static_cast<IMulModelAccessor*>(widget()->model());
                if( mData->mHighlightIndex == 0 )
                	{
                	mData->mBaseElement->ShowEmptyText(false);
				    }
                if( mData->mHighlightIndex >= 0)
                    {
                    // the same things have to be done for highlight>0 and highlight = 0
                    // the only diff is at highlight 0 dont show empty text 
                    accessor->ScrollWindow(WindowTop(mData->mHighlightIndex));
	           		mData->mBaseElement->UpdateVisuals();
	           		mData->mBaseElement->SetCounterText();	
	           		SetSliderTickPosition();
				    }           		
                else
                    {
                    // remove the visualisation and hide counter. 
                    mData->mBaseElement->RecycleIconVisuals();  
	                mData->mBaseElement->ShowEmptyText( true );
                    }				
	            }
            break;
            }
        case EItemsInserted:
            {
            HandleModelCountChange();
            break;
            }
        case EItemsRemoved:
        	{
        	HandleModelCountChange();
        	CAlfWidgetControl::processEvent( TAlfEvent( ETypeItemRemoved ));
        	break;
        	}
        
		}                                  
    }

// ---------------------------------------------------------------------------
// HandleLessItemsUpdate 
// ---------------------------------------------------------------------------
//    
void MulCoverFlowControl::HandleLessItemsUpdate(int aIndex)
	{
	MUL_LOG_ENTRY_EXIT("MUL:MulCoverFlowControl::HandleLessItemsUpdate");
	if(aIndex == mData->mHighlightIndex)
		{
		UpdateCoverflowItem(aIndex,mData->mNumVisibleItem);
		if(mData->mNumVisibleItem != 1 && TotalModelCount() == 3)	
			{
			UpdateCoverflowItem(aIndex,mData->mNumVisibleItem + 3);
			UpdateCoverflowItem(aIndex,mData->mNumVisibleItem - 3);	
			}
		}
	else 
		{
		// fulscreen tempalte and 2d in portrait
		if(mData->mNumVisibleItem == 1)	
			{
			UpdateCoverflowItem(aIndex,mData->mNumVisibleItem+1);
			UpdateCoverflowItem(aIndex,mData->mNumVisibleItem-1);				
			}
		else
			{
			// 2d template landscape mode
			if(TotalModelCount() == 2)
				{
				UpdateCoverflowItem(aIndex,mData->mNumVisibleItem+(aIndex - mData->mHighlightIndex));	
				}
			else
				{
				int relativeIndex = (aIndex - mData->mHighlightIndex) + mData->mNumVisibleItem;
				if(relativeIndex >= 0 && relativeIndex < 2*mData->mNumVisibleItem+1)
					{
					UpdateCoverflowItem(aIndex,relativeIndex);						
					}
				relativeIndex = relativeIndex + TotalModelCount();
				if(relativeIndex >= 0 && relativeIndex < 2*mData->mNumVisibleItem+1)
					{
					UpdateCoverflowItem(aIndex,relativeIndex);	
					}
								
				relativeIndex = mData->mNumVisibleItem - (TotalModelCount() - aIndex + mData->mHighlightIndex);
				if(relativeIndex >= 0 && relativeIndex < 2*mData->mNumVisibleItem+1)
					{
					UpdateCoverflowItem(aIndex,relativeIndex);					
					}
				relativeIndex = relativeIndex - TotalModelCount();
				if(relativeIndex >= 0 && relativeIndex < 2*mData->mNumVisibleItem+1)
					{
					UpdateCoverflowItem(aIndex,relativeIndex);	
					}					
				}
			}
		}
	}

// ---------------------------------------------------------------------------
// IsIndexInVisibleWindow 
// ---------------------------------------------------------------------------
//
bool MulCoverFlowControl::IsIndexInVisibleWindow(const int aIndex, int& aRelativeIndex)
    {
    MUL_LOG_ENTRY_EXIT("MUL:MulCoverFlowControl::IsIndexInVisibleWindow");

	int totalVisual = TotalModelCount();
    int noOfVisuals = mData->mNumVisibleItem; 
    int highlightRelativeIndex = mData->mNumVisibleItem;
    	
	int leftIndex = mData->mHighlightIndex - noOfVisuals;
	int rightIndex = mData->mHighlightIndex + noOfVisuals;
    
 	if (leftIndex >= 0)
	 	{
	 	if ((aIndex <= mData->mHighlightIndex) && (aIndex >= leftIndex))
		 	{
		 	aRelativeIndex = highlightRelativeIndex - (mData->mHighlightIndex - aIndex);
		 	return true;	
		 	}
	 	}
	else
		{
		if (aIndex <= mData->mHighlightIndex)
			{
			aRelativeIndex = highlightRelativeIndex - (mData->mHighlightIndex - aIndex);
			return true;	
			}
		else
			{
			leftIndex = totalVisual + leftIndex ;
			if (aIndex >= leftIndex)
				{
				aRelativeIndex = highlightRelativeIndex - noOfVisuals +	(aIndex - leftIndex);
				return true;
				}
			}	
		}
 
 	if (rightIndex < totalVisual)
	 	{
	 	if ((aIndex >= mData->mHighlightIndex) && (aIndex <= rightIndex))
		 	{
		 	aRelativeIndex = highlightRelativeIndex + (aIndex - mData->mHighlightIndex);
		 	return true;	
		 	}
	 	}
	else
		{
		if (aIndex >= mData->mHighlightIndex)
			{
			aRelativeIndex = highlightRelativeIndex + (aIndex - mData->mHighlightIndex);
			return true;	
			}
		rightIndex = rightIndex - totalVisual;
		if (aIndex <= rightIndex)
			{
			aRelativeIndex = highlightRelativeIndex + noOfVisuals - (rightIndex - aIndex);
			return true;	
			}
		}
 
	return false;
    }
        
        
// ---------------------------------------------------------------------------
// RelativeToAbsolute
// ---------------------------------------------------------------------------
//
int MulCoverFlowControl::RelativeToAbsolute(const int aRelativeIndex)
    {
    MUL_LOG_ENTRY_EXIT("MUL:MulCoverFlowControl::RelativeToAbsolute");
    int highlightRelativeIndex = mData->mNumVisibleItem;
    int absolute = -1;
    int totalVisuals = TotalModelCount();
    if(totalVisuals <= 0)
	    {
	    // absolute index is always -1, so no need of any calculations
	    }
    else if(totalVisuals < 2*mData->mNumVisibleItem + 1)
	    {
	    if(aRelativeIndex == highlightRelativeIndex)
			{
			absolute = 	mData->mHighlightIndex;
			}
		else
			{
			if(mData->mNumVisibleItem == 1)
				{
				if (totalVisuals == 1)
				    {
				    absolute = -1;
				    }
				else if(totalVisuals == 2)
					{
					absolute = mData->mHighlightIndex + 1;
					absolute = absolute >= totalVisuals ? 0 : absolute;
					}
				}
			else
				{
				if (totalVisuals == 1)
				    {
				    absolute = -1;
				    }
				else if(totalVisuals == 2)
					{
					absolute = mData->mHighlightIndex + (aRelativeIndex - highlightRelativeIndex);
					absolute = absolute >= totalVisuals ? -1 : absolute;
					}
				else // totalvisuals > 3
					{
					if(aRelativeIndex - highlightRelativeIndex > 0)
						{
						absolute = mData->mHighlightIndex + aRelativeIndex - highlightRelativeIndex;	
						}
					else
						{
						absolute = mData->mHighlightIndex + aRelativeIndex - highlightRelativeIndex
							+ TotalModelCount();	
						}					
					absolute = absolute >= totalVisuals ? absolute - totalVisuals : absolute;
					}
				}
			}
	    }
	else
		{
		if(aRelativeIndex == highlightRelativeIndex)
			{
			absolute = 	mData->mHighlightIndex;
			}
		else if (aRelativeIndex > highlightRelativeIndex)
		    {
		    absolute = mData->mHighlightIndex + (aRelativeIndex - highlightRelativeIndex);
		    absolute = absolute >= totalVisuals ? (absolute - totalVisuals) : absolute;	
		    }
		else
			{
		    absolute = mData->mHighlightIndex - (highlightRelativeIndex - aRelativeIndex);
		    absolute = absolute <0 ? (totalVisuals + absolute) : absolute;
		    absolute = absolute > totalVisuals - 1 ? -1 : absolute;	    
			}	
		}    
    return absolute;
    }

        
// ---------------------------------------------------------------------------
// TotalModelCount
// ---------------------------------------------------------------------------
//  
int MulCoverFlowControl::TotalModelCount()
    {
    MUL_LOG_ENTRY_EXIT("MUL:MulCoverFlowControl::TotalModelCount");
    
    IMulModelAccessor* accessor = static_cast<IMulModelAccessor*>(widget()->model());
    if(accessor)
        {
        return accessor->CurrentItemCount();
        }
	return -1;    
    }

// ---------------------------------------------------------------------------
// HandleModelCountChange
// ---------------------------------------------------------------------------
//
void MulCoverFlowControl::HandleModelCountChange()
	{
	MUL_LOG_ENTRY_EXIT("MUL:MulCoverFlowControl::HandleModelCountChange");
    IMulModelAccessor* accessor = static_cast<IMulModelAccessor*>(widget()->model());
    if(accessor)
	    {
	    if(TotalModelCount() < (2*mData->mNumVisibleItem + 1))
	    	{
	    	mData->mBaseElement->UpdateVisuals();
	    	}
		mData->mBaseElement->UpdateSliderTick( TotalModelCount() );
		mData->mBaseElement->SetCounterText();
	    }
	}

// ---------------------------------------------------------------------------
// modelAccessor
// ---------------------------------------------------------------------------
//  
IMulModelAccessor* MulCoverFlowControl::ModelAccessor()
    {
    MUL_LOG_ENTRY_EXIT("MUL:MulCoverFlowControl::ModelAccessor");
    if(widget())
    	{
    	return static_cast<IMulModelAccessor*>(widget()->model());
    	}    
    return NULL;    
    }

// ---------------------------------------------------------------------------
// ModelChanged
// ---------------------------------------------------------------------------
//
void MulCoverFlowControl::ModelChanged( IMulModelAccessor* aAccessor )
	{
	MUL_LOG_ENTRY_EXIT("MUL:MulCoverFlowControl::ModelChanged");
	Env().CancelCustomCommands(this);
	// update the highlight index as well as the no of item.
	mData->mHighlightIndex = aAccessor->Highlight();

	CAlfLayout* main = (CAlfLayout*) mData->mBaseElement->findVisual( KMainLayoutIndex );
	int width = main->Size().Target().AsSize().iWidth;
	int height = main->Size().Target().AsSize().iHeight;

	if( width == 0 && height == 0 )
		{
		return;
		}
	else
		{
		// @todo check if we are setting a new model with the same old template, 
		// then should we call CreateTemplateElement
		mData->mScreenWidth = width;
    	mData->mScreenHeight = height;
		CreateTemplateElement(aAccessor->Template());
		mData->mBaseElement->CreateAndInitializeVisuals(); 
		aAccessor->SetVisibleWindow( 2 * mData->mCoverFlow2DTemplate->MaxVisibleCount() + 1
		            ,WindowTop(mData->mHighlightIndex));
		// For setting or clearing empty text if depending on the new modle count
		SendEventToBaseElement( TAlfEvent( EEventWidgetInitialized ) );
		}
	}
		


//-----------------Template Element Creation API's --------------------------

// ---------------------------------------------------------------------------
// CreateTemplateElement
// ---------------------------------------------------------------------------
//
void MulCoverFlowControl::CreateTemplateElement( mulwidget::TLogicalTemplate aTemplateName)
	{
	MUL_LOG_ENTRY_EXIT("MUL:MulCoverFlowControl::CreateTemplateElement");
	if (aTemplateName == mulwidget::KTemplate1)
		{
		mData->mCoverFlow2DTemplate.reset( new(EMM) MulCoverFlowTemplate1(*this, *mData->mBaseElement));
		}
	else if(aTemplateName == mulwidget::KTemplate4)
		{
		mData->mCoverFlow2DTemplate.reset( new(EMM) MulCoverFlowTemplate4(*this, *mData->mBaseElement));
		}
	else
		{
		// Invalid template
		return;
		}
    mData->mCoverFlow2DTemplate->CreateVisualisation(IsLandscape());
	// set holding mode to gesture helper.
	SetHoldingEnabled(); 
	}


    
//--------------Orientation Change Related API's----------------------------- 
// ---------------------------------------------------------------------------
// VisualLayoutUpdated
// ---------------------------------------------------------------------------
//		
void MulCoverFlowControl::VisualLayoutUpdated (CAlfVisual& aVisual)
    {
    MUL_LOG_ENTRY_EXIT("MUL:MulCoverFlowControl::VisualLayoutUpdated");
        
    TSize topLayoutSize = aVisual.Size().Target().AsSize(); ;
    int height = topLayoutSize.iHeight;
    int width  = topLayoutSize.iWidth;
    
 	// to avoid unnecessary visual layout updated calls .
 	// in order to check if the orientation has changed really or not this is done .
    if( mData->mScreenWidth == width && height == mData->mScreenHeight  )
        {
        return;
        }
    
    mData->mScreenWidth = width;
    mData->mScreenHeight = height;
        
	
	IMulModelAccessor* accessor = ModelAccessor();
    if(IsLandscape() == mData->mIsLandscape)
        {  
        if(accessor)
            {
            CreateTemplateElement(accessor->Template());
            mData->mBaseElement->CreateAndInitializeVisuals(); 
            accessor->SetVisibleWindow( 2 * mData->mCoverFlow2DTemplate->MaxVisibleCount() + 1
                        ,WindowTop(mData->mHighlightIndex));
            // For setting or clearing empty text if depending on the new modle count
            SendEventToBaseElement( TAlfEvent( EEventWidgetInitialized ) );
            }
        else
        	{
        	// For null model
		    // reset the coverflow 2d template to NULL
		    mData->mCoverFlow2DTemplate.reset(NULL);
        	}
        }
    else
        { 
        mData->mIsLandscape = IsLandscape();
    	
    	if(!accessor)
    		{
    		mData->mBaseElement->SetDefaultSize(TSize(mData->mScreenWidth,mData->mScreenHeight));
    		return; 
    		}
          
        mData->mBaseElement->OrientationChange();
    	// enable or disable hold events depending on whether ECF enabled
    	SetHoldingEnabled();
        }
        
    }
	    
// ---------------------------------------------------------------------------
// RecycleVisuals
// ---------------------------------------------------------------------------
//   
void MulCoverFlowControl::RecycleVisuals()
    {
    MUL_LOG_ENTRY_EXIT("MUL:MulCoverFlowControl::RecycleVisuals");
    // reset the coverflow 2d template to NULL
    mData->mCoverFlow2DTemplate.reset(NULL);
    
    if(mData->mBaseElement)
        {
        mData->mBaseElement->RecycleIconVisuals();
        mData->mHighlightIndex = -1;
        }
    }  

//--------------------------- Gesture ----------------------------------------

// ---------------------------------------------------------------------------
// SetHoldingEnabled
// ---------------------------------------------------------------------------
// 
void MulCoverFlowControl::SetHoldingEnabled()
	{
	MUL_LOG_ENTRY_EXIT("MUL:MulCoverFlowControl::SetHoldingEnabled");

	mHelper->SetHoldingEnabled(EnhancedModeCondition());	
	}

// ---------------------------------------------------------------------------
// SetDoubleTapEnabled
// ---------------------------------------------------------------------------
// 
void MulCoverFlowControl::SetDoubleTapEnabled( bool aValue)
	{
	MUL_LOG_ENTRY_EXIT("MUL:MulCoverFlowControl::SetDoubleTapEnabled");

	mHelper->SetDoubleTapEnabled( aValue );	
	}
	
// ---------------------------------------------------------------------------
// HandleGestureL
// ---------------------------------------------------------------------------
//    
void MulCoverFlowControl::HandleGestureL( const MGestureEvent& aEvent )
    {
    MUL_LOG_ENTRY_EXIT("MUL:MulCoverFlowControl::HandleGestureL");
	if(aEvent.IsHolding())
		{
	    HandleHoldGestureEvents(aEvent);	
		}
	else
		{
		HandleNormalGestureEvents(aEvent);
		}
    }

// ---------------------------------------------------------------------------
// HandleNormalGestureEvents
// ---------------------------------------------------------------------------
//
void MulCoverFlowControl::HandleNormalGestureEvents( const MGestureEvent& aEvent )
    {
    MUL_LOG_ENTRY_EXIT("MUL:MulCoverFlowControl::HandleNormalGestureEvents");
    
    switch ( aEvent.Code(MGestureEvent::EAxisHorizontal) )
        {
        case EGestureStart:
            {
            SendFeedbackOnTouchDown(aEvent);
            }
            break;
		case EGestureDrag: // fall through
			{		
			MUL_LOG_INFO("MUL::MulCoverFlowControl:HandleNormalGestureEvents EGestureDrag");
			if(	aEvent.CurrentPos().iX > mData->mScreenWidth )
				{
				return;
				}

			if(TotalModelCount() > 0 )
				{
				// if ui is on send event to applications	
				if( mData->mCoverFlow2DTemplate->IsUiOnOffFlagEnabled() && 
	    			(mData->mBaseElement->IsUiOnMode()) )
					{
					CAlfWidgetControl::processEvent( TAlfEvent( ETypeSwitchUiStateOnDrag ));	
					}
					
				if (mData->mBounceInProgress && TotalModelCount() > 2)
					{
					mData->mBounceInProgress = false;
					Env().CancelCustomCommands(this);
					// bounce canceled once .dont bounce again for the next swipe.
					mData->mDontBounce = true;	
					}

				int distance =  aEvent.Distance().iX ;
				mData->mCurrGestureDir = distance;//CovertDistAnceToDirection(distance);				
				if (mData->mIsMirrored)
					{
					distance = -distance;
					}
				mData->mReferencePoint = aEvent.CurrentPos().iX - 10; //pointerDownPos + distanceTravelled/2;	

				mData->mBaseElement->StartDoodling(distance);
				}
			break;
			}
        
        case EGestureTap:
        	{
        	MUL_LOG_INFO("MUL::MulCoverFlowControl:HandleNormalGestureEvents EGestureTap ");
            HandleSingleTap(aEvent); 
            mFeedback->InstantFeedback(ETouchFeedbackList);
            break;   
        	}
        
        case EGestureDoubleTap:
           {
           MUL_LOG_INFO("MUL::MulCoverFlowControl:HandleNormalGestureEvents EGestureDoubleTap ");
           if(TotalModelCount() <= 0 )
	    		{
	    		//find out whether event should be sent on double tap or not 
	    		// in case of empty model and the feedback too.
	    		break; 
	    		}
            MulDoubleTapData doubleTapData;
            // Finding the visual which has been hit .
            CAlfVisual* hitVisual = aEvent.Visual();           
            if(hitVisual && (hitVisual->Tag() == KCoverflowIndicator) ) 
                {
                doubleTapData.mTapVisualIndex = mData->mHighlightIndex ;
                }
            else
                {
                int hitVisualIndex = GetHitVisualIndex( hitVisual );           
                if( hitVisualIndex == -1 )
                    {
                    break; 
                    }
                doubleTapData.mTapVisualIndex = hitVisualIndex;
                }
            doubleTapData.mDoubleTapPoint = aEvent.CurrentPos();
            
            mFeedback->InstantFeedback(ETouchFeedbackList); 
            CAlfWidgetControl::processEvent ( TAlfEvent ( ETypeDoubleTap,
                    uint(&doubleTapData)));
            break;  
            }
                           	
        case EGestureUnknown :
        	{
        	MUL_LOG_INFO("MUL::MulCoverFlowControl:HandleNormalGestureEvents EGestureUnknown");
        	if(TotalModelCount() > 0 )
	    		{
		    	// this will revert to the original position if gesture get cancelled
	        	// with some animation time.
	        	// @TODO animation time should be same as set by application.
	        	mData->mBaseElement->StopDoodling(200);
	    		}
            break;
        	}
                    
        case EGestureSwipeLeft:
        	{
        	MUL_LOG_INFO("MUL::MulCoverFlowControl:HandleNormalGestureEvents EGestureSwipeLeft");
        	if(TotalModelCount() > 0 )
	    		{
	    		// do the calculations for changing teh higlight left or right depnding on last direction
			    // and the total direction moved.
			    int finalSwipeDirection = mData->mBaseElement->FinalSwipeDirection(
			    	TotalSwipeDistance(aEvent),EEventScrollRight);

	    		HandleSwipe(finalSwipeDirection);
	    		}
            break;
        	}
              
        case EGestureSwipeRight:
            {
            MUL_LOG_INFO("MUL::MulCoverFlowControl:HandleNormalGestureEvents EGestureSwipeRight");
            if(TotalModelCount() > 0 )
	    		{
			    int finalSwipeDirection = mData->mBaseElement->FinalSwipeDirection(
			    	TotalSwipeDistance(aEvent),EEventScrollLeft);
	    		
	    		HandleSwipe(finalSwipeDirection);
	    		}
            break;
            }
        case EGestureMultiTouchStart:
           {
            MUL_LOG_INFO("MUL::MulCoverFlowControl:HandleNormalGestureEvents EGestureMultiTouchStart");
            mData->mBaseElement->StopDoodling(0);
            mFeedback->InstantFeedback(ETouchFeedbackMultiTouchRecognized);
            break;
          }
        case EGestureMultiTouchReleased:
            MUL_LOG_INFO("MUL::MulCoverFlowControl:HandleNormalGestureEvents EGestureMultiTouchReleased");
            break;
        case EGesturePinch:
            MUL_LOG_INFO("MUL::MulCoverFlowControl:HandleNormalGestureEvents EGesturePinch");
            CAlfWidgetControl::processEvent ( TAlfEvent ( ETypePinch,aEvent.PinchPercent()));
            break;
        case EGestureReleased:
        	{
        	 MUL_LOG_INFO("MUL::MulCoverFlowControl:HandleNormalGestureEvents EGestureReleased");
        	 break;
			}
        default:
            break;
        } // end of switch
    }

// ---------------------------------------------------------------------------
// TotalSwipeDistance
// ---------------------------------------------------------------------------
//
int MulCoverFlowControl::TotalSwipeDistance( const GestureHelper::MGestureEvent& aEvent )
	{
	MUL_LOG_ENTRY_EXIT("MUL:MulCoverFlowControl::TotalSwipeDistance");
	if(	aEvent.CurrentPos().iX > mData->mScreenWidth )
		{
		return aEvent.Distance().iX - (aEvent.CurrentPos().iX - mData->mScreenWidth);
		}
	return 	aEvent.Distance().iX;
	}

// ---------------------------------------------------------------------------
// GetHitVisualIndex
// ---------------------------------------------------------------------------
//
int MulCoverFlowControl::GetHitVisualIndex( CAlfVisual* aHitVisual )
    {
    MUL_LOG_ENTRY_EXIT("MUL:MulCoverFlowControl::GetHitVisualIndex");
    int visualIndex  = -1;
        
    if(!aHitVisual)
	    {
	    // no visual found
	    return -1;
	    }
     #ifdef _DEBUG
        const TDesC8& buffer = aHitVisual->Tag();
    #endif

    if ((aHitVisual->Tag() != KCoverflowIcon))
        {
        visualIndex = -1;
        }
    else
        {
        if(mData->mNumVisibleItem > 1)
	        {
    		CAlfLayout& iconFlow =mData->mBaseElement->FlowLayout( KIconFlowLayoutIndex );
    		CAlfVisual* imgVisual = NULL;
	        for(int i=2; i<=4;i++)	
		        {
		        imgVisual = iconFlow.Visual(i).FindTag(KCoverflowIcon);
		        if (imgVisual == aHitVisual)
			        {
			        visualIndex = RelativeToAbsolute(i);	
			        break;
			        }
		        }
	        }
	    else
	    	{
	    	visualIndex = mData->mHighlightIndex;
	    	}
        }
    return visualIndex;
    }


// ---------------------------------------------------------------------------
// HandleSingleTap
// ---------------------------------------------------------------------------
//  
void MulCoverFlowControl::HandleSingleTap(const MGestureEvent& aEvent) 
    {
    MUL_LOG_ENTRY_EXIT("MUL:MulCoverFlowControl::HandleSingleTap");

    if(TotalModelCount() <= 0 )
        {
        // in case of empty model and null model only when the empty text is set 
        // then only the selection event should be sent to the applications.
        if( mData->mBaseElement->IsEmptyText() )
            {
            SetSelection(-1);	
            }
        // In case of null model and empty model if the empty text is not set then no need to calculate
        //the hit visual index . Doest serve any purpose.
        }
    else
        {
        // Finding the visual which has been hit .
        CAlfVisual* hitVisual = aEvent.Visual();        	
        if(hitVisual && (hitVisual->Tag() == KCoverflowIndicator) ) 
            {
            CAlfWidgetControl::processEvent( TAlfEvent( EVideoIconSelect ));
            return;
            }
        int hitVisualIndex = GetHitVisualIndex( hitVisual );
        if( hitVisualIndex > -1 )
            {
            if( hitVisualIndex == mData->mHighlightIndex )
                {
                SetSelection(mData->mHighlightIndex );
                }
            else
                {
                //set the highlight in model
			    if(TotalModelCount() > 2 && hitVisualIndex == TotalModelCount()-1 
			    	&& mData->mHighlightIndex == 0)
				    {
				    HandleNavigationEvent ( EEventScrollLeft );	
				    }
				else if(TotalModelCount() > 2 && mData->mHighlightIndex == TotalModelCount()-1 
					&& hitVisualIndex == 0)
					{
					HandleNavigationEvent ( EEventScrollRight );	
					}
			    else if(hitVisualIndex > mData->mHighlightIndex)
			        {
			        HandleNavigationEvent ( EEventScrollRight );	    
			        }
			    else
			        {
			        HandleNavigationEvent ( EEventScrollLeft );   
			        }
                }
            }	
        }   
    }
     
// ---------------------------------------------------------------------------
// SendFeedbackOnTouchDown
// ---------------------------------------------------------------------------
//  
void MulCoverFlowControl::SendFeedbackOnTouchDown(const MGestureEvent& aEvent)
    {
    MUL_LOG_ENTRY_EXIT("MUL:MulCoverFlowControl::SendFeedbackOnTouchDown");

    IMulModelAccessor* modelaccessor = ModelAccessor();
    if(!modelaccessor ||  modelaccessor->CurrentItemCount() == 0)
        {
        return;   
        }
    CAlfVisual* hitVisual = aEvent.Visual();
    if((hitVisual && hitVisual->Tag() == KCoverflowIcon) || (mData->mCoverFlow2DTemplate->IsUiOnOffFlagEnabled() )) 
        {
        mFeedback->InstantFeedback(ETouchFeedbackList);  
        }    
    }
// ---------------------------------------------------------------------------
// HandleSwipe
// Handle the swipe events (swipe left and swipe right) provided by the gesture helper.
// Also handles the extreme doodling case (it may happen that in doodling you can move two
// or three highlight before doing a pointer up) but gesture only gives us a swipe left and 
// swipe right event. so we have to decide the no of highlight movement to do while getting 
// left or right event. 
// Also for 2 or 3 highlight movement if the 1st or 2nd highlight movement is the cycling case
// then we have to cancel the bounce as its already happend because of swipe. But the bounce
// will happen if the final highlight change is a cyclic case.
// ---------------------------------------------------------------------------
//  
void MulCoverFlowControl::HandleSwipe(int aEvent)
    {
    MUL_LOG_ENTRY_EXIT("MUL:MulCoverFlowControl::HandleSwipe");
    int event;
    if(aEvent == EItemScrollLeft)
	    {
		if (mData->mIsMirrored)
			{
		    event = EEventScrollRight;
			}
		else
			{
			event = EEventScrollLeft;
			}
	    }
	else if(aEvent == EItemScrollRight)
		{
		if (mData->mIsMirrored)
			{
		    event = EEventScrollLeft;
			}
		else
			{
			event = EEventScrollRight;
			}
		}
	else
		{
		mData->mBaseElement->StopDoodling(200);	
		return;
		}
		
	if(mData->mBaseElement->NumberOfSwipes() == EDoubleSwipe || mData->mBaseElement->NumberOfSwipes() == ETripleSwipe)
		{
		// done to avoid undesirable ui behaviour when we move the pointer little back.
		// check if it can be done in another way.
		CAlfEnv& env = Env();
		TAlfRefreshMode oldRefreshMode = env.RefreshMode();
		env.SetRefreshMode(EAlfRefreshModeManual);
		if(mData->mBaseElement->NumberOfSwipes() == ETripleSwipe)
			{
			HandleNavigationEvent( event );	
			}
		HandleNavigationEvent( event );
		env.SetRefreshMode(oldRefreshMode);			
		}
	
	HandleNavigationEvent( event );
	// Feedback for swipe event
	mFeedback->InstantFeedback(ETouchFeedbackList);
    }

// ---------------------------------------------------------------------------	
// HandleHoldGestureEvents
// ---------------------------------------------------------------------------
//
void MulCoverFlowControl::HandleHoldGestureEvents( const MGestureEvent& aEvent )
    {
    MUL_LOG_ENTRY_EXIT("MUL:MulCoverFlowControl::HandleHoldGestureEvents");

    switch ( aEvent.Code(MGestureEvent::EAxisHorizontal) )
        {
        case EGestureDrag: 
            {		
            MUL_LOG_INFO("MUL::MulCoverFlowControl:HandleHoldGestureEvents EGestureDrag");
            
            ReverseDirectionInFastScroll(aEvent);
            
            /// @todo The strip direction intially should start with the current gesture direction.
            // But if u keep dragging without releasing the pointer tehn the directioon should chnage whenever
            // the pointer crosses the centre of the screen.
            
            // if the pointer has crossed the centre position after holding started then the
            // direction should depend on which half of teh screen
            // else
            // the direction should depend on the swipe direction
			StartMoving(aEvent);		
	        break;
            }

        case EGestureReleased:
        case EGestureTap:
        	{
        	MUL_LOG_INFO("MUL::MulCoverFlowControl:HandleHoldGestureEvents EGestureReleased");
 	        HandleEnhancedStop();
	        break;
        	}	    	
	            
        case EGestureHoldLeft:
        case EGestureHoldRight:
        	{
        	MUL_LOG_INFO("MUL::MulCoverFlowControl:HandleHoldGestureEvents EGestureHoldLeft");
			if( EnhancedModeCondition() ) 
            	{
            	EnhancedStarted( aEvent );
                }
            break;
        	}
        	
        default:
            break;
        } // end of switch
    
    }
  
// ---------------------------------------------------------------------------
// ReverseDirectionInFastSrroll
// ---------------------------------------------------------------------------
//    
void MulCoverFlowControl::ReverseDirectionInFastScroll(const MGestureEvent& aEvent)
    {
    MUL_LOG_ENTRY_EXIT("MUL:MulCoverFlowControl::ReverseDirectionInFastScroll");

    int distTravelled = aEvent.Distance().iX;
    int pointerDownPos = aEvent.StartPos().iX;
    int currPosition = aEvent.CurrentPos().iX;
    int prevGestureDir = ConvertDistanceToDirection(mData->mCurrGestureDir);

    // if the distance travelled is negative i.e from right to left and the start position is on
    // the right half of the screen and the direction has not changed yet , then special case to handle reference point. and vice versa
    // case for left half of the screen.
    if( ((distTravelled < 0 && pointerDownPos > mData->mScreenWidth/2 ) || (distTravelled > 0 && 
    	pointerDownPos < mData->mScreenWidth/2 )) && mData->mReferencePoint != mData->mScreenWidth/2) 
        {
        if( mData->mDirectionChanged )
            {
            return;
            }
            
        ChangeDirReferencePoint(currPosition,distTravelled);
        }
    else
        {
        // else the reference point will be the middle of the screen.
        
        // if the direction is not equal .. just check whether it has crossed the reference point.
        // how to check whether it has crossed the reference point.
        if(( prevGestureDir < 0 &&  currPosition > mData->mReferencePoint )||(prevGestureDir > 0 && currPosition < mData->mReferencePoint))
            {
            mData->mCurrGestureDir *= -1;
            }
          
        mData->mReferencePoint = mData->mScreenWidth/2;	
        }    
    }
    
// ---------------------------------------------------------------------------
// ChangeDirReferencePoint
// ---------------------------------------------------------------------------
//    
void MulCoverFlowControl::ChangeDirReferencePoint(int aCurrPosition, int aDistanceTravelled )
    {
    MUL_LOG_ENTRY_EXIT("MUL:MulCoverFlowControl::ChangeDirReferencePoint");
   
    int prevGestureDir = ConvertDistanceToDirection(mData->mCurrGestureDir);
    int currGestureDir =  ConvertDistanceToDirection(aDistanceTravelled);
    
    if( prevGestureDir != currGestureDir ) 
        {
        // if the direction is not equal .. just check whether it has crossed the reference point.
        // how to check whether it has crossed the reference point.
        if(( prevGestureDir < 0 &&  aCurrPosition > mData->mReferencePoint )||(prevGestureDir > 0 && aCurrPosition < mData->mReferencePoint))
            {
            //crossed the reference point first time .now no need to maintain the reference point .its always half of the screen
            mData->mReferencePoint = mData->mScreenWidth/2;
            mData->mDirectionChanged = true;
            mData->mCurrGestureDir = aDistanceTravelled;
            }
        else
            {
            // not crossed the reference point yet
            // still going in the same direction .
            mData->mReferencePoint = aCurrPosition - 10;	
            }
        }
    else
        {
        // still going in the same direction .
        if(( prevGestureDir < 0 &&  aCurrPosition < mData->mScreenWidth/2 )||(prevGestureDir > 0 
        	&& aCurrPosition > mData->mScreenWidth/2))
            {
            mData->mReferencePoint = mData->mScreenWidth/2;			
            }
        else if(Abs(mData->mCurrGestureDir) < Abs(aDistanceTravelled)  )
            {
            // moving in the same direction in the increasing side towards the middle of the screen but hasnt 
            // crossed the reference point yet.
            mData->mReferencePoint = aCurrPosition - 10;		
            }
        }
    }
    
// ---------------------------------------------------------------------------
// ConvertDistanceToDirection
// ---------------------------------------------------------------------------
//
int MulCoverFlowControl::ConvertDistanceToDirection( int aDistance )
	{
	MUL_LOG_ENTRY_EXIT("MUL:MulCoverFlowControl::ConvertDistanceToDirection");
	return aDistance>0?1:-1;
	}      

// ---------------------------------------------------------------------------
// HandleEnhancedStop
// ---------------------------------------------------------------------------
//
void MulCoverFlowControl::HandleEnhancedStop()
	{
	MUL_LOG_ENTRY_EXIT("MUL:MulCoverFlowControl::HandleEnhancedStop");
        					
	CAlfWidgetControl::processEvent( TAlfEvent( ETypeFastScroll,EScrollStop ));
	mCoverFlowAo->StopMoving();
	mData->mFastScroll = false;
	mData->mGestureSpeed = 0;	
    // @TODO animation time should be same as set by the application.
    mData->mBaseElement->MoveVisuals(0,200);
	}
	
// ---------------------------------------------------------------------------
// FastScrollTransitionTime
// ---------------------------------------------------------------------------
// 	
int MulCoverFlowControl::FastScrollTransitionTime()
	{
	MUL_LOG_ENTRY_EXIT("MUL:MulCoverFlowControl::FastScrollTransitionTime");
	if (mData->mBounceInProgress)	
		{
		return 800;	
		}
	else
		{
		return mCoverFlowAo->FastScrollTransitionTime();
		}
	}	
	

// ---------------------------------------------------------------------------
// EnhancedModeCondition
// ---------------------------------------------------------------------------
//
bool MulCoverFlowControl::EnhancedModeCondition()
	{
	MUL_LOG_ENTRY_EXIT("MUL:MulCoverFlowControl::EnhancedMode");
	if( !mData->mCoverFlow2DTemplate.get() )
		{
		return false;
		}
	UString orientation  = IsLandscape() ? UString(KLandscape)  :  UString(KPortrait);

	// If EMulWidgetFlagFastScroll is not set, 
	// or the current orientation is not landscape
	// or in teh current template enhanced mode is not supported
	if( !(((MulCoverFlowWidget*)widget())->IsFlagSet(IMulMultiItemWidget::EMulWidgetFlagFastScroll) 
		&& !orientation.compare(KLandscape) 
		&& mData->mCoverFlow2DTemplate->EnhancedTagParsed()) )
		{		
		return false;
		}
	// If template4(i.e. ui on off mode is anabled) then if ui on mode then ecf canot be launched,so return false		
	else if( mData->mCoverFlow2DTemplate->IsUiOnOffFlagEnabled() && 
    		(mData->mBaseElement->IsUiOnMode()) )
		{
		return false;	
		}
    else
	    {
	    return true; // enhanced mode enabled
	    }
	}

// ---------------------------------------------------------------------------
// EnhancedStarted
// ---------------------------------------------------------------------------
// 	
void MulCoverFlowControl::EnhancedStarted(const GestureHelper::MGestureEvent& aEvent )
	{
	MUL_LOG_ENTRY_EXIT("MUL:MulCoverFlowControl::EnhancedStarted");
	CAlfWidgetControl::processEvent( TAlfEvent( ETypeFastScroll,EScrollStart ));
	mData->mFastScroll = true;
	StartMoving(aEvent);			
	}

// ---------------------------------------------------------------------------
// StartMoving
// ---------------------------------------------------------------------------
// 
void MulCoverFlowControl::StartMoving( const GestureHelper::MGestureEvent& aEvent )
	{
	MUL_LOG_ENTRY_EXIT("MUL:MulCoverFlowControl::StartMoving");

	TRealPoint speedPoint = GestureSpeed( aEvent );
	int distance =  mData->mCurrGestureDir;
	if (mData->mIsMirrored)
		{
		distance = -distance;
		}
	mCoverFlowAo->StartMoving(speedPoint.iX, ConvertDistanceToDirection(distance) );
	mData->mGestureSpeed = speedPoint.iX;
   	}

// ---------------------------------------------------------------------------
// GestureSpeed
// ---------------------------------------------------------------------------
//
TRealPoint MulCoverFlowControl::GestureSpeed( const MGestureEvent& aEvent )
    {
	MUL_LOG_ENTRY_EXIT("MUL:MulCoverFlowControl::GestureSpeed");    
    
    TRect area(this->DisplayArea());
    TRect rect(TPoint(0,0),TPoint(area.Width(),area.Height())); 
    return aEvent.SpeedPercent( rect );
    }	

//--------------------Slider related functions----------------------------

// ---------------------------------------------------------------------------
// CreateSliderWidget
// ---------------------------------------------------------------------------
//
IAlfWidget* MulCoverFlowControl::CreateSliderWidget()
	{
	MUL_LOG_ENTRY_EXIT("MUL:MulCoverFlowControl::CreateSliderWidget");
	
	IAlfWidgetFactory& widgetFactory = AlfWidgetEnvExtension::widgetFactory(Env());
	IAlfWidget* sliderWidget = widgetFactory.findWidget( KNameSliderWidget );
	
	if( sliderWidget )
		{
		mSliderWidget = static_cast<IMulSliderWidget*>(sliderWidget);
		mData->mBaseElement->UpdateSliderTick( 1 );
		}
	else
		{
		// Get the template Id in Integer from your template data 
		//NOTE: Set template takes int as parameter not UString
		
		// Slider Widget creation
		mSliderWidget = widgetFactory.createWidget<IMulSliderWidget> ( KNameSliderWidget, KNameSliderWidget, *widget()->parent(), NULL);
		IMulSliderModel* mSliderModel = widgetFactory.createModel<IMulSliderModel> ("mulslidermodel");
		mSliderWidget->setModel(mSliderModel, true);	
		mSliderModel->SetTemplate(ESliderTemplate1);
		mSliderWidget->SetHandleKeyEvent(false);	
		mSliderWidget->control()->disableState(IAlfWidgetControl::Focusable); 
		}
	
	IAlfWidgetEventHandler* coverFlowEventHandler = static_cast<IAlfWidgetEventHandler*> (
	        mData->mBaseElement->makeInterface( IAlfWidgetEventHandler::type() ) );
	
	//Register the slidereventhandler and coverfloweventhandler with one another
	mSliderWidget->control()->addEventHandler((coverFlowEventHandler));
			
	return mSliderWidget;	
	}
	
// ---------------------------------------------------------------------------
// GetSliderWidget
// ---------------------------------------------------------------------------
//	
IAlfWidget* MulCoverFlowControl::GetSliderWidget()
	{
	MUL_LOG_ENTRY_EXIT("MUL:MulCoverFlowControl::GetSliderWidget");

	IAlfWidgetFactory& widgetFactory = AlfWidgetEnvExtension::widgetFactory(*(CAlfEnv::Static()));
	return widgetFactory.findWidget( KNameSliderWidget );
	}		    		    		

// ---------------------------------------------------------------------------
// GetSliderModel
// ---------------------------------------------------------------------------
//	
IMulSliderModel* MulCoverFlowControl::GetSliderModel()
	{
	MUL_LOG_ENTRY_EXIT("MUL:MulCoverFlowControl::GetSliderModel");
	if( mSliderWidget )
		{
		IMulSliderWidget* mulSliderWidget = static_cast<IMulSliderWidget*>(mSliderWidget);
		IMulSliderModel* mulSliderModel = static_cast<IMulSliderModel*>(mulSliderWidget->model());
		return mulSliderModel;
		}
	return NULL;
	}
	
// ---------------------------------------------------------------------------
// DestroySlider
// ---------------------------------------------------------------------------
//  
void MulCoverFlowControl::DestroySlider()
	{
	MUL_LOG_ENTRY_EXIT("MUL:MulCoverFlowControl::DestroySlider");

    MulBaseElement* baseelement = static_cast<MulBaseElement*>(findElement(KBase));
    if(baseelement)
    	{
    	baseelement->RemoveSliderFromLayout();
    	}
        
	IAlfWidgetFactory& widgetFactory = AlfWidgetEnvExtension::widgetFactory(Env());
	IAlfWidget* sliderWidget = widgetFactory.findWidget( KNameSliderWidget );
	if( sliderWidget )
		{  
		IAlfElement* baseelement = findElement(KBase);
		if(baseelement)
			{
		
			IAlfWidgetEventHandler* coverFlowEventHandler = static_cast<IAlfWidgetEventHandler*> (
		          baseelement->makeInterface( IAlfWidgetEventHandler::type() ) );
		         
		    if( sliderWidget->control() )
				{
				sliderWidget->control()->removeEventHandler(*(coverFlowEventHandler));	
				}	
			
			}
		}		
	}

// ---------------------------------------------------------------------------
// UpdateItemAtIndex
// ---------------------------------------------------------------------------
//
void MulCoverFlowControl::UpdateItemAtIndex(const int aRelativeIndex, int aAnimationTime)	
	{
	MUL_LOG_ENTRY_EXIT("MUL:MulCoverFlowControl::UpdateItemAtIndex");
	int absoluteIndex = RelativeToAbsolute(aRelativeIndex);
	if(absoluteIndex >= 0)
		{
		UpdateCoverflowItem( absoluteIndex,aRelativeIndex, aAnimationTime );	
		}
	else
		{
		SetBlankTexture(aRelativeIndex);	
		}	
	}

// ---------------------------------------------------------------------------
// SetBlankTexture
// ---------------------------------------------------------------------------
//
void MulCoverFlowControl::SetBlankTexture(int aRelativeIndex)
	{
	MUL_LOG_ENTRY_EXIT("MUL:MulCoverFlowControl::SetBlankTexture");
    CAlfFlowLayout& iconFlow = static_cast<CAlfFlowLayout&>(mData->mBaseElement->FlowLayout( KIconFlowLayoutIndex ));
	
	CAlfDeckLayout& layout = static_cast<CAlfDeckLayout&>(iconFlow.Visual(aRelativeIndex));
	CAlfImageVisual* visual =static_cast<CAlfImageVisual*>(layout.FindTag(KCoverflowIcon));
	
    CAlfTextureManager& textureMgr = Env().TextureManager();
    TAlfImage blankTexture = textureMgr.BlankTexture();
    visual->SetImage(blankTexture);
    
    // remove the icon brush if any.
    mData->mBaseElement->RemoveBrushOnIcon(*visual);        
    
    CAlfImageVisual* indicatorVisual =static_cast<CAlfImageVisual*>(layout.FindTag(KCoverflowIndicator));    
    if(indicatorVisual)
        {
        indicatorVisual->SetImage(blankTexture);   
        }
	}

// ---------------------------------------------------------------------------
// UpdateCoverflowItem
// ---------------------------------------------------------------------------
//
void MulCoverFlowControl::UpdateCoverflowItem( int aItemIndex,int aVisualIndex, int aAnimationTime )
	{
	MUL_LOG_ENTRY_EXIT("MUL:MulCoverFlowControl::UpdateCoverflowItem");
    
    CAlfFlowLayout& iconFlow = static_cast<CAlfFlowLayout&>(mData->mBaseElement->FlowLayout( KIconFlowLayoutIndex ));
	IMulModelAccessor* accessor = static_cast<IMulModelAccessor*>(widget()->model()); 
		
	try
		{
		const MulVisualItem& item = accessor->Item(aItemIndex);

		CAlfDeckLayout& layout = static_cast<CAlfDeckLayout&>(iconFlow.Visual(aVisualIndex));
		CAlfImageVisual* visual =static_cast<CAlfImageVisual*>(layout.FindTag(KCoverflowIcon));
		IMulVariantType* varData = item.Attribute(mulvisualitem::KMulIcon1);
		if(varData && visual)
			{
			DoSetImage(varData,visual);
			mData->mBaseElement->ApplyScaleMode(*visual);
			}
		else
			{
			if (mData->mDefaultTextureId >= 0)
				{
				const CAlfTexture* texture = Env().TextureManager().Texture (mData->mDefaultTextureId);
				if(visual)
				    {
				    visual->SetImage (TAlfImage (*texture));
				    }
				mData->mBaseElement->ApplyScaleMode(*visual);	
				}
			else
				{
				if(visual)
				    {        
				    visual->SetImage(Env().TextureManager().BlankTexture());
				    }
				}
			}
		if(mData->mCoverFlow2DTemplate->IsUiOnOffFlagEnabled() )
    		{
    		mData->mBaseElement->DisplayIndicatorIcon(item,aVisualIndex);			
    		}
		}
	catch(...)
		{
		CAlfDeckLayout& layout = static_cast<CAlfDeckLayout&>(iconFlow.Visual(aVisualIndex));		
		CAlfImageVisual* visual =static_cast<CAlfImageVisual*>(layout.FindTag(KCoverflowIcon));
		if (mData->mDefaultTextureId >= 0)
			{
			const CAlfTexture* texture = Env().TextureManager().Texture (mData->mDefaultTextureId);
			visual->SetImage (TAlfImage (*texture));
			mData->mBaseElement->ApplyScaleMode(*visual);	
			}
		else
			{
		    visual->SetImage(Env().TextureManager().BlankTexture());	
			}			
		
		}//catch ends
	// if the item is the highlight item then update the text if required.	
	if (aVisualIndex == mData->mNumVisibleItem)		
		{
		mData->mBaseElement->UpdateTextValue(aAnimationTime);
		}
	}
	

// ---------------------------------------------------------------------------
// DoSetImage
// ---------------------------------------------------------------------------
//			
void MulCoverFlowControl::DoSetImage(IMulVariantType* aData,CAlfImageVisual* aImgVisual)
	{
	MUL_LOG_ENTRY_EXIT("MUL:MulCoverFlowControl::DoSetImage");

	if ( aData && aData->Type() == IMulVariantType::EDes)
		{
		TAlfImage image = AlfWidgetEnvExtension::resourcePool(
			aImgVisual->Env()).getImageResource((char*)(aData->DesC().Ptr()));
		if(image.HasTexture())
			{
			aImgVisual->SetImage(image);
			}
		else if(IsSVGImage(aData->DesC()))
			{
			LoadImageFromSvg(aData->DesC(),*aImgVisual);
			}
		else
			{
			// check if the texture is already loaded
			CAlfTextureManager& textureMgr = aImgVisual->Env().TextureManager();
			const TInt existingTextureId = textureMgr.TextureId( aData->DesC() );
			const CAlfTexture* texture = NULL;  

			if ( existingTextureId != KErrNotFound )
				{
				texture = textureMgr.Texture( existingTextureId );
				}
			else
				{
				TRAPD( err, texture = &( textureMgr.LoadTextureL(aData->DesC(),
				EAlfTextureFlagLoadAnimAsImage, KAlfAutoGeneratedTextureId ) ) );               
				if( err != KErrNone )
					{
					// means the texture is invalid.
					//draw a blank texture here.
					texture = &( textureMgr.BlankTexture() );
					}                           
				}                                   
			aImgVisual->SetImage( TAlfImage( *texture ) ); 
			}
		}
	else if ( aData && aData->Type ()== IMulVariantType::EInt)
		{
		const CAlfTexture* texture = Env().TextureManager().Texture (aData->integer());
		aImgVisual->SetImage (TAlfImage (*texture));	
		}
	}
	
// ---------------------------------------------------------------------------
// IsSVGImage	
// ---------------------------------------------------------------------------
//
bool MulCoverFlowControl::IsSVGImage(const TDesC& aImagePath)
	{
	MUL_LOG_INFO("MUL:MulCoverFlowControl::IsSVGImage");
    _LIT(KSvgFile,".svg");

	return  (KErrNotFound != aImagePath.FindC(KSvgFile));
	}
	
// ---------------------------------------------------------------------------
// LoadImageFromSvg	
// ---------------------------------------------------------------------------
//
void MulCoverFlowControl::LoadImageFromSvg(const TDesC& aImagePath,
											CAlfImageVisual& aImageVisual )
	{
	MUL_LOG_INFO("MUL:MulCoverFlowControl::LoadImageFromSvg");
	
	CAlfImageLoaderUtil imgLoaderUtil;
	imgLoaderUtil.SetSize (TSize (50, 50));

	MAlfBitmapProvider* iSvgBitMapProv= imgLoaderUtil.CreateSVGImageLoaderL (aImagePath);
	CAlfTexture &texture = aImageVisual.Env().TextureManager().CreateTextureL (
							KAlfAutoGeneratedTextureId,
							iSvgBitMapProv,
							EAlfTextureFlagLoadAnimAsImage);
	aImageVisual.SetImage (TAlfImage (texture));
	
	}

// ---------------------------------------------------------------------------
// StoreVisibleItemCount	
// ---------------------------------------------------------------------------
//		
void MulCoverFlowControl::StoreVisibleItemCount(int aVisibleItemCount)
	{
	MUL_LOG_ENTRY_EXIT("MUL:MulCoverFlowControl::StoreVisibleItemCount");
	mData->mNumVisibleItem = aVisibleItemCount;
	}

// ---------------------------------------------------------------------------
// SetDefaultImage	
// ---------------------------------------------------------------------------
//
void MulCoverFlowControl::SetDefaultImage(int aTextureId)
	{
	MUL_LOG_ENTRY_EXIT("MUL:MulCoverFlowControl::SetDefaultImage");
	mData->mDefaultTextureId = aTextureId;
	}

// ---------------------------------------------------------------------------
// WindowTop	
// ---------------------------------------------------------------------------
//
int MulCoverFlowControl::WindowTop(int aHighlightIndex)
	{
	MUL_LOG_ENTRY_EXIT("MUL:MulCoverFlowControl::WindowTop");
	int visibleItems = 2 * mData->mNumVisibleItem + 1;
	int windowTop = aHighlightIndex - (visibleItems / 2);
	IMulModelAccessor* accessor = static_cast<IMulModelAccessor*>(widget()->model());
    if(accessor)
        {
    	windowTop = (windowTop + visibleItems - 1) >= TotalModelCount() ? 
    		TotalModelCount() - visibleItems : windowTop;
    	windowTop = windowTop > 0 ? windowTop : 0;
        }

	return windowTop;
	}

// ---------------------------------------------------------------------------
// UpdateBaseElement	
// ---------------------------------------------------------------------------
//
void MulCoverFlowControl::UpdateBaseElement(MulBaseElement* aBaseElement)
	{
	MUL_LOG_ENTRY_EXIT("MUL:MulCoverFlowControl::UpdateBaseElement");
	mData->mBaseElement = aBaseElement;
	}

// ---------------------------------------------------------------------------
// IsLandscape	
// ---------------------------------------------------------------------------
//
bool MulCoverFlowControl::IsLandscape()
	{
	bool ret;
	TSize ScreenSz = AlfUtil::ScreenSize();
	if (ScreenSz.iWidth > ScreenSz.iHeight)
		{
		ret = true;
		}
	else
		{
		ret = false;
		}
	return ret;
	}

// ---------------------------------------------------------------------------
// Gesturehelper  
// ---------------------------------------------------------------------------
//
GestureHelper::CGestureHelper* MulCoverFlowControl::Gesturehelper()
    {
    return mHelper.get(); 
    }

	} // namespace Alf

//End of file
