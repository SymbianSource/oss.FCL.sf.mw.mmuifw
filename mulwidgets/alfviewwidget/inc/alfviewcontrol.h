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
* Description:   View Widget control Class Header.
*
*/


#ifndef ALFVIEWCONTROL_H_
#define ALFVIEWCONTROL_H_

#include <alf/alfwidgetcontrol.h>

namespace Alf
    {
class AlfViewWidget;
    
/**
 * Control class of the view widget.
 *
 * This class implements logic of the view widget.
 *
 * View widget automatically creates an CAlfViewControl object
 * for it's control.
 */
class CAlfViewControl : public CAlfWidgetControl
    {
public:

    /**
     * Default constructor
     *
     * @param aEnv UI Accelerator Toolkit environment where this view widget
     *       is created.
     * @param aViewWidget The view widget with which this control is associated with.
     */
    CAlfViewControl(CAlfEnv& aEnv, AlfViewWidget& aViewWidget);

    /**
     * Default destructor
     */
    virtual ~CAlfViewControl();
    
 // From base class CAlfWidgetControl

    /**
     * Handles events sent to widget control.
     *
     * @since S60 ?S60_version
     * @param aEvent The event to be handled.
     * @return see AlfEventStatus.
     */
   	virtual AlfEventStatus handleEvent(const TAlfEvent& aEvent);
   	
// New methods   	
   	
   	/**
   	 * Returns the view widget associated with this control.
   	 * The widget() method returns the AlfWidget implementation class
   	 * object that is used as an adapter in the AlfViewWidget object.
   	 */
   	AlfViewWidget& viewWidget();
   	   	   	
private:

	/** View widget in which this control is used */
	AlfViewWidget& mViewWidget;
   	
    };

    }

#endif /*ALFVIEWCONTROL_H_*/
