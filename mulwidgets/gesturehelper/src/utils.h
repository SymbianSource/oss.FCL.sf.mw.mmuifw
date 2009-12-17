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
* Description:  Gesture recognition utilities
*
*/

#ifndef _GESTURE_UTILS_H_
#define _GESTURE_UTILS_H_

#include <e32std.h>
#include <gestureobserver.h>

namespace GestureHelper
    {
      
    /**
     * @return the a rectange in which dragging will be ignored. Tolerance rect 
     *         represents a rectange within which any movement will not be considered
     *         intentional, but as accidental stylus movement.
     */
    TRect ToleranceRect( const TPoint& aCenterPoint ); 
    
    /**
     * @return the a rectange in which dragging will be ignored. Tolerance rect 
     *         represents a rectange within which any movement will not be considered
     *         as drag, This depends on the Axis.
     * For Example, any movement within a vertical strip at the point of touch down is
     *		if the interested Axis is Horizontal axis.
     */
    TRect ToleranceRect( const TPoint& aCenterPoint, MGestureEvent::TAxis aRelevantAxis );

    } // namespace GestureHelper

#endif // _GESTURE_UTILS_H_
