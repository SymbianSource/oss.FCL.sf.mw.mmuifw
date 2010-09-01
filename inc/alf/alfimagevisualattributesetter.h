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
* Description:   Image visual attribute setter Header.
*
*/


#ifndef ALFIMAGEVISUALATTRIBUTESETTER_H
#define ALFIMAGEVISUALATTRIBUTESETTER_H

#include <osn/osndefines.h>
#include "alf/alfcommonvisualattributesetter.h"

class CAlfImageVisual;

namespace Alf
    {
class AlfImageVisualAttributeSetterImpl;
class IAlfVariantType;
class ResourcePool;

/**
 *  @class AlfImageVisualAttributeSetter alfimagevisualattributesetter.h "alf/alfimagevisualattributesetter.h"
 *  The implementation of the attribute setter for setting image visual attributes.
 *  @see IAlfAttributeSetter
 *
 *  @lib alfwidgetmodel.lib
 *  @since S60 ?S60_version
 *  @status Draft
 *  @interfaces IAlfAttributeSetter
 */
class AlfImageVisualAttributeSetter : public AlfCommonVisualAttributeSetter
    {
public:

    /**
     * Constructor.
     *
     * @param aType Type of the attribute setter.
     */
    OSN_IMPORT AlfImageVisualAttributeSetter();

    /**
     * Destructor.
     */
    OSN_IMPORT virtual ~AlfImageVisualAttributeSetter();

    /**
     * Sets the attribute values to the target visual.
     * @exception osncore::AlfVisualException Thrown with error code osncore::EInvalidVisual if aVisual 
     *                                        is not an image visual.
     * @exception osncore::AlfDataException   Thrown with error code osncore::EInvalidAttribute if the data field  
     *                                        for data attributes is 0.
     * @exception osncore::AlfDataException   Thrown with error code osncore::EInvalidVariantDataType if aData  
     *                                        for data attributes is 0.
     * @exception osncore::AlfDataException   Thrown with error code osncore::EIncompleteAttributeValues if an  
     *                                        attempt is made to set a color attribute with insufficient data. 
     *                                        See language specification on how to create this attribute.
     * @exception osncore::AlfDataException   Thrown with error code osncore::EInvalidAttribute if an  
     *                                        attempt is made to set a color attribute with incompatible data types. 
     *                                        See language specification on how to create this attribute.
     * @exception osncore::AlfDataException   Thrown with error code osncore::EInvalidAttribute if an  
     *                                        attempt is made to set a drop shadow attribute with incompatible data. 
     *                                        See language specification on how to create this attribute.
     * @exception osncore::AlfDataException   Thrown with Symbian error code if the skin animation ID cannot be retreived 
     *                                        while setting an animated skin image.
     * @exception osncore::AlfDataException   Thrown with Symbian error code if the skin frame ID cannot be retreived 
     *                                        while setting an skin frame image.
     * @exception osncore::ThemeException     Thrown when skinid is not found.
     *
     * @since S60 ?S60_version
     * @param aVisual The target visual. Doesn't take ownership.
     * @param aContainer The container holding the value of the attributes.
     *        Ownership of the object is not transferred.
     * @param aData Used with data attributes to fetch the data.
     */
    OSN_IMPORT virtual void setAttributeValue (
        CAlfVisual &aVisual,
        AlfAttributeContainer* aContainer,
        IAlfMap* aData );

    /**
     * Creates a command to change the value of an attribute in the target visual gradually with a transition.
     *
     * @param aVisual The target visual. Doesn't take ownership
     * @param aContainer The container holding the new value of the attribute. Ownership of the object is not transferred.
     * @param aData Not used currently.
     * @param aTransitionTime Time used for the transition.
     * @param aRefVisual Optional reference visual. If not NULL, the target value of the attribute is defined by adding
     * the value specific by aContainer to the corresponding attribute value in the reference visual.
     * @ret The command to gradually change the attribute in the target visual.
     */
    OSN_IMPORT virtual TAlfCommand* createCommand(
        CAlfVisual& aVisual,
        AlfAttributeContainer* aContainer,
        IAlfMap* aData,
        int aTransitionTime = 0,
        CAlfVisual* aRefVisual = NULL );

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
    OSN_IMPORT virtual void createAndSendCommands(
        CAlfVisual& aVisual,
        AlfAttributeContainer* aContainer,
        CAlfVisual* aRefVisual = NULL );

protected:

    /**
     * From AlfCommonVisualAttributeSetter
     * Sets an value of a static attribute to target visual immediately without a transition.
     *
     * @param aVisual The target visual. Doesn't take ownership
     * @param aAttr The attribute to process. Doesn't take ownership
     * @param aContainer The container holding attributes. Ownership of the object is not transferred.
     * @param aData Contains data.Ownership of the object is not transferred.
     */
    virtual void handleStaticAttribute(CAlfVisual &aVisual, AlfAttribute& aAttr, AlfAttributeContainer& aContainer);

    /**
     * From AlfCommonVisualAttributeSetter
     * Sets an dynamic attribute value in the target visual using transitions defined in attributes
     *
     * @param aVisual The target visual. Doesn't take ownership
     * @param aAttr The attribute to process. Doesn't take ownership
     * @param aContainer The container holding attributes. Ownership of the object is not transferred.
     * @param aData Contains data.Ownership of the object is not transferred.
     */
    virtual void handleDynamicAttribute(CAlfVisual &aVisual, AlfAttribute& aAttr, AlfAttributeContainer& aContainer);

    /**
     * From AlfCommonVisualAttributeSetter
     * Sets an value for static attribute in the target visual immediately without a transition using data in map.
     *
     * @param aVisual The target visual. Doesn't take ownership
     * @param aAttr The attribute to process. Doesn't take ownership
     * @param aContainer The container holding attributes. Ownership of the object is not transferred.
     * @param aData Contains data.Ownership of the object is not transferred.
     */
    virtual void handleStaticDataAttribute(CAlfVisual &aVisual, AlfAttribute& aAttr, AlfAttributeContainer& aContainer, IAlfMap* aData);

    /**
     * From AlfCommonVisualAttributeSetter
     * Sets an value for dynamic attribute value in the target visual using transitions and data in map.
     *
     * @param aVisual The target visual. Doesn't take ownership
     * @param aAttr The attribute to process. Doesn't take ownership
     * @param aContainer The container holding attributes. Ownership of the object is not transferred.
     * @param aData Contains data.Ownership of the object is not transferred.
     */
    virtual void handleDynamicDataAttribute(CAlfVisual &aVisual, AlfAttribute& aAttr, AlfAttributeContainer& aContainer, IAlfMap* aData);

private:
    /**
     * Sets FrameImage for aVisual using the frame skin id stored in aAttr
     *
     * @param aVisual The target visual. Doesn't take ownership
     * @param aSkinFrameImage LogicalName for the frame image. Ownership of the object is not transferred
	 * @param aContainer The container holding attributes. Ownership of the object is not transferred.
	 * @param aData datamap used in staticdata case. Ownership of the object is not transferred.
	 *
	 * @exception osncore::ThemeException  Thrown when SkinId is not found. 
     */
    void setFrameImage(CAlfVisual& aVisual,const Utf8* aSkinFrameID, const AlfAttributeContainer& aContainer,IAlfMap* aData=NULL);
    /**
     * Gets the attribute value based on whether attribute type is static or staticdata.
	 *
     * @param aAttr The attribute to process. Doesn't take ownership.
     * @param aData datamap used in staticdata case. Doesn't take ownership
     *
     * @return returns the attributeValue.. 	 
     */ 
    int getValueFromAttribute(const AlfAttribute& aAttr,IAlfMap* aData) const;

    void HandleImagePathAttribute(
        const TPtrC8& aStringValue,
        CAlfImageVisual* aImageVisual,
        bool aPrimary );

private: // data
    auto_ptr<AlfImageVisualAttributeSetterImpl> mImpl;
    };

    } // namespace Alf

#endif // ALFIMAGEVISUALATTRIBUTESETTER_H

