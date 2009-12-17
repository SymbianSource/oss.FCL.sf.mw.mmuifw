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
*                for AlfWidget.
*
*/


#include <alf/alfexceptions.h>
#include "alf/alfattribute.h"
#include <alf/alfwidgetcontrol.h>
#include <alf/ialfelement.h>
#include <osn/ustring.h>
#include "alf/attrproperty.h"
#include "alf/alfattributecontainer.h" 
#include <alf/alfwidgetcommand.h> 


#include "alfwidgetattributeownerimpl.h"

using namespace duiuimodel;

namespace Alf
    {

// ---------------------------------------------------------------------------
// Constructor.
// ---------------------------------------------------------------------------
//
AlfWidgetAttributeOwnerImpl::AlfWidgetAttributeOwnerImpl(
    CAlfWidgetControl* aControl) :
    mControl(aControl)
    {
    mAttributeList.setAutoDelete(true);    
    }

// ---------------------------------------------------------------------------
// Destructor.
// ---------------------------------------------------------------------------
//
AlfWidgetAttributeOwnerImpl::~AlfWidgetAttributeOwnerImpl()
    {
    mAttributeList.clear();
    }
    
// ---------------------------------------------------------------------------
// From class IAlfInterfaceBase.
// Getter for interfaces provided by the widget.
// ---------------------------------------------------------------------------
//
IAlfInterfaceBase* AlfWidgetAttributeOwnerImpl::makeInterface(
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
int AlfWidgetAttributeOwnerImpl::attributeCount() const
    {
    int count = mAttributeList.count();
    // Add control attribute count.
    IAlfAttributeOwner* control =
        IAlfInterfaceBase::makeInterface<IAlfAttributeOwner>(mControl);
    if (control)
        {
        count += control->attributeCount();
        // Add root element attribute count
        for (int i = 0; i < mControl->numElements(); i++)
            {
             IAlfElement& element = mControl->element(i);
             if (element.parentElement() == 0)
                {
                // Get the attribute interface
                IAlfAttributeOwner* attributeOwner = 
                    IAlfInterfaceBase::makeInterface<IAlfAttributeOwner>(&element);
                    
                if (attributeOwner)
                    {
                    count += attributeOwner->attributeCount();
                    }
                }
            }
        }
    
    return count;
    }

// ---------------------------------------------------------------------------
// Gets the attribute with the given name.
// From class IAlfAttributeOwner.
// ---------------------------------------------------------------------------
//
const AlfAttribute* AlfWidgetAttributeOwnerImpl::getAttribute(
    const UString& aAttribName) const
    {
    const AlfAttribute* attribute = 0;
    
    // Try to find from widget
 	for (int i = 0; i < mAttributeList.count(); i++)
 	    {
 		if (!strcmp(aAttribName.getUtf8(), mAttributeList[i]->name()))
 		    {
 			attribute = mAttributeList[i];
 			break;
 		    }
 	    }
 	    
    if (!attribute)
        {
        // Try to find from control
        IAlfAttributeOwner* control =
            IAlfInterfaceBase::makeInterface<IAlfAttributeOwner>(mControl);
            
        if (control)
            {
            attribute = control->getAttribute(aAttribName);
            }
            
        if (!attribute)
            {
            // Try to find from root elements
            for (int i = 0; i < mControl->numElements(); i++)
                {
                 IAlfElement& element = mControl->element(i);
                 if (element.parentElement() == 0)
                    {
                    // Get the attribute interface
                    IAlfAttributeOwner* attributeOwner = 
                        IAlfInterfaceBase::makeInterface<IAlfAttributeOwner>(&element);
                        
                    if (attributeOwner)
                        {
                        attribute = attributeOwner->getAttribute(aAttribName);
                        if (attribute)
                            {
                            break;
                            }
                        }
                    }
                }
            }

        }
         	    
    return attribute;
    }

// ---------------------------------------------------------------------------
// Set a value of an attribute.
// From class IAlfAttributeOwner.
// ---------------------------------------------------------------------------
//
bool AlfWidgetAttributeOwnerImpl::setAttribute(const AlfAttribute& aAttribute)
    {
    // The attribute must have at least one value set.
    if (!aAttribute.getTargetValueCount())
        {
        ALF_THROW(
            AlfAttributeException, EInvalidAttribute, "AlfWidgetAttributeOwnerImpl")
        }   
       
    if (canHandleAttribute(aAttribute.name()))
        {
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
    
    bool supported(false);
    bool sendevent(true);
    
    // Let the control try to handle attribute.
    IAlfAttributeOwner* control =
        IAlfInterfaceBase::makeInterface<IAlfAttributeOwner>(mControl);
    if (control)
        {
        supported = control->setAttribute(aAttribute);
        }
        
    // Let the root elements try to handle attribute
    if (!supported)
        {
        supported = handleElementAttribute(aAttribute);
        sendevent = false; //event already sent
        }

    //inform env of attribute setting
    if (supported && sendevent)
        {
        UString target;
        auto_ptr<AlfAttribute> attribute(new (EMM) AlfAttribute());
        *attribute.get() = aAttribute;
        TAlfWidgetAttributeCommand command(attribute.get(), target, mControl->widget());
        attribute.release();
        command.ExecuteL(mControl->Env());
        }

    return (supported);
    }

// ---------------------------------------------------------------------------
// Set a value of an attribute.
// From class IAlfAttributeOwner.
// ---------------------------------------------------------------------------
//
bool AlfWidgetAttributeOwnerImpl::setAttribute(
    const UString& aAttribName, AlfAttributeValueType* aValue)
    {
    if (!aValue)
        {
        ALF_THROW(
            AlfAttributeException, EInvalidAttribute, "AlfWidgetAttributeOwnerImpl")
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
bool AlfWidgetAttributeOwnerImpl::setAttribute( const UString& aTargetId , 
												AlfAttribute& aAttribute ) 
	{
	
 	bool supported(false);
 
	 // The attribute must have at least one value set.
	if (!aAttribute.getTargetValueCount())
	    {
	    ALF_THROW(
	        AlfAttributeException, EInvalidAttribute, "AlfWidgetAttributeOwnerImpl")
	    }

    if (!aTargetId.isEmpty()
         && mControl)
        {

        // Try to find from control
        IAlfAttributeOwner* control =
            IAlfInterfaceBase::makeInterface<IAlfAttributeOwner>(mControl);
        if (control)
            {
            supported = control->setAttribute(aTargetId, aAttribute);
            }
        
        if (!supported)
            {
		    // Try finding element class using targetId
 		    IAlfElement* elem = mControl->findElement(aTargetId.getUtf8());
 		  	if (elem)	
		  		{
			    IAlfAttributeOwner* elementOwner =  IAlfInterfaceBase::makeInterface<IAlfAttributeOwner>(elem);
				if (elementOwner)
					{
					supported = elementOwner->setAttribute(aTargetId, aAttribute);	
					}
		  		}
	        }
	    }

	//inform env of attribute change
    if (supported)
        {
        UString target(aTargetId);
        auto_ptr<AlfAttribute> attribute(new (EMM) AlfAttribute());
        *attribute.get() = aAttribute;
        TAlfWidgetAttributeCommand command(attribute.get(), target, mControl->widget());
        attribute.release();
        command.ExecuteL(mControl->Env());
        }
 		        
    return supported;
   
	}

// ---------------------------------------------------------------------------
// Set a value of an attribute.
// From class IAlfAttributeOwner.
// ---------------------------------------------------------------------------
//
bool AlfWidgetAttributeOwnerImpl::setAttribute( const UString& aTargetId, 
											    const UString& aAttribName, 
											    AlfAttributeValueType* aValue )
	{
	bool ret(false);
	
    if (!aValue)
	    {
	    ALF_THROW(
	        AlfAttributeException, EInvalidAttribute, "AlfWidgetAttributeOwnerImpl")
	    }

    if (!aTargetId.isEmpty())
	    {
	    // Create a new static attribute.
	    auto_ptr<AlfAttribute> attribute(
	        new (EMM) AlfAttribute(aAttribName.getUtf8(), AlfAttribute::EStatic));  
	    attribute->addTargetValue(aValue);
	    ret = setAttribute(aTargetId, *attribute.get());
			
		}
	    
	return ret;    
    
	}

// ---------------------------------------------------------------------------
// Gets the attribute with the given name.
// From class IAlfAttributeOwner.
// ---------------------------------------------------------------------------
//
const AlfAttribute* AlfWidgetAttributeOwnerImpl::getAttribute( const UString& aTargetId,
															    const UString& aAttribName ) const
	{
	const AlfAttribute* attr(0);
	
	if (!aTargetId.isEmpty()
	    && mControl)
        {
        
        // Try to find from control
        IAlfAttributeOwner* control =
            IAlfInterfaceBase::makeInterface<IAlfAttributeOwner>(mControl);
        if (control)
            {
            attr = control->getAttribute(aTargetId, aAttribName);
            }
        
        if (!attr)
            {
	        // Try finding element class from control
	        IAlfElement* element = mControl->findElement(aTargetId.getUtf8());
	        
	        if (element)
		        {
		        // Get the attribute interface
		        IAlfAttributeOwner* attributeOwner = 
		            IAlfInterfaceBase::makeInterface<IAlfAttributeOwner>(element);
		            
		        if (attributeOwner)
		            {
		            attr = attributeOwner->getAttribute(aTargetId, aAttribName); 
		            }
		        }
            }
	    }
	      	    
	return attr;

	}
	
// ---------------------------------------------------------------------------
// Sets the attributes from container
// From class IAlfAttributeOwner.
// ---------------------------------------------------------------------------
//
bool AlfWidgetAttributeOwnerImpl::setAttributeContainer( AlfAttributeContainer& aAttributeContainer )
  	{
  	
  	bool ret(true);
	
    for (int i=0;i<aAttributeContainer.attributeCount();i++)
        {
        AlfAttribute& attr = aAttributeContainer.getAttribute(i);
        bool supported = setAttribute(attr);
        if (!supported)
	        {
	        ret = false;	
	        }
	    }

	return ret;
	
 	}

// ---------------------------------------------------------------------------
// Sets the attributes from container using targetId
// From class IAlfAttributeOwner.
// ---------------------------------------------------------------------------
//
	
bool AlfWidgetAttributeOwnerImpl::setAttributeContainer( const UString& aTargetId , 
                               AlfAttributeContainer& aAttributeContainer )
	{
	bool ret(true);
	
	if (!aTargetId.isEmpty() )
		{
        for (int i=0;i<aAttributeContainer.attributeCount();i++)
	        {
	        AlfAttribute& attr = aAttributeContainer.getAttribute(i);
	        bool supported = setAttribute(aTargetId, attr);
	        if (!supported)
		        {
		        ret = false;	
		        }
	        }
	    }

	return ret;

	}
    
// ---------------------------------------------------------------------------
// Handles the attribute, sets proper values.
// ---------------------------------------------------------------------------
//
bool AlfWidgetAttributeOwnerImpl::handleElementAttribute(
    const AlfAttribute& aAttribute)
    {
    bool supported(false);
    
    // Let the root elements try to handle attribute
    const char* attrName = aAttribute.name();   // for debuging
    int elementCount = mControl->numElements();
    for (int i = 0; i < elementCount; i++)
        {
         IAlfElement& element = mControl->element(i);
         if (element.parentElement() == 0)
            {
            // Get the attribute interface
            IAlfAttributeOwner* attributeOwner = 
                IAlfInterfaceBase::makeInterface<IAlfAttributeOwner>(&element);
                
            if (attributeOwner)
                {
                // let all IAlfAttributeOwners set the attribute
                if (attributeOwner->setAttribute(aAttribute))
                    {
                    UString target(element.name());
                    auto_ptr<AlfAttribute> attribute(new (EMM) AlfAttribute());
                    *attribute.get() = aAttribute;
                    TAlfWidgetAttributeCommand command(attribute.get(), target, mControl->widget());
                    attribute.release();
                    command.ExecuteL(mControl->Env());
                    supported = true;
                    }
                }
            }
        }
        
    return supported;               
    }       
    
// ---------------------------------------------------------------------------
// Handles the attribute, sets proper values.
// ---------------------------------------------------------------------------
//
void AlfWidgetAttributeOwnerImpl::handleAttribute(
    const AlfAttribute& /*aAttribute*/)
    {
    // Add common widget specific attribute handling here
    }
    
// ---------------------------------------------------------------------------
// Can attribute be handled with this widget.
// ---------------------------------------------------------------------------
//
bool AlfWidgetAttributeOwnerImpl::canHandleAttribute(
    const char* /*aAttributeName*/)
    {
    bool canHandle(false);
    // Add common wigdet specific attribute checking here like:
/*    if (!strcmp(aAttributeName, commonvisualattributes::KOpacity))  
        {
        canHandle = true;
        }*/
    return canHandle;        
    }
    
    } // namespace Alf
