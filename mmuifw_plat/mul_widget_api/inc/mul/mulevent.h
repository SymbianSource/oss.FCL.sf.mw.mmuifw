/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Event class.
*
*/


#ifndef T_MULEVENT_H
#define T_MULEVENT_H

namespace Alf
{
    enum TMulEventType
        {
        ETypeHighlight= 200, 		        /*!< A message regarding an item getting highlighted.
                                         *  Data for the event will be the highlighted index(int).*/
        ETypeSelect,			        /*!< a message regarding an item getting selected.
                                         *  Data for the event will be the selected index(int).*/
        ETypeDoubleTap,			        /*!< a message regarding an item getting double tapped.
                                         *  Data for the event will be the selected index(int).*/
        ETypeMark,		                /*!< a message regarding start/stop of marking.
                                         *  Data for the event will be marking state(TMulMarkState).*/
        ETypeFastScroll,	            /*!< a message regarding start/stop of fast scrolling
                                         *  Data for the event will be fast scrolling state(TMulFastScrollState).*/
        ETypePrimaryValueChange,		/*!< a message regarding slider primary value getting changed
                                         *  Data for the event will be previous & current value 
                                         *  encapsulated within TMulSliderPos.*/
        ETypeSecondaryValueChange,	    /*!< a message regarding slider secondary value getting changed
                                         *  Data for the event will be previous & current value 
                                         *  encapsulated within TMulSliderPos.*/
        ETypeAction,                     /*!< a message regarding an action item being selected
                                         *  Data for the action event will be encapsulated within TMulActionItem.*/
                                         
        ETypeRemove,                     /*!< a message regarding an item has to be removed.
                                         * Data for the event will be the highlighted index(int).*/
                                         
        ECustomEventMarqueeFinished,     /*!< A message regarding marquee Finished.
                                         *  no custom data required.*/
        
        ECustomEventMarqueeStart,        /*!< A message regarding marquee started.
                                         *  no custom data required.*/ 
                                         
        ECustomEventIconClick,            /*!< A message regarding clicking on icon.
                                         *  no custom data required.*/ 
                                         
        EMulEventReorder,                 /*!< Data for the event will be previous and current index
                                         *  encapsulated within MulReorderValues .*/               

        EMulEventSwap,                    /*!< Data for the event will be previous and current index
                                         *  encapsulated within MulReorderValues .*/

        EMulEditorUpdated,                /*!< A message regarding plain text editor of an item is changed.
                                         *  Data for the event will be the item index(int).*/
                                                                                         
        ECustomHighlightClearTransition,  /*!< A message regarding transition after clear focus and before set focus finished.
                                           *  no custom data required.*/
        
        ECustomHighlightFocusTransition,   /*!< A message regarding transition after set focus finished.
                                            *no custom data required.*/
                                           
        ETypeItemMarked,                    /*!< a message regarding an item getting marked.
                                             * Data for the event will be the marked index(int).*/
                                                 
        ETypeItemUnmarked,                  	 /*!< a message regarding an item getting unmarked.
                                            	 * Data for the event will be the unmarked index(int).*/
                                         
        ECustomEventTitleMarqueeFinished,     	 /*!< A message regarding title marquee Finished.
                                         	  	 * no custom data required.*/
                                         
        ECustomEventDetailMarqueeStart,    		/*!< A message regarding detail marquee started.
                                            	 *  no custom data required.*/ 
                                            	 
        ECustomEventContinuousScroll, 			/*!< A message regarding Continuous Scrolling.
                                         		*  Data for the event is the View Start Position.*/
       
        ESliderDrag, 				  			/*!< A message regarding Slider starting the drag
                                         		*/
			                                   
		EVideoIconSelect,          				/*!< A message regarding there was tap on the video icon of the Coverflow widget full screen template.
			                                   	*  no custom data required.*/  
			                                   	
		ECustomEventVisiblePageIndexChanged,	/*!< a message regarding change of visible page
                                               	* Data for the event will be the first index of the new visible page.*/ 
                                               	
		ETypeSwitchUiStateOnDrag,                /*!< a message to switch off the ui when drag starts.
                                               	* no custom data required. This event is specific to coverflow widget*/
		
		ECustomEventIconRelease,                /*!< A message regarding release on icon.
                                                * no custom data required. */   
		
		ETypePinch,                             /*!< A message regarding a multitouch pinch gesture identified in coverflow */  
		
		ETypeItemRemoved                        /*!< A message regarding a remove operation completed. Requested by photos for refreshing
                                                     the HDMI view when an item is deleted, No custom data  */ 
        };
                                                       
        
    /*! @enum TMulMarkState
     * Marking state of the widget.
     */
    enum TMulMarkState
        {
        EMarkStart,         /*!< A state which indicates that the marking has started.*/
        EMarkStop           /*!< A state which indicates that the marking has stopped.*/
        };
        
    /*! @enum TMulFastScrollState
     * Fast Scroll State of the widget.
     */
    enum TMulFastScrollState
        {
        EScrollStart,       /*!< A state which indicates that the fast scrolling has started.*/
        EScrollSmallIcon,   /*!< A state which indicates that the fast scrolling is using 
                             *   the smaller size of the thumbnails.*/
        EScrollLargeIcon,   /*!< A state which indicates that the fast scrolling is using 
                             *   the larger size of the thumbnails.*/
        EScrollStop         /*!< A state which indicates that the fast scrolling has stopped.*/
        };
        
    /*! @struct TMulSliderPos 
     * The structure holds the data change details of the slider. It stores both the previous
     * value and current value of the slider thumb.
     */
    struct MulSliderPos
        {
        /* @var Original value of the slider thumb.*/
        int mPreviousValue;     
        
        /* @var New value of the slider thumb.*/
        int mCurrentValue;      
        };
    
   struct MulReorderValues
        {
        /* @var old value of the item index.*/
        int mOldIndex;     
        
        /* @var New value of the item index */
        int mNewIndex;      
        };    
   
   struct MulDoubleTapData
       {
       TPoint mDoubleTapPoint;
       TInt mTapVisualIndex;
       MulDoubleTapData() {
       mDoubleTapPoint.SetXY(0,0);
       mTapVisualIndex = -1 ;
       }
       }; // end of structure
       
        
}//namespacealf
#endif // T_MULEVENT_H
