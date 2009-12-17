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
* Description:  The implementation for presentation elements.
*
*/


#ifndef ALF_RECYCLINGGRIDELEMENT_H
#define ALF_RECYCLINGGRIDELEMENT_H


//INCLUDES
#include <e32cmn.h> // TPoint
#include <osn/osndefines.h>
#include "alf/alfelement.h"
#include <alf/ialfwidgeteventhandler.h>

class CAlfControl;


namespace Alf
    {
class CAlfWidgetControl;
class AlfRecyclingGridElementImpl;

namespace alfrecyclinggridelement
    {
static const IfId Ident=
    {
    0,"alfrecyclinggridelement"
    };
    }

/** @class AlfRecyclingGridElement alfrecyclinggridelement.h "alf/alfrecyclinggridelement.h"
 * The implementation of a generic element with a functionality
 * for recyclign the created visual trees. This is commonly used
 * in scrolling list-like components, where only few of the items
 * are visible at the same time. Scrolling is supported in both
 * dimensions (rows / columns).
 *
 *  @interfaces IAlfElement
 *  @lib alfwidgetmodel.lib
 *  @since S60 ?S60_version
 *  @status Draft
 */
class AlfRecyclingGridElement : public AlfElement, public IAlfWidgetEventHandler
    {
public:
    static inline const IfId& type()
        {
        return alfrecyclinggridelement::Ident;
        }
    /**
     * Enumerations of the Fill Style
     */
    OSN_IMPORT enum FillStyle{EColumnsFirst, ERowsFirst};



    /**
     * Constructor.
	 * @exception std::bad_alloc     
     * @param aControl - Widget Control
     * @param aName    - Name of the Recycling grid element
     * @param aNumColumns - Number of Columns
     * @param aNumRows  - Number of Rows
     * @since S60 ?S60_version
     * @return New object. The widgets are owned by Alfred. The objects is pushed on to the cleanup stack
     */
    OSN_IMPORT AlfRecyclingGridElement(CAlfWidgetControl& aControl, const char* aName, int aNumColumns, int aNumRows);


    /** Class Destructor estructor.
    *
    *  @since S60 ?S60_version
    */
    OSN_IMPORT virtual ~AlfRecyclingGridElement();

    /**
     * Constructor.
     * @param aFillStyle - style to be used to fill the grid
     * @param aFillStyleParam -
     * @since S60 ?S60_version
     * @return New object. The widgets are owned by Alfred. The objects is pushed on to the cleanup stack
     */
    OSN_IMPORT void setFillStyle(FillStyle aFillStyle, int aFillStyleParam);
    /*
     * Sets the animation times for diferent visual changes
     * @param aScrollingTime - animation time while scrolling
     * @param aFadeInTime - amination time for fading in 
     * @param aFadeinDelay - a delay time after which fade in starts
     * @param aFadeOutTime - animation time for fading out
     * @param aFadeOutDelay - a delay time after which fade out will start
     */ 

    OSN_IMPORT void setAnimationTimes( int aScrollingTime,
                                       int aFadeInTime, int aFadeInDelay,
                                       int aFadeOutTime, int aFadeOutDelay );

    /**
     * Creates a visual tree with the data given for visuals.
     *
     * @exception std::bad_alloc 
     * @exception osncore::AlfVisualException Thrown with the error code osncore::ECanNotCreateVisual\n 
     *                                        when creation of visual fails.
     * @exception osncore::AlfVisualException Thrown with the error code osncore::EInvalidElement\n 
     *                                        when wrond data id is supplied.
     * @exception std::bad_alloc     
     * @since S60 ?S60_version
     * @param aData Data for visual tree to be created
     * @param aDataID Parent visual tree's data id
     * @param aParentLayout Parent layout to which newly created visual tree will be added
     * @param aLayoutIndex Position in parent layout at which new visual tree will be added
     */

    OSN_IMPORT virtual CAlfVisual* createVisualTree(
        IAlfVariantType& aData, uint aDataID,
        CAlfLayout* aParentLayout, int aLayoutIndex );

    //Implementation of IAlfWidgetEventHandler

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
    OSN_IMPORT virtual bool accept( CAlfWidgetControl& aControl, const TAlfEvent& aEvent ) const;

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
   OSN_IMPORT AlfEventStatus offerEvent( CAlfWidgetControl& aControl,const TAlfEvent& aEvent );
   
    /**
     * @see MAlfWidgetEventHandler
     */
    OSN_IMPORT void setActiveStates( unsigned int aStates );
    
    /**
     * Sets AlfWidgetEventHandlerInitData to event handler.
     *
     * @param aData A data structure which contains for example id of the event
     * handler.
     */
    OSN_IMPORT void setEventHandlerData( const AlfWidgetEventHandlerInitData& aData );
    
    /**
     * Returns AlfWidgetEventHandlerInitData.
     *
     * @return  A pointer to AlfWidgetEventHandlerInitData structure which contains
     * for example id of the event handler.
     */
    OSN_IMPORT AlfWidgetEventHandlerInitData* eventHandlerData();

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

    /**
     * From @see MAlfInterfaceBase.
     * @since S60 ?S60_version
     * @param aType The type of interface requested.
    * @return Returns the new Interface
     */
    OSN_IMPORT virtual  IAlfInterfaceBase* makeInterface( const IfId& aType );


    /**
     * Creates a visual tree with the data given for visuals.
     *
     * @exception std::bad_alloc 
     * @exception osncore::AlfVisualException Thrown with the error code osncore::ECanNotCreateVisual\n 
     *                                        when creation of visual fails.
     * @exception osncore::AlfVisualException Thrown with the error code osncore::EInvalidElement\n 
     *                                        when wrond data id is supplied.
     * @exception std::bad_alloc     
     * @since S60 ?S60_version
     * @param aData Data for visual tree to be created
     * @param aDataID Parent visual tree's data id
     * @param aParentLayout Parent layout to which newly created visual tree will be added
     * @param aLayoutIndex Position in parent layout at which new visual tree will be added
     */
    OSN_IMPORT virtual void createChildVisualTree( IAlfElement* aElement,
            IAlfVariantType& aChildData,
            IAlfBranch& aData,
            int aIndex, uint aDataID );
    /**
     * Removes visual tree from given element.
     *
     * @since S60 ?S60_version
     * @param aElement Element from which visual tree is to be removed
     * @param aData
     * @param aIndex 
     * @param aDataID 
     */
    OSN_IMPORT virtual void removeChildVisualTree( IAlfElement* aElement, IAlfBranch& aData, int aIndex, uint aDataID );

    /**
     * removes and destroys all the visuals.
     *
     * @param aTimeMilliseconds animation time for remove.
     */
    OSN_IMPORT void removeAndDestroyVisuals( int aTimeMilliseconds );

private:
    friend class AlfRecyclingGridElementImpl;
    auto_ptr<AlfRecyclingGridElementImpl> mImpl;
    };

    } // namespace

#endif // ALF_RECYCLINGGRIDELEMENT_H
