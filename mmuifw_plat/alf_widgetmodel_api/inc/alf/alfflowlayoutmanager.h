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
* Description:  flow layoutmanager.
*
*/


#ifndef ALFFLOWLAYOUTMANAGER
#define ALFFLOWLAYOUTMANAGER

#include <alf/alflayout.h>
#include <osn/osndefines.h>
#include <osn/osntypes.h>
#include <memory>
#include <alf/alflayoutmanager.h>

using std::auto_ptr;

namespace Alf
    {
class AlfFlowLayoutPolicyImpl;
class AlfFlowLayoutPreferencesImpl;

/** @class AlfFlowLayoutManager alfflowlayoutmanager.h "alf/alfflowlayoutmanager.h"
 * Flow layout manager positions children next to each other either horizontally or
 * vertically. The manager sizes the children by using the preferred sizes of the
 * children. @see IAlfLayoutPreferences::getPreferredSize()  
 *
 *
 * Flow layout manager takes system wide layout mirroring into consideration. 
 * In western locales the visuals are layed out from left to right whereas
 * in arabic locale for instance the visuals are layed out from right to left.
 * Mirroring is considered only when the flow is horizontal. The layout manager
 * will always lay its children from top to bottom.
 *
 * @interfaces IAlfLayoutManager, IAlfLayoutPreferences
 * @lib alfwidgetmodel.lib
 * @since S60 ?S60_version
 * @status Draft
 */
 
class AlfFlowLayoutManager : public AlfLayoutManager
    {
public:
    friend class AlfFlowLayoutPolicyImpl;
    friend class AlfFlowLayoutPreferencesImpl;
public:

    /**
     * layout manager constructor
     *
	 * @exception std::bad_alloc     
     * @param aLayoutType type of layout used by this manager.
     */
    OSN_IMPORT AlfFlowLayoutManager();
        
    /**
     * virtual destructor
     */
    OSN_IMPORT virtual ~AlfFlowLayoutManager();
    
public:
    //from AlfLayoutManager
    
    /**
     * from AlfLayoutManager
     * Notifies the layout manager, that the child control's layout
     * must be updated.
     * 
     * @param aControl control, which size has changed.
     */
    OSN_IMPORT virtual void updateChildLayout(CAlfWidgetControl* aControl);
    
public:
// from base class IAlfInterfaceBase
    
    /**
     * Interface getter. 
     * @see IAlfInterfaceBase::MakeInterface
     *
     * @since S60 ?S60_version
     * @param aType The type id of the queried interface.
     * @return The queried interface, or NULL if the interface is not
     *         supported or available.
     */    
    OSN_IMPORT virtual IAlfInterfaceBase* makeInterface( const IfId& aType );
protected:
    
private: // data

    auto_ptr<AlfFlowLayoutPolicyImpl> mData;
    auto_ptr<AlfFlowLayoutPreferencesImpl> mPrefImpl;
    };

    } // namespace Alf

#endif // ALFFLOWLAYOUTMANAGER
