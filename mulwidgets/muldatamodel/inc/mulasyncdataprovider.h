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
* Description:  Active Object header for providing data asynchronously
*
*/


#ifndef __MULASYNCDATAPROVIDER_H__
#define __MULASYNCDATAPROVIDER_H__

#include <e32base.h>    // For CActive
#include <mul/imulmodelprovider.h>
#include <mul/muldatapath.h>
#include <vector>

#include "muldatawindow.h"
#include "mulpagedatawindow.h"

using namespace std;
//Forward declarations


namespace Alf
	{
//Forward declarations

class MulAsyncData
    {
public:
    bool operator == (const MulAsyncData& aData )
        {
        return ( this->mStartIndex == aData.mStartIndex && this->mRange == aData.mRange );
        }
public:
    int mStartIndex;
	int mRange;
	MulDataPath mPath;
    };

class MulAsyncDataProvider : public CActive
	{
	
public : // Constructor and Destructor
    
    /// need to have two-phase construction
    /// - otherwise mixing leaves and throws
	MulAsyncDataProvider( IMulModelProvider& aDataProvider, const MulPageDataWindow& aDataWindow );
	
	~MulAsyncDataProvider();
	
    void ProvideData( int aStartIndex, int aRange, const MulDataPath& aPath, const bool aIsSync = false );
    
private: 
	
	/**
	 * Active call back funtion . 
	 */
	void RunL();
	
	void DoCancel();
	
	void ActivateAndComplete();
	
	int GetFirstIndexForWhichDataIsPresent(int aStartIndex, int aEndIndex);
	  
private: //data

	IMulModelProvider&             mDataProvider;
	vector<MulAsyncData>           mDataQueue;
	const MulPageDataWindow&       mDataWindow;
	}; //end of class.
	
	} // End of namespace .

#endif  //__MULASYNCDATAPROVIDER_H__

// End of file
