/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies). 
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

#include "alfgridlayoutpolicyimpl.h"

using namespace Alf;

namespace Alf
    {
//ifdef to prevent compiler warning: className not used.
#ifdef ALF_DEBUG_EXCEPTIONS
static const char* const className = "AlfGridLayoutPolicyImpl";
#endif
    
    
// ======== MEMBER FUNCTIONS ========
    
// ---------------------------------------------------------------------------
// layoutmanagerimpl constructor
// ---------------------------------------------------------------------------
//    
AlfGridLayoutPolicyImpl::AlfGridLayoutPolicyImpl(
    AlfGridLayoutManager& aGridLayoutManager) : 
    mGridLayoutManager(aGridLayoutManager)
    {
    }
    
// ---------------------------------------------------------------------------
// creates the layout used by the layoutmanager.
// ---------------------------------------------------------------------------
//    
AlfGridLayoutPolicyImpl::~AlfGridLayoutPolicyImpl()
    {
    }

// ---------------------------------------------------------------------------
// from IAlfGridLayoutPolicy
// Sets the number and weights of blocks in this grid layout in the direction of the specified
// dimension. 
// ---------------------------------------------------------------------------
void AlfGridLayoutPolicyImpl::fillWeights(gridLayoutDimension aDim, int aCount, const TAlfMetric& aWeight)
    {
    CAlfGridLayout& gridLayout = layout();
    TAlfGridDimension dim = (TAlfGridDimension)aDim;
    TRAPD(err, gridLayout.FillWeightsL(dim, aCount, aWeight));
    if(err != KErrNone)
        {
        ALF_THROW(AlfVisualException, err, className);
        }
    }

// ---------------------------------------------------------------------------
// from IAlfGridLayoutPolicy
// Add a new line of blocks to this grid layout at the last position in the direction of the 
// specified dimension. 
// ---------------------------------------------------------------------------
void AlfGridLayoutPolicyImpl::appendWeight(gridLayoutDimension aDim, const TAlfMetric& aWeight)
    {
    CAlfGridLayout& gridLayout = layout();
    TAlfGridDimension dim = (TAlfGridDimension)aDim;
    TRAPD(err, gridLayout.AppendWeightL(dim, aWeight));
    if(err != KErrNone)
        {
        ALF_THROW(AlfVisualException, err, className);
        }
    }

// ---------------------------------------------------------------------------
// from IAlfGridLayoutPolicy
// Add a new line of blocks to this grid layout at the specified position in the direciton of 
// the specified dimension. 
// ---------------------------------------------------------------------------
void AlfGridLayoutPolicyImpl::insertWeight(gridLayoutDimension aDim, const TAlfMetric& aWeight, int aPos)
    {
    CAlfGridLayout& gridLayout = layout();
    TAlfGridDimension dim = (TAlfGridDimension)aDim;
    TRAPD(err, gridLayout.InsertWeightL(dim, aWeight, aPos));
    if(err != KErrNone)
        {
        ALF_THROW(AlfVisualException, err, className);
        }
    }

// ---------------------------------------------------------------------------
// from IAlfGridLayoutPolicy
// Sets the weight of a specific line of blocks in this grid layout, in the direction of the supplied dimension.
// ---------------------------------------------------------------------------
void AlfGridLayoutPolicyImpl::replaceWeight(gridLayoutDimension aDim, const TAlfMetric& aWeight, int aPos)
    {
    CAlfGridLayout& gridLayout = layout();
    TAlfGridDimension dim = (TAlfGridDimension)aDim;
    TRAPD(err, gridLayout.ReplaceWeightL(dim, aWeight, aPos));
    if(err != KErrNone)
        {
        ALF_THROW(AlfVisualException, err, className);
        }
    }

// ---------------------------------------------------------------------------
// from IAlfGridLayoutPolicy
// Remove a line of blocks from this grid layout at the specified position in the 
// specified dimension. 
// ---------------------------------------------------------------------------
void AlfGridLayoutPolicyImpl::removeWeight(gridLayoutDimension aDim, int aPos)
    {
    CAlfGridLayout& gridLayout = layout();
    TAlfGridDimension dim = (TAlfGridDimension)aDim;
    TRAPD(err, gridLayout.RemoveWeightL(dim, aPos));
    if(err != KErrNone)
        {
        ALF_THROW(AlfVisualException, err, className);
        }
    }


// ---------------------------------------------------------------------------
// from IAlfGridLayoutPolicy
// Returns the weight of a specific line of blocks in this grid layout, in the
// specified dimension. 
// ---------------------------------------------------------------------------
TAlfMetric AlfGridLayoutPolicyImpl::weight(gridLayoutDimension aDim, int aPos) const
    {
    CAlfGridLayout& gridLayout = layout();
    TAlfGridDimension dim = (TAlfGridDimension)aDim;
    TAlfMetric result;
    TRAPD(err, result = gridLayout.Weight(dim, aPos));
    if(err != KErrNone)
        {
        ALF_THROW(AlfVisualException, err, className);
        }
    return result;
    }

    
// ---------------------------------------------------------------------------
// from IAlfGridLayoutPolicy
// Return the number of lines of blocks in this grid, along the specified dimension
// ---------------------------------------------------------------------------
int AlfGridLayoutPolicyImpl::count(gridLayoutDimension aDim) const
    {
    CAlfGridLayout& gridLayout = layout();
    TAlfGridDimension dim = (TAlfGridDimension)aDim;
    int result(0);
    TRAPD(err, result = gridLayout.DimensionCount(dim));
    if(err != KErrNone)
        {
        ALF_THROW(AlfVisualException, err, className);
        }
    return result;
    }



// ---------------------------------------------------------------------------
// From class IAlfInterfaceBase.
// Getter for interfaces provided by the element.
// ---------------------------------------------------------------------------
//
IAlfInterfaceBase* AlfGridLayoutPolicyImpl::makeInterface(const IfId& aType)
    {
    UString param(aType.mImplementationId);
    if (param == IAlfGridLayoutPolicy::type().mImplementationId)
        {
        return this;
        }      
    
    return NULL;
    }
    
// ---------------------------------------------------------------------------
// returns the gridlayout used by the layoutmanager
// ---------------------------------------------------------------------------
//
CAlfGridLayout& AlfGridLayoutPolicyImpl::layout() const
    {
    return dynamic_cast<CAlfGridLayout&>(mGridLayoutManager.getLayout());
    }

 } // Alf
