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
* Description:  Type declarations
*
*/



#ifndef ALFTYPES_H
#define ALFTYPES_H

namespace Alf
    {
struct IfId
    {
    unsigned int mIfId;
    const char*  mImplementationId;
    };
    

/**
 * Specfies the result of event processing when an event that 
 * is offered to an event handler.
 */
enum AlfEventStatus
    {
    /**
     * The event handler did not handle the event.
     */
    EEventNotHandled = 0,
    /**
     * The event handler handled the event but does not
     * desire to block the event flow to other interested
     * entities.
     */
    EEventHandled,
    /**
     * The event handler consumed the event and wishes to
     * terminate event flow to other interested enetities.
     * In other words, the event handler acted as a "filter" 
     * for the event.
     */ 
    EEventConsumed,
    };
    }

#endif

// End of File
