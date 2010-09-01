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


#ifndef ALFWIDGET_H
#define ALFWIDGET_H

#include <osn/osndefines.h>
#include <alf/ialfwidget.h>
#include <memory>
#include <osn/ustring.h>
#include "alf/ialfcontainerwidget.h"
#include <alf/alfwidgetcontrol.h>

#include <vector>


class CAlfEnv;
class CAlfDisplay;

namespace osncore
    {
template <class T> class AlfPtrVector;
    }

using namespace std;
using namespace osncore;


namespace Alf
    {

//class CAlfWidgetControl;
class IAlfModel;
class IAlfWidgetFactory;
class AlfWidgetImpl;


/**
 * @class AlfWidget alfwidget.h "alf/alfwidget.h"
 * The base class for widgets.
 * @see IAlfWidget IAlfContainerWidget IAlfHostApi
 *
 * @lib alfwidgetmodel.lib
 * @since S60 ?S60_version
 * @status Draft
 * @interfaces IAlfWidget,IAlfAttributeOwner,IAlfContainerWidget,IAlfWidgetControl,IAlfHostAPI,
 *             All interfaces returned by the elements associated with the widget's control,
 *             All interfaces returned by the layout manager associated widget control ,  
 *             All interfaces returned by the model assoiated with the widget.
 */ 
class AlfWidget : public IAlfContainerWidget
    {
public:
    /**
     * Constructor.
     *
     * This will create a default control to the widget to enable the widget
     * containment hierarchy from the beginning. User can replace the default
     * control with custom control by calling the setControl() - method.
     * @exception std::bad_alloc
     *
     * @see setControl()
     *
     * @param aWidgetName The name of the widget.
     * @param aContainer  Container widget that will contain this widget.
     * @param aEnv        Toolkit environment object that will be used to create the
     *                    default control.
     */
    OSN_IMPORT AlfWidget(const char* aWidgetName, IAlfContainerWidget& aContainer, CAlfEnv& aEnv);

    /**
     * Constructor to be used only by view widget.
     * All the other widgets should use the other public constructor to ensure
     * widget hierarchy consistency from start.
     * @exception std::bad_alloc
     *
     * @see AlfWidget(const char*, IAlfContainerWidget&, CAlfEnv&)
     *
     * @param aWidgetName The name of the widget.
     */
    OSN_IMPORT AlfWidget(const char* aWidgetName);

    /**
     * Destructor.
     *
     * It also takes care that all the child widgets contained by this widget
     * are destroyed correctly, provided that they have been instantiated correctly
     * (e.g. through widget factory) and thus added to the ALF environment (CAlfEnv).
     *
     * In other words, client code does not need to take care of destroying the child
     * widgets contained by a container widget.
     */
    OSN_IMPORT virtual ~AlfWidget();

    /**
     * Constructor. The new widget is left on the cleanup stack.
     *
     * @since S60 ?S60_version
     * @param aWidgetName The name of the widget.
     * @return New object. The widgets are owned by Alfred.
     */
    OSN_IMPORT CAlfWidgetControl* control() const;
    
    /**
     * Getter for the parent container. The ownership is not passed.
     *
     * @return the parent container or 0 if the widget is the root-widget.
     */
    OSN_IMPORT IAlfContainerWidget* parent() const;

    /**
     * Sets new widget control to this widget.
     * @exception osncore::AlfException Thrown with error code osncore::EInvalidArgument if aControl is  
     *                                  alreasy associated with another widget.      
     * @see IAlfWidget::setControl()
     *
     * @since S60 ?S60_version
     * @param aControl Control to be set to the widget.
     * @param aDeletePreviousControl If true, the previous widget control is destroyed. 
     */
    OSN_IMPORT void setControl(CAlfWidgetControl* aControl, bool aDeletePreviousControl = true);

    /**
     * Constructor. The new widget is left on the cleanup stack.
     *
     * @since S60 ?S60_version
     * @param aWidgetName The name of the widget.
     * @return New object. The widgets are owned by Alfred.
     */
    OSN_IMPORT IAlfModel* model();

    /**
     * Sets the model for the Widget.
     *
     * @since S60 ?S60_version
     * @param aModel Model to be set to this widget.
     * @param aTakeOwnership Whether takes ownership or not.
     */
    OSN_IMPORT void setModel( IAlfModel* aModel,bool aTakeOwnerShip=true );

    /**
     * Constructor. The new widget is left on the cleanup stack.
     *
     * @since S60 ?S60_version
     * @param aWidgetName The name of the widget.
     * @return New object. The widgets are owned by Alfred.
     */
    OSN_IMPORT const char* widgetName() const;

    //From IAlfInterfaceBase
    /**
     * Getter for interfaces provided by the widget, the control,
     * or the model.
     *
     * @since S60 ?S60_version
     * @param aWidget   The widget to get the interface for.
     * @param aType     The type id of the queried interface.
     * @return The queried interface, or NULL if the interface is not
     *         supported or available.
     */
    OSN_IMPORT static IAlfInterfaceBase* makeInterface( AlfWidget* aWidget, const IfId& aType );

    /**
     * Template getter for interfaces provided by the widget, the control,
     * or the model. The type of the queried interface is specified by the
     * template parameter.
     *
     * @since S60 ?S60_version
     * @param aWidget The widget to get the interface for.
     * @return The queried interface, or NULL if the interface is not
     *         supported or available.
     */
    template <class T>
    static T* makeInterface( AlfWidget* aWidget );

    // from base class IAlfInterfaceBase

    /**
     * From IAlfInterfaceBase.
     * Getter for interfaces provided by the widget classes.
     * Derived classes should always call the base class method
     * from the overridden MakeInterface.
     *
     * @since S60 ?S60_version
     * @param aType A descriptor to identify the type of the queried interface.
     * @return The queried interface, or NULL if the interface is not
     *         supported or available.
     */
    OSN_IMPORT virtual IAlfInterfaceBase* makeInterface( const IfId& aType );

    // from base class IAlfContainerWidget

    /**
     * Returns total number of widgets contained by this container.
     *
     * @return int Total number of widgets.
     */
    OSN_IMPORT int widgetCount() const;

    /**
     * Adds a child widget to the container.
     *
     * @param  aWidget Child widget to be added to container.
     * return void
     */
    OSN_IMPORT void addWidget(IAlfWidget& aWidget);

    /**
     * Returns child widget at given index.If index is not found return null.
     *
     * @param  aIndex Index of widget to be returned.
     * @return Widget at given index
     */
    OSN_IMPORT IAlfWidget* getWidget(int aIndex) const;
    
    /**
     * Returns the child index, when given the child widget.
     *
     * @param  aWidget child widget, which index is returned 
     * @return the index of the child widget, or -1, if widget is not child of this container.
     */
    OSN_IMPORT int getWidgetIndex(IAlfWidget& aWidget) const;

    /**
     * Removes the child widget from given index.
     *
     * @param aIndex Index from which widget has to be removed.
     * return void
     */
    void removeWidget(int aIndex);

    /**
     * Sets the base layout of container widget.
     *
     * @param aLayout New base layout of container widget.
     * return void
     */
    OSN_IMPORT void applyLayout(IAlfLayoutManager& aLayout);

    //from IAlfWidget

    /**
     * Sets/Releases the Focus from child widget of container.
     * Does not set the actual focused status in roster.
     *
     * @since S60 ?S60_version
     * @param aFocus boolean value for focused status
     * return void
     */
    OSN_IMPORT void setChildFocus(bool aFocus);
    
    /**
     * Removes the given widget from this widget's child widget
     * array. Does not modify control connections or control
     * group containment.
     *
     * Does nothing if the given widget is not found from
     * widget array.
     *
     * @param aWidget Reference to a widget that is searched
     *                from the widget array of this widget and removed
     *                of found.
     */    
    OSN_IMPORT void removeWidget(IAlfWidget& aWidget);

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
    OSN_IMPORT void setPresentation(const char* aFilePath);
    
protected:

    /**
     * The protected constructor.
     *
     * @since S60 ?S60_version
     */
    OSN_IMPORT AlfWidget();

private:

    /**
     * Adds the Common Properties for the widget
     *
     * @since S60 ?S60_version
     */
    void addCommonWidgetProperties();
    
    /**
     * Removes reference from this widget's control to this
     * widget.
     */
    void removeReferenceFromControl();

private: // data

    auto_ptr<AlfWidgetImpl> mImpl;
    };

using namespace osncore;

#include "alf/alfwidget.inl"

    } // namespace Alf

#endif // ALFWIDGET_H

// End of File
