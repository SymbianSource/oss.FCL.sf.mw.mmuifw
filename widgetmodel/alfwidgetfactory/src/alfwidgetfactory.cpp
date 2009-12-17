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
* Description:  Widget factory implementation file
*
*/


//STL includes
#include <string.h>

//ALF Client includes
#include <alf/alfenv.h>
#include <alf/alfdisplay.h>

//OSN core includes
#include <osn/ustring.h>
#include <osn/alfptrvector.h>

//Widget Utils includes
#include <alf/alfexceptions.h>

//Widget factory includes
#include <alf/ialffactoryplugin.h>

//Widget Model includes
#include <alf/ialfwidget.h>
#include <alf/ialfmodel.h>
#include <alf/ialfwidgetcontrol.h>
#include <alf/ialfelement.h>
#include "alf/ialfvisualtemplate.h"
#include <alf/ialfwidgeteventhandler.h>
#include <alf/ialflayoutmanager.h>

//View Widget includes
#include <alf/ialfviewwidget.h>

//Local Includes
#include "alfwidgetfactory.h"

using namespace osncore;

namespace Alf
    {

static const char* const KViewWidgetProductId = "viewwidget";
        
    
static IAlfViewWidget* createDefaultViewWidget(
    const char* aInstanceId,
    int aControlGroupID,
    DuiNode* aNode,
    CAlfDisplay& aDisplay,
    CAlfEnv& aEnv,
    IAlfFactoryPlugin& aFactoryPlugin,
    const char* aFilePath,
    AlfCustomInitDataBase* aCustomData)
    {
    IAlfViewWidget* ret = 0;
    
    // Create construction parameter structure.
    AlfViewWidgetInitData init;
    
    // Zero out the data in the construction structure.
    memset(&init, 0, sizeof(AlfViewWidgetInitData));
    
    // Set the construction parameters.
    init.mEnv = &aEnv;
    init.mDisplay = &aDisplay;    
    init.mWidgetId = aInstanceId;
    init.mControlGroupId = aControlGroupID;
    init.mNode = aNode;
    init.mFilePath = aFilePath;
    init.mCustomData = aCustomData;
    
    // Create a temporary factory product and typecast it to the correct 
    // interface.
    IAlfInterfaceBase* tmp = aFactoryPlugin.createProduct(
        KViewWidgetProductId, &init);
    if(tmp)
        {
        // Object created. Attempt to typecast to correct interface.        	
        ret = IAlfInterfaceBase::makeInterface<IAlfViewWidget>(tmp);
        if(!ret)
            {
            // Interface not found. Delete the temporary product.	
            delete tmp;            	
            }	
        }

    return ret;
    }

static IAlfWidget* createDefaultWidget(
    const char*     aLoadId,
    const char*     aInstanceId,
    IAlfContainerWidget& aContainerWidget,
    DuiNode*       aNode,
    CAlfEnv&        aEnv,
    IAlfFactoryPlugin&      aFactoryPlugin,
    const char* aFilePath,
    AlfCustomInitDataBase* aCustomData)
    {
    IAlfWidget* ret = 0;
    
    // Create construction parameter structure.
    AlfWidgetInitData init;

    // Zero out the data in the construction structure.
    memset(&init, 0, sizeof(AlfWidgetInitData));
    
    // Set the construction parameters.    
    init.mEnv = &aEnv;
    init.mContainerWidget = &aContainerWidget;
    init.mWidgetId = const_cast<char*>(aInstanceId);
    init.mNode = aNode;  
    init.mFilePath = aFilePath;  
    init.mCustomData = aCustomData;

    // Create a temporary factory product and typecast it to the correct 
    // interface.
    IAlfInterfaceBase* tmp = aFactoryPlugin.createProduct(aLoadId,&init);
    if(tmp)
        {
        // Object created. Attempt to typecast to correct interface.        	
        ret = IAlfInterfaceBase::makeInterface<IAlfWidget>(tmp);
        if(!ret)
            {
            // Interface not found. Delete the temporary product.	
            delete tmp;            	
            }	
        }

    return ret;       
    }

static IAlfModel* createDefaultModel(
    const char* aLoadId,
    IAlfFactoryPlugin& aFactoryPlugin,
    AlfCustomInitDataBase* aCustomData)
    {
    IAlfModel* ret = 0;

    // Create construction parameter structure.
    AlfModelInitData init;

    // Zero out the data in the construction structure.
    memset(&init, 0, sizeof(AlfModelInitData));

    // Set the construction parameters.        
    init.mCustomData = aCustomData;

    // Create a temporary factory product and typecast it to the correct 
    // interface.
    IAlfInterfaceBase* tmp = aFactoryPlugin.createProduct(aLoadId,&init);
    if(tmp)
        {
        // Object created. Attempt to typecast to correct interface.
        ret = IAlfInterfaceBase::makeInterface<IAlfModel>(tmp);
        if(!ret)
            {
            // Interface not found. Delete the temporary product.	
            delete tmp;            	
            }	
        }
   
    return ret;           
    }

static IAlfWidgetControl* createDefaultControl(
    const char* aLoadId,
    CAlfDisplay& aDisplay,
    IAlfFactoryPlugin& aFactoryPlugin,
    AlfCustomInitDataBase* aCustomData)
    {
    IAlfWidgetControl* ret(0);

    // Create construction parameter structure.
    AlfWidgetControlInitData init;

    // Zero out the data in the construction structure.
    memset(&init, 0, sizeof(AlfWidgetControlInitData));

    // Set the construction parameters.        
    init.mDisplay = &aDisplay;    
    init.mCustomData = aCustomData;
 
    // Create a temporary factory product and typecast it to the correct 
    // interface.
    IAlfInterfaceBase* tmp = aFactoryPlugin.createProduct(aLoadId,&init);
    if(tmp)
        {
        // Object created. Attempt to typecast to correct interface.
        ret = IAlfInterfaceBase::makeInterface<IAlfWidgetControl>(tmp);
        if(!ret)
            {
            // Interface not found. Delete the temporary product.	
            delete tmp;            	
            }	
        }
   
    return ret;       
    }

static IAlfElement* createDefaultElement(
    const char* aLoadId, 
    const char* aInstanceId, 
    DuiNode* aNode,
    IAlfWidgetControl& aControl,
    IAlfFactoryPlugin& aFactoryPlugin,
    AlfCustomInitDataBase* aCustomData)
    {
    IAlfElement* ret(0);
    
    // Create construction parameter structure.
    AlfElementInitData init;

    // Zero out the data in the construction structure.
    memset(&init, 0, sizeof(AlfElementInitData));

    // Set the construction parameters.        
    init.mControl= &aControl;
    init.mElementId = const_cast<char*>(aInstanceId);
    init.mNode= aNode;
    init.mCustomData = aCustomData; 
 
    // Create a temporary factory product and typecast it to the correct 
    // interface.
    IAlfInterfaceBase* tmp = aFactoryPlugin.createProduct(aLoadId,&init);
    if(tmp)
        {
        // Object created. Attempt to typecast to correct interface.
        ret = IAlfInterfaceBase::makeInterface<IAlfElement>(tmp);
        if(!ret)
            {
            // Interface not found. Delete the temporary product.	
            delete tmp;            	
            }	
        }

    return ret;       
    }   

static IAlfVisualTemplate* createDefaultVisualTemplate(
    const char* aLoadId, 
    const char* aInstanceId,
    DuiNode* aNode,
    IAlfFactoryPlugin& aFactoryPlugin,
    AlfCustomInitDataBase* aCustomData)
    {
    IAlfVisualTemplate* ret(0);
        
    // Create construction parameter structure.
    AlfVisualTemplateInitData init;

    // Zero out the data in the construction structure.
    memset(&init, 0, sizeof(AlfVisualTemplateInitData));

    // Set the construction parameters.        
    init.mVisualTemplateId = const_cast<char*>(aInstanceId);
    init.mNode= aNode;
    init.mCustomData = aCustomData;

    // Create a temporary factory product and typecast it to the correct 
    // interface.
    IAlfInterfaceBase* tmp = aFactoryPlugin.createProduct(aLoadId,&init);
    if(tmp)
        {
        // Object created. Attempt to typecast to correct interface.
        ret = IAlfInterfaceBase::makeInterface<IAlfVisualTemplate>(tmp);
        if(!ret)
            {
            // Interface not found. Delete the temporary product.	
            delete tmp;            	
            }	
        }
        
    return ret;       
    }
       
static IAlfWidgetEventHandler* createDefaultEventHandler(
    const char* aLoadId,
    const char* aInstanceId,
    DuiNode* aNode,
    IAlfFactoryPlugin& aFactoryPlugin,
    AlfCustomInitDataBase* aCustomData)
    {
    IAlfWidgetEventHandler* ret(0);
        
    // Create construction parameter structure.
    AlfWidgetEventHandlerInitData init;

    // Zero out the data in the construction structure.
    memset(&init, 0, sizeof(AlfWidgetEventHandlerInitData));

    // Set the construction parameters.        
    init.mWidgetEventHandlerId = const_cast<char*>(aInstanceId);
    init.mNode= aNode;
    init.mCustomData = aCustomData;
    
    // Create a temporary factory product and typecast it to the correct 
    // interface.
    IAlfInterfaceBase* tmp = aFactoryPlugin.createProduct(aLoadId,&init);
    if(tmp)
        {
        // Object created. Attempt to typecast to correct interface.
        ret = IAlfInterfaceBase::makeInterface<IAlfWidgetEventHandler>(tmp);
        if(!ret)
            {
            // Interface not found. Delete the temporary product.	
            delete tmp;            	
            }	
        }

    return ret;       
    }
    
static IAlfLayoutManager* createDefaultLayoutManager(
    const char* aLoadId,
    const char* aInstanceId,
    DuiNode* aNode,
    IAlfFactoryPlugin& aFactoryPlugin,
    AlfCustomInitDataBase* aCustomData)
    {
    IAlfLayoutManager* ret(0);

    // Create construction parameter structure.
    AlfLayoutManagerInitData init;

    // Zero out the data in the construction structure.
    memset(&init, 0, sizeof(AlfLayoutManagerInitData));

    // Set the construction parameters.        
    init.mLayoutManagerId = const_cast<char*>(aInstanceId);
    init.mNode= aNode;
    init.mCustomData = aCustomData;

    // Create a temporary factory product and typecast it to the correct 
    // interface.
    IAlfInterfaceBase* tmp = aFactoryPlugin.createProduct(aLoadId, &init);
    if(tmp)
        {
        // Object created. Attempt to typecast to correct interface.
        ret = IAlfInterfaceBase::makeInterface<IAlfLayoutManager>(tmp);
        if(!ret)
            {
            // Interface not found. Delete the temporary product.	
            delete tmp;            	
            }	
        }

    return ret;       
    }

static IAlfViewWidget* createRegisteredViewWidget(
    const char* aInstanceId,
    int aControlGroupID,
    DuiNode* aNode,
    CAlfEnv& aEnv,
    CAlfDisplay& aDisplay,
    AlfPtrVector<IAlfFactoryPlugin>& aFactoryList,
    const char* aFilePath,    
    AlfCustomInitDataBase* aCustomData)
    {
    IAlfViewWidget* ret(0);
    IAlfInterfaceBase* tmp(0);

    // Create construction parameter structure.    
    AlfViewWidgetInitData init;

    // Zero out the data in the construction structure.
    memset(&init, 0, sizeof(AlfViewWidgetInitData));

    // Set the construction parameters.        
    init.mEnv = &aEnv;
    init.mDisplay = &aDisplay;    
    init.mWidgetId = aInstanceId;
    init.mControlGroupId = aControlGroupID;
    init.mNode = aNode;
    init.mFilePath = aFilePath;    
    init.mCustomData = aCustomData;
    
    // Run through registered factories.     
    for(int i=0;i<aFactoryList.count()&&!ret;i++)
        {
        // Create a temporary factory product and typecast it to the correct 
        // interface.
        tmp = aFactoryList.at(i)->createProduct(KViewWidgetProductId,&init);   
        if(tmp)
            {
            // Object created. Attempt to typecast to correct interface.
            ret = IAlfInterfaceBase::makeInterface<IAlfViewWidget>(tmp);
            if(!ret)
                {
                // Interface not found. Delete the temporary product.	
                delete tmp;    
                tmp=0;        	
                }	
            }
        }
        
    return ret;     
    }
        
static IAlfWidget* createRegisteredWidget(
    const char* aLoadId,
    const char* aInstanceId,
    IAlfContainerWidget& aContainerWidget,
    DuiNode*   aNode,
    CAlfEnv&    aEnv,
    AlfPtrVector<IAlfFactoryPlugin>& aFactoryList,
    const char* aFilePath,
    AlfCustomInitDataBase* aCustomData)
    {
    IAlfWidget* ret(0);
    IAlfInterfaceBase* tmp(0);

    // Create construction parameter structure.        
    AlfWidgetInitData init;

    // Zero out the data in the construction structure.
    memset(&init, 0, sizeof(AlfWidgetInitData));

    // Set the construction parameters.        
    init.mEnv = &aEnv;
    init.mContainerWidget = &aContainerWidget;
    init.mWidgetId = const_cast<char*>(aInstanceId);
    init.mNode = aNode;    
    init.mFilePath= aFilePath;
    init.mCustomData = aCustomData;
        
    // Run through registered factories.     
    for(int i=0;i<aFactoryList.count()&&!ret;i++)
        {
        // Create a temporary factory product and typecast it to the correct 
        // interface.
        tmp = aFactoryList.at(i)->createProduct(aLoadId,&init);   
        if(tmp)
            {
            // Object created. Attempt to typecast to correct interface.
            ret = IAlfInterfaceBase::makeInterface<IAlfWidget>(tmp);
            if(!ret)
                {
                // Interface not found. Delete the temporary product.	
                delete tmp;    
                tmp=0;        	
                }	
            }
        }

    return ret;       
    }

static IAlfModel* createRegisteredModel(
    const char* aLoadId,
    AlfPtrVector<IAlfFactoryPlugin>& aFactoryList,
    AlfCustomInitDataBase* aCustomData)
    {
    IAlfModel* ret(0);
    IAlfInterfaceBase* tmp(0);

    // Create construction parameter structure.
    AlfModelInitData init;

    // Zero out the data in the construction structure.
    memset(&init, 0, sizeof(AlfModelInitData));

    // Set the construction parameters.        
    init.mCustomData = aCustomData;
        
    // Run through registered factories.     
    for(int i=0;i<aFactoryList.count()&&!ret;i++)
        {
        // Create a temporary factory product and typecast it to the correct 
        // interface.
        tmp = aFactoryList.at(i)->createProduct(aLoadId,&init);   
        if(tmp)
            {
            // Object created. Attempt to typecast to correct interface.
            ret = IAlfInterfaceBase::makeInterface<IAlfModel>(tmp);
            if(!ret)
                {
                // Interface not found. Delete the temporary product.	
                delete tmp;    
                tmp=0;        	
                }	
            }
        }

    return ret;     
    }
   
static IAlfWidgetControl* createRegisteredControl(
    const char* aLoadId,
    CAlfDisplay& aDisplay,
    AlfPtrVector<IAlfFactoryPlugin>& aFactoryList,
    AlfCustomInitDataBase* aCustomData)
    {
    IAlfWidgetControl* ret(0);
    IAlfInterfaceBase* tmp(0);


    // Create construction parameter structure.
    AlfWidgetControlInitData init;

    // Zero out the data in the construction structure.
    memset(&init, 0, sizeof(AlfWidgetControlInitData));

    // Set the construction parameters.        
    init.mDisplay = &aDisplay;    
    init.mCustomData = aCustomData;
 
    // Run through registered factories.     
    for(int i=0;i<aFactoryList.count()&&!ret;i++)
        {
        // Create a temporary factory product and typecast it to the correct 
        // interface.
        tmp = aFactoryList.at(i)->createProduct(aLoadId,&init);   
        if(tmp)
            {
            // Object created. Attempt to typecast to correct interface.
            ret = IAlfInterfaceBase::makeInterface<IAlfWidgetControl>(tmp);
            if(!ret)
                {
                // Interface not found. Delete the temporary product.	
                delete tmp;    
                tmp=0;        	
                }	
            }
        }
    
    return ret;       
    }
    
static IAlfElement* createRegisteredElement(
    const char* aLoadId, 
    const char* aInstanceId,
    DuiNode*   aNode, 
    IAlfWidgetControl& aControl,
    AlfPtrVector<IAlfFactoryPlugin>& aFactoryList,
    AlfCustomInitDataBase* aCustomData)
    {
    IAlfElement* ret(0);
    IAlfInterfaceBase* tmp(0);
    
    // Create construction parameter structure.
    AlfElementInitData init;

    // Zero out the data in the construction structure.
    memset(&init, 0, sizeof(AlfElementInitData));

    // Set the construction parameters.        
    init.mControl= &aControl;
    init.mElementId = const_cast<char*>(aInstanceId);
    init.mNode= aNode;
    init.mCustomData = aCustomData; 
 
    // Run through registered factories.     
    for(int i=0;i<aFactoryList.count()&&!ret;i++)
        {
        // Create a temporary factory product and typecast it to the correct 
        // interface.
        tmp = aFactoryList.at(i)->createProduct(aLoadId,&init);   
        if(tmp)
            {
            // Object created. Attempt to typecast to correct interface.
            ret = IAlfInterfaceBase::makeInterface<IAlfElement>(tmp);
            if(!ret)
                {
                // Interface not found. Delete the temporary product.	
                delete tmp;    
                tmp=0;        	
                }	
            }
        }

    return ret;       
    }
    
static IAlfVisualTemplate* createRegisteredVisualTemplate(
    const char* aLoadId, 
    const char* aInstanceId,
    DuiNode* aNode,
    AlfPtrVector<IAlfFactoryPlugin>& aFactoryList,
    AlfCustomInitDataBase* aCustomData)
    {
    IAlfVisualTemplate* ret(0);
    IAlfInterfaceBase* tmp(0);

    // Create construction parameter structure.
    AlfVisualTemplateInitData init;

    // Zero out the data in the construction structure.
    memset(&init, 0, sizeof(AlfVisualTemplateInitData));

    // Set the construction parameters.        
    init.mVisualTemplateId = const_cast<char*>(aInstanceId);
    init.mNode= aNode;
    init.mCustomData = aCustomData;
 
    // Run through registered factories.     
    for(int i=0;i<aFactoryList.count()&&!ret;i++)
        {
        // Create a temporary factory product and typecast it to the correct 
        // interface.
        tmp = aFactoryList.at(i)->createProduct(aLoadId,&init);   
        if(tmp)
            {
            // Object created. Attempt to typecast to correct interface.
            ret = IAlfInterfaceBase::makeInterface<IAlfVisualTemplate>(tmp);
            if(!ret)
                {
                // Interface not found. Delete the temporary product.	
                delete tmp;    
                tmp=0;        	
                }	
            }
        }

    return ret;  
    }

static IAlfWidgetEventHandler* createRegisteredEventHandler(
    const char* aLoadId,
    const char* aInstanceId,
    DuiNode*   aNode,
    AlfPtrVector<IAlfFactoryPlugin>& aFactoryList,
    AlfCustomInitDataBase* aCustomData)
    {
    IAlfWidgetEventHandler* ret(0);
    IAlfInterfaceBase* tmp(0);

    // Create construction parameter structure.
    AlfWidgetEventHandlerInitData init;

    // Zero out the data in the construction structure.
    memset(&init, 0, sizeof(AlfWidgetEventHandlerInitData));

    // Set the construction parameters.        
    init.mWidgetEventHandlerId = const_cast<char*>(aInstanceId);
    init.mNode= aNode;
    init.mCustomData = aCustomData;

    // Run through registered factories.     
    for(int i=0;i<aFactoryList.count()&&!ret;i++)
        {
        // Create a temporary factory product and typecast it to the correct 
        // interface.
        tmp = aFactoryList.at(i)->createProduct(aLoadId,&init);   
        if(tmp)
            {
            // Object created. Attempt to typecast to correct interface.
            ret = IAlfInterfaceBase::makeInterface<IAlfWidgetEventHandler>(
                                                                      tmp);
            if(!ret)
                {
                // Interface not found. Delete the temporary product.	
                delete tmp;    
                tmp=0;        	
                }	
            }
        }

    return ret;       
    }
    
static IAlfLayoutManager* createRegisteredLayoutManager(
    const char* aLoadId,
    const char* aInstanceId,    
    DuiNode* aNode,
    AlfPtrVector<IAlfFactoryPlugin>& aFactoryList,
    AlfCustomInitDataBase* aCustomData)
    {
    IAlfLayoutManager* ret(0);
    IAlfInterfaceBase* tmp(0);

    // Create construction parameter structure.
    AlfLayoutManagerInitData init;

    // Zero out the data in the construction structure.
    memset(&init, 0, sizeof(AlfLayoutManagerInitData));

    // Set the construction parameters.        
    init.mLayoutManagerId = const_cast<char*>(aInstanceId);
    init.mNode= aNode;
    init.mCustomData = aCustomData;

    // Run through registered factories.     
    for(int i=0;i<aFactoryList.count()&&!ret;i++)
        {
        // Create a temporary factory product and typecast it to the correct 
        // interface.
        tmp = aFactoryList.at(i)->createProduct(aLoadId,&init);   
        if(tmp)
            {
            // Object created. Attempt to typecast to correct interface.
            ret = IAlfInterfaceBase::makeInterface<IAlfLayoutManager>(tmp);
            if(!ret)
                {
                // Interface not found. Delete the temporary product.	
                delete tmp;    
                tmp=0;        	
                }	
            }
        }

    return ret;
    }    
                

AlfWidgetFactory::AlfWidgetFactory(CAlfEnv& aAlfEnv):
    mAlfEnv(aAlfEnv)
    {
    mWidgetList.setAutoDelete(true);
    }

AlfWidgetFactory::~AlfWidgetFactory()
    {
    mWidgetList.clear();
    mRegisteredFactoryList.clear();
    }

IAlfViewWidget* AlfWidgetFactory::createViewWidget(
    const char* aInstanceId, 
    int aControlGroupID, 
    DuiNode* aNode, 
    CAlfDisplay* aDisplay, 
    const char* aFilePath, 
    AlfCustomInitDataBase* aCustomData)
    {
    IAlfViewWidget* ret(0);
    auto_ptr<IAlfViewWidget> autoptr = auto_ptr<IAlfViewWidget>(0);

    IAlfFactoryPlugin* factoryPlugin = mFactoryPluginLoader.loadFactoryPlugin(
                                                        KViewWidgetProductId);    
    
    CAlfDisplay* display = aDisplay;

    //Check there is already a widget with the same instance ID.
    if(findWidget(aInstanceId))
        {
    	  ALF_THROW(AlfWidgetException,EInvalidWidgetInstanceId,
    	      "Instance ID same as an existing widget.")
        }
            
    //Both node and presentation declaration file handle cannot have 
    //non-null value at the same time.
    if(aNode && aFilePath)
        {
    	  ALF_THROW(AlfWidgetException,EInvalidArgument,
    	      "Both node & presentation declaration file handle cannot have non-null values at the same time")
        }
        
    if(!display)
        {
        if(mAlfEnv.DisplayCount() == 0)
            {
            // No display objects defined in the environment. 
            // Throw an exception.
            ALF_THROW(AlfException, EDisplayNotFound, 
            "No display found from environment.Unable to associate a display with the view widget.");
            }
        display = &mAlfEnv.PrimaryDisplay();    
        }    
    
    // Try default factory first    
    if(factoryPlugin)
        {
        autoptr.reset(createDefaultViewWidget(
                          aInstanceId,aControlGroupID,aNode,*display, 
                          mAlfEnv,*factoryPlugin,aFilePath,aCustomData));
        }

    // Then try registered factories
    if(!autoptr.get())
        {        
        autoptr.reset(createRegisteredViewWidget(
                          aInstanceId,aControlGroupID,aNode, 
                          mAlfEnv,*display,mRegisteredFactoryList, 
                          aFilePath,aCustomData));
        }
        
    if(autoptr.get() != 0)
        {
        // If trap occurs throw exception. Created widget will be automatically
        // destroyed since it is in auto pointer.
        TRAPD(err, appendWidget(
              IAlfInterfaceBase::makeInterface<IAlfWidget>(autoptr.get())));
        if(err != KErrNone)
            {
            ALF_THROW(AlfException, err, 
                "Appending created widget to environment failed.");
            }
            
        // After succesfull call to AppendWidgetL we can release the autoptr.
        ret = autoptr.release();
        }
        
    return ret;
    }

IAlfWidget* AlfWidgetFactory::createWidget(
    const char* aLoadId,
    const char* aInstanceId,
    IAlfContainerWidget& aContainerWidget,
    DuiNode* aNode,
    const char* aFilePath,
    AlfCustomInitDataBase* aCustomData)
    {
    IAlfWidget* ret(0);
    IAlfFactoryPlugin* pluginFactory = mFactoryPluginLoader.loadFactoryPlugin(
                                                                     aLoadId); 

    //Check there is already a widget with the same instance ID.
    if(findWidget(aInstanceId))
        {
    	  ALF_THROW(AlfWidgetException,EInvalidWidgetInstanceId,
    	      "Instance ID same as an existing widget.")
        }
        
    //Both node and presentation declaration file handle cannot have 
    //non-null value at the same time.
    if(aNode && aFilePath)
        {
    	  ALF_THROW(AlfWidgetException,EInvalidArgument,
    	      "Both node & presentation declaration file handle cannot have non-null values at the same time")
        }
    
    // Try first default factory    
    if(pluginFactory)
        {
        ret = createDefaultWidget(
                  aLoadId,aInstanceId,aContainerWidget,aNode,
                  mAlfEnv,*pluginFactory,aFilePath,aCustomData);    
        }

    // Then try registered factories
    if(!ret)
        {
        ret = createRegisteredWidget(
                  aLoadId,aInstanceId,aContainerWidget,aNode,mAlfEnv,
                  mRegisteredFactoryList,aFilePath,aCustomData);        
        }
        
    if(ret)
        {
        TRAPD(err, appendWidget(ret));
        if(err != KErrNone)
            {
            delete ret;	
            ALF_THROW(AlfException, err, 
                "Appending created widget to environment failed.");
            }
        }

    return ret;
    }

int AlfWidgetFactory::destroyWidget(IAlfWidget* aWidget)
    {
    int ret = -1;
    int widgetCount = mWidgetList.count();
    for ( int i = 0; i < widgetCount; ++i ) 
        {        
        if ( mWidgetList[i] == aWidget )
            {            
            // remove and destroy the widget
            mWidgetList.remove( i );
            ret = 0;
            break; ;
            }
        }    
    return ret;
    }
    
int AlfWidgetFactory::appendWidget(IAlfWidget* aWidget)
    {
    int ret = 0;
    int widgetCount = mWidgetList.count();
    for ( int i = 0; i < widgetCount; ++i ) 
        {        
        if ( mWidgetList[i] == aWidget )
            {
            ret = -1;
            break; ;
            }
        }
    if(ret != -1 ) 
	    {
	    mWidgetList.resize( mWidgetList.count()+1);
    	mWidgetList.insert( mWidgetList.count(), aWidget );	
	    }
	return ret;
    }
    
IAlfWidget* AlfWidgetFactory::findWidget(const char* aWidgetName) const
    {
    IAlfWidget* ret(NULL);
    int widgetCount = mWidgetList.count();
    for(int i=0; i<widgetCount && !ret; i++)
        {
        const char* widgetName = mWidgetList[i]->widgetName();
        if(!strcmp(widgetName,aWidgetName))
            {
            ret = mWidgetList[i];    
            }
        }
    return ret;
    }

IAlfModel* AlfWidgetFactory::createModel(
    const char* aLoadId,
    AlfCustomInitDataBase* aCustomData)
    {
    IAlfModel* ret(0);
    IAlfFactoryPlugin* pluginFactory = mFactoryPluginLoader.loadFactoryPlugin(
                                                                     aLoadId);

    // Try first default factory
    if(pluginFactory)
        {
        ret = createDefaultModel(aLoadId,*pluginFactory, aCustomData);    
        }

    // Then try registered factories        
    if(!ret)
        {
        ret = createRegisteredModel(
                  aLoadId,mRegisteredFactoryList,aCustomData);        
        }
    
    return ret;
    }
    
IAlfWidgetControl* AlfWidgetFactory::createControl(
    const char* aLoadId,
    const char* aInstanceId,
    CAlfDisplay* aDisplay,
    AlfCustomInitDataBase* aCustomData)
    {
    IAlfWidgetControl* ret(0);    	
    IAlfFactoryPlugin* pluginFactory = mFactoryPluginLoader.loadFactoryPlugin(
                                                                     aLoadId);
    //Compiler warning removal. Left here for future use cases.
    (void)aInstanceId; 
    
    CAlfDisplay* display = aDisplay;
    
    if(!display)
        {
        display = &mAlfEnv.PrimaryDisplay();    
        }
  
    // Try first default factory
    if(pluginFactory)
    	{
    	 ret = createDefaultControl(aLoadId,*display,*pluginFactory,aCustomData);
    	}

    // Then try registered factories         
    if(!ret)
        {
        ret = createRegisteredControl(
                  aLoadId,*display,mRegisteredFactoryList,aCustomData);        
        }

    return ret;     
    }

IAlfElement* AlfWidgetFactory::createElement(
    const char* aLoadId,
    const char* aInstanceId, 
    IAlfWidgetControl& aControl,
    DuiNode* aNode,
    AlfCustomInitDataBase* aCustomData)
    {
    IAlfElement* ret(0);
    IAlfFactoryPlugin* pluginFactory = mFactoryPluginLoader.loadFactoryPlugin(
                                                                     aLoadId);

    // Try first default factory
    if (pluginFactory)
        {
        ret = createDefaultElement(
                  aLoadId,aInstanceId,aNode,aControl,
                  *pluginFactory,aCustomData);
        }

    // Then try registered factories                 
    if(!ret)
        {
        ret = createRegisteredElement(
                  aLoadId,aInstanceId,aNode,aControl,
                  mRegisteredFactoryList,aCustomData);        
        }

    return ret;    
    }
   
IAlfVisualTemplate* AlfWidgetFactory::createVisualTemplate(
    const char* aLoadId,
    const char* aInstanceId,
    DuiNode* aNode,
    AlfCustomInitDataBase* aCustomData )
    {
     
    IAlfVisualTemplate* ret(0);
    IAlfFactoryPlugin* pluginFactory = mFactoryPluginLoader.loadFactoryPlugin(
                                                                     aLoadId);

    // Try first default factory                                                                 
    if (pluginFactory)
        {
        ret = createDefaultVisualTemplate(
                  aLoadId,aInstanceId,aNode,*pluginFactory,aCustomData);
        }
        
    // Then try registered factories          
    if(!ret)
        {
        ret = createRegisteredVisualTemplate(
                  aLoadId,aInstanceId,aNode,
                  mRegisteredFactoryList,aCustomData);        
        }
        
    return ret;         
    }
    
IAlfWidgetEventHandler* AlfWidgetFactory::createEventHandler(
    const char* aLoadId,
    const char* aInstanceId,
    DuiNode* aNode,
    AlfCustomInitDataBase* aCustomData )
    {
    IAlfWidgetEventHandler* ret(0);
    IAlfFactoryPlugin* pluginFactory = mFactoryPluginLoader.loadFactoryPlugin(
                                                                     aLoadId);

    // Try first default factory
    if (pluginFactory)
        {
        ret = createDefaultEventHandler(
                  aLoadId,aInstanceId,aNode,*pluginFactory,aCustomData);
        }

    // Then try registered factories        
    if(!ret)
        {
        ret = createRegisteredEventHandler(
                  aLoadId,aInstanceId,aNode,
                  mRegisteredFactoryList,aCustomData);        
        }
        
    return ret;    
    }
    
IAlfLayoutManager* AlfWidgetFactory::createLayoutManager(
    const char* aLoadId, 
    const char* aInstanceId, 
    DuiNode* aNode, 
    AlfCustomInitDataBase* aCustomData)
    {
    IAlfLayoutManager* ret(0);
    IAlfFactoryPlugin* pluginFactory = mFactoryPluginLoader.loadFactoryPlugin(
                                                                     aLoadId);

    // Try first default factory
    if (pluginFactory)
        {
        ret = createDefaultLayoutManager(
                  aLoadId,aInstanceId,aNode,*pluginFactory,aCustomData);
        }
    
    // Then try registered factories         
    if(!ret)
        {
        ret = createRegisteredLayoutManager(
                  aLoadId,aInstanceId,aNode,
                  mRegisteredFactoryList,aCustomData);
        }
        
    return ret;
    }

void AlfWidgetFactory::registerCustomWidgetFactory(
    IAlfFactoryPlugin* aFactory)
    {
    mRegisteredFactoryList.resize(mRegisteredFactoryList.count()+1);
    mRegisteredFactoryList.insert(mRegisteredFactoryList.count(),aFactory);
    }
    
IAlfInterfaceBase* AlfWidgetFactory::makeInterface(const IfId& aType)
    {
    if ( !strcmp(aType.mImplementationId, 
                 IAlfWidgetFactory::type().mImplementationId) )
        {
        return static_cast<IAlfWidgetFactory*>(this);
        }
    
    return 0;
    }
    } // namespace Alf    

// End of File
