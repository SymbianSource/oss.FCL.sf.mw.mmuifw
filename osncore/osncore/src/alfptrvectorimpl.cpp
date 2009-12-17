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
* Description: osn vector pointer implementation.
*
*/

#if defined(__GNUC__)
#include <stdlib.h>
#include <string.h>
#else
#include <libc/string.h>
#endif

#include <osn/alfptrvectorimpl.h>
#include <stdexcept>

namespace osncore
    {

// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
//
OSN_EXPORT AlfPtrVectorImpl::AlfPtrVectorImpl(IDeleter& aDeleter)
    : mdata(0), msize(0), mcount(0), mDeleter(aDeleter)
    {
    }

// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
//
OSN_EXPORT AlfPtrVectorImpl::AlfPtrVectorImpl(uint aSize, IDeleter& aDeleter): 
    mdata(0),
    msize(aSize), 
    mcount(0), mDeleter(aDeleter)
    {
    mdata = (void **)malloc(aSize * sizeof(void *));
    if(!mdata)
        {
        throw std::bad_alloc();    
        }
    memset(mdata, 0, aSize * sizeof(void *));
    }


// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
//
OSN_EXPORT AlfPtrVectorImpl::~AlfPtrVectorImpl()
{
    free(mdata);
}

// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
//
OSN_EXPORT void AlfPtrVectorImpl::clear(bool aDelItems)
    {
    mcount = 0;
    
    if (aDelItems) 
        {
    	for (uint i = 0; i < msize; ++i) 
    	    {
            void *item = mdata[i];
    	    if (item) 
    	        {
    		    mDeleter.deleteItem(item);
    	        }
    	}
	}
    free(mdata);
    mdata = 0;
    msize = 0;    
    }

// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
//
OSN_EXPORT bool AlfPtrVectorImpl::remove(uint aCount, bool aDelItems)
{
    if (aCount >= msize) {
        return false;
    }
    
    void *item = mdata[aCount];

    --mcount;

    //Move all the items below the deleted items up by 1 index
    for(uint i =aCount; i< msize && msize > aCount;++i)
    {
    	mdata[i]= mdata[i+1];
    }

    //Reset the items to null - from index = count to the size  after the shuffling.
    for(uint i=mcount;i<msize;++i)
    {
    	mdata[i] = 0;
    }

    // Array is now in good shape. Can call out of the class via destructors
    if (item) {
        if (aDelItems) {
            mDeleter.deleteItem(item);
        }
    }     
    return true;
}

// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
//
OSN_EXPORT bool AlfPtrVectorImpl::resize(uint aSize, bool aDelItems)
{
    uint oldSize = msize;
    
    for (uint i = aSize; i < oldSize; ++i) {
        void *item = mdata[i];
        if (item) {
            --mcount;
        }
    }
    
    for (uint i = aSize; i < oldSize; ++i) {
    	void *item = mdata[i];
    	if (item) {
        if (aDelItems) {
           mDeleter.deleteItem(item);
        	}
    	}
    }
    
    void** data = (void **)realloc(mdata, aSize * sizeof(void *));
    if(!data)
        {
        throw std::bad_alloc();    
        }
    mdata = data;    
    msize = aSize;    

    if (aSize > oldSize) 
    {
        memset(&mdata[oldSize], 0, (aSize - oldSize) * sizeof(void *));
    }
    return true;
}

// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
//
OSN_EXPORT bool AlfPtrVectorImpl::insert(uint aCount, void *aItem, bool /*aDelItems*/)
{
	uint iNum=0;
	//If there are no items and  the position to add > 0 or <0 then  return false. Do not do anything
	//if the Index is greater than the current count then Do not do anything
	//if aCount== count There will be scope to add at the most one item
	if((aCount > mcount) || (mcount==0 && aCount >iNum)||(mcount==0 && aCount < iNum))
           {
    	   return false;
           } 

	
	if(mcount>=msize)
    	{
		// 1. Let's increase the count
		int oldSize = msize;	
	    msize++;
	
		// 2. Re-allocate memeory area 	
    	void** tmp = (void **)realloc(mdata, msize * sizeof(void *));
	    if(!tmp)
	        {
	        msize--;
	        throw std::bad_alloc();    
	        }
	    mdata = tmp;    
	   	memset(&mdata[oldSize], 0, (msize- oldSize) * sizeof(void *));
        }

	    //Now move	elements from nth element to the last element by 1 position
	    //Move all
	    if(aCount < mcount)
	        { 
		    for(int i =mcount;i > aCount; --i)
		        {
		    	mdata[i]= mdata[i-1];
		        }    
		    }
	    //Now assign the new element at this position and increment the count	    	    
	    
	    mdata[aCount] = aItem;
	    if (aItem) 
	    {
	        ++mcount;
	    }
	        
    return true;
}

// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
//
OSN_EXPORT int AlfPtrVectorImpl::findRef(void *aItem)
{
    for (unsigned i = 0; i < mcount; i++) {
        if (mdata[i] == aItem) {
            return i;
        }
    }
    
    return -1;
}



} //osncore
