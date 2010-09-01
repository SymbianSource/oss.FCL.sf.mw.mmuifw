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
* Description:  callback interface for sort.
*
*/


#ifndef I_ALFSORTFUNCTION
#define I_ALFSORTFUNCTION

// CLASS DECLARATIONS
namespace Alf
    {
    
class IAlfMap;

/**
 * callback- interface for sort.
 * 
 * @since S60 ?S60_version  
 * @status Draft
 */
class IAlfSortFunction
    {
public:
    /**
     * compares two leafs
     *
     * @param aFirst first parameter for comparison
     * @param aSecond second parameter for comparison
     * @return < 0, if aFirst is less than aSecond, > 0 if aFirst is more than aSecond,
                0, if aFirst equals aSecond.
     * @since S60 ?S60_version
     */ 
    virtual int compareLeafs( const IAlfMap* aFirst, const IAlfMap* aSecond ) const = 0;
    
    };
    
    }
#endif

// End of File
