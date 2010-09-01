/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  widget command implementation
*
*/



//INCLUDES
#include <alf/alfwidgetcommand.h>
#include <alf/alfwidgetevents.h>
#include <alf/alfevent.h>
#include "alf/alfwidget.h"
#include "alf/alfattribute.h"

namespace Alf
{
TAlfWidgetAttributeCommand::TAlfWidgetAttributeCommand(  
    AlfAttribute* aAttribute, 
    UString& aTarget,
    AlfWidget* aRecipient
    ) : TAlfObjectCommand(EAlfCommandTypeNone, aRecipient, EAlfOpNone),
        mOwnedAttribute( aAttribute ),
        mTarget( aTarget )
    {
    }

void TAlfWidgetAttributeCommand::ExecuteL(CAlfEnv& /*aEnv*/) const
    {
    TAlfEvent newEvent( EEventWidgetAttributeChanged, (TInt)this );
    AlfWidget* widget = ((AlfWidget*) Object());
    if (widget)
        {
        CAlfControl* control = widget->control(); // Widgets are guaranteed to have controls
        control->OfferEventL(newEvent);
        }
    }

TAlfWidgetAttributeCommand::~TAlfWidgetAttributeCommand()
    {
    delete mOwnedAttribute;
    mOwnedAttribute = NULL;
    }

TInt TAlfWidgetAttributeCommand::Size() const
    {
    return sizeof(*this);
    }

}//namespace Alf;

