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
* Description:  attributesetter for common visual attributes
*
*/


#include "alf/alfattribute.h"
#include "alf/alfcommonvisualattributesetter.h"
#include "alf/alfattributevaluetype.h"
#include <alf/alfvisual.h>
#include <alf/alfattributeexception.h>
#include <alf/alfdataexception.h>
#include <alf/alfvisualexception.h>
#include "alf/alfattributecontainer.h"
#include <libc/string.h>
#include <osn/ustring.h>
#include "alf/attrproperty.h"
#include <alf/alfenv.h>

using namespace osncore;

using namespace duiuimodel::commonattributes;
using namespace duiuimodel::commonattributevalues;
using namespace duiuimodel::commonvisualattributes;
using namespace duiuimodel::layoutattributes;
using namespace duiuimodel::rect;
using namespace duiuimodel::padding;
using namespace duiuimodel::tactileattributes;

namespace Alf
    {

// ---------------------------------------------------------------------------
// gets the data from map pointed by attribute's field name.
// ---------------------------------------------------------------------------
//
IAlfVariantType* getData(const AlfAttribute& aAttr, IAlfMap* aData)
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
// gets the time from attribute safely.
// ---------------------------------------------------------------------------
//
int getTime(const AlfAttribute& aAttr, bool aImmediate = false)
    {
    int time = 0;
    if (!aImmediate)
        {
        time = aAttr.getTime();
        }

    return time;
    }

// ---------------------------------------------------------------------------
// sets source values from aAttr to aPoint.
// ---------------------------------------------------------------------------
//
void setSourceValue(TAlfTimedValue& aValue, const AlfAttribute& aAttr)
    {
    if (aAttr.getSourceValueCount() > 0)
        {
        aValue.SetValueNow(aAttr.getSourceValue()->realValue());
        }
    }
// ---------------------------------------------------------------------------
// sets source values to aPoint.
// ---------------------------------------------------------------------------
//
void SetSourceValues(TAlfTimedPoint& aPoint, const AlfAttribute& aAttrX,
                     const AlfAttribute& aAttrY)
    {
    if (aAttrX.getSourceValueCount() > 0)
        {
        aPoint.iX.SetValueNow(aAttrX.getSourceValue()->realValue());
        }
    if (aAttrY.getSourceValueCount() > 0)
        {
        aPoint.iY.SetValueNow(aAttrY.getSourceValue()->realValue());
        }
    }

// ---------------------------------------------------------------------------
// sets source values to aRect.
// ---------------------------------------------------------------------------
//
void SetSourceRect(TAlfRealRect& aRect, const AlfAttribute& aAttrTopX,
                   const AlfAttribute& aAttrTopY,
                   const AlfAttribute& aAttrBottomRightX,
                   const AlfAttribute& aAttrBottomRightY)
    {
    if (aAttrTopX.getSourceValueCount() > 0)
        {
        aRect.iTl.iX = aAttrTopX.getSourceValue()->realValue();
        }
    if (aAttrTopY.getSourceValueCount() > 0)
        {
        aRect.iTl.iY = aAttrTopY.getSourceValue()->realValue();
        }
    if (aAttrBottomRightX.getSourceValueCount() > 0)
        {
        aRect.iBr.iX = aAttrBottomRightX.getSourceValue()->realValue();
        }
    if (aAttrBottomRightY.getSourceValueCount() > 0)
        {
        aRect.iBr.iY = aAttrBottomRightY.getSourceValue()->realValue();
        }
    }

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Constructor.
// ---------------------------------------------------------------------------
//
OSN_EXPORT AlfCommonVisualAttributeSetter::AlfCommonVisualAttributeSetter()
    {
    }

// ---------------------------------------------------------------------------
// Destructor.
// ---------------------------------------------------------------------------
//
OSN_EXPORT AlfCommonVisualAttributeSetter::~AlfCommonVisualAttributeSetter()
    {
    }

// ---------------------------------------------------------------------------
// creates one command.
// deprecated
// ---------------------------------------------------------------------------
//
OSN_EXPORT TAlfCommand* AlfCommonVisualAttributeSetter::createCommand(
    CAlfVisual& /*aVisual*/, AlfAttributeContainer* /*aContainer*/,
    IAlfMap* /*aData*/, int /*aTransitionTime*/, CAlfVisual* /*aRefVisual*/ )
    {
    return 0;
    }

// ---------------------------------------------------------------------------
// creates and sends one or more commands.
// ---------------------------------------------------------------------------
//
OSN_EXPORT void AlfCommonVisualAttributeSetter::createAndSendCommands(
    CAlfVisual& aVisual,
    AlfAttributeContainer* aContainer,
    CAlfVisual* aRefVisual )
    {
    unsigned int attrCount = aContainer->attributeCount();

    for ( int i = 0; i < attrCount; ++i )
        {
        AlfAttribute& attr = aContainer->getAttribute(i);
        sendCommandsForAttribute(aVisual, *aContainer, attr, aRefVisual);
        }

    // Call the base class implementation also.
    // It goes through the same loop again and checks if there are attributes
    // that it handles.
    AlfCommonAttributeSetter::createAndSendCommands(
        aVisual,
        aContainer,
        aRefVisual );
    }

// ---------------------------------------------------------------------------
// Sets an value of a static attribute to target visual immediately
// without a transition.
// ---------------------------------------------------------------------------
//
void AlfCommonVisualAttributeSetter::handleStaticAttribute(
    CAlfVisual &aVisual, AlfAttribute& aAttr,
    AlfAttributeContainer& aContainer)
    {
    const char* attrName = aAttr.name();
    if ( !strcmp(attrName, KParentVisual))
        {
        }
    else if (!strcmp(attrName, KMaxHeight)||!strcmp(attrName, KMaxWidth))
        {
        setMaxSize(aVisual,aContainer,NULL);
        }
    else if (!strcmp(attrName, KMinHeight)||!strcmp(attrName, KMinWidth))
        {
        setMinSize(aVisual,aContainer,NULL);
        }
    else if (!strcmp(attrName, KPaddingTop)||!strcmp(attrName, KPaddingBottom)
             ||!strcmp(attrName, KPaddingLeft)||!strcmp(attrName, KPaddingRight))
        {
        setPadding(aVisual,aContainer,NULL);

        }
    else if (!strcmp(attrName, KSetFlags))
        {
        int val = aAttr.intValue();
        aVisual.SetFlags(val);
        }
    else if (!strcmp(attrName, KClearFlags))
        {
        int val = aAttr.intValue();
        aVisual.ClearFlags(val);
        }
    else if (!strcmp(attrName, KEventInput) || !strcmp(attrName, KFeedbackType))
        {
        setTactileFeedback(aVisual,aContainer,NULL);
        }
    // turnangle and secondaryalpha are implemented by derived attributesetters
    else
        {
        //check, if dynamic attribute, if so, set attributes immediately.
        if (!doHandleDynamicAttribute(aVisual, aAttr, aContainer, true))
            {
            //not handled here, try base class.
            AlfCommonAttributeSetter::handleStaticAttribute(aVisual,
                    aAttr, aContainer);
            }
        }
    }

void AlfCommonVisualAttributeSetter::setTactileFeedback(CAlfVisual &aVisual,AlfAttributeContainer& aContainer,IAlfMap* /*aData*/)
    {
    TRAPD( error, aVisual.SetTactileFeedbackL(
        aContainer.getAttributeByName( KEventInput ).intValue(),
        aContainer.getAttributeByName( KFeedbackType ).intValue() ) );

	if(error != KErrNone)
	    {
		ALF_THROW(AlfVisualException,EInvalidAttribute,"AlfCommonVisualAttributeSetter")
	    }
	try
    	{
        aContainer.getAttributeByName( duiuimodel::tactileattributes::KEventInput ).setDirty( false );
        aContainer.getAttributeByName( duiuimodel::tactileattributes::KFeedbackType ).setDirty( false );
    	}
    	
    catch( AlfDataException& exception )
	    {
		//exception needed here?
	    }
    }

// ---------------------------------------------------------------------------
// Sets an dynamic attribute value in the target visual
// using transitions defined in attributes
// ---------------------------------------------------------------------------
//
void AlfCommonVisualAttributeSetter::handleDynamicAttribute(
    CAlfVisual& aVisual, AlfAttribute& aAttr,
    AlfAttributeContainer& aContainer)
    {
    if (!doHandleDynamicAttribute(aVisual, aAttr, aContainer))
        {
        //not handled here, try base class.
        AlfCommonAttributeSetter::handleDynamicAttribute(aVisual, aAttr,
                aContainer);
        }
    }

// ---------------------------------------------------------------------------
// Sets an value for static attribute in the target visual immediately
// without a transition using data in map.
// ---------------------------------------------------------------------------
//
void AlfCommonVisualAttributeSetter::handleStaticDataAttribute(
    CAlfVisual &aVisual, AlfAttribute& aAttr,
    AlfAttributeContainer& aContainer, IAlfMap* aData)
    {

    if (aData)
        {
        const char* attrName = aAttr.name();

        if (!strcmp(attrName, KMaxHeight))
            {
            // If maxheight exists, maxwidth must exists also.
            const IAlfVariantType* maxHeight = getData(aAttr, aData);
            const IAlfVariantType* maxWidth = getData(
                                                  aContainer.getAttributeByName(KMaxWidth), aData);

            if (maxHeight && maxHeight->type() == IAlfVariantType::EReal &&
                    maxWidth && maxWidth->type() == IAlfVariantType::EReal)
                {
                TAlfRealSize size( maxWidth->real(), maxHeight->real() );
                aVisual.SetMaxSize(size);
                }
            }
        else if (!strcmp(attrName, KMinHeight))
            {
            //If minheight exists, minwidth must exists also.
            const IAlfVariantType* minHeight = getData(aAttr, aData);
            const IAlfVariantType* minWidth = getData(
                                                  aContainer.getAttributeByName(KMinWidth), aData);

            if (minHeight && minHeight->type() == IAlfVariantType::EReal &&
                    minWidth && minWidth->type() == IAlfVariantType::EReal)
                {
                TAlfRealSize size( minWidth->real(), minHeight->real() );
                aVisual.SetMinSize(size);
                }
            }
        else if (!strcmp(attrName, KPaddingTop))
            {
            const IAlfVariantType* top = getData(aAttr, aData);
            const IAlfVariantType* right = getData(
                                               aContainer.getAttributeByName(KPaddingRight), aData);
            const IAlfVariantType* bottom = getData(
                                                aContainer.getAttributeByName(KPaddingBottom), aData);
            const IAlfVariantType* left = getData(
                                              aContainer.getAttributeByName(KPaddingLeft), aData);

            if (top && top->type() == IAlfVariantType::EReal &&
                    right && right->type() == IAlfVariantType::EReal &&
                    bottom && bottom->type() == IAlfVariantType::EReal &&
                    left && left->type() == IAlfVariantType::EReal)
                {
                float l = left->real();
                float r = right->real();
                float t = top->real();
                float b = bottom->real();
                TAlfBoxMetric metric(l, r, t, b);
                aVisual.SetPadding(metric);
                }
            }
        else if (!strcmp(attrName, KSetFlags))
            {
            const IAlfVariantType* flag = getData(aAttr, aData);
            if (flag && flag->type() == IAlfVariantType::EInt)
                {
                int flagValue  = flag->integer();
                aVisual.SetFlags(flagValue);
                }
            }
        else if (!strcmp(attrName, KClearFlags))
            {
            const IAlfVariantType* flag = getData(aAttr, aData);
            if (flag && flag->type() == IAlfVariantType::EInt)
                {
                int flagValue  = flag->integer();
                aVisual.ClearFlags(flagValue);
                }
            }
        // turnangle and secondaryalpha are implemented by derived attributesetters
        else if (strcmp(attrName, KMaxWidth) &&         //these are already
                 strcmp(attrName, KMinWidth) &&         //handled above
                 strcmp(attrName, KPaddingRight) &&
                 strcmp(attrName, KPaddingBottom) &&
                 strcmp(attrName, KPaddingLeft))
            {
            //check, if dynamic attribute, if so, set attributes immediately.
            //If not, the function will call baseclass SetAttributeValue.
            if (!doHandleDynamicDataAttribute(aVisual, aAttr,
                                              aContainer, aData, true))
                {
                //not handled here, try base class.
                AlfCommonAttributeSetter::handleStaticDataAttribute(
                    aVisual, aAttr, aContainer, aData);
                }
            }
        }
    }

// ---------------------------------------------------------------------------
// From AlfCommonAttributeSetter
//
// Sets a value for dynamic data attribute value in the target visual
// using transitions and data in map.
// ---------------------------------------------------------------------------
//
void AlfCommonVisualAttributeSetter::handleDynamicDataAttribute(
    CAlfVisual& aVisual, AlfAttribute& aAttr,
    AlfAttributeContainer& aContainer, IAlfMap* aData)
    {
    if (!doHandleDynamicDataAttribute(aVisual, aAttr, aContainer, aData))
        {
        //not handled here, try base class.
        AlfCommonAttributeSetter::handleDynamicDataAttribute(aVisual, aAttr,
                aContainer, aData);
        }
    }

// ---------------------------------------------------------------------------
// Sets an dynamic attribute value in the target visual
// using transitions defined in attributes
// Handles only attributes, belonging to this attributesetter, not bases.
// ---------------------------------------------------------------------------
//
bool AlfCommonVisualAttributeSetter::doHandleDynamicAttribute(
    CAlfVisual& aVisual, AlfAttribute& aAttr,
    AlfAttributeContainer& aContainer, bool aImmediate)
    {
    const char* attrName = aAttr.name();
    bool handled = true;

    if ( !strcmp( attrName, KOpacity ) )
        {
        TAlfTimedValue v;
        setSourceValue(v, aAttr);

        float targetValue(aAttr.realValue());
        int time = getTime(aAttr, aImmediate);
        v.SetTarget(targetValue, time);
        v.SetStyle(aAttr.getInterpolationStyle());
        aVisual.SetOpacity(v);
        }

    else if (!strcmp( attrName, KPositionX)||!strcmp( attrName, KPositionY)) ////also handles: KPositionY
        {
        setPos(aVisual,aContainer,NULL,aImmediate);
        }

    else if (!strcmp( attrName, KWidth)||!strcmp( attrName, KHeight)) //also handles: KHeight
        {
        setSize(aVisual,aContainer,NULL,aImmediate);
        }
    else if (!strcmp(attrName, KTopLeftX)||!strcmp(attrName, KTopLeftY)
             ||!strcmp(attrName, KBottomRightX)||!strcmp(attrName, KBottomRightY))
        {
        setRect(aVisual,aContainer,NULL,aImmediate);
        }
    else
        {
        handled = false;
        }

    return handled;
    }

// ---------------------------------------------------------------------------
// Sets an value for dynamic attribute value in the target visual
// using transitions and data in map.
// ---------------------------------------------------------------------------
//
bool AlfCommonVisualAttributeSetter::doHandleDynamicDataAttribute(
    CAlfVisual& aVisual, AlfAttribute& aAttr,
    AlfAttributeContainer& aContainer, IAlfMap* aData, bool aImmediate )
    {
    bool handled = true;
    if (aData)
        {
        const char* attrName = aAttr.name();

        if ( !strcmp( attrName, KOpacity ) )
            {
            const IAlfVariantType* data = getData(aAttr, aData);
            if (data && data->type() == IAlfVariantType::EReal)
                {
                TAlfTimedValue v;
                setSourceValue(v, aAttr);

                float targetValue(data->real());
                int time = getTime(aAttr, aImmediate);
                v.SetTarget(targetValue, time);
                v.SetStyle(aAttr.getInterpolationStyle());
                aVisual.SetOpacity(v);
                }
            }
        else if (!strcmp( attrName, KPositionX)) //also handles: KPositionY
            {
            //If positionx exists, positiony must exists also
            const AlfAttribute& attrX = aAttr;
            const AlfAttribute& attrY = aContainer.getAttributeByName(KPositionY);

            const IAlfVariantType* posX = getData(attrX, aData);
            const IAlfVariantType* posY = getData(attrY, aData);

            if (posX && posX->type() == IAlfVariantType::EReal &&
                    posY && posY->type() == IAlfVariantType::EReal)
                {
                TAlfTimedPoint pos;
                SetSourceValues(pos, attrX, attrY);

                pos.iX.SetStyle(attrX.getInterpolationStyle());
                pos.iX.SetTarget(posX->real(), getTime(attrX, aImmediate));
                pos.iY.SetStyle(attrY.getInterpolationStyle());
                pos.iY.SetTarget(posY->real(), getTime(attrY, aImmediate));
                aVisual.SetPos(pos);
                }
            }
        else if (!strcmp( attrName, KWidth)) //also handles: KHeight
            {
            const AlfAttribute& attrWidth = aAttr;
            const AlfAttribute& attrHeight =
                aContainer.getAttributeByName(KHeight);

            const IAlfVariantType* width = getData(attrWidth, aData);
            const IAlfVariantType* height = getData(attrHeight, aData);

            if (width && width->type() == IAlfVariantType::EReal &&
                    height && height->type() == IAlfVariantType::EReal)
                {
                TAlfTimedPoint size;
                SetSourceValues(size, attrWidth, attrHeight);

                size.iX.SetStyle(attrWidth.getInterpolationStyle());
                size.iX.SetTarget(width->real(),
                                  getTime(attrWidth, aImmediate));
                size.iY.SetStyle(attrHeight.getInterpolationStyle());
                size.iY.SetTarget(height->real(),
                                  getTime(attrHeight, aImmediate));

                aVisual.SetSize(size);
                }
            }
        else if (!strcmp(attrName, KTopLeftX))
            {
            //If topleftx exists, topy, bottomx and bottomy must exists also
            const AlfAttribute& attrTopX = aAttr;
            const AlfAttribute& attrTopY =
                aContainer.getAttributeByName(KTopLeftY);
            const AlfAttribute& attrBottomRightX =
                aContainer.getAttributeByName(KBottomRightX);
            const AlfAttribute& attrBottomRightY =
                aContainer.getAttributeByName(KBottomRightY);

            const IAlfVariantType* topX = getData(attrTopX, aData);
            const IAlfVariantType* topY = getData(attrTopY, aData);
            const IAlfVariantType* bottomX = getData(attrBottomRightX, aData);
            const IAlfVariantType* bottomY = getData(attrBottomRightY, aData);

            if (topX && topX->type() == IAlfVariantType::EReal &&
                    topY && topY->type() == IAlfVariantType::EReal &&
                    bottomX && bottomX->type() == IAlfVariantType::EReal &&
                    bottomY && bottomY->type() == IAlfVariantType::EReal)
                {
                const TAlfRealPoint sTopLeft(aVisual.Pos().ValueNow());
                TAlfRealPoint size(aVisual.Size().ValueNow());
                TAlfRealRect sRect(sTopLeft, TAlfRealPoint(
                                       sTopLeft.iX + size.iX, sTopLeft.iY + size.iY));

                SetSourceRect(sRect, attrTopX, attrTopY, attrBottomRightX,
                              attrBottomRightY);

                aVisual.SetRect(sRect, 0);

                TAlfRealPoint topLeft(topX->real(), topY->real());
                TAlfRealPoint bottomRight(bottomX->real(), bottomY->real());
                TAlfRealRect realRect(topLeft, bottomRight);
                aVisual.SetRect(realRect, getTime(aAttr, aImmediate));
                }
            }
        else if (strcmp(attrName, KPositionY) &&      //these are already
                 strcmp(attrName, KHeight) &&         //handled above
                 strcmp(attrName, KTopLeftY) &&
                 strcmp(attrName, KBottomRightX) &&
                 strcmp(attrName, KBottomRightY))
            {
            handled = false;
            }
        }

    return handled;
    }

// ---------------------------------------------------------------------------
// Creates and returns one command
// ---------------------------------------------------------------------------
//
void AlfCommonVisualAttributeSetter::sendCommandsForAttribute(
    CAlfVisual& aVisual, AlfAttributeContainer& aContainer,
    AlfAttribute& aAttr, CAlfVisual* aRefVisual)
    {
    const char* attrName = aAttr.name();

    if ( !strcmp( attrName, KOpacity ) )
        {
        float value(aAttr.realValue());

        if (aRefVisual)
            {
            // Add the opacity of the reference visual to the value
            // of the opacity provided in the attribute 
            value += aRefVisual->Opacity().ValueNow(); 
            }

        int time = getTime(aAttr);

        //setting interpolation style
        TAlfInterpolationStyle style = aAttr.getInterpolationStyle();
        TAlfTimedValue v;
        v.SetStyle(style);
        aVisual.SetOpacity(v);

        //create command
        TAlfValueCommand cmd(&aVisual, EAlfValueCommandVisualSetOpacity,
                             EAlfOpSet, value, time);

        //send with delay.
        CAlfEnv::Static()->Send(cmd, aAttr.getDelay());
        }

    else if (!strcmp( attrName, KPositionX)) ////also handles: KPositionY
        {
        // Must contain both KPositionX and KPositionY
        const AlfAttribute& posXattrib = aAttr;
        const AlfAttribute& posYattrib =
            aContainer.getAttributeByName(KPositionY);

        TAlfRealPoint value(posXattrib.realValue(),
                            posYattrib.realValue());

        if ( aRefVisual )
            {
            value += aRefVisual->Pos().ValueNow(); // Add the reference point
            }

        int time = getTime(aAttr);

        //create command
        TAlfPointCommand cmd(&aVisual, EAlfPointCommandVisualSetPos,
                             value, time);

        //send with delay.
        CAlfEnv::Static()->Send(cmd, aAttr.getDelay());
        }

    else if (!strcmp( attrName, KWidth)) //also handles: KHeight
        {
        // Must contain both KWidth and KHeight
        const AlfAttribute& widthAttrib = aAttr;
        const AlfAttribute& heightAttrib =
            aContainer.getAttributeByName(KHeight);

        TAlfRealPoint value(widthAttrib.realValue(),
                            heightAttrib.realValue());

        if (aRefVisual)
            {
            value += aRefVisual->Size().ValueNow(); // Add the reference point
            }
        int time = getTime(aAttr);

        //create command
        TAlfPointCommand cmd(&aVisual,
                             EAlfPointCommandVisualSetSize, value, time);

        //send with delay.
        CAlfEnv::Static()->Send(cmd, aAttr.getDelay());
        }
    else if (!strcmp(attrName, KTopLeftX))
        {
        //If topleftx exists, topy, bottomx and bottomy must exists also
        float topX = aAttr.realValue();
        const AlfAttribute& attrTopY =
            aContainer.getAttributeByName(KTopLeftY);
        const AlfAttribute& attrBottomX =
            aContainer.getAttributeByName(KBottomRightX);
        const AlfAttribute& attrBottomY =
            aContainer.getAttributeByName(KBottomRightY);
        TAlfRealPoint topLeft(topX, attrTopY.realValue());
        TAlfRealPoint size(attrBottomX.realValue() - topLeft.iX,
                           attrBottomY.realValue() - topLeft.iY);
        if (aRefVisual)
            {
            // Add the reference point
            topLeft += aRefVisual->Pos().ValueNow();
            size += aRefVisual->Size().ValueNow();
            }

        int posTime = getTime(aAttr);
        int sizeTime = getTime(attrBottomX);

        //create position command
        TAlfPointCommand posCmd(&aVisual, EAlfPointCommandVisualSetPos,
                                topLeft, posTime);
        TAlfPointCommand sizeCmd(&aVisual, EAlfPointCommandVisualSetPos,
                                 size, sizeTime);

        //send with delay.
        CAlfEnv::Static()->Send(posCmd, aAttr.getDelay());
        CAlfEnv::Static()->Send(sizeCmd, attrBottomX.getDelay());
        }
    }

    
void AlfCommonVisualAttributeSetter::setPadding(CAlfVisual &aVisual,AlfAttributeContainer& aContainer,IAlfMap* /*aData*/)
    {
    //If padding-top exists, right, bottom and left must exists also

    AlfAttribute& attrTop =
        aContainer.getAttributeByName(KPaddingTop);
    AlfAttribute& attrRight =
        aContainer.getAttributeByName(KPaddingRight);
    AlfAttribute& attrBottom =
        aContainer.getAttributeByName(KPaddingBottom);
    AlfAttribute& attrLeft =
        aContainer.getAttributeByName(KPaddingLeft);

    TAlfBoxMetric metric(
        TAlfMetric(attrLeft.realValue(), attrLeft.unit()),
        TAlfMetric(attrRight.realValue(), attrRight.unit()),
        TAlfMetric(attrTop.realValue(), attrTop.unit()),
        TAlfMetric(attrBottom.realValue(), attrBottom.unit()));

    aVisual.SetPadding(metric);

    attrTop.setDirty(false);
    attrRight.setDirty(false);
    attrBottom.setDirty(false);
    attrLeft.setDirty(false);

    }

void AlfCommonVisualAttributeSetter::setMinSize(CAlfVisual &aVisual,AlfAttributeContainer& aContainer,IAlfMap* /*aData*/)
    {
    //If minheight exists, minwidth must exists also.
    AlfAttribute& attrHt =
        aContainer.getAttributeByName(KMinHeight);
    AlfAttribute& attrWidth =
        aContainer.getAttributeByName(KMinWidth);
    float height = attrHt.realValue();
    float width = attrWidth.realValue();
    TAlfRealSize size( width, height );

    aVisual.SetMinSize(size);

    attrHt.setDirty(false);
    attrWidth.setDirty(false);
    }

void AlfCommonVisualAttributeSetter::setMaxSize(CAlfVisual &aVisual,AlfAttributeContainer& aContainer,IAlfMap* /*aData*/)
    {
    AlfAttribute& attrHt =
        aContainer.getAttributeByName(KMaxHeight);
    AlfAttribute& attrWidth =
        aContainer.getAttributeByName(KMaxWidth);
    float height = attrHt.realValue();
    float width = attrWidth.realValue();
    TAlfRealSize size( width, height );

    aVisual.SetMaxSize(size);


    attrHt.setDirty(false);
    attrWidth.setDirty(false);


    }

void AlfCommonVisualAttributeSetter::setSize(CAlfVisual &aVisual,AlfAttributeContainer& aContainer,IAlfMap* /*aData*/,bool aImmediate)
    {
    // Must contain both KWidth and KHeight
    AlfAttribute& attrWidth =
        aContainer.getAttributeByName(KWidth);;
    AlfAttribute& attrHeight =
        aContainer.getAttributeByName(KHeight);

    TAlfTimedPoint size(aVisual.Size());
    SetSourceValues(size, attrWidth, attrHeight);

    size.iX.SetStyle(attrWidth.getInterpolationStyle());
    size.iX.SetTarget(attrWidth.realValue(),
                      getTime(attrWidth, aImmediate));
    size.iY.SetStyle(attrHeight.getInterpolationStyle());
    size.iY.SetTarget(attrHeight.realValue(),
                      getTime(attrHeight, aImmediate));


    aVisual.SetSize(size);


    attrWidth.setDirty(false);
    attrHeight.setDirty(false);
    }

void AlfCommonVisualAttributeSetter::setPos(CAlfVisual &aVisual,AlfAttributeContainer& aContainer,IAlfMap* /*aData*/,bool aImmediate)
    {
    // Must contain both KPositionX and KPositionY
    AlfAttribute& attrX = aContainer.getAttributeByName(KPositionX);
    AlfAttribute& attrY = aContainer.getAttributeByName(KPositionY);

    TAlfTimedPoint pos(aVisual.Pos());
    SetSourceValues(pos, attrX, attrY);

    pos.iX.SetStyle(attrX.getInterpolationStyle());
    pos.iX.SetTarget(attrX.realValue(), getTime(attrX, aImmediate));
    pos.iY.SetStyle(attrY.getInterpolationStyle());
    pos.iY.SetTarget(attrY.realValue(), getTime(attrY, aImmediate));

    aVisual.SetPos(pos);


    attrX.setDirty(false);
    attrY.setDirty(false);
    }

void AlfCommonVisualAttributeSetter::setRect(CAlfVisual &aVisual,AlfAttributeContainer& aContainer,IAlfMap* /*aData*/,bool aImmediate)
    {
    //If topleftx exists, topy, bottomx and bottomy must exists also
    AlfAttribute& attrTopX =
        aContainer.getAttributeByName(KTopLeftX);
    AlfAttribute& attrTopY =
        aContainer.getAttributeByName(KTopLeftY);
    AlfAttribute& attrBottomRightX =
        aContainer.getAttributeByName(KBottomRightX);
    AlfAttribute& attrBottomRightY =
        aContainer.getAttributeByName(KBottomRightY);

    const TAlfRealPoint sTopLeft(aVisual.Pos().ValueNow());
    TAlfRealPoint size(aVisual.Size().ValueNow());
    TAlfRealRect sRect(sTopLeft, TAlfRealPoint(sTopLeft.iX + size.iX,
                       sTopLeft.iY + size.iY));

    SetSourceRect(sRect, attrTopX, attrTopY, attrBottomRightX,
                  attrBottomRightY);

    aVisual.SetRect(sRect, 0);

    TAlfRealPoint tTopLeft(attrTopX.realValue(), attrTopY.realValue());
    TAlfRealPoint tBottomRight(attrBottomRightX.realValue(),
                               attrBottomRightY.realValue());
    TAlfRealRect tRealRect(tTopLeft, tBottomRight);

    int time = getTime(attrTopX, aImmediate);
    aVisual.SetRect(tRealRect, time);


    attrTopX.setDirty(false);
    attrTopY.setDirty(false);
    attrBottomRightX.setDirty(false);
    attrBottomRightY.setDirty(false);

    }
} // Alf
