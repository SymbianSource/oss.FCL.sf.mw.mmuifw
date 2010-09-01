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
* Description:  The base class for interfaces provided by widget classes.
*
*/


#ifndef I_ALFINTERFACEBASE_H
#define I_ALFINTERFACEBASE_H



namespace Alf
    {

struct IfId;
/**
 *  The base class for interfaces provided by
 *  widget classes, including CAlfWidget, CAlfModel,
 *  CAlfWidgetControl, MAlfWidgetEventHandler,
 *  MAlfElement and derived classes.
 *
 *  @lib alfwidgetmodel.lib
 *  @since S60 ?S60_version
 *  @status Draft
 */
class IAlfInterfaceBase
    {
public:
    /**
     * Default virtual destructor.
     */
    virtual ~IAlfInterfaceBase() {}

    /**
     * Interface getter with casting.
     *
     * @since S60 ?S60_version
     * @param aType The type id of the queried interface.
     * @return The queried interface, or NULL if the interface is not
     *         supported or available.
     */
    template <class T>
    static T* makeInterface(IAlfInterfaceBase* aBase)
        {
        return (aBase? static_cast<T*>(aBase->makeInterface(T::type())): 0);
        }


    /**
     * Interface getter.
     * Derived classes should always call the base class method
     * from the overridden MakeInterface.
     *
     * @since S60 ?S60_version
     * @param aType The type id of the queried interface.
     * @return The queried interface, or NULL if the interface is not
     *         supported or available.
     */
    virtual IAlfInterfaceBase* makeInterface( const IfId& aType ) = 0;


    };

    }

#endif // I_ALFINTERFACEBASE_H

// End of File
