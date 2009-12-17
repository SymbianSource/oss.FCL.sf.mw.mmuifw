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
* Description:  attributesetter for lct anchor layout.
*
*/


//includes

//widget model includes
#include "alf/alfattributecontainer.h"
#include "alf/alfattributevaluetype.h"
#include "alf/alfattribute.h"
#include "alf/alflctanchorlayoutattributesetter.h"
#include <alf/alfvisualexception.h>
#include <alf/alfdataexception.h>

//osn includes
#include <osn/ustring.h>

//alf includes
#include <alf/alfenv.h>
#include <alf/alfdisplay.h>
#include <alf/alflctanchorlayout.h>

//dui includes
#include "alf/attrproperty.h"
#include <alf/alftimedvalue.h>
//other includes
#include <libc/string.h>
#ifdef RD_ALF_IN_PLATFORM
#include <aknlayout2hierarchy.h>
#endif

#include "AlfLctAttributeSetterUtils.h"

using osncore::UString;

using namespace duiuimodel::layoutattributes::grid;

using namespace duiuimodel::lctvisualattributes;

using namespace duiuimodel::anchorlayoutinput;

namespace Alf
    {

//variable inside ifdef to prevent compiler warning.
#ifdef ALF_DEBUG_EXCEPTIONS
static const char* const ClassName = "AlfLCTAnchorLayoutAttributeSetter";
#endif

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Constructor.
// ---------------------------------------------------------------------------
//
OSN_EXPORT AlfLCTAnchorLayoutAttributeSetter::AlfLCTAnchorLayoutAttributeSetter()
    {
    }

// ---------------------------------------------------------------------------
// Destructor.
// ---------------------------------------------------------------------------
//
OSN_EXPORT AlfLCTAnchorLayoutAttributeSetter::~AlfLCTAnchorLayoutAttributeSetter()
    {
    }

// ---------------------------------------------------------------------------
// Sets Attribute Value. Delegates based on attribute Category.
// ---------------------------------------------------------------------------
//
OSN_EXPORT void AlfLCTAnchorLayoutAttributeSetter::setAttributeValue(
    CAlfVisual &aVisual,
    AlfAttributeContainer* aContainer,
    IAlfMap* aData )
    {
    CAlfLCTAnchorLayout* anchorlayout =
        dynamic_cast<CAlfLCTAnchorLayout*>(&aVisual);

    if ( !anchorlayout )
        {
        ALF_THROW(AlfVisualException, EInvalidVisual, ClassName)
        }

    AlfAnchorLayoutAttributeSetter::setAttributeValue(
        aVisual, aContainer, aData);
    }

// ---------------------------------------------------------------------------
// Deprecated
// ---------------------------------------------------------------------------
//
OSN_EXPORT TAlfCommand* AlfLCTAnchorLayoutAttributeSetter::createCommand(
    CAlfVisual& /*aVisual*/,
    AlfAttributeContainer* /*aContainer*/, IAlfMap* /*aData*/,
     int /*aTransitionTime*/, CAlfVisual* /*aRefVisual*/ )
    {
    //deprecated
    TAlfCommand* cmd = 0;
    return cmd;
    }

// ---------------------------------------------------------------------------
// Sends a command to Env
// ---------------------------------------------------------------------------
//
OSN_EXPORT void AlfLCTAnchorLayoutAttributeSetter::createAndSendCommands(
    CAlfVisual& aVisual,
    AlfAttributeContainer* aContainer,
    CAlfVisual* aRefVisual )
    {
    AlfAnchorLayoutAttributeSetter::createAndSendCommands(
        aVisual, aContainer, aRefVisual);
    }

// ---------------------------------------------------------------------------
// Sets dynamic attributes to visual
// ---------------------------------------------------------------------------
//
void AlfLCTAnchorLayoutAttributeSetter::handleDynamicAttribute(
    CAlfVisual& aVisual,
    AlfAttribute& aAttr, 
    AlfAttributeContainer& aContainer)
    {
    const char* attrName = aAttr.name();

#ifdef RD_ALF_IN_PLATFORM
    if ( !strcmp ( attrName, KLCT ) )
        {
        TAknLayoutHierarchyComponentHandle componentHandle =
            AlfLctAttributeSetterUtils::getCommonLctAttributes(&aContainer);

        int width(0);
        int height(0);
        if (AlfLctAttributeSetterUtils::itemSize(componentHandle, 
            width, height))
            {
            TInt displayWidth = 
                aVisual.Env().PrimaryDisplay().VisibleArea().Width();
            if (width > displayWidth)
                {
                width = displayWidth;
                }
                
            TAlfTimedPoint size(width, height);
            aVisual.SetSize(size);
            }
        }
    else
#endif
    if (strcmp(attrName,KRows) && 
        strcmp(attrName,KColumns) && 
        strcmp(attrName,KChildOrdinal))
        {
        AlfAnchorLayoutAttributeSetter::handleDynamicAttribute(
            aVisual, aAttr, aContainer);
        }
    }

// ---------------------------------------------------------------------------
// Sets static attributes to visual
// ---------------------------------------------------------------------------
//
void AlfLCTAnchorLayoutAttributeSetter::handleStaticAttribute(
    CAlfVisual &aVisual,
    AlfAttribute& aAttr, 
    AlfAttributeContainer& aContainer)
    {
    CAlfLCTAnchorLayout* anchorlayout = 
        dynamic_cast<CAlfLCTAnchorLayout*>(&aVisual);
        
    const char* attrName = aAttr.name();
    
#ifdef RD_ALF_IN_PLATFORM
    if ( !strcmp ( attrName, KLCT ) )
        {
        TAknLayoutHierarchyComponentHandle componentHandle =
            AlfLctAttributeSetterUtils::getCommonLctAttributes(&aContainer);

        int width(0);
        int height(0);
        if (AlfLctAttributeSetterUtils::itemSize(componentHandle, 
            width, height))
            {
            // Some cases returns wrong width, so check that width is reasonable
            TInt displayWidth = 
                aVisual.Env().PrimaryDisplay().VisibleArea().Width();
            if (width > displayWidth)
                {
                width = displayWidth;
                }
            TAlfTimedPoint size(width, height);
            aVisual.SetSize(size);
            }
        }
    else if ( !strcmp(attrName, KRows)||
            !strcmp (attrName,KColumns) || !strcmp(attrName,KChildOrdinal) )
        {
        TAknLayoutHierarchyComponentHandle componentHandle =
            AlfLctAttributeSetterUtils::getCommonLctAttributes(&aContainer);
        AlfAttribute& attrRows = aContainer.getAttributeByName(KRows);
        AlfAttribute& attrCols = aContainer.getAttributeByName(KColumns);
        AlfAttribute& attrOrdinal = 
            aContainer.getAttributeByName(KChildOrdinal);
        componentHandle.SetColumn(attrCols.realValue());
        componentHandle.SetRow(attrRows.realValue());
        TAlfTimedPoint timedpoint(attrCols.getSourceValue()->realValue(),
                                  attrRows.getSourceValue()->realValue());

        //since static, effect will be immediate
        timedpoint.iX.SetTarget(attrCols.getTargetValue()->realValue(), 0);
        timedpoint.iY.SetTarget(attrRows.getTargetValue()->realValue(), 0);

        timedpoint.iX.SetStyle(attrCols.getInterpolationStyle());
        timedpoint.iY.SetStyle(attrRows.getInterpolationStyle());

        //TO ADD: Mapping Function Identifiers
        anchorlayout->SetAnchors(attrOrdinal.intValue(), 
            componentHandle, timedpoint);
        attrRows.setDirty(false);
        attrCols.setDirty(false);
        attrOrdinal.setDirty(false);
        }
    else
#endif
        {
        AlfAnchorLayoutAttributeSetter::handleStaticAttribute(aVisual, aAttr,
                aContainer);
        }
    }

// ---------------------------------------------------------------------------
// Sets dynamic attributes to visual from data
// ---------------------------------------------------------------------------
//
void AlfLCTAnchorLayoutAttributeSetter::handleDynamicDataAttribute(
    CAlfVisual &aVisual,
    AlfAttribute& aAttr, 
    AlfAttributeContainer& aContainer, 
    IAlfMap* aData )
    {
    AlfAnchorLayoutAttributeSetter::handleDynamicDataAttribute(aVisual, aAttr,
            aContainer, aData);
    }

// ---------------------------------------------------------------------------
// Sets static attributes to visual  from data
// ---------------------------------------------------------------------------
//
void AlfLCTAnchorLayoutAttributeSetter::handleStaticDataAttribute(
    CAlfVisual &aVisual,
    AlfAttribute& aAttr, 
    AlfAttributeContainer& aContainer, 
    IAlfMap* aData )
    {
    AlfAnchorLayoutAttributeSetter::handleStaticDataAttribute(aVisual, aAttr,
            aContainer, aData);
    }

    } // namespace Alf

// End of file





