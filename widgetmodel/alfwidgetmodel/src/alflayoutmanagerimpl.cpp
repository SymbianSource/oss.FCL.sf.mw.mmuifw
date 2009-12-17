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
#include "alf/ialfattributeowner.h"
#include "alf/alfattribute.h"
#include <alf/alfvisual.h>
#include <alf/alfenv.h>
#include "alf/alfwidget.h"
#include <alf/alfvisualfactory.h>
#include <alf/alfexceptions.h>
#include <alf/ialflayoutpreferences.h>
#include <alf/alfwidgetenvextension.h>
#include <alf/ialfwidgetfactory.h>
#include <osn/osnnew.h>
#include <libc/assert.h>
#include <alf/attrproperty.h>
#include "alflayoutmanagerimpl.h"



namespace Alf
    {

//ifdef to prevent compiler warning: className not used.
#ifdef ALF_DEBUG_EXCEPTIONS
static const char* const className = "AlfLayoutManagerImpl";
#endif

// ======== MEMBER FUNCTIONS ========
    
// ---------------------------------------------------------------------------
// layoutmanagerimpl constructor
// ---------------------------------------------------------------------------
//    
AlfLayoutManagerImpl::AlfLayoutManagerImpl(
    TAlfLayoutType aLayoutType)
    {
    mLayoutType = aLayoutType;
    }
    
// ---------------------------------------------------------------------------
// creates the layout used by the layoutmanager.
// ---------------------------------------------------------------------------
//    
AlfLayoutManagerImpl::~AlfLayoutManagerImpl()
    {
    if (mLayout.get())
        {
        CAlfLayout* layout = mLayout.release();
        layout->RemoveAndDestroyAllD();
        }
    }
    
// ---------------------------------------------------------------------------
// creates the layout used by the layoutmanager.
// ---------------------------------------------------------------------------
//    
void AlfLayoutManagerImpl::createLayout(CAlfWidgetControl& aOwner, 
    CAlfLayout* aParentLayout, int aLayoutIndex)
    {
            
    //check parameter validity.
    if (aParentLayout && 
        (aLayoutIndex < 0 ||aLayoutIndex > aParentLayout->Count()))
        {
        ALF_THROW(AlfException, EInvalidArrayIndex, className);
        }
        
    //create layout
    CAlfLayout* layout = 0;
    TRAPD(err, layout = aOwner.AppendLayoutL(mLayoutType, aParentLayout));
        
	if(!layout || err != KErrNone)
	    {
	    ALF_THROW(AlfVisualException, ECanNotCreateVisual, className);
	    }

    //reorder, if needed.
    if (aParentLayout && aLayoutIndex != aParentLayout->Count() - 1)
        {
        aParentLayout->Reorder(*layout, aLayoutIndex);
        }
        
    mLayout.reset(layout);
    }
    
// ---------------------------------------------------------------------------
// returns the layout used by the layoutmanager.
// ---------------------------------------------------------------------------
//    
CAlfLayout& AlfLayoutManagerImpl::getLayout() const
    {
    checkLayout();
    return *mLayout.get();
    }
    
// ---------------------------------------------------------------------------
// updates the main layout
// ---------------------------------------------------------------------------
//
void AlfLayoutManagerImpl::updateMainLayout()
    {
    checkLayout();
    
    const IAlfLayoutPreferences* layoutPrefs = 
        getLayoutPreferences(getControl(*mLayout.get()));
    if (layoutPrefs)
        {
        TAlfXYMetric prefSize;
        if (layoutPrefs->getPreferredSize(prefSize))
            {
            TAlfRealPoint p(prefSize.iX.iMagnitude, 
                            prefSize.iY.iMagnitude);
            mLayout->SetSize(p);
            }
        }
    }
    
// ---------------------------------------------------------------------------
// notifies the layout manager, that the control's has been
// removed from the layout.
// ---------------------------------------------------------------------------
//
void AlfLayoutManagerImpl::childRemoved(CAlfWidgetControl* /*aControl*/)
    {
    checkLayout();
    mLayout->UpdateChildrenLayout();
    }
    
// ---------------------------------------------------------------------------
// returns the owner control of the layoutmanager.
// ---------------------------------------------------------------------------
//
CAlfWidgetControl& AlfLayoutManagerImpl::owner() const
    {
    checkLayout();
    CAlfWidgetControl* widgetControl = 0;
    
    /* The owner is always a AlfWidgetControl.
       So a static_cast is safe. see AlfLayoutManagerImpl::createLayout() */ 
    widgetControl = static_cast<CAlfWidgetControl*>(
                                     &mLayout->Owner());
   
    return *widgetControl;
    }

// ---------------------------------------------------------------------------
// from IAlfLayoutPreferences
// Returns the minimum area that this layout manager can occupy by observing
// the minimum sizes of the child UI elements in the layout
// ---------------------------------------------------------------------------
//
bool AlfLayoutManagerImpl::getMinimumSize( TAlfXYMetric& /*aMinSize*/ ) const
	{
	return false;
	}

// ---------------------------------------------------------------------------
// from IAlfLayoutPreferences
// Returns the maximum area that this layout manager can occupy by observing
// the maximum sizes of the child UI elements in the layout
// ---------------------------------------------------------------------------
//
bool AlfLayoutManagerImpl::getMaximumSize( TAlfXYMetric& /*aMaxSize*/ ) const
	{
	return false;
	}

// ---------------------------------------------------------------------------
// Combines and returns the preferred sizes of all child UI elements according
// to the layouting rules.
// ---------------------------------------------------------------------------
//
bool AlfLayoutManagerImpl::getPreferredSize(TAlfXYMetric& /*aPreferredSize*/) const
	{
	return false;
	}
 
// ---------------------------------------------------------------------------
// from IAlfLayoutPreferences
// At the moment doesn't do anything since preferred size is being queried
// from the child UI elements added to this layout manager.
// ---------------------------------------------------------------------------
//
void AlfLayoutManagerImpl::setPreferredSize( const TAlfXYMetric& /*aPreferredSize*/ )
	{
	
	}

// ---------------------------------------------------------------------------
// From class IAlfInterfaceBase.
// Getter for interfaces provided by the element.
// ---------------------------------------------------------------------------
//
IAlfInterfaceBase* AlfLayoutManagerImpl::makeInterface(const IfId& aType)
    {
    UString param(aType.mImplementationId);
    if (param == IAlfLayoutPreferences::type().mImplementationId)
        {
        return static_cast<IAlfLayoutPreferences*>(this);
        }      
    
    return 0;
    }

// ---------------------------------------------------------------------------
// gets control at aIndex position in the layout.
// ---------------------------------------------------------------------------
//    
CAlfWidgetControl* AlfLayoutManagerImpl::getControl(int aIndex) const
    {
    CAlfWidgetControl* control = 0;
    CAlfLayout* layout = mLayout.get();
    if (layout)
        {
        CAlfVisual& visual = layout->Visual(aIndex);
        control = getControl(visual);
        }
    return control;
    }
    
// ---------------------------------------------------------------------------
// returns the control, which owns the visual
// ---------------------------------------------------------------------------
//     
void AlfLayoutManagerImpl::doUpdateChildLayout(CAlfWidgetControl* aControl)
    {
    const IAlfLayoutPreferences* layoutPrefs = 
        getLayoutPreferences(aControl);
        
    if (layoutPrefs)
        {
        TAlfXYMetric prefSize;
        if (layoutPrefs->getPreferredSize(prefSize))
            {
            CAlfVisual* v = findRootVisual(aControl);
            if (v)
                {
                v->SetSize(TAlfRealPoint(prefSize.iX.iMagnitude,
                prefSize.iY.iMagnitude));
                }
            }
        }
    }

// ---------------------------------------------------------------------------
// returns the size of the widget.
// ---------------------------------------------------------------------------
//
bool AlfLayoutManagerImpl::controlRect(CAlfWidgetControl& aControl, 
    TAlfRealRect& aRect)
    {
    CAlfVisual* v = findRootVisual(&aControl);
    if (v != 0)
        {
        TAlfRealPoint size(v->Size().Target());
        TAlfRealPoint pos(v->Pos().Target());
        aRect.iTl = pos;
        aRect.iBr.iX = pos.iX + size.iX;
        aRect.iBr.iY = pos.iY + size.iY;
        return true;
        }
        
    return false;
    }
    
// ---------------------------------------------------------------------------
// set the size and position to the widget.
// ---------------------------------------------------------------------------
//
void AlfLayoutManagerImpl::setWidgetRect(IAlfWidget& aWidget, const TAlfRealRect &aRect)
    {
    const UString widthName(layoutattributes::KWidth);
    const UString heightName(layoutattributes::KHeight);
    const UString posX(layoutattributes::KPositionX);
    const UString posY(layoutattributes::KPositionY);
    
    IAlfAttributeOwner* attrOwner = 
        IAlfInterfaceBase::makeInterface<IAlfAttributeOwner>(&aWidget);
    if (attrOwner)
        {
    	auto_ptr<AlfAttributeValueType> w(
    	    new (EMM) AlfAttributeValueType(aRect.Width()));
    	auto_ptr<AlfAttributeValueType> h(
    	    new (EMM) AlfAttributeValueType(aRect.Height()));
    	auto_ptr<AlfAttributeValueType> x(
    	    new (EMM) AlfAttributeValueType(aRect.iTl.iX));
    	auto_ptr<AlfAttributeValueType> y(
    	    new (EMM) AlfAttributeValueType(aRect.iTl.iY));
    	attrOwner->setAttribute(heightName, h.get());
    	attrOwner->setAttribute(widthName, w.get());
    	attrOwner->setAttribute(posX, x.get());
    	attrOwner->setAttribute(posY, y.get());
    	
    	h.release();
    	w.release();
    	x.release();
    	y.release();
        }
    }  
    
// ---------------------------------------------------------------------------
// set the position for the widget.
// ---------------------------------------------------------------------------
//
void AlfLayoutManagerImpl::setWidgetPosition(IAlfWidget& aWidget, 
    const TAlfRealPoint &aPos)
    {
    const UString posX(layoutattributes::KPositionX);
    const UString posY(layoutattributes::KPositionY);
    
    IAlfAttributeOwner* attrOwner = 
        IAlfInterfaceBase::makeInterface<IAlfAttributeOwner>(&aWidget);
    if (attrOwner)
        {
    	auto_ptr<AlfAttributeValueType> x(
    	    new (EMM) AlfAttributeValueType(aPos.iX));
    	auto_ptr<AlfAttributeValueType> y(
    	    new (EMM) AlfAttributeValueType(aPos.iY));
    	attrOwner->setAttribute(posX, x.get());
    	attrOwner->setAttribute(posY, y.get());
    	x.release();
    	y.release();
        }
    }
    
// ---------------------------------------------------------------------------
// set the size for the widget.
// ---------------------------------------------------------------------------
//
void AlfLayoutManagerImpl::setWidgetSize(IAlfWidget& aWidget, const TAlfRealPoint &aSize)
    {
    const UString widthName(layoutattributes::KWidth);
    const UString heightName(layoutattributes::KHeight);
    IAlfAttributeOwner* attrOwner = 
        IAlfInterfaceBase::makeInterface<IAlfAttributeOwner>(&aWidget);
    if (attrOwner)
        {
    	auto_ptr<AlfAttributeValueType> w(
    	    new (EMM) AlfAttributeValueType(aSize.iX));
    	auto_ptr<AlfAttributeValueType> h(
    	    new (EMM) AlfAttributeValueType(aSize.iY));
    	attrOwner->setAttribute(heightName, h.get());
    	attrOwner->setAttribute(widthName, w.get());
    	h.release();
    	w.release();
        }
    }
    
// ---------------------------------------------------------------------------
// updates widget size and position properties for all the children
// to correct sizes and positions of the root visuals.
// ---------------------------------------------------------------------------
//    
void AlfLayoutManagerImpl::updateAllWidgetRects()
    {
    const int childCount = mLayout->Count();
    TAlfRealRect rect;
    for (int i = 0; i < childCount; i++)
        {
        CAlfWidgetControl* control = getControl(i);
        controlRect(*control, rect); //returns the real rect(of the root visual)
        AlfWidget* widget = control->widget();
        if(widget)
            {
            const char* name = widget->widgetName(); 
            IAlfWidget* ownerwidget = AlfWidgetEnvExtension::widgetFactory(control->Env()).findWidget(name); 
            if (ownerwidget)
                {
                setWidgetRect(*ownerwidget, rect); //sets the rect using widget properties.
                }
            }
        }
    
    }
    
// ---------------------------------------------------------------------------
// returns the child visual count
// ---------------------------------------------------------------------------
//
int AlfLayoutManagerImpl::count() const
    {
    return getLayout().Count();
    }    

// ---------------------------------------------------------------------------
// returns the control, which owns the visual
// ---------------------------------------------------------------------------
//      
CAlfWidgetControl* AlfLayoutManagerImpl::getControl(CAlfVisual& aVisual) const
    {
    CAlfWidgetControl* temp = 0;
    temp = dynamic_cast<CAlfWidgetControl*>(&aVisual.Owner());
    return temp;
    }
    
// ---------------------------------------------------------------------------
// returns the control, which owns the visual
// ---------------------------------------------------------------------------
//      
const IAlfLayoutPreferences* AlfLayoutManagerImpl::getLayoutPreferences(
    CAlfWidgetControl* aControl) const
    {
    IAlfLayoutPreferences* layoutPrefs = 0;
    if (aControl)
        {
        layoutPrefs = 
            CAlfWidgetControl::makeInterface<IAlfLayoutPreferences>(aControl);
        }
        
    return layoutPrefs;
    }
   
// ---------------------------------------------------------------------------
// finds the root visual for a control.
// ---------------------------------------------------------------------------
//    
CAlfVisual* AlfLayoutManagerImpl::findRootVisual(CAlfWidgetControl* aControl) const
    {
    CAlfVisual* root = 0;
    if (aControl->VisualCount() > 0)
        {
        //take first visual, 
        //go up in layout hierarchy until the control is not owner anymore.
        for (root = &aControl->Visual(0); 
             root && root->Layout() && &root->Layout()->Owner() == aControl;
             root = root->Layout()){}
        }
    return root;
    }

// ---------------------------------------------------------------------------
// throws an exception, if layout is not created.
// ---------------------------------------------------------------------------
//    
void AlfLayoutManagerImpl::checkLayout() const
    {
    if (!mLayout.get())
        {
        ALF_THROW(AlfVisualException, EInvalidVisual, className);
        }
    }

 } // Alf
