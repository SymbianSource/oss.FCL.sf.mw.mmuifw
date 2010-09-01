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

 
#include "mulcovertemplates.h"
#include "mulcoverflowdefinitions.h"
#include "mulvisualutility.h"
#include "mulutility.h"
#include <osn/osnnew.h> 

namespace Alf
    {
    
MulCoverLogicalTemplate3::~MulCoverLogicalTemplate3()
    {
    
    }
    
//----------------------------------------------------------------
// PopulateData
//----------------------------------------------------------------
//
void MulCoverLogicalTemplate3::PopulateData ()
    {	
    //For base element
    LCTElementStructure* prtBaseElem = new (EMM) LCTElementStructure();
	//for main layout
	prtBaseElem->SetId(UString(KBase));
	LCTVisualStructure* mainPrtVS = new (EMM) LCTVisualStructure();
	mainPrtVS->SetId(UString(KMainLayout));
	//@todo:: later these need to be retrieved from LCT utility
	mainPrtVS->SetPositions(0,0);
	mainPrtVS->SetDimensions(360,519);
	mainPrtVS->SetLCTId(UString("uiaccel_main_aa_coverflow_pane"));
	mainPrtVS->SetVarietyId(0);	
	CustomAttributeStructure* mainPrtOpacity = new (EMM) CustomAttributeStructure(
                UString(KOpacity), UString("1.0"));    
	mainPrtVS->AddAttribute(mainPrtOpacity);
   CustomAttributeStructure* mainPrtMCount = new (EMM) CustomAttributeStructure(
                    UString(KTempMaxCount), UString("3"));  
    mainPrtVS->AddAttribute(mainPrtMCount);
	prtBaseElem->AddVisualStructure(mainPrtVS);
	//for item_center
	LCTVisualStructure* item_centerPrtVS = new (EMM) LCTVisualStructure();
	item_centerPrtVS->SetId(UString(KItemCentre));
	item_centerPrtVS->SetPositions(10,22);
	item_centerPrtVS->SetDimensions(340,340);
	item_centerPrtVS->SetLCTId(UString("uiaccel_aacf_image_pane"));
	item_centerPrtVS->SetVarietyId(0);
	
	CustomAttributeStructure* item_centerPrtFM = new (EMM) CustomAttributeStructure(
	        UString(KFitMode), UString(KOff));
	item_centerPrtVS->AddAttribute(item_centerPrtFM);
	prtBaseElem->AddVisualStructure(item_centerPrtVS);
	//for counter visual
	LCTVisualStructure* counterPrtVS = new (EMM) LCTVisualStructure();
	counterPrtVS->SetId(UString(KMulCount));
	counterPrtVS->SetPositions(238,0);
	counterPrtVS->SetDimensions(112,21);
	counterPrtVS->SetLCTId(UString("uiaccel_main_aa_coverflow_pane_t1"));
	counterPrtVS->SetVarietyId(0);
	//add extra attributes to the countervisual
		
	CustomAttributeStructure* stylePrtC = new (EMM) CustomAttributeStructure(
	        UString(KTextStyle), UString("qfn_secondary"));
	
	CustomAttributeStructure* opacityPrtC = new (EMM) CustomAttributeStructure(
	        UString(KOpacity), UString("1.0"));
	
	CustomAttributeStructure* redPrtC = new (EMM) CustomAttributeStructure(
	        UString(KRedColorAttrib), UString("255"));
	
     CustomAttributeStructure* greenPrtC = new (EMM) CustomAttributeStructure(
                UString(KGreenColorAttrib), UString("255"));
     
    CustomAttributeStructure* bluePrtC = new (EMM) CustomAttributeStructure(
        UString(KBlueColorAttrib), UString("255"));
    
    CustomAttributeStructure* alphaPrtC = new (EMM) CustomAttributeStructure(
        UString(KAlphaColorAttrib), UString("0"));
    
    CustomAttributeStructure* horizontalalignPrtC = new (EMM) CustomAttributeStructure(
            UString("horizontalalign"), UString("2"));
    	    
    counterPrtVS->AddAttribute(stylePrtC);
    counterPrtVS->AddAttribute(opacityPrtC);
    counterPrtVS->AddAttribute(redPrtC);
    counterPrtVS->AddAttribute(greenPrtC);
    counterPrtVS->AddAttribute(bluePrtC);
    counterPrtVS->AddAttribute(alphaPrtC);
    counterPrtVS->AddAttribute(horizontalalignPrtC);   
    
	prtBaseElem->AddVisualStructure(counterPrtVS);
	
	//for text layout
	LCTVisualStructure* textPrtLtVS = new (EMM) LCTVisualStructure();
	textPrtLtVS->SetId(UString(KText));
	textPrtLtVS->SetPositions(20,360);
	textPrtLtVS->SetDimensions(340,82);
	textPrtLtVS->SetLCTId(UString("uiaccel_aacf_image_title_pane"));
	textPrtLtVS->SetVarietyId(4);
	prtBaseElem->AddVisualStructure(textPrtLtVS);
	
	//for text background
	LCTVisualStructure* textPrtBgVS = new (EMM) LCTVisualStructure();
	textPrtBgVS->SetId(UString(KMulBackGround));
	textPrtBgVS->SetPositions(0,0);
	textPrtBgVS->SetDimensions(340,82);
	textPrtBgVS->SetLCTId(UString("uiaccel_aacf_image_title_pane_g1"));
	textPrtBgVS->SetVarietyId(0);
    //Add attributes to the text background visual
	CustomAttributeStructure* bgHaPrtTV = new (EMM) CustomAttributeStructure(
                UString(KHorzAlign), UString("1"));
	textPrtBgVS->AddAttribute(bgHaPrtTV);
    CustomAttributeStructure* bgoPrtTV = new (EMM) CustomAttributeStructure(
        UString(KOpacity), UString("0.0"));
    textPrtBgVS->AddAttribute(bgoPrtTV);
    CustomAttributeStructure* bgcPrtTV = new (EMM) CustomAttributeStructure(
            UString(KColor), UString("0"));
    textPrtBgVS->AddAttribute(bgcPrtTV);
    prtBaseElem->AddVisualStructure(textPrtBgVS);
    
    //for text title
    LCTVisualStructure* textTPrtVS = new (EMM) LCTVisualStructure();
    textTPrtVS->SetId(UString(KMulTitle));
    textTPrtVS->SetPositions(15,9);
    textTPrtVS->SetDimensions(310,27);
    textTPrtVS->SetLCTId(UString("uiaccel_aacf_image_title_pane_t1"));
    textTPrtVS->SetVarietyId(0);
    //add extra attributes to the countervisual
    CustomAttributeStructure* textTStylePrt = new (EMM) CustomAttributeStructure(
             UString(KTextStyle), UString("qfn_secondary"));
    textTPrtVS->AddAttribute(textTStylePrt);
    
    CustomAttributeStructure* textTFtcPrt = new (EMM) CustomAttributeStructure(
            UString(KFontThemeColor), UString("9"));
    textTPrtVS->AddAttribute(textTFtcPrt);
    
    CustomAttributeStructure* textTHaPrt = new (EMM) CustomAttributeStructure(
            UString(KHorzAlign), UString("1"));
    textTPrtVS->AddAttribute(textTHaPrt);
    
    CustomAttributeStructure* textTVaPrt = new (EMM) CustomAttributeStructure(
            UString(KVertAlign), UString("1"));
    textTPrtVS->AddAttribute(textTVaPrt);
    
    CustomAttributeStructure* textTRedPrt = new (EMM) CustomAttributeStructure(
            UString(KRedColorAttrib), UString("255"));
    textTPrtVS->AddAttribute(textTRedPrt);
    
     CustomAttributeStructure* textTGreenPrt = new (EMM) CustomAttributeStructure(
                UString(KGreenColorAttrib), UString("255"));
     textTPrtVS->AddAttribute(textTGreenPrt);
     
    CustomAttributeStructure* textTBluePrt = new (EMM) CustomAttributeStructure(
        UString(KBlueColorAttrib), UString("255"));
    textTPrtVS->AddAttribute(textTBluePrt);
    
    CustomAttributeStructure* textTAlphaPrt = new (EMM) CustomAttributeStructure(
        UString(KAlphaColorAttrib), UString("0"));
    textTPrtVS->AddAttribute(textTAlphaPrt);
    
    CustomAttributeStructure* textTTsciPrt = new (EMM) CustomAttributeStructure(
            UString(KTextSkinColorId), UString("KAknsIIDQsnTextColors"));
    textTPrtVS->AddAttribute(textTTsciPrt);
    
    CustomAttributeStructure* textTTcsiPrt = new (EMM) CustomAttributeStructure(
        UString(KTextColorSkinIndex), UString("5"));
    textTPrtVS->AddAttribute(textTTcsiPrt);
    
    CustomAttributeStructure* textTOpacityPrt = new (EMM) CustomAttributeStructure(
        UString(KOpacity), UString("1.0"));
    textTPrtVS->AddAttribute(textTOpacityPrt);
    
    prtBaseElem->AddVisualStructure(textTPrtVS);
    
    //for text detail
    LCTVisualStructure* textDPrtVS = new (EMM) LCTVisualStructure();
    textDPrtVS->SetId(UString(KMulDetail));
    textDPrtVS->SetPositions(15,46);
    textDPrtVS->SetDimensions(310,27);
    textDPrtVS->SetLCTId(UString("uiaccel_aacf_image_title_pane_t2"));
    textDPrtVS->SetVarietyId(0);
    //add extra attributes to the countervisual
    CustomAttributeStructure* textDStylePrt = new (EMM) CustomAttributeStructure(
             UString(KTextStyle), UString("qfn_secondary"));
    textDPrtVS->AddAttribute(textDStylePrt);
    
    CustomAttributeStructure* textDFtcPrt = new (EMM) CustomAttributeStructure(
            UString(KFontThemeColor), UString("9"));
    textDPrtVS->AddAttribute(textDFtcPrt);
    
    CustomAttributeStructure* textDHaPrt = new (EMM) CustomAttributeStructure(
            UString(KHorzAlign), UString("1"));
    textDPrtVS->AddAttribute(textDHaPrt);
    
    CustomAttributeStructure* textDVaPrt = new (EMM) CustomAttributeStructure(
            UString(KVertAlign), UString("1"));
    textDPrtVS->AddAttribute(textDVaPrt);
    
    CustomAttributeStructure* textDRedPrt = new (EMM) CustomAttributeStructure(
            UString(KRedColorAttrib), UString("255"));
    textDPrtVS->AddAttribute(textDRedPrt);
    
     CustomAttributeStructure* textDGreenPrt = new (EMM) CustomAttributeStructure(
                UString(KGreenColorAttrib), UString("255"));
     textDPrtVS->AddAttribute(textDGreenPrt);
     
    CustomAttributeStructure* textDBluePrt = new (EMM) CustomAttributeStructure(
        UString(KBlueColorAttrib), UString("255"));
    textDPrtVS->AddAttribute(textDBluePrt);
    
    CustomAttributeStructure* textDAlphaPrt = new (EMM) CustomAttributeStructure(
        UString(KAlphaColorAttrib), UString("0"));
    textDPrtVS->AddAttribute(textDAlphaPrt);
    
    CustomAttributeStructure* textDTsciPrt = new (EMM) CustomAttributeStructure(
            UString(KTextSkinColorId), UString("KAknsIIDQsnTextColors"));
    textDPrtVS->AddAttribute(textDTsciPrt);
    
    CustomAttributeStructure* textDTcsiPrt = new (EMM) CustomAttributeStructure(
        UString(KTextColorSkinIndex), UString("5"));
    textDPrtVS->AddAttribute(textDTcsiPrt);
    
    CustomAttributeStructure* textDOpacityPrt = new (EMM) CustomAttributeStructure(
        UString(KOpacity), UString("0.7"));
    textDPrtVS->AddAttribute(textDOpacityPrt);
    
    CustomAttributeStructure* textDMarqueePrt = new (EMM) CustomAttributeStructure(
       UString(KMarquee), UString("yes"));
   textDPrtVS->AddAttribute(textDMarqueePrt);
    
    prtBaseElem->AddVisualStructure(textDPrtVS);
  	
	mTemplatePrtStruct.push_back(prtBaseElem);
    
    //now populate data for slider element
    //----------------------------------------------------------------------
    LCTElementStructure* prtSliderElem = new (EMM) LCTElementStructure();
    prtSliderElem->SetId(UString(KSlider)); 
    
    LCTVisualStructure* prtSliderVis = new (EMM)LCTVisualStructure();
    prtSliderVis->SetPositions(0,441);
    prtSliderVis->SetDimensions(340,78);
    prtSliderVis->SetLCTId(UString("uiaccel_aacf_slider_pane"));
    prtSliderVis->SetVarietyId(0);
    
    CustomAttributeStructure* sliderPrtOpacity = new CustomAttributeStructure(
         UString(KTemplateId),UString("1"));
    prtSliderVis->AddAttribute(sliderPrtOpacity);
    
    CustomAttributeStructure* sliderPrtTemplate = new CustomAttributeStructure(
            UString(KOpacity),UString("1.0"),EAttributeOpacity);
    prtSliderVis->AddAttribute(sliderPrtTemplate);

    prtSliderElem->AddVisualStructure(prtSliderVis);
    
    mTemplatePrtStruct.push_back(prtSliderElem);
	//==========================================================================	
	//same procedure for filling up of LSC
	//==========================================================================
	//for main layout
	LCTElementStructure* lscBaseElem = new (EMM) LCTElementStructure();
	lscBaseElem->SetId(UString(KBase));
	LCTVisualStructure* mainLscVS2 = new (EMM) LCTVisualStructure();
	mainLscVS2->SetId(UString(KMainLayout));
	mainLscVS2->SetPositions(0,0);
	mainLscVS2->SetDimensions(640,360);
	mainLscVS2->SetLCTId(UString("uiaccel_main_aa_coverflow_pane"));
	mainLscVS2->SetVarietyId(1);	
	CustomAttributeStructure* mainLscOpacity = new (EMM) CustomAttributeStructure(
	            UString(KOpacity), UString("1.0"));	
	mainLscVS2->AddAttribute(mainLscOpacity);
	CustomAttributeStructure* mainLscMCount = new (EMM) CustomAttributeStructure(
                        UString(KTempMaxCount), UString("3"));  
    mainLscVS2->AddAttribute(mainLscMCount);
    CustomAttributeStructure* mainLscFScroll = new (EMM) CustomAttributeStructure(
                        UString(KFastScroll), UString("0"));  
    mainLscVS2->AddAttribute(mainLscFScroll);
	lscBaseElem->AddVisualStructure(mainLscVS2);	
	//for item_center
	LCTVisualStructure* item_centerLscVs = new (EMM) LCTVisualStructure();
	item_centerLscVs->SetId(UString(KCentreItem));
	item_centerLscVs->SetPositions(198,37);
	item_centerLscVs->SetDimensions(242,242);
	item_centerLscVs->SetLCTId(UString("uiaccel_aacf_image_pane"));
	item_centerLscVs->SetVarietyId(2);
	CustomAttributeStructure* item_centerLscFM = new (EMM) CustomAttributeStructure(
	        UString(KFitMode), UString(KOff));
	item_centerLscVs->AddAttribute(item_centerLscFM);
	lscBaseElem->AddVisualStructure(item_centerLscVs);
	
	//For item right
	LCTVisualStructure* item_rightLscVs = new (EMM) LCTVisualStructure();
	item_rightLscVs->SetId(UString(KRightItem));
	item_rightLscVs->SetPositions(480,50);
	item_rightLscVs->SetDimensions(214,214);
	item_rightLscVs->SetLCTId(UString("uiaccel_aacf_image_pane_cp002"));
	item_rightLscVs->SetVarietyId(2);
    lscBaseElem->AddVisualStructure(item_rightLscVs);
	// For Counter
	LCTVisualStructure* counterLscVS = new (EMM) LCTVisualStructure();
	counterLscVS->SetId(UString(KMulCount));
	counterLscVS->SetPositions(522,6);
	counterLscVS->SetDimensions(112,21);
	counterLscVS->SetLCTId(UString("uiaccel_main_aa_coverflow_pane_t1"));
	counterLscVS->SetVarietyId(2);
    //add extra attributes to the gridlayout
        
    CustomAttributeStructure* styleLscC = new (EMM) CustomAttributeStructure(
            UString(KTextStyle), UString("qfn_secondary"));
    
    CustomAttributeStructure* opacityLscC = new (EMM) CustomAttributeStructure(
            UString(KOpacity), UString("1.0"));
    
    CustomAttributeStructure* redLscC = new (EMM) CustomAttributeStructure(
            UString(KRedColorAttrib), UString("255"));
    
     CustomAttributeStructure* greenLscC = new (EMM) CustomAttributeStructure(
                UString(KGreenColorAttrib), UString("255"));
     
    CustomAttributeStructure* blueLscC = new (EMM) CustomAttributeStructure(
        UString(KBlueColorAttrib), UString("255"));
    
    CustomAttributeStructure* alphaLscC = new (EMM) CustomAttributeStructure(
        UString(KAlphaColorAttrib), UString("0"));
    
    CustomAttributeStructure* horizontalalignLscC = new (EMM) CustomAttributeStructure(
            UString(KHorzAlign), UString("2"));
           
    counterLscVS->AddAttribute(styleLscC);
    counterLscVS->AddAttribute(opacityLscC);
    counterLscVS->AddAttribute(redLscC);
    counterLscVS->AddAttribute(greenLscC);
    counterLscVS->AddAttribute(blueLscC);
    counterLscVS->AddAttribute(alphaLscC);
    counterLscVS->AddAttribute(horizontalalignLscC);
    
	lscBaseElem->AddVisualStructure(counterLscVS);
	
	//for text layout
    LCTVisualStructure* textLscLtVS = new (EMM) LCTVisualStructure();
    textLscLtVS->SetId(UString(KText));
    textLscLtVS->SetPositions(161,284);
    textLscLtVS->SetDimensions(318,72);
    textLscLtVS->SetLCTId(UString("uiaccel_aacf_image_title_pane"));
    textLscLtVS->SetVarietyId(2);
    lscBaseElem->AddVisualStructure(textLscLtVS);
    
    //for text background
    LCTVisualStructure* textLscBgVS = new (EMM) LCTVisualStructure();
    textLscBgVS->SetId(UString(KMulBackGround));
    textLscBgVS->SetPositions(0,0);
    textLscBgVS->SetDimensions(318,72);
    textLscBgVS->SetLCTId(UString("uiaccel_aacf_image_title_pane_g1"));
    textLscBgVS->SetVarietyId(1);
    //Add attributes to the text background visual
    CustomAttributeStructure* bgHaLscTV = new (EMM) CustomAttributeStructure(
                UString(KHorzAlign), UString("1"));
    textLscBgVS->AddAttribute(bgHaLscTV);
    CustomAttributeStructure* bgoLscTV = new (EMM) CustomAttributeStructure(
        UString(KOpacity), UString("0.0"));
    textLscBgVS->AddAttribute(bgoLscTV);
    CustomAttributeStructure* bgcLscTV = new (EMM) CustomAttributeStructure(
            UString(KColor), UString("0"));
    textLscBgVS->AddAttribute(bgcLscTV);
    lscBaseElem->AddVisualStructure(textLscBgVS);
    
    //for text title
    LCTVisualStructure* textTLscVS = new (EMM) LCTVisualStructure();
    textTLscVS->SetId(UString(KMulTitle));
    textTLscVS->SetPositions(4,4);
    textTLscVS->SetDimensions(310,27);
    textTLscVS->SetLCTId(UString("uiaccel_aacf_image_title_pane_t1"));
    textTLscVS->SetVarietyId(1);
    //add extra attributes to the countervisual
    CustomAttributeStructure* textTStyleLsc = new (EMM) CustomAttributeStructure(
             UString(KTextStyle), UString("qfn_secondary"));
    textTLscVS->AddAttribute(textTStyleLsc);
    
    CustomAttributeStructure* textTFtcLsc = new (EMM) CustomAttributeStructure(
            UString(KFontThemeColor), UString("9"));
    textTLscVS->AddAttribute(textTFtcLsc);
    
    CustomAttributeStructure* textTHaLsc = new (EMM) CustomAttributeStructure(
            UString(KHorzAlign), UString("1"));
    textTLscVS->AddAttribute(textTHaLsc);
    
    CustomAttributeStructure* textTVaLsc = new (EMM) CustomAttributeStructure(
            UString(KVertAlign), UString("1"));
    textTLscVS->AddAttribute(textTVaLsc);
    
    CustomAttributeStructure* textTRedLsc = new (EMM) CustomAttributeStructure(
            UString(KRedColorAttrib), UString("255"));
    textTLscVS->AddAttribute(textTRedLsc);
    
     CustomAttributeStructure* textTGreenLsc = new (EMM) CustomAttributeStructure(
                UString(KGreenColorAttrib), UString("255"));
     textTLscVS->AddAttribute(textTGreenLsc);
     
    CustomAttributeStructure* textTBlueLsc = new (EMM) CustomAttributeStructure(
        UString(KBlueColorAttrib), UString("255"));
    textTLscVS->AddAttribute(textTBlueLsc);
    
    CustomAttributeStructure* textTAlphaLsc = new (EMM) CustomAttributeStructure(
        UString(KAlphaColorAttrib), UString("0"));
    textTLscVS->AddAttribute(textTAlphaLsc);
    
    CustomAttributeStructure* textTTsciLsc = new (EMM) CustomAttributeStructure(
            UString(KTextSkinColorId), UString("KAknsIIDQsnTextColors"));
    textTLscVS->AddAttribute(textTTsciLsc);
    
    CustomAttributeStructure* textTTcsiLsc = new (EMM) CustomAttributeStructure(
        UString(KTextColorSkinIndex), UString("5"));
    textTLscVS->AddAttribute(textTTcsiLsc);
    
    CustomAttributeStructure* textTOpacityLsc = new (EMM) CustomAttributeStructure(
        UString(KOpacity), UString("1.0"));
    textTLscVS->AddAttribute(textTOpacityLsc);
    
    lscBaseElem->AddVisualStructure(textTLscVS);
    
    //for text detail
    LCTVisualStructure* textDLscVS = new (EMM) LCTVisualStructure();
    textDLscVS->SetId(UString(KMulDetail));
    textDLscVS->SetPositions(4,38);
    textDLscVS->SetDimensions(310,27);
    textDLscVS->SetLCTId(UString("uiaccel_aacf_image_title_pane_t2"));
    textDLscVS->SetVarietyId(1);
    //add extra attributes to the countervisual
    CustomAttributeStructure* textDStyleLsc = new (EMM) CustomAttributeStructure(
             UString(KTextStyle), UString("qfn_secondary"));
    textDLscVS->AddAttribute(textDStyleLsc);
    
    CustomAttributeStructure* textDFtcLsc = new (EMM) CustomAttributeStructure(
            UString(KFontThemeColor), UString("9"));
    textDLscVS->AddAttribute(textDFtcLsc);
    
    CustomAttributeStructure* textDHaLsc = new (EMM) CustomAttributeStructure(
            UString(KHorzAlign), UString("1"));
    textDLscVS->AddAttribute(textDHaLsc);
    
    CustomAttributeStructure* textDVaLsc = new (EMM) CustomAttributeStructure(
            UString(KVertAlign), UString("1"));
    textDLscVS->AddAttribute(textDVaLsc);
    
    CustomAttributeStructure* textDRedLsc = new (EMM) CustomAttributeStructure(
            UString(KRedColorAttrib), UString("255"));
    textDLscVS->AddAttribute(textDRedLsc);
    
     CustomAttributeStructure* textDGreenLsc = new (EMM) CustomAttributeStructure(
                UString(KGreenColorAttrib), UString("255"));
     textDLscVS->AddAttribute(textDGreenLsc);
     
    CustomAttributeStructure* textDBlueLsc = new (EMM) CustomAttributeStructure(
        UString(KBlueColorAttrib), UString("255"));
    textDLscVS->AddAttribute(textDBlueLsc);
    
    CustomAttributeStructure* textDAlphaLsc = new (EMM) CustomAttributeStructure(
        UString(KAlphaColorAttrib), UString("0"));
    textDLscVS->AddAttribute(textDAlphaLsc);
    
    CustomAttributeStructure* textDTsciLsc = new (EMM) CustomAttributeStructure(
            UString(KTextSkinColorId), UString("KAknsIIDQsnTextColors"));
    textDLscVS->AddAttribute(textDTsciLsc);
    
    CustomAttributeStructure* textDTcsiLsc = new (EMM) CustomAttributeStructure(
        UString(KTextColorSkinIndex), UString("5"));
    textDLscVS->AddAttribute(textDTcsiLsc);
    
    CustomAttributeStructure* textDOpacityLsc = new (EMM) CustomAttributeStructure(
        UString(KOpacity), UString("0.7"));
    textDLscVS->AddAttribute(textDOpacityLsc);
    
    CustomAttributeStructure* textDMarqueeLsc = new (EMM) CustomAttributeStructure(
        UString(KMarquee), UString("yes"));
    textDLscVS->AddAttribute(textDMarqueeLsc);
    
    lscBaseElem->AddVisualStructure(textDLscVS);
	
	mTemplateLscStruct.push_back(lscBaseElem);
	
       }
		    
    }//namespace ends
