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
* Description:   Attribute setter implementation for mulimage visual.
 *
*/

 
// Class header
#include "../inc/mulimagevisualattributesetter.h"

// Alf headers
#include "alf/alfattribute.h"
#include <alf/alfattributeexception.h>
#include <alf/alfvisualexception.h>
#include "alf/alfattributecontainer.h"
#include <alf/alfimagevisual.h>
#include <alf/alftexture.h> 
#include <alf/alfenv.h>
#include <alf/alfdataexception.h>
#include <alf/alfdisplay.h>
#include <alf/alfbrusharray.h>
#include <alf/alfframebrush.h>
#include <alf/alfimageloaderutil.h>
#include <alf/alfbitmapprovider.h>
#include <alf/alfresourcepool.h> //for resource pooling
#include <alf/alfwidgetenvextension.h> //for resource pooling
// Osn headers
#include <osn/ustring.h>
#include <osn/alfptrvector.h>
//#include <osn/osnlogicerror.h>
// Std headers
#include <libc/string.h>
#include <utf.h>
// DUI headers
#include "alf/attrproperty.h"

//for logs
#include "mullog.h"
#include "mulutility.h"

using namespace osncore;

using namespace duiuimodel::imagevisualattributes;
using namespace duiuimodel::commonvisualattributes;
using namespace duiuimodel::layoutattributes;

namespace Alf
	{

class MulImageVisualAttributeSetterImpl
	{
public:
	MulImageVisualAttributeSetterImpl();
	~MulImageVisualAttributeSetterImpl();
public:
	//Keep track of loaded texture for imagevisuals.
	//Texture manager doesn't unload the texture from memory untill Env is deleted.
	//Hence need to unload them once attribute setter is deleted.
	AlfPtrVector<CAlfTexture> mLoadedTextures;
	//Env needed to access TextureManager while unloading textures.
	CAlfEnv* mEnv;
	};
	
	
// ---------------------------------------------------------------------------
// Constructor	
// ---------------------------------------------------------------------------
//  
MulImageVisualAttributeSetterImpl::MulImageVisualAttributeSetterImpl()
	{
		MUL_LOG_INFO("aakash::CMulImageVisualAttributeSetterImpl::MulImageVisualAttributeSetterImpl");
	//Do not delete textures here as they may be in use by the visuals that have not been deleted.
	mLoadedTextures.setAutoDelete (false);
	}

// ---------------------------------------------------------------------------
// Destructor	
// ---------------------------------------------------------------------------
//
MulImageVisualAttributeSetterImpl::~MulImageVisualAttributeSetterImpl()
	{
	MUL_LOG_INFO("aakash::CMulImageVisualAttributeSetterImpl::~MulImageVisualAttributeSetterImpl");
	//Unload all loaded textures created by this attributesetter to free up the memory.
	for (int i =0; i<mLoadedTextures.count ();i++)
		{
		CAlfTexture* texture = mLoadedTextures[i];
		mEnv->TextureManager().UnloadTexture (texture->Id ());
		}
	mLoadedTextures.clear ();
	}

// ---------------------------------------------------------------------------
// Constructor	
// ---------------------------------------------------------------------------
// 
 OSN_EXPORT MulImageVisualAttributeSetter::MulImageVisualAttributeSetter()
	{
	MUL_LOG_INFO("aakash::CMulImageVisualAttributeSetter::MulImageVisualAttributeSetter");
	mImpl.reset(new (EMM) MulImageVisualAttributeSetterImpl());
	mImageAttrSetter.reset(new (EMM) AlfImageVisualAttributeSetter());
	}

// ---------------------------------------------------------------------------
// Destructor	
// ---------------------------------------------------------------------------
//
OSN_EXPORT MulImageVisualAttributeSetter::~MulImageVisualAttributeSetter()
	{
	MUL_LOG_INFO("aakash::CMulImageVisualAttributeSetter::~MulImageVisualAttributeSetter");
	}

// ---------------------------------------------------------------------------
// Sets Attribute Value. Delegates based on attribute Category.
// ---------------------------------------------------------------------------
//
OSN_EXPORT void MulImageVisualAttributeSetter::setAttributeValue (
											  CAlfVisual &aVisual,
											  AlfAttributeContainer* aContainer,
											  IAlfMap* aData )
    {
    MUL_LOG_INFO("aakash::CMulImageVisualAttributeSetter::setAttributeValue");
    mData = aData;
    CAlfImageVisual* imageVisual = dynamic_cast<CAlfImageVisual*>(&aVisual);
    if (!imageVisual)
        {
        ALF_THROW ( AlfVisualException, EInvalidVisual, "AlfImageVisualAttributeSetter")
        }
    AlfCommonVisualAttributeSetter::setAttributeValue(aVisual,aContainer, aData);
    }
// ---------------------------------------------------------------------------
// HandleStaticDataAttribute	
// ---------------------------------------------------------------------------
// 
void MulImageVisualAttributeSetter::handleStaticDataAttribute(
		CAlfVisual &aVisual, AlfAttribute& aAttr,
		AlfAttributeContainer& aContainer, IAlfMap* aData)
	{
	MUL_LOG_INFO("aakash::CMulImageVisualAttributeSetter::handleStaticDataAttribute");
	CAlfImageVisual* imageVisual = dynamic_cast<CAlfImageVisual*>(&aVisual);
	if ( !imageVisual)
		{
		ALF_THROW ( AlfVisualException, EInvalidVisual, "CAlfImageVisualAttributeSetter")
		}

	if ( !aData) ALF_THROW ( AlfDataException, EInvalidVariantDataType, "CAlfTextVisualAttributeSetter" )

	const char* dataField = aAttr.getDataField ();
	if ( !dataField) ALF_THROW ( AlfDataException, EInvalidAttribute, "CAlfTextVisualAttributeSetter" )

	IAlfVariantType* data = aData->item (UString( dataField ));
	const char* attrName = aAttr.name ();	
	if ( data)
		{
		if( !strcmp(attrName, KImagePath))
			{
			if ( data->type ()== IAlfVariantType::EString)
				{
				if ( IsSVGImage (data->string ()))
					{
					LoadImageFromSvg(data->string (),*imageVisual);
					}
				else if (MulUtility::IsPoolResource(data->string ()))
				{
				//using resource pooling mechanism for image visual
				ResourcePool& pool =  AlfWidgetEnvExtension::resourcePool(imageVisual->Env());
				imageVisual->SetImage(pool.getImageResource((data->string ().getUtf8())));
				}
				else
					{
					//AlfImageVisualAttributeSetter::handleStaticDataAttribute(aVisual,aAttr,aContainer,aData);
					mImageAttrSetter->setAttributeValue(aVisual,&aContainer,aData);
					}
				}
			else if ( data->type ()== IAlfVariantType::EInt)
				{
				LoadImageFromTexture(data->integer(),*imageVisual);	
				}
			}
		else if( !strcmp(attrName, KSecondaryImagePath))
			{
			if ( data->type ()== IAlfVariantType::EString)
				{
				if ( IsSVGImage (data->string ()))
					{
					LoadImageFromSvg(data->string (),*imageVisual);
					}
				else if (MulUtility::IsPoolResource(data->string ()))
				{
				//using resource pooling mechanism for image visual
				ResourcePool& pool1 =  AlfWidgetEnvExtension::resourcePool(imageVisual->Env());
				imageVisual->SetImage(pool1.getImageResource((data->string ().getUtf8())));
				}
				else
					{
					mImageAttrSetter->setAttributeValue(aVisual,&aContainer,aData);
					}
				}
			else if ( data->type ()== IAlfVariantType::EInt)
				{
				LoadImageFromTexture(data->integer(),*imageVisual);	
				}
			}
		else
			{
			mImageAttrSetter->setAttributeValue(aVisual,&aContainer,aData);
			}
		}
	}

// ---------------------------------------------------------------------------
// handleStaticAttribute	
// ---------------------------------------------------------------------------
//  
void MulImageVisualAttributeSetter::handleStaticAttribute( CAlfVisual& aVisual, 
														   AlfAttribute& aAttr,
														   AlfAttributeContainer& aContainer )
	{
	MUL_LOG_INFO("aakash::CMulImageVisualAttributeSetter::handleStaticAttribute");
	CAlfImageVisual* imageVisual = dynamic_cast<CAlfImageVisual*>(&aVisual);
	if ( !imageVisual)
		{
		ALF_THROW ( AlfVisualException, EInvalidVisual, "AlfImageVisualAttributeSetter")
		}
	
	const char* attrName = aAttr.name ();
	if( !strcmp(attrName, KImagePath))
		{
		if ( IsSVGImage ( aAttr.stringValue()))
			{
			LoadImageFromSvg( aAttr.stringValue(),*imageVisual);
			}
			
		else
			{
			mImageAttrSetter->setAttributeValue(aVisual,&aContainer,mData);
			}
		}
	else if( !strcmp(attrName, KSecondaryImagePath))
		{
		if ( IsSVGImage (aAttr.stringValue()))
			{
			LoadImageFromSvg(aAttr.stringValue(),*imageVisual);
			}
			
		else
			{
			mImageAttrSetter->setAttributeValue(aVisual,&aContainer,mData);
			}
		}
	else
		{
		mImageAttrSetter->setAttributeValue(aVisual,&aContainer,mData);
		}
	}

// ---------------------------------------------------------------------------
// handleDynamicDataAttribute	
// ---------------------------------------------------------------------------
//
void MulImageVisualAttributeSetter::handleDynamicDataAttribute(CAlfVisual &aVisual,
																AlfAttribute& /*aAttr*/, 
																AlfAttributeContainer& aContainer,
																IAlfMap* aData)
	{
	MUL_LOG_INFO("aakash::CMulImageVisualAttributeSetter::handleDynamicDataAttribute");
	mImageAttrSetter->setAttributeValue(aVisual,&aContainer,aData);
	}

// ---------------------------------------------------------------------------
// handleDynamicDataAttribute	
// ---------------------------------------------------------------------------
//
void MulImageVisualAttributeSetter::handleDynamicAttribute(CAlfVisual &aVisual,
														   AlfAttribute& /*aAttr*/, 
														   AlfAttributeContainer& aContainer)
	{
	MUL_LOG_INFO("aakash::CMulImageVisualAttributeSetter::handleDynamicAttribute");
	mImageAttrSetter->setAttributeValue(aVisual,&aContainer,mData);
	}

// ---------------------------------------------------------------------------
// IsSVGImage	
// ---------------------------------------------------------------------------
//
OSN_EXPORT bool MulImageVisualAttributeSetter::IsSVGImage(const UString& aImagePath)
	{
	MUL_LOG_INFO("aakash::CMulImageVisualAttributeSetter::IsSVGImage");
	TPtrC8 src((TUint8*)aImagePath.getUtf8());
    HBufC* srcBuf = HBufC::NewLC(src.Length());
    
    srcBuf->Des().Copy(src);
    srcBuf->Des().LowerCase();
    
    _LIT(KSvgFile,".svg");

	bool retval = ( KErrNotFound != srcBuf->FindC(KSvgFile));
	
    CleanupStack::PopAndDestroy( srcBuf ); 
    
	return retval;
	}

// ---------------------------------------------------------------------------
// LoadImageFromSvg	
// ---------------------------------------------------------------------------
//
OSN_EXPORT void MulImageVisualAttributeSetter::LoadImageFromSvg(const UString& aSvgFilePath,
													 CAlfImageVisual& aImageVisual )
	{
	MUL_LOG_INFO("aakash::CMulImageVisualAttributeSetter::LoadImageFromSvg");
	TPtrC8 src((TUint8*)aSvgFilePath.getUtf8());
	HBufC* imagePath = CnvUtfConverter::ConvertToUnicodeFromUtf8L (src);
	CleanupStack::PushL( imagePath );
	
	CAlfImageLoaderUtil imgLoaderUtil;
	imgLoaderUtil.SetSize (TSize (50, 50));

	MAlfBitmapProvider* iSvgBitMapProv= imgLoaderUtil.CreateSVGImageLoaderL (*imagePath);
	CAlfTexture &texture = aImageVisual.Env().TextureManager().CreateTextureL (
							KAlfAutoGeneratedTextureId,
							iSvgBitMapProv,
							EAlfTextureFlagDefault);
	
	aImageVisual.SetScaleMode (CAlfImageVisual::EScaleFit);
	aImageVisual.SetImage (TAlfImage (texture));
	
	CleanupStack::PopAndDestroy(imagePath);
	}

// ---------------------------------------------------------------------------
// LoadImageFromTexture	
// ---------------------------------------------------------------------------
//
OSN_EXPORT void MulImageVisualAttributeSetter::LoadImageFromTexture( int aTextureId,
														  CAlfImageVisual& aImageVisual )
	{
	MUL_LOG_INFO("aakash::CMulImageVisualAttributeSetter::LoadImageFromTexture");
	//Seems like the Env's Texture Manager is not returning a copy. And When Env gets deleted the texture is destroyed
	//So ownership is not with the client
	const CAlfTexture* texture = aImageVisual.Env().TextureManager().Texture (aTextureId);
	if(!texture)
		{
		ALF_THROW ( AlfDataException, EInvalidAttribute, "AlfImageVisualAttributeSetter" )
		}
	aImageVisual.SetImage (TAlfImage (*texture));	
	}


}// Alf
