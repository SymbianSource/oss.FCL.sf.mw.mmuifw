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
* Description:   view widget factory pluging implementation.
*
*/


#include <ecom/implementationproxy.h>
#include <osn/osnnew.h>
#include <alf/ialfviewwidget.h>
#include <alf/alfexception.h>
#include "alfviewwidget.h"
#include "alfviewwidgetexception.h"
#include "alfviewwidgetfactoryplugin.h"

using namespace Alf;

// UID of the view widget factory plugin.
const TInt KViewWidgetFactoryPluginUid = {0x20010104};

// Amount of products produced by this factory.
const int KProductCount = 1;

AlfViewWidgetFactoryPlugin* AlfViewWidgetFactoryPlugin::newL()
    {
    return new (EMM) AlfViewWidgetFactoryPlugin();
    }

IAlfInterfaceBase* AlfViewWidgetFactoryPlugin::createProduct(const char* aProduct, void* aInitData)
    {
    AlfViewWidget* ret(0);
    
    if(!strcmp(aProduct, IAlfViewWidget::type().mImplementationId))
        {        
        // Typecast the initialization data
        AlfViewWidgetInitData* initData = (AlfViewWidgetInitData*) aInitData;
        
        // Throw exception if init data is null
        if(initData == 0)
        	{
        	ALF_THROW(AlfViewWidgetException, EInitializationError, "View widget initialization data was NULL.");
        	}
        	
        // Assert that the UI Accelerator Toolkit Environment in the initialization data is correct.
        if(initData->mEnv == 0)
        	{
        	ALF_THROW(AlfViewWidgetException, EInitializationError, "Environment instance in view widget initialization data was NULL.");
        	}        	
        	                	
        // Assert that widget id in the initialization data is correct.
        if(initData->mWidgetId == 0)
        	{
        	ALF_THROW(AlfViewWidgetException, EInitializationError, "Widget ID in view widget initialization data was NULL.");
        	}
        
        
        // Assert that display pointer in the initialization data is correct.
        if(initData->mDisplay == 0)
        	{
        	ALF_THROW(AlfViewWidgetException, EInitializationError, "Display instance in view widget initialization data was NULL.");
        	}
        
        // Create container widget        
        ret = new (EMM) AlfViewWidget(initData->mWidgetId, initData->mControlGroupId, *initData->mEnv, 
                *initData->mDisplay, initData->mNode, initData->mFilePath);
        }
    
    return static_cast<IAlfViewWidget*>(ret);
    }

int AlfViewWidgetFactoryPlugin::productCount() const
    {
    return KProductCount;
    }
 
const char* AlfViewWidgetFactoryPlugin::productInfo(int aIndex) const
    {
    switch(aIndex)
        {
        case 0: return alfviewwidget::ident.mImplementationId;
        default: break;            
        }

    return 0;
    }
 
IAlfInterfaceBase* AlfViewWidgetFactoryPlugin::makeInterface(const IfId& aType)
    {
    if(!strcmp(aType.mImplementationId, IAlfFactoryPlugin::type().mImplementationId))
        {
        return static_cast<IAlfFactoryPlugin*>(this);
        }
    
    return 0;
    }

// ========================== OTHER EXPORTED FUNCTIONS =========================

const TImplementationProxy ImplementationTable[] =
    {
    IMPLEMENTATION_PROXY_ENTRY( KViewWidgetFactoryPluginUid, 
                                AlfViewWidgetFactoryPlugin::newL )
    };

// Entry point for ECom architecture to resolve the instantiation method pointer
// for a given implementation UID.
EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
    {
    aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);
    return ImplementationTable;
    }
