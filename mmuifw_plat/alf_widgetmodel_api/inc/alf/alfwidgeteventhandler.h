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
* Description:   Header for generic event handler for alf widgets
*
*/


#ifndef ALFWIDGETEVENTHANDLER_H
#define ALFWIDGETEVENTHANDLER_H


#include <osn/osndefines.h>
#include <osn/osncommon.h>
#include <alf/alfevent.h>
#include <alf/alfcommand.h>
#include <alf/ialfwidgeteventhandler.h>
#include <alf/alfwidgetevents.h>
using namespace osncore;

#include <memory>
using namespace std;

namespace osncore
    {
class UString;
    }

using osncore::UString;

namespace Alf
    {
class CAlfWidgetControl;
class AlfWidget;
class AlfReferenceToVisual;
class IAlfAttributeSetter;
class AlfAttributeContainer;
class AlfWidgetEventHandlerImpl;
class AlfAttribute;
class AlfAttributeValueType;
class IfId;
class IAlfEventOutputSignalSubscriber;
class IAlfAppEventListener;

// Custom event IDs defined by the event handling framework.
enum
    {
    EActivateState = 1000000,
    EDeactivateState
    };

enum WidgetTactileFeedbackType
    {
    tactileFeedbackNone,
    tactileFeedbackBasic,
    tactileFeedbackSensitive
    };

/** @class AlfWidgetEventHandler alfwidgeteventhandler.h "alf/alfwidgeteventhandler.h"
 *  A generic event handler for alf widgets.
 *  One AlfWidgetEventHandler instance is meant to handle one or more event
 *  inputs, which result in the same event output. When any of the event inputs
 *  accept the given event, all the event outputs and event cancel outputs are
 *  executed. However, animation event outputs and cancel outputs are executed
 *  only in response to custom events, not pointer or key events.
 *  If an animation command is required in response to a key or pointer event,
 *  the event handler handling those should generate a custom event and then
 *  the event handler handling that should generate an animation command.
 *
 *  @code
 *   ?good_class_usage_example(s)
 *  @endcode
 *
 *  @lib alfwidgetmodel.lib
 *  @since S60 ?S60_version
 *  @status Draft
 */
OSN_NONSHARABLE_CLASS( AlfWidgetEventHandler ) :  public IAlfWidgetEventHandler
    {
public:

    /**
     * Constructor.
     * @param IAlfWidgetEventHandler::AlfEventHandlerType type whe
     * @param aEventHandlerType            Event handler type. 
     *                                     It can be presentaion related or logical.
     * @param aEventHandlerExecutionPhase  Event handler exceution phase. 
     *                                     It can be tunneling phase/bubbling phase/both.   
     */
    OSN_IMPORT AlfWidgetEventHandler( 
               IAlfWidgetEventHandler::AlfEventHandlerType 
                       aEventHandlerType = ELogicalEventHandler,
               IAlfWidgetEventHandler::AlfEventHandlerExecutionPhase 
                       aEventHandlerExecutionPhase = ETunnellingPhaseEventHandler );

    /**
    * Destructor.
    */
    virtual ~AlfWidgetEventHandler();

    /**
     * Adds a key event input to the event handler.
     *
     * @exception std::bad_alloc     
     * @param aEventID The key code
     * @param aEventTypeFlags Event type flags.
     * @param aKeyEventModifiers Bitmask of the possible modifiers of the key event.
     * This event handler is triggered only if the received key event contains these modifiers.
     * See enumeration KeyEventModifiers for the possible bit values.
     */
    OSN_IMPORT void addKeyInput(
        int aEventID,
        EventTypeFlags aEventTypeFlags = keyDown,
        uint aKeyEventModifiers = 0 );

    /**
     * Adds a custom event input to the event handler.
     *
	 * @exception std::bad_alloc     
     * @param aEventID The event ID.
     * @param aUseEventData Use the custom event data to determine the accepted events.
     * @param aEventData The accepted custom event data value. Only used if
     *                   aUseEventData is true.
     */
    OSN_IMPORT void addCustomInput( int aEventID, bool aUseEventData = EFalse,
                                    int aEventData = 0 );

    /**
     * Adds a pointer event input in the event handler.
     *
	 * @exception std::bad_alloc     
     * @param aElementName The name of the element.
     * @param aVisualName The name of the visual.
     * @param aEvent 
     */
    OSN_IMPORT void addPointerInput(
        const char* aElementName, const char* aVisualName, 
        WidgetPointerEvent aEvent = pointerEventUp );

    /**
     * Adds an event output in the event handler.
     *
	 * @exception std::bad_alloc     
     * @param aEventType The event type. The only accepted types are
     * TAlfEvent::ETypeCustom and TAlfEvent::ETypeKey.
     * @param aEventID The event ID.
     * @param aTargetWidget The name of the target widget.
     * In the case of custom events, it can be an empty string, which means
     * that the event is to be broadcast to all the event handlers.
     * Key event outputs must always define a target widget.
     * @param aDelay Delay in microseconds before sending the command.
     * The default value of zero means that the command is scheduled immediately.
     * Currently, key event outputs are always executed immediately.
     */
    OSN_IMPORT void addOutput(
        int aEventType,
        int aEventID,
        const char* aTargetWidget,
        unsigned int aDelay = 0 );

    /**
     * Adds an animation output in the event handler.
     *
	 * @exception std::bad_alloc     
     * @param aTargetVisual Reference to the animated visual.
     * @param aReferenceVisual Reference to the visual with the
     * reference value, or NULL if no reference value is used.
     *
     * @param aAttributeSetter The attribute setter for animating
     * the attribute using Hitchcock commands. Ownership is transferred,
     * if the method does not throw an exception.
     *
     * @param aAttributeContainer The attribute container holding the attribute
     * value(s) used by the attribute setter. Ownership is transferred, if the method does not
     * throw an exception. The container can hold new values for multiple different
     * attributes, in which case all of them are applied when the animation output
     * is triggered.
     *
     * @param aAnimationTime The duration of the animation. Note that this parameter
     * is used only if the transition time parameter is not defined in the
     * corresponding attribute in the attribute container.
     *
     * @param aDelay The delay in microseconds before starting the animation.
     * Note that this parameter is used only if the delay parameter is not defined in the
     * corresponding attribute in the attribute container.
     */
    OSN_IMPORT void addAnimationOutput(
        AlfReferenceToVisual* aTargetVisual,
        AlfReferenceToVisual* aReferenceVisual,
        IAlfAttributeSetter* aAttributeSetter,
        AlfAttributeContainer* aAttributeContainer,
        unsigned int aAnimationTime,
        unsigned int aDelay );

    /**
     * Adds an event output for canceling timed custom events
     * in the event handler.
     *
	 * @exception std::bad_alloc     
     * @param aTargetWidget The name of the target widget.
     * @param aEventID The event ID of the event to be canceled.
     * @param aCancelAll Whether to cancel all the custom commands that are
     * scheduled to be sent to the widget or not. If this is ETrue,
     * the aEventID parameter is ignored.
     */
    OSN_IMPORT void addCancelOutput(
        const char* aTargetWidget, int aEventID, bool aCancelAll );

    /**
     * Adds an event output for canceling timed animation events
     * in the event handler.
     *
	 * @exception std::bad_alloc     
     * @param aElementName Element name.
     * @param aVisualName Visual name.
     * @param aOperation Identifies the operation of the command to be canceled.
     * @param aCancelAll Whether to cancel all the animation commands that are
     * scheduled to be sent to the widget or not. If this is ETrue,
     * the aOperation parameter is ignored.
     */
    OSN_IMPORT void addAnimationCancelOutput(
        const char* aElementName,
        const char* aVisualName,
        TAlfOp aOperation,
        bool aCancelAll );

    /**
     * Adds an event output for changing the value of a certain attribute
     * of the widget.
     *
	 * @exception std::bad_alloc     
     * @param aAttribute The attribute
     * object is transferred and this function takes care of deleting it
     * even if the function leaves. Thus, it should not be in the cleanup stack
     * when calling this function, to avoid potential double deletion.
     */
    OSN_IMPORT void addWidgetAttributeOutput( AlfAttribute& aAttribute,
        const char* aWidgetId = 0 );

    /**
     * Adds an event output for changing the value of a certain attribute
     * of the widget.
     *
	 * @exception std::bad_alloc     
     * @param aName Name of the attribute
     * @param aValue Value of the attribute
     * object is transferred and this function takes care of deleting it
     * even if the function leaves. Thus, it should not be in the cleanup stack
     * when calling this function, to avoid potential double deletion.
     */
    OSN_IMPORT void addWidgetAttributeOutput( UString& aName,
        AlfAttributeValueType* aValue, const char* aWidgetId = 0 );
            
    /**
     * Adds an event output for changing the value of a certain attribute
     * of the widget.
     *
	 * @exception std::bad_alloc     
     * @param aName Name of the attribute
     * @param aValue Value of the attribute
     * object is transferred and this function takes care of deleting it
     * even if the function leaves. Thus, it should not be in the cleanup stack
     * when calling this function, to avoid potential double deletion.
     */
    OSN_IMPORT void addWidgetAttributeInput( UString& aName,
        AlfAttributeValueType* aValue, const char* aElementId = 0 );

    /**
     * Adds an event output for launching tactile feedback (instant feedback).
     * Note that the implementation of this function is flagged
     * with RD_TACTILE_FEEDBACK, i.e. if RD_TACTILE_FEEDBACK is not defined
     * the method does nothing --> no output added.
     *
	 * @exception std::bad_alloc     
     * @param aFeedbackType The type of the tactile feedback that should 
     * be used in the output.
     */
    OSN_IMPORT void addTactileOutput( 
        WidgetTactileFeedbackType aFeedbackType );

    /**
     * Adds an event output for event passing.
     *
     * @param aSourceWidget
     * @param aTargetWidget 
     * be used in the output.
     */
    OSN_IMPORT void addEventPasserOutput( 
    	const char* aSourceWidget , const char* aTargetWidget );	
    	
    /**
     * Adds an event output for event passing. 
     *
     * @param aSourceWidget defines source widget
     * @param aTargetWidget defines target widget. ie. Event is offered for the target widget
     * @param aNewEventId Event is mapped to this type of event.
     * @param aNewEventCustomData New event custom data
     * be used in the output.
     */
    OSN_IMPORT void addEventPasserOutput( 
      const char* aSourceWidget , const char* aTargetWidget, int aNewEventId, int aNewEventCustomData );
    /**
     * Adds an event input for event passing. 
     * @param aEventId integer contains the specific event id. 
     * @param aCustomData Event specific integer data
     */		
    OSN_IMPORT void addEventPasserInput (
	    int aEventId, int aCustomData );
    /**
     * Adds a subscriber to be notified when event handler's input is received.
     *
     * @param aSubscriber Object containing knowledge of the subscribing class and 
     * its callback method.
     */
    OSN_IMPORT void addWidgetEventOutputSignalSubscriber( IAlfEventOutputSignalSubscriber& aSubscriber );

    /**
     * Adds an event output for changing the state of a widget
     *
     * @exception Alf::AlfWidgetException value osncore::EInvalidWidget 
     *     if aTargetWidget is NULL or empty.
     * @exception Alf::AlfWidgetException value osncore::EInvalidArgument 
     *     if aDisableStates and aEnableStates have a common bit set. 
     *
	 * @exception std::bad_alloc     
     * @param aName Id of the target widget instance
     * @param aEnableStates mask of states to be set to true
     * @param aDisableStates mask of states to be cleared
     */
    OSN_IMPORT void addWidgetStateOutput( const UString& aTargetWidget,
        uint aEnableStates, uint aDisableStates );     
        
    /**
     * Adds application event output for event passing.
     *
     * @exception std::bad_alloc
     * @param aCmd command
     * @param aAppEvent subscriber
     */
    OSN_IMPORT void addAppEventOutput(const UString& aCmd , IAlfAppEventListener& aAppEvent ); 

// from base class IAlfWidgetEventHandler

    /**
     * @see IAlfWidgetEventHandler.
     */
    virtual AlfEventStatus offerEvent( CAlfWidgetControl& aControl,const TAlfEvent& aEvent);


    /**
     * @see IAlfWidgetEventHandler.
     */
    bool accept( CAlfWidgetControl& aControl, const TAlfEvent& aEvent ) const;

    /**
     * @see IAlfWidgetEventHandler.
     */
    void setActiveStates( unsigned int aStates );

    /**
     * @see IAlfWidgetEventHandler.
     */
    void setCapturing( bool aCapturing );

    /**
     * @see IAlfWidgetEventHandler.
     */
    bool capturing() const;
    
    /**
     * @see IAlfWidgetEventHandler.
     * Calls to this API will overwrite previous set values.
     */
    void setEventHandlerData( const AlfWidgetEventHandlerInitData& aData );
    
    /**
     * @see IAlfWidgetEventHandler.
     */
    AlfWidgetEventHandlerInitData* eventHandlerData();

    /**
     * Returns the type of EventHandler. 
     * @see IAlfWidgetEventHandler::AlfEventHandlerType
     *
     * @return The type of event handler.
     */
    IAlfWidgetEventHandler::AlfEventHandlerType eventHandlerType();

    /**
     * Returns information about the phase in the event handling cycle in
     * which the event hadler will be executed.
     * @see IAlfWidgetEventHandler::AlfEventHandlerExecutionPhase
     *
     * @return Event handler execution phase.
     */
    IAlfWidgetEventHandler::AlfEventHandlerExecutionPhase eventExecutionPhase();

// from base class IAlfInterfaceBase

    /**
     * @see IAlfInterfaceBase.
     */
    IAlfInterfaceBase* makeInterface( const IfId& aType );


private: // data

    auto_ptr<AlfWidgetEventHandlerImpl> mImpl;
    };

    } // namespace Alf

#endif // C_ALFWIDGETEVENTHANDLER_H

