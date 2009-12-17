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
* Description:  Widget interface with utilities for slider widget.
*
*/

#ifndef ALF_SCROLLBARWIDGET_H
#define ALF_SCROLLBARWIDGET_H

// ToolKit Includes
#include <alf/alfanchorlayout.h>

// WidgetModel Includes
#include <alf/ialfwidgeteventhandler.h>
#include "alf/alfwidget.h"
#include <alf/alfexceptions.h>

// Widget Includes
#include <mul/imulsliderwidget.h>
#include <mul/imulslidermodel.h>


// Forward Declarations
// From Alfred Client
class CAlfEnv;
class CAlfDisplay;
class CAlfControl;

// From namespace Alf
namespace Alf
    {
// Forward Declarations
class IAlfVariantType;
class IAlfElement;
class IAlfWidgetEventHandler;
class INode;
class IAlfWidgetFactoryBase;
class CAlfWidgetControl;
class IAlfModel;
class AlfElement;
class AlfVisualTemplate;
//class AlfScrollBarWidgetAttributeOwner;
    }

//From name space duiuimodel
namespace duiuimodel
    {
class DuiNode;
    }


namespace Alf
    {
    class IMulSliderBaseElementInternal;

    /**
     * A basic slider widget implementation.
     * Contains methods for creating the default
     * building blocks for the slider widget.
     */
class MulSliderWidget : public IMulSliderWidget
    {
public:

    /**
     * Creator Method.
     * 
     * @param aEnv The environment for the widget.
     * @param aContainer The container widget where this widget is contained.
     * @param aWidgetId The name of the widget. Not Owned.
     * @param aFilePath Path to XML file describing the presentation 
     *                  of the widget. Not Owned.
     * @param aCustomData Custom data. Not Owned.
     * @param aNode Node. Not Owned.
     * @return MulSliderWidget , or NULL if it does not create 
     */
    static MulSliderWidget* create(CAlfEnv& aEnv, 
                                    IAlfContainerWidget& aContainer, 
                                    const char* aWidgetId, 
                                    AlfCustomInitDataBase* aCustomData=NULL);

    /**
     * Virtual Destructor.
     */ 
    virtual ~MulSliderWidget();

    // From IAlfInterfaceBase
    /**
     * Getter for interfaces provided by the slider widget class.
     *
     * @since S60 ?S60_version
     * @param aType A descriptor to identify the type of the queried interface.
     * @return The queried interface, or NULL if the interface is not
     *         supported or available.
     */
    IAlfInterfaceBase* makeInterface(const IfId& aType);

    // From IAlfWidget

    /**
     * Return the control
     */
    CAlfWidgetControl* control() const;
    
    /**
     * getter for the parent container. The ownership is not passed.
     * @param none
     * @return the parent container or NULL if the widget is the root-widget.
     */
    virtual IAlfContainerWidget* parent() const;

    /**
     * Setter for the control. The control is owned by the Alfred environment.
     *
     * @since S60 ?S60_version
     * @param aControl The control for this widget.
     * @param aDeletePreviousControl delete previous control.TRUE by default.
     * @return void.
     */
    void setControl(CAlfWidgetControl* aControl, 
                                    bool aDeletePreviousControl = true);
		/**
		API returns the Slider Model of the widget.
    */                                    
    IMulSliderModel& SliderModel() ;
    /**
     * Gets the model.
     */
    IAlfModel* model();

    /** 
     * Sets the model.
     
     * @return void.
     */
    void setModel(IAlfModel* aModel, bool /*param*/);

    /** 
     * Return the widget name
     */
    const char* widgetName() const;

    /**
     * sets the focus to the child.
     
     * @return void.
     */
    void setChildFocus(bool /*aFocus*/);

    
    
    /** 
     * Sets the presentation for the widget using presentation XML file.
     
     * Destroys any existing presentation.
     * @param aFilePath Path to XML file describing the presentation 
     *                  of the widget. Not Owned.
     * @exception AlfWidgetException if no visualization node available 
     *                               in Presentation XML or if no control
     *                               is associated with the widget.
     * @exception bad_alloc 
     * @since S60 ?S60_version
     * @return void.
     */
    virtual void setPresentation(const char* aFilePath);
    
	/**
	 * Set the transparency for the background of the slider widget.
	 *
	 * @param True,  to set background as transparent
	 * @param False, to set background as opaque	
	 */
	void MakeTransparent( bool aTransparent ) ;
	
	/**
	 * Enable/Disable key handling .
	 *
	 * @param aVal: True,  to handle keyevents by Application
	 * 		 	  False, to handle keyevents as defined in the control	
	 */
	void SetHandleKeyEvent( bool aVal ) ;
	/**
	 * API to show the widget
	 * @param aShow - Shows the widget on value True. 
	 * @param aTransitionTime - Transition time for widget to be shown.
	 */
	void ShowWidget(bool aShow, int aTransitionTime =0);
	/**
	 * API returns true if the widget is hidden
	 */
	bool IsHidden();
	/**
	 * API returns the container layout. 
	 */
	const CAlfLayout& ContainerLayout(); 
		/**
	 * Update template .
	 *
	 * @param aTemplateId: template id
	 * 		 	 
	 */
	void changeTemplate(int aTemplateId);
	/**
	 * Adds event Handling for the Widget.
	 *
	 * @param aObserver: Observer of widget.
	 * 		 	 
	 */
	void AddEventHandler( IAlfWidgetEventHandler& aObserver) ;
	/**
	 * Remove an observer to widget
	 *
	 * @param aObserver Observer of widget.
	 */
	 void RemoveEventHandler( IAlfWidgetEventHandler& aObserver); 
	 /**
	 * Returns opcaity of the widget.
	 * 		 	 
	 */
	 bool GetOpacity(); 
	 /**
	 * Returns true if Key handling is enabled by widget.
	 * 		 	 
	 */
	 bool IsKeyEnabled(); 
	 
	 /** 
    	* returns the Slider element 
    	*/
    IMulSliderBaseElementInternal* GetSliderElement();
    
    
protected:

    /**
     * C++ constructor.
     * 
     * @param aEnv The environment for the widget.
     * @param aContainer The container widget where this widget is contained.
     * @param aWidgetId The name of the widget. Not Owned.
     * @param aFilePath Path to XML file describing the presentation 
     *                  of the widget. Not Owned.
     * @param aCustomData Custom data.
     * @param aNode Node. Not Owned.
     */
    MulSliderWidget(CAlfEnv& aEnv, 
                        IAlfContainerWidget& aContainer, 
                        const char* aWidgetId, 
                        AlfCustomInitDataBase* aCustomData=NULL);

    /**
     * Methods to create different default elements and control of scroll bar.
     *
     * @param aEnv The environment for the widget.
     * @param aDisplay The display for the widget.
     * @return void.
     */
    void constructDefault(CAlfEnv& aEnv, 
                            AlfCustomInitDataBase* aCustomData=NULL);

    /**
     * Constructs widget from declaration.
     *
     * @param aEnv The environment for the widget.
     * @param aNode declaration node.
     * @return void.
     */
    void constructComponentsFromNode(CAlfEnv& aEnv, 
                                    DuiNode& aNode);

    /**
     * Constructs widget from declaration. Recursive.
     *
     * @param aNode declaration node.
     */
    void processTreeRecursively( DuiNode& aNode );
    
    /** 
     * Creates the the widget using presentation XML file.
     * Other widget parts are default.
     * @param aEnv Alf Client Environment     
     * @param aFilePath Path to XML file describing the presentation 
     *                  of the widget. Not Owned.
     * @exception AlfWidgetException if no visualization node available 
     *                               in Presentation XML
     * @exception bad_alloc      
     * @since S60 ?S60_version
     * @return void.
     */
    void constructFromPresentationXML(CAlfEnv& aEnv, const char *aFilePath);

    /**
     * Creates the default control.
     *
     * @param aEnv The environment for the widget.
     * @return The Control.
     */
    CAlfWidgetControl *constructDefaultControl(CAlfEnv& aEnv);
    
private :
		/**
			* Creates a Slider elment.
			* @param - aElementId 
			*/
    void CreateSliderElement(const char* aElementId);
    
private:

    /**
     * Base widget. Own.
     */
     
    auto_ptr<AlfWidget> mWidget;
     
    /*
     * IAlfAttributeOwner implementation. Own.
     */
    //auto_ptr<AlfScrollBarWidgetAttributeOwner> mAttributeOwner;
    	 
	IMulSliderBaseElementInternal* mElement;
    IMulSliderModel * mSliderModel; 
    CAlfWidgetControl * mSliderControl;
    bool mOpacity;
    bool mHandleKeyEvent;
    }; // class MulSliderWidget
    
    } // namespace Alf

#endif // C_ALFSCROLLBARWIDGET_H
