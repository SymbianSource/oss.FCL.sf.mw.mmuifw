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
* Description:  flowlayoutmanager implementation
*
*/


#include <alf/alfexceptions.h>
#include <alf/alfwidgetcontrol.h>
#include <alf/alfflowlayoutmanager.h>
#include <osn/ustring.h>
#include <osn/osnnew.h>

#include "alfflowlayoutpolicyimpl.h"
#include "alfflowlayoutpreferencesimpl.h"

using osncore::UString;

using namespace Alf;

namespace Alf
    {

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// layout manager constructor
// ---------------------------------------------------------------------------
//
OSN_EXPORT AlfFlowLayoutManager::AlfFlowLayoutManager() 
    : AlfLayoutManager(EAlfLayoutTypeFlow)
    {
    mData.reset(new (EMM) AlfFlowLayoutPolicyImpl(*this));
    mPrefImpl.reset(new (EMM) AlfFlowLayoutPreferencesImpl(*this));
    }
        
// ---------------------------------------------------------------------------
// virtual destructor
// ---------------------------------------------------------------------------
//
OSN_EXPORT AlfFlowLayoutManager::~AlfFlowLayoutManager()
    {
    }
    
// ---------------------------------------------------------------------------
// from AlfLayoutManager
// notifies the layout manager, that the child control's layout
// must be updated.
// ---------------------------------------------------------------------------
//
OSN_EXPORT void AlfFlowLayoutManager::updateChildLayout(
    CAlfWidgetControl* aControl)
    {
    CAlfLayout& layout = getLayout();
    doUpdateChildLayout(aControl);
    layout.UpdateChildrenLayout();

    //doUpdateChildLayout changes the visual size and position.
    //need to notify the widget that size and position has changed. 
    //for aControl both the size and position might have changes.
    //Only the position changes for all the controls after aControl. 
    
    //set the size and postion for aControl
    TAlfRealRect rect;
    if (controlRect(*aControl, rect)) //returns the real rect (of the root visual)
        {
        setControlRect(*aControl, rect);
        }
    
    //update the position for controls after aControl in layout.
    const int childrenCount = count();
    bool childControlFound = false;
    
    for (int i = 0; i < childrenCount; i++)
        {
        CAlfWidgetControl* child = getControl(i);
        
        if (childControlFound && controlRect(*child, rect))
            {
            setControlPosition(*child, rect.iTl);
            }
        else if (child == aControl)
            {
            childControlFound = true;
            }
        }
    
    // Inform parent layout manager about the changed presentation size
    owner().updateParentLayout();
    }

// ---------------------------------------------------------------------------
// From class IAlfInterfaceBase.
// Getter for interfaces provided by the element.
// ---------------------------------------------------------------------------
//
OSN_EXPORT IAlfInterfaceBase* AlfFlowLayoutManager::makeInterface( 
    const IfId& aType )
    {
    UString param(aType.mImplementationId);
    IAlfInterfaceBase* ret = 0;
    
    ret = mData->makeInterface(aType);
    if (!ret)
        {
        ret = mPrefImpl->makeInterface(aType);
        }
    if (!ret)
        {
        ret = AlfLayoutManager::makeInterface(aType);
        }
        
    return ret;
    }
    
    } // Alf

