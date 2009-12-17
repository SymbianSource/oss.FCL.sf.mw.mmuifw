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
* Description:  Description of various grid templates
 *
*/

 
#include "mulcoverflowtemplate.h"

#include <AknUtils.h>

#include <alf/alftextvisual.h>
#include <alf/alfutil.h>
#include "mulassert.h"

namespace Alf
    {

// ---------------------------------------------------------------------------
// MulCoverLogicalTemplate4
// ---------------------------------------------------------------------------
//  
MulCoverFlowTemplate4::MulCoverFlowTemplate4(MulCoverFlowControl &aControl, 
        MulBaseElement &aBaseElement):MulCoverFlowTemplate(aControl, aBaseElement)
    {
    
    }

// ---------------------------------------------------------------------------
// ~MulCoverLogicalTemplate4
// ---------------------------------------------------------------------------
//  
MulCoverFlowTemplate4::~MulCoverFlowTemplate4()
    {
    
    }
    

// ---------------------------------------------------------------------------
// GetIconSize
// ---------------------------------------------------------------------------
//  
void MulCoverFlowTemplate4::GetIconSize(mulvisualitem::TVisualAttribute aAttribute,bool aIslandscape, int& aHeight, int& aWidth)
	{
	int appMainPaneVerityId;
	int coverFlowPaneVarietyId;
	int imagePaneVerityId ;
	int videoIndVerityId ;
	if (aIslandscape)
		{
		appMainPaneVerityId = 7;
		coverFlowPaneVarietyId = 1;
		imagePaneVerityId = 6;
		videoIndVerityId = 0;
		}
	else
		{
		appMainPaneVerityId = 7;
		coverFlowPaneVarietyId = 1;
		imagePaneVerityId = 6;
		videoIndVerityId = 0;
		}
	
	// Get  the size of the appwindow
	TAknWindowLineLayout appMainLayout = 
		AknLayoutScalable_UiAccel::main_pane(appMainPaneVerityId).LayoutLine();
	TAknLayoutRect appMainRect;
	appMainRect.LayoutRect(  AlfUtil::ScreenSize(), appMainLayout );
	TSize appMainSize(appMainRect.Rect().Size());
	
	// Get the size of the coverflow main window
	TAknWindowLineLayout mainLayout = 
	AknLayoutScalable_UiAccel::main_aa_coverflow_pane(coverFlowPaneVarietyId).LayoutLine();
	TAknLayoutRect mainRect;
	mainRect.LayoutRect(  AlfUtil::ScreenSize(), mainLayout );
	TSize mainSize(mainRect.Rect().Size());

	switch(aAttribute)
		{
		case mulvisualitem::KMulIcon1:
			{
            TAknWindowLineLayout centerLayout = 
                AknLayoutScalable_UiAccel::aacf_image_pane(imagePaneVerityId).LayoutLine();
            TAknLayoutRect centerRect;
            centerRect.LayoutRect( TSize(mainSize), centerLayout );
            aHeight = centerRect.Rect().Size().iHeight;
            aWidth  = centerRect.Rect().Size().iWidth;
			break;
			}
		case mulvisualitem::KMulIndicator2:
			{
            TAknWindowLineLayout centerLayout = 
                AknLayoutScalable_UiAccel::main_aa_video_pane_g1(videoIndVerityId).LayoutLine();
            TAknLayoutRect centerRect;
            centerRect.LayoutRect( TSize(mainSize), centerLayout );
            aHeight = centerRect.Rect().Size().iHeight;
            aWidth  = centerRect.Rect().Size().iWidth;			
			break;
			}
		default:
			{
			// invalid attribute
			// raise exception.
			__MUL_ASSERT(false,KInvalidAttribute);
			
			break;
			}
		}
	}
    
    
// ---------------------------------------------------------------------------
// CreateVisualisation
// ---------------------------------------------------------------------------
//  
void MulCoverFlowTemplate4::CreateVisualisation(bool aIsLandscape,bool aIsOrientationChange)
	{
	// set the default attribue of the template
	// irrespective of the orientation
    mData->mSliderEnabled = false;
    mData->mUiOnOffFlag = true;
	mData->mMaxVisibleCount = 1;
	mData->mEnhancedSupportFlag = true;
	
    // store the default attribute in base element
    mData->mBaseElement->SetFitMode(true);
	mData->mBaseElement->StoreTextVisibility( false );
	
    // get the main text layout
    CAlfLayout& textLayout = mData->mBaseElement->TextLayout();	
    textLayout.SetOpacity(0);
	
	mData->mOrientationChange = aIsOrientationChange;
	if (aIsLandscape)
		{
		// create visualisation for landscape
		CreateLscStructure();
		}
	else
		{
		// create visualisation for potrait
		CreatePrtStructure();
		}
	mData->mOrientationChange = false;
	}


// ---------------------------------------------------------------------------
// CreatePrtStructure
// ---------------------------------------------------------------------------
//  
void MulCoverFlowTemplate4::CreatePrtStructure()
	{
	// list of all component lct verity id.
	int mainPaneVerityId = 1;
	int imagePaneVerityId = 6;
	int videoPaneVerityId = 0;
	int counterPaneVerityId = 3;
	
	CAlfVisual* main = mData->mBaseElement->findVisual(KMainLayoutIndex);        
	CAlfLayout* parentLayout = main->Layout();
	TSize mainSize = parentLayout->Size().Target().AsSize();
	
	// get for main layout
	TAknWindowLineLayout componentLayout = 
		AknLayoutScalable_UiAccel::main_aa_coverflow_pane(mainPaneVerityId).LayoutLine();
	TRect mainRect = IconWindowRect(componentLayout, mainSize);
	main->SetPos(TAlfRealPoint(0,0));
	main->SetSize(mainRect.Size());
	
	
	// for deck layout
	CAlfDeckLayout& deckLayout = static_cast<CAlfDeckLayout&>(
	        static_cast<CAlfLayout*>(main)->Visual(KMainLayoutIndex) );
	deckLayout.SetSize(mainRect.Size(),0);
	
	// if set background with some skin id is called before calling set model
	// then the visual and texture would be created with 0,0 sizes
	// hence we need to resize them                
	CAlfVisual* backgroundVisual = deckLayout.FindTag(backgroundvisual);
	if(backgroundVisual)
	   {
	   backgroundVisual->SetSize(mainRect.Size(),0);              
	   }
	
	// for center item	  
    componentLayout = 
        AknLayoutScalable_UiAccel::aacf_image_pane(imagePaneVerityId).LayoutLine();
	TRect centerRect = IconWindowRect(componentLayout,mainRect);          
	
	TMulCoverFlowItem highlightItem(centerRect.iTl.iX,
		centerRect.iTl.iY,centerRect.Size().iWidth,centerRect.Size().iHeight);

	// apply attribute
    mData->mBaseElement->StoreVisualDimensions(highlightItem,highlightItem,mData->mMaxVisibleCount );
	 
   
	 
	 // for indicator icon
	componentLayout =
	    AknLayoutScalable_UiAccel::main_aa_video_pane_g1(videoPaneVerityId).LayoutLine();
	TRect indRect = IconWindowRect(componentLayout,mainRect);
    TSize indSize(indRect.Size());
    mData->mIndicatorDimensions.height = indSize.iHeight;
    mData->mIndicatorDimensions.width  = indSize.iWidth;
    mData->mIndicatorDimensions.posx   = indRect.iTl.iX;
    mData->mIndicatorDimensions.posy   = indRect.iTl.iY; 
    
    // crete counter
    ApplyCounterAttribute(counterPaneVerityId,mainRect);
	}

// ---------------------------------------------------------------------------
// CreateLscStructure
// ---------------------------------------------------------------------------
//  
void MulCoverFlowTemplate4::CreateLscStructure()
	{
	// list of all component lct verity id.
	int mainPaneVerityId = 1;
	int imagePaneVerityId = 6;
	int videoPaneVerityId = 0;
	int counterPaneVerityId = 6;
	if(AknLayoutUtils::CbaLocation() == AknLayoutUtils::EAknCbaLocationRight)
	    {
	    counterPaneVerityId = 8;
	    }
	
	CAlfVisual* main = mData->mBaseElement->findVisual(KMainLayoutIndex);        
	CAlfLayout* prntLay = main->Layout();
	TSize main_size = prntLay->Size().Target().AsSize();
	TAknWindowLineLayout componentLayout;

	// get for main layout
	componentLayout = 
		AknLayoutScalable_UiAccel::main_aa_coverflow_pane(mainPaneVerityId).LayoutLine();
	TRect mainRect = IconWindowRect(componentLayout, main_size);
	main->SetPos(TAlfRealPoint(0,0));
	main->SetSize(mainRect.Size());
	
	// for deck layout
	CAlfDeckLayout& deckLayout = static_cast<CAlfDeckLayout&>(
	        static_cast<CAlfLayout*>(main)->Visual(KMainLayoutIndex) );
	deckLayout.SetSize(mainRect.Size(),0);
	
	// if set background with some skin id is called before calling set model
	// then the visual and texture would be created with 0,0 sizes
	// hence we need to resize them                
	CAlfVisual* backgroundVisual = deckLayout.FindTag(backgroundvisual);
	if(backgroundVisual)
	   {
	   backgroundVisual->SetSize(mainRect.Size(),0);              
	   }
	
	// for center item
	  
    componentLayout = 
        AknLayoutScalable_UiAccel::aacf_image_pane(imagePaneVerityId).LayoutLine();
	TRect centerRect = IconWindowRect(componentLayout,mainRect);          

	TMulCoverFlowItem highlightItem(centerRect.iTl.iX,
		centerRect.iTl.iY,centerRect.Size().iWidth,centerRect.Size().iHeight);
	
	// apply attribute
    mData->mBaseElement->StoreVisualDimensions(highlightItem,highlightItem,mData->mMaxVisibleCount );
	   
 
	 // for indicator icon
	componentLayout =
	    AknLayoutScalable_UiAccel::main_aa_video_pane_g1(videoPaneVerityId).LayoutLine();
	TRect indRect = IconWindowRect(componentLayout,mainRect);
    TSize indSize(indRect.Size());
    mData->mIndicatorDimensions.height = indSize.iHeight;
    mData->mIndicatorDimensions.width  = indSize.iWidth;
    mData->mIndicatorDimensions.posx   = indRect.iTl.iX;
    mData->mIndicatorDimensions.posy   = indRect.iTl.iY; 
    
    // crete counter
    ApplyCounterAttribute(counterPaneVerityId,mainRect);
	}
	
// ---------------------------------------------------------------------------
// ApplyCounterAttribute
// ---------------------------------------------------------------------------
//  
void MulCoverFlowTemplate4::ApplyCounterAttribute(int acounterPaneVerityId,const TRect& aMainRect)
	{
    mData->mUiOnOffFlag = true;
	TAknTextLineLayout counterComponentLayout = 
		AknLayoutScalable_UiAccel::main_aa_coverflow_pane_t1(acounterPaneVerityId).LayoutLine();   
    	
	TRect counterRect = TextWindowRect(counterComponentLayout, aMainRect);
	TSize counterSize(counterRect.Size());
	mData->mCounterDimensions.posx = counterRect.iTl.iX;
	mData->mCounterDimensions.posy = counterRect.iTl.iY;
	mData->mCounterDimensions.width = counterSize.iWidth;
	mData->mCounterDimensions.height = counterSize.iHeight;
	
	// Create a Counter visual if it is not Created.
	CAlfTextVisual& counterVisual = mData->mBaseElement->CounterVisual();
	
    // Set Properties to Counter Visual if we are creating.
    // no need to apply attribute for orientation change
    // as all are same.
    if(!mData->mOrientationChange)
	    {
	    //get font Id for counter
	    int fontId = counterComponentLayout.FontId();
	    int counterFontId = 0x000000FF &  fontId;
		// apply default brush color to the counter background
		ApplyBrush(counterVisual,KRgbBlack,0.5); 
		// apply default attribute
		counterVisual.SetAlign(EAlfAlignHCenter,EAlfAlignVCenter);
	    SetTextStyle(counterFontId, counterVisual);       

		// check if user has set some different attribute.
		TMulVisualColorProperty counterColor = mData->mBaseElement->TextColor(ECounterVisual);
		if (counterColor.mIsColorSet)
			{
			// apply the color set by the application
			counterVisual.SetColor(counterColor.mColor);
			}
		else
			{
			// apply the default color
			counterVisual.SetColor(KRgbWhite);
			}
		mData->mCounterFontMaxCharLength = UpdateMaxFontWidth(&counterVisual);						
	    }
	
	}
      
    }//namespace ends
