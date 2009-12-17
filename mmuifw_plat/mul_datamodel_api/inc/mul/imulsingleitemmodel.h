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
* Description:  Interface for single item model
 *
*/

 
#ifndef IMULSINGLEITEMMODEL_H_
#define IMULSINGLEITEMMODEL_H_

// Includes
#include <alf/ialfmodel.h>

namespace osncore
    {
// Forward Declarations
class UString;
    }
using namespace osncore;

namespace Alf
    {

// Forward Declarations
class MulVisualItem;

namespace mulsingleitemmodel
    {
    static const int KLastIndex = -1;
    static const int KNumberOfIndices = 4;  
    
    static const char* const KBaseElementName = "base";

    static const char* const KInvalidVisualItem = "NULL Visual Item passed";
    static const char* const KIndexOutOfBound = "Index out of bound";
    static const char* const KLogicError = "Logic error";
    }// namespace mulsingleitemmodel
    
class IMulSingleItemModel : public IAlfModel
    {

public:
    
    /**
     * Adds an item into the specified container at the given index.
     *
     * @param aItem Item to be added. Takes ownership.
     * @param aContainerName Name of the container.
     * @param aIndex Index of the item in the container. By default appends item.
     * @throw Invalid Argument, incase visual item is NULL or out of bound index.
     */
    virtual void AddData(MulVisualItem* aItem, const UString& aContainerName, 
                         int aIndex = mulsingleitemmodel::KLastIndex) = 0;
    
    /**
     * Update an item within the specified container at the given index.
     *
     * @param aItem Item to be updated. Takes ownership.
     * @param aContainerName Name of the container.
     * @param aIndex Index of the item in the container.
     * @throw Invalid Argument, incase visual item is NULL, invalid container or out of bound index.
     * 		  Logic Error, cannot add data because of invalid internal structure
     */
    virtual void UpdateData(MulVisualItem& aItem, const UString& aContainerName, int aIndex) = 0;
    
    /**
     * Removes an item from the specified container at the given index.
     *
     * @param aContainerName Name of the container.
     * @param aIndex Index of the item in the container.
     * @throw Invalid Argument, incase of invalid container or out of bound index.
     */
    virtual void RemoveData(const UString& aContainer, int aIndex) = 0;
    
    /**
     * Returns an item from the specified container at the given index.
     *
     * @param aContainerName Name of the container.
     * @param aIndex Index of the item in the container.
     * @return Reference to the item.
     * @throw Invalid Argument, incase of invalid container or out of bound index.
     */
    virtual MulVisualItem& Data(const UString& aContainerName, int aIndex) = 0;
    
    /**
     * Returns number of items in the specified container.
     *
     * @param aContainerName Name of the container.
     * @return Number of items
     * @throw Invalid Argument, incase of invalid container.
     */
    virtual int Count(const UString& aContainerName) = 0;
    
    /** 
	 * Virtual destructor.
	 */
    virtual ~IMulSingleItemModel() {}
    
    };// class IMulSingleItemModel
        
    }// namespace Alf
#endif // IMULSINGLEITEMMODEL_H_

// End of file
