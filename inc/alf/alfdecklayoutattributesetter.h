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
* Description:   Deck layout attribute setter Header.
*
*/


#ifndef ALFDECKLAYOUTATTRIBUTESETTER_H
#define ALFDECKLAYOUTATTRIBUTESETTER_H

//includes

//widget model includes
#include "alf/alfcommonlayoutattributesetter.h"

//osn includes
#include <osn/osndefines.h>


namespace Alf
    {

/**
 *  @class AlfDeckLayoutAttributeSetter alfdecklayoutattributesetter.h "alf/alfdecklayoutattributesetter.h"
 *  The implementation of the attribute setter for setting curve path layout attributes.
 *  @see IAlfAttributeSetter
 *
 *  @lib alfwidgetmodel.lib
 *  @since S60 ?S60_version
 *  @status Draft
 *  @interfaces IAlfAttributeSetter
 */
 class AlfDeckLayoutAttributeSetter : public AlfCommonLayoutAttributeSetter
    {

public:

    /**
     * Constructor.
     */
    OSN_IMPORT AlfDeckLayoutAttributeSetter();

    /**
     * Destructor.
     */
    OSN_IMPORT virtual ~AlfDeckLayoutAttributeSetter();


    /**
     * Sets an attribute value in the target visual immediately without a transition.
     * @exception osncore::AlfVisualException Thrown with error code osncore::EInvalidVisual if aVisual 
     *                                        is not a deck layout.
     *
     * @param aVisual The target visual. Doesn't take ownership
     * @param aContainer The container holding the new value of the attribute.
     *      Ownership of the object is not transferred.
     * @param aData Used with data attributes to fetch the data.
     */
    OSN_IMPORT virtual void setAttributeValue (CAlfVisual &aVisual,
            AlfAttributeContainer* aContainer, IAlfMap* aData );

    /**
     * Depracated! Do not use!
     * Creates a command to change the value of an attribute in the target visual
     * gradually with a transition.
     *
     * @param aVisual The target visual. Doesn't take ownership
     * @param aContainer The container holding the new value of the attribute.
     *      Ownership of the object is not transferred.
     * @param aData Not used currently.
     * @param aTransitionTime Time used for the transition.
     * @param aRefVisual Optional reference visual. If not NULL, the target
     *      value of the attribute is defined by adding the value
     *      specific by aContainer to the corresponding attribute
     *      value in the reference visual.
     * @ret The command to gradually change the attribute in the target visual.
     */
    OSN_IMPORT virtual TAlfCommand* createCommand (CAlfVisual& aVisual,
            AlfAttributeContainer* aContainer, IAlfMap* aData,
            int aTransitionTime = 0, CAlfVisual* aRefVisual = NULL );

    /**
     * Creates commands to change the given attribute values in the target
     * visual gradually with a transition.
     * Then sends the commands via the ALF environment.
     *
     * @param aVisual The target visual. Doesn't take ownership
     * @param aContainer The attribute container holding the attribute value(s)
     *      used by the attribute setter. Ownership is not transferred.
     *     The container can hold new values for multiple different
     *     attributes, in which case several commands are executed.
     * @param aRefVisual Optional reference visual. If not NULL, the target
     *      value of the attribute is defined by adding the value
     *      specific by aContainer to the corresponding attribute
     *      value in the reference visual.
     */
    OSN_IMPORT virtual void createAndSendCommands (
        CAlfVisual& aVisual,
        AlfAttributeContainer* aContainer,
        CAlfVisual* aRefVisual = NULL );

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

private:// data
    };

    }// namespace Alf

#endif // ALFDECKLAYOUTATTRIBUTESETTER_H
// End of file

