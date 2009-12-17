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
* Description:   Lct image Visual attribute setter Header.
*
*/


#ifndef ALFLCTIMAGEVISUALATTRIBUTESETTER_H
#define ALFLCTIMAGEVISUALATTRIBUTESETTER_H

#include "alf/alfimagevisualattributesetter.h"

namespace Alf
    {

/** @class AlfLctImageVisualAttributeSetter alflctimagevisualattributesetter.h "alf/alflctimagevisualattributesetter.h"
 *  The implementation of the attribute setter.
 *
 *  @interfaces IAlfAttributeSetter
 *  @lib alfwidgetmodel.lib
 *  @since S60 ?S60_version
 *  @status Draft
 */
class AlfLctImageVisualAttributeSetter : public AlfImageVisualAttributeSetter
    {
public:

    /**
     * Constructor.
     *
     * @param aType Type of the attribute setter.
     */
    OSN_IMPORT AlfLctImageVisualAttributeSetter();

    /**
     * Destructor.
     */
    OSN_IMPORT virtual ~AlfLctImageVisualAttributeSetter();

// from base class MAlfAttributeSetter

    /**
     * Creates a command to change the value of an attribute in the target visual
     * gradually with a transition.
     *
     * @param aVisual The target visual. Doesn't take ownership
     * @param aContainer The container holding the new value of the attribute.
     *        Ownership of the object is not transferred.
     * @param aData Not used currently.
     * @param aTransitionTime Time used for the transition.
     * @param aRefVisual Optional reference visual. If not NULL, the target value
     *        of the attribute is defined by adding the value specific by aContainer
     *        to the corresponding attribute value in the reference visual.
     * @ret The command to gradually change the attribute in the target visual.
     */
    OSN_IMPORT virtual TAlfCommand* createCommand(
        CAlfVisual& aVisual, AlfAttributeContainer* aContainer, IAlfMap* aData,
        int aTransitionTime = 0, CAlfVisual* aRefVisual = NULL);

    /**
     * Creates commands to change the given attribute values in the target visual
     * gradually with a transition.Then sends the commands via the ALF environment.
     *
     * @param aVisual The target visual. Doesn't take ownership
     * @param aContainer The attribute container holding the attribute value(s)
     * used by the attribute setter. Ownership is not transferred. The container
     * can hold new values for multiple different attributes, in which case several
     * commands are executed.
     * @param aRefVisual Optional reference visual. If not NULL, the target value
     *        of the attribute is defined by adding the value specific by aContainer
     *        to the corresponding attribute value in the reference visual.
     */
    OSN_IMPORT virtual void createAndSendCommands(CAlfVisual& aVisual,
            AlfAttributeContainer* aContainer, CAlfVisual* aRefVisual = NULL);

protected:

    /**
     * Sets an dynamic attribute value in the target visual
     * using transitions defined in attributes.
     *
     * @since S60 ?S60_version
     * @param aVisual The target visual. Doesn't take ownership
     * @param aAttr The attribute to process. Doesn't take ownership
     * @param aContainer The container holding attributes.
     *        Ownership of the object is not transferred.
     */
    virtual void handleDynamicAttribute(CAlfVisual &aVisual,
                                        AlfAttribute& aAttr, AlfAttributeContainer& aContainer);

    /**
     * Sets an value for static attribute in the target visual immediately
     * without a transition using data in map.
     *
     * @since S60 ?S60_version
     * @param aVisual The target visual. Doesn't take ownership
     * @param aAttr The attribute to process. Doesn't take ownership
     * @param aContainer The container holding attributes.
     *        Ownership of the object is not transferred.
     * @param aData Contains data.Ownership of the object is not transferred.
     */
    virtual void handleStaticDataAttribute(CAlfVisual &aVisual,
                                           AlfAttribute& aAttr, AlfAttributeContainer& aContainer, IAlfMap* aData);

    /**
     * Sets an value for dynamic attribute value in the target visual
     * using transitions and data in map.
     *
     * @since S60 ?S60_version
     * @param aVisual The target visual. Doesn't take ownership
     * @param aAttr The attribute to process. Doesn't take ownership
     * @param aContainer The container holding attributes.
     *        Ownership of the object is not transferred.
     * @param aData Contains data.Ownership of the object is not transferred.
     */
    virtual void handleDynamicDataAttribute(CAlfVisual &aVisual,
                                            AlfAttribute& aAttr, AlfAttributeContainer& aContainer, IAlfMap* aData);

    /**
     * Sets an value of a static attribute to target visual immediately
     * without a transition.
     *
     * @since S60 ?S60_version
     * @param aVisual The target visual. Doesn't take ownership
     * @param aAttr The attribute to process. Doesn't take ownership
     * @param aContainer The container holding attributes.
     *        Ownership of the object is not transferred.
     */
    virtual void handleStaticAttribute(CAlfVisual &aVisual,
                                       AlfAttribute& aAttr, AlfAttributeContainer& aContainer);

private: // data
    };

    } // namespace Alf

#endif // ALFLCTIMAGEVISUALATTRIBUTESETTER_H

