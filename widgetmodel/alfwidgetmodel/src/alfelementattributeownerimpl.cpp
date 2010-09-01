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
* Description:  Implementation of the IAlfAttributeOwner interface 
*                for AlfElement.
*
*/


#include <alf/alfexceptions.h>
#include "alf/alfattribute.h"
#include "alf/alfattributecontainer.h"
#include "alf/ialfvisualtemplate.h"
#include <alf/alfwidgetcontrol.h>
#include "alf/ialfattributesetter.h"
#include "alf/alfcommonvisualattributesetter.h"
#include "alf/alfelement.h"
#include <osn/ustring.h>
#include "alf/attrproperty.h"
#include <alf/alfwidgetcommand.h>


#include "alfelementattributeownerimpl.h"

using namespace duiuimodel;

namespace Alf
    {

// ---------------------------------------------------------------------------
// Constructor.
// ---------------------------------------------------------------------------
//
AlfElementAttributeOwnerImpl::AlfElementAttributeOwnerImpl(
    AlfElement& aElement, CAlfWidgetControl& aControl) :
    mElement(aElement), mControl(aControl)
    {
    mAttributeList.setAutoDelete(true);    
    }

// ---------------------------------------------------------------------------
// Destructor.
// ---------------------------------------------------------------------------
//
AlfElementAttributeOwnerImpl::~AlfElementAttributeOwnerImpl()
    {
    mAttributeList.clear();
    }
    
// ---------------------------------------------------------------------------
// From class IAlfInterfaceBase.
// Getter for interfaces provided by the element.
// ---------------------------------------------------------------------------
//
IAlfInterfaceBase* AlfElementAttributeOwnerImpl::makeInterface(
    const IfId& aType)
    {
    UString param(aType.mImplementationId);
    if (param == IAlfAttributeOwner::type().mImplementationId)
        {
    	return static_cast<IAlfAttributeOwner*>(this);
        }
    return 0;
    }    
        
// ---------------------------------------------------------------------------
// Get the number of available attributes.
// From class IAlfAttributeOwner.
// ---------------------------------------------------------------------------
//
int AlfElementAttributeOwnerImpl::attributeCount() const
    {
    return mAttributeList.count();
    }

// ---------------------------------------------------------------------------
// Gets the attribute with the given name.
// From class IAlfAttributeOwner.
// ---------------------------------------------------------------------------
//
const AlfAttribute* AlfElementAttributeOwnerImpl::getAttribute(
    const UString& aAttribName) const
    {
    AlfAttribute* attribute = 0;
 	for (int i = 0; i < mAttributeList.count(); i++)
 	    {
 		if (!strcmp(aAttribName.getUtf8(), mAttributeList[i]->name()))
 		    {
 			attribute = mAttributeList[i];
 			break;
 		    }
 	    }
    return attribute;
    }

// ---------------------------------------------------------------------------
// Set a value of an attribute.
// From class IAlfAttributeOwner.
// ---------------------------------------------------------------------------
//
bool AlfElementAttributeOwnerImpl::setAttribute(const AlfAttribute& aAttribute)
    {
    // The attribute must have at least one value set.
    if (!aAttribute.getTargetValueCount())
        {
        ALF_THROW(
            AlfAttributeException, EInvalidAttribute, "AlfElementAttributeOwnerImpl")
        }
    
    if (!canHandleAttribute(aAttribute.name()))
        {
        return false;
        }
    
    // Check whether the attribute exists already.
    int i = 0;
    for (i = 0; i < mAttributeList.count() ; i++)
 	    {
 		if (!strcmp(aAttribute.name(), mAttributeList[i]->name()))
 		    {
 		    // Attribute exists already, modify the contents.
 			*mAttributeList[i] = aAttribute;
 			handleAttribute(*mAttributeList[i]);
 			return true;
 		    }
        }

    // Attribute with the given name does not exist,
    // clone the given attribute and insert it in the list.
    mAttributeList.resize(mAttributeList.count() + 1);
    // Insert cannot fail because already resized.
    mAttributeList.insert(mAttributeList.count(),
        const_cast<AlfAttribute&>(aAttribute).clone());
    
    handleAttribute(*mAttributeList[i]);
    return true;
    }

// ---------------------------------------------------------------------------
// Set a value of an attribute.
// From class IAlfAttributeOwner.
// ---------------------------------------------------------------------------
//
bool AlfElementAttributeOwnerImpl::setAttribute(
    const UString& aAttribName, AlfAttributeValueType* aValue)
    {
    if (!aValue)
        {
        ALF_THROW(
            AlfAttributeException, EInvalidAttribute, "AlfElementAttributeOwnerImpl")
        }
    
    // Create a new attribute.
    auto_ptr<AlfAttribute> attribute(
        new (EMM) AlfAttribute(aAttribName.getUtf8(), AlfAttribute::EStatic));
    attribute->addTargetValue(aValue);
 
    return setAttribute(*attribute.get());
    }      
// ---------------------------------------------------------------------------
// Set a value of an attribute.
// From class IAlfAttributeOwner.
// ---------------------------------------------------------------------------
//
bool AlfElementAttributeOwnerImpl::setAttribute( const UString& aTargetId , 
												 AlfAttribute& aAttribute ) 
	{
  	bool ret(false);
  
	// The attribute must have at least one value set.
	if (!aAttribute.getTargetValueCount())
		{
		ALF_THROW(
		    AlfAttributeException, EInvalidAttribute, "AlfElementAttributeOwnerImpl")
		}

	//check targetId
	if (!aTargetId.isEmpty() 
	 && !aTargetId.compare(mElement.name()) )
		{
		ret = setAttribute(aAttribute);
		}

  	return ret;
  	}


// ---------------------------------------------------------------------------
// Set a value of an attribute.
// From class IAlfAttributeOwner.
// ---------------------------------------------------------------------------
//
bool AlfElementAttributeOwnerImpl::setAttribute( const UString& aTargetId, 
												 const UString& aAttribName,
 												 AlfAttributeValueType* aValue )
	{
	bool ret(false);
	
	if (!aValue)
		{
		ALF_THROW(
		    AlfAttributeException, EInvalidAttribute, "AlfElementAttributeOwnerImpl")
		}
	
	if (!aTargetId.isEmpty())
		{
		 // Create a new static attribute.
	    auto_ptr<AlfAttribute> attribute(
	        new (EMM) AlfAttribute(aAttribName.getUtf8(), AlfAttribute::EStatic));
	    attribute->addTargetValue(aValue);
	 
	    ret = setAttribute(*attribute.get());
	
		}
			
	return ret;
		
	} 

// ---------------------------------------------------------------------------
// Gets the attribute with the given name.
// From class IAlfAttributeOwner.
// ---------------------------------------------------------------------------
//
const AlfAttribute* AlfElementAttributeOwnerImpl::getAttribute( const UString& aTargetId ,
															    const UString& aAttribName ) const
	{
	const AlfAttribute* attr(0);

	//check targetId
	if (!aTargetId.isEmpty() 
	     && !aTargetId.compare(mElement.name()))
		{
		attr = getAttribute(aAttribName); 
		}
		
	return attr;
	}
	
// ---------------------------------------------------------------------------
// Sets the attributes from container
// From class IAlfAttributeOwner.
// ---------------------------------------------------------------------------
//
bool AlfElementAttributeOwnerImpl::setAttributeContainer( AlfAttributeContainer& aAttributeContainer )
	{

    const int attributeCount = aAttributeContainer.attributeCount();
	int numAttributesHandled = 0;
	
    for (int i = 0; i < attributeCount ; i++)
        {
        AlfAttribute& attribute = aAttributeContainer.getAttribute(i);
        if (!strcmp(attribute.name(), duiuimodel::tactileattributes::KEventInput))
            {
            
            // Look for feedback type attribute.
            try
              {
              AlfAttribute& feedBackType = 
              aAttributeContainer.getAttributeByName(duiuimodel::tactileattributes::KFeedbackType);
              handleTactileAttribute(attribute,feedBackType);
              numAttributesHandled += 2;
              }
            catch(AlfDataException& exception)
              {
              // attribute not found,so return false
              return false;
              }
            }
         
        else if(setAttribute(attribute))
            {
            numAttributesHandled++;
            }
	    }


	return (numAttributesHandled == attributeCount);
	}

// ---------------------------------------------------------------------------
// Handles the Tactile Attribute.
// ---------------------------------------------------------------------------
//
void AlfElementAttributeOwnerImpl::handleTactileAttribute(
    AlfAttribute& aAttributeEvent,AlfAttribute& aAttributeFeedback)
    {
	    IAlfVisualTemplate* visualTemplate = mElement.getVisualTemplate();
		
	    // Find the root layout 
	    CAlfVisual* main = mElement.findVisual(0);//needs to be changed because in some cases id may be
	    										  // set by the user	
	    
	    if(visualTemplate )
	        {
			AlfAttribute* attrEvent = NULL;
			AlfAttribute* attrFeedback = NULL;

	        int setterCount = visualTemplate->numAttributeSetters();
	        bool found(false);
	        for (int j = 0; j < setterCount ; j++)
	            {
	            AlfAttributeContainer& container = visualTemplate->attributeContainer(j);
	            try
	                {
	                //find the attribute in each container until it is found
	                attrEvent = &(container.getAttributeByName(duiuimodel::tactileattributes::KEventInput));
	                attrFeedback = &(container.getAttributeByName(duiuimodel::tactileattributes::KFeedbackType));
	                }
	            catch(AlfDataException& exception)
	                {
	                // attribute not found,so continue
	                continue;
	                }
	            //attribute found in one of the existing containers of the visual template, update it
	            *attrEvent = aAttributeEvent;
	            *attrFeedback = aAttributeFeedback;
	            found = true;
	            IAlfAttributeSetter& setter  = visualTemplate->attributeSetter(j);
	            if(main)
	            	setter.setAttributeValue(*main,&container,0);
	            break;                                            
	            }
	        // attribute not found in any of the containers, so create one attribute container and one 
	        // attribute setter and add both to the visual template        
	        if (!found)
	            {
	            auto_ptr<AlfAttributeContainer> container( 
	                new (EMM) AlfAttributeContainer());
	            
	            auto_ptr<AlfCommonVisualAttributeSetter> setter(
	                new (EMM) AlfCommonVisualAttributeSetter());
	                
	            AlfAttribute* attribe =aAttributeEvent.clone();
	            AlfAttribute* attribf =aAttributeFeedback.clone();

	            container.get()->addAttribute(attribe);
	            container.get()->addAttribute(attribf);
	            if(main)
	            setter.get()->setAttributeValue(*main, container.get(), 0);
	            
	            visualTemplate->addAttributeSetter(setter.release(), 
	                container.release());
	            }
	        }
    }

// ---------------------------------------------------------------------------
// Sets the attributes from container using targetId
// From class IAlfAttributeOwner.
// ---------------------------------------------------------------------------
//
bool AlfElementAttributeOwnerImpl::setAttributeContainer( const UString& aTargetId , 
                               AlfAttributeContainer& aAttributeContainer )
	{


    const int attributeCount = aAttributeContainer.attributeCount();
	int numAttributesHandled = 0;
	
    for (int i = 0; i < attributeCount ; i++)
        {
        AlfAttribute& attribute = aAttributeContainer.getAttribute(i);
        if (!strcmp(attribute.name(), duiuimodel::tactileattributes::KEventInput))
        {
                // Look for feedback type attribute.
        
        // Look for feedback type attribute.
        try
          {
          AlfAttribute& feedBackType = 
          aAttributeContainer.getAttributeByName(duiuimodel::tactileattributes::KFeedbackType);
          handleTactileAttribute(attribute,feedBackType);
          numAttributesHandled += 2;
          }
        catch(AlfDataException& exception)
          {
          // attribute not found,so return false
          return false;
          }
        }
       else if(setAttribute(aTargetId,attribute))
        {
        	numAttributesHandled++;
        }
	    }

	return (numAttributesHandled == attributeCount);
		
	}
    
// ---------------------------------------------------------------------------
// Handles the attribute, sets proper values.
// ---------------------------------------------------------------------------
//
void AlfElementAttributeOwnerImpl::handleAttribute(AlfAttribute& aAttribute)
    {
    const char* attrName = aAttribute.name();
    
    if (!strcmp(attrName, commonvisualattributes::KOpacity)) 
        {
        handleOpacityAttribute(aAttribute);
        }
    else if(!strcmp(attrName, layoutattributes::KPositionX) || 
            !strcmp(attrName, layoutattributes::KPositionY))
        {
        handlePositionAttribute(aAttribute);
        }
    else if(!strcmp(attrName, layoutattributes::KWidth) || 
            !strcmp(attrName, layoutattributes::KHeight))
        {
        handleSizeAttribute(aAttribute);
        }
    else if(!strcmp(attrName, layoutattributes::KMaxWidth) || 
            !strcmp(attrName, layoutattributes::KMaxHeight))
        {
        handleMaxSizeAttribute(aAttribute);
        }
    else if(!strcmp(attrName, layoutattributes::KMinWidth) || 
            !strcmp(attrName, layoutattributes::KMinHeight))
        {
        handleMinSizeAttribute(aAttribute);
        }        
    }
    
// ---------------------------------------------------------------------------
// Can attribute be handled with this widget.
// ---------------------------------------------------------------------------
//
bool AlfElementAttributeOwnerImpl::canHandleAttribute(
    const char* aAttributeName)
    {
    bool canHandle(false);
    if (!strcmp(aAttributeName, commonvisualattributes::KOpacity) ||
        !strcmp(aAttributeName, layoutattributes::KPositionX) ||
        !strcmp(aAttributeName, layoutattributes::KPositionY) ||
        !strcmp(aAttributeName, layoutattributes::KWidth) ||
        !strcmp(aAttributeName, layoutattributes::KHeight) ||  
        !strcmp(aAttributeName, layoutattributes::KMaxWidth) ||
        !strcmp(aAttributeName, layoutattributes::KMaxHeight) ||  
        !strcmp(aAttributeName, layoutattributes::KMinWidth) ||
        !strcmp(aAttributeName, layoutattributes::KMinHeight))  
        {
        canHandle = true;
        }
    return canHandle;        
    }

// ---------------------------------------------------------------------------
// Handles the Opacity Attribute.
// ---------------------------------------------------------------------------
//
void AlfElementAttributeOwnerImpl::handleOpacityAttribute(
    AlfAttribute& aAttribute)
    {
    const char* attrName = aAttribute.name();
    
    IAlfVisualTemplate* visualTemplate = mElement.getVisualTemplate();
    
    // Find the root layout 
    CAlfVisual* main = mElement.findVisual(0);
    
    if(visualTemplate && main)
        {
        int setterCount = visualTemplate->numAttributeSetters();
        bool found(false);
        for (int j = 0; j < setterCount && main; j++)
            {
            AlfAttributeContainer& container = visualTemplate->attributeContainer(j);
            AlfAttribute* attr = NULL;
            try
                {
                //find the attribute in each container until it is found
                attr = &(container.getAttributeByName(attrName));
                }
            catch(AlfDataException& exception)
                {
                // attribute not found,so continue
                continue;
                }
            //attribute found in one of the existing containers of the visual template, update it
            *attr = aAttribute;
            found = true;
            IAlfAttributeSetter& setter  = visualTemplate->attributeSetter(j);
            setter.setAttributeValue(*main,&container,0);
            break;                                            
            }
        // attribute not found in any of the containers, so create one attribute container and one 
        // attribute setter and add both to the visual template        
        if (!found)
            {
            auto_ptr<AlfAttributeContainer> container( 
                new (EMM) AlfAttributeContainer());
            auto_ptr<AlfCommonVisualAttributeSetter> setter(
                new (EMM) AlfCommonVisualAttributeSetter());
            AlfAttribute* attribute =aAttribute.clone();
            container.get()->addAttribute(attribute);
           
            setter.get()->setAttributeValue(*main, container.get(), 0);
            visualTemplate->addAttributeSetter(setter.release(), 
                container.release());
            }
        }
    }

// ---------------------------------------------------------------------------
// Handles the Position Attribute
// ---------------------------------------------------------------------------
//    
void AlfElementAttributeOwnerImpl::handlePositionAttribute(AlfAttribute& aAttribute)
    {
    const char* attrName = aAttribute.name();
    const char* attrPair = layoutattributes::KPositionX;
    bool isPositionXAttr = false;
    if(!strcmp(attrName, layoutattributes::KPositionX))
        {
        attrPair = layoutattributes::KPositionY;
        isPositionXAttr = true;
        }
    if (handleAttributePairs(aAttribute, attrPair))
        {
        CAlfVisual* main = mElement.findVisual(0);
        if (main)
            {
            bool needsRelayouting = false;
            TAlfRealPoint pos(main->Pos().Target());
            
            //for optimizing relayouting, which is expensive. Check, whether we need to do it.
            if (isPositionXAttr)
                {
                //check against visual x position
                needsRelayouting = (pos.iX != aAttribute.realValue());
                }
            else
                {
                needsRelayouting = (pos.iY != aAttribute.realValue());
                }
                
            if (needsRelayouting)
                {
                main->UpdateChildrenLayout();
                mControl.updateParentLayout();
                }
            }
        }
    }

// ---------------------------------------------------------------------------
// Handles the Size Attribute.
// ---------------------------------------------------------------------------
//
void AlfElementAttributeOwnerImpl::handleSizeAttribute(AlfAttribute& aAttribute)
    {
    const char* attrName = aAttribute.name();
    const char* attrPair = layoutattributes::KWidth;
    bool isWidthAttr = false;
    
    if(!strcmp(attrName, layoutattributes::KWidth))
        {
        attrPair = layoutattributes::KHeight;
        isWidthAttr = true;
        }
    if (handleAttributePairs(aAttribute, attrPair))
        {
        CAlfVisual* main = mElement.findVisual(0);
        if (main)
            {
            bool needsRelayouting = false;
            TAlfRealPoint size(main->Size().Target());
            
            //for optimizing relayouting, which is expensive. Check, whether we need to do it.
            if (isWidthAttr)
                {
                needsRelayouting = (size.iX != aAttribute.realValue());
                }
            else
                {
                needsRelayouting = (size.iY != aAttribute.realValue());
                }
                
            if (needsRelayouting)
                {
                main->UpdateChildrenLayout();
                mControl.updateParentLayout();
                }
            }
        }
    }

// ---------------------------------------------------------------------------
// Handles the Max Size Attribute.
// ---------------------------------------------------------------------------
//
void AlfElementAttributeOwnerImpl::handleMaxSizeAttribute(AlfAttribute& aAttribute)
    {
    const char* attrName = aAttribute.name();
    const char* attrPair = layoutattributes::KMaxWidth;
    if(!strcmp(attrName, layoutattributes::KMaxWidth))
        {
        attrPair = layoutattributes::KMaxHeight;
        }
    handleAttributePairs(aAttribute, attrPair);
    } 

// ---------------------------------------------------------------------------
// Handles the Min Size Attribute.
// ---------------------------------------------------------------------------
//    
void AlfElementAttributeOwnerImpl::handleMinSizeAttribute(AlfAttribute& aAttribute)
    {
    const char* attrName = aAttribute.name();
    const char* attrPair = layoutattributes::KMinWidth;
    if(!strcmp(attrName, layoutattributes::KMinWidth))
        {
        attrPair = layoutattributes::KMinHeight;
        }
    handleAttributePairs(aAttribute, attrPair);
    }
    
// ---------------------------------------------------------------------------
// Handles setting of paired attributes: widget & height, xpos & ypos etc.
// ---------------------------------------------------------------------------
//    
bool AlfElementAttributeOwnerImpl::handleAttributePairs(AlfAttribute& aAttribute, 
    const char* aAttributeNamePair)
    {
    const char* attrName = aAttribute.name();
    bool changed(false);
    IAlfVisualTemplate* visualTemplate = mElement.getVisualTemplate();
       
    // Find the root layout
    CAlfVisual* main = mElement.findVisual(0);
    if(visualTemplate && main)
        {
        int setterCount = visualTemplate->numAttributeSetters();
        bool found(false);
        
        //find the attribute in all the existing containers
        for (int j = 0; j < setterCount; j++)
            {
            AlfAttributeContainer& container = visualTemplate->attributeContainer(j);
            AlfAttribute* attri = NULL;
            try
                {
                attri = &(container.getAttributeByName(attrName));
                }
            catch(AlfDataException& exception)
                {
                // attribute not found in the container
                // continue to find in other containers
                continue;
                }
            // attribute found, update it
            *attri = aAttribute;
            found = true;
            
            //check, that attribute pair exists.
            try
                {
                AlfAttribute& attr = 
                    container.getAttributeByName(aAttributeNamePair);
                }
            catch(AlfDataException& exception)
                {
                //attribute pair not found in the same container, so break
                break;
                }
                
            // attribute pair found, so call setAttributeValue()
            IAlfAttributeSetter& setter = visualTemplate->attributeSetter(j);                                
            setter.setAttributeValue(*main, &container, 0);
            changed = true;
            break;
            }
            
        // the attribute not found in any of the existing containers    
        if(!found)
            {
            bool foundAttrPair(false);
            
            // again try to find the attribute pair
            // this is for the situation where the attribute already exists
            for (int j = 0; j < setterCount; j++)
                {
                AlfAttributeContainer& container = visualTemplate->attributeContainer(j);
                  
                try
                    {
                    AlfAttribute& attr = 
                        container.getAttributeByName(aAttributeNamePair);
                    }
                catch(AlfDataException& exception)
                    {
                    // attribute not found,so continue
                    continue;
                    }
                // attribute pair found, so add the attribute to the same container
                // and call setAttributeValue()   
                AlfAttribute* attribute = aAttribute.clone();
                container.addAttribute(attribute);
                IAlfAttributeSetter& setter = visualTemplate->attributeSetter(j);                                
                setter.setAttributeValue(*main, &container, 0);
                changed = true;
                foundAttrPair = true;
                break;
                }
                
            // counter-part also does not exist, so create a new container and a attribute setter, add 
            // the attribute to the container, and the container and attribute setter to the visual template
            if(!foundAttrPair)
                {
                auto_ptr<AlfAttributeContainer> container(
                    new (EMM) AlfAttributeContainer());
                auto_ptr<AlfCommonVisualAttributeSetter> setter(
                    new (EMM) AlfCommonVisualAttributeSetter());
                AlfAttribute* attribute = aAttribute.clone();
                container.get()->addAttribute(attribute);
                visualTemplate->addAttributeSetter(setter.get(), 
                    container.get());
                container.release();
                setter.release();                
                }
            }
        }
        return changed;
    }     
    
    } // namespace Alf
