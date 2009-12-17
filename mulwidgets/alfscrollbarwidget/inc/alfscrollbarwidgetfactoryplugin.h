/*
* Copyright (c) 2007-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Scroollbar Widget factory plugin header
*
*/

#ifndef ALF_SCROLLBARWIDGETFACTORYPLUGIN_H
#define ALF_SCROLLBARWIDGETFACTORYPLUGIN_H

// Includes
#include <ecom/implementationproxy.h>
#include <alf/ialffactoryplugin.h>
#include <osn/osndefines.h>

namespace Alf
    {

class IfId;

    /**
     *  Plugin factory to create widget factory
     *
     *  @lib alfwidgetfactory.dll
     *  @since S60 5.0
     */
class AlfScrollBarWidgetFactoryPlugin: public IAlfFactoryPlugin
    {

public:

    /**
     * Two-phased Symbian constructor.
     */
    static AlfScrollBarWidgetFactoryPlugin* NewL();

    /**
     * Destructor 
     */
    virtual ~AlfScrollBarWidgetFactoryPlugin();

    // from IAlfFactoryPlugin

    /**
     * Create factory product.
     * @since Series 60 5.0
     * @param aProduct Product to create.
     * @param aInitData Initialization data for product
     * @return Instance or NULL
     */
    IAlfInterfaceBase* createProduct ( const char* aProduct, void* aInitData );

    /**
     * Count available products
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
    const char* productInfo ( int aIndex ) const;

    /**
     * From IAlfInterfaceBase.
     * Creates interface based on the given type.
     */
    IAlfInterfaceBase* makeInterface ( const IfId& aType );
    };

    } // namespace Alf

    /**
     * Exported factory function required by ECOM-framework
     */
    OSN_IMPORT const TImplementationProxy* ImplementationGroupProxy( 
        TInt& aTableCount );

#endif //ALF_SCROLLBARWIDGETFACTORYPLUGIN_H
//End Of File
