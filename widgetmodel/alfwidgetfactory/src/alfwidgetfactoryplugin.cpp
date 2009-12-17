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
* Description:   File imeplemets the widgetfactory for interfaces.
*
*/


//Symbian includes
#include <ecom/ecom.h>

//Alfred client includes
#include <alf/alfenv.h>

//OSN core includes
#include <osn/ustring.h>
#include <osn/osnnew.h>
#include <osn/osncommon.h>

//Widget utils includes
#include <alf/alftypes.h>

//Local includes
#include "alfwidgetfactoryplugin.h"
#include "alfwidgetfactory.h"

//Namespaces
using namespace Alf;
using namespace osncore;


namespace Alf
    {
    	
const TInt KWidgetFactoryPluginUid = {0x10201256};
const int KProductCount = 1; 
static const char* const KAlfWidgetFactory = "alfwidgetfactory";


IAlfFactoryPlugin* AlfWidgetFactoryPlugin::NewL()
    {
    // Must use (ELeave), because ECOM won't undestand bad_alloc
    return new(ELeave)AlfWidgetFactoryPlugin; 
    }

AlfWidgetFactoryPlugin::~AlfWidgetFactoryPlugin()
    {
    }
    
IAlfInterfaceBase* AlfWidgetFactoryPlugin::createProduct(
    const char* aProduct,
    void* aInitData)
    {
    (void) aProduct; //Compiler Warning removal.
    
    CAlfEnv* env = reinterpret_cast<CAlfEnv*>(aInitData);
    AlfWidgetFactory* widgetFactory( new( EMM ) AlfWidgetFactory(*env) );
    // This basically returns the widgetFactory object.
    // The ownership of that object is transferred to the caller.
    return widgetFactory->makeInterface(IAlfWidgetFactory::type());
    }

IAlfInterfaceBase* AlfWidgetFactoryPlugin::makeInterface(const IfId& aType)
    {
    UString param(aType.mImplementationId);
    if(param == UString(IAlfFactoryPlugin::type().mImplementationId))
        {
        return static_cast<IAlfFactoryPlugin*>(this);    
        }
    return NULL;
    }
    
int AlfWidgetFactoryPlugin::productCount()const
    {
    return KProductCount;    
    }
    
const char* AlfWidgetFactoryPlugin::productInfo(int aIndex)const
    {
    if(aIndex == 0)
        {
        return KAlfWidgetFactory;    
        }
    return 0;
    }
    
    } // namespace Alf    



const TImplementationProxy ImplementationTable[] =
    {
    #ifdef __EABI__ 
	  IMPLEMENTATION_PROXY_ENTRY(
	      KWidgetFactoryPluginUid, 
	      AlfWidgetFactoryPlugin::NewL),
	  #else
    {{KWidgetFactoryPluginUid}, AlfWidgetFactoryPlugin::NewL}
    #endif
    };

OSN_EXPORT const TImplementationProxy* ImplementationGroupProxy(
    TInt& aTableCount)
    {
    aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);

    return ImplementationTable;
    }

// End of File
