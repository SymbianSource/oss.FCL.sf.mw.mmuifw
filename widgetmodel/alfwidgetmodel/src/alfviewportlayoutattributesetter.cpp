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
* Description:   Implements attributesetters for ViewPortLayout.
*
*/



//includes

//widget model includes
#include "alf/alfattributecontainer.h"
#include "alf/alfattribute.h"
#include "alf/alfviewportlayoutattributesetter.h"
#include <alf/alfvisualexception.h>
#include <alf/alfdataexception.h>
#include <alf/alfattributeexception.h>

//osn includes
#include <osn/ustring.h>

//alf includes
#include <alf/alfvisual.h>
#include <alf/alfviewportlayout.h>

//dui includes
#include "alf/attrproperty.h"

//other includes
#include <libc/string.h>



//namespaces
using namespace osncore;
using namespace duiuimodel::layoutattributes::viewport;

namespace Alf
    {

// ======== MEMBER FUNCTIONS ========


// ---------------------------------------------------------------------------
// Constructor.
// ---------------------------------------------------------------------------
//
OSN_EXPORT AlfViewportLayoutAttributeSetter::AlfViewportLayoutAttributeSetter()
    {
    }

// ---------------------------------------------------------------------------
// Destructor.
// ---------------------------------------------------------------------------
//
OSN_EXPORT AlfViewportLayoutAttributeSetter::
    ~AlfViewportLayoutAttributeSetter()
    {
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
IAlfVariantType* AlfViewportLayoutAttributeSetter::getData(
    AlfAttribute& aAttr, IAlfMap* aData)
    {
    IAlfVariantType* data = 0;
    const char* dataField = aAttr.getDataField();
    if (dataField)
        {
        data = aData->item(UString(dataField));
        }
    return data;
    }


// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
int AlfViewportLayoutAttributeSetter::getTime(
    const AlfAttribute& aAttr, bool aImmediate)
    {
    int time = 0;
    if (!aImmediate)
        {
        time = aAttr.getTime();
        }
    return time;
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
OSN_EXPORT void AlfViewportLayoutAttributeSetter::setAttributeValue(
    CAlfVisual &aVisual, AlfAttributeContainer* aContainer, IAlfMap* aData)
    {
    CAlfViewportLayout* viewportLayout = 
        dynamic_cast<CAlfViewportLayout*>(&aVisual);
    if (!viewportLayout)
        {
        ALF_THROW (AlfVisualException, EInvalidVisual, "AlfViewportLayoutAttributeSetter")
        }     
    AlfCommonLayoutAttributeSetter::setAttributeValue(aVisual,
                                        aContainer, aData);
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
OSN_EXPORT TAlfCommand* AlfViewportLayoutAttributeSetter::createCommand(
    CAlfVisual& /*aVisual*/, AlfAttributeContainer* /*aContainer*/,
    IAlfMap* /*aData*/, int /*aTransitionTime*/, CAlfVisual* /*aRefVisual*/)
    {
    TAlfCommand* cmd = 0;
    return cmd;
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
OSN_EXPORT void AlfViewportLayoutAttributeSetter::createAndSendCommands(
    CAlfVisual& aVisual,
    AlfAttributeContainer* aContainer,
    CAlfVisual* aRefVisual )
    {
    // Call the base class implementation.
    // It goes through its loop and checks if there are attributes that it
    // handles.
    AlfCommonLayoutAttributeSetter::createAndSendCommands(aVisual,
                                        aContainer,aRefVisual);
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void AlfViewportLayoutAttributeSetter::handleDynamicDataAttribute(
    CAlfVisual& aVisual, AlfAttribute& aAttr,
    AlfAttributeContainer& aContainer, IAlfMap* aData)
    {
    if (!doHandleDynamicDataAttribute(aVisual, aAttr, aContainer, aData))
        {
        AlfCommonLayoutAttributeSetter::handleDynamicDataAttribute(aVisual,
                                            aAttr, aContainer, aData);
        }
    }
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
bool AlfViewportLayoutAttributeSetter::doHandleDynamicDataAttribute(
    CAlfVisual& aVisual, AlfAttribute& aAttr,
    AlfAttributeContainer& aContainer, IAlfMap* aData, bool aImmediate)
    {
    CAlfViewportLayout* viewportLayout = 
        dynamic_cast<CAlfViewportLayout*>(&aVisual);
    if (!viewportLayout)
        {
        ALF_THROW (AlfVisualException, EInvalidVisual, "AlfViewportLayoutAttributeSetter")
        }
    bool handled = true;
    if (!aData)
        {
        ALF_THROW (AlfDataException, EInvalidVariantDataType, "AlfViewportLayoutAttributeSetter" )
        }
    else
        {
        const char* attrName = aAttr.name();
        if (!strcmp( attrName, KVirtualHeight)) //also handles: KVirtualWidth
            {
            // item for both virtual height and virtual width should be there.
            const IAlfVariantType* virtualHeight = getData(aAttr, aData);
            const IAlfVariantType* virtualWidth = 
                getData(aContainer.getAttributeByName(KVirtualWidth), aData);

            if (virtualHeight && 
                virtualHeight->type() == IAlfVariantType::EReal &&
                virtualWidth && virtualWidth->type() == IAlfVariantType::EReal)
                {
                viewportLayout->SetVirtualSize(
                    TAlfRealSize(virtualWidth->real(),virtualHeight->real()),
                    getTime(aAttr, aImmediate));
                }
            else
                {
                ALF_THROW(AlfDataException, EInvalidVariantDataType, "AlfViewportLayoutAttributeSetter")
                }

            }
        else if (!strcmp( attrName, KVirtualWidth))
            {
            // both virtual height and virtual width should be there
            // if attribute for virtual height exists,don't do anything as this
            // will be taken care while looking for attribute for virtual 
            // height, else throw exception
            try
                {
                AlfAttribute& attrWidth = 
                    aContainer.getAttributeByName (KVirtualHeight );
                }

            catch (...)
                {
                ALF_THROW(AlfDataException, EIncompleteAttributeValues, "AlfViewportLayoutAttributeSetter")
                }
            }
        //also handles: KViewportWidth    
        else if (!strcmp( attrName, KViewportHeight)) 
            {
            // item for both viewport window height and viewport window width
            // should be there.
            const IAlfVariantType* viewportHeight = getData(aAttr, aData);
            const IAlfVariantType* viewportWidth = 
                getData(aContainer.getAttributeByName(KViewportWidth), aData);

            if (viewportHeight &&
                viewportHeight->type() == IAlfVariantType::EReal &&
                viewportWidth && 
                viewportWidth->type() == IAlfVariantType::EReal)
                {
                viewportLayout->SetViewportSize(
                    TAlfRealSize(viewportWidth->real(),viewportHeight->real()),
                    getTime(aAttr, aImmediate));
                }
            else
                {
                ALF_THROW(AlfDataException, EInvalidVariantDataType, "AlfViewportLayoutAttributeSetter")
                }    
            }
        else if (!strcmp( attrName, KViewportWidth))
            {
            // both viewport height and viewport width should be there
            // if attribute for viewport height exists,don't do anything as
            // this will be taken care while looking for attribute for 
            // virtual height, else throw exception
            try
                {
                AlfAttribute& attrWidth = 
                    aContainer.getAttributeByName (KViewportHeight );
                }

            catch (...)
                {
                ALF_THROW(AlfDataException, EIncompleteAttributeValues, "AlfViewportLayoutAttributeSetter")
                }
            }
        //also handles: KViewportPositionY    
        else if (!strcmp( attrName, KViewportPositionX)) 
            {
            // item for both viewport window X-position and Y-position 
            // should be there.
            const IAlfVariantType* viewportPosX = getData(aAttr, aData);
            const IAlfVariantType* viewportPosY = 
                   getData(aContainer.getAttributeByName(KViewportPositionY),
                           aData);

            if (viewportPosX &&
                viewportPosX->type() == IAlfVariantType::EReal &&
                viewportPosY && viewportPosY->type() == IAlfVariantType::EReal)
                {
                viewportLayout->SetViewportPos(
                    TAlfRealPoint(viewportPosX->real(),viewportPosY->real()),
                    getTime(aAttr, aImmediate));
                }
            else
                {
                ALF_THROW(AlfDataException, EInvalidVariantDataType, "AlfViewportLayoutAttributeSetter")
                }    
            }
        else if (!strcmp( attrName, KViewportPositionY))
            {
            // both viewport window X-position and Y-position should be there
            // if attribute for viewport window X-position exists,don't do 
            // anything as this will be taken care while looking for attribute
            // for viewport window X-position, else throw exception
            try
                {
                AlfAttribute& attrWidth = 
                    aContainer.getAttributeByName (KViewportPositionX );
                }

            catch (...)
                {
                ALF_THROW(AlfDataException, EIncompleteAttributeValues, "AlfViewportLayoutAttributeSetter")
                }
            }
        else
            {
            handled = false;
            }
        }
    return handled;
    }

void AlfViewportLayoutAttributeSetter::handleStaticDataAttribute(
    CAlfVisual &aVisual, AlfAttribute& aAttr, 
    AlfAttributeContainer& aContainer, IAlfMap* aData)
    {
    // viewport layout does not have any static attribute of its own
    // check, if dynamic attribute(but set by user as static), if so,
    // set attributes immediately.
    if (!doHandleDynamicDataAttribute(aVisual, aAttr,aContainer, aData, true))
        {
        AlfCommonLayoutAttributeSetter::handleStaticDataAttribute(
            aVisual, aAttr, aContainer, aData);
        }
    }

void AlfViewportLayoutAttributeSetter::handleDynamicAttribute(
    CAlfVisual &aVisual, AlfAttribute& aAttr, 
    AlfAttributeContainer& aContainer)
    {
    if (!doHandleDynamicAttribute(aVisual, aAttr, aContainer))
        {
        AlfCommonLayoutAttributeSetter::handleDynamicAttribute(
            aVisual, aAttr, aContainer);
        }
    }

bool AlfViewportLayoutAttributeSetter::doHandleDynamicAttribute(
    CAlfVisual &aVisual, AlfAttribute& aAttr,
    AlfAttributeContainer& aContainer, bool aImmediate)
    {
    CAlfViewportLayout* viewportLayout = 
        dynamic_cast<CAlfViewportLayout*>(&aVisual);
    if (!viewportLayout)
        {
        ALF_THROW (AlfVisualException, EInvalidVisual, "AlfViewportLayoutAttributeSetter")
        }

    bool handled = true;
    const char* attrName = aAttr.name();
    if (!strcmp(attrName, KVirtualHeight)||!strcmp(attrName, KVirtualWidth))
        {
        // both virtual height and virtual width should be there.
        AlfAttribute& attrWidth = 
            aContainer.getAttributeByName (KVirtualWidth );
        AlfAttribute& attrHt = aContainer.getAttributeByName (KVirtualHeight );
        if(attrWidth.type() == AlfAttributeValueType::EFloat &&
               attrHt.type() == AlfAttributeValueType::EFloat )
            {
            float height = attrHt.realValue();
            float width = attrWidth.realValue();
            viewportLayout->SetVirtualSize(
                TAlfRealSize(width,height),getTime(attrHt, aImmediate));
            attrHt.setDirty(false);
            attrWidth.setDirty(false);
            }
        else
            {
            ALF_THROW ( AlfAttributeException, EInvalidAttributeValue, "AlfViewportLayoutAttributeSetter");
            }
        }
    else if (!strcmp(attrName, KViewportHeight)||
             !strcmp(attrName, KViewportWidth))
        {
        // both viewport window height and width should be there.
        AlfAttribute& attrWidth = 
            aContainer.getAttributeByName (KViewportWidth );
        AlfAttribute& attrHt = 
            aContainer.getAttributeByName (KViewportHeight );
        if(attrWidth.type() == AlfAttributeValueType::EFloat &&
               attrHt.type() == AlfAttributeValueType::EFloat )
            {
            float height = attrHt.realValue();
            float width = attrWidth.realValue();
            viewportLayout->SetViewportSize(
                TAlfRealSize(width,height),getTime(attrHt, aImmediate));
            attrHt.setDirty(false);
            attrWidth.setDirty(false);
            }
        else
            {
            ALF_THROW ( AlfAttributeException, EInvalidAttributeValue, "AlfViewportLayoutAttributeSetter");
            }
        }
    else if (!strcmp(attrName, KViewportPositionX)||
             !strcmp(attrName, KViewportPositionY))
        {
        // both viewport window X-position and Y-position should be there.
        AlfAttribute& attrWidth = 
            aContainer.getAttributeByName (KViewportPositionY );
        AlfAttribute& attrHt = 
            aContainer.getAttributeByName (KViewportPositionX );
        if(attrWidth.type() == AlfAttributeValueType::EFloat &&
               attrHt.type() == AlfAttributeValueType::EFloat )
            {
            float xPosition = attrHt.realValue();
            float yPosition = attrWidth.realValue();
            viewportLayout->SetViewportPos(
               TAlfRealPoint(xPosition,yPosition),getTime(attrHt, aImmediate));
            attrHt.setDirty(false);
            attrWidth.setDirty(false);
            }
        else
            {
            ALF_THROW ( AlfAttributeException, EInvalidAttributeValue, "AlfViewportLayoutAttributeSetter");
            }
        }
    else
        {
        handled = false;
        }

    return handled;
    }

void AlfViewportLayoutAttributeSetter::handleStaticAttribute(
    CAlfVisual& aVisual, AlfAttribute& aAttr,
    AlfAttributeContainer& aContainer)
    {
    // viewport layout does not have any static attribute of its own
    // check, if dynamic attribute(but set as static by the user),
    // if so, set attributes immediately.
    if (!doHandleDynamicAttribute(aVisual, aAttr, aContainer, true))
        {
        AlfCommonLayoutAttributeSetter::handleStaticAttribute(aVisual, aAttr, 
                                            aContainer);
        }
    }
    
    } // Alf

