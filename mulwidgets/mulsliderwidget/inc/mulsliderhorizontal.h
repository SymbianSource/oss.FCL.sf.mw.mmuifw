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
* Description: Header for presentation elements - horizontal slider.
 *
*/

 
 
 
#ifndef MUL_SLIDERHORIZONTAL_H
#define MUL_SLIDERHORIZONTAL_H

//INCLUDES
#include <alf/alfanchorlayout.h>
#include "alf/alfelement.h"
#include <alf/alfimagevisual.h>
#include <alf/alflayout.h>
#include <alf/alfvisual.h>
#include <alf/ialfwidgeteventhandler.h>


#include <AknLayout2Def.h>

#include <mul/imulsliderwidget.h>
#include "mulslidermodel.h"

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
 * Default Base Element of the Slider.
 */
class MulSliderHorizontal : 
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
    MulSliderHorizontal (
        CAlfWidgetControl& aControl, 
        const char* aName, 
        AlfCustomInitDataBase* aCustomData=NULL);

    /**
     * Destructor 
     */    
    virtual ~MulSliderHorizontal();
    
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
         
    

    //void createVisualization();       
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


// From IAlfScrollBarBaseElementInternal

    /**
     * Notified when the base layout of scrollbar changes .
     * Relayouts all the visuals with respect to the new size.
     */
    void baseLayoutSizeChanged();
    
    /**
    * Initialises slider data
    */
    void initializeSliderData();
    /**
    * Updates visualization
    */
    void updateVisualization();
    /**
    * Updates text visualization
    */
    void updateTextVisualization();
		/**
    * handles Long tap event
    */
    AlfEventStatus handleLongTap();
    
    /**
    * Changes Thumb image
    */
    void changeThumbImage(bool aPressed = false);
    /**
    * Gets thumb minimum length.
    */
    void getThumbMinMaxLength();
    /**
    * Creates visualization
    */
    void createVisualization(sliderTemplate aTemplateId);
    // Creates Slider Template 3 ( Zoom Slider)
    //void createZoomSlider();
    /**
    * create a Dual progressive slider
    */
    void createDualProgressiveSlider();
   
 private: // New Methods
    
    /**
     * SetBackgroundBrush
     * 
     */                                              
    void SetBackgroundBrush();
    
	/**
     * Retrieves the data for the slider from the model into structure
     */
    void RetrieveData();
  	
	/**
	 * Sets the position of the handle
	 * 
	 * @param aCurrTick The new position of the handle
	 */
    void SetThumbPosition();//int aCurrTick); 
    
    /**
	 * Sets the text for the counter visual
	 * 
	 * @param aNewString The counter visual text value 
	 * @throw Invalid Argument incase there is no layout at zeroth position
	 */ 
    void SetTextLabels(const UString& aNewString);

    /**
	 * Sets the image for the image visual
	 * 
	 * @param aImagePath The path of the new image
	 * @throw Invalid Argument incase there is no layout at zeroth position
	 */ 
    void SetNewImagePath(const UString& aImagePath);
    
    /**
	 * Sets the image for the image visual from thr provided texture id
	 * 
	 * @param aTextureId The texture id of the new image
	 * @throw Invalid Argument incase there is no layout at zeroth position
	 */
    void SetNewImageTexture(const int aTextureId);

    /**
     * Notifies about the initiation of pointer down event on the visual.	
     *  
     * @returns true after the drag has started  
     */    
    //bool StartDrag(CAlfVisual *aDragVisual);
    
    /**
     * Sets the starting position of the pointer doen event and will
     * start ignoring the visual for further possible pointer events  
     *   
     * @param aOperation  true to signify start of drag.    
     */    
    //void Begin(TBool aOperation);   
    
    /**
     * Checks if the drag event is On
     * 
     * @returns true if the drag event is on.
     */
	//bool BeingDragged() const;
	
	/**
     * Clears the flag for ignoring the visual for pointer events.
     */
	//void StopDrag();
	
	/**
     * Handles the key events only.     
     *   
     * @param aEvent event to be handled
     * @returns if the event was handled or not
     */   
	AlfEventStatus HandleKeyEvents(const TAlfEvent& aEvent);
	
   /**
     * Handles the pointer events only.     
     * 
     * @aControl  The alfwidget control  
     * @param aEvent  Event to be handled
     * @returns  if the event was handled or not
     */   
	AlfEventStatus HandlePointerEvents( TAlfEvent* aEvent);
		/**
	  * Handles pointer down event
	  */
    AlfEventStatus handlePointerDownEvent(TAlfEvent * aPntrEvent);				
    /**
     * Checks if the slider visual is hit     
     * 
     * @param aVisual  Visual that has been touched. 
     * @param aPointerPt position of the pointer
     * @param aEvent  Event to be handled
     * @returns true/false if the visual is touched         
     */     
    bool IsSldVisualHit(const CAlfVisual& aVisual,TPoint aPointerPt,const TAlfEvent& aEvent);
    
    /**
     * Checks if the hit point is within the tappable area     
     *     
     * @param aPointerPt position of the pointer
     * @param aEvent  Event to be handled
     * @returns true/false if the point is in tappable area 
     */   	
	bool InTappableArea(const TPoint& aPointerPt,const TAlfEvent& aEvent) ;		
    
    /**
	 * Converts pixel to value according to range of slider
	 * @param aPixel The pixel value passed
	 * @param aMaxPixel The maximum value to be considered for calculation
	 * returns rounded int value after conversion
	 */
	int PixelToValue(int aPixel,int aMaxPixel);
  	
    /**
	 * Converts value to pixel according to range of slider
	 * @param aVal The value passed
	 * @param aMaxPixel The maximum value to be considered for calculation
	 * returns rounded int pixel after conversion
	 */
    int ValueToPixel(int aVal, int aMaxPixel); 

    /**
	 * Creates the new image for the given path
	 * @param aImageVisual The visual to which the image is to be set
	 * @param aPath The new path of the image
	 * @returns Returns the new image to be set
	 */
    TAlfImage CreateImageFromImagePath( const CAlfImageVisual& aImageVisual, const TPtrC8& aPath );
    
    /**
	 * Sets the new value for max and min range considering min range as zero.Its helpful
	 * for the negative min range values or greater than zero min range values.
	 *
	 * @param aMap Contains data.Used to get the data from current model.
	 */
    void NormalizeRange(/*IAlfMap& aMap*/);   
    
    /**
	 * Stores the value for x and y coordinate of baselayout position in structure.
	 * Later the values from the structure is used to get/set the relative values
	 * according to the new position of baselayout.	 
	 */    
    void RelativePos();
    void mirrorPointerPosition(const TPoint& aPointerPos );
    
    /*
     * Change volume icon
     */
    void VolumeIconChange(bool aFlag,int aValue);               
private:
   /**
    * IsSliderVisual  
    */
   bool IsSliderVisual(CAlfVisual *aVisual);


    /** returns pixel values
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
    bool initializeThumbDragData( const TPoint& aPointerPos );     
    
    /**
    * Stop drag 
    */
    bool stopDrag();
    
   
    /**
    * Handles pointer Events
    */
    void handlePointerUpEvent();
    /**
    * Snaps primary value to ticks
    */
    void snapPrimaryValueToTicks();
    /**
    * Set images for three part track
    *
    */
    void setTrackImage();
    /**
    * Checks if the track visual is Hit
    */
    AlfEventStatus trackVisualHit(CAlfVisual * aHitVisual);
     /**
     * layout the visuals 
     * 
     */
    void layoutVisuals(sliderTemplate aTemplateId);
private: // Class Data
    
//	auto_ptr< MulSliderHorizontalImpl>mData; // Owned
    struct MulHorizontalSliderDataImpl* mData;    
//    auto_ptr<SliderDefaultLCTBaseElementData> mData;
	auto_ptr<MulSliderPos> mPosData; // Structure for storing current and previous values
//	const UString& mStyle;        
    auto_ptr<MulSliderLongTapTimer> mLongTapTimer;
    CAlfVisual *mDragVisual;  
    MulSliderModel * mSliderModel;
    MulSliderWidget * mSliderWidget;  
    }; // end of class AlfScrollBarDefaultLCTBaseElement
    
    } // end of namespace Alf
    
#endif //MUL_SLIDERHORIZONTAL_H
//End Of File
