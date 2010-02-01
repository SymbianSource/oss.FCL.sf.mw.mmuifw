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
* Description:  The basic implementation for presentation elements.
*
*/


#include "alf/alfelement.h"
#include <alf/alfwidgetcontrol.h>
#include <alf/alfvarianttype.h>
#include "alf/ialfvisualtemplate.h"
#include "alf/alfreferencetovisual.h"
//#include "alf/alfperf.h"
#include <alf/alfexceptions.h>

#include "alfelementattributeownerimpl.h"

namespace Alf
    {

class  AlfVisualData
    {
public:
    AlfVisualData( CAlfVisual* aVisualTree, uint aDataID ) :
            mVisualTree( aVisualTree ), mDataID( aDataID )
        {
        }
    CAlfVisual *mVisualTree;
    uint mDataID;
    ~AlfVisualData()
        {

        }

    };

class AlfElementImpl
    {
public:
    AlfElementImpl()
        {
        mControl = 0;
        mVisualTemplate = 0;
        mParentElement = 0;
 	    mParentLayout = 0;
        }
    ~AlfElementImpl()
        {
        if (mVisualTemplate)
            {
            delete mVisualTemplate;
            mVisualTemplate = 0;
            }
        if (mParentLayout)
            {
            delete mParentLayout;
            mParentLayout = 0;
            }

        mVisualTreeArray.clear();
        }
    /**
     * The control. Not own.
     */
    CAlfWidgetControl* mControl;

    /**
     * The element name.
     */
    UString mName;

    /**
     * The visual template for creating visual trees. Own.
     */
    IAlfVisualTemplate* mVisualTemplate;

    /*
     * Parent element. NULL for root elements. Not own.
     */
    IAlfElement *mParentElement;

    /*
     * Array of created visual trees with the associated data IDs.
     */
    AlfPtrVector<AlfVisualData>mVisualTreeArray; //RArray<TAlfVisualData> mVisualTreeArray;

    /*
     * Reference to the default parent layout. Own.
     */
    AlfReferenceToVisual *mParentLayout;
    
    /*
     * IAlfAttributeOwner implementation. Own.
     */
    auto_ptr<AlfElementAttributeOwnerImpl> mAttributeOwnerImpl;
    };

// ======== MEMBER FUNCTIONS ========

OSN_EXPORT AlfElement::AlfElement()
    {
    }

OSN_EXPORT void AlfElement::construct( CAlfWidgetControl& aControl, const char* aName )
    {
    mData.reset(new (EMM) AlfElementImpl());

    mData->mControl = &aControl;
    mData->mName = UString(aName);
    mData->mParentLayout=NULL;
    mData->mParentElement=NULL;
    mData->mVisualTemplate=NULL;
    mData->mVisualTreeArray.setAutoDelete(true);
    mData->mAttributeOwnerImpl.reset(new (EMM) AlfElementAttributeOwnerImpl(*this, 
        *mData->mControl));
    aControl.addElement(this);
    }


OSN_EXPORT AlfElement::AlfElement(CAlfWidgetControl& aControl, const char* aName)
    {
    construct( aControl, aName );

    }

OSN_EXPORT AlfElement::~AlfElement()
    {
    }

OSN_EXPORT const char* AlfElement::name() const
    {
    return mData->mName.getUtf8();
    }

OSN_EXPORT void AlfElement::createChildVisualTree(
    IAlfElement* aElement, IAlfVariantType& aChildData,
    IAlfBranch& aData, int aIndex, uint aDataID )
    {
    // Pass directly to the child element CreateVisualTree, no need
    // to do anything else. Derived classes may need additional steps here.
//    ALF_PERF_START( perfdata, "AlfElement-createVisualTree-createChildVisualTree")
    aElement->createVisualTree( aChildData, aData, aIndex, aDataID,
                                aElement->defaultParentLayout( aDataID ),
                                aIndex );
//    ALF_PERF_STOP( perfdata, "AlfElement-createVisualTree-createChildVisualTree")
    }

OSN_EXPORT CAlfVisual* AlfElement::createVisualTree(
    IAlfVariantType& aData, IAlfBranch& aParentData, int aIndex,
    uint aParentDataID, CAlfLayout* aParentLayout, int aLayoutIndex )
    {
    if ( aIndex >= 0 && aIndex <= aParentData.childrenCount() )
        {
        // Update data IDs of all visual trees after the new visual tree
        // Start from the aIndex, and increase all indices by one
        updateDataIDs( aParentData, aIndex, 1, aParentDataID, aParentDataID );

        // Create the new visual tree
        uint dataID = control().elementDataId( *this, aIndex, aParentDataID );
        return createVisualTree( aData, dataID, aParentLayout, aLayoutIndex );
        }
    ALF_THROW(AlfElementException,EInvalidElement,"AlfElement")
    }

OSN_EXPORT CAlfVisual* AlfElement::createVisualTree(
    IAlfVariantType& aData, uint aDataID, CAlfLayout* aParentLayout, int aLayoutIndex )
    {
    CAlfVisual *retVisual = NULL;
    IAlfMap* currentData = NULL;
    IAlfBranch* branch = NULL;
    if ( aData.type()== IAlfVariantType::EMap )
        {
        currentData = aData.map();
        }
    else if (aData.type()== IAlfVariantType::EBranch)
        {
        branch = aData.branch();
        currentData = aData.branch()->data();
        }

    if ( currentData && mData->mVisualTemplate )
        {
        //Find the visual tree with data id
        int index = findFromArray( aDataID );
        if ( index < 0 )
            {
            // Create a new visual tree
            try
                {
                retVisual = mData->mVisualTemplate->createVisualTree(*mData->mControl,
                            currentData, aParentLayout, aLayoutIndex);
                }
            catch (...)
                {
                ALF_THROW(AlfVisualException,ECanNotCreateVisual,"AlfElement")
                }
            // Append the visual into the array
            mData->mVisualTreeArray.resize(mData->mVisualTreeArray.count()+1);
            mData->mVisualTreeArray.insert(mData->mVisualTreeArray.count(),(new (EMM) AlfVisualData(retVisual, aDataID)));
            }
        else
            {
            ALF_THROW(AlfVisualException,EInvalidElement,"AlfElement")
            }
        }

    if ( branch )
        {
        // Pass to children
        int count = branch->childrenCount();
        for ( int i = 0; i < count; ++i )
            {
            IAlfElement* element = mData->mControl->findElement(
                                       branch->childName( i ).getUtf8() );
            if ( element )
                {
                createChildVisualTree(
                    element, *branch->childData( i ), *branch, i, aDataID );
                }
            }
        }

    return retVisual;
    }

OSN_EXPORT void AlfElement::removeChildVisualTree(
    IAlfElement* aElement, IAlfBranch& aData, int aIndex, uint aDataID )
    {
    // Pass directly to the child element RemoveVisualTree, no need
    // to do anything else. Derived classes may need additional steps here.
    aElement->removeVisualTree( aData, aIndex, aDataID );
    }

OSN_EXPORT void AlfElement::removeVisualTree( IAlfBranch& aParentData,
        int aIndex, uint aParentDataID )
    {
    if ( aIndex < aParentData.childrenCount() )
        {
        IAlfVariantType* data = aParentData.childData( aIndex );
        uint dataID = control().elementDataId( *this, aIndex, aParentDataID );

        removeVisualTree( *data, dataID );

        // Update data IDs of all visual trees after the removed visual tree
        // Start from the aIndex + 1, and decrease all indices by one
        updateDataIDs( aParentData, aIndex + 1, -1, aParentDataID, aParentDataID );
        }
    else
        ALF_THROW(AlfVisualException,EInvalidElement,"AlfElement")
        }

OSN_EXPORT void AlfElement::removeVisualTree( IAlfVariantType& aData, uint aDataID )
    {
    if ( aData.type() == IAlfVariantType::EBranch )
        {
        // Pass to children
        IAlfBranch* branch = aData.branch();
        int count = branch->childrenCount();
        for ( int i = 0; i < count; ++i )
            {
            IAlfElement* element = mData->mControl->findElement( branch->childName( i ).getUtf8() );
            if ( element )
                {
                removeChildVisualTree( element, *branch, i, aDataID );
                }
            }
        }

    // Remove visual tree from this element
    int index = findFromArray( aDataID );
    if (index >= 0)
        {
        CAlfVisual* visual = mData->mVisualTreeArray[index]->mVisualTree;
        visual->RemoveAndDestroyAllD();
        mData->mVisualTreeArray.remove( index );
        }
    else
        {
        ALF_THROW(AlfVisualException,EInvalidElement,"AlfElement")
        }
    }

OSN_EXPORT void AlfElement::updateVisualTree(
    IAlfVariantType& aNewData, IAlfVariantType& aOldData, uint aDataID )
    {
    IAlfMap* currentNewData = NULL;
    IAlfMap* currentOldData = NULL;
    IAlfBranch* branch = NULL;
    IAlfBranch* oldBranch = NULL;

    if ( aNewData.type()== IAlfVariantType::EMap )
        {
        currentNewData = aNewData.map();
        }
    else if (aNewData.type()== IAlfVariantType::EBranch)
        {
        branch = aNewData.branch();
        currentNewData = branch->data();
        }

    if (&aOldData)
        {
        if (aOldData.type()== IAlfVariantType::EMap)
            {
            currentOldData = aOldData.map();
            }
        else if (aOldData.type()== IAlfVariantType::EBranch)
            {
            oldBranch = aOldData.branch();
            currentOldData = aOldData.branch()->data();
            }
        }

    if ( currentNewData && mData->mVisualTemplate )
        {
        //Find the visual tree with data id
        int index = findFromArray( aDataID );
        if (index >= 0)
            {
            // Update the contents of the visual tree
            CAlfVisual* visual = mData->mVisualTreeArray[index]->mVisualTree;
            //ALF_PERF_START( perfdata, "AlfElement-updateVisualTree-VTUpdateVisualTree")
            mData->mVisualTreeArray[index]->mVisualTree =
                mData->mVisualTemplate->updateVisualTree(
                    currentNewData, currentOldData, *visual );
            //ALF_PERF_STOP( perfdata, "AlfElement-UpdateVisualTree-VTUpdateVisualTree")
            }
        }

    if ( branch )
        {
        // Pass to children
        int count = branch->childrenCount();
        for ( int i = 0; i < count; ++i )
            {
            IAlfElement* element = mData->mControl->findElement( branch->childName( i ).getUtf8() );
            if ( element )
                {
                IAlfVariantType* oldData = NULL;
                if (oldBranch)
                    {
                    oldData = oldBranch->childData( i );
                    }

                element->updateVisualTree( *branch->childData( i ), *oldData,
                                           mData->mControl->elementDataId( *element, i, aDataID ) );
                }
            }
        }
    }

OSN_EXPORT void AlfElement::addVisualTree(CAlfVisual* aVisualTree, uint aDataID)
    {
    int index = findFromArray( aDataID );
    if ( index >= 0 || aVisualTree == NULL )
        {
        ALF_THROW(AlfVisualException,EInvalidElement,"AlfElement")
        }
    // Append the visual with the assiciated data id into the array

    mData->mVisualTreeArray.resize(mData->mVisualTreeArray.count()+1);
    mData->mVisualTreeArray.insert(mData->mVisualTreeArray.count(),(new (EMM) AlfVisualData(aVisualTree, aDataID)));
    }

OSN_EXPORT void AlfElement::replaceVisualTree( IAlfVariantType& aData, uint aDataID, uint aOldDataID )
    {
    IAlfVariantType* oldData = control().elementData(*this, aOldDataID );

    int index = findFromArray( aOldDataID );
    if ( index < 0 )
        {
        ALF_THROW(AlfVisualException,EInvalidElement,"AlfElement")
        }
    mData->mVisualTreeArray[index]->mDataID = aDataID;

    // Update the contents of the visual tree with the new data
    updateVisualTree( aData, *oldData, aDataID );
    }

OSN_EXPORT void AlfElement::updateDataIDs(
    IAlfBranch& aParentBranch, int aStartIndex,
    int aOffset, uint /*aOldParentDataId*/, uint aParentDataID )
    {

    for ( int i = 0; i < mData->mVisualTreeArray.count(); ++i )
        {
        uint oldDataID = mData->mVisualTreeArray[i]->mDataID;
        uint index = control().dataIdToIndex( *this, oldDataID );
        uint newDataID = control().elementDataId(
                             *this, index + aOffset, aParentDataID );
        if ( index >= aStartIndex && oldDataID != newDataID )
            {
            // Update the data ID
            mData->mVisualTreeArray[i]->mDataID = newDataID;
            // The old data ID was used to calculate the child element data IDs
            // We need to update the visual trees in child elements
            IAlfVariantType* data = aParentBranch.childData( index );
            if ( data != NULL )
                {
                if ( data->type() == IAlfVariantType::EBranch )
                    {
                    IAlfBranch* branch = data->branch();
                    for ( int j = 0; j < branch->childrenCount(); ++j )
                        {
                        IAlfElement* childElement =
                            mData->mControl->findElement( branch->childName( j ).getUtf8() );
                        if ( childElement )
                            {
                            childElement->updateDataIDs(
                                *branch, 0, 0, oldDataID, newDataID );
                            }
                        }
                    }
                }
            }
        }
    }

OSN_EXPORT CAlfLayout* AlfElement::defaultParentLayout(uint aParentDataID)
    {
    if (mData->mParentLayout)
        {
        CAlfVisual* visual=NULL;
        try
            {
            visual = mData->mParentLayout->resolve(*mData->mControl, aParentDataID);
            }
        catch (...)
            {
            ALF_THROW(AlfVisualException,EInvalidVisual,"AlfElement")
            }
        return (CAlfLayout *)visual;
        }
    return NULL;
    }

OSN_EXPORT void AlfElement::setDefaultParentLayout(AlfReferenceToVisual* aReference)
    {
    if (mData->mParentLayout)
        {
        delete mData->mParentLayout;
        mData->mParentLayout = 0;
        }
    mData->mParentLayout = aReference;
    }

OSN_EXPORT void AlfElement::setVisualTemplate(IAlfVisualTemplate& aTemplate) throw()
    {
    // Remove the visual template from its previous owner
    if(aTemplate.owner())
        {
        aTemplate.owner()->removeVisualTemplate();
        }
    
    // Set the visual template to this element
    mData->mVisualTemplate = &aTemplate;
    mData->mVisualTemplate->setOwner(this);
    }

OSN_EXPORT IAlfVisualTemplate* AlfElement::getVisualTemplate() const throw()
    {
    return mData->mVisualTemplate;
    }

OSN_EXPORT void AlfElement::destroyVisualTemplate() throw()
    {
    if (mData->mVisualTemplate)
        {
        delete mData->mVisualTemplate;
        mData->mVisualTemplate = 0;
        }    
    }

OSN_EXPORT IAlfVisualTemplate* AlfElement::removeVisualTemplate() throw()
    {
    IAlfVisualTemplate* ret = mData->mVisualTemplate;
    
    // Set owner to NULL in removed visual template
    if(mData->mVisualTemplate)
        {
        mData->mVisualTemplate->setOwner(0);
        }    
    mData->mVisualTemplate = 0;

    return ret;
    }

OSN_EXPORT CAlfVisual* AlfElement::findVisual(const char* aName, uint aDataID)
    {
    int index = findFromArray( aDataID );
    if ( index < 0 )
        {
        return NULL;
        }
    return mData->mVisualTreeArray[index]->mVisualTree->FindTag(TPtrC8((unsigned char*)aName));//Alf dependency
    }

OSN_EXPORT CAlfVisual* AlfElement::findVisual(uint aDataID)
    {
    int index = findFromArray( aDataID );
    if ( index < 0 )
        {
        return NULL;
        }
    return mData->mVisualTreeArray[index]->mVisualTree;
    }

OSN_EXPORT uint AlfElement::dataID( const CAlfVisual& aVisual ) const
    {
    for ( int i = 0; i < mData->mVisualTreeArray.count(); ++i )
        {
        if ( mData->mVisualTreeArray[i]->mVisualTree == &aVisual )
            {
            return mData->mVisualTreeArray[i]->mDataID;
            }
        }
    return 0;
    }

int AlfElement::findFromArray( uint aDataID ) const
    {
    for ( int i = 0; i < mData->mVisualTreeArray.count(); ++i )
        {
        if ( mData->mVisualTreeArray[i]->mDataID == aDataID )
            {
            return i;
            }
        }
    return -1;
    }

OSN_EXPORT const IAlfElement* AlfElement::parentElement() const
    {
    return mData->mParentElement;
    }

OSN_EXPORT void AlfElement::setParentElement(IAlfElement& aParent)
    {
    mData->mParentElement = &aParent;
    }

OSN_EXPORT  CAlfWidgetControl& AlfElement::control()
    {
    return *mData->mControl;
    }

OSN_EXPORT  void AlfElement::removeAndDestroyVisuals( int aTimeMilliseconds )
    {
    //remove child elements
    for (int i = 0; i<control().numElements(); i++)
        {
        IAlfElement& element = control().element(i);
        if (element.parentElement() == this)
            {
            element.removeAndDestroyVisuals(aTimeMilliseconds);
            }
        }
    
    //remove visual trees and elements, that are linked 
    //to this element with parentlayout.
    for (int i = 0 ; i < mData->mVisualTreeArray.count(); i++)
        {
        AlfVisualData* vData = mData->mVisualTreeArray[i];
        CAlfVisual* visual = vData->mVisualTree;
        if(visual)
            {
            CAlfLayout* layout = dynamic_cast<CAlfLayout*>(visual);
            if (layout)
                {
                for (int j = 0; j<control().numElements(); j++)
                    {
                    IAlfElement& element = control().element(j);
                    CAlfLayout* parent = element.defaultParentLayout(vData->mDataID);
                    if (parent && contains(*parent))
                        {
                        element.removeAndDestroyVisuals(aTimeMilliseconds);
                        }
                    }
                }
            visual->RemoveAndDestroyAllD();
            vData->mVisualTree = NULL;
            }
        }
        
    mData->mVisualTreeArray.clear();
    }


//From IAlfElement
OSN_EXPORT bool AlfElement::contains(CAlfVisual& aVisual) const
    {
    CAlfVisual* visual = &aVisual;
    //traverse back to the root parent
    while (visual)
        {
        for (int i = 0 ; i < mData->mVisualTreeArray.count(); i++)
            {
            if (visual == mData->mVisualTreeArray[i]->mVisualTree)//is this visual parent of passed visual?
                return true;
            }
        visual = visual->Layout();
        }
    return false; //referred visual is not part of this element.
    }

// ---------------------------------------------------------------------------
// From class IAlfInterfaceBase.
// Getter for interfaces provided by the element.
// ---------------------------------------------------------------------------
//
OSN_EXPORT IAlfInterfaceBase* AlfElement::makeInterface( const IfId& aType )
    {
    UString param(aType.mImplementationId);
    if (param == IAlfElement::type().mImplementationId)
        {
        return static_cast<IAlfElement*>(this);
        }
    else if (param == IAlfAttributeOwner::type().mImplementationId)
        {
    	return static_cast<IAlfAttributeOwner*>(mData->mAttributeOwnerImpl.get());
        }
    return NULL;
    }
    
    } // Alf
