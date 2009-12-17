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
* Description:  The base class for all widgets.
*
*/


#include <alf/alfenv.h>
#include "alf/alfwidget.h"
#include <alf/alfwidgetcontrol.h>
#include <alf/ialfmodel.h>
#include <alf/ialfviewwidget.h>
#include <alf/alfcontrolgroup.h>
#include <osn/ustring.h>
#include <alf/alfvarianttype.h>
#include <alf/alfexceptions.h>
#include <alf/alflayout.h>
#include "alf/alfattribute.h"
#include "alf/alfattributevaluetype.h"
#include "alf/ialfattributeowner.h"
#include <alf/ialfwidgetfactory.h>
#include <alf/alfevent.h>
#include <alf/alfwidgetevents.h>
#include <alf/alfwidgetenvextension.h>
#include <osn/osnnew.h>
#include <assert.h>

#include "alfwidgetimpl.h"
#include "alfwidgetattributeownerimpl.h"
#include "ialfhostapi.h"

namespace Alf
    {

// ======== MEMBER FUNCTIONS ========

OSN_EXPORT AlfWidget::AlfWidget()
    {
    }

OSN_EXPORT AlfWidget::AlfWidget(const char* aWidgetName)
	{
    mImpl.reset(new (EMM) AlfWidgetImpl());
    mImpl->setWidgetName(UString(aWidgetName));
    addCommonWidgetProperties();	
	}

OSN_EXPORT AlfWidget::AlfWidget(const char* aWidgetName, IAlfContainerWidget& aContainer, CAlfEnv& aEnv)    
    {    
    mImpl.reset(new (EMM) AlfWidgetImpl());
    mImpl->setWidgetName(UString(aWidgetName));
    addCommonWidgetProperties();    
    
    // Create default control for the widget so that widget containment hierarchy can be
    // created.
    if(!control())
        {
        auto_ptr<CAlfWidgetControl> control(new (EMM) CAlfWidgetControl(aEnv));
        setControl(control.get(), false);
        control.release();
        }
    
    // Add this widget to the container widget
    aContainer.addWidget(*this);
    }

void AlfWidget::addCommonWidgetProperties()
    {
    }

void AlfWidget::removeReferenceFromControl()
    {
    // Make sure that the widget's control is not pointing to this widget anymore
    if(control() && (control()->widget() == this))
    	{
    	control()->setOwnerWidget(0);
    	}	
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
OSN_EXPORT AlfWidget::~AlfWidget()
    {

    if( control() && control()->Host() )
        {
        CAlfWidgetControl* parentControl =
            dynamic_cast<CAlfWidgetControl*>( control()->Host() );
        
        if (control()->Focus())
            {
            control()->CAlfWidgetControl::RelinquishFocus();
            }
         
         // This widget is now deleted and it needs to be removed from the
        // container to avoid double deletion.
        if (parentControl)
            {
            AlfWidget* widget = parentControl->widget();
            if (widget)
                {
                widget->removeWidget( *this );
                }
            }
        }
            
    // Delete and remove all the connected child widgets.
    // AlfWidgetFactory::destroyWidget causes ~AlfWidget to be called for
    // the child widget so this works recursively. Note that it also
    // then removes the connection to the container widget in the beginning
    // of ~AlfWidget destructor call.
    
    IAlfWidgetFactory& factory = AlfWidgetEnvExtension::widgetFactory(*(CAlfEnv::Static()));
    
    while ( widgetCount() )
        {
        // Destroy the child widget and also remove it from the child widget
        // array to make sure that this loop terminates.
        // Normally also ~AlfWidget of the child widget removes the destroyed widget
        // from this widget's child array, but that might not be the case
        // if the child widget has not been added in the CAlfEnv
        // (all widgets should always be added there).
        // If not added in the env, destroyWidget does not delete the widget!
        
        IAlfWidget* child = mImpl->getWidget( 0 );

        removeWidget( *child );        
        IAlfWidget* actual = factory.findWidget( child->widgetName() );
        if( actual )
        	{
        	factory.destroyWidget(actual);
        	}              
        }
 
    // Ensure that the control is not pointing back to this widget
    removeReferenceFromControl();
    }


OSN_EXPORT CAlfWidgetControl* AlfWidget::control() const
    {
    return mImpl->control();
    }
    
// ---------------------------------------------------------------------------
// return the parent container
// ---------------------------------------------------------------------------
//
OSN_EXPORT IAlfContainerWidget* AlfWidget::parent() const
    {
    CAlfWidgetControl* ctrl = control();
    assert(ctrl); //should always be set.
    
    IAlfContainerWidget* containerWidget = 0;
    CAlfWidgetControl* parentControl =
        dynamic_cast<CAlfWidgetControl*>(ctrl->Host());
    IAlfWidgetFactory& factory = AlfWidgetEnvExtension::widgetFactory(*(CAlfEnv::Static()));
    if (parentControl)
        {
        IAlfWidget* widget = parentControl->widget();
        if (widget)
            {
            //control->widget may return wrong instance, temporary fix,
            // find widget from the CAlfEnv.
            const char* name = widget->widgetName();
            IAlfWidget* w = factory.findWidget(name);
            containerWidget = 
                IAlfWidget::makeInterface<IAlfContainerWidget>(w);
            assert(containerWidget);
            }
        }
        
    return containerWidget;
    }

// ---------------------------------------------------------------------------
// Sets the new control for this widget. The control will be automatically
// added as an observer for the model, and notified about any
// model changes (e.g., modelChangedL()).
// ---------------------------------------------------------------------------
//
OSN_EXPORT void AlfWidget::setControl(CAlfWidgetControl* aControl, bool aDeletePreviousControl)
    { 
    CAlfWidgetControl* myControl = control();
    if(aControl && (aControl != myControl))
        {
        // Throw exception if the set control is still a control of another widget
        if(aControl->widget())
            {
            ALF_THROW(AlfException, EInvalidArgument, "AlfWidget::setControl() - Control is already set to another widget.");
            }
        
        // Append control to widget's control group
        if(myControl)
            {
            assert(myControl->ControlGroup() != 0);
            aControl->resetControlGroup(*myControl->ControlGroup());
        
            // Move connections from old control to new control
            IAlfHostAPI& oldHostApi = myControl->hostAPI();
            IAlfHostAPI& newHostApi = aControl->hostAPI();
            
            // Move old controls children under new control
            int childCount = oldHostApi.getConnectionCount();
            while(childCount > 0)
                {
                newHostApi.setConnection(*oldHostApi.getConnection(0), EChild);
                childCount--;
                }
                
	        //relinquishfocus or else pointer is left in roster, even though memory for it
	        //might be released. Must be called before connection removed.
            if (myControl->Focus())
                {
                myControl->RelinquishFocus();
                }
                
            // Move new control under the old control's parent and remove old control from the parent.
            if(myControl->Host())
                {
                CAlfWidgetControl* parent = dynamic_cast<CAlfWidgetControl*>(myControl->Host());
                if(parent)
                    {
                    IAlfHostAPI& parentHostApi = parent->hostAPI();
                    parentHostApi.setConnection(*aControl, EChild);
                    parentHostApi.removeConnection(*myControl);
                    }
                }                
            }
                
        // Set the owner widget of the control to point to this widget
        aControl->setOwnerWidget(this);
        
        // Remove reference to this widget from the previous control
        removeReferenceFromControl();               
        }
    mImpl->setControl(aControl, aDeletePreviousControl);
    }

OSN_EXPORT IAlfModel* AlfWidget::model()
    {
    return  mImpl->model();
    }

// ---------------------------------------------------------------------------
// Sets the new model for this widget. If a control exists, it is automatically
// added as an observer for the model and notified for the model change.
// The old model is released.
// ---------------------------------------------------------------------------
//
OSN_EXPORT void AlfWidget::setModel( IAlfModel* aModel,bool aTakeOwnerShip)
    {
    mImpl->setModel( aModel ,aTakeOwnerShip);
    }


OSN_EXPORT const char* AlfWidget::widgetName() const
    {
    return mImpl->widgetName().getUtf8();
    }

// ---------------------------------------------------------------------------
// From class MAlfInterfaceBase.
// Static getter for interfaces provided by the widget, the control, or the model.
// ---------------------------------------------------------------------------
//
OSN_EXPORT IAlfInterfaceBase* AlfWidget::makeInterface( AlfWidget* aWidget, const IfId& aType )
    {
    if ( !aWidget )
        {
        return NULL;
        }
    else
        {
        return aWidget->makeInterface( aType );
        }
    }

// ---------------------------------------------------------------------------
// From class IAlfInterfaceBase.
// Getter for interfaces provided by the widget.
// ---------------------------------------------------------------------------
//
OSN_EXPORT IAlfInterfaceBase* AlfWidget::makeInterface( const IfId& aType )
    {
    UString param(aType.mImplementationId);
    if ( param == IAlfWidget::type().mImplementationId )
        {
        return static_cast<IAlfWidget*>(this);
        }
    else if (param == IAlfAttributeOwner::type().mImplementationId )
        {
        return static_cast<IAlfAttributeOwner*>(mImpl->getAttributeOwner());
        }
    else if (param == IAlfContainerWidget::type().mImplementationId )
        {
        return static_cast<IAlfContainerWidget*>(this);
        }
    IAlfInterfaceBase* result = NULL;
    if (control())
        {
        result = control()->makeInterface(aType);
        }
    if (!result && model())
        {
        result = model()->makeInterface(aType);
        }
    return result;
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
OSN_EXPORT int AlfWidget::widgetCount() const
    {
    return mImpl->widgetCount();
    }

// ---------------------------------------------------------------------------
// From class IAlfContainerWidget
// ---------------------------------------------------------------------------
OSN_EXPORT void AlfWidget::addWidget(IAlfWidget& aWidget)
    {
    // There should never be a situation where add widget is called on a
    // widget without a control.
    assert(aWidget.control() != 0);
    assert(control() != 0);
    assert(control()->ControlGroup() != 0);

    // Throw an exception if widget is added to itself.
    IAlfWidget* self = static_cast<IAlfWidget*>(this);
    if(self == &aWidget)
        {
        ALF_THROW(AlfException, EInvalidArgument, "Tried to add a widget to itself.");
        }

    // Throw an exception if added widget is a view widget.
    IAlfViewWidget* view = aWidget.makeInterface<IAlfViewWidget>(&aWidget);
    if(view != 0)
        {
        ALF_THROW(AlfException, EInvalidArgument, "Attempted to add a view widget into a container widget.");
        }        

    // Bail out if the given widget is already added in this widget.
    if(mImpl->findWidget(static_cast<AlfWidget*>(&aWidget)) != -1)
        {
        return;
        }

    // Append the added widget and its children to widget's control group.
    aWidget.control()->resetHierarchyControlGroup(*control()->ControlGroup());

    // Append widget to widget array and remove widget from previous container's widget array.
    mImpl->addWidget(static_cast<AlfWidget*>(&aWidget));
    CAlfWidgetControl* parentControl = dynamic_cast<CAlfWidgetControl*>(aWidget.control()->Host());
    if( parentControl )
        {
        parentControl->widget()->removeWidget(aWidget);
        }

    // Set connection to added widget's control
    CAlfWidgetControl* cntrl = control();
    if (cntrl)
        {
        IAlfHostAPI& hostApi = cntrl->hostAPI();
        try
            {
            hostApi.setConnection(*aWidget.control(), EChild);
            }
        catch (AlfException &e)
            {
            removeWidget(aWidget);
            // Attempt to add it back to the old.
            if(parentControl)
                {
                parentControl->widget()->mImpl->addWidget(static_cast<AlfWidget*>(&aWidget));
                hostApi.setConnection(*parentControl, EChild);
                }
            throw(e);
            }
        }
    }
	
OSN_EXPORT IAlfWidget* AlfWidget::getWidget(int aIndex) const
    {
    return mImpl->getWidget(aIndex);
    }
    
OSN_EXPORT int AlfWidget::getWidgetIndex(IAlfWidget& aWidget) const
    {
    return mImpl->findWidget(static_cast<AlfWidget*>(&aWidget));
    }

OSN_EXPORT void AlfWidget::removeWidget(int aIndex)
    {
    IAlfWidget* widget = getWidget(aIndex); //get the widget
    CAlfWidgetControl* cntrl = control();
    if (widget && cntrl)
        {
        CAlfWidgetControl* childcontrol = widget->control();
        if(childcontrol)
            {
            if (childcontrol->Focus())
                {
                try
                    {
                    childcontrol->RelinquishFocus();
                    }
                 catch(...)
                    {
                    // ignore any problems with getting rid of focus
                    }
                }
            else if(childcontrol->isContainer())
                {
                CAlfWidgetControl* focused = 
                        dynamic_cast<CAlfWidgetControl*>(childcontrol->FocusedConnection());                    
                if(focused)
                    {
                    try
                        {
                        control()->processEvent(TAlfEvent(EEventFocusNextWidget));
                        }
                    catch(...)
                        {
                        // cannot fail because of event processing to get rid of focus
                        }
                    }
                }
            IAlfHostAPI& hostApi = cntrl->hostAPI();
            hostApi.removeConnection(*childcontrol); //remove from hierarchy
            }
        }
    mImpl->removeWidget(aIndex); //remove it from array
    }

OSN_EXPORT void AlfWidget::applyLayout(IAlfLayoutManager& aLayout)
    {
    CAlfWidgetControl* cntrl = control();
    if (cntrl)
        {
        IAlfHostAPI& hostApi = cntrl->hostAPI();
      
        //remove connections first
        for (int i=0 ; i< widgetCount() ; i++)
            {
            IAlfWidget* widget = getWidget(i);
            hostApi.removeConnection(*widget->control());
            }

        hostApi.setBaseLayout(aLayout);    
        
        //add connections again, when new layout has been set.
        for (int i=0 ; i< widgetCount() ; i++)
            {
            IAlfWidget* widget = getWidget(i);
            hostApi.setConnection(*(widget->control()),EChild);
            }
        }
    }

OSN_EXPORT void AlfWidget::setChildFocus(bool /*aFocus*/)
    {

    }
    
OSN_EXPORT void AlfWidget::setPresentation(const char* /*aFilePath*/)
    {
    }
    
    
OSN_EXPORT void AlfWidget::removeWidget(IAlfWidget& aWidget)
    {
    for(int i = 0; i < mImpl->widgetCount(); ++i)
        {
        if(mImpl->getWidget(i) == &aWidget)
            {
            mImpl->removeWidget(i);
            break;
            }
        }
    }
    
    }//namespace Alf


