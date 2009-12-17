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



#ifndef I_ALFMAP_H
#define I_ALFMAP_H


//  INCLUDES
#include <alf/ialfvarianttype.h>
#include <osn/osntypes.h>

namespace osncore
    {
class UString;
    }

using osncore::UString;

namespace Alf
    {
/**
 * An interface for Container data type.
 *
 * @lib hitchcockwidgetmodel.lib
 * @since S60 ?S60_version
 * @status Draft
 */
class IAlfMap : public IAlfVariantType
    {
public:


    /**
     * Gets the Number of items in  the container
     *
     * @return number of items in the container
     * @since S60 ?S60_version
     */
    virtual int count() const = 0;

    /**
     * Gets the Item at a given index in  the container
     * @param aIndex - index at which the element is required
     * @return item as a MlfVariantType
     * @since S60 ?S60_version
     */
    virtual IAlfVariantType* item(uint aIndex) = 0;

    /**
     * Gets the Name at a given index in  the container
     * @param aIndex - index at which the name of the element in the map  is required
     * @return item name as a TDesC8
     * @since S60 ?S60_version
     */
    virtual const UString& name(uint aIndex) const = 0;

    /**
     * Clones the  entire container
     *
     * @return the cloned container
     * @since S60 ?S60_version
     */
    virtual IAlfMap* clone() = 0;

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
     * Adds a variantData item into the container
     * @param aData - varaint Data Item to be added into the container
     * @param @Name - Name associated with this Variant Data Item.
     * @return void
     * @since S60 ?S60_version
     */
    virtual void addItem(IAlfVariantType* aData, const UString& aName)=0;

    /**
     * Adds a variantData item into the container
     * @param aIndex index where the data is added
     * @param aData - variant Data Item to be added into the container
     * @param @Name - Name associated with this Variant Data Item.
     * @return void
     * @since S60 ?S60_version
     */
    virtual void addItem(uint aIndex, IAlfVariantType* aData, const UString& aName)=0;

    /**
     * Gets the item by the given name.
     * @param aName The name associated with requested item.
     * @return The value corresponding to the name of the item.
     * @since S60 ?S60_version
     */
    virtual IAlfVariantType* item(const UString& aName) = 0;

    /**
     * Gets the index of the item by the given name.
     * @param aName The name associated with requested item.
     * @return The index corresponding to the name of the item, or -1 if not found.
     * @since S60 ?S60_version
     */
    virtual int itemIndex(const UString& aName) = 0;

    /**
     * Removes the Item associated with a name
     * @param aName - Name associated with this Variant Data Item that is required to be removed in the map
     * @return void
     * @since S60 ?S60_version
     */
    virtual void removeItem(const UString& aName) = 0;

    /**
     * Replaces the Item associated with a name in the container with a new variant data type
     * @param aName - Name associated with this Variant Data Item that is required to be replaced in the map
     * @aNewData     - New Data to be set
     * @return void
     * @since S60 ?S60_version
     */
    virtual void replaceItem(const UString& aName,IAlfVariantType* aNewData) = 0;

    /**
     * Destructor
     *
     * @since S60 ?S60_version
     */
    virtual ~IAlfMap(){}
    };

    } // namespace Alf

#endif // I_ALFMAP_H

// End of File
