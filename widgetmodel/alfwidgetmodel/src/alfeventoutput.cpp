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
* Description:   File Implements the various output events, including custom events.
*
*/


#include <alf/alfevent.h>
#include <alf/alfcommand.h>
#include <alf/alfenv.h>
#include <osn/ustring.h>

#include <alf/alfwidgetcontrol.h>
#include "alf/alfreferencetovisual.h"
#include <alf/alfvarianttype.h>
#include "alf/alfwidget.h"
#include <alf/alfexceptions.h>
#include "alf/alfattribute.h"
#include "alf/alfattributevaluetype.h"
#include "alf/ialfattributeowner.h"
#include <alf/alfeventoutputsignalsubscriber.h>
#include <alf/ialfappeventlistener.h>
#include <alf/alfwidgetcommand.h>
#include <alf/alfwidgetenvextension.h>
#include <alf/ialfwidgetfactory.h>

#include "alfeventoutput.h"

namespace Alf
    {

// ======== LOCAL FUNCTIONS ========

void DeleteIAlfVariantType( TAny* aParam )
    {
    IAlfVariantType* ptr = static_cast<IAlfVariantType*>( aParam );
    delete ptr;
    }

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Class AlfEventOutput
// ---------------------------------------------------------------------------
//
AlfEventOutput::AlfEventOutput(
    int aEventType, int aEventID, const char* aTargetWidget, unsigned int aDelay )
    {
    if ( aEventType != TAlfEvent::ETypeCustom &&
            aEventType != TAlfEvent::ETypeKey )
        {
        ALF_THROW(AlfVisualException,EInvalidAttribute,"AlfEventOutput")
        }

    // Key event outputs must have a target widget.
    if ( aEventType == TAlfEvent::ETypeKey &&
            !UString(aTargetWidget).compare(UString("")) )
        {
        ALF_THROW(AlfVisualException,EInvalidAttribute,"AlfEventOutput")
        }

    mEventType = aEventType;
    mEventID = aEventID;
    mTargetWidget = UString(aTargetWidget);

    mDelay = aDelay;
    }
AlfEventOutput::AlfEventOutput()
    {
    }

AlfEventOutput::~AlfEventOutput()
    {
    }

// ---------------------------------------------------------------------------
// ?implementation_description
// ---------------------------------------------------------------------------
//
void AlfEventOutput::execute( const CAlfWidgetControl& aControl, const TAlfEvent& aEvent )
    {
    CAlfEnv& env = aControl.Env();
    CAlfWidgetControl* controller = NULL;

    if ( mTargetWidget.compare( UString("") ) )
        {
        IAlfWidget* widget = AlfWidgetEnvExtension::widgetFactory(env).findWidget( mTargetWidget.getUtf8() );

        if ( !widget )
            {
            ALF_THROW(AlfWidgetException,EInvalidWidget,"AlfEventOutput")
            }
        controller = widget->control();
        }

    if ( mEventType == TAlfEvent::ETypeCustom )
        {
        // control is NULL if the event is to be broadcast

        TAlfCustomEventCommand command( mEventID, controller, aEvent.CustomEventData() );
        env.Send( command, mDelay );
        }
    else if ( mEventType == TAlfEvent::ETypeKey && controller )
        {
        // Simulate a key event with a given event ID as scan code.
        
        TKeyEvent key =
            {
            0,          // iCode
            mEventID,   // iScanCode
            0,          // iModifiers
            0           // iRepeats
            };

        TAlfEvent event( *controller->Display(), key, EEventKey );
        controller->processEvent( event );
        }
    }

// ---------------------------------------------------------------------------
// Class AlfEventCancelOutput
// ---------------------------------------------------------------------------
//
AlfEventCancelOutput::AlfEventCancelOutput(
    const char* aTargetWidget, int aEventID, bool aCancelAll )
    {
    mTargetWidget = UString(aTargetWidget);
    mEventID = aEventID;
    mCancelAll = aCancelAll;
    }

AlfEventCancelOutput::AlfEventCancelOutput()
    {
    }

AlfEventCancelOutput::~AlfEventCancelOutput()
    {
    }

// ---------------------------------------------------------------------------
// ?implementation_description
// ---------------------------------------------------------------------------
//
void AlfEventCancelOutput::execute( const CAlfWidgetControl& aControl )
    {
    // This cancels either all or specified command in the given
    // target control.
    
    CAlfEnv& env = aControl.Env();

    IAlfWidget* widget = AlfWidgetEnvExtension::widgetFactory(env).findWidget( mTargetWidget.getUtf8() );

    if ( !widget )
        {
        ALF_THROW(AlfWidgetException,EInvalidWidget,"AlfEventCancelOutput")
        }

    CAlfControl* controller = widget->control();

    if ( mCancelAll )
        {
        env.CancelCustomCommands( controller );
        }
    else
        {
        env.CancelCustomCommands( controller, mEventID );
        }
    }

// ---------------------------------------------------------------------------
// Class AlfAnimationCancelOutput
// ---------------------------------------------------------------------------
//
AlfAnimationCancelOutput::AlfAnimationCancelOutput(
    const char* aElementName,
    const char* aVisualName,
    TAlfOp aOperation,
    bool aCancelAll )
    {
    mVisualRef.reset(AlfReferenceToVisual::create( aElementName, aVisualName, true ));

    mOperation = aOperation;
    mCancelAll = aCancelAll;
    }

AlfAnimationCancelOutput::AlfAnimationCancelOutput()
    {
    }

AlfAnimationCancelOutput::~AlfAnimationCancelOutput()
    {
    }
// ---------------------------------------------------------------------------
// ?implementation_description
// ---------------------------------------------------------------------------
//
void AlfAnimationCancelOutput::execute(
    CAlfWidgetControl& aControl, int aDataID )
    {
    CAlfEnv& env = aControl.Env();
    CAlfVisual* visual=NULL;

    visual = mVisualRef->resolve( aControl, aDataID );

    if ( visual )
        {
        if ( mCancelAll )
            {
            env.CancelCommands( visual );
            }
        else
            {
            env.CancelCommands( visual, mOperation );
            }
        }
    }

// ---------------------------------------------------------------------------
// Class AlfWidgetAttributeOuptut
// ---------------------------------------------------------------------------
//
AlfWidgetAttributeOutput::AlfWidgetAttributeOutput( AlfAttribute& aAttribute,
    const char* aWidgetId )
    {
    mAttr = &aAttribute;
    mWidgetId = "";
    if (aWidgetId)   
        {
        mWidgetId = aWidgetId;
        }
    }

AlfWidgetAttributeOutput::AlfWidgetAttributeOutput()
    {
    }

AlfWidgetAttributeOutput::~AlfWidgetAttributeOutput()
    {
    delete mAttr;
    }

void AlfWidgetAttributeOutput::execute( CAlfWidgetControl& aControl )
    {
    AlfWidget* widget = aControl.widget();
    
    IAlfAttributeOwner* owner = 0;
    if (mWidgetId.isEmpty())
        {
        owner = IAlfInterfaceBase::makeInterface<IAlfAttributeOwner>(widget);
        }
    else
        {
        IAlfWidget* widget1 = AlfWidgetEnvExtension::widgetFactory(aControl.Env()).findWidget(mWidgetId.getUtf8());
        owner = IAlfInterfaceBase::makeInterface<IAlfAttributeOwner>(widget1);
        }

    if ( owner )
        {        
        owner->setAttribute( *mAttr );
        UString target;
        TAlfWidgetAttributeCommand command( mAttr, target, widget );
        CAlfEnv& env = aControl.Env();
        env.Send( command, 0);
        command.mOwnedAttribute = NULL;
        }
    }


#ifdef RD_TACTILE_FEEDBACK
// ---------------------------------------------------------------------------
// Class AlfWidgetTactileOutput
// ---------------------------------------------------------------------------
//
AlfTactileOutput::AlfTactileOutput( TTouchLogicalFeedback aFeedbackType )
    {
    mFeedbackType = aFeedbackType;
    }

AlfTactileOutput::AlfTactileOutput()
    {
    }

AlfTactileOutput::~AlfTactileOutput()
    {
    }

void AlfTactileOutput::execute()
    {
    MTouchFeedback* feedback = MTouchFeedback::Instance();
    if ( feedback )
        {
        feedback->InstantFeedback( mFeedbackType );
        }
    }

#endif // RD_TACTILE_FEEDBACK


// ---------------------------------------------------------------------------
// Class AlfEventOutputSignal
// ---------------------------------------------------------------------------
//
AlfEventOutputSignal::AlfEventOutputSignal( 
	IAlfEventOutputSignalSubscriber& aSubscriber )
	: mSubscriber(&aSubscriber)
    {}

AlfEventOutputSignal::~AlfEventOutputSignal()
    {
    delete mSubscriber;
    }

void AlfEventOutputSignal::execute()
    {
    mSubscriber->receiveSignal();
    }

// ---------------------------------------------------------------------------
// Class AlfStateChangeOutput
// ---------------------------------------------------------------------------
//
AlfStateChangeOutput::AlfStateChangeOutput(
    const char* aTargetWidget, uint aEnableStates, uint aDisableStates ):
    mTargetWidget(aTargetWidget),
    mEnableStates(aEnableStates),
    mDisableStates(aDisableStates)
    {

    // State change events output must have a target widget.
    if ( !aTargetWidget || aTargetWidget[0] == 0 )
        {
        ALF_THROW(AlfWidgetException,EInvalidWidget,"AlfStateChangeOutput")
        }

    // State change events must not have ambiguous overlapping bits
    if ( mEnableStates & mDisableStates  )
        {
        ALF_THROW(AlfWidgetException,EInvalidArgument,"AlfStateChangeOutput")
        }

    }

AlfStateChangeOutput::~AlfStateChangeOutput()
    {
    }

// ---------------------------------------------------------------------------
// ?implementation_description
// ---------------------------------------------------------------------------
//
void AlfStateChangeOutput::execute( const CAlfWidgetControl& aControl)
    {
    CAlfEnv& env = aControl.Env();

    if ( mTargetWidget.compare( UString("") ) )
        {
        IAlfWidget* widget = AlfWidgetEnvExtension::widgetFactory(env).findWidget( mTargetWidget.getUtf8() );

        if ( !widget )
            {
            ALF_THROW(AlfWidgetException,EInvalidWidget,"AlfStateChangeOutput")
            }
        CAlfWidgetControl* target = widget->control();
        target->enableState( mEnableStates);
        target->disableState(mDisableStates);
        }
    }

// ---------------------------------------------------------------------------
// Class AlfEventPasserOutput
// ---------------------------------------------------------------------------
//
AlfEventPasserOutput::AlfEventPasserOutput( const char* aSourceWidget, const char* aTargetWidget ):
    mSourceWidget(aSourceWidget),
    mTargetWidget(aTargetWidget)                     
    {
    
    mEventMappingData = false;
    
    // Event passer events output must have a target widget.
    if ( !mTargetWidget )
        {
        ALF_THROW(AlfWidgetException,EInvalidWidget,"AlfEventPasserOutput")
        }
    }

// ---------------------------------------------------------------------------
// Class AlfEventPasserOutput
// ---------------------------------------------------------------------------
//
AlfEventPasserOutput::AlfEventPasserOutput( const char* aSourceWidget , const char* aTargetWidget, 
                                            int aNewEventId , int aNewEventCustomData ):
    mSourceWidget(aSourceWidget),
    mTargetWidget(aTargetWidget),
    mNewEventId(aNewEventId),
    mNewEventCustomData(aNewEventCustomData)                     
    {
    
    mEventMappingData = true;
    // Event passer events output must have a target widget.
    if ( !mTargetWidget )
        {
        ALF_THROW(AlfWidgetException,EInvalidWidget,"AlfEventPasserOutput")
        }
    }

AlfEventPasserOutput::AlfEventPasserOutput()
    {
    }

AlfEventPasserOutput::~AlfEventPasserOutput()
    {
    }

void AlfEventPasserOutput::execute( const CAlfWidgetControl* aControl , const TAlfEvent& aEvent )
    {
    if(mTargetWidget)
        {
        IAlfWidget* wdgt = AlfWidgetEnvExtension::widgetFactory(aControl->Env()).findWidget(mTargetWidget);
        //const char* wname = wdgt->widgetName(); //for debugging purposes
        // if widget is not found, ignore...
        if( wdgt  )
            {
            if(!mEventMappingData)
                {
                //pass to widgets control. The event handler that handles the event
                //is found from the target widgets controls event handler list.
                wdgt->control()->processEvent(aEvent);     
                }
            else
                {
                //map event to other type of event. Use user set event ids and custom data.
                wdgt->control()->processEvent(TAlfEvent(mNewEventId , mNewEventCustomData));      
                }
            }

        }
    }

// ---------------------------------------------------------------------------
// Class AlfAppEventOutput
// ---------------------------------------------------------------------------
//
AlfAppEventOutput::AlfAppEventOutput( const UString& aCmd, IAlfAppEventListener& aAppEvent ):
    mAppEvent(&aAppEvent),
    mCmd(aCmd)
    {
    }

AlfAppEventOutput::~AlfAppEventOutput()
    {
    }

void AlfAppEventOutput::execute(const TAlfEvent& aEvent)
    {
    mAppEvent->handleApplicationEvent(mCmd, aEvent);
    }

    
    } // namespace Alf
        
// End of File.
