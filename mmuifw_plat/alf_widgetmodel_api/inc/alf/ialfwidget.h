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
* Description:  The base class for all widgets.
*
*/


#ifndef I_ALFWIDGET_H
#define I_ALFWIDGET_H


#include <alf/ialfinterfacebase.h>
#include <alf/alftypes.h>
#include <alf/alfvarianttype.h>

namespace duiuimodel
    {
class DuiNode;
    }

using namespace duiuimodel;

class CAlfEnv;

namespace Alf
    {

class CAlfWidgetControl;
class IAlfModel;
class IAlfWidgetFactory;
class IAlfContainerWidget;
class AlfCustomInitDataBase;  


namespace alfwidget
    {
static const IfId ident=
    {
    0,"alfwidget"
    };
    }
/**
 *  An interface for all widgets.
 *  Widgets consist of a model and a control.
 *  The model is owned by the widget, but all
 *  controls are owned by Alfred environment.
 *  The widget can be queried for interfaces provided
 *  by derived widget classes, the control or the model.
 *  Widgets can be constructed using the widget factory.
 *  Widgets are owned and accessed using the Alfred
 *  environment.
 *
 *  @lib alfwidgetmodel.lib
 *  @since S60 ?S60_version
 *  @status Draft
 */
class IAlfWidget : public IAlfInterfaceBase
    {
public:
    static inline const IfId& type()
        {
        return alfwidget::ident;
        }

    /** Virtual destructor. */
    virtual ~IAlfWidget() {}

    /**
     * Getter for the control. The ownership is not passed.
     *
     * @since S60 ?S60_version
     * @return The control of this widget, or NULL if the control is not specified.
     */
    virtual CAlfWidgetControl* control() const = 0;
    
    /**
     * Getter for the parent container. The ownership is not passed.
     *
     * @return the parent container or 0 if the widget is the root-widget.
     */
    virtual IAlfContainerWidget* parent() const = 0;

    /**
     * Sets a new widget control. After this call has been succesfully executed, the control is 
     * owned by this widget. When a new widget control is set to a widget the previously set widget
     * control is removed.
     *
     * Through the aDeletePreviousControl - parameter user can specify whether the previously set 
     * control is destroyed when new control is set. Otherwise the previous control is left on the
     * ownership of the caller. The caller has to manually delete the control or reset it to another
     * widget or control group.
     *
     * If the control passed through this method exists in a control group it is appended to the
     * control group of the view where this widget is contained in and removed from its previous
     * control group.
     *
     * Widget should always have a valid control. An osncore::AlfException is thrown with error code
     * osncore::EInvalidArgument if a control that is already used in another widget is attempted to
     * be used with this widget. You have to replace the control in the source widget first
     * before you can reuse that control in the target widget. See two code snippets below for details:
     *
     * @code
     * // This will throw an osncore::AlfException with error code osncore::EInvalidArgument.
     * targetWidget.setControl(sourceWidget.control());
     * @endcode
     *
     * @code
     * CAlfWidgetControl* control = sourceWidget.control();
     * sourceWidget.setControl(newControl, false);
     * // This is ok, since control is not set in sourceWidget anymore.
     * targetWidget.setControl(control);
     * @endcode
     *
     * @see IAlfViewWidget
     *
     * @param aControl                The new widget control to be used with this widget.
     * @param aDeletePreviousControl  If set to true will destroy the previously set control from UI Accelerator Toolkit Environment.
     *
     * @exception osncore::AlfException If the given control is already set to another widget an osncore::AlfException
     *                                  is thrown with error code osncore::EInvalidArgument.
     */
    virtual void setControl( CAlfWidgetControl* aControl, bool aDeletePreviousControl = true ) = 0;

    /**
     * Getter for the model. The ownership is not passed.
     *
     * @since S60 ?S60_version
     * @return The model of this widget, or NULL if the model is not specified.
     */
    virtual IAlfModel* model() = 0;

    /**
     * Setter for the model. Ownership is passed and the old model is released.
     *
     * @since S60 ?S60_version
     * @param aModel The model for this widget.
     * @param aTakeOwnership Whether takes ownership of model or not
     * return void
     */
    virtual void setModel( IAlfModel* aModel,bool aTakeOwnership = true) = 0;

    /**
     * Get the name of the widget instance.
     *
     * @since S60 ?S60_version
     * @return Widget name
     */
    virtual const char* widgetName() const = 0;

    /**
     * Sets/Releases the Focus from child widget of container.
     * Does not set the actual focused status in roster.
     *
     * @since S60 ?S60_version
     * @param aFocus boolean value for focused status
     * return void
     */
    virtual void setChildFocus(bool aFocus) = 0;

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
    virtual void setPresentation(const char* aFilePath) = 0;    
    };

/**
 * Placeholder for information required to instantiate a widget
 *  via the widget factory mechanism.
 *  A pointer to this structure is casted to a void pointer and sent to the
 *  factory plugin.
 *  @lib alfwidgetmodel.lib
 *  @since S60 ?S60_version
 */
struct AlfWidgetInitData
    {
    /**
     * Owner environment for the widget
     */
    CAlfEnv* mEnv;

    /**
     * Container widget pointer that is going to contain the created widget.
     */
    IAlfContainerWidget* mContainerWidget;
     
    /**
     * Widget instance ID.This uniquely identifies every widget instance
     * and is its name. Also see AlfWidget::Widgetname().
     */
    char*     mWidgetId;

    /**
     * Pointer to node in declaration containing information for the widget.
     */
    DuiNode* mNode;
    
    /**
     * XML file name containing the declaration for the presention of the widget. 
     */
    const char* mFilePath;
    
    /**
     * Pointer to custom data passed via factory mechanism
     * Not Owned.
     */
    AlfCustomInitDataBase* mCustomData;  
    };


    } // namespace Alf

#endif // I_ALFWIDGET_H

// End of File
