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
* Description:  The private implementation of AlfHostAPI.
*
*/


#ifndef ALFHOSTAPIIMPL_H
#define ALFHOSTAPIIMPL_H

#include <osn/ustring.h>
#include <osn/alfptrvector.h>
#include <memory>

#include "ialfhostapi.h"

namespace Alf
    {

class CAlfWidgetControl;
class AlfWidget;
/**
 *  The private implementation of CAlfHostAPIImpl.
 *
 *  @lib alfwidgetmodel.lib
 *  @since S60 ?S60_version
 */
class AlfHostAPIImpl:public IAlfHostAPI
    {
public:

    /**
     * Constructor.
     *
     * @since S60 ?S60_version
     */
    AlfHostAPIImpl();

    /**
     * Destructor.
     *
     * @since S60 ?S60_version
     */
    virtual ~AlfHostAPIImpl();
    
    /**
     * Set the host control.Should be set before any IAlfHostAPI call is made
     */
    void setHostControl(CAlfWidgetControl& aControl);
    /**
     * setConnection.
     *
     * @since S60 ?S60_version
     */
    void setConnection(CAlfWidgetControl& aControl, AlfRole aRole);
    /**
     * getConnection.
     *
     * @since S60 ?S60_version
     */
    CAlfWidgetControl* getConnection(int aIndex) const;
    
    /**
     * gets the number of connections
     *
     * @since S60 ?S60_version
     */
    int getConnectionCount() const;
    
    /**
     * gets the connection index of aControl
     *
     * @since S60 ?S60_version
     * @param aControl control, which index to return.
     */
    int getConnectionIndex(CAlfWidgetControl& aControl) const;
        
    /**
     * removeConnection.
     *
     * @since S60 ?S60_version
     */
    void removeConnection(CAlfWidgetControl& aControl);
    /**
     * setBaseLayout.
     *
     * @since S60 ?S60_version
     */
    void setBaseLayout(IAlfLayoutManager& aLayout);
    /**
     * getBaseLayout.
     *
     * @since S60 ?S60_version
     */
    IAlfLayoutManager* getBaseLayout() const;
    /**
     * Template getter for interfaces provided by the control classes.
     * The type of the queried interface is specified by the
     * template parameter.
     *
     * @since S60 ?S60_version
     * @param aControl The control to get the interface for.
     * @return The queried interface, or NULL if the interface is not
     *         supported or available.
     */
    template <class T>
    static T* makeInterface( AlfHostAPIImpl* aControl );
    /**
     * From MAlfInterfaceBase.
     * Getter for interfaces provided by the control classes.
     * Derived classes should always call the base class method
     * from the overridden makeInterface.
     *
     * @since S60 ?S60_version
     * @param aType A descriptor to identify the type of the queried interface.
     * @return The queried interface, or NULL if the interface is not
     *         supported or available.
     */
    virtual IAlfInterfaceBase* makeInterface( const IfId& aType );
private:
    /**
     * Base layout of container widget.
     */
    auto_ptr<IAlfLayoutManager> mBaseLayout;
    /**
     * Host control for IAlfHostAPIs. Not Owned.
     */
    CAlfWidgetControl *mHostControl;
    }; //class AlfHostAPIImpl
    } // namespace Alf

#endif // C_ALFHOSTAPIIMPL_H
