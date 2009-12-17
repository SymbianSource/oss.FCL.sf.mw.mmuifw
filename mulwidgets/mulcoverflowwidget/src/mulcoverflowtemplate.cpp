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
* Description:  CoverFlow templates Implementation
 *
*/

//  Include Files

#include <AknUtils.h>

// Alf Headers
#include <alf/alfenv.h>
#include <alf/alfbrusharray.h>
#include <alf/alfgradientbrush.h>
#include <alf/alftextvisual.h>

// Class Header
#include "mulcoverflowtemplate.h"

// Local Headers
#include "mulcoverflowcontrol.h"
#include "mulbaseelement.h"
#include "mulleave.h"
#include "mullog.h" //for logs

namespace Alf
    {
  
// ---------------------------------------------------------------------------
// MulCoverFlowTemplate
// ---------------------------------------------------------------------------
//
MulCoverFlowTemplate::MulCoverFlowTemplate(MulCoverFlowControl &aControl, 
        MulBaseElement &aBaseElement)
    {
    MUL_LOG_ENTRY_EXIT("Mul::MulCoverFlowTemplate::MulCoverFlowTemplate");
  
    mData.reset( new (EMM)TMulCoverFlowTemplateImpl);
    mData->mControl = &aControl;
    mData->mBaseElement = &aBaseElement;   
    }


 void MulCoverFlowTemplate::CreateVisualisation(bool /*aIsLandscape*/,bool /*aIsOrientationChange*/)
	{
	
	}

// ---------------------------------------------------------------------------
// ~MulCoverFlowWidget
// ---------------------------------------------------------------------------
//
MulCoverFlowTemplate::~MulCoverFlowTemplate()
    {

    }

// ---------------------------------------------------------------------------
// ApplyBrush
// ---------------------------------------------------------------------------
//
void MulCoverFlowTemplate::ApplyBrush( CAlfTextVisual& aVisual,
     const TRgb& aBrushColour, float aBrushOpacity )
    {
    MUL_LOG_ENTRY_EXIT("Mul::MulCoverFlowTemplate::ApplyBrush");
    
    // Enable brush on the text visual
    aVisual.EnableBrushesL( ETrue );
    CAlfBrushArray* brushArray = aVisual.Brushes();
    if(brushArray)
			{
			if( brushArray->Count()!=0)
				{
				//brushArray->Reset();
				CAlfGradientBrush& brush = static_cast<CAlfGradientBrush&>(
						brushArray->At(0)); 
				brush.SetColor( aBrushColour ); 
				brush.SetOpacity(aBrushOpacity);
				}
			else
			{
			THROW_IF_LEAVES
					(
					CAlfGradientBrush* brush = CAlfGradientBrush::NewL(
					    mData->mControl->Env()  );
					// Set the brush colour
					brush->SetColor( aBrushColour );
					// Apply brush and set the opacity .
					aVisual.Brushes()->AppendL( brush, EAlfHasOwnership );    
					brush->SetOpacity(aBrushOpacity);   
					);
				}    
			} 
    }       


// ---------------------------------------------------------------------------
// GetFontId
// ---------------------------------------------------------------------------
// 
TInt MulCoverFlowTemplate::GetFontId(const UString &aFontStyle,CAlfTextStyleManager& manager)const
    {
    MUL_LOG_ENTRY_EXIT("Mul::MulCoverFlowTemplate::GetFontId");
    TInt fontId = 0;
    if ( aFontStyle == "qfn_secondary" )
        {
        THROW_IF_LEAVES            
        (
        fontId = manager.CreatePlatformTextStyleL(EAknLogicalFontSecondaryFont, -1);                    
        );
        }
    else // by default it assumes the primary font.
        {               
        THROW_IF_LEAVES            
        (
        fontId = manager.CreatePlatformTextStyleL(EAknLogicalFontPrimaryFont, -1);                    
        );
        } 
    return fontId;      
    }

// ---------------------------------------------------------------------------
// IconWindowRect
// ---------------------------------------------------------------------------
//
TRect MulCoverFlowTemplate::IconWindowRect(
        const TAknWindowLineLayout &aLayout,
        const TRect& aParentRect)
    {
    TAknLayoutRect layoutRect ;
    layoutRect.LayoutRect( aParentRect, aLayout );
    TRect rect(layoutRect.Rect());
    return rect;
    }

// ---------------------------------------------------------------------------
// TextWindowRect
// ---------------------------------------------------------------------------
//
TRect MulCoverFlowTemplate::TextWindowRect(
        const TAknTextLineLayout& aLayout,
        const TRect& aParentRect)
    {
    TAknLayoutText layoutText;
    layoutText.LayoutText( aParentRect, aLayout );
    TRect rect(layoutText.TextRect());
    return rect;
    }


// ---------------------------------------------------------------------------
//  UpdateMaxFontWidth
// ---------------------------------------------------------------------------
//
int MulCoverFlowTemplate ::UpdateMaxFontWidth(CAlfTextVisual* textVisual)
    {
    int maxCharWidth = 0;
    int fontId = textVisual->TextStyle();
    const CFont* font = AknLayoutUtils::FontFromId(fontId, NULL);
    TFontSpec curfontSpec = font->FontSpecInTwips();
    curfontSpec.iFontStyle.SetStrokeWeight(EStrokeWeightNormal); 
    maxCharWidth = font->MaxCharWidthInPixels();
    return maxCharWidth;
    }

// ---------------------------------------------------------------------------
// SetTextStyle
// ---------------------------------------------------------------------------
//
void MulCoverFlowTemplate:: SetTextStyle(int aTextFontId, CAlfTextVisual& aTextVisual)
    {
    MUL_LOG_ENTRY_EXIT("Mul::MulCoverFlowTemplate::SetTextStyle");
    UString textStyle;
    switch(aTextFontId)
        {    
        case 0: textStyle=UString("qfn_primary");
        break;
        
        case 1: textStyle=UString("qfn_primary");
        break;
        
        case 2: textStyle=UString("qfn_secondary");
        break;
        
        } 
    CAlfTextStyleManager& manager = aTextVisual.Env().TextStyleManager();
    aTextVisual.SetTextStyle(GetFontId(textStyle, manager));
    }

  }//namespace Alf

// End of file.
