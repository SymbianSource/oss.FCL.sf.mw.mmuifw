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
* Description:  Implementation of the common string pool for widget and the factory model class.
*
*/

#include <stdlib.h>


#include "osn/alfstringpool.h"
#include "osn/alfstring.h"


const int KPoolAllocSize = 32;

namespace osncore
    {
class CAlfStringPoolImpl
    {
public:    
    CAlfStringPoolImpl():iPool(0),iItemCount(0),iAllocCount(0)
        {
            
        }
    ~CAlfStringPoolImpl()
        {
        delete [] iPool;    
        }
    CAlfString* iPool;
    int iItemCount;
    int iAllocCount;
    };    
  

//-----------------------------------------------------------------------
// Only Method to access Strings from the String pool
//-----------------------------------------------------------------------
static CAlfString AppendToPool( 
    const CAlfString& aString, 
    CAlfStringPoolImpl*& aImpl)
    {
    bool found(false);
    CAlfString ret(aString);
    
    //Look if this string is present then just return the corresponding RAlfString else create new one and add it to the list
    for(int i=0;i<aImpl->iItemCount && !found;i++)
        {
        if( aImpl->iPool[i] == aString) //element found
            {
            found = true;
            ret = aImpl->iPool[i];
            }
        }
    if(!found)
        {
        if(aImpl->iItemCount < aImpl->iAllocCount )
            {
            aImpl->iPool[aImpl->iItemCount++] = aString;
            }
        else // // reserve more space for an array
            {
            CAlfString* tmp = (CAlfString*) realloc(aImpl->iPool, (sizeof (CAlfString) * KPoolAllocSize) );
            if(tmp)
                {
                aImpl->iAllocCount += KPoolAllocSize;
                aImpl->iPool = tmp;
                
                aImpl->iPool[aImpl->iItemCount++] = aString;     
                }
            else
                {
                User::LeaveIfNull(tmp);   
                }    
            }   
        }
    return ret;
    }



EXPORT_C  CAlfString CAlfStringPool::CreateStringL( const char* aString)
    {
    if(aString)
        {
        CAlfString tmp(aString);
        return AppendToPool(tmp,iImpl);    
        }
    return CAlfString();
    }

EXPORT_C  CAlfString CAlfStringPool::CreateStringL( const CAlfString& aString)
    {
    return AppendToPool(aString,iImpl);
    }

EXPORT_C CAlfStringPool::CAlfStringPool():iImpl(0)
    {
    iImpl = new(ELeave) CAlfStringPoolImpl;
    iImpl->iPool = new(ELeave)CAlfString[KPoolAllocSize];
    iImpl->iAllocCount = KPoolAllocSize;
    
    }

//-----------------------------------------------------------------------
// Default Destructor
//-----------------------------------------------------------------------
EXPORT_C CAlfStringPool::~CAlfStringPool()
    {
    
    delete iImpl;
    iImpl = NULL;
    }


  }
