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
* Description:  Plugin factory implementation for slider widget.
*
*/

// OSN Includes
#include <osn/ustring.h>

// WidgetModel Includes
#include "alf/alfwidget.h"
#include <alf/ialfwidget.h>
#include <alf/ialfelement.h>

// Symbian Includes
#include <ecom/ecom.h>

// OpenC Includes
#include <libc/string.h>

// Widget Includes
#include <mul/imulsliderwidget.h>
#include <mul/mulsliderproductids.h>
#include "imulsliderbaseelementinternal.h"
//Internal includes
#include "mulsliderwidgetfactoryplugin.h"
#include "mulsliderwidget.h"
#include "mulslidermodel.h"
#include "mulslidervertical.h"
#include "mulprogressbarslider.h"
#include "mulsliderhorizontal.h"
//#include "alfscrollbardefaultlctbaseelement.h"

using namespace osncore;

namespace Alf
    {
const TInt KScrollBarWidgetFactoryPluginUid = {0x2000FA80};
const int KProductCount = 5;


// ---------------------------------------------------------------------------
// Two-phased Symbian constructor.
// ---------------------------------------------------------------------------
//
MulSliderWidgetFactoryPlugin* MulSliderWidgetFactoryPlugin::NewL()
    {
    return new (EMM) MulSliderWidgetFactoryPlugin;
    }


// ---------------------------------------------------------------------------
// Destructor.
// ---------------------------------------------------------------------------
//
MulSliderWidgetFactoryPlugin::~MulSliderWidgetFactoryPlugin()
    {

    }
    
    
// ---------------------------------------------------------------------------
// Creates either a scrollbar widget or a scrollbar model.
// ---------------------------------------------------------------------------
//
IAlfInterfaceBase* MulSliderWidgetFactoryPlugin::createProduct(
    const char* aProduct,
    void* aInitData)
    {
    IAlfInterfaceBase* ret (0);
 
     if (!strcmp(aProduct, KSliderWidget))
        {
        AlfWidgetInitData* initData = (AlfWidgetInitData*)aInitData;        
        auto_ptr<MulSliderWidget> widget(MulSliderWidget::create(
            *initData->mEnv, 
            *initData->mContainerWidget,
            initData->mWidgetId,
            initData->mCustomData));            
        ret = widget->makeInterface (IMulSliderWidget::type());
        widget.release();
        }
    else if (!strcmp(aProduct, KSliderModel))
        {
        auto_ptr<MulSliderModel> model(new(EMM) MulSliderModel());
        ret = model->makeInterface (IMulSliderModel::type());
        model.release();
        }
    // Vertical  element support template 3, 9
    else if (!strcmp(aProduct, KSliderVerticalElement))
        {
        AlfElementInitData* initData = (AlfElementInitData*) aInitData;
        auto_ptr<MulSliderVertical> element(
            new(EMM) MulSliderVertical(
                (CAlfWidgetControl&)(*initData->mControl),
                initData->mElementId,
                initData->mCustomData));
                
        ret = element->makeInterface(IMulSliderBaseElementInternal::type());
        element.release();
        }
    // Progressbar element support template 7,8 
        else if(!strcmp(aProduct, KSliderProgressbarElement))
        {
        AlfElementInitData* initData = (AlfElementInitData*) aInitData;
        auto_ptr<MulSliderProgressive> element(
            new(EMM) MulSliderProgressive(
                (CAlfWidgetControl&)(*initData->mControl),
                initData->mElementId,
                initData->mCustomData));
        ret = element->makeInterface(IMulSliderBaseElementInternal::type());
        element.release();
        }      
    // Horizontal element support template 1,2,4,6      
        else if(!strcmp(aProduct, KSliderHorizontalElement))
        {
        AlfElementInitData* initData = (AlfElementInitData*) aInitData;
        auto_ptr<MulSliderHorizontal> element(
            new(EMM) MulSliderHorizontal(
                (CAlfWidgetControl&)(*initData->mControl),
                initData->mElementId,
                initData->mCustomData));
        ret = element->makeInterface(IMulSliderBaseElementInternal::type());
        element.release();
        }      
  
    return ret;
    }
    
    
// ---------------------------------------------------------------------------
// From IAlfInterfaceBase.
// Creates interface based on the given type.
// ---------------------------------------------------------------------------
//
IAlfInterfaceBase* MulSliderWidgetFactoryPlugin::makeInterface(
    const IfId& aType)
    {
    UString param (aType.mImplementationId);
    if (param == UString (IAlfFactoryPlugin::type().mImplementationId))
        {
        return static_cast<IAlfFactoryPlugin*> (this);
        }
    else
        {
        // Do Nothing
        }
    return NULL;
    }
    
    
// ---------------------------------------------------------------------------
// Returns the amount of products this factory can produce.
// ---------------------------------------------------------------------------
//
int MulSliderWidgetFactoryPlugin::productCount() const
    {
    return KProductCount;
    }
    
    
// ---------------------------------------------------------------------------
// Returns product information.
// ---------------------------------------------------------------------------
//
const char* MulSliderWidgetFactoryPlugin::productInfo (int aIndex) const
    {
    switch (aIndex)
        {
    case ESliderWidget:
        {
        return KSliderWidget;
        }    

    case ESliderModel:
        {
        return KSliderModel;
        }

    case ESliderHorizontalElement:
        {
        return KSliderHorizontalElement;
        }

    case ESliderVerticalElement:
        {
        return KSliderVerticalElement;
        }
        
    case ESliderProgressbarElement:
        {
        return KSliderProgressbarElement;
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
        MulSliderWidgetFactoryPlugin::NewL)
#else
        {
        {KScrollBarWidgetFactoryPluginUid}, 
        MulSliderWidgetFactoryPlugin::NewL}
#endif
    };

OSN_EXPORT const TImplementationProxy* ImplementationGroupProxy(
    TInt& aTableCount)
    {
    aTableCount = sizeof (ImplementationTable) / sizeof (TImplementationProxy);

    return ImplementationTable;
    }


