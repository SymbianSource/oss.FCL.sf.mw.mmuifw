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
* Description:  utility for ustring
*
*/


#include <osn/ustring.h>
#include <glib/gstring.h>

#include <stdlib.h>
#include <errno.h>
#include <stdexcept>
#include <osn/osnnew.h>

namespace osncore
{

const int KBase = 10;

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//   
template <typename T>
OSN_EXPORT UtfProxy<T>::~UtfProxy()
    {
    if(iUtf)
        {
        free(iUtf);
        }
    }
   
// ---------------------------------------------------------------------------
// Conversion to Utf16 from Utf8 using g_utf8_to_utf16
// ---------------------------------------------------------------------------
//   
OSN_EXPORT auto_ptr<Utf16Proxy> toUtf16(const UString& aSourceUtf8)
    {
    Utf16* ret = g_utf8_to_utf16(aSourceUtf8.getUtf8(),-1,0,0,0);
    
    auto_ptr<Utf16Proxy> tmp;
    try
        {
        tmp.reset(new (EMM)Utf16Proxy(ret));    
        }
    catch(std::bad_alloc& e)
        {
        free(ret);
        }
    return tmp;
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//   
OSN_EXPORT int toInt(const UString& aSource)
    {
    int ret(0);
    const Utf8* string = aSource.getUtf8();
    if (!string)
        {
        throw UString::InvalidUtf8();    
        }
    
    ret = strtol(string, (char **)NULL, KBase);
    if( errno == EINVAL || errno == ERANGE)
        {
        errno = 0;
        throw UString::InvalidUtf8();                
        }
    return ret;
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//   
static auto_ptr<UString> normalize(const UString& aSource, GNormalizeMode aMode)
    {
    const Utf8* string = aSource.getUtf8();
    if (!string)
        {
        throw UString::InvalidUtf8();    
        }
    
    Utf8* ret = g_utf8_normalize(string, -1, aMode);
    
    if(!ret)
        {
        throw UString::InvalidUtf8();                
        }
    auto_ptr<UString> tmp;
    try
        {
        tmp.reset(new (EMM)UString(ret));
       // g_free(ret);    
        free(ret);
        }
    catch(std::bad_alloc& e)
        {
       // g_free(ret);
        free(ret);
        }    
    return tmp;    
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//   
OSN_EXPORT auto_ptr<UString> normalizeNFD(const UString& aSource)
    {
    return normalize(aSource,G_NORMALIZE_DEFAULT);
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//   
OSN_EXPORT auto_ptr<UString> normalizeNFC(const UString& aSource)
    {
    return normalize(aSource,G_NORMALIZE_DEFAULT_COMPOSE);
    }    

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//   
OSN_EXPORT auto_ptr<UString> normalizeNFKD(const UString& aSource)
    {
    return normalize(aSource,G_NORMALIZE_ALL);
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//   
OSN_EXPORT auto_ptr<UString> normalizeNFKC(const UString& aSource)
    {
    return normalize(aSource,G_NORMALIZE_ALL_COMPOSE);
    }            
    
}
