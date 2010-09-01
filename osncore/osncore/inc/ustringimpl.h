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
* Description:  String class implementation
*
*/

#ifndef USTRING_IMPL_H
#define USTRING_IMPL_H

#include <gstring.h>
#include <osn/ustring.h>

#define MAX_STR_LEN 24

namespace osncore
{
    
class SString;

/**
 *  Glib string wrapper.
 *  @see ustring.h for description of ustring
 *  @lib osncore.lib
 *  @since S60 5.0
 */
class UStringImpl
	{
public:
    /**
     * Default constructor.
     */
    UStringImpl();
    /**
      * Construct a string as a copy of given null terminated string.
      *
      * @since S60 5.0
      * @exception std:bad_alloc is thrown if string cannot be allocated from heap
      * @param aStr Utf-8 encoded string
      */
    explicit UStringImpl(const Utf8* aStr);
    /**
     * Construct a string as a copy of given string
     * with byte length of the source buffer.
     * Because a length is provided, source doesn't need to be null terminated.
     *
     * @since S60 5.0
     * @exception std:bad_alloc is thrown if string cannot be allocated from heap
     * @param aStr Utf-8 encoded string
     * @param aByteCount Byte count of Utf-8 encoded string
     */
    explicit UStringImpl(const Utf8* aStr, int aLen);
    /**
      * Construct a string from unicode code point. If Unicode is invalid,
      * @exception std:bad_alloc is thrown if string cannot be allocated from heap
      * @since S60 5.0
      * @param aCodePoint Unicode code point
      */
    explicit UStringImpl(Unicode aCodePoint);
    
    /**
     * UStringImpl copy constructor
     * @exception std:bad_alloc is thrown if string cannot be allocated from heap
     * @since S60 5.0
     * @param aUString Copy source
     */
    UStringImpl(const UStringImpl& aUStringImpl);
   
    // Destructor
   ~UStringImpl();
   
    // Utilities
   /**
    * Checks whether string is empty.
    *
    * @since S60 5.0
    * @return True if string doesn't have content.
    */
   	bool empty()const;
    /**
     * Checks whether string is null.
     *
     * @since S60 5.0
     * @return True if string is null.
     */
   	bool null()const;
    /**
     * Returns the length of the string in characters.
     *
     * @since S60 5.0
     * @return Length
     */
   	long length()const;
    /**
     * Returns the length of the string in bytes
     *
     * @since S60 5.0
     * @return Bytes
     */
   	long bytes()const;
   	
    /**
     * Returns utf8 type string
     *
     * @since S60 5.0
     * @return utf8 type string
     */
   	const Utf8* utf8()const;
 
    /**
     * Compares two strings using strcmp(). Note that this is not linguistic comparison nor case insensitive.
     *
     * @since S60 5.0
     * @param aUStringImpl UStringImpl object to compare
     * @return < 0 if this compares before aRhs, 0 if they compare equal, > 0 if this compares after aRhs.
     */
    int compare(const UStringImpl& aUStringImpl)const;
    /**
     * Compares two strings using strcmp(). Note that this is not linguistic comparison nor case insensitive.
     *
     * @since S60 5.0
     * @param aStr String to compare
     * @return < 0 if this compares before aRhs, 0 if they compare equal, > 0 if this compares after aRhs.
     */
    int compare(const Utf8* aStr)const;
    /**
     * Compares two strings for ordering using the linguistically correct rules for the current locale.
     * When sorting a large number of strings, it will be significantly faster
     * to obtain collation keys with g_utf8_collate_key()from libglib and compare the keys
     * with strcmp() when sorting instead of sorting the original strings.
     *
     * @since S60 5.0
     * @param aUStringImpl UStringImpl object to compare
     * @return < 0 if this compares before aRhs, 0 if they compare equal, > 0 if this compares after aRhs.
     */
    int compareC(const UStringImpl& aUStringImpl)const;
    /**
     * Compares two strings for ordering using the linguistically correct rules for the current locale.
     * When sorting a large number of strings, it will be significantly faster
     * to obtain collation keys with g_utf8_collate_key() from libglib and compare the keys
     * with strcmp() when sorting instead of sorting the original strings.
     * If aStr is invalid utf8, UString::InvalidUtf8 exception is thrown.
     *
     *
     * @since S60 5.0
     * @param aStr String to compare
     * @return < 0 if this compares before aRhs, 0 if they compare equal, > 0 if this compares after aRhs.
     */
    int compareC(const Utf8* aStr)const;
    
    /**
     * Adds a string onto the end of string, expanding it if necessary.
     *
     * @since S60 5.0
     * @param aUStringImpl String object to append
     */
    void append(const UStringImpl& aUStringImpl);
    /**
     * Adds a string onto the end of string, expanding it if necessary.
     *
     * @since S60 5.0
     * @param aStr String to append
     */
    void append(const Utf8* aStr);
    
    /**
     * Inserts aStr into string, expanding it if necessary.
     * If aPos is -1, bytes are inserted at the end of the string.
     *
     * @since S60 5.0
     * @exception std:out_of_range is thrown if given position is invalid
     * @exception std:bad_alloc is thrown if string cannot be allocated from heap
     * @param aPos The character position to insert the copy of the string
     * @param aStr The string to insert
     */
    void insert(long aPos, const Utf8* aStr);
    /**
     * Inserts aStr into string, expanding it if necessary. Because length is
     * provided, aStr may contain embedded nulls and need not be null terminated.
     * If aPos is -1, bytes are inserted at the end of the string.
     *
     * @since S60 5.0
     * @exception std:out_of_range is thrown if given position is invalid
     * @exception std:bad_alloc is thrown if string cannot be allocated from heap
     * @param aPos The character position to insert the copy of the string
     * @param aStr The string to insert
     * @param aCharCount Character count.
     */
    void insert(long aPos, const Utf8* aStr, long aLength);
    /**
     * Replace a substring with a given string, expanding it if necessary
     *
     * @since S60 5.0
     * @exception std:out_of_range is thrown if given position is invalid
     * @exception std:bad_alloc is thrown if string cannot be allocated from heap
     * @param aPos The charater position to replace the string
     * @param aStr The string to insert
     */
    void replace(long aPos, const Utf8* aStr);
    /**
     * Replace a substring with a given string, expanding it if necessary. Because length is
     * provided, aStr may contain embedded nulls and need not be null terminated.
     * If aLength is < 0, length is assumed to be aStr's length.
     *
     * @since S60 5.0
     * @exception std:out_of_range is thrown if given position is invalid
     * @exception std:bad_alloc is thrown if string cannot be allocated from heap
     * @param aPos The character position to replace the string
     * @param aStr The string to insert
     * @param aCharCount Character count.
     */
    void replace(long aPos, const Utf8* aStr, long aLength);
    /**
     * Erase a substring
     *
     * @since S60 5.0
     * @exception std:out_of_range is thrown if given position is invalid
     * @param aPos The character position to start erasing from
     * @param aCharCount number of characters to erase.
     */
    void erase(long aPosOffset, long aLength);
    
    /**
     * Returns unicode at given position in string
     *
     * @since S60 5.0
     * @exception std:out_of_range is thrown if requested index is invalid
     * @param aPos The requested position
     */
    Unicode operator[](long aIndex);
    
private:
    /**
     * Initialises internal string
     * @since S60 5.0
     * @exception std:bad_alloc is thrown if string cannot be allocated from heap
     * @param aStr The string used for initialising
     */
    void initialize(const Utf8* aStr);
    /**
     * Allocates memory from heap if needed for internal string, throws if allocation fail
     * @since S60 5.0
     * @exception std:bad_alloc is thrown if string cannot be allocated from heap
     * @param aLength length of bytes to allocate
     */
    void checkBuffer(unsigned int aLength);
    /**
     * Implements the insertion functionality
     * @since S60 5.0
     * @exception std:bad_alloc is thrown if string cannot be allocated from heap
     * @param aIndex The character position to insert the copy of the string
     * @param aInsertChars The string to insert
     * @param aInsertLength The amount of characters to insert
     */
    void insertImpl(unsigned int aIndex, const char* aInsertChars, unsigned int aInsertLength);
    /**
     * Implements the append functionality
     * @since S60 5.0
     * @param aStr The string to append
     * @param aLength The amount of characters to append
     */
    void appendImpl(const char* aStr, unsigned int aLength);
    /**
     * Implements the erase functionality
     * @since S60 5.0
     * @exception std:out_of_range is thrown if given index is invalid
     * @param aIndex The character position to start erasing from
     * @param aLength The amount of characters to erase
     */
    void eraseImpl(unsigned int aIndex, unsigned int aLength);
private:
    /**
     * Internal buffer, allocated from heap.
     * Long strings are allocated from heap 
     */
	gchar* 	str;
    /**
     * Internal buffer, allocated from stack.
     * Short strings are allocated from stack for performance reason, 
     */
	gchar 	internalStr[MAX_STR_LEN];
    /**
     * length of string
     */
	gsize   len;    
    /**
     * length of allocated bytes
     */
	gsize   allocated_len;
    /**
     * Identifies if string is allocated from heap or not
     */
	bool    isInHeap;
	};
}	

#endif