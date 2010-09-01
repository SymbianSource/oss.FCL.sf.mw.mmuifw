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
* Description: Timer for creating longtap event 
*
*/
#include "alfscrollbarlongtaptimer.h"
#include <alf/ialfscrollbardefaultbaseelement.h>
#include "ialfscrollbarbaseelementinternal.h"

const int KInitialTimeInterval = 200000;
const int KTimeInterval =  100000;

namespace Alf
    {

//---------------------------------------------------------------------------
// Constructor
//---------------------------------------------------------------------------
//        
AlfScrollBarLongTapTimer::AlfScrollBarLongTapTimer(
    IAlfScrollBarDefaultBaseElement* aBaseElement):CActive(EPriorityStandard)
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
AlfScrollBarLongTapTimer::~AlfScrollBarLongTapTimer()
    {
    Stop();
    iTimer.Close();
    }
//---------------------------------------------------------------------------
// Activates the timer. 
//---------------------------------------------------------------------------
//  
void AlfScrollBarLongTapTimer::Start()
    {
    if (iState == ERunning)
        return;
    if (!IsActive())
        {
        iTimer.After(iStatus, TTimeIntervalMicroSeconds32(KInitialTimeInterval));
        SetActive();
        }
    iState = ERunning;    
    }
//---------------------------------------------------------------------------
// stops the timer. 
//---------------------------------------------------------------------------
// 
void AlfScrollBarLongTapTimer::Stop()
    {
    Cancel();
    iState = EStopped;
    }
//---------------------------------------------------------------------------
// Handles an active object's request completion event.
//---------------------------------------------------------------------------
//     
void AlfScrollBarLongTapTimer::RunL()
    {
    IAlfScrollBarBaseElementInternal* elementInternal = 
        static_cast<IAlfScrollBarBaseElementInternal*> (
        iBaseElement->makeInterface(IAlfScrollBarBaseElementInternal::type()));
   
    if (iState == ERunning)
            {
            iTimer.After(iStatus, TTimeIntervalMicroSeconds32(KTimeInterval));    
            SetActive();
            }
    if(elementInternal)
        {
        elementInternal->handleLongTapEvent();
        }    
    }

//---------------------------------------------------------------------------
// Implements cancellation of an outstanding request.
//---------------------------------------------------------------------------
// 
void AlfScrollBarLongTapTimer::DoCancel()
    {
    iTimer.Cancel();
    }

    }
