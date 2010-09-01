/*
* Copyright (c) 2007-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Utility class for widgets visualization in terms of LCT data
 *
*/


// includes
// Class header
#include "mulvisualutility.h"

// Alf headers
#include "alf/alfanchorlayoutattributesetter.h"
#include "alf/alfattribute.h"
#include "alf/alfattributecontainer.h"
#include <alf/alfenv.h> 
#include "alf/alftextvisualattributesetter.h"
#include "alf/alfvisualtemplate.h"
#include <alf/alfutil.h>

// DUI headers for xml parser

#include "alf/attrproperty.h"

#include<aknlayoutscalable_uiaccel.cdl.h>

// Local headers
#include "mulimagevisualattributesetter.h"
#include "mulassert.h"
#include "mulutility.h"
#include "mullog.h" //for logs
using namespace duiuimodel;


namespace Alf
{

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//	
OSN_EXPORT MulVisualUtility::MulVisualUtility()
    {
    mData.reset(new (EMM) LCTData);                    
    }
    
// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
OSN_EXPORT MulVisualUtility::~MulVisualUtility()
    {
	// Nothing to destroy
    }
    
    
// ---------------------------------------------------------------------------
// parseLCTInfo 
// ---------------------------------------------------------------------------
//
OSN_EXPORT void MulVisualUtility::parseLCTInfo(const LCTPair& aPair, CAlfLayout* aParentLayout)
	{
	TAknWindowLineLayout componentLayout;
	TAknLayoutRect layoutRect;
	UString lctId = aPair.mLctId;
	TSize parentSize (0,0) ; 
	if(aParentLayout )
	{
		parentSize = aParentLayout->Size().Target().AsSize();	
	}
	else
	{
		TSize size = AlfUtil::ScreenSize();
		TAknWindowLineLayout parentLayout;
		TAknLayoutRect layout;
		bool isLandscape = MulUtility::IsLandscape();
		if(!isLandscape)
		{
			parentLayout = AknLayoutScalable_UiAccel::main_pane(5).LayoutLine();
		}
		else
		{
			parentLayout = AknLayoutScalable_UiAccel::main_pane(6).LayoutLine();
		}
		
		layout.LayoutRect( TRect(size), parentLayout );
		parentSize.iWidth = layout.Rect().Width();
		parentSize.iHeight = layout.Rect().Height();
	}
	
	if(!lctId.compare(UString("uiaccel_main_aa_gridscroll_pane")))
	    {
	    componentLayout = AknLayoutScalable_UiAccel::main_aa_gridscroll_pane(aPair.mVariety).LayoutLine();
                
	    }
	if(!lctId.compare(UString("uiaccel_aagrid_gene_pane")))
	    {
	    componentLayout = AknLayoutScalable_UiAccel::aagrid_gene_pane(aPair.mVariety).LayoutLine();
        
        	    
	    }
	if(!lctId.compare(UString("uiaccel_aagrid_cell_image_pane")))
	    {
	    componentLayout = AknLayoutScalable_UiAccel::aagrid_cell_image_pane(aPair.mVariety, 0, 0 ).LayoutLine();
        
        }
	if(!lctId.compare(UString("uiaccel_aa_scroll_pane_cp001")))
	    {
	    componentLayout = AknLayoutScalable_UiAccel::aa_scroll_pane_cp001(aPair.mVariety).LayoutLine();
        
        }
        
    if(!lctId.compare(UString("uiaccel_aagrid_cell_image_pane_g1")) )
    	{
    	componentLayout = AknLayoutScalable_UiAccel::aagrid_cell_image_pane_g1(aPair.mVariety).LayoutLine();
    	}
    
    if(!lctId.compare(UString("uiaccel_aagrid_cell_image_pane_g2")) )
    	{
    	componentLayout = AknLayoutScalable_UiAccel::aagrid_cell_image_pane_g2(aPair.mVariety).LayoutLine();
    	}
    	
    if(!lctId.compare(UString("uiaccel_aagrid_cell_image_pane_g3")) )
    	{
    	componentLayout = AknLayoutScalable_UiAccel::aagrid_cell_image_pane_g3(aPair.mVariety).LayoutLine();
    	}			
	    
	if(!lctId.compare(UString("uiaccel_aagrid_cell_image_pane_g4")) )
    	{
    	componentLayout = AknLayoutScalable_UiAccel::aagrid_cell_image_pane_g4(aPair.mVariety).LayoutLine();
    	}			
	        
	    
	    layoutRect.LayoutRect( TRect(parentSize), componentLayout );
	    
	    mData->mPosX = layoutRect.Rect().iTl.iX;
        mData->mPosY = layoutRect.Rect().iTl.iY;
        mData->mWidth = layoutRect.Rect().Width();
        mData->mHeight = layoutRect.Rect().Height();
	
    }

// ---------------------------------------------------------------------------
// parseLCTInfo 
// ---------------------------------------------------------------------------
//
OSN_EXPORT void MulVisualUtility::parseLCTTextInfo()
	{
	
	}
	
// ---------------------------------------------------------------------------
// Returns the structure containing width, height etc info
// ---------------------------------------------------------------------------
//	
OSN_EXPORT const LCTData& MulVisualUtility::data() const
    {
    return *(mData.get());
    }

// ---------------------------------------------------------------------------
// Internal function to convert LCT IDs into absolute data for text
// ---------------------------------------------------------------------------
//
void MulVisualUtility::readLCTTextData()
	{
	
	}
	
// ---------------------------------------------------------------------------
// Internal function to concert LCT IDs into absolute data 
// ---------------------------------------------------------------------------
//
void MulVisualUtility::readLCTData()
	{
	
	}
	
// ---------------------------------------------------------------------------
// Internal function
// ---------------------------------------------------------------------------
//	
bool MulVisualUtility::isParentRelative(int /*aVal*/) const
    {
    return false;
    }

} // namespace Alf

// End of file
