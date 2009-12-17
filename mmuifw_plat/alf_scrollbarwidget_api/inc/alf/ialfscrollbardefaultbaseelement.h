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
* Description:  interface for the scrollbar default lct base element.
*
*/

#ifndef I_ALF_SCROLLBARDEFAULTLCTBASEELEMENT_H
#define I_ALF_SCROLLBARDEFAULTLCTBASEELEMENT_H

#include <alf/ialfelement.h>
#include <alf/ialfwidget.h>
#include <alf/ialfinterfacebase.h>
#include <alf/alfevent.h>
#include <alf/alfwidgetevents.h>

using namespace osncore;

namespace Alf
    {
    /**
     * @namespace alfscrollbardefaultbaseelement
     *
     * ident specifies the unique identifier for the base-element.
     */
    namespace alfscrollbardefaultbaseelement
        {
        static const IfId ident =
            {
            0,"BaseElement" 
            };
        }
/**
 * An interface for the default base element of the scrollbar.
 *
 * The default base element is an interface that contains the basic layouting and scrolling logic of the scrollbar widget.
 * The base element provides a simple scrollbar visualization.
 * It allows the user to:
 * 1. Specify the TotalLength  ViewLength and ThumbWidth
 * 2. Specify the opacity of thumb with respect to widget opacity
 *
 * The default  base element of the scrollbar uses Anchor Layout  
 *
 * The default base element also acts as an eventhandler and currently listens to following events:
 * 1. EEventModelChanged, EEventScrollBarClicked
 *
 * default  base element handles EEventScrollBarClicked event(Custom Event) which are convetred to higher level events
 * like EEventScrollUp ,EEventScrollDown,EEventScrollLeft,EEventScrollRight ,
 * On receiving these events, the default base element accordingly changes the position of the thumb  
 *
 * 
 *  @lib alfscrollbarwidget.lib

 *  @since S60 ?S60_version
 */
class IAlfScrollBarDefaultBaseElement: public IAlfInterfaceBase 
    {

public:
    
    /**
     * Getter for the type identifier of this interface.
     * Usage:
     * @code
     * IAlfElement* element;
     * IAlfBaseElement* baseElement = 
     * static_cast<IAlfScrollBarBaseElement*> ( 
     * element->makeInterface (IAlfScrollBarBaseElement::type() ) );
     * @endcode
     * @see \link IAlfInterfaceBase \endlink for
     * \link IAlfInterfaceBase::makeInterface \endlink
     * The above code provides all the interfaces of ScrollBar base element.
     * @since S60 ?S60_version
     * @return Identifier of this interface.
     */
    static inline const IfId& type()
        {
        return alfscrollbardefaultbaseelement :: ident;
        }
    
    
    /**
     * Sets the animation time for the thumb movement
     * Controls the movement of the thumb in the context of
     * scroll
     * @param aTime, Animation time in milliseconds
     */
    virtual void setThumbAnimationTime ( int aTime ) = 0;

    /**
     * Gets the Thumb animation time.
     *
     * @return Animation time.
     */
    virtual int getThumbAnimationTime() const = 0;

    /**
     * This function is deprecated. Use attribute APIs to set or get opacity.
     * Sets the Opacity of widget.
     * Default value of scrollbar opacity is 1.0.
     * 
     * @param aOpacity Opacity of the scrollbar. The value should be between 0.0
     *                  to 1.0. 
     *
     * @exception AlfException
     */
    virtual void setOpacity(float aOpacity)=0;

    /**
     * This function is deprecated. Use attribute APIs to set or get opacity.
     * Gets the Opacity of widget.
     *
     * @return: Thumb opacity
     */
    virtual float getOpacity() const=0;
    
    /**
     * Sets the Opacity of thumb. This is relative to widget opacity.Default value is 1.0.
     * The value of aOpacity should be between 0.0 to 1.0
     * @param aOpacity Opacity of the thumb
     */
    virtual void setThumbOpacity ( float aOpacity ) = 0;

    /**
     * Gets the Opacity of thumb.
     *
     * @return: Thumb opacity
     */
    virtual float getThumbOpacity() const = 0;
    

    /** 
     * Destructor.
     */
    virtual ~IAlfScrollBarDefaultBaseElement() {}
    }; // class IAlfScrollBarDefaultBaseElement
    } // namespace Alf

#endif // IALF_SCROLLBARDEFAULTLCTBASEELEMENT_H
