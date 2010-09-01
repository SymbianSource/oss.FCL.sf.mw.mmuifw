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
* Description:   Text visual attribute setter Header.
*
*/


#ifndef ALFTEXTVISUALATTRIBUTESETTER_H
#define ALFTEXTVISUALATTRIBUTESETTER_H

//includes

//widget model includes
#include "alf/alfcommonvisualattributesetter.h"

//osn includes
#include <osn/osndefines.h>

namespace Alf
    {

/**
 *  @class AlfTextVisualAttributeSetter alftextvisualattributesetter.h "alf/alftextvisualattributesetter.h"
 *  The implementation of the attribute setter for setting text visual attributes.
 *  @see IAlfAttributeSetter
 *
 *  @lib alfwidgetmodel.lib
 *  @since S60 ?S60_version
 *  @status Draft
 *  @interfaces IAlfAttributeSetter
 */
class AlfTextVisualAttributeSetter : public AlfCommonVisualAttributeSetter
    {

public:

    /**
     * Constructor.
     * @exception osncore::AlfVisualException Thrown with error code osncore::EInvalidVisual if aVisual is not 
     *                                        a text visual.
     * @exception osncore::AlfDataException   Thrown with error code osncore::EInvalidAttribute if the data field  
     *                                        for data attributes is 0.
     * @exception osncore::AlfDataException   Thrown with error code osncore::EInvalidVariantDataType if aData  
     *                                        for data attributes is 0.
     * @exception osncore::AlfDataException   Thrown with error code osncore::ECommonError if setting of text color
     *                                        from skin fails.
     * @exception osncore::AlfAttributeException   Thrown with error code osncore::EInvalidAttribute if there are 
     *                                        invalid number or type of attributes while attempting to set text color.
     *                                        See language specification on how to create these attributes.
     * @exception osncore::AlfDataException   Thrown with error code osncore::ECommonError if setting text to the  
     *                                        text visual fails.
     * @exception osncore::AlfDataException   Thrown with error code osncore::EInvalidVariantDataType if aData  
     *                                        for data attributes is 0.
     *
     * @param aType Type of the attribute setter.
     */
    OSN_IMPORT AlfTextVisualAttributeSetter();

    /**
     * Destructor.
     */
    OSN_IMPORT virtual ~AlfTextVisualAttributeSetter();

    /**
     * Sets the attribute values to the target visual.
     *
     * @since S60 ?S60_version
     * @param aVisual The target visual. Doesn't take ownership
     * @param aContainer The container holding the value of the attributes.
     *        Ownership of the object is not transferred.
     * @param aData Used with data attributes to fetch the data.
     */
    OSN_IMPORT virtual void setAttributeValue (
        CAlfVisual &aVisual,
        AlfAttributeContainer* aContainer,
        IAlfMap* aData );

    /**
     * Depracated! Do not use!
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
    OSN_IMPORT virtual TAlfCommand* createCommand (
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
    OSN_IMPORT virtual void createAndSendCommands (
        CAlfVisual& aVisual,
        AlfAttributeContainer* aContainer,
        CAlfVisual* aRefVisual = NULL );
protected:

    /**
     * From CAlfCommonVisualAttributeSetter
     * Sets an value of a static attribute to target visual immediately without a transition.
     *
     * @param aVisual The target visual. Doesn't take ownership
     * @param aAttr The attribute to process. Doesn't take ownership
     * @param aContainer The container holding attributes. Ownership of the object is not transferred.
     * @param aData Contains data.Ownership of the object is not transferred.
     */
    virtual void handleStaticAttribute ( CAlfVisual &aVisual, AlfAttribute& aAttr, AlfAttributeContainer& aContainer);

    /**
     * From CAlfCommonVisualAttributeSetter
     * Sets an dynamic attribute value in the target visual using transitions defined in attributes
     *
     * @param aVisual The target visual. Doesn't take ownership
     * @param aAttr The attribute to process. Doesn't take ownership
     * @param aContainer The container holding attributes. Ownership of the object is not transferred.
     */
    virtual void handleDynamicAttribute ( CAlfVisual &aVisual, AlfAttribute& aAttr, AlfAttributeContainer& aContainer);

    /**
     * From CAlfCommonVisualAttributeSetter
     * Sets an value for static attribute in the target visual immediately without a transition using data in map.
     *
     * @param aVisual The target visual. Doesn't take ownership
     * @param aAttr The attribute to process. Doesn't take ownership
     * @param aContainer The container holding attributes. Ownership of the object is not transferred.
     * @param aData Contains data.Ownership of the object is not transferred.
     */
    virtual void handleStaticDataAttribute ( CAlfVisual &aVisual, AlfAttribute& aAttr, AlfAttributeContainer& aContainer, IAlfMap* aData );

    /**
     * From CAlfCommonVisualAttributeSetter
     * Sets an value for dynamic attribute value in the target visual using transitions and data in map.
     *
     * @param aVisual The target visual. Doesn't take ownership
     * @param aAttr The attribute to process. Doesn't take ownership
     * @param aContainer The container holding attributes. Ownership of the object is not transferred.
     * @param aData Contains data.Ownership of the object is not transferred.
     */
    virtual void handleDynamicDataAttribute ( CAlfVisual &aVisual, AlfAttribute& aAttr, AlfAttributeContainer& aContainer, IAlfMap* aData );

private: // data
	
	/**
	 * Localisation attribute setter helper method. Helper Method.
	 *
	 * @aLocId localisationid defined in .loc files.
	 * @aLang  Required language for the text.
	 *
	 */
	 
	void setLocalisedText(CAlfVisual& aVisual,const char* aLocId,AlfAttribute* aLangAttr,AlfAttribute* aVarArgsAttr);

	/**
	 * Helper method to set Text Visual color.
	 * 
	 * @aTextVisual TextVisual to which Color has to be set.
	 * @aColorTable Logicalname for colortable.
	 * @aColorIndex Color Index for the colortable.
	 * 
	 * @exception osncore::ThemeException   Thrown when skinID is not found.
	 *
	 */
	void setTextSkinColor(CAlfTextVisual& aTextVisual, const Utf8* aColorTable, 
													   const char* aColorIndex=NULL);

    };

    } // namespace Alf

#endif // ALFIMAGEVISUALATTRIBUTESETTER_H
// End of file


