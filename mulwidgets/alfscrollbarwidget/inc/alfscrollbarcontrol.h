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
* Description:  Control interface with utilities for scrollbar widget.
*
*/

#ifndef ALF_SCROLLBARCONTROL_H
#define ALF_SCROLLBARCONTROL_H

//WidgetModel Includes
#include <alf/alfwidgetcontrol.h>
#include <alf/alfwidgeteventhandler.h>
#include <touchfeedback.h>

//Osn Includes
#include <osn/ustring.h>

//Internal Includes

using namespace osncore;
using namespace Alf;

// Forward declarations
//class TAlfCustomEventCommand;

const float roundOff = 0.5;
namespace Alf
    {
// Forward declaration
class AlfScrollBarControl : public CAlfWidgetControl
    {
public:

    /**
     * Constructor.
     */
    AlfScrollBarControl ( CAlfEnv& aEnv );

    /** 
     *  Class Destructor.
     *  @since S60 ?S60_version 
     */ 
    virtual ~AlfScrollBarControl();

    /**
     * Sets whether or not tactile feedback is enabled.
     *
     * @param aEnabled Whether or not tactile is enabled.
     */
    void EnableTactileFeedback( bool aEnabled );
    
    /**
     * Generates the feedback based on the events recieved
     * 
     */
     void TactileEffectOnTouchandHold();  
    
    /**
     * Generates the feedback based on the events recieved
     * 
     * @param aEvent Events recieved by the widget on Drag
     */ 
     void TactileEffectOnDrag(TAlfEvent& aEvent);
     
     /**
      * Generates the feedback based on the thumb release
      * or drag on thumb is stopped 
      */
     void TactileEffectOnRelease();
     
    
    /**
     * Returns the state of tactile feedback whether it is enabled/disabled.
     *
     */
    bool IsTactileFeedbackEnabled();

    // From Base class CAlfControl
    
    /**
     * Notifies the owner that the layout of a visual has been recalculated.
     * Called only when the EAlfVisualFlagLayoutUpdateNotification flag has
     * been set for the visual.
     *
     * @param aVisual  Visual that has been laid out.
     */
    void VisualLayoutUpdated ( CAlfVisual& aVisual );

    /**
     * Returns top level layout. Returns NULL if this control does not
     * implement the IAlfHostAPI interface or if there is no base layout.
     * 
     * @return Pointer to top level layout, or NULL if not available.
     */
    CAlfLayout* ContainerLayout ( const CAlfControl* aConnected ) const;

    // From base class CAlfWidgetControl
    
    /**
     * @see IAlfWidgetControl
     */
    AlfEventStatus handleEvent( const TAlfEvent& aEvent );


private:

    /**
     * Rounds float to int
     * 
     * @param aFloatValue float value
     * @return int
     */
    inline int roundFloatToInt ( float aFloatValue )
        {
        return(int)(aFloatValue + roundOff);
        }

private:

    /** 
     * Default contructor
     */
    AlfScrollBarControl();
  
    /**
     * height of the Layout.
     */
    int mLayoutHeight;

    /**
     * width of the Layout.
     */
    int mLayoutWidth;

    /**
     * tactile is enabled or not.
     */
    bool mEnableTactile;
    
    /**
     * Tactile effect for drag started
     */
    bool mDragTactileFeedbackStarted;

    /**
     * Tactile feedback instance
     */
		MTouchFeedback *mTactilefdbInstance;
		
    }; // class AlfScrollBarControl

    } // namespace Alf

#endif //ALF_SCROLLBARCONTROL_H
//End Of File
