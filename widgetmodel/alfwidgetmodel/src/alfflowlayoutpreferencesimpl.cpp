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
* Description:  layoutpreferences implementation for flowlayoutmanager.
*
*/


#include <assert.h>
#include <alf/ialfwidgetcontrol.h>
#include "alf/alfwidget.h"
#include <alf/alfexceptions.h>
#include <alf/ialflayoutpreferences.h>
#include <alf/alfflowlayoutmanager.h>
#include <alf/ialfflowlayoutpolicy.h>
#include <osn/osnnew.h>

//stl
#include <algorithm> //for min & max
#include "alfflowlayoutpreferencesimpl.h"

using namespace Alf;

namespace Alf
    {
// ======== MEMBER FUNCTIONS ========

AlfFlowLayoutPreferencesImpl::AlfFlowLayoutPreferencesImpl(
    AlfFlowLayoutManager& aFlowLayoutManager) : 
    mFlowLayoutManager(aFlowLayoutManager), mPreferredSize(TAlfMetric(0))
    {
    }
// ---------------------------------------------------------------------------
// from IAlfLayoutPreferences
// Returns the minimum area that this layout manager can occupy by observing
// the minimum sizes of the child UI elements in the layout
// ---------------------------------------------------------------------------
//
bool AlfFlowLayoutPreferencesImpl::getMinimumSize(TAlfXYMetric& aMinSize) const
	{
    return calculateChildSizes(aMinSize, 
        flowLayoutPolicy().flowDirection(), ESizeMin);
	}

// ---------------------------------------------------------------------------
// from IAlfLayoutPreferences
// Returns the maximum area that this layout manager can occupy by observing
// the maximum sizes of the child UI elements in the layout
// ---------------------------------------------------------------------------
//
bool AlfFlowLayoutPreferencesImpl::getMaximumSize(TAlfXYMetric& aMaxSize) const
	{
    return calculateChildSizes(aMaxSize, 
        flowLayoutPolicy().flowDirection(), ESizeMax);
	}

// ---------------------------------------------------------------------------
// Combines and returns the preferred sizes of all child UI elements according
// to the layouting rules.
// ---------------------------------------------------------------------------
//
bool AlfFlowLayoutPreferencesImpl::getPreferredSize(TAlfXYMetric& aPreferredSize) const
    {
    bool b = false;
    bool manualSet = (mPreferredSize.iX.iMagnitude > 0 && 
                      mPreferredSize.iY.iMagnitude > 0);
    
    if (manualSet)
        {
        aPreferredSize = mPreferredSize;
        b = true;
        }
    else
        {
        b = calculateChildSizes(aPreferredSize, 
            flowLayoutPolicy().flowDirection(), ESizePreferred);
        }
    return b;
	}
 
// ---------------------------------------------------------------------------
// from IAlfLayoutPreferences
// At the moment doesn't do anything since preferred size is being queried
// from the child UI elements added to this layout manager.
// ---------------------------------------------------------------------------
//
void AlfFlowLayoutPreferencesImpl::setPreferredSize( 
    const TAlfXYMetric& aPreferredSize )
	{
	mPreferredSize = aPreferredSize;
    mFlowLayoutManager.owner().updateParentLayout();
	}
    
// ---------------------------------------------------------------------------
// From class IAlfInterfaceBase.
// Getter for interfaces provided by the element.
// ---------------------------------------------------------------------------
//
IAlfInterfaceBase* AlfFlowLayoutPreferencesImpl::makeInterface(
    const IfId& aType)
    {
    UString param(aType.mImplementationId);
    IAlfInterfaceBase* ret = 0;
    if (param == IAlfLayoutPreferences::type().mImplementationId)
        {
        ret = static_cast<IAlfLayoutPreferences*>(this);
        }
    
    return ret;    
    }
    
// ---------------------------------------------------------------------------
// calculate the min-, preferred-, or max- sizes from the children. 
// ---------------------------------------------------------------------------
//    
bool AlfFlowLayoutPreferencesImpl::calculateChildSizes(
    TAlfXYMetric& aSize,  
    enum IAlfFlowLayoutPolicy::flowDirection aFlowDirection, 
    enum AlfFlowLayoutPreferencesImpl::sizeType aSizeType) const
    {
	bool ret = false; 
	TAlfXYMetric resultSize(TAlfMetric(0), TAlfMetric(0));
	
    // Iterate through all the child controls, and get their
    // preferred sizes.
    int childCount = mFlowLayoutManager.count();
    for (int i = 0; i < childCount; i++)
        {
        CAlfWidgetControl* childWidgetControl = mFlowLayoutManager.getControl(i);
        
        TAlfXYMetric size;
        if (getSize(childWidgetControl, size, aSizeType))
            {
            TAlfRealSize pixelSize = sizeInPixels(size);
            
            if (aFlowDirection == IAlfFlowLayoutPolicy::EFlowVertical)
                {
                //total width = max preferred width of child widgets.
                //total height = sum of the preferred heights of child widgets.
            	if(pixelSize.iWidth > resultSize.iX.iMagnitude)
            		{
            		resultSize.iX.iMagnitude = pixelSize.iWidth;
            		}
        		resultSize.iY.iMagnitude += pixelSize.iHeight;
        		ret = true;
                }
            else
                {
                //horizontal flow direction
                //total height = max preferred height of child widgets.
                //total width = sum of the preferred widths of child widgets.
            	if(pixelSize.iHeight > resultSize.iY.iMagnitude)
            		{
            		resultSize.iY.iMagnitude = pixelSize.iHeight;
            		}
        		resultSize.iX.iMagnitude += pixelSize.iWidth;
        		ret = true;
                }
            }
        }
	
    aSize = resultSize;
	return ret;
    }
    
// ---------------------------------------------------------------------------
// fetches the size from the IAlfLayoutPreferences-interface of the aControl.
// ---------------------------------------------------------------------------
//    
bool AlfFlowLayoutPreferencesImpl::getSize(
    CAlfWidgetControl* aControl,
    TAlfXYMetric& aSize,
    enum AlfFlowLayoutPreferencesImpl::sizeType aSizeType) const
    {
    bool ret = false;
    
    if (aControl)
        {
        const IAlfLayoutPreferences* layoutPrefs = 
            mFlowLayoutManager.getLayoutPreferences(aControl);
        
        if (layoutPrefs)
            {
            switch(aSizeType)
                {
                case AlfFlowLayoutPreferencesImpl::ESizePreferred:
                    ret = layoutPrefs->getPreferredSize(aSize);
                    break;
                case AlfFlowLayoutPreferencesImpl::ESizeMin:
                    ret = layoutPrefs->getMinimumSize(aSize);
                    break;
                case AlfFlowLayoutPreferencesImpl::ESizeMax:
                    ret = layoutPrefs->getMaximumSize(aSize);
                    break;
                }
            }
        }
        
    return ret;
    }
    
// ---------------------------------------------------------------------------
// get the flowlayoutpolicy-interface.
// ---------------------------------------------------------------------------
//    
const IAlfFlowLayoutPolicy& AlfFlowLayoutPreferencesImpl::flowLayoutPolicy() const
    {
	AlfFlowLayoutManager* man = 
	    const_cast<AlfFlowLayoutManager*>(&mFlowLayoutManager);
	IAlfFlowLayoutPolicy* policy = 
	    IAlfInterfaceBase::makeInterface<IAlfFlowLayoutPolicy>(man);
    assert(policy);
    return *policy;
    }
// ---------------------------------------------------------------------------
// converts size in metrics to size in pixels.
// implementation missing!
// ---------------------------------------------------------------------------
//    
TAlfRealSize AlfFlowLayoutPreferencesImpl::sizeInPixels(
    const TAlfXYMetric& aSize)
    {
    TAlfRealSize s;

    if(aSize.iX.iUnit == EAlfUnitPixel)
    	{
    	s.iWidth = aSize.iX.iMagnitude;
    	}
    if(aSize.iY.iUnit == EAlfUnitPixel)
        {
        s.iHeight = aSize.iY.iMagnitude;
        }

    return s;
    }
} // Alf
