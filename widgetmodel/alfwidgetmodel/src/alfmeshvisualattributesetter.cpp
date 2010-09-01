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
* Description:   Implements attributesetters for MeshVisual.
*
*/



#include "alf/alfattribute.h"
#include "alf/alfmeshvisualattributesetter.h"
#include "alf/alfattributevaluetype.h"
#include <alf/alfvisual.h>
#include <alf/alfattributeexception.h>
#include <alf/alfvisualexception.h>
#include "alf/alfattributecontainer.h"
#include <libc/string.h>
#include <osn/ustring.h>
#include "alf/attrproperty.h"
#include <uiacceltk/HuiRealPoint.h>

#include <alf/alfmeshvisual.h>
#include <alf/alftexture.h>
#include <utf.h>
#include <alf/alfenv.h>
#include <alf/alfdataexception.h>
#include <alf/alfdisplay.h>

#include <alf/alfbrusharray.h>
#include <alf/alfframebrush.h>


#include <alf/alfeventhandler.h>
#include <alf/alfevent.h>
#include <osn/alfptrvector.h>


using namespace osncore;

using namespace duiuimodel::meshvisualattributes;
using namespace duiuimodel::commonvisualattributes;

namespace Alf
    {

class AlfMeshVisualAttributeSetterImpl
    {
public:
    AlfMeshVisualAttributeSetterImpl();
    ~AlfMeshVisualAttributeSetterImpl();
public:
    //Keep track of loaded texture for imagevisuals.
    //Texture manager doesn't unload the texture from memory untill Env is deleted.
    //Hence need to unload them once attribute setter is deleted.
    AlfPtrVector<CAlfTexture> mLoadedTextures;
    //Env needed to access TextureManager while unloading textures.
    CAlfEnv* mEnv;
    };

AlfMeshVisualAttributeSetterImpl::AlfMeshVisualAttributeSetterImpl()
    {
    //Do not delete textures here as they may be in use by the visuals that have not been deleted.
    mEnv = CAlfEnv::Static();
    mLoadedTextures.setAutoDelete(false);
    }

AlfMeshVisualAttributeSetterImpl::~AlfMeshVisualAttributeSetterImpl()
    {
    //Unload all loaded textures created by this attributesetter to free up the memory.
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
OSN_EXPORT AlfMeshVisualAttributeSetter::AlfMeshVisualAttributeSetter()
    {
    mImpl.reset(new (EMM) AlfMeshVisualAttributeSetterImpl());
    }

// ---------------------------------------------------------------------------
// Destructor.
// ---------------------------------------------------------------------------
//
OSN_EXPORT AlfMeshVisualAttributeSetter::~AlfMeshVisualAttributeSetter()
    {
    }


// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
IAlfVariantType* AlfMeshVisualAttributeSetter::getData(AlfAttribute& aAttr, IAlfMap* aData)
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
int AlfMeshVisualAttributeSetter::getTime(const AlfAttribute& aAttr, bool aImmediate)
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

void AlfMeshVisualAttributeSetter::handleDynamicDataAttribute(CAlfVisual & aVisual,
        AlfAttribute& aAttr, AlfAttributeContainer& aContainer, IAlfMap* aData)
    {
    if (!doHandleDynamicDataAttribute(aVisual, aAttr, aContainer, aData))
        {
        AlfCommonVisualAttributeSetter::handleDynamicDataAttribute(
            aVisual, aAttr, aContainer, aData);
        }
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void AlfMeshVisualAttributeSetter::handleStaticDataAttribute(
    CAlfVisual &aVisual, AlfAttribute& aAttr,
    AlfAttributeContainer& aContainer, IAlfMap* aData)
    {
    CAlfMeshVisual* meshVisual = dynamic_cast<CAlfMeshVisual*>(&aVisual);
    if (!meshVisual)
        {
        ALF_THROW ( AlfDataException, ECommonError, "AlfMeshVisualAttributeSetter" )
        }
    if (!aData)  ALF_THROW ( AlfDataException, EInvalidVariantDataType, "AlfMeshVisualAttributeSetter" )

        const char* dataField = aAttr.getDataField();
    if ( !dataField )  ALF_THROW ( AlfDataException, EInvalidAttribute, "AlfMeshVisualAttributeSetter" )

        IAlfVariantType* data = aData->item ( UString(dataField));

    const char* attrName = aAttr.name();

    if (data)
        {
        if (!strcmp(attrName,KMeshType))
            {
            if (data->type() == IAlfVariantType::EInt)
                {
                int val = data->integer();
                TRAPD(err,meshVisual->CreateMeshL(val));
                if (err!=KErrNone)
                    {
                    ALF_THROW ( AlfDataException, err, "AlfMeshVisualAttributeSetter")
                    }
                }
            }
        else if (!strcmp(attrName, KImagePath))
            {
            if (data->type() == IAlfVariantType::EString)
                {
                TPtrC8 src;
                src.Set((TUint8*)data->string().getUtf8());

                HandleImagePathAttribute( src, meshVisual, primary );
                }
            }
        else if (!strcmp(attrName,KSecondaryImagePath))
            {
            if (data->type() == IAlfVariantType::EString)
                {
                TPtrC8 src;
                src.Set((TUint8*)data->string().getUtf8());

                HandleImagePathAttribute( src, meshVisual, secondary );
                }
            }
        else if (!strcmp(attrName,KSpecularImagePath))
            {
            if (data->type() == IAlfVariantType::EString)
                {
                TPtrC8 src;
                src.Set((TUint8*)data->string().getUtf8());

                HandleImagePathAttribute( src, meshVisual, specular );
                }
            }
        else
            {
            //check, if dynamic attribute(but set by user as static), if so, set attributes immediately.
            //If not, the function will call baseclass SetAttributeValue.
            if (!doHandleDynamicDataAttribute(aVisual, aAttr, aContainer, aData, true))
                {
                AlfCommonVisualAttributeSetter::handleStaticDataAttribute(
                    aVisual, aAttr, aContainer, aData);
                }
            }
        }
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void AlfMeshVisualAttributeSetter::handleDynamicAttribute(CAlfVisual &aVisual,
        AlfAttribute& aAttr, AlfAttributeContainer& aContainer)
    {
    if (!doHandleDynamicAttribute(aVisual, aAttr, aContainer))
        {
        AlfCommonVisualAttributeSetter::handleDynamicAttribute(
            aVisual, aAttr, aContainer);
        }
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void AlfMeshVisualAttributeSetter::handleStaticAttribute(
    CAlfVisual& aVisual, AlfAttribute& aAttr, AlfAttributeContainer& aContainer)
    {
    CAlfMeshVisual* meshVisual = dynamic_cast<CAlfMeshVisual*>(&aVisual);
    if(!meshVisual)
        {
        return;
        }
    const char* attrName = aAttr.name();

    // meshVisual attributes
    if (!strcmp(attrName,KMeshType))
        {
        int val = aAttr.intValue();
        TRAPD(err,meshVisual->CreateMeshL(val));
        if (err!=KErrNone)
            {
            ALF_THROW(AlfDataException, err, "AlfMeshVisualAttributeSetter");
            }
        }
    else if (!strcmp(attrName,KImagePath))
        {
        TPtrC8 src((TUint8*)aAttr.stringValue().getUtf8());        
        HandleImagePathAttribute( src, meshVisual, primary );
        }
    else if (!strcmp(attrName,KSecondaryImagePath))
        {
        TPtrC8 src((TUint8*)aAttr.stringValue().getUtf8());        
        HandleImagePathAttribute( src, meshVisual, secondary );
        }
    else if (!strcmp(attrName,KSpecularImagePath))
        {
        TPtrC8 src((TUint8*)aAttr.stringValue().getUtf8());        
        HandleImagePathAttribute( src, meshVisual, specular );
        }
    else
        {
        if (!doHandleDynamicAttribute(aVisual, aAttr, aContainer, true))
            {
            AlfCommonVisualAttributeSetter::handleStaticAttribute(aVisual, aAttr, aContainer);
            }
        }
    }


// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
OSN_EXPORT void AlfMeshVisualAttributeSetter::setAttributeValue(
    CAlfVisual &aVisual,
    AlfAttributeContainer* aContainer,
    IAlfMap* aData)
    {
    CAlfMeshVisual* meshVisual = dynamic_cast<CAlfMeshVisual*>(&aVisual);
    if (!meshVisual)
        {
        ALF_THROW ( AlfVisualException, EInvalidVisual, "AlfMeshVisualAttributeSetter")
        }

    AlfCommonVisualAttributeSetter::setAttributeValue(
        aVisual, aContainer, aData);
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
OSN_EXPORT TAlfCommand* AlfMeshVisualAttributeSetter::createCommand(
    CAlfVisual& /*aVisual*/, AlfAttributeContainer* /*aContainer*/,
    IAlfMap* /*aData*/, int /*aTransitionTime*/, CAlfVisual* /*aRefVisual*/)
    {
    TAlfCommand* cmd = 0;
    return cmd;
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
OSN_EXPORT void AlfMeshVisualAttributeSetter::createAndSendCommands(
    CAlfVisual& aVisual,
    AlfAttributeContainer* aContainer,
    CAlfVisual* aRefVisual )
    {

    // Call the base class implementation.It goes through its loop and checks if there are attributes
    // that it handles.
    AlfCommonVisualAttributeSetter::createAndSendCommands(
        aVisual,
        aContainer,
        aRefVisual );
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//

bool AlfMeshVisualAttributeSetter::doHandleDynamicDataAttribute(CAlfVisual & aVisual,
        AlfAttribute& aAttr, AlfAttributeContainer& /*aContainer*/, IAlfMap* aData, bool aImmediate)
    {
    bool handled = true;

    if (!aData)
        {
        ALF_THROW ( AlfDataException, EInvalidVariantDataType, "AlfMeshVisualAttributeSetter" )
        }
    else
        {
        CAlfMeshVisual* meshVisual = dynamic_cast<CAlfMeshVisual*>(&aVisual);
        if(!meshVisual)
            {
            return false;
            }
        const char* attrName = aAttr.name();

        if ( !strcmp( attrName, KSecondaryAlpha ) )
            {
            const IAlfVariantType* data = getData(aAttr, aData);
            if (data && data->type() == IAlfVariantType::EReal)
                {
                TAlfTimedValue tVal(data->real(), getTime(aAttr, aImmediate));
                tVal.SetStyle(aAttr.getInterpolationStyle());
                tVal.SetMappingFunctionIdentifier( aAttr.getMappingFunctionId() );
                meshVisual->SetSecondaryAlpha( tVal );
                }
            }
        else if ( !strcmp( attrName, KYawAngle ) )
            {
            const IAlfVariantType* data = getData(aAttr, aData);
            if (data && data->type() == IAlfVariantType::EReal)
                {
                TAlfTimedValue tVal(data->real(), getTime(aAttr, aImmediate));
                tVal.SetStyle(aAttr.getInterpolationStyle());
                tVal.SetMappingFunctionIdentifier( aAttr.getMappingFunctionId() );
                meshVisual->SetYawAngle( tVal );
                }
            }
        else if ( !strcmp( attrName, KPitchAngle ) )
            {
            const IAlfVariantType* data = getData(aAttr, aData);
            if (data && data->type() == IAlfVariantType::EReal)
                {
                TAlfTimedValue tVal(data->real(), getTime(aAttr, aImmediate));
                tVal.SetStyle(aAttr.getInterpolationStyle());
                tVal.SetMappingFunctionIdentifier( aAttr.getMappingFunctionId() );
                meshVisual->SetPitchAngle( tVal );
                }
            }
        else if ( !strcmp( attrName, KScale ) )
            {
            const IAlfVariantType* data = getData(aAttr, aData);
            if (data && data->type() == IAlfVariantType::EReal)
                {
                TAlfTimedValue tVal(data->real(), getTime(aAttr, aImmediate));
                tVal.SetStyle(aAttr.getInterpolationStyle());
                tVal.SetMappingFunctionIdentifier( aAttr.getMappingFunctionId() );
                meshVisual->SetScale( tVal );
                }
            }
        else
            {
            handled = false;
            }
        }

    return handled;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
bool AlfMeshVisualAttributeSetter::doHandleDynamicAttribute(
    CAlfVisual &aVisual, AlfAttribute& aAttr,
    AlfAttributeContainer& /*aContainer*/, bool aImmediate)
    {
    CAlfMeshVisual* meshVisual = dynamic_cast<CAlfMeshVisual*>(&aVisual);
    if(!meshVisual)
        {
        return false;
        }

    const char* attrName = aAttr.name();
    bool handled = true;

    // KSecondaryAlpha
    if (!strcmp(attrName, KSecondaryAlpha))
        {
        TAlfTimedValue tVal(aAttr.getSourceValue()->realValue());
        tVal.SetTarget(aAttr.getTargetValue()->realValue(),getTime(aAttr, aImmediate));
        tVal.SetStyle( aAttr.getInterpolationStyle() );
        tVal.SetMappingFunctionIdentifier( aAttr.getMappingFunctionId() );
        meshVisual->SetSecondaryAlpha( tVal );
        }
    //KYawAngle
    else if (!strcmp(attrName, KYawAngle))
        {
        TAlfTimedValue tVal((TReal32)aAttr.getSourceValue()->realValue());
        tVal.SetTarget((TReal32)aAttr.getTargetValue()->realValue(),getTime(aAttr, aImmediate));
        tVal.SetStyle( aAttr.getInterpolationStyle() );
        tVal.SetMappingFunctionIdentifier( aAttr.getMappingFunctionId() );
        meshVisual->SetYawAngle( tVal );
        }
    //KPitchAngle
    else if (!strcmp(attrName, KPitchAngle))
        {
        TAlfTimedValue tVal((TReal32)aAttr.getSourceValue()->realValue());
        tVal.SetTarget((TReal32)aAttr.getTargetValue()->realValue(),getTime(aAttr, aImmediate));
        tVal.SetStyle( aAttr.getInterpolationStyle() );
        tVal.SetMappingFunctionIdentifier( aAttr.getMappingFunctionId() );
        meshVisual->SetPitchAngle( tVal );
        }
    //KScale
    else if (!strcmp(attrName,KScale))
        {
        TAlfTimedValue tVal((TReal32)aAttr.getSourceValue()->realValue());
        tVal.SetTarget((TReal32)aAttr.getTargetValue()->realValue(),getTime(aAttr, aImmediate));
        tVal.SetStyle( aAttr.getInterpolationStyle() );
        tVal.SetMappingFunctionIdentifier( aAttr.getMappingFunctionId());
        meshVisual->SetScale( tVal );
        }
    else // Call the base class implementation also.
        //It goes through the same loop again and checks if there are attributes that it handles.
        {
        handled = false;
        }

    return handled;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void AlfMeshVisualAttributeSetter::HandleImagePathAttribute(
    const TPtrC8& aStringValue,
    CAlfMeshVisual* aVisual,
    imageCategory aImageCategory )
    {
    auto_ptr<HBufC> DesC;
    TRAPD( err, DesC.reset(CnvUtfConverter::ConvertToUnicodeFromUtf8L(aStringValue)));
    if ( err != KErrNone )
        {
        ALF_THROW( AlfDataException, err, "AlfMeshVisualAttributeSetter" );
        }

    // check if the texture is already loaded
    const TInt existingTextureId = aVisual->Env().TextureManager().TextureId(*(DesC.get()));
    if ( existingTextureId != KErrNotFound )
        {
        const CAlfTexture* texture = aVisual->Env().TextureManager().Texture( existingTextureId );

        if ( aImageCategory == primary )
            {
            aVisual->SetImage(TAlfImage(*texture));
            }
        else if ( aImageCategory == secondary )
            {
            aVisual->SetSecondaryImage(TAlfImage(*texture));
            }
        else // specular image
            {
            aVisual->SetSpecularImage(TAlfImage(*texture));
            }
        }
    else
        {
        mImpl->mLoadedTextures.resize(mImpl->mLoadedTextures.count()+1);

        CAlfTexture* texture = NULL;
        TRAP( err, texture = &(aVisual->Env().TextureManager().LoadTextureL(
            *(DesC.get()), EAlfTextureFlagDefault, KAlfAutoGeneratedTextureId)));

        if ( err != KErrNone )
            {
            ALF_THROW( AlfDataException, err, "AlfMeshVisualAttributeSetter" );
            }

        // This cannot fail because the array has already been resized
        mImpl->mLoadedTextures.insert(mImpl->mLoadedTextures.count(),texture);

        if ( aImageCategory == primary )
            {
            aVisual->SetImage(TAlfImage(*texture));
            }
        else if ( aImageCategory == secondary )
            {
            aVisual->SetSecondaryImage(TAlfImage(*texture));
            }
        else // specular image
            {
            aVisual->SetSpecularImage(TAlfImage(*texture));
            }
        }
    }


    }// Alf
