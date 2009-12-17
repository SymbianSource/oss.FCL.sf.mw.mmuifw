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
* Description:  Definition of the commonly used custom event ids.
*
*/


#ifndef ALFWIDGETEVENTS_H
#define ALFWIDGETEVENTS_H

namespace Alf
    {
/**
 * The list of commonly used custom event ids.
 * The widget-specific ids should use values from ELastCommonWidgetEvent.
 *
 * @lib alfwidgetmodel.lib
 * @since S60 ?S60_version
 */
enum CommonWidgetEvent
    {
    EEventActivateState             = 0,
    EEventDeactivateState           = 1,
    EEventWidgetInitialized         = 2,
    EEventSetFocus                  = 3,
    EEventClearFocus                = 4,
    EEventFocusedItemTypeChanged    = 5,
    EEventItemSelected              = 6,
    EEventNavigatePrimaryNext       = 7,
    EEventNavigatePrimaryPrev       = 8,
    EEventNavigateSecondaryNext     = 9,
    EEventNavigateSecondaryPrev     = 10,
    EEventNavigateLeft              = 11,
    EEventNavigateRight             = 12,
    EEventNavigateUp                = 13,
    EEventNavigateDown              = 14,
    EEventScrollToTarget            = 15,
    EEventScrollPrimaryNext         = 16,
    EEventScrollPrimaryPrev         = 17,
    EEventScrollSecondaryNext       = 18,
    EEventScrollSecondaryPrev       = 19,
    EEventScrollToFirst             = 20,
    EEventScrollToLast              = 21,
    EEventScrollUp                  = 22,
    EEventScrollDown                = 23,
    EEventScrollLeft                = 24,
    EEventScrollRight               = 25,
    EEventScrollPageUp              = 26,
    EEventScrollPageDown            = 27,
    EEventScrollPageLeft            = 28,
    EEventScrollPageRight           = 29,
    EEventScrollBarModelChanged     = 30,
    EEventFocusNextWidget           = 31,
    EEventFocusPreviousWidget       = 32,
    EEventWidgetGainedFocus         = 33,
    EEventWidgetLostFocus           = 34,
    EEventWidgetAttributeChanged    = 35,
    EEventWidgetPresentationDestroyed = 36,
    EEventDragVertical              = 37,
    EEventDragHorizontal            = 38,
    EEventMissedPointerUp           = 39,
    ELastCommonWidgetEvent          = 99
    };

/**
 * OSN pointer event enumeration.
 *
 * @lib alfwidgetmodel.lib
 * @since S60 ?S60_version
 */
enum WidgetPointerEvent
    {
    pointerEventDown = ELastCommonWidgetEvent + 1,
    pointerEventUp,
    pointerEventDrag
    };

/**
 * Event type flags.
 */
enum EventTypeFlags
    {
    custom       = 0x0001,   // custom event
    keyDown      = 0x0002,   // key down event, key repeat generates this event also
    keyUp	     = 0x0004,   // key up event

   /** Flag to indicate that the custom event data
     * should be used to determine the accepted events.
     * Can not be used with key events.
     */
    useEventData  = 0x8000
    };

/**
 * Key event modifiers.
 */
enum KeyEventModifiers
    {
    alt                     = 0x0001,
    ctrl                    = 0x0002,
    shift                   = 0x0004,
    func                    = 0x0008,
    capsLock                = 0x0010,
    numLock                 = 0x0020,
    scrollLock              = 0x0040
    };

    } // namespace Alf

#endif // ALFWIDGETEVENTS_H
