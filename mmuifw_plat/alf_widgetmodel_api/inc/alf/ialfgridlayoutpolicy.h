/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  interface for layout policy
*
*/


#ifndef I_ALFGRIDLAYOUTMANAGER
#define I_ALFGRIDLAYOUTMANAGER

#include <alf/ialfinterfacebase.h>
#include <alf/alftypes.h>

class CAlfLayout;
class CAlfControl;

namespace Alf
    {

class CAlfWidgetControl;

namespace ialfgridlayoutpolicy
    {
     static const IfId Ident =
        {
        0, "gridlayoutpolicy"
        };
    }
    
using Alf::CAlfWidgetControl;



/**
 * The interface for grid layout policy
 *
 * Provides grid specific layout APIs
 * 
 * @code
 * // Create layout manager interface.
 * IAlfLayoutManager* layoutManager = IAlfInterfaceBase::makeInterface<IAlfLayoutManager>(control);
 *
 * // See if layout policy is supported in the given layout manager.
 * IAlfGridLayoutPolicy* layoutPolicy = IAlfInterfaceBase::makeInterface<IAlfGridLayoutPolicy>(layoutManager);
 *
 * if(layoutPreferences != 0)
 *     {
 *     int rows = layoutPolicy->dimensionCount(EAlfGridDimensionRow);
 *     }
 * @endcode
 *
 * @lib alfwidgetmodel.lib
 * @since S60 ?S60_version
 * @status Draft
 */
class IAlfGridLayoutPolicy : public IAlfInterfaceBase
    {
public:
       
    static inline const IfId& type()
        {       
        return ialfgridlayoutpolicy::Ident;
        }
        
    virtual inline ~IAlfGridLayoutPolicy() {}
    
public:
    /* Each dimension corresponds to a different axis for the grid blocks */
    enum gridLayoutDimension
        {
        EGridDimensionColumn = 0,
        EGridDimensionRow
        };

public:
    
    /**
     * Sets the number and weights of blocks in this grid layout in the direction of the specified
     * dimension. Each block's weight will be set to equal the supplied value, the result
     * of which is that all blocks will be equally spaced, whatever the units. 
     *
     * @see appendWeight; for more information on how weights are used to calculate layout positions.
     *
     * @note This API could be useful if then subsequent calls to @c replaceWeight are made to 
     * change specific values, depending on how many of the values are different. 
     * @note Calling this with aCount set to 0 will clear all of the existing weights in the direction
     * of the specified dimension.
     * @note this is the same as using metrics with unit type EAlfUnitWeight.
     *
     * @param aDim the dimension along which to fill
     * @param aCount The number of lines of blocks to fill
     * @param aWeight the weights to be used for all blocks
     * @throw AlfVisualException
     */    
    virtual void fillWeights(gridLayoutDimension aDim, int aCount, const TAlfMetric& aWeight) = 0;

    /**
     * Add a new line of blocks to this grid layout at the last position in the direction of the 
     * specified dimension. It will have the supplied weight. Weights can be any metric value, 
     * hence different units can be used for each block. In particular, EAlfUnitWeight can be used 
     * to represent weight values in aribtrary proportional units.
     *
     * In the case of proportional weights, the effect of this will be to cause the other blocks to 
     * resize according to the new total weight.
     *
     * @note If non-relative coordinates are specified (e.g., real pixels), the combined blocks might not 
     *      fill the entire layout area. However, weights will always stretch to fill all available space 
     *      after the fixed units have been determined. Therefore, depending on circumstances it may
     *      be better to use EAlfUnitWeight
     * @note For example [2 weights, 1 weight, 2 weights] in a layout of 100 pixels would result 
     *      in [40 pixels, 20 pixels, 40 pixels]. 
     * @note For example [10 pixels, 1 weight, 15 pixels] in a layout of 100 pixels would result 
     *      in [10 pixels, 75 pixels, 15 pixels]. 
     *
     * @param aDim the dimension to which the weight corresponds
     * @param aWeight the weight to be used for the block in the specified dimension, 
     *          replacing any previously existing weight for that block
     * @throw AlfVisualException
     */
    virtual void appendWeight(gridLayoutDimension aDim, const TAlfMetric& aWeight) = 0;

    /**
     * Add a new line of blocks to this grid layout at the specified position in the direciton of 
     * the specified dimension. It will have the supplied weight. In the case of proportional 
     * weights, the effect of this will be to cause the other blocks to resize according to the new 
     * total weight. It will also mean that many child visuals will now occupy different blocks within
     * the grid according to how the blocks wrap.
     *
     * @note will leave if the specified position is greater than the number of objects currently in the array, 
     * so check first by calling @c preferredDimensionCount
     * @see appendWeight for more information on how weights are used to calculate layout positions.
     *
     * @param aDim the dimension to which the weight corresponds
     * @param aWeight the weight to be used for the block in the specified dimension, 
     *          replacing any previously existing weight for that block
     * @param aPos the index of the block
     * @throw AlfVisualException
     */
    virtual void insertWeight(gridLayoutDimension aDim, const TAlfMetric& aWeight, int aPos) = 0;
    
    /**
     * Sets the weight of a specific line of blocks in this grid layout, in the direction of the supplied dimension.
     * In the case of proportional weights, the effect of this will be to cause the 
     * other blocks to resize according to the new total weight. 
     *
     * @see appendWeight for more information on how weights are used to calculate layout positions.
     *
     * @param aDim the dimension to which the weight corresponds
     * @param aWeight the weight to be used for the block in the specified dimension, 
     *          replacing any previously existing weight for that cell
     * @param aPos the index of the cell
     * @throw AlfVisualException
     */
    virtual void replaceWeight(gridLayoutDimension aDim, const TAlfMetric& aWeight, int aPos) = 0;

    /**
     * Remove a line of blocks from this grid layout at the specified position in the 
     * specified dimension. In the case of proportional weights, the effect of this will 
     * be to cause the other blocks to resize according to the new total weight. It will also mean 
     * that many child visuals will now occupy different blocks within the grid according to how 
     * the blocks wrap.
     *
     * @see appendWeight for more information on how weights are used to calculate layout positions.
     *
     * @param aDim the dimension to which the weight corresponds
     * @param aPos the index of the cell
     * @throw AlfVisualException
     */
    virtual void removeWeight(gridLayoutDimension aDim, int aPos) = 0;

    /**
     * Returns the weight of a specific line of blocks in this grid layout, in the
     * specified dimension. 
     *
     * @see appendWeight for more information on how weights are used to calculate layout positions.
     *
     * @param aDim the dimension to which the weight corresponds
     * @param aPos the index of the cell
     * @return the weight being used for the cell in the specified dimension, will be 0 magnitude if not set
     * @throw AlfVisualException
     */
    virtual TAlfMetric weight(gridLayoutDimension aDim, int aPos) const = 0;
        
    /**
     * Return the number of lines of blocks in this grid, along the specified dimension
     *
     * @param aDim the dimension along which to count
     * @return The number of lines of blocks in this grid.
     */        
    virtual int count(gridLayoutDimension aDim) const = 0;
    
public:
// from base class IAlfInterfaceBase
    
    /**
     * Interface getter. 
     * @see IAlfInterfaceBase::MakeInterface
     *
     * @param aType The type id of the queried interface.
     * @return The queried interface, or NULL if the interface is not
     *         supported or available.
     */    
    virtual IAlfInterfaceBase* makeInterface( const IfId& aType ) = 0;
    };

    } // namespace Alf

#endif // I_ALFGRIDLAYOUTMANAGER
