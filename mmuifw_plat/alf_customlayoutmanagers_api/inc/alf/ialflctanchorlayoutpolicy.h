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
* Description:  interface for layout policy
*
*/


#ifndef I_ALFLCTANCHORLAYOUTMANAGER
#define I_ALFLCTANCHORLAYOUTMANAGER

#include <alf/ialfinterfacebase.h>
#include <alf/alftypes.h>

namespace osncore
    {
class UString;
    }
    
class CAlfControl;
class TAknLayoutHierarchyComponentHandle;

namespace Alf
    {

class CAlfWidgetControl;

namespace ialflctanchorlayoutpolicy
    {
     static const IfId Ident =
        {
        0, "lctanchorlayoutpolicy"
        };
    }

using namespace osncore;    
using Alf::CAlfWidgetControl;



/**
 * The interface for grid layout policy
 *
 * Provides grid specific layout APIs
 * 
 * @code
 * // Create layout manager interface.
 * IAlfLayoutManager* layoutManager = IAlfInterfaceBase::makeInterface<IAlfLayoutManager>(control);
 *
 * // See if layout policy is supported in the given layout manager.
 * IAlfLCTAnchorLayoutPolicy* layoutPolicy = IAlfInterfaceBase::makeInterface<IAlfLCTAnchorLayoutPolicy>(layoutManager);
 *
 * if(layoutPolicy != 0)
 *     {
 *     int rows = layoutPolicy->Attach(componentHandle);
 *     }
 * @endcode
 *
 * @lib alfwidgetmodel.lib
 * @since S60 ?S60_version
 */
class IAlfLCTAnchorLayoutPolicy : public IAlfInterfaceBase
    {
public:
       
    static inline const IfId& type()
        {       
        return ialflctanchorlayoutpolicy::Ident;
        }
        
    virtual inline ~IAlfLCTAnchorLayoutPolicy() {}


   /**
    * attach the anchors needed for a child visual, according to the layout data 
    * corresponding to the component handle.
    *
    * @note this should can be used for layout text components, but also use IAlfLCTTextLayoutPolicy 
    * with the contained text visual to set the text style and alignment
    *
    * @param aOrdinal the child visual index
    * @param aComponentHandle the layout data to use
    */ 
    virtual void Attach (int aOrdinal, const UString& aLCTString, int aVarietyIndex) = 0;
    

// from base class IAlfInterfaceBase
    
    /**
     * Interface getter. 
     * @see IAlfInterfaceBase::MakeInterface
     *
     * @param aType The type id of the queried interface.
     * @return The queried interface, or NULL if the interface is not
     *         supported or available.
     */    
    virtual IAlfInterfaceBase* makeInterface( const IfId& aType ) = 0;
    };

    } // namespace Alf

#endif // I_ALFLCTANCHORLAYOUTMANAGER
