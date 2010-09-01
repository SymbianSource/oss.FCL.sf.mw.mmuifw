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




#ifndef ALFMODEL_H
#define ALFMODEL_H


//INCLUDES
#include <osn/osncommon.h>
#include <osn/osndefines.h>
#include <alf/ialfmodel.h>
#include <memory>
namespace osncore
    {
template <class T> class AlfPtrVector;
    }
namespace std
    {
template <class T> class auto_ptr;
    }

using namespace osncore;
using namespace std;

namespace Alf
    {

/* Forward declarations*/
class IAlfVariantType;
class IAlfModelChangeObserver;
class AlfModelOperation;
class AlfWidgetImpl;
class AlfModelImpl;
class IfId;


/** @class AlfModel alfmodel.h "alf/alfmodel.h"
 * Concerete implementation of the IAlfModel
 *
 *  @interfaces IAlfModel
 *  @lib alfwidgetmodel.lib
 *  @since S60 ?S60_version
 *  @status Draft
 */
class AlfModel : public IAlfModel
    {
public:

    /**
     * Constructor.
     * @exception std::bad_alloc
     *
     * @param aDataSource - datasource from where the model is initialized,
     *        ownership is transferred to the model
     * @since S60 ?S60_version
     * @return New object.
     */
    OSN_IMPORT AlfModel( IAlfVariantType* aDataSource);


    /**
     * The Default constructor.
     *
     * @since S60 ?S60_version
     */
    OSN_IMPORT AlfModel();

    /**
     * Class Destructor.
     *
     * @since S60 ?S60_version
     */
    OSN_IMPORT virtual ~AlfModel();


    //IAlfModel virtual Methods
    
    /**
     * From IAlfModel.
     * Add a new observer to be notified of any changes in the model.
     *
     * @exception std::bad_alloc
     *
     * @since S60 ?S60_version
     * @param aObserver The model change observer to be added.
     *
     * @return void
     */
    OSN_IMPORT virtual void addModelChangeObserver(
                           IAlfModelChangeObserver &aObserver);

    /**
     * From IAlfModel.
     * Remove an observer which is added to get notified of any changes 
     * in the model.
     *
     * @since S60 ?S60_version
     * @param aObserver The model change observer to be removed.
     *
     * @return void
     */
    OSN_IMPORT virtual void removeModelChangeObserver(
                           IAlfModelChangeObserver &aObserver);


    /**
     * From IAlfModel.
     * Get the root data object of this model.
     *
     * @since S60 ?S60_version
     * @return The root data object.
     */
    OSN_IMPORT virtual IAlfVariantType* data() const;

    /**
     * From IAlfModel.
     * Clears the Model.
     *
     * @since S60 ?S60_version
     * @return void
     */
    OSN_IMPORT virtual void clearModel();


    /**
     * Api to change the model completely at one short. From IAlfModel
     *
     * @since S60 ?S60_version
     * @param aData The new data.
     */
    OSN_IMPORT void setData(IAlfVariantType* aData);

    /**
     * Change a given  of data fields in the model . All model change
     * observers are notified about the change.
     * @exception osncore::AlfDataException Thrown with the error code 
     *     osncore::EInvalidModelOperation when the operation fails.
     *
     * @since S60 ?S60_version
     * @param aNumContainerIndices The number of indices in aContainerIndices.
     * @param aContainerIndices The container indices to find the correct
     *            location in the data hierarchy. The ownership is transferred.
     * @param aData - The new data to be set
     */
    OSN_IMPORT void updateData(
                   int aNumContainerIndices,
                   int* aContainerIndices,
                   IAlfVariantType* aData ) ;

    /**
     * Add a  number of data fields in the model data. All model change
     * observers are notified about the change.
     *
     * @exception osncore::AlfDataException Thrown with the error code 
     *     osncore::EInvalidModelOperation when the operation fails.
     *
     * @since S60 ?S60_version
     * @param aNumContainerIndices The number of indices in aContainerIndices.
     * @param aContainerIndices The container indices to find the correct 
     *            location in the data hierarchy. The ownership is transferred.
     *
     * @param aData The new data.
     */
    OSN_IMPORT void addData(
                   int aNumContainerIndices,
                   int* aContainerIndices,
                   IAlfVariantType* aData );

    /**
     * Remove a  number of data fields in the model data. All model change
     * observers are notified about the change.
     * @exception osncore::AlfDataException Thrown with the error code 
     *     osncore::EInvalidModelOperation when the operation fails.
     *
     * @since S60 ?S60_version
     * @param aNumContainerIndices The number of indices in aContainerIndices.
     * @param aContainerIndices The container indices to find the correct
     *            location in the data hierarchy. The ownership is transferred.
     */
    OSN_IMPORT void removeData(
                   int aNumContainerIndices,
                   int* aContainerIndices) ;

    /**
     * Api for executing batch operations
     *
     * @exception osncore::AlfDataException Thrown with the error code 
     *     osncore::EInvalidModelOperation when the operation fails.
     *
     * @since S60 ?S60_version
     * @param aOperationsArray  - An RPOinterArray containing the list of 
     *            operations to be executed in batch.
     */
    OSN_IMPORT  void executeOperations(
                    AlfPtrVector<AlfModelOperation>&  aOperationsArray);

    /**
     * Api for executing one operation at a Time
     *
     * @since S60 ?S60_version
     * @param aOperation    - An Operation to be executed on the model.
     */
    OSN_IMPORT void executeOperation(AlfModelOperation* aOperation);

    /**
     * Notifies the observers, that the model has changed.
     */
    OSN_IMPORT void notifyModelChanged();

    
// from base class IAlfInterfaceBase

    /**
     * From IAlfInterfaceBase.
     * Getter for interfaces provided by the widget classes.
     * Derived classes should always call the base class method
     * from the overridden makeInterface.
     *
     * @since S60 ?S60_version
     * @param aType A descriptor to identify the type of the queried interface.
     *
     * @return The queried interface, or NULL if the interface is not
     *         supported or available.
     */
    virtual IAlfInterfaceBase* makeInterface( const IfId& aType );


protected:

private:
    /**
     * Internal addDataL method .
     * @param aAddOperation - An Addoperation
     * @since S60 ?S60_version
     */
    void addData(AlfModelOperation* aAddOperation);

    /**
     * Internal removeDataL method .
     * @param aRemoveOperation - A remove operation
     * @since S60 ?S60_version
     */
    void removeData(AlfModelOperation* aRemoveOperation);

    /**
     * Internal updateDataL method .
     * @param aUpdateOperation - An update operation
     * @since S60 ?S60_version
     */
    void updateData(AlfModelOperation* aUpdateOperation);

private:

    auto_ptr<AlfModelImpl> mData;

    };

    } // namespace Alf

#endif // ALFMODEL_H
