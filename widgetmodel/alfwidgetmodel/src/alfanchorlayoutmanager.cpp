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
* Description:  anchorlayoutmanager with focus handling in 1D (next/previous)
*
*/


#include <alf/alfanchorlayoutmanager.h>
#include <alf/alfexceptions.h>
#include <alf/alfwidgetcontrol.h>
#include <osn/ustring.h>
#include <osn/osnnew.h>

#include "alfanchorlayoutmanagerimpl.h"

using osncore::UString;

using namespace Alf;

namespace Alf
    {

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// layout manager constructor
// ---------------------------------------------------------------------------
//
OSN_EXPORT AlfAnchorLayoutManager::AlfAnchorLayoutManager() 
    : AlfLayoutManager(EAlfLayoutTypeAnchor)
    {
    mData.reset(new (EMM) AlfAnchorLayoutManagerImpl(*this));
    }
        
// ---------------------------------------------------------------------------
// virtual destructor
// ---------------------------------------------------------------------------
//
OSN_EXPORT AlfAnchorLayoutManager::~AlfAnchorLayoutManager()
    {
    }
    
// ---------------------------------------------------------------------------
// from AlfLayoutManager
// notifies the layout manager, that the child control's layout
// must be updated.
// ---------------------------------------------------------------------------
//
OSN_EXPORT void AlfAnchorLayoutManager::updateChildLayout(
    CAlfWidgetControl* aControl)
    {
    CAlfLayout& layout = getLayout();
    doUpdateChildLayout(aControl);
    layout.UpdateChildrenLayout();

    //doUpdateChildLayout changes the visual size and position.
    //need to notify the widget that size and position has changed.
    
    //set the size and postion for aControl
    TAlfRealRect rect;
    if (controlRect(*aControl, rect)) //returns the real rect (of the root visual)
        {
        setControlRect(*aControl, rect);
        }

    // Inform parent layout manager about the changed presentation size
    owner().updateParentLayout();
    }
        
// ---------------------------------------------------------------------------
// from IAlfLayoutManager
// notifies the layout manager, that the control's has been
// removed from the layout.
// ---------------------------------------------------------------------------
//
OSN_EXPORT void AlfAnchorLayoutManager::childRemoved(CAlfWidgetControl* aControl)
    {
    //when control is removed from layout, no change to other widget rects,
    // just call the base class
    AlfLayoutManager::childRemoved(aControl);
    }
    
// ---------------------------------------------------------------------------
// from AlfLayoutManager
// notifies the layout manager, that the child control's layout
// must be updated.
// ---------------------------------------------------------------------------
//    
OSN_EXPORT void AlfAnchorLayoutManager::doUpdateChildLayout(
    CAlfWidgetControl* /*aControl*/)
    {
    //anchorlayout changes the position and size, nothing to do here.
    }

// ---------------------------------------------------------------------------
// From class IAlfInterfaceBase.
// Getter for interfaces provided by the element.
// ---------------------------------------------------------------------------
//
OSN_EXPORT IAlfInterfaceBase* AlfAnchorLayoutManager::makeInterface( 
    const IfId& aType )
    {
    UString param(aType.mImplementationId);
    if (param == IAlfLayoutManager::type().mImplementationId)
        {
        return this;
        }      

    // Let the implementation class try the interface creation
    IAlfInterfaceBase* pInterface = mData->makeInterface(aType);
    return pInterface;
    }
 } // Alf
