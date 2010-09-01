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
* Description:  layoutpreferences implementation class for flowlayoutmanager
*
*/


#ifndef ALFFLOWLAYOUTPREFERENCESIMPL_H
#define ALFFLOWLAYOUTPREFERENCESIMPL_H

#include <osn/osndefines.h>
#include <alf/ialflayoutpreferences.h>

using namespace Alf;

namespace Alf
    {
class AlfFlowLayoutManager;

/**
 * layout preferences implementation for flowlayoutmanager.
 *
 * @lib alfwidgetmodel.lib
 * @since S60 ?S60_version
 */
class AlfFlowLayoutPreferencesImpl : public IAlfLayoutPreferences
    {
public:
    AlfFlowLayoutPreferencesImpl(AlfFlowLayoutManager& aFlowLayoutManager);
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
// from base class IAlfInterfaceBase
    
    /**
     * Interface getter. 
     * @see IAlfInterfaceBase::makeInterface
     *
     * @since S60 ?S60_version
     * @param aType The type id of the queried interface.
     * @return The queried interface, or NULL if the interface is not
     *         supported or available.
     */    
    virtual IAlfInterfaceBase* makeInterface( const IfId& aType );
protected:

    /** size type to calculate @see calculateChildSizes */
    enum sizeType
        {
        ESizePreferred,
        ESizeMin,
        ESizeMax
        };
        
	/**
	 * calculate the min-, preferred-, or max- sizes from the children. 
	 * 
	 * @param aSize on return, contains the calculated size.
	 * @param aFlowDirection direction of flow.
	 * @param aSizeType specifies, whether the min-, preferred- or max-size of the
	 *        IAlfLayoutPreferences is used for calculation.
	 */
    bool calculateChildSizes(TAlfXYMetric& aSize,  
        enum IAlfFlowLayoutPolicy::flowDirection aFlowDirection, 
        enum sizeType aSizeType) const;
        
	/**
	 * fetches the size from the IAlfLayoutPreferences-interface of the aControl.
	 * 
	 * @param aControl control, which preferences-interface to query.
	 * @param aSize on return, contains the size.
	 * @param aSizeType specifies, whether the min-, preferred- or max-size of the
	 *        IAlfLayoutPreferences is returned.
	 */
    bool getSize(CAlfWidgetControl* aControl, 
        TAlfXYMetric& aSize,
        enum sizeType aSizeType) const;
        
    /**
     * @return the flow layout policy-instance of the layoutmanager.
     *
     */
    const IAlfFlowLayoutPolicy& flowLayoutPolicy() const;

	/**
	 * converts size in metrics to size in pixels. Implementation missing.
	 * 
	 * @param aSize size to convert
	 * @return size in pixels.
	 */
    static TAlfRealSize sizeInPixels(const TAlfXYMetric& aSize);
    
private:
    AlfFlowLayoutManager &mFlowLayoutManager;
    TAlfXYMetric mPreferredSize;
    };

    } // namespace Alf

#endif // ALFFLOWLAYOUTPREFERENCESIMPL_H
