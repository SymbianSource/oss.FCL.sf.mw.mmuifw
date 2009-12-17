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




#ifndef ALFMODELOPERATION_H
#define ALFMODELOPERATION_H


//INCLUDES
#include <osn/osndefines.h>
#include <osn/ustring.h>

using namespace osncore;

namespace Alf
    {

/* Forward declarations*/
class IAlfVariantType;

/** @class AlfModelOperation alfmodeloperation.h "alf/alfmodeloperation.h"
*  A class for storing information about changes in the model.
*  CAlfModelOperation can be used to make changes in a batch mode,
*  i.e., executing multiple changes in one call.
*  @lib hitchcockwidgetmodel.lib
*  @since S60 ?S60_version
*  @status Draft
*/
class AlfModelOperation
    {
public:

    /** The operation constants. */
    enum OperationType{EOperationAdd, EOperationRemove, EOperationUpdate};

    /**
     * constructor. The new operation is left on the cleanup stack.
     *
	 * @exception std::bad_alloc     
     * @since S60 ?S60_version
     * @param aOperation The operation to be performed.
     * @param aNumContainerIndices The number of indices in aContainerIndices.
     * @param aContainerIndices The container indices to find the correct location
     *                          in the data hierarchy. The ownership is transferred.
     * @param aData The new data in EOperationAdd and EOperationUpdate operations.
     *              The ownership is transferred.
     */
    OSN_IMPORT static AlfModelOperation* create( OperationType aOperation,
            int aNumContainerIndices,
            int* aContainerIndices,
            IAlfVariantType* aData = 0 );

    /**
     * constructor for adding or updating a map item.
     * The new operation is left on the cleanup stack.
     *
	 * @exception std::bad_alloc     
     * @since S60 ?S60_version
     * @param aOperation The operation to be performed.
     * @param aNumContainerIndices The number of indices in aContainerIndices.
     * @param aContainerIndices The container indices to find the correct location
     *                          in the data hierarchy. The ownership is transferred.
     * @param aData The new data. The ownership is transferred.
     * @param aDataName The name of the new data in EOperationAdd and EOperationUpdate
     *                  operations. Only needed when adding or updating a map item.
     */
    OSN_IMPORT static AlfModelOperation* create( OperationType aOperation,
            int aNumContainerIndices,
            int* aContainerIndices,
            IAlfVariantType* aData,
            const UString& aDataName );

    /** Destructor.*/
    OSN_IMPORT ~AlfModelOperation();

    /**
     * Get the name of the new data field in the container map.
     * Panics if the operation is EOperationRemove.
     *
     * @exception osncore::AlfDataException Thrown with the error code osncore::EInvalidModelOperation\n 
     *										  when wrong operation is performed on Model.
     * @return The new data.
     * @since S60 ?S60_version
     */
    OSN_IMPORT const UString& newDataName() const;

    /**
     * Get the new data used in EOperationAdd or EOperationUpdate operations.
     * Panics if the operation is EOperationRemove.
     *
     * @exception osncore::AlfDataException Thrown with the error code osncore::EInvalidModelOperation\n 
     *										  when wrong operation is performed on Model.
     * @return The new data.
     * @since S60 ?S60_version
     */
    OSN_IMPORT IAlfVariantType& newData() const;

    /**
     * Get the new data used in EOperationAdd or EOperationUpdate operations.
     * Panics if the operation is EOperationRemove.
     * The ownership of the data is transferred to the caller,
     * and the data of this operation is set to NULL.
     *
     * @exception osncore::AlfDataException Thrown with the error code osncore::EInvalidModelOperation\n 
     *										  when wrong operation is performed on Model.
     * @return The new data.
     * @since S60 ?S60_version
     */
    OSN_IMPORT IAlfVariantType* getNewData();

    /**
     * Get the Operation.
     *
     * @return The new data.
     * @since S60 ?S60_version
     */
    OSN_IMPORT AlfModelOperation::OperationType operation() const;

    /**
     * Get the parent data for the operation. The parent data
     * is a container for the data to be added, removed or updated.
     * The index of the data can be queried using Index().
     *
     * @exception osncore::AlfDataException Thrown with the error code osncore::EInvalidContainerOperation\n 
     *										  when wrong operation is performed on Model.
     * @param aRoot The root of the data hierarchy to perform the operation.
     * @return The parent data.
     * @since S60 ?S60_version
     */
    OSN_IMPORT IAlfVariantType* parentData(IAlfVariantType& aRoot ) const;

    /**
     * Get the index of the data inside the parent container.
     * The parent container can be queried using ParentData().
     * If the parent container is a branch, index 0 refers to
     * the branch data, and index 1 refers to the child data.
     *
     * @return The new data.
     * @since S60 ?S60_version
     */
    OSN_IMPORT int index() const;

    /**
     * Get the number of container indices to find the
     * location in the data hierarchy.
     *
     * @return The number of container indices.
     * @since S60 ?S60_version
     */
    OSN_IMPORT int numContainerIndices() const;

    /**
     * Get the container index to traverse through the data hierarchy.
     * If the container in the hierarchy is a branch, index 0 refers to
     * the branch data, and index 1 refers to the child data.
     *
     * @return The container index.
     * @since S60 ?S60_version
     */
    OSN_IMPORT int containerIndex( int aIndex ) const;

private:

    AlfModelOperation(
        OperationType aOperation,
        int aNumContainerIndices,
        int* aContainerIndices,
        IAlfVariantType* aData = 0 );

    AlfModelOperation(
        OperationType aOperation,
        int aNumContainerIndices,
        int* aContainerIndices,
        IAlfVariantType* aData,
        const UString& aDataName );

private: // data

    // Target location in the data hierarchy
    int mNumContainerIndices;
    int* mContainerIndices;

    // The new data
    IAlfVariantType* mData;
    // The name of the new data, if adding or updating a map item
    UString mDataName;

    // The operation to be performed
    OperationType mOperation;
    };

    } // namespace Alf

#endif // ALFMODELOPERATION_H
