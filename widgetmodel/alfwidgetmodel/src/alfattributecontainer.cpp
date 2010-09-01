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
* Description:   Implements the Container class for atrributes.
*
*/


#include <osn/osnnew.h>
#include <osn/alfptrvector.h>
#include <alf/alfdataexception.h>
#include "alf/alfattribute.h"
#include "alf/alfattributecontainer.h"
#include <libc/string.h>
#include <stdexcept>
#include <assert.h> // from libc

namespace Alf
    {


/**
 * Attribute container implementation.
 */
class AlfAttributeContainerImpl
    {
public:
    osncore::AlfPtrVector<AlfAttribute> mAttributes;
    AlfAttributeContainer::Type mType;
    uint mTime;
    };

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Constructor.
// ---------------------------------------------------------------------------
//

OSN_EXPORT AlfAttributeContainer::AlfAttributeContainer(Type aType) :
        mData(new (EMM) AlfAttributeContainerImpl())
    {
    mData->mType = aType;
    mData->mTime = 0;
    }

// ---------------------------------------------------------------------------
// Destructor.
// ---------------------------------------------------------------------------
//
OSN_EXPORT AlfAttributeContainer::~AlfAttributeContainer()
    {

    }

// ---------------------------------------------------------------------------
// Gets the type of the container.
// ---------------------------------------------------------------------------
//
OSN_EXPORT AlfAttributeContainer::Type AlfAttributeContainer::type() const
    {
    return mData->mType;
    }

// ---------------------------------------------------------------------------
// Adds a new attribute to the container.
// ---------------------------------------------------------------------------
//
OSN_EXPORT void AlfAttributeContainer::addAttribute(AlfAttribute* aAttribute)
    {
    mData->mAttributes.resize(mData->mAttributes.count() +1 );
    mData->mAttributes.insert(mData->mAttributes.count(), aAttribute);
    }

// ---------------------------------------------------------------------------
// Returns the attribute count.
// ---------------------------------------------------------------------------
//
OSN_EXPORT unsigned int AlfAttributeContainer::attributeCount()const
    {
    return mData->mAttributes.count();
    }

// ---------------------------------------------------------------------------
// Returns the attribute.
// ---------------------------------------------------------------------------
//
OSN_EXPORT AlfAttribute& AlfAttributeContainer::getAttribute(
    unsigned int aIndex) const
    {
    // invariant
    assert(aIndex < mData->mAttributes.count());
    if (aIndex >= mData->mAttributes.count())
        {
        ALF_THROW(AlfDataException,EInvalidAttribute,"AlfAttributeContainer")
        }

    return *(mData->mAttributes[aIndex]);
    }

// ---------------------------------------------------------------------------
// Returns the attribute.
// ---------------------------------------------------------------------------
//
OSN_EXPORT AlfAttribute& AlfAttributeContainer::getAttributeByName(
    const char * aName) const
    {
    for (TInt i=0; i < mData->mAttributes.count(); i++)
        {
        if (!strcmp(mData->mAttributes[i]->name(), aName))
            {
            return *(mData->mAttributes[i]);
            }
        }
    ALF_THROW(AlfDataException,EInvalidAttribute,"AlfAttributeContainer")
    }

// ---------------------------------------------------------------------------
// Removes the attribute.
// ---------------------------------------------------------------------------
//
OSN_EXPORT void AlfAttributeContainer::removeAttribute(unsigned int aIndex)
    {
    assert(aIndex < mData->mAttributes.count());
    if (aIndex >= mData->mAttributes.count())
        {
        ALF_THROW(AlfDataException,EInvalidAttribute,"AlfAttributeContainer")
        }

    mData->mAttributes.remove(aIndex);
    }

// ---------------------------------------------------------------------------
// Sets the animation time of the attribute container.
// ---------------------------------------------------------------------------
//
OSN_EXPORT void AlfAttributeContainer::setTime(unsigned int aTime)
    {
    mData->mTime = aTime;
    }

// ---------------------------------------------------------------------------
// Returns the animation time of the attribute container.
// ---------------------------------------------------------------------------
//
OSN_EXPORT unsigned int AlfAttributeContainer::getTime()const
    {
    return mData->mTime;
    }

    } // Alf

