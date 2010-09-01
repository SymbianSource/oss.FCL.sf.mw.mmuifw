/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:   Model Class used by widgets.
*
*/




//INCLUDES 
#include <alf/ialfmodelchangeobserver.h> 
#include <osn/alfptrvector.h>
#include <memory>

#include <alf/alfmodel.h>
#include "alf/alfmodeloperation.h"
#include <alf/alfvarianttype.h>
#include <alf/alfexceptions.h>
//#include "alf/alfperf.h"
#include <osn/osnnew.h>

using osncore::AlfPtrVector;
using std::auto_ptr;

// Forward declared inside the Alf namespace
namespace Alf
    {
    
    
class AlfModelImpl
    {
public :
	AlfModelImpl():mDataSource(0)
	    {
	    }

	~AlfModelImpl()
	    {
        delete mDataSource;
	    mObserverArray.clear();
		}
		
    /**
     * Array of Observers for the Model individual observers not owned
     */
    AlfPtrVector<IAlfModelChangeObserver> mObserverArray;
    
    /**
     * Data Source for the Model owned
     */ 
    IAlfVariantType* mDataSource;
    };


////////////////////// AlfModel //////////////////////

// ============================ MEMBER FUNCTIONS ==============================


// ---------------------------------------------------------------------------
// Description : Constructor - 
// ---------------------------------------------------------------------------
//
OSN_EXPORT AlfModel::AlfModel()
    {
    }

// ---------------------------------------------------------------------------
// Description : Constructor - 
// Data Source for the model is passed
// ---------------------------------------------------------------------------
//
OSN_EXPORT AlfModel::AlfModel( IAlfVariantType* aDataSource )
    {
    mData.reset( new(EMM) AlfModelImpl() );
    mData->mDataSource  = aDataSource;  
    mData->mObserverArray.setAutoDelete(false);
    }

// ---------------------------------------------------------------------------
// Description : Class Destructor
// ---------------------------------------------------------------------------
//
OSN_EXPORT AlfModel::~AlfModel()
    {
    }

// ---------------------------------------------------------------------------
// Description : Adds model change observers
// ---------------------------------------------------------------------------
//
OSN_EXPORT void AlfModel::addModelChangeObserver(
    IAlfModelChangeObserver& aObserver)
    {
    if(mData->mObserverArray.findRef(&aObserver)<0)
    	{
	    mData->mObserverArray.resize(mData->mObserverArray.count()+1);
	    mData->mObserverArray.insert(mData->mObserverArray.count(),
	                                 &aObserver);
    	aObserver.modelChanged( *this );
    	}
    }
    
    
    
// ---------------------------------------------------------------------------
// Description : Removes model change observers
// ---------------------------------------------------------------------------
//
OSN_EXPORT void AlfModel::removeModelChangeObserver(
    IAlfModelChangeObserver& aObserver)
    {
    for ( int i = 0; i < mData->mObserverArray.count(); ++i )
    	{
    	if(mData->mObserverArray[i]==&aObserver)
    	 {
    	  mData->mObserverArray.remove( i );
		  return;
    	 }
    	}
    }


// ---------------------------------------------------------------------------
// Description : Sets the data for the model
// ---------------------------------------------------------------------------
//
OSN_EXPORT void AlfModel::setData(IAlfVariantType* aData)
    {
    if( mData->mDataSource != 0 )
        {
        delete mData->mDataSource;
        mData->mDataSource=0;
        }
    mData->mDataSource = aData;

    for ( int i = 0; i < mData->mObserverArray.count(); ++i )
        {
        mData->mObserverArray[i]->modelChanged( *this );
        }
    }

// ---------------------------------------------------------------------------
// Description : Returns the Data Source
// ---------------------------------------------------------------------------
//
OSN_EXPORT IAlfVariantType* AlfModel::data() const
    {
    return  mData->mDataSource;
    }

// ---------------------------------------------------------------------------
// Description : notifies, that the model is changed.
// ---------------------------------------------------------------------------
//    
OSN_EXPORT void AlfModel::notifyModelChanged()
    {
    //Notify all the observers
    for ( int i = 0; i < mData->mObserverArray.count(); ++i )
        {
        mData->mObserverArray[i]->modelChanged(*this);
        }    
    }

// ---------------------------------------------------------------------------
// Description : Clears the Data Source
// ---------------------------------------------------------------------------
//
OSN_EXPORT void AlfModel::clearModel()
    {
    //Delete the Model
    if( mData->mDataSource != 0 )
        {
        delete mData->mDataSource;
        mData->mDataSource=0;        
        }
    //Notify all the observers
    for ( int i = 0; i < mData->mObserverArray.count(); ++i )
        {
        mData->mObserverArray[i]->modelChanged(*this);
        }
    }

// ---------------------------------------------------------------------------
// From class IAlfInterfaceBase.
// Static getter for interfaces provided by the widget,
// the control, or the model.
// ---------------------------------------------------------------------------
//
OSN_EXPORT IAlfInterfaceBase* AlfModel::makeInterface( const IfId& aType)
    {
    UString param(aType.mImplementationId);
    if ( param == IAlfModel::type().mImplementationId )
        {
        return this;
        }
    return 0;
    }

//----------------------------------------------------------------------------
//Add a  number of data fields in the model data. All model change
//observers are notified about the change.  Internal API not exported
//-----------------------------------------------------------------------------
void AlfModel::addData(AlfModelOperation* aAddOperation)
    {
    IAlfVariantType* newData = 0;
    IAlfVariantType* parentData = 0;
    try
        {
        //ALF_PERF_START( perfdata, "AlfModel-AddData-Adding")
        newData = aAddOperation->getNewData();     
        parentData  = aAddOperation->parentData(*data());
        int index = aAddOperation->index();
        
        if(parentData->type()== IAlfVariantType::EBranch &&
               newData->type() == IAlfVariantType::EMap )
            {
            // This is actually the same as updating the data,
            // since we can not have multiple branch or child data maps
            if ( index == 0 )
                {
                // Set data for the branch
                parentData->branch()->setData( newData->map() );
                }
            else if ( index == 1 )
                {
                // Set data for the branch children
                parentData->branch()->setChildData( newData->map() );
                }
            }
        else if(parentData->type()== IAlfVariantType::EMap)
            {
            parentData->map()->addItem( newData,
                                   aAddOperation->newDataName() );
            }
        else if(parentData->type()== IAlfVariantType::EContainer )
            {
            parentData->container()->addItem(index, newData);
            }
        //ALF_PERF_STOP( perfdata, "AlfModel-AddData-Adding")
        }
	catch(AlfDataException& e)	        
        {
        newData = 0;
        parentData = 0;
        ALF_THROW(AlfDataException,EInvalidModelOperation,"AlfModel")
        // Note the aAddOperation is not deleted. In case of exception the user
        // should delete this object 
        }
    }
    
//-----------------------------------------------------------------------------
//Remove a  number of data fields in the model data. All model change
//observers are notified about the change.
//----------------------------------------------------------------------------- 
void AlfModel::removeData(AlfModelOperation* aRemoveOperation)
    {
    IAlfVariantType* parentData = 0;

    try
        {
        //ALF_PERF_START( perfdata, "AlfModel-RemoveData-Removing")
        parentData  = aRemoveOperation->parentData(*data());         
        int index = aRemoveOperation->index();

        if(parentData->type()== IAlfVariantType::EBranch )
            {
            if ( index == 0 )
                {
                // Remove the branch data
                parentData->branch()->setData( 0 );
                }
            else if ( index == 1 )
                {
                // Remove the branch children data
                parentData->branch()->setChildData( 0 );
                }
            }
        else if(parentData->type()== IAlfVariantType::EMap)
            {
            parentData->map()->removeItem(index);
            }
        else if(parentData->type()== IAlfVariantType::EContainer )
            {
            parentData->container()->removeItem(index);
            }    
        //ALF_PERF_STOP( perfdata, "AlfModel-RemoveData-Removing")
        }
    catch(AlfDataException& e)
        {
        parentData = 0;
        ALF_THROW(AlfDataException,EInvalidModelOperation,"AlfModel")
        // Note the aRemoveOperation is not deleted. In case of exception the
        // user should delete this object	
        }
    }


void AlfModel::updateData(AlfModelOperation* aUpdateOperation)
    {
    IAlfVariantType* newData = 0;
    IAlfVariantType* parentData = 0;

    try
        {
        //ALF_PERF_START( perfdata, "AlfModel-UpdateData-Updating")
        newData= aUpdateOperation->getNewData();

        parentData  = aUpdateOperation->parentData(*data());
        int index = aUpdateOperation->index();

        if(parentData->type()== IAlfVariantType::EBranch &&
                newData->type() == IAlfVariantType::EMap )
            {
            if ( index == 0 )
                {
                // Update the branch data
                parentData->branch()->setData( newData->map() );
                }
            else if ( index == 1 )
                {
                // Update the data for the branch children
                parentData->branch()->setChildData( newData->map() );
                }
            }
        else if(parentData->type()== IAlfVariantType::EMap)
            {
            parentData->map()->replaceItem(index, newData);
            }
        else if(parentData->type()== IAlfVariantType::EContainer )
            {
            parentData->container()->replaceItem(index,newData);
            }
        //ALF_PERF_STOP( perfdata, "AlfModel-UpdateData-Updating")
        }
    catch(AlfDataException& e)
        {
        parentData=0;
        newData=0;
        ALF_THROW(AlfDataException,EInvalidModelOperation,"AlfModel")
        //Note the aUpdateOperation is not deleted. In case of exception the user should delete this object
        }
    }

//----------------------------------------------------------------------------
//Add a  number of data fields in the model data. All model change
//observers are notified about the change. 
//----------------------------------------------------------------------------- 
OSN_EXPORT void AlfModel::addData(
    int aNumContainerIndices, int* aContainerIndices, IAlfVariantType* aData)
    {
    auto_ptr<AlfModelOperation> op( AlfModelOperation::create(
    AlfModelOperation::EOperationAdd,
    aNumContainerIndices,
    aContainerIndices,
    aData ) );

    executeOperation( op.get() );
    }

//-----------------------------------------------------------------------------
//Remove a  number of data fields in the model data. All model change
//observers are notified about the change.
//----------------------------------------------------------------------------- 
OSN_EXPORT void AlfModel::removeData(
    int aNumContainerIndices, int* aContainerIndices)
    {
    auto_ptr<AlfModelOperation> op( AlfModelOperation::create(
    AlfModelOperation::EOperationRemove,
    aNumContainerIndices,
    aContainerIndices ) );

    executeOperation( op.get() );
    }

//-----------------------------------------------------------------------------
//Remove a  number of data fields in the model data. All model change
//observers are notified about the change.
//----------------------------------------------------------------------------- 
OSN_EXPORT void AlfModel::updateData(
    int aNumContainerIndices, int* aContainerIndices, IAlfVariantType* aData)
    {
    auto_ptr<AlfModelOperation> op( AlfModelOperation::create(
    AlfModelOperation::EOperationUpdate,
    aNumContainerIndices,
    aContainerIndices,
    aData ) );

    executeOperation( op.get() );
    }

//-----------------------------------------------------------------------------
    /**
    *  Api for executing batch operations
    * 
    *  @param aOperationsArray  - An RPOinterArray containing the list of 
    *                             operations to be executed in batch.
    *  @since S60 ?S60_version 
    */  
//----------------------------------------------------------------------------- 
OSN_EXPORT void AlfModel::executeOperations(
    AlfPtrVector<AlfModelOperation>&  aOperationsArray)
    {
    int index = 0;
    int count = aOperationsArray.count();  
    int obsvrCount = mData->mObserverArray.count();
    AlfModelOperation* tempOperation=0;

    for ( index = 0; index < obsvrCount; index++ )
        {
        mData->mObserverArray[index]->dataChanging( aOperationsArray );
        }

    for ( index = 0; index < count; index++ )
        {
        tempOperation = aOperationsArray[index];
        switch( tempOperation->operation() )
            {
            case AlfModelOperation::EOperationAdd:                 
                 this->addData( tempOperation );
                break;
            case AlfModelOperation::EOperationRemove:                   
                 this->removeData( tempOperation );
                break;
            case AlfModelOperation::EOperationUpdate:
                 this->updateData( tempOperation );
            default:
                break;
         
            }
        }

    for ( index = 0; index < obsvrCount; index++ )
        {
        mData->mObserverArray[index]->dataChanged();
        }
    }

//-----------------------------------------------------------------------------
    /**
    *  Api for executing one operation at a Time
    * 
    *  @param aOperation    - An Operation to be executed on the model.
    *  @since S60 ?S60_version 
    */  
//----------------------------------------------------------------------------- 
OSN_EXPORT void AlfModel::executeOperation(AlfModelOperation* aOperation)
    {
    int index = 0;
    int obsvrCount = mData->mObserverArray.count();

    for ( index = 0; index < obsvrCount; index++ )
        {
        mData->mObserverArray[index]->dataChanging( *aOperation );
        }

    switch(aOperation->operation())
        {
        case AlfModelOperation::EOperationAdd:                 
             this->addData(aOperation);
            break;
        case AlfModelOperation::EOperationRemove:                   
             this->removeData(aOperation);
            break;
        case AlfModelOperation::EOperationUpdate:
             this->updateData(aOperation);
        default:
            break;
        }

    for ( index = 0; index < obsvrCount; index++ )
        {
        mData->mObserverArray[index]->dataChanged();
        }
    }
} // Alf
