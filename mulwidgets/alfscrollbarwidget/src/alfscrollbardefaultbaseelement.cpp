/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  The implementation for presentation elements.
*
*/

//Toolkit Includes
#include <alf/alfimagevisual.h>
#include <alf/alfevent.h>
#include <alf/alfenv.h>
#include <AknUtils.h>
//AknLayout2 includes
#include <aknlayoutscalable_uiaccel.cdl.h>

// Alfred Client includes
#include <alf/alfroster.h>
#include <alf/alfdisplay.h>

//Widget Model Includes
#include <alf/alflayout.h>
#include "alf/alfelement.h"
#include <alf/alfwidgeteventhandler.h>
#include <alf/alfwidgetenvextension.h>
#include <alf/ialfmodel.h>

//scrollbar widget includes
#include <alf/alfscrollevents.h>
#include "alfscrollbardefaultbaseelement.h"
#include "alfscrollbarwidget.h"
#include "alfscrollbarcontrol.h"
#include "alfscrollbarmodel.h"
#include "alfscrollbarlongtaptimer.h"

#include <math.h>

enum 
    {
    EReleased,
    ETrackPressed,
    EThumbPressed
    };    

// EPSILON is an small constant used to compare two real values
const float KEpsilon  = 0.000001;

namespace Alf
    {

// ---------------------------------------------------------------------------
//  A function to compare two real values. The difference less than aEpsilon
//  is assumed negligible.
// ---------------------------------------------------------------------------
//
inline int realCompare(float aReal1, float aReal2, float aEpsilon)
    {
    if (Abs(aReal1-aReal2) < aEpsilon)
        {
        return 0;
        }
    else if ((aReal1-aReal2) > 0)
        {
        return 1;
        }
    else
        {
        return -1;
        }
    };  
    
struct ScrollBarBaseElementData
    {
    TOperation mOperation;              // Thumb Drag
    CAlfVisual* mVisual;                // Hit visual
    TPoint mDragPoint;                  // Current pointer value
    TPoint mLongTapPos;                 // Long Tap Point
    int mDragStartStep;                 // Value of view start position when drag starts
    int mCurrentState;                  // track or thumb clicked state indicator
    bool mLongTapStarted;               // Long Tap Timer started ( true if started) 
   
    int mViewLength;                    // For avoiding thumb length calculation 
                                        // everytime when model data changes
                                        // Stores current viewLength
    
    int mTotalLength;                   // For avoiding thumb length calculation 
                                        // everytime when model data changes
                                        // Stores current spanLength
    
    float mWidgetOpacity;               // default value =1.0.
    float mThumbOpacity;                // default value =1.0. thumb will eventually have
                                        // opacity=iWidgetOpacity*iThumbOpacity.
    
    int mThumbAnimationTime;            // thumb animation time in milliseconds.
    float mThumbLength;                 // Length of scrollbar thumb
    float mThumbMinLength;              // Minimum Length of thumb
    float mThumbMaxLength;              // Maximum Length of thumb
    float mStepSize;                    // Step Size
    bool mThumbSizeChanged;             // flag to indicate whether thumbSize is Changed.
    int mExtendedTouch;                 // extended area width.
    AlfScrollBarModel* mScrollbarModel; // Pointer to scrollbar model
   
    CAlfLayout* mRootLayout ;           // Layouts and visuals
    CAlfLayout* mTrackLayout ;
    CAlfLayout* mExtendedTrackLayout;
    CAlfLayout* mThumbLayout ;
    CAlfLayout* mExtendedThumbLayout;
    
    CAlfImageVisual* mTrackTopImage ;
    CAlfImageVisual* mTrackMiddleImage ;
    CAlfImageVisual* mTrackEndImage ;
    
    CAlfImageVisual* mThumbTopImage ;
    CAlfImageVisual* mThumbMiddleImage ;
    CAlfImageVisual* mThumbEndImage ;
    };//End of struct ScrollBarBaseElementData
        
struct ScrollbarThumbLCTData
    {
    TInt left;      //used to specify left value
    TInt top;       //used to specify top value
    TInt right;     //used to specify right value
    TInt bottom;    //used to specify bottom value
    TInt height;    //used to specify height value
    TInt width;     //used to specify width value
    };

// ---------------------------------------------------------------------------
//  C++ default constructor.
// ---------------------------------------------------------------------------
//
AlfScrollBarDefaultBaseElement::AlfScrollBarDefaultBaseElement(
                                    CAlfWidgetControl& aControl,
                                    const char* aName, 
                                    DuiNode* /*mNode*/,
                                    AlfCustomInitDataBase* /*aCustomData*/)
    : mData(NULL)
    {
    try
        {
        // Base class (AlfElement) construct
        construct(aControl, aName);   
          
        mLongTapTimer.reset( 
            new (EMM) AlfScrollBarLongTapTimer(this));
        
        mIsLongTapObserverSet=EFalse;
        //For Imperative construct
        createDefaultBaseElement();
        }
    catch(AlfException& e)
        {
        aControl.removeElement(*this);
        throw;
        }
    // AlfScrollBarDefaultBaseElement is an event handler too so it 
    // should be added to the control
    aControl.addEventHandler(this);
    }

// ---------------------------------------------------------------------------
//  Destructor
// ---------------------------------------------------------------------------
//
AlfScrollBarDefaultBaseElement::~AlfScrollBarDefaultBaseElement()
    {
    
    if(mData)
        {
        delete mData;
        }
    else
        {
        
        }
    if(mLCTData)
        {
        delete mLCTData;
        }
    else
        {
        
        }
    }//End of Destructor

//--------------------------------------------------------------------------
// APIs from the IAlfScrollBarDefaultBaseElement for controlling the 
// visualization of the scrollbar
//--------------------------------------------------------------------------


// ---------------------------------------------------------------------------
// Sets the thumb animation time.
// @aTime     : Time in miliseconds
// @exception : for aTime less then O.0 miliseconds
// ---------------------------------------------------------------------------
//
void AlfScrollBarDefaultBaseElement::setThumbAnimationTime ( int aTime )
    {
    // Sanity check
    if (aTime < 0)
    	{
    	ALF_THROW(AlfException, EInvalidArgument, "AnimationTime is invalid");
    	}
    else
    	{
    	// do nothing
    	}	
    
    if(mData->mThumbAnimationTime != aTime)
    	{
    	mData->mThumbAnimationTime = aTime;
    	}
    }

// ---------------------------------------------------------------------------
// From class IAlfScrollBarWdget.
// ---------------------------------------------------------------------------
//
int AlfScrollBarDefaultBaseElement::getThumbAnimationTime() const
    {
    return mData->mThumbAnimationTime;
    }

// ---------------------------------------------------------------------------
// Sets opacity value for the Thumb.
// Thumb opacity is relative to the track.
// @param     : aOpacity float value between 0.0 to 1.0
// @exception : value of aOpacity more than 1.0 and less than 0.0
// ---------------------------------------------------------------------------
//
void AlfScrollBarDefaultBaseElement::setThumbOpacity (float aOpacity)
    {
    // Sanity check
   if (realCompare(aOpacity, 0.0, KEpsilon) < 0
       || realCompare(aOpacity, 1.0, KEpsilon) > 0)
       {
       ALF_THROW(AlfException, EInvalidArgument, "Opacity is invalid");
       }
   else
       {
       // do nothing
       }
    // if there is a change, update the visualization
    if (aOpacity != mData->mThumbOpacity)
        {
        mData->mThumbOpacity = aOpacity;
        const TAlfTimedValue opacity(mData->mThumbOpacity * mData->mWidgetOpacity);
        if(mData->mThumbLayout)
            {
            mData->mThumbLayout->SetOpacity(opacity);
            control().Visual(0).UpdateChildrenLayout();
            }
        }
    }//End of setThumbOpacity

// ---------------------------------------------------------------------------
// Returns Thumb opacity is relative to the track.
// ---------------------------------------------------------------------------
//
float AlfScrollBarDefaultBaseElement::getThumbOpacity() const
    {
    return mData->mThumbOpacity;
    }

// ---------------------------------------------------------------------------
// Sets opacity value for the widget.
// 
// @param     : aOpacity float value between 0.0 to 1.0
// @exception : value of aOpacity more than 1.0 and less than 0.0
// ---------------------------------------------------------------------------
//
void AlfScrollBarDefaultBaseElement::setOpacity (float aOpacity)
    {
    // Sanity check
   if (realCompare(aOpacity, 0.0, KEpsilon) < 0
       || realCompare(aOpacity, 1.0, KEpsilon) > 0)
       {
       ALF_THROW(AlfException, EInvalidArgument, "Opacity is invalid");
       }
   else
       {
       // do nothing
       }
    // if there is a change, update the visualization
    if (aOpacity != mData->mWidgetOpacity)
        {
        mData->mWidgetOpacity = aOpacity;
        const TAlfTimedValue opacity(mData->mWidgetOpacity);
        if(mData->mTrackLayout)
            {
            mData->mRootLayout->SetOpacity(opacity);
            control().Visual(0).UpdateChildrenLayout();
            }
        }
    }//End of setThumbOpacity

// ---------------------------------------------------------------------------
// Returns Widget opacity is relative to the track.
// ---------------------------------------------------------------------------
//
float AlfScrollBarDefaultBaseElement::getOpacity() const
    {
    return mData->mWidgetOpacity;
    }

//--------------------------------------------------------------------------
// APIs from the IAlfScrollBarBaseElement 
//--------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// Gets called when the parent layout is relayouted.
// resets all the anchor points and updates the visualization
// ---------------------------------------------------------------------------
//
void AlfScrollBarDefaultBaseElement::baseLayoutSizeChanged()
    {
    initializeLayout();    
    }

// ---------------------------------------------------------------------------
// processPointerEvent generates custom events 
// according the focus visual .
// ---------------------------------------------------------------------------
//
bool AlfScrollBarDefaultBaseElement::processPointerEvent(TAlfEvent * aPntrEvent)
    {
    // retrieve pointer event (NULL check not required here)
    // Get the hit visual
    CAlfVisual *focusvisual =aPntrEvent->Visual();
    
    if ( aPntrEvent->IsPointerEvent()  )
        {
        if (aPntrEvent->PointerEvent().iType == TPointerEvent::EButton1Down)
            {
            bool ret = handlePointerDownEvent(aPntrEvent);	
            return ret;
            }
        else if (aPntrEvent->PointerEvent().iType == TPointerEvent::EDrag)
            {
            if(!mData->mOperation) 
                {
                // Drag during tap and hold on track 
                if(mIsLongTapObserverSet)
                    {
                    mData->mLongTapPos=aPntrEvent->PointerEvent().iParentPosition;
                    }
                else
                    {
                    //Do Nothing.
                    }    
                }
            else
                {
                // Drag on thumb
                if(mIsLongTapObserverSet &&
                   mData->mVisual && (isScrollbarThumbVisual(focusvisual)) )
                    {
                    mData->mLongTapPos= aPntrEvent->PointerEvent().iParentPosition ;
                    }
                else
                    {
                    // Do nothing
                    }
                int dist = aPntrEvent->PointerEvent().iParentPosition.iY - mData->mDragPoint.iY;
                int steps = ceil(dist / mData->mStepSize) + mData->mDragStartStep;
                                                
                // checking steps to avoid negative value, incase of max scrollbar length
                if(  steps >  mData->mScrollbarModel->getTotalLength() -  mData->mScrollbarModel->getViewLength()) 
                     {
                     steps =  mData->mScrollbarModel->getTotalLength() -  mData->mScrollbarModel->getViewLength(); 
                     }   
               else if(steps <  0 )   
                     {
                     steps =  0;
                     } 
                if(steps !=  mData->mScrollbarModel->getViewStartPosition())
                    {
                    // send tactile sensitive event
                    //control().processEvent(TAlfEvent(EEventScrollBarDrag));
                    TAlfEvent customevent(EEventDragVertical,(TInt )steps);
                    control().processEvent(customevent);
                    }
                // Tactile Feedback for Thumb drag
                AlfScrollBarControl& slbrcntrl = static_cast<AlfScrollBarControl&>(control());
                slbrcntrl.TactileEffectOnDrag(*aPntrEvent);
                }    
            }
        
        else if(aPntrEvent->PointerUp()  )
            {
            mData->mLongTapStarted = false;
            handlePointerUpEvent();
            }
        else
            {
            //Do Nothing.
            }
    }
    
    return true;
    }//End of processPointerEvent

void AlfScrollBarDefaultBaseElement::createVisualization()
    {
    initializeScrollModel();
    CAlfWidgetControl  *ctrl = &control();
    
    // Create Root Layout
    mData->mRootLayout = CAlfLayout::AddNewL(*((CAlfControl*)ctrl),NULL);
    mData->mRootLayout->SetFlag(EAlfVisualFlagLayoutUpdateNotification);
    mData->mRootLayout->SetFlag( EAlfVisualFlagAutomaticLocaleMirroringEnabled );	
                       
    // Create Track Layout
    mData->mTrackLayout = CAlfLayout::AddNewL(*((CAlfControl*)ctrl),mData->mRootLayout);
    mData->mTrackLayout->SetFlag( EAlfVisualFlagAutomaticLocaleMirroringEnabled );
    mData->mExtendedTrackLayout = CAlfLayout::AddNewL(*((CAlfControl*)ctrl),mData->mTrackLayout);
    
    if(mData->mTrackLayout)
        {
        mData->mTrackTopImage = CAlfImageVisual::AddNewL(
            *((CAlfControl*)ctrl),mData->mTrackLayout);
        mData->mTrackMiddleImage = CAlfImageVisual::AddNewL(
            *((CAlfControl*)ctrl),mData->mTrackLayout);
        mData->mTrackEndImage = CAlfImageVisual::AddNewL(
            *((CAlfControl*)ctrl),mData->mTrackLayout);
            
        if(mData->mTrackTopImage && 
           mData->mTrackMiddleImage && 
           mData->mTrackEndImage)
            {
            setTrackImage();
            }
         
        // Create Thumb Visuals    
        mData->mThumbLayout = CAlfLayout::AddNewL(*((CAlfControl*)ctrl),
                                                        mData->mRootLayout);
        mData->mThumbLayout->SetFlag( EAlfVisualFlagAutomaticLocaleMirroringEnabled );
        mData->mExtendedThumbLayout = CAlfLayout::AddNewL(*((CAlfControl*)ctrl),
                                                        mData->mThumbLayout);
       
        if(mData->mThumbLayout)
            {
            mData->mThumbTopImage = CAlfImageVisual::AddNewL(
                *((CAlfControl*)ctrl),mData->mThumbLayout);
            mData->mThumbMiddleImage = CAlfImageVisual::AddNewL(
                *((CAlfControl*)ctrl),mData->mThumbLayout);
            mData->mThumbEndImage = CAlfImageVisual::AddNewL(
                *((CAlfControl*)ctrl),mData->mThumbLayout);
               
            if(mData->mThumbTopImage && 
               mData->mThumbMiddleImage && 
               mData->mThumbEndImage)
                {
                setThumbImage();
                }
            }
        }
    }

void AlfScrollBarDefaultBaseElement::updateVisualization()
    {
    calculateStepSizeAndThumbLength();
    if(mData->mViewLength !=  mData->mScrollbarModel->getViewLength() || mData->mTotalLength !=  mData->mScrollbarModel->getTotalLength())
        {
        mData->mViewLength =  mData->mScrollbarModel->getViewLength();
        mData->mTotalLength =  mData->mScrollbarModel->getTotalLength();
        initializeThumbLayout();
        }
    TAlfRealPoint thumbPos;
    thumbPos.iX = mData->mThumbLayout->Pos().iX.ValueNow();        	
    thumbPos.iY = ceil(mData->mStepSize *  mData->mScrollbarModel->getViewStartPosition());
    mData->mThumbLayout->SetPos(thumbPos,mData->mThumbAnimationTime);
    }

void AlfScrollBarDefaultBaseElement::destroyVisualization()
    {
    mData->mRootLayout->RemoveAndDestroyAllD();
    delete mData;
    delete mLCTData;
    createDefaultBaseElement();
    }

// ---------------------------------------------------------------------------
// From class MAlfInterfaceBase.
// Getter for interfaces provided by the scrollbar widget.
// ---------------------------------------------------------------------------
//
IAlfInterfaceBase* AlfScrollBarDefaultBaseElement::makeInterface(
                        const IfId& aType)
    {
    UString param (aType.mImplementationId);
    if (param == 
        IAlfScrollBarDefaultBaseElement::type().mImplementationId)
        {
        return static_cast<IAlfScrollBarDefaultBaseElement*>(this);
        }
    else if (param == IAlfWidgetEventHandler::type().mImplementationId)
        {
        return static_cast<IAlfWidgetEventHandler*>(this);
        }

    else if (param == 
        IAlfScrollBarBaseElementInternal::type().mImplementationId)
        {
        return static_cast<IAlfScrollBarBaseElementInternal*>(this);
        } 
    else
        {
        //do nothing
        }
    return AlfElement::makeInterface(aType);
    }

// ---------------------------------------------------------------------------
//  handles the pointerDown event
// 
// ---------------------------------------------------------------------------
//
bool AlfScrollBarDefaultBaseElement::handlePointerDownEvent(TAlfEvent * aPntrEvent)
    {
    CAlfVisual * focusvisual = aPntrEvent->Visual();
    AlfEventStatus result = EEventNotHandled;
    
    if (focusvisual)
        {
        mData->mVisual = focusvisual;
        
        // Register with the Roster to receive drag Event    
        control().Display()->Roster().SetPointerEventObservers( 
           EAlfPointerEventReportDrag ,control());
        
        if(!mData->mOperation)
                {
                mIsLongTapObserverSet = ETrue;
                mData->mLongTapPos = aPntrEvent->PointerEvent().iParentPosition;
                mData->mLongTapStarted = true;
                control().processEvent(TAlfEvent(EEventScrollBarTouchDown));
                mLongTapTimer->Start();
                }
           
        // Pointer down on track   
        if (isScrollbarTrackVisual(focusvisual))
                {
                mData->mLongTapStarted = true;
                return trackVisualHit(focusvisual);
                }
        
        // Pointer down on thumb
        else if (isScrollbarThumbVisual(focusvisual))
                {
                if(!mData->mOperation)
                    {
                    initializeThumbDragData( aPntrEvent->PointerEvent().iParentPosition); 
                    AlfScrollBarControl& slbrcntrl = static_cast<AlfScrollBarControl&>(control());
                    slbrcntrl.TactileEffectOnTouchandHold();
                    }
                result = EEventHandled;
                }
        }
    return result;
    }

// ---------------------------------------------------------------------------
//  Sends drag Up or Down event, according to the hit position
//  on track
// ---------------------------------------------------------------------------
//
AlfEventStatus AlfScrollBarDefaultBaseElement::trackVisualHit(CAlfVisual * /*aHitVisual*/)
    {
    //Get Co-Ordinates of the click event
    //Find the thumb Anchor Layout.
    TAlfTimedPoint thumbVisPos = mData->mThumbLayout->Pos();
    TAlfTimedPoint thumbVisSize =mData->mThumbLayout->Size();
    TAlfRealRect rct= mData->mTrackLayout->DisplayRect() ;
    //Check if click was above/below of thumb
    if (mData->mLongTapPos.iY - rct.iTl.iY < thumbVisPos.iY.ValueNow())
        {
        if(mData->mLongTapStarted)
            {
            //control().processEvent(TAlfEvent(EEventScrollBarDrag));
            control().processEvent(TAlfEvent (EEventScrollPageUp));
            }
        }
    else if (mData->mLongTapPos.iY - rct.iTl.iY > (thumbVisPos.iY.ValueNow() + 
        thumbVisSize.iY.ValueNow()) )
        {
        if(mData->mLongTapStarted)
            {
            //control().processEvent(TAlfEvent(EEventScrollBarDrag));
            control().processEvent(TAlfEvent(EEventScrollPageDown));
            }
        }
    else 
        {
        if(mIsLongTapObserverSet)
            {
            // Stop the long tap timer
            mLongTapTimer->Stop();
            mData->mLongTapStarted = false; 
            //mIsLongTapObserverSet = EFalse;    
            }
        else
            {
            //Do Nothing.
            }                            
            }
    return EEventHandled;
    }  

// ---------------------------------------------------------------------------
//  handles the pointerUp event
// 
// ---------------------------------------------------------------------------
//
void AlfScrollBarDefaultBaseElement::handlePointerUpEvent()
    {
    // if thumb or track is clicked, change the graphics
    if(mData->mCurrentState == ETrackPressed || mData->mCurrentState == EThumbPressed)
        {
        mData->mCurrentState = EReleased;
        changeGraphics();
        }
    
    if(mData->mOperation)
        {
        //Somehow missed the Up event 
        stopDrag(NULL,0);
        AlfScrollBarControl& slbrcntrl = static_cast<AlfScrollBarControl&>(control());
        slbrcntrl.TactileEffectOnRelease();
        }
    if(mIsLongTapObserverSet)
        {
        // Stop the long tap timer
        mLongTapTimer->Stop();
        
        // Un-register with the roster for long tap events 
        control().Display()->Roster().SetPointerEventObservers(0,
            control());
        mIsLongTapObserverSet = EFalse;    
        }
    }

AlfEventStatus AlfScrollBarDefaultBaseElement::handleLongTapEvent()
    {
    AlfEventStatus ret= EEventNotHandled;
    
    if (isScrollbarTrackVisual(mData->mVisual))
        {
        mData->mLongTapStarted = true;
        ret = trackVisualHit(mData->mVisual);
        // Long tap Tactile feedback 
        AlfScrollBarControl& slbrcntrl = static_cast<AlfScrollBarControl&>(control());
        slbrcntrl.TactileEffectOnTouchandHold();
        }
    return ret;
    }

//--------------------------------------------------------------------------
//Overriden Apis from IAlfWidgetEventHandlers
//--------------------------------------------------------------------------
//

// ---------------------------------------------------------------------------
// From class IAlfWidgetEventHandler.
// ---------------------------------------------------------------------------
//
bool AlfScrollBarDefaultBaseElement::accept(
        CAlfWidgetControl& /*aControl*/,
        const TAlfEvent& aEvent ) const
    {
    // Scrollbar baseElement handles only EEventScrollBarClicked 
    // and EEventScrollBarClicked cusbtom events
    // EEventScrollBarClicked event come from scrollbar control and  
    // EEventScrollBarModelChanged event comes from scrollable widget
    if (aEvent.CustomParameter() == EEventScrollBarClicked ||
        aEvent.CustomParameter() == EEventScrollBarModelChanged)
        {
        return true;
        }
    return false;
    }

IAlfWidgetEventHandler::AlfEventHandlerType AlfScrollBarDefaultBaseElement::eventHandlerType()
    {
    return IAlfWidgetEventHandler::EPresentationEventHandler;
    }

IAlfWidgetEventHandler::AlfEventHandlerExecutionPhase AlfScrollBarDefaultBaseElement::eventExecutionPhase()
    {
    return IAlfWidgetEventHandler::ETunnellingPhaseEventHandler;
    }

AlfEventStatus AlfScrollBarDefaultBaseElement::offerEvent(
            CAlfWidgetControl& /*aControl*/,
            const TAlfEvent& aEvent )
    {
    AlfEventStatus ret= EEventNotHandled;
    // Dont do event handling incase widget & thumb opacity is zero
    if(mData->mWidgetOpacity && mData->mThumbOpacity )
        {
        if (aEvent.IsCustomEvent())
        {
        switch(aEvent.CustomParameter())
            {
            // The event is coming from scrollbale widget
            case EEventScrollBarModelChanged:
                {
                if( mData->mScrollbarModel)
                    {
                    ScrollModelChangedEventData* scrollEventData = 
                    (ScrollModelChangedEventData*)aEvent.CustomEventData();  
                    
                        // Change the model which intern will update visualization 
                         mData->mScrollbarModel->changeData(
                                        scrollEventData->mSpan,
                                        scrollEventData->mViewLength,
                                        scrollEventData->mViewStartPos);
                        ret=EEventHandled;
                    }
            }
            break;
            
            case EEventMissedPointerUp:
                {
                mData->mLongTapStarted = false;
                ret=EEventHandled;
                handlePointerUpEvent();
                }
            break;
            
            case EEventScrollBarClicked:
                {
                ret = handleScrollBarClickedEvent(aEvent);
                }
            break;
            
            default:
            break;
            }
        }
    }
    return ret;
    }

 AlfEventStatus AlfScrollBarDefaultBaseElement::handleScrollBarClickedEvent(
            const TAlfEvent& aEvent )
     {
     AlfEventStatus ret= EEventNotHandled;
     TAlfEvent * pntrEvent=(TAlfEvent *)aEvent.CustomEventData();
     if(pntrEvent)
         {
         // Get the hit visual
         CAlfVisual *focusvisual = pntrEvent->Visual();
         if(focusvisual && 
                 pntrEvent->PointerEvent().iType == TPointerEvent::EButton1Down)
                    {
                    // check if the click is made on thumb
                    if (isScrollbarThumbVisual(focusvisual))
                        {
                        mData->mCurrentState = EThumbPressed;
                        // Change the graphics to pressed one
                        changeGraphics();
                        }
                    else if (isScrollbarTrackVisual(focusvisual))
                        {     
                        mData->mCurrentState  = ETrackPressed;
                        // Change the graphics to pressed one
                        changeGraphics();
                        }
                    }
         // Blocking event processing in case of maxthumblength,
         // case 1 : revert back the graphics change incase of maxscrollbar length
         if(pntrEvent->PointerUp() &&  mData->mScrollbarModel->getViewLength() >=  mData->mScrollbarModel->getTotalLength())
             {
             handlePointerUpEvent();
             }
         // case 2 : process the event in all other case except max length
         else if( mData->mScrollbarModel->getViewLength() !=  mData->mScrollbarModel->getTotalLength())
             {
             processPointerEvent(pntrEvent);
             }
         // case2 : do nothing incase of maxscrollbar length
         else
             {
             // do nothing
             }
             ret = EEventHandled;
         }
     else
         {
         ret = EEventNotHandled;
         }
    
     return ret;
    }

void AlfScrollBarDefaultBaseElement::setActiveStates(
    unsigned int /*aStates*/)
    {
    //do nothing
    }

// ---------------------------------------------------------------------------
// From class IAlfScrollBarBaseElementInternal.
// ---------------------------------------------------------------------------
//

// ---------------------------------------------------------------------------
// initializes scrollbar layout 
// ---------------------------------------------------------------------------
//
void AlfScrollBarDefaultBaseElement::initializeLayout()
    {
    setThumbMinMaxLength();
    calculateStepSizeAndThumbLength();
    initializeTrackLayout();
    initializeThumbLayout();
    }

// ---------------------------------------------------------------------------
// initializes scrollbar Track layout 
// ---------------------------------------------------------------------------
//
void AlfScrollBarDefaultBaseElement::initializeTrackLayout()
    {
    
    TAknWindowLineLayout ExtendedTrackLayoutHandle = 
                AknLayoutScalable_UiAccel::
                aid_size_touch_aascroll_bar(0).LayoutLine();
    // negative offset for the extended active area layout.
    // Width values based on LCT data for extended touch
    mData->mExtendedTouch = - (ExtendedTrackLayoutHandle.iW -  
                    mData->mRootLayout->Size().iX.ValueNow());                  
                    
    // Create Track Visuals
    if(mData->mTrackLayout)
        {
        TAknWindowLineLayout TrackLayoutHandle = 
            AknLayoutScalable_UiAccel:: hc_scroll_bg_pane(0).LayoutLine();
        
        // Root layout rect, parent rect for track and thumb layout
        TRect rootLayoutRect(0,
                             0,
                             mData->mRootLayout->Size().iX.ValueNow(),
                             mData->mRootLayout->Size().iY.ValueNow());
        
        TAknLayoutRect TracklayoutRect;
        TracklayoutRect.LayoutRect(rootLayoutRect, TrackLayoutHandle);
        mData->mTrackLayout->SetRect(TracklayoutRect.Rect(),0.0);
        
        // set position and size for extended track layout
        setPositionAndSize(mData->mExtendedTrackLayout,
                mData->mExtendedTouch,0 ,
                mData->mExtendedTouch,
                mData->mTrackLayout->Size().iY.ValueNow());
        
        if(mData->mTrackTopImage && 
           mData->mTrackMiddleImage && 
           mData->mTrackEndImage)
            {
            // parent rect for track images
            TRect Trackrect(0,
                            0,
                            mData->mTrackLayout->Size().iX.ValueNow(),
                            mData->mTrackLayout->Size().iY.ValueNow());
            
            TAknWindowLineLayout TrackTopHandle = 
                AknLayoutScalable_UiAccel::hc_scroll_bg_pane_g1(0).LayoutLine();
            setLCTData(Trackrect,TrackTopHandle,mData->mTrackTopImage);
           
            TAknWindowLineLayout TrackMiddleHandle = 
                AknLayoutScalable_UiAccel::hc_scroll_bg_pane_g2(0).LayoutLine();
            setLCTData(Trackrect,TrackMiddleHandle,mData->mTrackMiddleImage);
            
            TAknWindowLineLayout TrackEndHandle = 
                AknLayoutScalable_UiAccel::hc_scroll_bg_pane_g3(0).LayoutLine();
            setLCTData(Trackrect,TrackEndHandle,mData->mTrackEndImage);
           }
         }
    }

// ---------------------------------------------------------------------------
// initializes scrollbar Thumb layout 
// ---------------------------------------------------------------------------
//
void AlfScrollBarDefaultBaseElement::initializeThumbLayout()
    {
    // Create Track Visuals
    if(mData->mTrackLayout)
        {
        if(mData->mThumbLayout)
            {
            // Get thumb WindowLine Layout
            TAknWindowLineLayout ThumbLayoutHandle = 
                    AknLayoutScalable_UiAccel::hc_scroll_handle_pane(0).LayoutLine();
            
            // Root layout rect, parent rect for track and thumb layout
            TRect rootLayoutRect(0,
                                 0,
                                 mData->mRootLayout->Size().iX.ValueNow(),
                                 mData->mRootLayout->Size().iY.ValueNow());
            
           TAknLayoutRect ThumblayoutRect;
           ThumblayoutRect.LayoutRect(rootLayoutRect, ThumbLayoutHandle);
           
           // set thumb height and thumb start position from model
           mLCTData->height = mData->mThumbLength;
                               	   
           mLCTData->top = 
                      ceil(mData->mStepSize *  mData->mScrollbarModel->getViewStartPosition());     
                      
           setPositionAndSize(mData->mThumbLayout,
                         ThumblayoutRect.Rect().iTl.iX,
                         mLCTData->top,
                         ThumblayoutRect.Rect().Size().iWidth,
                         mLCTData->height);
           // Setting position and size for extended thumb layout
           setPositionAndSize(mData->mExtendedThumbLayout ,
                         mData->mExtendedTouch ,
                         0,
                         mData->mExtendedTouch,
                         mLCTData->height);
            
            if(mData->mThumbTopImage && 
               mData->mThumbMiddleImage && 
               mData->mThumbEndImage)
                {
               // parent rect for thumb images
               TRect ThumbRect(0,
                               0,
                               mData->mThumbLayout->Size().iX.ValueNow(),
                               mData->mThumbLayout->Size().iY.ValueNow());
                
               TAknWindowLineLayout ThumbTopHandle = 
                    AknLayoutScalable_UiAccel::hc_scroll_handle_pane_g1(0).LayoutLine();
               setLCTData(ThumbRect,ThumbTopHandle,mData->mThumbTopImage);
               
               TAknWindowLineLayout ThumbMiddleHandle = 
                    AknLayoutScalable_UiAccel::hc_scroll_handle_pane_g2(0).LayoutLine();
               setLCTData(ThumbRect,ThumbMiddleHandle,mData->mThumbMiddleImage);
               
               TAknWindowLineLayout ThumbEndHandle = 
                    AknLayoutScalable_UiAccel::hc_scroll_handle_pane_g3(0).LayoutLine();
               setLCTData(ThumbRect,ThumbEndHandle,mData->mThumbEndImage);
                }
            }
        }
    }

// ---------------------------------------------------------------------------
// sets LCT Data (position and size) for the visual 
// ---------------------------------------------------------------------------
//
void AlfScrollBarDefaultBaseElement::setLCTData(const TRect& aRect ,
                                                TAknWindowLineLayout& aWindowLineLayout,
                                                CAlfVisual * aVisual)
    {
    TAknLayoutRect layoutRect;
    layoutRect.LayoutRect(aRect, aWindowLineLayout);
         
    setPositionAndSize(aVisual,layoutRect.Rect().iTl.iX,layoutRect.Rect().iTl.iY,
                      layoutRect.Rect().Size().iWidth,layoutRect.Rect().Size().iHeight);
    }

// ---------------------------------------------------------------------------
// sets position and size for the visual 
// ---------------------------------------------------------------------------
//
void AlfScrollBarDefaultBaseElement::setPositionAndSize(CAlfVisual * aVisual,
                                                      int aXVal,int aYVal, 
                                                      int aWidth,int aHeight)
    {
    TAlfRealPoint trackTopPnt(aXVal,aYVal);
    TAlfRealSize trackTopSize(aWidth,aHeight);
    aVisual->SetPos(trackTopPnt,0);
    aVisual->SetSize(trackTopSize,0);
    }

// ---------------------------------------------------------------------------
// initializes scrollbar model pointer for further reference 
// ---------------------------------------------------------------------------
//
void AlfScrollBarDefaultBaseElement::initializeScrollModel()
    {
    // Store the ScrollBar Model Pointer;
    IAlfWidgetFactory& widgetFactory =AlfWidgetEnvExtension::widgetFactory(control().Env());
    AlfScrollBarWidget* wdgt = static_cast<AlfScrollBarWidget*>(widgetFactory.findWidget(control().widget()->widgetName()));
    //Get model pointer
    IAlfScrollBarModel* model = static_cast<IAlfScrollBarModel*>(wdgt->model());
    mData->mScrollbarModel = static_cast<AlfScrollBarModel*>(model);
    }


// ---------------------------------------------------------------------------
// Set images for the track 
// ---------------------------------------------------------------------------
//
void AlfScrollBarDefaultBaseElement::setTrackImage()
    {
    TInt flags = EAlfTextureFlagSkinContent;
    flags |= EAlfTextureFlagAutoSize;
   
    // Create skin Image
    TAlfImage trackTopImage(KAknsIIDQsnCpScrollBgTop,TSize(0,0), 
                            EAspectRatioNotPreserved,NULL, 
                            -1,-1,0.f,0.f,1.f,1.f,
                            (TAlfTextureFlags)flags);
    TAlfImage trackMiddleImage(KAknsIIDQsnCpScrollBgMiddle, TSize(0,0), 
                               EAspectRatioNotPreserved, NULL, 
                               -1, -1, 0.f, 0.f, 1.f, 1.f, 
                               (TAlfTextureFlags)flags);
    TAlfImage trackEndImage(KAknsIIDQsnCpScrollBgBottom, TSize(0,0), 
                            EAspectRatioNotPreserved, NULL, 
                            -1, -1, 0.f, 0.f, 1.f, 1.f, 
                            (TAlfTextureFlags)flags);
    TAlfImage trackTopPressedImage(KAknsIIDQsnCpScrollBgTopPressed,TSize(0,0), 
                                EAspectRatioNotPreserved,NULL, 
                                -1,-1,0.f,0.f,1.f,1.f,
                                (TAlfTextureFlags)flags);
    TAlfImage trackMiddlePressedImage(KAknsIIDQsnCpScrollBgMiddlePressed, TSize(0,0), 
                               EAspectRatioNotPreserved, NULL, 
                               -1, -1, 0.f, 0.f, 1.f, 1.f, 
                               (TAlfTextureFlags)flags);
    TAlfImage trackEndPressedImage(KAknsIIDQsnCpScrollBgBottomPressed, TSize(0,0), 
                            EAspectRatioNotPreserved, NULL, 
                            -1, -1, 0.f, 0.f, 1.f, 1.f, 
                            (TAlfTextureFlags)flags);
    
    // Set Image on visuals
    mData->mTrackTopImage->SetImage(trackTopImage);
    mData->mTrackMiddleImage->SetImage(trackMiddleImage);                        
    mData->mTrackEndImage->SetImage(trackEndImage);
    mData->mTrackTopImage->SetSecondaryImage(trackTopPressedImage);
    mData->mTrackMiddleImage->SetSecondaryImage(trackMiddlePressedImage);                        
    mData->mTrackEndImage->SetSecondaryImage(trackEndPressedImage);
    }//End of setImage

// ---------------------------------------------------------------------------
// Set images for the thumb 
// ---------------------------------------------------------------------------
//
void AlfScrollBarDefaultBaseElement::setThumbImage()
    {
    TInt flags = EAlfTextureFlagSkinContent;
       flags |= EAlfTextureFlagAutoSize;
   
	  TAlfImage thumbTopImage(KAknsIIDQsnCpScrollHandleTop,TSize(0,0), 
							   EAspectRatioNotPreserved,NULL, 
							   -1,-1,0.f,0.f,1.f,1.f,
							   (TAlfTextureFlags)flags);
	  TAlfImage thumbMiddleImage(KAknsIIDQsnCpScrollHandleMiddle, TSize(0,0), 
                              EAspectRatioNotPreserved, NULL, 
                              -1, -1, 0.f, 0.f, 1.f, 1.f, 
                              (TAlfTextureFlags)flags);
	  TAlfImage thumbEndImage(KAknsIIDQsnCpScrollHandleBottom, TSize(0,0), 
	                           EAspectRatioNotPreserved, NULL, 
	                           -1, -1, 0.f, 0.f, 1.f, 1.f, 
	                           (TAlfTextureFlags)flags);
	  TAlfImage thumbTopPressedImage(KAknsIIDQsnCpScrollHandleTopPressed,TSize(0,0), 
	                           EAspectRatioNotPreserved,NULL, 
	                           -1,-1,0.f,0.f,1.f,1.f,
	                           (TAlfTextureFlags)flags);
      TAlfImage thumbMiddlePressedImage(KAknsIIDQsnCpScrollHandleMiddlePressed, TSize(0,0), 
                              EAspectRatioNotPreserved, NULL, 
                              -1, -1, 0.f, 0.f, 1.f, 1.f, 
                              (TAlfTextureFlags)flags);
      TAlfImage thumbEndPressedImage(KAknsIIDQsnCpScrollHandleBottomPressed, TSize(0,0), 
                              EAspectRatioNotPreserved, NULL, 
                              -1, -1, 0.f, 0.f, 1.f, 1.f, 
                              (TAlfTextureFlags)flags);

       mData->mThumbTopImage->SetImage(thumbTopImage);
       mData->mThumbMiddleImage->SetImage(thumbMiddleImage);                        
       mData->mThumbEndImage->SetImage(thumbEndImage);
       mData->mThumbTopImage->SetSecondaryImage(thumbTopPressedImage);
       mData->mThumbMiddleImage->SetSecondaryImage(thumbMiddlePressedImage);                        
       mData->mThumbEndImage->SetSecondaryImage(thumbEndPressedImage);
 
    }//End of setImage
// ---------------------------------------------------------------------------
// change images for the track 
// ---------------------------------------------------------------------------
//
void AlfScrollBarDefaultBaseElement::changeTrackImage(bool aPressed)
    {
    if(aPressed)
    	{
    	TAlfTimedValue alpha(1.0, 0.0);
    	mData->mTrackTopImage->SetSecondaryAlpha(alpha);
	    mData->mTrackMiddleImage->SetSecondaryAlpha(alpha);                        
	    mData->mTrackEndImage->SetSecondaryAlpha(alpha);
	    }
    else
    	{
    	TAlfTimedValue alpha(0.0, 0.0);
    	mData->mTrackTopImage->SetSecondaryAlpha(alpha);
	    mData->mTrackMiddleImage->SetSecondaryAlpha(alpha);                        
	    mData->mTrackEndImage->SetSecondaryAlpha(alpha);
    	
    	}
    }
// ---------------------------------------------------------------------------
// change images for the thumb 
// ---------------------------------------------------------------------------
//
void AlfScrollBarDefaultBaseElement::changeThumbImage(bool aPressed)
    {
    if(aPressed)
    	{
    	TAlfTimedValue alpha(1.0,0.0);
    	mData->mThumbTopImage->SetSecondaryAlpha(alpha);
	    mData->mThumbMiddleImage->SetSecondaryAlpha(alpha);                        
	    mData->mThumbEndImage->SetSecondaryAlpha(alpha);
	    }
    else
    	{
    	TAlfTimedValue alpha(0.0,0.0);
    	mData->mThumbTopImage->SetSecondaryAlpha(alpha);
	    mData->mThumbMiddleImage->SetSecondaryAlpha(alpha);                        
	    mData->mThumbEndImage->SetSecondaryAlpha(alpha);
    	}
    }
// ---------------------------------------------------------------------------
// calculates Step size and Thumb length 
// ---------------------------------------------------------------------------
//
void  AlfScrollBarDefaultBaseElement::calculateStepSizeAndThumbLength()
    {
    int totalLength =  mData->mScrollbarModel->getTotalLength();
    int viewLength =  mData->mScrollbarModel->getViewLength();
    int viewStartPos =  mData->mScrollbarModel->getViewStartPosition();
    
    TAlfTimedPoint size = mData->mRootLayout->Size();
    mData->mThumbLength = ((float)viewLength / 
                                (float)totalLength) * size.iY.ValueNow();
    
    if(mData->mThumbMinLength != 0)
        {
        if(mData->mThumbLength<mData->mThumbMinLength)
            {
            mData->mThumbLength = mData->mThumbMinLength;
            }
        else if(mData->mThumbLength>mData->mThumbMaxLength)
            {
            mData->mThumbLength = mData->mThumbMaxLength;
            }
        else
            {
            
            }
        }
    else
        {
        
        }
        
    if(totalLength != viewLength)
	    {    
	    mData->mStepSize = (size.iY.ValueNow() - mData->mThumbLength) / 
	                        (totalLength - viewLength);                       
	    }
    else
	    {
	    mData->mStepSize = 0.0;	
	    }
    }
// ---------------------------------------------------------------------------
// set maximum and minimum thumblength value from LCT data 
// ---------------------------------------------------------------------------
//
void AlfScrollBarDefaultBaseElement::setThumbMinMaxLength()
    {
    TAknWindowLineLayout ThumbMinLengthHandle = 
                AknLayoutScalable_UiAccel::
                aid_size_min_handle_cp002(0).LayoutLine();

    TAknWindowLineLayout ThumbMaxLengthHandle = 
                AknLayoutScalable_UiAccel::
                    aid_hc_size_max_handle(0).LayoutLine();
    
    // Since LCT data corresponding to this is very small (20), 
    // we are hardcoding the value as 30,this has to be removed 
    // once if we get the correct LCT data
    // mData->mThumbMinLength = 30;
    mData->mThumbMinLength = ThumbMinLengthHandle.iH;
    mData->mThumbMaxLength = mData->mRootLayout->Size().iY.ValueNow() -
                             ThumbMaxLengthHandle.ib;
    }

// ---------------------------------------------------------------------------
// createDefaultBaseElement, initializes class member variables 
// ---------------------------------------------------------------------------
//
void AlfScrollBarDefaultBaseElement::createDefaultBaseElement()
    {
    
    mData = new (EMM) ScrollBarBaseElementData();
    mData->mOperation = ENop;
    mData->mVisual = NULL;
    mData->mDragPoint = TPoint(0,0);
    mData->mLongTapPos = TPoint(0,0);
    mData->mDragStartStep = 0;
    mData->mViewLength = 0;
    mData->mThumbLength = 0;
    mData->mThumbMinLength = 0;
    mData->mThumbMaxLength = 0;
    mData->mStepSize = 0;
    mData->mThumbSizeChanged = false;
    mData->mWidgetOpacity = 1.0;
    mData->mThumbOpacity = 1.0;
    mData->mLongTapStarted = false;
    mData->mCurrentState = EReleased;
    mData->mTrackLayout = NULL;
    mData->mTrackTopImage = NULL ;
    mData->mTrackMiddleImage = NULL ;
    mData->mTrackEndImage = NULL ;
    mData->mThumbLayout = NULL ;
    mData->mThumbTopImage = NULL ;
    mData->mThumbMiddleImage = NULL ;
    mData->mThumbEndImage = NULL ;
    mData->mExtendedTouch = 0;
    mData->mScrollbarModel = NULL;
    mData->mThumbAnimationTime = 0;
    
    mLCTData = new ( EMM ) ScrollbarThumbLCTData();
    mLCTData->left=0;
    mLCTData->top=0;
    mLCTData->right=0;
    mLCTData->bottom=0;
    mLCTData->height=0;
    mLCTData->width=0;
    
    }//End of createDefaultBaseElement

// ---------------------------------------------------------------------------
// initializeThumbDragData
//  
// ---------------------------------------------------------------------------
//    
bool AlfScrollBarDefaultBaseElement::initializeThumbDragData( const TPoint& aPointerPos )
    {
    if(mData->mVisual )
        {
        mData->mOperation = EMoveItem;
        mData->mDragPoint = aPointerPos;
        mData->mDragStartStep = mData->mScrollbarModel->getViewStartPosition();
        }
    else
        {
        // Do Nothing
        }   
    return ETrue;  
    }


// ---------------------------------------------------------------------------
// stopDrag
// Stop Dragging 
// ---------------------------------------------------------------------------
//
bool AlfScrollBarDefaultBaseElement::stopDrag( CAlfVisual* /*aHitVisual*/, 
                                               TInt /*aAnimTime*/)
    {
    TRAPD(err,
          control().Display()->Roster().SetPointerEventObservers(
          0, 
          control()));
    
    if(err == KErrNone)
        {
        if(mData->mVisual)
            {
            mData->mVisual = NULL;
            }
        mData->mOperation = ENop;
        return true;
        }
    return false;
    }

// ---------------------------------------------------------------------------
// changeGraphics
// change the skin image graphics based on the pressedState
// ---------------------------------------------------------------------------
//
void AlfScrollBarDefaultBaseElement::changeGraphics()
    {
    if(mData->mCurrentState == EReleased)    // Normal graphics
        {
        changeTrackImage(false);
        changeThumbImage(false);
        }
    else if(mData->mCurrentState == EThumbPressed || mData->mCurrentState == ETrackPressed)  // Pressed graphics
        {
        if(mData->mCurrentState == EThumbPressed)
            {
            // set the index for pressed graphics (thumb and track)
            changeTrackImage(true);
            changeThumbImage(true);
            }
        else if (mData->mCurrentState == ETrackPressed)
            {
            // set the index for pressed graphics (track)
            changeTrackImage(true);
            }
        }
    }

// ---------------------------------------------------------------------------
// isScrollbarTrackVisual
// checks whether the visual is scrollbar track visual or not
// ---------------------------------------------------------------------------
// 
bool AlfScrollBarDefaultBaseElement::isScrollbarTrackVisual(CAlfVisual *aVisual)
    {
    if(aVisual)
        {
        if(aVisual == mData->mExtendedTrackLayout ||
                aVisual == mData->mTrackTopImage ||
                aVisual == mData->mTrackMiddleImage ||
                aVisual == mData->mTrackEndImage)
                {
                return true;
                }
        }
     return false;   
    }  

// ---------------------------------------------------------------------------
// isScrollbarThumbVisual
// checks whether the visual is scrollbar thumb visual or not
// ---------------------------------------------------------------------------
// 
bool AlfScrollBarDefaultBaseElement::isScrollbarThumbVisual(CAlfVisual *aVisual)
    {
    if(aVisual)
        {
        if(aVisual == mData->mExtendedThumbLayout ||
                aVisual == mData->mThumbTopImage ||
                aVisual == mData->mThumbMiddleImage ||
                aVisual == mData->mThumbEndImage)
                {
                return true;
                }
        }
     return false;   
    }  

    }//namespace Alf
    
//End Of File
