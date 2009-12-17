/*
* Copyright (c) 2007-2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  factory plugin Implementation
*
*/

//  Include Files

#include "mulcoverflowwidgetfactoryplugin.h"
#include "mulcoverflowwidget.h"

namespace Alf
    {

const TInt KCoverFlowWidgetFactoryPluginUid =
	{
			0x2000D241
	};

const int KProductCount = 1;

static const char* const KWidget = "mulcoverflowwidget";

// ---------------------------------------------------------------------------
// NewL
// ---------------------------------------------------------------------------
//    
MulCoverFlowWidgetFactoryPlugin* MulCoverFlowWidgetFactoryPlugin::NewL()
    {
    return new(ELeave)MulCoverFlowWidgetFactoryPlugin;
    }

// ---------------------------------------------------------------------------
// ~MulCoverFlowWidgetFactoryPlugin
// ---------------------------------------------------------------------------
//    
MulCoverFlowWidgetFactoryPlugin::~MulCoverFlowWidgetFactoryPlugin()
    {   
    }
    
// ---------------------------------------------------------------------------
// createProduct
// ---------------------------------------------------------------------------
//    
IAlfInterfaceBase* MulCoverFlowWidgetFactoryPlugin::createProduct(
    const char* aProduct,
    void* aInitData)
    {
    IAlfInterfaceBase* ret(0);

    //Creates the instance of the CoverFlow widget
    if(!strcmp(aProduct,KWidget))
        {
        AlfWidgetInitData* initData = reinterpret_cast<AlfWidgetInitData*>(aInitData);
        auto_ptr<MulCoverFlowWidget> Widget (new (EMM) MulCoverFlowWidget(*initData->mEnv,
        		initData->mWidgetId,*initData->mContainerWidget) );
        ret = Widget->makeInterface(IMulCoverFlowWidget::Type());    
        Widget.release();
        }
    return ret;
    }

// ---------------------------------------------------------------------------
// makeInterface
// ---------------------------------------------------------------------------
//    
IAlfInterfaceBase* MulCoverFlowWidgetFactoryPlugin::makeInterface(
        const IfId& aType)
    {    
    if( UString(aType.mImplementationId) == UString(IAlfFactoryPlugin::type().mImplementationId) )
        {
        return static_cast<IAlfFactoryPlugin*>(this);            
        }
    return NULL;
    }

// ---------------------------------------------------------------------------
// productCount
// ---------------------------------------------------------------------------
//        
int MulCoverFlowWidgetFactoryPlugin::productCount()const
    {
    return KProductCount;    
    }
 
// ---------------------------------------------------------------------------
// productInfo
// ---------------------------------------------------------------------------
//        
const char* MulCoverFlowWidgetFactoryPlugin::productInfo(int aIndex)const
    {
    /// @bug minor:avanhata:3/7/2008 good old if would be clearer
    /// (Surely it is a caller error to call this function with aIndex >= productCount()
    /// anyway, so can simply assert)
    switch(aIndex)
        {
		case 0:
			return KWidget;

		default:
			break;
        }
    
    return 0;
    }
    
    }  //namespace Alf         
// ========================== OTHER EXPORTED FUNCTIONS =========================

using namespace Alf;

const TImplementationProxy
		ImplementationTable[] =
    {
	#ifdef __EABI__ 
    
    IMPLEMENTATION_PROXY_ENTRY(KCoverFlowWidgetFactoryPluginUid, MulCoverFlowWidgetFactoryPlugin::NewL)
    
    #else
	
	{ {KCoverFlowWidgetFactoryPluginUid},MulCoverFlowWidgetFactoryPlugin::NewL }
	
	#endif
    };

OSN_EXPORT const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
    {
    aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);

    return ImplementationTable;
    }
    
//End of File
