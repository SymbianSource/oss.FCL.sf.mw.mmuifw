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
* Description:  Implementation for Assert Macros
*
*/


#ifndef MULASSERT_H_
#define MULASSERT_H_

#include <stdexcept>
#include <e32std.h>

/**
 * Assert macro.
 * It checks expression if expression is evaluated false then panic with specified code.
 * 
 * @param x Result after expression is evaluated
 * @param y Error code for Assertion fail
 */ 
#define __MUL_ASSERT(x,y) __ASSERT_ALWAYS(x,User::Panic(y,0))

	

/**
 * Assert Macro for debug version.
 * It checks expression if expression is evaluated false then panic with specified code.
 * 
 * @param x bool result after expression is evaluated
 * @param y Error code for Assertion fail
 */
#define __MUL_ASSERT_DEBUG(x,y) __ASSERT_DEBUG(x,User::Panic(y,0))
	
namespace Alf
   {
   
/**
* Assert function.
* It takes tamplate argument that is used kind of expceton that need to be thrown.
* It checks expression if expression is evaluated false then it throws logic exception.
* 
* @param aExpression bool result after expression is evaluated
* @param aExceptionString description of error
*/
template<class X> 
static void MUL_ASSERT(bool aExpression, const char* aExceptionString)
	{
   	if( !aExpression )
   		{
   		throw X(aExceptionString);
 		}
	}
   

	
/**
 * Assert function for debug version.
 * It takes tamplate argument that is used kind of expceton that need to be thrown.
 * It checks expression if expression is evaluated false then it throws logic exception.
 * 
 * @param aExpression bool result after expression is evaluated
 * @param aExceptionString description of error
 */
template<class X> 
static void MUL_ASSERT_DEBUG(bool aExpression, const char* aExceptionString)
	{
#ifdef _DEBUG
	
	if( !aExpression )
		{
		throw X( aExceptionString );
		}
	
#endif // _DEBUG
	}



   } // namespace Alf

#endif  // MULASSERT_H_

// End of file

