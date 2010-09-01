/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  contains sorting routine
*
*/



#ifndef ALFSORT_H
#define ALFSORT_H

//  INCLUDES
#include <osn/osntypes.h>

namespace Alf
    {

class IAlfVariantType;
class IAlfSortFunction;
    }

namespace Alf
    {
/**
*  class containig sort routine.
*  @since S60 ?S60_version
*/
class AlfSort
    {
public:
    /**
     * sorts the array
     *
     * @since S60 ?S60_version
     * @param aArr array to sort
     * @param aCount count of items in aArr.
     * @param aSortFunction. Callback interface, specifies how the items are sorted.
     */
    static void sort(IAlfVariantType** aArr, uint aCount, const IAlfSortFunction& aSortFunction );
    };

    }


#endif
