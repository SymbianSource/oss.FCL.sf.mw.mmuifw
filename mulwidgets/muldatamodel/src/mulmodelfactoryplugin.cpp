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
* Description:  Plugin factory implementation for model.
*
*/

#include "mulmodelfactoryplugin.h"

#include <osn/ustring.h>
#include "alf/alfwidget.h"
#include <ecom/ecom.h>
#include <libc/string.h>

#include <mul/imulmodel.h>

#include "mulmodelimpl.h"

using namespace osncore;

namespace Alf
    {

const TInt KMulModelFactoryPluginUid = {0x2000D23F};
const int KProductCount = 1;

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Two-phased Symbian constructor.
// ---------------------------------------------------------------------------
//
MulFactoryPlugin* MulFactoryPlugin::NewL()
    {
    return new (ELeave) MulFactoryPlugin;
    }

// ---------------------------------------------------------------------------
// Destructor.
// ---------------------------------------------------------------------------
//
MulFactoryPlugin::~MulFactoryPlugin()
    {
    }
    
// ---------------------------------------------------------------------------
// Creates either a list widget or a list model.
// ---------------------------------------------------------------------------
//
IAlfInterfaceBase* MulFactoryPlugin::createProduct(const char* aProduct,
													void* /*aInitData*/ )
    {
    IAlfInterfaceBase* ret(0);
   const char* KLoadNameModel("mulmodel");
   //if( !strcmp( aProduct, IMulModel::Type().iImplementationId ) )
   if( !strcmp( aProduct, KLoadNameModel ) )
        {
        // create list model
        IMulModel* model = new (EMM) MulModelImpl();
        ret = IAlfInterfaceBase::makeInterface<IMulModel>( model );    
        }
    return ret;
    }

// ---------------------------------------------------------------------------
// From MAlfInterfaceBase.
// Creates interface based on the given type.
// ---------------------------------------------------------------------------
//
IAlfInterfaceBase* MulFactoryPlugin::makeInterface(const IfId& aType)
    {
    UString param( aType.mImplementationId );
    
    if( param == UString( IAlfFactoryPlugin::type().mImplementationId ) )
        {
        return static_cast<IAlfFactoryPlugin*>( this );            
        }
    return NULL;
    }

// ---------------------------------------------------------------------------
// Returns the amount of products this factory can produce.
// ---------------------------------------------------------------------------
//
int MulFactoryPlugin::productCount()const
    {
    return KProductCount;
    }

// ---------------------------------------------------------------------------
// Returns product information.
// ---------------------------------------------------------------------------
//
const char* MulFactoryPlugin::productInfo(int aIndex)const
    {
    switch( aIndex )
        {
        case 0: return mulmodel::Ident.mImplementationId;
        default: break;            
        }  
    return 0;
    }
    
    } // namespace Alf

// ========================== OTHER EXPORTED FUNCTIONS =========================

using namespace Alf;
const TImplementationProxy ImplementationTable[] =
    {
#ifdef __EABI__ 
	IMPLEMENTATION_PROXY_ENTRY( KMulModelFactoryPluginUid, 
								MulFactoryPlugin::NewL )
	#else
    { {KMulModelFactoryPluginUid}, MulFactoryPlugin::NewL }
#endif
    };

EXPORT_C const TImplementationProxy* ImplementationGroupProxy( TInt& aTableCount )
    {
    aTableCount = sizeof( ImplementationTable ) / sizeof( TImplementationProxy );

    return ImplementationTable;
    }
    
      
