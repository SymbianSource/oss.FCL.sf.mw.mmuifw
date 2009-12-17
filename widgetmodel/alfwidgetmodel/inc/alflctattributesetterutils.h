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
* Description:  TThe attribute setter utility for lct attributesetters
*
*/


#ifndef ALFCTATTRIBUTESETTERUTILS_H
#define ALFCTATTRIBUTESETTERUTILS_H

#ifdef RD_ALF_IN_PLATFORM
#include <aknlayout2hierarchy.h>
#endif

namespace Alf
    {

class AlfAttributeContainer;
class IAlfMap;

/**
 *  The implementation of the attribute setter utility for lct attributesetters.
 *
 *  @lib alfwidgetmodel.lib
 *  @since S60 ?S60_version
 */
class AlfLctAttributeSetterUtils
    {

public:

    /**
     * Gets common lct attributes.
     *
     * @param aContainer The container holding the new value of the attribute.
     *        Ownership of the object is not transferred.
     * @ret Component handle
     */
    static TAknLayoutHierarchyComponentHandle getCommonLctAttributes(
        AlfAttributeContainer* aContainer);

    /**
     * Gets lct size.
     *
    * @param aComponentHandle Component handle
    * @param aWidth returned width.
    * @param aHeight returned height.
    * @return true if size is found.
     */
    static bool itemSize(const TAknLayoutHierarchyComponentHandle& aComponentHandle,
                         int& aWidth, int& aHeight);

private: // data
    };

    } // namespace Alf

#endif // ALFCTATTRIBUTESETTERUTILS_H

