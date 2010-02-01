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
* Description:  Visual template class for alfred widget model.
*      This class creates and updates visual tree.
*
*/




//INCLUDES
#include <algorithm>
#include <assert.h>
#include <alf/alfcontrol.h>
#include <alf/alfbrush.h>
#include <alf/alfvisualfactory.h>
#include <alf/alfbrusharray.h>
#include <utf.h>
#include <alf/alfimagevisual.h>
#include <alf/alftexture.h>
#include <alf/alfenv.h>
#include <alf/alfexceptions.h>
//#include "alf/alfperf.h"
#include <osn/osnnew.h>
#include <alf/ialfelement.h>
#include "alf/alfvisualtemplate.h"
#include "alf/alfvarianttype.h"
#include "alf/ialfattributesetter.h"
#include "alf/alfattributecontainer.h"
#include "alf/alfattribute.h"

// The number of attribute setters is usually between 1 and 5
static const int KAttributeArrayGranularity = 4;
static const int KContainerArrayGranularity = 4;

namespace Alf
    {

struct DeleteTemplate
{
    void operator()(const IAlfVisualTemplate* aTemplate) const
        {
        delete aTemplate;
        }
};

// -----------------------------------------------------------------------------
// Helper function to iterate through a visual template hierarchy and
// investigate whether a given visual template exists in it.
// -----------------------------------------------------------------------------
bool existsInHierarchy(IAlfVisualTemplate& aRoot, const IAlfVisualTemplate& aTemplate)
    {
    // Check the template against the root node.
    if(&aTemplate == &aRoot)
        {
        return true;
        }
    
    // Iterate through all the root's children
    for(int i = 0; i < aRoot.numChildTemplates(); ++i)
        {
        if(existsInHierarchy(aRoot.childTemplate(i), aTemplate))
            {
            return true;
            }
        }
        
    return false;        
    }    
    
// ============================ MEMBER FUNCTIONS ===============================

// ---------------------------------------------------------------------------
// Description : Constructor
// ---------------------------------------------------------------------------
OSN_EXPORT AlfVisualTemplate* AlfVisualTemplate::create()
    {
    return new( EMM ) AlfVisualTemplate;
    }

// ---------------------------------------------------------------------------
// Description : Constructor
// ---------------------------------------------------------------------------
OSN_EXPORT AlfVisualTemplate* AlfVisualTemplate::create(TAlfVisualType aType)
    {
    return new( EMM ) AlfVisualTemplate( aType );
    }

// ---------------------------------------------------------------------------
// Description : Constructor
// ---------------------------------------------------------------------------
OSN_EXPORT AlfVisualTemplate* AlfVisualTemplate::create(TAlfLayoutType aType)
    {
    return new( EMM ) AlfVisualTemplate( aType );
    }

// ============================ MEMBER FUNCTIONS ===============================

// ---------------------------------------------------------------------------
// Description : Constructor
// ---------------------------------------------------------------------------
OSN_EXPORT AlfVisualTemplate::AlfVisualTemplate():
    mVisualType(EAlfVisualTypeVisual),
    mAttributeArray(KAttributeArrayGranularity),
    mContainerArray(KContainerArrayGranularity),
    mSelectOneChild(false),
    mOwner(0),
    mParent(0)
    {
    }

// ---------------------------------------------------------------------------
// Description : Constructor
// ---------------------------------------------------------------------------
OSN_EXPORT AlfVisualTemplate::AlfVisualTemplate(TAlfVisualType aType):
    mVisualType(aType),
    mAttributeArray(KAttributeArrayGranularity),
    mContainerArray(KContainerArrayGranularity),
    mSelectOneChild(false),
    mOwner(0),
    mParent(0)
    {
    }

// ---------------------------------------------------------------------------
// Description : Constructor
// ---------------------------------------------------------------------------
OSN_EXPORT AlfVisualTemplate::AlfVisualTemplate(TAlfLayoutType aType):
    mVisualType(-1 - aType),
    mAttributeArray(KAttributeArrayGranularity),
    mContainerArray(KContainerArrayGranularity),
    mSelectOneChild(false),
    mOwner(0),
    mParent(0)
    {
    }

// ---------------------------------------------------------------------------
// Description : Destructor
// ---------------------------------------------------------------------------
OSN_EXPORT AlfVisualTemplate::~AlfVisualTemplate()
    {
    mBrushArray.clear();
    mAttributeArray.clear();
    mContainerArray.clear();
    
    // Remove this visual template from it's parent or owner
    if(parent())
        {
        parent()->removeChildTemplate(*this);
        }
    if(owner())
        {
        owner()->removeVisualTemplate();
        }
    
    // Destroy child visual templates in two passes, since destructor of a child
    // visual template could affect the content of the this visual template's child vector.
    vector<IAlfVisualTemplate*> children(mChildren);
    mChildren.clear();
    for_each(children.begin(), children.end(), DeleteTemplate());
    }

// ---------------------------------------------------------------------------
// Description : Set the owner element.
// ---------------------------------------------------------------------------
OSN_EXPORT void AlfVisualTemplate::setOwner(IAlfElement* aOwner) throw()
    {
	if(aOwner != mOwner)
	    {
	    // Remove this visual template from it's parent. Parent and owner are
	    // mutually exclusive properties on a visual template.
	    if(aOwner && (parent() != 0))
	        {
	        parent()->removeChildTemplate(*this);
	        }
	    
	    // Set the owner of this visual template.
		mOwner = aOwner;
	    }
    }

OSN_EXPORT IAlfElement* AlfVisualTemplate::owner() const throw()
    {
	return mOwner;
    }
    
// ---------------------------------------------------------------------------
// Description : Set the name of the visual, i.e., the tag.
// ---------------------------------------------------------------------------
OSN_EXPORT void AlfVisualTemplate::setName(const char* aName)
    {
    mVisualName = UString(aName);
    }

// ---------------------------------------------------------------------------
// Description : Get the name of the visual, i.e., the tag.
// ---------------------------------------------------------------------------
OSN_EXPORT const char* AlfVisualTemplate::name() const throw()
    {
    return mVisualName.getUtf8();
    }

// ---------------------------------------------------------------------------
// Description : Add a child visual template.
// ---------------------------------------------------------------------------
OSN_EXPORT void AlfVisualTemplate::addChildTemplate(IAlfVisualTemplate& aChild)
    {    
    insertChildTemplate(aChild, mChildren.size());
    }

// ---------------------------------------------------------------------------
// Description : Inserts a child visual template at given index
// ---------------------------------------------------------------------------
OSN_EXPORT void AlfVisualTemplate::insertChildTemplate(IAlfVisualTemplate& aChild, int aIndex)
	{
	if(!(aIndex >=0 && aIndex <= mChildren.size()))
	    ALF_THROW(AlfException, EInvalidArrayIndex, "AlfVisualTemplate::insertChildTemplate() - Index out of bounds.");
	     	
    // Verify that the given argument is valid
    if(existsInHierarchy(*this, aChild))
        {
        ALF_THROW(AlfException, EInvalidHierarchy, "AlfVisualTemplate::insertChildTemplate() - Adding a child visual template that is already in the hierarchy.");
        }
    if(existsInHierarchy(aChild, *this))
        {
        ALF_THROW(AlfException, EInvalidHierarchy, "AlfVisualTemplate::insertChildTemplate() - Attempt to create a recursive visual template tree.");
        }
    if(layoutType() < 0)
        {
        ALF_THROW(AlfException, EInvalidHierarchy, "AlfVisualTemplate::insertChildTemplate() - Attempt to add a child visual into a non-layout visual template.");
        }     	
     	
    // Insert child visual template to the array
	mChildren.insert(mChildren.begin() + aIndex, &aChild);
	
    // Remove child from previous hierarchy and add it under this visual template.
    if(aChild.parent())
        {
        // Visual template cannot have a parent and owner at the same time.
        assert(aChild.owner() == 0);
        aChild.parent()->removeChildTemplate(aChild);        
        }
    if(aChild.owner())
        {
        // Visual template cannot have a parent and owner at the same time.
        assert(aChild.parent() == 0);
        aChild.owner()->removeVisualTemplate();
        }
    
    // Set a new parent for this visual template.
    aChild.setParent(this);	
	}

// ---------------------------------------------------------------------------
// Description : Destroyes a child visual template at given index
// ---------------------------------------------------------------------------
OSN_EXPORT void AlfVisualTemplate::destroyChildTemplate(int aIndex) throw()
	{
	// Remove and destroy a child template by the given index
	if(aIndex >=0 && aIndex < mChildren.size())
		{
		IAlfVisualTemplate* child = mChildren[aIndex];				
		removeChildTemplate(*child);
		delete child;
		}				
	}
	
// ---------------------------------------------------------------------------
// Description : Destroyes a child visual template.with the given name
// ---------------------------------------------------------------------------
OSN_EXPORT void AlfVisualTemplate::destroyChildTemplate(const char* aName) throw()
	{
	// Search through the children for the given name.
	for(int i = 0; i < mChildren.size() ;i++)
    	{
		if(!strcmp(mChildren[i]->name(), aName))
    		{
    		// Remove and destroy the found child visual template
    		IAlfVisualTemplate* child = mChildren[i];
    		removeChildTemplate(*child);
    		delete child;
    		}
    	}
	}
	
// ---------------------------------------------------------------------------
// Description : Get the number of child visual templates.
// ---------------------------------------------------------------------------
OSN_EXPORT int AlfVisualTemplate::numChildTemplates() const throw()
    {
    return mChildren.size();
    }

// ---------------------------------------------------------------------------
// Description : Get a child visual template.
// ---------------------------------------------------------------------------
OSN_EXPORT IAlfVisualTemplate& AlfVisualTemplate::childTemplate(int aIndex) const
    {
    if((aIndex < 0) || (aIndex >= numChildTemplates()))
        {
        ALF_THROW(AlfException, EInvalidArrayIndex, "AlfVisualTemplate::childTemplate() - Index out of bounds.");
        }
    return *mChildren[aIndex];
    }

// -----------------------------------------------------------------------------
// Description : Add a new visual attribute setter. The ownership is not passed.
// -----------------------------------------------------------------------------
OSN_EXPORT void AlfVisualTemplate::addAttributeSetter(
    IAlfAttributeSetter* aSetter, AlfAttributeContainer* aContainer )
    {
    mAttributeArray.resize(mAttributeArray.count()+1);
    mContainerArray.resize(mContainerArray.count()+1);

    mAttributeArray.insert(mAttributeArray.count(),aSetter);
    mContainerArray.insert(mContainerArray.count(),aContainer);
    }

// -----------------------------------------------------------------------------
// Description : Get the number of visual attribute setters.
// -----------------------------------------------------------------------------
OSN_EXPORT int AlfVisualTemplate::numAttributeSetters() const throw()
    {
    return mAttributeArray.count();
    }

// -----------------------------------------------------------------------------
// Description : Get a visual attribute setter.
// -----------------------------------------------------------------------------
OSN_EXPORT IAlfAttributeSetter& AlfVisualTemplate::attributeSetter(int aIndex) const
    {
    if((aIndex < 0) || (aIndex >= mAttributeArray.count()))
        {
        ALF_THROW(AlfException, EInvalidArrayIndex, "AlfVisualTemplate::attributeSetter() - Index out of bounds.");
        }    
    return *mAttributeArray[aIndex];
    }

// -----------------------------------------------------------------------------
// Description :
// -----------------------------------------------------------------------------
OSN_EXPORT AlfAttributeContainer& AlfVisualTemplate::attributeContainer(int aIndex) const
    {
    if((aIndex < 0) || (aIndex >= mContainerArray.count()))
        {
        ALF_THROW(AlfException, EInvalidArrayIndex, "AlfVisualTemplate::attributeContainer() - Index out of bounds.");
        }        
    return *mContainerArray[aIndex];
    }

// -----------------------------------------------------------------------------
// Description : Update an existing visual tree with new values
// -----------------------------------------------------------------------------
OSN_EXPORT CAlfVisual* AlfVisualTemplate::updateVisualTree(IAlfMap* aData, IAlfMap* aOldData, CAlfVisual& aVisual)
    {

    CAlfVisual* retVisual = &aVisual;
    // Update the attributes.
    for (int i = 0; i < mAttributeArray.count(); ++i)
        {
        try
            {
            //ALF_PERF_START( perfdata, "AlfVisualTemplate-updateVisualTree-SetAttributeValue")
            mAttributeArray[i]->setAttributeValue(aVisual, mContainerArray[i], aData);
            //ALF_PERF_STOP( perfdata, "AlfVisualTemplate-updateVisualTree-setAttributeValue")
            }
        catch (...)
            {
            ALF_THROW(AlfAttributeException,EInvalidAttributeValue,"AlfVisualTemplate")
            }
        }
    // Pass to children
    if (mChildren.size())
        {
        int ind = selectedChildInd( aData );
        if (ind >= 0 && ind < mChildren.size())
            {
            CAlfLayout *layout = (CAlfLayout *)&aVisual;
            int indOld = selectedChildInd( aOldData );
            if ( ind == indOld )
                {
                mChildren[ind]->updateVisualTree( aData, aOldData, *layout );
                }
            else
                {
                CAlfControl& c(aVisual.Owner());

                //new visual tree is created. Replaces the current (now old) layout,
                //which is removed from its parent and destroyed.
                //new layout is returned from the function.
                CAlfLayout* parentLayout = layout->Layout();
                TInt ind = parentLayout->FindVisual(layout);
                parentLayout->Remove(layout);
                layout->RemoveAndDestroyAllD();
                CAlfLayout* newLayout = (CAlfLayout *) createVisualTree(c, aData, parentLayout, ind);
                retVisual = newLayout;
                }
            }
        else
            {
            CAlfLayout *layout = (CAlfLayout *)&aVisual;
            for (int i = 0; i < mChildren.size(); ++i)
                {
                mChildren[i]->updateVisualTree(aData, aOldData, layout->Visual(i));
                }
            }
        }
    return retVisual;
    }
    
// -----------------------------------------------------------------------------
// Description : create a new visual tree and initialize the visuals
// -----------------------------------------------------------------------------
OSN_EXPORT CAlfVisual* AlfVisualTemplate::createVisualTree(CAlfControl& aControl,
        IAlfMap* aData, CAlfLayout* aParentLayout, int aLayoutIndex)
    {
    // create the visual
    CAlfVisual *result = NULL;
    CAlfLayout *layout = NULL;
    if (!mSelectOneChild)
        {
        if (mVisualType < 0)
            {
            //ALF_PERF_START( perfdata, "AlfVisualTemplate-createVisualTree-NewLayout")
            // create layout
            result = layout = AlfVisualFactory::NewLayoutL(
                                  (TAlfLayoutType)(-1 - mVisualType), aParentLayout, aControl, aControl.Env());
            //ALF_PERF_STOP( perfdata, "AlfVisualTemplate-createVisualTree-NewLayout")
            if (!result)
                {
                ALF_THROW(AlfVisualException,ECanNotCreateVisual,"AlfVisualTemplate")
                }
            }
        else
            {
            // create visual
            //ALF_PERF_START( perfdata, "AlfVisualTemplate-createVisualTree-NewVisual")
            result = AlfVisualFactory::NewVisualL(
                         (TAlfVisualType)mVisualType, aParentLayout, aControl, aControl.Env());
            //ALF_PERF_STOP( perfdata, "AlfVisualTemplate-createVisualTree-NewVisual")
            if (!result)
                {
                ALF_THROW(AlfVisualException,ECanNotCreateVisual,"AlfVisualTemplate")
                }

            //Add the brushes to the visual
            result->EnableBrushesL(true);
            for (int i=0; i < mBrushArray.count(); i++)
                {
                result->Brushes()->AppendL(mBrushArray[i], EAlfDoesNotHaveOwnership);
                }
            }
        if (aParentLayout &&
                aLayoutIndex >= 0 && aLayoutIndex <= aParentLayout->Count())
            {
            //when aConstructedWithParentInformation- parameter is ETrue,
            //no message sent to server
            aParentLayout->Append(result, ETrue);

            //reorder, if needed.
            if (aLayoutIndex != aParentLayout->Count() - 1)
                {
                aParentLayout->Reorder(*result, aLayoutIndex);
                }
            }
        aControl.Append(result);

        if (mVisualName.isNull())
            mVisualName = UString("");
        result->SetTagL(TPtrC8((unsigned char*)mVisualName.getUtf8()));
        
        // Set the attributes
        for (int i = 0; i < mAttributeArray.count(); ++i)
            {
            try
                {
                //ALF_PERF_START( perfdata, "AlfVisualTemplate-createVisualTree-setAttributeValue")
                // set dirtines of all attribute in createvisualtree
                for(int j = 0; j < mContainerArray[i]->attributeCount() ;j++ )
                    {
                    mContainerArray[i]->getAttribute(j).setDirty(true);
                    }
                mAttributeArray[i]->setAttributeValue(*result, mContainerArray[i], aData);
                //ALF_PERF_STOP( perfdata, "AlfVisualTemplate-createVisualTree-setAttributeValue")
                }
            catch (...)
                {
                ALF_THROW(AlfAttributeException,EInvalidAttributeValue,"AlfVisualTemplate")
                }
            }        
        }
    
    // Pass to children
    int ind = selectedChildInd( aData );
    if (ind >= 0 && ind < mChildren.size())
        {
        result = mChildren[ind]->createVisualTree(aControl, aData, aParentLayout, aLayoutIndex);
        }
    else if (!mSelectOneChild)
        {
        for (int i = 0; i < mChildren.size(); ++i)
            {
            mChildren[i]->createVisualTree(aControl, aData, layout, i);
            }
        }
    return result;
    }

// -----------------------------------------------------------------------------
// reads and returns selected ind from data, if set. If not set returns -1.
// -----------------------------------------------------------------------------
int AlfVisualTemplate::selectedChildInd( IAlfMap* aData )
    {
    int ind = -1;
    if (aData && mSelectOneChild)
        {
        IAlfVariantType* data = aData->item(mChildIndFieldName);

        //field value contains index to child array.
        //if field contains no data and template has only one
        // child, assume it's the child wanted.
        bool valueIsEmpty = false;
        if (!data)
            {
            valueIsEmpty = true;
            }
        else
            {
            switch ( data->type() )
                {
                case IAlfVariantType::EInt:
                    ind = data->integer();
                    break;
                default:
                    break;
                }
            }
        if ( valueIsEmpty && mChildren.size() == 1 )
            {
            ind = 0;
            }
        }
    return ind;
    }

// -----------------------------------------------------------------------------
// Description : Removes a visual template from the child array without
// destroying the child template object.
// -----------------------------------------------------------------------------
void AlfVisualTemplate::removeChildTemplate(IAlfVisualTemplate& aChild) throw()
    {
    std::vector<IAlfVisualTemplate*>::iterator it = find(mChildren.begin(), mChildren.end(), &aChild);
    if(it != mChildren.end())
        {
        mChildren.erase(it);
        aChild.setParent(0);
        }
    }

// -----------------------------------------------------------------------------
// Description : Returns the parent visual template object
// -----------------------------------------------------------------------------
OSN_EXPORT IAlfVisualTemplate* AlfVisualTemplate::parent() const throw()
    {
    return mParent;
    }
    
// -----------------------------------------------------------------------------
// Description : Set the visual type
// -----------------------------------------------------------------------------
OSN_EXPORT void AlfVisualTemplate::setParent(IAlfVisualTemplate* aParent) throw()
    {
    mParent = aParent;
    }

// -----------------------------------------------------------------------------
// Description : Set the visual type
// -----------------------------------------------------------------------------
OSN_EXPORT void AlfVisualTemplate::setVisualType(TAlfVisualType aType)
    {
    mVisualType = aType;
    }

// -----------------------------------------------------------------------------
// Description : Get the visual type
// -----------------------------------------------------------------------------
OSN_EXPORT int AlfVisualTemplate::visualType() const throw()
    {
    return mVisualType;
    }
// -----------------------------------------------------------------------------
// Description : Set the layout type
// -----------------------------------------------------------------------------
OSN_EXPORT void AlfVisualTemplate::setLayoutType(TAlfLayoutType aType)
    {
    mVisualType = -1 - aType;
    }

// -----------------------------------------------------------------------------
// Description : Get the layout type
// -----------------------------------------------------------------------------
OSN_EXPORT int AlfVisualTemplate::layoutType() const throw()
    {
    return -1 - mVisualType;
    }

// -----------------------------------------------------------------------------
// Description : Adds brush to the brush array.
// -----------------------------------------------------------------------------
OSN_EXPORT void AlfVisualTemplate::addBrush(CAlfBrush& aBrush)
    {
    mBrushArray.resize(mBrushArray.count()+1);
    mBrushArray.insert(mBrushArray.count(),&aBrush);
    }

// -----------------------------------------------------------------------------
// Description : Get the munber of brushes
// -----------------------------------------------------------------------------
OSN_EXPORT int AlfVisualTemplate::numBrushes() const
    {
    return mBrushArray.count();
    }

// -----------------------------------------------------------------------------
// Description : Get a Brush
// -----------------------------------------------------------------------------
OSN_EXPORT CAlfBrush& AlfVisualTemplate::brush(int aIndex) const
    {
    return *mBrushArray[aIndex];
    }


// -----------------------------------------------------------------------------
// Description : puts the class in to selected child mode.
// -----------------------------------------------------------------------------
OSN_EXPORT void AlfVisualTemplate::setSelectChildMode(bool aSelectChild,
        const UString& aChildIndFieldName )
    {
    mSelectOneChild = aSelectChild;
    mChildIndFieldName = aChildIndFieldName;
    }


// ---------------------------------------------------------------------------
// From class IAlfInterfaceBase.
// Getter for interfaces provided by the visual template.
// ---------------------------------------------------------------------------
//
OSN_EXPORT IAlfInterfaceBase* AlfVisualTemplate::makeInterface( const IfId& aType )
    {
    UString param(aType.mImplementationId);
    if (param == IAlfVisualTemplate::type().mImplementationId)
        {
        return static_cast<IAlfVisualTemplate*>(this);
        }
    return NULL;
    }

    } //Alf
