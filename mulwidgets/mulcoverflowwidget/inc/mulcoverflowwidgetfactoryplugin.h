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
* Description:  factory plugin header for CoverFlow
*
*/

#ifndef MULCOVERFLOWWIDGETFACTORYPLUGIN_H
#define MULCOVERFLOWWIDGETFACTORYPLUGIN_H


#include <ecom/implementationproxy.h>
#include <alf/ialffactoryplugin.h>
#include <osn/osndefines.h>


namespace Alf
    {
class IfId;

/**
 *  Plugin factory to create widget factory
 *  @lib alfwidgetfactory.dll
 */
/// @bug comment:avanhata:3/7/2008 This class is only used from its own cpp file,
/// so the definition can be moved into the cpp and the header file removed.
class MulCoverFlowWidgetFactoryPlugin: public IAlfFactoryPlugin
    {
public:
    /**
    * Two-phased symbian constructor.
    * 
    * @return New instance of this factory plugin.
    */ 
    static MulCoverFlowWidgetFactoryPlugin* NewL();
    
    /**
     * Destructor.
     */
    virtual ~MulCoverFlowWidgetFactoryPlugin();
    
public: // from MAlfPluginFactory    

    IAlfInterfaceBase* createProduct(const char* aProduct,void* aInitData);
    
    int productCount()const;
    
    const char* productInfo(int aIndex)const;
    
    IAlfInterfaceBase* makeInterface(const IfId& aType);
    
    };
    
    } // namespace Alf
    
/// @bug comment:avanhata:3/7/2008 does not need to be declared - just define in cpp
// Exported factory function required by ECOM-framework
IMPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount);
    
#endif //MULCOVERFLOWWIDGETFACTORYPLUGIN_H

//End of file