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
* Description:  Model interface with utilities for scrollbar widget.
*
*/


#ifndef IALF_SCROLLBARMODEL_H
#define IALF_SCROLLBARMODEL_H


//WidgetModel Includes
#include <alf/ialfmodel.h>

using namespace osncore;

namespace Alf
    {

namespace alfscrollbarmodel
    {
    static const IfId ident =
        {
        0, "alfscrollbarmodel"
        };
    }

/**
 *  An interface of the scrollbar model.
 */
class IAlfScrollBarModel : public IAlfModel
    {

public:

    /**
     * Getter for the type identifier of this interface.
     *
     * @return A descriptor to identify the type of this interface.
     */
    static inline const IfId& type()
        {
        return alfscrollbarmodel::ident;
        } 

    /**
     * Function for initializing the model.
     * 
     * The unit of paremeters aTotalLength, aViewLength, aViewStartPosition 
     * depends on the user of scrollbar. For example editor may 
     * decide to support scrolling by line,means in case of editor 
     * aTotalLenght can be total number of lines in the document, 
     * aViewlength can be the number of lines which fit inside a page/view,
     * aViewstartPosition can be the line number from which view starts and 
     *  
     * (currently no visualization is there for focus position inside scrollbar)
     *
     * @param aTotalLength - Total length of the content or document if it 
     *                       were laid out. Minimum Value of total length can be
     *                       1. If a value less than 1 is passed it is converted 
     *                       to 1 automatically.
     *
     * @param aViewLength - Amount of TotalLength which is viewable. Minmum 
     *                      value of viewlength is 1 and maximum value is equal 
     *                      to total length. If a value less than 1 is passed
     *                      it is converted to 1 similarly, if a value greater 
     *                      than aTotalLength is passed it is converted to 
     *                      aTotalLength.
     * @param aViewStartPosition - view start position. This can be between 0 
     *                             and (aTotalLength - aViewLength). As in 
     *                             case of total length and view length, if a 
     *                             value out side range is passed it is convered
     *                             to the nearest boundary value (0 or 
     *                             aTotalLength - aViewLength).
     * @return void.
     */
    virtual void initializeData(
        int aTotalLength,
        int aViewLength,
        int aViewStartPosition) = 0;

    /**
     * Sets the Total Length for the Scrollbar
     * which is the length of the document or viewable data.
     *
     * Arbitrary units are used, but the same units as the rest of the model.
     * This length is best current estimate of the length of the entire 
     * viewable data in this model's direction.(i.e, vertical, horizontal).
     * It represents physical extent (e.g. could be pixels), not logical size
     * of the data.
     * It is understood to represent the physical laid out size of the data 
     * if it were possible to view it all at once.
     *
     * @param aTotalLength - Total length of the content or document. Minimum 
     *                       Value of total length can be 1. If a value less 
     *                       than 1 is passed, it is converted to 1 
     *                       automatically.
     *                       If the value passed is less than the current view 
     *                       length, current view length is also changed to the 
     *                       passed total length so that view length is still
     *                       valid. Similarly if required, view start position
     *                       is also changed so that it is still valid. 
     *                       See the documentation of initializeData() to know
     *                       valid values of each model data. If change of more
     *                       than one model data is required, use changeData() 
     *                       function.
     *
     * @return void.
     */
    virtual void setTotalLength(int aTotalLength) = 0;

    /**
     * Sets the View Length for the Scrollbar
     * For horizontal models, this is the width of the view.
     * For veritcal, this is the height of the view.
     *
     * Note the units are arbitrary but uniform with the rest of the model.
     * Note that this measure is proportional to the physical width or height
     * of the view.This metric is sometimes called the Thumb size.
     * @param aViewLegth - Amount of Total Length which is viewable. Minmum 
     *                     value of viewlength is 1 and maximum value is equal 
     *                     to total length. If a value out of range is passed
     *                     it is converted to the nearest boundary (1 or total
     *                     length).
     *                     See the documentation of initializeData() to know
     *                     valid values of each model data. If change of more
     *                     than one model data is required, use changeData() 
     *                     function.
     * @return void.
     */
    virtual void setViewLength(int aViewLength) = 0;

    /**
     * Sets the View Start Position for the Scrollbar
     * Position in arbitrary units of the start of the currently viewed data.
     * For horizontal models, this is the position of the left of the view 
     * relative to the far left end of the document.
     * For veritcal, this is the position of the top of the view from the 
     * top of the viewable data.
     *
     * Note the units are arbitrary but uniform with the rest of the model.
     * Position is in physical space as if the entire data were laid out.
     * This metric is sometimes called the Thumb position.
     *
     * @param aViewStartPosition - start position of the viewable area. This can
     *                             be between 0 and (aTotalLength - aViewLength  
     *                             ). If a value out side range is passed, it
     *                             is convered to the nearest boundary value (0
     *                             or totalLength - viewLength)
     *                             See the documentation of initializeData() to 
     *                             know valid values of each model data. If 
     *                             change of more than one model data is 
     *                             required, use changeData() function.     
     *                             
     * @return void.
     */
    virtual void setViewStartPosition(int aViewStartPosition) = 0;

    /**
     * Sets TotalLength, View length and ViewStartPosition for the Scrollbar.
     *
     * See the documentation of initializeData() to know valid values of each 
     * model data. If the value passed are not valid, they are changed to the 
     * closest valid value.
     *
     * @param aTotalLength - total length
     * @param aViewLength - view Length
     * @param aViewStartPosition - View Start Position
     * @return void.
     */
    virtual void changeData(
        int aTotalLength,
        int aViewLength,
        int aViewStartPosition) = 0;

    /**
     * Length of the document or viewable data.
     * (refer setter function for detailed concept).
     * @return length in arbitrary units of the viewable data 
     */
    virtual int getTotalLength() const = 0;

    /**
     * Length of the view (refer setter function for detailed concept).
     *
     * @return view length
     */
    virtual int getViewLength() const = 0;

    /**
     * Position in arbitrary units of the start of the currently viewed data 
     * (refer setter function for detailed concept).
     * @return position of the start of the view
     */
    virtual int getViewStartPosition() const = 0;

    };

    } // namespace Alf

#endif // IALF_SCROLLBARMODEL_H
//End Of File
