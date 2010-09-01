/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:   The private implementation file for WidgetEventHandler.
*
*/


#include "alfwidgeteventhandlerimpl.h"

using namespace Alf;


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// ?description_if_needed
// ---------------------------------------------------------------------------
//
AlfWidgetEventHandlerImpl::AlfWidgetEventHandlerImpl(
    IAlfWidgetEventHandler::AlfEventHandlerType aEventHandlerType,
    IAlfWidgetEventHandler::AlfEventHandlerExecutionPhase aEventHandlerExecutionPhase):
    mActiveStates( ~0 ), // By default active in all states
    mCapturing( false ),
    mEventHandlerType(aEventHandlerType),
    mEventHandlerExecutionPhase(aEventHandlerExecutionPhase)
    {
    construct();
    }

// ---------------------------------------------------------------------------
// ?description_if_needed
// ---------------------------------------------------------------------------
//
void AlfWidgetEventHandlerImpl::construct()
    {
    mInputs.setAutoDelete( true );
    mEhData.mWidgetEventHandlerId = 0;
  	mEhData.mNode = 0;
    }

// ---------------------------------------------------------------------------
// ?description_if_needed
// ---------------------------------------------------------------------------
//
AlfWidgetEventHandlerImpl::~AlfWidgetEventHandlerImpl()
    {
    mInputs.clear();
    mPointerInputs.clear();
    mOutputs.clear();
    mCancelOutputs.clear();
    mAnimationOutputs.clear();
    mAnimationCancelOutputs.clear();
    mWidgetAttributeOutputs.clear();
    mWidgetAttributeInputs.clear();
#ifdef RD_TACTILE_FEEDBACK
    mTactileOutputs.clear();
#endif // RD_TACTILE_FEEDBACK
    mWidgetEventOutputSignals.clear();
    mStateChangeOutputs.clear();
    mEventPasserInputs.clear();
    mEventPasserOutputs.clear();
    mAppEventOutputs.clear();
    if (mEhData.mWidgetEventHandlerId)
		 {
		 delete[] mEhData.mWidgetEventHandlerId;
		 }
    }

// End of File.
