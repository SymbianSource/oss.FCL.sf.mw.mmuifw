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
* Description:  Animation class for alfred widget model.
				  This class creates the animation and commands.
*
*
*/




//INCLUDES
#include "alf/alfreferencetovisual.h"
#include "alf/ialfattributesetter.h"
#include "alf/alfattributecontainer.h"
#include <alf/alfvisual.h>
#include <alf/alfcontrol.h>
#include <alf/alfenv.h>
#include <alf/alfexceptions.h>
#include "alf/alfperf.h"
#include "alfanimationoutput.h"

namespace Alf
    {

////////////////////// AlfAnimationOutput //////////////////////


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// Description : Constructor
// -----------------------------------------------------------------------------
//
OSN_EXPORT AlfAnimationOutput::AlfAnimationOutput(
    AlfReferenceToVisual* aTargetVisual,
    AlfReferenceToVisual* aReferenceVisual,
    IAlfAttributeSetter* aAttributeSetter,
    AlfAttributeContainer* aAttributeContainer,
    unsigned int aAnimationTime,
    unsigned int aDelay)
    {
    if (!aTargetVisual || !aAttributeSetter || !aAttributeContainer )
        {
        ALF_THROW( AlfException, EInvalidArgument, "AlfAnimationOutput" )
        }
        
    mTargetVisual = aTargetVisual;
    mReferenceVisual = aReferenceVisual;
    mAttributeSetter = aAttributeSetter;
    mAttributeContainer = aAttributeContainer;
    mAnimationTime = aAnimationTime;
    mDelay = aDelay;
    }

// -----------------------------------------------------------------------------
// Description : Destructor
// -----------------------------------------------------------------------------
//
OSN_EXPORT AlfAnimationOutput::~AlfAnimationOutput()
    {
    delete mTargetVisual;
    delete mReferenceVisual;
    delete mAttributeSetter;
    delete mAttributeContainer;
    }

// -----------------------------------------------------------------------------
// Description : Create and send the animation command
// -----------------------------------------------------------------------------
//
OSN_EXPORT void AlfAnimationOutput::sendCommand(
    CAlfWidgetControl& aControl, unsigned int aDataID )
    {
    CAlfVisual* ref = 0;

    if ( mReferenceVisual )
        {
        // Find the reference visual
        ref = mReferenceVisual->resolve( aControl, aDataID );
        }
    // Find the animated visual
    CAlfVisual* visual = NULL;
    visual = mTargetVisual->resolve( aControl, aDataID );
    if ( visual )
        {
        // Create the animation command(s) and send them through the env.
        ALF_PERF_START( perfdata, "AlfAnimationOutput-sendCommand-createAndSendCommands" )
        // reference visual is optional, it may be 0.
        mAttributeSetter->createAndSendCommands( *visual, mAttributeContainer, ref );
        ALF_PERF_STOP( perfdata, "AlfAnimationOutput-sendCommand-createAndSendCommands" )
        }
    }

    } // namespace Alf

// end of file
