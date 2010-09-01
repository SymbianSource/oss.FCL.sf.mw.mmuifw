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
* Description:  layoutmanager implementation class with focus handling in 1D (next/previous)
*
*/


#include <alf/ialfwidgetcontrol.h>
#include <alf/alfvisual.h>
#include <alf/alfgridlayout.h>
#include "alf/alfwidget.h"
#include <alf/alfvisualfactory.h>
#include <alf/alfexceptions.h>
#include <alf/ialflayoutpreferences.h>
#include <osn/osnnew.h>

//stl
#include <algorithm> //for min & max

#include "alfgridlayoutmanagerimpl.h"

using namespace Alf;

namespace Alf
    {
// ======== MEMBER FUNCTIONS ========
    
// ---------------------------------------------------------------------------
// layoutmanagerimpl constructor
// ---------------------------------------------------------------------------
//    
AlfGridLayoutManagerImpl::AlfGridLayoutManagerImpl(
    AlfGridLayoutManager& aGridLayoutManager) : 
    mGridLayoutManager(aGridLayoutManager)
    {
    }
    
// ---------------------------------------------------------------------------
// creates the layout used by the layoutmanager.
// ---------------------------------------------------------------------------
//    
AlfGridLayoutManagerImpl::~AlfGridLayoutManagerImpl()
    {
    }

// ---------------------------------------------------------------------------
// from IAlfLayoutPreferences
// Returns the minimum area that this layout manager can occupy by observing
// the minimum sizes of the child UI elements in the layout
// ---------------------------------------------------------------------------
//
bool AlfGridLayoutManagerImpl::getMinimumSize( TAlfXYMetric& /*aMinSize*/ ) const
	{
	return false;
	}

// ---------------------------------------------------------------------------
// from IAlfLayoutPreferences
// Returns the maximum area that this layout manager can occupy by observing
// the maximum sizes of the child UI elements in the layout
// ---------------------------------------------------------------------------
//
bool AlfGridLayoutManagerImpl::getMaximumSize( TAlfXYMetric& /*aMaxSize*/ ) const
	{
	return false;
	}

// ---------------------------------------------------------------------------
// Combines and returns the preferred sizes of all child UI elements according
// to the layouting rules.
// ---------------------------------------------------------------------------
//
bool AlfGridLayoutManagerImpl::getPreferredSize(TAlfXYMetric& aPreferredSize) const
	{	
	bool hasPreferredSize = false;
	TAlfXYMetric resultSize = TAlfXYMetric(TAlfMetric(0), TAlfMetric(0));
		
    const CAlfGridLayout& gridlayout = layout();
    const int childCount = mGridLayoutManager.count();
    const int gridColCount = gridlayout.ColumnCount();
    
    //iterate the columns to calculate the preferred width.
    const int colCount = std::min<int>(gridColCount, childCount);
    for (int i = 0; i < colCount; i++)
        {
        CAlfWidgetControl* childWidgetControl = mGridLayoutManager.getControl(i);
        //getControl may return null, but getLayoutPreferences checks that.
        const IAlfLayoutPreferences* layoutPrefs = 
            mGridLayoutManager.getLayoutPreferences(childWidgetControl);        
        
        if (layoutPrefs)
            {
            TAlfXYMetric prefSize;
            if (layoutPrefs->getPreferredSize(prefSize))
                {
                if(prefSize.iX.iUnit == EAlfUnitPixel)
                    {
                    resultSize.iX.iMagnitude += prefSize.iX.iMagnitude;
                    hasPreferredSize = true;
                    }
                }
            }
        }
    
    //iterate the rows to calculate the preferred height  
    if (hasPreferredSize)
        {
        //count rows, that have at least one visual.
        int usedRows = childCount / gridColCount;
        if (childCount % gridColCount)
            {
            //last wor not filled completely, but still valid to get preferred height
            //for this row also.
            usedRows++;
            }
            
        const int rowCount = std::min<int>(gridlayout.RowCount(), usedRows);
        for (int i = 0; i < rowCount; i+= gridColCount)
            {
            CAlfWidgetControl* childWidgetControl = mGridLayoutManager.getControl(i);
            //getControl may return null, but getLayoutPreferences checks that.
            const IAlfLayoutPreferences* layoutPrefs = 
                mGridLayoutManager.getLayoutPreferences(childWidgetControl);        
            
            if (layoutPrefs)
                {
                TAlfXYMetric prefSize;
                if (layoutPrefs->getPreferredSize(prefSize))
                    {
                    if(prefSize.iY.iUnit == EAlfUnitPixel)
                        {
                        resultSize.iY.iMagnitude += prefSize.iY.iMagnitude;
                        hasPreferredSize = true;
                        }
                    }
                }
            }
        }
        
    aPreferredSize = resultSize;
    return hasPreferredSize;
	}
 
// ---------------------------------------------------------------------------
// from IAlfLayoutPreferences
// At the moment doesn't do anything since preferred size is being queried
// from the child UI elements added to this layout manager.
// ---------------------------------------------------------------------------
//
void AlfGridLayoutManagerImpl::setPreferredSize( const TAlfXYMetric& aPreferredSize)
	{
	TAlfTimedPoint size((float)aPreferredSize.iX.iMagnitude,(float)aPreferredSize.iY.iMagnitude);
	layout().SetSize(size);
	mGridLayoutManager.owner().updateParentLayout();
	}

// ---------------------------------------------------------------------------
// From class IAlfInterfaceBase.
// Getter for interfaces provided by the element.
// ---------------------------------------------------------------------------
//
IAlfInterfaceBase* AlfGridLayoutManagerImpl::makeInterface(const IfId& aType)
    {
    UString param(aType.mImplementationId);
    if (param == IAlfLayoutPreferences::type().mImplementationId)
        {
        return this;
        }      
    
    return NULL;
    }
    
// ---------------------------------------------------------------------------
// returns the gridlayout used by the layoutmanager
// ---------------------------------------------------------------------------
//
CAlfGridLayout& AlfGridLayoutManagerImpl::layout() const
    {
    return dynamic_cast<CAlfGridLayout&>(mGridLayoutManager.getLayout());
    }

 } // Alf
