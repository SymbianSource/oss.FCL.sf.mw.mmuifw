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
* Description:   Class for various output events, including custom events.
*
*/


#ifndef ALFEVENTOUTPUT_H
#define ALFEVENTOUTPUT_H


#include <osn/osndefines.h>
#include <alf/alfcommand.h>
#include <alf/alftypes.h>

#include <osn/osntypes.h>
#include <osn/ustring.h>
using namespace osncore;

#include <memory>
using namespace std;

#ifdef RD_TACTILE_FEEDBACK
#include <touchfeedback.h>
#endif // RD_TACTILE_FEEDBACK

namespace Alf
    {

class CAlfWidgetControl;
class AlfReferenceToVisual;
class IAlfVariantType;
class AlfAttribute;
class IAlfEventOutputSignalSubscriber;
class IAlfAppEventListener;

/**
 *  Event output. Internal.
 */
OSN_NONSHARABLE_CLASS( AlfEventOutput )
    {
public:

    /**
     * If the event is to be broadcast, aTargetWidget is set to an empty string.
     */
    AlfEventOutput(int aEventType, int aEventID, const char* aTargetWidget, unsigned int aDelay );

    ~AlfEventOutput();

public:
    /**
     * Executes the event output.
     */
    void execute( const CAlfWidgetControl& aControl, const TAlfEvent& aEvent );

private:
    AlfEventOutput();

public: // data

    int mEventType; // either custom or key event
    int mEventID;
    UString mTargetWidget; // Empty string if the event is to be broadcast.
    unsigned int mDelay;
    };

/**
 *  Event cancel output. Internal.
 */
OSN_NONSHARABLE_CLASS( AlfEventCancelOutput )
    {
public:
    /**
     * constructor.
     */
    AlfEventCancelOutput( const char* aTargetWidget, int aEventID, bool aCancelAll );

    ~AlfEventCancelOutput();

public:
    /**
       * Executes the event output.
       */
    void execute( const CAlfWidgetControl& aControl );
    /**
    * returns target widget id.
    */
    inline const UString& targetWidget() const
        {
        return mTargetWidget;
        }

private:
    /**
       * cancels the excecution.
       */
    AlfEventCancelOutput();

private: // data

    UString mTargetWidget;
    int mEventID;
    bool mCancelAll;
    };

/**
 *  Animation Event cancel output. Internal.
 */
OSN_NONSHARABLE_CLASS( AlfAnimationCancelOutput )
    {
public:
    /**
     * constructor.
     */
    AlfAnimationCancelOutput(const char* aElementName,const char* aVisualName, TAlfOp aOperation, bool aCancelAll );
    /**
     * destructor.
     */
    ~AlfAnimationCancelOutput();

public:
    /**
        * excecutes the command.
        */
    void execute( CAlfWidgetControl& aControl, int aDataID );

private:
    /**
        * constructor
        */
    AlfAnimationCancelOutput();


private: // data

    auto_ptr<AlfReferenceToVisual> mVisualRef;
    TAlfOp mOperation;
    bool mCancelAll;
    };


OSN_NONSHARABLE_CLASS( AlfWidgetAttributeOutput )
    {
public:
    /**
    * Constructor.
    */
    AlfWidgetAttributeOutput( AlfAttribute& aAttribute, 
        const char* aWidgetId );

    ~AlfWidgetAttributeOutput();

public:
    /**
       * Executes the event output.
       */
    void execute( CAlfWidgetControl& aControl );

private:
    /**
       * constructor
       */
    AlfWidgetAttributeOutput();

private: // data
    AlfAttribute* mAttr;
    
    // Reference to outside widget
    UString mWidgetId;
    };

/**
 *  State change output. Internal.
 */
OSN_NONSHARABLE_CLASS( AlfStateChangeOutput )
    {
public:
    /**
     * constructor.
     *
     * @exception Alf::AlfWidgetException value osncore::EInvalidWidget 
     *     if aTaretWidget is NULL or empty
     * @exception Alf::AlfWidgetException value osncore::EInvalidArgument 
     *     if aDisableStates and aEnableStates have a common bit set 
     */
    AlfStateChangeOutput( const char* aTargetWidget, uint aEnableStates, uint aDisableStates );

    ~AlfStateChangeOutput();

public:
    /**
     * Executes the event output.
     */
    void execute( const CAlfWidgetControl& aControl );

    /**
     * returns target widget id.
     */
    inline const UString& targetWidget() const
        {
        return mTargetWidget;
        }

private: // data

    UString mTargetWidget;
    uint mEnableStates;
    uint mDisableStates;
    };


#ifdef RD_TACTILE_FEEDBACK
OSN_NONSHARABLE_CLASS( AlfTactileOutput )
    {
public:

    /**
     * Constructor.
     */
    AlfTactileOutput( TTouchLogicalFeedback aFeedbackType );

    /**
     * Destructor.
     */
    ~AlfTactileOutput();

public:

    /**
     * Executes the event output.
     */
    void execute();

private:

    /**
     * Constructor.
     */
    AlfTactileOutput();

private: // data

    TTouchLogicalFeedback mFeedbackType;

    };

#endif // RD_TACTILE_FEEDBACK

/**
 *  Event output signal. 
 */
OSN_NONSHARABLE_CLASS( AlfEventOutputSignal )
    {
public:
	explicit AlfEventOutputSignal( IAlfEventOutputSignalSubscriber& aSubscriber );

    ~AlfEventOutputSignal();

    /**
     * Executes the event output.
     */
    void execute();

private: // data
	IAlfEventOutputSignalSubscriber* mSubscriber;
    };

OSN_NONSHARABLE_CLASS( AlfEventPasserOutput)
    {
public:

    /**
     * Constructor.
     */
    AlfEventPasserOutput( const char* aSourceWidget , const char* aTargetWidget );
   
    /**
     * Constructor. Occurred event is mapped to a new event (specified by aNewEventId)
     */
    AlfEventPasserOutput::AlfEventPasserOutput( const char* aSourceWidget , const char* aTargetWidget, 
                                                int aNewEventId , int aNewEventCustomData );
    /**
     * Destructor.
     */
    ~AlfEventPasserOutput();

public:

    /**
     * Executes the event output.
     */
    void execute( const CAlfWidgetControl* aControl , const TAlfEvent& aEvent );

private:

    /**
     * Constructor.
     */
    AlfEventPasserOutput();

private: // data

    const char* mSourceWidget;
    const char* mTargetWidget;
    bool mEventMappingData;
    int mNewEventId;
    int mNewEventCustomData; 
    };

/**
 *  Application Event output.
 */
OSN_NONSHARABLE_CLASS( AlfAppEventOutput )
    {
public:
	explicit AlfAppEventOutput( const UString& aCmd, IAlfAppEventListener& aAppEvent );

    ~AlfAppEventOutput();

    /**
     * Executes the event output.
     */
    void execute(const TAlfEvent& aEvent);

private: // data
	IAlfAppEventListener* mAppEvent;
	UString mCmd;
    };

    } // namespace Alf

#endif // ALFEVENTOUTPUT_H
