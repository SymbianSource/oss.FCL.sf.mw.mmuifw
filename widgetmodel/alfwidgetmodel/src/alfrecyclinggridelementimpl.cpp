/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:   Private Implementation file for AlfRecyclingGridElement.
*
*/


#include "alfrecyclinggridelementimpl.h"
#include "autoarrayptr.h"
#include <uiacceltk/HuiLayout.h> // Just for CHuiLayout::EExpandVertically
#include <alf/alfenv.h>
#include <alf/alfconstants.h>
#include <alf/alfcommand.h>

#include <alf/alfgridlayout.h>
#include <alf/alflayout.h>
#include <alf/alfcontrol.h>
#include <alf/alfevent.h>

#include "alf/ialfvisualtemplate.h"
#include <alf/alfvarianttype.h>
#include <alf/alfwidgetcontrol.h>
#include <alf/alfvisualexception.h>
#include <alf/alfwidgetevents.h>
#include <osn/ustring.h>
#include <memory>
#include <osn/osnnew.h>

using namespace osncore;

_LIT8(KMainLayoutName, "MainLayout");
_LIT8(KScrollLayoutName, "ScrollLayout");

const int KDefaultScrollingTime = 400;

using namespace Alf;

AlfRecyclingGridElementImpl::AlfRecyclingGridElementImpl(
    AlfRecyclingGridElement& aOwner,
    int aNumColumns,
    int aNumRows) : mOwner( aOwner )
    {
    mNumVisibleColumns     = aNumColumns;
    mNumVisibleRows     = aNumRows;
    mNumColumns      = 0;
    mNumRows      = 0;
    mFillStyle      = AlfRecyclingGridElement::AlfRecyclingGridElement::EColumnsFirst;
    mFillStyleParam     = 0;
    mScrollingTime     = KDefaultScrollingTime;
    mFadeInTime      = KDefaultScrollingTime / 2.f;
    mFadeInDelay     = KDefaultScrollingTime / 2.f;
    mFadeOutTime     = KDefaultScrollingTime / 2.f;
    mFadeOutDelay     = 0;
    mScrollOffset.iX    = 0;
    mScrollOffset.iY    = 0;
    mHorizontalOffset    = false;
    mVerticalOffset     = false;

    setFillStyle(AlfRecyclingGridElement::AlfRecyclingGridElement::EColumnsFirst, aNumColumns);
    }

//----------------------------------------------------------------
// Class Destructor
//----------------------------------------------------------------
AlfRecyclingGridElementImpl::~AlfRecyclingGridElementImpl()
    {
    }

//----------------------------------------------------------------
// Sets the fill style for the Recycle grid element
//----------------------------------------------------------------
void AlfRecyclingGridElementImpl::setFillStyle(AlfRecyclingGridElement::FillStyle aFillStyle, int aFillStyleParam)
    {
    mFillStyle = aFillStyle;
    mFillStyleParam = aFillStyleParam;
    mNumRows = mNumVisibleRows;
    mNumColumns = mNumVisibleColumns;
    if (mFillStyle == AlfRecyclingGridElement::EColumnsFirst ||
            (mFillStyle == AlfRecyclingGridElement::ERowsFirst && mFillStyleParam > mNumRows))
        {
        mNumRows++; // 1 extra row for scrolling
        }
    if (mFillStyle == AlfRecyclingGridElement::ERowsFirst ||
            (mFillStyle == AlfRecyclingGridElement::EColumnsFirst && mFillStyleParam > mNumColumns))
        {
        mNumColumns++; // 1 extra column for scrolling
        }
    }


void AlfRecyclingGridElementImpl::setAnimationTimes(
    int aScrollingTime, int aFadeInTime, int aFadeInDelay,
    int aFadeOutTime, int aFadeOutDelay )
    {
    mScrollingTime = aScrollingTime;
    mFadeInTime = aFadeInTime;
    mFadeInDelay = aFadeInDelay;
    mFadeOutTime = aFadeOutTime;
    mFadeOutDelay = aFadeOutDelay;
    }


CAlfVisual* AlfRecyclingGridElementImpl::createVisualTree(
    IAlfVariantType& aData, uint aDataID, CAlfLayout* aParentLayout, int aLayoutIndex )
    {
    CAlfLayout *main = NULL;
    CAlfGridLayout *grid = NULL;
    CAlfVisual* visualTree = mOwner.findVisual(aDataID);

    if ( visualTree )
        {
        main = (CAlfLayout *)visualTree;
        grid = (CAlfGridLayout *)&main->Visual( 0 );
        }
    else
        {
        main = CAlfLayout::AddNewL((CAlfControl &)mOwner.control());
        main->SetTagL(KMainLayoutName);

        // Create grid layout
        grid = CAlfGridLayout::AddNewL((CAlfControl&)mOwner.control(),mNumColumns, mNumRows, main);

        // Scroll layout for additional content,
        // created by the visual template or other elements.
        CAlfLayout *scroll = CAlfLayout::AddNewL((CAlfControl&)mOwner.control(), main);
        scroll->SetTagL(KScrollLayoutName);

        if (mOwner.getVisualTemplate())
            {
            mOwner.getVisualTemplate()->createVisualTree(
                mOwner.control(), aData.branch()->data(), scroll, scroll->Count());
            }
        if (aParentLayout)
            {
            aParentLayout->Insert(main, aLayoutIndex );
            }
        }

    TAlfRealPoint size = main->Size().ValueNow();
    TAlfRealPoint itemSize(size.iX / (TReal)mNumVisibleColumns,size.iY / (TReal)mNumVisibleRows);
    TAlfRealPoint gridSize(itemSize.iX * mNumColumns, itemSize.iY * mNumRows);
    grid->SetSize(gridSize);

    mOwner.addVisualTree(main, aDataID);

    CAlfLayout *parent = NULL;
    if (mFillStyle == AlfRecyclingGridElement::EColumnsFirst)
        {
        // No reordering needed, add directly to the grid layout
        parent = grid;
        }
    CAlfVisual** array = new( EMM ) CAlfVisual*[mNumColumns * mNumRows];

    // Ownership transferred to the autoptr.
    auto_arrayptr<CAlfVisual*> arrayCleanup( array );

    int childIndex = 0;
    // Create the children visual trees
    if ( aData.type() == IAlfVariantType::EBranch &&
            aData.branch()->childrenCount())
        {
        IAlfBranch* branch = aData.branch();
        TPoint childPos = calculateChildPos(0);
        int maxColumn = mScrollOffset.iX + mNumColumns;
        int maxRow = mScrollOffset.iY + mNumRows;
        for (int i = 0; i < branch->childrenCount() &&
                ( childPos.iX < maxColumn || childPos.iY < maxRow); ++i)
            {
            childPos = calculateChildPos(i);
            if (childPos.iX >= mScrollOffset.iX && childPos.iX < maxColumn &&
                    childPos.iY >= mScrollOffset.iY && childPos.iY < maxRow)
                {
                IAlfElement* child = mOwner.control().findElement( aData.branch()->childName(i).getUtf8());
                uint childID = mOwner.control().elementDataId(*child, i, aDataID);
                array[childIndex] = child->createVisualTree(
                                        *branch->childData( i ), *branch, i, aDataID, parent, childIndex );
                childIndex++;
                }

            }
        }

    // Reorder and insert to the grid layout
    if (mFillStyle == AlfRecyclingGridElement::ERowsFirst)
        {
        for (int i = 0; i < mNumRows && childIndex >= 0; ++i)
            {
            for (int j = 0; j < mNumColumns && childIndex >= 0; ++j)
                {
                int err=grid->Append(array[j * mNumRows + i]);
				if(err!=KErrNone)
					{
					ALF_THROW ( AlfVisualException, err, "Grid Append Failed" )
					}                
                childIndex--;
                }
            }
        }

    return main;
    }


//----------------------------------------------------------------
// Gets the Point of  a child at a given index
//----------------------------------------------------------------
TPoint AlfRecyclingGridElementImpl::calculateChildPos(int aInd) const
    {
    if (mFillStyle == AlfRecyclingGridElement::EColumnsFirst)
        {
        return TPoint(aInd % mFillStyleParam, aInd / mFillStyleParam);
        }
    else if (mFillStyle == AlfRecyclingGridElement::ERowsFirst)
        {
        return TPoint(aInd / mFillStyleParam, aInd % mFillStyleParam);
        }
    return TPoint(0, 0);
    }

//----------------------------------------------------------------
// Gets the Index of  a child at a given point
//----------------------------------------------------------------
int AlfRecyclingGridElementImpl::calculateChildInd(TPoint aPos) const
    {
    if (aPos.iX < 0 || aPos.iY < 0)
        {
        return -1; // Does not exist
        }
    if (mFillStyle == AlfRecyclingGridElement::EColumnsFirst)
        {
        if (aPos.iX >= mFillStyleParam)
            {
            return -1; // Does not exist
            }
        return aPos.iY * mFillStyleParam + aPos.iX;
        }
    else if (mFillStyle == AlfRecyclingGridElement::ERowsFirst)
        {
        if (aPos.iY >= mFillStyleParam)
            {
            return -1; // Does not exist
            }
        return aPos.iX * mFillStyleParam + aPos.iY;
        }
    return 0;
    }



// ---------------------------------------------------------------------------
// OfferEventL - Handle the Event.
// ?implementation_description
// ---------------------------------------------------------------------------
bool AlfRecyclingGridElementImpl::offerEvent( CAlfWidgetControl& aControl, const TAlfEvent& aEvent )
    {

    if ( !accept( aControl, aEvent ) )
        {
        return false;
        }
    uint aEventID = aEvent.CustomParameter();

    if ( aEventID == EEventScrollPrimaryNext )
        {
        aEventID = ( mFillStyle == AlfRecyclingGridElement::EColumnsFirst ? EEventScrollDown : EEventScrollRight );
        }
    else if ( aEventID == EEventScrollPrimaryPrev )
        {
        aEventID = ( mFillStyle == AlfRecyclingGridElement::EColumnsFirst ? EEventScrollUp : EEventScrollLeft );
        }
    else if ( aEventID == EEventScrollSecondaryNext )
        {
        aEventID = ( mFillStyle == AlfRecyclingGridElement::EColumnsFirst ? EEventScrollRight : EEventScrollDown );
        }
    else if ( aEventID == EEventScrollSecondaryPrev )
        {
        aEventID = ( mFillStyle == AlfRecyclingGridElement::EColumnsFirst ? EEventScrollLeft : EEventScrollUp );
        }

    if ((int)aEventID == (int)EEventScrollPageDown )
        {
        int iter=0;
        if ((int)mFillStyle == (int)AlfRecyclingGridElement::EColumnsFirst )
            {
            iter = mNumRows-1 ;
            }
        else
            {
            iter = mNumColumns-1 ;
            }

        for (int i=0;i<iter;i++)
            {
            moveSingleRowOrColumn(aControl , false);
            }
        return true;
        }
    else if ((int)aEventID == (int)EEventScrollPageUp )
        {
        int iter=0;
        if ((int)mFillStyle == (int)AlfRecyclingGridElement::EColumnsFirst )
            {
            iter = mNumRows-1 ;
            }
        else
            {
            iter = mNumColumns-1 ;
            }

        for (int i=0;i<iter;i++)
            {
            moveSingleRowOrColumn(aControl , true);
            }
        return true;

        }
    else
        {
        doPositioningAndScrolling(aControl,aEventID);
        }
    return true;

    }

// ---------------------------------------------------------------------------
// ?implementation_description
// ---------------------------------------------------------------------------
//
void AlfRecyclingGridElementImpl::setActiveStates( unsigned int aStates )
    {
    mActiveStates = aStates;
    }


void AlfRecyclingGridElementImpl::doPositioningAndScrolling( CAlfWidgetControl& aControl,uint aEventID)
    {
    //unsigned int16 id = aEvent.CustomEventData();//Index of the focus element

    CAlfLayout *main = (CAlfLayout *)mOwner.findVisual(0);   //(CAlfLayout *)VisualTreeArray()[0];//root element
    CAlfGridLayout *grid = (CAlfGridLayout *)&main->Visual(0);
    CAlfLayout *scroll = (CAlfGridLayout *)&main->Visual(1);


    TSize size;

    // Grid/scroll layout movement
    TPoint layoutMovement(0, 0);

    // New number of the new items to reveal (and old items to hide)
    int numNewItems = 0;

    // Starting indices and increment step for recycling visuals
    int recycleSrcInd = 0, recycleDstInd = 0;
    int recycleIndStep = 0;

    // Starting indices and increment step for fading in/out visuals
    int fadeInInd = 0, fadeOutInd = 0;
    int fadeIndStep = 0;

    // Is recycling required?
    bool recycle = false;

    // Index of the first new child visual
    TPoint firstNewChildPos(0, 0);

    // Increment step for the new child indices
    TPoint childPosStep(0, 0);

    if (aEventID == EEventScrollUp) // Up
        {
        firstNewChildPos = mScrollOffset + TPoint(mHorizontalOffset ? -1 : 0, -1);
        childPosStep = TPoint(1, 0);
        mScrollOffset.iY--;
        numNewItems = mNumColumns;
        fadeInInd = mNumColumns - 1;
        fadeOutInd = grid->Count() - 1;
        recycleSrcInd = grid->Count() - mNumColumns;
        recycleIndStep = 1;
        fadeIndStep = -1;
        grid->ChildSize(recycleDstInd, size);
        layoutMovement.iY = -size.iHeight;
        recycle = !mVerticalOffset;
        mVerticalOffset = false;

        }
    else if (aEventID == EEventScrollDown) // Down
        {
        firstNewChildPos = mScrollOffset + TPoint(mHorizontalOffset ? -1 : 0, mNumRows - 1);
        childPosStep = TPoint(1, 0);
        mScrollOffset.iY++;
        numNewItems = mNumColumns;
        recycleDstInd = grid->Count()- 1;
        fadeInInd = grid->Count() - mNumColumns;
        fadeIndStep = 1;
        grid->ChildSize(recycleDstInd, size);
        layoutMovement.iY = size.iHeight;

        recycle = mVerticalOffset;
        mVerticalOffset = true;


        }
    else if (aEventID == EEventScrollLeft) // Left
        {
        firstNewChildPos = mScrollOffset + TPoint(-1, mVerticalOffset ? -1 : 0);
        childPosStep = TPoint(0, 1);
        mScrollOffset.iX--;
        numNewItems = mNumRows;
        fadeOutInd = recycleSrcInd = mNumColumns - 1;
        fadeIndStep = recycleIndStep = mNumColumns;
        grid->ChildSize(recycleDstInd, size);
        layoutMovement.iX = -size.iWidth;
        recycle = !mHorizontalOffset;
        mHorizontalOffset = false;

        }

    else if (aEventID == EEventScrollRight) // Right
        {
        firstNewChildPos = mScrollOffset + TPoint(mNumColumns - 1, mVerticalOffset ? -1 : 0);
        childPosStep = TPoint(0, 1);
        mScrollOffset.iX++;
        numNewItems = mNumRows;
        fadeInInd = recycleDstInd = mNumColumns - 1;
        fadeIndStep = recycleIndStep = mNumColumns;
        grid->ChildSize(recycleDstInd, size);
        layoutMovement.iX = size.iWidth;
        recycle = mHorizontalOffset;
        mHorizontalOffset = true;
        }

    IAlfVariantType *recyclingGridData = aControl.elementData(mOwner, 0);
    if ( recyclingGridData->type()!=  IAlfVariantType::EBranch)
        {
        return ;
        }

    aControl.Env().PauseRefresh();

    // Perform scrolling
    if (aEventID == EEventScrollPageDown || aEventID == EEventScrollPageUp || aEventID == EEventScrollUp || aEventID == EEventScrollDown ||aEventID == EEventScrollLeft || aEventID == EEventScrollRight)
        {
        if (recycle)
            {
            // Move the layout
            grid->SetPos(grid->Pos().Target() + layoutMovement);
            // Recycle visual trees
            for (int i = 0; i < numNewItems; ++i)
                {
                CAlfVisual &visual = grid->Visual(recycleSrcInd + i * recycleIndStep);
                grid->Reorder(visual, recycleDstInd + i * recycleIndStep);
                TPoint childPos = firstNewChildPos + TPoint(i * childPosStep.iX, i * childPosStep.iY);
                int childInd = calculateChildInd(childPos);
                if (childInd >= 0 && childInd < recyclingGridData->branch()->childrenCount())
                    {
                    IAlfVariantType *recyclingGridChildData = recyclingGridData->branch()->childData(childInd);

                    const UString& childName  = recyclingGridData->branch()->childName(childInd);
                    IAlfElement *childElement = mOwner.control().findElement(childName.getUtf8());
                    if (childElement!=NULL)
                        {
                        int newDataID= mOwner.control().elementDataId(*childElement,childInd, 0);
                        childElement->replaceVisualTree(
                            *recyclingGridChildData,newDataID ,childElement->dataID(visual));
                        }
                    }
                }
            }
        else
            {
            scroll->SetPos(scroll->Pos().Target() - layoutMovement, mScrollingTime);
            }
        // Fade in / out
        for (int i = 0; i < numNewItems; ++i)
            {
            // Fade out the old items
            CAlfVisual &oldItem = grid->Visual(fadeOutInd + i * fadeIndStep);
            if ( mFadeOutDelay == 0 )
                {
                TAlfTimedValue zero;
                zero.SetValueNow(1.f);
                zero.SetTarget(0.f, mFadeOutTime);
                oldItem.SetOpacity(zero);
                }
            else
                {
                TAlfValueCommand cmd((void*)&oldItem, EAlfValueCommandVisualSetOpacity, 0.f, mFadeOutTime);
                aControl.Env().Send(cmd, mFadeOutDelay);
                }
            // Fade in the new items
            CAlfVisual &newItem = grid->Visual(fadeInInd + i * fadeIndStep);
            TAlfTimedValue zero2;
            zero2.SetValueNow(0.f);
            zero2.SetTarget(0.f, 0);
            newItem.SetOpacity(zero2);
            TPoint childPos = firstNewChildPos + TPoint(i * childPosStep.iX, i * childPosStep.iY);
            int childInd = calculateChildInd(childPos);
            IAlfVariantType *recyclingGridChildData = recyclingGridData->branch()->childData(childInd);
            if (childInd >= 0 && recyclingGridChildData!=NULL)
                {
                if ( mFadeInDelay == 0 )
                    {
                    TAlfTimedValue one;
                    one.SetValueNow(0.f);
                    one.SetTarget(1.f, mFadeInTime);
                    newItem.SetOpacity(one);
                    }
                else
                    {
                    TAlfValueCommand cmd((void*)&newItem, EAlfValueCommandVisualSetOpacity, 1.f, mFadeInTime);
                    aControl.Env().Send(cmd, mFadeInDelay);
                    }
                }
            else
                {
                newItem.SetOpacity( TAlfTimedValue(0) );
                }
            }
        // Animate layout scrolling
        grid->SetPos(grid->Pos().Target() - layoutMovement, mScrollingTime);
        }

    aControl.Env().ContinueRefresh();
    }

// ---------------------------------------------------------------------------
// OfferEventL - Handle the Event.
// ?implementation_description
// ---------------------------------------------------------------------------
bool AlfRecyclingGridElementImpl::moveSingleRowOrColumn( CAlfWidgetControl& aControl, bool aPageUp)
    {
    uint aEventID;

    if (!aPageUp)
        {
        aEventID = (mFillStyle == AlfRecyclingGridElement::EColumnsFirst ? EEventScrollDown : EEventScrollRight);
        }
    else
        {
        aEventID = (mFillStyle == AlfRecyclingGridElement::EColumnsFirst ? EEventScrollUp : EEventScrollLeft);
        }

    doPositioningAndScrolling(aControl,aEventID);
    return true;
    }


bool AlfRecyclingGridElementImpl::accept( CAlfWidgetControl& /*aControl*/, const TAlfEvent& aEvent ) const
    {

    uint aEventID;
    if (!aEvent.IsCustomEvent())
        {
        return false;
        }
    aEventID = aEvent.CustomParameter();
    if ( aEventID == EEventScrollPrimaryNext || aEventID == EEventScrollPrimaryPrev ||
            aEventID == EEventScrollSecondaryNext || aEventID == EEventScrollSecondaryPrev ||
            aEventID == EEventScrollToFirst || aEventID == EEventScrollToLast ||
            aEventID == EEventScrollUp || aEventID == EEventScrollDown ||
            aEventID == EEventScrollLeft || aEventID == EEventScrollRight ||
            aEventID == EEventScrollPageDown || aEventID == EEventScrollPageUp)
        {
        return true;
        }

    return false;
    }


void AlfRecyclingGridElementImpl::createChildVisualTree(
    IAlfElement* aElement, IAlfVariantType& aChildData,
    IAlfBranch& aData, int aIndex, uint aDataID )
    {
    int firstVisibleInd=0, lastVisibleInd=0;
    int firstVisibleDataId=0, lastVisibleDataId=0;
    CAlfLayout *main = (CAlfLayout *)mOwner.findVisual(0);
    CAlfGridLayout *grid = (CAlfGridLayout *)&main->Visual(0);

    if (grid->Count() > 0)
        {
        CAlfVisual &first  = grid->Visual( 0 );
        CAlfVisual &last   = grid->Visual( grid->Count() - 1 );

        firstVisibleDataId= aElement->dataID(first);
        lastVisibleDataId= aElement->dataID(last);

        firstVisibleInd= mOwner.control().dataIdToIndex(*aElement, firstVisibleDataId);
        lastVisibleInd= mOwner.control().dataIdToIndex(*aElement, lastVisibleDataId);

        // Replace the visual tree
        IAlfElement* child = mOwner.control().findElement( aData.branch()->childName(0).getUtf8());


        if (aData.childrenCount()>= mNumColumns*mNumRows)
            {
            // Check that the child index is in the visible area
            if ( aIndex >= firstVisibleInd && aIndex <= lastVisibleInd )
                {
                CAlfVisual &last   = grid->Visual( grid->Count()-1 );

                // Move the last visual to the right position
                grid->Reorder( last, aIndex - firstVisibleInd);


                // Update data IDs
                child->updateDataIDs( aData, aIndex, 1, aDataID, aDataID );

                int oldElementDataID = aElement->dataID( last );
                int newElementDataID = mOwner.control().elementDataId( *aElement, aIndex - firstVisibleInd, aDataID );

                child->replaceVisualTree( aChildData, newElementDataID ,oldElementDataID );
                }
            }
        else
            {
            int newElementDataID = mOwner.control().elementDataId( *aElement, aIndex - firstVisibleInd, aDataID );

            //Update data IDs
            child->updateDataIDs( aData, aIndex, 1, aDataID, aDataID );

            //CAlfVisual* newItem = child->createVisualTree(aChildData, aData,aData.childrenCount(), aDataID, static_cast<CAlfLayout*>(grid ));
            CAlfVisual* newItem = child->createVisualTree(aChildData, newElementDataID,static_cast<CAlfLayout*>(grid ),grid->Count()-1);

            // Move the last visual to the right position
            grid->Reorder( *newItem, aIndex - firstVisibleInd);
            }
        }
    else
        {
        aElement->createVisualTree( aChildData, aData, aIndex, aDataID, grid, aIndex );
        }
    }

void AlfRecyclingGridElementImpl::removeChildVisualTree(
    IAlfElement* aElement, IAlfBranch& aData, int aIndex, uint aDataID )
    {
    int firstVisibleInd=0, lastVisibleInd=0;
    int firstVisibleDataId=0, lastVisibleDataId=0;
    CAlfLayout *main = (CAlfLayout *)mOwner.findVisual(0);
    CAlfGridLayout *grid = (CAlfGridLayout *)&main->Visual(0);

    CAlfVisual &first  = grid->Visual( 0 );

    if (aData.childrenCount()-1 > (grid->Count() - 1) )
        {
        CAlfVisual &last   = grid->Visual( grid->Count() - 1 );
        firstVisibleDataId= aElement->dataID(first);
        lastVisibleDataId= aElement->dataID(last);

        firstVisibleInd= mOwner.control().dataIdToIndex(*aElement, firstVisibleDataId);
        lastVisibleInd= mOwner.control().dataIdToIndex(*aElement, lastVisibleDataId);

        /*Check that the child index is in the visible area*/
        if ( aIndex >= firstVisibleInd && aIndex <= lastVisibleInd )
            {
            // Find the visual to be removed - it's used to show the next visual tree below the visible area
            CAlfVisual &removed = grid->Visual( aIndex  - firstVisibleInd );

            // Move it to the end of the grid
            grid->Reorder( removed, grid->Count() - 1 );


            // Replace the visual tree
            IAlfElement* child = mOwner.control().findElement( aData.branch()->childName(0).getUtf8());

            IAlfVariantType* data =NULL;
            data = aData.childData( lastVisibleInd + 1 );

            int oldElementDataID = aElement->dataID( removed );

            int newElementDataID ;
            newElementDataID = mOwner.control().elementDataId( *aElement, lastVisibleInd + 1, aDataID );

            if (data!=NULL)
                {
                child->replaceVisualTree(*data, newElementDataID, oldElementDataID);
                }

            if (aData.childrenCount()-1 <= (grid->Count() - 1 ))
                {
                child->removeVisualTree(aData, aData.childrenCount()-1,aDataID);
                }

            // Update data IDs
            child->updateDataIDs( aData, aIndex, -1, aDataID, aDataID );
            }
        }
    else
        {
        CAlfVisual &last   = grid->Visual(aData.childrenCount()-1);
        firstVisibleDataId= aElement->dataID(first);
        lastVisibleDataId= aElement->dataID(last);

        firstVisibleInd= mOwner.control().dataIdToIndex(*aElement, firstVisibleDataId);
        lastVisibleInd= mOwner.control().dataIdToIndex(*aElement, lastVisibleDataId);


        /*Check that the child index is in the visible area*/
        if ( aIndex >= firstVisibleInd && aIndex <= lastVisibleInd )
            {
            // Find the visual to be removed - it's used to show the next visual tree below the visible area
            CAlfVisual &removed = grid->Visual( aIndex  - firstVisibleInd );

            grid->Reorder( removed, aData.childrenCount()-1);

            // Replace the visual tree
            IAlfElement* child = mOwner.control().findElement( aData.branch()->childName(0).getUtf8());

            IAlfVariantType* data =NULL;
            data = aData.childData( aData.childrenCount() -1 );

            // Replace the visual tree

            CAlfVisual &last   = grid->Visual( aData.childrenCount()-1 );

            int oldElementDataID = aElement->dataID( removed );

            int newElementDataID ;
            newElementDataID = mOwner.control().elementDataId( *aElement, aData.childrenCount()-1, aDataID );

            if (data!=NULL)
                {
                child->replaceVisualTree(*data, newElementDataID, oldElementDataID);
                }

            if (aData.childrenCount()-1 <= (grid->Count() - 1 ))
                {
                child->removeVisualTree(aData, aData.childrenCount()-1,aDataID);
                }

            // Update data IDs
            child->updateDataIDs( aData, aIndex, -1, aDataID, aDataID );
            }
        }
    }
// ---------------------------------------------------------------------------
// Description: Do not destroy visuals, because it's done elsewhere.
// ---------------------------------------------------------------------------
void AlfRecyclingGridElementImpl::removeAndDestroyVisuals( int /*aTimeMilliseconds*/ )
    {
    }

// End of File.
