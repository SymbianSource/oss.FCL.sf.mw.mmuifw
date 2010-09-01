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
* Description:   attribute setter base implementation header.
*
*/


#ifndef ALFCOMMONATTRIBUTESETTER_H
#define ALFCOMMONATTRIBUTESETTER_H

#include "alf/ialfattributesetter.h"

namespace Alf
    {

/**
 *  @class AlfCommonAttributeSetter alfcommonattributesetter.h "alf/alfcommonattributesetter.h"
 *  Base implementation for attribute setters.
 *  @see IAlfAttributeSetter 
 *
 *  @lib alfwidgetmodel.lib
 *  @since S60 ?S60_version
 *  @status Draft
 *  @interfaces IAlfAttributeSetter
 */
class AlfCommonAttributeSetter : public IAlfAttributeSetter
    {
public:

    /**
     * Constructor.
     *
     * @param aType Type of the attribute setter.
     */
    OSN_IMPORT AlfCommonAttributeSetter();

    /**
     * Destructor.
     */
    OSN_IMPORT virtual ~AlfCommonAttributeSetter();

    // from base class IAlfAttributeSetter

    /**
     * Returns the type of the attribute setter.
     *
     * @return The type of the attribute setter.
     */
    OSN_IMPORT virtual SetterType setterType();

    /**
     * Sets an attribute value in the target visual immediately without a transition.
     * @exception osncore::AlfAttributeException Thrown with error code osncore::EInvalidAttribute if aContainer is invalid. 
     *
     * @param aVisual The target visual. Doesn't take ownership
     * @param aContainer The container holding the new value of the attribute.
     *        Ownership of the object is not transferred.
     */
    OSN_IMPORT virtual void setAttributeValue(
        CAlfVisual &aVisual, AlfAttributeContainer* aContainer, IAlfMap* aData);

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

// from base class IAlfInterfaceBase

    /**
     * From IAlfInterfaceBase.
     * Getter for interfaces provided by the element classes.
     * Derived classes should always call the base class method
     * from the overridden makeInterface.
     *
     * @since S60 ?S60_version
     * @param aType A descriptor to identify the type of the queried interface.
     * @return The queried interface, or NULL if the interface is not
     *         supported or available.
     */
    OSN_IMPORT IAlfInterfaceBase* makeInterface(const IfId& aType);

    /**
     * Converts text between Unicode (UCS-2) and the Unicode transformation format UTF-8.
     *
     * @param aSrc The source string.
     * @return Converted string.
     */
    HBufC* convertToUnicodeFromUtf8(const TPtrC8& aSrc) const;

    /**
     * Sets Dirty check flag.
     *
     * @param aCheck whether dirty check should be enabled or not.
     */
    OSN_IMPORT void enableDirtyCheck(bool aCheck);
protected:

    /**
     * Sets a value of a static attribute to target visual immediately without a transition.
     *
     * @param aVisual The target visual. Ownership not passed.
     * @param aAttr The attribute to process. Ownership not passed.
     * @param aContainer The container holding attributes. Ownership not passed.
     */
    virtual void handleStaticAttribute(CAlfVisual& aVisual, AlfAttribute& aAttr,
                                       AlfAttributeContainer& aContainer);

    /**
     * Sets a dynamic attribute value in the target visual using transitions defined in attributes
     *
     * @param aVisual The target visual. Ownership not passed.
     * @param aAttr The attribute to process. Ownership not passed.
     * @param aContainer The container holding attributes. Ownership not passed.
     */
    virtual void handleDynamicAttribute(CAlfVisual& aVisual, AlfAttribute& aAttr,
                                        AlfAttributeContainer& aContainer);

    /**
     * Sets a value for static data attribute in the target visual
     * immediately without a transition using data in map.
     *
     * @param aVisual The target visual. Ownership not passed.
     * @param aAttr The attribute to process. Ownership not passed.
     * @param aContainer The container holding attributes. Ownership not passed.
     * @param aData Contains data. Ownership not passed.
     */
    virtual void handleStaticDataAttribute(CAlfVisual& aVisual, AlfAttribute& aAttr,
                                           AlfAttributeContainer& aContainer, IAlfMap* aData);

    /**
      * Sets a value for dynamic data attribute value in the target visual using transitions and data in map.
      *
      * @param aVisual The target visual. Ownership not passed.
      * @param aAttr The attribute to process. Ownership not passed.
      * @param aContainer The container holding attributes. Ownership not passed.
      * @param aData Contains data. Ownership not passed.
      */
    virtual void handleDynamicDataAttribute(CAlfVisual& aVisual, AlfAttribute& aAttr,
                                            AlfAttributeContainer& aContainer, IAlfMap* aData);

private: // data
    bool mDoDirtycheck;
    };

    } // namespace Alf

#endif // ALFCOMMONATTRIBUTESETTER_H
