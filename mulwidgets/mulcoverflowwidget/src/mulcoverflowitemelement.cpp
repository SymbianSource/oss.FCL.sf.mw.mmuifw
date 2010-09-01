/*
* Copyright (c) 2007-2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Item element implementation for Cover Flow.
*
*/


// This file defines the API for mulcoverflowwidget.dll

// Includes
#include <osn/osnnew.h>
#include <alf/alfwidgetcontrol.h>
#include <alf/alfevent.h>
#include <alf/alfwidgetevents.h>
#include <alf/alfflowlayout.h>

// Local headers
#include "mulvisualtemplate.h"
#include "mulcoverflowcontrol.h"
#include "mulcoverflowdefinitions.h"
#include "mulbaseelement.h"
#include "mullog.h"

// Mul headers
#include "imulmodelaccessor.h"
#include <alf/alfwidget.h>
#include <mul/mulevent.h>


//Class header
#include "mulcoverflowitemelement.h"

namespace Alf
	{	
				
//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------
//		
MulCoverFlowItemElement::MulCoverFlowItemElement( CAlfWidgetControl& aControl, const char* aName, MulTemplateInterface& aControlInterface )
	{
	//Call the base class method      
    construct(aControl,aName);
		
	mVisualRecycled = false;
	mNumVisualsRecycled = 0;
	mIconFlow = NULL;
	}

//-----------------------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------------------
//	
MulCoverFlowItemElement::~MulCoverFlowItemElement()
	{
	// Nothind to delete .
	}

//-----------------------------------------------------------------------------
// accept
//-----------------------------------------------------------------------------
//	
bool MulCoverFlowItemElement::accept(CAlfWidgetControl& /*aControl*/, const TAlfEvent& aEvent) const
	{
	
	if(!aEvent.IsCustomEvent())
		{
		return false;
		}
	return true;	
	}

//-----------------------------------------------------------------------------
// setActiveStates
//-----------------------------------------------------------------------------
//
void MulCoverFlowItemElement::setActiveStates(unsigned int /*aStates*/)
	{
	
	}

// offerEvent
//-----------------------------------------------------------------------------
//
AlfEventStatus MulCoverFlowItemElement::offerEvent(CAlfWidgetControl& /*aControl*/, const TAlfEvent& /*aEvent*/)
	{
	return EEventNotHandled;	
 	}
	
//-----------------------------------------------------------------------------
// makeInterface
//-----------------------------------------------------------------------------
//
IAlfInterfaceBase* MulCoverFlowItemElement::makeInterface(const IfId& /*aType*/)
	{
	return NULL;
	}

//-----------------------------------------------------------------------------
// setEventHandlerData
//-----------------------------------------------------------------------------
//
void MulCoverFlowItemElement::setEventHandlerData( const AlfWidgetEventHandlerInitData& /*aData*/ )
	{
	
	}

//-----------------------------------------------------------------------------
// eventHandlerData
//-----------------------------------------------------------------------------
//  
AlfWidgetEventHandlerInitData* MulCoverFlowItemElement::eventHandlerData()
	{
	return NULL;
	}

// ----------------------------------------------------------------------------
// eventHandlerType
// ----------------------------------------------------------------------------
//
 IAlfWidgetEventHandler::AlfEventHandlerType MulCoverFlowItemElement::eventHandlerType() 
	{
	return IAlfWidgetEventHandler::ELogicalEventHandler ;
	}
// ----------------------------------------------------------------------------
// eventExecutionPhase
// ----------------------------------------------------------------------------
//

IAlfWidgetEventHandler::AlfEventHandlerExecutionPhase MulCoverFlowItemElement::eventExecutionPhase()
	{
	return EBubblingPhaseEventHandler;
	}   
	
			
CAlfVisual* MulCoverFlowItemElement::CreateIconStructure(CAlfLayout* aParentLayout, const UString& aTemplateId)
    {
    return (static_cast<MulVisualTemplate*>(getVisualTemplate()))->CreateIconStructure(aParentLayout, aTemplateId);

    }
    
void MulCoverFlowItemElement::DoSetImage(IAlfVariantType* data,CAlfImageVisual* aImgVisual)
{
	static_cast<MulVisualTemplate*>(getVisualTemplate())->DoSetImage(data, aImgVisual );
}
    
		
	} // namespace ends here
	



