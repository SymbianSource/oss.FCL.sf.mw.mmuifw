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
* Description:   Implements attributesetters for curvePathLayout.
*
*/



//includes

//widget model includes
#include "alf/alfattributecontainer.h"
#include "alf/alfattributevaluetype.h"
#include "alf/alfattribute.h"
#include "alf/alfcurvepathlayoutattributesetter.h"
#include <alf/alfvisualexception.h>
#include <alf/alfdataexception.h>
#include <alf/alfattributeexception.h>


//osn includes
#include <osn/ustring.h>

//alf includes
#include <alf/alfenv.h>
#include <alf/alfcurvepathlayout.h>
#include <alf/alfcurvepath.h>

//dui includes
#include "alf/attrproperty.h"

//other includes
#include <libc/string.h>
#include <utf.h>

//namespaces

using namespace osncore;

using namespace duiuimodel::commonvisualattributes;

using namespace duiuimodel::layoutattributes;

using namespace duiuimodel::curvepathattributes;


namespace Alf
    {

static void throwIfErr ( int aErr )
    {
    if (aErr!=KErrNone)
        {
        ALF_THROW ( AlfAttributeException, aErr,
                    "AlfCurvePathLayoutAttributeSetter")
        }
    }

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Constructor.
// ---------------------------------------------------------------------------
//
OSN_EXPORT AlfCurvePathLayoutAttributeSetter::
AlfCurvePathLayoutAttributeSetter()
    {
    }

// ---------------------------------------------------------------------------
// Destructor.
// ---------------------------------------------------------------------------
//
OSN_EXPORT AlfCurvePathLayoutAttributeSetter::
~AlfCurvePathLayoutAttributeSetter()
    {
    }


// ---------------------------------------------------------------------------
// Sets Attribute Value. Delegates based on attribute Category.
// ---------------------------------------------------------------------------
//
OSN_EXPORT void AlfCurvePathLayoutAttributeSetter::setAttributeValue (
    CAlfVisual &aVisual,
    AlfAttributeContainer* aContainer,
    IAlfMap* aData )
    {

    CAlfCurvePathLayout* curvepathlayout =
        dynamic_cast<CAlfCurvePathLayout*>(&aVisual);

    if ( !curvepathlayout )
        {
        ALF_THROW ( AlfVisualException, EInvalidVisual,
                    "AlfCurvePathLayoutAttributeSetter" )
        }

    curvepathlayout->CurvePath().Reset();

    AlfCommonLayoutAttributeSetter::setAttributeValue(aVisual, aContainer,
            aData);

    }

// ---------------------------------------------------------------------------
// Deprecated
// ---------------------------------------------------------------------------
//
OSN_EXPORT TAlfCommand* AlfCurvePathLayoutAttributeSetter::createCommand (
    CAlfVisual& /*aVisual*/,
    AlfAttributeContainer* /*aContainer*/,
    IAlfMap* /*aData*/,
    int /*aTransitionTime*/,
    CAlfVisual* /*aRefVisual*/ )
    {
    //deprecated
    TAlfCommand* cmd = NULL;
    return cmd;
    }

// ---------------------------------------------------------------------------
// Sends a command to Env
// ---------------------------------------------------------------------------
//
OSN_EXPORT void AlfCurvePathLayoutAttributeSetter::createAndSendCommands (
    CAlfVisual& aVisual,
    AlfAttributeContainer* aContainer,
    CAlfVisual* aRefVisual )
    {
    //SetOffset in CAlfCurvePath can be sent as a TAlfCustomEventCommand

    AlfCommonLayoutAttributeSetter::createAndSendCommands (
        aVisual, aContainer, aRefVisual );
    }


// ---------------------------------------------------------------------------
// Sets dynamic attributes to visual
// ---------------------------------------------------------------------------
//
void AlfCurvePathLayoutAttributeSetter::handleDynamicAttribute (
    CAlfVisual &aVisual,
    AlfAttribute& aAttr,
    AlfAttributeContainer& aContainer)
    {
    CAlfCurvePathLayout* curvepathlayout =
        dynamic_cast<CAlfCurvePathLayout*>(&aVisual);
    if(!curvepathlayout)
        {
        return;
        }

    const char* attrName = aAttr.name();

    if ( !strcmp ( attrName, KOffsetX ) ||!strcmp ( attrName, KOffsetY ) )
        {
        // throws if attribute is not found
        AlfAttribute& offsety = aContainer.getAttributeByName ( KOffsetY );
        AlfAttribute& offsetx = aContainer.getAttributeByName ( KOffsetX );

        TAlfTimedPoint offset((TReal32)offsetx.getSourceValue()->realValue(),
                              (TReal32)offsety.getSourceValue()->realValue());

        offset.iX.SetTarget((TReal32)offsetx.getTargetValue()->realValue(),
                            offsetx.getTime());

        offset.iY.SetTarget((TReal32)offsety.getTargetValue()->realValue(),
                            offsety.getTime());

        offset.iX.SetStyle ( offsetx.getInterpolationStyle() );
        offset.iY.SetStyle ( offsety.getInterpolationStyle() );

        offset.iX.SetMappingFunctionIdentifier(
            offsetx.getMappingFunctionId());
        offset.iY.SetMappingFunctionIdentifier(
            offsety.getMappingFunctionId());

        curvepathlayout->CurvePath().SetOffset( offset );
        offsetx.setDirty(false);
        offsety.setDirty(false);
        }
    else
        {
        AlfCommonLayoutAttributeSetter::handleDynamicAttribute(
            aVisual, aAttr, aContainer);
        }
    }

// ---------------------------------------------------------------------------
// Sets static attributes to visual
// ---------------------------------------------------------------------------
//
void AlfCurvePathLayoutAttributeSetter::handleStaticAttribute (
    CAlfVisual &aVisual,
    AlfAttribute& aAttr,
    AlfAttributeContainer& aContainer)
    {

    CAlfCurvePathLayout* curvepathlayout =
        dynamic_cast<CAlfCurvePathLayout*>(&aVisual);
    if(!curvepathlayout)
        {
        return;
        }

    const char* attrName = aAttr.name();

    if ( !strcmp ( attrName, KEnableLoop ) )
        {
        // if value is not int, then use default parameter of EnableLoop
        if ( AlfAttributeValueType::EInt != aAttr.type() )
            {
            curvepathlayout->CurvePath().EnableLoop();
            }

        else
            {
            int enableLoop = aAttr.intValue();
            TBool flag = ETrue;      //Assume true for any non-zero value.
            if ( 0 == enableLoop )
                {
                flag = EFalse;
                }
            curvepathlayout->CurvePath().EnableLoop( flag );
            }
        }

    else if ( !strcmp ( attrName, KOrigin ) )
        {
        curvepathlayout->CurvePath().SetOrigin( aAttr.realValue() );
        }

    else if ( !strcmp ( attrName, KOffsetX ) || !strcmp ( attrName, KOffsetY ) )
        {
        // throws if attribute is not found
        AlfAttribute& offsety = aContainer.getAttributeByName ( KOffsetY );
        AlfAttribute& offsetx = aContainer.getAttributeByName ( KOffsetX );

        TAlfTimedPoint offset( offsetx.realValue(), offsety.realValue());
        curvepathlayout->CurvePath().SetOffset ( offset );
        offsety.setDirty(false);
        offsetx.setDirty(false);
        }
    else if ( !strcmp ( attrName, KLine ) )
        {
        // This multi-value attribute has to be filled in this order:
        // startx,starty,endx,endy,linelength

        if ( 5 != aAttr.getTargetValueCount())
            {
            ALF_THROW ( AlfAttributeException, EInvalidAttribute,
                        "AlfCurvePathLayoutAttributeSetter" );
            }

        float startx = this->floatOrInt( aAttr, 0 );
        float starty = this->floatOrInt( aAttr, 1 );
        float endx   = this->floatOrInt( aAttr, 2 );
        float endy   = this->floatOrInt( aAttr, 3 );
        float len    = aAttr.realValue(4);

        TRAPD(err1,curvepathlayout->CurvePath().AppendLineL(
                  TAlfRealPoint(startx,starty),
                  TAlfRealPoint(endx,endy),len )
             );
        throwIfErr(err1);
        }

    else if ( !strcmp ( attrName, KArc ) )
        {
        // This multi-value attribute has to be filled in this order:
        // arcoriginx,arcoriginy,archorzradius,arcvertradius,arcstartangle,
        // arcendangle,arclength

        if ( 7 != aAttr.getTargetValueCount())
            {
            ALF_THROW ( AlfAttributeException, EInvalidAttribute,
                        "AlfCurvePathLayoutAttributeSetter" );
            }

        float originx    = this->floatOrInt( aAttr, 0 );
        float originy    = this->floatOrInt( aAttr, 1 );
        float horzradius = this->floatOrInt( aAttr, 2 );
        float vertradius = this->floatOrInt( aAttr, 3 );
        float startangle = aAttr.realValue(4);
        float endangle   = aAttr.realValue(5);
        float len        = aAttr.realValue(6);
        TRAPD(err1,curvepathlayout->CurvePath().AppendArcL(
                  TAlfRealPoint(originx,originy),
                  TAlfRealSize(horzradius,vertradius),
                  startangle,endangle,len));
        throwIfErr(err1);
        }

    else
        {
        AlfCommonLayoutAttributeSetter::handleStaticAttribute (
            aVisual, aAttr, aContainer);
        }
    }
// ---------------------------------------------------------------------------
// Sets dynamic attributes to visual from data
// ---------------------------------------------------------------------------
//
void AlfCurvePathLayoutAttributeSetter::handleDynamicDataAttribute (
    CAlfVisual &aVisual,
    AlfAttribute& aAttr,
    AlfAttributeContainer& aContainer,
    IAlfMap* aData )
    {
    CAlfCurvePathLayout* curvepathlayout =
        dynamic_cast<CAlfCurvePathLayout*> ( &aVisual );
    if ( !curvepathlayout )
        {
        ALF_THROW ( AlfDataException, ECommonError, "AlfCurvePathLayoutAttributeSetter" )
        }        

    const char* attrName = aAttr.name();
    const char* dataField = aAttr.getDataField();

    if ( !dataField )
        {
        ALF_THROW ( AlfDataException, ECommonError, "AlfCurvePathLayoutAttributeSetter" )
        }

    if (!aData)
        {
        ALF_THROW ( AlfDataException, ECommonError,
                    "AlfCurvePathLayoutAttributeSetter" )
        }

    IAlfVariantType* data = aData->item ( UString(dataField) );

    if (data)
        {
        const char* attrName = aAttr.name();

        if ( !strcmp ( attrName, KOffsetX ) )
            {
            // throws if attribute not found
            AlfAttribute& offsety = aContainer.getAttributeByName( KOffsetY );

            const char* offsetyDataField = offsety.getDataField();
            if ( !offsetyDataField )
                {
                ALF_THROW ( AlfDataException,
                            ECommonError, "AlfCurvePathLayoutAttributeSetter")
                }

            IAlfVariantType* offsetyData = aData->item ( UString(offsetyDataField) );

            if ( data->type() == IAlfVariantType::EReal &&
                    offsetyData && offsetyData->type() == IAlfVariantType::EReal)
                {
                // The time value is in the attribute? Is it not
                // part of data as well?
                TAlfTimedPoint offset;
                offset.iX.SetTarget(data->real(), aAttr.getTime());
                offset.iX.SetStyle ( aAttr.getInterpolationStyle() );
                offset.iX.SetMappingFunctionIdentifier (aAttr.getMappingFunctionId());

                offset.iY.SetTarget(offsetyData->real(), offsety.getTime());
                offset.iY.SetStyle ( offsety.getInterpolationStyle() );
                offset.iY.SetMappingFunctionIdentifier (
                    offsety.getMappingFunctionId());

                curvepathlayout->CurvePath().SetOffset( offset );
                }
            }

        else if ( !strcmp ( attrName, KOffsetY ) )
            {
            //Make sure X Attribute also exists, but do nothing.
            //Actual values will be set in the iteration that checks
            //for KOffsetX
            aContainer.getAttributeByName ( KOffsetX );
            //will throw if attribute is not found
            }

        else
            {
            AlfCommonLayoutAttributeSetter::handleDynamicDataAttribute(
                aVisual, aAttr, aContainer, aData);
            }
        }
    }

// ---------------------------------------------------------------------------
// Sets static attributes to visual  from data
// ---------------------------------------------------------------------------
//
void AlfCurvePathLayoutAttributeSetter::handleStaticDataAttribute (
    CAlfVisual &aVisual,
    AlfAttribute& aAttr,
    AlfAttributeContainer& aContainer,
    IAlfMap* aData )
    {
    CAlfCurvePathLayout* curvepathlayout =
        dynamic_cast<CAlfCurvePathLayout*> ( &aVisual );
    if(!curvepathlayout)
        {
        ALF_THROW ( AlfDataException, ECommonError,
                            "AlfCurvePathLayoutAttributeSetter" )
        }
    const char* attrName = aAttr.name();

    const char* dataField = aAttr.getDataField();

    if ( !dataField )
        {
        ALF_THROW ( AlfDataException, ECommonError,
                    "AlfCurvePathLayoutAttributeSetter" )
        }

    if (!aData)
        {
        ALF_THROW ( AlfDataException, ECommonError,
                    "AlfCurvePathLayoutAttributeSetter" )
        }

    IAlfVariantType* data = aData->item ( UString(dataField) );

    if ( data )
        {

        if ( !strcmp ( attrName, KEnableLoop ) )
            {
            // if value is not bool, then use default parameter of EnableLoop
            if ( data->type() != IAlfVariantType::EBool )
                {
                curvepathlayout->CurvePath().EnableLoop();
                }
            else
                {
                curvepathlayout->CurvePath().EnableLoop( data->boolean() );
                }
            }

        else if ( !strcmp ( attrName, KOrigin ) )
            {
            if ( data->type() == IAlfVariantType::EReal )
                {
                float origin =  data->real() ;
                curvepathlayout->CurvePath().SetOrigin ( origin );
                }
            }

        else if ( !strcmp ( attrName, KOffsetX ) )
            {
            AlfAttribute& offsety
            = aContainer.getAttributeByName ( KOffsetY );
            //will throw if attribute is not found

            const char* offsetyDataField = offsety.getDataField();
            if ( !offsetyDataField )
                {
                ALF_THROW ( AlfDataException,
                            ECommonError, "AlfCurvePathLayoutAttributeSetter")
                }

            IAlfVariantType* offsetyData = aData->item ( UString(offsetyDataField) );

            if ( data->type() == IAlfVariantType::EReal &&
                    offsetyData && offsetyData->type() == IAlfVariantType::EReal)
                {
                TAlfTimedPoint offset( data->real(), offsetyData->real());
                curvepathlayout->CurvePath().SetOffset( offset );
                }
            }

        else if ( !strcmp ( attrName, KOffsetY ) )
            {
            //Make sure X Attribute also exists, but do nothing.
            //Actual values will be set in the iteration that checks for
            //KOffsetX
            aContainer.getAttributeByName( KOffsetX );
            //will throw if attribute is not found
            }

        else if ( !strcmp ( attrName, KLine ) )
            {

            //if not a container, dont proceed
            if (!(data->type() == IAlfVariantType::EContainer))
                {
                return;
                }

            IAlfContainer* container = data->container();
            //if all item are not present, throw
            if (container->count() != 5)
                {
                ALF_THROW ( AlfAttributeException, EInvalidAttribute,
                            "AlfCurvePathLayoutAttributeSetter" );
                }

            IAlfVariantType* lineStartXvalue = container->item(0);
            IAlfVariantType* lineStartYvalue = container->item(1);
            IAlfVariantType* lineEndXvalue = container->item(2);
            IAlfVariantType* lineEndYvalue = container->item(3);
            IAlfVariantType* lineLengthvalue = container->item(4);

            float startx = this->floatOrIntFromData( lineStartXvalue);
            float starty = this->floatOrIntFromData( lineStartYvalue);
            float endx   = this->floatOrIntFromData( lineEndXvalue  );
            float endy   = this->floatOrIntFromData( lineEndYvalue  );
            float len    = float(lineLengthvalue->real());
            TRAPD(err1,
                  curvepathlayout->CurvePath().AppendLineL(
                      TAlfRealPoint(startx,starty),
                      TAlfRealPoint(endx,endy),len)
                 );
            throwIfErr(err1);
            }

        else if ( !strcmp ( attrName, KArc ) )
            {

            //if not a container, dont proceed
            if (!(data->type() == IAlfVariantType::EContainer))
                {
                return;
                }

            IAlfContainer* container = data->container();

            //if all item are not present, throw
            if (container->count() != 7)
                {
                ALF_THROW ( AlfAttributeException, EInvalidAttribute,
                            "AlfCurvePathLayoutAttributeSetter" );
                }

            IAlfVariantType* arcOriginXValue = container->item(0);
            IAlfVariantType* arcOriginYValue = container->item(1);
            IAlfVariantType* archorzradiusValue = container->item(2);
            IAlfVariantType* arcvertradiusValue = container->item(3);
            IAlfVariantType* arcstartangleValue = container->item(4);
            IAlfVariantType* arcendangleValue = container->item(5);
            IAlfVariantType* arclengthValue = container->item(6);

            float originx    = this->floatOrIntFromData( arcOriginXValue);
            float originy    = this->floatOrIntFromData( arcOriginYValue);
            float horzradius = this->floatOrIntFromData( archorzradiusValue);
            float vertradius = this->floatOrIntFromData( arcvertradiusValue);
            float startangle = float(arcstartangleValue->real());
            float endangle   = float(arcendangleValue->real());
            float len        = float(arclengthValue->real());
            TRAPD(err1,
                  curvepathlayout->CurvePath().AppendArcL(
                      TAlfRealPoint(originx,originy),
                      TAlfRealSize(horzradius,vertradius),
                      startangle,endangle,len));
            throwIfErr(err1);
            }
        else
            {
            AlfCommonLayoutAttributeSetter::handleStaticDataAttribute (
                aVisual, aAttr, aContainer, aData );
            }
        }
    }

// ---------------------------------------------------------------------------
// Check if data in the attribute is int or float.
// Return the value as a float value
// ---------------------------------------------------------------------------
//
float AlfCurvePathLayoutAttributeSetter::floatOrInt(
    const AlfAttribute& aAttr,
    int aIndex)
    {
    if (aAttr.type(aIndex) == AlfAttributeValueType::EInt)
        {
        return float(aAttr.intValue(aIndex));
        }
    else if (aAttr.type(aIndex) == AlfAttributeValueType::EFloat)
        {
        return aAttr.realValue(aIndex); //will throw an exception i
        }
    else
        {
        ALF_THROW ( AlfAttributeException,
                    ECommonError, "AlfCurvePathLayoutAttributeSetter" );
        }
    }


// ---------------------------------------------------------------------------
// Check if data in the variant type is int or float.
// Return the value as a float value
// ---------------------------------------------------------------------------
//
float AlfCurvePathLayoutAttributeSetter::floatOrIntFromData(
    IAlfVariantType* aData)
    {
    if (IAlfVariantType::EInt == aData->type())
        {
        return float(aData->integer());
        }
    else if (IAlfVariantType::EReal == aData->type())
        {
        return aData->real();; //will throw an exception i
        }
    else
        {
        ALF_THROW ( AlfAttributeException,
                    ECommonError, "AlfCurvePathLayoutAttributeSetter" );
        }
    }

    } // namespace Alf
// End of file





