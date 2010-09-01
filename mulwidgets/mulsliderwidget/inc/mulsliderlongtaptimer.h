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
* Description: Header for presentation elements - Slider long tap timer.
*
*/

#ifndef MULSLIDERLONGTAPTIMER_H_
#define MULSLIDERLONGTAPTIMER_H_

#include <e32std.h>
#include <e32base.h>
#include<alf/alfevent.h>
#include "imulsliderbaseelementinternal.h"


namespace Alf
    {
        
class MulSliderLongTapTimer: public CActive
    {

    enum TState
        {
        //To indicate the event is in running state.
        ERunning,
        
        //To indicate the event is in stopped state.
        EStopped
        };        
public:
    /**
     * Constructor
     * @param aBaseElement
     */
    MulSliderLongTapTimer(IMulSliderBaseElementInternal* aSliderElement);

    /**
     * Destructor 
     */      
    ~MulSliderLongTapTimer();

    /**
     * Start the Timer. 
     */    
    void Start();

    /**
     * Stops the Timer. 
     */    
    void Stop();
   
    /**
     * user code runs under this active object. 
     */
    void RunL();
    
    /**
     * Implements cancellation of an outstanding request. 
     */
    void DoCancel();        
    
       
private:
    
    RTimer iTimer;
    TState iState;  
    IMulSliderBaseElementInternal *iBaseElement;
    };

    }

#endif /*MULSLIDERLONGTAPTIMER_H_*/
