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
* Description:   File implements eventhandler in widgetmodel.
*
*/

#include <alf/alftypes.h>
#include <osn/ustring.h>

#include "alf/alfwidgeteventhandler.h"
#include <alf/alfwidgetcontrol.h>
#include <alf/alfvarianttype.h>
#include <alf/alfexceptions.h>
#include "alf/alfattribute.h"
#include <alf/alfeventoutputsignalsubscriber.h>
#include <alf/ialfappeventlistener.h>
#include <stdexcept>
#include <osn/osnnew.h>

#ifdef RD_TACTILE_FEEDBACK
#include <touchfeedback.h>
#endif // RD_TACTILE_FEEDBACK

#include "alfwidgeteventhandlerimpl.h"
#include "alfanimationoutput.h"
#include "alfeventinput.h"
#include "alfeventoutput.h"


namespace Alf
    {
// ======== LOCAL FUNCTIONS ========

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// ?description_if_needed
// ---------------------------------------------------------------------------
//
OSN_EXPORT AlfWidgetEventHandler::AlfWidgetEventHandler(
    IAlfWidgetEventHandler::AlfEventHandlerType aEventHandlerType,
    IAlfWidgetEventHandler::AlfEventHandlerExecutionPhase aEventHandlerExecutionPhase )
    {
    mImpl.reset( new (EMM) AlfWidgetEventHandlerImpl(
        aEventHandlerType, aEventHandlerExecutionPhase ) );
    }

// ---------------------------------------------------------------------------
// ?description_if_needed
// ---------------------------------------------------------------------------
//
AlfWidgetEventHandler::~AlfWidgetEventHandler()
    {
    }

// ---------------------------------------------------------------------------
// ?implementation_description
// ---------------------------------------------------------------------------
//
OSN_EXPORT void AlfWidgetEventHandler::addKeyInput(
    int aEventID,
    EventTypeFlags aEventTypeFlags,
    uint aModifiers )
    {
    if ( aEventTypeFlags != keyDown && aEventTypeFlags != keyUp )
        {
        ALF_THROW(
            AlfException,
            EInvalidArgument,
            "AlfWidgetEventHandler - invalid event type given in addKeyInput()" );
        }
    
    mImpl->mInputs.resize(mImpl->mInputs.count()+1);
    AlfEventInput* input = new( EMM ) AlfEventInput( aEventTypeFlags, aEventID, 0, aModifiers );
    mImpl->mInputs.insert(mImpl->mInputs.count(),input);
    }

// ---------------------------------------------------------------------------
// ?implementation_description
// ---------------------------------------------------------------------------
//
OSN_EXPORT void AlfWidgetEventHandler::addCustomInput(
    int aEventID, bool aUseEventData, int aEventData )
    {
    mImpl->mInputs.resize(mImpl->mInputs.count()+1);
    AlfEventInput* input = new( EMM ) AlfEventInput(custom, aEventID, aEventData, 0 );
    if ( aUseEventData )
        {
        input->mEventTypeFlags |= useEventData;
        }

    mImpl->mInputs.insert(mImpl->mInputs.count(), input);
    }

// ---------------------------------------------------------------------------
// ?implementation_description
// ---------------------------------------------------------------------------
//
OSN_EXPORT void AlfWidgetEventHandler::addPointerInput(
    const char* aElementName, const char* aVisualName, 
    WidgetPointerEvent aEvent )
    {
    mImpl->mPointerInputs.resize(mImpl->mPointerInputs.count()+1);
    AlfPointerEventInput* input = new( EMM ) AlfPointerEventInput( aElementName, aVisualName, aEvent );
    mImpl->mPointerInputs.insert(mImpl->mPointerInputs.count(), input);
    }

// ---------------------------------------------------------------------------
// ?implementation_description
// ---------------------------------------------------------------------------

OSN_EXPORT void AlfWidgetEventHandler::addOutput(
    int aEventType,
    int aEventID,
    const char* aTargetControl,
    unsigned int aDelay )
    {
    mImpl->mOutputs.resize(mImpl->mOutputs.count()+1);
    AlfEventOutput* output = new( EMM ) AlfEventOutput( aEventType, aEventID, aTargetControl, aDelay );
    mImpl->mOutputs.insert(mImpl->mOutputs.count(), output);
    }

// ---------------------------------------------------------------------------
// ?implementation_description
// ---------------------------------------------------------------------------
//
OSN_EXPORT void AlfWidgetEventHandler::addAnimationOutput(
    AlfReferenceToVisual* aTargetVisual,
    AlfReferenceToVisual* aReferenceVisual,
    IAlfAttributeSetter* aAttributeSetter,
    AlfAttributeContainer* aAttributeContainer,
    unsigned int aAnimationTime,
    unsigned int aDelay )
    {
    mImpl->mAnimationOutputs.resize(mImpl->mAnimationOutputs.count()+1);

    AlfAnimationOutput* output = new( EMM ) AlfAnimationOutput(
        aTargetVisual,
        aReferenceVisual,
        aAttributeSetter,
        aAttributeContainer,
        aAnimationTime,
        aDelay );

    // This should always succeed, because the array has already been resized.
    mImpl->mAnimationOutputs.insert(mImpl->mAnimationOutputs.count(), output );
    }

// ---------------------------------------------------------------------------
// ?implementation_description
// ---------------------------------------------------------------------------
//
OSN_EXPORT void AlfWidgetEventHandler::addCancelOutput(
    const char* aTargetWidget, int aEventID, bool aCancelAll )
    {
    mImpl->mCancelOutputs.resize(mImpl->mCancelOutputs.count()+1);
    AlfEventCancelOutput* output = new( EMM ) AlfEventCancelOutput( aTargetWidget, aEventID, aCancelAll );
    mImpl->mCancelOutputs.insert(mImpl->mCancelOutputs.count(), output);
    }

// ---------------------------------------------------------------------------
// ?implementation_description
// ---------------------------------------------------------------------------
//
OSN_EXPORT void AlfWidgetEventHandler::addAnimationCancelOutput(
    const char* aElementName,
    const char* aVisualName,
    TAlfOp aOperation,
    bool aCancelAll )
    {
    mImpl->mAnimationCancelOutputs.resize(mImpl->mAnimationCancelOutputs.count()+1);
    AlfAnimationCancelOutput* output = new( EMM ) AlfAnimationCancelOutput(aElementName, aVisualName, aOperation, aCancelAll );
    mImpl->mAnimationCancelOutputs.insert(mImpl->mAnimationCancelOutputs.count(), output);
    }

// ---------------------------------------------------------------------------
// AddWidgetAttributeOutput
// ---------------------------------------------------------------------------
//
OSN_EXPORT void AlfWidgetEventHandler::addWidgetAttributeOutput(
    AlfAttribute& aAttribute, const char* aWidgetId )
    {
    mImpl->mWidgetAttributeOutputs.resize( mImpl->mWidgetAttributeOutputs.count() + 1 );
    AlfWidgetAttributeOutput* output = new (EMM) AlfWidgetAttributeOutput( aAttribute, aWidgetId );
    mImpl->mWidgetAttributeOutputs.insert( mImpl->mWidgetAttributeOutputs.count(), output );
    }

// ---------------------------------------------------------------------------
// addWidgetAttributeOutput
// ---------------------------------------------------------------------------
//
OSN_EXPORT void AlfWidgetEventHandler::addWidgetAttributeOutput(
    UString& aName, AlfAttributeValueType* aValue, const char* aWidgetId )
    {
    auto_ptr<AlfAttribute> attr(
        new( EMM ) AlfAttribute(aName.getUtf8(), AlfAttribute::EStatic ) );
    attr->addTargetValue( aValue );

    mImpl->mWidgetAttributeOutputs.resize( mImpl->mWidgetAttributeOutputs.count() + 1 );
    // Add attribute to attribute output. Onwership is transferred.
    AlfWidgetAttributeOutput* output = new (EMM) AlfWidgetAttributeOutput( *attr.get(), aWidgetId );
    attr.release();
    mImpl->mWidgetAttributeOutputs.insert( mImpl->mWidgetAttributeOutputs.count(), output );
    }
    
// ---------------------------------------------------------------------------
// addWidgetAttributeInput
// ---------------------------------------------------------------------------
//
OSN_EXPORT void AlfWidgetEventHandler::addWidgetAttributeInput(
	UString& aName, AlfAttributeValueType* aValue, const char* aElementId )
    {
    auto_ptr<AlfAttribute> attr(
        new( EMM ) AlfAttribute(aName.getUtf8(), AlfAttribute::EStatic ) );
    attr->addTargetValue( aValue );

    mImpl->mWidgetAttributeInputs.resize( mImpl->mWidgetAttributeInputs.count() + 1 );
    // Add attribute to attribute input. Onwership is transferred.
    AlfWidgetAttributeInput* input = new (EMM) AlfWidgetAttributeInput( *attr.get(), aElementId );
    attr.release();
    mImpl->mWidgetAttributeInputs.insert( mImpl->mWidgetAttributeInputs.count(), input );
    }

// ---------------------------------------------------------------------------
// addTactileOutput
// ---------------------------------------------------------------------------
//
OSN_EXPORT void AlfWidgetEventHandler::addTactileOutput( 
    WidgetTactileFeedbackType aFeedbackType )
    {
#ifdef RD_TACTILE_FEEDBACK
	// map the feedback type to official tactile feedback type
    TTouchLogicalFeedback feedback = ETouchFeedbackNone;

    switch ( aFeedbackType )
        {
        case tactileFeedbackBasic:
            feedback = ETouchFeedbackBasic;
            break;
        case tactileFeedbackSensitive:
            feedback = ETouchFeedbackSensitive;
            break;
        case tactileFeedbackNone:
        default:
            break;
        }

    // create tactile output
    mImpl->mTactileOutputs.resize( mImpl->mTactileOutputs.count() + 1 );
    AlfTactileOutput* output = new (EMM) AlfTactileOutput( feedback );
    mImpl->mTactileOutputs.insert( mImpl->mTactileOutputs.count(), output );
#endif // RD_TACTILE_FEEDBACK
    }

// ---------------------------------------------------------------------------
// addEventPasserInput
// ---------------------------------------------------------------------------
//
OSN_EXPORT void AlfWidgetEventHandler::addEventPasserInput (
    int aEventId, int aCustomData )
    {
    mImpl->mEventPasserInputs.resize( mImpl->mEventPasserInputs.count() + 1 );                                                                      
    
    AlfEventPasserInput* input = new( EMM ) AlfEventPasserInput(
        aEventId, aCustomData );
    mImpl->mEventPasserInputs.insert( mImpl->mEventPasserInputs.count(), input ); 
    }

// ---------------------------------------------------------------------------
// addEventPasserOutput
// ---------------------------------------------------------------------------
//
OSN_EXPORT void AlfWidgetEventHandler::addEventPasserOutput( 
    const char* aSourceWidget , const char* aTargetWidget )
    {
    mImpl->mEventPasserOutputs.resize( mImpl->mEventPasserOutputs.count() + 1 );

    //create new output here
    AlfEventPasserOutput* output = new( EMM ) AlfEventPasserOutput(
        aSourceWidget, aTargetWidget );
        
    mImpl->mEventPasserOutputs.insert( mImpl->mEventPasserOutputs.count(), output );
    }

// ---------------------------------------------------------------------------
// addEventPasserOutput
// ---------------------------------------------------------------------------
//
OSN_EXPORT void AlfWidgetEventHandler::addEventPasserOutput( 
    const char* aSourceWidget , const char* aTargetWidget, int aNewEventId, int aNewEventCustomData )
    {
    //create new output here
    auto_ptr<AlfEventPasserOutput> output(new (EMM) AlfEventPasserOutput( aSourceWidget,
                                                                          aTargetWidget,
                                                                          aNewEventId,
                                                                          aNewEventCustomData ));
    mImpl->mEventPasserOutputs.resize( mImpl->mEventPasserOutputs.count() + 1 );                                                                      
    mImpl->mEventPasserOutputs.insert( mImpl->mEventPasserOutputs.count(), output.get() ); 
    output.release();        
    }
    
// ---------------------------------------------------------------------------
// addWidgetEventOutputSignalSubscriber
// ---------------------------------------------------------------------------
//
OSN_EXPORT void AlfWidgetEventHandler::addWidgetEventOutputSignalSubscriber(
	IAlfEventOutputSignalSubscriber& aSubscriber )
	{
	auto_ptr<AlfEventOutputSignal> signal(new (EMM) AlfEventOutputSignal( aSubscriber ));
	mImpl->mWidgetEventOutputSignals.resize( mImpl->mWidgetEventOutputSignals.count() + 1 );
	mImpl->mWidgetEventOutputSignals.insert( mImpl->mWidgetEventOutputSignals.count(), signal.get() );
	signal.release();
	}

// ---------------------------------------------------------------------------
// addWidgetStateOutput
// ---------------------------------------------------------------------------
//
OSN_EXPORT void AlfWidgetEventHandler::addWidgetStateOutput( 
    const UString& aName, uint aEnableStates, uint aDisableStates )
    {
    auto_ptr<AlfStateChangeOutput> output(new (EMM) AlfStateChangeOutput( aName.getUtf8(), aEnableStates, aDisableStates ));
    mImpl->mStateChangeOutputs.resize( mImpl->mStateChangeOutputs.count() + 1 );
    mImpl->mStateChangeOutputs.insert( mImpl->mStateChangeOutputs.count(), output.get() ); 
    output.release();
    }

// ---------------------------------------------------------------------------
// addAppEventOutput
// ---------------------------------------------------------------------------
//
OSN_EXPORT void AlfWidgetEventHandler::addAppEventOutput(const UString& aCmd , IAlfAppEventListener& aAppEvent )
    {
    auto_ptr<AlfAppEventOutput> appOutput(new (EMM) AlfAppEventOutput( aCmd, aAppEvent ));
    mImpl->mAppEventOutputs.resize( mImpl->mAppEventOutputs.count() + 1 );
    mImpl->mAppEventOutputs.insert( mImpl->mAppEventOutputs.count(), appOutput.get() );
    appOutput.release();
    }
    
// ---------------------------------------------------------------------------
// From class IAlfWidgetEventHandler.
// ?implementation_description
// ---------------------------------------------------------------------------
//
bool AlfWidgetEventHandler::accept(
    CAlfWidgetControl& aControl, const TAlfEvent& aEvent ) const
    {
    bool ret = false;

    // Custom and key events
    if ( !aEvent.IsPointerEvent() )
        {
        int count = mImpl->mInputs.count();

        for ( int i = 0 ; i < count ; i++ )
            {
            if ( mImpl->mInputs[i]->accept( aEvent ) )
                {
                ret = true;
                break;
                }
            }
        }

    // Pointer events
    else
        {
        int count = mImpl->mPointerInputs.count();

        for ( int i = 0 ; i < count ; i++ )
            {
            if ( mImpl->mPointerInputs[i]->accept( aControl, aEvent ) )
                {
                ret = true;
                break;
                }
            }
        }

    // All the inputs below handle only custom events.
    
    if (!ret) 
        {
        int count = mImpl->mWidgetAttributeInputs.count();

        for ( int i = 0 ; i < count ; i++ )
            {
            if ( mImpl->mWidgetAttributeInputs[i]->accept( aEvent ) )
                {
                ret = true;
                break;
                }
            }
        }
    
    if (!ret) 
        {
        int count = mImpl->mEventPasserInputs.count();

        for ( int i = 0 ; i < count ; i++ )
            {
            if ( mImpl->mEventPasserInputs[i]->accept( aEvent ) )
                {
                ret = true;
                break;
                }
            }
        }        

    return ret;
    }

// ---------------------------------------------------------------------------
// From class IAlfWidgetEventHandler.
// ?implementation_description
// ---------------------------------------------------------------------------
//
AlfEventStatus AlfWidgetEventHandler::offerEvent( 
    CAlfWidgetControl& aControl,
    const TAlfEvent& aEvent )
    {
    AlfEventStatus ret(EEventNotHandled);
        
    // If the event handler is active in the control's current state and
    // any of the inputs accept the event, execute all outputs.

    if ( ( (mImpl->mActiveStates == ~0) ||
            (aControl.state() & mImpl->mActiveStates) ) &&
            accept( aControl, aEvent ) )
        {
        ret = EEventHandled;

        // Normal event outputs, property outputs and cancel outputs
        // are executed with all types of events (custom, key, pointer).

        int i;
        int count = mImpl->mCancelOutputs.count();

        for ( i = 0 ; i < count ; i++ )
            {
            mImpl->mCancelOutputs[i]->execute( aControl );
            }

        count = mImpl->mOutputs.count();

        for ( i = 0 ; i < count ; i++ )
            {
            mImpl->mOutputs[i]->execute( aControl, aEvent );
            }

        // Tactile outputs
#ifdef RD_TACTILE_FEEDBACK
        count = mImpl->mTactileOutputs.count();

        for ( i = 0 ; i < count ; i++ )
            {
            mImpl->mTactileOutputs[i]->execute();
            }
#endif // RD_TACTILE_FEEDBACK

        // widget attribute outputs
        count = mImpl->mWidgetAttributeOutputs.count();

        for ( i = 0 ; i < count ; i++ )
            {
            mImpl->mWidgetAttributeOutputs[i]->execute(
                aControl );
            }

        // Event output signal subscribers
        count = mImpl->mWidgetEventOutputSignals.count();
        for ( i = 0 ; i < count ; i++ )
            {
            mImpl->mWidgetEventOutputSignals[i]->execute();
            }
        
        // Animation event outputs, animation cancel outputs and property outputs
        // are executed only with custom events.

        if ( aEvent.IsCustomEvent() )
            {
            // Custom events have the associated data ID for finding the target
            // visual in the event data parameter.

            int dataID = aEvent.CustomEventData();

            count = mImpl->mAnimationCancelOutputs.count();

            for ( i = 0 ; i < count ; i++ )
                {
                mImpl->mAnimationCancelOutputs[i]->execute( aControl, dataID );
                }

            count = mImpl->mAnimationOutputs.count();

            for ( i = 0 ; i < count ; i++ )
                {
                mImpl->mAnimationOutputs[i]->sendCommand( aControl, dataID );
                }
            }

        // State change outputs
        count = mImpl->mStateChangeOutputs.count();
        for( i = 0; i < count; i++)
            {
            mImpl->mStateChangeOutputs[i]->execute( aControl );
            }
            
        // Event passer outputs
        count = mImpl->mEventPasserOutputs.count();
        for( i = 0; i < count; i++)
            {
            mImpl->mEventPasserOutputs[i]->execute( &aControl , aEvent );
            }      
        
        // AlfAppEvent outputs
        count = mImpl->mAppEventOutputs.count();
        for( i = 0; i < count; i++)
            {
            mImpl->mAppEventOutputs[i]->execute( aEvent );
            } 
        }

    return ret;
    }

// ---------------------------------------------------------------------------
// From class IAlfWidgetEventHandler.
// ---------------------------------------------------------------------------
//
void AlfWidgetEventHandler::setActiveStates( unsigned int aStates )
    {
    mImpl->mActiveStates = aStates;
    }

// ---------------------------------------------------------------------------
// From class IAlfWidgetEventHandler.
// ?implementation_description
// ---------------------------------------------------------------------------
//
void AlfWidgetEventHandler::setCapturing( bool aCapturing )
    {
    mImpl->mCapturing = aCapturing;
    }

// ---------------------------------------------------------------------------
// From class IAlfWidgetEventHandler.
// ?implementation_description
// ---------------------------------------------------------------------------
//
bool AlfWidgetEventHandler::capturing() const
    {
    return mImpl->mCapturing;
    }

// ---------------------------------------------------------------------------
// From class MAlfInterfaceBase.
// ?implementation_description
// ---------------------------------------------------------------------------
//
IAlfInterfaceBase* AlfWidgetEventHandler::makeInterface( const IfId& aType )
    {
    UString param( aType.mImplementationId );
    if ( param == IAlfWidgetEventHandler::type().mImplementationId )
        {
        return static_cast<IAlfWidgetEventHandler*>( this );
        }

    return NULL;
    }
    
// ---------------------------------------------------------------------------
// setEventHandlerData
// ---------------------------------------------------------------------------
//    
void AlfWidgetEventHandler::setEventHandlerData(const AlfWidgetEventHandlerInitData& aData)
	{
	// Set event handler id
	if (aData.mWidgetEventHandlerId)
	    {
	    int len = strlen(aData.mWidgetEventHandlerId);
	    char* tmp = new( EMM ) char[len + 1];
	    
        delete[] mImpl->mEhData.mWidgetEventHandlerId;
	        
        strcpy(tmp, aData.mWidgetEventHandlerId);
	    tmp[len] = 0;
	    mImpl->mEhData.mWidgetEventHandlerId = tmp;
	    }
	// we don't need to set node at the moment
	mImpl->mEhData.mNode = 0;
	}

// ---------------------------------------------------------------------------
// eventHandlerData
// ---------------------------------------------------------------------------
//
AlfWidgetEventHandlerInitData* AlfWidgetEventHandler::eventHandlerData()
	{
	return &mImpl->mEhData;
	}

// ---------------------------------------------------------------------------
// eventHandlerType
// ---------------------------------------------------------------------------
//
IAlfWidgetEventHandler::AlfEventHandlerType AlfWidgetEventHandler::eventHandlerType()
    {
    return mImpl->mEventHandlerType;
    }

// ---------------------------------------------------------------------------
// eventExecutionPhase
// ---------------------------------------------------------------------------
//
IAlfWidgetEventHandler::AlfEventHandlerExecutionPhase AlfWidgetEventHandler::eventExecutionPhase()
    {
    return mImpl->mEventHandlerExecutionPhase;
    }
}
// End of File.
