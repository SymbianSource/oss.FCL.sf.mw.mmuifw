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
* Description:  Widget interface with utilities for container widget.
*
*/


#include "alfcontainerwidget.h"
#include "alfcontainercontrol.h"
#include "alfcontainerwidgetexception.h"
#include <libc/stdio.h>
#include <libc/string.h>
#include "alf/alfwidget.h"
#include <alf/alfenv.h>
#include <alf/alfmetric.h>
#include <alf/alfgridlayoutmanager.h>
#include <alf/ialfgridlayoutpolicy.h>

#include "alf/attrproperty.h"

namespace Alf
{

AlfContainerWidget::AlfContainerWidget(
	const char* aWidgetName, 
	IAlfContainerWidget& aContainer, 
	CAlfEnv& aEnv, 
	DuiNode* /*aNode*/,
	const char* /*aFilePath*/)
	: mWidget(0), 
	mEnv(aEnv)
    {
    mWidget.reset(new (EMM) AlfWidget(aWidgetName, aContainer, aEnv));
            
       
    
        constructDefault();
        setDefaultLayoutManager();
        
    }

AlfContainerWidget::~AlfContainerWidget()
    {
    }

CAlfWidgetControl* AlfContainerWidget::control() const
    {
    return mWidget->control();
    }
    
IAlfContainerWidget* AlfContainerWidget::parent() const
    {
    return mWidget->parent();
    }

void AlfContainerWidget::setControl(CAlfWidgetControl* aControl, bool aDeletePreviousControl)
    {
    mWidget->setControl(aControl, aDeletePreviousControl);
    }

IAlfModel* AlfContainerWidget::model()
    {
    return 0;
    }

void AlfContainerWidget::setModel(IAlfModel* /*aModel*/, bool /*aTakeOwnership*/)
    {
    
    }

const char* AlfContainerWidget::widgetName() const
    {
    return mWidget->widgetName();
    }

void AlfContainerWidget::setChildFocus(bool /*aFocus*/)
    {
    
    }


//---------------------------------------------------------------------------
// Creates the presentation for the widget from XML. Destroys any existing
// presentation.
//---------------------------------------------------------------------------
//    
void AlfContainerWidget::setPresentation (const char* aFilePath)
    {
    if(mWidget.get())
    	{
        mWidget->setPresentation(aFilePath);    	
    	}
    }    

int AlfContainerWidget::widgetCount() const
    {
    return mWidget->widgetCount();
    }

void AlfContainerWidget::addWidget(IAlfWidget& aWidget)
    {
    mWidget->addWidget(aWidget);
    }

IAlfWidget* AlfContainerWidget::getWidget(int aIndex) const
    {
    return mWidget->getWidget(aIndex);
    }
    
int AlfContainerWidget::getWidgetIndex(IAlfWidget& aWidget) const
    {
    return mWidget->getWidgetIndex(aWidget);
    }

void AlfContainerWidget::removeWidget(int aIndex)
    {
    mWidget->removeWidget(aIndex);
    }

void AlfContainerWidget::applyLayout(IAlfLayoutManager& aLayout)
    {
    mWidget->applyLayout(aLayout);
    }

IAlfInterfaceBase* AlfContainerWidget::makeInterface(const IfId& aType)
    {    
    // Type cast to IAlfWidget
    if(!strcmp(aType.mImplementationId, IAlfWidget::type().mImplementationId))
        {
        return static_cast<IAlfWidget*>(this);
        }
        
    // Type cast to IAlfContainerWidget        
    if(!strcmp(aType.mImplementationId, IAlfContainerWidget::type().mImplementationId))
        {
        return static_cast<IAlfContainerWidget*>(this); 
        } 
    
    return mWidget->makeInterface ( aType );
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//    
void AlfContainerWidget::constructDefault()
    {
    // Create control for the container widget
    auto_ptr<AlfContainerControl> control(new (EMM) AlfContainerControl(mEnv));    
    setControl(control.get());
    control.release();
   
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//   
void AlfContainerWidget::setDefaultLayoutManager()
	{
	 // Default layout manager provided by the container widget
    auto_ptr<AlfGridLayoutManager> layoutManager(new (EMM) AlfGridLayoutManager());
    this->applyLayout(*layoutManager.get());
    AlfGridLayoutManager* lm = layoutManager.release();
    
    IAlfGridLayoutPolicy* gridPolicy = IAlfInterfaceBase::makeInterface<IAlfGridLayoutPolicy>(lm);
    gridPolicy->fillWeights(IAlfGridLayoutPolicy::EGridDimensionColumn, 1, TAlfMetric(1, EAlfUnitWeight));
    gridPolicy->fillWeights(IAlfGridLayoutPolicy::EGridDimensionRow, 1, TAlfMetric(1, EAlfUnitWeight));
	}

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//   
void AlfContainerWidget::constructComponentsFromNode()
	{
	constructDefault();
    
    //check, if the layoutmanager is already set.
    IAlfLayoutManager* layoutManager = IAlfInterfaceBase::makeInterface<IAlfLayoutManager>(control());
    if (!layoutManager)
        {
        setDefaultLayoutManager();
        }
	}
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//  
void AlfContainerWidget::processTreeRecursively()
    {
    }

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//    
void AlfContainerWidget::constructFromPresentationXML(const char* /*aFilePath*/ )
    {
    constructDefault();
    setDefaultLayoutManager();      
    }
}
