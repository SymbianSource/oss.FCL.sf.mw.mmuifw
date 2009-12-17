/*
* Copyright (c) 2007, 2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Model implementation with utilities for scrollbar widget.
*
*/
//User Includes
#include "alfscrollbarmodel.h"
#include "alfscrollbarwidget.h"

//Widget includes
#include <alf/ialfscrollbardefaultbaseelement.h>
#include "ialfscrollbarbaseelementinternal.h"
#include "alfscrollbardefaultbaseelement.h"

#include <alf/ialfscrollbarwidget.h>
#include <alf/alfmodel.h>
#include <alf/alfexception.h>

namespace Alf
    {
    	
/*
 * To store data required in the model class
 */
struct ScrollbarModelDataImpl
    {
    int mTotalLength;
    int mViewLength;
    int mViewStartPosition;
    };

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
//  C++ default constructor.
// ---------------------------------------------------------------------------
//
AlfScrollBarModel::AlfScrollBarModel(IAlfVariantType* aDataSource):
    mModel ( NULL )
    {
    construct ( aDataSource );
    
     // Create an instance of model specific data
    mModelData = new (EMM) ScrollbarModelDataImpl();
    
    // Initialise model data with default values
    mModelData->mTotalLength       = 0;
    mModelData->mViewLength        = 0;
    mModelData->mViewStartPosition = 0;
    
    mScrollWidget = NULL;
    }//End of AlfScrollBarModel function.


// ---------------------------------------------------------------------------
// Destructor.
// ---------------------------------------------------------------------------
//
AlfScrollBarModel::~AlfScrollBarModel()
    {
    if(mModelData)
        delete mModelData;
        
    delete mModel;
    }//End of ~AlfScrollBarModel function.


// ---------------------------------------------------------------------------
//  Second Phase Constructor.
// ---------------------------------------------------------------------------
//
void AlfScrollBarModel::construct ( IAlfVariantType* /*aDataSource */)
    {

    }//End of construct function.


// ---------------------------------------------------------------------------
// From class IAlfModel.
// Add a new observer to be notified of any changes in the model.
// ---------------------------------------------------------------------------
//
void AlfScrollBarModel::addModelChangeObserver(
    IAlfModelChangeObserver& /*aObserver*/)
    {
    
    }//End of addModelChangeObserver function.

    
// ---------------------------------------------------------------------------
// From class IAlfModel.
// Add a new observer to be notified of any changes in the model.
// ---------------------------------------------------------------------------
//
void AlfScrollBarModel::removeModelChangeObserver(
    IAlfModelChangeObserver& /*aObserver*/ )
    {
    
    }//End of function.


// ---------------------------------------------------------------------------
// From class IAlfModel.
// Api to change the model completely at one short.
// ---------------------------------------------------------------------------
//    
void AlfScrollBarModel::setData ( IAlfVariantType* /*aData*/ )
    {
        
    }//End of removeModelChangeObserver function.


// ---------------------------------------------------------------------------
// From class IAlfModel.
// Change a given  of data fields in the model.
// ---------------------------------------------------------------------------
//
void AlfScrollBarModel::updateData(
    TInt /*aNumContainerIndices*/,
    TInt* /*aContainerIndices*/,
    IAlfVariantType* /*aData*/)
    {
    ALF_THROW(AlfException,EInvalidArgument,"updateData")        
    }//End of updateData function.

    
// ---------------------------------------------------------------------------
// From class IAlfModel.
// Add a number of data fields in the model data.
// ---------------------------------------------------------------------------
//
void AlfScrollBarModel::addData(
    TInt /*aNumContainerIndices*/,
    TInt* /*aContainerIndices*/,
    IAlfVariantType* /*aData*/)
    {
    ALF_THROW(AlfException,EInvalidArgument,"addData")        
    }//End of addData function.


// ---------------------------------------------------------------------------
// From class IAlfModel.
// Remove a  number of data fields in the model data.
// ---------------------------------------------------------------------------
//  
void AlfScrollBarModel::removeData(
    TInt /*aNumContainerIndices*/,
    TInt* /*aContainerIndices*/ )
    {
    ALF_THROW(AlfException,EInvalidArgument,"removeData")        
    }//End of removeData function.


// ---------------------------------------------------------------------------
// From class IAlfModel.
// Api for executing batch operations.
// ---------------------------------------------------------------------------
//  
void AlfScrollBarModel::executeOperations(
    AlfPtrVector<AlfModelOperation>& /*aOperationsArray*/)
    {
    ALF_THROW(AlfException,EInvalidArgument,"executeOperations")    
    }//End of executeOperations function.


// ---------------------------------------------------------------------------
// From class IAlfModel.
// Api for executing one operation at a Time.
// ---------------------------------------------------------------------------
//  
void AlfScrollBarModel::executeOperation(AlfModelOperation* /*aOperation*/)
    {
    ALF_THROW(AlfException,EInvalidArgument,"executeOperation")    
    }//End of executeOperation function.


// ---------------------------------------------------------------------------
// From class IAlfModel.
// Clears the Model.
// ---------------------------------------------------------------------------
//  
void AlfScrollBarModel::clearModel()
    {
    
    }//End of clearModel function.


// ---------------------------------------------------------------------------
// From class IAlfModel.
// Get the root data object of this model.
// ---------------------------------------------------------------------------
//  
IAlfVariantType* AlfScrollBarModel::data() const
    {
    return NULL;
    }//End of data function.


// ---------------------------------------------------------------------------
// From class IAlfInterfaceBase.
// Getter for interfaces provided by the scrollbar model.
// ---------------------------------------------------------------------------
IAlfInterfaceBase* AlfScrollBarModel::makeInterface(const IfId& aType)
    {
    IAlfInterfaceBase* ret ( 0 );
    UString param ( aType.mImplementationId );

    if ( param == IAlfScrollBarModel::type().mImplementationId )
        {
        ret = static_cast<IAlfScrollBarModel*>(this);
        }
    else if ( param == IAlfModel::type().mImplementationId )
        {
        ret = static_cast<IAlfModel*>(this);
        }
    else
        {
        //do nothing
        }

    return ret;
    }//End of makeInterface function.

// ---------------------------------------------------------------------------
// From class IAlfScrollBarModel.
// Create the default data structure for a ScrollBar widget.
// ---------------------------------------------------------------------------
//
void AlfScrollBarModel::initializeData(
    int aTotalLength,
    int aViewLengthInSteps,
    int aViewStartPosition)
    {

    validateAllData(aTotalLength, 
        aViewLengthInSteps,
        aViewStartPosition);
    
    // Initialise model with the data provided
    mModelData->mTotalLength       = aTotalLength;
    mModelData->mViewLength        = aViewLengthInSteps;
    mModelData->mViewStartPosition = aViewStartPosition;
   
    changeData(
        mModelData->mTotalLength,
        mModelData->mViewLength,
        mModelData->mViewStartPosition);

    }//End of function.

// ---------------------------------------------------------------------------
// Set Total Length .
// ---------------------------------------------------------------------------
//
void AlfScrollBarModel::setTotalLength(int aTotalLength)
    {
    int viewLength = getViewLength();
    int viewStartPos = getViewStartPosition();
       
    if (validateAllData(aTotalLength, 
            viewLength, 
            viewStartPos))
        {
        updateScrollBarModel(aTotalLength, -1, -1 );
        }
    else
        {
        updateScrollBarModel(aTotalLength, viewLength, viewStartPos);
        }
    }//End of setTotalLength function.

// ---------------------------------------------------------------------------
// Set ViewLength.
// ---------------------------------------------------------------------------
//
void AlfScrollBarModel::setViewLength(int aViewLengthInSteps)
    {
    validateViewLength(aViewLengthInSteps);
    updateScrollBarModel ( -1, aViewLengthInSteps, -1);
    
    }//End of setViewLength function.


// ---------------------------------------------------------------------------
// Set Thumb Position.
// ---------------------------------------------------------------------------
//
void AlfScrollBarModel::setViewStartPosition ( int aViewStartPosition )
    {
    validateViewStartPos(aViewStartPosition);
    updateScrollBarModel (-1, -1, aViewStartPosition);
    }//End of setViewStartPosition function.

// ---------------------------------------------------------------------------
// Changes all the data of model with new values.
// ---------------------------------------------------------------------------
//
void AlfScrollBarModel::changeData(
    int aTotalLength,
    int aViewLength,
    int aViewStartPosition )
    {
    
    validateAllData(aTotalLength,
        aViewLength,
        aViewStartPosition);
          
     if(getViewStartPosition() == aViewStartPosition && 
        getTotalLength() == aTotalLength && 
        getViewLength() == aViewLength)
        { 
        // nothing is changed
        return ;
        }
    else
	    {
	    updateScrollBarModel(
	            aTotalLength,
	            aViewLength,
	            aViewStartPosition);
	    }
            
  
    }//End of changeData function.

// ---------------------------------------------------------------------------
// getTotalLength.
// ---------------------------------------------------------------------------
//
int AlfScrollBarModel::getTotalLength() const
    {
    return mModelData->mTotalLength;
    }//End of getTotalLength function.


// ---------------------------------------------------------------------------
// Returns ViewLength.
// ---------------------------------------------------------------------------
//
int AlfScrollBarModel::getViewLength() const
    {
    
    return mModelData->mViewLength;
    }//End of getViewLength function.

    
// ---------------------------------------------------------------------------
// Returns ViewStartPosition.
// ---------------------------------------------------------------------------
//
int AlfScrollBarModel::getViewStartPosition() const
    {
    return mModelData->mViewStartPosition;
    }//End of getViewStartPosition function.

// ---------------------------------------------------------------------------
// Updates the ScrollBar visualization
// according to the model data.
// ---------------------------------------------------------------------------
//
void AlfScrollBarModel::updateScrollBarVisualization()
    {
     //updateVisualization with new model
     IAlfElement* element = (mScrollWidget)->control()->findElement("BaseElement");
     IAlfScrollBarDefaultBaseElement* defaultLCTBaseElement = static_cast<IAlfScrollBarDefaultBaseElement*> (
    		 element->makeInterface (IAlfScrollBarDefaultBaseElement::type() ) );
     AlfScrollBarDefaultBaseElement* baseElement = (AlfScrollBarDefaultBaseElement*)defaultLCTBaseElement;
     
     baseElement->updateVisualization();
    }

// ---------------------------------------------------------------------------
// Updates the ScrollBar model according to the parameters.
// ---------------------------------------------------------------------------
//
void AlfScrollBarModel::updateScrollBarModel(
    int aTotalLength,
    int aViewLength,
    int aViewStartPosition)
    {
    //change only if the parameters are non negative . 
    
    if(aTotalLength!=-1)
        {
        mModelData->mTotalLength = aTotalLength;
        }
    else
        {
        // Do Nothing
        }
    
    if(aViewLength!=-1)
        {
        mModelData->mViewLength =  aViewLength; 
        }
    else
        {
        // Do Nothing 
        }
        
    
    if(aViewStartPosition!=-1)
        {
        mModelData->mViewStartPosition = aViewStartPosition;  
        }
    else
        {
        // Do Nothing  
        }
 
    //updateVisualization with new model    
    updateScrollBarVisualization();
    }//End of updateScrollBarModel function.

// ---------------------------------------------------------------------------
// validateViewLength
// ---------------------------------------------------------------------------
//
bool AlfScrollBarModel::validateViewLength(int & aViewLength)
    {
    bool ret = true;
    
    if (aViewLength <= 0)
        {
        // Minimum view length
        aViewLength = 1;
        ret = false;
        }
    else
        {
        int totalLength = getTotalLength();
        if (aViewLength > totalLength)
            {
            aViewLength = totalLength;
            ret = false;
            }
        else
            {
            // do nothing
            }
        }
    
    return ret;
    }

// ---------------------------------------------------------------------------
// validateViewStartPos
// ---------------------------------------------------------------------------
//
bool AlfScrollBarModel::validateViewStartPos(int & aViewStartPos)
    {
    bool ret = true;
    
    if (aViewStartPos < 0)
        {
        aViewStartPos = 0;
        ret = false;
        }
    else
        {
        int totalLength = getTotalLength();
        int viewLength = getViewLength();     
        if (aViewStartPos > (totalLength-viewLength))
            {
            // Maximum value
            aViewStartPos = totalLength-viewLength;            
            ret = false;
            }
        else
            {
            // do nothing
            }        
        }
    return ret;
    }

// ---------------------------------------------------------------------------
// validateAllData
// ---------------------------------------------------------------------------
//    
bool AlfScrollBarModel::validateAllData(int & aTotalLength, 
        int & aViewLength, 
        int & aViewStartPos)
    {
     bool ret = true;
     if (aTotalLength <= 0)
        {
        aTotalLength = 1;
        ret = false;
        }
     else
        {
        // do nothing
        }
     
     // validate view length   
     if (aViewLength <= 0)
        {
        aViewLength = 1;
        ret = false;
        }
     else if (aViewLength > aTotalLength)
        {
        // Setting maximum possible value of View Length
        aViewLength = aTotalLength;
        ret = false;
        }
     else
        {
        // do nothing
        }
     
     // validate view start pos
     if (aViewStartPos < 0)
        {
        aViewStartPos = 0;
        ret = false;
        }
     else if (aViewStartPos > (aTotalLength-aViewLength) )
        {
        // Setting maximum possible value of View Length
        aViewStartPos = aTotalLength-aViewLength;        
        ret = false;
        }
     else
        {
        // do nothing
        }
        
     // No need to validate aFocusPos for now
    return ret;
    }
    
// ---------------------------------------------------------------------------
// StoreWidget
// ---------------------------------------------------------------------------
//    
void AlfScrollBarModel::storeWidget(AlfScrollBarWidget* aWidget)
    {
    mScrollWidget = aWidget;
    }
}//Namespace Alf
    
