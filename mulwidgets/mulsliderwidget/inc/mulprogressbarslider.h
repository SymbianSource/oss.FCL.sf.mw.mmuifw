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
* Description: Header for slider presentation elements.
 *
*/

 
#ifndef MUL_SLIDERPROGRESSIVE_H
#define MUL_SLIDERPROGRESSIVE_H

//INCLUDES

#include "alf/alfelement.h"
#include "mulslidermodel.h"
#include "imulsliderbaseelementinternal.h"
#include <alf/alfbitmapprovider.h>
#include <alf/ialfwidgeteventhandler.h>
namespace Alf
    {
    class CAlfWidgetControl;
	/**
	 * Default Base Element of the slider.
	 */
	class MulSliderProgressive : 
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
		    MulSliderProgressive (
		        CAlfWidgetControl& aControl, 
		        const char* aName, AlfCustomInitDataBase* /*aCustomData*/);
			 /**
		     * Destructor 
		     */    
		    virtual ~MulSliderProgressive();
    
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
		     * Getter for interfaces provided by the  widget class.
		     * @since S60 ?S60_version
		     * @param aType A descriptor to identify the type of the queried
		     *  interface.
		     * @return The queried interface, or NULL if the interface is not
		     *         supported or available.
		     */
		     virtual IAlfInterfaceBase* makeInterface(const IfId& aType);
		      /**
		     * @see IAlfWidgetEventHandler
		     */
		    virtual AlfEventHandlerType eventHandlerType();
			/**
		     * @see IAlfWidgetEventHandler
		     */
		    virtual AlfEventHandlerExecutionPhase eventExecutionPhase();
		    
		    
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
		     * a custom event ID that can be used to
		     *  simulate the actual pointer event.
		     * @since S60 ?S60_version
		     * @param aEvent The event.
		     * @param aControl
		     * @return <code>ETrue</code> if the event can be processed. 
		     * Otherwise <code>EFalse</code>.
		     */
		    bool accept(
		        CAlfWidgetControl& aControl, const TAlfEvent& aEvent) const;
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
		     * By default, if this method 
		     * is not called, the event handler is expected
		     * to be active in all states.
		     * @param aStates A bitmask defining the widget
		     * states, where the event
		     * handler is active. The low 16 bits are reserved for the states
		     * defined by the framework in <TODO: add reference>.
		     * The high 16 bits are
		     * available for states defined by the client code.
		     */
		    void setActiveStates(unsigned int aStates);
		    /**
		     * Sets AlfWidgetEventHandlerInitData to event handler.
		     * @param aData A data structure which contains
		     * for example id of the event
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
		        
	    	//--------------------------------------------------------------------------
		    //Overriden Apis from IMulSliderBaseElementInternal
		    //--------------------------------------------------------------------------
		    //
	  
		    /**
		     * Notified when the base layout of slider changes .
		     * Relayouts all the visuals with respect to the new size.
		     */
		    void baseLayoutSizeChanged();
		   
		     /**
		     * update visualization
		     *
		     */  
		    void updateVisualization();
		   
		    /**
		     * create visulization for slider
		     * @param aTemplateId - template id
		     */ 
		    void createVisualization(sliderTemplate aTemplateId);
		    		    
		    
		    /**
		     * Updates Text visualization.
         	 * 
		     */
		    void updateTextVisualization();
		    
		private:
            
            /**
             * layoutVisuals setpos visulization for slider
             * @param aTemplateId - template id
             */ 
		    void layoutVisuals(sliderTemplate templateid);
            
		    /**
		     * initializeSliderData to initialize the model and widget pointers
		     */
			 void initializeSliderData();
		   
		   /**
			 * Normalise the template specific data with that from the Model.
			 */
     	
		 	 void NormalizeRange();
		    /**
			 * Sets the position of the handle
			 * 
			 * @param aCurrTick The new position of the progress bar
			 * 
			 */
		     void SetTickPosition(int aCurrTick);
	
	   	 private: // Class Data
		    struct MulSliderProgressBarImpl* mData;
		    MulSliderModel * mSliderModel;
		    MulSliderWidget * mSliderWidget;
		    CFbsBitmap* iBitmap;
	    }; // end of class
	    
    } // end of namespace Alf
    
#endif //MUL_SLIDERPROGRESSIVE_H
//End Of File
