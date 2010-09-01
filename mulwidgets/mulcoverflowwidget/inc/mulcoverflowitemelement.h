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
* Description:  Item element header for Cover Flow.
*
*/

// This file defines the API for mulcoverflowwidget.dll
#ifndef MULCOVERFLOWITEMELEMENT_H_
#define MULCOVERFLOWITEMELEMENT_H_


// Includes
#include <alf/alfelement.h>
#include <alf/ialfwidgeteventhandler.h>

#include <vector>

#include "mulvisualtemplate.h"
// Forward declarations.
class MulTemplateInterface;

namespace Alf
	{
		
class MulCoverFlowItemElement: public AlfElement, public IAlfWidgetEventHandler
	{
				
public: // Constructor and Destructor
	
   /**
	* Constructor
	* @param aControl WidgetControl
	* @param aName Name of the element.	
	*/
	// third parameter shd be moved.
	MulCoverFlowItemElement( CAlfWidgetControl& aControl, const char* aName , MulTemplateInterface& aControlInterface );
	
   /**
    * Destructor
	*/
	~MulCoverFlowItemElement();
	
public: // From IAlfWidgetEventHandler

    bool accept( CAlfWidgetControl& aControl, const TAlfEvent& aEvent ) const;
    
    void setActiveStates( unsigned int aStates );
    
    AlfEventStatus offerEvent( CAlfWidgetControl& aControl, const TAlfEvent& aEvent );
    
    IAlfInterfaceBase* makeInterface( const IfId& aType );

    void setEventHandlerData( const AlfWidgetEventHandlerInitData& aData );
	  
	AlfWidgetEventHandlerInitData* eventHandlerData();
	
	  AlfEventHandlerType eventHandlerType() ;

    AlfEventHandlerExecutionPhase eventExecutionPhase() ;

    
public: // new methods 
	
	CAlfVisual* CreateIconStructure(CAlfLayout* aParentLayout, const UString& aTemplateId);  
    
    void DoSetImage(IAlfVariantType* data, CAlfImageVisual* aImgVisual);
	      	
			

private:

   /**
 	* gets the handle to the visual template.
 	*/
  	MulVisualTemplate* VisualTemplate()
		{
		return static_cast<MulVisualTemplate*>(getVisualTemplate());
		}
private: // Member data
	
    std::vector<CAlfVisual*> mRecycleVisualArray;        //stores sizes and positions of all items 
	bool mVisualRecycled;
	int mNumVisualsRecycled;
	CAlfFlowLayout* mIconFlow;	
	};
	
	} // namespace ends here
	
#endif /*MULCOVERFLOWITEMELEMENT_H_*/
