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
* Description:  Interface implements by CAlfWidgetControl to provide containment
*
*/


#ifndef I_ALFHOSTAPI_H
#define I_ALFHOSTAPI_H

#include <alf/ialfinterfacebase.h>
#include <alf/alflayout.h>
#include <alf/ialfwidgetcontrol.h>

namespace Alf
    {

class IAlfLayoutManager;
class CAlfWidgetControl;

enum AlfRole
    {
    EChild,
    EParent
    };

namespace alfhostapi
    {
static const IfId ident =
    {
    0, "alfhostapi"
    };
    }
/**
 * The interface for host appis used by widget controls.
 *
 * @lib alfwidgetmodel.lib
 * @since S60 ?S60_version
 * @status Draft
 */
class IAlfHostAPI : public IAlfInterfaceBase
    {
public:

    static inline const IfId& type()
        {
        return alfhostapi::ident;
        }

    virtual inline ~IAlfHostAPI() {}

    /**
     * Sets a connection from this host to a given control.
     * 
     * A control can never have more than one host control. If the connected
     * control has a host control the connection from old host control
     * to connected control will be removed.
     *
     * @param aControl  Control to be connected.
     * @param aRole     Relationship with connected control.
     */
    virtual void setConnection(CAlfWidgetControl& aControl, AlfRole aRole) = 0;

    /**
     * Get the connected control at given index.
     *
     * @param  aIndex Index of connected control.
     * @return Connected control at given index. If index is not found returns null.
     */
    virtual CAlfWidgetControl* getConnection(int aIndex) const = 0;
    
    /**
     * gets the number of connections
     *
     * @since S60 ?S60_version
     */
    virtual int getConnectionCount() const = 0;
    
    /**
     * gets the connection index of aControl
     *
     * @since S60 ?S60_version
     * @param aControl control, which index to return.
     */
    virtual int getConnectionIndex(CAlfWidgetControl& aControl) const = 0;

    /**
     * Removes connection with the given control.
     *
     * If the connected control is in the same control group with host control,
     * it will be removed from the host control's control group.
     * 
     * If the connected control is in a separate control group or if the
     * host control is not in a control group, the connected control's control
     * group relation will not be affected.
     *
     * @see setConnection()
     *
     * @param aControl Control with which connection is to be removed.
     */
    virtual void removeConnection(CAlfWidgetControl& aControl) = 0;

    /**
     * Sets the base layout of control.
     *
     * @param aLayout Base layout.
     */
    virtual void setBaseLayout(IAlfLayoutManager& aLayout) = 0;

    /**
     * Returns the base layout of control.
     *
     * @return Base layout of control.Returns null id there is no base layout.
     */
    virtual IAlfLayoutManager* getBaseLayout() const = 0;


    };

    } // namespace Alf

#endif // I_ALFHOSTAPI_H
