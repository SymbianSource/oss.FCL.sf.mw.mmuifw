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
* Description:  Defines a branch data type for the data hierarchy
*
*/



#ifndef I_ALFBRANCH_H
#define I_ALFBRANCH_H


//  INCLUDES
#include <alf/ialfvarianttype.h>

namespace osncore
    {
class UString;
    }

using osncore::UString;

namespace Alf
    {
class IAlfMap;
class IAlfSortFunction;

/**
 * An interface for Container data type.
 *
 * @lib hitchcockwidgetmodel.lib
 * @since S60 ?S60_version
 * @status Draft
 */
class IAlfBranch : public IAlfVariantType
    {
public:

    /**
     * The data for this branch
     *
     * @return the data for this branch as a Map Interface pointer
     * @since S60 ?S60_version
     */
    virtual IAlfMap* data() = 0;

    /**
     * Set the data for this branch.
     * @param aData The new data for this branch.
     * @since S60 ?S60_version
     */
    virtual void setData( IAlfMap* aData ) = 0;

    /**
     * The data for a child at a given index
     * @param aIndex- index of the child at which the data needs to be fetched
     * @return the data for its children as a Map Interface pointer
     * @since S60 ?S60_version
     */
    virtual IAlfVariantType* childData(int aIndex) = 0;

    /**
     * APi to get the childName
     * @param aIndex- index of the child at which the data needs to be fetched
     * @return the name  for the child at the given index
     * @since S60 ?S60_version
     */
    virtual const UString& childName(int aIndex) = 0;

    /**
     * The data for all children.
     * @return the data for its children as a Map Interface pointer
     * @since S60 ?S60_version
     */
    virtual IAlfMap* childData() = 0;

    /**
     * Set the data for children of this branch.
     * @param aChildData The new data for the children.
     * @since S60 ?S60_version
     */
    virtual void setChildData( IAlfMap* aChildData ) = 0;

    /**
     * Get the number of children in this branch
     * @return The number of children
     * @since S60 ?S60_version
     */
    virtual uint childrenCount() = 0;

    /**
     * Destructor
     * @since S60 ?S60_version
     */
    virtual ~IAlfBranch() {};

    /**
     * Sorts the branch using user defined sort function.
     *
     * @param aSortFunction - sort function.
     * @since S60 ?S60_version
     */
    virtual void sort( const IAlfSortFunction& aSortFunction ) = 0;

    /**
     * Function to Clone the Branch
     * @since S60 ?S60_version
     * @return The clone
     */
    virtual IAlfBranch* clone() = 0;

    };

    } // namespace Alf

#endif // I_ALFBRANCH_H

// End of File
