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
* Description:  interface for the slider base element internal functions.
 *
*/

#ifndef I_MULSLIDERBASEELEMENTINTERNAL_H
#define I_MULSLIDERBASEELEMENTINTERNAL_H

//Widget Model include
#include <alf/ialfinterfacebase.h>

//Toolkit include
#include <alf/alfevent.h>
#include <alf/alftypes.h>
#include <mul/imulsliderwidget.h>
namespace Alf
    {
    enum SliderLCTIDs
    {
    EHSliderPane,// horizontal slider aacf_slider_pane(0) for base layout 
    EHSliderBase,
    EHSliderCentre,
    EHSliderCenter,// horizontal slider aaslider_bg_pane(0) for track
    EHSliderRight,
    EHSliderLeft,
    EHSliderHandle,// horizontal slider aacf_slider_pane_g1(0) for thumb 
    EHSliderHandleExtended,
    EVSliderPane,  // vertical slider aaslider_pane(2) for mainlayout
    EVSliderBackground, // vertical slider aaslider_pane_g1(2) for mainlayout
    EVSliderBgPane , // aaslider_bg_pane_cp001     
    EVSliderTop ,
    EVSliderMiddle ,
    EVSliderEnd ,
    EVSliderPlus, //aid_touch_size_slider_max
    EVSliderMinus, //aid_touch_size_slider_min
    EVSliderMarker, //aacf_slider_pane_g1
    EVSliderMarkerExtended,
    EVSliderZoomText, //aaslider_pane_t3
    EVSliderAudioImage,
    EVSliderInnerRect,
    EPSliderLeft,
    EPSliderCentre,
    EPSliderRight   
    };    
enum TOperation
{
ENop,      // Nothing is going on.
EMoveItem   // Item is begin moved.
}; 
/**
 * @namespace mulsliderbaseelementinternal
 *
 * internal interface.
 */
namespace mulsliderbaseelementinternal
    {
    static const IfId ident =
        {
        0,"mulsliderbaseelementinternal" 
        };
    }


class IMulSliderBaseElementInternal : public IAlfInterfaceBase 
    {

public:

 
    static inline const IfId& type()
        {
        return mulsliderbaseelementinternal :: ident;
        }

    /**
     * Notified when the base layout of Slider changes .
     * Relayouts all the visuals with respect to the new size.
     */

    virtual void baseLayoutSizeChanged()=0;
    /**
      * Creates the Visualization of Slider
      * 
      */
 
    virtual void createVisualization(sliderTemplate aTemplate) = 0;
    /**
      * Updates the Visualization of Slider
      * 
      */

    virtual void updateVisualization() = 0;
    /**
      * Updates the Text Visualization of Slider
      * 
      */

    virtual void updateTextVisualization() = 0;
    /**
      * Stops all the event handling operations 
      * 
      */

    virtual void stopEvents()
        {
        
        }
    /**
      * API to make the widget transparent.
      * @param aTransparent - True will make it transparent.
      *                     - False for Opaque. 
      */

    virtual void MakeTransparent( bool /*aTransparent*/ )
    {
    	
    }
    /**
      * API to Handle Long Tap events.
      */   
    virtual AlfEventStatus handleLongTap() 
    {
    	 return EEventNotHandled;
    }
       
    /** 
     *virtual Destructor.
     *
     */
    virtual ~IMulSliderBaseElementInternal() {}

    }; // end of class IMulSliderBaseElementInternal
  
    } // end of namespace Alf

#endif // I_MULSLIDERBASEELEMENTINTERNAL_H
//End Of File
