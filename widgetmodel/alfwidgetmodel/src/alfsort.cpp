/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  sorting routine. uses Symbian services: User::QuickSort
*
*/


//INCLUDES
#include <e32std.h>
#include <alf/ialfmap.h>
#include <alf/alfvarianttype.h>
#include <alf/ialfsortfunction.h>
//#include "alf/alfperf.h"
#include "alfsort.h"

using namespace Alf;

/**
*  Defines the characteristics of a key used
*  to access the elements of an array. Used for sort.
*  @since S60 ?S60_version
*/
NONSHARABLE_CLASS(AlfSortKey) : public TKey
    {
public:

    /**
     * constructor
     *
     * @since S60 ?S60_version
     * @param aSortFunction callback interface for sort.
     * @param aArr array to sort.
     * @param aCount. count of items in the array.
     */
    AlfSortKey( const IAlfSortFunction& aSortFunction,
    IAlfVariantType** aArr, uint aCount );

    /**
     * returns data from array
     *
     * @since S60 ?S60_version
     * @param anIndex index to element to return
     * @return element at index anIndex.
     */
    TAny *At(TInt anIndex) const;

    /**
     * compares two items.
     *
     * @since S60 ?S60_version
     * @param aLeft index to the array for left parameter.
     * @return the value returned from sort function:
     *  < 0, if aFirst is less than aSecond,
     *  > 0 if aFirst is more than aSecond,
     *  0, if aFirst equals aSecond.
     */
    TInt Compare(TInt aLeft, TInt aRight) const;
private:

    //sort function not owned.
    const IAlfSortFunction& iSortFunction;

    //array to sort. not owned
    IAlfVariantType** iArr;

    //count of items in array.
    uint iCount;
    };

/**
*  Defines the basic behaviour for swapping two elements of an array.
*  Used for sort
*  @since S60 ?S60_version
*/
NONSHARABLE_CLASS(AlfSortSwap) : public TSwap
    {
public:

    /**
     * constructor
     *
     * @since S60 ?S60_version
     * @param aArr array, which elements are to be swapped.
     * @param aCount. count of items in the array.
     */
    AlfSortSwap( IAlfVariantType** aArr, uint aCount );

    /**
     * swaps two items in the array.
     *
     * @since S60 ?S60_version
     * @param aLeft  left index
     * @param aRight right index.
     */
    void Swap(TInt aLeft, TInt aRight) const;
private:

    //array, which elements are to be swapped. Not owned.
    IAlfVariantType** iArr;

    //count of items in the array.
    uint iCount;
    };

// ---------------------------------------------------------------------------
// Description : constructor
// ---------------------------------------------------------------------------
//
AlfSortKey::AlfSortKey( const IAlfSortFunction& aSortFunction,
                        IAlfVariantType** aArr, uint aCount ) :
        iSortFunction(aSortFunction), iArr(aArr), iCount(aCount)
    {
    }

// ---------------------------------------------------------------------------
// Description : return element from index anIndex.
// ---------------------------------------------------------------------------
//
TAny *AlfSortKey::At(TInt anIndex) const
    {
    return iArr[anIndex];
    }

// ---------------------------------------------------------------------------
// Description : compares two items.
// ---------------------------------------------------------------------------
//
TInt AlfSortKey::Compare(TInt aLeft, TInt aRight) const
    {
    const IAlfMap* leftMap = iArr[aLeft]->map();
    const IAlfMap* rightMap = iArr[aRight]->map();
    return iSortFunction.compareLeafs( leftMap, rightMap );
    }

// ---------------------------------------------------------------------------
// Description : constructor
// ---------------------------------------------------------------------------
//
AlfSortSwap::AlfSortSwap( IAlfVariantType** aArr, uint aCount ) :
        iArr(aArr), iCount(aCount)
    {
    }

// ---------------------------------------------------------------------------
// Description : swaps two items
// ---------------------------------------------------------------------------
//
void AlfSortSwap::Swap(TInt aLeft, TInt aRight) const
    {
    IAlfVariantType* tmp = iArr[aLeft];
    iArr[aLeft] = iArr[aRight];
    iArr[aRight] = tmp;
    }

// ---------------------------------------------------------------------------
// Description : The sorting routine.
// ---------------------------------------------------------------------------
//
void AlfSort::sort(IAlfVariantType** aArr, uint aCount,
                   const IAlfSortFunction& aSortFunction )
    {
    //ALF_PERF_START( perfdata, "AlfSort-Sort-Sorting")
    AlfSortKey key(aSortFunction, aArr, aCount);
    AlfSortSwap swap(aArr, aCount);
    User::QuickSort( aCount, key, swap );
    //ALF_PERF_STOP( perfdata, "AlfSort-Sort-Sorting")
    }

