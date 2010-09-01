/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Gesture helper constant definitions
*
*/

#ifndef _GESTUREDEFS_H_
#define _GESTUREDEFS_H


namespace GestureHelper
{

/// time to hold the stylus in the same position to activate holding
const TInt KHoldDuration = 1000000; // microseconds 

/// time to tap again for the second tap to be a double tap
const TInt KMaxDoubleTapDuration = 400000; // microseconds 

/// time in which speed becomes zero if user stops stylus movement before
/// lifting the pointer
const TInt KSpeedStopTime = 100000; // microseconds

/// tolerance in degrees to either side of an angle, for direction to be the 
/// same as reference angle
const TReal KAngleTolerance = 25;

/// Nth pointer event before the latest point
const TInt KPreviousPointOffset = 3;

/// Tolerance for ignoring small movements, percent of longer screen edge
const TInt KGestureTolerancePercent = 4; // of 100

/// Tolerance for considering it as pinch gesture for the first time
const TInt KInitialPinchTolerance = 5; // Pixels

/// Tolerance for considering it as pinch gesture after a pinch is detected
const TInt KPinchTolerance = 5; // Pixels

/** 
 * Panic codes 
 */
enum TGesturePanic
    {
    EGesturePanicIllegalLogic
    };  

/** Panics the thread */
inline void Panic( TGesturePanic aReason )
    {
     _LIT( KComponentName, "GestureHelper" );
    User::Panic( KComponentName, aReason );
    }

} // namespace GestureHelper

#endif // _GESTUREDEFS_H
