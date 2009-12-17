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

// Class Header
#include "mulcoverflowao.h"

// Alf Headers
#include <math.h>

// Local Header
#include "mulcoverflowcontrol.h"
#include "mulbaseelement.h"
#include "mullog.h"

namespace Alf
	{

// ---------------------------------------------------------------------------
// MulCoverFlowAo
// ---------------------------------------------------------------------------
//
MulCoverFlowAo::MulCoverFlowAo( MulCoverFlowControl& aControl ) : CTimer( EPriorityNormal ), mControl(aControl)							     								
	{
	MUL_LOG_ENTRY_EXIT("aakash::MulCoverFlowAo::MulCoverFlowAo"); 
	ConstructL();
	CActiveScheduler::Add(this);
	}

// ---------------------------------------------------------------------------
// ~MulCoverFlowAo
// ---------------------------------------------------------------------------
//
MulCoverFlowAo::~MulCoverFlowAo()
	{
	MUL_LOG_ENTRY_EXIT("aakash::MulCoverFlowAo::~MulCoverFlowAo"); 
	StopTimer();	
	}
	
// ---------------------------------------------------------------------------
// StartMoving
// ---------------------------------------------------------------------------
//     
void MulCoverFlowAo::StartMoving(TReal32 aSpeedPoint, int aCurrGestureDirection,bool aFastScrollForKey,int aAnimationTime)
    {
	MUL_LOG_ENTRY_EXIT("aakash::MulCoverFlowAo::StartMoving"); 
	int previosDir = mCurrGestureDirection;
	mCurrGestureDirection = aCurrGestureDirection;

	aSpeedPoint = aSpeedPoint < 0 ? aSpeedPoint * -1 : aSpeedPoint;
	
	// Fast scroll in key event ,
	// Three steps are there Max(100),150,200(Min).	
	if (aFastScrollForKey)
		{
		mTransitionTime = aAnimationTime;	
		}
	else
		{
		mTransitionTime = 200 *(1 + (1 - aSpeedPoint)*5);		
		}
		
	if( !mStarted )
		{
		// Cancle any pending requests and start a new request
	    // this is because the layout direction n speed should be changed according to the current swipe
	    if( IsActive() )
	    	{
	    	MUL_LOG_INFO("MUL::MulCF::Cancel Previous Animation ");
	    	Cancel();
	    	}
	    		
	    MUL_LOG_INFO("MUL::MulCF::Start New Animation ");
	    mStarted = true;
		After(0);	
		}
	else
		{
		if (previosDir != mCurrGestureDirection)
			{
			mTransitionTime = 200;
			Cancel();
			After(0);	
			}
		else
			{
			}
		}
		
    }
	
// ---------------------------------------------------------------------------
// StopMoving
// ---------------------------------------------------------------------------
//  
void MulCoverFlowAo::StopMoving()
	{
	MUL_LOG_ENTRY_EXIT("aakash::MulCoverFlowAo::StopMoving"); 	
	StopTimer();
	mCurrGestureDirection = 0;
	mTransitionTime = 0;
	mWaitingTime = 0;
	mStarted = false;
	}
	
// ---------------------------------------------------------------------------
// StopTimer
// ---------------------------------------------------------------------------
// 
void MulCoverFlowAo::StopTimer()
	{
	MUL_LOG_ENTRY_EXIT("aakash::MulCoverFlowAo::StopTimer"); 	
	if( IsActive() )
    	{
    	Cancel();
    	}
	}
// ---------------------------------------------------------------------------
// RunL
// ---------------------------------------------------------------------------
// 	
void MulCoverFlowAo::RunL()
	{
	MUL_LOG_ENTRY_EXIT("aakash::MulCoverFlowAo::RunL");	
	// Check if the layout ahs moved by one complete visaul. 
	// Then change the highlight and again start moving the layout.
	// If it hasnot moved by complete visaul then allow the layout to move to new position but
	// dont change the highlight
	ChangeHighLight();	
	After(fabs(mTransitionTime*600));
	}
	
// ---------------------------------------------------------------------------
// ChangeHighLight
// ---------------------------------------------------------------------------
// 	
void MulCoverFlowAo::ChangeHighLight()
	{
	MUL_LOG_ENTRY_EXIT("aakash::MulCoverFlowAo::ChangeHighLight"); 	
	// Inform widget/model that highlight has changed	
	if(mCurrGestureDirection > 0 )
		{
		mControl.HandleNavigationEvent ( EEventScrollLeft );	
		}
	else if(mCurrGestureDirection < 0 )
		{
		mControl.HandleNavigationEvent ( EEventScrollRight );
		}
	else
		{
		// no need to change the highlight	
		}
	}

// ---------------------------------------------------------------------------
// FastScrollTransitionTime
// ---------------------------------------------------------------------------
// 	
int MulCoverFlowAo::FastScrollTransitionTime()
	{	
	return mTransitionTime;
	}
			
	} // End of namespace .

//End of file


