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
* Description:  Model interface with utilities for slider widget.
*
*/


#ifndef I_MULSLIDERMODEL_H
#define I_MULSLIDERMODEL_H


//WidgetModel Includes
#include <alf/ialfmodel.h>
#include <mul/mulvisualitem.h>
//#include <mul/imulsliderwidget.h>

using namespace osncore;

namespace Alf
    {
    	
enum sliderTemplate
     {
     	ESliderTemplate1,
     	ESliderTemplate2,
     	ESliderTemplate3,
     	ESliderTemplate4,
     	ESliderTemplate5,
     	ESliderTemplate6,
     	ESliderTemplate7,
     	ESliderTemplate8,
     	ESliderTemplate9,
     	ESliderTemplateNone
     };
namespace mulslidermodel
    {
    static const IfId ident =
        {
        0, "mulslidermodel"
        };
    }

/**
 *  An interface of the slider model.
 */
class IMulSliderModel : public IAlfModel
    {

public:

    /**
     * Getter for the type identifier of this interface.
     *
     * @return A descriptor to identify the type of this interface.
     */
    static inline const IfId& type()
        {
        return mulslidermodel::ident;
        } 
        
   /**
		 * Change slider related attributes within the model.
		 *
		 * @param aSliderItem Slider data to be changed.
		 *
		 */
		virtual void SetData(const MulVisualItem& aSliderItem) = 0;

		/**
		 * Return a collection of all the slider attributes.
		 * 
		 * @return A visual item containing the slider attributes. 
		 */
		virtual const MulVisualItem& Data() const = 0;
		
   /**
		 * Set the value of the primary thumb.
		 *
		 * @param aValue Position of the primary thumb.
		 *  
		 */
	virtual void SetPrimaryValue(int aValue) = 0;

	/**
	 * Return the value of the primary thumb of the slider.
	 *
	 *  @return Value of the primary thumb.
	 *  
	 */
	virtual int PrimaryValue() const = 0;
	
	/**
	 * Set the value of the secondary thumb.
	 *
	 * @param aValue Position of the secondary thumb.
	 *  
	 */
	virtual void SetSecondaryValue(int aValue) = 0;

	/**
	 * Return the value of the secondary thumb of the slider.
	 *
	 *  @return Value of the secondary thumb.
	 *  
	 */
	virtual int SecondaryValue() const = 0;

	/**
	 * Return the total number of ticks.
	 *
	 *  @return Number of ticks (int)
	 *  
	 */
	virtual int TotalTicks() const = 0;


	/**
	 * Set Template that will be used to represent data.
	 * 
	 * @param aTemplateId Template associated with the widget.
	 * 
	 */
	 
	virtual void SetTemplate( sliderTemplate aTemplateId) = 0;
 /**
	 * Get Template that will be used to represent data.
	 * 
	 * @param aTemplateId Template associated with the widget.
	 * 
	 */	
	virtual sliderTemplate GetTemplate() = 0;
 /**
	 * API to set text on the right side of Slider.
	 * 
	 * @param aRightText - Text that has to be Displayed.
	 * 
	 */	
	
	virtual void SetRightText(char* aRightText) = 0;
	/**
	 * API to Get text on the right side of Slider.
	 * 
	 */	
	
	virtual const char* GetRightText() = 0;
	/**
	 * API to set text on the Left side of Slider.
	 * 
	 * @param aLeftText - Text that has to be Displayed.
	 * 
	 */	
	
	virtual void SetLeftText(char* aLeftText) = 0;
	/**
	 * API to Get text on the Left side of Slider.
	 * 
	 */	
	virtual const char* GetLeftText() = 0;
	/**
	 * API to set the Percentage of Slider movement on Track.
	 * 
	 * @param aPercText - Text that has to be Displayed.
	 * 
	 */
	virtual void SetPercentText(char* aPercText) = 0;
	/**
	 * API to indicate the percentage movement of Thumb on Track.
	 * 
	 */			
	virtual const char* GetPercentText() = 0;
	/**
	 * API to set the Minimum value of Track.
	 * 
	 * @param aMinRange - Minimum value for the start of the track.
	 * 
	 */
	virtual void SetMinRange(int aMinRange) = 0;
	/**
	 * API to retrieve the Minimum value of Track.
	 * 
	 */
	virtual int MinRange()const = 0;
	/**
	 * API to set the Maximum value of Track.
	 * 
	 * @param aMaxRange - Maximum value for the start of the track.
	 * 
	 */				
	virtual void SetMaxRange(int aMaxRange) = 0;
	/**
	 * API to retrieve the Maximum value of Track.
	 * 
	 */
	
	virtual int MaxRange()const = 0;
	/**
	 * API to set Tick value for Slider.
	 * 
	 * @param aTick - Interger value for representing the tick.
	 * 
	 */
	virtual void SetTick(int aTick) = 0;
	/**
	 * API to retrieve the tick value.
	 * 
	 */
	virtual int Tick()const = 0;
    };

    } // namespace Alf

#endif // I_MULSLIDERMODEL_H
//End Of File
