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

#include <osn/osnnew.h>
#include <alf/alffactorypluginloader.h>
#include "alffactorypluginloaderimpl.h"

namespace Alf
    {


OSN_EXPORT AlfFactoryPluginLoader::AlfFactoryPluginLoader():
        mImpl(new (EMM)AlfFactoryPluginLoaderImpl())
    {

    }

OSN_EXPORT AlfFactoryPluginLoader::~AlfFactoryPluginLoader()
    {

    }

OSN_EXPORT IAlfFactoryPlugin* AlfFactoryPluginLoader::loadFactoryPlugin(
    const char*  aLoadId)
    {
    return mImpl->load(aLoadId);
    }

    }

// End of File
