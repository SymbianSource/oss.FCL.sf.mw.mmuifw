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
* Description:    Timer for creating longtap event 
*
*/

#ifndef __ALF_SCROLLBARLLONGTAPTIMER_H__
#define __ALF_SCROLLBARLLONGTAPTIMER_H__

#include <e32std.h>
#include <e32base.h>
#include <alf/ialfscrollbardefaultbaseelement.h>

namespace Alf
    {

// Forward declarations
class AlfScrollBarBaseElement;
        
class AlfScrollBarLongTapTimer: public CActive
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
  * @param aBaseElement .     
  * @since S60 ?S60_version
  */
	AlfScrollBarLongTapTimer(IAlfScrollBarDefaultBaseElement* aBaseElement);

/**
  * Destructor 
  */      
   ~AlfScrollBarLongTapTimer();

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
    
    /**
    * Instance of IAlfScrollBarBaseElement.
    */
	IAlfScrollBarDefaultBaseElement* iBaseElement;
	
	/**
    * Instance of TAlfEvent.
    */	
    TAlfEvent* mEvent;
    };

    }

#endif // __ALF_SCROLLBARLLONGTAPTIMER_H__
