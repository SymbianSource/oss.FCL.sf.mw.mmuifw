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
* Description:  interface for layout managers
*
*/


#ifndef ALFLAYOUTMANAGER
#define ALFLAYOUTMANAGER

#include <osn/osndefines.h>
#include <osn/osntypes.h>
#include <memory>
#include <alf/ialflayoutmanager.h>
#include <alf/alflayout.h>

using std::auto_ptr;

namespace Alf
    {
class IAlfWidgetControl;
class AlfLayoutManagerImpl;
class IAlfLayoutPreferences;


/**
 * @class AlfLayoutManager alflayoutmanager.h "alf/alflayoutmanager.h"
 * The interface for layout managers
 *
 * Layout manager handles the layouting of child widgets in the container widget.
 * It uses IAlfLayoutPreferences-interface (@see IAlfLayoutPreferences) 
 * of child widgets as a guide for laying out the widgets. The layoutmanager 
 * is set to the IAlfHostApi- interface (@see IAlfHostApi::setBaseLayout)
 * createLayout-method should be the first call after creating the layoutmanager.
 *
 * Layout manager may also provide information about the layout preferences of its
 * children. If this is supported the layout manager will combine the layout preferences
 * of its children taking into consideration the technique used to lay out the children
 * using the layout manager. This will provide information about the whole display area
 * occupied by the layout manager. For instance vertical flow layout manager will provide
 * preferred size area of its children by summing their heights and providing the maximum
 * width among the children.
 *
 * @code
 * // Create layout manager interface.
 * IAlfLayoutManager* layoutManager = IAlfInterfaceBase::makeInterface<IAlfLayoutManager>(control);
 *
 * // See if layout preferences is supported in the given layout manager.
 * IAlfLayoutPreferences* layoutPreferences = IAlfInterfaceBase::makeInterface<IAlfLayoutPreferences>(layoutManager);
 *
 * if(layoutPreferences != 0)
 *     {
 *     TAlfXYMetric preferredSize;
 *     // This will report the preferred size of the layout by combining together preferred sizes of its children.
 *     bool result = layoutPreferences->getPreferredSize(preferredSize);
 *     }
 * @endcode
 *
 * @lib alfwidgetmodel.lib
 * @since S60 ?S60_version
 * @status Draft
 * @interfaces IAlfLayoutManager, IAlfLayoutPreferences
 */
class AlfLayoutManager : public IAlfLayoutManager
    {
public:

    /**
     * layout manager constructor
     *
     * @param aLayoutType type of layout used by this manager.
     */
    OSN_IMPORT AlfLayoutManager(TAlfLayoutType aLayoutType);
        
    /**
     * virtual destructor
     */
    OSN_IMPORT virtual ~AlfLayoutManager();
    
public:

    /**
     * @return the owner control of the layoutmanager.
     */        
    OSN_IMPORT CAlfWidgetControl& owner() const;

    /**
     * returns the control at aIndex.
     *
     * @param aIndex index for the control.
     * @return control at aIndex
     */
    OSN_IMPORT CAlfWidgetControl* getControl(int aIndex) const;
    
    /**
     * returns the count of controls inside layoutmanager.
     *
     * @return count of controls inside layoutmanager
     */
    OSN_IMPORT int count() const;
    
public:
    //from IAlfLayoutManager
        
    /**
     * creates the layout used by this layoutmanager.
     *
     * @param aOwner, owner-control of the the created layout.
     * @param aParentLayout parent for the created layout
     * @param aLayoutIndex index, where created visual should be placed in the parent layout.
     * @exception osncore::AlfVisualException Thrown with error code osncore::EInvalidVisual if aLayoutIndex is out of bounds.     
     * @exception osncore::AlfVisualException Thrown with error code osncore::ECanNotCreateVisual if the layout creation failed.
     */    
    OSN_IMPORT virtual void createLayout(CAlfWidgetControl& aOwner,
        CAlfLayout* aParentLayout, int aLayoutIndex);
            
    /**
     * from IAlfLayoutManager
     * Returns the layout used by this layoutmanager.
     *
     * @return layout used by this layoutmanager.
     * @exception osncore::AlfVisualException Thrown with error code osncore::EInvalidVisual if layout is not created.
     */
    OSN_IMPORT virtual CAlfLayout& getLayout() const;
    
    /**
     * from IAlfLayoutManager
     * Notifies the layout manager, that the child control's layout
     * must be updated
     * 
     * @param aControl control, which size has changed.
     * @exception osncore::AlfVisualException Thrown with error code osncore::EInvalidVisual if layout is not created.
     *
     */
    OSN_IMPORT virtual void updateChildLayout(CAlfWidgetControl* aControl);
    
    /**
     * from IAlfLayoutManager
     * Notifies the layout manager, that all the child control's layouts
     * must be updated.
     * @exception osncore::AlfVisualException Thrown with error code osncore::EInvalidVisual if layout is not created.     
     */
    OSN_IMPORT virtual void updateChildrenLayout();
    
    /**
     * from IAlfLayoutManager
     * Notifies the layout manager, that the control's has been
     * removed from the layout.
     * 
     * @param aControl control, which has been removed from the layout.
     * @exception osncore::AlfVisualException Thrown with error code osncore::EInvalidVisual if layout is not created.     
     */    
    OSN_IMPORT virtual void childRemoved(CAlfWidgetControl* aControl);
    
protected:
    /**
     * Notifies the layout manager, that the child control's layout
     * must be updated. Called in updateChildLayout for the updated control
     * and in updateChildrenLayout for all the controls in the layout.
     * 
     * @param aControl control, which size has changed.    
     */
    OSN_IMPORT virtual void doUpdateChildLayout(CAlfWidgetControl* aControl);
    
    /**
     * returns the rect of the control.
     *
     * @param aControl, control, which size is queried.
     * @param aRect will receive the rect of aControl, if return value is true.
     * @return true, if control rect was fetched, false, if the control hasn't 
     * got a visualization.
     */    
    OSN_IMPORT bool controlRect(
        CAlfWidgetControl& aControl, TAlfRealRect& aRect);
        
    /**
     * sets the size and position to the control
     *
     * @param aControl, control, which size and position is being set.
     * @param aRect, rect for aControl
     */        
    OSN_IMPORT void setControlRect(
        CAlfWidgetControl& aControl, const TAlfRealRect &aRect);

    /**
     * sets the position to the control
     *
     * @param aControl, control, which position is being set.
     * @param aPos, position for aControl
     */
    OSN_IMPORT void setControlPosition(
        CAlfWidgetControl& aControl, const TAlfRealPoint& aPos);
        
    /**
     * sets the size to control
     *
     * @param aControl, control, which size is being set.
     * @param aSize, new size for aControl.
     */        
    OSN_IMPORT void setControlSize(
        CAlfWidgetControl& aControl, const TAlfRealPoint &aSize);
    
    /**
     * returns layout preferences for aControl
     *
     * @return layout preferences for aControl.
     */
    OSN_IMPORT const IAlfLayoutPreferences* getLayoutPreferences(
        CAlfWidgetControl* aControl) const;

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
private: // data

    auto_ptr<AlfLayoutManagerImpl> mData;
    
    };

    } // namespace Alf

#endif // ALFLAYOUTMANAGER
