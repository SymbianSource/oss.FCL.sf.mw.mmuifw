/*
* Copyright (c) 2007-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Plugin factory header for model.
*
*/


#ifndef MULMODELFACTORYPLUGIN_H
#define MULMODELFACTORYPLUGIN_H

#include <ecom/implementationproxy.h>
#include <alf/ialffactoryplugin.h>

namespace Alf
    {

class IfId;

/**
 * Factory plugin for creating list widget related products.
 */
class MulFactoryPlugin: public IAlfFactoryPlugin
    {
public:

   /**
    * Two-phased symbian constructor.
    * 
    * @return New instance of this factory plugin.
    */ 
    static MulFactoryPlugin* NewL();

   /**
    * Destructor.
    */ 
    virtual ~MulFactoryPlugin();
    
// From base class MAlfPluginFactory

   /**
    * Create factory product.
	*
    * @param aProduct Product to create.
    * @param aInitData Initialization data for product
    * @return Instance or NULL
    */ 
    IAlfInterfaceBase* createProduct( const char* aProduct, void* aInitData );

   /**
    * Count available products
    *
    * @return Product count
    */
    int productCount() const;

   /**
    * Fetch product info by index
    *
    * @param aIndex List index
    * @return Descriptor describing product
    */
    const char* productInfo( int aIndex ) const;
    
// From base class MAlfInterfaceBase

   /**
    * Creates interface based on the given type.
    * 
    * @param aType Interface type to create.
    * @return Requested interface.
    */ 
    IAlfInterfaceBase* makeInterface( const IfId& aType );
    };
    
    } // namespace Alf

// Exported factory function required by ECOM-framework
IMPORT_C const TImplementationProxy* ImplementationGroupProxy( TInt& aTableCount );
    
#endif //MULMODELFACTORYPLUGIN_H

//End of file

