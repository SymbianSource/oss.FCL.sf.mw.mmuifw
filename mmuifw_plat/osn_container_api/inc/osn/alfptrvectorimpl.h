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
* Description:  vector implementation
 *
*/



#ifndef ALFPTRVECTORIMPL_H
#define ALFPTRVECTORIMPL_H

#include <osn/osndefines.h>
#include <osn/osntypes.h>

namespace osncore
{

class IDeleter
    {
private:
    friend class AlfPtrVectorImpl;
    virtual void deleteItem(void* aItem)=0;
    };


/**
 *  @deprecated Use STL vector
 *  @class AlfPtrVectorImpl alfptrvectorimpl.h "osn/alfptrvectorimpl.h"
 *  Implementation class for AlfPtrVector
 *
 *  @lib osncore.lib
 *  @since S60 5.0
 *  @status Draft
 */
class AlfPtrVectorImpl
    {
public: // constructors / destructor

    /**
     * @deprecated Use STL vector
     * Constructor
     *
     * @since S60 5.0
     * @param aDeleter
     */
    OSN_IMPORT AlfPtrVectorImpl( IDeleter& aDeleter );

    /**
     * @deprecated Use STL vector
     * Constructor
     *
     * @since S60 5.0
     * @param aSize
     * @param aDeleter
     */
    OSN_IMPORT AlfPtrVectorImpl(uint aSize, IDeleter& aDeleter);

    /**
     * @deprecated Use STL vector
     * Destructor
     */
    OSN_IMPORT ~AlfPtrVectorImpl();

    /**
     * @deprecated Use STL vector
     * Clears the vector
     *
     * @since S60 5.0
     * @param aDelItems determines whether the items are deleted
     */
    OSN_IMPORT void clear(bool aDelItems);

    /**
     * @deprecated Use STL vector
     * Removes an item from the vector
     *
     * @since S60 5.0
     * @param aCount the item to be deleted
     * @param aDelItems determines whether the item is deleted
     * @return true/false to indicate if item has been removed.
     */
    OSN_IMPORT bool remove(uint aCount, bool aDelItems);

    /**
     * @deprecated Use STL vector
     * Resizes the vector to new size. Existing items in
     * the vector will be restored if auto delete property is false.
     *
     * @since S60 5.0
     * @param aSize New size in bytes.
     * @param aDelItems determines whether the items are deleted
     * @return true/false to indicate if item has been resized.
     */
    OSN_IMPORT bool resize(uint aSize, bool aDelItems);

    /**
     * @deprecated Use STL vector
     * Inserts a new item at given position in the vector.
     *
     * @since S60 5.0
     * @param aCount Position at which item has to be inserted.
     * @param aItem Pointer to the item to be inserted.
     * @param aDelItems determines whether the item is deleted, not used!
     * @return true/false to indicate if the item has been inserted.
     */
    OSN_IMPORT bool insert(uint aCount, void *aItem, bool aDelItems);

    /**
     * @deprecated Use STL vector
     * Returns the position of the item in the vector.
     *
     * @since S60 5.0
     * @param aItem Pointer to the item being searched in the vector.
     * @return Position of item in the vector.-1 is returned if item is not found.
     */
    OSN_IMPORT int findRef(void *aItem);

public: // inliners
    bool isEmpty() const { return mcount == 0; }
    uint count() const { return mcount; }
    uint size() const { return msize; }
    void* at(uint aCount) const { return mdata[aCount]; }
    void** data() { return mdata; }


private:
    AlfPtrVectorImpl& operator=(const AlfPtrVectorImpl&);

    void** mdata;
    uint msize;
    uint mcount;
    IDeleter& mDeleter;
    };

} // namespace core
#endif
