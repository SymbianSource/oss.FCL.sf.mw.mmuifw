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


// includes
#include <w32std.h>
#include <eikdef.h>
#include <eikenv.h>
#include <AknUtils.h>

#include "mulkeyboardutility.h"

const TUid KUidKeyBoardUtility = {0x02D811B8} ;

CQwertySubscriber::CQwertySubscriber(TCallBack aCallBack, RProperty& aProperty)
     : CActive(EPriorityNormal), iCallBack(aCallBack), iProperty(aProperty)
{
     CActiveScheduler::Add(this);
}
 
 CQwertySubscriber::~CQwertySubscriber()
{
     Cancel();
}
 
 void CQwertySubscriber::SubscribeL()
{
     if (!IsActive())
         {
         iProperty.Subscribe(iStatus);
         SetActive();
         }
}
 
void CQwertySubscriber::StopSubscribe()
{
   Cancel();
}
 
void CQwertySubscriber::RunL()
{
    if (iStatus.Int() == KErrNone)
    {
         iCallBack.CallBack();
         SubscribeL();
    }
}
 
void CQwertySubscriber::DoCancel()
{
   iProperty.Cancel();
}

CKeyBoardUtility* CKeyBoardUtility:: NewL()
{ 
    CKeyBoardUtility* self = static_cast<CKeyBoardUtility*>( CCoeEnv::Static( KUidKeyBoardUtility ) ) ; 
    if(!self)
        {
        self = new( ELeave ) CKeyBoardUtility() ;
        }
    return self ;
}


CKeyBoardUtility::CKeyBoardUtility():CCoeStatic(KUidKeyBoardUtility, CCoeStatic::EThread /*or EApp*/ )
{
	bQwerty = EFalse;
    User::LeaveIfError(iQwertyModeStatusProperty.Attach(KCRUidAvkon, KAknQwertyInputModeActive));
    iQwertyModeStatusSubscriber = new (ELeave) CQwertySubscriber(TCallBack(QwertyModeChangeNotification, this), iQwertyModeStatusProperty);
    iQwertyModeStatusSubscriber->SubscribeL();
}

CKeyBoardUtility::~CKeyBoardUtility()
{

    // Qwerty Notify clean-up
    if (iQwertyModeStatusSubscriber)
    {
       iQwertyModeStatusSubscriber->StopSubscribe();
    }
    iQwertyModeStatusProperty.Close();
    delete iQwertyModeStatusSubscriber;   
}

TInt CKeyBoardUtility::QwertyModeChangeNotification(TAny* aObj)
{
    if (aObj != NULL)
    {
    	static_cast<CKeyBoardUtility*>(aObj)->HandleQwertyModeChangeNotification();
        return KErrNone;
    }
    else
    {
        return KErrArgument;
    }
}

void CKeyBoardUtility::HandleQwertyModeChangeNotification()
{
	     TInt value = 0;
	     iQwertyModeStatusProperty.Get(value);    
	     if(value > 0) 
	     {
	     	bQwerty = ETrue;
	     }
	     else
	     {
	     	bQwerty = EFalse;
	     }
}

bool CKeyBoardUtility::IsQwerty()
{
     TInt value = 0;
     iQwertyModeStatusProperty.Get(value);    
     if(value > 0) 
     {
     	bQwerty = true;
     }
     else
     {
     	bQwerty = false;
     }
	return bQwerty;
}
