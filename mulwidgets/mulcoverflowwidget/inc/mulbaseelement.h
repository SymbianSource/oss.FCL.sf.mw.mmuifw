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
* Description:  base element header for CoverFlow
*
*/

// This file defines the API for MulCoverFlowWidget.dll

#ifndef __MULBASEELEMENT_H__
#define __MULBASEELEMENT_H__


//  Include Files
#include <alf/alfdecklayout.h>
#include "alf/alfelement.h"
#include <alf/alfflowlayout.h>
#include <alf/alfwidgeteventhandler.h>
#include <mul/imulwidget.h>

#include "mulcoverflowdefinitions.h"

namespace Alf
    {
    
//Forward declarations    
class MulCoverFlowTemplate; 
struct TMulBaseElementImpl;

// Structure to store the position and sizes of cover flow items
struct TMulCoverFlowItem
    {
    int height;
    int width;
    int posx;
    int posy;
    
    TMulCoverFlowItem()
    	:height(0),width(0),posx(0),posy(0){}

    TMulCoverFlowItem(int aPosx,int aPosy,int aWidth,int aHight)
    	:posx(aPosx),posy(aPosy),width(aWidth),height(aHight){}
        
    };

namespace mulbaseelement
    {
     static const IfId Ident=
        {
        0,"mulbaseelement"
        };
    }


/* Element class which handles all the visuals in the layout
 * The class also handles  event handling and applies animation as required
 * by the widget.The class forms the basis of the entire layout heriarchy in the widget
 * */
/// @bug minor:avanhata:7/7/2008 This class is coverflow-specific, so name it as such
class MulBaseElement : public AlfElement, 
                       public IAlfWidgetEventHandler
    {
public://Constructor and destructor
    
   /**
    * C++ constructor.
    */
    MulBaseElement( CAlfWidgetControl& aControl, const char* aName );
    
    /**
     * Destructor.
     */
    virtual ~MulBaseElement();
            
        
public: //From IAlfWidgetEventHandler

    static inline const IfId& Type()
        {
        return mulbaseelement::Ident;    
        } 
        
    bool accept( CAlfWidgetControl& aControl, const TAlfEvent& aEvent ) const;
    
    void setActiveStates( unsigned int aStates );
    
    AlfEventStatus offerEvent( CAlfWidgetControl& aControl, const TAlfEvent& aEvent );
    
    IAlfInterfaceBase* makeInterface( const IfId& aType ); 
    
    void setEventHandlerData( const AlfWidgetEventHandlerInitData& aData );
	  
	AlfWidgetEventHandlerInitData* eventHandlerData();
	
	AlfEventHandlerType eventHandlerType() ;

    AlfEventHandlerExecutionPhase eventExecutionPhase() ;

public: //New Methods

    /**
	 * Sets the Item Brush Color.
	 * @param aColor color to be set.
	 */
    void SetItemBrush(const TRgb& aColor); 
    
     /**
	 * changes the opacity of counter accordingly.
	 * @param aUiOn true if UIOn or else false.
	 * @param aTransitionTime transition time.
	 */
    void UIOnOffMode(bool aUiOn, int aTransitionTime);
    
    /**
	 * Sets the Scroll direction during the step scroll.
	 * @param TItemScroll sets the direction if it is right or left.
	 */
    void SetTextColor(TTextVisualType aVisualType, const TRgb& aColor);
    
    /**
	 * returns the color of the specified text
	 * @param aVisualType is the visual type.
	 */
	TMulVisualColorProperty TextColor(TTextVisualType aVisualType);
    
    
    /**
	 * Sets the Scroll direction during the step scroll.
	 * @param TItemScroll sets the direction if it is right or left.
	 */
    void SetScrollDir( TItemScroll aScrollDir );
    
    /**
     * Update the text opacity and counter values whenever highlight is modified
     *
     * @param aAnimationTime, text will be updated after the aAnimationTime.
     */
    void UpdateTextAndCounter(int animationTime = 0); 
    
    /**
     * Stores the dimensions of the highlighted and nonhighlighted item from the template.
     * 
     * @param aHighlightItemDimensions The highlight item dimensions .
     * @param aNonHighlightItemDimensions The non highlight item dimensions.
     * @param aVisibleIconCount The count of visible items on screen.
     */
    void StoreVisualDimensions( TMulCoverFlowItem aHighlightItemDimensions, TMulCoverFlowItem aNonHighlightItemDimensions,int aVisibleIconCount );
    
    /**
     * Stores whether the text is visible or not in the current orientation and template.
     *
     * @parama aIsTextVisible Boolean value indicating whether text is there or not.
     */
    void StoreTextVisibility( bool aIsTextVisible );
    
    /**
     * Return is the textvisible or not
     * 
     */
    bool IsTextVisibile( ) const ;
    
	/**
	 *  
	 *
	 */
    void ApplyBrushOnIcon( CAlfImageVisual& aImageVisual );
    
    /**
	 *  
	 *
	 */
	void RemoveBrushOnIcon( CAlfImageVisual& aImageVisual );
        
 	/**
	 * To hide and unhide Widget.
	 *
	 * @param aShow                 value to hide/unhide the Widget 
	 * @param aTransitionTime       Transition Time in MilliSeconds
	 */
	void ShowWidget( bool aShow, int aTransitionTime = 0) ;
	
	/*
	 * Takes the new positions of the visuals and applies the same to the visuals
	 * On change of orientation
	 */
	void OrientationChange();
	
	/**
	 * Sets the empty text to be displayed when the number of items is 0.
	 * 
	 * @param aDefaultText The text to be displayed.
	 */
	void SetEmptyText( const UString& aDefaultText );
	
	/**
	 * Shows/Hide the empty text when the number of items in the model or layout is 0 . Also when the model is NULL.
	 * 
	 * @param aShow Boolean value to show/hide the empty text.
	 */
	void ShowEmptyText( bool aShow );
	
	/**
	 * Returns true if empty text is set by the application else returns false.
	 * 
	 * @returns true if empty text is set else returns false.
	 */
	bool IsEmptyText();
	
	/**
	 * Updates the Slider Widget ticks when item is added or removed
	 *
	 * @param aUpdateTick takes the count of items in the coverflow widget
	 */
	void UpdateSliderTick(int aUpdateTick);

	/**
     * Returns the icon/text flow layout in vertical flow layout
     * 
     * @param aLayoutIndex the index will determine which layout to return from the main/vertical flow layout
     */
    CAlfLayout& FlowLayout( int aLayoutIndex );
      
	 /**
	  *
	  * Sets the text to be displayed for the counter element .
	  *
	  * @param aTransitionTime the transition time in milliseconds for animation 
	  */
	 void SetCounterText( int aTransitionTime = 0 );
    
    /**
     * Creates a visual for counter if not created initailly else return the existing counter visual.
     */
    CAlfTextVisual& CounterVisual();
      
    /**
     * Move the Iconflow by a items width in case of bounce effect.
     * 
     * @param aBounceDirection Tells the direction by which the iconflowlayout should move.
     */
    void StartBounce(int aBounceDirection); 
    
    /**
     * Returns whether its in Ui on mode or not.
     * 
     * @return true if its currently in Ui on mode.
     */    
    bool IsUiOnMode();    

	/**
	 * Creates the text visuals if it is not created, else returns the existing 
	 * visual for the title and detail text.
	 * 
	 * @param aTextString, visual tag
	 * @param aLayout, Layout in which to create the text visual
	 */      
    CAlfTextVisual& CreateTextVisual(const TDesC8& aName, CAlfLayout& aLayout); 
	
	/**
	 * Updates the value of the text visuals when an image has scrolled .
	 * 
	 * @param aAnimationTime, text will be updated after the aAnimationTime.
	 */
	void UpdateTextValue( int aAnimationTime = 0);
	
	/**
	 * returns the layout to text.
	 */
	CAlfLayout& TextLayout();
    
    /**
     * Start marquee for the text.
     * 
     * @param aName, text type is title or detail 
     */
    void StartMarquee(mulvisualitem::TVisualAttribute aName );
    
    /**
     * Stop marquee for the text.
     * 
     * * @param aName, text type is title or detail 
     */
	void StopMarquee(mulvisualitem::TVisualAttribute aName);
	
	/**
	 * Decide whether to show or hide the slider based on the parameter.
	 *
	 * @param aFlag, boolean to decide slider to be shown or not.
	 */	
	void ShowSlider(bool aFlag);
	
	/**
	 * Return size of hightlight icon for the template specified .
	 *
	 * @param aTemplateId, id of the template whose icon size to be calculated.
	 * @param aAttribute, attribute whose size is to be calculated.
	 * @param aHeight, refrence in which the height parameter is returned.
	 * @param aWidth, refrence in which the width parameter is returned.
	 */	
	void GetIconSize( mulwidget::TLogicalTemplate aTemplateId, 
		    mulvisualitem::TVisualAttribute aAttribute, int& aHeight, int& aWidth);
	
	/**
	 * Sets the Scroll Animation Time defined by the application.
	 *
	 * @param aScrollAnimationTime, ScrollAnimation Time set by the application.
	 */	

	void SetScrollAnimationTime( int aScrollAnimationTime );

	/**
	 * Set background for coverflow
	 *
	 * @param aColor, background color.
	 */
	void SetBackground(const TRgb& aColor);
    
	/**
	 * Set background for coverflow.
	 *
	 * @param aIID, Skin id that shoudl be used as background.
	 */    
    void SetBackground(const TAknsItemID& aIID);
    
    /**
     * Stores the fit mode flag (on or off).
     * 
     * @param aFlag fitmode flag
     */
   	void SetFitMode(bool aFlag);
   	
    /**
     * Apply scale mode on image visual and also appy brush for remaining area.
     * 
     * @param aImageVisual Image visual
     */
    void ApplyScaleMode( CAlfImageVisual& aImageVisual);
    
	/**
	 *  
	 *
	 */    
	void CreateAndInitializeVisuals(bool aIsOrientationChange = false);
	
	/**
	 *  
	 *
	 */	
	void SetHighlight(int aHighlightIndex, int aAnimationTime = 0);
	
	/**
	 *  
	 *
	 */	
	void MoveVisuals(int aDistance,int aAnimationTime = 0);
	
	/**
	 *  
	 *
	 */	
    CAlfVisual* CreateIconStructure(CAlfLayout* aParentLayout);
    
	/**
	 *  
	 *
	 */	
	void MoveVisualsWithSizeChange(int aDistance, int aAnimationTime=0);
	
	/**
	 *  
	 *
	 */	
	void StartDoodling(int aDistance, int aAnimationTime=0);
	
	/**
	 *  
	 *
	 */	
	void StopDoodling(int aAnimationTime=0);
	
    /**
     * Recycle the Icon visuals instead of destroying and creating them each time.
     */
    void RecycleIconVisuals();
    
	/**
	 *  
	 *
	 */		
	void UpdateVisuals();    

	/**
	 * Sets the specified size to the base layout and deck layout and also on the empty text visual
	 */
	void SetDefaultSize( TSize aSize );
	
	/**
	 * Displays the indicator icon for the specified item index.
	 *
	 * @param aVisualItem visual item which conatins the index.
	 * @param aIndex index of the item.
	 */
	void DisplayIndicatorIcon(const MulVisualItem & aVisualItem, int aIndex);
	
	/**
	 * Returns whether the user has doodled enough to change the highlight multiple times.
	 *
	 * @returns TNumberofSwipes, Doodling distance converted into multiple swipes.
	 */
	TNumberofSwipes NumberOfSwipes();
		
	/**
	 *  Returns the final swipe direction based on which highlight is nerest to teh centre point
	 * in the last direction
	 *
	 * @param aDistance, doodled distance
	 * @param, aastSwipeDirection, last direction in which the user has swiped.
	 */	
	int FinalSwipeDirection(int aDistance, int aLastSwipeDirection);	
	
	/**
	 * Returns teh current direction in which itesm are scrolled
	 *
	 * @return, Scroll direction
	 */		
	TItemScroll ScrollDir();

	/**
	 * Removes the slider layout form the coverflow main layout
	 *
	 */	
	void RemoveSliderFromLayout();	
	
	/**
	 *  Rotate the image in teh given direction
	 * 
	 * @param aDirection, Direction of the rotation (ERotateLeft/ERotateRight).
	 * @param aImageSize, The actual size of the current highlighted image.
	 * @param aTransition transition time to rotate the image.
	 * @param aAdjustSize, true for orientation change 
	 *
	 */		
	void RotateImage(IMulWidget::TMulRotation aDirection,TSize aImageSize,int aAnimationTime,bool aAdjustSize = false);
	
	/**
	 *  If the image on which rotation is applied, is updated then remove the rotation
	 *
	 * @param aIndex, relative index which is updated
	 */			
	void CancelRotationOnUpdate(int aIndex);
	
private: 
	
	/**
     * Set the virtual Size of viewportlayout(parent) of text 
     *
     * @param aName, text type is title or detail 
     */
    void SetVirtualViewPortSize(mulvisualitem::TVisualAttribute aName);
	
    /**
     * Cancell all the custom commands related to marquee. 
     */
    void CancelAllCommands(); 
	
	/**
     * Reset counter opacity( depending on the current ui on/off mode
     *  when an item is added to an empty model.
     *
     * @param aTransitioTime transition time defined by app.
     */
    void ResetUiOnOff( int aTransitionTime = 0 );
    
    /**
     * Calculate the new position for counter visual based on the value of the higlight index and total model count
     *
     * @param aCounterLctDimension Lct Dimensions of counter visual.
     * @param aOrientationChanged Orientation Changed or not
     */
    void SetNewCounterPosition(const TMulCoverFlowItem& aCounterLctDimension,bool aOrientationChanged = false );
    
    /**
     * Returns number of digits in a number.
     *
     * @param aNumber The number whose number of digits have to be found.
     * @returns Number of digits in a number.
     */
    int NumberOfDigits(int aNumber);
    
	/**
     * Returns the current 2D coverflow template.
     */	
    MulCoverFlowTemplate*  Template2D();
        
    //slider related apis
	/**
	 * Created the slider widget and calls the other function related to slider.
	 */
	void CreateSlider();
	
	/**
	 * Constructs the slider widget if its not already constructed .
	 */
	void ConstructSliderWidget();
	
	/**
	 * Checks whether slider needs to be shown or hidden and sets its opacity accordingly.
	 *
	 */
	void SetSliderOpacity();
	
	/**
	 * Creates the base layout structure required for the base element to contain
	 * images and slider and meta data information . 
	 */
	void CreateBaseLayoutStructure();
	
	/**
	 * Sets the text data of the current highlight to the  text visual .
	 * 
	 * @param aName The name of the visual  in the text visual e.g mul_tiltle or mul_detail.
	 * @param aData The variant type containing the new data to be set to the text visual .
	 */
	void SetTextToVisual( mulvisualitem::TVisualAttribute aName,IMulVariantType* aData );
		
     /**
      * Create empty text visualisation to show empty text when there are no items to display .
      */
     void CreateEmptyTextVisualisation();
     
     /**
      * Decides the size to be applied on the main layout depending on whether the model is null or not.
      */
     void HandleSizeChangeOnMain();
     
     /**
      * Shows/Hides the text and counter visuals when empty text has to be hidden/shown.
      *
      * @param aShow true when text/counter have to be shown , 
      *              false when text/counter have to be hidden.
      */
     void ShowHideVisualsForEmptyText( bool aShow );
	 
	 /**
	  * Calculates the padding between the images based on the position
	  * and sizes of the highlighted and non higlighted items.
	  */
	 void CalculatePadding();
	 
	 /**
	  *
	  * Sets opacity to the counter text visual .
	  *
	  * @param aOpacity, opacity value that should be set
	  */	 
	 void SetCounterTextOpacity( int aOpacity);	     
	
      /**
	   * Arranging the Visual with padding and using Transition time
	   * 
	   */
	 void ArrangeItems();    

     /**
      * Sets the opacity of all the text visuals in the specifed visual  to the given opacity (aOpacity)
      * 
      * @param aOpacity The opacity to be set for the text visuals .
      * @param aTransitionTime The transition time
      */
     void SetTextVisualOpacity(  int aOpacity , int aTransitionTime = 0 );
     
      /**
	   * Returns the percentage factor with which new Position and Sizes are calculated  .
	   * 
	   * @returns The change in percentage to be applied on the current size and position.
	   */
     int CalculateDistanceChangeFactor(); 
    
	/**
	 *  
	 *
	 */		
	void CreateVisuals();

	 /**
      * Creates a Indicator Image visual if not created initially.
      * 
      * @param aLayout decklayout to which icon is to be added .
      */
	CAlfImageVisual*  CreateIndicatorIconVisual(CAlfDeckLayout& aLayout);

	/**
	 *  
	 *
	 */		
	void  SetIndicatorDimensions(CAlfImageVisual& aIndicatorVisual); 
	
	/**
	 *  
	 *
	 */		
	const TDesC8& IdToTag( int aId );
	
	/**
	 * Creates an array of midpoints of all the visuals form left to right
	 *
	 * @return, array of midpoints
	 */	
	std::vector<int> ReferenceMidPoints();

	/**
	 * Calculates the new midpoints after the doodling
	 *
	 * @param aReferenceMidPoints,midpoints before doodling
	 * @param aDistance, final doodled distance
	 * @return array of new midpoints
	 */		
	std::vector<int> FinalMidPoints(std::vector<int> aReferenceMidPoints,int aDistance);

	/**
	 * Returns the position of the visual in the array which is nearest to midpoint
	 * in swipe direction
	 *
	 * @param aFinalMidPoints, new midpoints after the doodling
	 * @param aLastSwipeDirection, Last swipe direction
	 */		
	int NearestPoint(std::vector<int> aFinalMidPoints,int aLastSwipeDirection);
	
	/**
	 * The image visual size and position is modified to stretch/compress the image
	 * after rotation. This is done because the before rotation teh texture will be 
	 * generated according to the current orientation. To show the rotated texture
	 * image visual is modified such that the texture looks rotated preserving 
	 * the aspect ration
	 *
	 * @param aVisual,Visual for which the rotation is applied
	 * @param aAnimationTime, time for rotation
	 */		
	void AdjustVisualSizeInRotation(CAlfImageVisual* aVisual,int aAnimationTime);

	/**
	 * Handle rotation of 90degree or 270 degree. here the image visuals size
	 * needs to be altered if required as the current width will become teh height of
	 * the texture after rotation.
	 *
	 * @param aVisual,Visual for which the rotation is applied
	 * @param aAnimationTime, time for rotation
	 */		
	void HandleRotationOnVerticalAxis(CAlfImageVisual* aVisual,int aAnimationTime);

	/**
	 * Handle rotation of 180degree or 0 degree. Here no need to modify the image visuals
	 * size.
	 *
	 * @param aVisual,Visual for which the rotation is applied
	 * @param aAnimationTime, time for rotation
	 */	
	void HandleRotationOnHorizontalAxis(CAlfImageVisual* aVisual,int aAnimationTime);
	
	/**
	 * Cancel the rotation applied for teh visual at the given relative index.
	 * Relative index -1 indicates cancel the rotation on highlighted visual
	 *
	 * @param aIndex, relative index of teh visual for which the rotation needs to be canceled
	 */			
	void CancelRotation(int aIndex = -1);
	
	/**
	 * Preserve the rotation after orientation change.
	 *
	 */			
	void UpdateRotation();		

private: //Class Data
    
    auto_ptr<TMulBaseElementImpl> mData; //owned
   
    };
    
    }  // namespace Alf
    
#endif  // __MULBASEELEMENT_H__

//End of file
