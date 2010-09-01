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
* Description: Implementation for slider widget.
 *
*/


// Alfred Client includes
#include <alf/alfenv.h>

// Widget Model includes
#include "alf/alfelement.h"

// Widget Includes
#include "imulsliderbaseelementinternal.h"
#include <mul/mulsliderproductids.h>
// Internal includes
#include "mulslidervertical.h"
#include "mulsliderwidget.h"
#include "mulslidermodel.h"
#include "mulslidercontrol.h"
#include <alf/alfwidgetenvextension.h>
using namespace mulsliderwidget;

using Alf::CommonWidgetEvent;
namespace Alf
    {

// ---------------------------------------------------------------------------
//  Creator Method.
// ---------------------------------------------------------------------------
//        
MulSliderWidget* MulSliderWidget :: create(
    CAlfEnv& aEnv, 
    IAlfContainerWidget& aContainer,
    const char* aWidgetId, 
    AlfCustomInitDataBase* aCustomData)
    {
    
    return (new( EMM ) MulSliderWidget(aEnv, 
        aContainer, aWidgetId,aCustomData));
    }


// ---------------------------------------------------------------------------
//  Destructor.
// ---------------------------------------------------------------------------
//        
MulSliderWidget::~MulSliderWidget()
    {
    if(mSliderModel)
        {
        delete mSliderModel;
        }
    else 
        {
        
        }
    }
    
/**
 * Get the model for widget.
 *
 * @return model of the slider widget
 */
IMulSliderModel& MulSliderWidget::SliderModel() 
    {
    return *mSliderModel;
    }
/**
 * Set the transparency for the background of the slider widget.
 *
 * @param True,  to set background as transparent
 * @param False, to set background as opaque	
 */
 void MulSliderWidget::MakeTransparent( bool aTransparent) 
    {
    mElement->MakeTransparent(aTransparent);
    }

/**
 * Enable/Disable key handling .
 *
 * @param aVal: True,  to handle keyevents by Application
 * 		 	  False, to handle keyevents as defined in the control	
 */
void MulSliderWidget::SetHandleKeyEvent( bool aVal ) 
    {
    mHandleKeyEvent = aVal;
    }
/**
 * 
 * Returns KeyEvent enable status
 *	
 */    
bool MulSliderWidget::IsKeyEnabled()
    {
    return mHandleKeyEvent;
    }
    
// ---------------------------------------------------------------------------
// From class IAlfWidget.
// Getter for the parent container. The ownership not passed.
// ---------------------------------------------------------------------------
//
IAlfContainerWidget* MulSliderWidget::parent() const
    {
    return mWidget->parent();
    }
    
    
// From class MAlfInterfaceBase.

// ---------------------------------------------------------------------------
// Getter for interfaces provided by the slider widget.
// ---------------------------------------------------------------------------
//
IAlfInterfaceBase* MulSliderWidget :: makeInterface(const IfId& aType)
    {
    UString param ( aType.mImplementationId );

    if (param == IMulSliderWidget::type().mImplementationId)
        {
        return static_cast<IMulSliderWidget*>(this);
        }
    else if (param == IAlfWidget::type().mImplementationId)
        {
        return static_cast<IAlfWidget*>(this);
        }

    else
        {
        //do nothing
        }

    // @todo other interfaces
    return mWidget->makeInterface(aType);
    }
    
      
// From class IAlfWidget.
    
// ---------------------------------------------------------------------------
// Getter for the control. The ownership is not passed.
// ---------------------------------------------------------------------------
//
CAlfWidgetControl* MulSliderWidget :: control() const
    {
    //return mSliderControl;
    return mWidget->control();
    }
    

// ---------------------------------------------------------------------------
// Setter for the control. The control is owned by the Alfred environment.
// ---------------------------------------------------------------------------
//    
void MulSliderWidget :: setControl( 
    CAlfWidgetControl* aControl, bool /*aDeletePreviousControl*/ )
    {
    mSliderControl = aControl;
    mWidget->setControl(aControl);
    }


// ---------------------------------------------------------------------------
// Getter for the model. The ownership is not passed.
// ---------------------------------------------------------------------------
//
IAlfModel* MulSliderWidget :: model()
    {    
    return mSliderModel;
    }
    

// ---------------------------------------------------------------------------
// Setter for the model. Ownership is passed and the old model is released.
// ---------------------------------------------------------------------------
//
void MulSliderWidget :: setModel( IAlfModel* aModel, bool /*aTakeOwnerShip*/)
    {
    
    if(aModel==NULL)
        {
        // Destroy the visualization if Model is NULL.
			if(mElement)
			{
    		// Destroy the visual tree
			mSliderControl->Visual(0).RemoveAndDestroyAllD();
    		// Removes the element from control and destroys it
            mSliderControl->removeAndDestroyElement(*((IAlfElement *)mElement));
            mElement = NULL;
			}
        delete mSliderModel;
        mSliderModel = NULL;
        }
    else 
        {
        if(mSliderModel)
            {
            delete mSliderModel;
            mSliderModel = NULL;
            }
         mSliderModel = static_cast<IMulSliderModel*>( aModel->makeInterface(
			          IMulSliderModel::type()));
        MulSliderModel* sliderModel = (MulSliderModel*)mSliderModel;
        // Store the widget pointer in model 
        sliderModel->storeWidget(this);
		if(mElement)
    		{
    		// Destroy the visual tree
    		mSliderControl->Visual(0).RemoveAndDestroyAllD();
    		// Removes the element from control and destroys it
            mSliderControl->removeAndDestroyElement(*((IAlfElement *)mElement));
            mElement = NULL;
    		}
		if(mSliderModel)
        	{
            sliderTemplate tmplateID = sliderModel->GetTemplate();
            if(tmplateID != ESliderTemplateNone)
                {
                switch(tmplateID)
                    {
                	case ESliderTemplate3:// zoom slider
                	  CreateSliderElement(KSliderVerticalElement);
                	  break;
                	case ESliderTemplate7:// progressbar
                	  CreateSliderElement(KSliderProgressbarElement);
                	  break;
                	default:// rest all tempaltes are horizonatal
                	  CreateSliderElement(KSliderHorizontalElement);
                	}
                if(mElement)
                    {
                    mElement->createVisualization(tmplateID);
                    ShowWidget(!IsHidden());
                    }
                
                }
            }
        }
    }// End of setModel 

// ---------------------------------------------------------------------------
// Get the name of the widget instance.
// ---------------------------------------------------------------------------
//
const char* MulSliderWidget :: widgetName() const
    {
    return mWidget->widgetName();
    } 
    
// ---------------------------------------------------------------------------
// Sets/Releases the Focus from child widget of container.
// ---------------------------------------------------------------------------
//
void MulSliderWidget::setChildFocus(bool /*aFocus*/)
    {
    }
    
//---------------------------------------------------------------------------
// Creates the presentation for the widget from XML. Destroys any existing
// presentation.
//---------------------------------------------------------------------------
//    
void MulSliderWidget::setPresentation(const char* /*aFilePath*/)
    {
    } 
    
    
//---------------------------------------------------------------------------
// Gets the Widget Opacity
// 
//---------------------------------------------------------------------------
// 
bool MulSliderWidget::GetOpacity()
    {
    return mOpacity;
    }

//---------------------------------------------------------------------------
// Gets the Widget Opacity
// 
//---------------------------------------------------------------------------
// 
bool MulSliderWidget::IsHidden()
    {
    return !mOpacity;
    }
    
// ---------------------------------------------------------------------------
//   Default constructor.
// ---------------------------------------------------------------------------
//
MulSliderWidget::MulSliderWidget(
    CAlfEnv& aEnv, IAlfContainerWidget& aContainer,
    const char* aWidgetId, 
    AlfCustomInitDataBase* /*aCustomData*/)
    { 
    // Default widget is shown 
    mOpacity = true;
    mSliderModel = NULL;
    mElement = NULL;
    mHandleKeyEvent = true;
    mWidget.reset(new(EMM) AlfWidget(aWidgetId,aContainer,aEnv));
    // Default Imperative Construction
    // Create Control
    CAlfWidgetControl* ctrl = constructDefaultControl(aEnv);
    setControl(ctrl);
    }
    
void MulSliderWidget::CreateSliderElement(const char* aElementId)
    {
    IAlfWidgetFactory& widgetFactory = AlfWidgetEnvExtension::widgetFactory(
                        control()->Env());
    
    IAlfElement * baseelement = widgetFactory.createElement<IAlfElement>
         	        (   aElementId, 
         	            baseElementName(),
         	            *control(), 
         	            NULL, 
         	            NULL );
         	            
    // Store the element pointer
    mElement = static_cast<IMulSliderBaseElementInternal*>(
    baseelement->makeInterface(
    IMulSliderBaseElementInternal::type()));     	            
    }   
    
// ---------------------------------------------------------------------------
//  Construct and returns Slider Control.
// ---------------------------------------------------------------------------
//
CAlfWidgetControl *MulSliderWidget::constructDefaultControl(CAlfEnv& aEnv)
    {
    return(new(EMM) MulSliderControl(aEnv) );
    } 
       
void MulSliderWidget::changeTemplate(int /*aTemplateId*/)
    {
	if( mSliderModel )
		{
    	if(mElement)
    		{
    		mSliderControl->Visual(0).RemoveAndDestroyAllD();
    			mSliderControl->removeAndDestroyElement(*((IAlfElement*)mElement));
    		// delete baseelement;
    		mElement = NULL;
    		}
        
		MulSliderModel* sliderModel = (MulSliderModel*)mSliderModel;
		sliderTemplate i = sliderModel->GetTemplate();
		switch(i)
			{
			case ESliderTemplateNone :// no template id is set
				return;
			case ESliderTemplate3:// zoom slider
			  CreateSliderElement(KSliderVerticalElement);
			  break;
    		case ESliderTemplate7:// progressbar
    		  CreateSliderElement(KSliderProgressbarElement);
    		  break;
    		default:// rest aal tempaltes are horizonatal
			  CreateSliderElement(KSliderHorizontalElement);
			}        
        mElement->createVisualization(i);
        ShowWidget(!IsHidden());
        }
    }
	    	
void MulSliderWidget::ShowWidget(bool opacity,int aTransitionTime)
    {
    mOpacity = opacity;
	if(mElement && mSliderModel)
	    {
		MulSliderModel* sliderModel = static_cast<MulSliderModel*>(mSliderModel);
		
		if( sliderModel->GetTemplate() == ESliderTemplate3 && !mOpacity)
			{
	        mElement->stopEvents();
			}
	    CAlfLayout *layout = mSliderControl->ContainerLayout(NULL);
	    TAlfTimedValue t1(0,aTransitionTime);
	    if(opacity)
    	    {
    	    t1.SetTarget(1,aTransitionTime);
    	    }
    	
	    layout->SetOpacity(t1);
	    }
    }
    
// ---------------------------------------------------------------------------
// Get the root layout of slider
// ---------------------------------------------------------------------------
//
const CAlfLayout& MulSliderWidget ::ContainerLayout()
    {
	CAlfLayout*  layout = mSliderControl->ContainerLayout(NULL);
	return *layout;
    }

// ---------------------------------------------------------------------------
// AddEventHandler
// ---------------------------------------------------------------------------
//	    
void MulSliderWidget::AddEventHandler( IAlfWidgetEventHandler& aObserver)
	{
	control()->addEventHandler(&aObserver);
	}

// ---------------------------------------------------------------------------
// RemoveEventHandler
// ---------------------------------------------------------------------------
//
void MulSliderWidget::RemoveEventHandler( IAlfWidgetEventHandler& aObserver)
	{
	control()->removeEventHandler(aObserver);
	}
// ---------------------------------------------------------------------------
// RemoveEventHandler
// ---------------------------------------------------------------------------
//
IMulSliderBaseElementInternal* MulSliderWidget::GetSliderElement()
	{
	return mElement;
	}	    
    
    }//End of Alf
    
