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


#ifndef ALFCONTAINERCONTROL_H_
#define ALFCONTAINERCONTROL_H_

#include <alf/alfwidgetcontrol.h>

namespace Alf
{

/**
 * Control class of the container widget.
 * 
 * This class implements logic of the container widget.
 * 
 * Container widget automatically creates an AlfContainerControl object
 * for it's control. It is not possible to construct this control through
 * AlfContainerWidgetFactoryPlugin.  
 */
class AlfContainerControl : public CAlfWidgetControl
{
public:

	/**
	 * Default constructor
	 * 
	 * @param aEnv UI Accelerator Toolkit environment where this container widget
	 * 			   is created.
	 */
	AlfContainerControl(CAlfEnv& aEnv);
	
	/**
	 * Default destructor
	 */
	virtual ~AlfContainerControl();
	
    /**
     * Handles events sent to widget control.
     *
     * @since S60 ?S60_version
     * @param aEvent The event to be handled.
     * @return True if the event is handled, false elsevhere.
     */ 
	virtual AlfEventStatus handleEvent( const TAlfEvent& aEvent );
};

}

#endif /*ALFCONTAINERCONTROL_H_*/
