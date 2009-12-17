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
* Description:  Header file for presentation elements.
 *
*/

  
#ifndef ALF_SCROLLBAR_DEFAULT_BASEELEMENT_H
#define ALF_SCROLLBAR_DEFAULT_BASEELEMENT_H

//INCLUDES
#include "alf/alfelement.h"
#include <alf/alflayout.h>
#include <alf/alfimagevisual.h>

#include <alf/ialfwidgeteventhandler.h>
#include <alf/ialfscrollbardefaultbaseelement.h>
#include "ialfscrollbarbaseelementinternal.h"

#include "alfscrollbarmodel.h"
#include <AknLayout2Def.h>

using namespace osncore;

namespace Alf
    {
   
enum TOperation
    {
    ENop,      // Nothing is going on.
    EMoveItem   // Item is begin moved.
    };
// Forward declarations
class IAlfWidgetEventHandler;
class CAlfWidgetControl;
class AlfScrollBarLongTapTimer;

/**
 * Default Base Element of the Scrollbar.
 */
class AlfScrollBarDefaultBaseElement : 
    public IAlfScrollBarDefaultBaseElement, 
    public AlfElement, 
    public IAlfWidgetEventHandler,
    public IAlfScrollBarBaseElementInternal
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
    AlfScrollBarDefaultBaseElement (
        CAlfWidgetControl& aControl, 
        const char* aName, 
        DuiNode* mNode = NULL, 
        AlfCustomInitDataBase* aCustomData=NULL);

    /**
     * Destructor 
     */    
    virtual ~AlfScrollBarDefaultBaseElement();

    //--------------------------------------------------------------------------
    // APIs from the IAlfScrollBarDefaultBaseElement for controlling the 
    // visualization of the scrollbar
    //--------------------------------------------------------------------------

    /**
     * Sets the animation time for the thumb movement.
     * Controls the movement of the thumb in the context of scroll
     * @since S60 ?S60_version
     * @param Animation time in milliseconds
     */
    void setThumbAnimationTime(int aTime);

    /**
     * Gets the Thumb animation time.
     * @since S60 ?S60_version
     * @return Animation time.
     */
    int getThumbAnimationTime() const;

    /**
     * Sets the Opacity of thumb. This is relative to widget opacity.
     * Default value is 1.0. The value of aOpacity should be between 0.0 to 1.0
     * @since S60 ?S60_version
     * @param Opacity of the thumb
     */
    void setThumbOpacity(float aOpacity);

    /**
     * Gets the Opacity of thumb.
     * @since S60 ?S60_version
     * @return: Thumb opacity
     */
    float getThumbOpacity() const;
    
    /**
     * Sets the Opacity of widget.
     * Default value of scrollbar opacity is 1.0.
     * 
     * @param aOpacity Opacity of the scrollbar. The value should be between 0.0
     *                  to 1.0. 
     *
     * @exception AlfException
     */
    void setOpacity(float aOpacity);

    /**
     * Gets the Opacity of widget.
     *
     * @return: Thumb opacity
     */
    float getOpacity() const;
    
    //--------------------------------------------------------------------------
    // APIs from the IAlfScrollBarBaseElement 
    //--------------------------------------------------------------------------
    

          
    //--------------------------------------------------------------------------
    // Overriden Apis from IAlfWidgetEventHandlers
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
    friend class AlfScrollBarLongTapTimer;

    //--------------------------------------------------------------------------
    //Overriden Apis from IAlfScrollBarBaseElementInternal
    //--------------------------------------------------------------------------
    //

    /**
     * Notified when the base layout of scrollbar changes .
     * Relayouts all the visuals with respect to the new size.
     */
    void baseLayoutSizeChanged();
    
    /**
     * Send the corresponding Custom events to the scrollbar eventhandlers 
     * according to the visual clicked. 
     */
    bool processPointerEvent(TAlfEvent * aPntrEvent); 
    
    /**
      * Creates the Visualization of scrollbar
      * 
      */
    void createVisualization(); 
    
    /**
      * Updates the Visualization of scrollbar
      * 
      */
    void updateVisualization();
    
    /**
      * Destroy the Visualization of scrollbar
      * 
      */
    void destroyVisualization();
    
    /**
     * Returns pointer to AlfScrollBarModel
     * 
     */
    void initializeScrollModel();
    
    /**
     * handles longTap  event
     */
    AlfEventStatus handleLongTapEvent();
    
    // ---------------------------------------------------------------------------
    // From class IAlfInterfaceBase.
    // 
    // ---------------------------------------------------------------------------
    //
    /**
     * Getter for interfaces provided by the widget.
     * @since S60 ?S60_version
     * @param aType A descriptor to identify the type of the queried interface.
     * @return The queried interface, or NULL if the interface is not
     *         supported or available.
     */
    virtual IAlfInterfaceBase* makeInterface(const IfId& aType);
            
    
    //--------------------------------------------------------------------------
    // Internal member functions AlfScrollBarDefaultBaseElement
    //--------------------------------------------------------------------------
    //
 
    /**
     * handles the pointerDown event
     */
    bool handlePointerDownEvent(TAlfEvent * aPntrEvent);
    
    /**
     * Sends pageup or pagedown  event according to hit point on track
     */
    AlfEventStatus trackVisualHit(CAlfVisual * aHitVisual);
    
    /**
     * handles the pointerUp event
     */
    void handlePointerUpEvent();
    
    /**
     * Initializes the scrollbar layouts with LCT layout data
     */
    void initializeLayout();
    
    /**
     * Initializes the scrollbar Tracklayout with LCT layout data
     */
    void initializeTrackLayout();
    
    /**
     * Initializes the scrollbar Thumblayout with LCT layout data
     */
    void initializeThumbLayout();  
    
    /**
     * Sets position and Size on the Visual/layout
     * @param aVisual   Visual/Layout on which position and size is being set
     * @param aXVal     Position X value
     * @param aYVal     Position Y value
     * @param aWidth    Width 
     * @param aHeight   Height
     */
    void setPositionAndSize(CAlfVisual * aVisual,
                            int aXVal, int aYVal, 
                            int aWidth, int aHeight);
    
    /**
     * Set skin images on scrollbar track
     */
    void setTrackImage();
    
    /**
     * Set skin images on scrollbar thumb
     */
    void setThumbImage();
    
    /**
     * Changes the scrollbar track image, based on the flag state
     * @param aPressed   false for normal graphics,
     * 					 true  for pressed graphics
     */    
    void changeTrackImage(bool aPressed = false);
    
    /**
     * Changes the scrollbar thumb image, based on the flag state
     * @param aPressed   false for normal graphics,
     * 					 true  for pressed graphics
     */    
    void changeThumbImage(bool aPressed = false);
    
    /**
     * Calculates the StepSize and thumbLength 
     */
    void  calculateStepSizeAndThumbLength();
    
    /**
     * Calculates minimum and maximum thumbLength 
     */
    void setThumbMinMaxLength();
 
                
private:
   
    /**
     * handles the scroll bar click event
     *
     */
    AlfEventStatus handleScrollBarClickedEvent(
            const TAlfEvent& aEvent );
   /**
     * Creates the visual template hirarchy of scrollbar.
     */
    void createDefaultBaseElement();
    
    /**
     * Sets LctLayoutData on the Visual/layout
     * @param aRect     	   Parent rect of the visual
     * @param aWindowLineLayout TAknWindowLineLayout handle which has the LCT values
     * 						  to be set
     * @param aVisual         Visual/Layout on which LctLayoutData is being set
     */
    void setLCTData(const TRect& aRect ,
                    TAknWindowLineLayout& aWindowLineLayout,CAlfVisual * aVisual);
    
    /**
     * Initialize drag flags and store drag start point 
     */
    bool initializeThumbDragData( const TPoint& aPointerPos );
    
    /**
     * Stop drag 
     */
    bool stopDrag(CAlfVisual* aHitVisual, TInt aAnimTime);
 	
    /**
     * changes the graphics of scrollbar to pressed/normal  
     */
    void changeGraphics();
    
    /**
     * checks whether it is scrollbar trackVisual or not  
     */
    bool isScrollbarTrackVisual(CAlfVisual *aVisual);
    
     /**
     * changes the graphics of scrollbar to pressed/normal  
     */
    bool isScrollbarThumbVisual(CAlfVisual *aVisual);
               
    /*
     * Instance of ScrollBarLongTapTimer.
     */    
    auto_ptr<AlfScrollBarLongTapTimer> mLongTapTimer;
    
    /*
     *  holds the value if minimum thumb size.
     */ 
    TAlfMetric mThumbMinSize;

    /*
     * holds that value of minimum thumb size specified by the user.
     */ 
    TAlfMetric mUserThumbMinSize;

    /*
     * Flag set to true if minimum thumbsize is set.
     */ 
    bool mIsMinSizeFlagSet;

    /*
     * Flag set to true if longTapObserver is set.
     */ 
    bool mIsLongTapObserverSet;

    /*
     * pointer to scrollbarThumbLCTData.
     */ 
    struct ScrollbarThumbLCTData *mLCTData;
    
    /*
     * pointer to ScrollBarDefaultLCTBaseElementData.
     */     
    struct ScrollBarBaseElementData *mData;
   
    }; // end of class AlfScrollBarDefaultBaseElement
    
    } // end of namespace Alf
    
#endif //ALF_SCROLLBAR_DEFAULT_BASEELEMENT_H
//End Of File
