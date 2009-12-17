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
#include <alf/alfanchorlayout.h>
#include "alf/alfwidget.h"
#include <alf/alfvisualfactory.h>
#include <alf/alfexceptions.h>
#include <alf/ialflayoutpreferences.h>
#include <osn/osnnew.h>

//stl
#include <algorithm> //for min & max

#include "alfanchorlayoutmanagerimpl.h"

using namespace Alf;

namespace Alf
    {
// ======== MEMBER FUNCTIONS ========
    
// ---------------------------------------------------------------------------
// layoutmanagerimpl constructor
// ---------------------------------------------------------------------------
//    
AlfAnchorLayoutManagerImpl::AlfAnchorLayoutManagerImpl(
    AlfAnchorLayoutManager& aAnchorLayoutManager) : 
    mAnchorLayoutManager(aAnchorLayoutManager)
    {
    }
    
// ---------------------------------------------------------------------------
// creates the layout used by the layoutmanager.
// ---------------------------------------------------------------------------
//    
AlfAnchorLayoutManagerImpl::~AlfAnchorLayoutManagerImpl()
    {
    }

// ---------------------------------------------------------------------------
// from IAlfLayoutPreferences
// Returns the minimum area that this layout manager can occupy by observing
// the minimum sizes of the child UI elements in the layout
// ---------------------------------------------------------------------------
//
bool AlfAnchorLayoutManagerImpl::getMinimumSize( TAlfXYMetric& /*aMinSize*/ ) const
	{
	return false;
	}

// ---------------------------------------------------------------------------
// from IAlfLayoutPreferences
// Returns the maximum area that this layout manager can occupy by observing
// the maximum sizes of the child UI elements in the layout
// ---------------------------------------------------------------------------
//
bool AlfAnchorLayoutManagerImpl::getMaximumSize( TAlfXYMetric& /*aMaxSize*/ ) const
	{
	return false;
	}

// ---------------------------------------------------------------------------
// Combines and returns the preferred sizes of all child UI elements according
// to the layouting rules.
// ---------------------------------------------------------------------------
//
bool AlfAnchorLayoutManagerImpl::getPreferredSize(TAlfXYMetric& /*aPreferredSize*/) const
	{
	return false;
	}
 
// ---------------------------------------------------------------------------
// from IAlfLayoutPreferences
// At the moment doesn't do anything since preferred size is being queried
// from the child UI elements added to this layout manager.
// ---------------------------------------------------------------------------
//
void AlfAnchorLayoutManagerImpl::setPreferredSize( const TAlfXYMetric& aPreferredSize)
	{
	TAlfTimedPoint size((float)aPreferredSize.iX.iMagnitude,(float)aPreferredSize.iY.iMagnitude);
	layout().SetSize(size);
	mAnchorLayoutManager.owner().updateParentLayout();
	}

// ---------------------------------------------------------------------------
// From class IAlfInterfaceBase.
// Getter for interfaces provided by the element.
// ---------------------------------------------------------------------------
//
IAlfInterfaceBase* AlfAnchorLayoutManagerImpl::makeInterface(const IfId& aType)
    {
    UString param(aType.mImplementationId);
    if (param == IAlfLayoutPreferences::type().mImplementationId)
        {
        return this;
        }      
    
    return NULL;
    }
    
// ---------------------------------------------------------------------------
// returns the anchorlayout used by the layoutmanager
// ---------------------------------------------------------------------------
//
CAlfAnchorLayout& AlfAnchorLayoutManagerImpl::layout() const
    {
    return dynamic_cast<CAlfAnchorLayout&>(mAnchorLayoutManager.getLayout());
    }

 } // Alf
