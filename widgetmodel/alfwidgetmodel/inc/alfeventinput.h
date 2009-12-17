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
* Description:  Class for various input events, including custom events.
*
*/


#ifndef ALFEVENTINPUT_H
#define ALFEVENTINPUT_H

#include <alf/alfwidgetevents.h>
#include <osn/osndefines.h>
#include <memory>
#include <osn/ustring.h>
class TAlfEvent;

using namespace std;
using namespace osncore;

namespace Alf
    {

class CAlfWidgetControl;
class AlfReferenceToVisual;
class AlfAttribute;

// bit definitions in the received key events
enum
    {
	eventBitLeftAlt     =0x00000004,
	eventBitRightAlt    =0x00000008,
	eventBitAlt         =0x00000010,
	eventBitLeftCtrl    =0x00000020,
	eventBitRightCtrl   =0x00000040,
	eventBitCtrl        =0x00000080,
	eventBitLeftShift   =0x00000100,
	eventBitRightShift  =0x00000200,
	eventBitShift       =0x00000400,
	eventBitLeftFunc    =0x00000800,
	eventBitRightFunc   =0x00001000,
	eventBitFunc        =0x00002000,
	eventBitCapsLock    =0x00004000,
	eventBitNumLock     =0x00008000,
	eventBitScrollLock  =0x00010000
    };

/**
 *  Event input, which may be either a custom event or a key event.
 *  Internal.
 */
OSN_NONSHARABLE_CLASS( AlfEventInput )
    {
public:
public:
    /**
         * accepts the event.
        */
    bool accept( const TAlfEvent& aEvent ) const;

public: // data
	AlfEventInput(int, int, int, int);
    // Event type flags, combination of EventTypeFlags values
    int mEventTypeFlags;
    // Key or custom event id
    int mEventID;
    // Custom event data, if used
    int mEventData;
    // Key event modifiers
    int mKeyEventModifiers;
    };

/**
 *  Pointer event input.
 *  Internal.
 */
OSN_NONSHARABLE_CLASS( AlfPointerEventInput ) //: public CBase
    {
public:
    /**
     * constructor.
     */
    AlfPointerEventInput(const char* aElementName, const char* aVisualName,
                         WidgetPointerEvent aEvent );

    /**
    * Destructor.
    */
    virtual ~AlfPointerEventInput();

public:
    /**
       * accept the event.
       */
    bool accept( CAlfWidgetControl& aControl, const TAlfEvent& aEvent ) const;

private:
    AlfPointerEventInput();

private: // data

    auto_ptr<AlfReferenceToVisual> mVisualRef;
    WidgetPointerEvent mEvent;
    };
OSN_NONSHARABLE_CLASS( AlfWidgetAttributeInput )
    {
public:
    /**
    * Constructor.
    */
    AlfWidgetAttributeInput( AlfAttribute& aAttribute, 
        const char* aElementId );

    ~AlfWidgetAttributeInput();

public:
    /**
       * Executes the event output.
       */
    void execute( CAlfWidgetControl& aControl, const TAlfEvent& aEvent );
    
    bool accept(const TAlfEvent& aEvent) const;

private:
    /**
       * constructor
       */
    AlfWidgetAttributeInput();

private: // data
    AlfAttribute* mAttr;
    // Reference to element
    UString mElementId;
    };    
OSN_NONSHARABLE_CLASS( AlfEventPasserInput )
    {
public:
    /**
    * Constructor.
    */
    AlfEventPasserInput( int aEventId, int aEventData );

    ~AlfEventPasserInput();

public:
    
    bool accept(const TAlfEvent& aEvent) const;

private:
    /**
       * constructor
       */
    AlfEventPasserInput();

private: // data
    int mEventId;
    int mEventData;
    };      
    } // namespace Alf

#endif // ALFEVENTINPUT_H
