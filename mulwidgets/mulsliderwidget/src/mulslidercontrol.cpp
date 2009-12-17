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
* Description:  Control Implementation with utilities for slider widget.
*
*/

// Toolkit includes
#include <alf/alfenv.h>
#include <alf/alfroster.h>
#include <alf/alfdisplay.h>

// Widget model includes
#include <alf/alfwidgeteventhandler.h>
#include <alf/alfwidgetevents.h>
#include "alf/alfattribute.h"
#include <alf/alfutil.h>
#include "alf/alfmodeloperation.h"
#include <alf/alfcontrolgroup.h>

// libc includes
#include <libc/string.h>
#include <libc/stdlib.h>
#include <mul/mulevent.h>

//Widget includes

//Internal includes
#include "mulslidercontrol.h"
#include "mulsliderwidget.h"
#include "mulslidermodel.h"
#include "imulsliderbaseelementinternal.h"

//namespaces
using namespace osncore;
using namespace mulsliderwidget;


namespace Alf
    {
// ---------------------------------------------------------------------------
//  Constructor.
// ---------------------------------------------------------------------------
//
MulSliderControl::MulSliderControl(CAlfEnv& aEnv) : 
    CAlfWidgetControl(aEnv)
    {
    mLayoutHeight = -1; //this means that the first draw is yet to happen.
    mLayoutWidth = -1;  //this means that the first draw is yet to happen.
    mDragTactileFeedbackStarted = false;
    mEnableTactile = false; // Next statement will make it true
    mTactilefdbInstance = NULL;
    EnableTactileFeedback(true);
    }
    
    
// ---------------------------------------------------------------------------
// Destructor.
// ---------------------------------------------------------------------------
//
MulSliderControl::~MulSliderControl()
    {

    }
   
   
// ---------------------------------------------------------------------------
// Notifies the owner that the layout of a visual has been recalculated.
// Called only when the EAlfVisualFlagLayoutUpdateNotification flag has
// been set for the visual.
// ---------------------------------------------------------------------------
//
void MulSliderControl::VisualLayoutUpdated(CAlfVisual& aVisual)
    {
    if((mLayoutWidth == -1 && roundFloatToInt(
        aVisual.Size().ValueNow().iX) == 0) ||
        (mLayoutHeight == -1 && roundFloatToInt(
            aVisual.Size().ValueNow().iY) == 0))
        {
        //return; /*No size assigned yet*/
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
    IMulSliderBaseElementInternal* elementInternal = 
        static_cast<IMulSliderBaseElementInternal*> (
            baseelement->makeInterface(IMulSliderBaseElementInternal::type()));
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
CAlfLayout* MulSliderControl::ContainerLayout(
    const CAlfControl* /*aConnected*/) const
    {
    if(VisualCount())
    {
    //Return the root layout of the control
    	return(static_cast<CAlfLayout*> (&Visual(0)));		
    }
    else
    {
    	return NULL;	
    }
    }


// ---------------------------------------------------------------------------
// From base class CAlfWidgetControl
// Handle Alfred events.
// ---------------------------------------------------------------------------
//
AlfEventStatus MulSliderControl::handleEvent( const TAlfEvent& aEvent )
    {
    AlfEventStatus ret = EEventNotHandled;
    IAlfElement *base = findElement("BaseElement");
    if(base)
        {
        IAlfWidgetEventHandler* element = 
        static_cast<IAlfWidgetEventHandler*> (
            base->makeInterface(IAlfWidgetEventHandler::type()));
      
        if(aEvent.IsCustomEvent())
            {
            if(aEvent.CustomParameter() == EEventMissedPointerUp) 
                {
                ret = element->offerEvent(*this,aEvent);
                }
            }
        else if(aEvent.IsKeyEvent() || 
                aEvent.IsPointerEvent())
            {
            // if the type of base element is slider , then
            // convert the event to custom event
            IMulSliderBaseElementInternal* elementInternal = 
            static_cast<IMulSliderBaseElementInternal*> (
                base->makeInterface(IMulSliderBaseElementInternal::type()));
            ret = element->offerEvent(*this , aEvent);    
            }
        else
            { 
            }
        }
     return ret;   
    }

    
// ---------------------------------------------------------------------------
// From base class CAlfWidgetControl.
// Notification that data is about to change in the data model.
// ---------------------------------------------------------------------------
//
void MulSliderControl::dataChanging(const AlfModelOperation& /*aOperation*/)
    {}


// ---------------------------------------------------------------------------
// From class IAlfInterfaceBase.
// Getter for interfaces provided by the list widget.
// ---------------------------------------------------------------------------
//
IAlfInterfaceBase* MulSliderControl::makeInterface(const IfId& aType)
    {
    UString param(aType.mImplementationId);
   
    return CAlfWidgetControl::makeInterface(aType);
    }

// ---------------------------------------------------------------------------
// 
// removeAndDestroyElement
// 
// ---------------------------------------------------------------------------
//
void MulSliderControl::removeAndDestroyElement(const IAlfElement& /*aElement*/)
    {
    // reset the variables
    mLayoutWidth = -1;
    mLayoutHeight = -1;    
        
    CAlfWidgetControl::removeAndDestroyElement(*findElement("BaseElement"));    
    }
//----------------------------------------------------------------------
//
//enableTactileFeedback
//----------------------------------------------------------------------
void MulSliderControl::EnableTactileFeedback(bool aEnabled)
    {
        if (!mTactilefdbInstance)
            {
             mEnableTactile = aEnabled;	
             mTactilefdbInstance = MTouchFeedback::Instance();
            }
    }
//----------------------------------------------------------------------
//TactileEffectOnTouch
//
//----------------------------------------------------------------------
void MulSliderControl::TactileEffectOnTouchandHold()
    {
    if (mTactilefdbInstance)
        {
        mTactilefdbInstance->InstantFeedback(ETouchFeedbackSlider);
        }
    }
//----------------------------------------------------------------------
//TactileEffectOnDrag
//
//----------------------------------------------------------------------
void MulSliderControl::TactileEffectOnDrag(TAlfEvent& aEvent)
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
//   TactileEffectOnRelease
//--------------------------------------------------------------------------------
//    
void MulSliderControl::TactileEffectOnRelease()    
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
}//End of namespace Alf
//End of File

