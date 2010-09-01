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
* Description:  The base class for all widget controls.
*
*/


#include <alf/alfevent.h>

#include "alf/alfwidget.h"
#include <alf/alfwidgetcontrol.h>
#include <alf/alfmodel.h>
#include <alf/alfwidgeteventhandler.h>
#include "alf/alfelement.h"
#include <alf/alfvarianttype.h>
#include "alf/alfmodeloperation.h"
#include <osn/ustring.h>
#include <osn/alfptrvector.h>
#include <alf/alfexceptions.h>
#include <alf/alfenv.h>
#include <alf/alfcontrolgroup.h>
#include <alf/ialflayoutmanager.h>
#include <alf/alfwidgetevents.h>
#include "alf/ialfattributeowner.h"
#include <osn/osnnew.h>
#include <assert.h>

#include "alfhostapiimpl.h"
#include "alfwidgetcontroleventfilter.h"

using namespace osncore;

namespace Alf
    {
// ======== INTERNAL DATA TYPES ========

// Forward declared inside the Alf namespace


//Internal Class to Store the Element Data
class ElementData
    {
public:
    //Default constructor
    ElementData():mElement(0)
        {
        }
    //Destructor
    ~ElementData()
        {
        delete mElement;
        }
    /**
     * The element. Own.
     */
    IAlfElement* mElement;

    /**
     * The data id range for the element.
     */
    uint mDataIdRange;
    };

class AlfWidgetControlImpl
    {

public:
    AlfWidgetControlImpl():mWidget(0),mEventFilter(0),mAlfHostAPIImpl(0)
        {

        }
    ~AlfWidgetControlImpl()
        {

        }
public:
    /**
     * The element data. Elements are owned by the control.
     */
    AlfPtrVector<ElementData> mElementArray;

    /**
     * The event handlers. Event handlers are owned by the control.
     */
    AlfPtrVector<IAlfWidgetEventHandler> mEventHandlerArray;

    /**
     * The owner widget.
     */
    AlfWidget* mWidget;
    
    /**
     * PointerUp Event Filter.
     */
    AlfWidgetControlEventFilter *mEventFilter;
    
    /**
     * The state of the control.
     * The state is a combination of binary state flags.
     */
    uint mState;
    /**
     * AlfAPIImpl auto pointer.
     * This will be used to provide container control's functionality by 
     * widget control.Owned.
     */
    auto_ptr<AlfHostAPIImpl> mAlfHostAPIImpl;
    };

// ======== MEMBER FUNCTIONS ========

OSN_EXPORT CAlfWidgetControl::CAlfWidgetControl(CAlfEnv& aEnv)
    {
    construct(aEnv);
    }

OSN_EXPORT CAlfWidgetControl::CAlfWidgetControl()
    {
    }

OSN_EXPORT void* CAlfWidgetControl::operator new(
                 size_t aSize, newarg /*aEnumVal*/)  throw (std::bad_alloc)
    { 
    void* ret = 0;
    TRAPD(err, ret = CBase::operator new((TUint)aSize, ELeave));
    if(err != KErrNone)
        {
        throw std::bad_alloc();
        }
    return ret;         
    } 

OSN_EXPORT void CAlfWidgetControl::construct(
    CAlfEnv& aEnv)
    {
    mWdgtControlData.reset( new (EMM) AlfWidgetControlImpl() );
    mWdgtControlData->mWidget = 0;
    //by default control is focusable and enabled
    mWdgtControlData->mState = IAlfWidgetControl::Focusable |
        IAlfWidgetControl::Enabled;

    // Call CAlfControl second phase constructor
    TRAPD(err, CAlfControl::ConstructL( aEnv ));
    if(err != KErrNone)
        {
        ALF_THROW(AlfException, err, "CAlfWidgetControl::construction failed.");
        }
    mWdgtControlData->mElementArray.setAutoDelete(true);
    mWdgtControlData->mEventHandlerArray.setAutoDelete(true);
    mWdgtControlData->mAlfHostAPIImpl.reset(0);

    // Instatiate Event filter
    mWdgtControlData->mEventFilter = 
        new (EMM) AlfWidgetControlEventFilter();
    addEventHandler(mWdgtControlData->mEventFilter);
    }

OSN_EXPORT CAlfWidgetControl::~CAlfWidgetControl()
    {
    //release all connections
    while (this->ConnectionCount())
        {
        this->RemoveConnection(&(this->Connection(0)));
        }
    mWdgtControlData->mElementArray.setAutoDelete(true);
    
    for(int i = 0; i < mWdgtControlData->mElementArray.count(); ++i)
        {
        // Fetch the element
        IAlfElement *element = mWdgtControlData->mElementArray[i]->mElement;

        // If the element is also an event handler
        IAlfWidgetEventHandler* eventHandler =
        IAlfInterfaceBase::makeInterface<IAlfWidgetEventHandler>( element );
        int eventHandlerIndex = 
            mWdgtControlData->mEventHandlerArray.findRef( eventHandler );
        if ( eventHandler && eventHandlerIndex != KErrNotFound )
            {
             //cache auto delete state.
            bool autoDeleteState = 
                mWdgtControlData->mEventHandlerArray.autoDelete();
            mWdgtControlData->mEventHandlerArray.setAutoDelete(false);
            mWdgtControlData->mEventHandlerArray.remove( eventHandlerIndex );
             //restore auto delete state.
            mWdgtControlData->mEventHandlerArray.setAutoDelete(
                                                     autoDeleteState);
            }
        }
    
    mWdgtControlData->mElementArray.clear();
        
    mWdgtControlData->mEventHandlerArray.setAutoDelete(true);
    mWdgtControlData->mEventHandlerArray.clear(); 
    	 
    if(mWdgtControlData->mWidget)
        {
        // false: don't delete twice
        mWdgtControlData->mWidget->setControl(0, false);
        mWdgtControlData->mWidget = 0;
        }
    }

OSN_EXPORT uint CAlfWidgetControl::state() const
    {
    // Verify that the internal state stored in the member variable
    // is in sync with the CAlfWidget state. This might not be the case
    // if someone has called CAlfWidget APIs, e.g. AcquireFocus(), directly.
    
    if ( Focus() )
        {
        mWdgtControlData->mState |= IAlfWidgetControl::Focused;
        }
    else
        {
        mWdgtControlData->mState &= ~IAlfWidgetControl::Focused;
        }

    return mWdgtControlData->mState;
    }

OSN_EXPORT void CAlfWidgetControl::setState( uint aState )
    {
     // Checks the state invariants. Throws exceptions if not OK
    checkStateInvariants(aState);
    
    // Visible
    if ( aState & IAlfWidgetControl::Visible )
        {
        enableStateVisible();
        }
    else
        {
        disableStateVisible();
        }

    // Enabled
    if ( aState & IAlfWidgetControl::Enabled )
        {
        enableStateEnabled();
        }
    else
        {
        disableStateEnabled();
        }

    // Focused
    if ( aState & IAlfWidgetControl::Focused )
        {
        enableStateFocused();
        }
    else
        {
        disableStateFocused();
        }        

    // Focusable
    if ( aState & IAlfWidgetControl::Focusable )
        {
        enableStateFocusable();
        }
    else
        {
        disableStateFocusable();
        }        
    }

OSN_EXPORT void CAlfWidgetControl::enableState( uint aState )
    {
    // Predicts the state and checks the state invariants.
    // Throws exceptions if not OK
    predictAndCheckStateInvariants(aState, true);

    // Visible
    if ( aState & IAlfWidgetControl::Visible )
        {
        enableStateVisible();
        }

    // Enabled
    if ( aState & IAlfWidgetControl::Enabled )
        {
        enableStateEnabled();
        }

    // Set Focusable. This is done first, before putting on fucus
    if ( aState & IAlfWidgetControl::Focusable )
        {
        enableStateFocusable();
        }
        
    // Focused
    if ( aState & IAlfWidgetControl::Focused )
        {
        enableStateFocused();
        }

    }

OSN_EXPORT void CAlfWidgetControl::disableState( uint aState )
    {
    // Predicts the state and checks the state invariants.
    // Throws exceptions if not OK
    predictAndCheckStateInvariants(aState, false);

    // Visible
    if ( aState & IAlfWidgetControl::Visible )
        {
        disableStateVisible();
        }

    // Enabled
    if ( aState & IAlfWidgetControl::Enabled )
        {
        disableStateEnabled();
        }

    // Focused
    if ( aState & IAlfWidgetControl::Focused )
        {
        disableStateFocused();
        }

    // Focusable
    if ( aState & IAlfWidgetControl::Focusable )
        {
        disableStateFocusable();
        }
    }

OSN_EXPORT bool CAlfWidgetControl::checkState( uint aState ) const
    {
    return ( state() & aState );
    }

OSN_EXPORT int CAlfWidgetControl::numEventHandlers() const
    {
    return mWdgtControlData->mEventHandlerArray.count();
    }

OSN_EXPORT IAlfWidgetEventHandler& CAlfWidgetControl::eventHandler(
    int aIndex )
    {
    return *mWdgtControlData->mEventHandlerArray[aIndex];
    }

OSN_EXPORT int CAlfWidgetControl::eventHandlerIndex(
    IAlfWidgetEventHandler& aEventHandler ) const
    {
    for ( int i = 0; i < mWdgtControlData->mEventHandlerArray.count(); ++i )
        {
        if ( mWdgtControlData->mEventHandlerArray[i] == &aEventHandler )
            {
            return i;
            }
        }
    return -1;
    }

OSN_EXPORT IAlfWidgetEventHandler* CAlfWidgetControl::findEventHandler(
    const TAlfEvent& aEvent )
    {
    for ( int i = 0; i < mWdgtControlData->mEventHandlerArray.count(); ++i )
        {
        if ( mWdgtControlData->mEventHandlerArray[i]->accept( *this, aEvent ) )
            {
            return mWdgtControlData->mEventHandlerArray[i];
            }
        }
    return 0;
    }

OSN_EXPORT void CAlfWidgetControl::addEventHandler(
    IAlfWidgetEventHandler* aEventHandler, int aIndex )
    {
    try
        {
        if ( mWdgtControlData->mEventHandlerArray.findRef( aEventHandler ) == 
             KErrNotFound )
            {
            if ( aIndex == -1 )
                {
                mWdgtControlData->mEventHandlerArray.resize(
                    mWdgtControlData->mEventHandlerArray.count()+1);
                mWdgtControlData->mEventHandlerArray.insert(
                    mWdgtControlData->mEventHandlerArray.count(),
                    aEventHandler );
                }
            else
                {
                mWdgtControlData->mEventHandlerArray.resize(
                    mWdgtControlData->mEventHandlerArray.count()+1);
                mWdgtControlData->mEventHandlerArray.insert( aIndex,
                                                         aEventHandler);
                }
            }
        }
    catch (...)
        {
        ALF_THROW(AlfWidgetException,ECommonError,"CAlfWidgetControl: Adding event handler failed.")
        }
    }

OSN_EXPORT void CAlfWidgetControl::removeAndDestroyEventHandler(
    IAlfWidgetEventHandler& aEventHandler )
    {

    for (int i =0; i<mWdgtControlData->mEventHandlerArray.count();i++)
        {
        if (mWdgtControlData->mEventHandlerArray[i] == &aEventHandler)
            {
            IAlfWidgetEventHandler *handler = 
                mWdgtControlData->mEventHandlerArray[i];
            //check if it is an eventhandler associated with presentation 
            if(handler->eventHandlerType() == 
               IAlfWidgetEventHandler::EPresentationEventHandler)
                {
                ElementData* elementData = 0;
                // if the event handller is also an element, remove the 
                // corresponding element data from mEventHandlerArray
                elementData = removePesentationElementData(*handler);
                // if the element data does not exist, remove the event handler
                // from mEventHandlerArray and destroy it 
                if(!elementData)
                    {
                    mWdgtControlData->mEventHandlerArray.remove( i );
                    }
                // remove the event hanlder from the array but dont destroy it,
                // and then delete element data which in turn will destroy the
                // element and thus the event handler
                else
                    {
                    // cache auto delete state.
                    bool autoDeleteState = 
                        mWdgtControlData->mEventHandlerArray.autoDelete(); 
                    mWdgtControlData->mEventHandlerArray.setAutoDelete(false);
                    mWdgtControlData->mEventHandlerArray.remove( i );
                    // restore auto delete state.
                    mWdgtControlData->mEventHandlerArray.setAutoDelete(
                                                             autoDeleteState);
                    delete elementData;
                    }
                }
            else
                {
                mWdgtControlData->mEventHandlerArray.remove( i );
                }
            return;
            }

        }

    }

OSN_EXPORT void CAlfWidgetControl::removeEventHandler(
    IAlfWidgetEventHandler& aEventHandler )
    {
    for (int i =0; i<mWdgtControlData->mEventHandlerArray.count();i++)
        {
        if (mWdgtControlData->mEventHandlerArray[i] == &aEventHandler)
            {
            // cache auto delete state.
            bool autoDeleteState = 
                mWdgtControlData->mEventHandlerArray.autoDelete();
            mWdgtControlData->mEventHandlerArray.setAutoDelete(false);
            mWdgtControlData->mEventHandlerArray.remove( i );
            // restore auto delete state.
            mWdgtControlData->mEventHandlerArray.setAutoDelete(
                                                     autoDeleteState);
            return;
            }
        }
    }

OSN_EXPORT void CAlfWidgetControl::removeAndDestroyEventHandler(
    const UString& aHandlerId )
    {
    for (int i =0; i<mWdgtControlData->mEventHandlerArray.count();i++)
        {
        IAlfWidgetEventHandler* handler = 
            mWdgtControlData->mEventHandlerArray[i];
        AlfWidgetEventHandlerInitData* eventData = handler->eventHandlerData();
        if(eventData != 0)
            {
            //check for the event id/name
            if (!aHandlerId.compare(eventData->mWidgetEventHandlerId))
                {
                //check if it is an eventhandler associated with presentation 
                if(handler->eventHandlerType() == 
                   IAlfWidgetEventHandler::EPresentationEventHandler)
                    {
                    ElementData* elementData = 0;
                    // if the event handller is also an element, remove the 
                    // corresponding element data from mEventHandlerArray
                    elementData = removePesentationElementData(*handler);
                    // if the element data does not exist, remove the event 
                    // handler from mEventHandlerArray and destroy it 
                    if(!elementData)
                        {
                        // removes and destoys the event handler
                        mWdgtControlData->mEventHandlerArray.remove( i );
                        }
                    // remove the event hanlder from the array but dont destroy
                    // it, and then delete element data which in turn will 
                    // destroy the element and thus the event handler      
                    else
                        {
                        // cache auto delete state.
                        bool autoDeleteState = 
                            mWdgtControlData->mEventHandlerArray.autoDelete();
                        mWdgtControlData->mEventHandlerArray.setAutoDelete(
                                                                 false);
                        mWdgtControlData->mEventHandlerArray.remove( i );
                        // restore auto delete state.
                        mWdgtControlData->mEventHandlerArray.setAutoDelete(
                                                             autoDeleteState);
                        //delete element data
                        delete elementData;
                        }
                    }
                else
                    {
                    mWdgtControlData->mEventHandlerArray.remove( i );
                    }
                return;
                }
            }
        
        }
    }
    
OSN_EXPORT void CAlfWidgetControl::removeAndDestroyPresentationEventHandlers()
    {
    int i=0;
    while(i<mWdgtControlData->mEventHandlerArray.count())
        {
        IAlfWidgetEventHandler* handler = 
                                    mWdgtControlData->mEventHandlerArray[i];
        
        //check if it is an eventhandler associated with presentation 
        if(handler->eventHandlerType() == 
               IAlfWidgetEventHandler::EPresentationEventHandler)
            {
            ElementData* elementData = 0;
            // if the event handller is also an element, remove the 
            // corresponding element data from mEventHandlerArray
            elementData = removePesentationElementData(*handler);
            // if the element data does not exist, remove the event handler
            // from mEventHandlerArray and destroy it 
            if(!elementData)
                {
                mWdgtControlData->mEventHandlerArray.remove( i );
                }
            // remove the event hanlder from the array but dont destroy it,
            // and then delete element data which in turn will destroy the
            // element and thus the event handler    
            else
                {
                // cache auto delete state.
                bool autoDeleteState = 
                    mWdgtControlData->mEventHandlerArray.autoDelete();
                mWdgtControlData->mEventHandlerArray.setAutoDelete(false);
                mWdgtControlData->mEventHandlerArray.remove( i );
                // restore auto delete state.
                mWdgtControlData->mEventHandlerArray.setAutoDelete(
                                                         autoDeleteState);
                //delete element data
                delete elementData;
                }
            }
        else
            {
            i++;
            }
        }
    
    }
    
OSN_EXPORT void CAlfWidgetControl::removePresentationEventHandlers()
    {
    int i=0;
    // cache auto delete state.
    bool autoDeleteState = mWdgtControlData->mEventHandlerArray.autoDelete();
    mWdgtControlData->mEventHandlerArray.setAutoDelete(false);
    while(i<mWdgtControlData->mEventHandlerArray.count())
        {
        IAlfWidgetEventHandler* handler = 
                                    mWdgtControlData->mEventHandlerArray[i];
        //check if it is an eventhandler associated with presentation 
        if(handler->eventHandlerType() == 
           IAlfWidgetEventHandler::EPresentationEventHandler)
            {
            mWdgtControlData->mEventHandlerArray.remove( i );
            }
        else
            {
            i++;
            }
        }
    // restore auto delete state.    
    mWdgtControlData->mEventHandlerArray.setAutoDelete(autoDeleteState);
    }

ElementData* CAlfWidgetControl::removePesentationElementData( 
    IAlfWidgetEventHandler& aEventHandler )
    {
    IAlfElement* element =
        IAlfInterfaceBase::makeInterface<IAlfElement>( &aEventHandler );
        
    for ( int i = 0; i < mWdgtControlData->mElementArray.count(); ++i )
        {
        if ( mWdgtControlData->mElementArray[i]->mElement == element )
            {
            // cache auto delete state.
            bool autoDeleteState = 
                mWdgtControlData->mElementArray.autoDelete();
            mWdgtControlData->mElementArray.setAutoDelete(false);
            // Store the Element Data which is to be removed.
            // This pointer will be returned to the caller
            ElementData* elementData = mWdgtControlData->mElementArray[i];
            mWdgtControlData->mElementArray.remove( i );
            // restore auto delete state.
            mWdgtControlData->mElementArray.setAutoDelete(autoDeleteState);
            return elementData;;
            }
        }
    return 0;
    }

OSN_EXPORT int CAlfWidgetControl::numElements() const
    {
    return mWdgtControlData->mElementArray.count();
    }

OSN_EXPORT IAlfElement& CAlfWidgetControl::element( int aIndex )
    {
    return *(mWdgtControlData->mElementArray[aIndex]->mElement);
    }

OSN_EXPORT IAlfElement* CAlfWidgetControl::findElement( const char* aName )
    {
    UString name(aName);
    for ( int i = 0; i < mWdgtControlData->mElementArray.count(); ++i )
        {
        IAlfElement* element = mWdgtControlData->mElementArray[i]->mElement;
        if (name == UString(element->name()))
            {
            return element;
            }
        }
    return 0;
    }

OSN_EXPORT void CAlfWidgetControl::addElement( IAlfElement* aElement )
    {
    for ( int i = 0; i < mWdgtControlData->mElementArray.count(); ++i )
        {
        if ( mWdgtControlData->mElementArray[i]->mElement == aElement )
            {
            return; // Already exists
            }
        }
    auto_ptr<ElementData> elemData( new (EMM) ElementData() );
    elemData->mElement = aElement;
    elemData->mDataIdRange = 0xFFFFFFFF;
    try
        {
        mWdgtControlData->mElementArray.resize(
                              mWdgtControlData->mElementArray.count()+1);
        mWdgtControlData->mElementArray.insert(
                              mWdgtControlData->mElementArray.count(),
                              elemData.get() );
        elemData.release(); // ownership transferred
        }
    catch (...)
        {
        // change the element of element data to 0.
        // this will ensure that the element does not get deleted when the 
        // elemData gets out of scope, since elemData was not successfully 
        // added to the array
        elemData->mElement = 0;
        ALF_THROW(AlfException,ECommonError,"AlfWidgetControl: Adding the element failed.")
        }    
    }

OSN_EXPORT void CAlfWidgetControl::removeAndDestroyElement(
    const IAlfElement& aElement )
    {
    for ( int i = 0; i < mWdgtControlData->mElementArray.count(); ++i ) 
        {
        if (mWdgtControlData->mElementArray[i])
            {
            if ( mWdgtControlData->mElementArray[i]->mElement == &aElement )
                {
                // Remove the element
                IAlfElement *element = 
                                mWdgtControlData->mElementArray[i]->mElement;

                // If the element is also an event handler
                IAlfWidgetEventHandler* eventHandler =
                    IAlfInterfaceBase::makeInterface<IAlfWidgetEventHandler>(
                                           element );
                int eventHandlerIndex = 
                    mWdgtControlData->mEventHandlerArray.findRef(
                                          eventHandler );
                if ( eventHandler && eventHandlerIndex != KErrNotFound )
                    {
                    // cache auto delete state.
                    bool autoDeleteState = 
                        mWdgtControlData->mEventHandlerArray.autoDelete();
                    mWdgtControlData->mEventHandlerArray.setAutoDelete(false);
                    mWdgtControlData->mEventHandlerArray.remove(
                                                         eventHandlerIndex );
                    // restore auto delete state.
                    mWdgtControlData->mEventHandlerArray.setAutoDelete(
                                                             autoDeleteState);
                    }
                // remove and destroy the element
                mWdgtControlData->mElementArray.remove( i );
                return;
                }

            }
        }
    }
    
OSN_EXPORT void CAlfWidgetControl::removeElement(const IAlfElement& aElement )
    {
    for ( int i = 0; i < mWdgtControlData->mElementArray.count(); ++i ) 
        {
        if (mWdgtControlData->mElementArray[i])
            {
            if ( mWdgtControlData->mElementArray[i]->mElement == &aElement )
                {
                // make the element pointer of elementData null, and then 
                // destroy elementData. This will ensure that the memory
                // allocated for the elementData is freed but the element
                // is not destroyed
                mWdgtControlData->mElementArray[i]->mElement = 0;
                mWdgtControlData->mElementArray.remove(i);
                return;
                }
            }
        }
    }    

OSN_EXPORT uint CAlfWidgetControl::elementDataId(
    const IAlfElement& aElement,
    uint aIndex,
    uint aParentDataId )
    {
    uint range = dataIdRange( aElement );

    if ( aIndex >= range )
        {
        }
    // In hierarchical element structures this may overflow
    return aParentDataId * range + aIndex;
    }

OSN_EXPORT uint CAlfWidgetControl::parentElementDataId(
    const IAlfElement& aElement,
    uint aDataId )
    {
    uint dataIdRng = dataIdRange( aElement );
    if (0 == dataIdRng) return(0xFFFFFFFF);
    else
        return (uint)( aDataId / dataIdRange( aElement ) );
    }

OSN_EXPORT uint CAlfWidgetControl::dataIdToIndex( 
    const IAlfElement& aElement,
    uint aDataId )
    {
    uint dataIdRng = dataIdRange( aElement );
    if (0 == dataIdRng) return(0xFFFFFFFF);
    else
        return aDataId % dataIdRange( aElement );
    }

OSN_EXPORT IAlfVariantType* CAlfWidgetControl::elementData(
    const IAlfElement& aElement, uint aDataId )
    {
    if (!mWdgtControlData->mWidget)
        {
        return 0;	
        }
    IAlfModel* widgetModel = mWdgtControlData->mWidget->model();
    if ( !widgetModel )
        {
    	return 0;
        }
    IAlfVariantType* modelData = widgetModel->data();
    if ( !modelData || modelData->type() != IAlfVariantType::EMap )
        {
    	return 0;
        }
        
    IAlfMap* map = modelData->map();

    // Use "current" to traverse elements from the root to the leaf
    const IAlfElement* last = 0;
    while ( last != &aElement )
        {
        int currentDataId = aDataId;
        const IAlfElement* current;
        for ( current = &aElement ;
                current->parentElement() != last ;
                current = current->parentElement() )
            {
            currentDataId = parentElementDataId( *current, currentDataId );
            }
        int index = dataIdToIndex( *current, currentDataId );
        last = current;

        IAlfVariantType* data = map->item( UString(current->name()) );
        if ( data->type() == IAlfVariantType::EContainer )
            {
            data = data->container()->item( index );
            }
        if ( current == &aElement )
            {
            return data;
            }
        if ( data->type() == IAlfVariantType::EBranch )
            {
            // Continue with the child data
            map = data->branch()->childData();
            continue;
            }
        if ( data->type() == IAlfVariantType::EMap )
            {
            map = data->map();
            }
        else
            {
            return 0;
            }
        }

    return 0;
    }

OSN_EXPORT void CAlfWidgetControl::setDataIdRange(
    const IAlfElement& aElement, uint aRange )
    {
    for ( int i = 0; i < mWdgtControlData->mElementArray.count(); ++i )
        {
        if ( mWdgtControlData->mElementArray[i]->mElement == &aElement )
            {
            mWdgtControlData->mElementArray[i]->mDataIdRange = aRange;
            return;
            }
        }
    }

OSN_EXPORT uint CAlfWidgetControl::dataIdRange( const IAlfElement& aElement )
    {
    for ( int i = 0; i < mWdgtControlData->mElementArray.count(); ++i )
        {
        if ( mWdgtControlData->mElementArray[i]->mElement == &aElement )
            {
            return mWdgtControlData->mElementArray[i]->mDataIdRange;
            }
        }
    return 0;
    }

OSN_EXPORT void CAlfWidgetControl::updatePresentation()
    {
    if ( !mWdgtControlData->mWidget || !mWdgtControlData->mWidget->model() ||
            !mWdgtControlData->mWidget->model()->data() ||
            mWdgtControlData->mWidget->model()->data()->type() != 
            IAlfVariantType::EMap )
        {
        return;
        }
    IAlfMap* map = mWdgtControlData->mWidget->model()->data()->map();
    for ( int i = 0; i < map->count(); ++i )
        {
        IAlfElement* element = findElement( map->name( i ).getUtf8() );
        if ( element )
            {

            int childInd = childIndex( *map, i, 0, true );
            IAlfVariantType* data = map->item( i );
            try
                {
                if ( data->type() == IAlfVariantType::EMap ||
                        data->type() == IAlfVariantType::EBranch)
                    {
                    // Data for a single presentation instance (visual tree)
                    element->createVisualTree( *data,
                        elementDataId( *element, childInd, 0 ),
                        element->defaultParentLayout( 0 ), childInd );
                    }
                else if ( data->type() == IAlfVariantType::EContainer )
                    {

                    // Data for multiple presentation instances (visual trees)
                    IAlfContainer* container = data->container();
                    for ( int j = 0; j < container->count(); ++j )
                        {
                        element->createVisualTree( *container->item( j ),
                            elementDataId( *element, childInd + j, 0 ),
                            element->defaultParentLayout( 0 ), childInd + j );
                        }
                    }
                }
            catch (...)
                {
                ALF_THROW(AlfVisualException,ECanNotCreateVisual,"CAlfWidgetControl::updatePresentation failed")
                }

            }

        }
    }

OSN_EXPORT void CAlfWidgetControl::destroyPresentation( int aTimeMilliseconds )
    {
    (void)aTimeMilliseconds;

    for ( int i = 0; i < mWdgtControlData->mElementArray.count(); ++i )
        {
        mWdgtControlData->mElementArray[i]->mElement->removeAndDestroyVisuals(
                                                          aTimeMilliseconds );
        }
    processEvent(TAlfEvent(EEventWidgetPresentationDestroyed));
    }
    
OSN_EXPORT void CAlfWidgetControl::destroyVisualsAndElements(
    int aTimeMilliseconds )
    {
    (void)aTimeMilliseconds;

    // destroy visual trees from elements
    for( int i = 0; i < mWdgtControlData->mElementArray.count(); ++i )
        {
        mWdgtControlData->mElementArray[i]->mElement->removeAndDestroyVisuals(
                                                          aTimeMilliseconds );
        }
    // destroy elements
    while( numElements() )
        {
        removeAndDestroyElement( element(numElements()-1) );
        }	
	
    // destroy presentation related event handlers
    removeAndDestroyPresentationEventHandlers();
	
    // send notification
    processEvent(TAlfEvent(EEventWidgetPresentationDestroyed));		
    }
    
OSN_EXPORT void CAlfWidgetControl::updateParentLayout()
    {
    //get the parent control
    CAlfWidgetControl* parent = dynamic_cast<CAlfWidgetControl*>(Host());
    if (parent)
        {
        IAlfLayoutManager* layoutManager = 
            CAlfWidgetControl::makeInterface<IAlfLayoutManager>(parent);
        if (layoutManager)
            {
            layoutManager->updateChildLayout(this);
            }
        }
    }
    
OSN_EXPORT bool CAlfWidgetControl::isContainer() const
    {
    IAlfHostAPI* api = mWdgtControlData->mAlfHostAPIImpl.get();
    return (api && api->getConnectionCount() > 0);
    }
    
OSN_EXPORT bool CAlfWidgetControl::isFocusable() const
    {
    return checkState(IAlfWidgetControl::Enabled) && 
           checkState(IAlfWidgetControl::Focusable);
    }

void CAlfWidgetControl::setOwnerWidget( AlfWidget* aWidget )
    {
    mWdgtControlData->mWidget = aWidget;
    }

OSN_EXPORT AlfWidget* CAlfWidgetControl::widget()
    {
    return mWdgtControlData->mWidget;
    }

OSN_EXPORT AlfEventStatus CAlfWidgetControl::handleEvent(
    const TAlfEvent& aEvent )
    {
    AlfEventStatus ret = EEventNotHandled;
    if(isContainer())
        {
        CAlfWidgetControl* focused = 
                    dynamic_cast<CAlfWidgetControl*>(FocusedConnection());
        if(focused && aEvent.IsKeyEvent())
            {
            ret = focused->processEvent(aEvent);
            }
        else if ( aEvent.IsCustomEvent() )
            {
            int eventId = aEvent.CustomParameter();
            switch(eventId)
                {
                case EEventFocusNextWidget:
                    {
                      if( handleFocusNext() )
                          {
                          ret = EEventConsumed;
                          }
                    }
                    break;
                case EEventFocusPreviousWidget:
                    {
                      if( handleFocusPrevious() )
                          {
                          ret = EEventConsumed;
                          }
                    }
                    break;
                }
            }
        }
    return ret;

    }


OSN_EXPORT TBool CAlfWidgetControl::OfferEventL( const TAlfEvent& aEvent )
    {
    TBool ret(EFalse);
    try
        {
        ret = processEvent(aEvent);  
        }
    catch(AlfException& ae)
        {
        int reason = ae.errorCode();
        User::Leave( reason );
        }
    
    return ret;
    }
    
    
    
    
 OSN_EXPORT AlfEventStatus CAlfWidgetControl::processEvent(
    const TAlfEvent& aEvent )
    {
    AlfEventStatus ret(EEventNotHandled);
    
    /* If the event is not a custom event, stop execution if the widget is
       not in enabled state. For custom event continue execution anyway. */
    if(!(mWdgtControlData->mState & IAlfWidgetControl::Enabled) &&
          !(aEvent.IsCustomEvent()))
        {
        return EEventNotHandled;    
        }
    
    // Go through tunneling phase event handlers.
    AlfEventStatus ret2 = ProcessEventHandlingPhase( tunneling, aEvent );
    
    if( ret2 == EEventConsumed )  
        {
        return EEventConsumed;  
        }
    else if ( ret2 == EEventHandled )     
        {
        ret = EEventHandled;
        }

    // Execute business logic.
    if(aEvent.IsPointerEvent())
        {
        if (aEvent.PointerDown() && isFocusable() && !Focus())
             {
             AcquireFocus();  
             }
        }
    
    ret2 = handleEvent( aEvent );
    
    if( ret2 == EEventConsumed )  
        {
        return EEventConsumed;  
        }
    else if ( ret2 == EEventHandled )     
        {
        ret = EEventHandled;
        }
    
    // Go through bubbling phase event handlers.
    ret2 = ProcessEventHandlingPhase( bubbling, aEvent );
    
    if( ret2 == EEventConsumed )  
        {
        return EEventConsumed;  
        }
    else if ( ret2 == EEventHandled )     
        {
        ret = EEventHandled;
        }
        
    return ret;
    }

AlfEventStatus CAlfWidgetControl::ProcessEventHandlingPhase(
    int aPhase, const TAlfEvent& aEvent )
    {
    // *** Implementation note ***
    //
    // offerEvent implementations may remove event handlers from the array that is
    // being gone through. So that needs to be taken in consideration in the
    // loop implementation.

    AlfEventStatus ret = EEventNotHandled;

    AlfPtrVector<IAlfWidgetEventHandler>& eventHandlerArray =
        mWdgtControlData->mEventHandlerArray;    
    
    for (int i=0; i < eventHandlerArray.count() ; ++i)
        {
        IAlfWidgetEventHandler* eventHandler = 
            mWdgtControlData->mEventHandlerArray[i];
        
        int phase = eventHandler->eventExecutionPhase();
        
        bool matchingPhase = false;
        if ( aPhase == bubbling )
            {
            if ( phase == IAlfWidgetEventHandler::
                              EBubblingPhaseEventHandler || 
                 phase == IAlfWidgetEventHandler::
                              ETunnellingAndBubblingPhaseEventHandler )
                {
                matchingPhase = true;
                }
            }
        else if ( aPhase == tunneling )
            {
            if ( phase == IAlfWidgetEventHandler::
                              ETunnellingPhaseEventHandler || 
                 phase == IAlfWidgetEventHandler::
                              ETunnellingAndBubblingPhaseEventHandler )
                {
                matchingPhase = true;
                }            
            }
        
        if ( matchingPhase )
            {
            // This is preparation for the situation where event handlers are
            // removed or added during offerEvent call.
            // Store pointer to the event handler at next index.
            IAlfWidgetEventHandler* nextHandler = 0;
            
            if ( i+1 < eventHandlerArray.count() )
                {
                nextHandler = eventHandlerArray[i+1];
                }
            
            // Call offerEvent            
            AlfEventStatus ret2 = eventHandler->offerEvent(*this, aEvent);
            
            // Update status in the function return value.
            if( ret2 == EEventConsumed )  
                {
                ret = EEventConsumed;
                break;  
                }
            else if ( ret2 == EEventHandled )
	            {
	            ret = EEventHandled;
	            }            
            
            // Now check whether next handler in the event handler array still
            // matches with the stored pointer.
            if ( nextHandler )
                {
                // Check whether the event handler at index i+1 is still the 
                // same as before calling offerEvent. If not, the array has 
                // been modified and the variable i needs to be set again. 
                if ( i+1 >= eventHandlerArray.count() ||
                     (i+1 < eventHandlerArray.count() && 
                     eventHandlerArray[i+1] != nextHandler) )
                    {
                    // Array has changed in offerEvent, the handler at index 
                    // 'i+1' is not the same any more. Find the index of the 
                    // current event handler again in the array and fix the
                    // loop variable 'i' point to that and continue.
                    int newCount = eventHandlerArray.count();
                    for ( int j = 0 ; j < newCount ; j++ )
                        {
                        if ( eventHandlerArray[j] == eventHandler )
                            {
                            i = j;
                            break;
                            }
                        }
                    
                    }
                }                        
            }
        }
        
    return ret;
    }

// from base class IAlfModelChangeObserver

OSN_EXPORT void CAlfWidgetControl::modelChanged( IAlfModel& aModel )
    {
    (void)aModel;
    // This way the model change animation is customizable using event handlers
    // The default event handler could implement this:
    destroyPresentation( 0 );
    updatePresentation();
    updateParentLayout();
    }

OSN_EXPORT void CAlfWidgetControl::dataChanging(
    const AlfPtrVector<AlfModelOperation>& aOperations )
    {
    for ( int i = 0; i < aOperations.count(); ++i )
        {
        dataChanging( *aOperations[i] );
        }
    }

OSN_EXPORT void CAlfWidgetControl::dataChanging(
    const AlfModelOperation& aOperation )
    {
    int numIndices = aOperation.numContainerIndices();
    if ( !mWdgtControlData->mWidget->model() || numIndices <= 0 )
        {
        return;
        }
    uint dataId = 0, parentDataId = 0;
    IAlfElement* element = 0;
    IAlfElement* parentElement = 0;
    IAlfVariantType* data = mWdgtControlData->mWidget->model()->data();
    if(data == 0)
        {
        return;
        }
    IAlfBranch* parentBranch = 0;
    // Traverse the data hierarchy to find the data, dataId and the element
    for ( int i = 0; i < numIndices; ++i )
        {
        int index = aOperation.containerIndex( i );
        if ( data->type() == IAlfVariantType::EMap  &&
                index >= 0 && index < data->map()->count() )
            {
            parentElement = element;
            element = findElement( data->map()->name( index ).getUtf8() );
            if ( element == 0 )
                {
                return; // Not found
                }
            IAlfMap* map = data->map();
            data = data->map()->item( index );

            if ( data->type() == IAlfVariantType::EContainer )
                {
                ++i;
                int containerIndex = aOperation.containerIndex( i );
                if ( containerIndex < 0 || 
                       containerIndex >= data->container()->count() )
                    {
                    break; // Not found
                    }
                int childInd = 
                    childIndex( *map, index, containerIndex, i == 1 );
                parentDataId = dataId;
                dataId = elementDataId( *element, childInd, parentDataId );
                data = data->container()->item( containerIndex );
                }
            else
                {
                int childInd = childIndex( *map, index, 0, i == 0 );
                parentDataId = dataId;
                dataId = elementDataId( *element, childInd, parentDataId );
                }
            }
        else if ( data->type() == IAlfVariantType::EBranch &&
                  index == 0 && i == numIndices - 1 )
            {
            // Changing the parent data
            data = data->branch()->data();
            }
        else if ( data->type() == IAlfVariantType::EBranch && index == 1 )
            {
            parentBranch = data->branch();
            data = data->branch()->childData();
            }
        else
            {
            return; // Not found
            }
        }

    // Perform the operation
    if ( aOperation.operation() == AlfModelOperation::EOperationAdd )
        {
        if ( parentBranch )
            {
            // The parent is responsible for it's children
            parentElement->createChildVisualTree( 
                               element, aOperation.newData(),
                               *parentBranch,aOperation.index(),
                               parentDataId );
            }
        else
            {
            if(element)
                {
                // Data added to a root element
                try
                    {
                    element->createVisualTree(
                                 aOperation.newData(), dataId,
                                 element->defaultParentLayout( parentDataId ),
                                 aOperation.index() );
                    }
                catch (...)
                    {
                    ALF_THROW(AlfVisualException,ECanNotCreateVisual,"CAlfWidgetControl::dataChanging failed")
                    }               
                }
            else
                {
                ALF_THROW(AlfVisualException,ECanNotCreateVisual,"CAlfWidgetControl::dataChanging failed")
                }

            }
        }
    else if ( aOperation.operation() == AlfModelOperation::EOperationRemove )
        {
        if ( parentBranch )
            {
            // The parent is responsible for it's children
            parentElement->removeChildVisualTree(
                element, *parentBranch, aOperation.index(), parentDataId );
            }
        else
            {
            if(element)
                {
                // Data removed from a root element
                element->removeVisualTree( *data, dataId );
                }
            }
        }
    else if ( aOperation.operation() == AlfModelOperation::EOperationUpdate )
        {
        if(element)
            {
            element->updateVisualTree( aOperation.newData(), *data, dataId );            
            }
        }
    }

OSN_EXPORT void CAlfWidgetControl::dataChanged()
    {
    // The default implementation is empty.
    }

uint CAlfWidgetControl::childIndex(
    IAlfMap& aMap, uint aMapIndex,
    uint aContainerIndex, bool aRoot )
    {
    // This method is only used internally
    const UString& elementName = aMap.name( aMapIndex );
    int index = 0;
    for ( int i = 0; i < aMapIndex && i < aMap.count(); ++i )
        {
        if ( !aRoot || elementName == aMap.name( i ) )
            {
            IAlfVariantType* data = aMap.item( i );
            if ( data->type() == IAlfVariantType::EMap ||
                    data->type() == IAlfVariantType::EBranch)
                {
                index++;
                }
            else if ( data->type() == IAlfVariantType::EContainer )
                {
                index += data->container()->count();
                }
            }
        }
    return index + aContainerIndex;
    }

// ---------------------------------------------------------------------------
// From class IAlfInterfaceBase.
// Getter for interfaces provided by the control.
// ---------------------------------------------------------------------------
//
OSN_EXPORT IAlfInterfaceBase* CAlfWidgetControl::makeInterface(
    const IfId& aType )
    {
    IAlfInterfaceBase* interface = 0;
    UString param(aType.mImplementationId);
    if ( param == IAlfWidgetControl::type().mImplementationId )
        {
        return static_cast<IAlfWidgetControl*>( this );
        }
    else if (param == IAlfAttributeOwner::type().mImplementationId)
        {
        return 0; // control does not have AttributeOwner instance
        		  // and do not try get IF from elements in control
        }
    // Let the layout manager create the interface that is queried.
    else if(mWdgtControlData->mAlfHostAPIImpl.get())
    	{
    	IAlfLayoutManager* layoutManager = 
    	    mWdgtControlData->mAlfHostAPIImpl->getBaseLayout();
    	if(layoutManager)
    		{
    		interface = layoutManager->makeInterface(aType);
    		}
    	}

    if(interface == 0)
    	{
        //go through all the elements    
        for (int i = 0; i < numElements() && !interface; i++)
            {
            IAlfElement& ele = element(i);
            interface = ele.makeInterface(aType);
            }
    	}

    return interface;
    }


OSN_EXPORT CAlfLayout* CAlfWidgetControl::ContainerLayout(
    const CAlfControl* /*aConnected*/) const
    {
    if(mWdgtControlData->mAlfHostAPIImpl.get() != 0)
        {
        IAlfLayoutManager* layoutManager = 
            mWdgtControlData->mAlfHostAPIImpl->getBaseLayout();
        if(layoutManager != 0)
        	{
        	return &layoutManager->getLayout();
        	}
        }
    return 0;
    }

    
OSN_EXPORT void CAlfWidgetControl::AcquireFocus()
    {
    if (!FocusedConnection() && isFocusable())
        {
        if (isContainer())
            {
            IAlfHostAPI& hostApi = hostAPI();
            //select the first connection.
            
            CAlfWidgetControl* firstChild = nextFocusableControl(0);
            if (firstChild)
                {
                firstChild->AcquireFocus();
                }
            }
        else
            {
            CAlfControl::AcquireFocus();
            }
        }
    }
    
OSN_EXPORT void CAlfWidgetControl::RelinquishFocus()
    {
    if (Focus())
        {
        if (isContainer())
            {
            handleFocusNext();
            }
        else
            {
            //get parent control, check if it can focus next control.
            CAlfWidgetControl* parent = 
                dynamic_cast<CAlfWidgetControl*>(Host());
            if (parent)
                {
                parent->handleFocusNext();
                }
                
            CAlfControl::RelinquishFocus();
            }
        }
    }
    
// ---------------------------------------------------------------------------
// return hostapi. create instance if needed.
// ---------------------------------------------------------------------------
//
IAlfHostAPI& CAlfWidgetControl::hostAPI()
    {
    if (!mWdgtControlData->mAlfHostAPIImpl.get()) // was it already created?
        {
        //late creation
        mWdgtControlData->mAlfHostAPIImpl.reset( new (EMM) AlfHostAPIImpl() ); 
        mWdgtControlData->mAlfHostAPIImpl->setHostControl(*this);
        }
        
    return *mWdgtControlData->mAlfHostAPIImpl.get();
    }
    
OSN_EXPORT bool CAlfWidgetControl::handleFocusNext()
    {
    bool focusChanged = false;
    if (isContainer())
        {
        CAlfWidgetControl* focused = 
            dynamic_cast<CAlfWidgetControl*>(FocusedConnection());
        if (focused)
            {
            CAlfWidgetControl* nextChild = nextFocusableControl(focused);
                
            if (!nextChild)
                {
                //no next child, change focus to upper level, if that fails,
                //loop to first connection. 
                CAlfWidgetControl* parent = 
                    dynamic_cast<CAlfWidgetControl*>(Host());
                if (parent)
                    {
                    focusChanged = parent->handleFocusNext();
                    }
                if (!focusChanged)
                    {
                    //loop to first connection. 
                    nextChild = nextFocusableControl(0);
                    }
                }
                
            //go deeper down the widget hierarchy, until a no-container widget
            // is found
            while(nextChild && nextChild->isContainer())
                {
                nextChild = nextChild->nextFocusableControl(0);
                }
                
            if (nextChild)
                {
                nextChild->AcquireFocus();
                focusChanged = true;
                }
            }
        }
    return focusChanged;
    }
    
OSN_EXPORT bool CAlfWidgetControl::handleFocusPrevious()
    {
    bool focusChanged = false;
    if (isContainer())
        {

        CAlfWidgetControl* focused = 
            dynamic_cast<CAlfWidgetControl*>(FocusedConnection());
        if (focused)
            {
            CAlfWidgetControl* prevChild = previousFocusableControl(
                focused);

            if (!prevChild)
                {
                // no previous child, change focus to upper level, if that 
                // fails, loop to last connection.
                CAlfWidgetControl* parent = 
                    dynamic_cast<CAlfWidgetControl*>(Host());
                if (parent)
                    {
                    focusChanged = parent->handleFocusPrevious();
                    }
                if (!focusChanged)
                    {                    
                    //loop to last connection.
                    prevChild = previousFocusableControl(0);
                    }
                }
            //go deeper down the widget hierarchy, until a no-container widget
            //is found. 
            while(prevChild && prevChild->isContainer())
                {
                //in each level, select the last focused widget.
                prevChild = prevChild->previousFocusableControl(0);
                }
                
            if (prevChild)
                {
                prevChild->AcquireFocus();
                focusChanged = true;
                }
            }
        }
        
    return focusChanged;
    }
        
OSN_EXPORT CAlfWidgetControl* CAlfWidgetControl::getFirstFocusable(
    IAlfContainerWidget& aContainer)
    {
    CAlfWidgetControl* control = 0;
    if (aContainer.widgetCount() > 0)
        {
        control = aContainer.getWidget(0)->control();
        }
    return control;
    }

OSN_EXPORT CAlfWidgetControl* CAlfWidgetControl::getLastFocusable(
    IAlfContainerWidget& aContainer)
    {
    CAlfWidgetControl* control = 0;
    if (aContainer.widgetCount() > 0)
        {
        IAlfWidget* w = aContainer.getWidget(aContainer.widgetCount()-1);
        control = w->control();
        }
    return control;
    }
    
OSN_EXPORT CAlfWidgetControl* CAlfWidgetControl::getFocusableAfter(
    IAlfContainerWidget& aContainer,
    CAlfWidgetControl& aControl)
    {
    CAlfWidgetControl* control = 0;
    int ind = aContainer.getWidgetIndex(*aControl.widget());
    if (ind >= 0)
        {
        ind++;
        if (ind < aContainer.widgetCount())
            {
            IAlfWidget* w = aContainer.getWidget(ind);
            control = w->control();
            }
        }
        
    return control;
    }

OSN_EXPORT CAlfWidgetControl* CAlfWidgetControl::getFocusableBefore(
    IAlfContainerWidget& aContainer, CAlfWidgetControl& aControl)
    {
    CAlfWidgetControl* control = 0;
    int ind = aContainer.getWidgetIndex(*aControl.widget());
    ind--;
    if (ind >= 0)
        {
        IAlfWidget* w = aContainer.getWidget(ind);
        control = w->control();
        }
        
    return control;
    }
    
void CAlfWidgetControl::resetControlGroup(CAlfControlGroup& aControlGroup)
    {
    // If control group is already set and its same than the control group
    // given as a parameter there's nothing we need to do.
    if(!ControlGroup() || (ControlGroup() != &aControlGroup))
        {
        // Remove control from previous control group
        if(ControlGroup())
            {
            ControlGroup()->Remove(this);
            }
        // Append control to the new control group
        TRAPD(err, aControlGroup.AppendL(this));
        if(err != KErrNone)
            {
            ALF_THROW(AlfException, err, "CAlfWidgetControl::resetControlGroup - Appending control to a new control group failed.");
            }                    
        }
    }

void CAlfWidgetControl::resetHierarchyControlGroup(
    CAlfControlGroup& aControlGroup)
    {
    // Append this control to the new control group
    resetControlGroup(aControlGroup);
        
    // Call this recursively to all connected controls
    AlfHostAPIImpl* hostApi = mWdgtControlData->mAlfHostAPIImpl.get();
    if(hostApi != 0)
        {
        for(int i = 0; i < hostApi->getConnectionCount(); ++i)
            {
            CAlfWidgetControl* connectedControl = hostApi->getConnection(i);
            if(connectedControl)
                {
                connectedControl->resetHierarchyControlGroup(aControlGroup);
                }
            }
        }    
    }
    
    
OSN_EXPORT void CAlfWidgetControl::FocusChanged(
    CAlfDisplay& /*aDisplay*/,
    TBool aFocused)
    {
    // Ensure that the state bit remains synched to the toolkit's version
    if (aFocused)
        {
        mWdgtControlData->mState |= IAlfWidgetControl::Focused;
        }
    else
        {
        mWdgtControlData->mState &=~ IAlfWidgetControl::Focused;
        }

    if (aFocused)
        {
        processEvent(TAlfEvent(EEventWidgetGainedFocus));
        }
    else
        {
        processEvent(TAlfEvent(EEventWidgetLostFocus));
        }
    }
    
OSN_EXPORT CAlfWidgetControl* CAlfWidgetControl::nextFocusableControl(
    CAlfWidgetControl* aControl)
    {
    CAlfWidgetControl* control = 0;
    
    if (isContainer())
        {
        IAlfContainerWidget* container = 
            IAlfInterfaceBase::makeInterface<IAlfContainerWidget>(widget());
        
        assert(container);
        
        if (!aControl)
            {
            control = getFirstFocusable(*container);
            }
        else
            {
            control = getFocusableAfter(*container, *aControl);
            }
            
        while(control && !control->isFocusable())
            {
            control = control->getFocusableAfter(*container, *control);
            }
        }
        
    return control;
    }
    
OSN_EXPORT CAlfWidgetControl* CAlfWidgetControl::previousFocusableControl(
    CAlfWidgetControl* aControl)
    {
    CAlfWidgetControl* control = 0;
    
    if (isContainer())
        {
        IAlfContainerWidget* container = 
            IAlfInterfaceBase::makeInterface<IAlfContainerWidget>(widget());
        
        assert(container);
        
        if (!aControl)
            {
            control = getLastFocusable(*container);
            }
        else
            {
            control = getFocusableBefore(*container, *aControl);
            }
            
        while(control && !control->isFocusable())
            {
            control = control->getFocusableBefore(*container, *control);
            }
        }
        
    return control;
    }


// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void CAlfWidgetControl::enableStateVisible()
    {
    // Only take action if the current state is different
    if ( !(mWdgtControlData->mState & IAlfWidgetControl::Visible) )
        {
        // Update the new state in member variable
        mWdgtControlData->mState |= IAlfWidgetControl::Visible;

        }    
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void CAlfWidgetControl::disableStateVisible()
    {
    // Only take action if the current state is different
    if ( mWdgtControlData->mState & IAlfWidgetControl::Visible )
        {
        // Update the new state in member variable
        mWdgtControlData->mState &= ~IAlfWidgetControl::Visible;
        }    
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void CAlfWidgetControl::enableStateEnabled()
    {
    // Only take action if the current state is different
    if ( !(mWdgtControlData->mState & IAlfWidgetControl::Enabled) )
        {
        // Update the new state in member variable
        mWdgtControlData->mState |= IAlfWidgetControl::Enabled;
        }        
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void CAlfWidgetControl::disableStateEnabled()
    {
    // Only take action if the current state is different
    if ( mWdgtControlData->mState & IAlfWidgetControl::Enabled )
        {
        // Update the new state in member variable
        mWdgtControlData->mState &= ~IAlfWidgetControl::Enabled;
        }
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void CAlfWidgetControl::enableStateFocused()
    {
    // Only take action if the current state is different
    if ( !(mWdgtControlData->mState & IAlfWidgetControl::Focused) )
        {
        // Set the new state
        AcquireFocus();
        
        // Update the new state in member variable
        mWdgtControlData->mState |= IAlfWidgetControl::Focused;
        }
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void CAlfWidgetControl::disableStateFocused()
    {
    // Only take action if the current state is different
    if ( mWdgtControlData->mState & IAlfWidgetControl::Focused )
        {
        // Set the new state
        RelinquishFocus();
        
        // Update the new state in member variable
        mWdgtControlData->mState &= ~IAlfWidgetControl::Focused;
        }        
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void CAlfWidgetControl::enableStateFocusable()
    {
    // Only take action if the current state is different
    if ( !(mWdgtControlData->mState & IAlfWidgetControl::Focusable) )
        {
        // Update the new state in member variable
        mWdgtControlData->mState |= IAlfWidgetControl::Focusable;
        }    
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void CAlfWidgetControl::disableStateFocusable()
    {
    // Only take action if the current state is different
    if ( mWdgtControlData->mState & IAlfWidgetControl::Focusable )
        {
        // Update the new state in member variable
        mWdgtControlData->mState &= ~IAlfWidgetControl::Focusable;
        }    
    }

void CAlfWidgetControl::checkStateInvariants(uint aState)
    {
    // To be focused, it needs to be focusable (Focus -> Focusable)
    if(aState & IAlfWidgetControl::Focused 
    &&  !(aState & IAlfWidgetControl::Focusable)  )
        {
        ALF_THROW(AlfException, EInvalidArgument, "AlfWidgetControl() - state changing operation. Illegal state would result.");
        }

    // To be focusable, it needs to be enabled (Focusable -> Enabled)
    if(aState & IAlfWidgetControl::Focusable 
    &&  !(aState & IAlfWidgetControl::Enabled)  )
        {
        ALF_THROW(AlfException, EInvalidArgument, "AlfWidgetControl() - state changing operation. Illegal state would result.");
        }
    }

void CAlfWidgetControl::predictAndCheckStateInvariants(
    uint aChangePattern,
    bool aEnabling) const
    {
    uint predictedState(mWdgtControlData->mState);
    if(aEnabling)
        {
        predictedState |= aChangePattern;
        }
    else
        {
        predictedState &= ~aChangePattern;
        }
    checkStateInvariants(predictedState);
    }


} // namespace Alf

