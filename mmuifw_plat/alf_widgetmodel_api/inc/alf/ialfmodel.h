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




#ifndef I_ALFMODEL_H
#define I_ALFMODEL_H


//INCLUDES
#include <alf/ialfinterfacebase.h>
#include <alf/alftypes.h>

namespace osncore
    {
template <class T> class AlfPtrVector;
    }
using namespace osncore;

namespace Alf
    {

/* Forward declarations*/
class IAlfModelChangeObserver;
class IAlfVariantType;
class AlfModelOperation;
class AlfCustomInitDataBase;


namespace alfmodel
    {
static const IfId ident=
    {
    0,"alfmodel"
    };
    }

/**
 * An interface of the component model.
 * @lib alfwidgetmodel.lib
 * @since S60 ?S60_version
 * @status Draft
 */

class IAlfModel: public IAlfInterfaceBase
    {
public:
    static inline const IfId& type()
        {
        return alfmodel::ident;
        }
        
    /**
     * Virtual destructor.
     *
     * @since S60 ?S60_version
     */
    virtual ~IAlfModel() {}

    /**
     * Add a new observer to be notified of any changes in the model.
     *
     * @exception std::bad_alloc
     *
     * @param aObserver The model change observer to be added.
     * @return void
     * @since S60 ?S60_version
     */
    virtual void addModelChangeObserver(
                IAlfModelChangeObserver& aObserver ) = 0;


    /**
     * Remove an observer which is added to get notifications.
     *
     * @param aObserver The model change observer to be removed.
     * @return void
     * @since S60 ?S60_version
     */
    virtual void removeModelChangeObserver(
                IAlfModelChangeObserver& aObserver )=0;

    /**
     * Api to change the model completely at one short. From IAlfModel
     *
     * @param aData  The new data.
     * @since S60 ?S60_version
     */
    virtual void setData( IAlfVariantType* aData ) = 0;

    /**
     * Change a given  of data fields in the model . All model change
     * observers are notified about the change.
     * @param aNumContainerIndices The number of indices in aContainerIndices.
     * @param aContainerIndices The container indices to find the correct
     *            location in the data hierarchy. The ownership is transferred.
     * @param aData - The new data to be set
     * @since S60 ?S60_version
     */
    virtual void updateData(
                int aNumContainerIndices,int* aContainerIndices,
                IAlfVariantType* aData ) = 0;

    /**
     * Add a  number of data fields in the model data. All model change
     * observers are notified about the change.
     *
     * @param aNumContainerIndices The number of indices in aContainerIndices.
     * @param aContainerIndices The container indices to find the correct
     *            location in the data hierarchy. The ownership is transferred.
     * @param aData The new data.
     * @since S60 ?S60_version
     */
    virtual void addData(
                int aNumContainerIndices,int* aContainerIndices,
                IAlfVariantType* aData ) = 0;

    /**
     * Remove a  number of data fields in the model data. All model change
     * observers are notified about the change.
     *
     * @param aNumContainerIndices The number of indices in aContainerIndices.
     * @param aContainerIndices The container indices to find the correct
     *            location in the data hierarchy. The ownership is transferred.
     * @since S60 ?S60_version
     */
    virtual void removeData(
            int aNumContainerIndices,
            int* aContainerIndices ) = 0;

    /**
     * Api for executing batch operations
     *
     * @param aOperationsArray  - An RPOinterArray containing the list of 
     *            operations to be executed in batch.
     * @since S60 ?S60_version
     */
    virtual void executeOperations(
                AlfPtrVector<AlfModelOperation>&  aOperationsArray) = 0;

    /**
     * Api for executing one operation at a Time
     *
     * @param aOperation    - An Operation to be executed on the model.
     * @since S60 ?S60_version
     */
    virtual void executeOperation(AlfModelOperation* aOperation) = 0;

    /**
     * Clears the Model.
     * @return void
     * @since S60 ?S60_version
     */
    virtual void clearModel() = 0;
    /**
     * Get the root data object of this model.
     *
     * @return The root data object.
     * @since S60 ?S60_version
     */
    virtual IAlfVariantType* data() const = 0;

    };

/**
 *  Placeholder for information required to instantiate a model
 *  via the widget factory mechanism.
 *  A pointer to this structure is casted to a void pointer and sent to the
 *  factory plugin.
 *  @lib alfwidgetmodel.lib
 *  @since S60 ?S60_version
 */
struct AlfModelInitData
    {
    /**
     * Pointer to custom data passed via factory mechanism
     * Not Owned.
     */
    AlfCustomInitDataBase* mCustomData;  
    };
    
    } // namespace Alf

#endif // I_ALFMODEL_H

// End of File
