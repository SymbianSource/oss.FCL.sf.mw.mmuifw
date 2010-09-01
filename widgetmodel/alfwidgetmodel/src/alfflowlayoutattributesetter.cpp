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
* Description:   File implements the atrributesetters for the flowlayout.
*
*/



//includes

//widget model includes
#include "alf/alfattributecontainer.h"
#include "alf/alfattribute.h"
#include "alf/alfflowlayoutattributesetter.h"
#include <alf/alfvisualexception.h>
#include <alf/alfdataexception.h>
#include <alf/alfattributeexception.h>

//osn includes
#include <osn/ustring.h>

//alf includes
#include <alf/alfvisual.h>
#include <alf/alfflowlayout.h>

//dui includes
#include "alf/attrproperty.h"

//other includes
#include <libc/string.h>



//namespaces
using namespace osncore;
using namespace duiuimodel::layoutattributes;

namespace Alf
    {

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Constructor.
// ---------------------------------------------------------------------------
//
OSN_EXPORT AlfFlowLayoutAttributeSetter::AlfFlowLayoutAttributeSetter()
    {
    }

// ---------------------------------------------------------------------------
// Destructor.
// ---------------------------------------------------------------------------
//
OSN_EXPORT AlfFlowLayoutAttributeSetter::~AlfFlowLayoutAttributeSetter()
    {
    }

// ---------------------------------------------------------------------------
// Sets Attribute Value. Delegates based on attribute Category.
// ---------------------------------------------------------------------------
//
OSN_EXPORT void AlfFlowLayoutAttributeSetter::setAttributeValue (
    CAlfVisual &aVisual,
    AlfAttributeContainer* aContainer,
    IAlfMap* aData )
    {
    // Ensure that the visual is a flow layout
    CAlfFlowLayout* flowLayout = dynamic_cast<CAlfFlowLayout*>(&aVisual);
    if (!flowLayout)
        {
        ALF_THROW ( AlfVisualException, EInvalidVisual, "AlfFlowLayoutAttributeSetter");
        }
    AlfCommonLayoutAttributeSetter::setAttributeValue(aVisual,
            aContainer, aData);
    }
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
OSN_EXPORT TAlfCommand* AlfFlowLayoutAttributeSetter::createCommand(
    CAlfVisual& /*aVisual*/, AlfAttributeContainer* /*aContainer*/,
    IAlfMap* /*aData*/, int /*aTransitionTime*/, CAlfVisual* /*aRefVisual*/)
    {
    TAlfCommand* cmd = 0;
    return cmd;
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
OSN_EXPORT void AlfFlowLayoutAttributeSetter::createAndSendCommands(
    CAlfVisual& aVisual,
    AlfAttributeContainer* aContainer,
    CAlfVisual* aRefVisual )
    {
    // Flow layout does not have any specific API which takes timed value.
    // So, call the base class implementation. It goes through its loop and
    // checks if there are attributes that it handles.
    AlfCommonLayoutAttributeSetter::createAndSendCommands(aVisual,aContainer,
                                        aRefVisual);
    }

void AlfFlowLayoutAttributeSetter::handleDynamicDataAttribute (
    CAlfVisual& aVisual, AlfAttribute& aAttr,
    AlfAttributeContainer& aContainer, IAlfMap* aData)
    {
    // Flow Layout does not have any specific dynamic attribute, so call the
    // base class implementation. It goes through its loop and checks if there
    // are attributes that it handles.
    AlfCommonLayoutAttributeSetter::handleDynamicDataAttribute(aVisual, aAttr,
                                        aContainer, aData);
    }

void AlfFlowLayoutAttributeSetter::handleStaticDataAttribute(
    CAlfVisual &aVisual, AlfAttribute& aAttr,
    AlfAttributeContainer& aContainer, IAlfMap* aData)
    {
    if (!aData)  ALF_THROW ( AlfDataException, EInvalidVariantDataType, "AlfFlowLayoutAttributeSetter" )

    const char* dataField = aAttr.getDataField();
    if ( !dataField )  ALF_THROW ( AlfDataException, EInvalidAttribute, "AlfFlowLayoutAttributeSetter" )

    IAlfVariantType* data = aData->item ( UString(dataField));
    if (data)
        {
        const char* attrName = aAttr.name();
        CAlfFlowLayout* flowLayout = dynamic_cast<CAlfFlowLayout*>(&aVisual);
        if (!flowLayout)
            {
            ALF_THROW ( AlfVisualException, EInvalidVisual, "AlfFlowLayoutAttributeSetter");
            }

        if (!strcmp(attrName,flow::KFlowDirection))
            {
            if (data->type() == IAlfVariantType::EInt)
                {
                int direction = data->integer();
                flowLayout->SetFlowDirection(
                                (CAlfFlowLayout::TFlowDirection)direction);
                }
            else
                {
                ALF_THROW ( AlfDataException, EInvalidVariantDataType, "AlfFlowLayoutAttributeSetter");
                }    
            }
        else if (!strcmp(attrName,flow::KLayoutMode))
            {
            if (data->type() == IAlfVariantType::EInt)
                {
                int layoutMode = data->integer();
                flowLayout->SetMode(layoutMode);
                }
            else
                {
                ALF_THROW ( AlfDataException, EInvalidVariantDataType, "AlfFlowLayoutAttributeSetter");
                }    
            }
        else if (!strcmp(attrName,flow::KLayoutCentering))
            {
            if (data->type() == IAlfVariantType::EBool)
                {
                TBool layoutCentering = data->boolean();
                flowLayout->SetCentering(layoutCentering);
                }
            else
                {
                ALF_THROW ( AlfDataException, EInvalidVariantDataType, "AlfFlowLayoutAttributeSetter");
                }    
            }
        else
            {
            // Call the base class implementation .
            // It goes through its loop and checks if there are attributes 
            // that it handles.
            AlfCommonLayoutAttributeSetter::handleStaticDataAttribute(aVisual,
                                                aAttr, aContainer, aData);
            }
        }
    return;    
    }

void AlfFlowLayoutAttributeSetter::handleDynamicAttribute(
    CAlfVisual &aVisual, AlfAttribute& aAttr, 
    AlfAttributeContainer& aContainer)
    {
    // Flow Layout does not have any specific dynamic attribute, so call the 
    // base class implementation also. It goes through its loop and checks if
    // there are attributes that it handles.
    AlfCommonLayoutAttributeSetter::handleDynamicAttribute(aVisual, aAttr,
                                        aContainer);
    }

void AlfFlowLayoutAttributeSetter::handleStaticAttribute(
    CAlfVisual& aVisual, AlfAttribute& aAttr,
    AlfAttributeContainer& aContainer)
    {
    CAlfFlowLayout* flowLayout = dynamic_cast<CAlfFlowLayout*>(&aVisual);
    if (!flowLayout)
        {
        ALF_THROW ( AlfVisualException, EInvalidVisual, "AlfFlowLayoutAttributeSetter");
        }   

    const char* attrName = aAttr.name();

    if (!strcmp(attrName, flow::KFlowDirection))
        {
        if(aAttr.type() == AlfAttributeValueType::EInt)
            {
            int direction = aAttr.intValue();
            flowLayout->SetFlowDirection(
                        (CAlfFlowLayout::TFlowDirection)direction);
            }
        else
            {
            ALF_THROW ( AlfAttributeException, EInvalidAttributeValue, "AlfFlowLayoutAttributeSetter");
            }    
        }
    else if (!strcmp(attrName, flow::KLayoutMode))
        {
        if (aAttr.type() == AlfAttributeValueType::EInt)
            {
            int layoutMode = aAttr.intValue();
            flowLayout->SetMode(layoutMode);
            }
        else
            {
            ALF_THROW ( AlfAttributeException, EInvalidAttributeValue, "AlfFlowLayoutAttributeSetter");
            }        
        }
    else if (!strcmp(attrName, flow::KLayoutCentering))
        {
        if (aAttr.type() == AlfAttributeValueType::EInt)
            {
            int layoutCentering = aAttr.intValue();
            TBool flag=ETrue;           //Assume true for any non-zero value.
            if (0==layoutCentering) flag=EFalse;
            flowLayout->SetCentering(flag);
            }
        else
            {
            ALF_THROW ( AlfAttributeException, EInvalidAttributeValue, "AlfFlowLayoutAttributeSetter");
            }        
        }
    else // Call the base class implementation also.
         // It goes through the same loop again and checks if there are
         // attributes that it handles.
        {
        AlfCommonLayoutAttributeSetter::handleStaticAttribute(aVisual, aAttr,
                                            aContainer);
        }
    return;
    }

    } // Alf


