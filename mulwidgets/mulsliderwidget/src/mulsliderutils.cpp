/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Utilities for slider widget.
*
*/





#include <aknlayoutscalable_uiaccel.cdl.h>
#include "mulsliderdefinitions.h"
#include "mulsliderutils.h"
#include <alf/alfimageloaderutil.h>
#include <alf/alftexture.h>
#include <alf/alfimage.h>
#include <alf/alfenv.h>
#include <utf.h>
#include "mulutility.h"


namespace Alf
    {

    //------------------------------------------------------------------------
    // GetComponentRect
    //------------------------------------------------------------------------
	 TAknLayoutRect MulSliderUtils::GetComponentRect(
	     SliderLCTIDs aId,CAlfLayout *aLayout,int aVariety)
	 {
	 	TAknLayoutRect layoutRect;
	 	TPoint layoutRectSize = aLayout->Size().IntValueNow();
	 	    
	 	TRect rect2(0,
	 	           0,
	 	           layoutRectSize.iX,
	 	           layoutRectSize.iY);
		switch(aId)
		    {
			case EVSliderPane:  
			{
			TAknWindowLineLayout LayoutHandle =
			    AknLayoutScalable_UiAccel::aaslider_pane(aVariety).LayoutLine();
			layoutRect.LayoutRect( rect2,LayoutHandle);  
			}
			break;
			
			case EVSliderBackground:
			{
			TAknWindowLineLayout LayoutHandle = 
			    AknLayoutScalable_UiAccel::
			        aaslider_pane_g1(aVariety).LayoutLine();
			layoutRect.LayoutRect( rect2,LayoutHandle);          
			}
			break;
			
			case EVSliderBgPane:
			{
			TAknWindowLineLayout LayoutHandle = 
			    AknLayoutScalable_UiAccel::
			        aaslider_bg_pane_cp001(aVariety).LayoutLine();
			layoutRect.LayoutRect(rect2,LayoutHandle);
			}
			break;  
			
			case EVSliderTop:
			{
			TAknWindowLineLayout LayoutHandle = 
			    AknLayoutScalable_UiAccel::
			        aaslider_bg_pane_cp001_g1(aVariety).LayoutLine();
			layoutRect.LayoutRect(rect2,LayoutHandle);
			}
			break;  
			
			case EVSliderMiddle:
			{
			TAknWindowLineLayout LayoutHandle = 
			    AknLayoutScalable_UiAccel::
			        aaslider_bg_pane_cp001_g3(aVariety).LayoutLine();
			layoutRect.LayoutRect(rect2,LayoutHandle);
			}
			break;  
			
			case EVSliderEnd:
			{
			TAknWindowLineLayout LayoutHandle = 
			    AknLayoutScalable_UiAccel::
			        aaslider_bg_pane_cp001_g2(aVariety).LayoutLine();
			layoutRect.LayoutRect(rect2,LayoutHandle);
			}
			break;  
			
			case EVSliderPlus:
			{
			TAknWindowLineLayout LayoutHandle = 
			    AknLayoutScalable_UiAccel::aaslider_pane_g4(aVariety).LayoutLine();
			layoutRect.LayoutRect( rect2,LayoutHandle);
			}
			break;
			
			case EVSliderMinus:
			{
			TAknWindowLineLayout LayoutHandle = 			                     
			    AknLayoutScalable_UiAccel::
			    aaslider_pane_g5(aVariety).LayoutLine();			        
			layoutRect.LayoutRect( rect2,LayoutHandle);
			}
			break;
			
			case EVSliderMarker:
			{
			TAknWindowLineLayout LayoutHandle = 
                    AknLayoutScalable_UiAccel::
                    aaslider_pane_g2(aVariety).LayoutLine();
			layoutRect.LayoutRect( rect2,LayoutHandle);
			}
			break;
			
			case EVSliderInnerRect:
			{
			TAknWindowLineLayout LayoutHandleInner = 
			    AknLayoutScalable_UiAccel::
			        aid_aaslider_pane_rect_inner(aVariety).LayoutLine();
			layoutRect.LayoutRect( rect2,LayoutHandleInner);
			}
			break;
			/*case EVSliderAudioImage:
			{
			TAknWindowLineLayout LayoutHandle =                                                   
			    AknLayoutScalable_UiAccel::
			        aaslider_pane_g3(aVariety).LayoutLine();                              
			layoutRect.LayoutRect(rect2,LayoutHandle);
			}   
			break;*/

			case EHSliderPane:
			{
			TAknWindowLineLayout TrackLayoutHandle = 
			    AknLayoutScalable_UiAccel::
			        aacf_slider_pane(aVariety).LayoutLine();
			layoutRect.LayoutRect( rect2,TrackLayoutHandle);          
			}
			break;

			case EHSliderBase:
			{
			TAknWindowLineLayout TrackLayoutHandle = 
			    AknLayoutScalable_UiAccel::
			        aacf_slider_pane(aVariety).LayoutLine();
			layoutRect.LayoutRect( rect2,TrackLayoutHandle);          
			}
			break;

			case EHSliderCenter:    
			{
			TAknWindowLineLayout TrackLayoutHandle = 
			    AknLayoutScalable_UiAccel::
			        aaslider_bg_pane(aVariety).LayoutLine();
			layoutRect.LayoutRect(rect2, TrackLayoutHandle);
			}
			break;
			
		    case EHSliderLeft:
			{
			TAknWindowLineLayout TrackLayoutHandle = 
			    AknLayoutScalable_UiAccel::
			        aaslider_bg_pane_g1(aVariety).LayoutLine();
			layoutRect.LayoutRect( rect2,TrackLayoutHandle);          
			}
			break;
			
			case EHSliderCentre:
			{
			TAknWindowLineLayout TrackLayoutHandle = 
			    AknLayoutScalable_UiAccel::
			        aaslider_bg_pane_g2_copy1(aVariety).LayoutLine();
			layoutRect.LayoutRect( rect2,TrackLayoutHandle);          
			}
			break;

      case EHSliderRight:
			{
			TAknWindowLineLayout TrackLayoutHandle = 
			    AknLayoutScalable_UiAccel::
			        aaslider_bg_pane_g2(aVariety).LayoutLine();
			layoutRect.LayoutRect( rect2,TrackLayoutHandle);          
			}
			break;
			
			/*case EHSliderHandle:    
			{
			TAknWindowLineLayout TrackLayoutHandle = 
			    AknLayoutScalable_UiAccel::
			    aaslider_pane_g2(aVariety).LayoutLine();
			layoutRect.LayoutRect(rect2, TrackLayoutHandle);
			}
			break;*/
			case EHSliderHandle:    
            {
            TAknWindowLineLayout TrackLayoutHandle = 
                AknLayoutScalable_UiAccel::
                    aacf_slider_pane_g1(aVariety).LayoutLine();
            layoutRect.LayoutRect(rect2, TrackLayoutHandle);
            }
            break;
			case EVSliderMarkerExtended:
            {
            TAknWindowLineLayout LayoutHandle = 
                AknLayoutScalable_UiAccel::
                    aid_touch_size_slider_marker(aVariety).LayoutLine();
            layoutRect.LayoutRect( rect2,LayoutHandle);
            }
            break;
			case EPSliderLeft:    
			{
			TAknWindowLineLayout ProgressPaneLeft = 
			    AknLayoutScalable_UiAccel::
			        aalist_progress_pane_g1(aVariety).LayoutLine();
			layoutRect.LayoutRect(rect2, ProgressPaneLeft);
			}
			break;
			
			case EPSliderCentre:    
			{
			TAknWindowLineLayout ProgressPaneCentre = 
			    AknLayoutScalable_UiAccel::
			        aalist_progress_pane_g2(aVariety).LayoutLine();
			layoutRect.LayoutRect(rect2, ProgressPaneCentre);
			}
			break;
			
			case EPSliderRight:    
			{
			TAknWindowLineLayout ProgressPaneRight = 
			    AknLayoutScalable_UiAccel::
			        aalist_progress_pane_g3(aVariety).LayoutLine();
			layoutRect.LayoutRect(rect2, ProgressPaneRight);
			}
			break;
			default: ;
			         
	        }// end of switch
     
		  return layoutRect;

	 }
	 
	//-------------------------------------------------------------------------
	// GetTextComponentRect
	//------------------------------------------------------------------------- 
	 TAknLayoutText MulSliderUtils::GetTextComponentRect(
	     SliderLCTIDs aId, CAlfLayout *aLayout, int aVariety)
	 {
	 	TAknLayoutText textRect;
    
		TRect rect2(0,
		           0,
		           aLayout->Size().iX.ValueNow(),
		           aLayout->Size().iY.ValueNow());
		switch(aId)
		    {
			case EVSliderZoomText:  
			{			     
			TAknTextLineLayout textLayout =  
        	    AknLayoutScalable_UiAccel::aaslider_pane_t3(aVariety).LayoutLine();     		
    		textRect.LayoutText(rect2, textLayout );
			}
			break;
			
			default: ;
			
		    }
		    
		return textRect;
	 }


	} // End of namespace Alf
	

