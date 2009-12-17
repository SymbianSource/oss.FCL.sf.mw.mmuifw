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
* Description:  implementation for flowlayoutpolicy
*
*/


#include <assert.h>
#include <alf/alfflowlayout.h>
#include <alf/alfflowlayoutmanager.h>
#include <osn/ustring.h>
#include <osn/osnnew.h>

#include "alfflowlayoutpolicyimpl.h"

using osncore::UString;

using namespace Alf;

namespace Alf
    {
    
typedef enum IAlfFlowLayoutPolicy::flowDirection flowDirection;

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// flowlayoutpolicyimpl constructor
// ---------------------------------------------------------------------------
//    
AlfFlowLayoutPolicyImpl::AlfFlowLayoutPolicyImpl(
    AlfFlowLayoutManager& aFlowLayoutManager) : 
    mFlowLayoutManager(aFlowLayoutManager), mDirection(EFlowVertical)
    {
    }
    
// ---------------------------------------------------------------------------
// creates the layout used by the layoutmanager.
// ---------------------------------------------------------------------------
//    
AlfFlowLayoutPolicyImpl::~AlfFlowLayoutPolicyImpl()
    {
    }
    
// ---------------------------------------------------------------------------
// Sets the direction of the flow.
// ---------------------------------------------------------------------------
//
void AlfFlowLayoutPolicyImpl::setFlowDirection(enum flowDirection aDirection)
    {
    mDirection = aDirection;
    
    CAlfFlowLayout::TFlowDirection flowdir(CAlfFlowLayout::EFlowHorizontal);
    if (aDirection == EFlowVertical)
        {
        flowdir = CAlfFlowLayout::EFlowVertical;
        }
    
    layout().SetFlowDirection(flowdir);
    }
  
// ---------------------------------------------------------------------------
// returns the flow direction
// ---------------------------------------------------------------------------
//         
flowDirection AlfFlowLayoutPolicyImpl::flowDirection() const
    {
    return mDirection;
    }

// ---------------------------------------------------------------------------
// Sets the mode of the flow. The mode flags determine how the flow layout
// behaves.
// ---------------------------------------------------------------------------
//         
void AlfFlowLayoutPolicyImpl::setMode(int aMode)
    {
    layout().SetMode(aMode);
    }
    
// ---------------------------------------------------------------------------
// Returns the mode flags of the flow.
// ---------------------------------------------------------------------------
//         
int AlfFlowLayoutPolicyImpl::mode() const
    {
    return layout().Mode();
    }

// ---------------------------------------------------------------------------
// From class IAlfInterfaceBase.
// Getter for interfaces provided by the element.
// ---------------------------------------------------------------------------
//
IAlfInterfaceBase* AlfFlowLayoutPolicyImpl::makeInterface(const IfId& aType)
    {
    UString param(aType.mImplementationId);
    IAlfInterfaceBase* ret = 0;
    if (param == IAlfFlowLayoutPolicy::type().mImplementationId)
        {
        ret = static_cast<IAlfFlowLayoutPolicy*>(this);
        }
    
    return ret;
    }
    
// ---------------------------------------------------------------------------
// returns the flow layout used by the layoutmanager
// ---------------------------------------------------------------------------
//
CAlfFlowLayout& AlfFlowLayoutPolicyImpl::layout() const
    {
    return dynamic_cast<CAlfFlowLayout&>(mFlowLayoutManager.getLayout());
    }

 } // Alf
