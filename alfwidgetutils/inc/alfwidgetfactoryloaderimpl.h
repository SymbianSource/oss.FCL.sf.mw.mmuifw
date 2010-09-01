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
* Description:  Widget factory plugin loader implementation
*
*/



#ifndef ALFWIDGETFACTORYLOADERIMPL_H
#define ALFWIDGETFACTORYLOADERIMPL_H



class CAlfEnv;

namespace Alf
    {
class IAlfFactoryPlugin;
class IAlfWidgetFactory;


/**
 *
 *  @class AlfWidgetFactoryLoaderImpl AlfWidgetFactoryLoaderImpl.h "alf/alfwidgetfactoryloaderimpl.h"
 *
 *  Factory plugin loader loads factory plugins.
 *
 *  @lib alfwidgetutils.lib
 *  @since S60 ?S60_version
 *  @status Draft 
 */
class AlfWidgetFactoryLoaderImpl
    {
public:  // Constructors and destructor

    /**
     * Constructor
     */
    AlfWidgetFactoryLoaderImpl();

    /**
     * Destructor.
     */
    virtual ~AlfWidgetFactoryLoaderImpl();

public:

    /**
     * Loads a widget factory plugin.
     *
     * @since S60 5.0
     * @return Pointer to IAlfWidgetFactory or NULL. 
     *         Ownership is not transferred to caller.
     */
    IAlfWidgetFactory* load(CAlfEnv& aEnv);

private:
    IAlfFactoryPlugin* mFactoryPlugin;
    IAlfWidgetFactory* mWidgetFactory;
    int mDtorKey;

    };
    }

#endif

// End of File
