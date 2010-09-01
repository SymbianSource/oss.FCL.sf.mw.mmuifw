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
* Description:  Slider long tap timer
 *
*/

 

// Include Files

// Class header
#include "mulsliderlongtaptimer.h"
#include "mulsliderdefinitions.h"

const int KInitialtimeinterval  = 200000;
const int KTimeinterval = 100000;
namespace Alf
    {

//---------------------------------------------------------------------------
// Constructor
//---------------------------------------------------------------------------
//        
MulSliderLongTapTimer::MulSliderLongTapTimer(
    IMulSliderBaseElementInternal* aBaseElement)
    :CActive(EPriorityStandard)
    {
    iTimer.CreateLocal();
    iBaseElement = aBaseElement;
    CActiveScheduler::Add(this);
    iState = EStopped;
    }

//---------------------------------------------------------------------------
// Destructor 
//---------------------------------------------------------------------------
//
MulSliderLongTapTimer::~MulSliderLongTapTimer()
    {
    Stop();
    iTimer.Close();
    }
//---------------------------------------------------------------------------
// Activates the timer. 
//---------------------------------------------------------------------------
//  
void MulSliderLongTapTimer::Start()
    {
    if (!IsActive())
        {
        iTimer.After(iStatus, TTimeIntervalMicroSeconds32(KInitialtimeinterval));
        SetActive();
        }
    iState = ERunning;    
    }
//---------------------------------------------------------------------------
// stops the timer. 
//---------------------------------------------------------------------------
// 
void MulSliderLongTapTimer::Stop()
    {
    Cancel();
    iState = EStopped;
    }
//---------------------------------------------------------------------------
// Handles an active object's request completion event.
//---------------------------------------------------------------------------
//     
void MulSliderLongTapTimer::RunL()
    {
    IMulSliderBaseElementInternal* elementInternal = 
        static_cast<IMulSliderBaseElementInternal*> (
        iBaseElement->makeInterface(IMulSliderBaseElementInternal::type()));
    if(elementInternal)
        {
        // call handleLongTap of element to handle the longtap 
        elementInternal->handleLongTap();
        }    
    if (iState == ERunning)
        {
        iTimer.After(iStatus, TTimeIntervalMicroSeconds32(KTimeinterval));    
        SetActive();
        }
    }

//---------------------------------------------------------------------------
// Implements cancellation of an outstanding request.
//---------------------------------------------------------------------------
// 
void MulSliderLongTapTimer::DoCancel()
    {
    iTimer.Cancel();
    }

   
   }

