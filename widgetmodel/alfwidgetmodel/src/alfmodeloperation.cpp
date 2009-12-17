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
* Description:   This file implements the various types of operations to be performed on model.
*
*/




//INCLUDES 
#include "alf/alfmodeloperation.h"
#include <alf/alfvarianttype.h>
#include <alf/alfexceptions.h>
#include <osn/osnnew.h>

using namespace Alf;


OSN_EXPORT AlfModelOperation* AlfModelOperation::create(
    OperationType aOperation, int aNumContainerIndices,
    int* aContainerIndices, IAlfVariantType* aData )
    {
    return new( EMM ) AlfModelOperation(
        aOperation, aNumContainerIndices, aContainerIndices, aData );
    }


OSN_EXPORT AlfModelOperation* AlfModelOperation::create( OperationType aOperation, int aNumContainerIndices,
    int* aContainerIndices, IAlfVariantType* aData,
    const UString& aDataName )
    {
    return new( EMM ) AlfModelOperation(
        aOperation, aNumContainerIndices, aContainerIndices, aData, aDataName );
    }

AlfModelOperation::AlfModelOperation(
    OperationType aOperation, int aNumContainerIndices,
    int* aContainerIndices, IAlfVariantType* aData )
    {
    mOperation 				= aOperation ;
    mNumContainerIndices 	= aNumContainerIndices;
	mContainerIndices		= aContainerIndices ;
	mData 					= aData;
    }


AlfModelOperation::AlfModelOperation( OperationType aOperation, int aNumContainerIndices,
    int* aContainerIndices, IAlfVariantType* aData,
    const UString& aDataName )
    {
	mOperation 				= aOperation ;
    mNumContainerIndices 	= aNumContainerIndices;
	mContainerIndices		= aContainerIndices ;
	mDataName 				= aDataName;    	
	mData 					= aData;
    }


OSN_EXPORT AlfModelOperation::~AlfModelOperation()
    {
    delete mData;
    delete [] mContainerIndices;
    }

OSN_EXPORT IAlfVariantType& AlfModelOperation::newData() const
    {
	    if(mOperation == EOperationRemove)
	    {
	    	ALF_THROW(AlfDataException,EInvalidModelOperation,"AlfModelOperation")
	    }
        return *mData;
    }

OSN_EXPORT const UString& AlfModelOperation::newDataName() const
    {
	    if(mOperation == EOperationRemove)
	    {
	    	ALF_THROW(AlfDataException,EInvalidModelOperation,"AlfModelOperation")
	    }
        return mDataName;
    }

OSN_EXPORT IAlfVariantType* AlfModelOperation::getNewData()
    {
	    if(mOperation == EOperationRemove)
	    {
	    	ALF_THROW(AlfDataException,EInvalidModelOperation,"AlfModelOperation")
	    }
        IAlfVariantType* data = mData;
        mData = NULL;
        return data;
    }

OSN_EXPORT AlfModelOperation::OperationType AlfModelOperation::operation() const
    {
    return mOperation;
    }


OSN_EXPORT IAlfVariantType* AlfModelOperation::parentData( IAlfVariantType& aRoot ) const
    {
    try
    {
    if ( mNumContainerIndices == 0 )
        {
        return NULL;
        }
    IAlfVariantType* current = &aRoot;
    for ( int i = 0; current && i < mNumContainerIndices - 1; ++i )
        {
        if ( current->type() == IAlfVariantType::EContainer &&
             mContainerIndices[i] < current->container()->count() )
            {
            current = current->container()->item( mContainerIndices[i] );
            }
        else if ( current->type() == IAlfVariantType::EMap &&
                  mContainerIndices[i] < current->map()->count() )
            {
            current = current->map()->item( mContainerIndices[i] );
            }
        else if ( current->type() == IAlfVariantType::EBranch )
            {
            if ( mContainerIndices[i] == 0 )
                {
                // Index 0 is for the parent data
                current = current->branch()->data();
                }
            else if ( mContainerIndices[i] == 1 )
                {
                // Index 1 is for the child data
                current = current->branch()->childData();
                }
            }
        else
            {
            // Not found
            return NULL;
            }
        }
    return current;
	 }
	 catch(AlfDataException& aException)
	 {
	 	ALF_THROW(AlfDataException,EInvalidContainerOperation,"AlfModelOperation")
	 }
    }

OSN_EXPORT int AlfModelOperation::index() const
    {
    return mContainerIndices[mNumContainerIndices - 1];
    }

OSN_EXPORT int AlfModelOperation::numContainerIndices() const
    {
    return mNumContainerIndices;
    }

OSN_EXPORT int AlfModelOperation::containerIndex( int aIndex ) const
    {
    return mContainerIndices[aIndex];
    }

