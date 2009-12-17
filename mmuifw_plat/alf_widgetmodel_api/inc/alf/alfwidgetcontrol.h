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
* Description:  The base class for all widget controls.
*
*/


#ifndef ALFWIDGETCONTROL_H
#define ALFWIDGETCONTROL_H

#include <osn/osndefines.h>
#include <osn/osnnew.h> // for OSN operator new parameter types
#include <alf/ialfwidgetcontrol.h>
#include <alf/alfcontrol.h>
#include <alf/ialfmodelchangeobserver.h>
#include <memory>
namespace osncore
    {
template <class T> class AlfPtrVector;
    }

namespace std
    {
template <class T> class auto_ptr;
    }

using namespace osncore;
using namespace std;

namespace Alf
    {
class IAlfContainerWidget;
class IAlfWidgetEventHandler;
class IAlfElement;
class AlfWidget;
class IAlfMap;
class IAlfVariantType;
class AlfWidgetControlImpl;
class IAlfHostAPI;
class IfId;
class ElementData;

/** @class CAlfWidgetControl alfwidgetcontrol.h "alf/alfwidgetcontrol.h"
 *  The base class for all widget controls.
 *  Widget controls are derived from Alfred controls, and owned by
 *  the Alfred environment or control groups. Controls are responsible
 *  for the widget logic, i.e., storing and controlling the state of
 *  the widget. This usually includes input event handling, updating
 *  the presentation, and communication with the model, the application,
 *  and other controls. Event handlers can be used to implement parts
 *  of the widget logic.
 *
 *  @interfaces IAlfWidgetControl, IAlfHostAPI, IAlfAttributeOwner
 *  @lib alfwidgetmodel.lib
 *  @since S60 ?S60_version
 *  @status Draft
 */
class CAlfWidgetControl :
            public CAlfControl,
            public IAlfModelChangeObserver,
            public IAlfWidgetControl
    {
public:

    /**
     * The constructor.
     *
     * @since S60 ?S60_version
     * @param aEnv      The Alfred environment.
     */
    OSN_IMPORT CAlfWidgetControl(CAlfEnv& aEnv);


    /**
     * operator new method to ensure correct OS-neutral handling of failure to
     * allocate memory.
     *
     * This operator will throw std::bad_alloc if memory cannot be allocated.
     * If memory is successfully allocated, it will fill all bytes of allocated
     * memory with zero.
     * 
     * Note that the parameter list below is not relevant to the actual usage 
     * which is typically:
     *
     *   auto_ptr<CAlfWidgetControl> control(new(EMM) CAlfWidgetControl(aEnv));
     * or
     *   CAlfWidgetControl* mControl = new(EMM) CAlfWidgetControl(aEnv);
     * 
     * @exception Throws std::bad_alloc if there is a failure to allocate 
     *                memory for the object under construction.
     *
     * @param aSize The number of bytes required for an instance of the class.
     *            This parameter is supplied automatically.
     * @param aEnumValue An enumeration value of newarg. Always use EMM.
     * @return a pointer to the zero-ed memory allocated on the heap. 
     *             Ownership transferred to caller.
     */
    OSN_IMPORT void* operator new(size_t aSize, newarg aEnumValue) 
                                  throw (std::bad_alloc);

    /**
     * Destructor.
     */
    OSN_IMPORT virtual ~CAlfWidgetControl();

    /**
     * Set the owner widget of this control.
     * Called by AlfWidget::setControlL().
     *
     * @since S60 ?S60_version
     * @param aWidget The owner widget of this control.
     */
    void setOwnerWidget( AlfWidget* aWidget );
    
    /**
     * Returns top level layout. Returns NULL if this control does not
     * 
     * @return Pointer to top level layout, or NULL if not available.
     */
    OSN_IMPORT CAlfLayout* ContainerLayout(
                   const CAlfControl* /*aConnected*/) const;
    
    /**
     * state.
     */
    OSN_IMPORT uint state() const;
    
    /**
     * sets the state.
     */
    OSN_IMPORT void setState( uint aState );
    
    /**
     * enables State.
     */
    OSN_IMPORT void enableState( uint aState );
    
    /**
     * disables State.
     */
    OSN_IMPORT void disableState( uint aState );
    
    /**
     * checks  State.
     */
    OSN_IMPORT bool checkState( uint aState ) const;
    
    /**
     * returns no of event handlers.
     */
    OSN_IMPORT int numEventHandlers() const;
    
    /**
     * returns the event handlers at the index.
     */
    OSN_IMPORT IAlfWidgetEventHandler& eventHandler( int aIndex );
    
    /**
     * returns event handler index.
     */
    OSN_IMPORT int eventHandlerIndex(
                   IAlfWidgetEventHandler& aEventHandler ) const;
    
    /**
     * returns finds the event handler .
     */
    OSN_IMPORT IAlfWidgetEventHandler* findEventHandler(
                   const TAlfEvent& aEvent );
    
    /**
     * adds an event handler.
     * @exception osncore::AlfWidgetException Thrown with the error code 
     *     osncore::ECommonError when user tries to add an event handler at an
     *     index but it fails.
     */
    OSN_IMPORT void addEventHandler(
                   IAlfWidgetEventHandler* aEventHandler,
                   int aIndex = -1 );
    
    /**
     * removes an event handlers.
     */
    OSN_IMPORT void removeAndDestroyEventHandler(
                   IAlfWidgetEventHandler& aEventHandler );
    
    /**
     * returns no of event handlers.
     */
    OSN_IMPORT void removeEventHandler(
                   IAlfWidgetEventHandler& aEventHandler );
    
    /**
     * Remove and destroy an event handler from the control bases on its name.
     *
     * @since S60 ?S60_version
     * @param aHandlerId The name/Id of the event handler to be removed
     *                   and destroyed.
     */
    OSN_IMPORT void removeAndDestroyEventHandler( const UString& aHandlerId );
    
    /**
     * Remove and destroy an event handler from the control bases on its name.
     *
     * @since S60 ?S60_version
     * @param aHandlerId The name/Id of the event handler to be removed 
     *                   and destroyed.
     */
    OSN_IMPORT void removeAndDestroyEventHandler( UString& aHandlerId );
    
    /**
     * Remove and destroy all event handlers associated with presentation 
     * elements.
     * @since S60 ?S60_version
     */
    OSN_IMPORT void removeAndDestroyPresentationEventHandlers();
    
    /**
     * Removes all event handlers associated with presentation elements from 
     * this control.
     *
     * @since S60 ?S60_version
     */
    OSN_IMPORT void removePresentationEventHandlers();
    
    /**
     * Returns no of elements.
     */
    OSN_IMPORT int numElements() const;
    
    /**
     * Returns an element at the index.
     */
    OSN_IMPORT IAlfElement& element( int aIndex );
    
    /**
     * Finds an element with specified name.
     */
    OSN_IMPORT IAlfElement* findElement( const char* aName );
    
    /**
     * Adds an element in the end.
     * @exception std::bad_alloc
     * @exception osncore::AlfException Thrown with the error code 
     *     osncore::ECommonError if the given element object has been added 
     *     in the widget control already.
     *
     * @param aElement Element to be added.
     * Ownership is transferred iff no exception is thrown.
     */
    OSN_IMPORT void addElement( IAlfElement* aElement );
    
    /**
     * removes and destroys an element.
     */
    OSN_IMPORT void removeAndDestroyElement( const IAlfElement& aElement );

   /**
     * removes an element but does not destroy it.
     */
    OSN_IMPORT void removeElement( const IAlfElement& aElement ); 
       
    /**
     * Returns the data id of an element.
     */
    OSN_IMPORT virtual uint elementDataId(
                           const IAlfElement& aElement,
                           uint aIndex,
                           uint aParentDataId );
    
    /**
     * Returns the data id of the parent element of an element.
     */
    OSN_IMPORT virtual uint parentElementDataId(
                           const IAlfElement& aElement,
                           uint aDataId );
    
    /**
     * Returns index corresponding to a data id.
     */
    OSN_IMPORT virtual uint dataIdToIndex(
                           const IAlfElement& aElement,
                           uint aDataId );
    
    /**
     * Returns data of an element.
     */
    OSN_IMPORT IAlfVariantType* elementData(
                   const IAlfElement& aElement,
                   uint aDataId );
    
    /**
     * Sets the data id range of an element.
     */
    OSN_IMPORT void setDataIdRange( const IAlfElement& aElement, uint aRange );
    
    /**
     * Returns the data id range of an element.
     */
    OSN_IMPORT uint dataIdRange( const IAlfElement& aElement );
    
    /**
     * Updates presentation.
     * @exception osncore::AlfVisualException Thrown with the error code
     *    osncore::ECanNotCreateVisual when during updation of presentation, 
     *    visual trees are created and an error occurs during the creation of
     *    visuals.
     * @since S60 ?S60_version
     */
    OSN_IMPORT void updatePresentation();
    
    /**
     * Destroys presentation (visual trees under elements).
     */
    OSN_IMPORT void destroyPresentation( int aTimeMilliseconds = 0 );
    
    /**
     * Destroys all visuals and elements.
     */
    OSN_IMPORT void destroyVisualsAndElements( int aTimeMilliseconds = 0 ); 
    
    /**
     * Return the owner widget of this control .
     */
    OSN_IMPORT AlfWidget* widget();

    /*
     * Notifies the parent control that the layout should be recalculated.
     * If there is no parent, nothing is done.
     */
    OSN_IMPORT void updateParentLayout();
    
    /*
     * @return true if the control acts as a container for other widgets.
     * 
     */
    OSN_IMPORT bool isContainer() const;
    
    /*
     * @return true if the control is focusable, false otherwise.
     */
    OSN_IMPORT bool isFocusable() const;
    
    /**
     * Template getter for interfaces provided by the control classes.
     * The type of the queried interface is specified by the
     * template parameter.
     *
     * @exception std::bad_alloc 
     *
     * @since S60 ?S60_version
     * @param aControl The control to get the interface for.
     *
     * @return The queried interface, or NULL if the interface is not
     *         supported or available.
     */
    template <class T>
    static T* makeInterface( CAlfWidgetControl* aControl );

    /**
     * Handles events sent to widget control.
     * <b> Widgets can override this. </b>
     * The default implementation tunnels key events towards the 
     * focussed child widget. If custom events EEventFocusNextWidget/
     * EEventFocusPreviousWidget are received the default implementation
     * attempts to switch focus to the next/previous child widget.
     * 
     * @since S60 ?S60_version
     * @param aEvent The event to be handled.
     *
     * @return Result of event processing. See AlfEventStatus.
     */
    OSN_IMPORT virtual AlfEventStatus handleEvent( const TAlfEvent& aEvent );
    
    /**
     * Processes the events sent to this control.
     * The events are passed to the tunnelling phase event handlers,
     * handleEvent method and the bubbling phase event handlers in 
     * that order till one of them consumes the event.
     *
     * @since S60 ?S60_version
     * @param aEvent The event to be handled.
     *
     * @return Result of event processing. See AlfEventStatus.
     */
    OSN_IMPORT AlfEventStatus processEvent( const TAlfEvent& aEvent );

// from base class IAlfModelChangeObserver

    /**
     * From IAlfInterfaceBase.
     * Update the control according to the changed model.
     * Destroy existing visuals and cancel all animation commands. 
     * Create new visuals according to the data in the new model.
     *
     * @since S60 ?S60_version
     * @param aModel The new model.
     */
    OSN_IMPORT virtual void modelChanged( IAlfModel& aModel );

    /**
     * Update the presentation according to the data changes.
     *
     * @since S60 ?S60_version
     * @param aArrayOfOperation An Array containing the operations performed 
     *                          on the model.
     */
    OSN_IMPORT virtual void dataChanging(
                          const AlfPtrVector<AlfModelOperation>& aOperations );

    /**
     * Update the presentation according to the data changes.
     *
     * @exception osncore::AlfVisualException Thrown with the error code 
     *     osncore::ECanNotCreateVisual when adding the visual tree through
     *     adding the data for it in the model but the creation of visual tree
     *     is failed.
     *
     * @since S60 ?S60_version
     * @param aOperation An operation performed on the model.
     */
    OSN_IMPORT virtual void dataChanging(
                           const AlfModelOperation& aOperation );

    /**
     * The default implementation is empty.
     * Derived classes may update the state of the
     * control according to the changed data.
     *
     * @since S60 ?S60_version
     */
    OSN_IMPORT virtual void dataChanged();

// from base class IAlfInterfaceBase

    /**
     * From IAlfInterfaceBase.
     * Getter for interfaces provided by the control classes.
     * Derived classes should always call the base class method
     * from the overridden MakeInterface.
     *
     * @since S60 ?S60_version
     * @param aType A descriptor to identify the type of the queried interface.
     *
     * @return The queried interface, or NULL if the interface is not
     *         supported or available.
     */
    OSN_IMPORT virtual IAlfInterfaceBase* makeInterface( const IfId& aType );

public:

// New non-virtual methods

    /**
     * Resets control group of this control. If the control is in a
     * control group it will be removed from its currect control group
     * and appended to the given control group.
     *
     * @param Reference to the control group to which this control is appended.
     */
    void resetControlGroup(CAlfControlGroup& aControlGroup);

    /**
     * Resets control group of this control and the whole hierarchy of
     * controls that are connected to this control or its children with
     * connections.
     *
     * This control and the tree of controls below it are removed from
     * their previous control groups.
     *
     * @since S60 ?S60_version
     * @param Reference to the control group to which controls are appended.
     *        This control group replaces control groups to which the controls
     *        in this control hierarchy belong before the call is made.
     */
    void resetHierarchyControlGroup(CAlfControlGroup& aControlGroup);
    
    /**
     * Attempt to acquire focus for this control.
     */
    OSN_IMPORT void AcquireFocus();
    
    /**
     * Attempt to relinquish focus for this control.
     */ 
    OSN_IMPORT void RelinquishFocus();
    
    /**
     * returns hostApi for this control.
     * 
     * @return hostApi-instance.
     */
    IAlfHostAPI& hostAPI();

protected:

    /**
     * The protected constructor.
     *
     * @since S60 ?S60_version
     */
    OSN_IMPORT CAlfWidgetControl();


    /**
     * The constructor.
     *
     * @since S60 ?S60_version
     * @param aEnv The Alfred environment.
     */
    OSN_IMPORT void construct(CAlfEnv& aEnv);
    
    /**
     * handles focusing to next control. 
     * Handles the container inside container cases. If next focusable
     * control is not found, moves focus to upper level container, if no
     * next focusable controls found in any of the upper level
     * containers, loops back to the first control.
     *
     * @since S60 ?S60_version
     * @return true, if focus was changed.
     */    
    OSN_IMPORT bool handleFocusNext();
    
    /**
     * handles focusing to previous control.
     * Handles also the container inside container cases. If next focusable
     * control is not found, moves focus to upper lever container, if no
     * previous focusable controls found in any of the upper level 
     * containers, loops back to the last control.
     *
     * @since S60 ?S60_version
     * @return true, if focus was changed.
     */        
    OSN_IMPORT bool handleFocusPrevious();
        
    /**
     * return the first focusable child control.
     * First focusable is also the default control to get the focus.
     *
     * @since S60 ?S60_version
     * @param aHostApi hostapi-instance. Use: for iterating the connected 
     *                 controls.
     * @return control, which should be the first to get the focus. 
     */        
    OSN_IMPORT virtual CAlfWidgetControl* getFirstFocusable(
                                              IAlfContainerWidget& aContainer);

    /**
     * return the last focusable child control.
     *
     * @since S60 ?S60_version
     * @param aHostApi hostapi-instance. Use: for iterating the connected
     *                 controls.
     * @return control, which should be the last one to get the focus
     */
    OSN_IMPORT virtual CAlfWidgetControl* getLastFocusable(
                                              IAlfContainerWidget& aContainer);
        
    /**
     * return next focusable control right after aControl
     *
     * @since S60 ?S60_version
     * @param aHostApi hostapi-instance. Use: for iterating the connected
     *                 controls.
     * @param aControl reference control
     * @return control, which should be focused right after aControl.
     *      If no focusable control left, should return 0.
     */
    OSN_IMPORT virtual CAlfWidgetControl* getFocusableAfter(
                                              IAlfContainerWidget& aContainer, 
                                              CAlfWidgetControl& aControl);
    
    /**
     * return next focusable control right before aControl.
     *
     * @since S60 ?S60_version
     * @param aHostApi hostapi-instance. Use: for iterating the connected
     *                 controls.
     * @param aControl reference control.
     * @return control, which should be focused right before aControl.
     *      If no focusable control left, should return 0.
     */
    OSN_IMPORT virtual CAlfWidgetControl* getFocusableBefore(
                                             IAlfContainerWidget& aContainer,
                                             CAlfWidgetControl& aControl);

    /**
     * Called when the control's focus state changes.
     */    
    OSN_IMPORT void FocusChanged(CAlfDisplay& aDisplay, TBool aFocused);

private:

    // from base class MAlfEventHandler

    /**
     * Sends the event to processEvent method.
     * <b>Widgets are discouraged from overriding this method.</b>
     * Leaves with exception description if processEvent throws.
     * 
     * @since S60 ?S60_version
     * @param aEvent The event to be handled.
     * 
     * @return  <code>ETrue</code>, if the event was handled.
     *          Otherwise <code>EFalse</code>.
     *
     */
    OSN_IMPORT virtual TBool OfferEventL( const TAlfEvent& aEvent );
    
    /**
     * Calculate the child index inside a map.
     * In the data hierarchy, map items may by children
     * or containers of children. In order to calculate the
     * absolute child index, all previous map items need to be
     * considered. If the map is the root data, child items
     * with different element names are not considered.
     *
     * @since S60 ?S60_version
     * @param aMap              The map containing the children.
     * @param aMapIndex         The index inside the map.
     * @param aContainerIndex   The index inside the container.
     * @param aRoot             Is the map the root data.
     *
     * @return The child index.
     */
    uint childIndex( IAlfMap& aMap, uint aMapIndex, 
             uint aContainerIndex, bool aRoot );
                     
    /**
     * returns next control, that can be focused in relation to
     * the aControl.
     *
     * @since S60 ?S60_version
     * @param aControl reference control. 
     *
     * @return next focusable control, if aControl == 0, first focusable 
     *         control should be returned.
     */
    CAlfWidgetControl* nextFocusableControl(CAlfWidgetControl* aControl);
        
    /**
     * returns previous control, that can be focused in relation to
     * the aControl.
     *
     * @since S60 ?S60_version
     * @param aControl reference control, 
     *
     * @return previous focusable control, if aControl == 0, last focusable
     *         control should be returned.
     */
    CAlfWidgetControl* previousFocusableControl(CAlfWidgetControl* aControl);
        
    // Private state handler methods

    void enableStateVisible();
    void disableStateVisible();
    void enableStateEnabled();
    void disableStateEnabled();
    void enableStateFocused();
    void disableStateFocused();
    void enableStateFocusable();
    void disableStateFocusable();

    // Tools for checking state invariants
    static void checkStateInvariants(uint aNewState);
    void predictAndCheckStateInvariants(
             uint aChangePattern, bool aEnabling) const;

    /**
     * Removes presentaion elements that is associated with 
     * the eventhandler.This will remove the element reference from the 
     * element array .
     *
     * @since S60 ?S60_version
     * @param aEventHandler A reference to event handler associated with
     *                      presentation element.
     *
     * @return the pointer to element data holding the pointer to the element
     *         if it is found, else null
     */
    ElementData* removePesentationElementData(
                    IAlfWidgetEventHandler& aEventHandler );

    /**
     * Constants for aPhase parameter in function ProcessEventHandlingPhase.
     */
    static const int tunneling = 0;
    static const int bubbling = 1;

    /**
     * Goes through all the event handlers in either tunneling or 
     * bubbling phase.
     */
    AlfEventStatus ProcessEventHandlingPhase(
        int aPhase,
        const TAlfEvent& aEvent );

private: // data
    auto_ptr<AlfWidgetControlImpl> mWdgtControlData;
    };

#include "alf/alfwidgetcontrol.inl"

    } // namespace Alf

#endif // ALFWIDGETCONTROL_H

