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
* Description:  flow layout policy.
*
*/


#ifndef I_ALFFLOWLAYOUTPOLICY
#define I_ALFFLOWLAYOUTPOLICY

#include <alf/ialfinterfacebase.h>
#include <alf/alftypes.h>

namespace Alf
    {

namespace ialfflowlayoutpolicy
    {
     static const IfId Ident =
        {
        0, "flowlayoutpolicy"
        };
    }

/**
 * interface for flow layout policy. The policy-interface can be queried
 * from the layoutmanager using the makeInterface- call. 
 * @see IAlfLayoutManager
 *
 * @lib alfwidgetmodel.lib
 * @since S60 ?S60_version
 * @status Draft
 */
class IAlfFlowLayoutPolicy : public IAlfInterfaceBase
    {
public:

    /** Flow directions. */
    enum flowDirection
        {
        /**
         * Flow horizontally. System wide layout mirroring is considered
         * to determine whether visuals are layed from left to right
         * or from right to left.
         */
        EFlowHorizontal,
        
        /** Flow vertically. */
        EFlowVertical
        };
        
    /** Mode flags that specify how the flow is laid out. */
    enum flowModeFlags
        {
        /** Center the children perpendicular to the flow direction.
            For example, if the flow direction is horizontal (left->right)
            this flag will center the children vertically along the flow. */
        EFlowModeCenterPerpendicular = 0x1,
        
        /** Fit the children perpendicular to the flow direction.
            For example, if the flow direction is horizontal, the children
            will be vertically resized to fit the layout's rectangle. */
        EFlowModeFitPerpendicular = 0x2
        };
public:
       
    static inline const IfId& type()
        {
        return ialfflowlayoutpolicy::Ident;
        }
        
    virtual inline ~IAlfFlowLayoutPolicy() {}
    
    /**
     * Sets the direction of the flow.
     *
     * @param aDirection  Flow direction.
     */    
    virtual void setFlowDirection(enum flowDirection aDirection) = 0;
    
    /**
     * returns the flow direction
     *
     * @param aDirection  Flow direction.
     */    
    virtual enum flowDirection flowDirection() const = 0;
    
    /**
     * Sets the mode of the flow. The mode flags determine how the flow layout
     * behaves.
     *
     * @param aMode  Mode flags (OR'd together).
     *
     * @see IAlfFlowLayoutPolicy::flowModeFlags
     */
    virtual void setMode(int aMode) = 0;
    
    /**
     * Returns the mode flags of the flow.
     *
     * @see IAlfFlowLayoutPolicy::flowModeFlags
     *
     * @return  Returns the current mode flags.
     */
    virtual int mode() const = 0;

public:
// from base class IAlfInterfaceBase
    
    /**
     * Interface getter. 
     * @see IAlfInterfaceBase::MakeInterface
     *
     * @since S60 ?S60_version
     * @param aType The type id of the queried interface.
     * @return The queried interface, or NULL if the interface is not
     *         supported or available.
     */    
    virtual IAlfInterfaceBase* makeInterface( const IfId& aType ) = 0;
    };
    
    } // namespace Alf

#endif // I_ALFFLOWLAYOUTPOLICY
