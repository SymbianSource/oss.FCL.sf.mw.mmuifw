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


#ifndef ALF_RESOURCEPOOL_IMPL_H
#define ALF_RESOURCEPOOL_IMPL_H

#include <alf/alfimage.h>
#include <alf/alfmetric.h>
#include <alf/alfimageloaderutil.h>
#include <osn/ustring.h>
#include <osn/alfptrvector.h>
#include <vector>

#include "alf/alfresourcepool.h"

class CAlfTextureManager;



namespace Alf 
{
//Forward declaration


using namespace osncore;

/**
 *  ?one_line_short_description
 *  ?more_complete_description
 *
 *  @code
 *   ?good_class_usage_example(s)
 *  @endcode
 *
 *  @lib ?library
 *  @since S60 ?S60_version *** for example, S60 v3.0
 */
class ResourcePoolImpl
    {
private:

    enum ImageResourceType
        {
        imageResourceTypeSkin,
        imageResourceTypeFileSVG,
        imageResourceTypeFileOther
        };
        
    struct ResourceInstanceData
        {
        ResourceInstanceData(): mTextureId(0), mImageLoaderUtil(0), mAutoSizeTexture(false) {}
        ~ResourceInstanceData() { delete mImageLoaderUtil; }
        
        int mTextureId;
        CAlfAutoSizeImageLoaderUtil* mImageLoaderUtil;
        bool mAutoSizeTexture;
        };

    struct ImageResource
        {
        ImageResource();
        virtual ~ImageResource();
        
        Utf8* mTag;
        ImageResourceType mType;
        AlfPtrVector<ResourceInstanceData> mLoadedTextures;
        TAlfXYMetric mInitialSizeHint;
        ResourcePool::AspectRatio mAspectRatio;
        int mReferenceCount;
        TAlfTextureFlags mFlag;
        };
        
    struct SkinImageResource : public ImageResource
        {
        int mSkinIdMajor;
        int mSkinIdMinor;
        UString mFallBackFileName;
        int mFallBackIndex;
        int mFallBackMaskIndex;
        };
        
    struct FileImageResource : public ImageResource
        {
        UString mFileName;
        };
public:

    ResourcePoolImpl( 
        CAlfTextureManager& aTextureManager ,
        ResourcePool* aParentPool );
    ~ResourcePoolImpl();

    void createLogicalImageResource( const Utf8* aTag );
    
    void createThemeImageResource( 
        const Utf8* aTag,
        const UString& aThemeDefinition );
    
    void createFileImageResource( 
        const Utf8* aTag,
        const UString& aFileName ,TAlfTextureFlags aFlag);
     
    void deleteImageResource( const Utf8* aTag );
    
    bool hasImageResource( const Utf8* aTag ) const;
      
    void setInitialSize( 
        const Utf8* aTag, 
        const TAlfXYMetric& aInitialSizeHint );
        
    void setAspectRatio( 
        const Utf8* aTag, 
        ResourcePool::AspectRatio aAspectRatio );

    TAlfImage getImageResource( const Utf8* aTag );

    TAlfImage getImageResource( 
        const Utf8* aTag, 
        const TAlfXYMetric& aSizeHint );

private:
    ImageResource* findResource( const Utf8* aTag ) const;
    int findResourceIndex( const Utf8* aTag ) const;
    void DetermineSkinInstanceL( const Utf8* aTag, TAknsItemID& aSkinItemID ) const;
    TSize determineSizeInPixels( const TAlfXYMetric& aSize );
    void determineSkinId( const UString& aNumberString, int& aSkinItem );
    TAlfImage CreateSkinImageResourceL(SkinImageResource& aSkinImageResource, const TAlfXYMetric& aSizeHint);
    TAlfImage CreateFileImageResourceL(FileImageResource& aFileImageResource, const TAlfXYMetric& aSizeHint ,TAlfTextureFlags aFlag);
    TAlfImage CreateSVGImageResourceL(FileImageResource& aSVGImageResource, const TAlfXYMetric& aSizeHint);
    
    static bool areSizesCloseEnough( const TSize& aSize1, const TSize& aSize2 );
private:

    CAlfTextureManager& mTextureManager;
    ResourcePool* mParentPool;

    AlfPtrVector<ImageResource> mResources; // use hash table instead?
    };
    
} // namespace Alf

#endif // ALF_RESOURCEPOOL_IMPL_H
