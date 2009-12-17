/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:   Event output signal subscriber interface
*
*/


#ifndef ALFEVENTOUTPUTSIGNALSUBSCRIBER_H
#define ALFEVENTOUTPUTSIGNALSUBSCRIBER_H

#include <osn/osndefines.h>
//using namespace osncore;

namespace Alf
    {

/**
 *  Event output signal subscriber interface. 
 */
class IAlfEventOutputSignalSubscriber
	{
public:
    /**
    * Called by event output signal. This method calls subscriber's callback method.
    */
	virtual void receiveSignal() = 0;
	virtual ~IAlfEventOutputSignalSubscriber() {}
	};

/**
 *  Event output signal subscriber. 
 */	
template <class T>
OSN_NONSHARABLE_CLASS( AlfEventOutputSignalSubscriber )
	: public IAlfEventOutputSignalSubscriber
	{
public:
	explicit AlfEventOutputSignalSubscriber(T& aSubscriber, void (T::*aFnc)(void))
		:mSubscriber(aSubscriber), mFnc(aFnc) {}

	void receiveSignal() {(mSubscriber.*mFnc)();}

private: // data
	// Subscriber
	T& mSubscriber;
	// Callback
	void (T::*mFnc)(void);
	};

    } // namespace Alf

#endif // ALFEVENTOUTPUTSIGNALSUBSCRIBER_H
