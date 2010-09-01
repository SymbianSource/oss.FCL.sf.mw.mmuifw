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
* Description:  Gesture class
*
*/

#ifndef _GESTURE_H_
#define _GESTURE_H_

#include <e32base.h>

#include "gestureobserver.h"
#include "gesturerecogniser.h"
#include "pointarray.h"

namespace GestureHelper
{

/**  
 * TGesture represents the gesture that the user has made.
 * Implements MGestureEvent interface to allow clients to 
 * inspect the gesture
 */
NONSHARABLE_CLASS( CGesture ) : public CBase, public MGestureEvent
    {
public:
    /** constructor */
     CGesture();
    /** destructor */
    ~CGesture();
    
    /** 
     * @return a new gesture that will have only the first point
     * of this gesture. Can be used to create gesture's a "start" event
     * after some other points have been received. 
     */ 
    CGesture* AsStartEventLC() const;
    
    /** 
     * Sets the gesture as empty. resets timers to receive points
     * immediately
     */
    void Reset();
    
    /** @return ETrue if the gesture has no points */
    TBool IsEmpty() const;
    
    /** @return ETrue if there are no points in the second array */
    TBool IsMultiTouch() const;
    
    /**
     * Add a point to the sequence of points that forms the gesture
     * Call Reset() just before adding the first point
     * @param aPoint the point to add
     * @return error code
     */
    TInt AddPoint( const TPoint& aPoint );
 
    /**
     * Add a point to the sequence of points that forms the gesture
     * Adds the point to the secondray pointer array.
     * Used to add second pointer movement in case of multi touch
     * Call Reset() just before adding the first point
     * @param aPoint the point to add
     * @return error code
     */
    TInt AddSecondaryPoint( const TPoint& aPoint );
    
    /** Set the visual of this gesture. Should be called when gesture starts */
    void SetVisual( CAlfVisual* aVisual );
    
    /** @return ETrue if the point is very near the holding point */
    TBool IsNearHoldingPoint( const TPoint& aPoint ) const;
    /** Set the latest point as the holding point */
    void SetHoldingPoint();
    /** @return ETrue if aPoint is the same as point added last */
    TBool IsLatestPoint( const TPoint& aPoint ) const;
    /** @return ETrue if aPoint is the same as point added last in the secondary array */
    TBool IsLatestSecondaryPoint( const TPoint& aPoint ) const;
    
    /** 
     * After call, Code(...) will return appropriate holding gesture code.
     */
    void StartHolding();
    /** 
     * After call, Code(...) will return a "hold released" gesture code when
     * the gesture is completed. Meanwhile, code will be drag.
     */
    void ContinueHolding(); 
    /** Set as single touch gesture started */
    void SetSingleTouchActive();
    /** Set as multitouch gesture started */
    void SetMultiTouchActive();    
	/** Set as stylus released when there was only one stylus pressed */
    void SetSingleTouchReleased();
    /** Set as stylus released when there were two pointers, one pointer is still pressed down */
    void SetMultiTouchReleased();
    /** Set the single touch gesture as complete. Gesture is completed at pointer up */
    void SetSingleTouchComplete();
    /** Set the multitouch gesture as complete. Single touch still continues */
    void SetMultiTouchComplete();    
    /** After call, Code(...) will return a "cancelled" gesture code */
    void SetCancelled();
    
    /** force returning a code as double tap when gesture is complete */
    void SetDoubleTap();
    
    /** 
     * @return Whether the current gesture is a tap (and hence not a swipe)
     *         Does not consider holding or other state information (such as
     *         whether the gesture was just started or is dragging).  
     *         Results are based purely on the current stream of points.
     */ 
    TBool IsTap() const;
 
    /** 
     * Checks if the current points in primary and secondary pointer array results in
     * a pinch gesture. If yes updates the previous distance.
     * @return Whether the current gesture is a pinch
     *         Returns EFalse if multitouch is not active currently.  
     *         Results are based purely on the current stream of points.
     */ 
    TBool IsPinch();
    
    /** 
     * Reset all the events recieved in the current gesture.
     * Calls Reset() internally.
     * create a new points starting from the last point of the
     * earlier gesture
     * Used when the multitouch point is released to start the 
     * single touch operations.
     * @param aFirstPointer, Should reset to the first pointers last point or to the 2d pointers.
     *          false (2nd pointers last point) when multitouch is going on and user releases the first point and
     *          continues to hold the second point
     */ 
    void ResetToLastPoint(TBool aSetPointerZero,TBool aSetToZero); 
  
// From MGestureEvent
    TGestureCode Code( TAxis aRelevantAxis ) const;
    TBool IsHolding() const;
    TPoint StartPos() const;
    TPoint CurrentPos() const; 
    TRealPoint Speed() const;
    TRealPoint SpeedPercent( const TRect& aEdges ) const;
    TPoint Distance() const; 
    CAlfVisual* Visual() const;
    TInt PinchPercent() const;
    TPoint PinchCentrePoint()const;
    
private:
    /** @return elapsed time between the latest and previous points */
    inline TTimeIntervalMicroSeconds32 TimeFromPreviousPoint() const; 
    
    /** @return elapsed time between the latest and first points */
    inline TTimeIntervalMicroSeconds32 TimeFromStart() const; 
    
    /** @return ETrue if user has stopped moving the stylus before lifting it */
    inline TBool IsMovementStopped() const;
    
    /** @return aOffset'th last entry in the list of points */
    inline const TPointEntry& NthLastEntry( TInt aOffset ) const;
    
    /** @return previous point (may not be exactly the point before last) */
    inline const TPointEntry& PreviousEntry() const;
    
    /** @return position of point received just before the latest point */
    inline TPoint PreviousPos() const;
    
    /** @return the last point in the given pointer array */
    inline const TPointEntry LastPoint( const RArray< TPointEntry >& aPoints ) const;
    
    /** @return whether a slightly higher tolarence needs to be used for pinch or not*/
    inline TBool IsHighPinchTolerance() const;
    
private: 

    /// sequence of points that make up the gesture. own.
    /// Single touch pointer variations.
    RArray< TPointEntry > iPoints;
	
    /// sequence of points that make up the gesture. own.
    /// Second touch pointer variations.
    RArray< TPointEntry > iSecondaryPoints;
    
    /// index in iPoints of the point in which user started holding
    TInt iHoldingPointIndex;

    /// the different states that indicate whether user has made a holding gesture 
    enum THoldingState
        {
        ENotHolding = 0, // before holding activated (i.e., holding not activated)
        EHoldStarting,   // holding activated just now (lasts just one callback round)
        EHolding         // holding activated earlier 
        };
  
    /// state that indicates is user has held the pointer down in one position 
    /// for long time to indicate a holding gesture    
    THoldingState iHoldingState;
    
    /// Whether the gesture completion code will be forcefully overridden to be double tap,
    /// regardless of what the points are
    TBool iIsDoubleTap;
    
    /// If in the current gesture, there was a multi touch at any in between point.
    /// This is required, if after doing a multitouch, release one pointer and do some 
    /// operation with the other pointer still touched down. Though the new variations
    /// of pointer results in a tap, tap is not possible after a multitouch.
    TBool iIsMultiTouched;
        
    enum TCompletionState 
        {
        ENotActive = 0,
        ESingleTouchActive,
        EMultiTouchActive,
        ECancelled,
        EMultiTouchComplete,
        EMultiTouchReleased,
        ESingleTouchComplete,        
        ESingleTouchReleased       
        };
      
    /// gesture only becomes complete when user lifts the pointer
    TCompletionState iState;

    /// time when the stylus was lifted.
    TTime iCompletionTime;
    
    /// visual relevant to this gesture. null if not known.
    CAlfVisual* iVisual;
    
    /// The distance between the two points, at the time of last identified pinch gesture
    /// This will also be the start distance for next pinch. At start it will
    /// be the distance between touch down points
    TInt iPinchStartDistance;

    /// The distance between the two points, at the time of pinch identified
    TInt iPinchEndDistance;
    
    /// Pinch detcted, true for one cycle,untill a new point is recieved.
    TBool iPinchDetected;
 
    enum TZoomState 
        {
        ENoZoom = 0,
        EZoomOut,
        EZoomIn    
        };
  
    /// gesture only becomes complete when user lifts the pointer
    TZoomState iZoomState;
    
    };

} // namespace GestureHelper

#endif // _GESTURE_H_
