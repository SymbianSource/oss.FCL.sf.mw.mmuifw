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
* Description:  operator new overloads
 *
*/



#ifndef OSNNEW_H
#define OSNNEW_H

#ifdef __GNUC__
#include <stdexcept>
#endif
#include <osn/osndefines.h>

#ifndef __GNUC__
namespace std
    {
    class bad_alloc;
    }
#endif
enum newarg {EMM};

typedef unsigned int size_t;

/**
 * Overloaded operator new.
 *
 * @since S60 5.0
 * @param s size of the allocation
 * @return void* pointer to allocated object.
 */
OSN_IMPORT void* operator new(size_t s,newarg) throw (std::bad_alloc);

/**
 * Overloaded operator delete.
 *
 * @since S60 5.0
 * @param ptr pointer to the object to be deleted
 */
OSN_IMPORT void operator delete(void* ptr,newarg)throw();

/**
 * Overloaded operator new[].
 *
 * @since S60 5.0
 * @param s size of the allocation
 * @return void* pointer to allocated object.
 */
OSN_IMPORT void* operator new[] (size_t s,newarg) throw (std::bad_alloc);

/**
 * Overloaded operator delete[].
 *
 * @since S60 5.0
 * @param ptr pointer to the object to be deleted
 */
OSN_IMPORT void operator delete[](void* ptr,newarg)throw();


#endif // OSNNEW_H
