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
* Description:  grid layout manager
*
*/


#ifndef ALFLCTANCHORLAYOUTMANAGER_H
#define ALFLCTANCHORLAYOUTMANAGER_H

#include <osn/osndefines.h>
#include <osn/osntypes.h>
#include <memory>
#include <alf/alflayoutmanager.h>
#include <alf/alflayout.h>

using std::auto_ptr;

namespace Alf
    {
class IAlfWidgetControl;
class AlfLCTAnchorLayoutPreferencesImpl;
class AlfLCTAnchorLayoutPolicyImpl;

/**
 * Grid layout manager
 *
 * Layout manager handles the layouting of child widgets in the container widget.
 * It uses IAlfLayoutPreferences-interface (@see IAlfLayoutPreferences) 
 * of child widgets as a guide for laying out the widgets. The layoutmanager 
 * is set to the IAlfHostApi- interface (@see IAlfHostApi::setBaseLayout)
 * createLayout-method should be the first call after creating the layoutmanager. 
 *
 * @lib alfwidgetmodel.lib
 * @since S60 ?S60_version
 */
class AlfLCTAnchorLayoutManager : public AlfLayoutManager
    {
    friend class AlfLCTAnchorLayoutPreferencesImpl;
    friend class AlfLCTAnchorLayoutPolicyImpl;

public:

    /**
     * layout manager constructor
     *
     */
    AlfLCTAnchorLayoutManager();
        
    /**
     * virtual destructor
     */
    virtual ~AlfLCTAnchorLayoutManager();
    
public:
    //from IAlfLayoutManager
            
    /**
     * from IAlfLayoutManager
     * Notifies the layout manager, that the child control's layout
     * must be updated
     * 
     * @param aControl control, which size has changed.
     * @throw AlfVisualException(EInvalidVisual), if layout is not created.
     */
     virtual void updateChildLayout(CAlfWidgetControl* aControl);
    
    /**
     * from IAlfLayoutManager
     * Notifies the layout manager, that the control's has been
     * removed from the layout.
     * 
     * @param aControl control, which has been removed from the layout.
     * @throw AlfVisualException(EInvalidVisual), if layout is not created.
     */    
    virtual void childRemoved(CAlfWidgetControl* aControl);
public:
protected:
 
    void doUpdateChildLayout(CAlfWidgetControl* aControl);
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
    virtual IAlfInterfaceBase* makeInterface( const IfId& aType );

private: // data
    auto_ptr<AlfLCTAnchorLayoutPreferencesImpl> mDataPreferences;
    auto_ptr<AlfLCTAnchorLayoutPolicyImpl> mDataPolicy;
    };

    } // namespace Alf

#endif // ALFLCTANCHORLAYOUTMANAGER_H
