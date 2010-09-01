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
* Description:  Encapsulates widget factory plugin loading mechanism
*
*/

#include <osn/osnnew.h>
#include <alf/alfwidgetfactoryloader.h>
#include "alfwidgetfactoryloaderimpl.h"

namespace Alf
    {

OSN_EXPORT AlfWidgetFactoryLoader::AlfWidgetFactoryLoader():
        mImpl(new (EMM)AlfWidgetFactoryLoaderImpl())
    {

    }

OSN_EXPORT AlfWidgetFactoryLoader::~AlfWidgetFactoryLoader()
    {

    }

OSN_EXPORT IAlfWidgetFactory* AlfWidgetFactoryLoader::loadWidgetFactory(
    CAlfEnv& aEnv)
    {
    return mImpl->load(aEnv);
    }

    }


// End of File

