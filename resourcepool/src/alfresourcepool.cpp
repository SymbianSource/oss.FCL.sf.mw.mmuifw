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
* Description:  API for handling alf graphic resources
*
*/


#include "alf/alfresourcepool.h"
#include "alfresourcepoolimpl.h"

#include <osn/osnnew.h>

namespace Alf 
{

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// ?description_if_needed
// ---------------------------------------------------------------------------
//
OSN_EXPORT ResourcePool::ResourcePool( 
    CAlfTextureManager& aTextureManager,
    ResourcePool* aParentPool ) 
    : mResourcePoolImpl( 
        new (EMM) ResourcePoolImpl( aTextureManager, aParentPool ) )
    {
    }


// ---------------------------------------------------------------------------
// ?description_if_needed
// ---------------------------------------------------------------------------
//
OSN_EXPORT ResourcePool::~ResourcePool()
    {
    }


// ---------------------------------------------------------------------------
// ?implementation_description
// ---------------------------------------------------------------------------
//
OSN_EXPORT void ResourcePool::createLogicalImageResource( const Utf8* aTag )
    {
    mResourcePoolImpl->createLogicalImageResource( aTag );
    }

// ---------------------------------------------------------------------------
// ?implementation_description
// ---------------------------------------------------------------------------
//    
OSN_EXPORT void ResourcePool::createThemeImageResource( 
        const Utf8* aTag,
        const UString& aThemeDefinition )
    {
    mResourcePoolImpl->createThemeImageResource( 
        aTag,
        aThemeDefinition );
    }

// ---------------------------------------------------------------------------
// ?implementation_description
// ---------------------------------------------------------------------------
//    
OSN_EXPORT void ResourcePool::createFileImageResource( 
        const Utf8* aTag,
        const UString& aFileName ,
        TAlfTextureFlags aFlag)
    {
    mResourcePoolImpl->createFileImageResource( aTag, aFileName, aFlag);
    }

// ---------------------------------------------------------------------------
// ?implementation_description
// ---------------------------------------------------------------------------
//    
OSN_EXPORT void ResourcePool::deleteImageResource( const Utf8* aTag )
    {
    mResourcePoolImpl->deleteImageResource( aTag );
    }

// ---------------------------------------------------------------------------
// ?implementation_description
// ---------------------------------------------------------------------------
//    
OSN_EXPORT bool ResourcePool::hasImageResource( const Utf8* aTag ) const
    {
    return mResourcePoolImpl->hasImageResource( aTag );
    }

// ---------------------------------------------------------------------------
// ?implementation_description
// ---------------------------------------------------------------------------
//    
OSN_EXPORT void ResourcePool::setInitialSize( 
    const Utf8* aTag, 
    const TAlfXYMetric& aInitialSizeHint )
    {
    mResourcePoolImpl->setInitialSize( aTag, aInitialSizeHint);
    }

// ---------------------------------------------------------------------------
// ?implementation_description
// ---------------------------------------------------------------------------
//     
OSN_EXPORT void ResourcePool::setAspectRatio( 
        const Utf8* aTag, 
        AspectRatio aAspectRatio )
    {
    mResourcePoolImpl->setAspectRatio( aTag, aAspectRatio);
    }

// ---------------------------------------------------------------------------
// ?implementation_description
// ---------------------------------------------------------------------------
//
OSN_EXPORT TAlfImage ResourcePool::getImageResource( 
    const Utf8* aTag )
    {
    return mResourcePoolImpl->getImageResource( aTag );
    }

// ---------------------------------------------------------------------------
// ?implementation_description
// ---------------------------------------------------------------------------
//
OSN_EXPORT TAlfImage ResourcePool::getImageResource( 
    const Utf8* aTag, 
    const TAlfXYMetric& aSizeHint )
    {
    return mResourcePoolImpl->getImageResource( aTag, aSizeHint );
    }


} // namespace Alf
