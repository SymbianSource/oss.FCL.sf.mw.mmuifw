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
* Description:  Widget interface with utilities for slider widget.
*
*/

#ifndef I_MUL_SLIDERWIDGET_H
#define I_MUL_SLIDERWIDGET_H

//WidgetModel includes
#include <alf/ialfwidget.h>
#include <alf/alfevent.h>
#include <alf/alfwidgetevents.h>
#include <alf/ialfwidgetfactory.h>
#include <mul/imulslidermodel.h>
#include <alf/alflayout.h>
#include<alf/ialfwidgeteventhandler.h>
//osn includes
#include <osn/osndefines.h>
#include <osn/ustring.h>

namespace Alf
    {
namespace mulsliderwidget
    {
    /**
     * @namespace mulsliderwidget
     * 
     * ident specifies the unique identifier for the grid widget.
     */
    static const IfId ident =
        {
        0, "mulsliderwidget"
        };
    static const Utf8* const KBaseElementName = "BaseElement";
    }



/**
 * The data which can be used as last (optional) paramenter of 
 * IAlfWidgetFactory::createWidget function while creating the slider widget 
 * imperatively. If this data is not passed, default values are used
 * to create visualization.
 *
 */
class MulSliderInitData : public AlfCustomInitDataBase
    {
public:
    /**
     * mBaseElementLoadId visualization base element is created 
     *             using Load ID.
     */
    const char* mBaseElementLoadId;
    
    /* mBaseElementData pointed to baseElement custom data
	   * All baseelement related data are in this class
     */
 
    AlfCustomInitDataBase *mBaseElementData;
    };
    
    
/**
 * Interface for the slider widget.
 *
 *
 * Default visualization of slider contains following components.
 * <ul>
 * <li>Handle (The position of the handle shows the current tick position)</li>
 * <li>Track (Place holder for Handle)</li>
 * </ul>
 *
 *
 *  A slider widget is used along with photos,coverflow and list widget.
 * <ul> 
 * <li> template 1 is used i.e. in 2D coverflow as a navigational component.
 * 
 * <li> template 3 is used i.e. in Photos as a pop-up zoom slider.
 *
 * <li> template 7 is used i.e. in Video Centre list view as a progressive
 * downloading track counter/progress bar.
 * There is no potrait version of this template.
 * 
 * </ul>
 * The communication between slider and the widgets happens by means of an
 * event ETypePrimaryValueChange from slider and in response widgets directly
 * update slider's model.
 * Slider handles both Key and Pointer Events.
 *
 * KeyEvents : EStdKeyRightArrow,EStdKeyLeftArrow,EEventKeyDown,EEventKeyUp.
 * PointerEvents : EButton1Down,EDrag.
 *
 *
 */
class IMulSliderWidget: public IAlfWidget
    {
public:
    /**
     * Get the type of widget.
     * 
     * @return The type of widget
     */
    static  const IfId& type()
        {
        return mulsliderwidget::ident;
        }

    /**
     * Gets the Base Element Name.
     *
     * @return The base element's name
     */
    static inline const char* baseElementName()
        {
        return mulsliderwidget::KBaseElementName;
        }
    	/**
		 * Get the model for widget.
		 *
		 * @return model of the slider widget
		 */
		virtual IMulSliderModel& SliderModel() = 0;
		
		/**
		 * Set the transparency for the background of the slider widget.
		 *
		 * @param True,  to set background as transparent
		 * @param False, to set background as opaque	
		 */
		virtual void MakeTransparent( bool aTransparent ) = 0;
		
		/**
		 * Enable/Disable key handling .
		 *
		 * @param aVal: True,  to handle keyevents by Application
		 * 		 	  False, to handle keyevents as defined in the control	
		 */
		virtual void SetHandleKeyEvent( bool aVal ) = 0;    
		/**
		 * Hide or Show widget  .
		 *
		 * @param aShow: True,  Shows the widget
		 * 		 	  False, Hides the widget	
		 * @param aTransitionTime: Transition time to show/Hide.
		 */
		virtual void ShowWidget(bool aShow, int aTransitionTime =0) = 0;
		/**
		 * Returns the state of the Widget(Hide/show).
		 *
		 */
		virtual bool IsHidden() = 0;
		
		/**
		 * Returns the container layout.
		 *
		 */
		virtual const CAlfLayout& ContainerLayout() = 0; 
    	   /**
		 * Add an observer to widget inorder to get event notifications
		 * from the widget. Widget can have multiple observersers.
		 *
		 * @param aObserver Observer of widget.
		 **/
		virtual void AddEventHandler( IAlfWidgetEventHandler& aObserver) = 0;
		/**
		 * Remove an observer to widget
		 *
		 * @param aObserver Observer of widget.
		 */
		virtual void RemoveEventHandler( IAlfWidgetEventHandler& aObserver) = 0;  
    /**
     * Destructor.
     */	
    virtual ~IMulSliderWidget() {}

    };

    } // namespace Alf

#endif // I_MUL_SLIDERWIDGET_H
//End Of File
