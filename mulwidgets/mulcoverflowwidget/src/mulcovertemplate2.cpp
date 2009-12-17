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
    
MulCoverLogicalTemplate2::~MulCoverLogicalTemplate2()
    {
    
    }

//----------------------------------------------------------------
// PopulateData
//----------------------------------------------------------------
//    
void MulCoverLogicalTemplate2::PopulateData ()
    {	
    //For base element
    LCTElementStructure* prtBaseElem = new (EMM) LCTElementStructure();
	//for main layout
	prtBaseElem->SetId(UString(KBase));
	LCTVisualStructure* mainPrtVS = new (EMM) LCTVisualStructure();
	mainPrtVS->SetId(UString(KMainLayout));
	//@todo:: later these need to be retrieved from LCT utility
	mainPrtVS->SetPositions(0,0);
	mainPrtVS->SetDimensions(360,441);
	mainPrtVS->SetLCTId(UString("uiaccel_main_aa_coverflow_pane"));
	mainPrtVS->SetVarietyId(3);	
	CustomAttributeStructure* mainPrtOpacity = new (EMM) CustomAttributeStructure(
                UString("opacity"), UString("1.0"));    
	mainPrtVS->AddAttribute(mainPrtOpacity);
	prtBaseElem->AddVisualStructure(mainPrtVS);
	//for item_center
	LCTVisualStructure* item_centerPrtVS = new (EMM) LCTVisualStructure();
	item_centerPrtVS->SetId(UString(KItemCentre));
	item_centerPrtVS->SetPositions(10,22);
	item_centerPrtVS->SetDimensions(340,340);
	item_centerPrtVS->SetLCTId(UString("uiaccel_aacf_image_pane"));
	item_centerPrtVS->SetVarietyId(0);
	
	CustomAttributeStructure* item_centerPrtFM = new (EMM) CustomAttributeStructure(
	        UString("fitmode"), UString("off"));
	item_centerPrtVS->AddAttribute(item_centerPrtFM);
	prtBaseElem->AddVisualStructure(item_centerPrtVS);
	//for counter visual
	LCTVisualStructure* counterPrtVS = new (EMM) LCTVisualStructure();
	counterPrtVS->SetId(UString(KMulCount));
	counterPrtVS->SetPositions(238,0);
	counterPrtVS->SetDimensions(112,21);
	counterPrtVS->SetLCTId(UString("uiaccel_main_aa_coverflow_pane_t1"));
	counterPrtVS->SetVarietyId(0);
	//add extra attributes to the gridlayout
		
	CustomAttributeStructure* stylePrtC = new (EMM) CustomAttributeStructure(
	        UString("style"), UString("qfn_secondary"));
	
	CustomAttributeStructure* opacityPrtC = new (EMM) CustomAttributeStructure(
	        UString("opacity"), UString("1.0"));
	
	CustomAttributeStructure* redPrtC = new (EMM) CustomAttributeStructure(
	        UString("red"), UString("255"));
	
     CustomAttributeStructure* greenPrtC = new (EMM) CustomAttributeStructure(
                UString("green"), UString("255"));
     
    CustomAttributeStructure* bluePrtC = new (EMM) CustomAttributeStructure(
        UString("blue"), UString("255"));
    
    CustomAttributeStructure* alphaPrtC = new (EMM) CustomAttributeStructure(
        UString("alpha"), UString("0"));
    
    CustomAttributeStructure* horizontalalignPrtC = new (EMM) CustomAttributeStructure(
            UString("horizontalalign"), UString("2"));
        
    counterPrtVS->AddAttribute(stylePrtC);
    counterPrtVS->AddAttribute(opacityPrtC);
    counterPrtVS->AddAttribute(redPrtC);
    counterPrtVS->AddAttribute(greenPrtC);
    counterPrtVS->AddAttribute(bluePrtC);
    counterPrtVS->AddAttribute(alphaPrtC);
    counterPrtVS->AddAttribute(horizontalalignPrtC);
    
    //for text layout
    LCTVisualStructure* textPrtLtVS = new (EMM) LCTVisualStructure();
    textPrtLtVS->SetId(UString(KText));
    textPrtLtVS->SetPositions(10,230);
    textPrtLtVS->SetDimensions(340,82);
    textPrtLtVS->SetLCTId(UString("uiaccel_aacf_image_title_pane"));
    textPrtLtVS->SetVarietyId(0);
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
                UString("horizontalalign"), UString("1"));
    textPrtBgVS->AddAttribute(bgHaPrtTV);
    CustomAttributeStructure* bgoPrtTV = new (EMM) CustomAttributeStructure(
        UString("opacity"), UString("0.5"));
    textPrtBgVS->AddAttribute(bgoPrtTV);
    CustomAttributeStructure* bgcPrtTV = new (EMM) CustomAttributeStructure(
            UString("colour"), UString("0"));
    textPrtBgVS->AddAttribute(bgcPrtTV);
    prtBaseElem->AddVisualStructure(textPrtBgVS);
    
    //for text title
    LCTVisualStructure* textTPrtVS = new (EMM) LCTVisualStructure();
    textTPrtVS->SetId(UString(KMulTitle));
    textTPrtVS->SetPositions(10,9);
    textTPrtVS->SetDimensions(320,27);
    textTPrtVS->SetLCTId(UString("uiaccel_aacf_image_title_pane_t1"));
    textTPrtVS->SetVarietyId(0);
    //add extra attributes to the countervisual
    CustomAttributeStructure* textTStylePrt = new (EMM) CustomAttributeStructure(
             UString("style"), UString("qfn_secondary"));
    textTPrtVS->AddAttribute(textTStylePrt);
    
    CustomAttributeStructure* textTFtcPrt = new (EMM) CustomAttributeStructure(
            UString("fontthemecolor"), UString("9"));
    textTPrtVS->AddAttribute(textTFtcPrt);
    
    CustomAttributeStructure* textTHaPrt = new (EMM) CustomAttributeStructure(
            UString("horizontalalign"), UString("1"));
    textTPrtVS->AddAttribute(textTHaPrt);
    
    CustomAttributeStructure* textTVaPrt = new (EMM) CustomAttributeStructure(
            UString("verticalalign"), UString("1"));
    textTPrtVS->AddAttribute(textTVaPrt);
    
    CustomAttributeStructure* textTRedPrt = new (EMM) CustomAttributeStructure(
            UString("red"), UString("255"));
    textTPrtVS->AddAttribute(textTRedPrt);
    
     CustomAttributeStructure* textTGreenPrt = new (EMM) CustomAttributeStructure(
                UString("green"), UString("255"));
     textTPrtVS->AddAttribute(textTGreenPrt);
     
    CustomAttributeStructure* textTBluePrt = new (EMM) CustomAttributeStructure(
        UString("blue"), UString("255"));
    textTPrtVS->AddAttribute(textTBluePrt);
    
    CustomAttributeStructure* textTAlphaPrt = new (EMM) CustomAttributeStructure(
        UString("alpha"), UString("0"));
    textTPrtVS->AddAttribute(textTAlphaPrt);
    
    CustomAttributeStructure* textTTsciPrt = new (EMM) CustomAttributeStructure(
            UString("textskincolorid"), UString("KAknsIIDQsnTextColors"));
    textTPrtVS->AddAttribute(textTTsciPrt);
    
    CustomAttributeStructure* textTTcsiPrt = new (EMM) CustomAttributeStructure(
        UString("textcolorskinindex"), UString("5"));
    textTPrtVS->AddAttribute(textTTcsiPrt);
    
    CustomAttributeStructure* textTOpacityPrt = new (EMM) CustomAttributeStructure(
        UString("opacity"), UString("1.0"));
    textTPrtVS->AddAttribute(textTOpacityPrt);
    
    prtBaseElem->AddVisualStructure(textTPrtVS);
    
    //for text detail
    LCTVisualStructure* textDPrtVS = new (EMM) LCTVisualStructure();
    textDPrtVS->SetId(UString(KMulDetail));
    textDPrtVS->SetPositions(10,46);
    textDPrtVS->SetDimensions(320,27);
    textDPrtVS->SetLCTId(UString("uiaccel_aacf_image_title_pane_t2"));
    textDPrtVS->SetVarietyId(0);
    //add extra attributes to the countervisual
    CustomAttributeStructure* textDStylePrt = new (EMM) CustomAttributeStructure(
             UString("style"), UString("qfn_secondary"));
    textDPrtVS->AddAttribute(textDStylePrt);
    
    CustomAttributeStructure* textDFtcPrt = new (EMM) CustomAttributeStructure(
            UString("fontthemecolor"), UString("9"));
    textDPrtVS->AddAttribute(textDFtcPrt);
    
    CustomAttributeStructure* textDHaPrt = new (EMM) CustomAttributeStructure(
            UString("horizontalalign"), UString("1"));
    textDPrtVS->AddAttribute(textDHaPrt);
    
    CustomAttributeStructure* textDVaPrt = new (EMM) CustomAttributeStructure(
            UString("verticalalign"), UString("1"));
    textDPrtVS->AddAttribute(textDVaPrt);
    
    CustomAttributeStructure* textDRedPrt = new (EMM) CustomAttributeStructure(
            UString("red"), UString("255"));
    textDPrtVS->AddAttribute(textTRedPrt);
    
     CustomAttributeStructure* textDGreenPrt = new (EMM) CustomAttributeStructure(
                UString("green"), UString("255"));
     textDPrtVS->AddAttribute(textTGreenPrt);
     
    CustomAttributeStructure* textDBluePrt = new (EMM) CustomAttributeStructure(
        UString("blue"), UString("255"));
    textDPrtVS->AddAttribute(textTBluePrt);
    
    CustomAttributeStructure* textDAlphaPrt = new (EMM) CustomAttributeStructure(
        UString("alpha"), UString("0"));
    textDPrtVS->AddAttribute(textTAlphaPrt);
    
    CustomAttributeStructure* textDTsciPrt = new (EMM) CustomAttributeStructure(
            UString("textskincolorid"), UString("KAknsIIDQsnTextColors"));
    textDPrtVS->AddAttribute(textTTsciPrt);
    
    CustomAttributeStructure* textDTcsiPrt = new (EMM) CustomAttributeStructure(
        UString("textcolorskinindex"), UString("5"));
    textDPrtVS->AddAttribute(textDTcsiPrt);
    
    CustomAttributeStructure* textDOpacityPrt = new (EMM) CustomAttributeStructure(
        UString("opacity"), UString("0.7"));
    textDPrtVS->AddAttribute(textDOpacityPrt);
    
    CustomAttributeStructure* textDMarqueePrt = new (EMM) CustomAttributeStructure(
        UString("marquee"), UString("yes"));
    textDPrtVS->AddAttribute(textDMarqueePrt);
    
    prtBaseElem->AddVisualStructure(textDPrtVS);

	prtBaseElem->AddVisualStructure(counterPrtVS);
		
	mTemplatePrtStruct.push_back(prtBaseElem);
	
	//now populate data for prtNonhilightElem element
    //----------------------------------------------------------------------
    LCTElementStructure* prtNonhilightElem = new (EMM) LCTElementStructure();
    prtNonhilightElem->SetId(UString("nohighlight"));

    //for backgroung of non-highlighted imagevisual        
    LCTVisualStructure* nHPrtDeckVs = new (EMM) LCTVisualStructure();
    nHPrtDeckVs->SetId(UString("deck"));
    CustomAttributeStructure* nHPrtDeckOpacity = new CustomAttributeStructure(
                UString("opacity"),UString("1.0"),EAttributeOpacity);
    nHPrtDeckVs->AddAttribute(nHPrtDeckOpacity);
    prtNonhilightElem->AddVisualStructure(nHPrtDeckVs);
	    
    //for non-highlighted imagevisual
    LCTVisualStructure* nonHPrtIcon = new (EMM) LCTVisualStructure();
    nonHPrtIcon->SetVisualType(EVisualImage);
    nonHPrtIcon->SetId(UString("mul_icon"));
    nonHPrtIcon->SetPositions(0,0);
    nonHPrtIcon->SetDimensions(340,340);
    nonHPrtIcon->SetLCTId(UString("uiaccel_aacf_image_pane_g1"));
    nonHPrtIcon->SetVarietyId(0);
    
    //ADD EXTRA ATTRIBUTE FOR FITMODE
       
    CustomAttributeStructure* nonHIPath = new CustomAttributeStructure(
        UString("imagepath"),UString("mul_icon"),EAttributeNone,
        ECategoryData);
    nonHPrtIcon->AddAttribute(nonHIPath);
    
    CustomAttributeStructure* nHIOpacity = new CustomAttributeStructure(
        UString("opacity"),UString("1.0"),EAttributeOpacity);
    nonHPrtIcon->AddAttribute(nHIOpacity);
    
    prtNonhilightElem->AddVisualStructure(nonHPrtIcon);
    
    mTemplatePrtStruct.push_back(prtNonhilightElem);
    
    //now populate data for prthilightElem element
    //----------------------------------------------------------------------
    LCTElementStructure* prtHilightElem = new (EMM) LCTElementStructure();
    prtHilightElem->SetId(UString("highlight"));

    //for backgroung of non-highlighted imagevisual        
    LCTVisualStructure* hPrtDeckVs = new (EMM) LCTVisualStructure();
    hPrtDeckVs->SetId(UString("deck"));
    CustomAttributeStructure* hDeckOpacity = new CustomAttributeStructure(
            UString("opacity"),UString("1.0"),EAttributeOpacity);
    hPrtDeckVs->AddAttribute(hDeckOpacity);
    prtHilightElem->AddVisualStructure(hPrtDeckVs);
        
    //for non-highlighted imagevisual
    LCTVisualStructure* hPrtIcon = new (EMM) LCTVisualStructure();
    hPrtIcon->SetVisualType(EVisualImage);
    hPrtIcon->SetId(UString("mul_icon"));
    hPrtIcon->SetPositions(0,0);
    hPrtIcon->SetDimensions(340,340);
    hPrtIcon->SetLCTId(UString("uiaccel_main_aa_coverflow_pane"));
    hPrtIcon->SetVarietyId(0);
    
    //ADD EXTRA ATTRIBUTE FOR FITMODE
        
    CustomAttributeStructure* hIPath = new CustomAttributeStructure(
            UString("mul_icon"),UString("imagepath"),EAttributeNone,
        ECategoryData);
    hPrtIcon->AddAttribute(hIPath);
    
    CustomAttributeStructure* hIOpacity = new CustomAttributeStructure(
        UString("opacity"),UString("1.0"),EAttributeOpacity);
    hPrtIcon->AddAttribute(hIOpacity);
    
    prtHilightElem->AddVisualStructure(hPrtIcon);
    
    mTemplatePrtStruct.push_back(prtHilightElem);
    
	//==========================================================================	
	//same procedure for filling up of LSC
	//==========================================================================
	//for main layout
	LCTElementStructure* lscBaseElem = new (EMM) LCTElementStructure();
	lscBaseElem->SetId(UString("base"));
	LCTVisualStructure* mainLscVS2 = new (EMM) LCTVisualStructure();
	mainLscVS2->SetId(UString("mainlayout"));
	mainLscVS2->SetPositions(0,0);
	mainLscVS2->SetDimensions(502,299);
	mainLscVS2->SetLCTId(UString("uiaccel_main_aa_coverflow_pane"));
	mainLscVS2->SetVarietyId(0);	
	CustomAttributeStructure* mainLscOpacity = new (EMM) CustomAttributeStructure(
	            UString("opacity"), UString("1.0"));	
	mainLscVS2->AddAttribute(mainLscOpacity);
	lscBaseElem->AddVisualStructure(mainLscVS2);	
	//for item_center
	LCTVisualStructure* item_centerLscVs = new (EMM) LCTVisualStructure();
	item_centerLscVs->SetId(UString("item_center"));
	item_centerLscVs->SetPositions(114,16);
	item_centerLscVs->SetDimensions(266,266);
	item_centerLscVs->SetLCTId(UString("uiaccel_aacf_image_pane"));
	item_centerLscVs->SetVarietyId(1);
	CustomAttributeStructure* item_centerLscFM = new (EMM) CustomAttributeStructure(
	        UString("fitmode"), UString("off"));
	item_centerLscVs->AddAttribute(item_centerLscFM);
	lscBaseElem->AddVisualStructure(item_centerLscVs);
	// For Counter
	LCTVisualStructure* counterLscVS = new (EMM) LCTVisualStructure();
	counterLscVS->SetId(UString("mul_count"));
	counterLscVS->SetPositions(420,32);
	counterLscVS->SetDimensions(112,21);
	counterLscVS->SetLCTId(UString("uiaccel_main_aa_coverflow_pane_t1"));
	counterLscVS->SetVarietyId(1);
    //add extra attributes to the gridlayout    
    CustomAttributeStructure* styleLscC = new (EMM) CustomAttributeStructure(
            UString("style"), UString("qfn_secondary"));
    
    CustomAttributeStructure* opacityLscC = new (EMM) CustomAttributeStructure(
            UString("opacity"), UString("1.0"));
    
    CustomAttributeStructure* redLscC = new (EMM) CustomAttributeStructure(
            UString("red"), UString("255"));
    
     CustomAttributeStructure* greenLscC = new (EMM) CustomAttributeStructure(
                UString("green"), UString("255"));
     
    CustomAttributeStructure* blueLscC = new (EMM) CustomAttributeStructure(
        UString("blue"), UString("255"));
    
    CustomAttributeStructure* alphaLscC = new (EMM) CustomAttributeStructure(
        UString("alpha"), UString("0"));
    
    CustomAttributeStructure* horizontalalignLscC = new (EMM) CustomAttributeStructure(
            UString("horizontalalign"), UString("2"));
            
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
    textLscLtVS->SetPositions(114,210);
    textLscLtVS->SetDimensions(266,72);
    textLscLtVS->SetLCTId(UString("uiaccel_aacf_image_title_pane"));
    textLscLtVS->SetVarietyId(1);
    lscBaseElem->AddVisualStructure(textLscLtVS);
    
    //for text background
    LCTVisualStructure* textLscBgVS = new (EMM) LCTVisualStructure();
    textLscBgVS->SetId(UString(KMulBackGround));
    textLscBgVS->SetPositions(0,0);
    textLscBgVS->SetDimensions(266,72);
    textLscBgVS->SetLCTId(UString("uiaccel_aacf_image_title_pane_g1"));
    textLscBgVS->SetVarietyId(0);
    //Add attributes to the text background visual
    CustomAttributeStructure* bgHaLscTV = new (EMM) CustomAttributeStructure(
                UString("horizontalalign"), UString("1"));
    textLscBgVS->AddAttribute(bgHaLscTV);
    CustomAttributeStructure* bgoLscTV = new (EMM) CustomAttributeStructure(
        UString("opacity"), UString("0.5"));
    textLscBgVS->AddAttribute(bgoLscTV);
    CustomAttributeStructure* bgcLscTV = new (EMM) CustomAttributeStructure(
            UString("colour"), UString("0"));
    textLscBgVS->AddAttribute(bgcLscTV);
    lscBaseElem->AddVisualStructure(textLscBgVS);
    
    //for text title
    LCTVisualStructure* textTLscVS = new (EMM) LCTVisualStructure();
    textTLscVS->SetId(UString(KMulTitle));
    textTLscVS->SetPositions(3,4);
    textTLscVS->SetDimensions(260,27);
    textTLscVS->SetLCTId(UString("uiaccel_aacf_image_title_pane_t1"));
    textTLscVS->SetVarietyId(0);
    //add extra attributes to the countervisual
    CustomAttributeStructure* textTStyleLsc = new (EMM) CustomAttributeStructure(
             UString("style"), UString("qfn_secondary"));
    textTLscVS->AddAttribute(textTStyleLsc);
    
    CustomAttributeStructure* textTFtcLsc = new (EMM) CustomAttributeStructure(
            UString("fontthemecolor"), UString("9"));
    textTLscVS->AddAttribute(textTFtcLsc);
    
    CustomAttributeStructure* textTHaLsc = new (EMM) CustomAttributeStructure(
            UString("horizontalalign"), UString("1"));
    textTLscVS->AddAttribute(textTHaLsc);
    
    CustomAttributeStructure* textTVaLsc = new (EMM) CustomAttributeStructure(
            UString("verticalalign"), UString("1"));
    textTLscVS->AddAttribute(textTVaLsc);
    
    CustomAttributeStructure* textTRedLsc = new (EMM) CustomAttributeStructure(
            UString("red"), UString("255"));
    textTLscVS->AddAttribute(textTRedLsc);
    
     CustomAttributeStructure* textTGreenLsc = new (EMM) CustomAttributeStructure(
                UString("green"), UString("255"));
     textTLscVS->AddAttribute(textTGreenLsc);
     
    CustomAttributeStructure* textTBlueLsc = new (EMM) CustomAttributeStructure(
        UString("blue"), UString("255"));
    textTLscVS->AddAttribute(textTBlueLsc);
    
    CustomAttributeStructure* textTAlphaLsc = new (EMM) CustomAttributeStructure(
        UString("alpha"), UString("0"));
    textTLscVS->AddAttribute(textTAlphaLsc);
    
    CustomAttributeStructure* textTTsciLsc = new (EMM) CustomAttributeStructure(
            UString("textskincolorid"), UString("KAknsIIDQsnTextColors"));
    textTLscVS->AddAttribute(textTTsciLsc);
    
    CustomAttributeStructure* textTTcsiLsc = new (EMM) CustomAttributeStructure(
        UString("textcolorskinindex"), UString("5"));
    textTLscVS->AddAttribute(textTTcsiLsc);
    
    CustomAttributeStructure* textTOpacityLsc = new (EMM) CustomAttributeStructure(
        UString("opacity"), UString("1.0"));
    textTLscVS->AddAttribute(textTOpacityLsc);
    
    lscBaseElem->AddVisualStructure(textTLscVS);
    
    //for text detail
    LCTVisualStructure* textDLscVS = new (EMM) LCTVisualStructure();
    textDLscVS->SetId(UString(KMulDetail));
    textDLscVS->SetPositions(4,38);
    textDLscVS->SetDimensions(260,27);
    textDLscVS->SetLCTId(UString("uiaccel_aacf_image_title_pane_t2"));
    textDLscVS->SetVarietyId(0);
    //add extra attributes to the countervisual
    CustomAttributeStructure* textDStyleLsc = new (EMM) CustomAttributeStructure(
             UString("style"), UString("qfn_secondary"));
    textDLscVS->AddAttribute(textDStyleLsc);
    
    CustomAttributeStructure* textDFtcLsc = new (EMM) CustomAttributeStructure(
            UString("fontthemecolor"), UString("9"));
    textDLscVS->AddAttribute(textDFtcLsc);
    
    CustomAttributeStructure* textDHaLsc = new (EMM) CustomAttributeStructure(
            UString("horizontalalign"), UString("1"));
    textDLscVS->AddAttribute(textDHaLsc);
    
    CustomAttributeStructure* textDVaLsc = new (EMM) CustomAttributeStructure(
            UString("verticalalign"), UString("1"));
    textDLscVS->AddAttribute(textDVaLsc);
    
    CustomAttributeStructure* textDRedLsc = new (EMM) CustomAttributeStructure(
            UString("red"), UString("255"));
    textDLscVS->AddAttribute(textDRedLsc);
    
     CustomAttributeStructure* textDGreenLsc = new (EMM) CustomAttributeStructure(
                UString("green"), UString("255"));
     textDLscVS->AddAttribute(textDGreenLsc);
     
    CustomAttributeStructure* textDBlueLsc = new (EMM) CustomAttributeStructure(
        UString("blue"), UString("255"));
    textDLscVS->AddAttribute(textDBlueLsc);
    
    CustomAttributeStructure* textDAlphaLsc = new (EMM) CustomAttributeStructure(
        UString("alpha"), UString("0"));
    textDLscVS->AddAttribute(textDAlphaLsc);
    
    CustomAttributeStructure* textDTsciLsc = new (EMM) CustomAttributeStructure(
            UString("textskincolorid"), UString("KAknsIIDQsnTextColors"));
    textDLscVS->AddAttribute(textDTsciLsc);
    
    CustomAttributeStructure* textDTcsiLsc = new (EMM) CustomAttributeStructure(
        UString("textcolorskinindex"), UString("5"));
    textDLscVS->AddAttribute(textDTcsiLsc);
    
    CustomAttributeStructure* textDOpacityLsc = new (EMM) CustomAttributeStructure(
        UString("opacity"), UString("0.7"));
    textDLscVS->AddAttribute(textDOpacityLsc);
    
    CustomAttributeStructure* textDMarqueeLsc = new (EMM) CustomAttributeStructure(
        UString("marquee"), UString("yes"));
    textDLscVS->AddAttribute(textDMarqueeLsc);
    
    lscBaseElem->AddVisualStructure(textDLscVS);
	mTemplateLscStruct.push_back(lscBaseElem);
	
   //now populate data for prtNonhilightElem element
    //----------------------------------------------------------------------
    LCTElementStructure* lscNonhilightElem = new (EMM) LCTElementStructure();
    lscNonhilightElem->SetId(UString("nohighlight"));

    //for backgroung of non-highlighted imagevisual        
    LCTVisualStructure* nHLscDeckVs = new (EMM) LCTVisualStructure();
    nHLscDeckVs->SetId(UString("deck"));
    CustomAttributeStructure* nHDeckOpacity = new CustomAttributeStructure(
                UString("opacity"),UString("1"),EAttributeOpacity);
    nHLscDeckVs->AddAttribute(nHDeckOpacity);
    lscNonhilightElem->AddVisualStructure(nHLscDeckVs);
        
    //for non-highlighted imagevisual
    LCTVisualStructure* nonHLscIcon = new (EMM) LCTVisualStructure();
    nonHLscIcon->SetVisualType(EVisualImage);
    nonHLscIcon->SetId(UString("mul_icon"));
    nonHLscIcon->SetPositions(0,0);
    nonHLscIcon->SetDimensions(640,360);
    nonHLscIcon->SetLCTId(UString("uiaccel_main_aa_coverflow_pane"));
    nonHLscIcon->SetVarietyId(1);
    
    //ADD EXTRA ATTRIBUTE FOR FITMODE
       
    CustomAttributeStructure* nonHLscIPath = new CustomAttributeStructure(
        UString("imagepath"),UString("mul_icon"),EAttributeNone,
        ECategoryData);
    nonHLscIcon->AddAttribute(nonHLscIPath);
    
    CustomAttributeStructure* nHLscIOpacity = new CustomAttributeStructure(
        UString("opacity"),UString("1"),EAttributeOpacity);
    nonHLscIcon->AddAttribute(nHLscIOpacity);
    
    lscNonhilightElem->AddVisualStructure(nonHLscIcon);
    
    mTemplateLscStruct.push_back(lscNonhilightElem);
	    
    //now populate data for prthilightElem element
    //----------------------------------------------------------------------
    LCTElementStructure* lscHilightElem = new (EMM) LCTElementStructure();
    lscHilightElem->SetId(UString("highlight"));

    //for backgroung of non-highlighted imagevisual        
    LCTVisualStructure* hLscDeckVs = new (EMM) LCTVisualStructure();
    hLscDeckVs->SetId(UString("deck"));
    CustomAttributeStructure* hLscDeckOpacity = new CustomAttributeStructure(
            UString("opacity"),UString("1"),EAttributeOpacity);
    hLscDeckVs->AddAttribute(hLscDeckOpacity);
    lscHilightElem->AddVisualStructure(hLscDeckVs);
        
    //for non-highlighted imagevisual
    LCTVisualStructure* hLscIcon = new (EMM) LCTVisualStructure();
    hLscIcon->SetVisualType(EVisualImage);
    hLscIcon->SetId(UString("mul_icon"));
    hLscIcon->SetPositions(0,0);
    hLscIcon->SetDimensions(360,640);
    hLscIcon->SetLCTId(UString("uiaccel_main_aa_coverflow_pane"));
    hLscIcon->SetVarietyId(1);
    
    //ADD EXTRA ATTRIBUTE FOR FITMODE
    
    CustomAttributeStructure* hLscIPath = new CustomAttributeStructure(
        UString("imagepath"),UString("mul_icon"),EAttributeNone,
        ECategoryData);
    hLscIcon->AddAttribute(hLscIPath);
    
    CustomAttributeStructure* hLscIOpacity = new CustomAttributeStructure(
        UString("opacity"),UString("1"),EAttributeOpacity);
    hLscIcon->AddAttribute(hLscIOpacity);
    
    lscHilightElem->AddVisualStructure(hLscIcon);
    
    mTemplateLscStruct.push_back(lscHilightElem);
    }
		    
    }//namespace ends
