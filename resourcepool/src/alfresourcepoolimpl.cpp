/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Implementation of the resource pool
*
*/


// From this component
#include "alfresourcepoolimpl.h"

// From the same subsystem
#include <osn/osnnew.h>
#include <alf/alfimageloaderutil.h>
#include <alf/alfutil.h>
#include <alf/alfdisplay.h>
#include <alf/alfwidgetenvextension.h>

// Outside the same subsystem
#include <AknsItemID.h>
#include <utf.h>

// standard C++/STL
#include <algorithm>
#include <cstdlib>
#include <stdexcept>

namespace Alf 
{

#define KResourcePoolHexadecimalPrefix "0x"

const unsigned int INITIAL_IMAGE_WIDTH = 0; // no magic
const unsigned int INITIAL_IMAGE_HEIGHT = 0; // no magic

const TAlfTextureFlags IMAGE_RESOURCE_DEFAULT_TEXTURE_FLAGS = TAlfTextureFlags(EAlfTextureFlagAutoSize);
const TAlfTextureFlags IMAGE_RESOURCE_SKINNED_TEXTURE_FLAGS = TAlfTextureFlags(EAlfTextureFlagAutoSize | EAlfTextureFlagSkinContent);

// Function is Not used.
/**********************************
Utf8* alloc8BitBuffer( const Utf8* aSource )
    {
    int length( 0 );
    while ( aSource && aSource[length] != '\0' )
        {
        length++;
        }
    length++; // for the NULL characher
    
    Utf8* result = new (EMM) Utf8[ length ];
    
    for ( int c = 0; c < ( length - 1 ); c++ )
        {
        result[c] = aSource[c];
        }
        
    result[length - 1] = '\0'; // add the NULL character at the end
    return result;
    }
********************************************/    
 
Utf8* allocLowerCase( const Utf8* aTag )
    {
    const unsigned int length = aTag ? strlen(aTag) : 0;
    Utf8* result = new (EMM) Utf8[ length+1 ];
    for ( int s = 0 ; s < length ; s++ )
        {
        result[s] = tolower( aTag[s]);
        }
    result[length] = '\0'; // add the NULL character at the end
    return result;
    }
    
TInt RoundFloatToInt(TReal32 aVal)
	{
	return (aVal < 0 ? (TInt)(aVal - 0.5f) : (TInt)(aVal + 0.5f));
	}

// ======== MEMBER FUNCTIONS ========

ResourcePoolImpl::ImageResource::ImageResource()
: mTag(0), mAspectRatio( ResourcePool::aspectRatioPreserved ), mReferenceCount(1)
    {
    mInitialSizeHint.iX.iMagnitude = INITIAL_IMAGE_WIDTH;
    mInitialSizeHint.iX.iUnit = EAlfUnitPixel;
    mInitialSizeHint.iY.iMagnitude = INITIAL_IMAGE_HEIGHT;
    mInitialSizeHint.iY.iUnit = EAlfUnitPixel;
    }
    
ResourcePoolImpl::ImageResource::~ImageResource()
    {
    delete mTag;
    }

// ---------------------------------------------------------------------------
// ?description_if_needed
// ---------------------------------------------------------------------------
//
ResourcePoolImpl::ResourcePoolImpl( 
    CAlfTextureManager& aTextureManager,
    ResourcePool* aParentPool )
 : mTextureManager( aTextureManager ),
   mParentPool( aParentPool )
    {
    //This may throw an exception leading to object creation failure.
    
    }

// ---------------------------------------------------------------------------
// ?description_if_needed
// ---------------------------------------------------------------------------
//
ResourcePoolImpl::~ResourcePoolImpl()
    {

  
    // delete the texture instances
    for ( int resourceIndex = 0 ; resourceIndex < mResources.count() ; resourceIndex++ )
        {
        ImageResource* resource = mResources[resourceIndex];
        for ( int textureIndex = 0 ; textureIndex < resource->mLoadedTextures.count() ; textureIndex++ )
            {
            const CAlfTexture* texture = mTextureManager.Texture( resource->mLoadedTextures[textureIndex]->mTextureId);
            if ( texture->Id() == resource->mLoadedTextures[textureIndex]->mTextureId )
                {
                // make sure we do not delete blank texure
                delete texture;

                if (resource->mLoadedTextures[textureIndex]->mAutoSizeTexture)
                    {
                    mTextureManager.RemoveAutoSizeObserver(resource->mLoadedTextures[textureIndex]->mImageLoaderUtil);        
                    }            
                }
            }
        }
    // items in mResources will be automatically deleted in the destructor of
    // the AlfPtrVector 
    }


void ResourcePoolImpl::createLogicalImageResource( const Utf8* aTag )
    {
    }

void ResourcePoolImpl::createThemeImageResource(
    const Utf8* aTag,
    const UString& aThemeDefinition )
    {
    	
    	
    }
    
void ResourcePoolImpl::createFileImageResource( 
    const Utf8* aTag,
    const UString& aFileName , TAlfTextureFlags aFlag)
    {
    if ( !aTag ) 
        {
        throw invalid_argument("NULL ptr");
        }
    auto_ptr<Utf8> lowerCaseTag( allocLowerCase( aTag ) );
    
    ImageResource* existingResource = findResource( lowerCaseTag.get() );
    if ( existingResource )
        {
        existingResource->mReferenceCount++;
        return;
        }
        
    // : check that the file exists.
    
    auto_ptr<FileImageResource> newFileResource( new (EMM) FileImageResource );
    newFileResource->mTag = lowerCaseTag.get();
    lowerCaseTag.release();
    newFileResource->mType = imageResourceTypeFileOther;
    newFileResource->mFileName = aFileName;
    newFileResource->mFlag = aFlag;
    // check the .svg prefix
    const long length = aFileName.getCharLength();
    if ( length > 4 )
        {
        auto_ptr<Utf8> lowerCaseTag( allocLowerCase( aFileName.getUtf8() ) );
        if ( lowerCaseTag.get()[length-4] == '.' &&
             lowerCaseTag.get()[length-3] == 's' &&
             lowerCaseTag.get()[length-2] == 'v' &&
             lowerCaseTag.get()[length-1] == 'g' )
            {
            newFileResource->mType = imageResourceTypeFileSVG;
            }
        }
    
    mResources.resize(mResources.count()+1);
    mResources.insert(mResources.count(),newFileResource.get());
    newFileResource.release();
    }


// ---------------------------------------------------------------------------
// ?implementation_description
// ---------------------------------------------------------------------------
//    
void ResourcePoolImpl::deleteImageResource( const Utf8* aTag )
    {
    auto_ptr<Utf8> lowerCaseTag( allocLowerCase( aTag ) );
    
    ImageResource* existingResource = findResource( lowerCaseTag.get() );
    if ( !existingResource )
        {
        return;
        }
    
    existingResource->mReferenceCount--;
    if ( existingResource->mReferenceCount > 0 )
        {
        // not time to delete yet.
        return;
        }
    
    const int resouceIndex = findResourceIndex( lowerCaseTag.get() );

    // delete CAlfTexture instances created by this resource
    if ( resouceIndex != -1 )
       {
       ImageResource* resource = mResources[resouceIndex];
        for ( int textureIndex = 0 ; textureIndex < resource->mLoadedTextures.count() ; textureIndex++ )
            {
            const CAlfTexture* texture = mTextureManager.Texture( resource->mLoadedTextures[textureIndex]->mTextureId);
            if ( texture->Id() == resource->mLoadedTextures[textureIndex]->mTextureId )
                {
                // make sure we do not delete blank texure
                delete texture;
                
                if (resource->mLoadedTextures[textureIndex]->mAutoSizeTexture)
                    {
                    mTextureManager.RemoveAutoSizeObserver(resource->mLoadedTextures[textureIndex]->mImageLoaderUtil);        
                    }            
                }
            }
        
        mResources.remove( resouceIndex );
       }
    }

// ---------------------------------------------------------------------------
// ?implementation_description
// ---------------------------------------------------------------------------
//    
bool ResourcePoolImpl::hasImageResource( const Utf8* aTag ) const
    {
    auto_ptr<Utf8> lowerCaseTag( allocLowerCase( aTag ) );
    int resouceIndex = findResourceIndex( lowerCaseTag.get() );
    if ( resouceIndex != -1 )
        {
        return true;
        }
        
    return false;
    }

// ---------------------------------------------------------------------------
// ?implementation_description
// ---------------------------------------------------------------------------
//      
void ResourcePoolImpl::setInitialSize( 
    const Utf8* aTag, 
    const TAlfXYMetric& aInitialSizeHint )
    {
    auto_ptr<Utf8> lowerCaseTag( allocLowerCase( aTag ) );
    ImageResource* imageResource = findResource( lowerCaseTag.get() );
    if ( imageResource )
        {
        imageResource->mInitialSizeHint = aInitialSizeHint;
        }
    }
    
// ---------------------------------------------------------------------------
// ?implementation_description
// ---------------------------------------------------------------------------
//     
void ResourcePoolImpl::setAspectRatio( 
        const Utf8* aTag, 
        ResourcePool::AspectRatio aAspectRatio )
    {
    auto_ptr<Utf8> lowerCaseTag( allocLowerCase( aTag ) );
    ImageResource* imageResource = findResource( lowerCaseTag.get() );
    if ( imageResource )
        {
        imageResource->mAspectRatio = aAspectRatio;
        }
    }

// ---------------------------------------------------------------------------
// ?implementation_description
// ---------------------------------------------------------------------------
//
TAlfImage ResourcePoolImpl::getImageResource( const Utf8* aTag )
    {
    auto_ptr<Utf8> lowerCaseTag( allocLowerCase( aTag ) );
    ImageResource* imageResource = findResource( lowerCaseTag.get() );
    if ( imageResource )
        {
        return getImageResource( 
            lowerCaseTag.get() , 
            imageResource->mInitialSizeHint );
        }
    return TAlfImage(); // return empty
    }

// ---------------------------------------------------------------------------
// ?implementation_description
// ---------------------------------------------------------------------------
//
TAlfImage ResourcePoolImpl::getImageResource( 
    const Utf8* aTag, 
    const TAlfXYMetric& aSizeHint )
    {
    auto_ptr<Utf8> lowerCaseTag( allocLowerCase( aTag ) );
    
    ImageResource* imageResource = findResource( lowerCaseTag.get() );
    if ( imageResource )
        {
        // check if the texture ID with the given size hint is already generated
        if ( imageResource->mLoadedTextures.count() )
            {
            const TSize requestedSize( determineSizeInPixels(aSizeHint) );
            const CAlfTexture* texture = NULL;
            for ( TInt idIndex = 0 ; idIndex < imageResource->mLoadedTextures.count() ; idIndex++ )
                {
                texture = mTextureManager.Texture(imageResource->mLoadedTextures[idIndex]->mTextureId);
                if ( texture->Id() != imageResource->mLoadedTextures[idIndex]->mTextureId )
                    {
                    // if the texture manager returns blank, return empty.
                    return TAlfImage(); // return empty
                    }
                TSize textureSize( const_cast<CAlfTexture*>(texture)->MaxTextureSize() );
                if ( textureSize.iWidth == 0 || textureSize.iHeight == 0 )
                    {
                    // texture loaded directly with the size, use that.
                    textureSize = texture->Size();
                    }
                
                const bool autosize = imageResource->mLoadedTextures[idIndex]->mAutoSizeTexture;
                if ( requestedSize == TSize(0,0) && autosize )
                    {
                    // if zero size is requested, we think autosizing image is requested
                    // and if we find one autosizing texture we return it.
                    return TAlfImage( *texture );                        
                    }
                else if ( areSizesCloseEnough( requestedSize, textureSize ) && !autosize )
                    {
                    // we found existing texure -> use that
                    return TAlfImage( *texture );
                    }
                }
            }
            
        // If we have not found a suitable resource, create one.    
        TAlfImage result;
        TRAPD( sError , 
            {
                
            // Create new one.
            switch( imageResource->mType )
                {
                case imageResourceTypeSkin:
                    result = CreateSkinImageResourceL( 
                        static_cast<SkinImageResource&>( *imageResource ), aSizeHint );
                    break;
                    
                case imageResourceTypeFileOther:
                    result = CreateFileImageResourceL( 
                        static_cast<FileImageResource&>( *imageResource ), aSizeHint , imageResource->mFlag );
                    break;
                    
                case imageResourceTypeFileSVG:
                    result = CreateSVGImageResourceL( 
                        static_cast<FileImageResource&>( *imageResource ), aSizeHint );
                    break;
                    
                default:
                    break;
                }
            }); // TRAP
                
        if ( sError != KErrNone  )
            {
            throw invalid_argument("cannot create image resource");
            }
        return result;
        }
    else
        {
        // this pool does not contain the resource, check the parent pool
        if ( mParentPool )
            {
            return mParentPool->getImageResource( lowerCaseTag.get(), aSizeHint );
            }
        }
    
    return TAlfImage(); // return empty
    }

// ---------------------------------------------------------------------------
// ?implementation_description
// ---------------------------------------------------------------------------
//
ResourcePoolImpl::ImageResource* ResourcePoolImpl::findResource( const Utf8* aTag ) const
    {
    const int index = findResourceIndex( aTag );
    if ( index != -1 )
        {
        return mResources.at(index);
        }
    return NULL;
    }

// ---------------------------------------------------------------------------
// ?implementation_description
// ---------------------------------------------------------------------------
//    
int ResourcePoolImpl::findResourceIndex( const Utf8* aTag ) const
    {
    for ( int i = 0 ; i < mResources.count() ; i++ )
        {
        // : find a better way to compare
        int c = 0;
        while ( mResources.at(i)->mTag[c] != '\0' )
            {
            if ( mResources.at(i)->mTag[c] != aTag[c] )
                {
                break;
                }
            c++;
            }
            
        if ( mResources.at(i)->mTag[c] == '\0' && aTag[c] == '\0' )
            {
            return i;
            }
        }
    return -1;
    }

// ---------------------------------------------------------------------------
// ?implementation_description
// ---------------------------------------------------------------------------
//
void ResourcePoolImpl::DetermineSkinInstanceL( 
    const Utf8* aTag, TAknsItemID& aSkinItemID ) const
    {
    }

// ---------------------------------------------------------------------------
// ?implementation_description
// ---------------------------------------------------------------------------
//
TSize ResourcePoolImpl::determineSizeInPixels( const TAlfXYMetric& aSize )
    {
    TSize result(0,0);
    TRect displayRect(0,0,0,0);
    
    // check X
    if ( aSize.iX.iUnit == EAlfUnitPixel )
        {
        result.iWidth = aSize.iX.iMagnitude;
        }
    else if ( aSize.iX.iUnit == EAlfUnitDisplaySize )
        {
        // check the primary display
        if ( mTextureManager.Env().DisplayCount() )
            {
            const CAlfDisplay& display = mTextureManager.Env().PrimaryDisplay();
            displayRect = display.VisibleArea();
            result.iWidth = RoundFloatToInt( TReal32(displayRect.Width())*aSize.iX.iUnit );
            }
        }
        
    // check Y
    if ( aSize.iY.iUnit == EAlfUnitPixel )
        {
        result.iHeight = aSize.iY.iMagnitude;
        }
    else if ( aSize.iY.iUnit == EAlfUnitDisplaySize )
        {
        // check the primary display (if not checked already
        if ( !displayRect.Height() && mTextureManager.Env().DisplayCount() )
            {
            const CAlfDisplay& display = mTextureManager.Env().PrimaryDisplay();
            displayRect = display.VisibleArea();
            }
        result.iHeight = RoundFloatToInt( TReal32(displayRect.Height())*aSize.iY.iUnit );
        }
        
    return result;
    }

// ---------------------------------------------------------------------------
// ?implementation_description
// ---------------------------------------------------------------------------
//
void ResourcePoolImpl::determineSkinId( const UString& aNumberString, int& aSkinItem )
    {
    aSkinItem = -1;
    
    const unsigned int charLength = aNumberString.getCharLength();
    if ( charLength )
        {
        // there is something
        // check if we are dealing with hexadecimal entry
        const string strNumber = aNumberString.getUtf8();
        if ( charLength > 3 && strNumber.substr(0,2).compare( KResourcePoolHexadecimalPrefix ) == 0 )
            {
            // hexa it is
            aSkinItem = strtol( aNumberString.getUtf8(), NULL, 16 );
            }
        else
            {
            // try normal decimal number
            aSkinItem = atoi( aNumberString.getUtf8() );
            // if the 'atoi' returns 0, make sure it is that
            if ( aSkinItem == 0 )
                {
                if ( aNumberString.getUtf8()[0] != '0' )
                    {
                    aSkinItem = -1;
                    }
                }
            }
        }
    }


// ---------------------------------------------------------------------------
// ?implementation_description
// ---------------------------------------------------------------------------
//
TAlfImage ResourcePoolImpl::CreateSkinImageResourceL(
    SkinImageResource& aSkinImageResource, 
    const TAlfXYMetric& aSizeHint)
    {
    CAlfAutoSizeImageLoaderUtil* imageLoaderUtil = new (ELeave) CAlfAutoSizeImageLoaderUtil;
    CleanupStack::PushL( imageLoaderUtil );
    CAlfTexture* texture = NULL;
    MAlfBitmapProvider* bitmapProvider = NULL;

    TAknsItemID skinId;
    skinId.Set( aSkinImageResource.mSkinIdMajor, aSkinImageResource.mSkinIdMinor );

    TSize size( determineSizeInPixels(aSizeHint) );
    imageLoaderUtil->SetSize( size, static_cast<TScaleMode>(aSkinImageResource.mAspectRatio));
    
    TInt textureFlags = IMAGE_RESOURCE_SKINNED_TEXTURE_FLAGS;        
    if (size != TSize(0,0))
        {
        // If sizehint was provided, turn autosize off at least for now.
        textureFlags &= ~EAlfTextureFlagAutoSize;          
        }
            
    if ( aSkinImageResource.mFallBackFileName.getCharLength() )
        {    
        HBufC* sUnicodeBuffer = 
            CnvUtfConverter::ConvertToUnicodeFromUtf8L(
                TPtrC8((TUint8*)aSkinImageResource.mFallBackFileName.getUtf8()));
        CleanupStack::PushL( sUnicodeBuffer );
        bitmapProvider =   
            imageLoaderUtil->CreateImageLoaderL( 
                skinId, 
                *sUnicodeBuffer, 
                aSkinImageResource.mFallBackIndex,
                aSkinImageResource.mFallBackMaskIndex );
        CleanupStack::PopAndDestroy( sUnicodeBuffer );
        }
    else
        {
        bitmapProvider =  
            imageLoaderUtil->CreateImageLoaderL( 
                skinId, 
                KNullDesC(), 
                -1,
                -1 );
        }
        
    texture = &mTextureManager.CreateTextureL( KAlfAutoGeneratedTextureId, bitmapProvider, TAlfTextureFlags(textureFlags));
    CleanupStack::PushL( texture );
    
    // Enable ref counting
    texture->EnableRefCounting();

    // store texture ID and create image
    ResourceInstanceData* newResourceInstance = new (ELeave) ResourceInstanceData;
    newResourceInstance->mTextureId = texture->Id();
    newResourceInstance->mImageLoaderUtil = imageLoaderUtil;
    newResourceInstance->mAutoSizeTexture = (textureFlags & EAlfTextureFlagAutoSize);
    
    try 
        {
        aSkinImageResource.mLoadedTextures.resize(aSkinImageResource.mLoadedTextures.count()+1);
        aSkinImageResource.mLoadedTextures.insert(aSkinImageResource.mLoadedTextures.count(),newResourceInstance);
        }
    catch ( ... )
        {
        delete newResourceInstance;
        User::Leave( KErrNoMemory );
        }
    CleanupStack::Pop( texture );
    CleanupStack::Pop( imageLoaderUtil ); // mLoadedTextures has taken the ownership

    if (textureFlags & EAlfTextureFlagAutoSize)
        {
        mTextureManager.AddAutoSizeObserverL(imageLoaderUtil);    
        }    

    return TAlfImage( *texture );
    }

// ---------------------------------------------------------------------------
// ?implementation_description
// ---------------------------------------------------------------------------
//
TAlfImage ResourcePoolImpl::CreateFileImageResourceL(
    FileImageResource& aFileImageResource, 
    const TAlfXYMetric& aSizeHint ,  TAlfTextureFlags aFlag)
    {
    CAlfTexture* texture = NULL;
   
    HBufC* sUnicodeBuffer = 
        CnvUtfConverter::ConvertToUnicodeFromUtf8L(
            TPtrC8((TUint8*)aFileImageResource.mFileName.getUtf8()));
    CleanupStack::PushL( sUnicodeBuffer );
    TSize size(determineSizeInPixels( aSizeHint ) );
    
    TInt textureFlags = IMAGE_RESOURCE_DEFAULT_TEXTURE_FLAGS;
    textureFlags |= aFlag;
    if (size != TSize(0,0))
        {
        // If sizehint was provided, turn autosize off at least for now.
        textureFlags &= ~EAlfTextureFlagAutoSize;          
        }

    if ( size.iWidth && size.iHeight )
        {
        texture = &mTextureManager.LoadTextureL( 
            *sUnicodeBuffer,
            size, 
            TAlfTextureFlags(textureFlags), 
            KAlfAutoGeneratedTextureId );
        }
    else
        {
        texture = &mTextureManager.LoadTextureL( 
            *sUnicodeBuffer, 
            TAlfTextureFlags(textureFlags), 
            KAlfAutoGeneratedTextureId );
        }
    CleanupStack::PopAndDestroy( sUnicodeBuffer );
    CleanupStack::PushL( texture );

    // Enable ref counting
    texture->EnableRefCounting();

    // store texture ID and create image
    ResourceInstanceData* newResourceInstance = new (ELeave) ResourceInstanceData;
    newResourceInstance->mTextureId = texture->Id();
    newResourceInstance->mAutoSizeTexture = (textureFlags & EAlfTextureFlagAutoSize);
    
    try 
        {
        aFileImageResource.mLoadedTextures.resize(aFileImageResource.mLoadedTextures.count()+1);
        aFileImageResource.mLoadedTextures.insert(aFileImageResource.mLoadedTextures.count(),newResourceInstance);
        }
    catch ( ... )
        {
        delete newResourceInstance;
        User::Leave( KErrNoMemory );
        }
    
    CleanupStack::Pop( texture );
    return TAlfImage( *texture );
    }

// ---------------------------------------------------------------------------
// ?implementation_description
// ---------------------------------------------------------------------------
//
TAlfImage ResourcePoolImpl::CreateSVGImageResourceL(
    FileImageResource& aSVGImageResource, 
    const TAlfXYMetric& aSizeHint)
    {        
    CAlfAutoSizeImageLoaderUtil* imageLoaderUtil = new (ELeave) CAlfAutoSizeImageLoaderUtil;
    CleanupStack::PushL( imageLoaderUtil );
    MAlfBitmapProvider* bitmapProvider = NULL;
    CAlfTexture* texture = NULL;

    TSize size( determineSizeInPixels(aSizeHint) );
    imageLoaderUtil->SetSize( size, static_cast<TScaleMode>(aSVGImageResource.mAspectRatio) );

    TInt textureFlags = IMAGE_RESOURCE_SKINNED_TEXTURE_FLAGS;
    if (size != TSize(0,0))
        {
        // If sizehint was provided, turn autosize off at least for now.
        textureFlags &= ~EAlfTextureFlagAutoSize;          
        }
            
    HBufC* sUnicodeBuffer = 
        CnvUtfConverter::ConvertToUnicodeFromUtf8L(
            TPtrC8((TUint8*)aSVGImageResource.mFileName.getUtf8()));
    CleanupStack::PushL( sUnicodeBuffer );
    bitmapProvider = imageLoaderUtil->CreateSVGImageLoaderL( *sUnicodeBuffer );
    CleanupStack::PopAndDestroy( sUnicodeBuffer );
    
    texture = &mTextureManager.CreateTextureL( KAlfAutoGeneratedTextureId, bitmapProvider, TAlfTextureFlags(textureFlags));
    CleanupStack::PushL( texture );
    
    // Enable ref counting
    texture->EnableRefCounting();
    
    // store texture ID and create image
    ResourceInstanceData* newResourceInstance = new (ELeave) ResourceInstanceData;
    newResourceInstance->mTextureId = texture->Id();
    newResourceInstance->mImageLoaderUtil = imageLoaderUtil;
    newResourceInstance->mAutoSizeTexture = (textureFlags & EAlfTextureFlagAutoSize);
    
    try 
        {
        aSVGImageResource.mLoadedTextures.resize(aSVGImageResource.mLoadedTextures.count()+1);
        aSVGImageResource.mLoadedTextures.insert(aSVGImageResource.mLoadedTextures.count(),newResourceInstance);
        }
    catch ( ... )
        {
        delete newResourceInstance;
        User::Leave( KErrNoMemory );
        }
    CleanupStack::Pop( texture );
    CleanupStack::Pop( imageLoaderUtil ); // mLoadedTextures has taken the ownership 

    if (textureFlags & EAlfTextureFlagAutoSize)
        {
        mTextureManager.AddAutoSizeObserverL(imageLoaderUtil);    
        }    

    return TAlfImage( *texture );
    }
    
bool ResourcePoolImpl::areSizesCloseEnough( const TSize& aSize1, const TSize& aSize2 )
    {
    // this is the inteligent part:)
    // the function tries to determine do we need to rasterize a new texture or use the existing one.
   
    // check if the other one is zero size
    if ( !aSize1.iWidth || !aSize1.iHeight || !aSize2.iWidth || !aSize2.iHeight )
        {
        return false;
        }
        
    // check if both are smaller than 10px
    if ( aSize1.iWidth < 10 && aSize1.iHeight < 10 && aSize2.iWidth < 10 && aSize2.iHeight < 10 )
        {
        return true;
        }
        
    // check if both dimensions fit in 10% range.
    bool result = false;
    // check width
    if ( aSize1.iWidth <= aSize2.iWidth*1.1f && aSize1.iWidth >= aSize2.iWidth*0.9f )
        {
        // width in the range, chech height
        if ( aSize1.iHeight <= aSize2.iHeight*1.1f && aSize1.iHeight >= aSize2.iHeight*0.9f )
            {
            // height in the range
            result = true;
            }
        }
    
    return result;
    }
    
} // namespace Alf

