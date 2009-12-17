/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Model Interface with utilities for scrollbar widget.
*
*/

#ifndef ALF_SCROLLBARMODEL_H
#define ALF_SCROLLBARMODEL_H

// Widget Model and Exported API includes
#include <alf/ialfscrollbarmodel.h>

// OsnCore includes
#include <osn/alfptrvector.h>
#include <osn/ustring.h>

// Namespace
using namespace osncore;

namespace Alf
    {

//Forward Declarations
class AlfModel;
class AlfScrollBarWidget;



/**
 *  ScrollBar model implementation.
 *
 *  Implements utility methods to create and update
 *  the data hierarchy of a scrollbar.
 */
class AlfScrollBarModel : public IAlfScrollBarModel
    {
public:

    /**
     * Constructor.
     */
    AlfScrollBarModel(IAlfVariantType* aDataSource = NULL);

    /**
     * Destructor.
     */
    ~AlfScrollBarModel();
    
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

    /**
    *  Get the root data object of this model.
    *
    *  @return The root data object.
    *  @since S60 ?S60_version
    */
    IAlfVariantType* data() const;
    
    //  From base class IAlfInterfaceBase
    
    IAlfInterfaceBase* makeInterface(const IfId& aType);

    //  From base class IAlfScrollBarModel     

	/**
	 * Create the default data structure for a ScrollBar widget.
	 * 
	 * @param       aTotalLengthInSteps
	 * @param       aViewLengthInSteps
	 * @param       aViewStartPosition
	 *
	 *
	 * @return      void
	 * @exception   std::bad_alloc
	 * @since       S60 ?S60_version
	 */  
    void initializeData(int aTotalLengthInSteps,
           int aViewLengthInSteps,
           int aViewStartPosition);

    /**
     * Sets the TotalLength for the Scrollbar. Does not notify observers about this.
     * 
     * @param aTotalLength  TotalLength value
     *
     * @return void.
     * @since S60 ?S60_version
     */
    void setTotalLength(int aTotalLength);

    /**
     * Sets the View Length for the Scrollbar. Does not notify observers 
     * about this.
     *
     * @param aViewSize 
     * 
     * @return void.
     * @since S60 ?S60_version
     */
    void setViewLength(int aViewSize);

    /**
     * Sets the View Start Position for the Scrollbar. Does not notify 
     * observers about this.
     *
     * @param aViewStartPosition
     * 
     * @return void.
     * @since S60 ?S60_version
     */
    void setViewStartPosition(int aViewStartPosition);

    /**
         * Sets the TotalLength for the Scrollbar. Does not notify observers about this.
         *
         * @param aTotalLength              TotalLength value
         * @param aViewLength       view length value
         * @param aViewStartPosition view start position  
         *
         * @return void.
         * @since S60 ?S60_version
         */
    void changeData(int aTotalLength,int aViewLength, int aViewStartPosition);   
        
    /**
     * Length of the document or viewable data.
     *
     * Arbitrary units are used, but the same units as the rest of the model.
     * This length is best current estimate of the length of the entire 
     * viewable data in this model's direction.(i.e, vertical, horizontal).
     * It represents physical extent (e.g. could be pixels), not logical
     * size of the data. It is understood to represent the physical laid out 
     * size of the data if it were possible to view it all at once.
     *
     * @return length in arbitrary units of the viewable data
     * @since S60 ?S60_version
     */
    int getTotalLength() const;

    /**
     * Length of the view.
     *
     * For horizontal models, this is the width of the view.
     * For veritcal, this is the height of the view.
     *
     * Note the units are arbitrary but uniform with the rest of the model.
     * Note that this measure is proportional to the physical width or height
     * of the view. This metric is sometimes called the Thumb size.
     *
     * @return position of the start of the view
     * @since S60 ?S60_version
     */
    int getViewLength() const;
    /**
     * Position in arbitrary units of the start of the currently viewed data.
     *
     * For horizontal models, this is the position of the left of the view 
     * relative to the far left end of the document.
     * For vertical, this is the position of the top of the view from the top
     * of the viewable data.
     *
     * Note the units are arbitrary but uniform with the rest of the model.
     * Position is in physical space as if the entire data were laid out.
     * This metric is sometimes called the Thumb position.
     *
     * @return position of the start of the view
     * @since S60 ?S60_version
     */
    int getViewStartPosition() const;
       
    /**
     * Updates the model according to the values passed.
     *
     * @param       aTotalLengthInSteps
     * @param       aViewLength
     * @param       aViewStartPosition
     *
     * @return void.
     * @since S60 ?S60_version
     *
     */
    void updateScrollBarModel(int aTotalLengthInSteps,
            int aViewLength,
            int aViewStartPosition);
     /**
     * Updates visualization of scrollbar according 
     * to the model values.
     */  
    void updateScrollBarVisualization(); 
    
    /**
     * Stores the scrollbar widget pointer in model
     */
    void storeWidget(AlfScrollBarWidget* aWidget);
    
private:
    
    /** 
     * This function validates view length passed and changes it if required
     * 
     * @param aViewLength - in/out param
     *
     * @return bool false, if the data was not valid and changes were made
     *              else returns true     
     */
    bool validateViewLength(int & aViewLength);

    /** 
     * This function validates view start position passed and changes it if
     *  required
     *
     * @param aViewStartPos - in/out param
     *
     * @return bool false, if the data was not valid and changes were made
     *              else returns true     
     */
    bool validateViewStartPos(int & aViewStartPos);
    
    /** 
     * This function validates total length passed and changes it if 
     * required. This validation can change aViewLength, aViewStartPos
     * too if reuired.
     * 
     * @param aTotalLength - in/out param
     * @param aViewLength - in/out param
     * @param aViewStartPos - in/out param
     *
     * @return bool false, if the data was not valid and changes were made
     *              else returns true
     */
    bool validateAllData(int & aTotalLength, 
        int & aViewLength, 
        int & aViewStartPos);

    /**
     * Private method to create the ScrollBar Widget.
     *
     * @param     aDataSource  Model for the ScrollBar
     * @return    void
     * @since S60 ?S60_version
     * 
     */
    void construct ( IAlfVariantType* aDataSource );
    
	/*
     * pointer to scrollbarmodelData.
     */ 
    struct ScrollbarModelDataImpl *mModelData;

    AlfModel* mModel;  //Owned
    AlfScrollBarWidget* mScrollWidget;
    }; // class AlfScrollBarModel

    } // End of namespace Alf

#endif // ALF_SCROLLBARMODEL_H
//End Of File
