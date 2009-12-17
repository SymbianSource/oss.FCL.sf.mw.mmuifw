/*
* Copyright (c) 2007-2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Active Object header for CoverFlow
*
*/

#ifndef __MULCOVERFLOWAO_H__
#define __MULCOVERFLOWAO_H__

#include <e32base.h>    // For CTimer

namespace Alf
	{
//Forward declarations
class MulCoverFlowControl;

class MulCoverFlowAo:public CTimer
	{
	
public : // Constructor and Destructor
    /// @bug critical:avanhata:7/7/2008 this is a symbian-dependent class, 
    /// need to have two-phase construction
    /// - otherwise mixing leaves and throws
	MulCoverFlowAo( MulCoverFlowControl& aControl );
	
	~MulCoverFlowAo();

public: //new method

    /**
     * Keeps moving layout asynchronously until it stopmoving() is called .
     * The layout will move on to show more items .
     * 
     * @param aSpeedPoint Gesture speed point.
     * @param, aCurrGestureDirection, Current direction of gesture
     * @param, aFastScrollForKey , if fast scroll due to key event
     * @param, aAnimationTime , animation time for fast scroll in key event.
     */
	void StartMoving(TReal32 aSpeedPoint, int aCurrGestureDirection,bool aFastScrollForKey = false,int aAnimationTime = 0);
	
	/**
	 *  Stop moving the layout.
	 *
	 */
	void StopMoving();

	/**
	 * Transition time for highlight change in fast scroll
	 *
	 * @return, transition time to be used
	 */	
	int FastScrollTransitionTime();		
	
private: 
	
	/**
	 * Timer call back funtion . Performs animations and moves the layout 
	 * for a particular distance depending on the length of the swipe
	 * in a particular time period depending on the speed .
	 */
	void RunL();
	
private : //new methods

	/**
	 * Cancel all pending requests.
	 */ 
	void StopTimer();
		
	/**
	 *  Increment the highlight with +-1 depending on the swipe direction and informs the 
	 *  control class to cahnge teh highlight.
	 */	
	void ChangeHighLight();
    
private: //data

	MulCoverFlowControl& mControl; //doesnt own
	int mTransitionTime;
	int mWaitingTime;
	int mCurrGestureDirection;
	bool mStarted;
	}; //end of class.
	
	} // End of namespace .

#endif  //__MULCOVERFLOWAO_H__

// End of file
