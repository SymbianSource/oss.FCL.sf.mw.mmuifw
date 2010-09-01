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
* Description:  interface for the scrollbar base element internal functions.
*
*/

#ifndef I_ALFSCROLLBARBASEELEMENTINTERNAL_H
#define I_ALFSCROLLBARBASEELEMENTINTERNAL_H

//Widget Model include
#include <alf/ialfinterfacebase.h>

//Toolkit include
#include <alf/alfevent.h>

namespace Alf
    {
 
/**
 * @namespace alfscrollbarbaseelementinternal
 *
 * ident specifies the unique identifier for the scrollbar base-element 
 * internal interface.
 */
namespace alfscrollbarbaseelementinternal
    {
    static const IfId ident =
        {
        0,"scrollbarbaseelementinternal" 
        };
    }

/**
 * An internal interface to be used by custom scrollbar base element writer.
 *
 * An interface which should be implemented by all the base elements of 
 * scrollbar (e.g. AlfScrollBarDefaltBaseElement, AlfScrollBarArrowBAseElement,
 * AlfScrollBarDefaltLctBaseElement). The interface is exported for the 
 * developers who wants to create their custom scrollbar base element.
 * The functions of this interface are not supposed to be called by scrollbar
 * widget users directly. These functions are called by different part of 
 * scrollbar widget (e.g. scrollbar control, long tap pointer event timer).
 * 
 */
class IAlfScrollBarBaseElementInternal : public IAlfInterfaceBase 
    {

public:

    /**
     * Getter for the type identifier of this interface.
     * Usage:
     * @code
     * IAlfElement* element;
     * IAlfScrollBarBaseElementInternal* elementInternal = 
     * static_cast<IAlfScrollBarBaseElementInternal*> ( 
     * element->makeInterface (IAlfScrollBarBaseElementInternal::type() ) );
     * @endcode
     * @see \link IAlfInterfaceBase \endlink for
     * \link IAlfInterfaceBase::makeInterface \endlink
     * The above code provides all the interfaces of ScrollBar base element.
     * @since S60 ?S60_version
     * @return Identifier of this interface.
     */
    static inline const IfId& type()
        {
        return alfscrollbarbaseelementinternal :: ident;
        }

    /** 
     * baseLayoutSizeChanged. 
     * This function is called by scrollbar control to notify scrollbar base
     * element the base layout size has been changed ( and so the base element
     * should relayout its internal visualization).
     * 
     * @since S60 ?S60_version
     */
    virtual void baseLayoutSizeChanged()=0;
    
    /**
     * Send the corresponding Custom events to the scrollbar eventhandlers 
     * according to the visual clicked.
     * 
     */
    virtual bool processPointerEvent(TAlfEvent * aPntrEvent)=0;
    
    /**
     * creates the visualization  
     * 
     */
    virtual void createVisualization() = 0;
    
    /**
     * Destroys the visualization 
     * 
     */
    virtual void updateVisualization() = 0;
    
    /**
     * updates the visualization 
     * 
     */
    virtual void destroyVisualization() = 0;
    
    /**
     * Returns pointer to AlfScrollBarModel
     */
    virtual void initializeScrollModel() = 0;
    
    /**
     *  Handles Longtap event
     */
    virtual AlfEventStatus handleLongTapEvent() = 0;
    
    /** 
     *virtual Destructor.
     *
     */
    virtual ~IAlfScrollBarBaseElementInternal() {}

    }; // end of class IAlfScrollBarBaseElementInternal
  
    } // end of namespace Alf

#endif // I_ALFSCROLLBARBASEELEMENTINTERNAL_H
//End Of File
