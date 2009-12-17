/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:   File Implements the various input events, including custom events.
*
*/


#include <alf/alfenv.h>
#include <alf/alfevent.h>
#include <alf/alfvisual.h>
#include <osn/osncommon.h>
#include "alf/alfreferencetovisual.h"
#include <alf/alfexceptions.h>
#include "alf/alfattribute.h"
#include "alf/alfwidget.h"
#include "alf/ialfattributeowner.h"
#include <alf/alfwidgetevents.h>
#include <alf/alfwidgetcommand.h>
#include "alfeventinput.h"

namespace Alf
    {


// ======== LOCAL FUNCTIONS ========

// ======== MEMBER FUNCTIONS ========

AlfEventInput::AlfEventInput(int aEvtTypeFlags,int aEventID,int aEventData, int aKeyEventModifiers)
    {
    mEventTypeFlags = aEvtTypeFlags;
    mEventID = aEventID;
    mEventData = aEventData;
    mKeyEventModifiers = aKeyEventModifiers;
    }

// ---------------------------------------------------------------------------
// ?implementation_description
// ---------------------------------------------------------------------------
//
bool AlfEventInput::accept( const TAlfEvent& aEvent ) const
    {
    bool ret = false;

	// Custom event
    if ( aEvent.Type() == TAlfEvent::ETypeCustom &&
            ( mEventTypeFlags & custom ) &&
            aEvent.CustomParameter() == mEventID )
        {
        // If event data is defined to be used, it must match also
        if ( ( mEventTypeFlags & useEventData ) == 0 ||
                aEvent.CustomEventData() == mEventData )
            {
            ret = true;
            }
        }
    // Key event
    else if( aEvent.Type() == TAlfEvent::ETypeKey &&
              ( mEventTypeFlags & custom ) == 0 &&
    		   aEvent.KeyEvent().iScanCode == mEventID )
        {
        // Test that key event up/down matches
        
        // keyDown triggers from EEventKey event.
        // keyUp triggers from EEventKeyUp event.
        
        if ( ( mEventTypeFlags & keyDown &&
               aEvent.Code() == EEventKey ) ||
             ( mEventTypeFlags & keyUp &&
               aEvent.Code() == EEventKeyUp ) )
        	{
        	// Set trigger status true first, match checks will change it to false if no match.
        	ret = true;

            // If no modifiers requested, no need to check anything
            if ( mKeyEventModifiers )
                {                
            	uint receivedModifiers = aEvent.KeyEvent().iModifiers;
            	
            	while ( true )
            	    {
                	if ( mKeyEventModifiers & alt )
                	    {
            	        if ( !(receivedModifiers & (eventBitLeftAlt|eventBitRightAlt|eventBitAlt)) )
            	            {
            	            ret = false;
            	            break;
            	            }
               	        }
                	if ( mKeyEventModifiers & ctrl )
            	        {
            	        if ( !(receivedModifiers & (eventBitLeftCtrl|eventBitRightCtrl|eventBitCtrl)) )
            	            {
            	            ret = false;
            	            break;
            	            }        	        
            	        }
                	if ( mKeyEventModifiers & shift )
                	    {
            	        if ( !(receivedModifiers & (eventBitLeftShift|eventBitRightShift|eventBitShift)) )
            	            {
                	        ret = false;
                	        break;
                	        }        	        
                	    }
                	if ( mKeyEventModifiers & func )
                	    {
            	        if ( !(receivedModifiers & (eventBitLeftFunc|eventBitRightFunc|eventBitFunc)) )
            	            {
                	        ret = false;
                	        break;
                	        }        	    
                	    }
                	    
                	break;
            	    }
                	    
               	if ( mKeyEventModifiers & capsLock && !(receivedModifiers & eventBitCapsLock) )
               	    {
               	    ret = false;
               	    }
               	    
               	if ( mKeyEventModifiers & numLock && !(receivedModifiers & eventBitNumLock) )
               	    {
               	    ret = false;
               	    }
               	    
               	if ( mKeyEventModifiers & scrollLock && !(receivedModifiers & eventBitScrollLock) )
               	    {
               	    ret = false;
               	    }
                }
        	}
        }

    return ret;
    }

// ---------------------------------------------------------------------------
// ?description_if_needed
// ---------------------------------------------------------------------------
//
AlfPointerEventInput::AlfPointerEventInput()
    {
    }

// ---------------------------------------------------------------------------
// ?description_if_needed
// ---------------------------------------------------------------------------
//
AlfPointerEventInput::AlfPointerEventInput( const char* aElementName, 
                                            const char* aVisualName,
                                            WidgetPointerEvent aEvent )
    {
    mVisualRef.reset(AlfReferenceToVisual::create(aElementName, aVisualName, true ));
    mEvent = aEvent;
    }

// ---------------------------------------------------------------------------
// ?description_if_needed
// ---------------------------------------------------------------------------
//
AlfPointerEventInput::~AlfPointerEventInput()
    {
    }

// ---------------------------------------------------------------------------
// ?implementation_description
// ---------------------------------------------------------------------------
//
bool AlfPointerEventInput::accept(
    CAlfWidgetControl& aControl, const TAlfEvent& aEvent ) const
    {
    // Check whether the tapped visual matches with element and visual names
    // of this pointer event input.
    bool ret = false;

    // map OSN pointer event to TPointerEvent::TType
    TPointerEvent::TType eventType = TPointerEvent::EButton1Down;
    switch (mEvent)
        {
        case pointerEventDown:
            eventType = TPointerEvent::EButton1Down;
            break;
        case pointerEventUp:
            eventType = TPointerEvent::EButton1Up;
            break;
        case pointerEventDrag:
            eventType = TPointerEvent::EDrag;
            break;
        default:
            break;
        }
    
    ret = aEvent.IsPointerEvent() && 
          (eventType == aEvent.PointerEvent().iType) && 
          mVisualRef->resolve( aControl, aEvent.Visual());

    return ret;
    }
// ---------------------------------------------------------------------------
// Class AlfWidgetAttributeInput
// ---------------------------------------------------------------------------
//
AlfWidgetAttributeInput::AlfWidgetAttributeInput( AlfAttribute& aAttribute,
	    const char* aElementId )
	    {
	    mAttr = &aAttribute;
	    mElementId = "";
	    if (aElementId)   
	        {
	        mElementId = aElementId;
	        }
	    }

AlfWidgetAttributeInput::AlfWidgetAttributeInput()
    {
    }

AlfWidgetAttributeInput::~AlfWidgetAttributeInput()
    {
    delete mAttr;
    }
    
bool AlfWidgetAttributeInput::accept(const TAlfEvent& aEvent) const
    {
    bool accept(false);
    
    if (aEvent.IsCustomEvent())
        {
        unsigned int eventID = aEvent.CustomParameter(); 
        if (eventID == EEventWidgetAttributeChanged)
            {
            TAlfWidgetAttributeCommand* command = 
                (TAlfWidgetAttributeCommand*)aEvent.CustomEventData();
            AlfAttribute* attribute = command->mOwnedAttribute;
            const char* name = attribute->name();
            const char* targetelement = command->mTarget.getUtf8();
                        
            accept = true;
            if (!mElementId.isEmpty() && targetelement)
            	{
	            if ( strcmp(mElementId.getUtf8(), targetelement) )
	                {
	                accept = false;
	                }
            	}
            
            if (accept)
            	{
            	accept= false;
	            if (!strcmp(mAttr->name(), name))
	                {
	                accept = true;
	                }
            	}
            }
        }
    
    return accept;
    }    
// ---------------------------------------------------------------------------
// Class AlfEventPasserInput
// ---------------------------------------------------------------------------
//
// AlfEventInput could be used instead of this class, it contains all the same
// functionality! 
//
AlfEventPasserInput::AlfEventPasserInput( int aEventId, int aEventData  )
    {
    mEventId = aEventId;
    mEventData = aEventData;//not used yet...
    }

AlfEventPasserInput::AlfEventPasserInput()
    {
    }

AlfEventPasserInput::~AlfEventPasserInput()
    {
    }
    
bool AlfEventPasserInput::accept( const TAlfEvent& aEvent ) const
    {
    // check whether the event matches to the one we need to pass to another widget

    return aEvent.IsCustomEvent() &&
        aEvent.CustomParameter() == mEventId;    
    }    
        
    }//Alf

// End of File.
