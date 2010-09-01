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
* Description:  Visual class for alfred widget model.
      This class queries the visual by name and ID.
*
*/




//INCLUDES

#include "alf/alfreferencetovisual.h"
#include <alf/alfwidgetcontrol.h>
#include <alf/alfvisual.h>
#include "alf/alfelement.h"
#include <osn/ustring.h>
#include <osn/osnnew.h>
#include <alf/alfexceptions.h>

//using namespace osncore;

namespace Alf
    {
////////////////////// CAlfReferenceToVisual //////////////////////


// ============================ MEMBER FUNCTIONS ===============================

OSN_EXPORT AlfReferenceToVisual* AlfReferenceToVisual::create(
    const char *aElementName,
    const char *aVisualName,
    bool aUseDataID)
    {
    return new( EMM ) AlfReferenceToVisual( aElementName, aVisualName, aUseDataID );
    }

// ---------------------------------------------------------------------------
// Description : Constructor
// ---------------------------------------------------------------------------
//
AlfReferenceToVisual::AlfReferenceToVisual(const char *aElementName,
        const char *aVisualName, bool aUseDataID)
    {
    mUseDataID = aUseDataID;
    mElementName = UString(aElementName);
    mVisualName = UString(aVisualName);
    }

// ---------------------------------------------------------------------------
// Description : Class Destructor
// ---------------------------------------------------------------------------
//
OSN_EXPORT AlfReferenceToVisual::~AlfReferenceToVisual()
    {

    }

// ---------------------------------------------------------------------------
// Description :Find the referenced visual according to the given data ID.
// ---------------------------------------------------------------------------
//
OSN_EXPORT CAlfVisual *AlfReferenceToVisual::resolve(CAlfWidgetControl &aControl, unsigned int aDataID)
    {
    // Find the element.
    CAlfVisual* visual = NULL;
    IAlfElement *element = aControl.findElement(mElementName.getUtf8());
    if (element)
        {
        unsigned int dataID = aDataID;

        // Find the visual.
        visual = element->findVisual(mVisualName.getUtf8(), dataID);
        if (!visual)
            {
            // maybe global visual.
            visual = element->findVisual(mVisualName.getUtf8(), 0);
            }
        }
    return visual;
    }

// ---------------------------------------------------------------------------
// Description :Finds the referenced visual according to the given pointer.
// ---------------------------------------------------------------------------
//
OSN_EXPORT CAlfVisual *AlfReferenceToVisual::resolve(CAlfWidgetControl &aControl, CAlfVisual* aSourceVisual)
    {
    // Find the element.
    CAlfVisual* visual = NULL;
    //Get the element with the name stored herein
    IAlfElement *element = aControl.findElement(mElementName.getUtf8());
    if (element)
        {
        //is passed visual part of the element
        if (element->contains(*aSourceVisual))
            {
            HBufC8 *buf8 = NULL;
            TRAPD(error,buf8 = aSourceVisual->Tag().AllocL());
            if (error != KErrNone)
            	{
            	ALF_THROW(AlfVisualException,ECommonError,"AlfReferenceToVisual")
            	}

            UString sourceTag((const char*)buf8->Des().PtrZ());
            if (mVisualName.compare(sourceTag)==0) //Is this visual same as what was stored herein?
           		{
           		visual = aSourceVisual;
           		}
            delete buf8;
            }
        }
    return visual;
    }

    } // namespace Alf
