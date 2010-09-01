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
* Description:  Utilities to create and access widget model extensions to AlfEnv
*
*/


#ifndef ALFWIDGETENVEXTENSION_H
#define ALFWIDGETENVEXTENSION_H

#include <osn/osndefines.h>

class CAlfEnv;

namespace Alf
    {
    class ResourcePool;
    class IAlfWidgetFactory;
    class AlfThemeManagerUtil;
    }
namespace Alf
    {

/** 
 *  @class AlfWidgetEnvExtension alfwidgetenvextension.h "alf/alfwidgetenvextension.h"
 *  class with methods to access extensions in AlfEnv
 *  @lib alfwidgetmodel.lib
 *  @since S60 ?S60_version
 *  @status Draft
 */
class AlfWidgetEnvExtension
    {
public:

    /**
     * Initialize the singleton objects that are required for the widget model
     *
     * @exception osncore::AlfWidgetException Thrown with the error code
     *                osncore::ECommonError when the resource pool extension
     *                can not be hooked to Alf environment.
     * @exception osncore::AlfWidgetException Thrown with the error code 
     *                osncore::ECommonError when the factory extension can
     *                not be hooked to Alf environment.
     * @exception osncore::AlfWidgetException Thrown with the error code
     *                osncore::ECommonError when the widget factory cannot
     *                be created. 
     * 
     * @param aEnv   Alf environment object.
     */
    OSN_IMPORT static void initializeEnvironment(CAlfEnv& aEnv);
    
    /**
     * Initialize the singleton object for the resource pool required for the
     * widget model 
     *
     * @param aEnv   Alf environment object
     */
    static void initializeEnvironmentForResourcePool(CAlfEnv& aEnv);
    
    /**
    * Initializes environment with Theme Manager extension
    * 
    * @param aEnv   Alf environment object
    */    
    static void AlfWidgetEnvExtension::initializeEnvironmentForTheme(CAlfEnv& aEnv);
    
    /**
     * Initialize the singleton object for the widget factory required for 
     * the widget model
     * 
     * @param aEnv   Alf environment object
     */
    static void initializeEnvironmentForFactory(CAlfEnv& aEnv);

    /**
     * Access to the application-level resource pool
     * It also initializes the environment with resource pool extension,
     * if it was not initialized earlier
     *
     * @exception osncore::AlfWidgetException Thrown with the error code
     *                osncore::ECommonError when the extension can not be 
     *                hooked to Alf environment.
     * @param aEnv  The Alf environment object in which to find the pool
     * @return ref to the application-level resource pool
     */
    OSN_IMPORT static ResourcePool& resourcePool(CAlfEnv& aEnv);
    
    
    /**
     * Access to the widget factory
     * It also initializes the environment with widget factory extension,
     * if it was not initialized earlier
     *
     * @exception osncore::AlfWidgetException Thrown with the error code 
     *                osncore::ECommonError when the extension can not be 
     *                hooked to Alf environment.
     * @exception osncore::AlfWidgetException Thrown with the error code
     *                osncore::ECommonError when the widget factory cannot
     *                be created.
     * @param aEnv  The Alf environment object in which to find the 
     *              widget factory
     * @return ref to the widget factory
     */
    OSN_IMPORT static IAlfWidgetFactory& widgetFactory( CAlfEnv& aEnv);
	/**
     * Access to the application-level Thememanager.
     * 
     * @param aEnv  The Alf environment object in which to find the thememanager.
     * @return ref to the application-level ThemeManager.
     */
    OSN_IMPORT static AlfThemeManagerUtil& themeManagerUtil(CAlfEnv& aEnv);

    };

    }
#endif // ALFWIDGETENVEXTENSION_H
