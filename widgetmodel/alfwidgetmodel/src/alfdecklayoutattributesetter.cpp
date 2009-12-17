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
* Description:   Implements attributesetters for DeckLayout.
*
*/



//includes

//widget model includes
#include "alf/alfattributecontainer.h"
#include "alf/alfattributevaluetype.h"
#include "alf/alfattribute.h"
#include "alf/alfdecklayoutattributesetter.h"
#include <alf/alfattributeexception.h>
#include <alf/alfvisualexception.h>
#include <alf/alfdataexception.h>

//osn includes
#include <osn/ustring.h>

//alf includes
#include <alf/alfenv.h>
#include <alf/alfvisual.h>
#include <alf/alfdecklayout.h>

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
OSN_EXPORT AlfDeckLayoutAttributeSetter::AlfDeckLayoutAttributeSetter()
    {
    }

// ---------------------------------------------------------------------------
// Destructor.
// ---------------------------------------------------------------------------
//
OSN_EXPORT AlfDeckLayoutAttributeSetter::~AlfDeckLayoutAttributeSetter()
    {
    }


// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
OSN_EXPORT void AlfDeckLayoutAttributeSetter::setAttributeValue(
    CAlfVisual &aVisual,
    AlfAttributeContainer* aContainer,
    IAlfMap* aData)
    {
    CAlfDeckLayout* deckLayout = dynamic_cast<CAlfDeckLayout*>(&aVisual);
    if (!deckLayout)
        {
        ALF_THROW ( AlfVisualException, EInvalidVisual, "AlfDeckLayoutAttributeSetter")
        }

    // Deck layout does not have any attribute of its own
    // So, call the base class implementation.
    AlfCommonLayoutAttributeSetter::setAttributeValue(aVisual, aContainer, aData);
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
OSN_EXPORT TAlfCommand* AlfDeckLayoutAttributeSetter::createCommand(
    CAlfVisual& /*aVisual*/, AlfAttributeContainer* /*aContainer*/,
    IAlfMap* /*aData*/, int /*aTransitionTime*/, CAlfVisual* /*aRefVisual*/)
    {
    TAlfCommand* cmd = 0;
    return cmd;
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
OSN_EXPORT void AlfDeckLayoutAttributeSetter::createAndSendCommands(
    CAlfVisual& aVisual,
    AlfAttributeContainer* aContainer,
    CAlfVisual* aRefVisual )
    {
    // Deck layout has no api specific to it which takes a timedvalue
    // So, call the base class implementation.
    // It goes through its loop and checks if there are attributes that it handles.
    AlfCommonLayoutAttributeSetter::createAndSendCommands(aVisual,aContainer,aRefVisual);
    }

// ---------------------------------------------------------------------------
// Sets a value of a static attribute to target visual immediately
// without a transition.
// ---------------------------------------------------------------------------
//
void AlfDeckLayoutAttributeSetter::handleStaticAttribute(
    CAlfVisual& aVisual, AlfAttribute& aAttr, AlfAttributeContainer& aContainer)
    {
    AlfCommonLayoutAttributeSetter::handleStaticAttribute(aVisual,
            aAttr, aContainer);
    }

// ---------------------------------------------------------------------------
// Sets a dynamic attribute value in the target visual
// using transitions defined in attributes
// ---------------------------------------------------------------------------
//
void AlfDeckLayoutAttributeSetter::handleDynamicAttribute(
    CAlfVisual& aVisual, AlfAttribute& aAttr, AlfAttributeContainer& aContainer)
    {
    AlfCommonLayoutAttributeSetter::handleDynamicAttribute(aVisual,
            aAttr, aContainer);
    }

// ---------------------------------------------------------------------------
// Sets a value for static data attribute in the target visual
// immediately without a transition using data in map.
// ---------------------------------------------------------------------------
//
void AlfDeckLayoutAttributeSetter::handleStaticDataAttribute(
    CAlfVisual& aVisual, AlfAttribute& aAttr,
    AlfAttributeContainer& aContainer, IAlfMap* aData)
    {
    AlfCommonLayoutAttributeSetter::handleStaticDataAttribute(
        aVisual, aAttr, aContainer, aData);
    }

// ---------------------------------------------------------------------------
// Sets a value for dynamic data attribute value in the target visual
// using transitions and data in map.
// ---------------------------------------------------------------------------
//
void AlfDeckLayoutAttributeSetter::handleDynamicDataAttribute(
    CAlfVisual& aVisual, AlfAttribute& aAttr,
    AlfAttributeContainer& aContainer, IAlfMap* aData)
    {
    AlfCommonLayoutAttributeSetter::handleDynamicDataAttribute(aVisual,
            aAttr, aContainer, aData);
    }

    }// Alf
