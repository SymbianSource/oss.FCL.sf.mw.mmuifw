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
* Description: Osn String Implementation.
*
*/

#if defined(__GNUC__)
#include <string.h>
#include <assert.h>
#else
#include <e32base.h>
#include <libc/string.h>
#include <libc/assert.h>
#endif

#include <stdio.h>
#include <stdarg.h>
#include "osn/alfstring.h"
#include "stringmacros.h"
//#include "char.h"
#include "alfstringdata.h"
//#include "CHARCONV.H"
#include <osn/osnnew.h>
#include <osn/osnbadalloc.h>
#define CHECK_FOR_HANDLE_LEAKS 0

using namespace std;
namespace osncore
    {




#define IS_ASCII_QCHAR(c) ((c).unicode() > 0 && (c).unicode() <= 0xff)




EXPORT_C CAlfString::CAlfString()
    :internalData(new(S60)SAlfStringData())
    {
    }



EXPORT_C CAlfString::~CAlfString()
    {

    if(internalData.get())
        {
        internalData->deref();
        if(internalData->refCount != 0)
            {
            internalData.release(); // don't kill it, we have ref
            }
        }
    }




EXPORT_C CAlfString::CAlfString(const char *chs)
    :internalData(new(S60)SAlfStringData())
    {
    internalData->initialize(chs, strlen(chs));

    }



EXPORT_C CAlfString::CAlfString(const CAlfString &qs)
    :internalData(qs.internalData.get())
    {
    internalData->ref();  
    }

EXPORT_C CAlfString &CAlfString::operator=(const CAlfString &qs)
    {
    if (this == &qs)
        return *this;
    qs.internalData->ref(); // increase source ref count
    internalData->deref();
    if(internalData->refCount != 0)
        {
        internalData.release();
        }
        
    internalData.reset(qs.internalData.get()); 
    return *this;
    }

EXPORT_C  const char* CAlfString::latin1() const
{
	return internalData->ascii();
}



EXPORT_C CAlfString::CAlfString(const char *chs, int len)
    : internalData(new(S60)SAlfStringData())
    {

    internalData->initialize(chs, len);
 
    }

EXPORT_C int CAlfString::compare(const CAlfString& s) const
{
    //if (internalData->_isAsciiValid && s.internalData->_isAsciiValid)
        return strcmp(latin1(), s.latin1());
    //return ucstrcmp(*this,s);
    // return -1;
}


EXPORT_C CAlfString& CAlfString::append(const CAlfString &s)
    {
    return insert(internalData->_length, s);
    }


EXPORT_C CAlfString &CAlfString::insert(uint index, const char *insertChars, uint insertLength)
{
    if (insertLength == 0)
        return *this;

    //detach();

    // if (internalData->_isAsciiValid){
        uint originalLength = internalData->_length;
        char *targetChars;

        // Ensure that we have enough space.
        if( !setLength (originalLength + insertLength) ) 
            {
            throw std::bad_alloc();
            //return *this;    
            }
        
        targetChars = (char *)internalData->ascii();

        // Move tail to make space for inserted characters.
        memmove (targetChars+index+insertLength, targetChars+index, originalLength-index);

        // Insert characters.
        memcpy (targetChars+index, insertChars, insertLength);

        internalData->_isUnicodeValid = 0;
    //}
    /*    
    else if (dataHandle[0]->_isUnicodeValid){
        uint originalLength = dataHandle[0]->_length;
        QChar *targetChars;

        // Ensure that we have enough space.
        if( !setLength (originalLength + insertLength) ) return *this;
        targetChars = (QChar *)unicode();

        // Move tail to make space for inserted characters.
        memmove (targetChars+(index+insertLength), targetChars+index, (originalLength-index)*sizeof(QChar));

        // Insert characters.
        uint i = insertLength;
        QChar *target = targetChars+index;

        while (i--)
            *target++ = *insertChars++;
        }
    else
        FATAL("invalid character cache",0);
        */

    return *this;
}



EXPORT_C CAlfString& CAlfString::insert(uint index, const CAlfString &s)
{
    if (s.internalData->_length == 0)
        return *this;

#ifdef QSTRING_DEBUG_UNICODE
    //forceUnicode();
#endif

    // if (internalData->_isAsciiValid) //&& s.isAllLatin1()
        // {
        insert(index, s.latin1(), s.internalData->_length);
        // }
    /*
    else 
        {
        uint insertLength = qs.dataHandle[0]->_length;
        uint originalLength = dataHandle[0]->_length;
        AlfChar *targetChars;

        // Ensure that we have enough space.
        if( !setLength (originalLength + insertLength) ) return *this;
        targetChars = forceUnicode();

        // Move tail to make space for inserted characters.
        memmove (targetChars+(index+insertLength), targetChars+index, (originalLength-index)*sizeof(QChar));

        // Insert characters.
        if (qs.dataHandle[0]->_isAsciiValid){
            uint i = insertLength;
            AlfChar *target = targetChars+index;
            char *a = (char *)qs.ascii();

            while (i--)
                *target++ = *a++;
        } 
        else {
            AlfChar *insertChars = (AlfChar *)qs.unicode();
            memcpy (targetChars+index, insertChars, insertLength*sizeof(QChar));
        }

        internalData->_isAsciiValid = 0;
        }
    */

    return *this;
}


// Increase buffer size if necessary.  Newly allocated
// bytes will contain garbage.
bool CAlfString::setLength(uint newLen)
{

    if (newLen == 0) {
        //setUnicode(0, 0);
        return true;
    }

    // Missing optimization: Could avoid copying characters we are going to remove
    // by making a special version of detach().
//    detach();

//    ASSERT(dataHandle != shared_null_handle);

/*
#ifdef QSTRING_DEBUG_UNICODE
    forceUnicode();
#endif
*/

    // if (internalData->_isAsciiValid){
        if (newLen+1 > internalData->_maxAscii) {  
            if( internalData->increaseAsciiSize(newLen+1) == false ) return false;
        }
        // Ensure null termination, although newly allocated
        // bytes contain garbage.
        internalData->_ascii[newLen] = 0;
    // }
    /*else if (internalData->_isUnicodeValid){
        if (newLen > internalData->_maxUnicode) {
            if( internalData->increaseUnicodeSize(newLen) == false ) return false;
        }
    }*/
/*    
    else
        FATAL("invalid character cache",0);
*/        

    internalData->_length = newLen;
    return true;
}



EXPORT_C bool CAlfString::operator==(const CAlfString &s1)const
    {
    // if (internalData->_isAsciiValid && s1.internalData->_isAsciiValid) 
    //     {
        return strcmp(latin1(), s1.latin1()) == 0;
    //    }
    // return false;    
    /*    
    return s1.dataHandle[0]->_length == s2.dataHandle[0]->_length
        && memcmp(s1.unicode(), s2.unicode(), s1.dataHandle[0]->_length * sizeof(QChar)) == 0;
        */
    }



}
