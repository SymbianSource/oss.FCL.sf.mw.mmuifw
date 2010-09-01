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
* Description:  The Control header for CoverFlow Widgets.
 *
*/

#ifndef MULCOVERFLOWCONTROL_H
#define MULCOVERFLOWCONTROL_H

// Alf Headers
#include <alf/alfwidgetcontrol.h>

// Osn Headers
#include <touchfeedback.h>

// Gesture Helper
#include <gestureobserver.h>

// Mul Headers
#include <mul/imulvarianttype.h>
#include <mul/imulwidget.h>
#include "imulmodelobserver.h"

//Gesture Helper namespace 
namespace GestureHelper
    {
    class CGestureHelper;
    struct TRealPoint;
    }

//Forward declarations
class CAlfFlowLayout ;
class CAlfImageVisual;

namespace Alf
    {
//Forward declarations
class IAlfWidget;
class IMulModelAccessor;
class IMulSliderModel;
class IMulSliderWidget;
class MulBaseElement;
class MulCoverFlowAo;
class MulCoverFlowTemplate;

struct TMulCoverFlowControlImpl;

namespace mulcoverflowcontrol
    {
     static const IfId Ident=
        {
        0,"MulCoverFlowControl"
        };
    }
    
class MulCoverFlowControl : public CAlfWidgetControl,
							public IMulModelObserver,
							public GestureHelper::MGestureObserver
    {

public: //Constructor and destructor

    /**
     * C++ constructor.
     */
    MulCoverFlowControl( CAlfEnv& aEnv );

    /**
     * Destructor.
     */
    virtual ~MulCoverFlowControl();
    
public: //From CAlfWidgetControl

    AlfEventStatus handleEvent( const TAlfEvent& aEvent );
        
    static inline const IfId& Type()
        {
        return mulcoverflowcontrol::Ident;    
        }
    
    IAlfInterfaceBase* makeInterface( const IfId& aType );
                
    void VisualLayoutUpdated (CAlfVisual& aVisual);
        
public:// From IMulModelObserver
    
	void ModelStateChanged( TMulChangedState aState, IMulVariantType& aData );
    
public:// From MGestureObserver

    /**
     * Handles gesture events and delegates the processing to the base element
     * 
     * @param aEvent Gesture event e.g . EGestureSwipeLeft/Right
     */
    void HandleGestureL( const GestureHelper::MGestureEvent& aEvent );
    

public: //New Methods
	
	/**
	 * Returns the total count that application has set to the model . 
	 * The number of items that application has set to the model i,e the total 
	 * number of items that the application may insert into the model may not
	 * be equal to the total number of items given to the widget for presentation 
	 * by the model . 
	 * 
	 * @returns The number of items that the application has set to the model .
	 */
	int TotalModelCount();
	
    /**
     * Sets the highlight index to the given aIndex
     *
     * @param aIndex, Index to which the current focus visulisation to be moved.
     */		
	void SetHighlightIndex( int aIndex ,bool aUpdateSlider = true);
	
    /**
     * Return the Highlight Item Index maintained in control when update highlight to model is blocked
     *
     * @return, Current Highlight
     */
    int HighlightIndex();
    
    /**
     * Returns the current 2D coverflow template.
     *
     * @return, CoverflowTemplate
     */	
    MulCoverFlowTemplate*  Template2D();
        
    /**
     * Returns whether the coverflow is currently in fast scroll mode or not .
     *
     * @returns true if fast scroll mode is launched, else returns false
     */
    bool IsFastScrollMode();
    
    /**
     * Resets the show widget flag depending on the parameter passed. 
     * 
     * @param aNewResetValue The new value with which the flag needs to be reset.
     */
    void ResetShowWidgetFlag( bool aNewResetValue );
    
    
    /**
     * Returns the accessor to the current model of the widget.
     */
    IMulModelAccessor* ModelAccessor();
     
	
	/**
     * Recycles all the visuals added to the Icon Layout.
     * The function is called whenever there is an model change
     */
    void RecycleVisuals(); 
    
    /**
     * Creates the slider widget . The creation of slider widget depends on the xml . 
     * That is if the xml has a slider tag then only slider widget is created . 
     */
    IAlfWidget* CreateSliderWidget();
    
    /**
     * Returns the created slider widget . If the widget is  not created , then returns NULL.
     */
    IAlfWidget* GetSliderWidget();
	
	/**
	 * Returns the slider model pointer . 
	 *
	 * @returns The slider model pointer if slider widget is created and exists, else returns NULL.
	 */	
	IMulSliderModel* GetSliderModel(); 
	
    /**
     * Check whether we need hold events (Enhanced coverflow can be launched) and
     * set the state to gesture helper accordingly
     */	    
    void SetHoldingEnabled();
    
    /**
     * Enable/Disable double tap on Gesture helper based on the aValue specified.
     * 
     * @param aValue The value whether double tap should be enabled or not.
     */	
    void SetDoubleTapEnabled( bool aValue);
        
    /**
     * Handles the scrolling event and changes the focus accordingly.
     * 
     * @param aEvent the scrolling event to be handled
     */
    void HandleNavigationEvent( int aEvent );
    
	/**
	 * Transition time for highlight change in fast scroll
	 *
	 * @return, transition time to be used
	 */	
	int FastScrollTransitionTime();	 

	/**
	 * Update the item at relative index Of aVisualIndex with the data at aItemIndex
	 *
	 * @param aItemIndex, Absolute index
	 * @param aVisualIndex, Relative index
	 * @param aAnimationTime, text will be updated after the aAnimationTime.
	 */
	void UpdateCoverflowItem( int aItemIndex, int aVisualIndex, int aAnimationTime = 0);
	
	/**
	 * Checks the current index is in visible window and returns its relative index in aRelativeIndex
	 *
	 * @param aIndex, 
	 * @param aRelativeIndex,
	 */	
	bool IsIndexInVisibleWindow(const int aIndex,int& aRelativeIndex);
	
	/**
	 *  
	 *
	 */	
	int RelativeToAbsolute(const int aRelativeIndex);
	
	/**
	 * update the item at the relative index. 
	 * 
	 * @param aVisualIndex, Relative index
	 * @param aAnimationTime, text will be updated after the aAnimationTime.
	 *
	 */	
	void UpdateItemAtIndex(const int aRelativeIndex, int aAnimationTime = 0);

	/**
	 *  
	 *
	 */
    void DoSetImage(IMulVariantType* aImageData,CAlfImageVisual* aImgVisual);
    
   /**
    * Checks the given file path is of SVG image or not
    *
    * @param aImagePath Path of the image file,which need to be checked for SVG file format. 
    */
    bool IsSVGImage(const TDesC& aImagePath);
    
    /**
     * Load Image visual with image provided from file path
     * 
     * @param aSvgFilePath path of Image
     * @param aImageVisual visual on with image to be loaded
     */
    void LoadImageFromSvg(const TDesC& aImagePath,CAlfImageVisual& aImageVisual );
    
	/**
	 *  
	 *
	 */    
    void StoreVisibleItemCount(int aVisibleItemCount);
    
	/**
	 *  
	 *
	 */
	void SetDefaultImage(int aTextureId);
	
	/**
	 *  
	 *
	 */
	int WindowTop(int aHighlightIndex);

	/**
	 *  
	 *
	 */	
	void UpdateBaseElement(MulBaseElement* aBaseElement);
	
    /**
     * Slider handles the key events  .
     * Slider thumb positions itself to the gesture and key events of coverflow widget 
     *
     */
    void SetSliderTickPosition();
	
	/**
	 *  
	 *
	 */	
    void ModelChanged( IMulModelAccessor* aAccessor);

	/**
	 * Determins the current resolution
	 * 
	 *
	 */	
	bool IsLandscape();
    
    /**
     * Destroys slider widget.
     */	
    void DestroySlider(); 
 
    /**
     * Return gesture helper.
     */ 
    GestureHelper::CGestureHelper* Gesturehelper();
    
private:
    
    // Event handling related api.
    /**
     * Handles key event related manipulation. 
     * 
     * @param TAlfEvent
     * @returns The status of the handled event.
     */
    AlfEventStatus HandleKeyEvent( const TAlfEvent& aEvent );
    
    /**
     * Handles repeated key event. 
     * 
     * @param TAlfEvent 
     * @returns The status of the handled event.
     */
    AlfEventStatus HandleRepeatKeyEvent( const TAlfEvent& aEvent );  

    /**
     * Handle fast scrolling and speed variations with repeated key events. 
     * 
     * @param aDirection, Direction in which to change the highlight
     */    
    void HandleFastScrollWithKeyEvents(int aDirection);  
    
    /**
     * Handles custom event that control has raised to itself like bounce,bounce over etc. 
     * 
     * @returns The status of the handled event.
     */
    AlfEventStatus HandleCustomEvent( const TAlfEvent& aEvent );
    
    /**
     * Handles swipe events(only on pointer events from gesture helper)
     * 
     * @param aEvent - SwipeRight/SwipeLeft.
     */
    void HandleSwipe(int aEvent);
    
    /**
     * Performs the focus visualisation for the new focusedindex, and clears the old
     * index visulisation. Called for every navigation events and sethighlight api call.
     *
     * @param aIndex new Index to which the focus visualisation is to be set.    
     */        
    void DoSetFocusIndex( int aIndex, int aAnimationTime =0);
    
    /**
     * Sets the current highlighted item as the selected item . 
     * Sends the selection event to the application . 
     */
    void SetSelection(int aHighlight);
        
    /**
     * Creates a new template element with aTemplateName .
     * 
     * @param aTemplateName The name of thew new template element.
     */
    void CreateTemplateElement( mulwidget::TLogicalTemplate aTemplateName);
        
       
    /**
     * Sends the specified event to the base element .
     * 
     * @param aEvent Event to be sent to the base element.
     */
    void SendEventToBaseElement( const TAlfEvent& aEvent );
    
    /**
     * Handles if there is any change in the total count of model .
     * Updates the slider tick if the total count of model increases/decreases.
     *
     */
     
    void HandleModelCountChange();
            
    /**
     * Gets the absolute index of the specified visual, which has been tapped.
     * 
     * @param aHitVisual The visual which has been tapped or hit.
     */
    int GetHitVisualIndex( CAlfVisual* aHitVisual );
    
    /**
     * Handle enhanced stop.
     */ 
	void HandleEnhancedStop();
   
    /**
     * Performs all the enhanced mode related checkings ..e.g whether the fast scroll flag
     * is set or not / whether the application is in landscape mode or not .
     *
     * @return Boolean value informing whether enhanced mode should be started or not .
     */
    bool EnhancedModeCondition();
    
    /**
     * Performs animations when enhanced mode is started .
     * The coverflow transits to enhanced mode from 2D normal mode .
     *
     * @param aEvent , gesture event
     */
    void EnhancedStarted( const GestureHelper::MGestureEvent& aEvent );
    
    /**
     * Returns the current Gesture Speed. 
     * 
     * @param aEvent Event from the gesture helper.
     * @returns The current gesture speed.
     */
    GestureHelper::TRealPoint GestureSpeed( const GestureHelper::MGestureEvent& aEvent );
    
    /**
     * Performs animations in enhanced mode.
     * Moves the layout along with the pointer event . The function calls the active object class
     * to move the layout with smooth animation .
     *
     * @param aEvent Gesture event .
     */
    void StartMoving( const GestureHelper::MGestureEvent& aEvent );

    /**
     * Handles all normal gesture events other than hold events
     * 
     * @param aEvent Gesture event e.g . EGestureSwipeLeft/Right
     */    
	void HandleNormalGestureEvents(const GestureHelper::MGestureEvent& aEvent);

    /**
     * Handles hold gesture events.
     * 
     * @param aEvent Gesture event e.g . EGestureSwipeHoldLeft/Right
     */
	void HandleHoldGestureEvents(const GestureHelper::MGestureEvent& aEvent);

    /**
     * Handles updating utems when the number of items in model are less than total visible items
     * 
     * @param aIndex, Index to update.
     */	
	void HandleLessItemsUpdate(int aIndex);  

    /**
     * Sets a blank texture the visual at relative index.
     * 
     * @param aRelativeIndex, Index for which the blank texture should be set
     */	
	void SetBlankTexture(int aRelativeIndex);	  


    /**
     * Converts the gesture distance into the form of direction (For right
     * its plus and left its minus)
     * 
     * @param aDistance, Gesture distance
     */
	int ConvertDistanceToDirection( int aDistance );    	 
    /**
     * Sends Tactile feedback in case of touch down event.
     *
     * @param aEvent Gesture event.
     */	
	void SendFeedbackOnTouchDown(const GestureHelper::MGestureEvent& aEvent); 
	
    /**
     * Decides in which direction the strip should move during fast scroll and also decides the position of 
     * reference point for some special cases .
     *
     * @param aEvent Gesture event.
     */
	void ReverseDirectionInFastScroll(const GestureHelper::MGestureEvent& aEvent); 
	
	/**
	 * Change the reference point of reversing the direction until the reference point becomes the 
	 * middle of the screen .
	 *
	 * @param aCurrPosition The current pointer position .
	 * @param aDistanceTravelled The distance travelled by the gesture till now.
	 */
	void ChangeDirReferencePoint(int aCurrPosition, int aDistanceTravelled );
	
	/**
     * Handles the operations on the Single tap.
     *
     * @param aEvent Gesture event.
     */
	void HandleSingleTap(const GestureHelper::MGestureEvent& aEvent); 

	/**
     * 
     *
     * 
     */	
	int TotalSwipeDistance( const GestureHelper::MGestureEvent& aEvent );	 

    	 
private:
    
     auto_ptr<GestureHelper::CGestureHelper> mHelper; //owns it
    
     auto_ptr<TMulCoverFlowControlImpl> mData; // Owned
     
     auto_ptr<MulCoverFlowAo> mCoverFlowAo;   // owned
     
     MTouchFeedback* mFeedback; // tactile feedback, doesnt own it.
     
    IMulSliderWidget* mSliderWidget;  //    
    
    };
    
    } // namespace Alf
    
#endif // MULCOVERFLOWCONTROL_H

//End of file
