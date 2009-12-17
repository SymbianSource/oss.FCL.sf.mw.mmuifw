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


#ifndef I_ALFLAYOUTMANAGER
#define I_ALFLAYOUTMANAGER

#include <alf/ialfinterfacebase.h>
#include <alf/alftypes.h>

class CAlfLayout;
class CAlfControl;

namespace duiuimodel
    {
class DuiNode;
    }
using namespace duiuimodel;

namespace Alf
    {

class CAlfWidgetControl;
class AlfCustomInitDataBase;

namespace ialflayoutmanager
    {
     static const IfId Ident =
        {
        0, "ialflayoutmanager"
        };
    } 

/**
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
 */
class IAlfLayoutManager : public IAlfInterfaceBase
    {
public:
       
    static inline const IfId& type()
        {       
        return ialflayoutmanager::Ident;
        }
        
    virtual inline ~IAlfLayoutManager() {}
    
    /**
     * Creates the layout used by this layoutmanager.
     *
     * @param aOwner owner-control of the the created layout.
     * @param aParentLayout parent for the created layout
     * @param aLayoutIndex index, where created visual should be placed in the parent layout.
     * @throw AlfVisualException(EInvalidArrayIndex), if aLayoutIndex is out of bounds
     *        AlfVisualException(ECanNotCreateVisual), if the layout creation failed.
     */    
    virtual void createLayout(CAlfWidgetControl& aOwner, 
        CAlfLayout* aParentLayout, int aLayoutIndex) = 0;
    
    /**
     * Returns the layout used by this layoutmanager.
     *
     * @return layout used by this layoutmanager.
     * @throw AlfVisualException(EInvalidVisual), if layout is not created.
     */
    virtual CAlfLayout& getLayout()const  = 0;
    
    /**
     * Notifies the layout manager, that the child control's layout
     * must be updated
     * 
     * @param aControl control, which size has changed.
     * @throw AlfVisualException(EInvalidVisual), if layout is not created.
     */    
    virtual void updateChildLayout(CAlfWidgetControl* aControl) = 0;
    
    /**
     * Notifies the layout manager, that all the child control's layouts
     * must be updated.
     * @throw AlfVisualException(EInvalidVisual), if layout is not created.
     */    
    virtual void updateChildrenLayout() = 0;
    
    /**
     * Notifies the layout manager, that the control's has been
     * removed from the layout.
     * 
     * @param aControl control, which has been removed from the layout.
     * @throw AlfVisualException(EInvalidVisual), if layout is not created.
     */    
    virtual void childRemoved(CAlfWidgetControl* aControl) = 0;
    
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
    virtual IAlfInterfaceBase* makeInterface( const IfId& aType ) = 0;
    };
    
/**
 * Placeholder for information required to instantiate an layoutmanager
 * via the widget factory mechanism.
 * A pointer to this structure is casted to a void pointer and sent to the
 * factory plugin.
 * @lib alfwidgetmodel.lib
 * @since S60 ?S60_version
 */
struct AlfLayoutManagerInitData
    {
    /**
     * Event handler instance ID.This uniquely identifies every event handler instance.
     */
    char* mLayoutManagerId;
    
    /**
     * Pointer to node in declaration containing information for the widget.
     */
    DuiNode* mNode;

    /**
     * Pointer to custom data passed via factory mechanism
     * Not Owned.
     */
    AlfCustomInitDataBase* mCustomData;
    };


    } // namespace Alf

#endif // I_ALFLAYOUTMANAGER
