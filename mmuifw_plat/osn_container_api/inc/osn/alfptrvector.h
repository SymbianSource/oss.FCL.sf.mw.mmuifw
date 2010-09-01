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


#ifndef ALF_PTRVECTOR_H
#define ALF_PTRVECTOR_H


#include <osn/osntypes.h>
#include <osn/alfptrvectorimpl.h>

namespace osncore
{

/**
*  @deprecated Use STL vector
*  @class AlfPtrVector alfptrvector.h "osn/alfptrvector.h"
*  Template class for a pointer vector
*
*  @lib osncore.lib
*  @since S60 5.0
*  @status Draft
*  @interfaces AlfPtrVector
*/
template<class T>
class AlfPtrVector: public IDeleter
    {
public:
    /**
     * @deprecated Use STL vector
     * Default constructor for vector.
     * By default contents are automatically deleted
     * when destructor is called.
     *
     * @since S60 5.0
     */
    AlfPtrVector() : mImpl(*this),mDelItem(true) {}

    /**
     * @deprecated Use STL vector
     * Constructor for vector with some default size.
     * Size can be increased or decreased afterwards.
     *
     * @since S60 5.0
     * @param aSize Default size of vector.
     */
    explicit AlfPtrVector(uint aSize) : mImpl(aSize,*this),mDelItem(true) {}

    /**
     * @deprecated Use STL vector
     * Destructor.
     */
     ~AlfPtrVector()  { if (mDelItem) { mImpl.clear(mDelItem); } }

    /**
     * @deprecated Use STL vector
     * Returns the value of auto delete property.
     *
     * @since S60 5.0
     * @return true or false.
     */
     bool autoDelete() { return mDelItem; }

    /**
     * @deprecated Use STL vector
     * Sets the auto delete property of vector.
     *
     * @since S60 5.0
     * @param aAutoDelete Value(true/false) of the property.Default value is true.
     */
     void setAutoDelete(bool aAutoDelete=true) { mDelItem = aAutoDelete; }

    /**
     * Deletes all items of vector.
     *
     * @since S60 5.0
     */
     void clear() { mImpl.clear(mDelItem); } 

    /**
     * @deprecated Use STL vector
     * Checks if there are some items in vector.
     *
     * @since S60 5.0
     * @return true or false.
     */
     bool isEmpty()  const { return mImpl.isEmpty(); } 

    /**
     * @deprecated Use STL vector
     * Returns number of items in vector.
     *
     * @since S60 5.0
     * @return number of items in vector.
     */
     uint count()  const  { return mImpl.count(); }

    /**
     * @deprecated Use STL vector
     * Returns the size in bytes occupied by vector.
     *
     * @since S60 5.0
     * @return number of bytes used by vector.
     */
     uint size()  const  { return mImpl.size(); }

    /**
     * @deprecated Use STL vector
     * Removes the item from vector at given position.
     * Item will be automatically deleted if auto delete
     * property is set to true.
     *
     * @since S60 5.0
     * @param aPos Position of the item to be removed.
     * @return true/false to indicate if item has been removed.
     */
     bool remove(uint aPos) { return mImpl.remove(aPos, mDelItem); }

    /**
     * @deprecated Use STL vector
     * Resizes the vector to new size. Existing items in
     * the vector will be restored if auto delete property is false.
     *
     * @since S60 5.0
     * @param aSize New size in bytes.
     * @return true/false to indicate if item has been resized.
     */
     bool resize(uint aSize) { return mImpl.resize(aSize, mDelItem); }

    /**
     * @deprecated Use STL vector
     * Inserts a new item at given position in the vector.
     *
     * @since S60 5.0
     * @param aPos Position at which item has to be inserted.
     * @param aItem Pointer to the item to be inserted.
     * @return true/false to indicate if the item has been inserted.
     */
     bool insert(uint aPos, T *aItem) { return mImpl.insert(aPos, aItem, mDelItem); }

    /**
     * @deprecated Use STL vector
     * Returns pointer to the item at given position in the vector.
     *
     * @since S60 5.0
     * @param aPos Position from which item is queried.
     * @return Pointer to the item.
     */
     T *at(int aPos) const {return (T *)mImpl.at(aPos); }

    /**
     * @deprecated Use STL vector
     * Returns pointer to the raw data used by vector to stroe the items.
     *
     * @since S60 5.0
     * @return Pointer to data.
     */
     T **data() {return (T **)mImpl.data(); } 

    /**
     * @deprecated Use STL vector
     * Returns the position of the item in the vector.
     *
     * @since S60 5.0
     * @param aItem Pointer to the item being searched in the vector.
     * @return Position of item in the vector.-1 is returned if item is not found.
     */
     int findRef(T *aItem){return mImpl.findRef(aItem);}

    /**
     * @deprecated Use STL vector
     * Returns the item at given index in the vector.
     *
     * @since S60 5.0
     * @param aPos Position from which item has been queried.
     * @return Pointer to the item at given position.
     */
     T* operator[](uint aPos) const  { return (T *)mImpl.at(aPos); }

private:
    void deleteItem(void* aItem)
        {
        delete (T*) aItem;
        }
    AlfPtrVector(const AlfPtrVector&);
    AlfPtrVector& operator=(const AlfPtrVector&);
private:
    /**
     * Owned body
     */
    AlfPtrVectorImpl mImpl;
    /**
     * Auto delete
     */
    bool mDelItem;

    };

}
#endif
