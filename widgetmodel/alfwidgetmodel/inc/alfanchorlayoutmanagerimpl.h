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
* Description:  anchor layoutmanager private implementation header.
*
*/


#ifndef ALFANCHORLAYOUTMANAGERIMPL_H
#define ALFANCHORLAYOUTMANAGERIMPL_H

#include <osn/osndefines.h>
#include <alf/alfanchorlayoutmanager.h>
#include <alf/ialflayoutpreferences.h>

using std::auto_ptr;
using Alf::AlfAnchorLayoutManager;

class CAlfControl;
class CAlfAnchorLayout;

namespace Alf
    {
    
class CAlfWidgetControl;
class AlfWidget;
class IAlfLayoutPreferences;
    
/**
 * layoutmanager implementation class
 *
 * @lib alfwidgetmodel.lib
 * @since S60 ?S60_version
 */
class AlfAnchorLayoutManagerImpl : public IAlfLayoutPreferences
    {
public:
    /**
     * layout manager constructor
     *
     * @param aLayoutType type of layout used by the manager.
     */
    AlfAnchorLayoutManagerImpl(AlfAnchorLayoutManager& aAnchorLayoutManager);
    
    /**
     * destructor
     */
    ~AlfAnchorLayoutManagerImpl();
    
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
     * returns the anchorlayout used by the layoutmanager.
     */    
    CAlfAnchorLayout& layout() const;
private:

    AlfAnchorLayoutManager & mAnchorLayoutManager;

    };

    } // namespace Alf

#endif // ALFANCHORLAYOUTMANAGERIMPL_H
