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
* Description:  Utility class for keyboard handling  
 *
*/


#ifndef MULKEYBOARDUTILITY_H_
#define MULKEYBOARDUTILITY_H_

#include <e32base.h>
#include <e32std.h>
#include <e32property.h>
#include <centralrepository.h>
#include <aknfepinternalcrkeys.h>
#include <avkoninternalcrkeys.h>   // KAknQwertyInputModeActive
#include <coemain.h>

 class CQwertySubscriber : public CActive
 {
     public:
          CQwertySubscriber(TCallBack aCallBack, RProperty& aProperty);
         ~CQwertySubscriber();
 
     public: // New functions

         void SubscribeL();
         void StopSubscribe();
 
     private: // from CActive

         void RunL();
         void DoCancel();
 
     private:
         TCallBack   iCallBack;
         RProperty&  iProperty;
 };

class CKeyBoardUtility: public CCoeStatic
{
	public:
		static CKeyBoardUtility* NewL();
	private: 
		static TInt QwertyModeChangeNotification(TAny* aObj);
	private:
		~CKeyBoardUtility();
		CKeyBoardUtility(); 
		bool bQwerty;
	    /** Qwerty Notification */
	    CQwertySubscriber* iQwertyModeStatusSubscriber;
	    RProperty iQwertyModeStatusProperty;
	    void HandleQwertyModeChangeNotification();
	public:
		bool IsQwerty();
};
#endif //MULKEYBOARDUTILITY_H_
