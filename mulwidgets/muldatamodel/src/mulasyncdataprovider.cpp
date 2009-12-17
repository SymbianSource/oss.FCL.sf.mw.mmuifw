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
* Description:  Active Object Implementation for providing data asynchronously
*
*/


#include "mulasyncdataprovider.h"

#include <osn/osnnew.h>
#include <algorithm> //for find algorithm

#include "mullog.h"

namespace Alf
    {

// ---------------------------------------------------------------------------
// MulAsyncDataProvider
// ---------------------------------------------------------------------------
//
MulAsyncDataProvider::MulAsyncDataProvider( IMulModelProvider& aDataProvider,
                                            const MulPageDataWindow& aDataWindow )
                     :CActive( EPriorityNormal ),
                      mDataProvider(aDataProvider),
                      mDataWindow(aDataWindow)
    {
    MUL_LOG_ENTRY_EXIT("MUL::MulAsyncDataProvider::MulAsyncDataProvider");
    CActiveScheduler::Add(this);
    }

// ---------------------------------------------------------------------------
// ~MulAsyncDataProvider
// ---------------------------------------------------------------------------
//
MulAsyncDataProvider::~MulAsyncDataProvider()
    {
    MUL_LOG_ENTRY_EXIT("MUL::MulAsyncDataProvider::~MulAsyncDataProvider");  
    Cancel();
    mDataQueue.clear();
    }

// ---------------------------------------------------------------------------
// RunL
// ---------------------------------------------------------------------------
//
void MulAsyncDataProvider::RunL()
    {  
    vector<MulAsyncData>::iterator iter = mDataQueue.begin();
    if( iter < mDataQueue.end() )
        {
        bool inDataWindow = false;
        MulAsyncData asyncData = *iter;
        
        if(mDataWindow.IsItemInDataWindow(asyncData.mStartIndex) /*&& mDataWindow->IsItemInDataWindow(mStartIndex+ITEMS)*/)
            { 
            if( asyncData.mRange < mDataWindow.WindowSize() )
                {
                mDataProvider.ProvideData( asyncData.mStartIndex, asyncData.mRange, asyncData.mPath); 
                }
            else
                {
                mDataProvider.ProvideData( asyncData.mStartIndex, mDataWindow.WindowSize() , asyncData.mPath); 
                }
            inDataWindow = true;
            }
                  
        asyncData.mStartIndex += mDataWindow.WindowSize();
        asyncData.mRange -= mDataWindow.WindowSize();
        mDataQueue.erase(iter);
        
       if( asyncData.mRange > 0 && inDataWindow )
            {     
            mDataQueue.insert( iter, asyncData ); 
            if( !IsActive() )
                {
                ActivateAndComplete();
                }
            }
         else if(mDataQueue.begin() < mDataQueue.end())
            {  
            if( !IsActive() )
                {
                ActivateAndComplete();
                }
            }
        }     
    }

// ---------------------------------------------------------------------------
// DoCancel
// ---------------------------------------------------------------------------
//
void MulAsyncDataProvider::DoCancel()
    {
    //does this function required??
    }
//    
// ---------------------------------------------------------------------------
// ProvideData
// ---------------------------------------------------------------------------
//
void MulAsyncDataProvider::ProvideData( int aStartIndex, int aRange, const MulDataPath& aPath, const bool aIsSync )
    { 
    if( aIsSync )
        {
        mDataProvider.ProvideData( aStartIndex, aRange, aPath);
        return;
        }
    MulAsyncData asyncData;
    asyncData.mStartIndex = aStartIndex;
    asyncData.mRange = aRange;
    asyncData.mPath = aPath;
    if( std::find( mDataQueue.begin(), mDataQueue.end(), asyncData ) ==  mDataQueue.end())
        {
        mDataQueue.push_back( asyncData );
        }
                 
    for( int i = 0 ; i < mDataQueue.size() ; ++i )
        {
        MulAsyncData data = mDataQueue[i]; 
        if ( (mDataWindow.IsItemInDataWindow( data.mStartIndex ) &&
              !mDataWindow.IsItemInDataWindow( data.mStartIndex + data.mRange - 1 ) ) )
            {
            int index = GetFirstIndexForWhichDataIsPresent(data.mStartIndex , data.mStartIndex + data.mRange - 1 );
            if( index!= -1 &&( index - data.mStartIndex + 1) > 0)
                {
                mDataQueue.erase(mDataQueue.begin() + i);
                data.mRange = index - data.mStartIndex + 1;
                mDataQueue.insert(mDataQueue.begin() + i, data);
                }
            else
                {
                mDataQueue.erase(mDataQueue.begin() + i);
                i--;
                }    
            
            }
        else if ( (!mDataWindow.IsItemInDataWindow( data.mStartIndex ) &&
              mDataWindow.IsItemInDataWindow( data.mStartIndex + data.mRange - 1 ) ) )
            {
            int index = GetFirstIndexForWhichDataIsPresent(data.mStartIndex , data.mStartIndex + data.mRange - 1 );
            if( index!= -1 &&( data.mStartIndex + data.mRange - index ) > 0 )
                {
                mDataQueue.erase(mDataQueue.begin() + i);
                int endIndex = (data.mStartIndex + data.mRange - 1);
                data.mStartIndex = index;
                data.mRange = (endIndex - index + 1);
                mDataQueue.insert(mDataQueue.begin() + i, data);
                }
            else
                {
                mDataQueue.erase(mDataQueue.begin() + i);
                i--;
                }
            }
        else if ( (!mDataWindow.IsItemInDataWindow( data.mStartIndex ) &&
              !mDataWindow.IsItemInDataWindow( data.mStartIndex + data.mRange - 1 ) ) )
           {
           mDataQueue.erase(mDataQueue.begin() + i);
           i--;
           MUL_LOG_INFO2( "MUL::MulAsyncDataProvider::ProvideData Removing From Q: startIndx=%d,range=%d", data.mStartIndex, data.mRange ); 
           }
        }
    
    if( !IsActive() )
        { 
        ActivateAndComplete();
        }
    }
// ---------------------------------------------------------------------------
// GetFirstIndexForWhichDataIsPresent
// ---------------------------------------------------------------------------
//
int MulAsyncDataProvider::GetFirstIndexForWhichDataIsPresent(int aStartIndex, int aEndIndex)
    {
    if ( aStartIndex > aEndIndex )
        {
        return -1;
        }
    int mid = (aStartIndex + aEndIndex)/2;
    if(mDataWindow.IsItemInDataWindow(aStartIndex))
        {
        if(mid == aStartIndex)
            {
            return mid;
            }
        if(mDataWindow.IsItemInDataWindow(mid))
            {
            return GetFirstIndexForWhichDataIsPresent(mid, aEndIndex);
            }
        else
            {
            return GetFirstIndexForWhichDataIsPresent(aStartIndex, mid);
            }
        }
     else if(mDataWindow.IsItemInDataWindow(aEndIndex))
        {
        if(mid == aStartIndex)
            {
            return (mid + 1);
            }
        if(mDataWindow.IsItemInDataWindow(mid))
            {
            return GetFirstIndexForWhichDataIsPresent(aStartIndex, mid);
            }
        else
            {
            return GetFirstIndexForWhichDataIsPresent(mid, aEndIndex);
            }           
        }   
    return -1;
    }
  
// ---------------------------------------------------------------------------
// ActivateAndComplete
// ---------------------------------------------------------------------------
//
 void MulAsyncDataProvider::ActivateAndComplete()
    {
    TRequestStatus* lStatus = &iStatus;
    iStatus = KRequestPending;
    SetActive();
    User::RequestComplete(lStatus, KErrNone);
    }
 
    } // End of namespace Alf .


// End of file
