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
* Description: OSN string data implementation.
*
*/


#include <stdio.h>
#include <stdarg.h>

#if defined(__GNUC__)
#include <string.h>
#else
#include <libc/string.h>
#endif

#include <osn/osntypes.h>

#include "alfstringdata.h"
//#include "osn/char.h"
#include <osn/osnbadalloc.h>

using namespace std;

namespace osncore
    {
        
    
   
// -------------------------------------------------------------------------
// stringData
// -------------------------------------------------------------------------
SAlfStringData::SAlfStringData() :  refCount(1), /*_unicode(0),*/ _ascii(0), _maxAscii(QS_INTERNAL_BUFFER_CHARS), _isAsciiValid(1)
{
//#ifdef QSTRING_DEBUG_ALLOCATIONS
 //   stringDataInstances++;
//#endif
	refCount = 1;
 	_length=0;
//	_unicode = 0;
	_isUnicodeValid=0;
	_isAsciiValid = 1;
	_isHeapAllocated=0;
    _ascii = _internalBuffer;
    _internalBuffer[0] = 0;
}

SAlfStringData::~SAlfStringData()
{
  //  ASSERT(refCount == 0);
 /*   if (_unicode && !isUnicodeInternal())
    {
    DELETE_QCHAR(_unicode);	
    }
   */     
    if (_ascii && !isAsciiInternal())
    {
    DELETE_CHAR(_ascii);	
    }
     
}


// Copy data
void SAlfStringData::initialize(const char *a, int l)
{
    refCount = 1;
    _length = l;
   // _unicode = 0;
    _isUnicodeValid = 0;
    _maxUnicode = 0;
    _isAsciiValid = 1;
    _isHeapAllocated = 0;

   if (l > QS_INTERNAL_BUFFER_CHARS) 
    {
        _maxAscii = ALLOC_CHAR_GOOD_SIZE(l+1);
        _ascii = ALLOC_CHAR(_maxAscii);
    //Make sure we have memory before doing data copy.
    if(_ascii)
    	{
      	if (a)
      		{
      		memcpy(_ascii, a, l);	
      		}
      	_ascii[l] = 0;
    	}
    else{
        _length = 0;
        refCount = 0;
        _length = 0;
       // _unicode = 0;
        _isUnicodeValid = 0;
        _maxUnicode = 0;
        _isAsciiValid = 1;
        _isHeapAllocated = 0;
        throw bad_alloc();
    	}
    } 
    else {
        _maxAscii = QS_INTERNAL_BUFFER_CHARS;
        _ascii = _internalBuffer;
        if (a)
            memcpy(_internalBuffer, a, l);
        _internalBuffer[l] = 0;
    }
    
   
}


bool SAlfStringData::increaseAsciiSize(uint size)
{
    //ASSERT(this != QString::shared_null);

    uint newSize = (uint)ALLOC_CHAR_GOOD_SIZE((size * 3 + 1) / 2);

	/*
    if (!_isAsciiValid)
        return false;
    */
   // ASSERT(_isAsciiValid);

    if (isAsciiInternal()) {
        char *newAscii = ALLOC_CHAR(newSize);
        if( !newAscii ) return false;
        
        if (_length)
            memcpy(newAscii, _ascii, _length);
        _ascii = newAscii;
    } else {
        char* ap = _ascii;
        _ascii = REALLOC_CHAR( _ascii, newSize );
        
        if( !_ascii )
        {
            // memory manager won't delete the original pointer if realloc failed
            _ascii = ap;
            return false;
        }        
    }

    _maxAscii = newSize;
    _isAsciiValid = 1;
    _isUnicodeValid = 0;
    return true;
}





char* SAlfStringData::ascii()
{
// return _isAsciiValid ? _ascii :0;	
return _ascii;
}


/*
AlfChar *SAlfStringData::makeUnicode()
{
    //ASSERT(this != QString::shared_null);

    if (_isAsciiValid){
        char copyBuf[QS_INTERNAL_BUFFER_CHARS];
     //   char *str;

        if (_unicode && !isUnicodeInternal())
            DELETE_QCHAR(_unicode);

        if (_length <= QS_INTERNAL_BUFFER_UCHARS){
            if (isAsciiInternal()) {
                uint i = _length;
                char *tp = &copyBuf[0], *fp = _ascii;
                while (i--)
                    *tp++ = *fp++;
              //  str = &copyBuf[0];
                _isAsciiValid = 0;
            }
            else
                {
               // str = _ascii;
                
                }
            _unicode = (AlfChar *)_internalBuffer;
            _maxUnicode = QS_INTERNAL_BUFFER_UCHARS;
        }
        else {
            uint newSize = ALLOC_QCHAR_GOOD_SIZE(_length);
            _unicode = ALLOC_QCHAR(newSize);
            if( !_unicode )
            {
                _maxUnicode = 0;
                return 0;    
            }
            _maxUnicode = newSize;
        //    str = _ascii;
        }
   //     uint i = _length;
   //     AlfChar *cp = _unicode;
   //     while ( i-- )
   //         *cp++ = *str++;

        _isUnicodeValid = 1;
    }
  //  else if (!_isUnicodeValid)
  //      FATAL("invalid character cache",0);

    return _unicode;
}

*/
} // osncore

