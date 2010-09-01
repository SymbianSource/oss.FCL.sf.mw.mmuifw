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
* Description:  core functionality
*
*/

#include <stdlib.h>
#include <stdexcept>
#include <osn/osnnew.h> 

// ---------------------------------------------------------------------------
// Overloaded operator new.
// ---------------------------------------------------------------------------
//
OSN_EXPORT void* operator new(size_t s,newarg) throw (std::bad_alloc)
    {
    void* any = malloc(s);
    if(!any)
    	{
    	throw std::bad_alloc();	
    	}

  	return any;	
    }

// ---------------------------------------------------------------------------
// Overloaded operator delete.
// ---------------------------------------------------------------------------
//
OSN_EXPORT void operator delete(void* ptr,newarg) throw()
    {
    free(ptr);
    }

// ---------------------------------------------------------------------------
// Overloaded operator new[].
// ---------------------------------------------------------------------------
//
OSN_EXPORT void* operator new[] (size_t s,newarg)throw (std::bad_alloc)
	{
    void* any = malloc(s);
    if(!any)
    	{
    	throw std::bad_alloc();	
    	}
    	
   	return any;	
	}	

// ---------------------------------------------------------------------------
// Overloaded operator delete[].
// ---------------------------------------------------------------------------
//
OSN_EXPORT void operator delete[](void* ptr,newarg) throw()
	{
 	free(ptr);	
	}
