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
* Description: Header for presentation element - vertical slider.
 *
*/

 
 
 
#ifndef MUL_SLIDERVERTICAL_H
#define MUL_SLIDERVERTICAL_H

//INCLUDES
#include <alf/alfanchorlayout.h>
#include "alf/alfelement.h"
#include <alf/alfimagevisual.h>
#include <alf/alflayout.h>
#include <alf/alftexture.h>
#include <mul/imulsliderwidget.h>

#include <alf/ialfwidgeteventhandler.h>
#include <alf/alfbitmapprovider.h>

#include "mulslidermodel.h"
#include <AknLayout2Def.h>
#include "imulsliderbaseelementinternal.h"


namespace duiuimodel
    {
    class DuiNode;
    }

using namespace osncore;

namespace Alf
    {
    struct MulSliderPos;

    
// Forward declarations
class IAlfWidgetEventHandler;
class CAlfWidgetControl;
class MulSliderLongTapTimer;

/**
 * Default Base Element of the Scrollbar.
 */
class MulSliderVertical : 
    public AlfElement, 
    public IAlfWidgetEventHandler,
    public IMulSliderBaseElementInternal,
    public MAlfBitmapProvider
    {
public:
    
    /**
     * Constructor
     * @param aControl Widget control.
     * @param aName Name of the item element.
     * @param aCustomData .
     * @param mNode .
     * @since S60 ?S60_version
     */
    MulSliderVertical (
        CAlfWidgetControl& aControl, 
        const char* aName, 
        AlfCustomInitDataBase* aCustomData=NULL);

    /**
     * Destructor 
     */    
    virtual ~MulSliderVertical();
    
    /**
     * bitmap provider.
     * @since S60 ?S60_version
     * @param aId bitmap ID.
     * @param aBitmap bitmap.
     * @param aMaskBitmap mask for the bitmap.
     * @return 
     */
		void ProvideBitmapL (TInt aId, 
		                     CFbsBitmap *& aBitmap, 
                         CFbsBitmap *& aMaskBitmap);

    /**
     * From IAlfInterfaceBase.
     * Getter for interfaces provided by the Button widget class.
     * @since S60 ?S60_version
     * @param aType A descriptor to identify the type of the queried interface.
     * @return The queried interface, or NULL if the interface is not
     *         supported or available.
     */
    virtual IAlfInterfaceBase* makeInterface(const IfId& aType);
    
      
    /**
	 * Sets the text for the counter visual
	 * 
	 * @param aNewString The counter visual text value 
	 * @throw Invalid Argument incase there is no layout at zeroth position
	 */ 
    void SetTextLabels(const char* aStr);
         
      
    //--------------------------------------------------------------------------
    //Overriden Apis from IAlfWidgetEventHandlers
    //--------------------------------------------------------------------------
    //

    /**
     * Used to identify event handlers that are capable of handling
     * a specified key or custom event.
     * Pointer event handlers cannot be identified directly, 
     * since they are dependent on the presentation.
     * Therefore, pointer event handlers should be associated with
     * a custom event ID that can be used to simulate the actual pointer event.
     * @since S60 ?S60_version
     * @param aEvent The event.
     * @param aControl
     * @return <code>ETrue</code> if the event can be processed. 
     * Otherwise <code>EFalse</code>.
     */
    bool accept(CAlfWidgetControl& aControl, const TAlfEvent& aEvent) const;


   
    /**
     * @see IAlfWidgetEventHandler
     */
    virtual AlfEventHandlerType eventHandlerType();

    /**
     * @see IAlfWidgetEventHandler
     */
    virtual AlfEventHandlerExecutionPhase eventExecutionPhase();
    /**
     * Called when an input event is being offered to this event handler.
     * The return value is used to indicate if the event was processed.
     * Processed events may not be offered to other event handlers.
     * @since S60 ?S60_version
     * @param aControl    The parent control of this event handler.
     * @param aEvent    The event.
     * @return <code>ETrue</code> if the event was processed. 
     * Otherwise <code>EFalse</code>.
     */
   
   
    virtual AlfEventStatus offerEvent(
        CAlfWidgetControl& aControl, 
        const TAlfEvent& aEvent );

    /**
     * Defines the widget states, where the event handler is active.
     * By default, if this method is not called, the event handler is expected
     * to be active in all states.
     * @param aStates A bitmask defining the widget states, where the event
     * handler is active. The low 16 bits are reserved for the states
     * defined by the framework in <TODO: add reference>. The high 16 bits are
     * available for states defined by the client code.
     */
    void setActiveStates(unsigned int aStates);
    
    /**
     * Sets AlfWidgetEventHandlerInitData to event handler.
     * @param aData A data structure which contains for example id of the event
     * handler.
     */
    virtual void setEventHandlerData(
        const AlfWidgetEventHandlerInitData& /*aData*/ )
        {
        
        }
    
    /**
     * Returns AlfWidgetEventHandlerInitData.
     * @return  A pointer to AlfWidgetEventHandlerInitData structure 
     * which contains for example id of the event handler.
     */
    virtual AlfWidgetEventHandlerInitData* eventHandlerData()
        {
          return NULL;
        }

    /**
     * Friend class for timer functions.
     * 
     */
    friend class MulSliderLongTapTimer;


	//--------------------------------------------------------------------------
	//Overriden Apis from IMulSliderBaseElementInternal
	//--------------------------------------------------------------------------
	//
	
    /**
     * Notified when the base layout of scrollbar changes .
     * Relayouts all the visuals with respect to the new size.
     */
    void baseLayoutSizeChanged();
   
    /**
     * createVisualization
     * @param aTemplateId .aTemplateId takes 3 or 9 as parameter.
     */
    void createVisualization(sliderTemplate aTemplateId);
    
    /**
     * updateVisualization
     * Notified when the base layout of scrollbar changes .
     * Relayouts all the visuals with respect to the new size.
     */
    void updateVisualization();

   /**
     * Updates Text visualization.
     * 
	   */
    
     void updateTextVisualization();
   /**
         * Sets transparency for the Widget
         * @param aTransparent - opacity level
         */ 
     void MakeTransparent(bool aVal);
	 /**
	   *Stops all event handling 
	   *
	   *
	   */
     void stopEvents() ;
 private: 
    /**
     * Set skin images on scrollbar track
     */
    void setTrackImage();
    
    /**
     * Set skin images on scrollbar thumb
     */
    void setThumbImage();
	/**
	 * Sets the position of the handle
	 * 
	 * 
	 */
    void SetThumbPosition(); 
    
	/**
     * Handles the key events only.     
     *   
     * @param aEvent event to be handled
     * @returns The return value is used to indicate if the event was processed.
     */   
	AlfEventStatus HandleKeyEvents(const TAlfEvent& aEvent);
	
   /**
     * Handles the pointer events only.     
     * 
     *  
     * @param aEvent  Event to be handled
     * @returns The return value is used to indicate if the event was processed.
     */   
	AlfEventStatus HandlePointerEvents( TAlfEvent* aEvent);
	
	/**
     * handles the pointerDown event
     *   
     * @param aEvent  Event to be handled
     * @returns  The return value is used to indicate if the event was processed.
     */
    AlfEventStatus handlePointerDownEvent(TAlfEvent * aPntrEvent);
    		
    
private:
    
  
    /**
     * IsSliderVisual
     * 
     */
     bool IsSliderVisual(CAlfVisual *aVisual);    
    
     
    /**
     * layout the visuals 
     * 
     */
    void layoutVisuals( sliderTemplate aTemplateId) ; 
    
    
                       
    /**
     * 
     * initializes Model and Widget Pointer
     */
    void initializeSliderData();
     
     
                         
    /**
     * SetBackgroundBrush
     * 
     */                                              
    void SetBackgroundBrush();
    
    /**
     * ConvertDataToPixels
     * 
     */ 
    void ConvertDataToPixels();
    
    /**
     * updateModelPrimaryValue
     * 
     */
    void updateModelPrimaryValue(int aNewPosValue);
    
    /**
     * Start drag 
     */
    bool initializeThumbDragData(const TPoint& aPointerPos); 
                      
    
    /**
     * Stop drag 
     */
    bool stopDrag();
    
    /**
    * handlePointerUpEvent  
    */
    void handlePointerUpEvent();
    
    /**
    * snapPrimaryValueToTicks  
    */
    void snapPrimaryValueToTicks();
    
    /**
    * trackVisualHit  
    */
    AlfEventStatus trackVisualHit(CAlfVisual * aHitVisual);
    
    /**
     * Texture for Visuals of slider. 
     * 
     */
    TAlfImage skinTexture(TSize aPrtImageSize, TSize aLscImageSize, TInt aPrtSkinId, TInt aLscSkinId );
    
private: // Class Data
    

    struct MulVerticalSliderDataImpl* mData;

    // Structure for storing current and previous values
	auto_ptr<MulSliderPos> mPosData; 
      
      	
    auto_ptr<MulSliderLongTapTimer> mLongTapTimer;
    MulSliderModel * mSliderModel;
    MulSliderWidget * mSliderWidget;
   
    }; // end of class 
    
    } // end of namespace Alf
    
#endif //MUL_SLIDERVERTICAL_H
//End Of File
