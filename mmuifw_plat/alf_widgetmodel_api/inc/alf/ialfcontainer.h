/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  This interface defines a generic data type
*
*/



#ifndef I_ALFCONTAINER_H
#define I_ALFCONTAINER_H


//  INCLUDES
#include <alf/ialfvarianttype.h>

namespace Alf
    {

// FORWARD DECLARATIONS
class IAlfSortFunction;

/**
 * An interface for Container data type.
 * @lib hitchcockwidgetmodel.lib
 * @since S60 ?S60_version
 * @status Draft
 */
class IAlfContainer : public IAlfVariantType
    {
public:

    /**
     * Adds a variantData item into the container
     * @param aData variant data to be added into the container
     * @return void.
     * @since S60 ?S60_version
     */
    virtual void addItem(IAlfVariantType* aData)=0;

    /**
     * Adds a variantData item into the container
     * @param aIndex index where the data is added
     * @param aData variant data to be added into the container
     * @return void.
     * @since S60 ?S60_version
     */
    virtual void addItem(uint aIndex, IAlfVariantType* aData)=0;

    /**
     * Gets the Number of items in  the container
     *
     * @return number of items in the container
     * @since S60 ?S60_version
     */
    virtual int count() = 0;

    /**
     * Gets the Item at a given index in  the container
     * @param aIndex - index at which the element is required
     * @return item as a MlfVariantType
     * @since S60 ?S60_version
     */
    virtual IAlfVariantType* item(uint aIndex) = 0;

    /**
     * Clones the  entire container
     *
     * @return the cloned container
     * @since S60 ?S60_version
     */
    virtual IAlfContainer* clone() = 0;


    /**
     * Removes  an item from  given index in  the container
     * @param aIndex - index at which the element is to be removed
     * @return item as a MlfVariantType
     * @since S60 ?S60_version
     */
    virtual void removeItem(uint aIndex) = 0;

    /**
     * Clear the Container
     *
     * @return void
     * @since S60 ?S60_version
     */
    virtual void clear() = 0;

    /**
     * Replace  an item from  given index in  the container
     * @param aIndex - index at which the element is to be replaced
     * @param aNewData - new Data to be replaced
     * @return void
     * @since S60 ?S60_version
     */
    virtual void replaceItem(uint aIndex, IAlfVariantType* aNewData ) = 0;

    /**
     * sorts the container using user defined sort function.
     *
     * @param aSortFunction - sort function.
     * @since S60 ?S60_version
     */
    virtual void sort( const IAlfSortFunction& aSortFunction ) = 0;

    /**
     * Destructor
     *
     * @since S60 ?S60_version
     */
    virtual ~IAlfContainer(){}
    };


    } // namespace Alf

#endif // I_ALFCONTAINER_H

// End of File
