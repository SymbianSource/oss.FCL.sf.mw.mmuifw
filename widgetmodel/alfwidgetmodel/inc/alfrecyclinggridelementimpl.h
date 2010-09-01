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
* Description:   Class for private implementation for AlfRecyclingGridElement.
*
*/


#ifndef ALFRECYCLINGGRIDELEMENTIMPL_H
#define ALFRECYCLINGGRIDELEMENTIMPL_H

#include "alf/alfrecyclinggridelement.h"
#include <osn/osndefines.h>

class CAlfVisual;
class CAlfLayout;
class TAlfEvent;

namespace Alf
    {
class CAlfWidgetControl;
class IAlfVariantType;
class IAlfBranch;
class IAlfElement;

/**
 *  Implementation class used with the implPtr paradigm.
 */
OSN_NONSHARABLE_CLASS( AlfRecyclingGridElementImpl )
    {
public:
    /**
        * Gets the unit.
        *
        * @since S60 ?S60_version
        * @return The unit.
        */
    AlfRecyclingGridElementImpl(
        AlfRecyclingGridElement& aOwner,
        int aNumColumns,
        int aNumRows);
    /**
        * Gets the unit.
        *
        * @since S60 ?S60_version
        * @return The unit.
        */
    ~AlfRecyclingGridElementImpl();
    /**
        * setFillStyle.
        *
        * @since S60 ?S60_version
        * @return The unit.
        */
    void setFillStyle(AlfRecyclingGridElement::FillStyle aFillStyle, int aFillStyleParam);

    /**
     * setAnimationTimes.
     *
     * @since S60 ?S60_version
     * @return The unit.
     */
    void setAnimationTimes( int aScrollingTime,
                            int aFadeInTime, int aFadeInDelay,
                            int aFadeOutTime, int aFadeOutDelay );
    /**
     * createVisualTree.
     *
     * @since S60 ?S60_version
     * @return The unit.
     */
    CAlfVisual* createVisualTree(
        IAlfVariantType& aData, uint aDataID,
        CAlfLayout* aParentLayout, int aLayoutIndex );
    /**
     * accept.
     *
     * @since S60 ?S60_version
     * @return The unit.
     */
    bool accept( CAlfWidgetControl& aControl, const TAlfEvent& aEvent ) const;
    /**
     * offerEvent.
     *
     * @since S60 ?S60_version
     * @return The unit.
     */
    bool offerEvent( CAlfWidgetControl& aControl, const TAlfEvent& aEvent );
    /**
     * setActiveStates.
     *
     * @since S60 ?S60_version
     * @return The unit.
     */
    void setActiveStates( unsigned int aStates );
    /**
     * Sets AlfWidgetEventHandlerInitData to event handler.
     *
     * @param aData A data structure which contains for example id of the event
     * handler.
     */
    void setEventHandlerData( const AlfWidgetEventHandlerInitData& /*aData*/ ) { }
    
    /**
     * Returns AlfWidgetEventHandlerInitData.
     *
     * @return  A pointer to AlfWidgetEventHandlerInitData structure which contains
     * for example id of the event handler.
     */
    AlfWidgetEventHandlerInitData* eventHandlerData() { return NULL; }
    /**
     * createChildVisualTree.
     *
     * @since S60 ?S60_version
     * @return The unit.
     */
    void createChildVisualTree( IAlfElement* aElement,
                                IAlfVariantType& aChildData,
                                IAlfBranch& aData,
                                int aIndex, uint aDataID );
    /**
     * removeChildVisualTree.
     *
     * @since S60 ?S60_version
     * @return The unit.
     */
    void removeChildVisualTree( IAlfElement* aElement, IAlfBranch& aData, int aIndex, uint aDataID );
    /**
     * removeAndDestroyVisuals.
     *
     * @since S60 ?S60_version
     * @return The unit.
     */
    void removeAndDestroyVisuals( int aTimeMilliseconds );

private:

    /**
     * Api to naviagte by a single row or column.
     * @since S60 ?S60_version
     * @param aControl - Widget Control
     * @param aPageUp  - indicates the direction either a page up or page down
     */
    bool moveSingleRowOrColumn( CAlfWidgetControl& aControl, bool aPageUp);

    /**
     * APi to find the point in the grid for a given index in the visual array on the recycle grid  .
     * @since S60 ?S60_version
     * @param aInd - index of the element
     * @return  TPoint - Point or cordinates
     */
    TPoint calculateChildPos(int aInd) const;

    /**
     * APi to find the index for a given point
     * @since S60 ?S60_version
     * @param aPos- position for whom the index needs to be found out
     * @return  int  - Index of the cordinate
     */
    int calculateChildInd(TPoint aPos) const;

    /**
     * Internal APi to do the index calcuation for navigation and recycling
    */
    void doPositioningAndScrolling( CAlfWidgetControl& aControl,uint aEventID);

private:

    AlfRecyclingGridElement& mOwner;

    //class Data
    /**
     * The number of visible rows and columns.
     */
    int mNumVisibleColumns, mNumVisibleRows;

    /**
     * The number of rows and columns in the grid layout.
     */
    int mNumColumns, mNumRows;

    /**
     * The strategy for adding new items to the grid
     */
    AlfRecyclingGridElement::FillStyle mFillStyle;

    /**
     * The maximum number of items used with EColumnsFirst and ERowsFirst fill styles
     */
    int mFillStyleParam;


    /**
     * Duration of the scrolling animation
     */
    int mScrollingTime;

    /**
     * Duration of the fade in animation
     */
    int mFadeInTime;

    /**
     * Delay before the fade in animation
     */
    int mFadeInDelay;

    /**
     * Duration of the fade out animation
     */
    int mFadeOutTime;

    /**
     * Delay before the fade out animation
     */
    int mFadeOutDelay;

    /**
     * The logical scrolling offset.
     */
    TPoint  mScrollOffset;

    bool    mHorizontalOffset;
    bool    mVerticalOffset;
    unsigned int mActiveStates;
    };

    } // namespace Alf

#endif // C_ALFWIDGETEVENTHANDLERIMPL_H
