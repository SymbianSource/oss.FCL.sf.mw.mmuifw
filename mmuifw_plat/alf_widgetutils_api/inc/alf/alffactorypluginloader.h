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
* Description:  Factory plugin loader
*
*/



#ifndef ALFFACTORYPLUGINLOADER_H
#define ALFFACTORYPLUGINLOADER_H

#include <osn/osndefines.h>
#include <memory>

using namespace std;

namespace Alf
    {
class IAlfFactoryPlugin;
class AlfFactoryPluginLoaderImpl;

/**
 *  @class AlfFactoryPluginLoader alffactorypluginloader.h "alf/alffactorypluginloader.h"
 *
 *  Factory plugin loader loads factory plugins. Loader will resolve factory 
 *  plugin based on given string and load factory which can create requested 
 *  product. Loader owns factory plugins. Factory products and their 
 *  derivatives must be destroyed before destroying a loader object.
 *
 *  @lib alfwidgetutils.lib
 *  @since S60 ?S60_version
 *  @status Draft
 */
class AlfFactoryPluginLoader
    {
public:  // Constructors and destructor

    /**
     * Constructor.
     *     
     * @exception std::bad_alloc Thrown if out of memory during the 
     *                           construction.
     */
    OSN_IMPORT AlfFactoryPluginLoader();

    /**
     * Destructor.
     */
    OSN_IMPORT virtual ~AlfFactoryPluginLoader();

public:

    /**
     * Load a factory plugin.
     *
     * @param aFactoryProduct Factory plugin is loaded based on a given string
     * @since S60 5.0
     * @return Pointer to IAlfFactoryPlugin or NULL. Ownership is not 
     *         transferred to caller.
     */
    OSN_IMPORT IAlfFactoryPlugin* loadFactoryPlugin(const char* aFactoryProduct);

private:
    /**
     * Copy Constructor
     */
    AlfFactoryPluginLoader(const AlfFactoryPluginLoader&);

    /**
     * Assignment Operator
     */
    AlfFactoryPluginLoader& operator=(const AlfFactoryPluginLoader&);

private:    // Data
    auto_ptr<AlfFactoryPluginLoaderImpl> mImpl;
    };

    } // namespace Alf

#endif

// End of File
