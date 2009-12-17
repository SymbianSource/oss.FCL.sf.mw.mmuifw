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
* Description:  Widget interface with utilities for scrollbar widget.
 *
*/



#ifndef ALF_SCROLLBARWIDGET_H
#define ALF_SCROLLBARWIDGET_H


// WidgetModel Includes
#include <alf/ialfwidgeteventhandler.h>
#include "alf/alfwidget.h"
#include <alf/alfexceptions.h>

// Widget Includes
#include <alf/ialfscrollbarwidget.h>
#include <alf/ialfscrollbarmodel.h>


// Forward Declarations
class CAlfEnv;

// From namespace Alf
namespace Alf
    {
	// Forward Declarations
	class CAlfWidgetControl;
	class IAlfModel;
    }

//From name space duiuimodel
namespace duiuimodel
    {
class DuiNode;
    }

namespace Alf
    {

    /**
     * A basic scrollbar widget implementation.
     * Contains methods for creating the default
     * building blocks for the scrollbar widget.
     */
class AlfScrollBarWidget : public IAlfScrollBarWidget
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
     * @return AlfScrollBarWidget , or NULL if it does not create 
     */
    static AlfScrollBarWidget* create(CAlfEnv& aEnv, 
                                    IAlfContainerWidget& aContainer, 
                                    const char* aWidgetId, 
                                    DuiNode *aNode=NULL, 
                                    const char* aFilePath=NULL, 
                                    AlfCustomInitDataBase* aCustomData=NULL);

    /**
     * Virtual Destructor.
     */ 
    virtual ~AlfScrollBarWidget();

    // From IAlfInterfaceBase
    /**
     * Getter for interfaces provided by the scrollbar widget class.
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
     * Sets whether or not tactile feedback is enabled.
     *
     * @param aFlag Whether or not tactile is enabled.
     */
    void enableTactile(bool aFlag);

	/**
     * Returns the state of tactile feedback whether it is enabled/disabled.
     *
     */
    bool IsTactileEnabled();
    
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
    AlfScrollBarWidget(CAlfEnv& aEnv, 
                        IAlfContainerWidget& aContainer, 
                        const char* aWidgetId, 
                        DuiNode *aNode=NULL, 
                        const char *aFilePath=NULL, 
                        AlfCustomInitDataBase* aCustomData=NULL);

    /**
     * Methods to create different default elements and control of scroll bar.
     *
     * @param aEnv The environment for the widget.
     * @return void.
     */
    void constructDefault(CAlfEnv& aEnv);
    
    /**
     * Creates the default control.
     *
     * @param aEnv The environment for the widget.
     * @return The Control.
     */
    CAlfWidgetControl *constructDefaultControl(CAlfEnv& aEnv);

private:

    /**
     * Base widget. Own.
     */
    auto_ptr<AlfWidget> mWidget;
    
    IAlfScrollBarModel * mScrollModel; 
    CAlfWidgetControl * mScrollControl;
    }; // class AlfScrollBarWidget
    
    } // namespace Alf

#endif // C_ALFSCROLLBARWIDGET_H
