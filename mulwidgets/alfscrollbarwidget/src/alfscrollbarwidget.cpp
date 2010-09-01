/*
* Copyright (c) 2007, 2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation for scrollbar widget.
 *
*/


// Alfred Client includes
#include <alf/alfenv.h>

// Widget Model includes
#include "alf/alfelement.h"
#include <alf/alfexception.h>

// Widget Includes
#include <alf/ialfscrollbardefaultbaseelement.h>
#include <alf/alfscrollbarproductids.h>
// Internal includes

#include "alfscrollbardefaultbaseelement.h"
#include "alfscrollbarwidget.h"
#include "alfscrollbarmodel.h"
#include "alfscrollbarcontrol.h"
#include <alf/alfwidgetenvextension.h>
using namespace alfscrollbarwidget;

using Alf::CommonWidgetEvent;
namespace Alf
    {

// ---------------------------------------------------------------------------
//  Creator Method.
// ---------------------------------------------------------------------------
//        
AlfScrollBarWidget* AlfScrollBarWidget :: create(
    CAlfEnv& aEnv, 
    IAlfContainerWidget& aContainer,
    const char* aWidgetId, 
    DuiNode *aNode, 
    const char *aFilePath, 
    AlfCustomInitDataBase* aCustomData)
    {
    return (new( EMM ) AlfScrollBarWidget(aEnv, 
        aContainer, aWidgetId, aNode, aFilePath, aCustomData));
    }


// ---------------------------------------------------------------------------
//  Destructor.
// ---------------------------------------------------------------------------
//        
AlfScrollBarWidget::~AlfScrollBarWidget()
    {
    if(mScrollModel)
        {
        delete mScrollModel;
        }
    else 
        {
        
        }
    }
    
    
    
// ---------------------------------------------------------------------------
// From class IAlfWidget.
// Getter for the parent container. The ownership not passed.
// ---------------------------------------------------------------------------
//
IAlfContainerWidget* AlfScrollBarWidget::parent() const
    {
    return mWidget->parent();
    }
    
    
// From class MAlfInterfaceBase.

// ---------------------------------------------------------------------------
// Getter for interfaces provided by the scrollbar widget.
// ---------------------------------------------------------------------------
//
IAlfInterfaceBase* AlfScrollBarWidget :: makeInterface(const IfId& aType)
    {
    UString param ( aType.mImplementationId );

    if (param == IAlfScrollBarWidget::type().mImplementationId)
        {
        return static_cast<IAlfScrollBarWidget*>(this);
        }
    else if (param == IAlfWidget::type().mImplementationId)
        {
        return static_cast<IAlfWidget*>(this);
        }
    else
        {
        //do nothing
        }

    return mWidget->makeInterface(aType);
    }
    
      
// From class IAlfWidget.
    
// ---------------------------------------------------------------------------
// Getter for the control. The ownership is not passed.
// ---------------------------------------------------------------------------
//
CAlfWidgetControl* AlfScrollBarWidget :: control() const
    {
    return mWidget->control();
    }
    

// ---------------------------------------------------------------------------
// Setter for the control. The control is owned by the Alfred environment.
// ---------------------------------------------------------------------------
//    
void AlfScrollBarWidget :: setControl( 
    CAlfWidgetControl* aControl, bool /*aDeletePreviousControl*/ )
    {
    mScrollControl = aControl;
    mWidget->setControl(aControl);
    }


// ---------------------------------------------------------------------------
// Getter for the model. The ownership is not passed.
// ---------------------------------------------------------------------------
//
IAlfModel* AlfScrollBarWidget :: model()
    {    
    return mScrollModel;
    }
    

// ---------------------------------------------------------------------------
// Setter for the model. Ownership is passed and the old model is released.
// ---------------------------------------------------------------------------
//
void AlfScrollBarWidget :: setModel( IAlfModel* aModel, bool /*aTakeOwnerShip*/)
    {
    IAlfElement* baseelement = control()->findElement ("BaseElement");
    if(baseelement)
    	{
    IAlfScrollBarBaseElementInternal* elementInternal = 
            static_cast<IAlfScrollBarBaseElementInternal*> (
    baseelement->makeInterface(
                    IAlfScrollBarBaseElementInternal::type()));
            
    if(aModel==NULL)
        {
        if(elementInternal)
        	{
        	elementInternal->destroyVisualization();
        	}
        delete mScrollModel;
        mScrollModel = NULL;
        }
    else
        {
        if(mScrollModel)
            {
            delete mScrollModel;
            mScrollModel = NULL;
            
            mScrollModel = static_cast<IAlfScrollBarModel*>( aModel->makeInterface(
                IAlfScrollBarModel::type()));
            AlfScrollBarModel* scrollBarModel = (AlfScrollBarModel*)mScrollModel;
            // Store the widget pointer in model
            scrollBarModel->storeWidget(this);
            
            if(mScrollModel && elementInternal)
                {
                elementInternal->initializeScrollModel();
                elementInternal->updateVisualization();
                }
            }
        else
            {
            mScrollModel =static_cast<IAlfScrollBarModel*>( aModel->makeInterface(
                IAlfScrollBarModel::type()));
            AlfScrollBarModel* scrollBarModel = (AlfScrollBarModel*)mScrollModel;
            scrollBarModel->storeWidget(this);
            
            if(mScrollModel && elementInternal)
                {
                elementInternal->createVisualization();
                }
             }

        }
    	}
    }
    

// ---------------------------------------------------------------------------
// Get the name of the widget instance.
// ---------------------------------------------------------------------------
//
const char* AlfScrollBarWidget :: widgetName() const
    {
    return mWidget->widgetName();
    } 
    
    
// ---------------------------------------------------------------------------
// Sets/Releases the Focus from child widget of container.
// ---------------------------------------------------------------------------
//
void AlfScrollBarWidget :: setChildFocus(bool /*aFocus*/)
    {
    }
    
    
//---------------------------------------------------------------------------
// Creates the presentation for the widget from XML. Destroys any existing
// presentation.
//---------------------------------------------------------------------------
//    
void AlfScrollBarWidget :: setPresentation(const char* /*aFilePath*/)
    {
    } 


            
// ---------------------------------------------------------------------------
//  C++ default constructor.
// ---------------------------------------------------------------------------
//
AlfScrollBarWidget :: AlfScrollBarWidget(
    CAlfEnv& aEnv, IAlfContainerWidget& aContainer,
    const char* aWidgetId, DuiNode * /*aNode*/, 
    const char * /*aFilePath*/,
    AlfCustomInitDataBase* /*aCustomData*/)
    { 
    mScrollModel = NULL;
    mWidget.reset(new(EMM) AlfWidget(aWidgetId,aContainer,aEnv));
    // Default Imperative Construction
    constructDefault(aEnv);

    }


// ---------------------------------------------------------------------------
// Constructs default Visualization for widget.
// ---------------------------------------------------------------------------
//
void AlfScrollBarWidget :: constructDefault(CAlfEnv& aEnv)
    {
    const char* mBaseElementLoadId=KScrollBarDefaultBaseElement; 
    
    // Create Control
    CAlfWidgetControl* ctrl = constructDefaultControl(aEnv);
    setControl(ctrl);
    
    // Create Element
    IAlfWidgetFactory& widgetFactory = AlfWidgetEnvExtension::widgetFactory(control()->Env());
    
    widgetFactory.createElement<IAlfElement>(mBaseElementLoadId, 
					         	             baseElementName(),
					         	             *control());    
    }
    
// ---------------------------------------------------------------------------
//  Construct and returns Scrollbar Control.
// ---------------------------------------------------------------------------
//
CAlfWidgetControl *AlfScrollBarWidget :: constructDefaultControl(CAlfEnv& aEnv)
    {
    return(new(EMM) AlfScrollBarControl(aEnv) );
    }    

// ---------------------------------------------------------------------------
//  Enables tactile feedback based on the flag state
// ---------------------------------------------------------------------------
//
void AlfScrollBarWidget ::enableTactile(bool aFlag)
	{
	AlfScrollBarControl* scrollControl = (AlfScrollBarControl*)control();
	scrollControl->EnableTactileFeedback(aFlag);
	}

// ---------------------------------------------------------------------------
//  Getter for tactile feedback state
// ---------------------------------------------------------------------------
//               
bool AlfScrollBarWidget ::IsTactileEnabled()
	{
	AlfScrollBarControl* scrollControl = (AlfScrollBarControl*)control();
	return scrollControl->IsTactileFeedbackEnabled();
	}  
    
    }//End of Alf
    
