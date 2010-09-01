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
* Description:  flow layout policy implementation.
*
*/


#ifndef ALFFLOWLAYOUTPOLICYIMPL_H
#define ALFFLOWLAYOUTPOLICYIMPL_H

#include <osn/osndefines.h>
#include <alf/ialfflowlayoutpolicy.h>

using std::auto_ptr;

class CAlfFlowLayout;

using namespace Alf;

namespace Alf
    {
class AlfFlowLayoutManager;
        
/**
 * flow layout policy implementation
 *
 * @lib alfwidgetmodel.lib
 * @since S60 ?S60_version
 */
class AlfFlowLayoutPolicyImpl : public IAlfFlowLayoutPolicy
    {
public:
    /**
     * layout policy constructor
     *
     * @param aFlowLayoutManager flow layout manager instance
     */
    AlfFlowLayoutPolicyImpl(AlfFlowLayoutManager& aFlowLayoutManager);
    
    /**
     * destructor
     */
    ~AlfFlowLayoutPolicyImpl();
    
public:
//from IAlfFlowLayoutPolicy

    /**
     * Sets the direction of the flow.
     *
     * @param aDirection  Flow direction.
     */
    virtual void setFlowDirection(enum flowDirection aDirection);
        
    /**
     * returns the flow direction
     *
     * @param aDirection  Flow direction.
     */    
    virtual enum flowDirection flowDirection() const;
    
    /**
     * Sets the mode of the flow. The mode flags determine how the flow layout
     * behaves.
     *
     * @param aMode  Mode flags (OR'd together).
     *
     * @see IAlfFlowLayoutPolicy::flowModeFlags
     */
    virtual void setMode(int aMode);
    
    /**
     * Returns the mode flags of the flow.
     *
     * @see IAlfFlowLayoutPolicy::flowModeFlags
     *
     * @return  Returns the current mode flags.
     */
    virtual int mode() const;

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
    virtual IAlfInterfaceBase* makeInterface( const IfId& aType );	
	
private:
    /**
     * returns the native flowlayout used by the implementation.
     */    
    CAlfFlowLayout& layout() const;
private:

    AlfFlowLayoutManager &mFlowLayoutManager;
    enum IAlfFlowLayoutPolicy::flowDirection mDirection;
    
    };

    } // namespace Alf

#endif // ALFFLOWLAYOUTPOLICYIMPL_H
