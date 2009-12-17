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


// Class Header
#include "mulcoverflowtemplate.h"

// system includes
#include <AknUtils.h>
#include <alf/alfviewportlayout.h>
#include <alf/alftextvisual.h> 
#include <alf/alfutil.h>

// Local includes
#include "mulassert.h"
#include "mulcoverflowcontrol.h"

namespace Alf
    {
    
// ---------------------------------------------------------------------------
// MulCoverLogicalTemplate1
// ---------------------------------------------------------------------------
//  
MulCoverFlowTemplate1::MulCoverFlowTemplate1(MulCoverFlowControl &aControl, 
        MulBaseElement &aBaseElement):MulCoverFlowTemplate(aControl, aBaseElement)
    {
    
    }

// ---------------------------------------------------------------------------
// ~MulCoverLogicalTemplate1
// ---------------------------------------------------------------------------
//  
MulCoverFlowTemplate1::~MulCoverFlowTemplate1()
    {
    
    }
    
// ---------------------------------------------------------------------------
// GetIconSize
// ---------------------------------------------------------------------------
//      
void MulCoverFlowTemplate1::GetIconSize(mulvisualitem::TVisualAttribute aAttribute,bool aIslandscape, int& aHeight, int& aWidth)
	{
	int appMainPaneVerityId;
	int coverFlowPaneVarietyId;
	int imagePaneVerityId ;
	if (aIslandscape)
		{
		appMainPaneVerityId = 3; 
		coverFlowPaneVarietyId = 2;
		imagePaneVerityId = 4;
		}
	else
		{
		appMainPaneVerityId = 5; 
		coverFlowPaneVarietyId = 0;
		imagePaneVerityId = 0;
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
	mainRect.LayoutRect( appMainSize, mainLayout );
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
void MulCoverFlowTemplate1::CreateVisualisation(bool aIsLandscape,bool aIsOrientationChange)
	{
	mData->mOrientationChange = aIsOrientationChange;	
	
	// set the default attribue of the template
	// irrespective of the orientation
    mData->mUiOnOffFlag = false;
	mData->mMaxVisibleCount = 3;
	mData->mEnhancedSupportFlag = false;
	
    // store the default attribute in base element
    mData->mBaseElement->SetFitMode(false);
	
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
void MulCoverFlowTemplate1::CreatePrtStructure()
	{
	// list of all component lct verity id.
	int mainPaneVerityId = 0;
	int imagePaneCenterVerityId = 0;
	int counterPaneVerityId = 0;
	
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
        AknLayoutScalable_UiAccel::aacf_image_pane(imagePaneCenterVerityId).LayoutLine();
	TRect centerRect = IconWindowRect(componentLayout,mainRect);          
	
	TMulCoverFlowItem highlightItem(centerRect.iTl.iX,
		centerRect.iTl.iY,centerRect.Size().iWidth,centerRect.Size().iHeight);

	// apply attribute
    mData->mBaseElement->StoreVisualDimensions(highlightItem,highlightItem,1);
	 
    ApplyCounterAttribute(counterPaneVerityId,mainRect);
	ApplyTextAttribute(mainRect);
    
    // set the status of the slider
    mData->mSliderEnabled = true;
	ApplySliderAttribute(mainRect);	
	}

// ---------------------------------------------------------------------------
// CreateLscStructure
// ---------------------------------------------------------------------------
//      	
void MulCoverFlowTemplate1::CreateLscStructure()
	{
	// list of all component lct verity id.
	int mainPaneVerityId = 2;
	int imagePaneCenterVerityId = 4;
	int imagePaneRightVerityId = 3;	
	int counterPaneVerityId = 4;
	
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
        AknLayoutScalable_UiAccel::aacf_image_pane(imagePaneCenterVerityId).LayoutLine();
	TRect centerRect = IconWindowRect(componentLayout,mainRect);          
	
	TMulCoverFlowItem highlightItem(centerRect.iTl.iX,
		centerRect.iTl.iY,centerRect.Size().iWidth,centerRect.Size().iHeight);
	
	// for right item
    if (AknLayoutUtils::LayoutMirrored())
    	{
    	componentLayout =
        	AknLayoutScalable_UiAccel::aacf_image_pane_cp001(imagePaneRightVerityId).LayoutLine();
    	}
    else
    	{
    	componentLayout =
        	AknLayoutScalable_UiAccel::aacf_image_pane_cp002(imagePaneRightVerityId).LayoutLine();
    	}
	TRect rightItemRect = IconWindowRect(componentLayout,mainRect);

	TMulCoverFlowItem nonHighlightItem(rightItemRect.iTl.iX,
		rightItemRect.iTl.iY,rightItemRect.Size().iHeight,rightItemRect.Size().iHeight);
	
	// apply attribute
    mData->mBaseElement->StoreVisualDimensions(highlightItem,nonHighlightItem,3);
	 
    ApplyCounterAttribute(counterPaneVerityId,mainRect);
	ApplyTextAttribute(mainRect);
    
    // set the status of the slider
    mData->mSliderEnabled = false;
	}
	
// ---------------------------------------------------------------------------
// ApplyCounterAttribute
// ---------------------------------------------------------------------------
//      		
void MulCoverFlowTemplate1::ApplyCounterAttribute(int acounterPaneVerityId,const TRect& aMainRect)
	{
	TAknTextLineLayout counterComponentLayout = 
		AknLayoutScalable_UiAccel::main_aa_coverflow_pane_t1(acounterPaneVerityId).LayoutLine();   
    	
    TRect counterRect = TextWindowRect(counterComponentLayout,aMainRect);
    TSize counterSize(counterRect.Size());
    mData->mCounterDimensions.posx = counterRect.iTl.iX;
    mData->mCounterDimensions.posy = counterRect.iTl.iY;
    mData->mCounterDimensions.width = counterSize.iWidth;
    mData->mCounterDimensions.height = counterSize.iHeight;
        
    // Set Properties to Counter Visual if we are creating.
    // no need to apply attribute for orientation change
    // as all are same.
    if(!mData->mOrientationChange)
	    {
	    // Create a Counter visual if it is not Created.
	    CAlfTextVisual& counterVisual = mData->mBaseElement->CounterVisual();
	    
	    //get font Id for counter
	    int fontId = counterComponentLayout.FontId();
	    int counterFontId = 0x000000FF &  fontId;	    
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

// ---------------------------------------------------------------------------
// ApplyTextAttribute
// ---------------------------------------------------------------------------
//      		
void MulCoverFlowTemplate1::ApplyTextAttribute(const TRect& aMainRect)
	{
	// list of all component lct verity id.	
	int textPaneVerityId ;
	int textPaneBgVerityId ;
	int textPaneTitleVerityId ;	
	int textPaneDetailVerityId ;
	float bgTextOpacity;
	
	if (mData->mControl->IsLandscape())
		{
		textPaneVerityId = 3;
		textPaneBgVerityId = 0;
		textPaneTitleVerityId = 0;
		textPaneDetailVerityId = 0;
		bgTextOpacity = 0.5;
		}
	else
		{
		textPaneVerityId = 0;
		textPaneBgVerityId = 0;
		textPaneTitleVerityId = 0;
		textPaneDetailVerityId = 0;
		bgTextOpacity = 0;
		}
	
	// set text visibility 
	mData->mBaseElement->StoreTextVisibility( true );	
	
	// Create and apply attribute for text visual.
	 
    CAlfLayout& textLayout = mData->mBaseElement->TextLayout();
    
    // for parent text layout
	TAknWindowLineLayout windowLineLayout = 
        AknLayoutScalable_UiAccel::aacf_image_title_pane(textPaneVerityId).LayoutLine();
    TRect textRect = IconWindowRect(windowLineLayout, aMainRect);
    TRect textLayoutRect = TRect(0,0,textRect.Width(),textRect.Height());
    
    // set the position of parent text layout.     
    textLayout.SetPos(textRect.iTl);
    textLayout.SetSize(textRect.Size()); 
    
    // for the background visual
    CAlfTextVisual* bkgTextVisual = static_cast<CAlfTextVisual*>(
            textLayout.FindTag(KLitMulBackGround));
    if(!bkgTextVisual)
        {
        bkgTextVisual = CAlfTextVisual::AddNewL(*mData->mControl,  &textLayout); 
        bkgTextVisual->SetTagL(KLitMulBackGround);
        bkgTextVisual->SetFlag(EAlfVisualFlagIgnorePointer);                
        }
    windowLineLayout = 
      AknLayoutScalable_UiAccel::aacf_image_title_pane_g1(textPaneBgVerityId).LayoutLine();
    TRect bkgRect = IconWindowRect(windowLineLayout , textLayoutRect);
    bkgTextVisual->SetSize(bkgRect.Size());
    bkgTextVisual->SetPos(bkgRect.iTl);
    
    ApplyBrush(*bkgTextVisual,KRgbBlack,bgTextOpacity);
    
    ApplyTitleTextAttribute(textPaneTitleVerityId,textLayoutRect);
    ApplyDetailTextAttribute(textPaneDetailVerityId,textLayoutRect);
    
	}

// ---------------------------------------------------------------------------
// ApplyTitleTextAttribute
// ---------------------------------------------------------------------------
//      		
void MulCoverFlowTemplate1::ApplyTitleTextAttribute(int atextPaneTitleVerityId,const TRect& aTextLayoutRect)
	{
		
    // get the main text layout
    CAlfLayout& textLayout = mData->mBaseElement->TextLayout();	
    
	// for title visual
    CAlfViewportLayout *viewPortLayout =static_cast<CAlfViewportLayout*>(
            textLayout.FindTag(KTitleViewPortLayout));
   if(viewPortLayout == NULL)
       {
       viewPortLayout = CAlfViewportLayout::AddNewL(*mData->mControl,
               &textLayout);
       viewPortLayout->SetTagL(KTitleViewPortLayout);    
       viewPortLayout->SetFlag(EAlfVisualFlagAutomaticLocaleMirroringEnabled);   
       viewPortLayout->SetFlag(EAlfVisualFlagIgnorePointer);                
       } 
   else
       {
       viewPortLayout = static_cast <CAlfViewportLayout*>(
              textLayout.FindTag(KTitleViewPortLayout));
       }
    TAknTextLineLayout textWindowLayout =
     AknLayoutScalable_UiAccel::aacf_image_title_pane_t1(atextPaneTitleVerityId).LayoutLine(); 
    TRect titleRect = TextWindowRect(textWindowLayout, aTextLayoutRect);
    mData->mTitleParentSize = titleRect.Size();
    viewPortLayout->SetSize(titleRect.Size());
    viewPortLayout->SetPos(titleRect.iTl); 
    viewPortLayout->SetViewportSize(TAlfRealSize(titleRect.Size()),0);           
    viewPortLayout->SetClipping(true);
    CAlfTextVisual* textVisual = static_cast<CAlfTextVisual*>(
    	&mData->mBaseElement->CreateTextVisual(KCoverflowTitle,*viewPortLayout)); 
              
   if(!mData->mOrientationChange) 
       {
		//Get font ID
		int fontId = textWindowLayout.FontId();
		int textFontId = 0x000000FF &  fontId;
		SetTextStyle(textFontId, *textVisual);
		mData->mTitleFontMaxCharLength = UpdateMaxFontWidth(textVisual);
		// Apply default attribute.
		textVisual->SetAlign(EAlfAlignHLocale,EAlfAlignVCenter);
		textVisual->SetOpacity(1);
       
		// check if user has set some different attribute.
		TMulVisualColorProperty textColor = mData->mBaseElement->TextColor(ETilteDetailTextVisual);
		if (textColor.mIsColorSet)
			{
			// apply the color set by the application
			textVisual->SetColor(textColor.mColor);
			}
		else
			{
			// apply the default color
			textVisual->SetColor(KRgbWhite);
			}
       }	
	}

// ---------------------------------------------------------------------------
// ApplyDetailTextAttribute
// ---------------------------------------------------------------------------
//      		
void MulCoverFlowTemplate1::ApplyDetailTextAttribute(int aTextPaneDetailVerityId,const TRect& aTextLayoutRect)
	{	
    // get the main text layout
    CAlfLayout& textLayout = mData->mBaseElement->TextLayout();		
	
    CAlfViewportLayout *viewPortLayout = NULL;
    CAlfTextVisual* textVisual = NULL;
    viewPortLayout =static_cast<CAlfViewportLayout*>(
            textLayout.FindTag(KDetailViewPortLayout));
   if(viewPortLayout == NULL)
       {
       viewPortLayout = CAlfViewportLayout::AddNewL(*mData->mControl,
               &textLayout);
       viewPortLayout->SetTagL(KDetailViewPortLayout);   
       viewPortLayout->SetFlag(EAlfVisualFlagAutomaticLocaleMirroringEnabled); 
       viewPortLayout->SetFlag(EAlfVisualFlagIgnorePointer);                               
       } 
   else
       {
       viewPortLayout = static_cast <CAlfViewportLayout*>(
              textLayout.FindTag(KDetailViewPortLayout));
       }
    TAknTextLineLayout textWindowLayout =
       AknLayoutScalable_UiAccel::aacf_image_title_pane_t2(aTextPaneDetailVerityId).LayoutLine(); 
    TRect detailRect = TextWindowRect(textWindowLayout, aTextLayoutRect);
    mData->mDetailParentSize = detailRect.Size();
    viewPortLayout->SetSize(detailRect.Size());
    viewPortLayout->SetPos(detailRect.iTl); 
    viewPortLayout->SetViewportSize(TAlfRealSize(detailRect.Size()),0);           

    viewPortLayout->SetClipping(true);
    textVisual = static_cast<CAlfTextVisual*>(
      &mData->mBaseElement->CreateTextVisual(KCoverflowDetail , *viewPortLayout )); 
   if(!mData->mOrientationChange) 
       {
       //Get font ID
       int fontId = textWindowLayout.FontId();
       int textFontId = 0x000000FF &  fontId;
       SetTextStyle(textFontId, *textVisual);
       mData->mDetailFontMaxCharLength = UpdateMaxFontWidth(textVisual);
       // Apply default attribute.
       textVisual->SetAlign(EAlfAlignHLocale,EAlfAlignVCenter);
       textVisual->SetOpacity(0.7);
       
		// check if user has set some different attribute.
		TMulVisualColorProperty textColor = mData->mBaseElement->TextColor(ETilteDetailTextVisual);
		if (textColor.mIsColorSet)
			{
			// apply the color set by the application
			textVisual->SetColor(textColor.mColor);
			}
		else
			{
			// apply the default color
			textVisual->SetColor(KRgbWhite);
			}
       }
	
	}

// ---------------------------------------------------------------------------
// ApplySliderAttribute
// ---------------------------------------------------------------------------
//      		
void MulCoverFlowTemplate1::ApplySliderAttribute(const TRect& aMainRect)
	{
    int sliderPaneVerityId = 1;
    
    TAknWindowLineLayout componentLayout =
        AknLayoutScalable_UiAccel::aacf_slider_pane(sliderPaneVerityId).LayoutLine();
    TRect sliderRect = IconWindowRect(componentLayout, aMainRect);
    TSize sliderSize(sliderRect.Size()); 
    mData->mSliderDimensions.height = sliderSize.iHeight;
    mData->mSliderDimensions.width = sliderSize.iWidth;
    mData->mSliderDimensions.posx = sliderRect.iTl.iX;
    mData->mSliderDimensions.posy = sliderRect.iTl.iY;
	}
	
    }//namespace ends
