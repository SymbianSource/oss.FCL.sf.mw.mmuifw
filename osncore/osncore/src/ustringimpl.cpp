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
* Description:  String class implementation
*
*/


#include "ustringimpl.h"

#include <gstring.h>
#include <stdexcept>
#if defined(__GNUC__)
#include <string.h>
#else
#include <string.h>
#endif
#include <osn/osnnew.h>
using namespace std;

 
namespace osncore
{

const int KMaxUtf8Length = 6;


// -------------------------------------------------------------------------
// Returns the byte count for aStr with aCharCount length
// -------------------------------------------------------------------------
//
static long bytesFromCharacters(const Utf8* aStr, long aCharCount)
    {
    int byteCount = 0;
    
    if (aStr)
        {
        const char* srcEndOffset = g_utf8_offset_to_pointer(aStr,aCharCount); 
        byteCount = srcEndOffset - aStr;    
        }
    
    return byteCount;
    }

// -------------------------------------------------------------------------
// Initializes the string from stack or heap depending on its length
// -------------------------------------------------------------------------
//
void UStringImpl::initialize(const Utf8* aStr)
    {
    if (len < MAX_STR_LEN)
        {
        str = internalStr;
        memcpy(internalStr, aStr, len);
        }
    else
        {
        str = (gchar*)calloc(len+1, sizeof (gchar));
    	if(!str)
    		{
    		throw bad_alloc();
    		}
    	memcpy(str,aStr,len);
    	isInHeap = true;
        }
	
	str[len] = 0;
	allocated_len = len+1;    
    }
    
// -------------------------------------------------------------------------
// Allocates memory from heap if needed, throws if allocation fails
// -------------------------------------------------------------------------
//
void UStringImpl::checkBuffer(unsigned int aLength)
	{
	if (aLength < allocated_len)
		{
		return;
		}
    if (aLength >= MAX_STR_LEN)
        {
        if (!isInHeap) //str is in stack, move it to heap
            {
            gchar* temp = (gchar*)calloc(aLength + 1, sizeof(gchar));
            if (!temp)
                {
        		throw bad_alloc();
                }
            if(str)
                {
                memcpy(temp, str, len);
                }
            str = temp;
            isInHeap = true;
            }
        else
            {
        	gchar* tmp(0);
        	tmp = (gchar*)realloc(str,aLength+1);
        	if(!tmp)
        		{
        		throw bad_alloc();
        		}
        	str = tmp;
            }
        }
    else
        {
        // if ustring is constructed with null or "" set internal buffer to
        // point to str
        if (!str)
            str = internalStr;
        }
	allocated_len = aLength+1;

     
	// reset allocated space
	memset(str+len, 0, allocated_len-len);
	}
// -------------------------------------------------------------------------
// Implements the actual insertion functionality
// -------------------------------------------------------------------------
//
void UStringImpl::insertImpl(unsigned int aIndex, const char* aInsertChars, unsigned int aInsertLength)
	{
    // Ensure that we have enough space.
	checkBuffer(len + aInsertLength); 
	// Move tail to make space for inserted characters.
    memmove(str+aIndex+aInsertLength, str+aIndex, len-aIndex);
    // Insert characters.
    memcpy(str+aIndex, aInsertChars, aInsertLength);
    // Update len
    len += aInsertLength;
	}  
// -------------------------------------------------------------------------
// Implements the actual append functionality
// -------------------------------------------------------------------------
//
void UStringImpl::appendImpl(const char* aStr, unsigned int aLength)
	{
	insertImpl(len, aStr, aLength);
	}
// -------------------------------------------------------------------------
// Implements the actual erase functionality
// -------------------------------------------------------------------------
//
void UStringImpl::eraseImpl(unsigned int aIndex, unsigned int aLength)
	{
	if(aIndex > len)
		{
		throw out_of_range("out_of_range");
		}
	
	if(aIndex+aLength >= len)
		{
		// erase from given index to end of the string
		if(aIndex<len)
			{
			len = aIndex;
			}
		memset(str+aIndex,0,len-aIndex);
		}
	else
		{
		// Move tail (remove begin or between) to given index.
	    memmove(str+aIndex, str+aIndex+aLength, len-(aIndex+aLength));
		len -=  aLength;
		memset(str+len,0,allocated_len-len);
		}
	}
    
// -------------------------------------------------------------------------
// Constructor
// -------------------------------------------------------------------------
//
UStringImpl::UStringImpl():str(0),len(0),allocated_len(0), isInHeap(0)
	{
#ifdef _DEBUG
	g_setenv("G_SLICE","always-malloc",1);
#endif
	}
// -------------------------------------------------------------------------
// Constructs the string from aStr
// -------------------------------------------------------------------------
//
UStringImpl::UStringImpl(const Utf8* aStr)
    :str(0),len(0),allocated_len(0), isInHeap(0)
    {
#ifdef _DEBUG    
    g_setenv("G_SLICE","always-malloc",1);
#endif    
    //NULL byte not accepted
    if (aStr)
        {
        len = strlen(aStr); 
        initialize(aStr);
        }   
    }

// -------------------------------------------------------------------------
// Constructs the string from aStr with aByteCount length
// -------------------------------------------------------------------------
//
UStringImpl::UStringImpl(const Utf8* aStr, int aByteCount)
    :str(0),len(0),allocated_len(0), isInHeap(0)
    {
#ifdef _DEBUG    
    g_setenv("G_SLICE","always-malloc",1);
#endif    
    //NULL byte not accepted
    if (aStr)
        {
        len = aByteCount;
        initialize(aStr);
        }    
    }

// -------------------------------------------------------------------------
// Constructs the string from aCodePoint
// -------------------------------------------------------------------------
//
UStringImpl::UStringImpl(Unicode aCodePoint)
    :str(0),len(0),allocated_len(0), isInHeap(0)
    {
#ifdef _DEBUG    
    g_setenv("G_SLICE","always-malloc",1);
#endif    
        
    char string[KMaxUtf8Length]; 
    long usedBytes = g_unichar_to_utf8((unsigned int)aCodePoint,&string[0]);
    if(usedBytes > 0)
        {
        len = usedBytes;
        initialize(&string[0]);
        }
    }

// -------------------------------------------------------------------------
// Copy constructor
// -------------------------------------------------------------------------
//
UStringImpl::UStringImpl(const UStringImpl& aUStringImpl)
    :str(0),len(aUStringImpl.len),allocated_len(0), isInHeap(0)
    {
#ifdef _DEBUG    
    g_setenv("G_SLICE","always-malloc",1);
#endif    
    if(aUStringImpl.str)
        {
        initialize(aUStringImpl.str);
        }
    }


// -------------------------------------------------------------------------
// Destructor
// -------------------------------------------------------------------------
//
UStringImpl::~UStringImpl()
	{
	if (str && isInHeap)
	    {
	    delete str;
	    }
	}	
// -------------------------------------------------------------------------
// Returns true if the string is empty
// -------------------------------------------------------------------------
//
bool UStringImpl::empty()const
	{
	return (len > 0) ? false : true;
	}

// -------------------------------------------------------------------------
// Returns true if the string is a null string
// -------------------------------------------------------------------------
//
bool UStringImpl::null()const
	{
	return (str) ? false : true;
	}
	
// -------------------------------------------------------------------------
// Returns the length of the string in characters 
// -------------------------------------------------------------------------
//
long UStringImpl::length()const
    {
    long ret(0);
    if(str && (len > 0))
        {
        ret = g_utf8_strlen(str, len);
        }
    return ret;    
    }        

// -------------------------------------------------------------------------
// Returns the length of the string in bytes
// -------------------------------------------------------------------------
//
long UStringImpl::bytes()const
    {
    return (long)len;    
    }        

	
// -------------------------------------------------------------------------
// Returns the utf8 type string
// -------------------------------------------------------------------------
//
const Utf8* UStringImpl::utf8()const
    {
    return str;
    }
    
// -------------------------------------------------------------------------
// Compares the string to another UString with strcmp
// -------------------------------------------------------------------------
//
int UStringImpl::compare(const UStringImpl& aUStringImpl)const
    {
    int ret(1);
    if(!str && !aUStringImpl.str)
        {
        ret = 0;    
        }
    else if(str && aUStringImpl.str)
        {
        ret = strcmp(str, aUStringImpl.str);
        }
    else if(!str)
        {
        ret = -1;
        }
  
    return ret;
    }	 	
// -------------------------------------------------------------------------
// Compares the string to Utf8 string with strcmp
// -------------------------------------------------------------------------
//
int UStringImpl::compare(const Utf8* aStr)const
    {    
    int ret(1);
    if(str && aStr)
        {
        ret = strcmp(str, aStr);
        }
    else if(!str && !strlen(aStr))
        {
        ret = 0;    
        }
    else if(!str)
        {
        ret = -1;
        }
   
    return ret;
    }	 

// -------------------------------------------------------------------------
// Compares the string to another UString using the linguistically
// correct rules for the current locale
// -------------------------------------------------------------------------
//
int UStringImpl::compareC(const UStringImpl& aUStringImpl)const
    {
    int ret(1);
    if(!str && !aUStringImpl.str)
        {
        ret = 0;    
        }
    else if(str && aUStringImpl.str)
        {
        ret = g_utf8_collate(str, aUStringImpl.str);
        }
    else if(!str)
        {
        ret = -1;
        }
  
    return ret;
    }	 	
// -------------------------------------------------------------------------
// Compares the string to Utf8 string using the linguistically
// correct rules for the current locale
// -------------------------------------------------------------------------
//
int UStringImpl::compareC(const Utf8* aStr)const
    {    
    int ret(1);
    if(str && aStr)
        {
        ret = g_utf8_collate(str, aStr);
        }
    else if(!str && !strlen(aStr))
        {
        ret = 0;    
        }
    else if(!str)
        {
        ret = -1;
        }
   
    return ret;
    } 

// -------------------------------------------------------------------------
// Appends another UString to this string 
// -------------------------------------------------------------------------
//
void UStringImpl::append(const UStringImpl& aUStringImpl)
    {
    if(str && aUStringImpl.str)
        {
        appendImpl(aUStringImpl.str, aUStringImpl.len);
        }
    }	 
// -------------------------------------------------------------------------
// Appends a Utf8 string to this string
// -------------------------------------------------------------------------
//
void UStringImpl::append(const Utf8* aStr)
    {
    if(aStr && (strlen(aStr) !=0))
        {
        appendImpl(aStr, strlen(aStr));
        }
    }

// -------------------------------------------------------------------------
// Inserts Utf8 string to this string 
// -------------------------------------------------------------------------
//
void UStringImpl::insert( 
    long aPos, 
    const Utf8* aStr)
    {
   
    if(aStr)
       {
       if (!str)
           checkBuffer(0);
       long dstCharCount = g_utf8_strlen(str, -1);
       if(aPos < -1 || aPos > dstCharCount)
           {
           throw out_of_range("out_of_range");  
           }
        // find out byte position for given character position
       long startBytePos(-1);
       if(aPos >= 0)
           {
           if(str)
               {
               const char* startOffset = g_utf8_offset_to_pointer(str, aPos);
               startBytePos = startOffset - str;
               }
           }
       if(aPos == -1)
    	   {
    	   appendImpl(aStr, strlen(aStr));
    	   }
       else
    	   {
    	   insertImpl(startBytePos, aStr, strlen(aStr));
    	   }
       }
    }
   
// -------------------------------------------------------------------------
// Inserts Utf8 string to this string with length of the Utf8 specified
// -------------------------------------------------------------------------
//
void UStringImpl::insert( 
    long aPos, 
    const Utf8* aStr,
    long aCharCount)
    {
    if (aCharCount < 1)
        {
        throw out_of_range("out_of_range");    
        }
    
    if (aStr)
        {
        long srcByteLen = bytesFromCharacters(aStr, aCharCount);
        if(!str)
            checkBuffer(0);
        
        // calculate how many bytes are needed for given characters
        long srcCharCount = g_utf8_strlen(aStr, srcByteLen);
        long dstCharCount = g_utf8_strlen(str, -1); 
        long startBytePos(-1); // appends end of the string

        if(aPos < -1 || aPos > dstCharCount-1 || aCharCount > srcCharCount)
            {
            throw out_of_range("out_of_range");  
            }
           
        if(aPos != -1)
            {
            const char* startOffset = g_utf8_offset_to_pointer(str, aPos); 
            startBytePos = startOffset - str; 
            }
        const char* endOffset = g_utf8_offset_to_pointer(aStr, aCharCount);
        long insertByteLength = endOffset - aStr;   

        if(aPos == -1)
            {
            appendImpl(aStr, insertByteLength);
            }
        else
            {
            insertImpl(startBytePos, aStr, srcByteLen);
            }
        }
    }

// -------------------------------------------------------------------------
// Replaces Utf8 string to this string 
// -------------------------------------------------------------------------
//
void UStringImpl::replace(
    long aPos, 
    const Utf8* aStr)
    {
    if (aStr)
        {
        if (!str)
            checkBuffer(0);
        
        long srcByteLen = strlen(aStr);
        long srcCharCount = g_utf8_strlen(aStr,-1); 
        long dstCharCount = g_utf8_strlen(str, -1);
        if(aPos > dstCharCount || aPos < 0)
            {
            throw out_of_range("out_of_range");    
            }
        
        // calculate start byte position
        if(str)
            {
            const char* startOffset = g_utf8_offset_to_pointer(str, aPos);
            long startBytePos = startOffset - str; 
            
            // calculate erasable character count in bytes
                if(startOffset)
                    {
	                const char* endOffset = g_utf8_offset_to_pointer(startOffset, srcCharCount);
	                long eraseByteLength = endOffset - startOffset;  
	                // erase existing characters
	                eraseImpl(startBytePos, eraseByteLength);
                    }
            insertImpl(startBytePos, aStr, srcByteLen);
            }
        }
   }  
      
   
// -------------------------------------------------------------------------
// Replaces a Utf8 string to this string with length of the Utf8 specified
// -------------------------------------------------------------------------
//
void UStringImpl::replace(
    long aPosOffset, 
    const Utf8* aStr,
    long aCharCount)
    {
    if (aCharCount < 1)
        {
        throw out_of_range("out_of_range");    
        }
        
    if (aStr)
        {
        long srcByteLen = bytesFromCharacters(aStr, aCharCount);
        if(!str)
            checkBuffer(0);
        
        long srcCharCount = g_utf8_strlen(aStr, srcByteLen); 
        long dstCharCount = g_utf8_strlen(str, -1);
        if(aPosOffset > dstCharCount || aPosOffset < 0 || aCharCount > srcCharCount)
            {
            throw out_of_range("out_of_range");    
            }
        // calculate start byte position
        if(str)
            {
            const char* startOffset = g_utf8_offset_to_pointer(str, aPosOffset);
            long startBytePos = startOffset - str; 
        
        // calculate erasable character count in bytes
            if(startOffset)
                {
                const char* endOffset = g_utf8_offset_to_pointer(startOffset, srcCharCount);
                long eraseByteLength = endOffset - startOffset;  
                // erase existing characters
                eraseImpl(startBytePos, eraseByteLength);
                }
            insertImpl(startBytePos, aStr, srcByteLen);
            }
        }
   }  
 
// -------------------------------------------------------------------------
// Erases aCharCount characters from the string at position specified by 
// aPosOffset
// -------------------------------------------------------------------------
//
void UStringImpl::erase(
    long aPosOffset, 
    long aCharCount)
    {
    
    if(str)
        {
        long srcCharCount = g_utf8_strlen(str, -1); 
        
        if (aPosOffset < 0 || aCharCount < 1 || aCharCount > srcCharCount)
            {
            throw out_of_range("out_of_range"); 
            }
        
        // calc startpos
        const char* startOffset = g_utf8_offset_to_pointer(str, aPosOffset); 
        long startBytePos = startOffset - str; 
        
        // calc bytes to erase
        const char* endOffset = g_utf8_offset_to_pointer(startOffset, aCharCount); 
        long eraseByteLength = endOffset - startOffset; 
       
        eraseImpl(startBytePos, eraseByteLength);
        }
   }  

// -------------------------------------------------------------------------
// Returns unicode at aIndex position in the string
// -------------------------------------------------------------------------
//
Unicode UStringImpl::operator[](long aIndex)
   {
   Unicode ret(0xF8FF);
    
   if(str)
       {
       long charCount = g_utf8_strlen(str, -1);
       if(aIndex < 0 || aIndex > charCount-1)
           {
           throw out_of_range("out_of_range"); 
           }
       // calc startpos
       const char* uniChar = g_utf8_offset_to_pointer(str, aIndex); 
        
       ret = g_utf8_get_char(uniChar);        
       }
   return ret;     
   }  

}
    		

