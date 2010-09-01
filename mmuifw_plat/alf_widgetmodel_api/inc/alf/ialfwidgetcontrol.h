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
* Description:  The base class for all widgets.
*
*/


#ifndef I_ALFWIDGETCONTROL_H
#define I_ALFWIDGETCONTROL_H

#include <osn/osntypes.h>
#include <alf/ialfinterfacebase.h>
#include <alf/alftypes.h>

//FORWARD DECLARATION
class TAlfEvent;
class CAlfDisplay;

namespace osncore
    {
    class UString;
    }
using namespace osncore;

namespace Alf
    {
//FORWARD DECLARATIONS    
class IAlfWidgetEventHandler;
class IAlfElement;
class AlfWidget;
class IAlfVariantType;
class AlfCustomInitDataBase;
 

namespace alfwidgetcontrol
    {
static const IfId ident=
    {
    0,"alfwidgetcontrol"
    };
    }

/**
 *  The interface for widget controls.
 *  Widget controls are derived from Alfred controls. This is
 *  the primary interface to access the widget control functionality.
 *  Alfred control interface can be queried, but direct access should not
 *  be needed. Widget controls are responsible for the widget logic, i.e.,
 *  storing and controlling the state of the widget. This usually includes
 *  input event handling, updating the presentation, and communication with
 *  the model, the application, and other controls. Event handlers can be
 *  used to implement parts of the widget logic.
 *
 *  @lib alfwidgetmodel.lib
 *  @since S60 ?S60_version
 *  @status Draft
 */
class IAlfWidgetControl : public IAlfInterfaceBase
    {
public:

    /**
     * Defines the bitflags used with the state interfaces.
     */

    // Visible is set if the widget control is currently intended to be visible
    static const uint Visible = 0x0001;

    // Enabled is set if the widget control is to be responsive to user input
    // Note: Presentation may render this control in some way to indicate that
    // it is disabled
    static const uint Enabled = 0x0002;
    
    // Focused is set if the control is to be currently attracting key events 
    static const uint Focused = 0x0004;
    
    // Focusable is set iff the widget control is to allow focus to be put
    // on it
    static const uint Focusable = 0x0008;

public:
    static inline const IfId& type()
        {
        return alfwidgetcontrol::ident;
        }
    /**
     * Virtual destructor.
     *
     * @since S60 ?S60_version
     */
    virtual ~IAlfWidgetControl() {}

    /**
     * Get the state of the control.
     * The state is a combination of binary state flags.
     *
     * The individual aspects of the state can be queried by testing 
     * for the bits in the return value.
     *
     * @since S60 ?S60_version
     * @return The bit-pattern encoding the state
     */
    virtual uint state() const = 0;

    /**
     * Sets the widget control's state.
     *
     * The entire state of the widget is set to the passed-in value. 
     * 
     * The new state must conform to the following invariants:
     *
     * If IAlfWidgetControl::Focus is set, then IAlfWidgetControl::Focusable must also be set
     * If IAlfWidgetControl::Focusable is set, then IAlfWidgetControl::Enabled must also be set
     *
     * If these invariants are violated, then this method will throw AlfException::EInvalidArgument
     *
     * @since S60 ?S60_version
     * @param aState The new state of the control.
     */
    virtual void setState( uint aState ) = 0;

    /**
     * Enable given states.
     *
     * This method modifies a state by enabling bits. No bits are disabled.
     * This is equal to <code>SetState( State() | aState )</code>.
     *
     * The resulting state is tested for the invariants documented with setState() 
     * and throws the same exceptions.
     *
     * @since S60 ?S60_version
     * @param aState The states to be enabled.
     */
    virtual void enableState( uint aState ) = 0;

    /**
     * Disable given states.
     *
     * This method modifies a state by clearing bits. No bits are set.
     * This is equal to <code>SetState( State() & ( 0xFFFFFFFF ^ aState ) )</code>.
     *
     * The resulting state is tested for the invariants documented with setState() 
     * and throws the same exceptions.
     *
     * @since S60 ?S60_version
     * @param aState The states to be disabled.
     */
    virtual void disableState( uint aState ) = 0;

    /**
     * Checks the current state of the widget control for a single aspect.
     * This is equal to <code>enabled = State() & aState</code>
     * 
     * The parameter passed in should usually be a single bit state aspect 
     * e.g. <code>IAlfWidgetControl::Focusable</code>
     * If a pattern with more than one bit set is passed in, the result
     * is not easily usable.
     *
     * @since S60 ?S60_version
     * @param aState a state bit pattern to test
     * @return true if and only one of the passed-in bits is set in the state
     */
    virtual bool checkState( uint aState ) const = 0;

    /**
     * Get the number of event handlers in the control.
     *
     * @since S60 ?S60_version
     * @return The number of event handlers.
     */
    virtual int numEventHandlers() const = 0;

    /**
     * Get an event handler by index.
     *
     * @since S60 ?S60_version
     * @param aIndex The index of the event handler to get.
     *
     * @return The event handler at the given index.
     */
    virtual IAlfWidgetEventHandler& eventHandler( int aIndex ) = 0;

    /**
     * Get the index of the given event handler.
     * The index specifies the order in which
     * the event handlers are processed.
     *
     * @since S60 ?S60_version
     * @param aEventHandler The event handler.
     *
     * @return The index of the event handler, or -1 if the event handler 
     *         does not exist.
     */
    virtual int eventHandlerIndex( 
                    IAlfWidgetEventHandler& aEventHandler ) const = 0;


    /**
     * Find an event handler responsible for the given event.
     *
     * @since S60 ?S60_version
     * @param aEvent The event.
     *
     * @return The first event handler to handle the given event, or NULL 
     *         if not found.
     */
    virtual IAlfWidgetEventHandler* findEventHandler(
                                        const TAlfEvent& aEvent ) = 0;

    /**
     * Add a new event handler.
     * The control takes the ownership of the event handler.
     *
     * @since S60 ?S60_version
     * @param aEventHandler The event handler to be added.
     * @param aIndex The index of the new event handler,
     *               or -1 to append the event handler after the existing
     *               event handlers.
     */
    virtual void addEventHandler(
                     IAlfWidgetEventHandler* aEventHandler,
                     int aIndex = -1 ) = 0;

    /**
     * Remove and destroy an event handler.
     *
     * @since S60 ?S60_version
     * @param aEventHandler The event handler to be removed and destroyed.
     */
    virtual void removeAndDestroyEventHandler(
                     IAlfWidgetEventHandler& aEventHandler ) = 0;

    /**
     * Remove and destroy an event handler from the control bases on its name.
     *
     * @since S60 ?S60_version
     * @param aHandlerId The name/Id of the event handler to be removed
     *                   and destroyed.
     */
    virtual void removeAndDestroyEventHandler( const UString& aHandlerId ) = 0;
   
  
    /**
     * Remove and destroy an event handler.
     *
     * @since S60 ?S60_version
     * @param aEventHandler The event handler to be removed and destroyed.
     */
    virtual void removeEventHandler(
                     IAlfWidgetEventHandler& aEventHandler ) = 0;
    
    
    /**
     * Remove and destroy all event handlers associated with presentation.
     *
     * @since S60 ?S60_version
     */
    virtual void removeAndDestroyPresentationEventHandlers() = 0;
    
    /**
     * Removes all event handlers associated with presentation from 
     * this control.
     *
     * @since S60 ?S60_version
     */ 
    virtual void removePresentationEventHandlers() = 0;
    
    /**
     * Get the number of elements in the control.
     *
     * @since S60 ?S60_version
     * @return The number of elements.
     */
    virtual int numElements() const = 0;

    /**
     * Get an element by index.
     *
     * @since S60 ?S60_version
     * @param aIndex The index of the element to get.
     *
     * @return The element at the given index.
     */
    virtual IAlfElement& element( int aIndex ) = 0;

    /**
     * Find an element by name.
     *
     * @since S60 ?S60_version
     * @param aName The name of the element to find.
     *
     * @return The found element, or NULL if not found.
     */
    virtual IAlfElement* findElement( const char* aName ) = 0;

    /**
     * Add a new element.
     * The control takes the ownership of the element.
     *
     * @since S60 ?S60_version
     * @param aElement The element to be added.
     */
    virtual void addElement( IAlfElement* aElement ) = 0;

    /**
     * Remove and destroy an element. All visuals created by the
     * element are destroyed.
     *
     * @since S60 ?S60_version
     * @param aElement The element to be removed and destroyed.
     */
    virtual void removeAndDestroyElement( const IAlfElement& aElement ) = 0;
    
    /**
     * Remove an element.
     *
     * @since S60 ?S60_version
     * @param aElement The element to be removed.
     */
    virtual void removeElement( const IAlfElement& aElement ) = 0; 

    /**
     * Get an id for mapping data to a visual tree created by an element.
     * The data id is required to be unique withing the scope of the element.
     * The parent data id and data id ranges specified by SetDataIdRange can
     * be used to calculate data ids in hierarchical element structures.
     * In the default implementation, the data id is calculated as follows:
     * \f[
     * data id = aParentDataId * DataIdRange( aElement ) + aIndex
     * \f]
     *
     * @since S60 ?S60_version
     * @param aElement      The element to associate the data id with.
     * @param aIndex        The index of the data field.
     * @param aParentDataId The parent data id in the data hierarchy.
     * @return A unique data id for mapping data to a visual tree.
     */
    virtual uint elementDataId(
                     const IAlfElement& aElement,
                     uint aIndex,
                     uint aParentDataId ) = 0;

    /**
     * Get the parent data id from the given data id.
     * In the default implementation, the parent data id is calculated 
     * as follows:
     * \f[
     * parent data id = aDataId / DataIdRange( aElement )
     * \f]
     * The implementation should match the functionality of ElementDataId().
     *
     * @since S60 ?S60_version
     * @param aElement  The element associated with the given data id.
     * @param aDataId   The data id.
     * @return The parent data id.
     */
    virtual uint parentElementDataId(
                     const IAlfElement& aElement,
                     uint aDataId ) = 0;

    /**
     * Get the index of the given child data id in the parent data container.
     * In the default implementation, the parent data id is calculated
     * as follows:
     * \f[
     * index = aDataId % DataIdRange( aElement )
     * \f]
     * The implementation should match the functionality of ElementDataId().
     *
     * @since S60 ?S60_version
     * @param aElement  The element associated with the given data id.
     * @param aDataId   The data id.
     * @return The parent data id.
     */
    virtual uint dataIdToIndex(
                     const IAlfElement& aElement,
                     uint aDataId ) = 0;
                     
    /**
     * Get data for a visual tree created by the given element.
     *
     * @since S60 ?S60_version
     * @param aElement  The element containing the visual tree.
     * @param aDataId   The data id of the visual tree.
     * @return Data for the visual tree.
     */
    virtual IAlfVariantType* elementData(
                                 const IAlfElement& aElement,
                                 uint aDataId ) = 0;

    /**
     * Set the range of data ids for an element.
     * The range is used for data mapping in ElementDataId() and ElementData().
     * The default range for all elements is 0xFFFFFFFF. The ranges need to be
     * adjusted for child elements in hierarchical element structures.
     * Otherwise,the data ids will overflow and not remain unique.
     * @see ElementDataId().
     *
     * @since S60 ?S60_version
     * @param aElement  The element.
     * @param aRange    The range of data ids.
     */
    virtual void setDataIdRange(
                     const IAlfElement& aElement,
                     uint aRange ) = 0;

    /**
     * Get the range of data ids for the given element.
     * The range is used for data mapping in ElementDataId() and ElementData().
     * @see SetDataIdRange().
     *
     * @since S60 ?S60_version
     * @param aElement  The element.
     * @return The range of data ids for the element.
     */
    virtual uint dataIdRange( const IAlfElement& aElement ) = 0;

    /**
     * Create or update the presentation for the widget.
     *
     * @since S60 ?S60_version
     */
    virtual void updatePresentation() = 0;

    /**
     * Remove visuals from all elements. The visuals are destroyed
     * after the given delay. This allows executing an animation
     * sequence before the actual destruction of visuals.
     *
     * @since S60 ?S60_version
     * @param aTimeMilliseconds The remaining time to destruction of 
     *                          all visuals.
     */
    virtual void destroyPresentation( int aTimeMilliseconds ) = 0;
    
    /**
     * Remove visuals from all elements and destroy all elements.
     * The visuals and elements are destroyed after the given delay.
     * This allows executing an animation sequence before the actual
     * destruction of visuals.
     *
     * @since S60 ?S60_version
     * @param aTimeMilliseconds The remaining time to destruction of 
     *                          all visuals.
     */    
    virtual void destroyVisualsAndElements( int aTimeMilliseconds = 0 ) = 0;

    /**
     * Get the owner widget of this control.
     *
     * @since S60 ?S60_version
     * @return The owner widget, or NULL if this control does not belong to
     *         any widget.
     */
    virtual AlfWidget* widget() = 0;
    
    /**
     * Handles events sent to widget control.
     *
     * @since S60 ?S60_version
     * @param aEvent The event to be handled.
     * @return Result of event processing. See AlfEventStatus.
     */
    virtual AlfEventStatus handleEvent( const TAlfEvent& aEvent ) = 0;
    
    };

/**
 *  Placeholder for information required to instantiate a widgetcontrol
 *  via the widget factory mechanism.
 *  A pointer to this structure is casted to a void pointer and sent to the
 *  factory plugin.
 *  @lib alfwidgetmodel.lib
 *  @since S60 ?S60_version
 */
struct AlfWidgetControlInitData
    {

    /**
     * Display pointer
     */
    CAlfDisplay* mDisplay;
    
    /**
     * Pointer to custom data passed via factory mechanism
     * Not Owned.
     */
    AlfCustomInitDataBase* mCustomData;  
    };

    } // namespace Alf

#endif // I_ALFWIDGETCONTROL_H

// End of File
