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
* Description:  Control interface with utilities for slider widget.
*
*/

#ifndef MUL_SLIDERCONTROL_H
#define MUL_SLIDERCONTROL_H

//WidgetModel Includes
#include <alf/alfwidgetcontrol.h>
#include <alf/alfwidgeteventhandler.h>
#include "alf/alfelement.h"
#include <touchfeedback.h>
//Osn Includes
#include <osn/ustring.h>


using namespace osncore;
using namespace Alf;

// Forward declarations
class TAlfCustomEventCommand;

const float roundOff = 0.5;
namespace Alf
    {
    // Forward declaration
class IAlfModel;
class AlfModelOperation;
class MulSliderControl : public CAlfWidgetControl
    {
public:

    /**
     * Constructor.
     */
    MulSliderControl ( CAlfEnv& aEnv );

    /** 
     *  Class Destructor.
     *  @since S60 ?S60_version 
     */ 
    virtual ~MulSliderControl();

    //--------------------------------------------------------------------------
	//Overriden Apis from Base class CAlfControl
	//--------------------------------------------------------------------------
	//

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

// from base class CAlfWidgetControl

    /**
     * @see IAlfWidgetControl
     */
    AlfEventStatus handleEvent( const TAlfEvent& aEvent );

    /**
     * Update the presentation according to the data changes.
     *
     * @since S60 ?S60_version
     * @param aArrayOfOperation An Array containing the operations performed on
     * the model.
     */
    void dataChanging ( const AlfModelOperation& aOperation );
    
    /**
     * removes an element.
     */
    void removeAndDestroyElement(const IAlfElement& aElement );


// From base class IAlfInterfaceBase

    /**
     * From IAlfInterfaceBase.
     * Getter for interfaces provided by the control classes.
     * Derived classes should always call the base class method
     * from the overridden MakeInterface.
     *
     * @since S60 ?S60_version
     * @param aType A descriptor to identify the type of the queried interface.
     * @return The queried interface, or NULL if the interface is not
     *         supported or available.
     */
    IAlfInterfaceBase* makeInterface ( const IfId& aType );

public: 
    
    /**
     * Generates the feedback based on the events recieved
     * 
     */
     void TactileEffectOnTouchandHold();  
    
    /**
     * Generates the feedback based on the events recieved
     * 
     * @param aEvent Events recieved by the widget on Drag
     * @param aIntensity Intensity of the feedback.If not passed default will be 50
     */ 
     void TactileEffectOnDrag(TAlfEvent& aEvent , int aIntensity = 50 );
     
     /**
      * Generates the feedback based on the thumb release
      * or drag on thumb is stopped 
      */
     void TactileEffectOnRelease();

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
        
    /**
     * Sets whether or not tactile feedback is enabled.
     *
     * @param aEnabled Whether or not tactile is enabled.
     */
    void EnableTactileFeedback( bool aEnabled );

private:

    /** 
     * Default contructor
     */
    MulSliderControl();
    
    /**
     * IAlfAttributeOwner implementation. Own.
     */
  
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
     * Tactile Feedback for drag started
     */
    bool mDragTactileFeedbackStarted;
    /**
     * pointer to get the current tactile instance
     */
    MTouchFeedback *mTactilefdbInstance;

    }; // class MulSliderControl

    } // namespace Alf

#endif //MUL_SLIDERCONTROL_H
//End Of File
