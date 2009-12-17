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
* Description:   Implements attributesetters for image.
*
*/


#include "alf/alfattribute.h"
#include "alf/alfimagevisualattributesetter.h"
#include "alf/alfattributevaluetype.h"
#include <alf/alfvisual.h>
#include <alf/alfattributeexception.h>
#include <alf/alfvisualexception.h>
#include "alf/alfattributecontainer.h"
#include <libc/string.h>

#include <uiacceltk/HuiRealPoint.h>

#include <alf/alfimagevisual.h>
#include <alf/alftexture.h>
#include <utf.h>
#include <alf/alfenv.h>
#include <alf/alfdataexception.h>
#include <alf/alfdisplay.h>
#include <alf/alfutil.h>

#include <alf/alfbrusharray.h>
#include <alf/alfframebrush.h>


#include <alf/alfeventhandler.h>
#include <alf/alfevent.h>
#include <osn/alfptrvector.h>
#include <alf/alfresourcepool.h>
#include <alf/alfmetric.h>
#include <alf/alfwidgetenvextension.h>

#include <osn/osnnew.h>
#include <osn/ustring.h>
#include <alf/attrproperty.h>

using namespace osncore;

using namespace duiuimodel::imagevisualattributes;
using namespace duiuimodel::commonvisualattributes;
using namespace duiuimodel::layoutattributes;

namespace Alf
{

class AlfImageVisualAttributeSetterImpl
    {
public:
    AlfImageVisualAttributeSetterImpl();
    ~AlfImageVisualAttributeSetterImpl();
public:
    //Keep track of loaded texture for imagevisuals.
    //Texture manager doesn't unload the texture from memory untill Env is deleted.
    //Hence need to unload them once attribute setter is deleted.
    AlfPtrVector<CAlfTexture> mLoadedTextures;
    //Env needed to access TextureManager while unloading textures.
    CAlfEnv* mEnv;

    // Utility functions. Keep them static so whole class can become static in future
    TAlfImage createImageFromImagePath( const CAlfImageVisual& aImageVisual, const TPtrC8& aPath );
    TAlfImage createNonPoolImageFromSkinLogicalID(CAlfEnv& aEnv, const Utf8* aResourceRef, AlfAttributeContainer& aContainer);    
    };

AlfImageVisualAttributeSetterImpl::AlfImageVisualAttributeSetterImpl()
    {
    //Do not delete textures here as they may be in use by the visuals that have not been deleted.
    mEnv = CAlfEnv::Static();
    mLoadedTextures.setAutoDelete(false);
    }

AlfImageVisualAttributeSetterImpl::~AlfImageVisualAttributeSetterImpl()
    {
    //Unload all loaded textures created by this attributesetter to free up the memory.
    for (int i =0;i<mLoadedTextures.count();i++)
        {
        CAlfTexture* texture = mLoadedTextures[i];
        mEnv->TextureManager().UnloadTexture(texture->Id());
        }
    mLoadedTextures.clear();

    }

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Constructor.
// ---------------------------------------------------------------------------
//
OSN_EXPORT AlfImageVisualAttributeSetter::AlfImageVisualAttributeSetter()
    {
    mImpl.reset(new (EMM) AlfImageVisualAttributeSetterImpl());
    }

// ---------------------------------------------------------------------------
// Destructor.
// ---------------------------------------------------------------------------
//
OSN_EXPORT AlfImageVisualAttributeSetter::~AlfImageVisualAttributeSetter()
    {
    }

// ---------------------------------------------------------------------------
// Sets Attribute Value. Delegates based on attribute Category.
// ---------------------------------------------------------------------------
//
OSN_EXPORT void AlfImageVisualAttributeSetter::setAttributeValue (
    CAlfVisual &aVisual,
    AlfAttributeContainer* aContainer,
    IAlfMap* aData )
    {
    CAlfImageVisual* imageVisual = dynamic_cast<CAlfImageVisual*>(&aVisual);
    if (!imageVisual)
        {
        ALF_THROW ( AlfVisualException, EInvalidVisual, "AlfImageVisualAttributeSetter")
        }

    AlfCommonVisualAttributeSetter::setAttributeValue(aVisual,
            aContainer, aData);
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void AlfImageVisualAttributeSetter::handleDynamicDataAttribute(
    CAlfVisual& aVisual, AlfAttribute& aAttr,
    AlfAttributeContainer& aContainer, IAlfMap* aData)
    {
    AlfCommonVisualAttributeSetter::handleDynamicDataAttribute( aVisual, aAttr,
            aContainer, aData);
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void AlfImageVisualAttributeSetter::handleStaticDataAttribute(
    CAlfVisual &aVisual, AlfAttribute& aAttr,
    AlfAttributeContainer& aContainer, IAlfMap* aData)
    {
    if (!aData) ALF_THROW ( AlfDataException, EInvalidVariantDataType, "CAlfTextVisualAttributeSetter" )

        const char* dataField = aAttr.getDataField();
    if (!dataField) ALF_THROW ( AlfDataException, EInvalidAttribute, "CAlfTextVisualAttributeSetter" )

        IAlfVariantType* data = aData->item ( UString(dataField));

    const char* attrName = aAttr.name();

    if (data)
        {
        CAlfImageVisual* imageVisual = dynamic_cast<CAlfImageVisual*>(&aVisual);
        if(!imageVisual)
            {
            ALF_THROW ( AlfDataException, EInvalidVariantDataType, "CAlfTextVisualAttributeSetter" )
            }

        if (!strcmp(attrName,KImageScaleMode))
            {
            if (data->type() == IAlfVariantType::EInt)
                {
                int val = data->integer();
                imageVisual->SetScaleMode(CAlfImageVisual::TScaleMode(val));
                }
            }
        else if (!strcmp(attrName,KImageColorMode))
            {
            if (data->type() == IAlfVariantType::EInt)
                {
                int val = data->integer();
                imageVisual->SetColorMode(CAlfImageVisual::TColorMode(val));
                }
            }
        else if (!strcmp(attrName,KDropShadow))
            {

            // needs to have discussion about how to store unit in the data

            }
            
        else if (!strcmp(attrName, KImagePath))
            {
            if (data->type() == IAlfVariantType::EString)
                {
                TPtrC8 src;
                src.Set((TUint8*)data->string().getUtf8());
                HandleImagePathAttribute( src, imageVisual, true );
                }
            }

        else if (!strcmp(attrName,KImageResourceRef))
            {
            if (data->type() == IAlfVariantType::EString)
                {
                TAlfImage image;
                try
                    {
                	image = AlfWidgetEnvExtension::resourcePool(aVisual.Env()).getImageResource(data->string().getUtf8());	
                    }
                catch(invalid_argument &)
                    {
                  	ALF_THROW ( AlfDataException, EInvalidAttribute, "AlfImageVisualAttributeSetter:: Image does not exist");
                    }
                
                if(image.HasTexture())
                    {
                    imageVisual->SetImage(image);
                    }
                else
                    {
                    ALF_THROW ( AlfDataException, EInvalidAttribute, "AlfImageVisualAttributeSetter:: Image does not exist");
                    }
                }
            }

        else if (!strcmp(attrName,KSecondaryImagePath))
            {
            if (data->type() == IAlfVariantType::EString)
                {
                TPtrC8 src;
                src.Set((TUint8*)data->string().getUtf8());
            
                HandleImagePathAttribute( src, imageVisual, false );
                }
            }

        else if (!strcmp(attrName,KSecondaryImageResourceRef))
            {
            if (data->type() == IAlfVariantType::EString)
                {
                TAlfImage image;
                try
                    {
                	image = AlfWidgetEnvExtension::resourcePool(aVisual.Env()).getImageResource(data->string().getUtf8());	
                    }
                catch(invalid_argument &)
                    {
                  	ALF_THROW ( AlfDataException, EInvalidAttribute, "AlfImageVisualAttributeSetter:: Image does not exist");
                    }
                if(image.HasTexture())
                    {
                    imageVisual->SetSecondaryImage(image);
                    }
                else
                    {
                    ALF_THROW ( AlfDataException, EInvalidAttribute, "AlfImageVisualAttributeSetter:: Image does not exist");
                    }
                }
            }

        else if (!strcmp(attrName,KSkinImagePath))
            {
            if (data->type() == IAlfVariantType::EString)
                {
                TAlfImage image;
                try
                    {
                	image = AlfWidgetEnvExtension::resourcePool(aVisual.Env()).getImageResource(data->string().getUtf8());	
                    }
                catch(invalid_argument &)
                    {
                    // This image is not present in currently active Theme, 
                    // hence Resource Pool throws an exception
                    // which can be ignored and we will try to load it for first time.
                  	}
                if(!image.HasTexture())
                    {
                    image = mImpl->createNonPoolImageFromSkinLogicalID( imageVisual->Env(), data->string().getUtf8(), aContainer);
                    }                	
                imageVisual->SetImage(image);
                }

            }
        else if (!strcmp(attrName,KAnimatedSkinImage))
            {  
            
            // Retrieve animation skin id
            
    		auto_ptr< HBufC> skinAnimId;
    		if (data->type() == IAlfVariantType::EString)
    			{    		
    			TPtrC8 src( (TUint8*) data->string().getUtf8() );    		
    			TRAPD(err,skinAnimId.reset(CnvUtfConverter::ConvertToUnicodeFromUtf8L(src)));
    		    	
    			if(err != KErrNone)
    				{
    				ALF_THROW ( AlfDataException, err, "AlfImageVisualAttributeSetter") 
    				}  			 
    	   
    	    	// loading the texture from animation id.            
       	    	TAlfImage skinAnimImage(imageVisual->Env().TextureManager().
       	   			           LoadAnimatedTextureL(*(skinAnimId.get()),
       	   						                    imageVisual->Env().PrimaryDisplay().VisibleArea().Size(),
       	   					 	                    EAlfTextureFlagDefault,
       	   					 	                    KAlfAutoGeneratedTextureId));
       	   
       	    	// setting the texture to imagevisual only if it exists.
       	    	if(skinAnimImage.HasTexture())
       	   			{
       	   			imageVisual->SetImage(skinAnimImage);
       	   			}
    			}
       	   			
            }
        else if (!strcmp(attrName,KSecondarySkinImagePath))
            {
            if (data->type() == IAlfVariantType::EString)
                {
                TAlfImage image;
                try
                    {
                	image = AlfWidgetEnvExtension::resourcePool(aVisual.Env()).getImageResource(data->string().getUtf8());	
                    }
                catch(invalid_argument &)
                    {
                    // This image is not present in currently active Theme, 
                    // hence Resource Pool throws an exception
                    // which can be ignored and we will try to load it for first time.
                    }
                if(!image.HasTexture())
                    {
                    image = mImpl->createNonPoolImageFromSkinLogicalID( imageVisual->Env(), data->string().getUtf8(), aContainer);
                    }                	
                imageVisual->SetSecondaryImage(image);
                }
            }
        else if(!strcmp(attrName,KSkinFrameImage))
            {
 		
    		if (data->type() == IAlfVariantType::EString)
                {   		
    				
    			setFrameImage(aVisual,data->string().getUtf8(), aContainer,aData);    				
                }    			
            }
        else if (!strcmp(attrName,KStretchMode))
            {
            if (data->type() == IAlfVariantType::EInt)
                {
                int val = data->integer();
                imageVisual->SetStretchMode(CAlfGc::TStretchMode(val));
                }
            }
        else if (!strcmp(attrName,KColor))
            {
            if (data->type() == IAlfVariantType::EContainer )
                {
                IAlfContainer* dataContainer = data->container();

                if ( 4 != dataContainer -> count() ) ALF_THROW ( AlfDataException, EIncompleteAttributeValues, "AlfImageVisualAttributeSetter")

                    IAlfVariantType* red = dataContainer->item ( 0 ); //red

                IAlfVariantType* green = dataContainer->item ( 1 ); //green

                IAlfVariantType* blue = dataContainer->item ( 2 ); //blue

                IAlfVariantType* alpha = dataContainer->item ( 3 ); //alpha

                if ( red && IAlfVariantType::EInt == red->type() && green && IAlfVariantType::EInt == green->type() && blue && IAlfVariantType::EInt == blue->type() && alpha && IAlfVariantType::EInt == alpha->type() )
                    {
                    TRgb color ( red->integer(), green->integer(), blue->integer(), alpha->integer() );
                    imageVisual->SetColor ( color );
                    }
                else
                    {
                    ALF_THROW ( AlfDataException, EInvalidAttribute, "AlfImageVisualAttributeSetter" )
                    }
                }

            }
        else
            {
            AlfCommonVisualAttributeSetter::handleStaticDataAttribute(aVisual, aAttr, aContainer, aData);
            }
        }
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void AlfImageVisualAttributeSetter::handleDynamicAttribute(CAlfVisual &aVisual, AlfAttribute& aAttr, AlfAttributeContainer& aContainer)
    {
    CAlfImageVisual* imageVisual = dynamic_cast<CAlfImageVisual*>(&aVisual);
    if(!imageVisual)
        {
        return;
        }

    const char* attrName = aAttr.name();

    //KScale
    if (!strcmp(attrName,KScale))
        {
        TAlfTimedValue tVal((TReal32)aAttr.getSourceValue()->realValue());
        tVal.SetTarget((TReal32)aAttr.getTargetValue()->realValue(), (TInt)aAttr.getTime());
        tVal.SetStyle( aAttr.getInterpolationStyle() );
        tVal.SetMappingFunctionIdentifier( aAttr.getMappingFunctionId());
        imageVisual->SetScale( tVal );
        }
    // KSecondaryAlpha
    else if (!strcmp(attrName, KSecondaryAlpha))
        {
        TAlfTimedValue tVal((TReal32)aAttr.getSourceValue()->realValue());
        tVal.SetTarget((TReal32)aAttr.getTargetValue()->realValue(), (TInt)aAttr.getTime());
        tVal.SetStyle( aAttr.getInterpolationStyle() );
        tVal.SetMappingFunctionIdentifier( aAttr.getMappingFunctionId() );
        imageVisual->SetSecondaryAlpha( tVal );
        }
    //KTurnAngle
    else if (!strcmp(attrName, KTurnAngle))
        {
        TAlfTimedValue tVal((TReal32)aAttr.getSourceValue()->realValue());
        tVal.SetTarget((TReal32)aAttr.getTargetValue()->realValue(), (TInt)aAttr.getTime());
        tVal.SetStyle( aAttr.getInterpolationStyle() );
        tVal.SetMappingFunctionIdentifier( aAttr.getMappingFunctionId() );
        imageVisual->SetTurnAngle( tVal );
        }
    else if (!strcmp(attrName,KOffsetX)||!strcmp(attrName,KOffsetY))
        {
        AlfAttribute& attrX = aContainer.getAttributeByName( KOffsetX );
        AlfAttribute& attrY = aContainer.getAttributeByName( KOffsetY );

        TAlfTimedPoint timedpoint((TReal32)attrX.getSourceValue()->realValue(),
                                  (TReal32)attrY.getSourceValue()->realValue());

        timedpoint.iX.SetTarget((TReal32)attrX.getTargetValue()->realValue(), (TInt)attrX.getTime());
        timedpoint.iY.SetTarget((TReal32)attrY.getTargetValue()->realValue(), (TInt)attrY.getTime());

        timedpoint.iX.SetStyle(attrX.getInterpolationStyle());
        timedpoint.iY.SetStyle(attrY.getInterpolationStyle() );

        timedpoint.iX.SetMappingFunctionIdentifier(attrX.getMappingFunctionId());
        timedpoint.iY.SetMappingFunctionIdentifier(attrY.getMappingFunctionId());

        imageVisual->SetOffset(timedpoint);
        attrX.setDirty(false);
        attrY.setDirty(false);

        }
    else
        {
        AlfCommonVisualAttributeSetter::handleDynamicAttribute(
            aVisual, aAttr, aContainer);
        }
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void AlfImageVisualAttributeSetter::handleStaticAttribute(CAlfVisual& aVisual, AlfAttribute& aAttr, AlfAttributeContainer& aContainer)
    {
    CAlfImageVisual* imageVisual = dynamic_cast<CAlfImageVisual*>(&aVisual);
    if(!imageVisual)
        {
        return;
        }

    const char* attrName = aAttr.name();

    // imagevisual attributes
    if (!strcmp(attrName,KImageScaleMode))
        {
        int val = aAttr.intValue();
        imageVisual->SetScaleMode(CAlfImageVisual::TScaleMode(val));
        }
    else if (!strcmp(attrName,KImageColorMode))
        {
        int val = aAttr.intValue();
        imageVisual->SetColorMode( CAlfImageVisual::TColorMode(val));
        }
    else if (!strcmp(attrName,KDropShadow))
        {
        int count = aAttr.getTargetValueCount();

        //user has added one value to the attribute,so this value will be used for both x and y directions.
        if (1==count)
            {
            AlfAttributeValueType* attrType = aAttr.getTargetValue(0);
            imageVisual->SetDropShadow( TAlfMetric(attrType->unit(),attrType->realValue()) );
            }
        //user has not added two values to the attribute,so the first and second values
        // will be used for x and y directions respectively.
        else if (2==count)
            {
            const AlfAttributeValueType* attrType1 = aAttr.getTargetValue(0);
            const AlfAttributeValueType* attrType2 = aAttr.getTargetValue(1);
            imageVisual->SetDropShadow(TAlfXYMetric(TAlfMetric(attrType1->unit(),attrType1->realValue()),TAlfMetric(attrType2->unit(),attrType2->realValue())));
            }
        else
            {
            ALF_THROW ( AlfDataException, EInvalidAttribute, "AlfImageVisualAttributeSetter")
            }

        }
    else if (!strcmp(attrName,KImagePath))
        {
        TPtrC8 src((TUint8*)aAttr.stringValue().getUtf8());
        HandleImagePathAttribute( src, imageVisual, true );
        }
    else if (!strcmp(attrName,KImageResourceRef))
        {

    	TAlfImage image;
	    try
	        {
    		image = AlfWidgetEnvExtension::resourcePool(aVisual.Env()).getImageResource(aAttr.stringValue().getUtf8());
	        }
	    catch(invalid_argument &)
	        {
	        ALF_THROW ( AlfDataException, EInvalidAttribute, "AlfImageVisualAttributeSetter:: Image does not exist");
	        }
       	if(image.HasTexture())
       		imageVisual->SetImage(image);
    	else
    		{
        	ALF_THROW ( AlfDataException, EInvalidAttribute, "AlfImageVisualAttributeSetter:: Image does not exist");
        	}
    
        }
    else if (!strcmp(attrName,KSecondaryImagePath))
        {
        TPtrC8 src((TUint8*)aAttr.stringValue().getUtf8());
        HandleImagePathAttribute( src, imageVisual, false );        
        }
    else if (!strcmp(attrName,KSecondaryImageResourceRef))
        {

    	TAlfImage image;
        try
            {
    		image = AlfWidgetEnvExtension::resourcePool(aVisual.Env()).getImageResource(aAttr.stringValue().getUtf8());
            }
        catch(invalid_argument &)
            {
             ALF_THROW ( AlfDataException, EInvalidAttribute, "AlfImageVisualAttributeSetter:: Image does not exist");
            }
       	if(image.HasTexture())
       		imageVisual->SetSecondaryImage(image);
    	else
    		{
        	ALF_THROW ( AlfDataException, EInvalidAttribute, "AlfImageVisualAttributeSetter:: Image does not exist");
        	}
    
        }
    else if (!strcmp(attrName,KSkinImagePath))
        {
        TAlfImage image;
        try
            {
        	image = AlfWidgetEnvExtension::resourcePool(aVisual.Env()).getImageResource(aAttr.stringValue().getUtf8());
            }
        catch(invalid_argument &)
            {
            // This image is not present in currently active Theme, 
            // hence Resource Pool throws an exception
            // which can be ignored and we will try to load it for first time.
	        }
        if(!image.HasTexture())
            {
            image = mImpl->createNonPoolImageFromSkinLogicalID( 
                imageVisual->Env() ,aAttr.stringValue().getUtf8(), aContainer);
            }
        imageVisual->SetImage(image);
        }
    else if (!strcmp(attrName,KAnimatedSkinImage))
        {
        
        // Retrieve animation skin id        
    	auto_ptr< HBufC> skinAnimId;
    	TPtrC8 src( (TUint8*) aAttr.stringValue().getUtf8() );    		
    	TRAPD(err,skinAnimId.reset(CnvUtfConverter::ConvertToUnicodeFromUtf8L(src)));
    		    	
    	if(err != KErrNone)
    		{
    		ALF_THROW ( AlfDataException, err, "AlfImageVisualAttributeSetter") 
    		}  			 
    	   
    	// loading the texture from animation id.        
       	TAlfImage skinAnimImage(imageVisual->Env().TextureManager().
       	   			           LoadAnimatedTextureL(*(skinAnimId.get()),
       	   						                    imageVisual->Env().PrimaryDisplay().VisibleArea().Size(),
       	   					 	                    EAlfTextureFlagDefault,
       	   					 	                    KAlfAutoGeneratedTextureId));
       	   
       	// setting the texture to imagevisual only if it exists.
       	if(skinAnimImage.HasTexture())
       		{
       		imageVisual->SetImage(skinAnimImage);
       		}
       		
        }
    else if (!strcmp(attrName,KSecondarySkinImagePath))
        {
        TAlfImage	image; 
        try
            {
        	image = AlfWidgetEnvExtension::resourcePool(aVisual.Env()).getImageResource(aAttr.stringValue().getUtf8());
            }
        catch(invalid_argument &)
            {
            // This image is not present in currently active Theme, 
            // hence Resource Pool throws an exception
            // which can be ignored and we will try to load it for first time.
	        }
        if(!image.HasTexture())
            {
            image = mImpl->createNonPoolImageFromSkinLogicalID( imageVisual->Env(), aAttr.stringValue().getUtf8(), aContainer);
            }
        imageVisual->SetSecondaryImage(image);
        }
    else if(!strcmp(attrName,KSkinFrameImage))
        {	
		setFrameImage(aVisual, aAttr.stringValue().getUtf8(), aContainer);		
        }
    else if (!strcmp(attrName,KStretchMode))
        {
        int val = aAttr.intValue();
        imageVisual->SetStretchMode(CAlfGc::TStretchMode(val));
        }
    else if (!strcmp(attrName, KSecondaryAlpha))
        {
        TAlfTimedValue tVal((TReal32)aAttr.getSourceValue()->realValue());
        tVal.SetTarget((TReal32)aAttr.getTargetValue()->realValue(), (TInt)aAttr.getTime());
        tVal.SetStyle( aAttr.getInterpolationStyle() );
        tVal.SetMappingFunctionIdentifier( aAttr.getMappingFunctionId());
        imageVisual->SetSecondaryAlpha( tVal );
        }
    else if (!strcmp(attrName, KTurnAngle))
        {
        TAlfTimedValue tVal((TReal32)aAttr.getSourceValue()->realValue());
        tVal.SetTarget((TReal32)aAttr.getTargetValue()->realValue(), (TInt)aAttr.getTime());
        tVal.SetStyle( aAttr.getInterpolationStyle() );
        tVal.SetMappingFunctionIdentifier( aAttr.getMappingFunctionId() );
        imageVisual->SetTurnAngle( tVal );
        }
    else if (!strcmp(attrName, KColor))
        {
        if ( 4 == aAttr.getTargetValueCount() && AlfAttributeValueType::EInt == aAttr.type(0) && AlfAttributeValueType::EInt == aAttr.type(1) && AlfAttributeValueType::EInt == aAttr.type(2) && AlfAttributeValueType::EInt == aAttr.type(3))
            {
            TRgb color ( aAttr.intValue(0), aAttr.intValue(1), aAttr.intValue(2), aAttr.intValue(3) );
            imageVisual->SetColor ( color );
            }
        else
            {
            ALF_THROW ( AlfDataException, EInvalidAttribute, "AlfImageVisualAttributeSetter")
            }
        }
    else
        {
        AlfCommonVisualAttributeSetter::handleStaticAttribute(aVisual, aAttr, aContainer);
        }
    }



// ---------------------------------------------------------------------------
// Helper method to set the frame image to the image visual.
// ---------------------------------------------------------------------------
//
void AlfImageVisualAttributeSetter::setFrameImage(CAlfVisual& /*aVisual*/, const Utf8* /*aSkinFrameID*/,
							const AlfAttributeContainer& /*aContainer*/,IAlfMap* /*aData*/)
	{	
        	
  }

// ---------------------------------------------------------------------------
// This method gets the value of the attribute based on whether the attribute is static
// or staticData. 
// Currently handled only for static or staticdata.
// ---------------------------------------------------------------------------
   
int AlfImageVisualAttributeSetter::getValueFromAttribute(const AlfAttribute& aAttr,IAlfMap* aData) const
	{	
	int attrValue=0;
	
	// Check whether attribute is from Static type.
	
	if(AlfAttribute::EStatic==aAttr.category())
		{
		attrValue=aAttr.intValue();
		}
	else
		{		
		
		// static data case.		
	    try
	    	{
	    	 if(aData)
	    	 	{	    	 	
	    	 
	    	 	const char* dataValue = aAttr.getDataField();
             	IAlfVariantType* dataVariant = aData->item ( UString(dataValue) );

             	if (dataVariant && dataVariant->type() == IAlfVariantType::EInt )
                	{                
                	
                	attrValue=dataVariant->integer();
                		    		  
	    			}
	    	 	}
	    	}
	    catch(AlfDataException& e)
	    	{	    		    	
	   	    // Error in getting the value.
	   	    // default is already set to 0,dont do anything here.			
	    	}
		}
		
	return attrValue;
	
	}

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
OSN_EXPORT TAlfCommand* AlfImageVisualAttributeSetter::createCommand(
    CAlfVisual& /*aVisual*/, AlfAttributeContainer* /*aContainer*/,
    IAlfMap* /*aData*/, int /*aTransitionTime*/, CAlfVisual* /*aRefVisual*/)
    {
    TAlfCommand* cmd = 0;
    return cmd;
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
OSN_EXPORT void AlfImageVisualAttributeSetter::createAndSendCommands(
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
void AlfImageVisualAttributeSetter::HandleImagePathAttribute(
    const TPtrC8& aStringValue,
    CAlfImageVisual* aImageVisual,
    bool aPrimary )
    {
    TAlfImage image = mImpl->createImageFromImagePath( *aImageVisual, aStringValue); 
    if ( aPrimary )
        {                
        aImageVisual->SetImage(image);
        }
    else
        {
        aImageVisual->SetSecondaryImage(image);                
        }      
    }

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------    
TAlfImage AlfImageVisualAttributeSetterImpl::createNonPoolImageFromSkinLogicalID(CAlfEnv& /*aEnv*/,  const Utf8* /*aLogicalId*/, AlfAttributeContainer& /*aContainer*/)
    {
    
    // Depricated
    return TAlfImage();   
   
    }

TAlfImage AlfImageVisualAttributeSetterImpl::createImageFromImagePath( const CAlfImageVisual& aImageVisual, const TPtrC8& aPath )
    {
    TAlfImage image; // return value

    auto_ptr<HBufC> textureId;
    TRAPD(err,textureId.reset(CnvUtfConverter::ConvertToUnicodeFromUtf8L(aPath)));
    if ( err != KErrNone )
        {
        ALF_THROW ( AlfDataException, err, "AlfImageVisualAttributeSetter")
        }

    // check if the texture is already loaded
    CAlfTextureManager& textureMgr = aImageVisual.Env().TextureManager();
    const TInt existingTextureId = textureMgr.TextureId(*(textureId.get()));
    if (existingTextureId != KErrNotFound)
    	{
            const CAlfTexture* texture = textureMgr.Texture( existingTextureId );
            image = TAlfImage(*texture);            
       	}
    else
        {
        CAlfTexture* mutableTexture = 0;
        TRAP(err, mutableTexture = &(textureMgr.LoadTextureL(*(textureId.get()), EAlfTextureFlagDefault, KAlfAutoGeneratedTextureId)));
        if ( err != KErrNone )
            {
            ALF_THROW ( AlfDataException, err, "AlfImageVisualAttributeSetter")
            }    
        mutableTexture->EnableRefCounting(ETrue);
        image = TAlfImage(*mutableTexture);
        mLoadedTextures.resize(mLoadedTextures.count()+1);
        mLoadedTextures.insert(mLoadedTextures.count(),mutableTexture);       
        }	
    
    return image;
    }
	
}// Alf
