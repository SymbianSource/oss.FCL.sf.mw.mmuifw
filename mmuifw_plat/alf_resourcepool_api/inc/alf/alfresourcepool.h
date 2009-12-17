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


#ifndef ALF_RESOURCEPOOL_H
#define ALF_RESOURCEPOOL_H

#include <osn/osndefines.h>
#include <osn/ustring.h>

#include <alf/alftexture.h>
class TAlfImage;
class CAlfTextureManager;
struct TAlfXYMetric;

namespace Alf 
{

#define KResourcePoolThemeDefinitionPrefixS60 "s60"

using namespace osncore;

class ResourcePoolImpl;
 
/**
 *  Resoure pool is responsible of controlling commonly used image resources
 *  in an application. The resources will be indentified by tags, which are not
 *  case sensitive. 
 *
 *  @code
 *   // create image resources
 *   resourcePool->createLogicalImageResource( "qgn_indi_find_glass" );
 *   resourcePool->setInitialSize( "qgn_indi_find_glass", XYMetric( 64, 64 ) );
 *   resourcePool->setAspectRatio( "qgn_indi_find_glass", ResourcePool::aspectRatioPreserved );
 *
 *   resourcePool->createImageResource(
 *       "my_face",
 *       pic0001.jpg );
 *
 *   // utilize the resource
 *   imageVisual->setImage( 
 *       resourcePool->getImageResource( "qgn_indi_find_glass" ) );
 *
 *   sharpImageVisual->setImage( 
 *       resourcePool->getImageResource( "my_face" ),
 *       XYMetric( 256, 256 ) );
 *
 *   // when the resource in not used anymore, the resource can be deleted
 *   resourcePool->deleteImageResource( "qgn_indi_find_glass" );
 *   resourcePool->deleteImageResource( "my_face" );
 *   
 *  @endcode
 *
 *  @lib alfresourcepool.lib
 *  @since S60 v5.0.1
 */
class ResourcePool
    {
public:

    // Aspect ratio of the image resource
    // @see setAspectRatio
    enum AspectRatio
        {
        /*
        * Scales the icon to the maximum size that fits in the given size,
        * whilst preserving the aspect ratio of the icon. The sizes of the resulting
        * bitmaps are exactly of the given size. If the aspect ratio of
        * the given size differs from the aspect ratio of the icon,
        * the resulting bitmaps will contain an unused area.
        */
        aspectRatioPreserved = 0,

        /*
        * Scales the icon to the maximum size that fits in the given size,
        * whilst preserving the aspect ratio of the icon. The resulting bitmaps
        * are resized so that any unused portion of the given size is not
        * present in them.
        *
        * This mode should be used when only the height or the width of the icon
        * is known and the other should be based on the aspect ratio of the icon.
        * The unknown dimension should be defined high enough so
        * that it does not limit the scaling based on the aspect ratio of the icon.
        */
        aspectRatioPreservedAndUnusedSpaceRemoved = 1,

        /*
        * Scales the icon exactly to the given size. Does not preserve the aspect
        * ratio of the icon.
        */
        aspectRatioNotPreserved = 2,
            
        /*
        * Scales the icon to the minimum size that covers the given size,
        * whilst preserving the aspect ratio of the icon. The sizes of the resulting
        * bitmaps are exactly of the given size. If the aspect ratio of
        * the given size differs from the aspect ratio of the icon, some parts of the 
        * icon will be sliced from the resulting bitmaps.
        */
        aspectRatioPreservedSlice = 3
        };
    
public:

    /** 
     * Constructs a resource pool
     *
     * @param aTextureManager Used texture manager.
     * @param aParentPool Chained parent pool (optional) - not supported currently
     */
    OSN_IMPORT ResourcePool( 
        CAlfTextureManager& aTextureManager,
        ResourcePool* aParentPool = NULL);
    
    /**
     * Destructor 
     */
    OSN_IMPORT ~ResourcePool();
    
public: 
    
    /**
     * Creates an image resource based on a logical name. The name will
     * be also used as the image resource tag because the logical name
     * is unique in the system.
     *
     * The logical name should be format "qgn_xxx_xxx" i.e. as stated
     * in the S60 reference icons document.
     *
     * @note The KAknsIIDQgnXxxXxx format works for a while but it will be 
     *       deprecated -> use the qgn_xxx_xxx format.
     *
     * @param aTag Resource tag. NULL terminated 8-bit string
     *
     * @exception invalid_argument if the tag cannot be matched with any 
     *                              logical name
     */
    OSN_IMPORT void createLogicalImageResource( const Utf8* aTag );
    
    /**
     * Creates an image resource based on a theme definition. This definition
     * may concists of several different theme systems.
     * 
     * Currently supported formats:
     * #1 S60 Skin; 
     *    s60;mifFilePath;mifImageIndex;mifMaskIndex;majorID;minorId
     *
     *    s60:          literal "s60", which specifies that the following
     *                  item definition is based on the S60 Skin system
     *    mifFilePath:  literal into the multibitmap file (mif/mbm). Can be empty. 
     *    mifImageIndex:unsigned integer which is the index to the mifFilePath. Only used if mifFilePath is given.
     *    mifMaskIndex: unsigned integer which is the mask index to the mifFilePath. Only used if mifFilePath is given.
     *    majorID:      unsigned integer or hexadecimal starting with 0x of the Major skin ID. Can be empty.
     *    minorId:      unsigned integer or hexadecimal starting with 0x of the Minor skin ID. Only used if majorID is given.
     *    
     *    examples:     "s60;;;;268457670;76" // 0x100056c6;0x4c -> KAknsIIDQgnIndiFindGlass
     *                  "s60;c:\myFile.mif;1;2;;"
     *                  "s60;myMbmFile.mbm;435;;2342345;234" // If skin ID not found, use mbm file with index 435 (no mask)
     *
     * @param aTag Resource tag.
     * @param aThemeDefinition Definition of the theme element.
     *
     * @exception invalid_argument theme definition is not supported.
     */
    OSN_IMPORT void createThemeImageResource( 
        const Utf8* aTag,
        const UString& aThemeDefinition );
    
    /**
     * Creates an image resource based on a file name.
     * The aFileName can be a full path with the drive letter or it
     * can be relative path to the texture manager defaul path
     *
     * @param aTag Resource tag.
     * @param aFileName File name.
     * @param aFlag How to convert the bitmap to texture
     *               default all fileimages will be treated as nonanimated
     * @see TextureManager::setImagePath()
     */
    OSN_IMPORT void createFileImageResource( 
        const Utf8* aTag,
        const UString& aFileName ,TAlfTextureFlags aFlag = EAlfTextureFlagLoadAnimAsImage );
    
    /**
     * Deletes image resource
     *
     * @param aTag Resource tag.
     */    
    OSN_IMPORT void deleteImageResource( const Utf8* aTag );
    
    /**
     * Checks if the given resource identifier exists.
     *
     * @param aTag Resource tag.
     *
     * @return 'true' if the indentified resource exists.
     */
    OSN_IMPORT bool hasImageResource( const Utf8* aTag ) const;
    
    /**
     * Sets initial/default loading size for the image resource. The given size
     * is only a hint and the resource pool may used it or ignore it. Without the 
     * initial size hint, the image will be rasterized to size determined by the 
     * resource pool. The initial size hint can be overridden with the appropriate
     * getImageResource() function.
     *
     * @param aTag Resource tag.
     * @param aInitialSizeHint Initial/default size hint (only Pixel and RelativeToDisplay supported)
     */    
    OSN_IMPORT void setInitialSize( 
        const Utf8* aTag, 
        const TAlfXYMetric& aInitialSizeHint );
        
    /**
     * Sets aspect ratio mode. This affects to the aspect ratio of the loaded image
     * resource. The new setting will take place when the image resource is loaded.
     *
     * @note Currently only SVG icons are supported. For file image loading, the aspect ratio
     *       is preserved.
     *
     * @note The blitting of the image resource can still scale and change the aspect ratio
     *       of the drawn image. @see ImageVisual::setScaleMode
     *
     * @param aTag Resource tag.
     * @param aAspectRatio Used aspect ratio.
     */    
    OSN_IMPORT void setAspectRatio( 
        const Utf8* aTag, 
        AspectRatio aAspectRatio );

    /**
     * Finds a image resource with the given tag. The initial size or system
     * default size is used.
     *
     * If the given resource is not found, the Image::hasTexture() returns false.
     *
     * @param aTag Resource tag.
     * 
     * @return Image which can be passed to any visual/brush.
     *
     * @exception invalid_argument if the resource is found but cannot be instantiated
     */
    OSN_IMPORT TAlfImage getImageResource( const Utf8* aTag );

    /**
     * Finds a image resouce with the given tag. The size hint may be
     * used to find/re-rasterize the image resource. 
     * 
     * If the given resource is not found, the Image::hasTexture() returns false.
     *
     * @param aTag Resource tag.
     * @param aSizeHint Rasteration size hint (only Pixel and RelativeToDisplay supported)
     * 
     * @return Image which can be passed to any visual/brush.
     *
     * @exception invalid_argument if the resource is found but cannot be instantiated
     */
    OSN_IMPORT TAlfImage getImageResource( 
        const Utf8* aTag, 
        const TAlfXYMetric& aSizeHint );
    
private:
   
    auto_ptr<ResourcePoolImpl> mResourcePoolImpl;
    };

} // namespace Alf

#endif // ALF_RESOURCEPOOL_H
