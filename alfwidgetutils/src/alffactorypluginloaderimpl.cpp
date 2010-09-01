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
#include <alf/ialffactoryplugin.h>
#include <string.h>
#include <memory>
#include <osn/osnnew.h>
#include <stdexcept>

#include "alf/alfecompluginfactoryuid.rh"
#include "alffactorypluginloaderimpl.h"

using namespace std;

namespace Alf
    {



class AlfFactoryPluginMap
    {
public:
    AlfFactoryPluginMap();
    ~AlfFactoryPluginMap();

    /**
     * Owned factory plugin.
     */
    IAlfFactoryPlugin*  mFactoryPlugin;
    /**
     * ECom destructor handle.
     */
    int    mDtorKey;
    };

static bool loadPluginIntoMap(
    AlfFactoryPluginMap*& aMap,
    const char* aProduct)
    {
    TEComResolverParams resolverParams;
    resolverParams.SetDataType(TPtrC8((TUint8*)aProduct));
    resolverParams.SetWildcardMatch(ETrue);

    TAny* plugin = NULL;
    TUid tmpDtor;

    TRAPD(err,plugin = REComSession::CreateImplementationL(
                           TUid::Uid(KFactoryPluginUid),
                           tmpDtor,resolverParams))


    if (!err)
        {
        aMap->mDtorKey = tmpDtor.iUid;
        aMap->mFactoryPlugin = reinterpret_cast<IAlfFactoryPlugin*>(
                                                            plugin);
        return true;
        }

    return false;
    }

AlfFactoryPluginMap::AlfFactoryPluginMap()
        :mFactoryPlugin(0),mDtorKey(0)
    {

    }

AlfFactoryPluginMap::~AlfFactoryPluginMap()
    {
    if (mFactoryPlugin)
        {
        REComSession::DestroyedImplementation(TUid::Uid(mDtorKey));
        delete mFactoryPlugin;
        }
    }

AlfFactoryPluginLoaderImpl::AlfFactoryPluginLoaderImpl()
    {

    }

AlfFactoryPluginLoaderImpl::~AlfFactoryPluginLoaderImpl()
    {
    mFactoryPluginList.clear();

    }

IAlfFactoryPlugin* AlfFactoryPluginLoaderImpl::load(
    const char*  aLoadId)
    {
    IAlfFactoryPlugin* ret(0);
    IAlfFactoryPlugin* tmp(0);
    // Do we have it already?
    unsigned int itemCount(mFactoryPluginList.count());

    for (int i=0;i<itemCount && !ret;i++)
        {
        tmp = mFactoryPluginList.at(i)->mFactoryPlugin;
        int productCount = tmp->productCount();
        for (int j=0;j<productCount && !ret;j++)
            {
            if (!strcmp(tmp->productInfo(j),aLoadId ))
                {
                ret = tmp;
                }
            }
        }
    if (!ret)
        {
        auto_ptr<AlfFactoryPluginMap> mapPtr(new(EMM)AlfFactoryPluginMap());
        AlfFactoryPluginMap* map = mapPtr.get();

        if (loadPluginIntoMap(map,aLoadId))
            {
            ret = mapPtr->mFactoryPlugin;
            mFactoryPluginList.resize(itemCount+1);
            mFactoryPluginList.insert(itemCount,map);

            mapPtr.release();

            }

        }

    return ret;

    }
    
    }

// End of File
