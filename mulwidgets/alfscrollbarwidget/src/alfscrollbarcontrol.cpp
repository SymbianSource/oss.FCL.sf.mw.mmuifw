/*
* Copyright (c) 2007, 2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Control Implementation with utilities for scrollbar widget.
*
*/

// Toolkit includes
#include <alf/alfenv.h>
#include <alf/alfroster.h>
#include <alf/alfdisplay.h>

// Widget model includes
#include <alf/alfwidgetevents.h>
#include <alf/alfcontrolgroup.h>

// Widget includes
#include <alf/ialfscrollbardefaultbaseelement.h>
#include "ialfscrollbarbaseelementinternal.h"

// Internal includes
#include "alfscrollbarcontrol.h"
#include "alfscrollbarwidget.h"


// namespaces
using namespace osncore;
using namespace alfscrollbarwidget;

namespace Alf
    {

// ---------------------------------------------------------------------------
//  Constructor.
// ---------------------------------------------------------------------------
//
AlfScrollBarControl::AlfScrollBarControl(CAlfEnv& aEnv) : 
    CAlfWidgetControl(aEnv)
    {
    mLayoutHeight = -1; //this means that the first draw is yet to happen.
    mLayoutWidth = -1;  //this means that the first draw is yet to happen.
    disableState(IAlfWidgetControl::Focusable);
    mTactilefdbInstance = NULL;
    mDragTactileFeedbackStarted = false;
    mEnableTactile = false; // Next statement will make it true
    EnableTactileFeedback(true);
    }
   
// ---------------------------------------------------------------------------
// Destructor.
// ---------------------------------------------------------------------------
//
AlfScrollBarControl::~AlfScrollBarControl()
    {
    }
   
// ---------------------------------------------------------------------------
// 
// enableTactileFeedback
// ---------------------------------------------------------------------------
//
void AlfScrollBarControl::EnableTactileFeedback(bool aEnabled)
    {
        if (!mTactilefdbInstance)
            {
             mEnableTactile = aEnabled;
             mTactilefdbInstance = MTouchFeedback::Instance();
            }
    }

//-----------------------------------------------------------------------------
//
//  TouchTactileEffectOnTouch
//-----------------------------------------------------------------------------

void AlfScrollBarControl::TactileEffectOnTouchandHold()
    {
    if (mTactilefdbInstance)
        {
         mTactilefdbInstance->InstantFeedback(ETouchFeedbackSlider);
        }
    }

//-------------------------------------------------------------------------------
//
//
//--------------------------------------------------------------------------------     
            
void AlfScrollBarControl::TactileEffectOnDrag(TAlfEvent& aEvent)
{            
 if (mTactilefdbInstance)
  {
    CCoeControl* ccoecntrl = static_cast<CCoeControl*>(Display()->ObjectProvider());
    TInt intensity = 50;
    TTimeIntervalMicroSeconds32 timespan = 1000000;
    mTactilefdbInstance->StartFeedback(ccoecntrl,ETouchContinuousSlider,&aEvent.PointerEvent(),intensity,timespan);
    mDragTactileFeedbackStarted = true;
  }
}

//-------------------------------------------------------------------------------
//
//   TouchTactileEffectOnRelease
//--------------------------------------------------------------------------------
//    
void AlfScrollBarControl::TactileEffectOnRelease()    
{
	if (mTactilefdbInstance)
	{
	if ( mDragTactileFeedbackStarted )
	    {
	    	// Touch Release on Thumb after drag
        CCoeControl* ccoecntrl = static_cast<CCoeControl*>(Display()->ObjectProvider());
	    mTactilefdbInstance->StopFeedback(ccoecntrl);
	    mDragTactileFeedbackStarted = false;
	    }
	else
	    {
	    // Touch Release on thumb	
	    mTactilefdbInstance->InstantFeedback(ETouchFeedbackSlider);
	    }
	}
}

// ---------------------------------------------------------------------------
//
// getTactileFeedbackState
// 
// ---------------------------------------------------------------------------
bool AlfScrollBarControl::IsTactileFeedbackEnabled()
    {
    return mEnableTactile;
    }

// ---------------------------------------------------------------------------
// From Base class CAlfControl
// Notifies the owner that the layout of a visual has been recalculated.
// Called only when the EAlfVisualFlagLayoutUpdateNotification flag has
// been set for the visual.
// ---------------------------------------------------------------------------
//
void AlfScrollBarControl::VisualLayoutUpdated(CAlfVisual& aVisual)
    {
    if((mLayoutWidth == -1 && roundFloatToInt(
        aVisual.Size().ValueNow().iX) == 0) ||
        (mLayoutHeight == -1 && roundFloatToInt(
            aVisual.Size().ValueNow().iY) == 0))
        {
        return; /*No size assigned yet*/
        }
    else
        {
        //do nothing
        }   

    if(mLayoutWidth == roundFloatToInt(aVisual.Size().ValueNow().iX)  && 
        mLayoutHeight == roundFloatToInt(aVisual.Size().ValueNow().iY))
        {
        //return; /* No change in layout size */
        }
        else
        {
        //do nothing
        }

    mLayoutWidth = roundFloatToInt(aVisual.Size().ValueNow().iX);
    mLayoutHeight = roundFloatToInt(aVisual.Size().ValueNow().iY);
    IAlfElement* baseelement = findElement ("BaseElement");
    IAlfScrollBarBaseElementInternal* elementInternal = NULL;
    if(baseelement)
	    {
	    elementInternal = 
	      static_cast<IAlfScrollBarBaseElementInternal*> (
	      baseelement->makeInterface(IAlfScrollBarBaseElementInternal::type()));	
	    }
	else
		{
		// do nothing	
		}
    
    if(elementInternal)
        {
        elementInternal->baseLayoutSizeChanged();
        }
    else
        {
        //do nothing
        }
    }

// ---------------------------------------------------------------------------
// Returns top level layout.
// ---------------------------------------------------------------------------
//
CAlfLayout* AlfScrollBarControl::ContainerLayout(
    const CAlfControl* /*aConnected*/) const
    {
    //Return the root layout of the control
    return(static_cast<CAlfLayout*> (&Visual(0)));
    }

// ---------------------------------------------------------------------------
// From base class CAlfWidgetControl
// Handle Alfred events.
// ---------------------------------------------------------------------------
//
AlfEventStatus AlfScrollBarControl::handleEvent( const TAlfEvent& aEvent )
    {
    IAlfElement *base = findElement("BaseElement");
        
    if(aEvent.IsCustomEvent())
        {
        if(aEvent.CustomParameter() == EEventMissedPointerUp) 
            {
            if(base)
            	{
            	IAlfWidgetEventHandler* element = 
	                    static_cast<IAlfWidgetEventHandler*> (
	                        base->makeInterface(IAlfWidgetEventHandler::type()));
	            if(element)
		            {
		            	element->offerEvent(*this,aEvent);
		            }
	            
            	}
            }
        }
    else if(aEvent.IsPointerEvent())
        {
        // if the type of base element is scrollbardefaultbaseelement, then
        // convert the event to custom event
        if(base)
        	{
         
	        if(base->makeInterface(
	            IAlfScrollBarDefaultBaseElement::type()) != NULL)
	            {
	            TAlfEvent customevent(EEventScrollBarClicked,(TInt )&aEvent);
	            return CAlfWidgetControl::processEvent(customevent);	            
	            }
	        else // other elements (like AlfElement) dont understand custom events
	            { // Let them handle the original event in their own way
	            return CAlfWidgetControl::handleEvent(aEvent);	                       
		        }
            }
        }     
    //ScrollBar should not handle any key event
    else
        { 
        return EEventNotHandled;
        }
    
    return EEventNotHandled;
    }

    }//End of namespace Alf
//End of File
