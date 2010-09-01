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
* Description:   AlfContainerWidgetFactoryPlugin implementation
*
*/

#include <osn/osnnew.h>
#include <ecom/implementationproxy.h>
#include "alf/ialfcontainerwidget.h"
#include "alf/alfwidget.h"
#include <libc/string.h>
#include "alfcontainerwidget.h"
#include "alfcontainerwidgetexception.h"
#include "alfcontainerwidgetfactoryplugin.h"

using namespace Alf;

// UID of the container widget factory plugin.
const TInt KContainerWidgetFactoryPluginUid = {0x200100FF};

// Amount of products produced by this factory.
const int KProductCount = 1;

AlfContainerWidgetFactoryPlugin* AlfContainerWidgetFactoryPlugin::newL()
    {
    return new (ELeave) AlfContainerWidgetFactoryPlugin();
    }

IAlfInterfaceBase* AlfContainerWidgetFactoryPlugin::createProduct(const char* aProduct, void* aInitData)
    {
    IAlfInterfaceBase* ret(0);
    
    if(!strcmp(aProduct, IAlfContainerWidget::type().mImplementationId))
        {        
        // Typecast the initialization data
        AlfWidgetInitData* initData = (AlfWidgetInitData*) aInitData;
        
        // Throw exception if init data is null
        if(initData == 0)
        	{
        	ALF_THROW(AlfContainerWidgetException, EInitializationError, "Container widget initialization data was NULL.");
        	}
        
        // Assert that widget id in the initialization data is correct.
        if(initData->mWidgetId == 0)
        	{
        	ALF_THROW(AlfContainerWidgetException, EInitializationError, "Widget ID was NULL.");
        	}
        
        // Assert that the UI Accelerator Toolkit Environment in the initialization data is correct.
        if(initData->mEnv == 0)
        	{
        	ALF_THROW(AlfContainerWidgetException, EInitializationError, "Environment instance was NULL.");
        	}
        
        // Assert that container widget pointer in the initialization data is correct.
        if(initData->mContainerWidget == 0)
        	{
        	ALF_THROW(AlfContainerWidgetException, EInitializationError, "Container widget instance was NULL.");
        	}
        
        // Create container widget
        ret = new (EMM) AlfContainerWidget(
        		initData->mWidgetId, *initData->mContainerWidget, *initData->mEnv, initData->mNode, initData->mFilePath);     
        }
    
    return ret;
    }

int AlfContainerWidgetFactoryPlugin::productCount() const
    {
    return KProductCount;
    }
 
const char* AlfContainerWidgetFactoryPlugin::productInfo(int aIndex) const
    {
    switch(aIndex)
        {
        case 0: return ialfcontainerwidget::ident.mImplementationId;
        default: break;            
        }

    return 0;
    }
 
IAlfInterfaceBase* AlfContainerWidgetFactoryPlugin::makeInterface(const IfId& aType)
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
    IMPLEMENTATION_PROXY_ENTRY( KContainerWidgetFactoryPluginUid, 
                                AlfContainerWidgetFactoryPlugin::newL )
    };

// Entry point for ECom architecture to resolve the instantiation method pointer
// for a given implementation UID.
EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
    {
    aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);
    return ImplementationTable;
    }
