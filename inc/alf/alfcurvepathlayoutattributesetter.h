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
* Description:   curve path layout attribute setter header.
*
*/


#ifndef ALFCURVEPATHLAYOUTATTRIBUTESETTER_H
#define ALFCURVEPATHLAYOUTATTRIBUTESETTER_H

//includes

//widget model includes
#include "alf/alfcommonlayoutattributesetter.h"

//osn includes
#include <osn/osndefines.h>


namespace Alf
    {

/**
 *  @class AlfCurvePathLayoutAttributeSetter alfcurvepathlayoutattributesetter.h "alf/alfcurvepathlayoutattributesetter.h"
 *  The implementation of the attribute setter for setting curve path layout attributes.
 *  @see IAlfAttributeSetter
 *
 *  @lib alfwidgetmodel.lib
 *  @since S60 ?S60_version
 *  @status Draft
 *  @interfaces IAlfAttributeSetter
 */
class AlfCurvePathLayoutAttributeSetter : public AlfCommonLayoutAttributeSetter
    {

public:

    /**
     * Constructor.
     *
     * @param aType Type of the attribute setter.
     */
    OSN_IMPORT AlfCurvePathLayoutAttributeSetter();

    /**
     * Destructor.
     */
    OSN_IMPORT virtual ~AlfCurvePathLayoutAttributeSetter();

    /**
     * Sets an attribute value in the target visual immediately without
     * a transition.
     * @exception osncore::AlfVisualException Thrown with error code osncore::EInvalidVisual if aVisual 
     *                                        is not a curvepath layout.
     * @exception osncore::AlfDataException   Thrown with error code osncore::ECommonError if the data field  
     *                                        for data attributes is 0.
     * @exception osncore::AlfDataException   Thrown with error code osncore::ECommonError if aData  
     *                                        for data attributes is 0.
     * @exception osncore::AlfAttributeException Thrown with error code osncore::EInvalidAttribute if an  
     *                                        attempt is made to set a line attribute to a curvepath layout
     *                                        with incomplete data. See language specification on how to 
     *                                        create this attribute.
     * @exception osncore::AlfAttributeException Thrown with error code osncore::EInvalidAttribute if an  
     *                                        attempt is made to set an arc attribute to a curvepath layout
     *                                        with incomplete data. See language specification on how to 
     *                                        create this attribute.
     * @exception osncore::AlfDataException   Thrown with error code osncore::ECommonError if the attribute
     *                                        (or aData in case of data attributes) contains invalid data types
     *                                        while setting a line or arc to a curvepath layout.  
     *
     * @param aVisual The target visual. Doesn't take ownership
     * @param aContainer The container holding the new value of the attribute.
     *                   Ownership of the object is not transferred.
     * @param aData Not used currently.
     */
    OSN_IMPORT virtual void setAttributeValue (
        CAlfVisual &aVisual,
        AlfAttributeContainer* aContainer,
        IAlfMap* aData );

    /**
     * Deprecated! Do not use!
     * Creates a command to change the value of an attribute in the target
     * visual gradually with a transition.
     *
     * @param aVisual The target visual. Doesn't take ownership
     * @param aContainer The container holding the new value of the
     *      attribute.Ownership of the object is not transferred.
     * @param aData Not used currently.
     * @param aTransitionTime Time used for the transition.
     * @param aRefVisual Optional reference visual. If not NULL, the target
     *       value of the attribute is defined by adding the value
     *       specific by aContainer to the corresponding attribute
     *      value in the reference visual.
     * @ret The command to gradually change the attribute in the target visual
     */
    OSN_IMPORT virtual TAlfCommand* createCommand (
        CAlfVisual& aVisual,
        AlfAttributeContainer* aContainer,
        IAlfMap* aData,
        int aTransitionTime = 0,
        CAlfVisual* aRefVisual = NULL );

    /**
     * Creates commands to change the given attribute values in the target
     * visual gradually with a transition.
     * Then sends the commands via the ALF environment.
     *
     * @param aVisual The target visual. Doesn't take ownership
     * @param aContainer The attribute container holding attribute value(s)
     *       used by the attribute setter. Ownership is not
     *      transferred. The container can hold new values for
     *       multiple different attributes, in which case several
     *       commands are executed.
     * @param aRefVisual Optional reference visual. If not NULL, the target
     *                   value of the attribute is defined by adding the value
     *                   specific by aContainer to the corresponding attribute
     *      value in the reference visual
     */
    OSN_IMPORT virtual void createAndSendCommands (
        CAlfVisual& aVisual,
        AlfAttributeContainer* aContainer,
        CAlfVisual* aRefVisual = NULL );


protected:
    /**
     * Sets an dynamic attribute value in the target visual using transitions
     * defined in attributes
     *
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
     * Sets an value for static attribute in the target visual immediately
     * without a transition using data in map.
     *
     * @param aVisual The target visual. Doesn't take ownership
     * @param aAttr The attribute to process. Doesn't take ownership
     * @param aContainer The container holding attributes. Ownership of the object
     *                   is not transferred.
     * @param aData Contains data.Ownership of the object is not transferred.
     */
    virtual void handleStaticDataAttribute ( CAlfVisual &aVisual,
            AlfAttribute& aAttr,
            AlfAttributeContainer& aContainer,
            IAlfMap* aData );

    /**
     * Sets an value for dynamic attribute value in the target visual using transitions and data in map.
     *
     * @param aVisual The target visual. Doesn't take ownership
     * @param aAttr The attribute to process. Doesn't take ownership
     * @param aContainer The container holding attributes. Ownership of the object is not transferred.
     * @param aData Contains data.Ownership of the object is not transferred.
     */
    virtual void handleDynamicDataAttribute ( CAlfVisual &aVisual,
            AlfAttribute& aAttr,
            AlfAttributeContainer& aContainer,
            IAlfMap* aData );

    /**
     * Sets an value of a static attribute to target visual immediately without a transition.
     *
     * @param aVisual The target visual. Doesn't take ownership
     * @param aAttr The attribute to process. Doesn't take ownership
     * @param aContainer The container holding attributes. Ownership of the object is not transferred.
     */
    virtual void handleStaticAttribute ( CAlfVisual &aVisual,
                                         AlfAttribute& aAttr,
                                         AlfAttributeContainer& aContainer);

    /**
     * Check if data in the attribute is int or float.
     * Return the value as a float value
     *
     * @param aAttr The attribute containing the value
     * @param aIndex The index of the value to be checked
     * return Content of the attribute
     */
    float floatOrInt(const AlfAttribute& aAttr, int aIndex);

    /**
     * Check if data in the attribute is int or float.
     * Return the value as a float value
     *
     * @param aData The variant type containing the value
     * return Content of the variant type
     */
    float floatOrIntFromData(IAlfVariantType* aData);

private: // data
    };

    } // namespace Alf

#endif // ALFCURVEPATHLAYOUTATTRIBUTESETTER_H
// End of file


