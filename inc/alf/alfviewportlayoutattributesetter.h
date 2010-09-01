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
* Description:   Viewport layout attribute setter header.
*
*/


#ifndef ALFVIEWPORTLAYOUTATTRIBUTESETTER_H
#define ALFVIEWPORTLAYOUTATTRIBUTESETTER_H

//includes

//widget model includes
#include "alf/alfcommonlayoutattributesetter.h"

//osn includes
#include <osn/osndefines.h>


namespace Alf
    {

/**
 *  @class AlfViewportLayoutAttributeSetter alfviewportlayoutattributesetter.h
 *  "alf/alfviewportlayoutattributesetter.h"
 *
 *  The implementation of the attribute setter for setting viewport 
 *  layout attributes.
 *
 *  @see IAlfAttributeSetter
 *
 *  @lib alfwidgetmodel.lib
 *  @since S60 ?S60_version
 *  @status Draft
 *  @interfaces IAlfAttributeSetter
 */
class AlfViewportLayoutAttributeSetter : public AlfCommonLayoutAttributeSetter
    {

public:

    /**
     * Constructor.
     */
    OSN_IMPORT AlfViewportLayoutAttributeSetter();

    /**
     * Destructor.
     */
    OSN_IMPORT virtual ~AlfViewportLayoutAttributeSetter();


    /**
     * Sets an attribute value in the target visual immediately without a transition.
     * @exception osncore::AlfVisualException Thrown with error code 
     *     osncore::EInvalidVisual if aVisual is not viewport layout.
     * @exception osncore::AlfDataException Thrown with error code 
     *     osncore::EInvalidVariantDataType if aData  for data attributes is 0.
     * @exception osncore::AlfDataException Thrown with error code 
     *     osncore::EIncompleteAttributeValues if the aContainer contains an
     *     incomplete set of attributes. See language specification on creating
     *     attributes for viewport layout. 
     *
     * @param aVisual The target visual. Doesn't take ownership
     * @param aContainer The container holding the new value of the attribute.
     *      Ownership of the object is not transferred.
     * @param aData Used with data attributes to fetch the data.
     */
    OSN_IMPORT virtual void setAttributeValue (
                                CAlfVisual &aVisual,
                                AlfAttributeContainer* aContainer,
                                IAlfMap* aData );

    /**
     * Depracated! Do not use!
     * Creates a command to change the value of an attribute in the target
     * visual gradually with a transition.
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
    OSN_IMPORT virtual TAlfCommand* createCommand (
                                        CAlfVisual& aVisual,
                                        AlfAttributeContainer* aContainer,
                                        IAlfMap* aData,
                                        int aTransitionTime = 0,
                                        CAlfVisual* aRefVisual = NULL );

    /**
     * Creates commands to change the given attribute values in the target
     * visual gradually with a transition. Then sends the commands via the
     * ALF environment.
     *
     * @param aVisual The target visual. Doesn't take ownership
     * @param aContainer The attribute container holding the attribute value(s)
     *     used by the attribute setter. Ownership is not transferred.The
     *     container can hold new values for multiple different attributes, in
     *     which case several commands are executed.
     * @param aRefVisual Optional reference visual. If not NULL, the target
     *     value of the attribute is defined by adding the value specific by 
     *     aContainer to the corresponding attribute value in the reference
     *     visual.
     */
    OSN_IMPORT virtual void createAndSendCommands (
                                CAlfVisual& aVisual,
                                AlfAttributeContainer* aContainer,
                                CAlfVisual* aRefVisual = NULL );


protected:

    /**
     * Sets a dynamic attribute value in the target visual using transitions
     * defined in attributes
     *
     * @since S60 ?S60_version
     * @param aVisual The target visual. Doesn't take ownership
     * @param aAttr The attribute to process. Doesn't take ownership
     * @param aContainer The container holding attributes. Ownership of the
     *      object is not transferred.
     */
    virtual void handleDynamicAttribute (
                     CAlfVisual &aVisual,
                     AlfAttribute& aAttr,
                     AlfAttributeContainer& aContainer);

    /**
     * Sets a value for static attribute in the target visual immediately
     * without a transition using data in map.
     *
     * @param aVisual The target visual. Doesn't take ownership
     * @param aAttr The attribute to process. Doesn't take ownership
     * @param aContainer The container holding attributes. Ownership of the
     *      object is not transferred.
     * @param aData Contains data.Ownership of the object is not transferred.
     */
    virtual void handleStaticDataAttribute (
                     CAlfVisual &aVisual, AlfAttribute& aAttr,
                     AlfAttributeContainer& aContainer, IAlfMap* aData );

    /**
     * Sets a value for dynamic attribute value in the target visual using
     * transitions and data in map.
     *
     * @param aVisual The target visual. Doesn't take ownership
     * @param aAttr The attribute to process. Doesn't take ownership
     * @param aContainer The container holding attributes. Ownership of the
     *      object is not transferred.
        * @param aData Contains data.Ownership of the object is not transferred.
     */
    virtual void handleDynamicDataAttribute (
                     CAlfVisual &aVisual, AlfAttribute& aAttr,
                     AlfAttributeContainer& aContainer, IAlfMap* aData);

    /**
     * Sets a value of a static attribute to target visual immediately without
     * a transition.
     *
     * @param aVisual The target visual. Doesn't take ownership
     * @param aAttr The attribute to process. Doesn't take ownership
     * @param aContainer The container holding attributes. Ownership of the
     *      object is not transferred.
     * @param aData Contains data.Ownership of the object is not transferred.
     */
    virtual void handleStaticAttribute (
                     CAlfVisual &aVisual, AlfAttribute& aAttr,
                     AlfAttributeContainer& aContainer);

private:

    /**
     * Sets a dynamic attribute value in the target visual using transitions
     * defined in attributes
     *
     * @since S60 ?S60_version
     * @param aVisual The target visual. Doesn't take ownership
     * @param aAttr The attribute to process. Doesn't take ownership
     * @param aContainer The container holding attributes. Ownership of the
     *      object is not transferred.
     * @param aImmediate if true, the change is immediate and time value of
     *      the attribute is not used.
     */
    bool doHandleDynamicAttribute (
            CAlfVisual &aVisual, AlfAttribute& aAttr,
            AlfAttributeContainer& aContainer, bool aImmediate = false);

    /**
     * Sets a value for dynamic attribute value in the target visual using
     * transitions and data in map.
     *
     * @param aVisual The target visual. Doesn't take ownership
     * @param aAttr The attribute to process. Doesn't take ownership
     * @param aContainer The container holding attributes. Ownership of the
     *      object is not transferred.
     * @param aData Contains data.Ownership of the object is not transferred.
     * @param aImmediate if true, the change is immediate and time value of
     *      the attribute is not used.
     */
    bool doHandleDynamicDataAttribute (
             CAlfVisual &aVisual, AlfAttribute& aAttr,
             AlfAttributeContainer& aContainer, IAlfMap* aData,
             bool aImmediate = false);

    /**
    * Gets the data from map pointed by attribute's field name.
    *
    * @param aAttr The attribute to process. Doesn't take ownership
    * @param aData Contains data.Ownership of the object is not transferred.
    *
    * @return pointer to the data from map pointed by attribute's field name.
    */
    IAlfVariantType* getData(AlfAttribute& aAttr, IAlfMap* aData);

    /**
    * gets the time from attribute safely.
    *
    * @param aAttr The attribute to process. Doesn't take ownership
    * @param aImmediate if true, returns 0 else the time set for the attribute
    */
    int getTime(const AlfAttribute& aAttr, bool aImmediate = false);

private: // data
    };

    } // namespace Alf

#endif // ALFVIEWPORTLAYOUTATTRIBUTESETTER_H
// End of file


