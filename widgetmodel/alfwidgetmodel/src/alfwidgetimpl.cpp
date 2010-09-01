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
* Description:  The private implementation of CAlfWidget.
*
*/


#include "alfwidgetimpl.h"
#include <alf/alfwidgetcontrol.h>
#include <alf/alfmodel.h>
#include <alf/alfcontrolgroup.h>
#include "alf/alfwidget.h"
#include "alf/alfattribute.h"
#include "alf/alfattributevaluetype.h"
#include <alf/alfexceptions.h>
#include <libc/string.h>
#include <alf/ialfelement.h>

#include "alfwidgetattributeownerimpl.h"


namespace Alf
    {

AlfWidgetImpl::AlfWidgetImpl()
    {
    mControl = NULL;
    mModel = NULL;
    mAttributeList.setAutoDelete(true);
    mWidgets.setAutoDelete(false);
    mTakesModelOwnership=false;
    }

AlfWidgetImpl::~AlfWidgetImpl()
    {
    setControl( NULL, true );
    
    if (mTakesModelOwnership)
        {
        delete mModel;
        mModel = NULL;
        }        
    mAttributeList.clear();
    mWidgets.clear();
    }

void AlfWidgetImpl::setWidgetName( const UString& aWidgetName )
    {
    mWidgetName = aWidgetName;
    }

const UString& AlfWidgetImpl::widgetName() const
    {
    return mWidgetName;
    }


CAlfWidgetControl* AlfWidgetImpl::control()
    {
    return mControl;
    }	

void AlfWidgetImpl::setControl( CAlfWidgetControl* aControl,bool aDeletePreviousControl)
    {    
    if ( mControl != aControl )
        {
        if( mControl )
            {
            if( mModel )
                {
                // Remove model change observer from control
                mModel->removeModelChangeObserver( *mControl );
                }
            if( aDeletePreviousControl )
                {
                // Delete control from possible control group to avoid double deletion
                if(mControl->ControlGroup())
                    {
                    mControl->ControlGroup()->Remove(mControl);
                    }
                // Delete the control
                delete mControl;
                mControl = NULL;
                }
            }

        // Set the new control.
        mControl = aControl;

        // Add the new control as an observer for model changes.
        if( mControl )
            {
            if( mModel )
                {
                mModel->addModelChangeObserver( *mControl );
                }
            mAttributeOwnerImpl.reset(new (EMM) AlfWidgetAttributeOwnerImpl(mControl));
            }
        }
    }

IAlfModel* AlfWidgetImpl::model()
    {
    return mModel;
    }

void AlfWidgetImpl::setModel( IAlfModel* aModel,bool aTakeOwnerShip )
    {
    if ( mModel != aModel )
        {
        // Release the old model.
        if (mTakesModelOwnership)
            {
            delete mModel;
            }

        else if (mModel&&mControl)
            {
            mModel->removeModelChangeObserver(*mControl);
            }

        // Set the new model.
        mModel = aModel;
        mTakesModelOwnership=aTakeOwnerShip;
        // Add the control as an observer for model changes.
        if ( mModel && mControl )
            {
            mModel->addModelChangeObserver( *mControl );
            }
        }
    else // just copy the ownership flag 
        { 
        if (mModel != NULL)
            {
            mTakesModelOwnership=aTakeOwnerShip;
            }
        }

    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void AlfWidgetImpl::addWidget(AlfWidget *aWidget)
    {
    for (int i=0 ; i< mWidgets.count(); i++ )
        {
        if (mWidgets.findRef(aWidget) >=0)
            {
            //widget already there.
            return;
            }
        }
    mWidgets.resize(mWidgets.count()+1);
    mWidgets.insert(mWidgets.count(),aWidget);
    }

int  AlfWidgetImpl::widgetCount()
    {
    return mWidgets.count();
    }

AlfWidget* AlfWidgetImpl::getWidget(int aIndex)
    {
    if (aIndex>=0 && aIndex < mWidgets.count())
        return mWidgets[aIndex];
    return 0;
    }

int AlfWidgetImpl::findWidget(AlfWidget* aWidget)
    {
    return mWidgets.findRef(aWidget);
    }

void AlfWidgetImpl::removeWidget(int aIndex)
    {
    if (aIndex >=0 and aIndex < mWidgets.count())
        mWidgets.remove(aIndex);
    }
    
// ---------------------------------------------------------------------------
//  Returns attribute owner interface.
// ---------------------------------------------------------------------------
//
AlfWidgetAttributeOwnerImpl* AlfWidgetImpl::getAttributeOwner()
    {
    return mAttributeOwnerImpl.get();
    }

    } //namespace alf



