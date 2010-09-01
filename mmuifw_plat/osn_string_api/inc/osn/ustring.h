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
* Description:  String class
*
*/




#ifndef USTRING_H
#define USTRING_H

#include <osn/osndefines.h>
#include <stdexcept>
#include <memory>
#include <string>

using namespace std;

namespace osncore
{

class UStringImpl;

typedef char Utf8;
typedef unsigned short Utf16;
typedef unsigned long Unicode;

/**
 *  @class UString ustring.h "osn/ustring.h"
 *  Class encapsulates utf-8 encoded string. UString has a character based API thus
 *  e.g length will return number of charaters. Unicode operations for utf8 encoded
 *  string can be done using glib unicode manipulation functions and/or using non-member functions for
 *  UString type.
 *
 *  @lib osncore.lib
 *  @since S60 5.0
 *  @status Draft
 *  @interfaces UString
 */
class UString
    {
public:
    /**
     * UString exception class
     * @since S60 5.0
     */
    class InvalidUtf8: public invalid_argument
        {
    public:

        InvalidUtf8(): invalid_argument("invalid"){}
        /**
         * Gets the invalid utf string
         * @since S60 5.0
         * @return the class name (InvalidUtf8) as a C-style string.
         */
        OSN_IMPORT virtual const char* what()const throw();
        };
public:
    /**
     * Default constructor.
     */
    OSN_IMPORT UString();

    /**
     * Construct a UString as a copy of given null terminated string.
     *
     * @since S60 5.0
     * @exception std::bad_alloc
     * @param aStr Utf-8 encoded string
     */
    OSN_IMPORT explicit UString(const Utf8* aStr);

    /**
     * Construct a UString as a copy of given string
     * with byte length of the source buffer.
     * Because a length is provided, source doesn't need to be null terminated.
     *
     * @since S60 5.0
     * @exception std::bad_alloc
     * @param aStr Utf-8 encoded string
     * @param aByteCount Byte count of Utf-8 encoded string
     */
    OSN_IMPORT explicit UString(const Utf8* aStr, int aByteCount);

    /**
     * Construct a UString from unicode code point. If Unicode is invalid,
     * @since S60 5.0
     * @exception std::bad_alloc
     * @param aCodePoint Unicode code point
     */
    OSN_IMPORT explicit UString(Unicode aCodePoint);

    /**
     * UString copy constructor
     * @since S60 5.0
     * @exception std::bad_alloc
     * @param aUString Copy source
     */
    OSN_IMPORT UString(const UString& aUString);

    /**
     * Copy assignment
     *
     * @since S60 5.0
     * @param aRhs Assignment source
     */
    OSN_IMPORT UString& operator=(const UString& aRhs);

    /**
     * Copy assignment
     *
     * @since S60 5.0
     * @param aRhs Assignment source
     */
    OSN_IMPORT UString& operator=(const Utf8* aRhs);

    /**
     * Destructor.
     */
    OSN_IMPORT virtual ~UString();

    /**
     * Checks whether string is empty.
     *
     * @since S60 5.0
     * @return True if string doesn't have content.
     */
    OSN_IMPORT bool isEmpty()const;

    /**
     * Returns the length of the string in characters.
     *
     * @since S60 5.0
     * @return Length
     */
    OSN_IMPORT long getCharLength()const;

    /**
     * Returns the length of the string in bytes
     *
     * @since S60 5.0
     * @return Bytes
     */
    OSN_IMPORT long getByteLength()const;

    /**
     * Returns utf8 type string
     *
     * @since S60 5.0
     * @return utf8 type string
     */
    OSN_IMPORT const Utf8* getUtf8()const;

    /**
     * Compares two strings using strcmp(). Note that this is not linguistic comparison nor case insensitive.
     *
     * @since S60 5.0
     * @param aUString UString object to compare
     * @return < 0 if this compares before aRhs, 0 if they compare equal, > 0 if this compares after aRhs.
     */
    OSN_IMPORT int compare(const UString& aUString)const;

    /**
     * Compares two strings using strcmp(). Note that this is not linguistic comparison nor case insensitive.
     *
     * @since S60 5.0
     * @param aStr String to compare
     * @return < 0 if this compares before aRhs, 0 if they compare equal, > 0 if this compares after aRhs.
     */
    OSN_IMPORT int compare(const Utf8* aStr)const;

    /**
     * Compares two strings for ordering using the linguistically correct rules for the current locale.
     * When sorting a large number of strings, it will be significantly faster
     * to obtain collation keys with g_utf8_collate_key()from libglib and compare the keys
     * with strcmp() when sorting instead of sorting the original strings.
     *
     * @since S60 5.0
     * @param aUString UString object to compare
     * @return < 0 if this compares before aRhs, 0 if they compare equal, > 0 if this compares after aRhs.
     */
    OSN_IMPORT int compareC(const UString& aUString)const;

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
    OSN_IMPORT int compareC(const Utf8* aStr)const;


    /**
     * Compares two strings using strcmp()
     *
     * @since S60 5.0
     * @param aRhs UString object to compare
     * @return true if they compare equal.
     */
    OSN_IMPORT bool operator==(const UString& aRhs)const;

    /**
     * Compares two strings using strcmp()
     *
     * @since S60 5.0
     * @param aRhs String to compare
     * @return true if they compare equal.
     */
    OSN_IMPORT bool operator==(const Utf8* aRhs) const;

    /**
     * Adds a string onto the end of string, expanding it if necessary.
     *
     * @since S60 5.0
     * @exception std::bad_alloc
     * @param aUString String object to append
     */
    OSN_IMPORT void append(const UString& aUString);

    /**
     * Adds a string onto the end of string, expanding it if necessary.
     *
     * @since S60 5.0
     * @exception std::bad_alloc
     * @param aStr String to append
     */
    OSN_IMPORT void append(const Utf8* aStr);

    /**
     * Inserts aStr into string, expanding it if necessary.
     * If aPos is -1, bytes are inserted at the end of the string.
     *
     * @since S60 5.0
     * @exception std::bad_alloc
     * @exception std:out_of_range is thrown if given position is invalid
     * @param aPos The character position to insert the copy of the string
     * @param aStr The string to insert
     */
    OSN_IMPORT void insert(long aPos, const Utf8* aStr);

    /**
     * Inserts aStr into string, expanding it if necessary. Because length is
     * provided, aStr may contain embedded nulls and need not be null terminated.
     * If aPos is -1, bytes are inserted at the end of the string.
     *
     * @since S60 5.0
     * @exception std::bad_alloc
     * @exception std:out_of_range is thrown if given position is invalid
     * @param aPos The character position to insert the copy of the string
     * @param aStr The string to insert
     * @param aCharCount Character count.
     */
    OSN_IMPORT void insert(long aPos, const Utf8* aStr, long aCharCount);

    /**
     * Replace a substring with a given string, expanding it if necessary
     *
     * @since S60 5.0
     * @exception std::bad_alloc
     * @exception std:out_of_range is thrown if given position is invalid
     * @param aPos The charater position to replace the string
     * @param aStr The string to insert
     */
    OSN_IMPORT void replace(long aPos, const Utf8* aStr);

    /**
     * Replace a substring with a given string, expanding it if necessary. Because length is
     * provided, aStr may contain embedded nulls and need not be null terminated.
     * If aLength is < 0, length is assumed to be aStr's length.
     *
     * @since S60 5.0
     * @exception std::bad_alloc
     * @exception std:out_of_range is thrown if given position is invalid
     * @param aPos The character position to replace the string
     * @param aStr The string to insert
     * @param aCharCount Character count.
     */
    OSN_IMPORT void replace(long aPos, const Utf8* aStr, long aCharCount);

    /**
     * Erase a substring
     *
     * @since S60 5.0
     * @exception std:out_of_range is thrown if given position is invalid
     * @param aPos The character position to start erasing from
     * @param aCharCount number of characters to erase.
     */
    OSN_IMPORT void erase(long aPos, long aCharCount);

    /**
     * Returns unicode at given position in string
     *
     * @since S60 5.0
     * @exception std:out_of_range is thrown if given position is invalid
     * @param aPos The requested position
     */
    OSN_IMPORT Unicode operator[](long aPos);

    /**
     * Checks whether string is null.
     *
     * @since S60 5.0
     * @return True if string is null.
     */
    OSN_IMPORT bool isNull()const;

private: // data

    /**
     * Implementation.
     */
    auto_ptr<UStringImpl> mImpl;
    };


/**
 *  @class UtfProxy ustring.h "osn/ustring.h"
 *  Proxy class to ease utf encoded string memory management.
 *
 *  @lib osncore.lib
 *  @since S60 5.0
 *  @status Draft
 */
template<typename T>
class UtfProxy
    {
public:
    /**
     * Constructor for templated UtfProxy class
     */
    explicit UtfProxy(T* aString):iUtf(aString){}

    /**
     * Destructor
     */
    OSN_IMPORT ~UtfProxy();
 
    /**
     * Raw data
     *
     * @return const pointer to templated class
     * @since S60 5.0
     */
    const T* getUtf()const{return iUtf;}
private:
    /**
     * Default constructor disabled
     */
    UtfProxy();
    /**
     * Copy constructor disabled
     */
    UtfProxy(const UtfProxy& a);
    /**
     * Assignment disabled
     */
    UtfProxy& operator=(const UtfProxy& a);

private:
    /**
     * pointer to templated class
     */
    T* iUtf;
    };

typedef UtfProxy<Utf16> Utf16Proxy;
typedef UtfProxy<Utf8> Utf8Proxy;

// Non member functions for UString type

/**
 * Convert a string from UTF-8 to UTF-16. A 0 word will be added to the result after the converted text.
 *
 * @since S60 5.0
 * @param aSourceUtf8 Non empty string object.
 * @return auto_ptr to proxy object holding converted string.
 */
OSN_IMPORT auto_ptr<Utf16Proxy> toUtf16(const UString& aSourceUtf8);

/**
 * Converts a string to int.If no conversion could be performed, UString::InvalidUtf8 exception is returned.
 *
 * @since S60 5.0
 * @param aSource source string
 * @return integer
 */
OSN_IMPORT int toInt(const UString& aSource);

/**
 * Normalize a unnormalized text. Characters are decomposed by canonical equivalence e.g û -> u^
 *
 * @since S60 5.0
 * @param aSource source string
 * @return auto_ptr holding UString object.
 */
OSN_IMPORT auto_ptr<UString> normalizeNFD(const UString& aSource);

/**
 * Normalize a unnormalized text. Does canonical decomposition,followed by canonical composition.
 * Resulting text is to be a canonical equivalent to the original unnormalized text.
 *
 * @since S60 5.0
 * @param aSource source string
 * @return auto_ptr holding UString object.
 */
OSN_IMPORT auto_ptr<UString> normalizeNFC(const UString& aSource);

/**
 * Normalize a unnormalized text. Does compatibility decomposition.
 * Resulting text is to be a compatibility equivalent to the original unnormalized text.
 *
 * @since S60 5.0
 * @param aSource source string
 * @return auto_ptr holding UString object.
 */
OSN_IMPORT auto_ptr<UString> normalizeNFKD(const UString& aSource);

/**
 * Normalize a unnormalized text. Does compatibility decomposition,followed by compatibility composition.
 * Resulting text is to be a compatibility equivalent to the original unnormalized text.
 *
 * @since S60 5.0
 * @param aSource source string
 * @return auto_ptr holding UString object.
 */
OSN_IMPORT auto_ptr<UString> normalizeNFKC(const UString& aSource);
}


#endif
