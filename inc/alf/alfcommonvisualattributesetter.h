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
* Description:  attributesetter for common visual attributes
*
*/


#ifndef ALFCOMMONVISUALATTRIBUTESETTER_H
#define ALFCOMMONVISUALATTRIBUTESETTER_H

#include <osn/osndefines.h>
#include <memory>
#include <osn/ustring.h>
#include "alf/alfcommonattributesetter.h"

using namespace osncore;

using std::auto_ptr;

namespace Alf
    {

/**
 *  @class AlfCommonVisualAttributeSetter alfcommonvisualattributesetter.h "alf/alfcommonvisualattributesetter.h"
 *  Base implementation for visual attribute setters. Attribute setters for custom visuals can derive from this 
 *  class.
 *  @see IAlfAttributeSetter 
 *
 *  @lib alfwidgetmodel.lib
 *  @since S60 ?S60_version
 *  @status Draft
 *  @interfaces IAlfAttributeSetter
 */
class AlfCommonVisualAttributeSetter : public AlfCommonAttributeSetter
    {
public:

    /**
     * Constructor.
     *
     */
    OSN_IMPORT AlfCommonVisualAttributeSetter();

    /**
     * Destructor.
     */
    OSN_IMPORT virtual ~AlfCommonVisualAttributeSetter();

    /**
     * Creates a command to change the value of an attribute
     * in the target visual gradually with a transition.
     *
     * @since S60 ?S60_version
     * @param aVisual The target visual. Doesn't take ownership
     * @param aContainer The container holding the new value of the attribute.
     *        Ownership of the object is not transferred.
     * @param aData Not used currently.
     * @param aTransitionTime Time used for the transition.
     * @param aRefVisual Optional reference visual. If not NULL,
     *        the target value of the attribute is defined by adding
     *        the value specific by aContainer to the corresponding
     *        attribute value in the reference visual.
     * @return The command to gradually change the attribute in the target visual.
     */
    OSN_IMPORT virtual TAlfCommand* createCommand(
        CAlfVisual& aVisual,
        AlfAttributeContainer* aContainer,
        IAlfMap* aData,
        int aTransitionTime = 0,
        CAlfVisual* aRefVisual = NULL );

    /**
     * Creates commands to change the given attribute values
     * in the target visual gradually with a transition.
     * Then sends the commands via the ALF environment.
     *
     * @since S60 ?S60_version
     * @param aVisual The target visual. Doesn't take ownership
     * @param aContainer The attribute container holding the attribute value(s)
     *        used by the attribute setter. Ownership is not transferred.
     *        The container can hold new values for multiple different
     *        attributes, in which case several commands are executed.
     * @param aRefVisual Optional reference visual. If not NULL,
     *        the target value of the attribute is defined by adding the value
     *        specific by aContainer to the corresponding attribute value
     *        in the reference visual.
     */
    OSN_IMPORT virtual void createAndSendCommands(
        CAlfVisual& aVisual,
        AlfAttributeContainer* aContainer,
        CAlfVisual* aRefVisual = NULL );

protected:

    /**
     * From AlfCommonAttributeSetter
     * Sets a value of a static attribute to target visual immediately without a transition.
     *
     * @param aVisual The target visual. Ownership not passed.
     * @param aAttr The attribute to process. Ownership not passed.
     * @param aContainer The container holding attributes. Ownership not passed.
     */
    virtual void handleStaticAttribute(CAlfVisual& aVisual, AlfAttribute& aAttr,
                                       AlfAttributeContainer& aContainer);

    /**
     * From AlfCommonAttributeSetter
     * Sets a dynamic attribute value in the target visual using transitions defined in attributes
     *
     * @param aVisual The target visual. Ownership not passed.
     * @param aAttr The attribute to process. Ownership not passed.
     * @param aContainer The container holding attributes. Ownership not passed.
     */
    virtual void handleDynamicAttribute(CAlfVisual& aVisual, AlfAttribute& aAttr,
                                        AlfAttributeContainer& aContainer);

    /**
     * From AlfCommonAttributeSetter
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
      * From AlfCommonAttributeSetter
      * Sets a value for dynamic data attribute value in the target visual using transitions and data in map.
      *
      * @param aVisual The target visual. Ownership not passed.
      * @param aAttr The attribute to process. Ownership not passed.
      * @param aContainer The container holding attributes. Ownership not passed.
      * @param aData Contains data. Ownership not passed.
      */
    virtual void handleDynamicDataAttribute(CAlfVisual& aVisual, AlfAttribute& aAttr,
                                            AlfAttributeContainer& aContainer, IAlfMap* aData);

private:

    /**
     * Sets an dynamic attribute value in the target visual using transitions defined in attributes
     *
     * @since S60 ?S60_version
     * @param aVisual The target visual. Doesn't take ownership
     * @param aAttr The attribute to process. Doesn't take ownership
     * @param aContainer The container holding attributes. Ownership of the object is not transferred.
     * @param aData Contains data.Ownership of the object is not transferred.
     * @param aImmediate if true, the change is immediate and time value of the attribute is not used.
     * @return true, if attribute was handled, false otherwise.
     */
    bool doHandleDynamicAttribute(CAlfVisual &aVisual, AlfAttribute& aAttr,
                                  AlfAttributeContainer& aContainer, bool aImmediate = false);

    /**
     * Sets an value for dynamic attribute value in the target visual using transitions and data in map.
     *
     * @since S60 ?S60_version
     * @param aVisual The target visual. Doesn't take ownership
     * @param aAttr The attribute to process. Doesn't take ownership
     * @param aContainer The container holding attributes. Ownership of the object is not transferred.
     * @param aData Contains data.Ownership of the object is not transferred.
     * @param aImmediate if true, the change is immediate and time value of the attribute is not used.
     */
    bool doHandleDynamicDataAttribute(CAlfVisual &aVisual, AlfAttribute& aAttr,
                                      AlfAttributeContainer& aContainer, IAlfMap* aData, bool aImmediate = false);

    /**
     * Creates commands to change the given attribute values in the target visual gradually with a transition.
     *
     * @since S60 ?S60_version
     * @param aVisual The target visual. Doesn't take ownership
     * @param aContainer The attribute container holding the attribute value(s)
     * used by the attribute setter. Ownership is not transferred.
     * @param aAttr main attribute, from which the command is created.
       The function can also use other attributes found in the aContainer.
     * @param aRefVisual Optional reference visual. If not NULL, the target value of the attribute is defined by adding
     * the value specific by aContainer to the corresponding attribute value in the reference visual.
     */
    void sendCommandsForAttribute(CAlfVisual& aVisual,
                                  AlfAttributeContainer& aContainer, AlfAttribute& aAttr,
                                  CAlfVisual* aRefVisual);

    /**
     * Sets an value of a static attribute to target visual immediately without a transition.
     *
     * @since S60 ?S60_version
     * @param aVisual The target visual. Doesn't take ownership
     * @param aContainer The container holding attributes. Ownership of the object is not transferred.
     * @param aData Contains data.Ownership of the object is not transferred.
     */
    void setPadding(CAlfVisual &aVisual,AlfAttributeContainer& aContainer,IAlfMap* aData);
    /**
     * Sets an value of a static attribute to target visual immediately without a transition.
     *
     * @since S60 ?S60_version
     * @param aVisual The target visual. Doesn't take ownership
     * @param aContainer The container holding attributes. Ownership of the object is not transferred.
     * @param aData Contains data.Ownership of the object is not transferred.
     */
    void setMinSize(CAlfVisual &aVisual,AlfAttributeContainer& aContainer,IAlfMap* aData);
    /**
     * Sets an value of a static attribute to target visual immediately without a transition.
     *
     * @since S60 ?S60_version
     * @param aVisual The target visual. Doesn't take ownership
     * @param aContainer The container holding attributes. Ownership of the object is not transferred.
     * @param aData Contains data.Ownership of the object is not transferred.
     */
    void setMaxSize(CAlfVisual &aVisual,AlfAttributeContainer& aContainer,IAlfMap* aData);

    /**
     * Sets an dynamic attribute value in the target visual using transitions defined in attributes.
     *
     * @since S60 ?S60_version
     * @param aVisual The target visual. Doesn't take ownership
     * @param aContainer The container holding attributes. Ownership of the object is not transferred.
     * @param aData Contains data.Ownership of the object is not transferred.
     */

    void setPos(CAlfVisual &aVisual,AlfAttributeContainer& aContainer,IAlfMap* aData,bool aImmediate);
    /**
     * Sets an dynamic attribute value in the target visual using transitions defined in attributes.
     *
     * @since S60 ?S60_version
     * @param aVisual The target visual. Doesn't take ownership
     * @param aContainer The container holding attributes. Ownership of the object is not transferred.
     * @param aData Contains data.Ownership of the object is not transferred.
     */

    void setSize(CAlfVisual &aVisual,AlfAttributeContainer& aContainer,IAlfMap* aData,bool aImmediate);
    /**
     * Sets an dynamic attribute value in the target visual using transitions defined in attributes.
     *
     * @since S60 ?S60_version
     * @param aVisual The target visual. Doesn't take ownership
     * @param aContainer The container holding attributes. Ownership of the object is not transferred.
     * @param aData Contains data.Ownership of the object is not transferred.
     */

    void setRect(CAlfVisual &aVisual,AlfAttributeContainer& aContainer,IAlfMap* aData,bool aImmediate);

	void setTactileFeedback(CAlfVisual &aVisual,AlfAttributeContainer& aContainer,IAlfMap* aData);
    
private: // data
    };

    } // namespace Alf

#endif // ALFCOMMONVISUALATTRIBUTESETTER_H

