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
* Description:  Extensions can be added to AlfEnv through this class.
               Currently resourcepool,widgetfactory can be added as extensions to AlfEnv.
*
*/


#include <alf/alfenv.h>
#include <alf/alfenvobject.h>
#include <alf/alfresourcepool.h>

#include <osn/osnnew.h>
#include <alf/alfwidgetexception.h>
#include <alf/alfwidgetenvextension.h>

#include <alf/alfwidgetfactoryloader.h>
#include <alf/ialfwidgetfactory.h>
namespace Alf
    {
    const int KAlfExtensionResourcePoolUid( 89 ); // Bogus id for resource 
                                                  // pool extension
    
    const int KAlfExtensionWidgetFactoryUid( 106 ); // Bogus id for widget
                                                    // factory extension
    
    // Env extension for the resource pool
    class AlfExtensionResourcePoolContainer : public MAlfEnvObject
        {
    public:        

        AlfExtensionResourcePoolContainer( ResourcePool* aResourcePool) :
        mResourcePool(aResourcePool )
            {
            }

        virtual void Release()
            {
            delete this;
            }

        ~AlfExtensionResourcePoolContainer()
            {
            delete mResourcePool;
            }
        // Owned
        ResourcePool* mResourcePool;
        };
        

    // Env extension for the widget factory
    class AlfExtensionWidgetFactory : public MAlfEnvObject
        {
    public:        

        AlfExtensionWidgetFactory( 
            AlfWidgetFactoryLoader* amWidgetFactoryLoader,
            IAlfWidgetFactory* aWidgetFactory) :
        mWidgetFactoryLoader( amWidgetFactoryLoader ),
        mWidgetFactory( aWidgetFactory )
            {
            }

        virtual void Release()
            {
            delete this;
            }

        ~AlfExtensionWidgetFactory()
            {
            delete mWidgetFactoryLoader;
            }
        // Owned
        AlfWidgetFactoryLoader* mWidgetFactoryLoader;
        // Not owned
        IAlfWidgetFactory* mWidgetFactory;
        };    
    
// ---------------------------------------------------------------------------
// Initializes environment with resource pool and factory extensions
// ---------------------------------------------------------------------------
//    
OSN_EXPORT void AlfWidgetEnvExtension::initializeEnvironment(CAlfEnv& aEnv)
    {
	AlfExtensionResourcePoolContainer* poolContainer = 
        static_cast<AlfExtensionResourcePoolContainer*>(
            aEnv.Extension( KAlfExtensionResourcePoolUid));
    if ( poolContainer == NULL)
        {
        // Initialize the env with resource pool extension
        initializeEnvironmentForResourcePool(aEnv);
        }
	AlfExtensionWidgetFactory* widgetFactoryExtension = 
        static_cast<AlfExtensionWidgetFactory*>(
            aEnv.Extension( KAlfExtensionWidgetFactoryUid));
    if ( widgetFactoryExtension == NULL)
        {
        // Initialize the env with widget factory extension
        initializeEnvironmentForFactory(aEnv);
        }
    }
// ---------------------------------------------------------------------------
// Initializes environment with Theme Manager extension
// ---------------------------------------------------------------------------
//   
void AlfWidgetEnvExtension::initializeEnvironmentForTheme(CAlfEnv& /*aEnv*/ )
    {
    }

// ---------------------------------------------------------------------------
// Initializes environment with resource pool extension
// ---------------------------------------------------------------------------
//        
void AlfWidgetEnvExtension::initializeEnvironmentForResourcePool(CAlfEnv& aEnv)
    {
    auto_ptr<ResourcePool> pool(new(EMM) ResourcePool(
                                             aEnv.TextureManager(), NULL));
    auto_ptr<AlfExtensionResourcePoolContainer> poolContainer(
        new(EMM) AlfExtensionResourcePoolContainer( pool.release()));
    int err = aEnv.AddExtension( KAlfExtensionResourcePoolUid,
                       (MAlfEnvObject*)(poolContainer.get()));
    if ( err != KErrNone)
        {
        ALF_THROW(AlfWidgetException, ECommonError, "Failed to add AlfEnv extension");
        }
    else
        {
        poolContainer.release();
        }   
    }
// ---------------------------------------------------------------------------
// Returns the reference for Resource Pool extension object. 
// ---------------------------------------------------------------------------
//
OSN_EXPORT ResourcePool& AlfWidgetEnvExtension::resourcePool( CAlfEnv& aEnv)
    {
    Alf::AlfExtensionResourcePoolContainer* poolContainer = 
        static_cast<Alf::AlfExtensionResourcePoolContainer*>(aEnv.Extension( KAlfExtensionResourcePoolUid));
    if ( poolContainer == NULL)
        {
        initializeEnvironment(aEnv);
        }
    //re-fetch the pool
    poolContainer = 
        static_cast<Alf::AlfExtensionResourcePoolContainer*>(aEnv.Extension( KAlfExtensionResourcePoolUid));
    return *(poolContainer->mResourcePool);
}

// ---------------------------------------------------------------------------
// Initializes environment with factory extension
// ---------------------------------------------------------------------------
//    
void AlfWidgetEnvExtension::initializeEnvironmentForFactory(CAlfEnv& aEnv)
    {
    auto_ptr<AlfWidgetFactoryLoader> widgetFactoryLoader( 
                                         new (EMM) AlfWidgetFactoryLoader() );
    
    IAlfWidgetFactory* widgetFactory = NULL;
    widgetFactory = widgetFactoryLoader->loadWidgetFactory(aEnv);
    if(!widgetFactory)
        {
        ALF_THROW(AlfWidgetException, ECommonError, "Failed to create widget factory");
        }
    auto_ptr<AlfExtensionWidgetFactory> widgetFactoryExtension(
        new(EMM) AlfExtensionWidgetFactory( widgetFactoryLoader.release(),
                     widgetFactory));
    int err = aEnv.AddExtension( KAlfExtensionWidgetFactoryUid,
                       (MAlfEnvObject*)(widgetFactoryExtension.get()));
    if ( err != 0)
        {
        ALF_THROW(AlfWidgetException, ECommonError, "Failed to add AlfEnv extension");
        }
    else
        {
    	widgetFactoryExtension.release();
        }        
    }

// ---------------------------------------------------------------------------
// Returns the reference for Theme Manager extension object. 
// ---------------------------------------------------------------------------
//    
OSN_EXPORT AlfThemeManagerUtil& AlfWidgetEnvExtension::themeManagerUtil( CAlfEnv& /*aEnv*/)
    {
    // Depricated
    ALF_THROW(AlfWidgetException, EInvalidArgument, "Depricated Module");
    }

    
// ---------------------------------------------------------------------------
//Access to the widget factory
// ---------------------------------------------------------------------------
//        
OSN_EXPORT IAlfWidgetFactory& AlfWidgetEnvExtension::widgetFactory( 
    CAlfEnv& aEnv)
    {
    AlfExtensionWidgetFactory* widgetFactoryExtension = 
        static_cast<AlfExtensionWidgetFactory*>(
            aEnv.Extension( KAlfExtensionWidgetFactoryUid));
    if ( widgetFactoryExtension == NULL)
        {
        initializeEnvironmentForFactory(aEnv);
        }
    //re-fetch the widget factory extension
    widgetFactoryExtension = 
        static_cast<AlfExtensionWidgetFactory*>(
            aEnv.Extension( KAlfExtensionWidgetFactoryUid));
    return *(widgetFactoryExtension->mWidgetFactory);
    }    

    } // Alf
