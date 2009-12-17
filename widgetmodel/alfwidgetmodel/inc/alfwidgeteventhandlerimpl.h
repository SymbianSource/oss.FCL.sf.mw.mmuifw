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
* Description:   Class for private implementation for AlfWidgetEventHandler.
*
*/


#ifndef ALFWIDGETEVENTHANDLERIMPL_H
#define ALFWIDGETEVENTHANDLERIMPL_H

#include <osn/alfptrvector.h>
#include <osn/osndefines.h>
#include <alf/ialfwidgeteventhandler.h>
#include "alfeventinput.h"
#include "alfeventoutput.h"
#include "alfanimationoutput.h"

using namespace osncore;

namespace Alf
    {

/**
 *  Widget event handler internal implementation.
 */
OSN_NONSHARABLE_CLASS( AlfWidgetEventHandlerImpl )
    {
public:

    /**
     * constructor.
     * 
     * @param aEventHandlerType            Event handler type. 
     *                                     It can be presentaion related or logical.
     * @param aEventHandlerExecutionPhase  Event handler exceution phase. 
     *                                     It can be tunneling phase/bubbling phase/both.
     */
    AlfWidgetEventHandlerImpl(IAlfWidgetEventHandler::AlfEventHandlerType aEventHandlerType,
         IAlfWidgetEventHandler::AlfEventHandlerExecutionPhase aEventHandlerExecutionPhase);

    /**
    * Destructor.
    */
    virtual ~AlfWidgetEventHandlerImpl();

private:
    /**
       * Destructor.
       */
    void construct();

public: // data

    AlfPtrVector<AlfEventInput>mInputs;
    AlfPtrVector<AlfPointerEventInput> mPointerInputs;
    AlfPtrVector<AlfEventOutput> mOutputs;
    AlfPtrVector<AlfAnimationOutput> mAnimationOutputs;
    AlfPtrVector<AlfWidgetAttributeOutput> mWidgetAttributeOutputs;
    AlfPtrVector<AlfWidgetAttributeInput> mWidgetAttributeInputs;
    AlfPtrVector<AlfEventCancelOutput> mCancelOutputs;
    AlfPtrVector<AlfAnimationCancelOutput> mAnimationCancelOutputs;
    AlfPtrVector<AlfStateChangeOutput> mStateChangeOutputs;
    AlfPtrVector<AlfEventPasserInput> mEventPasserInputs;
    AlfPtrVector<AlfEventPasserOutput> mEventPasserOutputs;

#ifdef RD_TACTILE_FEEDBACK
    AlfPtrVector<AlfTactileOutput> mTactileOutputs;
#endif // RD_TACTILE_FEEDBACK
    AlfPtrVector<AlfEventOutputSignal> mWidgetEventOutputSignals;
    AlfPtrVector<AlfAppEventOutput> mAppEventOutputs;

    unsigned int mActiveStates;
    bool mCapturing;
    /**
     * Event handler init data
     */
    AlfWidgetEventHandlerInitData mEhData;
    
    /*
     * Type of event handler. It can be presentaion related or logical.
     */
    IAlfWidgetEventHandler::AlfEventHandlerType mEventHandlerType;

    /*
     * Event handler exceution phase. It can be tunneling phase/bubbling phase/both.
     */
    IAlfWidgetEventHandler::AlfEventHandlerExecutionPhase mEventHandlerExecutionPhase;
    };

    } // namespace Alf

#endif // C_ALFWIDGETEVENTHANDLERIMPL_H
