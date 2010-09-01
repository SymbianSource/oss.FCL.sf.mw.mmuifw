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


#ifndef __ALFELEMENTATTRIBUTEOWNERIMPL_H__
#define __ALFELEMENTATTRIBUTEOWNERIMPL_H__

#include "alf/ialfattributeowner.h"
#include <osn/alfptrvector.h>

namespace Alf
    {
    
class AlfElement;   

/**
 * Implementation of the IAlfAttributeOwner interface for AlfElement.
 */    
class AlfElementAttributeOwnerImpl : public IAlfAttributeOwner
    {
public:

    /**
     * Constructor.
     *
     * @param aElement Reference to owner element.
     * @param aControl Reference to control
     */
    AlfElementAttributeOwnerImpl(AlfElement& aElement,
        CAlfWidgetControl& aControl);

    /**
     * Destructor.
     */
    virtual ~AlfElementAttributeOwnerImpl();
    
// from base class IAlfInterfaceBase

    /**
     * Getter for interfaces provided by the element classes.
     * Derived classes should always call the base class method
     * from the overridden MakeInterface.
     *
     * @since S60 ?S60_version
     * @param aType A descriptor to identify the type of the queried interface.
     * @return The queried interface, or NULL if the interface is not
     *         supported or available.
     */
    virtual IAlfInterfaceBase* makeInterface(const IfId& aType);       
            
// from base class IAlfAttributeOwner

	/**
     * Get the number of available attributes.
     *
     * @return The number of attributes.
     */
    virtual int attributeCount() const;

    /**
     * Gets the attribute with the given name.
     * The returned attribute instance is owned by the widget and
     * should not be deleted.
     * Also the returned attribute instance should not be modified.
     * Use SetAttribute for that.
     *
     * @param aAttribName The name of the attribute.
     * @return The attribute, or 0 if the attribute was not found.
     */
    virtual const AlfAttribute* getAttribute(const UString& aAttribName) const;

    /**
     * Gets the attribute with the given name from specific target.
     * The returned attribute instance is owned by the widget and
     * should not be deleted.
     * Also the returned attribute instance should not be modified.
     * Use SetAttribute for that.
     *
     * @param aTargetId   The name of the target widget part, e.g. element class
     * @param aAttribName The name of the attribute.
     * @return A pointer to the attribute, or 0 if the attribute was not found.
     */
    virtual const AlfAttribute* getAttribute( const UString& aTargetId ,
                                              const UString& aAttribName ) const; 
    /**
     * Set a value of an attribute.
     *
     * @param aAttribute Reference attribute, the data is copied from it.
     * The ownership is not transferred.
     * @return True if the attribute was supported, false if not.
     */
    virtual bool setAttribute(const AlfAttribute& aAttribute); 

    /**
     * Sets a value of an attribute to a specific target. 
     * The target is identified by using aTargetId parameter.
     *
     * @param aTargetId  Name of the target widget part, e.g element class tag.
     * @param aAttribute Reference attribute, the data is copied from it.
     *                   The ownership is not transferred.
     * @return True if the attribute was supported, false if not.
     */    
    virtual bool setAttribute( const UString& aTargetId , 
                               AlfAttribute& aAttribute ); 
    /**
     * Set a value of an attribute.
     * This SetAttribute variant is for simple non animated attributes 
     * that only have name and value.
     *
     * @param aAttribName Name of the attribute.
     * @param aValue Value of the attribute. Ownership is transferred
     * @return True if the attribute was supported, false if not.
     */    
    virtual bool setAttribute(const UString& aAttribName, 
        AlfAttributeValueType* aValue);         
        
    /**
     * Sets a value of an static attribute.
     * This SetAttribute variant is for simple non animated attributes 
     * that only have name and value.
     *
     * @param aTargetId   The name of the target widget part, e.g. element class
     * @param aAttribName Name of the attribute.
     * @param aValue      Value of the attribute. Ownership is transferred
     * @return True if the attribute was supported, false if not.
     */
    virtual bool setAttribute( const UString& aTargetId, 
                               const UString& aAttribName, 
                               AlfAttributeValueType* aValue );
    
    /**
     * Sets values of each attribute in given container to a specific target. 
     * The target is identified by using aTargetId parameter.
     * 
     * When the target attributeowner instance cannot handle an attribute in the container,
     * an AlfAttributeException-exception is thrown with EInvalidAttribute parameter.
     *
     * @param aTargetId  Name of the target widget part, e.g element class tag.
     * @param aAttributeContainer Reference attributeContainer, All attributes
     *        in the container are handled. The ownership is not transferred.
     * @return True if all attributes were supported, false if not.
     */    
    virtual bool setAttributeContainer( const UString& aTargetId , 
                               AlfAttributeContainer& aAttributeContainer ); 

    /**
     * Sets values of each attribute in given container
     *
     * When an attributeowner-instance (which can handle some of the attributes in the container) 
     * cannot handle an attribute in the container,
     * an AlfAttributeException-exception is thrown with EInvalidAttribute parameter.
     *
     * @param aAttributeContainer Reference attributeContainer, All attributes
     *        in the container are handled. The ownership is not transferred.
     *                            
     * @return True if all attributes were supported, false if not.
     */    
    virtual bool setAttributeContainer( AlfAttributeContainer& aAttributeContainer );
    
     
private:

    /**
     * Handles the attribute, sets proper values.
     *
     * @param aAttribute Reference attribute, the data is copied from it.
     * The ownership is not transferred.
     */
    void handleAttribute(AlfAttribute& aAttribute);      
    
    /**
     * Can attribute be handled with this widget.
     *
     * @param aAttributeName Name of the attribute.
     * @return True if attribute is supported, false elsewhere.
     */
    bool canHandleAttribute(const char* aAttributeName);    
    
    /**
     * Handles the Opacity Attribute.
     *
     * @param aAttribute the attribute to be handled.
     */
    void handleOpacityAttribute(AlfAttribute& aAttribute);
    
    /**
     * Handles the Position Attribute.
     *
     * @param aAttribute the attribute to be handled.
     */
    void handlePositionAttribute(AlfAttribute& aAttribute);

    /**
     * Handles the Tactile Attribute.
     *
     * @param aAttribute the attribute to be handled.
     */
	void handleTactileAttribute(AlfAttribute& aAttributeEvent,AlfAttribute& aAttributeFeedback);

    /**
     * Handles the Size Attribute.
     *
     * @param aAttribute the attribute to be handled.
     */
    void handleSizeAttribute(AlfAttribute& aAttribute);
    
    /**
     * Handles the Max Size Attribute.
     *
     * @param aAttribute the attribute to be handled.
     */
    void handleMaxSizeAttribute(AlfAttribute& aAttribute);
    
    /**
     * Handles the Min Size Attribute.
     *
     * @param aAttribute the attribute to be handled.
     */
    void handleMinSizeAttribute(AlfAttribute& aAttribute);
    
    /**
     * Handles setting of paired attributes: width & height, xpos & ypos etc.
     *
     * @param aAttribute the attribute to be handled.
     * @param aAttributeNamePair name for the pair attribute of aAttribute.
     */
    bool handleAttributePairs(AlfAttribute& aAttribute, 
        const char* aAttributeNamePair);

private:

    /*
     * Reference to owner element. Not own.
     */
    AlfElement& mElement;
    
    /*
     * Reference to control.
     */
    CAlfWidgetControl& mControl;

    /*
     * Array of attributes set through IAlfAttributeOwner interface. Own.
     */
    AlfPtrVector<AlfAttribute> mAttributeList;        
    };
    
    }   // namespace Alf
    
#endif // __ALFELEMENTATTRIBUTEOWNERIMPL_H__
