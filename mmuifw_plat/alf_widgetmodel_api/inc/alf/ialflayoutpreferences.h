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
* Description:  The interface for inquiring the layout preferences of a laid-out object
*
*/


#ifndef I_ALFLAYOUTPERENCES_H
#define I_ALFLAYOUTPERENCES_H

#include <alf/ialfinterfacebase.h>
#include <alf/alftypes.h>
#include <alf/alfmetric.h>
#include <osn/osntypes.h>

using namespace osncore;

namespace Alf
    {

class IAlfLayoutManager;

namespace alflayoutpreferences
    {
     static const IfId Ident=
        {
        0,"alflayoutpreferences"
        };
    }

/**
 * The interface for finding the layout preferences for an Alf widget model 
 * presentation object.
 * 
 * This interface is called by layout managers (implementors of @c IAlfLayoutManager)
 * in order to find out information about their contained presentation objects.
 * 
 * It is implemented by presentation objects in order to report to the layout manager
 * what sizes objects prefer.
 * 
 * The layout manager is responsible for finding out layout preferences for all the
 * AlfElement instances in the next level down of containment - the "child" elements.
 * It calls @c makeInterface<IAlfLayoutPreferences> on these elements.
 * The layout manager uses its knowledge about how these child elements relate to 
 * each other geometrically to come up with a decision on how many child elements
 * can be shown, where they are to be positioned, and how big they should be.
 * @see IAlfLayoutManager
 * 
 * @c AlfElements that represent contained sets of visuals - those that take up 
 * space within the visual containment - should implement @c IAlfLayoutPreferences,
 * or defer its implementation to another presentation object which is capable 
 * of reporting layout preferences.
 *
 * The following units are allowed to be used in the returned parameter values:
 *  EAlfUnitPixel - pixels
 *  EAlfUnitDisplaySize - units relative to the display
 *  EAlfS60Unit - S60 layout specification units
 *
 * Other units do not make sense, since the metric is being passed
 * to a different layout context. Furthermore, the reason for this API
 * is for the implementing object to state its own, internal, preferences
 * without recourse to its containment. This implies the use of the more
 * absolute units.
 * 
 * The units used for the returned @c TAlfXYMetric objects are chosen independently 
 * by the implementations of each API. They do not have to be return the same units.
 *
 * @lib alfwidgetmodel.lib
 * @since S60 ?S60_version
 * @status Draft
 */
class IAlfLayoutPreferences : public IAlfInterfaceBase
    {
public:
    static inline const IfId& type()
        {
        return alflayoutpreferences::Ident;    
        }

    /**
     * Return the minimum desirable size of a presentation object.
     * 
     * Minimum size should mean the size at which the visual would start to become 
     * unreadable in the case of text, or too difficult to see with sufficient 
     * clarity in the case of graphics.
     * 
     * false is to be returned if the implementing presentation object does
     * not care how small it is made.
     *
     * @param aMinSize The minimum size that the object would like to be laid out to
     * @return false iff the object queried does not care about a minimum size
     */
    virtual bool getMinimumSize( TAlfXYMetric& aMinSize ) const = 0;

    /**
     * Return the maximum size desired by a presentation object.
     * 
     * There may often be no preference as to maximum size. The implementation
     * should then return false.
     * 
     * @param aMaxSize The maximum size that the object would like to be laid out to
     * @return false iff the object queried does not care about a maximum size
     */
    virtual bool getMaximumSize( TAlfXYMetric& aMaxSize ) const = 0;

    /**
     * Return the preferred size for a presentation object.
     * 
     * The preferred size is the optimum size of the object in the absence
     * of external factors.
     *
     * The value returned should greater than or equal in size to a value
     * returned by getMinimumSize()
     *
     * The value returned should less than or equal in size to a value
     * returned by getMaximumSize()
     *
     * @param aPreferredSize The size that the object would like to be laid out to
     * @return false iff the object queried does not have a preferred size
     */
    virtual bool getPreferredSize( TAlfXYMetric& aPreferredSize ) const = 0;
    
    /**
     * Set the preferred size into a presentation object.
     * 
     * The preferred size is being dictated by external code in this case.
     * This new value should over-ride any internal preferred size.
     *
     * Calling this API has several side-effects.  Subsequent calls to 
     * getPreferredSize should return @c true and return the new preferred
     * size.
     *
     * Subsequent calls to getMinimumSize must return either @c false, or return
     * @c true with a value less than or equal to the preferred size.
     *
     * Subsequent calls to getMaximumSize must return either @c false, or return
     * @c true with a value greater than or equal to the preferred size.
     *
     * @param aPreferredSize The new preferred size for the laid-out object
     */
    virtual void setPreferredSize( const TAlfXYMetric& aPreferredSize ) = 0;

    };

    } // namespace Alf
#endif // I_ALFLAYOUTPERENCES_H
