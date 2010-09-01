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
* Description: Header for presentation element - Utilities for slider widget.
*
*/


#ifndef MUL_SLIDERUTILS_H
#define MUL_SLIDERUTILS_H


#include "imulsliderbaseelementinternal.h"
#include <AknUtils.h>
#include <alf/alfimagevisual.h>

namespace Alf
    {


/**
 *  Slider Utils implementation.
 *
 *  Implements utility methods to support the slider
 */
class MulSliderUtils
    {
public:
   
	/**
	 * Get the component layout rect value from lct.
	 *
	 * @param aId SliderLCTIDs, the LCTID of the component.
	 * @param aLayout CAlfLayout, the parent layout of the component.
	 * @param aVariety variety ID of the component.
	 */
	 static TAknLayoutRect GetComponentRect(SliderLCTIDs aId, CAlfLayout *aLayout, int aVariety);
	 
	 /**
	 * Get the text component layout rect value.
	 *
	 * @param aId SliderLCTIDs, the LCTID of the component.
	 * @param aLayout CAlfLayout, the parent layout of the component.
	 * @param aVariety variety ID of the component.
	 */
	 static TAknLayoutText GetTextComponentRect(SliderLCTIDs aId, CAlfLayout *aLayout, int aVariety);
	

	  }; // class MulSliderUtils

    } // End of namespace Alf

#endif // MUL_SLIDERUTILS_H
//End Of File
