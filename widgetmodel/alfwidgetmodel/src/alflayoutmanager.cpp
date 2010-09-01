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
* Description:  layoutmanager with focus handling in 1D (next/previous)
*
*/


#include <alf/alfenv.h>
#include <alf/alflayout.h>
#include <alf/alflayoutmanager.h>
#include <alf/alfexceptions.h>
#include <alf/alfwidgetcontrol.h>
#include <alf/ialfwidgetfactory.h>
#include <alf/alfwidgetenvextension.h>
#include <osn/ustring.h>
#include <osn/osnnew.h>
#include "alf/alfwidget.h"
#include "alflayoutmanagerimpl.h"

using osncore::UString;

using namespace Alf;

namespace Alf
    {

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// layout manager constructor
// ---------------------------------------------------------------------------
//
OSN_EXPORT AlfLayoutManager::AlfLayoutManager(TAlfLayoutType aLayoutType)
    {
    mData.reset(new (EMM) AlfLayoutManagerImpl(aLayoutType));
    }
        
// ---------------------------------------------------------------------------
// virtual destructor
// ---------------------------------------------------------------------------
//
OSN_EXPORT AlfLayoutManager::~AlfLayoutManager()
    {
    }
    
// ---------------------------------------------------------------------------
// returns the owner of the layoutmanager
// ---------------------------------------------------------------------------
//    
OSN_EXPORT CAlfWidgetControl& AlfLayoutManager::owner() const
    {
    return mData->owner();
    }
    
// ---------------------------------------------------------------------------
// returns the widget at index aIndex.
// ---------------------------------------------------------------------------
//    
OSN_EXPORT CAlfWidgetControl* AlfLayoutManager::getControl(int aIndex) const
    {
    return mData->getControl(aIndex);
    }
    
// ---------------------------------------------------------------------------
// returns the count of controls in the layoutmanager.
// ---------------------------------------------------------------------------
//    
OSN_EXPORT int AlfLayoutManager::count() const
    {
    return mData->count();
    }    
    
// ---------------------------------------------------------------------------
// from IAlfLayoutManager
// creates the layout used by this layoutmanager.
// ---------------------------------------------------------------------------
//
OSN_EXPORT void AlfLayoutManager::createLayout(CAlfWidgetControl& aOwner, 
    CAlfLayout* aParentLayout, int aLayoutIndex)
    {
	mData->createLayout(aOwner, aParentLayout, aLayoutIndex);
    updateChildrenLayout();
    }
    
// ---------------------------------------------------------------------------
// from IAlfLayoutManager
// returns the layout used by this layoutmanager.
// ---------------------------------------------------------------------------
//
OSN_EXPORT CAlfLayout& AlfLayoutManager::getLayout() const
    {
    return mData->getLayout();
    }
    
// ---------------------------------------------------------------------------
// from IAlfLayoutManager
// notifies the layout manager, that the child control's layout
// must be updated.
// ---------------------------------------------------------------------------
//    
OSN_EXPORT void AlfLayoutManager::updateChildLayout(
    CAlfWidgetControl* aControl)
    {
    mData->getLayout(); //checks, that the layout exists, throws exception if not. 
    doUpdateChildLayout( aControl );
    }
    
// ---------------------------------------------------------------------------
// from IAlfLayoutManager
// notifies the layout manager, that all the child controls' layouts
// must be updated.
// ---------------------------------------------------------------------------
//
OSN_EXPORT void AlfLayoutManager::updateChildrenLayout()
    {
    CAlfLayout& layout = mData->getLayout();
    mData->updateMainLayout();
    
  //  CAlfWidgetControl& container = mData->owner();
    const int childrenCount = count();
    
    // go through all the child controls, and update the layout
    for (int i = 0; i < childrenCount; i++)
        {
        doUpdateChildLayout(getControl(i));
        }
    
    layout.UpdateChildrenLayout();
    
    //doUpdateChildLayout changes the visual size and position.
    //need to notify all the widgets that size and position might have changed. 
    mData->updateAllWidgetRects();
    
    // Inform parent layout manager about the changed presentation size
    owner().updateParentLayout();
    }
    
// ---------------------------------------------------------------------------
// from IAlfLayoutManager
// notifies the layout manager, that the control's has been
// removed from the layout.
// ---------------------------------------------------------------------------
//
OSN_EXPORT void AlfLayoutManager::childRemoved(CAlfWidgetControl* aControl)
    {
    mData->childRemoved(aControl);
    }
   
// ---------------------------------------------------------------------------
// notifies the layout manager, that the child control's layout
// must be updated.
// ---------------------------------------------------------------------------
//
OSN_EXPORT void AlfLayoutManager::doUpdateChildLayout(
    CAlfWidgetControl* aControl)
    {
    mData->doUpdateChildLayout(aControl);
    }

// ---------------------------------------------------------------------------
// returns the size of the widget.
// ---------------------------------------------------------------------------
//    
OSN_EXPORT bool AlfLayoutManager::controlRect(
    CAlfWidgetControl& aControl, TAlfRealRect& aRect)
    {
    return mData->controlRect(aControl, aRect);
    }

// ---------------------------------------------------------------------------
// sets the size and position of the widget.
// ---------------------------------------------------------------------------
//
OSN_EXPORT void AlfLayoutManager::setControlRect(
    CAlfWidgetControl& aControl, const TAlfRealRect &aRect)
    {
    AlfWidget* widget = aControl.widget();
    if (widget)
        {
        const char* name = widget->widgetName(); 
        IAlfWidget* ownerwidget = AlfWidgetEnvExtension::widgetFactory(aControl.Env()).findWidget(name); 
        if (ownerwidget)
            {
            mData->setWidgetRect(*ownerwidget, aRect);
            }
        }
    }
    
// ---------------------------------------------------------------------------
// sets the position of the widget.
// ---------------------------------------------------------------------------
//
OSN_EXPORT void AlfLayoutManager::setControlPosition(
    CAlfWidgetControl& aControl, const TAlfRealPoint& aPos)
    {
    AlfWidget* widget = aControl.widget();
    if (widget)
        {
        const char* name = widget->widgetName(); 
        IAlfWidget* ownerwidget = AlfWidgetEnvExtension::widgetFactory(aControl.Env()).findWidget(name); 
        if (ownerwidget)
            {
            mData->setWidgetPosition(*ownerwidget, aPos);
            }
        }
    }
    
// ---------------------------------------------------------------------------
// sets the size of the widget.
// ---------------------------------------------------------------------------
//
OSN_EXPORT void AlfLayoutManager::setControlSize(
    CAlfWidgetControl& aControl, const TAlfRealPoint &aSize)
    {
    AlfWidget* widget = aControl.widget();
    if (widget)
        {
        const char* name = widget->widgetName(); 
        IAlfWidget* ownerwidget = AlfWidgetEnvExtension::widgetFactory(aControl.Env()).findWidget(name); 
        if (ownerwidget)
            {
            mData->setWidgetSize(*ownerwidget, aSize);
            }
        }
    }
    
// ---------------------------------------------------------------------------
// returns layout preferences for aControl
// ---------------------------------------------------------------------------
//    
OSN_EXPORT const IAlfLayoutPreferences* AlfLayoutManager::getLayoutPreferences(
    CAlfWidgetControl* aControl) const
    {
    return mData->getLayoutPreferences(aControl);
    }

// ---------------------------------------------------------------------------
// From class IAlfInterfaceBase.
// Getter for interfaces provided by the element.
// ---------------------------------------------------------------------------
//
OSN_EXPORT IAlfInterfaceBase* AlfLayoutManager::makeInterface( 
    const IfId& aType )
    {
    UString param(aType.mImplementationId);
    if (param == IAlfLayoutManager::type().mImplementationId)
        {
        return static_cast<IAlfLayoutManager*>(this);
        }
        

    // Let the implementation class try the interface creation
    return mData->makeInterface(aType);
    }
 } // Alf
