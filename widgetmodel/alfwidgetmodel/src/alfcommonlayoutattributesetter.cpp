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
* Description:   Implements common attributesetters for layout.
*
*/



#include "alf/alfattribute.h"
#include "alf/alfcommonlayoutattributesetter.h"
#include "alf/alfattributevaluetype.h"
#include <alf/alfvisual.h>
#include <alf/alflayout.h>
#include <alf/alfattributeexception.h>
#include "alf/alfattributecontainer.h"
#include <libc/string.h>
#include <osn/ustring.h>
#include "alf/attrproperty.h"
#include <alf/alfexception.h>
#include <alf/alfdataexception.h>
#include <alf/alfvisualexception.h>
#include <e32cmn.h>
#include <utf.h>

using namespace osncore;

using namespace duiuimodel::layoutattributes;

namespace Alf
    {

static void throwIfNot ( bool aBoolean )
    {
    if ( !aBoolean )
        {
        ALF_THROW ( AlfAttributeException, EInvalidAttribute,"CAlfCommonlayoutAttributeSetter")
        }
    }

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Constructor.
// ---------------------------------------------------------------------------
//
OSN_EXPORT AlfCommonLayoutAttributeSetter::AlfCommonLayoutAttributeSetter()
    {
    }

// ---------------------------------------------------------------------------
// Destructor.
// ---------------------------------------------------------------------------
//
OSN_EXPORT AlfCommonLayoutAttributeSetter::~AlfCommonLayoutAttributeSetter()
    {
    }

// ---------------------------------------------------------------------------
// Sets Attribute Value. Delegates based on attribute Category.
// ---------------------------------------------------------------------------
//
OSN_EXPORT void AlfCommonLayoutAttributeSetter::setAttributeValue (
    CAlfVisual &aVisual,
    AlfAttributeContainer* aContainer,
    IAlfMap* aData )
    {
    CAlfLayout* layout = dynamic_cast<CAlfLayout*> ( &aVisual );

    if ( !layout )
        {
        ALF_THROW ( AlfVisualException, EInvalidVisual,
                    "AlfCommonLayoutAttributeSetter" )
        }

    AlfCommonVisualAttributeSetter::setAttributeValue(aVisual,
            aContainer, aData);
    }

void AlfCommonLayoutAttributeSetter::handleStaticDataAttribute ( CAlfVisual &aVisual, AlfAttribute& aAttr,
        AlfAttributeContainer& aContainer, IAlfMap* aData )
    {

    if ( aData )
        {
        CAlfLayout* layout = dynamic_cast<CAlfLayout*> ( &aVisual );
        if(!layout)
            {
            return;
            }

        const char* attrName = aAttr.name();

        if ( !strcmp ( attrName, KBaseUnitX ) )
            {
            IAlfVariantType* dataX = aData->item ( UString(aAttr.getDataField()) );
            AlfAttribute& attrY = aContainer.getAttributeByName ( KBaseUnitY );

            IAlfVariantType* dataY = aData->item ( UString(attrY.getDataField()));

            if (dataX!=NULL&&dataX->type() == IAlfVariantType::EMap&&
                    dataY!=NULL&&dataY->type() == IAlfVariantType::EMap)
                {
                IAlfVariantType* valueX=((IAlfMap*)dataX)->item(UString("value"));
                IAlfVariantType* unitX=((IAlfMap*)dataX)->item(UString("unit"));
                IAlfVariantType* valueY=((IAlfMap*)dataY)->item(UString("value"));
                IAlfVariantType* unitY=((IAlfMap*)dataY)->item(UString("unit"));

                TAlfMetric xMetric ( valueX->real(), (TAlfUnit)unitX->integer() );
                TAlfMetric yMetric ( valueX->real(), (TAlfUnit)unitX->integer());
                TAlfXYMetric xyMetric ( xMetric, yMetric );
                layout->SetBaseUnit( xyMetric );
                }
            else if (dataX!=NULL&&dataX->type() == IAlfVariantType::EReal&&
                     dataY!=NULL&&dataY->type() == IAlfVariantType::EReal)
                {
                TAlfMetric xMetric ( (float)dataX->real() );
                TAlfMetric yMetric ( (float)dataY->real());
                TAlfXYMetric xyMetric ( xMetric, yMetric );
                layout->SetBaseUnit( xyMetric );
                }
            }
        else if ( !strcmp ( attrName, KBaseUnitY ) )
            {
            //we have handled it
            }

        else if ( !strcmp ( attrName, KUpdateLayoutTime ) )
            {
            IAlfVariantType* data = aData->item (UString(aAttr.getDataField()) );

            if ( data && data->type() == IAlfVariantType::EInt )
                {
                layout->SetTransitionTime ( data->integer() );
                }
            else if ( data && data->type() == IAlfVariantType::EReal )
                {
                layout->SetTransitionTime ( ( int ) data->real() );
                }
            }
        else if ( !strcmp ( attrName, KTransitionTime ) )
            {
            IAlfVariantType* data = aData->item (UString(aAttr.getDataField()) );

            if ( data && data->type() == IAlfVariantType::EInt )
                {
                layout->SetTransitionTime ( data->integer() );
                }
            else if ( data && data->type() == IAlfVariantType::EReal )
                {
                layout->SetTransitionTime ( ( int ) data->real() );
                }
            }
        else if ( !strcmp ( attrName, KAutomaticLocaleMirroring ) )
            {
            IAlfVariantType* data = aData->item (UString(aAttr.getDataField()) );
            throwIfNot ( data != NULL && data->type() == IAlfVariantType::EString );

            if ( !strcmp ( data->string().getUtf8(), "true" ) )
                layout->SetFlag ( EAlfVisualFlagAutomaticLocaleMirroringEnabled );
            else if ( !strcmp ( data->string().getUtf8(), "false" ) )
                layout->ClearFlag ( EAlfVisualFlagAutomaticLocaleMirroringEnabled );
            }
        else if ( !strcmp ( attrName, KInnerPaddingHoriz ) )
            {
            IAlfVariantType* dataX = aData->item (UString(aAttr.getDataField()) );
            AlfAttribute& attrY = aContainer.getAttributeByName ( KInnerPaddingVertical );

            IAlfVariantType* dataY = aData->item ( UString(attrY.getDataField() ));

            if (dataX&&dataX->type() == IAlfVariantType::EMap&&dataY
                    &&dataY->type()== IAlfVariantType::EMap)
                {
                IAlfVariantType* valueX=((IAlfMap*)dataX)->item(UString("value"));
                IAlfVariantType* unitX=((IAlfMap*)dataX)->item(UString("unit"));
                IAlfVariantType* valueY=((IAlfMap*)dataY)->item(UString("value"));
                IAlfVariantType* unitY=((IAlfMap*)dataY)->item(UString("unit"));

                TAlfMetric xMetric ( valueX->real(), (TAlfUnit)unitX->integer() );
                TAlfMetric yMetric ( valueX->real(), (TAlfUnit)unitX->integer());
                TAlfXYMetric xyMetric ( xMetric, yMetric );
                layout->SetInnerPadding( xyMetric );
                }
            if ( dataX&&dataX->type() == IAlfVariantType::EReal&&dataY
                    &&dataY->type()== IAlfVariantType::EReal)
                {
                TAlfMetric xMetric ( dataX->real(),EAlfUnitPixel );
                TAlfMetric yMetric ( dataY->real() ,EAlfUnitPixel);
                TAlfXYMetric xyMetric ( xMetric, yMetric );
                layout->SetInnerPadding( xyMetric );
                }
            }
        else if ( !strcmp ( attrName, KInnerPaddingVertical ) )
            {
			// we have handled it
            }
        else if ( !strcmp ( attrName, KScrolling ) )
            {
            IAlfVariantType* data = aData->item ( UString(aAttr.getDataField() ));

            throwIfNot ( data != NULL && data->type() == IAlfVariantType::EString );

            if ( !strcmp ( data->string().getUtf8(), "true" ) )
                {
                TRAPD(err,layout->EnableScrollingL())
                throwIfNot(err!=KErrNone);
                }

            else
                {
                TRAPD(err,layout->EnableScrollingL(false))
                throwIfNot(err!=KErrNone);
                }
            }
        else if ( !strcmp ( attrName, KLayoutScrollOffsetX ) )
            {
            IAlfVariantType* dataX = aData->item ( UString(aAttr.getDataField() ));
            throwIfNot ( dataX != NULL && dataX->type() == IAlfVariantType::EReal );

            AlfAttribute& attrY = aContainer.getAttributeByName ( KLayoutScrollOffsetY );
            IAlfVariantType* dataY = aData->item (UString( attrY.getDataField() ));
            throwIfNot ( dataY != NULL && dataY->type() == IAlfVariantType::EReal );

            TAlfTimedPoint point ( dataX->real(), dataY->real() );
            layout->SetScrollOffset ( point );
            }
        else if ( !strcmp ( attrName, KLayoutScrollOffsetY ) )
            {
			// we have handled it
            }
        else
            {
            AlfCommonVisualAttributeSetter::handleStaticDataAttribute( aVisual, aAttr, aContainer, aData);
            }
        }
    }

void AlfCommonLayoutAttributeSetter::handleDynamicDataAttribute ( CAlfVisual &aVisual, AlfAttribute& aAttr,
        AlfAttributeContainer& aContainer, IAlfMap* aData )
    {
    AlfCommonVisualAttributeSetter::handleDynamicDataAttribute( aVisual, aAttr, aContainer, aData);
    }

void AlfCommonLayoutAttributeSetter::handleStaticAttribute ( CAlfVisual &aVisual, AlfAttribute& aAttr,
        AlfAttributeContainer& aContainer)
    {
    CAlfLayout* layout = dynamic_cast<CAlfLayout*> ( &aVisual );
    if(!layout)
        {
        return;
        }
    const char* attrName = aAttr.name();

    if ( !strcmp ( attrName, KBaseUnitX )||!strcmp ( attrName, KBaseUnitY ) )
        {
        AlfAttribute& attrX = aContainer.getAttributeByName ( KBaseUnitX );
        AlfAttribute& attrY = aContainer.getAttributeByName ( KBaseUnitY );
        TAlfMetric xMetric ( attrX.realValue(), attrX.unit() );
        TAlfMetric yMetric ( attrY.realValue(), attrY.unit() );
        TAlfXYMetric xyMetric ( xMetric, yMetric );
        layout->SetBaseUnit ( xyMetric );
        attrX.setDirty(false);
        attrY.setDirty(false);
        }
    else if ( !strcmp ( attrName, KUpdateLayoutTime ) )
        {
        if ( aAttr.type() == AlfAttributeValueType::EInt )
            {
            layout->SetTransitionTime ( aAttr.intValue() );
            }
        else if ( aAttr.type() == AlfAttributeValueType::EFloat )
            {
            layout->SetTransitionTime ( ( int ) aAttr.realValue() );
            }
        }
    else if ( !strcmp ( attrName, KTransitionTime ) )
        {
        if ( aAttr.type() == AlfAttributeValueType::EInt )
            {
            layout->SetTransitionTime ( aAttr.intValue() );
            }
        else if ( aAttr.type() == AlfAttributeValueType::EFloat )
            {
            layout->SetTransitionTime ( ( int ) aAttr.realValue() );
            }
        }
    else if ( !strcmp ( attrName, KAutomaticLocaleMirroring ) )
        {
        throwIfNot ( aAttr.type() == AlfAttributeValueType::EString );

        if ( !strcmp ( aAttr.stringValue().getUtf8(), "true" ) )
            {
            layout->SetFlag ( EAlfVisualFlagAutomaticLocaleMirroringEnabled );
            }
        else if ( !strcmp ( aAttr.stringValue().getUtf8(), "false" ) )
            {
            layout->ClearFlag ( EAlfVisualFlagAutomaticLocaleMirroringEnabled );
            }
        }
    else if ( !strcmp ( attrName, KInnerPaddingHoriz )||!strcmp ( attrName, KInnerPaddingVertical ) )
        {
        AlfAttribute& attrY = aContainer.getAttributeByName ( KInnerPaddingVertical );
        AlfAttribute& attrX= aContainer.getAttributeByName ( KInnerPaddingHoriz );
        throwIfNot ( attrX.type() == AlfAttributeValueType::EFloat );
        throwIfNot ( attrY.type() == AlfAttributeValueType::EFloat );
        TAlfMetric xMetric ( attrX.realValue(), attrX.unit() );
        TAlfMetric yMetric ( attrY.realValue(), attrY.unit() );
        TAlfXYMetric xyMetric ( xMetric, yMetric );
        layout->SetInnerPadding ( xyMetric );
        attrX.setDirty(false);
        attrY.setDirty(false);
        }
    else if ( !strcmp ( attrName, KScrolling ) )
        {
        throwIfNot ( aAttr.type() == AlfAttributeValueType::EString );

        if ( !strcmp ( aAttr.stringValue().getUtf8(), "true" ) )
            {
            TRAPD(err, layout->EnableScrollingL () );
            throwIfNot (err==KErrNone);
            }
        else
            {
            TRAPD(err, layout->EnableScrollingL ( false ) );
            throwIfNot (err==KErrNone);
            }
        }
    else if ( !strcmp ( attrName, KLayoutScrollOffsetX )||!strcmp ( attrName, KLayoutScrollOffsetY ) )
        {
        AlfAttribute& attrY = aContainer.getAttributeByName ( KLayoutScrollOffsetY );
        AlfAttribute& attrX = aContainer.getAttributeByName ( KLayoutScrollOffsetX );

        TAlfTimedPoint point ( attrX.realValue(), attrY.realValue() );

        layout->SetScrollOffset ( point );
        attrX.setDirty(false);
        attrY.setDirty(false);
        }
    else
        {
        AlfCommonVisualAttributeSetter::handleStaticAttribute(aVisual, aAttr, aContainer);
        }
    }

void AlfCommonLayoutAttributeSetter::handleDynamicAttribute( CAlfVisual &aVisual, AlfAttribute& aAttr,
        AlfAttributeContainer& aContainer)
    {
    AlfCommonVisualAttributeSetter::handleDynamicAttribute(
        aVisual, aAttr, aContainer);
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
OSN_EXPORT TAlfCommand* AlfCommonLayoutAttributeSetter::createCommand (
    CAlfVisual& aVisual, AlfAttributeContainer* aContainer,
    IAlfMap* aData, int aTransitionTime, CAlfVisual* aRefVisual )
    {
    TAlfCommand* cmd = 0;
    cmd = AlfCommonVisualAttributeSetter::createCommand (
              aVisual,
              aContainer,
              aData,
              aTransitionTime,
              aRefVisual );
    return cmd;
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
OSN_EXPORT void AlfCommonLayoutAttributeSetter::createAndSendCommands (
    CAlfVisual& aVisual,
    AlfAttributeContainer* aContainer,
    CAlfVisual* aRefVisual )
    {

    unsigned int attrCount = aContainer->attributeCount();

    for ( int i = 0; i < attrCount; ++i )
        {
        AlfAttribute* attrib = & ( aContainer->getAttribute ( i ) );
        const char* attrName = attrib->name();

        if (strcmp ( attrName, KBaseUnitX )&&strcmp ( attrName, KBaseUnitY )
                &&strcmp ( attrName, KUpdateLayoutTime )&&strcmp ( attrName, KTransitionTime )
                &&strcmp ( attrName, KAutomaticLocaleMirroring )&&strcmp ( attrName, KInnerPaddingHoriz )
                &&strcmp ( attrName, KInnerPaddingVertical ))
            {
            // Call the base class implementation also.
            // It goes through the same loop again and checks if there are attributes
            // that it handles.
            AlfCommonVisualAttributeSetter::createAndSendCommands (
                aVisual,
                aContainer,
                aRefVisual );
            }
        }
    }


    } // Alf
