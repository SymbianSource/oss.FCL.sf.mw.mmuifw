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
* Description:   Implements attributesetters for linevisual.
*
*/


//includes

//widget model includes
#include "alf/alfattributecontainer.h"
#include "alf/alfattributevaluetype.h"
#include "alf/alfattribute.h"
#include "alf/alflinevisualattributesetter.h"
#include <alf/alfvisualexception.h>
#include <alf/alfdataexception.h>
#include <alf/alfattributeexception.h>

//osn includes
#include <osn/ustring.h>
#include <osn/alfptrvector.h>

//alf includes
#include <alf/alfenv.h>
#include <alf/alflinevisual.h>
#include <alf/alftexture.h>
#include <alf/alfcurvepath.h>

//dui includes
#include "alf/attrproperty.h"

//other includes
#include <libc/string.h>
#include <utf.h>



//namespaces

using namespace osncore;

using namespace duiuimodel::commonvisualattributes;
using namespace duiuimodel::linevisualattributes;
using namespace duiuimodel::curvepathattributes;

namespace Alf
    {

static void throwIfErr ( int aErr )
    {
    if (aErr!=KErrNone)
        {
        ALF_THROW ( AlfAttributeException, aErr,
                    "AlfLineVisualAttributeSetter")
        }
    }


class AlfLineVisualAttributeSetterImpl
    {
public:
    AlfLineVisualAttributeSetterImpl();
    ~AlfLineVisualAttributeSetterImpl();
public:

    // Texture manager doesn't unload the texture from memory untill Env
    // is deleted. Hence need to unload them once attribute setter is deleted.
    AlfPtrVector<CAlfTexture> mLoadedTextures;
    // Env needed to access TextureManager while unloading textures.
    CAlfEnv* mEnv;
    };

AlfLineVisualAttributeSetterImpl::AlfLineVisualAttributeSetterImpl()
    {
    // Do not delete textures here as they may be in use by the visuals
    // that have not been deleted.
    mLoadedTextures.setAutoDelete(false);
    }

AlfLineVisualAttributeSetterImpl::~AlfLineVisualAttributeSetterImpl()
    {
    // Unload all loaded textures created by this attributesetter to free
    // up the memory.
    for (int i =0;i<mLoadedTextures.count();i++)
        {
        CAlfTexture* Texture = mLoadedTextures[i];
        mEnv->TextureManager().UnloadTexture(Texture->Id());
        }
    mLoadedTextures.clear();
    }


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Constructor.
// ---------------------------------------------------------------------------
//
OSN_EXPORT AlfLineVisualAttributeSetter::AlfLineVisualAttributeSetter()
    {
    mImpl.reset(new (EMM) AlfLineVisualAttributeSetterImpl());
    }

// ---------------------------------------------------------------------------
// Destructor.
// ---------------------------------------------------------------------------
//
OSN_EXPORT AlfLineVisualAttributeSetter::~AlfLineVisualAttributeSetter()
    {
    }


// ---------------------------------------------------------------------------
// Sets Attribute Value. Delegates based on attribute Category.
// ---------------------------------------------------------------------------
//
OSN_EXPORT void AlfLineVisualAttributeSetter::setAttributeValue (
    CAlfVisual &aVisual,
    AlfAttributeContainer* aContainer,
    IAlfMap* aData )
    {
    CAlfLineVisual* lineVisual = dynamic_cast<CAlfLineVisual*>( &aVisual);

    if ( !lineVisual )
        {
        ALF_THROW ( AlfVisualException, EInvalidVisual, "AlfLineVisualAttributeSetter" )
        }

    CAlfCurvePath* curve = lineVisual->Path();
    if (curve)
        {
        curve->Reset();
        }

    AlfCommonVisualAttributeSetter::setAttributeValue(aVisual,
            aContainer, aData);
    }

// ---------------------------------------------------------------------------
// Deprecated
// ---------------------------------------------------------------------------
//
OSN_EXPORT TAlfCommand* AlfLineVisualAttributeSetter::createCommand (
    CAlfVisual& /*aVisual*/,
    AlfAttributeContainer* /*aContainer*/,
    IAlfMap* /*aData*/,
    int /*aTransitionTime*/,
    CAlfVisual* /*aRefVisual*/ )
    {
    //deprecated
    TAlfCommand* cmd = 0;
    return cmd;
    }

// ---------------------------------------------------------------------------
// Sends a command to Env
// ---------------------------------------------------------------------------
//
OSN_EXPORT void AlfLineVisualAttributeSetter::createAndSendCommands (
    CAlfVisual& aVisual,
    AlfAttributeContainer* aContainer,
    CAlfVisual* aRefVisual )
    {
    // SetOffset in CAlfCurvePath,
    // SetThickness,SetShadowThickness SetStartPos & SetEndPos
    // in Line Visual can be sent as a TAlfCustomEventCommand
    AlfCommonVisualAttributeSetter::createAndSendCommands (
        aVisual, aContainer, aRefVisual );

    }


// ---------------------------------------------------------------------------
// Sets dynamic attributes to visual
// ---------------------------------------------------------------------------
//
void AlfLineVisualAttributeSetter::handleDynamicAttribute (
    CAlfVisual &aVisual,
    AlfAttribute& aAttr,
    AlfAttributeContainer& aContainer)
    {

    CAlfLineVisual* lineVisual = dynamic_cast<CAlfLineVisual*>( &aVisual);
    if ( !lineVisual )
        {
        return;
        }  
    const char* attrName = aAttr.name();

    if ( !strcmp ( attrName, KThickness ) )
        {
        TAlfTimedValue tVal((TReal32)aAttr.getSourceValue()->realValue());
        tVal.SetTarget ((TReal32)aAttr.getTargetValue()->realValue(),
                        (TInt) aAttr.getTime());
        tVal.SetStyle ( aAttr.getInterpolationStyle() );
        tVal.SetMappingFunctionIdentifier (aAttr.getMappingFunctionId());

        lineVisual->SetThickness(tVal);
        }
        
    else if ( !strcmp ( attrName, KShadowThickness ) )
        {
        TAlfTimedValue tVal((TReal32)aAttr.getSourceValue()->realValue());
        tVal.SetTarget ((TReal32)aAttr.getTargetValue()->realValue(),
                        (TInt) aAttr.getTime());
        tVal.SetStyle ( aAttr.getInterpolationStyle() );
        tVal.SetMappingFunctionIdentifier (aAttr.getMappingFunctionId());

        lineVisual->SetShadowThickness(tVal);
        }

    else if ( !strcmp ( attrName, KStartPos ) )
        {
        TAlfTimedValue tVal((TReal32)aAttr.getSourceValue()->realValue());
        tVal.SetTarget ((TReal32)aAttr.getTargetValue()->realValue(),
                        (TInt) aAttr.getTime());
        tVal.SetStyle ( aAttr.getInterpolationStyle() );
        tVal.SetMappingFunctionIdentifier (aAttr.getMappingFunctionId());

        lineVisual->SetStartPos(tVal);
        }

    else if ( !strcmp ( attrName, KEndPos ) )
        {
        TAlfTimedValue tVal((TReal32)aAttr.getSourceValue()->realValue());
        tVal.SetTarget ((TReal32)aAttr.getTargetValue()->realValue(),
                        (TInt) aAttr.getTime());
        tVal.SetStyle ( aAttr.getInterpolationStyle() );
        tVal.SetMappingFunctionIdentifier (aAttr.getMappingFunctionId());

        lineVisual->SetEndPos(tVal);
        }

    else if ( !strcmp ( attrName, KOffsetX )||!strcmp ( attrName, KOffsetY ) )
        {
        AlfAttribute& offsety = aContainer.getAttributeByName (
                                    KOffsetY );
        AlfAttribute& offsetx = aContainer.getAttributeByName (
                                    KOffsetX );
        //will throw if attribute is not found

        TAlfTimedPoint offset((TReal32)offsetx.getSourceValue()->realValue(),
                              (TReal32)offsety.getSourceValue()->realValue());

        offset.iX.SetTarget((TReal32)offsetx.getTargetValue()->realValue(),
                            offsetx.getTime());

        offset.iY.SetTarget((TReal32)offsety.getTargetValue()->realValue(),
                            offsety.getTime());

        offset.iX.SetStyle ( offsetx.getInterpolationStyle() );
        offset.iY.SetStyle ( offsety.getInterpolationStyle() );

        offset.iX.SetMappingFunctionIdentifier (
            offsetx.getMappingFunctionId());
        offset.iY.SetMappingFunctionIdentifier (
            offsety.getMappingFunctionId());

        CAlfCurvePath* curve = this->getCurvePath(lineVisual);
        curve->SetOffset ( offset );
        offsetx.setDirty(false);
        offsety.setDirty(false);
        }
        
    else
        {
        AlfCommonVisualAttributeSetter::handleDynamicAttribute(
            aVisual, aAttr, aContainer);
        }
    }

// ---------------------------------------------------------------------------
// Sets static attributes to visual
// ---------------------------------------------------------------------------
//
void AlfLineVisualAttributeSetter::handleStaticAttribute (
    CAlfVisual& aVisual,
    AlfAttribute& aAttr,
    AlfAttributeContainer& aContainer)
    {

    CAlfLineVisual* lineVisual = dynamic_cast<CAlfLineVisual*>( &aVisual);
    if ( !lineVisual )
        {
        return;
        }  
    
    const char* attrName = aAttr.name();

    if ( !strcmp ( attrName, KImagePath ) )
        {
        TPtrC8 src;
        src.Set((TUint8*)aAttr.stringValue().getUtf8());
        auto_ptr<HBufC> DesC;
        TRAPD(err1,
            DesC.reset(CnvUtfConverter::ConvertToUnicodeFromUtf8L(src)));
        throwIfErr(err1);

        if (DesC.get())
            {
            CAlfTexture* texture = NULL;
            TRAPD(err2,texture = &(aVisual.Env().TextureManager().
                                   LoadTextureL( *(DesC.get()),
                                       EAlfTextureFlagDefault,
                                       KAlfAutoGeneratedTextureId)));
            throwIfErr(err2);
            lineVisual->SetImage(TAlfImage(*texture));
            mImpl->mEnv = &aVisual.Env();
            mImpl->mLoadedTextures.resize(mImpl->mLoadedTextures.count()+1);
            mImpl->mLoadedTextures.insert(mImpl->mLoadedTextures.count(),
                                       texture);
            }
        }

    else if ( !strcmp ( attrName, KThickness ) )
        {
        TAlfTimedValue val(aAttr.realValue(),0);
        //static attribute,apply with immidiate effect

        lineVisual->SetThickness(val);
        }

    else if ( !strcmp ( attrName, KShadowThickness ) )
        {
        TAlfTimedValue val(aAttr.realValue(),0);
        //static attribute,apply with immidiate effect

        lineVisual->SetShadowThickness(val);
        }

    else if ( !strcmp ( attrName, KStartPos ) )
        {
        TAlfTimedValue val(aAttr.realValue(),0);
        //static attribute,apply with immidiate effect

        lineVisual->SetStartPos(val);
        }

    else if ( !strcmp ( attrName, KEndPos ) )
        {
        TAlfTimedValue val(aAttr.realValue(),0);
        //static attribute,apply with immidiate effect

        lineVisual->SetEndPos(val);
        }

    else if ( !strcmp ( attrName, KAlphaMappingFunction ) )
        {
        // assumption:
        // The int value-mapping function mapping is the following:
        // 0 - KConstMappingFunction
        // 1 - duiuimodel::nodetypes::KLinearMappingFunction
        // 2 - KSineMappingFunction
        // 3 - KCosineMappingFunction

        MAlfMappingFunction* func =  this->getMappingFunction(aAttr,
                                               lineVisual->Env());
        if (!func)
            {
            ALF_THROW ( AlfAttributeException,
                        EInvalidAttribute, "AlfLineVisualAttributeSetter")
            }

        lineVisual->SetAlphaFunction(func);
        }

    else if ( !strcmp ( attrName, KWidthMappingFunction ) )
        {
        //assumption:
        //The int value-mapping function mapping is the following:
        //0 - KConstMappingFunction
        //1 - duiuimodel::nodetypes::KLinearMappingFunction
        //2 - KSineMappingFunction
        //3 - KCosineMappingFunction

        MAlfMappingFunction* func =  this->getMappingFunction(aAttr,
                                     lineVisual->Env());
        if (!func)
            {
            ALF_THROW ( AlfAttributeException,
                        EInvalidAttribute, "AlfLineVisualAttributeSetter")
            }
        lineVisual->SetWidthFunction(func);
        }

    else if ( !strcmp ( attrName, KColor ) )
        {
        if ( 4 == aAttr.getTargetValueCount() &&
                AlfAttributeValueType::EFloat == aAttr.type(0) &&
                AlfAttributeValueType::EFloat == aAttr.type(1) &&
                AlfAttributeValueType::EFloat == aAttr.type(2) &&
                AlfAttributeValueType::EFloat == aAttr.type(3))
            {
            TRgb color ( aAttr.realValue(0),
                         aAttr.realValue(1),
                         aAttr.realValue(2),
                         aAttr.realValue(3) );
            lineVisual->SetColor ( color );
            }
        else
            {
            ALF_THROW ( AlfAttributeException, EInvalidAttribute,
                        "AlfLineVisualAttributeSetter")
            }
        }

    else if ( !strcmp ( attrName, KEnableLoop ) )
        {

        CAlfCurvePath* curve = this->getCurvePath(lineVisual);

        // if value is not int, then use default parameter of EnableLoop
        if ( AlfAttributeValueType::EInt != aAttr.type() )
            {
            curve->EnableLoop ();
            }
        else
            {
            int enableLoop = aAttr.intValue();

            TBool flag = ETrue;
            //Assume true for any non-zero value.

            if ( 0 == enableLoop )
                {
                flag = EFalse;
                }
            curve->EnableLoop ( flag );
            }
        }

    else if ( !strcmp ( attrName, KOrigin ) )
        {
        CAlfCurvePath* curve = this->getCurvePath(lineVisual);
        curve->SetOrigin ( aAttr.realValue() );
        }

    else if ( !strcmp ( attrName, KOffsetX )||!strcmp ( attrName, KOffsetY ) )
        {
        CAlfCurvePath* curve = this->getCurvePath(lineVisual);
        AlfAttribute& offsety = aContainer.getAttributeByName (
                                               KOffsetY );
        AlfAttribute& offsetx = aContainer.getAttributeByName (
                                               KOffsetX );
        //will throw if attribute is not found

        TAlfTimedPoint offset( offsetx.realValue(), offsety.realValue());
        curve->SetOffset ( offset );
        offsetx.setDirty(false);
        offsety.setDirty(false);
        }
        
    else if ( !strcmp ( attrName, KLine ) )
        {
        CAlfCurvePath* curve = this->getCurvePath(lineVisual);

        // This multi-value attribute has to be filled in this order:
        // startx,starty,endx,endy,linelength

        if ( 5 != aAttr.getTargetValueCount())
            {
            ALF_THROW ( AlfAttributeException, EInvalidAttribute,
                        "CAlfLineVisualLayoutAttributeSetter" );
            }
        float startx = this->floatOrInt( aAttr, 0 );
        float starty = this->floatOrInt( aAttr, 1 );
        float endx = this->floatOrInt( aAttr, 2 );
        float endy = this->floatOrInt( aAttr, 3 );
        float len = aAttr.realValue(4);

        TRAPD(err1,
              curve->AppendLineL ( TAlfRealPoint(startx,starty),
                                   TAlfRealPoint(endx,endy),len ))
        throwIfErr(err1);
        }
        
    else if ( !strcmp ( attrName, KArc ) )
        {
        CAlfCurvePath* curve = this->getCurvePath(lineVisual);

        // This multi-value attribute has to be filled in this order:
        // arcoriginx,arcoriginy,archorzradius,arcvertradius,
        // arcstartangle,arcendangle,arclength

        if ( 7 != aAttr.getTargetValueCount())
            {
            ALF_THROW ( AlfAttributeException, EInvalidAttribute,
                        "CAlfLineVisualLayoutAttributeSetter" );
            }

        float originx = this->floatOrInt( aAttr, 0 );
        float originy = this->floatOrInt( aAttr, 1 );
        float horzradius = this->floatOrInt( aAttr, 2 );
        float vertradius = this->floatOrInt( aAttr, 3 );
        float startangle = aAttr.realValue(4);
        float endangle = aAttr.realValue(5);
        float len = aAttr.realValue(6);
        TRAPD(err1,
              curve->AppendArcL(TAlfRealPoint(originx,originy),
                                TAlfRealSize(horzradius,vertradius),
                                startangle, endangle, len))
        throwIfErr(err1);
        }
        
    else
        {
        AlfCommonVisualAttributeSetter::handleStaticAttribute (
            aVisual, aAttr, aContainer);
        }
    }



// ---------------------------------------------------------------------------
// Sets dynamic attributes to visual from data
// ---------------------------------------------------------------------------
//
void AlfLineVisualAttributeSetter::handleDynamicDataAttribute (
    CAlfVisual& aVisual,
    AlfAttribute& aAttr,
    AlfAttributeContainer& aContainer,
    IAlfMap* aData )
    {
    CAlfLineVisual* lineVisual = dynamic_cast<CAlfLineVisual*>( &aVisual);
    if ( !lineVisual )
        {
        ALF_THROW ( AlfDataException, ECommonError,
                    "AlfLineVisualAttributeSetter" )
        }    

    const char* attrName = aAttr.name();

    const char* dataField = aAttr.getDataField();

    if ( !dataField )
        {
        ALF_THROW ( AlfDataException, ECommonError,
                    "AlfLineVisualAttributeSetter")
        }

    if (!aData)
        {
        ALF_THROW ( AlfDataException, ECommonError,
                    "AlfLineVisualAttributeSetter" )
        }
        
    IAlfVariantType* data = aData->item ( UString(dataField) );
    
    if (data)
        {
        const char* attrName = aAttr.name();

        if ( !strcmp ( attrName, KOffsetX ) )
            {
            AlfAttribute& offsety = aContainer.getAttributeByName (
                                        KOffsetY );
            //will throw if attribute is not found

            const char* offsetyDataField = offsety.getDataField();
            if ( !offsetyDataField )
                {
                ALF_THROW ( AlfDataException, ECommonError,
                            "AlfLineVisualAttributeSetter" )
                }
            IAlfVariantType* offsetyData = 
                                 aData->item ( UString(offsetyDataField) );

            if ( data->type() == IAlfVariantType::EReal &&
                    offsetyData && 
                    offsetyData->type() == IAlfVariantType::EReal)
                {
                // The time value is in the attribute?
                //Is it not part of data as well?
                TAlfTimedPoint offset;
                offset.iX.SetTarget(data->real(), aAttr.getTime());
                offset.iX.SetStyle ( aAttr.getInterpolationStyle() );
                offset.iX.SetMappingFunctionIdentifier (
                    aAttr.getMappingFunctionId());
                offset.iY.SetTarget(offsetyData->real(), offsety.getTime());
                offset.iY.SetStyle ( offsety.getInterpolationStyle() );
                offset.iY.SetMappingFunctionIdentifier (
                    offsety.getMappingFunctionId());
                CAlfCurvePath* curve = this->getCurvePath(lineVisual);
                curve->SetOffset ( offset );
                }
            else
                {
                ALF_THROW ( AlfDataException, EInvalidVariantDataType, "AlfLineVisualtAttributeSetter");
                }
            }
            
        else if ( !strcmp ( attrName, KOffsetY ) )
            {
            //Make sure X Attribute also exists, but do nothing. Actual values
            //will be set in the iteration that checks for KOffsetX
            aContainer.getAttributeByName ( KOffsetX );
            //will throw if attribute is not found
            }
            
        else if ( !strcmp ( attrName, KThickness ) )
            {
            if ( data->type() == IAlfVariantType::EReal)
                {
                // The time value is in the attribute?
                // Is it not part of data as well?
                // how is the source value stored in data? policy not clear
                TAlfTimedValue tVal;
                tVal.SetTarget(data->real(),(TInt) aAttr.getTime());
                tVal.SetStyle ( aAttr.getInterpolationStyle() );
                tVal.SetMappingFunctionIdentifier (
                    aAttr.getMappingFunctionId());
                lineVisual->SetThickness(tVal);
                }
            else
                {
                ALF_THROW ( AlfDataException, EInvalidVariantDataType, "AlfLineVisualtAttributeSetter");
                }    
            }
            
        else if ( !strcmp ( attrName, KShadowThickness ) )
            {
            if ( data->type() == IAlfVariantType::EReal)
                {
                // The time value is in the attribute?
                // Is it not part of data as well?
                // how is the source value stored in data? policy not clear
                TAlfTimedValue tVal;
                tVal.SetTarget(data->real(),(TInt) aAttr.getTime());
                tVal.SetStyle ( aAttr.getInterpolationStyle() );
                tVal.SetMappingFunctionIdentifier (
                    aAttr.getMappingFunctionId());
                lineVisual->SetShadowThickness(tVal);
                }
            else
                {
                ALF_THROW ( AlfDataException, EInvalidVariantDataType, "AlfLineVisualtAttributeSetter");
                }    
            }
            
        else if ( !strcmp ( attrName, KStartPos ) )
            {
            if ( data->type() == IAlfVariantType::EReal)
                {
                // The time value is in the attribute?
                // Is it not part of data as well?
                // how is the source value stored in data? policy not clear
                TAlfTimedValue tVal;
                tVal.SetTarget(data->real(),(TInt) aAttr.getTime());
                tVal.SetStyle ( aAttr.getInterpolationStyle() );
                tVal.SetMappingFunctionIdentifier (
                    aAttr.getMappingFunctionId());
                lineVisual->SetStartPos(tVal);
                }
            else
                {
                ALF_THROW ( AlfDataException, EInvalidVariantDataType, "AlfLineVisualtAttributeSetter");
                }    
            }
            
        else if ( !strcmp ( attrName, KEndPos ) )
            {
            if ( data->type() == IAlfVariantType::EReal)
                {
                // The time value is in the attribute?
                // Is it not part of data as well?
                // how is the source value stored in data? policy not clear
                TAlfTimedValue tVal;
                tVal.SetTarget(data->real(),(TInt) aAttr.getTime());
                tVal.SetStyle ( aAttr.getInterpolationStyle() );
                tVal.SetMappingFunctionIdentifier (
                    aAttr.getMappingFunctionId());
                lineVisual->SetEndPos(tVal);
                }
            else
                {
                ALF_THROW ( AlfDataException, EInvalidVariantDataType, "AlfLineVisualtAttributeSetter");
                }    
            }

        else
            {
            AlfCommonVisualAttributeSetter::handleDynamicDataAttribute(
                aVisual, aAttr, aContainer, aData);
            }
        }
    }

// ---------------------------------------------------------------------------
// Sets static attributes to visual  from data
// ---------------------------------------------------------------------------
//
void AlfLineVisualAttributeSetter::handleStaticDataAttribute (
    CAlfVisual &aVisual,
    AlfAttribute& aAttr,
    AlfAttributeContainer& aContainer,
    IAlfMap* aData )
    {

    CAlfLineVisual* lineVisual = dynamic_cast<CAlfLineVisual*>( &aVisual);
    if ( !lineVisual )
        {
        ALF_THROW ( AlfDataException, ECommonError,
                    "AlfLineVisualAttributeSetter" )
        }  

    const char* dataField = aAttr.getDataField();

    if ( !dataField )
        {
        ALF_THROW ( AlfDataException, ECommonError,
                    "AlfLineVisualAttributeSetter" );
        }

    if (!aData)
        {
        ALF_THROW ( AlfDataException, ECommonError,
                    "AlfLineVisualAttributeSetter" );
        }

    IAlfVariantType* data = aData->item (UString( dataField) );

    const char* attrName = aAttr.name();

    if ( data )
        {
        if ( !strcmp ( attrName, KImagePath ) )
            {
            if (data->type() == IAlfVariantType::EString)
                {

                TPtrC8 src;
                src.Set((TUint8*)data->string().getUtf8());
                auto_ptr<HBufC> DesC;
                TRAPD(err1,
                  DesC.reset(CnvUtfConverter::ConvertToUnicodeFromUtf8L(src)));
                throwIfErr(err1);

                if (DesC.get())
                    {
                    CAlfTexture* texture = NULL;
                    TRAPD(err2,texture = &(aVisual.Env().TextureManager().
                                           LoadTextureL(*(DesC.get()), 
                                               EAlfTextureFlagDefault,
                                               KAlfAutoGeneratedTextureId)));
                    throwIfErr(err2);
                    lineVisual->SetImage(TAlfImage(*texture));
                    mImpl->mEnv = &aVisual.Env();
                    mImpl->mLoadedTextures.resize(
                        mImpl->mLoadedTextures.count()+1);
                    mImpl->mLoadedTextures.insert(
                        mImpl->mLoadedTextures.count(), texture);
                    }
                }
            else
                {
                ALF_THROW ( AlfDataException, EInvalidVariantDataType, "AlfLineVisualtAttributeSetter");
                }    
            }

        else if ( !strcmp ( attrName, KThickness ) )
            {
            if (data->type() == IAlfVariantType::EReal)
                {
                TAlfTimedValue val(data->real(),0);
                //static attribute,apply with immidiate effect

                lineVisual->SetThickness(val);
                }
            else
                {
                ALF_THROW ( AlfDataException, EInvalidVariantDataType, "AlfLineVisualtAttributeSetter");
                }    
            }

        else if ( !strcmp ( attrName, KShadowThickness ) )
            {
            if (data->type() == IAlfVariantType::EReal)
                {
                TAlfTimedValue val(data->real(),0);
                //static attribute,apply with immidiate effect

                lineVisual->SetShadowThickness(val);
                }
            else
                {
                ALF_THROW ( AlfDataException, EInvalidVariantDataType, "AlfLineVisualtAttributeSetter");
                }    
            }

        else if ( !strcmp ( attrName, KStartPos ) )
            {
            if (data->type() == IAlfVariantType::EReal)
                {
                TAlfTimedValue val(data->real(),0);
                //static attribute,apply with immidiate effect

                lineVisual->SetStartPos(val);
                }
            }

        else if ( !strcmp ( attrName, KEndPos ) )
            {
            if (data->type() == IAlfVariantType::EReal)
                {
                TAlfTimedValue val(data->real(),0);
                //static attribute,apply with immidiate effect

                lineVisual->SetEndPos(val);
                }
            else
                {
                ALF_THROW ( AlfDataException, EInvalidVariantDataType, "AlfLineVisualtAttributeSetter");
                }    
            }

        else if ( !strcmp ( attrName, KAlphaMappingFunction ) )
            {
            //assumption:
            //The int value-mapping function mapping is the following:
            //0 - KConstMappingFunction
            //1 - duiuimodel::nodetypes::KLinearMappingFunction
            //2 - KSineMappingFunction
            //3 - KCosineMappingFunction
            if (data->type() == IAlfVariantType::EContainer )
                {
                IAlfContainer* dataContainer = data->container();
                MAlfMappingFunction* func =
                    this->getMappingFunctionFromContainer(dataContainer,
                                                          lineVisual->Env());

                if (!func)
                    {
                    ALF_THROW ( AlfAttributeException,
                                EInvalidAttribute, "AlfLineVisualAttributeSetter")
                    }

                lineVisual->SetAlphaFunction(func);
                }
            else
                {
                ALF_THROW ( AlfDataException, EInvalidVariantDataType, "AlfLineVisualtAttributeSetter");
                }
            }
            
        else if ( !strcmp ( attrName, KWidthMappingFunction ) )
            {
            //assumption:
            //The int value-mapping function mapping is the following:
            //0 - KConstMappingFunction
            //1 - duiuimodel::nodetypes::KLinearMappingFunction
            //2 - KSineMappingFunction
            //3 - KCosineMappingFunction

            if (data->type() == IAlfVariantType::EContainer )
                {
                IAlfContainer* dataContainer = data->container();
                MAlfMappingFunction* func =
                    this->getMappingFunctionFromContainer(dataContainer,
                                                          lineVisual->Env());

                if (!func)
                    {
                    ALF_THROW ( AlfAttributeException,
                                EInvalidAttribute, "AlfLineVisualAttributeSetter")
                    }

                lineVisual->SetWidthFunction(func);
                }
            else
                {
                ALF_THROW ( AlfDataException, EInvalidVariantDataType, "AlfLineVisualtAttributeSetter");
                }
            }
            
        else if ( !strcmp ( attrName, KColor ) )
            {
            if (data->type() == IAlfVariantType::EContainer )
                {
                IAlfContainer* dataContainer = data->container();

                if ( 4 != dataContainer -> count() )
                    {
                    ALF_THROW ( AlfAttributeException,EIncompleteAttributeValues, "AlfLineVisualAttributeSetter")
                    }

                IAlfVariantType* red = dataContainer->item ( 0 );//red

                IAlfVariantType* green = dataContainer->item ( 1 );//green

                IAlfVariantType* blue = dataContainer->item ( 2 );//blue

                IAlfVariantType* alpha = dataContainer->item ( 3 );//alpha

                if ( red && IAlfVariantType::EReal == red->type()     &&
                        green && IAlfVariantType::EReal == green->type() &&
                        blue && IAlfVariantType::EReal == blue->type()   &&
                        alpha && IAlfVariantType::EReal == alpha->type() )
                    {
                    TRgb color ( red->real(),
                                 green->real(),
                                 blue->real(),
                                 alpha->real() );

                    lineVisual->SetColor ( color );
                    }
                else
                    {
                    ALF_THROW ( AlfAttributeException, EInvalidAttribute,
                                "AlfLineVisualAttributeSetter" )
                    }
                }
            else
                {
                ALF_THROW ( AlfDataException, EInvalidVariantDataType, "AlfLineVisualtAttributeSetter");
                }    
            }
            
        else if ( !strcmp ( attrName, KEnableLoop ) )
            {
            CAlfCurvePath* curve = this->getCurvePath(lineVisual);

            // if value is not bool, then use default parameter of EnableLoop
            if ( data->type() != IAlfVariantType::EBool )
                {
                curve->EnableLoop ();
                }

            else
                {
                curve->EnableLoop ( data->boolean() );
                }
            }

        else if ( !strcmp ( attrName, KOrigin ) )
            {
            CAlfCurvePath* curve = this->getCurvePath(lineVisual);

            if ( data->type() == IAlfVariantType::EReal )
                {
                float origin =  data->real() ;
                curve->SetOrigin ( origin );
                }
            else
                {
                ALF_THROW ( AlfDataException, EInvalidVariantDataType, "AlfLineVisualtAttributeSetter");
                }    
            }

        else if ( !strcmp ( attrName, KOffsetX ) )
            {
            CAlfCurvePath* curve = this->getCurvePath(lineVisual);

            AlfAttribute& offsety = aContainer.getAttributeByName (
                                        KOffsetY );

            //will throw if attribute is not found
            const char* offsetyDataField = offsety.getDataField();
            if ( !offsetyDataField )
                {
                ALF_THROW ( AlfDataException,
                            ECommonError,  "AlfLineVisualAttributeSetter" )
                }

            IAlfVariantType* offsetyData = 
                                 aData->item (UString( offsetyDataField ) );

            if ( data->type() == IAlfVariantType::EReal &&
                    offsetyData && 
                    offsetyData->type() == IAlfVariantType::EReal)
                {
                TAlfTimedPoint offset( data->real(), offsetyData->real());
                curve->SetOffset ( offset );
                }
            else
                {
                ALF_THROW ( AlfDataException, EInvalidVariantDataType, "AlfLineVisualtAttributeSetter");
                }    
            }

        else if ( !strcmp ( attrName, KOffsetY ) )
            {
            //Make sure X Attribute also exists, but do nothing.
            //Actual values will be set in the iteration that checks for
            //KOffsetX

            aContainer.getAttributeByName ( KOffsetX );
            //will throw if attribute is not found
            }

        else if ( !strcmp ( attrName, KLine ) )
            {
            CAlfCurvePath* curve = this->getCurvePath(lineVisual);

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
                            "AlfLineVisualAttributeSetter" );
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
                  curve->AppendLineL ( TAlfRealPoint(startx,starty),
                                       TAlfRealPoint(endx,endy),len ))
            throwIfErr(err1);
            }

        else if ( !strcmp ( attrName, KArc ) )
            {
            CAlfCurvePath* curve = this->getCurvePath(lineVisual);

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
                            "AlfLineVisualAttributeSetter" );
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
                  curve->AppendArcL(TAlfRealPoint(originx,originy),
                                    TAlfRealSize(horzradius,vertradius),
                                    startangle,endangle,len))

            throwIfErr(err1);
            }
            
        else
            {
            AlfCommonVisualAttributeSetter::handleStaticDataAttribute(
                aVisual, aAttr, aContainer, aData );
            }
        }

    }


// ---------------------------------------------------------------------------
// Creates and returns a toolkit defined mappping function based on the
// attribute contents
// assumption:
// The int value-mapping function mapping is the following:
// 0 - KConstMappingFunction
// 1 - duiuimodel::nodetypes::KLinearMappingFunction
// 2 - KSineMappingFunction
// 3 - KCosineMappingFunction
// ---------------------------------------------------------------------------
//
MAlfMappingFunction* AlfLineVisualAttributeSetter::getMappingFunction(
    AlfAttribute& aAttribute,
    CAlfEnv& aEnv)
    {
    MAlfMappingFunction* func=0;
    int intValue = aAttribute.realValue(0);
    
    switch (intValue)
        {
        case 0:
            {
            if (aAttribute.getTargetValueCount() == 2)
                {
                TRAPD(err1,func = CAlfConstantMappingFunction::NewL(aEnv,
                                      aAttribute.realValue(1)))
                throwIfErr(err1);
                }

            else
                {
                TRAPD(err1,func = CAlfConstantMappingFunction::NewL(aEnv))
                throwIfErr(err1);
                }
            }
        break;

        case 1:
            {
            if (aAttribute.getTargetValueCount() == 3)
                {
                TRAPD(err1,func = CAlfLinearMappingFunction::NewL(aEnv,
                                      aAttribute.realValue(1),
                                      aAttribute.realValue(2)))
                throwIfErr(err1);
                }

            else if (aAttribute.getTargetValueCount() == 2)
                {
                TRAPD(err1,func = CAlfLinearMappingFunction::NewL(aEnv,
                                      aAttribute.realValue(1)))
                throwIfErr(err1);
                }

            else
                {
                TRAPD(err1,func = CAlfLinearMappingFunction::NewL(aEnv))
                throwIfErr(err1);
                }
            }
        break;

        case 2:
            {
            if (aAttribute.getTargetValueCount() == 3)
                {
                TRAPD(err1,func = CAlfSineMappingFunction::NewL(aEnv,
                                      aAttribute.realValue(1),
                                      aAttribute.realValue(2)))
                throwIfErr(err1);
                }

            else if (aAttribute.getTargetValueCount() == 2)
                {
                TRAPD(err1,func = CAlfSineMappingFunction::NewL(aEnv,
                                  aAttribute.realValue(1)))
                throwIfErr(err1);
                }

            else
                {
                TRAPD(err1,func = CAlfSineMappingFunction::NewL(aEnv))
                throwIfErr(err1);
                }
            }
        break;

        case 3:
            {
            if (aAttribute.getTargetValueCount() == 3)
                {
                TRAPD(err1,func = CAlfCosineMappingFunction::NewL(aEnv,
                                      aAttribute.realValue(1),
                                      aAttribute.realValue(2)))
                throwIfErr(err1);
                }

            else if (aAttribute.getTargetValueCount() == 2)
                {
                TRAPD(err1,func = CAlfCosineMappingFunction::NewL(aEnv,
                                  aAttribute.realValue(1)))
                throwIfErr(err1);
                }

            else
                {
                TRAPD(err1,func = CAlfCosineMappingFunction::NewL(aEnv))
                throwIfErr(err1);
                }
            }
        break;

        default:
            break;

        }

    return func;
    }


// ---------------------------------------------------------------------------
// Creates and returns a toolkit defined mappping function based on the
// container contents
// assumption:
// The int value-mapping function mapping is the following:
// 0 - KConstMappingFunction
// 1 - duiuimodel::nodetypes::KLinearMappingFunction
// 2 - KSineMappingFunction
// 3 - KCosineMappingFunction
// ---------------------------------------------------------------------------
//
MAlfMappingFunction* AlfLineVisualAttributeSetter::
    getMappingFunctionFromContainer(IAlfContainer* aContainer, CAlfEnv& aEnv)
    {
    MAlfMappingFunction* func=0;

    if (IAlfVariantType::EInt != aContainer->item(0)->type())
        {
        return func;
        }

    switch (aContainer->item(0)->integer())
        {
        case 0:
            {
            if (aContainer -> count() == 2)
                {
                TRAPD(err1,func = CAlfConstantMappingFunction::NewL(aEnv,
                                  aContainer->item(1)->real()))
                throwIfErr(err1);
                }

            else
                {
                TRAPD(err1,func = CAlfConstantMappingFunction::NewL(aEnv))
                throwIfErr(err1);
                }
            }
        break;

        case 1:
            {
            if (aContainer -> count() == 3)
                {
                TRAPD(err1,func = CAlfLinearMappingFunction::NewL(aEnv,
                                      aContainer->item(1)->real(),
                                      aContainer->item(2)->real()))
                throwIfErr(err1);
                }

            else if (aContainer -> count() == 2)
                {
                TRAPD(err1,func = CAlfLinearMappingFunction::NewL(aEnv,
                                  aContainer->item(1)->real()))
                throwIfErr(err1);
                }

            else
                {
                TRAPD(err1,func = CAlfLinearMappingFunction::NewL(aEnv))
                throwIfErr(err1);
                }
            }
        break;

        case 2:
            {
            if (aContainer -> count() == 3)
                {
                TRAPD(err1,func = CAlfSineMappingFunction::NewL(aEnv,
                                      aContainer->item(1)->real(),
                                      aContainer->item(2)->real()))
                throwIfErr(err1);
                }

            else if (aContainer -> count() == 2)
                {
                TRAPD(err1,func = CAlfSineMappingFunction::NewL(aEnv,
                                  aContainer->item(1)->real()))
                throwIfErr(err1);
                }

            else
                {
                TRAPD(err1,func = CAlfSineMappingFunction::NewL(aEnv))
                throwIfErr(err1);
                }
            }
        break;

        case 3:
            {
            if (aContainer -> count() == 3)
                {
                TRAPD(err1,func = CAlfCosineMappingFunction::NewL(aEnv,
                                      aContainer->item(1)->real(),
                                      aContainer->item(2)->real()))
                throwIfErr(err1);
                }

            else if (aContainer -> count() == 2)
                {
                TRAPD(err1,func = CAlfCosineMappingFunction::NewL(aEnv,
                                  aContainer->item(1)->real()))
                throwIfErr(err1);
                }

            else
                {
                TRAPD(err1,func = CAlfCosineMappingFunction::NewL(aEnv))
                throwIfErr(err1);
                }
            }
        break;

        default:
            break;

        }

    return func;
    }


// ---------------------------------------------------------------------------
// If the line visual already has a path set to it, it is retrived.
// Else a new instance is created
// ---------------------------------------------------------------------------
//
CAlfCurvePath* AlfLineVisualAttributeSetter::getCurvePath(
    CAlfLineVisual* aLineVisual)
    {
    CAlfCurvePath* curve = 0;
    if(aLineVisual)
        {
        curve = aLineVisual->Path();

        if (!curve)
            {
            TRAPD(err1,curve = CAlfCurvePath::NewL(aLineVisual->Env()))
            throwIfErr(err1);

            aLineVisual->SetPath( curve, EAlfHasOwnership);
            }
        }
    return curve;
    }

// ---------------------------------------------------------------------------
// Check if data in the attribute is int or float.
// Return the value as a float value
// ---------------------------------------------------------------------------
//
float AlfLineVisualAttributeSetter::floatOrInt(
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
        ALF_THROW ( AlfAttributeException, ECommonError,
                    "AlfLineVisualAttributeSetter" );
        }
    }

// ---------------------------------------------------------------------------
// Check if data in the variant type is int or float.
// Return the value as a float value
// ---------------------------------------------------------------------------
//
float AlfLineVisualAttributeSetter::floatOrIntFromData(IAlfVariantType* aData)
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
        ALF_THROW ( AlfAttributeException, ECommonError,
                    "AlfLineVisualAttributeSetter" );
        }
    }

    }// namespace Alf
// End of file





