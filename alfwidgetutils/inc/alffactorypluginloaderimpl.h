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
* Description:  Factory plugin loader implementation
*
*/



#ifndef ALFFACTORYPLUGINLOADERIMPL_H
#define ALFFACTORYPLUGINLOADERIMPL_H

#include <osn/alfptrvector.h>

using namespace osncore;

namespace Alf
    {

class IAlfFactoryPlugin;
class AlfFactoryPluginMap;


/**
 *  @class AlfFactoryPluginLoaderImpl AlfFactoryPluginLoaderImpl.h "alf/alffactorypluginloaderimpl.h" 
 *
 *  Factory plugin loader loads factory plugins.
 *
 *  @lib alfwidgetutils.lib
 *  @since S60 ?S60_version
 *  @status Draft 
 */
class AlfFactoryPluginLoaderImpl
    {
public:  // Constructors and destructor

    /**
     * Constructor
     */
    AlfFactoryPluginLoaderImpl();

    /**
     * Destructor.
     */
    virtual ~AlfFactoryPluginLoaderImpl();

public:

    /**
     * Load a factory plugin.
     *
     * @param aLoadId Factory plugin identifier
     * @since S60 5.0
     * @return Pointer to IAlfFactoryPlugin or NULL. 
     *         Ownership is transferred to caller.
     */
    IAlfFactoryPlugin* load(const char* aLoadId);

private:
    /**
     * Owned factory plugins.
     */
    AlfPtrVector<AlfFactoryPluginMap>   mFactoryPluginList;

    };
    }

#endif

// End of File
