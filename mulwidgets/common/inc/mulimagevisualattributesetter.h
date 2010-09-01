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
* Description:  Custmized image attribute setter for MulWidgets
 *
*/


#ifndef MULIMAGEVISUALATTRIBUTESETTER_H
#define MULIMAGEVISUALATTRIBUTESETTER_H

#include <osn/osndefines.h>
#include "alf/alfimagevisualattributesetter.h"

namespace Alf
	{
	
class MulImageVisualAttributeSetterImpl;
/**
 *  The implementation of the attribute setter.
 *
 *  @lib alfwidgetmodel.lib
 *  @since S60 ?S60_version
 */
class MulImageVisualAttributeSetter : public AlfImageVisualAttributeSetter
	{
public:

	/**
	 * Constructor.
	 *
	 * @param aType Type of the attribute setter.
	 */
    OSN_IMPORT MulImageVisualAttributeSetter();

	/**
	 * Destructor.
	 */
	  OSN_IMPORT virtual ~MulImageVisualAttributeSetter();

private: //  from AlfImageVisualAttributeSetter

	void handleStaticDataAttribute(CAlfVisual &aVisual,
								  AlfAttribute& aAttr, 
								  AlfAttributeContainer& aContainer,
								  IAlfMap* aData);
			
	void handleStaticAttribute(CAlfVisual &aVisual, 
							   AlfAttribute& aAttr,
							   AlfAttributeContainer& aContainer);
	
	void setAttributeValue ( CAlfVisual &aVisual,
							 AlfAttributeContainer* aContainer,
							 IAlfMap* aData );
	
	void handleDynamicDataAttribute(CAlfVisual &aVisual,
									AlfAttribute& aAttr, 
									AlfAttributeContainer& aContainer,
									IAlfMap* aData);
	
	void handleDynamicAttribute(CAlfVisual &aVisual,
								AlfAttribute& aAttr, 
								AlfAttributeContainer& aContainer);
	
public: //new method	

   /**
    * Checks the given file path is of SVG image or not
    *
    * @param aImagePath Path of the image file,which need to be checked for SVG file format. 
    */
    OSN_IMPORT bool IsSVGImage(const UString& aImagePath);
        
    /**
     * Load Image visual with image provided from file path
     * 
     * @param aSvgFilePath path of Image
     * @param aImageVisual visual on with image to be loaded
     */
    OSN_IMPORT void LoadImageFromSvg(const UString& aSvgFilePath,CAlfImageVisual& aImageVisual );
    
    /**
     * Apply texture on Image visual
     * 
     * @param aTextureId Texture id 
     * @param aImageVisual visual on with image to be loaded
     */
    OSN_IMPORT void LoadImageFromTexture( int aTextureId,CAlfImageVisual& aImageVisual );
    
private: 	// data

	auto_ptr<MulImageVisualAttributeSetterImpl> mImpl;	
    auto_ptr<AlfImageVisualAttributeSetter> mImageAttrSetter;
    IAlfMap* mData;
	};

} // namespace Alf

#endif // MULIMAGEVISUALATTRIBUTESETTER_H

//end of file


