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
* Description:  layoutmanager implementation header.
*
*/


#ifndef ALFLAYOUTMANAGERIMPL
#define ALFLAYOUTMANAGERIMPL

#include <osn/osndefines.h>
#include <alf/alflayoutmanager.h>
#include <alf/ialflayoutpreferences.h>

using std::auto_ptr;

namespace Alf
    {
    
class CAlfWidgetControl;
class IAlfWidget;
class IAlfLayoutPreferences;
    
/**
 * layoutmanager implementation class
 *
 * @lib alfwidgetmodel.lib
 * @since S60 ?S60_version
 */
class AlfLayoutManagerImpl : public IAlfLayoutPreferences
    {
public:
    /**
     * layout manager constructor
     *
     * @param aLayoutType type of layout used by the manager.
     */
    AlfLayoutManagerImpl(TAlfLayoutType aLayoutType);
    
    /**
     * destructor
     */
    ~AlfLayoutManagerImpl();
    
    /**
     * creates the layout used by the layoutmanager.
     *
     * @param aOwner, owner-control of the the created layout.
     * @param aParentLayout parent for the created layout
     * @param aLayoutIndex index, where created visual should be placed in the parent layout.
     */
    void createLayout(CAlfWidgetControl& aOwner, CAlfLayout* aParentLayout,
        int aLayoutIndex);
        
    /**
     * returns the layout used by layoutmanager.
     *
     * @return layout used by this layoutmanager.
     */        
    CAlfLayout& getLayout() const;
    
    /**
     * updates the main layout
     */    
    void updateMainLayout();
    
    /**
     * notifies the layout manager, that the control's has been
     * removed from the layout.
     * 
     * @param aControl control, which size has changed.
     */    
    void childRemoved(CAlfWidgetControl* aControl);
    
    /**
     * notifies the layout manager, that the child control's layout
     * must be updated.
     * 
     * @param aControl control, which size or position has changed.
     */
    void doUpdateChildLayout(CAlfWidgetControl* aControl);
    
    /**
     * returns the rect of the control.
     *
     * @param aControl, control, which size is queried.
     * @param aRect, will receive the rect of aControl.
     */    
    bool controlRect(CAlfWidgetControl& aControl, TAlfRealRect& aRect);
    
    /**
     * sets the size and position to the widget.
     *
     * @param aWidget, widget, which size and position is being set.
     * @param aRect, rect for aWidget
     */
    void setWidgetRect(IAlfWidget& aWidget, const TAlfRealRect &aRect);
    
    /**
     * sets the position to the widget.
     *
     * @param aWidget, widget, which position is being set.
     * @param aPos, position for aWidget
     */
    void setWidgetPosition(IAlfWidget& aWidget, const TAlfRealPoint &aPos);
    
    /**
     * sets the size to widget.
     *
     * @param aWidget, widget, which size is being set.
     * @param aSize, new size for aWidget.
     */
    void setWidgetSize(IAlfWidget& aWidget, const TAlfRealPoint &aSize);
    
    /**
     * updates widget size and position properties for all the children
     * to correct sizes and positions of the root visuals.
     */
    void updateAllWidgetRects();
    
    /**
     * @return the child visual count
     */
    int count() const;    
    
    /**
     * returns the control at aIndex.
     *
     * @param aIndex index for the control.
     * @return control at aIndex
     */    
    CAlfWidgetControl* getControl(int aIndex) const;
    
    /**
     * @return the owner control of the layoutmanager.
     */
    CAlfWidgetControl& owner() const ;
    
public:
// from base class IAlfLayoutPreferences

	/**
	 * Returns the minimum size of the layout by combining the minimum sizes of the
	 * layed out visuals.
	 * 
	 * @param aMinSize The minimum size that the object would like to be laid out to
	 * @return false iff none of the layed out UI elements do not care about minimum size
	 */
	virtual bool getMinimumSize( TAlfXYMetric& aMinSize ) const;
	
	/**
	 * Returns the maximum size of the layout by combining the maximum sizes of the
	 * layed out visuals.
	 *  
	 * @param aMaxSize The maximum size that the object would like to be laid out to
	 * @return false iff none of the layed out UI elements do not care about maximum size
	 */
	virtual bool getMaximumSize( TAlfXYMetric& aMaxSize ) const;
	
    /**
     * Returns the size combined by all preferred sizes of child UI elements in this layout 
     * when they are being layed out according to the layout manager rules.
     * 
     * @param aPreferredSize Filled with the referred area occupied by child elements when
     * 						 layed out by layouting rules in this layout manager.
     * @return False if none of the child components report their preferred size. True
     * 		   otherwise.
     */
    virtual bool getPreferredSize(TAlfXYMetric& aPreferredSize) const;
	 
	/**
	 * Set the preferred size into a presentation object.
	 * 
	 * This is not supported on the layout manager. Users should set preferred sizes of the
	 * contained UI elements.
	 */
	virtual void setPreferredSize( const TAlfXYMetric& aPreferredSize );
	
    /**
     * returns the layout preferences for a control
     *
     * @param aControl control instance
     * @return layout preferences for a control.
     */    
    const IAlfLayoutPreferences* getLayoutPreferences(CAlfWidgetControl* aControl) const;

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
    virtual IAlfInterfaceBase* makeInterface( const IfId& aType );	
	
private:
    
    /**
     * returns the control, which owns the visual
     *
     * @param aVisual visual, which the control owns.
     * @return control, which owns the visual at aIndex.
     */
    CAlfWidgetControl* getControl(CAlfVisual& aVisual) const;
        
    /**
     * finds the root visual for a control
     *
     * @param aControl control, which rootvisual is searched
     * @return root visual
     */
    CAlfVisual* findRootVisual(CAlfWidgetControl* aControl) const;
    
    /**
     * checks, that layout is created. If not, throws an exception.
     */
    void checkLayout() const;

private:

    /**
     * the layout created and used by the layout manager. 
     * Destroyed along with instance of this class.
     */
    auto_ptr<CAlfLayout> mLayout;
    
    /**
     * the layout type of mLayout
     */
    TAlfLayoutType mLayoutType;
    };

    } // namespace Alf

#endif // ALFLAYOUTMANAGERIMPL
