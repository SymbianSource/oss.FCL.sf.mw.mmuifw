/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Ecom plugin Factory header.
*
*/


#ifndef C_ALFECOMPLUGINFACTORY_H
#define C_ALFECOMPLUGINFACTORY_H

#include <e32base.h>
#include <ecom/implementationinformation.h> // for typedeffed RImplInfoPtrArray
#include <alf/ialfinterfacebase.h>

namespace Alf
    {

/**
 *  @class CAlfEComPluginFactory alfecompluginfactory.h "alf/alfecompluginfactory.h"
 *  Base class for Ecom plugins. Takes care of ecom housekeeping.
 *
 *  @lib alfwidgetutils.lib
 *  @since S60 ?S60_version
 *  @status Draft
 */
class CAlfEComPluginFactory : public CBase, public IAlfInterfaceBase
    {
public:  // Constructors and destructor

    /**
    * Factory method to create plugin for given interface
    * @param aInterfaceUid UID of interface being queried/created.
    * @param aImplKey Implementation Key
    * @return An ecom plugin factory.
    */
    IMPORT_C static CAlfEComPluginFactory* createPluginFactory(
                        TUid aInterfaceUid,const char* aImplKey);

    /**
    * Destructor.
    */
    IMPORT_C virtual ~CAlfEComPluginFactory();
    
public:
    /**
     * List available interface implementations.
     * @since Series 60 3.2
     * @param aInterfaceUid: Interface Uid
     * @param aImplInfoArray: Place holder which gets populated 
     *                        after function call
     */
    IMPORT_C static void getImplementationsL(
                             TUid aInterfaceUid,
                             RImplInfoPtrArray& aImplInfoArray);

    /**
     * List available interface implementations.
     * @since Series 60 3.2
     * @param aInterfaceUid: Interface Uid
     * @param aImplKey Implementation Key
     * @param aImplInfoArray: Place holder which gets populated after 
     *                        function call
     */
    IMPORT_C static void getImplementationsL(TUid aInterfaceUid,
            const char* aImplKey,
            RImplInfoPtrArray& aImplInfoArray);

protected:

    /**
    * C++ default constructor.
    */
    IMPORT_C CAlfEComPluginFactory();

private:    // Data
    // Ecom instance release key
    TUid mDtorKey;
    };
    }

#endif

// End of File
