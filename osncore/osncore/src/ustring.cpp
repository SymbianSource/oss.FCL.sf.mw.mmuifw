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
* Description:  utf8 encoded byte container
*
*/


#include <osn/ustring.h>
#include <osn/osnnew.h> 
#include "ustringimpl.h"


namespace osncore
{
static const char* const KInvalidUtf8 = "InvalidUtf8";
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
//
OSN_EXPORT const char* UString::InvalidUtf8::what()const throw()
    {
    return KInvalidUtf8;
    }
    
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
//
OSN_EXPORT UString::UString():mImpl(new(EMM)UStringImpl())
    {

    }
	
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
//
OSN_EXPORT UString::UString(const Utf8* aStr)
    :mImpl(new(EMM)UStringImpl(aStr))
    {
	
    }

// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
//
OSN_EXPORT UString::UString(const Utf8* aStr, int aLen)
    :mImpl(new(EMM)UStringImpl(aStr, aLen))
    {
        
    }

// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
//
OSN_EXPORT UString::UString(const UString& aUString)
    :mImpl(new(EMM)UStringImpl(*aUString.mImpl.get()))
    {
        
    }
    
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
//
OSN_EXPORT UString::UString(Unicode aCodePoint)
    :mImpl(new(EMM)UStringImpl(aCodePoint))
    {
        
    }
    
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
//
OSN_EXPORT UString& UString::operator=(const UString& aRhs)
    {
    if (this != &aRhs)
        {
        UStringImpl* tmp = new (EMM)UStringImpl(*aRhs.mImpl.get());
        mImpl.reset(tmp);
        }
        
    return *this;        
    }
	
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
//
OSN_EXPORT UString& UString::operator=(const Utf8* aRhs)
    {
    if (aRhs)
        {
        UStringImpl* tmp = new (EMM)UStringImpl(aRhs);
        mImpl.reset(tmp);
        }
        
    return *this;       
    }
	
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
//
OSN_EXPORT UString::~UString()
    {

    }	
	
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
//
OSN_EXPORT bool UString::isEmpty()const
    {
    return mImpl->empty();
    }

// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
//
OSN_EXPORT long UString::getCharLength()const
    {
    return mImpl->length();
    }

// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
//
OSN_EXPORT long UString::getByteLength()const
    {
    return mImpl->bytes();
    }

// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
//
OSN_EXPORT const Utf8* UString::getUtf8()const
    {
   	return mImpl->utf8();
    }
    
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
//
OSN_EXPORT int UString::compareC(const UString& aUString)const
    {
    return mImpl->compareC(*aUString.mImpl.get());
    }
    	
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
//
OSN_EXPORT int UString::compareC(const Utf8* aStr)const
    {
    return mImpl->compareC(aStr);
    }
    
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
//
OSN_EXPORT int UString::compare(const UString& aUString)const
    {
    return mImpl->compare(*aUString.mImpl.get());
    }
    	
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
//
OSN_EXPORT int UString::compare(const Utf8* aStr)const
    {
    return mImpl->compare(aStr);
    }    

// -------------------------------------------------------------------------    	  
// -------------------------------------------------------------------------
//
OSN_EXPORT bool UString::operator==(const UString& aRhs)const
    {
    return !mImpl->compare(*aRhs.mImpl.get());    
    }
    
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
//
OSN_EXPORT bool UString::operator==(const Utf8* aRhs)const
    {
    return !mImpl->compare(aRhs);    
    }
    
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
//
OSN_EXPORT void UString::append(const UString& aUString)
    {
    mImpl->append(*aUString.mImpl.get());
    }

// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
//
OSN_EXPORT void UString::append(const Utf8* aStr)
    {
    mImpl->append(aStr);
    }   

// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
//
OSN_EXPORT void UString::insert(
    long aPos, 
    const Utf8* aStr)
    {
    mImpl->insert(aPos, aStr);
    }
          
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
//
OSN_EXPORT void UString::insert(
    long aPos, 
    const Utf8* aStr,
    long aLength)
    {
    mImpl->insert(aPos, aStr,aLength);
    }  
     
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
//
OSN_EXPORT void UString::replace(
    long aPos, 
    const Utf8* aStr)
    {
    mImpl->replace(aPos, aStr);
    }   

// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
//
OSN_EXPORT void UString::replace(
    long aPos, 
    const Utf8* aStr,
    long aLength)
    {
    mImpl->replace(aPos, aStr, aLength);
    }         

// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
//
OSN_EXPORT void UString::erase(
    long aPos, 
    long aLength)
    {
    mImpl->erase(aPos, aLength);
    }         

// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
//
OSN_EXPORT Unicode UString::operator[](long aIndex)
    {
    return mImpl->operator[](aIndex);
    }         

// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
//
OSN_EXPORT bool UString::isNull()const
    {
    return mImpl->null();
    }
}
