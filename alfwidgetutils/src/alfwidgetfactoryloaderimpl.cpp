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
* Description:  Encapsulates factory plugin loading mechanism
*
*/



#include <ecom/ecom.h>
//#include <alf/alfecompluginfactoryuid.rh>
#include <alf/ialfwidgetfactory.h>
#include <alf/ialffactoryplugin.h>
#include <osn/osnnew.h>

#include "alf/alfecompluginfactoryuid.rh"
#include "alfwidgetfactoryloaderimpl.h"

using namespace std;

namespace Alf
    {

static const char* const KWidgetFactory ="alfwidgetfactory";
const int KPluginIuid = 0x11201113;

AlfWidgetFactoryLoaderImpl::AlfWidgetFactoryLoaderImpl():
        mFactoryPlugin(0),
        mWidgetFactory(0),
        mDtorKey(0)
    {

    }

AlfWidgetFactoryLoaderImpl::~AlfWidgetFactoryLoaderImpl()
    {
    delete mWidgetFactory;
    if (mFactoryPlugin)
        {
        delete mFactoryPlugin;
        REComSession::DestroyedImplementation(TUid::Uid(mDtorKey));
        }

    }

IAlfWidgetFactory* AlfWidgetFactoryLoaderImpl::load(CAlfEnv& aEnv)
    {
    if (!mWidgetFactory)
        {
        TEComResolverParams resolverParams;
        resolverParams.SetDataType(TPtrC8((TUint8*)KWidgetFactory));
        resolverParams.SetWildcardMatch(ETrue);

        TAny* plugin = NULL;

        TUid tmpDtor;

        TRAPD(err,plugin = REComSession::CreateImplementationL(
                               TUid::Uid(KPluginIuid),tmpDtor,
                               resolverParams))


        if (!err)
            {
            mDtorKey = tmpDtor.iUid;
            mFactoryPlugin = reinterpret_cast<IAlfFactoryPlugin*>(plugin);
            IAlfInterfaceBase* tmp(0);
            tmp = mFactoryPlugin->createProduct(KWidgetFactory,&aEnv);
            mWidgetFactory = 
                IAlfInterfaceBase::makeInterface<IAlfWidgetFactory>(tmp);
            }
        }

    return mWidgetFactory;
    }

    }


// End of File

