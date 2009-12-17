/*
* Copyright (c) 2007,2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Provides system state information
*
*/


#ifndef IALFAPPEVENTLISTENER_H
#define IALFAPPEVENTLISTENER_H

class TAlfEvent;

namespace osncore
    {
    class UString;
    }

using namespace osncore;

namespace Alf
{
/**
 *  @class IAlfAppEvent
 *
 *  @discussion Provides function to handle application events.  
 *
 */
class IAlfAppEventListener
    {
public: 
   /**
    * @function handleApplicationEvent
    *
    * Function will be called when application event triggers.
    * Application has to register itself to listen events by
    * using duiengine's subscribeAppEvent function
    *
    * example:
    * <appevents>
    *   <eventhandler id="pressed" owner="gridwidget1">
    *     <eventinput id="pressedinput" name="customevent">
    *       <property name="eventid" value="1333" />
    *       <!-- value 1333 is selection changed in grid widget -->
    *     </eventinput>
    *     <eventoutput name="appevent"> 
    *       <property value="itemclicked"/>
    *     </eventoutput>
    *   </eventhandler>
    * </appevents>
    *
    * Register your application to listen application events via IDuiEngineConstruction:
    * duiEngineConstructionInterface->subscribeAppEvents(*this);
    *
    * aCmd is the value you have set on property's value attribute, in this case
    * "itemclicked"
    *
    * @param aCmd command which is specified to eventoutput
    * @param aEvent contains event specific data
    * @return void
    */ 
    virtual void handleApplicationEvent(const UString& aCmd, const TAlfEvent& aEvent) = 0;
    };
}
        

#endif 
            
// End of File