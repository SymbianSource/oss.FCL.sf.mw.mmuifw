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
* Description:  Navigation interface for list-like widgets.
*
*/



#ifndef I_ALFNAVIGATION_H
#define I_ALFNAVIGATION_H

#include <alf/ialfinterfacebase.h>

namespace Alf
    {

namespace alfnavigation
    {
static const IfId ident =
    {
    0, "alfnavigation"
    };
    }

/**
*  An interface that defines functionality for navigation in a
*  one-dimensional array of items. In addition to the normal
*  one-dimansional behavior, a common use case is to have
*  a two-dimensional layout and navigation model for the items.
*  Therefore, both one- and two-dimensional navigation models are
*  supported by the interface.
*
*  Since the logical behavior is decoupled from the presentation,
*  the navigation is controlled in terms of logical dimensions,
*  namely the primary and the secondary dimensions. The primary
*  dimension is the dimension in which the navigation space
*  expands when new items are inserted. In a one-dimensional layout,
*  only the primary dimension is needed. The secondary dimension
*  is used with two-dimensional layouts and navigation models.
*  The size of the navigation space in the secondary dimension
*  should be a constant.
*
*  The navigation interface consists of navigation parameters,
*  scrolling parameters, and orientation parameters. The navigation
*  parameters are used to control the focused item in the logical
*  navigation events. The scrolling parameters are used to send
*  scrolling events according to the focused item. The orientation
*  parameters are used to convert directional navigation events
*  to the logical navigation events. In addition, the usage of
*  default navigation key events, i.e., arrow keys, can be enabled
*  or disabled.
*
*  @lib alfwidgetmodel.lib
*  @since S60 ?S60_version
*  @status Draft
*/
class IAlfNavigation : public IAlfInterfaceBase
    {
public:
    /**
     * Getter for the type identifier of this interface.
     *
     * @since S60 ?S60_version
     * @return A descriptor to identify the type of this interface.
     */
    static inline const IfId& type()
        {
        return alfnavigation::ident;
        }

    /**
     * Getter for the index of the currently focused item.
     *
     * @since S60 ?S60_version
     * @return An index of the currently focused item.
     */
    virtual int focusedIndex() const = 0;

    /**
     * Setter for the index of the currently focused item.
     * If the given index is outside the range of items,
     * the index is set to the first or the last index.
     *
     * @since S60 ?S60_version
     * @param aIndex The new index of the focused item.
     */
    virtual void setFocusedIndex( int aIndex ) = 0;

    /**
     * Getter for the size of the page in primary dimension.
     * The page size is used for EEventNavigatePrimaryPageUp and
     * EEventNavigatePrimaryPageDown events, as well as for scrolling
     * in primary dimension.
     *
     * @since S60 ?S60_version
     * @return The page size in primary dimension.
     */
    virtual int primaryPageSize() const = 0;

    /**
     * Getter for the size of the page in secondary dimension.
     * The page size is used for EEventNavigateSecondaryPageUp and
     * EEventNavigateSecondaryPageDown events, as well as for scrolling
     * in secondary dimension.
     *
     * @since S60 ?S60_version
     * @return The page size in secondary dimension.
     */
    virtual int secondaryPageSize() const = 0;

    /**
     * Getter for the size of the navigation space in secondary dimension.
     * One step in the primary dimension changes the index of the focused
     * item by the secondary size. The size is also used for scrolling
     * in secondary dimension. The secondary size of a one-dimensional
     * list should be one.
     *
     * @since S60 ?S60_version
     * @return The size in secondary dimension.
     */
    virtual int secondarySize() const = 0;

    /**
     * Getter for looping in primary dimension.
     *
     * @since S60 ?S60_version
     * @return <code>true</code> if the looping is enabled,
     *         <code>false</code> otherwise.
     */
    virtual bool primaryLooping() const = 0;

    /**
     * Getter for looping in secondary dimension.
     *
     * @since S60 ?S60_version
     * @return <code>true</code> if the looping is enabled,
     *         <code>false</code> otherwise.
     */
    virtual bool secondaryLooping() const = 0;

    /**
     * Set the navigation parameters for one-dimensional navigation.
     * The secondary size and the secondary page size are set to one,
     * to indicate that navigation and scrolling in secondary direction
     * is disabled. Also the secondary looping is disabled.
     *
     * @since S60 ?S60_version
     * @param aPrimaryPageSize The page size in primary dimension.
     * @param aPrimaryLooping Enable or disable looping.
     */
    virtual void setNavigationParams( int aPrimaryPageSize,
                                      bool aPrimaryLooping ) = 0;

    /**
     * Set the navigation parameters for two-dimensional navigation.
     * The parameters determine the behavior of all navigation events.
     * Navigation and scrolling are supported in both dimensions.
     * If the secondary page size is equal to the size of the whole
     * navigation space, the scrolling in secondary dimension is disabled.
     *
     * @since S60 ?S60_version
     * @param aPrimaryPageSize The page size in primary dimension.
     * @param aPrimaryLooping Enable or disable looping.
     */
    virtual void setNavigationParams( int aPrimaryPageSize,
                                      int aSecondarySize,
                                      int aSecondaryPageSize,
                                      bool aPrimaryLooping,
                                      bool aSecondaryLooping ) = 0;

    /**
     * Getter for scrolling distance in primary dimension.
     * The scrolling distance is the minimum number of items before
     * the first or the last visible item. For example, the scrolling
     * distance of one defines that the focus should never get to the
     * first or the last visible item, except in the beginning or the
     * end of the list.
     *
     * @since S60 ?S60_version
     * @return The scrolling distance in primary dimension.
     */
    virtual int primaryScrollingDist() const = 0;

    /**
     * Getter for scrolling distance in secondary dimension.
     *
     * @since S60 ?S60_version
     * @return The scrolling distance in secondary dimension.
     */
    virtual int secondaryScrollingDist() const = 0;

    /**
     * Set the scrolling distances for the list control.
     * The scrolling distances are used to determine when
     * to start scrolling. The scrolling distance of zero
     * indicates that the scrolling should start if the focus
     * is moved outside of the visible items. Similarly,
     * the scrolling distance of one indicates that the
     * scrolling should start when the first or the last
     * visible item is focused.
     *
     * @param aPrimaryScrollingDist The minimum number of items before the first
     *                              or the last visible item in primary dimension.
     * @param aSecondaryScrollingDist The minimum number of items before the first
     *                                or the last visible item in secondary dimension.
     * @since S60 ?S60_version
     */
    virtual void setScrollingParams( int aPrimaryScrollingDist,
                                     int aSecondaryScrollingDist ) = 0;

    /**
     * Determine if the primary dimension on the screen is vertical.
     * This is used to convert directional navigation events to the
     * logical navigation events.
     *
     * @since S60 ?S60_version
     * @return <code>true</code> if the primary dimension is vertical,
     *         <code>false</code> if the primary dimension is horizontal.
     */
    virtual bool verticalPrimary() const = 0;

    /**
     * Determine if the indices in the navigation space
     * increase from left to right. This is used to determine
     * the direction of the horizontal navigation events.
     *
     * @since S60 ?S60_version
     * @return <code>true</code> if the indices increase from left to right,
     *         <code>false</code> if the indices increase from right to left.
     */
    virtual bool leftToRight() const = 0;

    /**
     * Determine if the indices in the navigation space
     * increase from up to down. This is used to determine
     * the direction of the vertical navigation events.
     *
     * @since S60 ?S60_version
     * @return <code>true</code> if the indices increase from up to down,
     *         <code>false</code> if the indices increase from down to up.
     */
    virtual bool upToDown() const = 0;

    /**
     * Set the orientation parameters.
     * The orientation parameters are used to convert directional
     * navigation events to logical navigation events.
     *
     * @since S60 ?S60_version
     * @param aVerticalPrimary Specify the orientation of the primary dimension.
     * @param aLeftToRight Specify the increasing order of horizontal indices.
     * @param aUpToDown Specify the increasing order of vertical indices.
     */
    virtual void setOrientationParams( bool aVerticalPrimary,
                                       bool aLeftToRight, bool aUpToDown ) = 0;

    /**
     * Determine if the default key events are used for navigation.
     *
     * @since S60 ?S60_version
     * @return <code>true</code> if the default key events are used,
     *         <code>false</code> otherwise.
     */
    virtual bool defaultKeysEnabled() const = 0;

    /**
     * Enable or disable the default key events.
     *
     * @since S60 ?S60_version
     * @param aEnable The usage of the default key events.
     */
    virtual void enableDefaultKeys( bool aEnable ) = 0;
    };

    } // namespace Alf

#endif // I_ALFNAVIGATION_H

// End of File
