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
* Description:   Attribute setter Interface header.
*
*/


#ifndef I_ALFATTRIBUTESETTER_H
#define I_ALFATTRIBUTESETTER_H

#include <alf/ialfinterfacebase.h>
#include <alf/alftypes.h>
#include "alf/alfsettertype.h"
#include <alf/alfvisual.h>
#include <alf/alfvarianttype.h>
#include <alf/alfcommand.h>
#include <osn/osntypes.h>

namespace Alf
    {

class CAlfWidgetControl;
class AlfAttribute;
class AlfAttributeContainer;
namespace alfattributesetter
    {
static const IfId ident =
    {
    0, "alfattributesetter"
    };
    }
/**
 * The interface for attribute setters used by widget controls.
 *
 * @lib alfwidgetmodel.lib
 * @since S60 ?S60_version
 * @status Draft
 */
class IAlfAttributeSetter : public IAlfInterfaceBase
    {
public:

    static inline const IfId& type()
        {
        return alfattributesetter::ident;
        }

    virtual inline ~IAlfAttributeSetter() {}

    /**
     * Returns the type of the attribute setter.
     *
     * @return The type of the attribute setter.
     */
    virtual SetterType setterType() = 0;

    /**
     * Sets an attribute value in the target visual immediately without a transition.
     *
     * @param aVisual The target visual. Doesn't take ownership
     * @param aContainer The container holding the new value of the attribute. Ownership of the object is not transferred.
     * @param aData Not used currently.
     */
    virtual void setAttributeValue(
        CAlfVisual& aVisual, AlfAttributeContainer* aContainer, IAlfMap* aData ) = 0;

    /**
     * Creates a command to change the value of an attribute in the target visual gradually with a transition.
     *
     * @deprecated USE CREATEANDSENDCOMMANDS INSTEAD!!!
     *
     * @param aVisual The target visual. Doesn't take ownership
     * @param aContainer The container holding the new value of the attribute. Ownership of the object is not transferred.
     * @param aData Not used currently.
     * @param aTransitionTime Time used for the transition.
     * @param aRefVisual Optional reference visual. If not NULL, the target value of the attribute is defined by adding
     * the value specific by aContainer to the corresponding attribute value in the reference visual.
     * @ret The command to gradually change the attribute in the target visual.
     */
    virtual  TAlfCommand* createCommand(
        CAlfVisual& aVisual,
        AlfAttributeContainer* aContainer,
        IAlfMap* aData,
        int aTransitionTime = 0,
        CAlfVisual* aRefVisual = NULL ) = 0;

    /**
     * Creates commands to change the given attribute values in the target visual gradually with a transition.
     * Then sends the commands via the ALF environment.
     *
     * @param aVisual The target visual. Doesn't take ownership
     * @param aContainer The attribute container holding the attribute value(s)
     * used by the attribute setter. Ownership is not transferred. The container
     * can hold new values for multiple different attributes, in which case several
     * commands are executed.
     * @param aRefVisual Optional reference visual. If not NULL, the target value of the attribute is defined by adding
     * the value specific by aContainer to the corresponding attribute value in the reference visual.
     */
    virtual void createAndSendCommands(
        CAlfVisual& aVisual,
        AlfAttributeContainer* aContainer,
        CAlfVisual* aRefVisual = NULL ) = 0;

    };

    } // namespace Alf

#endif // I_ALFATTRIBUTESETTER_H

// End of File
