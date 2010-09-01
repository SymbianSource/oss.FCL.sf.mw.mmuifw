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
* Description:  Algorithm to recognise gesture from a stream of points
*
*/

#ifndef _GESTURERECOGNISER_H_
#define _GESTURERECOGNISER_H_

#include <e32std.h>
#include <gestureobserver.h>

struct TPointerEvent;

namespace GestureHelper
{

class TPointArray;

/**
 * Note: Not a static class or a function pointer, just to make it sligthly 
 * easier to replace it with a heavier implementation
 */
NONSHARABLE_CLASS( TGestureRecogniser )
    {
public:
    /** 
     * Translates points into a gesture code
     * @param aPoints Points that form the gestures. Client is not required 
     *                to pass in repeated points (sequential points that are 
     *                almost in the same place)
     * @param aAxis Axis to filter the points
     * @return recognised gesture id or EUnknownGesture
     */
    TGestureCode GestureCode( const TPointArray& aPoints, MGestureEvent::TAxis aRelevantAxis ) const;

    /** 
     * Translates 2 sequences of points into a multitouch gesture code
     * @param aPoints Points that represent the first touch point variations. Client is not required 
     *                to pass in repeated points (sequential points that are 
     *                almost in the same place)
     * @param aSecondaryPoints Points that represent the second touch point variations.
     * @param aPreviousDistance distance between the two points when the pinch was recognized the last time.
     * @param aisFirstPinch, Is this the first pinch of teh current gesture. Required to decide which tolerance to use
     * @return recognised gesture id or EUnknownGesture if its not a valid multi touch gesture
     */
    TGestureCode MultiTouchGestureCode( const TPointArray& aPoints, const TPointArray& aSecondaryPoints, TInt aPreviousDistance,
            TBool aIsFirstPinch ) const;
    
    /** 
     * Validate the points to decide its a drag or not
     * @param aPoints Points that form the gestures. Client is not required 
     *                to pass in repeated points (sequential points that are 
     *                almost in the same place)
     * @param aAxis Axis to filter the points
     * @return EGestureDrag if valid else returns EUnknownGesture
     */    
    TGestureCode ValidateDrag( const TPointArray& aPoints, MGestureEvent::TAxis aRelevantAxis ) const;
    
    /** 
     * Validate the points to decide its a drag or not
     * @param aPoints Points that form the gestures. Client is not required 
     *                to pass in repeated points (sequential points that are 
     *                almost in the same place)
     * @param aAxis Axis to filter the points
     * @return EGestureDrag if valid else returns EUnknownGesture
     */    
    TInt Length( const TPoint aPoint, const TPoint aSecondaryPoint ) const;

    /** 
     * Return the pointer number from the given event
	 *
     * @param aEvent, event for which the pointer number needs to be fetched
     * 
	 * This function is added here only for simulating multiple pointer events
	 * for testing UT, For simulation purpose we will have a custom implemetation
	 * which will override this method and return our own pointer numbers
	 * So that we can simulate multiple pointer event.
     * @return TInt pointer number
     */       
    TInt PointerNumber( const TPointerEvent& aEvent);
    
    };
    
} // namespace GestureHelper

#endif // _GESTURERECOGNISER_H_
