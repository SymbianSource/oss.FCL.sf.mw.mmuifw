/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: Header for presentation element - slider model interface.
*
*/


#ifndef MUL_SLIDERMODEL_H
#define MUL_SLIDERMODEL_H

// Widget Model and Exported API includes
#include <mul/imulslidermodel.h>

// OsnCore includes
#include <osn/alfptrvector.h>
#include <osn/ustring.h>
#include "imulsliderbaseelementinternal.h"
#include <mul/mulvisualitem.h>
#include <mul/imulsliderwidget.h>
// Namespace
using namespace osncore;

namespace Alf
    {

//Forward Declarations
class AlfModel;
class MulSliderWidget;

/**
 *  slider model implementation.
 *
 *  Implements utility methods to create and update
 *  the data hierarchy of a slider.
 */
class MulSliderModel : public IMulSliderModel
    {
public:
    /////////////////////////////////////////
    /////// FROM MULSLIDER//////////////////
	/**
	 * Set the value of the primary thumb.
	 *
	 * @param aValue Position of the primary thumb.
	 *  
	 */
	 void SetPrimaryValue(int aValue) ;

	/**
	 * Return the value of the primary thumb of the slider.
	 *
	 *  @return Value of the primary thumb.
	 *  
	 */
	 int PrimaryValue() const ;
	
	/**
	 * Set the value of the secondary thumb.
	 *
	 * @param aValue Position of the secondary thumb.
	 *  
	 */
	 void SetSecondaryValue(int aValue) ;

	/**
	 * Return the value of the secondary thumb of the slider.
	 *
	 *  @return Value of the secondary thumb.
	 *  
	 */
	 int SecondaryValue() const ;

	/**
	 * Return the total number of ticks.
	 *
	 *  @return Number of ticks (int)
	 *  
	 */
	 int TotalTicks() const ;


	/**
	 * Set Template that will be used to represent data.
	 * 
	 * @param aTemplateId Template associated with the widget.
	 * 
	 */
	 void SetTemplate( sliderTemplate aTemplateId) ;
	 
	/**
	 * Get Template that will be used to represent data.
	 * 
	 * 
	 * 
	 */
	 sliderTemplate GetTemplate();
	 
    /**
	 * SetRightText that will be used to represent data.
	 * 
	 * @param aRightText
	 * 
	 */
	 void SetRightText(char* aRightText) ;
	
	/**
	 * GetRightText that will be used to represent data.
	 * 
	 *
	 * 
	 */
	 const char* GetRightText() ;
     
     /**
	 * SetLeftText that will be used to represent data.
	 * 
	 * @param aLeftText 
	 * 
	 */
	 void SetLeftText(char* aLeftText) ;
     
     /**
	 * GetLeftText that will be used to represent data.
	 * 
	 * 
	 * 
	 */
	 const char* GetLeftText() ;
	
	 /**
	 * SetPercentText that will be used to represent data.
	 * 
	 * 
	 */	 
	 void SetPercentText(char* aPercText) ;			
			
     /**
	 * GetPercentText that will be used to represent data.
	 *
	 * 
	 */				
	 const char* GetPercentText() ;
	
	  /**
	 * SetMinRange that will be used to represent data.
	 * 
	 * 
	 */
	 void SetMinRange(int aMinRange) ;
     
      /**
	 * GetMinRange that will be used to represent data.
	 * 
	 * 
	 */
	 int MinRange()const ;
	
	  /**
	 * SetMaxRange that will be used to represent data.
	 * 
	 * 
	 */				
	 void SetMaxRange(int aMaxRange) ;
     
     /**
	 * MaxRange that will be used to represent data.
	 * 
	 * 
	 */
	 int MaxRange()const ;
     
     /**
	 * SetTick that will be used to represent data.
	 * 
	 * 
	 */
	 void SetTick(int aTick) ;

     /**
	 * GetTick that will be used to represent data.
	 * 
	 * 
	 */
	 int Tick()const ;
    	 
    /**
     * Constructor.
     */
    MulSliderModel();

    /**
     * Destructor.
     */
    ~MulSliderModel();

    

    /** From base class IAlfModel
     *
     */

    /**
     *  Add a new observer to be notified of any changes in the model.
     *
     *  @param    aObserver   The model change observer to be added.
     *  @return void
     *  @since S60 ?S60_version
     */
    void addModelChangeObserver(IAlfModelChangeObserver& aObserver);

    /**
     *  Remove an observer which is added to get notified of any changes
     *  in the model.
     *
     *  @param aObserver The model change observer to be removed.
     *  @return void
     *  @since S60 ?S60_version
     */
    void removeModelChangeObserver(IAlfModelChangeObserver& aObserver);

    /**
     *  API to change the model completely at one short. 
     *  
     *  @see IAlfModel
     *  @param    aData    The new data.
     *  @return void
     *  @since S60 ?S60_version
     */
    void setData(IAlfVariantType* aData);

    /**
     * Change a given  of data fields in the model . All model change
     * observers are notified about the change.
     *
     * @param aNumContainerIndices The number of indices in aContainerIndices.
     * @param aContainerIndices    The container indices to find the correct 
     *                             location in the data hierarchy.The ownership 
     *                             is transferred. 
     * @param aData                The new data to be set.
     * @since S60 ?S60_version
     */
    void updateData(int  aNumContainerIndices,
        int* aContainerIndices,
        IAlfVariantType* aData);

    /**
     * Add a  number of data fields in the model data. All model change
     * observers are notified about the change.
     *
     * @param aNumContainerIndices The number of indices in aContainerIndices.
     * @param aContainerIndices    The container indices to find the correct 
     *                             location in the data hierarchy.The ownership
     *                             is transferred. 
     * @param aData                The new data.
     * @return void
     * @since S60 ?S60_version
     */
    void addData(int aNumContainerIndices,
        int* aContainerIndices,
        IAlfVariantType* aData);

    /**
     * Remove a  number of data fields in the model data. All model change
     * observers are notified about the change.
     *
     * @param aNumContainerIndices The number of indices in aContainerIndices.
     * @param aContainerIndices    The container indices to find the correct 
     *                             location in the data hierarchy.The ownership
     *                             is transferred.
     * @return void
     * @since S60 ?S60_version
     */
    void removeData(int aNumContainerIndices, int* aContainerIndices);

    /**
     *  API for executing batch operations
     *
     *  @param aOperationsArray    An RPOinterArray containing the list of 
     *                             operations to be executed in batch.
     *  @return void
     *  @since S60 ?S60_version
     */
    void executeOperations(AlfPtrVector<AlfModelOperation>&  aOperationsArray);

    /**
     *  API for executing one operation at a Time
     *
     *  @param aOperation          An Operation to be executed on the model.
     *  @since S60 ?S60_version
     */
    void executeOperation(AlfModelOperation* aOperation);

    /**
     *  Clears the Model.
     *
     *  @return void
     *  @since S60 ?S60_version
     */
    void clearModel();
       
    IAlfVariantType* data() const;

    IAlfInterfaceBase* makeInterface(const IfId& aType);
    
   /**
    *  Get the Element Interface.
    *
    *  @return The Element pointer.
    *  @since S60 ?S60_version
    */
    IMulSliderBaseElementInternal* GetElement();
    
    
    /**
     * SetData
     */
    void SetData(const MulVisualItem& aSliderItem);
  
    
    /**
     * Data
     */
    const MulVisualItem& Data() const;

    
    /*
     * IsLandscape
     */
    bool IsLandscape();
  
    
    /**
     * pointer to slidermodelData.
     */  
    void storeWidget(MulSliderWidget* aWidget);
    
    void UpdateElement();
 
private:
    
	/*
     * pointer to slidermodelData.
     */ 
    struct SliderModelDataImpl *mSldrModelData;

    AlfModel* mModel;  //Owned
    }; // class MulSliderModel

    } // End of namespace Alf

#endif // MUL_SLIDERMODEL_H
//End Of File
