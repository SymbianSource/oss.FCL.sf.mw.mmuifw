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
* Description:  Widget interface with utilities for container widget.
*
*/


#ifndef ALFCONTAINERWIDGET_H
#define ALFCONTAINERWIDGET_H

#include "alf/ialfcontainerwidget.h"
#include <alf/alflayout.h>

// Forward declarations
class CAlfEnv;

namespace Alf
{

// Forward declarations
class AlfWidget;

/**
 * Container widget implementation.
 * 
 * Container widget can be used to contain other widgets in an arbitrary layout.
 * To create a container widget see CAlfContainerWidgetFactoryPlugin class.
 * 
 * Container widget creates a control group with ID 0 and stores it's control
 * in it. It will also append all added widget controls into the same control group
 * unless the added widget control is added to its own control group.
 */
class AlfContainerWidget : public IAlfContainerWidget
{
public:

    /**
     * Default constructor.
     * 
     * Use CAlfContainerWidgetFactoryPlugin::CreateProduct() to instantiate a container
     * widget.
     * 
     * @param aWidgetName The widget name or ID to be assigned to this widget.
     * @param aContainer Container widget in which this container is contained. 
     * @param aEnv UI Accelerator Environment instance to which the widget is created. 
     * @param aNode Runtime declaration of the widget 
     * @param aFilePath Path to XML file describing the presentation of the widget 
     */
	AlfContainerWidget(	const char* aWidgetName, 
						IAlfContainerWidget& aContainer, 
						CAlfEnv& aEnv,
						DuiNode* aNode,
						const char* aFilePath);
    
    /**
     * Default destructor.
     */
	virtual ~AlfContainerWidget();
    
    // From Alf::IAlfWidget base class
    
    /**
     * Getter for the control. The ownership is not passed.
     *
     * @since S60 5.0
     * @return The control of this widget, or NULL if the control is not specified.
     */
    virtual CAlfWidgetControl* control() const;
    
    /**
     * getter for the parent container. The ownership is not passed.
     * @return the parent container or NULL if the widget is the root-widget.
     */
    virtual IAlfContainerWidget* parent() const;

    /**
     * Setter for the control. The control is owned by the Alfred environment.
     * The previously set control can be destroyed when new control is set.
     * 
     * The set control is added to the control group of this container widget.
     * The set control is removed from it's previous control group.
     *
     * @since S60 5.0
     * @param aControl The control for this widget.
     * @param aDeletePreviousControl A boolean to indicate whether the previously set control
     * 								 is to be destroyed when this method is called.
     */
    virtual void setControl(CAlfWidgetControl* aControl, bool aDeletePreviousControl = true);

    /**
     * Getter for the model. The ownership is not passed.
     *
     * @since S60 5.0
     * @return The model of this widget, or NULL if the model is not specified.
     */
    virtual IAlfModel* model();

    /**
     * Setter for the model. Ownership is passed and the old model is released.
     *
     * @since S60 5.0
     * @param aModel The model for this widget.
     * @param aTakeOwnership Whether takes ownership of model or not
     */
    virtual void setModel(IAlfModel* aModel, bool aTakeOwnership = true);
    
    /**
     * Get the name of the widget instance.
     *
     * @since S60 5.0
     * @return Widget name
     */
    virtual const char* widgetName() const;   
    
    /**
     * Sets/Releases the Focus from child widget of container.
     * Does not set the actual focused status in roster.
     *
     * @since S60 5.0
     * @param aFocus boolean value for focused status
     */
    virtual void setChildFocus(bool aFocus);    
    
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

    // From Alf::IAlfContainerWidget base class
    
    /**
     * Returns total number of widgets contained by this container.
     *
     * @return int Total number of widgets.
     */
    virtual int widgetCount() const;
    
    /**
     * Adds a child widget to the container.
     * 
     * Ownership of the widget is transferred to this container
     * object.
     * 
     * The control of the added widget is appended to the control group of
     * this container widget unless the control already is appended to
     * another control group.
     *
     * @param  aWidget Child widget to be added to container.
     */
    virtual void addWidget(IAlfWidget& aWidget);

    /**
     * Returns child widget at given index.
     * 
     * If index is not found returns null.
     *
     * @param  aIndex Index of widget to be returned.
     * @return Widget at given index or null if not found.
     */
    virtual IAlfWidget* getWidget(int aIndex) const;
    
    /**
     * Returns the child index, when given the child widget.
     *
     * @param  aWidget child widget, which index is returned 
     * @return the index of the child widget, or -1, if widget is not child of this container.
     */
    virtual int getWidgetIndex(IAlfWidget& aWidget) const;

    /**
     * Removes the child widget from given index.
     * 
     * This will destroy the widget object at the given index.
     *
     * @param aIndex Index from which widget has to be removed.
     */
    virtual void removeWidget(int aIndex);
    
    /**
     * Sets the base layout of container widget.
     *
     * @param aLayout New base layout of container widegt.
     */
    virtual void applyLayout(IAlfLayoutManager& aLayout);

    // From Alf::IAlfInterfaceBase base class

    /**
     * Interface getter.
     * Derived classes should always call the base class method
     * from the overridden MakeInterface.
     *
     * @since S60 5.0
     * @param aType The type id of the queried interface.
     * @return The queried interface, or NULL if the interface is not
     *         supported or available.
     */
    virtual IAlfInterfaceBase* makeInterface(const IfId& aType);

private:
	/**
	 * Constructs widget from declaration.
	 *
	 * @param aNode declaration node.
	 */
	void constructComponentsFromNode();

	/**
	 * Constructs widget from declaration.
	 *
	 * @param aNode declaration node.
	 */
	void processTreeRecursively();

	/**
	 * Constructs widget using the presentation XML.
	 *
	 * @param aFilePath Path to XML file describing the presentation of the widget.
	 */
	void constructFromPresentationXML(const char *aFilePath);

	/**
	 * Constructs control for widget
	 *
	 */
	void constructDefault();
	
	/**
     * Sets default layout mgr.
     */
    void setDefaultLayoutManager();
private:

	/** Pointer to the adaptee AlfWidget - object. */
	auto_ptr<AlfWidget> mWidget;
	
	/**
	 * UI Accelerator Environment where this widget is used.
	 * Not owned.
	 */
	CAlfEnv& mEnv;

};

}

#endif /*ALFCONTAINERWIDGET_H*/
