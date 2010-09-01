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
* Description:   Line visual attribute setter Header.
*
*/


#ifndef ALFLINEVISUALATTRIBUTESETTER_H
#define ALFLINEVISUALATTRIBUTESETTER_H

#include <osn/osndefines.h>
#include "alf/alfcommonvisualattributesetter.h"

class CAlfCurvePath;
class CAlfLineVisual;

namespace Alf
    {

class AlfLineVisualAttributeSetterImpl;

/** @class AlfLineVisualAttributeSetter alflinevisualattributesetter.h 
 *  "alf/alflinevisualattributesetter.h"
 *  The implementation of the attribute setter.
 *
 *  @interfaces IAlfAttributeSetter
 *  @lib alfwidgetmodel.lib
 *  @since S60 ?S60_version
 *  @status Draft
 */
class AlfLineVisualAttributeSetter : public AlfCommonVisualAttributeSetter
    {
public:

    /**
     * Constructor.
     *
	 * @exception std::bad_alloc     
     * @param aType Type of the attribute setter.
     */
    OSN_IMPORT AlfLineVisualAttributeSetter();

    /**
     * Destructor.
     */
    OSN_IMPORT virtual ~AlfLineVisualAttributeSetter();

    /**
     * Sets an attribute value in the target visual immediately without
     * a transition.
     *
     * @exception osncore::AlfVisualException Thrown with the error code 
     *     osncore::EInvalidVisual when supplied visual is not a line visual.
     *
     * @param aVisual The target visual. Doesn't take ownership
     * @param aContainer The container holding the new value of the attribute.
     *      Ownership of the object is not transferred.
     * @param aData Not used currently.
     */
    OSN_IMPORT virtual void setAttributeValue(
        CAlfVisual &aVisual,
        AlfAttributeContainer* aContainer,
        IAlfMap* aData);

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
    OSN_IMPORT virtual TAlfCommand* createCommand(
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
     *       transferred. The container can hold new values for
     *       multiple different attributes, in which case several
     *       commands are executed.
     * @param aRefVisual Optional reference visual. If not NULL, the target
     *       value of the attribute is defined by adding the value specific by
     *       aContainer to the corresponding attribute value in the reference
     *       visual
     */
    OSN_IMPORT virtual void createAndSendCommands(
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
     * Sets a value for static attribute in the target visual immediately 
     * without any transition using data in map.
     *
     * @exception osncore::AlfDataException Thrown with the error code 
     *     osncore::ECommonError when empty attribute for line visual 
     *     is supplied .
     * @exception osncore::AlfAttributeException Thrown with the error code 
     *     osncore::EInvalidAttribute when wrong attribute for line visual 
     *     is supplied .
     * @exception osncore::AlfAttributeException Thrown with the error code 
     *     osncore::EIncompleteAttributeValues when all values of an attribute
     *     for line visual are not supplied .
     * @param aVisual The target visual. Doesn't take ownership
     * @param aAttr The attribute to process. Doesn't take ownership
     * @param aContainer The container holding attributes. Ownership of the 
     *                   object is not transferred.
     * @param aData Contains data.Ownership of the object is not transferred.
     */
    virtual void handleStaticDataAttribute ( CAlfVisual &aVisual,
        AlfAttribute& aAttr,
        AlfAttributeContainer& aContainer,
        IAlfMap* aData );

    /**
     * Sets a value for dynamic attribute value in the target visual using 
     * transitions and data in map.
     *
     * @exception osncore::AlfDataException Thrown with the error code 
     *     osncore::ECommonError when empty attribute for line visual 
     *     is supplied .
     * @param aVisual The target visual. Doesn't take ownership
     * @param aAttr The attribute to process. Doesn't take ownership
     * @param aContainer The container holding attributes. Ownership of the
     *                   object is not transferred.
     * @param aData Contains data.Ownership of the object is not transferred.
     */
    virtual void handleDynamicDataAttribute (
        CAlfVisual &aVisual,
        AlfAttribute& aAttr,
        AlfAttributeContainer& aContainer,
        IAlfMap* aData );

    /**
     * Sets a value of a static attribute to target visual immediately without
     * any transition.
     *
     * @exception osncore::AlfAttributeException Thrown with the error code 
     *     osncore::EInvalidAttribute when wrong attribute data for line visual
     *     is supplied .
     * @param aVisual The target visual. Doesn't take ownership
     * @param aAttr The attribute to process. Doesn't take ownership
     * @param aContainer The container holding attributes. Ownership of the 
     *                   object is not transferred.
     */
    virtual void handleStaticAttribute (
        CAlfVisual &aVisual,
        AlfAttribute& aAttr,
        AlfAttributeContainer& aContainer);

private:

    /**
     * Creates and returns a toolkit defined mappping function based on the
     * attribute content.
     * The int value-mapping function mapping is the following:
     * 0 - KConstMappingFunction
     * 1 - KLinearMappingFunction
     * 2 - KSineMappingFunction
     * 3 - KCosineMappingFunction
     * @param aAttribute Attribute containing the deciding int value and data
     *                   required for construction of the mapping function.
     * @param aEnv       Environment
     * @result The mapping function pointer
     */
    MAlfMappingFunction* getMappingFunction(
        AlfAttribute& aAttribute ,
        CAlfEnv& aEnv);

    /**
     * Creates and returns a toolkit defined mappping function based on the
     * container content.
     * The int value-mapping function mapping is the following:
     * 0 - KConstMappingFunction
     * 1 - KLinearMappingFunction
     * 2 - KSineMappingFunction
     * 3 - KCosineMappingFunction
     * @param aContainer Container containing the deciding int value and data
     *                   required for construction of the mapping function.
     * @param aEnv       Environment
     * @result The mapping function pointer
     */
    MAlfMappingFunction* getMappingFunctionFromContainer(
        IAlfContainer* aContainer,
        CAlfEnv& aEnv);


    /**
     * Gets the path from the line visual or creates it if does not exist
     * @param aLineVisual Line Visual
     * @result The curvepath
     */
    CAlfCurvePath* getCurvePath(CAlfLineVisual* aLineVisual);

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
     * Check if data in the variant type is int or float.
     * Return the value as a float value
     *
     * @param aData The variant type containing the value
     * return Content of the variant type
     */
    float floatOrIntFromData(IAlfVariantType* aData);



private: // data
    auto_ptr<AlfLineVisualAttributeSetterImpl> mImpl;
    };

    } // namespace Alf

#endif // ALFLINEVISUALATTRIBUTESETTER_H

