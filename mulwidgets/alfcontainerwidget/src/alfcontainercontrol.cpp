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
* Description:  Container widget control class.
*
*/


// Local Includes
#include "../inc/alfcontainercontrol.h"

// AlfClient Includes
#include <alf/alfevent.h>

// Widget Model
#include <alf/alfwidgetevents.h>

using namespace Alf;

AlfContainerControl::AlfContainerControl(CAlfEnv& aEnv)
	: CAlfWidgetControl(aEnv)
	{
	}

AlfContainerControl::~AlfContainerControl()
	{
	}

// ---------------------------------------------------------------------------
// From base class IAlfWidgetControl
// Handles events sent to widget control.
// ---------------------------------------------------------------------------
//
AlfEventStatus AlfContainerControl::handleEvent( const TAlfEvent& aEvent )
	{
	AlfEventStatus ret = CAlfWidgetControl::handleEvent(aEvent);

    if(ret == EEventNotHandled && aEvent.IsKeyEvent())
		{
		if(aEvent.KeyEvent().iScanCode == EStdKeyRightArrow ||
		   aEvent.KeyEvent().iScanCode == EStdKeyDownArrow )
			{
			if(aEvent.Code() == EEventKey)
				{
				TAlfEvent focusNext(EEventFocusNextWidget);
				return CAlfWidgetControl::processEvent(focusNext);							
				}
			}
	    else if(aEvent.KeyEvent().iScanCode == EStdKeyLeftArrow ||
		        aEvent.KeyEvent().iScanCode == EStdKeyUpArrow)
			{
			if(aEvent.Code() == EEventKey)
				{
				TAlfEvent focusPrevious(EEventFocusPreviousWidget);
				return CAlfWidgetControl::processEvent(focusPrevious);
				}
			}
		}
	
	return ret;
	}
