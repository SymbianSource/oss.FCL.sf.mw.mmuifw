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
* Description:  factory plugin interface.
*
*/



#ifndef I_ALFFACTORYPLUGIN_H
#define I_ALFFACTORYPLUGIN_H


#include <alf/ialfinterfacebase.h>
#include <alf/alftypes.h>

namespace Alf
    {

namespace alffactoryplugin
    {
static const IfId ident=
    {
    0,"alffactoryplugin"
    };
    }

/**
 *  @class IAlfFactoryPlugin ialffactoryplugin.h "alf/ialffactoryplugin.h"
 *  Base class for factory plugins.
 *
 *  @lib alfwidgetutils.lib
 *  @since S60 ?S60_version
 *  @status Draft
 */
class IAlfFactoryPlugin : public IAlfInterfaceBase
    {
public:
    static const IfId& type()
        {
        return alffactoryplugin::ident;
        }
public:
    /**
     * Create factory product.
     * @param aProduct Product to create.
     * @param aInitData Initialization data for product.
     * @return Instance or 0.
     */
    virtual IAlfInterfaceBase* createProduct( 
                                   const char* aProduct, void* aInitData ) = 0;

    /**
     * Count available products.
     *
     * @return Product count.
     */
    virtual int productCount() const = 0;

    /**
     * Fetch product info by index.
     *
     * @param aIndex List index.
     * @return String describing product.
     */
    virtual const char* productInfo( int aIndex ) const = 0;

    /**
     * Destructor.
     */
    virtual ~IAlfFactoryPlugin() {};
    };
    }

#endif

// End of File
