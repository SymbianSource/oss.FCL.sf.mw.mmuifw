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


#ifndef MULCALLBACK_H_
#define MULCALLBACK_H_

#include <vector>

namespace Alf
	{
	
class MulCallback
	{
public:
	
	MulCallback()
		{
		}

	MulCallback( void (*aFunction) ( void* aObject, std::vector<int>& aArgument ), void* aObject )
		{
		mFunction = aFunction;
		mObject = aObject;
		}
	
	void Callback(  std::vector<int>& aArgument ) const
		{
		mFunction( mObject, aArgument );
		}
	
private:
	
	void (*mFunction) ( void* aObject,  std::vector<int>& aArgument );
	
	void* mObject;
		
	};

	}//namespace Alf

#endif //MULCALLBACK_H_