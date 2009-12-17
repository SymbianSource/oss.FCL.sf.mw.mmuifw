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
* Description:  Stray PointerUp Event Filter.
*
*/

#include <osn/osnnew.h>
#include <alf/alfevent.h>
#include <alf/alfenv.h>
#include <alf/alfwidgetevents.h>
#include <alf/ialfscrollbarmodel.h>
#include <alf/alfwidgetcontrol.h>

#include "alfwidgetcontroleventfilter.h"

enum EventfilterEvent
    {
    EEventStrayPointerUp=57423
    };
//namespace Alf
namespace Alf
{
AlfWidgetControlEventFilter::AlfWidgetControlEventFilter()
	{
	mButtonDown = false;
	}
	
AlfWidgetControlEventFilter::~AlfWidgetControlEventFilter()
	{
	}

IAlfWidgetEventHandler::AlfEventHandlerType AlfWidgetControlEventFilter
    ::eventHandlerType()
    {
    return IAlfWidgetEventHandler::ELogicalEventHandler;
    }

IAlfWidgetEventHandler::AlfEventHandlerExecutionPhase AlfWidgetControlEventFilter
    ::eventExecutionPhase()
    {
    return IAlfWidgetEventHandler::ETunnellingPhaseEventHandler;
    }

bool AlfWidgetControlEventFilter::accept( 
                            CAlfWidgetControl& /*aControl*/, 
                            const TAlfEvent& /*aEvent */) const
	{	
    return false;
	}
		
AlfEventStatus AlfWidgetControlEventFilter::offerEvent(
                     CAlfWidgetControl& aControl, 
                     const TAlfEvent& aEvent )
	{
	if(aEvent.IsPointerEvent())
        {
        if (aEvent.PointerEvent().iType == TPointerEvent::EButton1Down)
            {
            // Recieved PointerDown
            mButtonDown = true;
            }
        else if (aEvent.PointerUp())
            {
            if(!mButtonDown)
                {
                // Recieved a stray pointerUp 
                aControl.Env().Send(TAlfCustomEventCommand(EEventStrayPointerUp));
                }
            mButtonDown = false;
            }
        }
    else if(aEvent.IsCustomEvent())
        {
        if(aEvent.CustomParameter() == EEventStrayPointerUp)
            {
            if(mButtonDown)
                {
                mButtonDown = false;
                TAlfEvent customevent(EEventMissedPointerUp);
                aControl.handleEvent(customevent);
                }
            }
        }
	return EEventNotHandled;
	}
	
void AlfWidgetControlEventFilter::setActiveStates( unsigned int /*aStates*/ )
	{
	}
IAlfInterfaceBase* AlfWidgetControlEventFilter::makeInterface ( const IfId& /*aType*/ )
	{
	return NULL;
	}

}// namespace Alf

