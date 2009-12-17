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
* Description:  An interface used by design tools to get a description of the control.
*
*/


#ifndef I_ALFWIDGETCONTROLDESCRIPTOR_H
#define I_ALFWIDGETCONTROLDESCRIPTOR_H

#include <e32base.h>

#include <alf/ialfinterfacebase.h>

namespace Alf
    {

/**
 *  An interface used by design tools to get a description of the control.
 *  The description may include a generic description of the control, and
 *  descriptions of the event handlers and elements used by the control.
 *
 *  @lib alfwidgetmodel.lib
 *  @since S60 ?S60_version
 *  @status Draft
 */
class IAlfWidgetControlDescription : public IAlfInterfaceBase
    {
public:
    /**
     * Get the description of the control.
     *
     * @since S60 ?S60_version
     * @return The description of the control.
     */
    virtual const TDesC8& controlDescription() const = 0;

    /**
     * Get the number of required elements in the control.
     *
     * @since S60 ?S60_version
     * @return The number of required elements.
     */
    virtual int numRequiredElements() const = 0;

    /**
     * Get the name of a required element by index.
     *
     * @since S60 ?S60_version
     * @param aIndex The index of the element.
     * @return The name of the element at the given index.
     */
    virtual const TDesC8& elementName(int aIndex) const = 0;

    /**
     * Get the description of a required element by index.
     *
     * @since S60 ?S60_version
     * @param aIndex The index of the element.
     * @return The description of the element at the given index.
     */
    virtual const TDesC8& elementDescription(int aIndex) const = 0;

    /**
     * Get the number of required event handlers in the control.
     *
     * @since S60 ?S60_version
     * @return The number of required event handlers.
     */
    virtual int numRequiredEventHandlers() const = 0;

    /**
     * Get the event id of a required event handler by index.
     * The event id may be a custaom event id or a key event id.
     *
     * @since S60 ?S60_version
     * @param aIndex The index of the event handler.
     * @return The name of the event handler at the given index.
     */
    virtual uint eventHandlerId(int aIndex) const = 0;

    /**
     * Determine the type of a required event handler by index.
     * The event type may be a custom event or a key event.
     *
     * @note Pointer event handlers cannot be identified without
     * an associated custom event id. Therefore, pointer event handlers
     * are visible as custom event handlers.
     *
     * @since S60 ?S60_version
     * @param aIndex The index of the event handler.
     * @return <code>ETrue</code> if the event handler is a key event handler.
     *         <code>EFalse</code> if the event handler is a custom event handler.
     */
    virtual bool keyEventHandler(int aIndex) const = 0;

    /**
     * Get the name of a required event handler by index.
     *
     * @since S60 ?S60_version
     * @param aIndex The index of the event handler.
     * @return The name of the event handler at the given index.
     */
    virtual const TDesC8& eventHandlerName(int aIndex) const = 0;

    /**
     * Get the description of a required event handler by index.
     *
     * @since S60 ?S60_version
     * @param aIndex The index of the event handler.
     * @return The description of the event handler at the given index.
     */
    virtual const TDesC8& eventHandlerDescription(int aIndex) const = 0;

    };

    } // namespace Alf

#endif // I_ALFWIDGETCONTROLDESCRIPTOR_H

// End of File
