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
* Description:  Active object for performing Async filtering
*
*/


#ifndef MULFILTERAO_H_
#define MULFILTERAO_H_

#include <e32base.h>

#include <alf/alfvarianttype.h>
#include <osn/osndefines.h>

#include "mulcallback.h"


namespace osncore
    {
    template <class T> class CAlfPtrVector;  
    class UString;
    }  
using namespace osncore;

namespace Alf
	{
//Forward Declaration
	 
class MulCacheManager;
class MulDataWindow;
class MulFilter;
class IMulFilterDataProvider;
class MulCallback;
 	  
class MulFilterAo : public CActive
    {
    
public: //constructor and destructor

	/**
	 * symbian constcutor
	 * 
	 * @param aModel Model refrence
	 */
	//static MulFilterAo* NewL( IMulFilterDataProvider& aModel );
    
    /**
     * Destructor
     */
    ~MulFilterAo();
    
public: //constructor and destructor

	/**
	 * C++ constcutor
	 * 
	 * @param aModel Model refrence
	 */
	MulFilterAo(IMulFilterDataProvider& aModel );   
	
	/**
	 * symbian Second phase constructor
	 */
	//void ConstructL();
    
private: //from CActive

	void RunL();
	
	void DoCancel();

public: //new method

    /**
     * Perform Async filtering. Model call this function when SetFilter is called on Model.
     * After performing async filtering  function will notify model via callback object.
     * 
     * @param aFilter Filter object
     * @param aCallback Callback object
     */
	void DoAsyncFilter(MulFilter& aFilter,MulCallback aCallback);

private:
    
    /**
     * Perform garbage collection operation
     */
    void DoFilter();
    
private: //data

	IMulFilterDataProvider& 		mMulModel;
	MulFilter*						mFilter; //not own
	MulCallback						mCallback;
    };
	    
    } //namespace Alf

#endif /*MULFILTERAO_H_*/

//End of file


