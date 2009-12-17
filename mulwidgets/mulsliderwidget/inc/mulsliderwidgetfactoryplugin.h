/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Factory plugin for slider widget.
*
*/

#ifndef MUL_SLIDERWIDGETFACTORYPLUGIN_H
#define MUL_SLIDERWIDGETFACTORYPLUGIN_H

// Includes
#include <ecom/implementationproxy.h>
#include <alf/ialffactoryplugin.h>
#include <osn/osndefines.h>
#include "mulslidervertical.h"

enum ESliderProductInfo
    {
    ESliderWidget,
    
    ESliderModel,
      
    ESliderHorizontalElement,
       
    ESliderVerticalElement,
     
    ESliderProgressbarElement
    };
namespace Alf
    {

class IfId;

    /**
     *  Plugin factory to create widget factory
     *
     *  @lib alfwidgetfactory.dll
     *  @since S60 5.0
     */
class MulSliderWidgetFactoryPlugin: public IAlfFactoryPlugin
    {

public:

    /**
     * Two-phased Symbian constructor.
     */
    static MulSliderWidgetFactoryPlugin* NewL();

    /**
     * Destructor 
     */
    virtual ~MulSliderWidgetFactoryPlugin();

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

#endif //MUL_SLIDERWIDGETFACTORYPLUGIN_H
//End Of File
