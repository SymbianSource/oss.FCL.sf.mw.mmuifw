/*
* Copyright (c) 2007-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  xml parser header
*
*/


#ifndef MULTEMPLATEINTERFACE_H
#define MULTEMPLATEINTERFACE_H

#include <alf/ialfmap.h>
#include <osn/ustring.h>
#include <alf/alfimagevisual.h>
#include <alf/alflayout.h>

//#include "mulplaintexteditor.h"

namespace duiuimodel
    {
    class DuiNode;
    }
using namespace duiuimodel;

namespace Alf
    {
    
//Forward declarations
class MulXmlUtility;
class IAlfMap;
 	
class MulTemplateInterface
    {
        
public:

    /**
     * Display Area.
     *
     * @returns the display are of the control.
     */
    virtual TRect Displayarea() = 0;    
    
    /**
     * Data for the item.
     *
     * @aItemIndex index of the item.
     * @returns data map of the item at the index.
     */
    virtual IAlfMap* RootData(int aItemIndex) = 0;
    
    /**
     * Get the xml utility.
     *
     * @returns MulXmlUtility.
     */
    virtual MulXmlUtility& GetMulXmlUtlity() = 0;
    
    /**
     * Get the path where the xmls are stored.
     *
     * @returns the path.
     */
    virtual UString ResourcePath() = 0;        
    
    /**
    * Gets the template Attribute Name.
    */
    virtual UString TemplateAttribute() = 0; 
    
    /**
    * Gets the embedded widget load name.
    */
    virtual UString EmbedWidgetName() = 0;
    
    /**
     * Checks if the visual passed as argument belongs to the highlighted item
     * 
     * @param aVisual The visual to be checked if it belongs to the highlighted item.
     * @returns True: If aVisual corresponds to highlight item. otherwise, False.
     */
    virtual bool IsHighlightVisual(CAlfVisual& aVisual) = 0;
    
    /**
     * Return image scale mode
     * 
     * @return Scale mode
     */
    virtual CAlfImageVisual::TScaleMode ScaleMode() = 0;
    
    /**
     * Return images should be scaled or not
     * 
     * @return true if scaling should be applied, else false
     */
    virtual bool ApplyScaleMode() = 0;
    
    /**
     * Gets the plain text editor control
     * 
     * @return MulPlainTextEditor wraped in layout.
     */
    //virtual MulPlainTextEditor* PlainTextEditor() = 0;    
    
    /**
     * Gets the root node of the current template.
     * 
     * @return DuiNode pointer.
     */
    virtual DuiNode* TemplateRootNode() = 0;  

    virtual bool IsHomogeneous() = 0;
    
    
    /**
    * To Get the background is transparent or not.
    */
    virtual bool BackGroundTransparency() = 0;   
    
    
    }; //class
            
    } // namespace 

#endif

