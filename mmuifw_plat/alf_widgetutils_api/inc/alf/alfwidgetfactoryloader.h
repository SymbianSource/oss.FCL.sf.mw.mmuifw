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
* Description:  Loads widget factory plugin
*
*/



#ifndef I_ALFWIDGETFACTORYLOADER_H
#define I_ALFWIDGETFACTORYLOADER_H

#include <osn/osndefines.h>
#include <memory>

using namespace std;

class CAlfEnv;

namespace Alf
    {
class IAlfWidgetFactory;
class AlfWidgetFactoryLoaderImpl;

/**
 *  @class AlfWidgetFactoryLoader alfwidgetfactoryloader.h "alf/alfwidgetfactoryloader.h"
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
class AlfWidgetFactoryLoader
    {
public:  // Constructors and destructor

    /**
     * Constructor.
     *
     * @exception std::bad_alloc Thrown if out of memory during the construction.
     */
    OSN_IMPORT AlfWidgetFactoryLoader();

    /**
     * Destructor.
     */
    OSN_IMPORT virtual ~AlfWidgetFactoryLoader();

public:

    /**
     * Loads a widget factory.
     * 
     * @param aEnv Alfred Environment
     * @return Pointer to IAlfWidgetFactory or 0. 
     *         Ownership is not transferred to caller.
     */
    OSN_IMPORT IAlfWidgetFactory* loadWidgetFactory(CAlfEnv& aEnv);

private:

    /**
     * Copy Constructor
     */
    AlfWidgetFactoryLoader( const AlfWidgetFactoryLoader& );

    /**
     * Assignment Operator
     */
    AlfWidgetFactoryLoader& operator=( const AlfWidgetFactoryLoader& );

private:    // Data
    auto_ptr<AlfWidgetFactoryLoaderImpl> mImpl;
    };
    }

#endif

// End of File
