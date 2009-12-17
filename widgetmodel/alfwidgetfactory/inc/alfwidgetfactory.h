/*
* Copyright (c) 2007-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Widget factory interface implementation
*
*/


#ifndef ALFWIDGETFACTORY_H
#define ALFWIDGETFACTORY_H

//OSN core includes
#include <osn/alfptrvector.h>

//Widget Utils Includes
#include <alf/alffactorypluginloader.h>
#include <alf/alftypes.h>

//Widget Factory Includes
#include <alf/ialfwidgetfactory.h>

//Forward declarations 
class CAlfEnv;
class CAlfDisplay;

//Namespaces
namespace duiuimodel
{
class DuiNode;        
}

using namespace duiuimodel;
using namespace osncore;
   
namespace Alf
    {
//Forward declarations  
class IAlfWidgetControl;


/**
 *  @class AlfWidgetFactory alfwidgetfactory.h "alf/alfwidgetfactory.h"
 *
 *  Widget factory implementation.
 *
 *  @lib alfwidgetfactory.lib
 *  @since S60 ?S60_version
 *  @status Draft
 *  @interfaces IAlfWidgetFactory.
 */
class AlfWidgetFactory: public IAlfWidgetFactory
    {
public: 
    /**
     * Destructor.
     */
    ~AlfWidgetFactory();

    /**
     * Explicit Parametrized Constructor
     */    
    explicit AlfWidgetFactory(CAlfEnv& aEnv);
       
public: 
    
    // From IAlfWidgetFactory

    /**
     * @see IAlfWidgetFactory.
     */
    IAlfViewWidget* createViewWidget(
        const char* aInstanceId, 
        int aControlGroupID, 
        DuiNode* aNode=0, 
        CAlfDisplay* aDisplay=0, 
        const char* aFilePath=0, 
        AlfCustomInitDataBase* aCustomData=0);

    /**
     * @see IAlfWidgetFactory.
     */
    IAlfWidget* createWidget(
        const char* aLoadId, 
        const char* aInstanceId, 
        IAlfContainerWidget& aContainerWidget, 
        DuiNode* aNode=0, 
        const char* aFilePath=0,
        AlfCustomInitDataBase* aCustomData=0);

    /**
     * @see IAlfWidgetFactory.
     */
    int destroyWidget(IAlfWidget* aWidget);
    
    /**
     * @see IAlfWidgetFactory.
     */
    IAlfWidget* findWidget(const char* aWidgetName) const;
    
    
    /**
     * @see IAlfWidgetFactory.
     */
    int appendWidget(IAlfWidget* aWidget);
        
    
    /**
     * @see IAlfWidgetFactory.
     */
    IAlfModel* createModel(
        const char* aLoadId, 
        AlfCustomInitDataBase* aCustomData=0);
    
    /**
     * @see IAlfWidgetFactory.
     */
    IAlfWidgetControl* createControl(
        const char* aLoadId, 
        const char* aInstanceId, 
        CAlfDisplay* aDisplay=0, 
        AlfCustomInitDataBase* aCustomData=0);
    
    /**
     * @see IAlfWidgetFactory.
     */
    IAlfElement* createElement(
        const char* aLoadId, 
        const char* aInstanceId, 
        IAlfWidgetControl& aControl,
        DuiNode* aNode=0, 
        AlfCustomInitDataBase* aCustomData=0);
   
    /**
     * @see IAlfWidgetFactory.
     */
    IAlfVisualTemplate* createVisualTemplate(
        const char* aLoadId, 
        const char* aInstanceId, 
        DuiNode* aNode=0, 
        AlfCustomInitDataBase* aCustomData=0);
    
    /**
     * @see IAlfWidgetFactory.
     */
    IAlfWidgetEventHandler* createEventHandler(
        const char* aLoadId, 
        const char* aInstanceId,
        DuiNode* aNode=0, 
        AlfCustomInitDataBase* aCustomData=0);
    
    /**
     * @see IAlfWidgetFactory.
     */
    IAlfLayoutManager* createLayoutManager(
        const char* aLoadId, 
        const char* aInstanceId, 
        DuiNode* aNode=0, 
        AlfCustomInitDataBase* aCustomData=0);

    /**
     * @see IAlfWidgetFactory.
     */
    void registerCustomWidgetFactory(IAlfFactoryPlugin* aFactory);

public: // From IAlfInterfaceBase

    /**
     * @see IAlfInterfaceBase.
     */
    IAlfInterfaceBase* makeInterface(const IfId& aType);
   
private:
    /**
     * Not owned.
     */
    CAlfEnv& mAlfEnv;
    /**
     * Not owned.
     */
    AlfPtrVector<IAlfFactoryPlugin> mRegisteredFactoryList;
    
    /**
     * Owned.
     */    
    AlfFactoryPluginLoader mFactoryPluginLoader;
    
    /**
     * List of the widgets added to the factory
     * 
     * Owned
     */
    AlfPtrVector<IAlfWidget> mWidgetList;
    };


    } // namespace Alf
#endif 

// End of File
