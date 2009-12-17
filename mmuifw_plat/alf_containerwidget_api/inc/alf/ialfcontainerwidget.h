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
* Description:  Base interface for all container widgets
*
*/


#ifndef I_ALFCONTAINERWIDGET_H
#define I_ALFCONTAINERWIDGET_H

#include <alf/ialfwidget.h>
class CAlfLayout;

namespace Alf
    {
class IAlfLayoutManager;

namespace ialfcontainerwidget
    {
static const IfId ident =
    {
    0, "container"
    };
    }
    
/**
 * The interface for container widget.
 * 
 * Container widget maintains it's own control group. When a widget control is set to container
 * widget throught the IAlfWidget::setControl() - method the control is automatically appended to the 
 * container widget's control group. If the control that is being set to the container widget is 
 * already part of another control group it will be removed from the previous control group and appended
 * to the container widget's control group.
 *
 * @lib alfwidgetmodel.lib
 * @since S60 ?S60_version
 * @status Draft
 */
class IAlfContainerWidget : public IAlfWidget
    {
public:

    static inline const IfId& type()
        {
        return ialfcontainerwidget::ident;
        }

    virtual ~IAlfContainerWidget() {}

    /**
     * Returns total number of widgets contained by this container.
     *
     * @return int Total number of widgets.
     */
    virtual int widgetCount() const = 0;

    /**
     * Adds a child widget to the container.
     *
     * This will throw an osncore::AlfException exception with error code 
     * osncore::EInvalidArgument if the aWidget is the same widget that the container
     * widget itself or if user tries to add a view widget into a container
     * widget.
     *
     * Adding a widget instance that is already added will be silently
     * ignored.
     *
     * @exception osncore::AlfException with error code osncore::EInvalidArgument if the
     *            aWidget is the same widget as the container widget or when
     *            a view widget is attempted to be added to a container.
     *
     * @param  aWidget Child widget to be added to container.
     */
    virtual void addWidget(IAlfWidget& aWidget) = 0;

    /**
     * Returns child widget at given index. If index is not found return null.
     *
     * @param  aIndex Index of widget to be returned.
     * @return Widget at given index or null if the widget is not found.
     */
    virtual IAlfWidget* getWidget(int aIndex) const = 0;
    
    /**
     * Returns the child index, when given the child widget.
     *
     * @param  aWidget child widget, which index is returned 
     * @return the index of the child widget, or -1, if widget is not child of this container.
     */
    virtual int getWidgetIndex(IAlfWidget& aWidget) const = 0; 

    /**
     * Removes the child widget from given index.
     *
     * If widget for the given index is not found then the call
     * is silently ignored.
     *
     * @param aIndex Index from which widget has to be removed.
     */
    virtual void removeWidget(int aIndex) = 0;

    /**
     * Sets the base layout of container widget. Ownership of the
     * layout manager is transferred to container widget.
     *
     * @param aLayout New base layout of container widget.
     */
    virtual void applyLayout(IAlfLayoutManager& aLayout) = 0;


    };

    } // namespace Alf

#endif // I_ALFCONTAINERWIDGET_H
