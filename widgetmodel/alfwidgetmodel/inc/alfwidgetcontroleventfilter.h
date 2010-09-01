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
* Description:  Event Filter Header.
*
*/

#ifndef __ALFWIDGETCONTROLEVENTFILTER_H__
#define __ALFWIDGETCONTROLEVENTFILTER_H__
#include <libc/string.h>


#include <alf/alfevent.h>
#include <alf/alfwidgetevents.h>
#include <osn/ustring.h>
#include <osn/osndefines.h>
#include <alf/alfeventhandler.h>

#include <alf/alfscrollevents.h>
#include <alf/ialfwidgeteventhandler.h>

using namespace osncore;
using namespace Alf;
namespace Alf
{
/**
* AlfWidgetControlEventFilter delivers the Missed PointerUp event as a custom  
* event .If AfWidgetControl gets a stray pointerUp event, the 
* AlfWidgetControlEventFilter will consume the Stray pointerup event and send a 
* EEventMissedPointerUp custom event.
*
*/
class AlfWidgetControlEventFilter: public IAlfWidgetEventHandler
    {
	public:
	
	/**
	 * Constructor
	 *
	 */
	AlfWidgetControlEventFilter();
	
	/**
	 * Destructor
	 *
	 */
	~AlfWidgetControlEventFilter();
	
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
	bool accept ( CAlfWidgetControl& aControl, const TAlfEvent& aEvent ) const;

	/**
     * @see IAlfWidgetEventHandler
	 */
	virtual AlfEventStatus offerEvent ( CAlfWidgetControl& aControl, const TAlfEvent& aEvent );

    /**
     * Returns the type of EventHandler. 
     * @see IAlfWidgetEventHandler::AlfEventHandlerType
     *
     * @return The type of event handler.
     */
    AlfEventHandlerType eventHandlerType();

    /**
     * Returns information about the phase in the event handling cycle in
     * which the event hadler will be executed.
     * @see IAlfWidgetEventHandler::AlfEventHandlerExecutionPhase
     *
     * @return Event handler execution phase.
     */

    AlfEventHandlerExecutionPhase eventExecutionPhase();
	/**
	 * Defines the widget states, where the event handler is active.
	 * By default, if this method is not called, the event handler is expected
	 * to be active in all states.
	 *
	 * @param aStates A bitmask defining the widget states, where the event
	 * handler is active. The low 16 bits are reserved for the states
	 * defined by the framework in <: add reference>. The high 16 bits are
	 * available for states defined by the client code.
	 */
   /**
     * Sets AlfWidgetEventHandlerInitData to event handler.
     *
     * @param aData A data structure which contains for example id of the event
     * handler.
     */
    virtual void setEventHandlerData( const AlfWidgetEventHandlerInitData& /*aData*/ )
    {
    	
    }
    
    /**
     * Returns AlfWidgetEventHandlerInitData.
     *
     * @return  A pointer to AlfWidgetEventHandlerInitData structure which contains
     * for example id of the event handler.
     */
    virtual AlfWidgetEventHandlerInitData* eventHandlerData()
    {
    	return NULL;
    }
    

	void setActiveStates ( unsigned int aStates );
	
	IAlfInterfaceBase* makeInterface ( const IfId& aType );
	// Stores Button Down State
    bool mButtonDown ;

	};	
}

    
#endif // __ALFWIDGETCONTROLEVENTFILTER_H__ 

