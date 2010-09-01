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
* Description:   This class implements common attribute setters.
*
*/


#include <libc/string.h>
#include <utf.h>

#include <osn/ustring.h>
#include <stdexcept>
#include "alf/attrproperty.h"

#include "alf/alfattribute.h"
#include "alf/alfcommonattributesetter.h"
#include <alf/alfvisual.h>
#include <alf/alfattributeexception.h>
#include "alf/alfattributecontainer.h"

using namespace duiuimodel::commonattributes;

namespace Alf
    {

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Constructor.
// ---------------------------------------------------------------------------
//
OSN_EXPORT AlfCommonAttributeSetter::AlfCommonAttributeSetter()
    {
    mDoDirtycheck=false;
    }

// ---------------------------------------------------------------------------
// Destructor.
// ---------------------------------------------------------------------------
//
OSN_EXPORT AlfCommonAttributeSetter::~AlfCommonAttributeSetter()
    {

    }

// ---------------------------------------------------------------------------
// Returns the type of the attribute setter.
// ---------------------------------------------------------------------------
//
OSN_EXPORT SetterType AlfCommonAttributeSetter::setterType()
    {
    return ECustomAttributeSetter;
    }

// ---------------------------------------------------------------------------
// Sets an attribute value in the target visual immediately
// without a transition.
// ---------------------------------------------------------------------------
//
OSN_EXPORT void AlfCommonAttributeSetter::setAttributeValue(
    CAlfVisual &aVisual,
    AlfAttributeContainer* aContainer,
    IAlfMap* aData)
    {
    if (!aContainer)
        {
        ALF_THROW ( AlfAttributeException, EInvalidAttribute,
                    "AlfCommonAttributeSetter" )
        }

    unsigned int attrCount = aContainer->attributeCount();
    for (int i=0; i<attrCount; ++i)
        {
        AlfAttribute& attr = aContainer->getAttribute(i);
        const char* attrName = attr.name();

        if (attr.category() == AlfAttribute::EStaticData)
            {
            handleStaticDataAttribute(aVisual, attr, *aContainer, aData);
            }
        else if (attr.category() == AlfAttribute::EDynamicData)
            {
            handleDynamicDataAttribute(aVisual, attr, *aContainer, aData);
            }
        else if (attr.category() == AlfAttribute::EStatic)
            {
            if ((mDoDirtycheck&&attr.isDirty())||!mDoDirtycheck)
                {
                handleStaticAttribute(aVisual, attr, *aContainer);
                }
            attr.setDirty(false);

            }
        else if (attr.category() == AlfAttribute::EDynamic)
            {
            if ((mDoDirtycheck&&attr.isDirty())||!mDoDirtycheck)
                {
                handleDynamicAttribute(aVisual, attr, *aContainer);
                }
            attr.setDirty(false);

            }
        }
    }

// ---------------------------------------------------------------------------
// Creates a command to change the value of an attribute in the target visual.
// ---------------------------------------------------------------------------
//
OSN_EXPORT TAlfCommand* AlfCommonAttributeSetter::createCommand(
    CAlfVisual& /*aVisual*/, AlfAttributeContainer* /*aContainer*/,
    IAlfMap* /*aData*/, int /*aTransitionTime*/, CAlfVisual* /*aRefVisual*/)
    {
    // No need to implement.
    TAlfCommand* cmd = 0;
    return cmd;
    }

// ---------------------------------------------------------------------------
// Creates commands to change the given attribute values in the target visual.
// ---------------------------------------------------------------------------
//
OSN_EXPORT void AlfCommonAttributeSetter::createAndSendCommands(
    CAlfVisual& /*aVisual*/, AlfAttributeContainer* /*aContainer*/,
    CAlfVisual* /*aRefVisual*/)
    {
    // No need to implement.
    }

// ---------------------------------------------------------------------------
// Getter for interfaces provided by the element classes.
// ---------------------------------------------------------------------------
//
OSN_EXPORT IAlfInterfaceBase* AlfCommonAttributeSetter::makeInterface(
    const IfId& aType)
    {
    UString param(aType.mImplementationId);
    if (param == IAlfAttributeSetter::type().mImplementationId)
        {
        return this;
        }
    return NULL;
    }

// ---------------------------------------------------------------------------
// Converts text between Unicode (UCS-2) and the Unicode transformation
// format UTF-8.
// ---------------------------------------------------------------------------
//
HBufC* AlfCommonAttributeSetter::convertToUnicodeFromUtf8(
    const TPtrC8& aSrc) const
    {
    HBufC* desValue = 0;
    TRAPD( err, desValue = CnvUtfConverter::ConvertToUnicodeFromUtf8L(aSrc))
    if (err != KErrNone)
        {
        ALF_THROW(AlfAttributeException, err,"AlfCommonAttributeSetter")
        }
    return desValue;
    }

// ---------------------------------------------------------------------------
// Sets a value of a static attribute to target visual immediately
// without a transition.
// ---------------------------------------------------------------------------
//
void AlfCommonAttributeSetter::handleStaticAttribute(
    CAlfVisual& aVisual, AlfAttribute& aAttr,
    AlfAttributeContainer& /*aContainer*/)
    {
    const char* attrName = aAttr.name();
    if (!strcmp(attrName, KId))
        {
        const char* val = aAttr.stringValue().getUtf8();
        TPtrC8 src((TUint8*)val);
        TRAPD(err, aVisual.SetTagL( src ));
        if (err)
            {
            ALF_THROW(AlfAttributeException, err,
                      "AlfCommonAttributeSetter")
            }
        }
    }
// ---------------------------------------------------------------------------
// Sets a dynamic attribute value in the target visual using transitions
// defined in attributes.
// ---------------------------------------------------------------------------
//
void AlfCommonAttributeSetter::handleDynamicAttribute(
    CAlfVisual& /*aVisual*/, AlfAttribute& /*aAttr*/,
    AlfAttributeContainer& /*aContainer*/)
    {
    //nothing to do
    }

// ---------------------------------------------------------------------------
// Sets a value for static data attribute in the target visual
// immediately without a transition using data in map.
// ---------------------------------------------------------------------------
//
void AlfCommonAttributeSetter::handleStaticDataAttribute(
    CAlfVisual& /*aVisual*/, AlfAttribute& /*aAttr*/,
    AlfAttributeContainer& /*aContainer*/, IAlfMap* /*aData*/)
    {
    //nothing to do
    }

// ---------------------------------------------------------------------------
// Sets a value for dynamic data attribute value in the target visual
// using transitions and data in map.
// ---------------------------------------------------------------------------
//
void AlfCommonAttributeSetter::handleDynamicDataAttribute(
    CAlfVisual& /*aVisual*/, AlfAttribute& /*aAttr*/,
    AlfAttributeContainer& /*aContainer*/, IAlfMap* /*aData*/)
    {
    //nothing to do
    }

OSN_EXPORT void AlfCommonAttributeSetter::enableDirtyCheck(bool aCheck)
    {
    mDoDirtycheck=aCheck;
    }
    } // Alf
