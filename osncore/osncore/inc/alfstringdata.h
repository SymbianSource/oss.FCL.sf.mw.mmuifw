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
* Description: OSN string data header. 
*
*/

#ifndef ALFSTRINGDATA_H
#define ALFSTRINGDATA_H

#include <osn/osntypes.h>

#include "stringmacros.h"


namespace osncore
    {

class AlfChar;

class CAlfString;

struct SAlfStringData
    {
    // Uses shared null data.
    SAlfStringData();
    
    // No copy.
  //  SAlfStringData(AlfChar *u, uint l, uint m);
  //  void initialize(AlfChar *u, uint l, uint m);

    // Copy bytes.
  //  SAlfStringData(const AlfChar *u, uint l);
  //  void initialize(const AlfChar *u, uint l);

    
    void initialize(const char *u, int l);

   
    ~SAlfStringData();
/*
#ifdef QSTRING_DEBUG_ALLOCATIONS
    void* operator new(size_t s);
    void operator delete(void*p);
#endif
*/
    inline void ref() { refCount++; }
    inline void deref() { /*if (--refCount == 0 && _isHeapAllocated) delete this;*/ refCount--; }
    
    char* ascii();
   
    char *makeAscii();
    
  
    
    bool increaseAsciiSize(uint size);

  //  AlfChar *unicode();
  //  AlfChar *makeUnicode();
   

  //  bool isUnicodeInternal() const { return (char *)_unicode == _internalBuffer; }
    bool isAsciiInternal() const { return _ascii == _internalBuffer; }

    int refCount;
    int _length;
  //  mutable AlfChar *_unicode;
    mutable char *_ascii;
    int _maxUnicode:30;
    int _isUnicodeValid:1;
    int _isHeapAllocated:1;	// Fragile, but the only way we can be sure the instance was created with 'new'.
    int _maxAscii:31;
    int _isAsciiValid:1;

    char _internalBuffer[QS_INTERNAL_BUFFER_SIZE]; // Pad out to a (((size + 1) & ~15) + 14) size
 
private:
    SAlfStringData(const SAlfStringData &);
    SAlfStringData &operator=(const SAlfStringData &);
   
    };

    }// osncore
#endif