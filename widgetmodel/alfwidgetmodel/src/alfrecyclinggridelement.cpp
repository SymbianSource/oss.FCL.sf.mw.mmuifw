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
* Description:   The implementation for grid element with recycling of visuals supported.
*
*/

#include "alf/alfrecyclinggridelement.h"
#include "alfrecyclinggridelementimpl.h"

#include <alf/alfenv.h>
#include <alf/alfconstants.h>
#include <alf/alfcommand.h>

#include <alf/alfgridlayout.h>
#include <alf/alflayout.h>
#include <alf/alfcontrol.h>
#include <alf/alfevent.h>

#include "alf/ialfvisualtemplate.h"
#include <alf/alfvarianttype.h>
#include <alf/alfwidgetcontrol.h>
#include <alf/alfwidgetevents.h>
#include <osn/ustring.h>
#include <memory>
#include <osn/osnnew.h>

using namespace osncore;

namespace Alf
    {

//----------------------------------------------------------------
// 1st phase Constructor. Leaves the newly created object
// on the cleanup stack
//----------------------------------------------------------------
OSN_EXPORT AlfRecyclingGridElement::AlfRecyclingGridElement(
    CAlfWidgetControl& aControl,
    const char* aName,
    int aNumColumns,
    int aNumRows)
    {
    construct(aControl,aName);

    mImpl.reset( new( EMM ) AlfRecyclingGridElementImpl(
                     *this, aNumColumns, aNumRows ) );

    control().addEventHandler(this);
    }

//----------------------------------------------------------------
// Class Destructor
//----------------------------------------------------------------
OSN_EXPORT  AlfRecyclingGridElement::~AlfRecyclingGridElement()
    {
    control().removeEventHandler(*this);
    }

//----------------------------------------------------------------
// Sets the fill style for the Recycle grid element
//----------------------------------------------------------------
OSN_EXPORT void AlfRecyclingGridElement::setFillStyle(FillStyle aFillStyle, int aFillStyleParam)
    {
    mImpl->setFillStyle( aFillStyle, aFillStyleParam );
    }


OSN_EXPORT void AlfRecyclingGridElement::setAnimationTimes(
    int aScrollingTime, int aFadeInTime, int aFadeInDelay,
    int aFadeOutTime, int aFadeOutDelay )
    {
    mImpl->setAnimationTimes(
        aScrollingTime, aFadeInTime, aFadeInDelay,
        aFadeOutTime, aFadeOutDelay );
    }


OSN_EXPORT CAlfVisual* AlfRecyclingGridElement::createVisualTree(
    IAlfVariantType& aData, uint aDataID, CAlfLayout* aParentLayout, int aLayoutIndex )
    {
    return mImpl->createVisualTree(
               aData, aDataID, aParentLayout, aLayoutIndex );
    }

// ---------------------------------------------------------------------------
// OfferEventL - Handle the Event.
// ?implementation_description
// ---------------------------------------------------------------------------
OSN_EXPORT AlfEventStatus AlfRecyclingGridElement::offerEvent( CAlfWidgetControl& aControl, const TAlfEvent& aEvent )
    {
    return mImpl->offerEvent( aControl, aEvent )
                                ?EEventHandled:EEventNotHandled;
    }

// ---------------------------------------------------------------------------
// ?implementation_description
// ---------------------------------------------------------------------------
//
OSN_EXPORT void AlfRecyclingGridElement::setActiveStates( unsigned int aStates )
    {
    mImpl->setActiveStates( aStates );
    }

// ---------------------------------------------------------------------------
// From class IAlfWidgetEventHandler.
// sets event handler data.
// ---------------------------------------------------------------------------
//
OSN_EXPORT void AlfRecyclingGridElement::setEventHandlerData( const AlfWidgetEventHandlerInitData& aData )
    {
    mImpl->setEventHandlerData(aData);
    }

// ---------------------------------------------------------------------------
// From class IAlfWidgetEventHandler.
// returns the eventhandlertype .
// ---------------------------------------------------------------------------
//
IAlfWidgetEventHandler::AlfEventHandlerType AlfRecyclingGridElement::eventHandlerType()
    {
    return IAlfWidgetEventHandler::EPresentationEventHandler;
    }

// ---------------------------------------------------------------------------
// From class IAlfWidgetEventHandler.
// returns eventhandler excecutionphase.
// ---------------------------------------------------------------------------
//
IAlfWidgetEventHandler::AlfEventHandlerExecutionPhase AlfRecyclingGridElement::eventExecutionPhase()
    {
    return IAlfWidgetEventHandler::ETunnellingPhaseEventHandler;
    }
// returns event handler data.
// ---------------------------------------------------------------------------
//
OSN_EXPORT AlfWidgetEventHandlerInitData* AlfRecyclingGridElement::eventHandlerData()
    {
    return mImpl->eventHandlerData();
    }
    
// ---------------------------------------------------------------------------
// From class IAlfInterfaceBase.
// ?implementation_description
// ---------------------------------------------------------------------------
//
OSN_EXPORT IAlfInterfaceBase* AlfRecyclingGridElement::makeInterface( const IfId& aType )
    {
    IAlfInterfaceBase* ret(0);
    UString param(aType.mImplementationId);

    if (param == AlfRecyclingGridElement::type().mImplementationId )
        {
        ret = static_cast<IAlfElement*>(this);
        }
    else if (param == IAlfWidgetEventHandler::type().mImplementationId )
        {
        ret = static_cast<IAlfWidgetEventHandler*>(this);
        }
    else
        {
        ret = AlfElement::makeInterface(aType);
        }
    return ret;
    }


OSN_EXPORT bool AlfRecyclingGridElement::accept( CAlfWidgetControl& aControl, const TAlfEvent& aEvent ) const
    {
    return mImpl->accept( aControl, aEvent );
    }

OSN_EXPORT void AlfRecyclingGridElement::createChildVisualTree(
    IAlfElement* aElement, IAlfVariantType& aChildData,
    IAlfBranch& aData, int aIndex, uint aDataID )
    {
    mImpl->createChildVisualTree( aElement, aChildData, aData, aIndex, aDataID );
    }

OSN_EXPORT void AlfRecyclingGridElement::removeChildVisualTree(
    IAlfElement* aElement, IAlfBranch& aData, int aIndex, uint aDataID )
    {
    mImpl->removeChildVisualTree( aElement, aData, aIndex, aDataID );
    }

// ---------------------------------------------------------------------------
// Description: Do not destroy visuals, because it's done elsewhere.
// ---------------------------------------------------------------------------
void AlfRecyclingGridElement::removeAndDestroyVisuals( int aTimeMilliseconds )
    {
    mImpl->removeAndDestroyVisuals( aTimeMilliseconds );
    }

    } //Alf
