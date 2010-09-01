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
* Description:   Widget control event handler interface.
*
*/


#ifndef I_ALFWIDGETEVENTHANDLER_H
#define I_ALFWIDGETEVENTHANDLER_H

#include <alf/ialfinterfacebase.h>
#include <alf/alftypes.h>


class TAlfEvent;

namespace duiuimodel
    {
class DuiNode;
    }
using namespace duiuimodel;

namespace Alf
    {
//FORWARD DECLARATIONS
class CAlfWidgetControl;
class AlfCustomInitDataBase; 
struct AlfWidgetEventHandlerInitData; 


namespace alfwidgeteventhandler
    {
static const IfId ident =
    {
    0, "alfwidgeteventhandler"
    };
    }
    
/**
 *  The interface for event handlers used by
 *  widget controls. The event handlers are
 *  used for handling any Alfred events, including
 *  key events, pointer events and custom events.
 *  Interfaces provided by the event handlers can
 *  be accessed using IAlfInterfaceBase::MakeInterface().
 *
 *  @lib alfwidgetmodel.lib
 *  @since S60 ?S60_version
 *  @status Draft
 */
class IAlfWidgetEventHandler : public IAlfInterfaceBase
    {
public:
    enum AlfEventHandlerType
        {
        /**
         * Presentation related event handlers. These will be deleted when the 
         * presentation is destroyed.
         * @see: IAlfWigetControl::destroyPresentation
         * @see: IAlfWiget::setPresentation
         */ 
        EPresentationEventHandler,
        /**
         * Logical event handlers. These event handlers are not presentation related and
         * will not be destroyed during the destruction of the presentation.
         */
        ELogicalEventHandler,   
        };
    
    /**
     * Phase in which event handlers are executed.
     */
    enum AlfEventHandlerExecutionPhase
        {
        /**
         * Tunneling event handlers are executed only in the tunnelling phase of the 
         * event handling cycle. Tunnelling event handlers are executed before
         * IAlfWidgetControl::HandleEvent is executed.
         * @see IAlfWidgetControl::HandleEvent
         */ 
        ETunnellingPhaseEventHandler,
    
        /**
         * Bubbling event handlers are executed only in the bubbling phase of the 
         * event handling cycle. Bubbling event handlers are executed after
         * IAlfWidgetControl::HandleEvent is executed.
         * @see IAlfWidgetControl::HandleEvent
         */ 
        EBubblingPhaseEventHandler,     
    
        /**
         * These event handlers are executed both in the tunneling and bubbling phase 
         * of the event handling cycle i.e both before and after IAlfWidgetControl::HandleEvent
         * is executed.
         * @see IAlfWidgetControl::HandleEvent
         */ 
        ETunnellingAndBubblingPhaseEventHandler,        
        };
        
    static inline const IfId& type()
        {
        return alfwidgeteventhandler::ident;
        }

    virtual inline ~IAlfWidgetEventHandler() {}

    /**
     * Used to identify event handlers that are capable of handling
     * a specified key or custom event. Pointer event handlers cannot be
     * identified directly, since they are dependent on the presentation.
     * Therefore, pointer event handlers should be associated with
     * a custom event ID that can be used to simulate the actual pointer event.
     *
     * @since S60 ?S60_version
     * @param aEvent The event.
     * @return <code>ETrue</code> if the event can be processed. Otherwise <code>EFalse</code>.
     */
    virtual bool accept( CAlfWidgetControl& aControl, const TAlfEvent& aEvent ) const = 0;

    /**
     * Called when an input event is being offered to this event handler.
     * The return value is used to indicate if the event was processed.
     * Processed events may not be offered to other event handlers.
     *
     * @since S60 ?S60_version
     * @param aControl The parent control of this event handler.
     * @param aEvent The event.
     * @return Result of event processing. See AlfEventStatus.
     */
    virtual AlfEventStatus offerEvent( CAlfWidgetControl& aControl,const TAlfEvent& aEvent) = 0;

    /**
     * Defines the widget states, where the event handler is active.
     * By default, if this method is not called, the event handler is expected
     * to be active in all states.
     *
     * @param aStates A bitmask defining the widget states, where the event
     * handler is active. The low 16 bits are reserved for the states
     * defined by the framework in . The high 16 bits are
     * available for states defined by the client code.
     */
    virtual void setActiveStates( unsigned int aStates ) = 0;
    
    /**
     * Sets AlfWidgetEventHandlerInitData to event handler.
     *
     * @param aData A data structure which contains for example id of the event
     * handler.
     */
    virtual void setEventHandlerData( const AlfWidgetEventHandlerInitData& aData ) = 0;
    
    /**
     * Returns AlfWidgetEventHandlerInitData.
     *
     * @return  A pointer to AlfWidgetEventHandlerInitData structure which contains
     * for example id of the event handler.
     */
    virtual AlfWidgetEventHandlerInitData* eventHandlerData() = 0;

    /**
     * Returns the type of EventHandler. 
     * @see IAlfWidgetEventHandler::AlfEventHandlerType
     *
     * @return The type of event handler.
     */
    virtual AlfEventHandlerType eventHandlerType() = 0;

    /**
     * Returns information about the phase in the event handling cycle in
     * which the event hadler will be executed.
     * @see IAlfWidgetEventHandler::AlfEventHandlerExecutionPhase
     *
     * @return Event handler execution phase.
     */
    virtual AlfEventHandlerExecutionPhase eventExecutionPhase() = 0;
    };

/**
 * Placeholder for information required to instantiate an event handler
 * via the widget factory mechanism.
 * A pointer to this structure is casted to a void pointer and sent to the
 * factory plugin.
 * @lib alfwidgetmodel.lib
 * @since S60 ?S60_version
 */
struct AlfWidgetEventHandlerInitData
    {
    /**
     * Event handler instance ID.This uniquely identifies every event handler instance.
     */
    char* mWidgetEventHandlerId;
    
    /**
     * Pointer to node in declaration containing information for the widget.
     */
    DuiNode* mNode;

    /**
     * Pointer to custom data passed via factory mechanism
     * Not Owned.
     */
    AlfCustomInitDataBase* mCustomData;  
    };

    } // namespace Alf

#endif // I_ALFWIDGETEVENTHANDLER_H

// End of File
