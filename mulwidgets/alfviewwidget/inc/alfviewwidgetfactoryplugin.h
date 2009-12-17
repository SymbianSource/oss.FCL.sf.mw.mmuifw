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
* Description:   View Widget factory plugin header.
*
*/


#ifndef ALFVIEWWIDGETFACTORYPLUGIN_H
#define ALFVIEWWIDGETFACTORYPLUGIN_H

#include <alf/ialffactoryplugin.h>

namespace Alf
{

/**
 * AlfViewWidgetFactoryPlugin is a widget factory plugin implementation
 * that can be used to create view widgets.
 *
 * Can be used to create view widgets and associated products.
 */
class AlfViewWidgetFactoryPlugin : public IAlfFactoryPlugin
    {
public:
    // Constructors and destructor

    /**
     * Default Symbian constructor to create the factory plugin.
     */
    static AlfViewWidgetFactoryPlugin* newL();

    // From base class IAlfPluginFactory

    /**
     * Create view widget factory product.
     * @since Series 60 5.0
     * @param aProduct Product to create.
     * @param aInitData Initialization data for product
     * @return Instance or NULL
     */
    IAlfInterfaceBase* createProduct( const char* aProduct, void* aInitData );

    /**
     * Count available products this factory can produce.
     *
     * @since S60 5.0
     * @return Product count
     */
    int productCount() const;

    /**
     * Fetch product info by index
     *
     * @param aIndex List index
     * @since S60 5.0
     * @return Descriptor describing product
     */
    const char* productInfo(int aIndex) const;

    // From base class IAlfInterfaceBase

    /**
     * Creates interface based on the given type.
     *
     * @since Series 60 5.0
     * @param aType Interface type to create.
     * @return Requested interface.
     */
    IAlfInterfaceBase* makeInterface(const IfId& aType);

    };

    } // Namespace Alf

#endif // ALFVIEWWIDGETFACTORYPLUGIN_H

