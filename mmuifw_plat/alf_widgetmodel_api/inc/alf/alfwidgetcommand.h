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
* Description:  widget commands
*
*/


#ifndef ALFWIDGETCOMMAND_H
#define ALFWIDGETCOMMAND_H

#include <alf/alfcommand.h>
#include <osn/ustring.h>

using namespace osncore;

namespace Alf
    {

// FORWARD DECLARATIONS
    class AlfAttribute;
    class AlfWidget;

/**
 * @class TAlfWidgetAttributeCommand alfwidgetcommand.h "alf/alfwidgetcommand.h"
 *
 * AlfCommand type for carrying an attribute and delivering 
 * it to a presentation element of a recipient widget.
 *
 * @see AlfAttribute
 * @since S60 ?S60_version
 */
class TAlfWidgetAttributeCommand : public TAlfObjectCommand
    {
    public:
    /**
     * Constructor.
     *
     * @param aAttribute The attributes to be set
     * @param aTarget Name of the target AlfElement instance 
     * @param aRecipient Widget that the command is to be executed on
     * @since S60 ?S60_version
     */
    TAlfWidgetAttributeCommand( 
        AlfAttribute* aAttribute, 
        UString& aTarget,
        AlfWidget* aRecipient );
        
    /**
     * Destructor
     */
    ~TAlfWidgetAttributeCommand();
    
    /* Implementation of TAlfCommand::ExecuteL(). */
    void ExecuteL(CAlfEnv& aEnv) const;
    
    /* Implementation of TAlfCommand::Size(). */
    TInt Size() const;

    public:
    AlfAttribute* mOwnedAttribute; // Owned
    UString mTarget;
    };
       

    } // namespace Alf

#endif // ALFWIDGETCOMMAND_H
