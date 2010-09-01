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
* Description:  Widget interface with utilities for scrollbar widget.
 *
*/


#ifndef ALF_SCROLLEVENTS_H
#define ALF_SCROLLEVENTS_H

// WidgetModel Includes
#include <alf/alfwidgetevents.h>

// Widget Includes
#include <alf/ialfscrollbarwidget.h>
#include <alf/ialfscrollbardefaultbaseelement.h>

namespace Alf
    {
    // forward declaration
    class IAlfScrollBarDefaultBaseElement;    

    /**
     *  Flags to indicate the changes being done in the scrollbar model
     *  ENoChange Flag to indicate no scrollbar model data is changed 
     *  ESpanChanged Flag to indicate Span is changed
     *  EViewLengthChanged Flag to indicate view length is changed
     *  EViewStartPosChanged Flag to indicate view start position is changed
     *  EFocusPosChanged Flag to indicate focus position is changed
     *
     *  @since S60 ?S60_version
     */
    enum ScrollModelChangeMask
        {
        ENoChange             = 0, // Flag if scrollbar model data is changed
        ESpanChanged          = 1, // Flag to indicate Span is changed 
        EViewLengthChanged    = 2, // Flag to indicate view length is changed
        EViewStartPosChanged  = 4, // Flag if view start position is changed 
        };

    /**
     *  This structure is used to send data along with 
     * EEventScrollModelChanged scroll event
     *
     *  @since S60 ?S60_version
     */
    struct ScrollModelChangedEventData
        {
        int mSpan;          // Span 
        int mViewLength;    // View Length 
        int mViewStartPos;  // View Start position 
        int mChangedMask;   // combination (ORing) of one or more 
                            // TScrollModelChangeMask enums
        };

    } // namespace Alf

#endif // IALF_SCROLLEVENTS_H
//End Of File
