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
* Description:  Custom widget factories are registered into factory chain
*
*/


#ifndef I_ALFWIDGETFACTORY_H
#define I_ALFWIDGETFACTORY_H

//Widget Utils includes
#include <alf/alftypes.h>

//Widget Model Includes
#include <alf/ialfinterfacebase.h>

//Forward declaration
class CAlfDisplay;  

//Namespaces
namespace duiuimodel
    {
    class DuiNode;        
    }

using namespace duiuimodel;
    
namespace Alf
    {

//Forward declarations
class IAlfWidget;
class IAlfModel;
class IAlfWidgetControl;
class IAlfElement;
class IAlfVisualTemplate;
class IAlfWidgetEventHandler;
class IAlfFactoryPlugin;
class IAlfViewWidget;
class IAlfContainerWidget;
class IAlfLayoutManager;

//Namespace
namespace category
    {
    static const char* const KWidget = "widget";
    static const char* const KModel = "model";
    static const char* const KControl ="control";
    static const char* const KElement ="element";
    static const char* const KVisualTemplate ="visualtemplate";
    static const char* const KEventHandler = "eventhandler";        
    }    

namespace alfwidgetfactory
    {
     static const IfId ident=
        {
        0,"alfwidgetfactory"
        };
    
    }

/**
 *  @class AlfCustomInitDataBase ialfwidgetfactory.h "alf/ialfwidgetfactory.h"
 *
 *  Empty placeholder for custom data to be plumbed through the factory while 
 *  creating widgets or widget parts.
 *  Derive from this class to pass custom data via the factory.
 *
 *  @lib alfwidgetfactory.lib
 *  @since S60 ?S60_version
 *  @status Draft
 */
class AlfCustomInitDataBase
    {
public:

   /**
    * Virtual destructor.
    */
    virtual ~AlfCustomInitDataBase() {}
    };


/**
 *  @class IAlfWidgetFactory ialfwidgetfactory.h "alf/ialfwidgetfactory.h"
 *
 *  Interface to create widgets, register custom widget factories and query 
 *  factory products.
 *
 *  Widget factory provides methods to create widgets, event handlers, 
 *  controls, visual templates and elements through a factory chain.
 *  @code
 *  MAlfListWidget* list = widgetFactory.CreateWidget<MAlfListWidget>(
 *                                                     "alflistwidget");
 *  if( list )
 *      {
 *      // do operations with widget
 *      }
 *  @endcode
 *
 *  @lib alfwidgetfactory.lib
 *  @since S60 ?S60_version
 *  @status Draft
 */
class IAlfWidgetFactory: public IAlfInterfaceBase
    {
public:
    static inline const IfId& type()
        {
        return alfwidgetfactory::ident;    
        }

    /**
     * Destructor
     */
    virtual ~IAlfWidgetFactory() {}

    // Templates
    
    /**
     * Constructs a new widget.
     *
     * Each widget will have to be added into a container widget. The root 
     * container widget of an application user interface view can be created 
     * with the createViewWidget() method. The widget hierarchy can be 
     * constructed from the root container widget by using container and leaf 
     * widgets. When a new widget is created a container has to be specified 
     * that is going to contain the created widget. The container can be a 
     * root container widget or some of the inner container widgets in the 
     * widget containment hierarchy.
     *
     * Widget's control is bound to a display object when this widget is first 
     * shown. The widget is bound to the same display that was given in the 
     * constructor of its containing view.
     *
     * @see createViewWidget()
     * @see Alf::IAlfContainerWidget
     *
     * @exception osncore::AlfWidgetException Thrown with error code 
     *            osncore::EInvalidArgument if both node and declaration file 
     *            path have non-null value at the same time.
     * @exception osncore::AlfWidgetException Thrown with error code 
     *            osncore::EInvalidWidgetInstanceId if an attempt is made to 
     *            create a widget with the Instance ID of an existing widget.     
     *
     * @param aLoadId           Widget type identifier.
     * @param aInstanceId       Id or name of the widget to be created.
     * @param aContainerWidget  Reference to a container widget that will 
     *                          contain the created widget.
     * @param aNode             Declarative UI node which is used to construct
     *                          and configure this widget from a declaration. 
     *                          Default implementation if aNode is NULL.
     * @param aFilePath         File handle to an XML file describing the 
     *                          presentation of the widget. Both aNode and 
     *                          aFilePath cannot be non-null values at the 
     *                          same time.                             
     * @param aCustomData       Custom data to be passed to the widget during 
     *                          construction. 
     *
     * @return Pointer to the created widget or 0. The created widget is 
     *         maintained in the UI Accelerator Toolkit environment.
     */
    template<class T>
    T* createWidget(
        const char* aLoadId, const char* aInstanceId, 
        IAlfContainerWidget& aContainerWidget,
        DuiNode* aNode=0, const char* aFilePath=0, 
        AlfCustomInitDataBase* aCustomData=0)
        {
        return static_cast<T*>(
        createWidget(aLoadId,aInstanceId,aContainerWidget,
                     aNode,aFilePath,aCustomData));
        }

    /**
     * Destroy widget
     *
     * @param aWidget A widget to be destroyed.
     * @return Error code, if widget was not found, 0 otherwise.
     */
    template<class T>
    int destroyWidget(T* aWidget)
        {
        return destroyWidget(static_cast<IAlfWidget*>(aWidget));
        }
    
    /**
     * Create a widget model
     *
     * @param aLoadId Implementation to load
     * @param aCustomData  Custom data to be passed to the model during 
     *                     construction. 
     * @return Pointer to model or 0. 
     */
    template<class T>
    T* createModel(const char* aLoadId, AlfCustomInitDataBase* aCustomData=0)
        {
        return static_cast<T*>(createModel(aLoadId,aCustomData));      
        }
        
    /**
     * Create a control
     *
     * @param aLoadId Load identifier
     * @param aInstanceId Instance identifier
     * @param aDisplay Display where control is bound
     * @param aCustomData Custom data to be passed to the control during 
     *                    construction. 
     * @return Pointer to control or 0.
     */
    template<class T> T* createControl(
        const char* aLoadId, const char* aInstanceId, 
        CAlfDisplay* aDisplay=0, AlfCustomInitDataBase* aCustomData=0)
        {
        return static_cast<T*>(
        createControl(aLoadId,aInstanceId,aDisplay,aCustomData));
        } 
           
    /**
     * Create an element
     *
     * @param aLoadId Load identifier
     * @param aInstanceId Instance identifier
     * @param aControl Control Use by element contructor
     * @param aCustomData  Custom data to be passed to the element during 
     *                     construction. 
     * @return Pointer to element or 0.
     */
    template<class T> T* createElement(
        const char* aLoadId, const char* aInstanceId, 
        IAlfWidgetControl& aControl, DuiNode* aNode=0, 
        AlfCustomInitDataBase* aCustomData=0)
        {
        return static_cast<T*>(
        createElement(aLoadId,aInstanceId,aControl,aNode,aCustomData));
        }     

    /**
     * Create a visual template
     *
     * @param aLoadId Load identifier
     * @param aInstanceId Instance identifier
     * @param aCustomData  Custom data to be passed to the visual template 
     *                     during construction. 
     * @return Pointer to visual template or 0.
     */
    template<class T> T* createVisualTemplate(
        const char* aLoadId, const char* aInstanceId, 
        DuiNode* aNode=0, AlfCustomInitDataBase* aCustomData=0)
        {
        return static_cast<T*>(
        createVisualTemplate(aLoadId,aInstanceId,aNode,aCustomData));
        }
        
    /**
     * Create an event handler
     *
     * @param aLoadId Load identifier
     * @param aInstanceId Instance identifier
     * @param aCustomData  Custom data to be passed to the event handler 
     *                     during construction. 
     * @return Pointer to event handler or 0.
     */
    template<class T> T* createEventHandler(
        const char* aLoadId,
        const char* aInstanceId,
        DuiNode* aNode=0, 
        AlfCustomInitDataBase* aCustomData=0)
        {
        return static_cast<T*>(
        createEventHandler(aLoadId,aInstanceId,aNode,aCustomData));
        }      

    /**
     * Create an layoutmanager.
     *
     * @param aLoadId Load identifier
     * @param aInstanceId Instance identifier
     * @param aNode declaration node.
     * @param aCustomData Custom data to be passed to the layout manager 
     *                    during construction. 
     * @return Pointer to instance or NULL
     */
    template<class T> T* createLayoutManager(
        const char* aLoadId, 
        const char* aInstanceId, 
        DuiNode* aNode=0, 
        AlfCustomInitDataBase* aCustomData=0)
        {
        return static_cast<T*>(
        createLayoutManager(aLoadId,aInstanceId,aNode,aCustomData));
        }
        
public:

    /**
     * Constructs a new view widget.
     *
     * Application user interface contains always at least one view widget.
     *
     * View widget is a container widget that can be used to contain other 
     * container or leaf widgets. View widget is the root of the application 
     * user interface view into which the widget hierarchy can be built.
     *
     * View widget control is appended in a control group. Each view will be 
     * associated with a unique control group. A unique, unreserved control 
     * group ID should be passed through this construction method. 
     * Construction method will then create a new control group for the given 
     * ID and append the view widget's control into the control group. When 
     * the view widget is destroyed also its control group is destroyed and 
     * the control group ID is free to be used again. If an already reserved 
     * control group ID is passed an osncore::AlfException is thrown with 
     * error code osncore::EInvalidArgument.
     *
     * If the application contains multiple views it is possible to apply 
     * transitions from a view widget to another by manipulating the 
     * associated control groups.
     *
     * View widget is bound to a single display. If no display is given as a 
     * construction parameter then primary display of the UI Accelerator 
     * Toolkit environment is used. If no display is given and there is no 
     * primary display in the environment an osncore::AlfException is thrown 
     * with error code osncore::EDisplayNotFound.
     *
     * @see Alf::IAlfViewWidget
     * @see Alf::IAlfContainerWidget
     * @see CAlfControlGroup
     *
     * @exception osncore::AlfException Thrown with error code 
     *            osncore::EInvalidArgument if a control group with the 
     *            provided id already exists and with error code 
     *            osncore::EDisplayNotFound if no display object is provided 
     *            and there is no primary display.
     *
     * @exception osncore::AlfWidgetException Thrown with error code 
     *            osncore::EInvalidArgument if both node and declaration 
     *            file path have non-null value at the same time.
     * @exception osncore::AlfWidgetException Thrown with error code 
     *            osncore::EInvalidWidgetInstanceId if an attempt is made to 
     *            create a view widget with the Instance ID of an existing widget.
     *
     * @param aInstanceId      Id or name of the view widget to be created.
     * @param aControlGroupId  Id of the control group to be created. This has
     *                         to be unique and unreserved control group id.
     * @param aNode            Declarative UI node which is used to construct 
     *                         and configure this widget from a declaration. 
     *                         Default implementation if aNode is NULL.
     * @param aDisplay         Display in which the view widget is maintained.
     *                         Primary display is used if no display is 
     *                         provided.
     * @param aFilePath        File handle to an XML file describing the 
     *                         presentation of the view widget. Both aNode and 
     *                         aFilePath cannot be non-null values at the same 
     *                         time. 
     * @param aCustomData      Custom data to be passed to the widget during 
     *                         construction. 
     *
     * @return Pointer to a new view widget or 0. The created widget is 
     *         maintained in the UI Accelerator Toolkit environment.
     */
    virtual IAlfViewWidget* createViewWidget(
        const char* aInstanceId, 
        int aControlGroupId, 
        DuiNode* aNode=0, 
        CAlfDisplay* aDisplay=0, 
        const char* aFilePath=0,
        AlfCustomInitDataBase* aCustomData=0) = 0;
    
    /**
     * Constructs a new widget.
     *
     * Each widget will have to be added into a container widget. The root 
     * container widget of an application user interface view can be created 
     * with the createViewWidget() method. The widget hierarchy can be 
     * constructed from the root container widget by using container and leaf 
     * widgets. When a new widget is created a container has to be specified 
     * that is going to contain the created widget. The container can be a 
     * root container widget or some of the inner container widgets in the 
     * widget containment hierarchy.
     *
     * Widget's control is bound to a display object when this widget is first 
     * shown. The widget is bound to the same display that was given in the 
     * constructor of its containing view.
     *
     * @see createViewWidget()
     * @see Alf::IAlfContainerWidget
     *
     * @exception osncore::AlfWidgetException Thrown with error code 
     *            osncore::EInvalidArgument if both node and declaration file 
     *            path have non-null value at the same time.
     * @exception osncore::AlfWidgetException Thrown with error code 
     *            osncore::EInvalidWidgetInstanceId if an attempt is made to 
     *            create a widget with the Instance ID of an existing widget.     
     *
     * @param aLoadId           Widget type identifier.
     * @param aInstanceId       Id or name of the widget to be created.
     * @param aContainerWidget  Reference to a container widget that will 
     *                          contain the created widget.
     * @param aNode             Declarative UI node which is used to construct 
     *                          and configure this widget from a declaration. 
     *                          Default implementation if aNode is NULL.
     * @param aFilePath         File handle to an XML file describing the 
     *                          presentation of the widget. Both aNode and 
     *                          aFilePath cannot be non-null
     *                          values at the same time.                             
     * @param aCustomData       Custom data to be passed to the widget during 
     *                          construction. 
     *
     * @return Pointer to the created widget or 0. The created widget is 
     *         maintained in the UI Accelerator Toolkit environment.
     */
    virtual IAlfWidget* createWidget(
        const char* aLoadId,
        const char* aInstanceId,
        IAlfContainerWidget& aContainerWidget,
        DuiNode* aNode=0, 
        const char* aFilePath=0, 
        AlfCustomInitDataBase* aCustomData=0) = 0;

    /**
     * Destroy widget
     *
     * @param aWidget A widget to be destroyed.
     * @return -1, if widget was not found, 0 otherwise.
     */
    virtual int destroyWidget(IAlfWidget* aWidget) = 0;
    
    /**
     * Find named widgets owned by the factory
     *
     * @param aWidgetName A widget name to be found.
     *
     * @return The widget pointer if it is found, 0 otherwise.
     */
    virtual IAlfWidget* findWidget(const char* aWidgetName)const = 0;
    
    
    /**
     * Append widgets into the widget factory
     *
     * @param aWidget A widget to be added 
     *
     * @return -1, if widget was already added to the factory, 0 otherwise.
     */
    virtual int appendWidget(IAlfWidget* aWidget) = 0;
    
    /**
     * Create a widget model
     *
     * @param aLoadId Implementation to load
     * @param aCustomData  Custom data to be passed to the model during 
     *                     construction. 
     * @return Pointer to widget or 0. Ownership is transferred to caller.
     */
    virtual IAlfModel* createModel(
        const char* aLoadId, 
        AlfCustomInitDataBase* aCustomData=0) = 0;
    
    /**
     * Create a control
     *
     * @param aLoadId Load identifier
     * @param aInstanceId Instance identifier
     * @param aDisplay Display where control is bound
     * @param aCustomData  Custom data to be passed to the control during 
     *                     construction. 
     * @return Pointer to created control or 0.
     */
    virtual IAlfWidgetControl* createControl(
        const char* aLoadId,
        const char* aInstanceId,
        CAlfDisplay* aDisplay=0, 
        AlfCustomInitDataBase* aCustomData=0) = 0;
   
    /**
     * Create an element
     *
     * @param aLoadId Load identifier
     * @param aInstanceId Instance identifier
     * @param aControl Control Use by element contructor
     * @param aCustomData  Custom data to be passed to the element during 
     *                     construction. 
     * @return Pointer to created element or 0.
     */
    virtual IAlfElement* createElement(
        const char* aLoadId, 
        const char* aInstanceId, 
        IAlfWidgetControl& aControl,
        DuiNode* aNode=0, 
        AlfCustomInitDataBase* aCustomData=0) = 0;
    
    /**
     * Create a visual template
     *
     * @param aLoadId Load identifier
     * @param aInstanceId Instance identifier
     * @param aCustomData  Custom data to be passed to the visual template 
     *                     during construction. 
     * @return Pointer to created visual template or 0.
     */
    virtual IAlfVisualTemplate* createVisualTemplate(
        const char* aLoadId, 
        const char* aInstanceId,
        DuiNode* aNode=0, 
        AlfCustomInitDataBase* aCustomData=0) = 0;
    
    /**
     * Create an event handler
     *
     * @param aLoadId Load identifier
     * @param aInstanceId Instance identifier
     * @param aCustomData  Custom data to be passed to the event handler 
     *                     during construction. 
     * @return Pointer to created event handler or 0.
     */
    virtual IAlfWidgetEventHandler* createEventHandler(
        const char* aLoadId, 
        const char* aInstanceId,
        DuiNode* aNode=0, 
        AlfCustomInitDataBase* aCustomData=0) = 0;
    
    /**
     * Create an layoutmanager.
     *
     * @param aLoadId Load identifier
     * @param aInstanceId Instance identifier
     * @param aNode declaration node.
     * @param aCustomData Possible custom initialization data for the 
     *                    layoutmanager.
     * @return Pointer to created layout manager or 0.
     */
    virtual IAlfLayoutManager* createLayoutManager(
        const char* aLoadId, 
        const char* aInstanceId, 
        DuiNode* aNode=0, 
        AlfCustomInitDataBase* aCustomData=0) = 0;

    /**
     * Application can register a custom factory to create custom factory 
     * products.
     * Ownership is transferred to a widget factory chain, 
     * if call is successfull. 
     *
     * @exception std::bad_alloc
     * @param aFactory Custom factory
     * @return void
     */
    virtual void registerCustomWidgetFactory(IAlfFactoryPlugin* aFactory) = 0;
    };

    } // namespace Alf
    
#endif 

// End of File
