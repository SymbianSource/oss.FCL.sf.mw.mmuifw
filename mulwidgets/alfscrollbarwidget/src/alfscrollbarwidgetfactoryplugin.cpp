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
* Description:  Plugin factory implementation for scrollbar widget.
*
*/

// WidgetModel Includes
#include "alf/alfwidget.h"
#include <alf/ialfwidget.h>

// Symbian Includes
#include <ecom/ecom.h>

// OpenC Includes
#include <libc/string.h>

// Widget Includes
#include <alf/ialfscrollbarwidget.h>
#include <alf/alfscrollbarproductids.h>

//Internal includes
#include "alfscrollbarwidgetfactoryplugin.h"
#include "alfscrollbarwidget.h"
#include "alfscrollbarmodel.h"
#include "alfscrollbardefaultbaseelement.h"

using namespace osncore;

namespace Alf
    {
const TInt KScrollBarWidgetFactoryPluginUid = {0x20010106};
const int KProductCount = 3;


// ---------------------------------------------------------------------------
// Two-phased Symbian constructor.
// ---------------------------------------------------------------------------
//
AlfScrollBarWidgetFactoryPlugin* AlfScrollBarWidgetFactoryPlugin::NewL()
    {
    return new (EMM) AlfScrollBarWidgetFactoryPlugin;
    }


// ---------------------------------------------------------------------------
// Destructor.
// ---------------------------------------------------------------------------
//
AlfScrollBarWidgetFactoryPlugin::~AlfScrollBarWidgetFactoryPlugin()
    {

    }
    
    
// ---------------------------------------------------------------------------
// Creates either a scrollbar widget or a scrollbar model.
// ---------------------------------------------------------------------------
//
IAlfInterfaceBase* AlfScrollBarWidgetFactoryPlugin::createProduct(
    const char* aProduct,
    void* aInitData)
    {
    IAlfInterfaceBase* ret (0);
    if (aProduct == NULL)
        {
        // do nothing
        }
    else if (!strcmp(aProduct, KScrollBarWidget))
        {
        AlfWidgetInitData* initData = (AlfWidgetInitData*)aInitData;
       
        if (initData == 0)
            {
            // Throw exception if init data is null
            ALF_THROW(
                AlfException, 
                ECommonError, 
                "Scrollbar widget initialization data was NULL.");
            }
        else
            {
            // Do Nothing
            }
        
        if (initData->mWidgetId == 0)
            {
            // Assert that widget id in the initialization data is correct.
            ALF_THROW(
                AlfException, 
                ECommonError, 
                "Widget ID was NULL.");
            }
        else
            {
            // Do Nothing
            }
        
        if (initData->mEnv == 0)
            {
            // Assert that the UI Accelerator Toolkit Environment 
            // in the initialization data is correct.
            ALF_THROW(
                AlfException, 
                ECommonError, 
                "Environment instance was NULL.");
            }
        else
            {
            // Do Nothing
            }
        
        if (initData->mContainerWidget == 0)
            {
            // Assert that container widget pointer in the 
            // initialization data is correct.
            ALF_THROW(
                AlfException, 
                ECommonError, 
                "Container widget instance was NULL.");
            }
        else
            {
            // Do Nothing
            }
        
        auto_ptr<AlfScrollBarWidget> widget(AlfScrollBarWidget::create(
            *initData->mEnv, 
            *initData->mContainerWidget,
            initData->mWidgetId,
            initData->mNode, 
            initData->mFilePath, 
            initData->mCustomData));
            
        ret = widget->makeInterface (IAlfScrollBarWidget::type());
        widget.release();
        }
    else if (!strcmp(aProduct, KScrollBarModel))
        {
        auto_ptr<AlfScrollBarModel> model(new(EMM) AlfScrollBarModel(NULL));
        ret = model->makeInterface (IAlfScrollBarModel::type());
        model.release();
        }
    else if (!strcmp(aProduct, KScrollBarDefaultBaseElement))
        {
        AlfElementInitData* initData = (AlfElementInitData*) aInitData;
        // TBD: Need to change the exceptions to scrollbar specific exceptions. 
        // Throw exception if init data is null
        if (initData == 0)
            {
            ALF_THROW(
                AlfException, 
                ECommonError, 
                "Scrollbar widget initialization data was NULL.");
            }
        else
            {
            // Do Nothing
            }
        
        if (initData->mElementId == 0)
            {
            // Assert that element id in the initialization data is correct.
            ALF_THROW(
                AlfException, 
                ECommonError, 
                "Element ID was NULL.");
            }
        else
            {
            // Do Nothing
            }
        
        if (initData->mControl == 0)
            {
            // Assert that the widget control is correct.
            ALF_THROW(
                AlfException, 
                ECommonError, 
                "Environment instance was NULL.");
            }
        else
            {
            // Do Nothing
            }
            
        auto_ptr<AlfScrollBarDefaultBaseElement> element(
            new(EMM) AlfScrollBarDefaultBaseElement(
                (CAlfWidgetControl&)(*initData->mControl),
                initData->mElementId,
                initData->mNode,initData->mCustomData));
                
        ret = element->makeInterface(IAlfScrollBarDefaultBaseElement::type());
        element.release();
        }      
    else
        {
        // Do Nothing
        }

    return ret;
    }
    
    
// ---------------------------------------------------------------------------
// From IAlfInterfaceBase.
// Creates interface based on the given type.
// ---------------------------------------------------------------------------
//
IAlfInterfaceBase* AlfScrollBarWidgetFactoryPlugin::makeInterface(
    const IfId& /*aType*/)
    {
    return NULL;
    }
    
    
// ---------------------------------------------------------------------------
// Returns the amount of products this factory can produce.
// ---------------------------------------------------------------------------
//
int AlfScrollBarWidgetFactoryPlugin::productCount() const
    {
    return KProductCount;
    }
    
    
// ---------------------------------------------------------------------------
// Returns product information.
// ---------------------------------------------------------------------------
//
const char* AlfScrollBarWidgetFactoryPlugin::productInfo (int aIndex) const
    {
    switch (aIndex)
        {
    case 0:
        {
        return KScrollBarWidget;
        }    

    case 1:
        {
        return KScrollBarModel;
        }

    case 2:
        {
        return KScrollBarDefaultBaseElement;
        }
    

    default:
        break;

        }

    return 0;
    }

    }// NameSpace 


// ---------------------------------------------------------------------------
// OTHER EXPORTED FUNCTIONS.
// ---------------------------------------------------------------------------
//

using namespace Alf;
const TImplementationProxy ImplementationTable[] =
    {
#ifdef __EABI__
    IMPLEMENTATION_PROXY_ENTRY (
        KScrollBarWidgetFactoryPluginUid, 
        AlfScrollBarWidgetFactoryPlugin::NewL)
#else
        {
        {KScrollBarWidgetFactoryPluginUid}, 
        AlfScrollBarWidgetFactoryPlugin::NewL}
#endif
    };

OSN_EXPORT const TImplementationProxy* ImplementationGroupProxy(
    TInt& aTableCount)
    {
    aTableCount = sizeof (ImplementationTable) / sizeof (TImplementationProxy);

    return ImplementationTable;
    }


