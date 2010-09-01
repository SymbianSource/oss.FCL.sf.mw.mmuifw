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
* Description:  Widget factory interface
*
*/


#ifndef ALFWIDGETFACTORYPLUGIN_H
#define ALFWIDGETFACTORYPLUGIN_H

#include <ecom/implementationproxy.h>
#include <alf/ialffactoryplugin.h>
#include <osn/osndefines.h>

namespace Alf
    {
class AlfWidgetFactory;


/**
 *  @class AlfWidgetFactoryPlugin AlfWidgetFactoryPlugin.h "alf/alfwidgetfactoryplugin.h"
 *
 *  Plugin factory to create widget factory
 *
 *
 *  @lib alfwidgetfactory.dll
 *  @since S60 ?S60_version
 *  @status Draft  
 */
class AlfWidgetFactoryPlugin: public IAlfFactoryPlugin
    {
public:

    /**
     * Static Creator Method
     * @since S60 5.0
     * @return Interface pointer to factory plugin instance.
     */    
    static IAlfFactoryPlugin* NewL();

    /**
     * Destructor.
     */
    virtual ~AlfWidgetFactoryPlugin();
    
public: // from MAlfPluginFactory    
    IAlfInterfaceBase* createProduct(const char* aProduct,void* aInitData);
    int productCount()const;
    const char* productInfo(int aIndex)const; 
public:
    // from IAlfInterfaceBase
    virtual IAlfInterfaceBase* makeInterface(const IfId& aType);    
private:
    };
    } // namespace

// Exported factory function required by ECOM-framework
OSN_IMPORT const TImplementationProxy* ImplementationGroupProxy(
                                             TInt& aTableCount);
    
#endif 

// End of File
