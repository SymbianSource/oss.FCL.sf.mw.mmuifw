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
* Description:  The private implementation of AlfHostAPI.
*
*/


#include "alfhostapiimpl.h"
#include <alf/alfwidgetcontrol.h>
#include <alf/ialflayoutmanager.h>
#include <alf/alfexceptions.h>

namespace Alf
    {
    
AlfHostAPIImpl::AlfHostAPIImpl(): mBaseLayout(NULL), mHostControl(NULL)
    {
    }
    
    
AlfHostAPIImpl::~AlfHostAPIImpl()
    {
    }

OSN_EXPORT IAlfInterfaceBase* AlfHostAPIImpl::makeInterface(
    const IfId& aType )
    {
    UString param(aType.mImplementationId);
    if ( param == IAlfHostAPI::type().mImplementationId )
        {
        return static_cast<IAlfHostAPI*>( this );
        }
    return 0;
    }

void AlfHostAPIImpl::setHostControl(CAlfWidgetControl& aControl)
    {
    mHostControl = &aControl;
    }

void AlfHostAPIImpl::setConnection(CAlfWidgetControl& aControl, AlfRole aRole)
    {
    if (mHostControl)
        {
        // Remove previous host connection from connected control
        if(aControl.Host())
            {
            CAlfWidgetControl* parentControl = dynamic_cast<CAlfWidgetControl*>(aControl.Host());
            if(parentControl)
                {
                parentControl->hostAPI().removeConnection(aControl);
                }
            }
        
        // Add connection from host control to connected control
        TRAPD(err, mHostControl->AddConnectionL(&aControl,aRole));
        if(err != KErrNone)
            {
            ALF_THROW(AlfException, err, "AlfHostAPIImpl::setConnection() - AddConnectionL() failed.");
            }        
        
        // Update layout of host control if a child control was connected
        if (aRole == EChild && mBaseLayout.get())
            {
            mBaseLayout->updateChildLayout(&aControl);
            }
        }
    }

CAlfWidgetControl* AlfHostAPIImpl::getConnection(int aIndex) const
    {
    if (mHostControl)
    	{
    	//AlfControl doesn't perform bound checks.
    	if (aIndex >=0 && aIndex < mHostControl->ConnectionCount()) 
    		{
    		return (static_cast<CAlfWidgetControl*>(&(mHostControl->Connection(aIndex)))); 
    		}
    	}
    return 0; //NULL is not defined in osncore
    }
    
int AlfHostAPIImpl::getConnectionCount() const
    {
    int connectionCount = 0;
    if (mHostControl)
        {
        connectionCount = mHostControl->ConnectionCount();
        }
    return connectionCount;
    }
    
int AlfHostAPIImpl::getConnectionIndex(CAlfWidgetControl& aControl) const
    {
    return mHostControl->FindConnection(&aControl);
    }

void AlfHostAPIImpl::removeConnection(CAlfWidgetControl& aControl)
    {
    if (mHostControl)
        {
        mHostControl->RemoveConnection(&aControl);  
        if (getBaseLayout())
            {
            getBaseLayout()->childRemoved(&aControl);
            }
        }
    }

void AlfHostAPIImpl::setBaseLayout(IAlfLayoutManager& aLayout)
    {
    if(mHostControl)
    	{
        mBaseLayout.reset(&aLayout);
        mBaseLayout->createLayout(*mHostControl, 0, 0);
        }
    }

IAlfLayoutManager* AlfHostAPIImpl::getBaseLayout() const
    {
    return mBaseLayout.get();
    }

    } //namespace alf



