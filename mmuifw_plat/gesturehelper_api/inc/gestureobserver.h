/*
* Copyright (c) 2008-2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Gesture helper interface
*
*/

#ifndef _GESTUREOBSERVER_H_
#define _GESTUREOBSERVER_H_

#include <e32std.h>

class CAlfControl;
class CAlfVisual;

namespace GestureHelper
{

/// flag that indicates gesture code is a holding code
/// clients should use MGestureEvent::IsHolding
const TInt EFlagHold       = 0x10000000;

enum TGestureCode
    {
    // states
    EGestureStart          			= 0x00000001, // gesture just started (user pressed stylus down)
    EGestureDrag           			= 0x00000002, // user moved stylus (may be holding or not holding)
    EGestureReleased      		  = 0x00000003, // user lifted stylus while user was holding
    // gestures
    EGestureUnknown             = 0x00000005, // gesture was not recognised
    // these codes are sent when user lifts stylus (if holding not started)
    EGestureTap                 = 0x00000006,
    EGestureDoubleTap           = 0x00000007, // only if double tap is enabled 
                                         // (via CGestureHelper::SetDoubleTapEnabled or
                                         // CGestureControl::SetDoubleTapEnabled)
    EGestureSwipeLeft           = 0x00000008,
    EGestureSwipeRight          = 0x00000009,
    EGestureSwipeUp             = 0x0000000A,
    EGestureSwipeDown           = 0x0000000B,
    
    // gestures related to advanced pointer events. These codes are sent if the platform supports advanced pointer events.
    // states
    EGestureMultiTouchStart     = 0x0000000C, // multitouch gesture just started (user touched 2nd finger down)
    EGestureMultiTouchReleased  = 0x0000000D, // user released 2nd touch point while still holding the first touch point
	  // gestures
    EGesturePinch         		  = 0x0000000E, // This event is sent when the pinch is recognised for 
    																		 // the first time or when the pinch distance is changing.
    
    // these codes are sent when user initiates holding by keeping stylus in
    // same place for a longer duration
    EGestureHoldLeft       = EGestureSwipeLeft  | EFlagHold,
    EGestureHoldRight      = EGestureSwipeRight | EFlagHold,
    EGestureHoldUp         = EGestureSwipeUp    | EFlagHold,
    EGestureHoldDown       = EGestureSwipeDown  | EFlagHold
    };

/**
 * Point of float accuracy
 */
NONSHARABLE_STRUCT( TRealPoint )
    {
    inline TRealPoint();
    inline TRealPoint( const TRealPoint& aPoint );
    inline TRealPoint( TReal aX, TReal aY );
    inline TBool operator==( const TRealPoint& aPoint ) const;

    TReal32 iX;
    TReal32 iY;
    };

/**
 * a gesture event
 */
class MGestureEvent
    {
public:
    /** X and Y axes, or both */
    enum TAxis
        {
        EAxisBoth,
        EAxisHorizontal,
        EAxisVertical
        };

public:
    /**
     * @param aRelevantAxis indicates whether only x, y or both coordinates 
     *        should be considered when determining the code. For example,
     *        if client specifies EAxisVertical as relevant axis, a swipe
     *        to left and slightly up would result in an "swipe up" code,
     *        and not "swipe left" code. If client specifies EAxisHorizontal
     *        or EAxisBoth, "swipe left" code is returned.
     * @return gesture code
     */
    virtual TGestureCode Code( TAxis aRelevantAxis ) const = 0;
    
    /** 
     * @return ETrue if user has activated holding 
     *         (by keeping stylus in the same position for longer time)
     *         EFalse if holding has not been activated
     */
    virtual TBool IsHolding() const = 0;
    
    /** 
     * @return position where gesture started, i.e., where stulys 
     * was pressed down 
     */
    virtual TPoint StartPos() const = 0;
    
    /** 
     * @return current position of the stylus
     */
    virtual TPoint CurrentPos() const = 0; 
    
    /** 
     * @return speed of a swipe. unit is pixels per second.
     */
    virtual TRealPoint Speed() const = 0;
    
    /**
     * Abstracts the algorithm to calculate speed during swipe and hold. This
     * algorithm (currently) assumes that max speed is achieved at the edges of an
     * area.
     * @param aEdges represents the coordinates of the rectange on which speed is 
     *               calculated. Speed will reach maximum if stylus is on the edge 
     *               or beyond the rect. In practise, the value should match the
     *               area of the layout that contains the scrollable visuals.
     *               For example, if the control area is the whole screen, the
     *               rect should be the screen rect.
     * @returns the speed as a percentage between -100% and 100%
     */
    virtual TRealPoint SpeedPercent( const TRect& aEdges ) const = 0;
    
    /** 
     * @return Length of gesture from starting position
     *         (start pos - current pos)
     */
    virtual TPoint Distance() const = 0; 
    
    /**
     * @return visual on which the gesture started
     *         NULL if not known (e.g., AVKON-based client 
     */
    virtual CAlfVisual* Visual() const = 0;
    
    /**
     * Pinch distance
     *
     * Used only for pinch. Returns 0 for other gestures. 
     *
     * @return The ratio of pinch end distance to pinch start distance * 100
     * 				PinchPercent above 100 for zooming out and below hundred for zooming in
     */
    virtual TInt PinchPercent() const = 0;    
    
    /**
     * Centre point of the two touch points
     *
     * Used only for pinch. Returns TPoint(0,0) for other gestures. 
     *
     * @return the centre point calculated using teh two touch down points
     * This point will remian the same for one multitouch gesture start to end.
     * This doesnt vary as the two points move away or closer.
     */
    virtual TPoint PinchCentrePoint() const = 0;   
        
    };
    
/** 
 * Observer that will be notified when user makes gestures
 */
class MGestureObserver
    {
public:
    /**
     * Handle the gesture event
     * @param aEvent event describing the gesture 
     */
    virtual void HandleGestureL( const MGestureEvent& aEvent ) = 0;
    };

/** 
 * static class for finding a visual from a visual tree
 */
class HitTest
    {
public:
    /** 
     * @return the leaf-most visual is under aPos, or NULL if not found
     */
    IMPORT_C static CAlfVisual* VisualByCoordinates( const CAlfControl& aControl, 
        const TPoint& aPos );
    };
    
// ----------------------------------------------------------------------------
// Default constructor for real point
// ----------------------------------------------------------------------------
//
inline TRealPoint::TRealPoint()
        : iX( 0 ), iY( 0 )
    {    
    }

// ----------------------------------------------------------------------------
// Copy constructor for real point
// ----------------------------------------------------------------------------
//
inline TRealPoint::TRealPoint( const TRealPoint& aPoint )
        : iX( aPoint.iX ), iY( aPoint.iY )
    {    
    }

// ----------------------------------------------------------------------------
// Copy constructor for real point
// ----------------------------------------------------------------------------
//
inline TRealPoint::TRealPoint( TReal aX, TReal aY )
        : iX( aX ), iY( aY )
    {    
    }

// ----------------------------------------------------------------------------
// Default constructor for real point
// ----------------------------------------------------------------------------
//
inline TBool TRealPoint::operator==( const TRealPoint& aPoint ) const
    {    
    return iX == aPoint.iX && iY == aPoint.iY;
    }

    
} // namespace GestureHelper

#endif // _GESTUREOBSERVER_H_
